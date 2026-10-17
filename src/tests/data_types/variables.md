# variables

## index

- 001 — declare with a value
- 002 — reassign
- 003 — use another variable
- 004 — compound assignment
- 005 — declare variable (from tests-old/data_types.md #001)

---

## 001 — declare with a value

```ura
// expect: 10
fn main() i32:
    a i32 = 10
    return a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 10, i32* %a, align 4
  %load.a = load i32, i32* %a, align 4
  ret i32 %load.a
}
```

---

## 002 — reassign

```ura
// expect: 20
fn main() i32:
    a i32 = 10
    a = 20
    return a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 10, i32* %a, align 4
  store i32 20, i32* %a, align 4
  %load.a = load i32, i32* %a, align 4
  ret i32 %load.a
}
```

---

## 003 — use another variable

```ura
// expect: 30
fn main() i32:
    a i32 = 10
    b i32 = a * 2
    a = a + b
    return a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 10, i32* %a, align 4
  %load.a = load i32, i32* %a, align 4
  %MUL = mul i32 %load.a, 2
  store i32 %MUL, i32* %b, align 4
  %load.a1 = load i32, i32* %a, align 4
  %load.b = load i32, i32* %b, align 4
  %ADD = add i32 %load.a1, %load.b
  store i32 %ADD, i32* %a, align 4
  %load.a2 = load i32, i32* %a, align 4
  ret i32 %load.a2
}
```

---

## 004 — compound assignment

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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 10, i32* %a, align 4
  %load.a = load i32, i32* %a, align 4
  %MUL = mul i32 %load.a, 2
  store i32 %MUL, i32* %b, align 4
  %load.a1 = load i32, i32* %a, align 4
  %load.b = load i32, i32* %b, align 4
  %ADD = add i32 %load.a1, %load.b
  store i32 %ADD, i32* %a, align 4
  %old = load i32, i32* %a, align 4
  %ADD_ASSIGN = add i32 %old, 5
  store i32 %ADD_ASSIGN, i32* %a, align 4
  %old2 = load i32, i32* %a, align 4
  %SUB_ASSIGN = sub i32 %old2, 3
  store i32 %SUB_ASSIGN, i32* %a, align 4
  %old3 = load i32, i32* %a, align 4
  %MUL_ASSIGN = mul i32 %old3, 2
  store i32 %MUL_ASSIGN, i32* %a, align 4
  %old4 = load i32, i32* %a, align 4
  %DIV_ASSIGN = sdiv i32 %old4, 4
  store i32 %DIV_ASSIGN, i32* %a, align 4
  %old5 = load i32, i32* %a, align 4
  %MOD_ASSIGN = srem i32 %old5, 5
  store i32 %MOD_ASSIGN, i32* %a, align 4
  %load.a6 = load i32, i32* %a, align 4
  %load.b7 = load i32, i32* %b, align 4
  %ADD8 = add i32 %load.a6, %load.b7
  ret i32 %ADD8
}
```

---

## 005 — declare variable (from tests-old/data_types.md #001)

```ura
fn main() i32:
    a i32 = 10
    a = 20
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 10, i32* %a, align 4
  store i32 20, i32* %a, align 4
  ret i32 0
}
```

