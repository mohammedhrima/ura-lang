# functions

## index

- 001 — parameters and calls
- 002 — call before the definition
- 003 — nested calls
- 004 — a parameter is a copy
- 005 — call result in a variable
- 006 — void function and bare return
- 007 — function inside a function
- 008 — calling function before declaration (from tests-old/functions.md #001)
- 009 — math op on functions (from tests-old/functions.md #002)
- 010 — bare return in a void function (from tests-old/functions.md #010)

---

## 001 — parameters and calls

```ura
// expect: 25
fn square(x i32) i32:
    return x * x

fn add(a i32, b i32) i32:
    return a + b

fn main() i32:
    return add(square(3), square(4))
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @square(i32 %0) {
entry:
  %x = alloca i32, align 4
  store i32 %0, i32* %x, align 4
  %load.x = load i32, i32* %x, align 4
  %load.x1 = load i32, i32* %x, align 4
  %MUL = mul i32 %load.x, %load.x1
  ret i32 %MUL
}

define i32 @add(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  store i32 %1, i32* %b, align 4
  %load.a = load i32, i32* %a, align 4
  %load.b = load i32, i32* %b, align 4
  %ADD = add i32 %load.a, %load.b
  ret i32 %ADD
}

define i32 @main() {
entry:
  %"square(3)" = call i32 @square(i32 3)
  %"square(4)" = call i32 @square(i32 4)
  %"add(square(3),square(4))" = call i32 @add(i32 %"square(3)", i32 %"square(4)")
  ret i32 %"add(square(3),square(4))"
}
```

---

## 002 — call before the definition

```ura
// expect: 18
fn main() i32:
    return later(2)

fn later(n i32) i32:
    return n * 9
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %"later(2)" = call i32 @later(i32 2)
  ret i32 %"later(2)"
}

define i32 @later(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %load.n = load i32, i32* %n, align 4
  %MUL = mul i32 %load.n, 9
  ret i32 %MUL
}
```

---

## 003 — nested calls

```ura
// expect: 3
fn inc(x i32) i32:
    return x + 1

fn main() i32:
    return inc(inc(inc(0)))
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @inc(i32 %0) {
entry:
  %x = alloca i32, align 4
  store i32 %0, i32* %x, align 4
  %load.x = load i32, i32* %x, align 4
  %ADD = add i32 %load.x, 1
  ret i32 %ADD
}

define i32 @main() {
entry:
  %"inc(0)" = call i32 @inc(i32 0)
  %"inc(inc(0))" = call i32 @inc(i32 %"inc(0)")
  %"inc(inc(inc(0)))" = call i32 @inc(i32 %"inc(inc(0))")
  ret i32 %"inc(inc(inc(0)))"
}
```

---

## 004 — a parameter is a copy

```ura
// expect: 56
fn bump(x i32) i32:
    x += 1
    return x

fn main() i32:
    a i32 = 5
    b i32 = bump(a)
    return a * 10 + b
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @bump(i32 %0) {
entry:
  %x = alloca i32, align 4
  store i32 %0, i32* %x, align 4
  %old = load i32, i32* %x, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %x, align 4
  %load.x = load i32, i32* %x, align 4
  ret i32 %load.x
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 5, i32* %a, align 4
  %load.a = load i32, i32* %a, align 4
  %"bump(load.a)" = call i32 @bump(i32 %load.a)
  store i32 %"bump(load.a)", i32* %b, align 4
  %load.a1 = load i32, i32* %a, align 4
  %MUL = mul i32 %load.a1, 10
  %load.b = load i32, i32* %b, align 4
  %ADD = add i32 %MUL, %load.b
  ret i32 %ADD
}
```

---

## 005 — call result in a variable

```ura
// expect: 14
fn twice(n i32) i32:
    return n * 2

fn main() i32:
    a i32 = twice(3)
    b i32 = twice(a) + 2
    return b
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @twice(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %load.n = load i32, i32* %n, align 4
  %MUL = mul i32 %load.n, 2
  ret i32 %MUL
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %"twice(3)" = call i32 @twice(i32 3)
  store i32 %"twice(3)", i32* %a, align 4
  %load.a = load i32, i32* %a, align 4
  %"twice(load.a)" = call i32 @twice(i32 %load.a)
  %ADD = add i32 %"twice(load.a)", 2
  store i32 %ADD, i32* %b, align 4
  %load.b = load i32, i32* %b, align 4
  ret i32 %load.b
}
```

---

## 006 — void function and bare return

```ura
// expect: 4
fn nothing():
    return

fn main() i32:
    nothing()
    return 4
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define void @nothing() {
entry:
  ret void
}

define i32 @main() {
entry:
  call void @nothing()
  ret i32 4
}
```

---

## 007 — function inside a function

```ura
// expect: 7
fn main() i32:
    fn help() i32:
        return 7
    return help()
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %"help()" = call i32 @help()
  ret i32 %"help()"
}

define i32 @help() {
entry:
  ret i32 7
}
```

---

## 008 — calling function before declaration (from tests-old/functions.md #001)

```ura
fn main() i32:
    return later()

fn later() i32:
    return 9
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %"later()" = call i32 @later()
  ret i32 %"later()"
}

define i32 @later() {
entry:
  ret i32 9
}
```

---

## 009 — math op on functions (from tests-old/functions.md #002)

```ura
fn square(x i32) i32:
    return x * x

fn main() i32:
    return square(3) + square(4)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @square(i32 %0) {
entry:
  %x = alloca i32, align 4
  store i32 %0, i32* %x, align 4
  %load.x = load i32, i32* %x, align 4
  %load.x1 = load i32, i32* %x, align 4
  %MUL = mul i32 %load.x, %load.x1
  ret i32 %MUL
}

define i32 @main() {
entry:
  %"square(3)" = call i32 @square(i32 3)
  %"square(4)" = call i32 @square(i32 4)
  %ADD = add i32 %"square(3)", %"square(4)"
  ret i32 %ADD
}
```

---

## 010 — bare return in a void function (from tests-old/functions.md #010)

```ura
fn foo(a i32):
   return

fn foo(a bool):
   return

fn main() i32:
   foo(1)
   foo(True)
   return 7
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define void @foo(i32 %0) {
entry:
  %a = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  ret void
}

define void @foo.1(i1 %0) {
entry:
  %a = alloca i1, align 1
  store i1 %0, i1* %a, align 1
  ret void
}

define i32 @main() {
entry:
  call void @foo(i32 1)
  call void @foo.1(i1 true)
  ret i32 7
}
```

