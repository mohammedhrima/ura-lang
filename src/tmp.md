# next 5 features

Do them in order: each one uses the ones before it.
Check the exit code with `./exe.out; echo $?` against `expect`, and what it
prints against `stdout`.

---

## 1 — arrays, and declarations without a value

needs: `x i32` with no value is zero, `[a, b, c]` literals, `a[i]` read and
write, `i8[]` from a string literal is a writable copy (the literal itself is
read-only memory), arrays passed to functions.

```ura
// expect: 0
fn main() i32:
    x i32
    return x
```

```ura
// expect: 10
fn main() i32:
    a i32[] = [4, 5, 6]
    return a[0] + a[2]
```

```ura
// expect: 9
fn main() i32:
    a i32[] = [1, 2, 3]
    a[1] = 9
    return a[1]
```

```ura
// expect: 9   (index from a variable)
fn main() i32:
    a i32[] = [7, 8, 9]
    i i32 = 2
    return a[i]
```

```ura
// expect: 16   (10 + 2 + 4)
fn main() i32:
    x i32 = 10
    y i32 = 4
    a i32[] = [x, 2, y]
    return a[0] + a[1] + a[2]
```

```ura
// expect: 1   (a string in an i8[] is writable)
fn main() i32:
    s i8[] = "abc"
    s[0] = 'x'
    c i8 = s[1]
    if c == 'b' and s[0] == 'x':
        return 1
    return 0
```

```ura
// expect: 6
fn sum(a i32[]) i32:
    total i32 = 0
    i i32 = 0
    while i < 3:
        total += a[i]
        i += 1
    return total

fn main() i32:
    values i32[] = [1, 2, 3]
    return sum(values)
```

must fail:

```ura
// error: x is not an array
fn main() i32:
    x i32 = 1
    return x[0]
```

```ura
// error: an index must be an integer
fn main() i32:
    a i32[] = [1, 2]
    return a[True]
```

```ura
// error: mixed element types in an array literal
fn main() i32:
    a i32[] = [1, True]
    return 0
```

---

## 2 — structs

needs: `struct Name:` with typed attributes, `u User` zero-initialised,
`u.a` read and write, nested attributes (`r.b.x`), structs through refs.

```ura
// expect: 0   (zero-initialised)
struct User:
    a i32
    b i32

fn main() i32:
    u User
    return u.a + u.b
```

```ura
// expect: 7
struct User:
    a i32
    b i32

fn main() i32:
    u User
    u.b = 7
    return u.b
```

```ura
// expect: 3   (write through a ref to a struct)
struct User:
    a i32

fn set(p &User):
    p.a = 3

fn main() i32:
    u User
    set(&u)
    return u.a
```

```ura
// expect: 52   (r.a.x stays 0)
struct Point:
    x i32
    y i32

struct Rect:
    a Point
    b Point

fn main() i32:
    r Rect
    r.b.x = 5
    r.a.y = 2
    return r.b.x * 10 + r.a.y + r.a.x
```

```ura
// expect: 12   (9 + 3)
struct Row:
    cells i32[]

fn main() i32:
    r Row
    r.cells = [1, 2, 3]
    r.cells[0] = 9
    return r.cells[0] + r.cells[2]
```

must fail:

```ura
// error: User has no attribute c
struct User:
    a i32

fn main() i32:
    u User
    return u.c
```

```ura
// error: x is not a struct
fn main() i32:
    x i32 = 1
    return x.a
```

```ura
// error: a struct cannot contain itself (use &Node)
struct Node:
    next Node

fn main() i32:
    return 0
```

---

## 3 — methods

needs: `fn` inside a struct, `self` is a ref to the receiver (changes stay
visible to the caller), `u.m(...)` calls, a method may call a sibling declared
below it, calls through a ref receiver.

```ura
// expect: 8
struct User:
    a i32

    fn add(n i32) i32:
        return self.a + n

fn main() i32:
    u User
    u.a = 5
    return u.add(3)
```

