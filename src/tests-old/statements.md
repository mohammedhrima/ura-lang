# statements

## index

- 001 — basic if
- 002 — basic if/elif/else chain
- 003 — ifs with comparision operators
- 004 — if with a ref
- 005 — nested ifs
- 006 — logic operators and/or

---

## 001 — basic if

```ura
fn main() i32:
    a i32 = 0
    if a < 1:
        return 1
    return 2
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
  store i32 0, i32* %a, align 4
  %a1 = load i32, i32* %a, align 4
  %LT = icmp slt i32 %a1, 1
  br i1 %LT, label %then, label %endif

then:                                             ; preds = %entry
  ret i32 1

endif:                                            ; preds = %entry
  ret i32 2
}
```

---

## 002 — basic if/elif/else chain

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
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 0, i32* %a, align 4
  %a1 = load i32, i32* %a, align 4
  %LT = icmp slt i32 %a1, 1
  br i1 %LT, label %then, label %elif

then:                                             ; preds = %entry
  ret i32 1

elif:                                             ; preds = %entry
  %a3 = load i32, i32* %a, align 4
  %LT4 = icmp slt i32 %a3, 2
  br i1 %LT4, label %then2, label %else

then2:                                            ; preds = %elif
  ret i32 2

else:                                             ; preds = %elif
  ret i32 3

endif:                                            ; No predecessors!
  %a5 = load i32, i32* %a, align 4
  ret i32 %a5
}
```

---

## 003 — ifs with comparision operators

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
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %n = alloca i32, align 4
  store i32 5, i32* %a, align 4
  store i32 0, i32* %n, align 4
  %a1 = load i32, i32* %a, align 4
  %GT = icmp sgt i32 %a1, 3
  br i1 %GT, label %then, label %endif

then:                                             ; preds = %entry
  %n2 = load i32, i32* %n, align 4
  %ADD = add i32 %n2, 1
  store i32 %ADD, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %a5 = load i32, i32* %a, align 4
  %GT6 = icmp sgt i32 %a5, 5
  br i1 %GT6, label %then4, label %endif3

then4:                                            ; preds = %endif
  %n7 = load i32, i32* %n, align 4
  %ADD8 = add i32 %n7, 10
  store i32 %ADD8, i32* %n, align 4
  br label %endif3

endif3:                                           ; preds = %then4, %endif
  %a11 = load i32, i32* %a, align 4
  %LT = icmp slt i32 %a11, 6
  br i1 %LT, label %then10, label %endif9

then10:                                           ; preds = %endif3
  %n12 = load i32, i32* %n, align 4
  %ADD13 = add i32 %n12, 1
  store i32 %ADD13, i32* %n, align 4
  br label %endif9

endif9:                                           ; preds = %then10, %endif3
  %a16 = load i32, i32* %a, align 4
  %LT17 = icmp slt i32 %a16, 5
  br i1 %LT17, label %then15, label %endif14

then15:                                           ; preds = %endif9
  %n18 = load i32, i32* %n, align 4
  %ADD19 = add i32 %n18, 10
  store i32 %ADD19, i32* %n, align 4
  br label %endif14

endif14:                                          ; preds = %then15, %endif9
  %a22 = load i32, i32* %a, align 4
  %GE = icmp sge i32 %a22, 5
  br i1 %GE, label %then21, label %endif20

then21:                                           ; preds = %endif14
  %n23 = load i32, i32* %n, align 4
  %ADD24 = add i32 %n23, 1
  store i32 %ADD24, i32* %n, align 4
  br label %endif20

endif20:                                          ; preds = %then21, %endif14
  %a27 = load i32, i32* %a, align 4
  %GE28 = icmp sge i32 %a27, 6
  br i1 %GE28, label %then26, label %endif25

then26:                                           ; preds = %endif20
  %n29 = load i32, i32* %n, align 4
  %ADD30 = add i32 %n29, 10
  store i32 %ADD30, i32* %n, align 4
  br label %endif25

endif25:                                          ; preds = %then26, %endif20
  %a33 = load i32, i32* %a, align 4
  %LE = icmp sle i32 %a33, 5
  br i1 %LE, label %then32, label %endif31

then32:                                           ; preds = %endif25
  %n34 = load i32, i32* %n, align 4
  %ADD35 = add i32 %n34, 1
  store i32 %ADD35, i32* %n, align 4
  br label %endif31

endif31:                                          ; preds = %then32, %endif25
  %a38 = load i32, i32* %a, align 4
  %LE39 = icmp sle i32 %a38, 4
  br i1 %LE39, label %then37, label %endif36

then37:                                           ; preds = %endif31
  %n40 = load i32, i32* %n, align 4
  %ADD41 = add i32 %n40, 10
  store i32 %ADD41, i32* %n, align 4
  br label %endif36

endif36:                                          ; preds = %then37, %endif31
  %a44 = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %a44, 5
  br i1 %EQ, label %then43, label %endif42

then43:                                           ; preds = %endif36
  %n45 = load i32, i32* %n, align 4
  %ADD46 = add i32 %n45, 1
  store i32 %ADD46, i32* %n, align 4
  br label %endif42

endif42:                                          ; preds = %then43, %endif36
  %a49 = load i32, i32* %a, align 4
  %EQ50 = icmp eq i32 %a49, 4
  br i1 %EQ50, label %then48, label %endif47

then48:                                           ; preds = %endif42
  %n51 = load i32, i32* %n, align 4
  %ADD52 = add i32 %n51, 10
  store i32 %ADD52, i32* %n, align 4
  br label %endif47

endif47:                                          ; preds = %then48, %endif42
  %a55 = load i32, i32* %a, align 4
  %NQ = icmp ne i32 %a55, 4
  br i1 %NQ, label %then54, label %endif53

then54:                                           ; preds = %endif47
  %n56 = load i32, i32* %n, align 4
  %ADD57 = add i32 %n56, 1
  store i32 %ADD57, i32* %n, align 4
  br label %endif53

endif53:                                          ; preds = %then54, %endif47
  %a60 = load i32, i32* %a, align 4
  %NQ61 = icmp ne i32 %a60, 5
  br i1 %NQ61, label %then59, label %endif58

then59:                                           ; preds = %endif53
  %n62 = load i32, i32* %n, align 4
  %ADD63 = add i32 %n62, 10
  store i32 %ADD63, i32* %n, align 4
  br label %endif58

endif58:                                          ; preds = %then59, %endif53
  %n64 = load i32, i32* %n, align 4
  ret i32 %n64
}
```

