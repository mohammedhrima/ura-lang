# bool

## index

- 001 — a bool variable
- 002 — False skips the branch
- 003 — a comparison stored in a bool, reassigned
- 004 — bool parameters and return values
- 005 — or between bool variables
- 006 — a bool as a while condition
- 007 — a bool default value is False
- 008 — boolean type (from tests-old/data_types.md #005)

---

## 001 — a bool variable

```ura
// expect: 1
fn main() i32:
    flag bool = True
    if flag:
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
  %flag = alloca i1, align 1
  store i1 true, i1* %flag, align 1
  %load.flag = load i1, i1* %flag, align 1
  br i1 %load.flag, label %then, label %endif

then:                                             ; preds = %entry
  ret i32 1

endif:                                            ; preds = %entry
  ret i32 0
}
```

---

## 002 — False skips the branch

```ura
// expect: 0
fn main() i32:
    flag bool = False
    if flag:
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
  %flag = alloca i1, align 1
  store i1 false, i1* %flag, align 1
  %load.flag = load i1, i1* %flag, align 1
  br i1 %load.flag, label %then, label %endif

then:                                             ; preds = %entry
  ret i32 1

endif:                                            ; preds = %entry
  ret i32 0
}
```

---

## 003 — a comparison stored in a bool, reassigned

```ura
// expect: 2
fn main() i32:
    a i32 = 5
    big bool = a > 3
    n i32 = 0
    if big:
        n += 1
    big = a > 9
    if big:
        n += 10
    big = False
    if big:
        n += 100
    if big == False:
        n += 1
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
  %big = alloca i1, align 1
  %n = alloca i32, align 4
  store i32 5, i32* %a, align 4
  %load.a = load i32, i32* %a, align 4
  %GT = icmp sgt i32 %load.a, 3
  store i1 %GT, i1* %big, align 1
  store i32 0, i32* %n, align 4
  %load.big = load i1, i1* %big, align 1
  br i1 %load.big, label %then, label %endif

then:                                             ; preds = %entry
  %old = load i32, i32* %n, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %load.a1 = load i32, i32* %a, align 4
  %GT2 = icmp sgt i32 %load.a1, 9
  store i1 %GT2, i1* %big, align 1
  %load.big5 = load i1, i1* %big, align 1
  br i1 %load.big5, label %then4, label %endif3

then4:                                            ; preds = %endif
  %old6 = load i32, i32* %n, align 4
  %ADD_ASSIGN7 = add i32 %old6, 10
  store i32 %ADD_ASSIGN7, i32* %n, align 4
  br label %endif3

endif3:                                           ; preds = %then4, %endif
  store i1 false, i1* %big, align 1
  %load.big10 = load i1, i1* %big, align 1
  br i1 %load.big10, label %then9, label %endif8

then9:                                            ; preds = %endif3
  %old11 = load i32, i32* %n, align 4
  %ADD_ASSIGN12 = add i32 %old11, 100
  store i32 %ADD_ASSIGN12, i32* %n, align 4
  br label %endif8

endif8:                                           ; preds = %then9, %endif3
  %load.big15 = load i1, i1* %big, align 1
  %EQ = icmp eq i1 %load.big15, false
  br i1 %EQ, label %then14, label %endif13

then14:                                           ; preds = %endif8
  %old16 = load i32, i32* %n, align 4
  %ADD_ASSIGN17 = add i32 %old16, 1
  store i32 %ADD_ASSIGN17, i32* %n, align 4
  br label %endif13

endif13:                                          ; preds = %then14, %endif8
  %load.n = load i32, i32* %n, align 4
  ret i32 %load.n
}
```

---

## 004 — bool parameters and return values

```ura
// expect: 3
fn is_even(n i32) bool:
    return n % 2 == 0

fn both(a bool, b bool) bool:
    return a and b

fn main() i32:
    n i32 = 0
    if is_even(4):
        n += 1
    if both(is_even(2), is_even(6)):
        n += 2
    if both(is_even(2), is_even(7)):
        n += 10
    return n
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i1 @is_even(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %load.n = load i32, i32* %n, align 4
  %MOD = srem i32 %load.n, 2
  %EQ = icmp eq i32 %MOD, 0
  ret i1 %EQ
}

define i1 @both(i1 %0, i1 %1) {
entry:
  %a = alloca i1, align 1
  %b = alloca i1, align 1
  store i1 %0, i1* %a, align 1
  store i1 %1, i1* %b, align 1
  %load.a = load i1, i1* %a, align 1
  %load.b = load i1, i1* %b, align 1
  %AND = and i1 %load.a, %load.b
  ret i1 %AND
}

define i32 @main() {
entry:
  %n = alloca i32, align 4
  store i32 0, i32* %n, align 4
  %"is_even(4)" = call i1 @is_even(i32 4)
  br i1 %"is_even(4)", label %then, label %endif

then:                                             ; preds = %entry
  %old = load i32, i32* %n, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %"is_even(2)" = call i1 @is_even(i32 2)
  %"is_even(6)" = call i1 @is_even(i32 6)
  %"both(is_even(2),is_even(6))" = call i1 @both(i1 %"is_even(2)", i1 %"is_even(6)")
  br i1 %"both(is_even(2),is_even(6))", label %then2, label %endif1

then2:                                            ; preds = %endif
  %old3 = load i32, i32* %n, align 4
  %ADD_ASSIGN4 = add i32 %old3, 2
  store i32 %ADD_ASSIGN4, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %then2, %endif
  %"is_even(2)7" = call i1 @is_even(i32 2)
  %"is_even(7)" = call i1 @is_even(i32 7)
  %"both(is_even(2),is_even(7))" = call i1 @both(i1 %"is_even(2)7", i1 %"is_even(7)")
  br i1 %"both(is_even(2),is_even(7))", label %then6, label %endif5

then6:                                            ; preds = %endif1
  %old8 = load i32, i32* %n, align 4
  %ADD_ASSIGN9 = add i32 %old8, 10
  store i32 %ADD_ASSIGN9, i32* %n, align 4
  br label %endif5

endif5:                                           ; preds = %then6, %endif1
  %load.n = load i32, i32* %n, align 4
  ret i32 %load.n
}
```

---

## 005 — or between bool variables

```ura
// expect: 5
fn main() i32:
    a bool = False
    b bool = True
    n i32 = 0
    if a or b:
        n += 5
    if a and b:
        n += 50
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
  %a = alloca i1, align 1
  %b = alloca i1, align 1
  %n = alloca i32, align 4
  store i1 false, i1* %a, align 1
  store i1 true, i1* %b, align 1
  store i32 0, i32* %n, align 4
  %load.a = load i1, i1* %a, align 1
  %load.b = load i1, i1* %b, align 1
  %OR = or i1 %load.a, %load.b
  br i1 %OR, label %then, label %endif

then:                                             ; preds = %entry
  %old = load i32, i32* %n, align 4
  %ADD_ASSIGN = add i32 %old, 5
  store i32 %ADD_ASSIGN, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %load.a3 = load i1, i1* %a, align 1
  %load.b4 = load i1, i1* %b, align 1
  %AND = and i1 %load.a3, %load.b4
  br i1 %AND, label %then2, label %endif1

then2:                                            ; preds = %endif
  %old5 = load i32, i32* %n, align 4
  %ADD_ASSIGN6 = add i32 %old5, 50
  store i32 %ADD_ASSIGN6, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %then2, %endif
  %load.n = load i32, i32* %n, align 4
  ret i32 %load.n
}
```

---

## 006 — a bool as a while condition

```ura
// expect: 4
fn main() i32:
    i i32 = 0
    going bool = True
    while going:
        i += 1
        going = i < 4
    return i
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %i = alloca i32, align 4
  %going = alloca i1, align 1
  store i32 0, i32* %i, align 4
  store i1 true, i1* %going, align 1
  br label %cond

cond:                                             ; preds = %then, %entry
  %load.going = load i1, i1* %going, align 1
  br i1 %load.going, label %then, label %endwhile

then:                                             ; preds = %cond
  %old = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 4
  store i1 %LT, i1* %going, align 1
  br label %cond

endwhile:                                         ; preds = %cond
  %load.i1 = load i32, i32* %i, align 4
  ret i32 %load.i1
}
```

---

## 007 — a bool default value is False

```ura
// expect: 7
fn main() i32:
    flag bool
    if flag:
        return 1
    return 7
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %flag = alloca i1, align 1
  store i1 false, i1* %flag, align 1
  %load.flag = load i1, i1* %flag, align 1
  br i1 %load.flag, label %then, label %endif

then:                                             ; preds = %entry
  ret i32 1

endif:                                            ; preds = %entry
  ret i32 7
}
```

---

## 008 — boolean type (from tests-old/data_types.md #005)

```ura
fn main() i32:
    flag bool = True
    if flag:
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
  %flag = alloca i1, align 1
  store i1 true, i1* %flag, align 1
  %load.flag = load i1, i1* %flag, align 1
  br i1 %load.flag, label %then, label %endif

then:                                             ; preds = %entry
  ret i32 1

endif:                                            ; preds = %entry
  ret i32 0
}
```

