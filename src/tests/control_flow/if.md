# if

## index

- 001 — if / elif / else chain
- 002 — every comparison
- 003 — and / or
- 004 — body on the same line
- 005 — nested ifs
- 006 — no branch taken: code after the if runs
- 007 — recursion
- 008 — a variable declared in a block
- 009 — basic if (from tests-old/statements.md #001)
- 010 — basic if/elif/else chain (from tests-old/statements.md #002)
- 011 — ifs with comparision operators (from tests-old/statements.md #003)
- 012 — nested ifs (from tests-old/statements.md #005)
- 013 — logic operators and/or (from tests-old/statements.md #006)

---

## 001 — if / elif / else chain

```ura
// expect: 123
fn grade(n i32) i32:
    if n < 10:
        return 1
    elif n < 20:
        return 2
    else:
        return 3

fn main() i32:
    return grade(5) * 100 + grade(15) * 10 + grade(25)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @grade(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %load.n = load i32, i32* %n, align 4
  %LT = icmp slt i32 %load.n, 10
  br i1 %LT, label %then, label %next

then:                                             ; preds = %entry
  ret i32 1

next:                                             ; preds = %entry
  %load.n3 = load i32, i32* %n, align 4
  %LT4 = icmp slt i32 %load.n3, 20
  br i1 %LT4, label %then1, label %next2

then1:                                            ; preds = %next
  ret i32 2

next2:                                            ; preds = %next
  ret i32 3

endif:                                            ; No predecessors!
  ret i32 0
}

define i32 @main() {
entry:
  %"grade(5)" = call i32 @grade(i32 5)
  %MUL = mul i32 %"grade(5)", 100
  %"grade(15)" = call i32 @grade(i32 15)
  %MUL1 = mul i32 %"grade(15)", 10
  %ADD = add i32 %MUL, %MUL1
  %"grade(25)" = call i32 @grade(i32 25)
  %ADD2 = add i32 %ADD, %"grade(25)"
  ret i32 %ADD2
}
```

---

## 002 — every comparison

```ura
// expect: 6
fn main() i32:
    a i32 = 5
    n i32 = 0
    if a == 5:
        n += 1
    if a != 4:
        n += 1
    if a < 6:
        n += 1
    if a <= 5:
        n += 1
    if a > 4:
        n += 1
    if a >= 5:
        n += 1
    if a > 5:
        n += 100
    return n
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
  %n = alloca i32, align 4
  store i32 5, i32* %a, align 4
  store i32 0, i32* %n, align 4
  %load.a = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %load.a, 5
  br i1 %EQ, label %then, label %endif

then:                                             ; preds = %entry
  %old = load i32, i32* %n, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %load.a3 = load i32, i32* %a, align 4
  %NQ = icmp ne i32 %load.a3, 4
  br i1 %NQ, label %then2, label %endif1

then2:                                            ; preds = %endif
  %old4 = load i32, i32* %n, align 4
  %ADD_ASSIGN5 = add i32 %old4, 1
  store i32 %ADD_ASSIGN5, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %then2, %endif
  %load.a8 = load i32, i32* %a, align 4
  %LT = icmp slt i32 %load.a8, 6
  br i1 %LT, label %then7, label %endif6

then7:                                            ; preds = %endif1
  %old9 = load i32, i32* %n, align 4
  %ADD_ASSIGN10 = add i32 %old9, 1
  store i32 %ADD_ASSIGN10, i32* %n, align 4
  br label %endif6

endif6:                                           ; preds = %then7, %endif1
  %load.a13 = load i32, i32* %a, align 4
  %LE = icmp sle i32 %load.a13, 5
  br i1 %LE, label %then12, label %endif11

then12:                                           ; preds = %endif6
  %old14 = load i32, i32* %n, align 4
  %ADD_ASSIGN15 = add i32 %old14, 1
  store i32 %ADD_ASSIGN15, i32* %n, align 4
  br label %endif11

endif11:                                          ; preds = %then12, %endif6
  %load.a18 = load i32, i32* %a, align 4
  %GT = icmp sgt i32 %load.a18, 4
  br i1 %GT, label %then17, label %endif16

then17:                                           ; preds = %endif11
  %old19 = load i32, i32* %n, align 4
  %ADD_ASSIGN20 = add i32 %old19, 1
  store i32 %ADD_ASSIGN20, i32* %n, align 4
  br label %endif16

endif16:                                          ; preds = %then17, %endif11
  %load.a23 = load i32, i32* %a, align 4
  %GE = icmp sge i32 %load.a23, 5
  br i1 %GE, label %then22, label %endif21

then22:                                           ; preds = %endif16
  %old24 = load i32, i32* %n, align 4
  %ADD_ASSIGN25 = add i32 %old24, 1
  store i32 %ADD_ASSIGN25, i32* %n, align 4
  br label %endif21

endif21:                                          ; preds = %then22, %endif16
  %load.a28 = load i32, i32* %a, align 4
  %GT29 = icmp sgt i32 %load.a28, 5
  br i1 %GT29, label %then27, label %endif26

then27:                                           ; preds = %endif21
  %old30 = load i32, i32* %n, align 4
  %ADD_ASSIGN31 = add i32 %old30, 100
  store i32 %ADD_ASSIGN31, i32* %n, align 4
  br label %endif26

endif26:                                          ; preds = %then27, %endif21
  %load.n = load i32, i32* %n, align 4
  ret i32 %load.n
}
```

---

## 003 — and / or

```ura
// expect: 11
fn main() i32:
    a i32 = 5
    n i32 = 0
    if a > 1 and a < 10:
        n += 1
    if a < 1 or a == 5:
        n += 10
    if a < 1 and a == 5:
        n += 100
    return n
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
  %n = alloca i32, align 4
  store i32 5, i32* %a, align 4
  store i32 0, i32* %n, align 4
  %load.a = load i32, i32* %a, align 4
  %GT = icmp sgt i32 %load.a, 1
  %load.a1 = load i32, i32* %a, align 4
  %LT = icmp slt i32 %load.a1, 10
  %AND = and i1 %GT, %LT
  br i1 %AND, label %then, label %endif

then:                                             ; preds = %entry
  %old = load i32, i32* %n, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %load.a4 = load i32, i32* %a, align 4
  %LT5 = icmp slt i32 %load.a4, 1
  %load.a6 = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %load.a6, 5
  %OR = or i1 %LT5, %EQ
  br i1 %OR, label %then3, label %endif2

then3:                                            ; preds = %endif
  %old7 = load i32, i32* %n, align 4
  %ADD_ASSIGN8 = add i32 %old7, 10
  store i32 %ADD_ASSIGN8, i32* %n, align 4
  br label %endif2

endif2:                                           ; preds = %then3, %endif
  %load.a11 = load i32, i32* %a, align 4
  %LT12 = icmp slt i32 %load.a11, 1
  %load.a13 = load i32, i32* %a, align 4
  %EQ14 = icmp eq i32 %load.a13, 5
  %AND15 = and i1 %LT12, %EQ14
  br i1 %AND15, label %then10, label %endif9

then10:                                           ; preds = %endif2
  %old16 = load i32, i32* %n, align 4
  %ADD_ASSIGN17 = add i32 %old16, 100
  store i32 %ADD_ASSIGN17, i32* %n, align 4
  br label %endif9

endif9:                                           ; preds = %then10, %endif2
  %load.n = load i32, i32* %n, align 4
  ret i32 %load.n
}
```

---

## 004 — body on the same line

```ura
// expect: 3
fn main() i32:
    a i32 = 5
    n i32 = 0
    if a > 3: n += 1
    if a > 9: n += 10
    if a == 5: n += 2
    return n
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
  %n = alloca i32, align 4
  store i32 5, i32* %a, align 4
  store i32 0, i32* %n, align 4
  %load.a = load i32, i32* %a, align 4
  %GT = icmp sgt i32 %load.a, 3
  br i1 %GT, label %then, label %endif

then:                                             ; preds = %entry
  %old = load i32, i32* %n, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %load.a3 = load i32, i32* %a, align 4
  %GT4 = icmp sgt i32 %load.a3, 9
  br i1 %GT4, label %then2, label %endif1

then2:                                            ; preds = %endif
  %old5 = load i32, i32* %n, align 4
  %ADD_ASSIGN6 = add i32 %old5, 10
  store i32 %ADD_ASSIGN6, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %then2, %endif
  %load.a9 = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %load.a9, 5
  br i1 %EQ, label %then8, label %endif7

then8:                                            ; preds = %endif1
  %old10 = load i32, i32* %n, align 4
  %ADD_ASSIGN11 = add i32 %old10, 2
  store i32 %ADD_ASSIGN11, i32* %n, align 4
  br label %endif7

endif7:                                           ; preds = %then8, %endif1
  %load.n = load i32, i32* %n, align 4
  ret i32 %load.n
}
```

---

## 005 — nested ifs

```ura
// expect: 1
fn main() i32:
    a i32 = 5
    b i32 = 7
    if a == 5:
        if b == 7:
            return 1
        return 2
    return 3
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
  store i32 5, i32* %a, align 4
  store i32 7, i32* %b, align 4
  %load.a = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %load.a, 5
  br i1 %EQ, label %then, label %endif

then:                                             ; preds = %entry
  %load.b = load i32, i32* %b, align 4
  %EQ3 = icmp eq i32 %load.b, 7
  br i1 %EQ3, label %then2, label %endif1

then2:                                            ; preds = %then
  ret i32 1

endif1:                                           ; preds = %then
  ret i32 2

endif:                                            ; preds = %entry
  ret i32 3
}
```

---

## 006 — no branch taken: code after the if runs

```ura
// expect: 9
fn main() i32:
    a i32 = 1
    if a == 2:
        a = 100
    elif a == 3:
        a = 200
    return a + 8
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
  store i32 1, i32* %a, align 4
  %load.a = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %load.a, 2
  br i1 %EQ, label %then, label %next

then:                                             ; preds = %entry
  store i32 100, i32* %a, align 4
  br label %endif

next:                                             ; preds = %entry
  %load.a2 = load i32, i32* %a, align 4
  %EQ3 = icmp eq i32 %load.a2, 3
  br i1 %EQ3, label %then1, label %endif

then1:                                            ; preds = %next
  store i32 200, i32* %a, align 4
  br label %endif

endif:                                            ; preds = %then1, %next, %then
  %load.a4 = load i32, i32* %a, align 4
  %ADD = add i32 %load.a4, 8
  ret i32 %ADD
}
```

---

## 007 — recursion

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
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @fact(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %load.n = load i32, i32* %n, align 4
  %LE = icmp sle i32 %load.n, 1
  br i1 %LE, label %then, label %endif

then:                                             ; preds = %entry
  ret i32 1

endif:                                            ; preds = %entry
  %load.n1 = load i32, i32* %n, align 4
  %load.n2 = load i32, i32* %n, align 4
  %SUB = sub i32 %load.n2, 1
  %"fact(load.n - 1)" = call i32 @fact(i32 %SUB)
  %MUL = mul i32 %load.n1, %"fact(load.n - 1)"
  ret i32 %MUL
}

define i32 @main() {
entry:
  %"fact(5)" = call i32 @fact(i32 5)
  ret i32 %"fact(5)"
}
```

---

## 008 — a variable declared in a block

```ura
// expect: 3
fn main() i32:
    a i32 = 1
    if a == 1:
        b i32 = 2
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
  store i32 1, i32* %a, align 4
  %load.a = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %load.a, 1
  br i1 %EQ, label %then, label %endif

then:                                             ; preds = %entry
  store i32 2, i32* %b, align 4
  %load.a1 = load i32, i32* %a, align 4
  %load.b = load i32, i32* %b, align 4
  %ADD = add i32 %load.a1, %load.b
  store i32 %ADD, i32* %a, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %load.a2 = load i32, i32* %a, align 4
  ret i32 %load.a2
}
```

---

## 009 — basic if (from tests-old/statements.md #001)

```ura
fn main() i32:
    a i32 = 0
    if a < 1:
        return 1
    return 2
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
  store i32 0, i32* %a, align 4
  %load.a = load i32, i32* %a, align 4
  %LT = icmp slt i32 %load.a, 1
  br i1 %LT, label %then, label %endif

then:                                             ; preds = %entry
  ret i32 1

endif:                                            ; preds = %entry
  ret i32 2
}
```

---

## 010 — basic if/elif/else chain (from tests-old/statements.md #002)

```ura
fn main() i32:
    a i32 = 0
    if a < 1:
        return 1
    elif a < 2:
        return 2
    else:
        return 3
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
  store i32 0, i32* %a, align 4
  %load.a = load i32, i32* %a, align 4
  %LT = icmp slt i32 %load.a, 1
  br i1 %LT, label %then, label %next

then:                                             ; preds = %entry
  ret i32 1

next:                                             ; preds = %entry
  %load.a3 = load i32, i32* %a, align 4
  %LT4 = icmp slt i32 %load.a3, 2
  br i1 %LT4, label %then1, label %next2

then1:                                            ; preds = %next
  ret i32 2

next2:                                            ; preds = %next
  ret i32 3

endif:                                            ; No predecessors!
  %load.a5 = load i32, i32* %a, align 4
  ret i32 %load.a5
}
```

---

## 011 — ifs with comparision operators (from tests-old/statements.md #003)

```ura
fn main() i32:
    a i32 = 5
    n i32 = 0
    if a > 3: n = n + 1
    if a > 5: n = n + 10
    if a < 6: n = n + 1
    if a < 5: n = n + 10
    if a >= 5: n = n + 1
    if a >= 6: n = n + 10
    if a <= 5: n = n + 1
    if a <= 4: n = n + 10
    if a == 5: n = n + 1
    if a == 4: n = n + 10
    if a != 4: n = n + 1
    if a != 5: n = n + 10
    return n
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
  %n = alloca i32, align 4
  store i32 5, i32* %a, align 4
  store i32 0, i32* %n, align 4
  %load.a = load i32, i32* %a, align 4
  %GT = icmp sgt i32 %load.a, 3
  br i1 %GT, label %then, label %endif

then:                                             ; preds = %entry
  %load.n = load i32, i32* %n, align 4
  %ADD = add i32 %load.n, 1
  store i32 %ADD, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %load.a3 = load i32, i32* %a, align 4
  %GT4 = icmp sgt i32 %load.a3, 5
  br i1 %GT4, label %then2, label %endif1

then2:                                            ; preds = %endif
  %load.n5 = load i32, i32* %n, align 4
  %ADD6 = add i32 %load.n5, 10
  store i32 %ADD6, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %then2, %endif
  %load.a9 = load i32, i32* %a, align 4
  %LT = icmp slt i32 %load.a9, 6
  br i1 %LT, label %then8, label %endif7

then8:                                            ; preds = %endif1
  %load.n10 = load i32, i32* %n, align 4
  %ADD11 = add i32 %load.n10, 1
  store i32 %ADD11, i32* %n, align 4
  br label %endif7

endif7:                                           ; preds = %then8, %endif1
  %load.a14 = load i32, i32* %a, align 4
  %LT15 = icmp slt i32 %load.a14, 5
  br i1 %LT15, label %then13, label %endif12

then13:                                           ; preds = %endif7
  %load.n16 = load i32, i32* %n, align 4
  %ADD17 = add i32 %load.n16, 10
  store i32 %ADD17, i32* %n, align 4
  br label %endif12

endif12:                                          ; preds = %then13, %endif7
  %load.a20 = load i32, i32* %a, align 4
  %GE = icmp sge i32 %load.a20, 5
  br i1 %GE, label %then19, label %endif18

then19:                                           ; preds = %endif12
  %load.n21 = load i32, i32* %n, align 4
  %ADD22 = add i32 %load.n21, 1
  store i32 %ADD22, i32* %n, align 4
  br label %endif18

endif18:                                          ; preds = %then19, %endif12
  %load.a25 = load i32, i32* %a, align 4
  %GE26 = icmp sge i32 %load.a25, 6
  br i1 %GE26, label %then24, label %endif23

then24:                                           ; preds = %endif18
  %load.n27 = load i32, i32* %n, align 4
  %ADD28 = add i32 %load.n27, 10
  store i32 %ADD28, i32* %n, align 4
  br label %endif23

endif23:                                          ; preds = %then24, %endif18
  %load.a31 = load i32, i32* %a, align 4
  %LE = icmp sle i32 %load.a31, 5
  br i1 %LE, label %then30, label %endif29

then30:                                           ; preds = %endif23
  %load.n32 = load i32, i32* %n, align 4
  %ADD33 = add i32 %load.n32, 1
  store i32 %ADD33, i32* %n, align 4
  br label %endif29

endif29:                                          ; preds = %then30, %endif23
  %load.a36 = load i32, i32* %a, align 4
  %LE37 = icmp sle i32 %load.a36, 4
  br i1 %LE37, label %then35, label %endif34

then35:                                           ; preds = %endif29
  %load.n38 = load i32, i32* %n, align 4
  %ADD39 = add i32 %load.n38, 10
  store i32 %ADD39, i32* %n, align 4
  br label %endif34

endif34:                                          ; preds = %then35, %endif29
  %load.a42 = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %load.a42, 5
  br i1 %EQ, label %then41, label %endif40

then41:                                           ; preds = %endif34
  %load.n43 = load i32, i32* %n, align 4
  %ADD44 = add i32 %load.n43, 1
  store i32 %ADD44, i32* %n, align 4
  br label %endif40

endif40:                                          ; preds = %then41, %endif34
  %load.a47 = load i32, i32* %a, align 4
  %EQ48 = icmp eq i32 %load.a47, 4
  br i1 %EQ48, label %then46, label %endif45

then46:                                           ; preds = %endif40
  %load.n49 = load i32, i32* %n, align 4
  %ADD50 = add i32 %load.n49, 10
  store i32 %ADD50, i32* %n, align 4
  br label %endif45

endif45:                                          ; preds = %then46, %endif40
  %load.a53 = load i32, i32* %a, align 4
  %NQ = icmp ne i32 %load.a53, 4
  br i1 %NQ, label %then52, label %endif51

then52:                                           ; preds = %endif45
  %load.n54 = load i32, i32* %n, align 4
  %ADD55 = add i32 %load.n54, 1
  store i32 %ADD55, i32* %n, align 4
  br label %endif51

endif51:                                          ; preds = %then52, %endif45
  %load.a58 = load i32, i32* %a, align 4
  %NQ59 = icmp ne i32 %load.a58, 5
  br i1 %NQ59, label %then57, label %endif56

then57:                                           ; preds = %endif51
  %load.n60 = load i32, i32* %n, align 4
  %ADD61 = add i32 %load.n60, 10
  store i32 %ADD61, i32* %n, align 4
  br label %endif56

endif56:                                          ; preds = %then57, %endif51
  %load.n62 = load i32, i32* %n, align 4
  ret i32 %load.n62
}
```

---

## 012 — nested ifs (from tests-old/statements.md #005)

```ura
fn main() i32:
    a i32 = 5
    b i32 = 7
    if a == 5:
        if b == 7: return 1
        return 2
    return 3
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
  store i32 5, i32* %a, align 4
  store i32 7, i32* %b, align 4
  %load.a = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %load.a, 5
  br i1 %EQ, label %then, label %endif

then:                                             ; preds = %entry
  %load.b = load i32, i32* %b, align 4
  %EQ3 = icmp eq i32 %load.b, 7
  br i1 %EQ3, label %then2, label %endif1

then2:                                            ; preds = %then
  ret i32 1

endif1:                                           ; preds = %then
  ret i32 2

endif:                                            ; preds = %entry
  ret i32 3
}
```

---

## 013 — logic operators and/or (from tests-old/statements.md #006)

```ura
fn main() i32:
    a i32 = 5
    if a > 1 and a < 10:
        return 1
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
  store i32 5, i32* %a, align 4
  %load.a = load i32, i32* %a, align 4
  %GT = icmp sgt i32 %load.a, 1
  %load.a1 = load i32, i32* %a, align 4
  %LT = icmp slt i32 %load.a1, 10
  %AND = and i1 %GT, %LT
  br i1 %AND, label %then, label %endif

then:                                             ; preds = %entry
  ret i32 1

endif:                                            ; preds = %entry
  ret i32 0
}
```