---

## 004 — if with a ref

```ura
fn main() i32:
    b b1 = False
    p &b1 = &b
    p = True
    if b:
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
  %b = alloca i1, align 1
  %p = alloca i1*, align 8
  store i1 false, i1* %b, align 1
  store i1* %b, i1** %p, align 8
  %p1 = load i1*, i1** %p, align 8
  store i1 true, i1* %p1, align 1
  %b2 = load i1, i1* %b, align 1
  br i1 %b2, label %then, label %endif

then:                                             ; preds = %entry
  ret i32 1

endif:                                            ; preds = %entry
  ret i32 0
}
```

---

## 005 — nested ifs

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
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 5, i32* %a, align 4
  store i32 7, i32* %b, align 4
  %a1 = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %a1, 5
  br i1 %EQ, label %then, label %endif

then:                                             ; preds = %entry
  %b4 = load i32, i32* %b, align 4
  %EQ5 = icmp eq i32 %b4, 7
  br i1 %EQ5, label %then3, label %endif2

then3:                                            ; preds = %then
  ret i32 1

endif2:                                           ; preds = %then
  ret i32 2

endif:                                            ; preds = %entry
  ret i32 3
}
```

---

## 006 — logic operators and/or

```ura
fn main() i32:
   a i32 = 5
   if a > 1 and a < 10:
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
  %a = alloca i32, align 4
  store i32 5, i32* %a, align 4
  %a1 = load i32, i32* %a, align 4
  %GT = icmp sgt i32 %a1, 1
  %a2 = load i32, i32* %a, align 4
  %LT = icmp slt i32 %a2, 10
  %AND = and i1 %GT, %LT
  br i1 %AND, label %then, label %endif

then:                                             ; preds = %entry
  ret i32 1

endif:                                            ; preds = %entry
  ret i32 0
}
```

