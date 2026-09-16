# functions

## index

- 001 — calling function before declaration
- 002 — math op on functions
- 003 — nested calling
- 004 — factorial
- 005 — fn returns ref
- 006 — swap

---

## 001 — calling function before declaration

```ura
fn main() i32:
    return later()

fn later() i32:
    return 9
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %later = call i32 @later()
  ret i32 %later
}

define i32 @later() {
entry:
  ret i32 9
}
```

---

## 002 — math op on functions

```ura
fn square(x i32) i32:
    return x * x

fn main() i32:
    return square(3) + square(4)
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @square(i32 %0) {
entry:
  %x = alloca i32, align 4
  store i32 %0, i32* %x, align 4
  %x1 = load i32, i32* %x, align 4
  %x2 = load i32, i32* %x, align 4
  %MUL = mul i32 %x1, %x2
  ret i32 %MUL
}

define i32 @main() {
entry:
  %square = call i32 @square(i32 3)
  %square1 = call i32 @square(i32 4)
  %ADD = add i32 %square, %square1
  ret i32 %ADD
}
```

---

## 003 — nested calling

```ura
fn inc(x i32) i32:
    return x + 1

fn main() i32:
    return inc(inc(inc(0)))
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @inc(i32 %0) {
entry:
  %x = alloca i32, align 4
  store i32 %0, i32* %x, align 4
  %x1 = load i32, i32* %x, align 4
  %ADD = add i32 %x1, 1
  ret i32 %ADD
}

define i32 @main() {
entry:
  %inc = call i32 @inc(i32 0)
  %inc1 = call i32 @inc(i32 %inc)
  %inc2 = call i32 @inc(i32 %inc1)
  ret i32 %inc2
}
```

---

## 004 — factorial

```ura
fn fact(n i32) i32:
    if n <= 1:
        return 1
    return n * fact(n - 1)

fn main() i32:
    return fact(5)
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @fact(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %n1 = load i32, i32* %n, align 4
  %LE = icmp sle i32 %n1, 1
  br i1 %LE, label %then, label %endif

then:                                             ; preds = %entry
  ret i32 1

endif:                                            ; preds = %entry
  %n2 = load i32, i32* %n, align 4
  %n3 = load i32, i32* %n, align 4
  %SUB = sub i32 %n3, 1
  %fact = call i32 @fact(i32 %SUB)
  %MUL = mul i32 %n2, %fact
  ret i32 %MUL
}

define i32 @main() {
entry:
  %fact = call i32 @fact(i32 5)
  ret i32 %fact
}
```

---

## 005 — fn returns ref

```ura
fn same(p ref(i32)) ref(i32):
    return p

fn main() i32:
    a i32 = 6
    q ref(i32) = same(own(a))
    return dref(q)
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32* @same(i32* %0) {
entry:
  %p = alloca i32*, align 8
  store i32* %0, i32** %p, align 8
  %p1 = load i32*, i32** %p, align 8
  ret i32* %p1
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 6, i32* %a, align 4
  %q = alloca i32*, align 8
  %same = call i32* @same(i32* %a)
  store i32* %same, i32** %q, align 8
  %q1 = load i32*, i32** %q, align 8
  %dref = load i32, i32* %q1, align 4
  ret i32 %dref
}
```

---

## 006 — swap

```ura
fn swap(x ref(i32), y ref(i32)):
    t i32 = dref(x)
    dref(x) = dref(y)
    dref(y) = t

fn main() i32:
    a i32 = 1
    b i32 = 2
    swap(own(a), own(b))
    return a * 10 + b
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define void @swap(i32* %0, i32* %1) {
entry:
  %x = alloca i32*, align 8
  store i32* %0, i32** %x, align 8
  %y = alloca i32*, align 8
  store i32* %1, i32** %y, align 8
  %t = alloca i32, align 4
  %x1 = load i32*, i32** %x, align 8
  %dref = load i32, i32* %x1, align 4
  store i32 %dref, i32* %t, align 4
  %y2 = load i32*, i32** %y, align 8
  %dref3 = load i32, i32* %y2, align 4
  %x4 = load i32*, i32** %x, align 8
  store i32 %dref3, i32* %x4, align 4
  %t5 = load i32, i32* %t, align 4
  %y6 = load i32*, i32** %y, align 8
  store i32 %t5, i32* %y6, align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 1, i32* %a, align 4
  %b = alloca i32, align 4
  store i32 2, i32* %b, align 4
  call void @swap(i32* %a, i32* %b)
  %a1 = load i32, i32* %a, align 4
  %MUL = mul i32 %a1, 10
  %b2 = load i32, i32* %b, align 4
  %ADD = add i32 %MUL, %b2
  ret i32 %ADD
}
```

