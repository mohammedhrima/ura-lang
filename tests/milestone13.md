# milestone13

## index

- 001 — function value: fn-typed value: assign top-level fn to a local, call indirectly
- 002 — function value: fn-typed parameter: callback passed to higher-order fn
- 003 — function value: store a function in a fn-typed variable, then call it
- 004 — function value: pass a function as an argument (higher-order)
- 005 — function value: nested non-capturing helper function
- 006 — function value: proto then definition, and definition then proto
- 007 — function value: signature conflict with a prelude proto
- 008 — function value: conflict between two of your own protos
- 009 — function value: calling a null (zero-init) function value traps with file:line
- 010 — function value: infinite recursion overflows the stack
- 011 — function value: ret is the short spelling of return
- 012 — function value: ret and return mix freely in one file
- 013 — function value: a bare ret in a void function
- 014 — function value: ret ref returns a reference
- 015 — variadic: default argument promotion in a user variadic call
- 016 — directive: @if/@elif/@else selects the matching platform branch
- 017 — directive: @if unix matches on macOS and linux
- 018 — directive: a non-matching branch is dropped before parsing
- 019 — directive: a platform-gated top-level proto
- 020 — directive: @if selects a value inside a function
- 021 — directive: without @no-warn, operator drop alone warns
- 022 — directive: @no-warn above operator drop silences that warning
- 023 — directive: @no-warn covers only the declaration it precedes

---

## 001 — function value: fn-typed value: assign top-level fn to a local, call indirectly

```ura
// stdout: 5
// stdout: 6
// expect: 0

fn add(a i32, b i32) i32:
    return a + b

fn sub(a i32, b i32) i32:
    return a - b

fn main() i32:
    op fn(i32, i32) i32 = add
    output(op(2, 3), "\n")

    op = sub
    output(op(10, 4), "\n")
    return 0
```

---

## 002 — function value: fn-typed parameter: callback passed to higher-order fn

```ura
// stdout: 42
// stdout: 21
// expect: 0

fn double_it(n i32) i32:
    return n * 2

fn triple_it(n i32) i32:
    return n * 3

fn apply(cb fn(i32) i32, x i32) i32:
    return cb(x)

fn main() i32:
    output(apply(double_it, 21), "\n")
    output(apply(triple_it, 7), "\n")
    return 0
```

---

## 003 — function value: store a function in a fn-typed variable, then call it

```ura
// stderr: exit: 42
// expect: 1

fn twice(n i32) i32:
    return n + n
fn main() i32:
    f fn(i32) i32 = twice
    return f(21)
```

---

## 004 — function value: pass a function as an argument (higher-order)

```ura
// stderr: exit: 42
// expect: 1

fn twice(n i32) i32:
    return n + n
fn apply(f fn(i32) i32, x i32) i32:
    return f(x)
fn main() i32:
    return apply(twice, 21)
```

---

## 005 — function value: nested non-capturing helper function

```ura
// stderr: exit: 42
// expect: 1

fn main() i32:
    fn dbl(k i32) i32:
        return k + k
    return dbl(21)
```

---

## 006 — function value: proto then definition, and definition then proto

```ura
// stdout: 42 42
// expect: 0

proto fn tally(n i32) i32

fn tally(n i32) i32:
    return n + 1

fn double(n i32) i32:
    return n * 2

proto fn double(n i32) i32

fn main() i32:
    output(tally(41), " ", double(21), "\n")
    return 0
```

---

## 007 — function value: signature conflict with a prelude proto

```ura
// error: Conflicting declaration of 'write': parameter 3 is i32, expected i64

proto fn write(fd i32, ptr pointer, len i32) i32

fn main() i32:
    output("unreachable\n")
    return 0
```

---

## 008 — function value: conflict between two of your own protos

```ura
// error: Conflicting declaration of 'scan': expected 2 parameter(s), found 3

proto fn scan(a i32, b pointer) i32

proto fn scan(a i32, b pointer, c i32) i32

fn main() i32:
    output("unreachable\n")
    return 0
```

---

## 009 — function value: calling a null (zero-init) function value traps with file:line

```ura
// stderr: runtime error: Call to a null function value
// expect: 1

fn main() i32:
    f fn(i32) i32
    return f(21)
```

---

## 010 — function value: infinite recursion overflows the stack

```ura
// stderr: exit: 139
// expect: 1

fn recurse(n i32) i32:
    return recurse(n)

fn main() i32:
    return recurse(1)
```

---

## 011 — function value: ret is the short spelling of return

```ura
// stdout: 100 0
// stdout: True False
// expect: 0

fn clamp(val i32, lo i32, hi i32) i32:
    if val < lo: ret lo
    if val > hi: ret hi
    ret val

fn is_alive(hp i32) bool: ret hp > 0

fn main() i32:
    output(clamp(120, 0, 100), " ", clamp(-5, 0, 100), "\n")
    output(is_alive(3), " ", is_alive(0), "\n")
    return 0
```

