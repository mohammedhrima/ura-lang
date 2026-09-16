# ref

## index

- 001 — own address of a variable
- 002 — derefrencing variable
- 003 — ref as parameter

---

## 001 — own address of a variable

```ura
fn main() i32:
    a i32 = 10

    p ref(i32) = own(a)
    // dref(p) = 12
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

## 002 — derefrencing variable

```ura
fn main() i32:
    a i32 = 10

    p ref(i32) = own(a)
    dref(p) = 12
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
  %p1 = load i32*, i32** %p, align 8
  store i32 12, i32* %p1, align 4
  ret i32 0
}
```

---

## 003 — ref as parameter

```ura
fn foo(a ref(i32)):
    dref(a) = 2

fn main() i32:
    a i32 = 10

    foo(own(a))

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

