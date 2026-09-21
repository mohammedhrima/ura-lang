# ref

## index

- 001 — take the address of a variable
- 002 — write through a ref
- 003 — ref as parameter
- 004 — read through a ref
- 005 — math through a ref
- 006 — copy the pointed value between refs
- 007 — rebind a ref

---

## 001 — take the address of a variable

```ura
fn main() i32:
    a i32 = 10
    p &i32 = &a
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 10, i32* %a, align 4
  %p = alloca i32*, align 8
  store i32* %a, i32** %p, align 8
  ret i32 0
}
```

---

## 002 — write through a ref

```ura
fn main() i32:
    a i32 = 10
    p &i32 = &a
    p = 12
    return a
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 10, i32* %a, align 4
  %p = alloca i32*, align 8
  store i32* %a, i32** %p, align 8
  %p1 = load i32*, i32** %p, align 8
  store i32 12, i32* %p1, align 4
  %a2 = load i32, i32* %a, align 4
  ret i32 %a2
}
```

---

## 003 — ref as parameter

```ura
fn foo(a &i32):
    a = 2

fn main() i32:
    a i32 = 10
    foo(&a)
    return a
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define void @foo(i32* %0) {
entry:
  %a = alloca i32*, align 8
  store i32* %0, i32** %a, align 8
  %a1 = load i32*, i32** %a, align 8
  store i32 2, i32* %a1, align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 10, i32* %a, align 4
  call void @foo(i32* %a)
  %a1 = load i32, i32* %a, align 4
  ret i32 %a1
}
```

---

## 004 — read through a ref

```ura
fn main() i32:
    a i32 = 10
    p &i32 = &a
    b i32 = p
    return b
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 10, i32* %a, align 4
  %p = alloca i32*, align 8
  store i32* %a, i32** %p, align 8
  %b = alloca i32, align 4
  %p1 = load i32*, i32** %p, align 8
  %dref = load i32, i32* %p1, align 4
  store i32 %dref, i32* %b, align 4
  %b2 = load i32, i32* %b, align 4
  ret i32 %b2
}
```

---

## 005 — math through a ref

```ura
fn main() i32:
    a i32 = 10
    p &i32 = &a
    p = p + 5
    return a
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 10, i32* %a, align 4
  %p = alloca i32*, align 8
  store i32* %a, i32** %p, align 8
  %p1 = load i32*, i32** %p, align 8
  %dref = load i32, i32* %p1, align 4
  %ADD = add i32 %dref, 5
  %p2 = load i32*, i32** %p, align 8
  store i32 %ADD, i32* %p2, align 4
  %a3 = load i32, i32* %a, align 4
  ret i32 %a3
}
```

---

## 006 — copy the pointed value between refs

```ura
fn main() i32:
    a i32 = 1
    b i32 = 2
    p1 &i32 = &a
    p2 &i32 = &b
    p1 = p2
    return a
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 1, i32* %a, align 4
  %b = alloca i32, align 4
  store i32 2, i32* %b, align 4
  %p1 = alloca i32*, align 8
  store i32* %a, i32** %p1, align 8
  %p2 = alloca i32*, align 8
  store i32* %b, i32** %p2, align 8
  %p21 = load i32*, i32** %p2, align 8
  %dref = load i32, i32* %p21, align 4
  %p12 = load i32*, i32** %p1, align 8
  store i32 %dref, i32* %p12, align 4
  %a3 = load i32, i32* %a, align 4
  ret i32 %a3
}
```

---

## 007 — rebind a ref

```ura
fn main() i32:
    a i32 = 1
    b i32 = 2
    p1 &i32 = &a
    p2 &i32 = &b
    p1 = &p2
    p1 = 9
    return b
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 1, i32* %a, align 4
  %b = alloca i32, align 4
  store i32 2, i32* %b, align 4
  %p1 = alloca i32*, align 8
  store i32* %a, i32** %p1, align 8
  %p2 = alloca i32*, align 8
  store i32* %b, i32** %p2, align 8
  %p21 = load i32*, i32** %p2, align 8
  store i32* %p21, i32** %p1, align 8
  %p12 = load i32*, i32** %p1, align 8
  store i32 9, i32* %p12, align 4
  %b3 = load i32, i32* %b, align 4
  ret i32 %b3
}
```

