# data_types

## index

- 001 — declare variable
- 002 — compound assignment/unary operator
- 003 — declaring char/i8 variable
- 004 — chars type
- 005 — boolean type
- 006 — escaped characters

---

## 001 — declare variable

```ura
fn main() i32:
    a i32 = 10
    a = 20
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
  store i32 20, i32* %a, align 4
  ret i32 0
}
```

### stdout

```

```

### stderr

```

```

---

## 002 — compound assignment/unary operator

```ura
fn main() i32:
    a i32 = -10
    a += +2
    a *= 4
    a /= 2
    a -= 2
    a %= 3
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
  store i32 -10, i32* %a, align 4
  %a1 = load i32, i32* %a, align 4
  %ADD = add i32 %a1, 2
  store i32 %ADD, i32* %a, align 4
  %a2 = load i32, i32* %a, align 4
  %MUL = mul i32 %a2, 4
  store i32 %MUL, i32* %a, align 4
  %a3 = load i32, i32* %a, align 4
  %DIV = sdiv i32 %a3, 2
  store i32 %DIV, i32* %a, align 4
  %a4 = load i32, i32* %a, align 4
  %SUB = sub i32 %a4, 2
  store i32 %SUB, i32* %a, align 4
  %a5 = load i32, i32* %a, align 4
  %MOD = srem i32 %a5, 3
  store i32 %MOD, i32* %a, align 4
  ret i32 0
}
```

---

## 003 — declaring char/i8 variable

```ura
proto exit(code i32)

fn main() i32:
   c1 char = 'a'
   c2 i8 = 'b'
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

declare void @exit(i32)

define i32 @main() {
entry:
  %c1 = alloca i8, align 1
  store i8 97, i8* %c1, align 1
  %c2 = alloca i8, align 1
  store i8 98, i8* %c2, align 1
  ret i32 0
}
```

---

## 004 — chars type

```ura
fn main() i32:
   str chars = "abcdef"
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private unnamed_addr constant [7 x i8] c"abcdef\00", align 1

define i32 @main() {
entry:
  %str = alloca i8*, align 8
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i8** %str, align 8
  ret i32 0
}
```

---

## 005 — boolean type

```ura
fn main() i32:
   flag b1 = True
   if flag:
      return 1
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
  %flag = alloca i1, align 1
  store i1 true, i1* %flag, align 1
  %flag1 = load i1, i1* %flag, align 1
  br i1 %flag1, label %then, label %endif

then:                                             ; preds = %entry
  ret i32 1

endif:                                            ; preds = %entry
  ret i32 0
}
```

---

## 006 — escaped characters

```ura
proto printf(s chars, ...)

fn main() i32:
   printf("tab\there\n")
   return 0
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private unnamed_addr constant [10 x i8] c"tab\09here\0A\00", align 1

declare void @printf(i8*, ...)

define i32 @main() {
entry:
  call void (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0))
  ret i32 0
}
```