```ura
// expect: 3   (self changes the caller's struct)
struct Counter:
    n i32

    fn inc():
        self.n += 1

fn main() i32:
    c Counter
    c.inc()
    c.inc()
    c.inc()
    return c.n
```

```ura
// expect: 42   (twice calls get, declared below it)
struct Box:
    v i32

    fn twice() i32:
        return self.get() * 2

    fn get() i32:
        return self.v

fn main() i32:
    b Box
    b.v = 21
    return b.twice()
```

```ura
// expect: 4   (method through a ref receiver)
struct User:
    a i32

    fn get() i32:
        return self.a

fn via(p &User) i32:
    return p.get()

fn main() i32:
    u User
    u.a = 4
    return via(&u)
```

must fail:

```ura
// error: User has no method nope
struct User:
    a i32

fn main() i32:
    u User
    return u.nope()
```

```ura
// error: self outside a method
fn main() i32:
    return self.a
```

```ura
// error: a is an attribute: write self.a
struct User:
    a i32

    fn set():
        a = 1

fn main() i32:
    return 0
```

---

## 4 — overloading

needs: several functions (or methods) with one name, chosen by the number
and types of the arguments; each gets its own LLVM name.

```ura
// stdout: int 42
//         str hi
proto printf(fmt i8[], ...) i32

fn show(n i32):
    printf("int %d\n", n)

fn show(s i8[]):
    printf("str %s\n", s)

fn main() i32:
    show(42)
    show("hi")
    return 0
```

```ura
// expect: 36   (add(1, 2) * 10 + add(1, 2, 3) = 30 + 6)
fn add(a i32, b i32) i32:
    return a + b

fn add(a i32, b i32, c i32) i32:
    return a + b + c

fn main() i32:
    return add(1, 2) * 10 + add(1, 2, 3)
```

```ura
// expect: 17   (set() gives 1, set(7) gives 7: 1 * 10 + 7)
struct User:
    a i32

    fn set(n i32):
        self.a = n

    fn set():
        self.a = 1

fn main() i32:
    u User
    u.set()
    x i32 = u.a
    u.set(7)
    return x * 10 + u.a
```

must fail:

```ura
// error: f differs only by return type
fn f() i32:
    return 1

fn f() b1:
    return True

fn main() i32:
    return 0
```

```ura
// error: no add takes (b1)
fn add(a i32, b i32) i32:
    return a + b

fn main() i32:
    return add(True)
```

---

## 5 — builtins: output, typeof, sizeof

needs: `output(...)` prints each argument by its type (no format string),
`typeof(x)` gives the type name as a string, `sizeof(x)` folds to a constant
with ABI padding. `output` uses printf under the hood: it must not clash with
a user's own `proto printf`.

```ura
// stdout: 42 z hi True
fn main() i32:
    n i32 = 42
    c i8 = 'z'
    s i8[] = "hi"
    flag b1 = n > 1
    output(n, " ", c, " ", s, " ", flag, "\n")
    return 0
```

```ura
// stdout: n=3	tab   (a real tab)
//         no newline
fn main() i32:
    n i32 = 3
    output("n=", n, "\ttab\n")
    output("no newline")
    output("\n")
    return 0
```

```ura
// stdout: i32 i8[] User
struct User:
    count i32

fn main() i32:
    x i32 = 1
    s i8[] = "abc"
    u User
    output(typeof(x), " ", typeof(s), " ", typeof(u), "\n")
    return 0
```

```ura
// expect: 20   (i32 = 4, i8[] = 8 (a pointer), Pad = 8 (i8, 3 bytes padding, i32))
struct Pad:
    flag i8
    count i32

fn main() i32:
    x i32
    s i8[]
    p Pad
    return sizeof(x) + sizeof(s) + sizeof(p)
```

must fail:

```ura
// error: nope is not declared
fn main() i32:
    return sizeof(nope)
```

---

# left from last round

## 3 — references (&i32)

needs: `&type` as a type, `&a` takes the address of a variable, a ref reads
and writes through to what it points to, `p1 = &p2` rebinds instead.



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