---

## 012 — function value: ret and return mix freely in one file

```ura
// stdout: 8 -3
// expect: 0

fn short(n i32) i32:
    ret n * 2

fn long(n i32) i32:
    return n + 1

fn both(n i32) i32:
    if n > 0: ret short(n)
    return long(n)

fn main() i32:
    output(both(4), " ", both(-4), "\n")
    return 0
```

---

## 013 — function value: a bare ret in a void function

```ura
// stdout: already down
// stdout: still standing: 12
// expect: 0

fn announce(hp i32):
    if hp <= 0:
        output("already down\n")
        ret
    output("still standing: ", hp, "\n")

fn main() i32:
    announce(0)
    announce(12)
    return 0
```

---

## 014 — function value: ret ref returns a reference

```ura
// stdout: 25
// expect: 0

struct Hero:
    hp i32

fn strongest(party Hero[]) ref Hero:
    best i32 = 0
    for i in 1..party.len:
        if party[i].hp > party[best].hp: best = i
    ret ref party[best]

fn main() i32:
    party Hero[] = new Hero[3]
    party[0].hp = 10
    party[1].hp = 40
    party[2].hp = 25
    champ &Hero = strongest(party)
    champ.hp = champ.hp - 15
    output(party[1].hp, "\n")
    clean party
    return 0
```

---

## 015 — variadic: default argument promotion in a user variadic call

```ura
// stdout: 3.50 A 7 1 42
// stdout: 123456789
// stdout: 2.50 Z 9
// stdout: 7.00 43
// stdout: 1 0
// stdout: none
// expect: 0

proto fn printf(format pointer, ...) i32

fn main() i32:
    f f32 = 3.5
    c char  = 'A'
    s i16 = 7 as i16
    b bool  = True
    l i64  = 123456789 as i64
    i i32   = 42

    printf("%.2f %c %d %d %d\n", f, c, s, b, i)
    printf("%ld\n", l)
    printf("%.2f %c %d\n", 2.5, 'Z', 9)
    printf("%.2f %d\n", f + f, i + 1)
    printf("%d %d\n", 1 < 2, 3 == 4)
    printf("none\n")
    return 0
```

---

## 016 — directive: @if/@elif/@else selects the matching platform branch

```ura
// stdout: mac
// expect: 0
fn main() i32:
    @if macos:
        output("mac\n")
    @elif linux:
        output("linux\n")
    @else:
        output("other\n")
    return 0
```

---

## 017 — directive: @if unix matches on macOS and linux

```ura
// stdout: posix
// expect: 0
fn main() i32:
    @if unix:
        output("posix\n")
    @else:
        output("windows\n")
    return 0
```

---

## 018 — directive: a non-matching branch is dropped before parsing

```ura
// stdout: kept
// expect: 0
fn main() i32:
    @if linux:
        undefined_that_would_error()
    @else:
        output("kept\n")
    return 0
```

---

## 019 — directive: a platform-gated top-level proto

```ura
// stdout: True
// expect: 0
@if unix:
    proto fn getpid() i32
@else:
    proto fn GetCurrentProcessId() u32

fn main() i32:
    output(getpid() > 0, "\n")
    return 0
```

---

## 020 — directive: @if selects a value inside a function

```ura
// stdout: 1
// expect: 0
fn tag() i32:
    @if macos:
        return 1
    @elif linux:
        return 2
    @else:
        return 0

fn main() i32:
    output(tag(), "\n")
    return 0
```

---

## 021 — directive: without @no-warn, operator drop alone warns

```ura
// stdout: dropping 1
// stderr: warning: Struct Owned has 'operator drop' but no 'operator ='; copying it makes two owners of the same memory, and both will be destroyed
// expect: 1

struct Owned:
    tag i32

    operator drop:
        output("dropping ", self.tag, "\n")

fn main() i32:
    o Owned
    o.tag = 1
    return 0
```

---

## 022 — directive: @no-warn above operator drop silences that warning

```ura
// stdout: dropping 1
// expect: 0

struct Owned:
    tag i32

    @no-warn
    operator drop:
        output("dropping ", self.tag, "\n")

fn main() i32:
    o Owned
    o.tag = 1
    return 0
```

---

## 023 — directive: @no-warn covers only the declaration it precedes

```ura
// stdout: loud 2
// stdout: quiet 1
// stderr: warning: Struct Loud has 'operator drop' but no 'operator ='; copying it makes two owners of the same memory, and both will be destroyed
// expect: 1

struct Quiet:
    tag i32

    @no-warn
    operator drop:
        output("quiet ", self.tag, "\n")

struct Loud:
    tag i32

    operator drop:
        output("loud ", self.tag, "\n")

fn main() i32:
    q Quiet
    l Loud
    q.tag = 1
    l.tag = 2
    return 0
```

