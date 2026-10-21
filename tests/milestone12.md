# milestone12

## index

- 001 — nullable: comparing a never-null value to null is rejected
- 002 — nullable: null needs a target type
- 003 — try: a throw is caught in the same function
- 004 — try: a throw propagates across calls to a caller's catch
- 005 — try: a rethrow inside a catch propagates outward
- 006 — try: a nested try: the inner catch handles it
- 007 — try: return inside a try returns from the function
- 008 — try: break inside a try exits the enclosing loop
- 009 — try: an uncaught throw prints its message and exits
- 010 — try: throwing a non-Error is a compile error
- 011 — try: catch without a matching try is a parse error
- 012 — try: code after a throw is unreachable and dropped

---

## 001 — nullable: comparing a never-null value to null is rejected

```ura
// error: 's1' is never null, so this comparison is always the same

fn main() i32:
    s1 char[] = "hello"
    if s1 != null:
        output("never\n")
    return 0
```

---

## 002 — nullable: null needs a target type

```ura
// error: 'x' can never be null; declare it optional to assign null to it

fn main() i32:
    x char[] = null
    return 0
```

---

## 003 — try: a throw is caught in the same function

```ura
// stdout: caught: boom
// expect: 0
use "@/error"

fn main() i32:
    try:
        throw Error::make("boom")
    catch e:
        output("caught: ", e.message, "\n")
    return 0
```

---

## 004 — try: a throw propagates across calls to a caller's catch

```ura
// stdout: caught: from deep
// stdout: after
// expect: 0
use "@/error"

fn deep():
    throw Error::make("from deep")

fn middle():
    deep()

fn main() i32:
    try:
        middle()
    catch e:
        output("caught: ", e.message, "\n")
    output("after\n")
    return 0
```

---

## 005 — try: a rethrow inside a catch propagates outward

```ura
// stdout: mid: first
// stdout: main: second
// expect: 0
use "@/error"

fn middle():
    try:
        throw Error::make("first")
    catch e:
        output("mid: ", e.message, "\n")
        throw Error::make("second")

fn main() i32:
    try:
        middle()
    catch e:
        output("main: ", e.message, "\n")
    return 0
```

---

## 006 — try: a nested try: the inner catch handles it

```ura
// stdout: inner: inner
// expect: 0
use "@/error"

fn main() i32:
    try:
        try:
            throw Error::make("inner")
        catch e:
            output("inner: ", e.message, "\n")
    catch e:
        output("outer: ", e.message, "\n")
    return 0
```

---

## 007 — try: return inside a try returns from the function

```ura
// stdout: ret = 5
// expect: 0
use "@/error"

fn pick() i32:
    try:
        return 5
    catch e:
        return 0

fn main() i32:
    output("ret = ", pick(), "\n")
    return 0
```

---

## 008 — try: break inside a try exits the enclosing loop

```ura
// stdout: i=0
// stdout: done
// expect: 0
use "@/error"

fn main() i32:
    for i in 0..3:
        try:
            if i == 1:
                break
            output("i=", i, "\n")
        catch e:
            output("x\n")
    output("done\n")
    return 0
```

---

## 009 — try: an uncaught throw prints its message and exits

```ura
// stderr: uncaught
// expect: 1
use "@/error"

fn main() i32:
    throw Error::make("uncaught\n")
    output("unreachable\n")
    return 0
```

---

## 010 — try: throwing a non-Error is a compile error

```ura
// error: 'throw' expects an Error, got i32
use "@/error"

fn main() i32:
    throw 5
    return 0
```

---

## 011 — try: catch without a matching try is a parse error

```ura
// error: 'catch' without a matching 'try'
fn main() i32:
    catch e:
        output("x\n")
    return 0
```

---

## 012 — try: code after a throw is unreachable and dropped

```ura
// stdout: caught: x
// expect: 0
use "@/error"

fn boom():
    throw Error::make("x")
    output("dead\n")

fn main() i32:
    try:
        boom()
    catch e:
        output("caught: ", e.message, "\n")
    return 0
```

