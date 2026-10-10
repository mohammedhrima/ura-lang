# next 5 features

Do them in order: each one uses the ones before it.
Every program returns its result from `main`, so check it with the exit code:
`./exe.out; echo $?` must print the `expect` value.

---

## 1 — integer expressions

needs: codegen for `+ - * / %`, `( expr )` and unary `-` in `parse_prime`.

```ura
// expect: 14
fn main() i32:
    return 2 + 3 * 4
```

```ura
// expect: 18
fn main() i32:
    return (2 + 3) * 4 - 20 / 4 % 3
```

```ura
// expect: 5   (left to right: (10 - 3) - 2)
fn main() i32:
    return 10 - 3 - 2
```

```ura
// expect: 7
fn main() i32:
    return -5 + 12
```

---

## 2 — local variables

needs: declaration `name type = value`, read, assign, compound assign,
lookup through the scope stack.

```ura
// expect: 21
fn main() i32:
    a i32 = 10
    b i32 = a * 2
    a = a + b
    a += 5
    a -= 3
    a *= 2
    a /= 4
    a %= 5
    return a + b
```

must fail:

```ura
// error: x is not declared
fn main() i32:
    return x
```

```ura
// error: a is declared twice
fn main() i32:
    a i32 = 1
    a i32 = 2
    return a
```

```ura
// error: b is assigned without a declaration
fn main() i32:
    b = 2
    return 0
```

---

## 3 — functions with parameters and calls

needs: parameters, calls with arguments, declare every function before
generating any body (so a call can come before the definition),
parameters behave like local variables.

```ura
// expect: 25
fn square(x i32) i32:
    return x * x

fn add(a i32, b i32) i32:
    return a + b

fn main() i32:
    return add(square(3), square(4))
```

```ura
// expect: 18   (called before it is defined)
fn main() i32:
    return later(2)

fn later(n i32) i32:
    return n * 9
```

```ura
// expect: 56   (changing a parameter does not change the caller)
fn bump(x i32) i32:
    x += 1
    return x

fn main() i32:
    a i32 = 5
    b i32 = bump(a)
    return a * 10 + b
```

must fail:

```ura
// error: nope is not declared
fn main() i32:
    return nope()
```

```ura
// error: add takes 2 arguments, got 1
fn add(a i32, b i32) i32:
    return a + b

fn main() i32:
    return add(1)
```

```ura
// error: f is defined twice
fn f() i32:
    return 1

fn f() i32:
    return 2

fn main() i32:
    return f()
```

```ura
// error: parameter a is declared twice
fn f(a i32, a i32) i32:
    return a

fn main() i32:
    return f(1, 2)
```

---

## 4 — use (imports)

needs: the design we agreed on: file list, find-or-create by realpath,
`use` node pointing at the file, transitive lookup, cycle-safe.
Paths are relative to the file that writes the `use`, `.ura` is added.

```ura
// ---- helper.ura
fn help() i32:
    return 7

// ---- main.ura
// expect: 8
use "helper"

fn main() i32:
    return help() + 1
```

```ura
// ---- sub/deep.ura
fn deep() i32:
    return 3

// ---- mid.ura
use "sub/deep"

fn total() i32:
    return deep() + 1

// ---- main.ura
// expect: 7   (deep is reachable through mid)
use "mid"

fn main() i32:
    return total() + deep()
```

```ura
// ---- base.ura
fn base() i32:
    return 5

// ---- d1.ura
use "base"

fn one() i32:
    return base() + 1

// ---- d2.ura
use "base"

fn two() i32:
    return base() + 2

// ---- main.ura
// expect: 13   (base.ura is loaded once, no "defined twice" error)
use "d1"
use "d2"

fn main() i32:
    return one() + two()
```

```ura
// ---- c1.ura
use "c2"

fn ping() i32:
    return 1

// ---- c2.ura
use "c1"

fn pong() i32:
    return 2

// ---- main.ura
// expect: 3   (a cycle compiles)
use "c1"

fn main() i32:
    return ping() + pong()
```

```ura
// ---- main.ura
// expect: 7   (use inside a block)
fn main() i32:
    use "helper"
    return help()
```

```ura
// ---- a.ura
// expect: 4   (run: ura a.ura b.ura, no use needed)
fn main() i32:
    return from_b()

// ---- b.ura
fn from_b() i32:
    return 4
```

must fail:

```ura
// error: Cannot find file 'nope.ura'
use "nope"

fn main() i32:
    return 0
```

```ura
// ---- main.ura
// error: help is not declared (the use is only inside main)
fn main() i32:
    use "helper"
    return help()

fn other() i32:
    return help()
```

```ura
// ---- x.ura
fn same() i32:
    return 1

// ---- main.ura
// error: same is defined twice (main.ura and x.ura)
use "x"

fn same() i32:
    return 2

fn main() i32:
    return same()
```

---

## 5 — if / elif / else, comparisons, and / or

needs: `== != < <= > >=`, `and` / `or`, basic blocks for each branch,
a block is a scope (its variables die at the end of the block).

```ura
// expect: 123
fn grade(n i32) i32:
    if n < 10:
        return 1
    elif n < 20:
        return 2
    else:
        return 3

fn main() i32:
    return grade(5) * 100 + grade(15) * 10 + grade(25)
```

```ura
// expect: 6
fn main() i32:
    a i32 = 5
    n i32 = 0
    if a == 5:
        n += 1
    if a != 4:
        n += 1
    if a < 6:
        n += 1
    if a <= 5:
        n += 1
    if a > 4:
        n += 1
    if a >= 5:
        n += 1
    if a > 5:
        n += 100
    return n
```

```ura
// expect: 11
fn main() i32:
    a i32 = 5
    n i32 = 0
    if a > 1 and a < 10:
        n += 1
    if a < 1 or a == 5:
        n += 10
    if a < 1 and a == 5:
        n += 100
    return n
```

```ura
// expect: 3   (body on the same line)
fn main() i32:
    a i32 = 5
    n i32 = 0
    if a > 3: n += 1
    if a > 9: n += 10
    if a == 5: n += 2
    return n
```

```ura
// expect: 120   (recursion)
fn fact(n i32) i32:
    if n <= 1:
        return 1
    return n * fact(n - 1)

fn main() i32:
    return fact(5)
```

```ura
// expect: 3
fn main() i32:
    a i32 = 1
    if a == 1:
        b i32 = 2
        a = a + b
    return a
```

must fail:

```ura
// error: b is not declared (it died with the if block)
fn main() i32:
    a i32 = 1
    if a == 1:
        b i32 = 2
    return b
```
