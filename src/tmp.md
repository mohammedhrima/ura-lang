# next 5 features

Do them in order: each one uses the ones before it.
Check the exit code with `./exe.out; echo $?` against `expect`, and what it
prints against `stdout`.

---

## 1 — while / break / continue

needs: `while cond:` with a condition block, a body block and an end block;
`break` jumps to the end of the innermost loop, `continue` to its condition.

```ura
// expect: 10
fn main() i32:
    a i32 = 0
    while a < 10:
        a += 1
    return a
```

```ura
// expect: 3
fn main() i32:
    a i32 = 0
    while a < 10:
        if a == 3:
            break
        a += 1
    return a
```

```ura
// expect: 25   (sum of the odd numbers below 10: 1 + 3 + 5 + 7 + 9)
fn main() i32:
    i i32 = 0
    sum i32 = 0
    while i < 10:
        i += 1
        if i % 2 == 0:
            continue
        sum += i
    return sum
```

```ura
// expect: 12   (3 x 4, j declared again on every outer iteration)
fn main() i32:
    i i32 = 0
    c i32 = 0
    while i < 3:
        j i32 = 0
        while j < 4:
            c += 1
            j += 1
        i += 1
    return c
```

```ura
// expect: 3   (break leaves only the inner loop)
fn main() i32:
    i i32 = 0
    c i32 = 0
    while i < 3:
        while c < 100:
            c += 1
            break
        i += 1
    return c
```

```ura
// expect: 7   (return from inside a loop)
fn main() i32:
    i i32 = 0
    while i < 100:
        if i == 7:
            return i
        i += 1
    return 0
```

must fail:

```ura
// error: break outside a loop
fn main() i32:
    break
    return 0
```

```ura
// error: continue outside a loop
fn main() i32:
    if 1 < 2:
        continue
    return 0
```

---

## 2 — b1 type

needs: `b1` variables, `True` / `False` literals, comparisons give a `b1`,
`b1` parameters and return values, and a first type check on assignment.

```ura
// expect: 1
fn main() i32:
    flag b1 = True
    if flag:
        return 1
    return 0
```

```ura
// expect: 2
fn main() i32:
    a i32 = 5
    big b1 = a > 3
    n i32 = 0
    if big:
        n += 1
    big = a > 9
    if big:
        n += 10
    big = False
    if big:
        n += 100
    if big == False:
        n += 1
    return n
```

```ura
// expect: 3
fn is_even(n i32) b1:
    return n % 2 == 0

fn both(a b1, b b1) b1:
    return a and b

fn main() i32:
    n i32 = 0
    if is_even(4):
        n += 1
    if both(is_even(2), is_even(6)):
        n += 2
    if both(is_even(2), is_even(7)):
        n += 10
    return n
```

must fail:

```ura
// error: flag is b1, 5 is i32
fn main() i32:
    flag b1 = 5
    return 0
```

---

## 3 — references (&i32)

needs: `&type` as a type, `&a` takes the address of a variable, a ref reads
and writes through to what it points to, `p1 = &p2` rebinds instead.

```ura
// expect: 12   (write through)
fn main() i32:
    a i32 = 10
    p &i32 = &a
    p = 12
    return a
```

```ura
// expect: 15   (read through, then write through)
fn main() i32:
    a i32 = 10
    p &i32 = &a
    p = p + 5
    return a
```

```ura
// expect: 21   (a = 2, b = 1)
fn swap(x &i32, y &i32):
    t i32 = x
    x = y
    y = t

fn main() i32:
    a i32 = 1
    b i32 = 2
    swap(&a, &b)
    return a * 10 + b
```

```ura
// expect: 2   (p1 = p2 copies the value: a becomes 2)
fn main() i32:
    a i32 = 1
    b i32 = 2
    p1 &i32 = &a
    p2 &i32 = &b
    p1 = p2
    return a
```

```ura
// expect: 9   (p1 = &p2 rebinds p1 to b, then writes 9 into b)
fn main() i32:
    a i32 = 1
    b i32 = 2
    p1 &i32 = &a
    p2 &i32 = &b
    p1 = &p2
    p1 = 9
    return b
```

must fail:

```ura
// error: a ref must be bound to an address (&a), not a value
fn main() i32:
    p &i32 = 5
    return 0
```

```ura
// error: x is a ref parameter, 3 is a value
fn foo(x &i32):
    x = 1

fn main() i32:
    foo(3)
    return 0
```

---

## 4 — proto, string literals, variadic calls

needs: `proto` declares a C function (no body), string literals become
constant `i8[]` globals, `...` accepts any extra arguments, and a proto is
declared once in LLVM even when several `use` copies bring it in.

```ura
// stdout: hello 42
// expect: 0
proto printf(fmt i8[], ...) i32

fn main() i32:
    printf("hello %d\n", 42)
    return 0
```

```ura
// expect: 10
proto exit(code i32)

fn main() i32:
    exit(10)
    return 0
```

```ura
// stdout: <abcdef>
proto printf(fmt i8[], ...) i32

fn main() i32:
    str i8[] = "abcdef"
    printf("<%s>\n", str)
    return 0
```

```ura
// stdout: a	b   (a real tab between a and b)
proto printf(fmt i8[], ...) i32

fn main() i32:
    printf("a\tb\n")
    return 0
```

```ura
// stdout: abc
// expect: 3   (printf returns how many chars it wrote)
proto printf(fmt i8[], ...) i32

fn main() i32:
    return printf("abc")
```

```ura
// ---- io.ura
proto printf(fmt i8[], ...) i32

// ---- a.ura
use "io"

fn hi():
    printf("hi\n")

// ---- main.ura
// stdout: hi
//         main
// the IR has one `declare i32 @printf(i8*, ...)`, never @printf.1
use "io"
use "a"

fn main() i32:
    hi()
    printf("main\n")
    return 0
```

must fail:

```ura
// error: printf needs at least 1 argument (fmt)
proto printf(fmt i8[], ...) i32

fn main() i32:
    printf()
    return 0
```

```ura
// error: ... must be the last parameter
proto bad(..., x i32)

fn main() i32:
    return 0
```

---

## 5 — char / i8

needs: `char` and `i8` variables, character literals with escapes, char
comparisons, and C argument promotion: an `i8` passed through `...` is sent
as an `i32` (that's what printf's `%c` reads).

```ura
// stdout: a b
proto printf(fmt i8[], ...) i32

fn main() i32:
    c1 char = 'a'
    c2 i8 = 'b'
    printf("%c %c\n", c1, c2)
    return 0
```

```ura
// expect: 2
fn main() i32:
    c char = 'x'
    n i32 = 0
    if c == 'x':
        n += 1
    if c != 'y':
        n += 1
    if c == 'z':
        n += 10
    return n
```

```ura
// stdout: [
//         ]
proto printf(fmt i8[], ...) i32

fn main() i32:
    nl char = '\n'
    printf("[%c]\n", nl)
    return 0
```

must fail:

```ura
// error: a character literal holds one character
fn main() i32:
    c char = 'ab'
    return 0
```

```ura
// error: empty character literal
fn main() i32:
    c char = ''
    return 0
```
