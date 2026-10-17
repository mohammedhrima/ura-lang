# while

## index

- 001 — count to 10
- 002 — break
- 003 — continue
- 004 — nested loops
- 005 — break leaves only the inner loop
- 006 — continue in the inner loop
- 007 — return from inside a loop
- 008 — loop that never runs
- 009 — loop in a function, called from main
- 010 — basic while loop (from tests-old/while.md #001)
- 011 — break (from tests-old/while.md #002)
- 012 — continue (from tests-old/while.md #003)
- 013 — nested whiles (from tests-old/while.md #004)
- 014 — get crazy (from tests-old/while.md #005)

---

## 001 — count to 10

```ura
// expect: 10
fn main() i32:
    a i32 = 0
    while a < 10:
        a += 1
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
  br label %cond

cond:                                             ; preds = %then, %entry
  %load.a = load i32, i32* %a, align 4
  %LT = icmp slt i32 %load.a, 10
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %old = load i32, i32* %a, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %a, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  %load.a1 = load i32, i32* %a, align 4
  ret i32 %load.a1
}
```

---

## 002 — break

```ura
// expect: 3
fn main() i32:
    a i32 = 0
    while a < 10:
        if a == 3:
            break
        a += 1
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
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.a = load i32, i32* %a, align 4
  %LT = icmp slt i32 %load.a, 10
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.a2 = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %load.a2, 3
  br i1 %EQ, label %then1, label %endif

endwhile:                                         ; preds = %then1, %cond
  %load.a3 = load i32, i32* %a, align 4
  ret i32 %load.a3

then1:                                            ; preds = %then
  br label %endwhile

endif:                                            ; preds = %then
  %old = load i32, i32* %a, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %a, align 4
  br label %cond
}
```

---

## 003 — continue

```ura
// expect: 25   (1 + 3 + 5 + 7 + 9)
fn main() i32:
    i i32 = 0
    sum i32 = 0
    while i < 10:
        i += 1
        if i % 2 == 0:
            continue
        sum += i
    return sum
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
  %sum = alloca i32, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %sum, align 4
  br label %cond

cond:                                             ; preds = %endif, %then1, %entry
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 10
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %old = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  %load.i2 = load i32, i32* %i, align 4
  %MOD = srem i32 %load.i2, 2
  %EQ = icmp eq i32 %MOD, 0
  br i1 %EQ, label %then1, label %endif

endwhile:                                         ; preds = %cond
  %load.sum = load i32, i32* %sum, align 4
  ret i32 %load.sum

then1:                                            ; preds = %then
  br label %cond

endif:                                            ; preds = %then
  %load.i3 = load i32, i32* %i, align 4
  %old4 = load i32, i32* %sum, align 4
  %ADD_ASSIGN5 = add i32 %old4, %load.i3
  store i32 %ADD_ASSIGN5, i32* %sum, align 4
  br label %cond
}
```

---

## 004 — nested loops

```ura
// expect: 12   (3 x 4)
fn main() i32:
    i i32 = 0
    c i32 = 0
    while i < 3:
        j i32 = 0
        while j < 4:
            c += 1
            j += 1
        i += 1
    return c
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
  %c = alloca i32, align 4
  %j = alloca i32, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %c, align 4
  br label %cond

cond:                                             ; preds = %endwhile3, %entry
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 3
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  store i32 0, i32* %j, align 4
  br label %cond1

endwhile:                                         ; preds = %cond
  %load.c = load i32, i32* %c, align 4
  ret i32 %load.c

cond1:                                            ; preds = %then2, %then
  %load.j = load i32, i32* %j, align 4
  %LT4 = icmp slt i32 %load.j, 4
  br i1 %LT4, label %then2, label %endwhile3

then2:                                            ; preds = %cond1
  %old = load i32, i32* %c, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %c, align 4
  %old5 = load i32, i32* %j, align 4
  %ADD_ASSIGN6 = add i32 %old5, 1
  store i32 %ADD_ASSIGN6, i32* %j, align 4
  br label %cond1

endwhile3:                                        ; preds = %cond1
  %old7 = load i32, i32* %i, align 4
  %ADD_ASSIGN8 = add i32 %old7, 1
  store i32 %ADD_ASSIGN8, i32* %i, align 4
  br label %cond
}
```

---

## 005 — break leaves only the inner loop

```ura
// expect: 3
fn main() i32:
    i i32 = 0
    c i32 = 0
    while i < 3:
        while c < 100:
            c += 1
            break
        i += 1
    return c
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
  %c = alloca i32, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %c, align 4
  br label %cond

cond:                                             ; preds = %endwhile3, %entry
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 3
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  br label %cond1

endwhile:                                         ; preds = %cond
  %load.c7 = load i32, i32* %c, align 4
  ret i32 %load.c7

cond1:                                            ; preds = %then
  %load.c = load i32, i32* %c, align 4
  %LT4 = icmp slt i32 %load.c, 100
  br i1 %LT4, label %then2, label %endwhile3

then2:                                            ; preds = %cond1
  %old = load i32, i32* %c, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %c, align 4
  br label %endwhile3

endwhile3:                                        ; preds = %then2, %cond1
  %old5 = load i32, i32* %i, align 4
  %ADD_ASSIGN6 = add i32 %old5, 1
  store i32 %ADD_ASSIGN6, i32* %i, align 4
  br label %cond
}
```

---

## 006 — continue in the inner loop

```ura
// expect: 6   (j = 1 and j = 3, three times)
fn main() i32:
    i i32 = 0
    c i32 = 0
    while i < 3:
        j i32 = 0
        while j < 4:
            j += 1
            if j % 2 == 0:
                continue
            c += 1
        i += 1
    return c
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
  %c = alloca i32, align 4
  %j = alloca i32, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %c, align 4
  br label %cond

cond:                                             ; preds = %endwhile3, %entry
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 3
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  store i32 0, i32* %j, align 4
  br label %cond1

endwhile:                                         ; preds = %cond
  %load.c = load i32, i32* %c, align 4
  ret i32 %load.c

cond1:                                            ; preds = %endif, %then5, %then
  %load.j = load i32, i32* %j, align 4
  %LT4 = icmp slt i32 %load.j, 4
  br i1 %LT4, label %then2, label %endwhile3

then2:                                            ; preds = %cond1
  %old = load i32, i32* %j, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %j, align 4
  %load.j6 = load i32, i32* %j, align 4
  %MOD = srem i32 %load.j6, 2
  %EQ = icmp eq i32 %MOD, 0
  br i1 %EQ, label %then5, label %endif

endwhile3:                                        ; preds = %cond1
  %old9 = load i32, i32* %i, align 4
  %ADD_ASSIGN10 = add i32 %old9, 1
  store i32 %ADD_ASSIGN10, i32* %i, align 4
  br label %cond

then5:                                            ; preds = %then2
  br label %cond1

endif:                                            ; preds = %then2
  %old7 = load i32, i32* %c, align 4
  %ADD_ASSIGN8 = add i32 %old7, 1
  store i32 %ADD_ASSIGN8, i32* %c, align 4
  br label %cond1
}
```

---

## 007 — return from inside a loop

```ura
// expect: 7
fn main() i32:
    i i32 = 0
    while i < 100:
        if i == 7:
            return i
        i += 1
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
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 100
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.i2 = load i32, i32* %i, align 4
  %EQ = icmp eq i32 %load.i2, 7
  br i1 %EQ, label %then1, label %endif

endwhile:                                         ; preds = %cond
  ret i32 0

then1:                                            ; preds = %then
  %load.i3 = load i32, i32* %i, align 4
  ret i32 %load.i3

endif:                                            ; preds = %then
  %old = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  br label %cond
}
```

---

## 008 — loop that never runs

```ura
// expect: 5
fn main() i32:
    a i32 = 5
    while a < 0:
        a = 100
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
  store i32 5, i32* %a, align 4
  br label %cond

cond:                                             ; preds = %then, %entry
  %load.a = load i32, i32* %a, align 4
  %LT = icmp slt i32 %load.a, 0
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  store i32 100, i32* %a, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  %load.a1 = load i32, i32* %a, align 4
  ret i32 %load.a1
}
```

---

## 009 — loop in a function, called from main

```ura
// expect: 55   (1 + 2 + ... + 10)
fn sum_to(n i32) i32:
    total i32 = 0
    i i32 = 1
    while i <= n:
        total += i
        i += 1
    return total

fn main() i32:
    return sum_to(10)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @sum_to(i32 %0) {
entry:
  %n = alloca i32, align 4
  %total = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  store i32 0, i32* %total, align 4
  store i32 1, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %then, %entry
  %load.i = load i32, i32* %i, align 4
  %load.n = load i32, i32* %n, align 4
  %LE = icmp sle i32 %load.i, %load.n
  br i1 %LE, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.i1 = load i32, i32* %i, align 4
  %old = load i32, i32* %total, align 4
  %ADD_ASSIGN = add i32 %old, %load.i1
  store i32 %ADD_ASSIGN, i32* %total, align 4
  %old2 = load i32, i32* %i, align 4
  %ADD_ASSIGN3 = add i32 %old2, 1
  store i32 %ADD_ASSIGN3, i32* %i, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  %load.total = load i32, i32* %total, align 4
  ret i32 %load.total
}

define i32 @main() {
entry:
  %"sum_to(10)" = call i32 @sum_to(i32 10)
  ret i32 %"sum_to(10)"
}
```

---

## 010 — basic while loop (from tests-old/while.md #001)

```ura
fn main() i32:
    a i32 = 0
    while a < 10:
        a = a + 1
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
  br label %cond

cond:                                             ; preds = %then, %entry
  %load.a = load i32, i32* %a, align 4
  %LT = icmp slt i32 %load.a, 10
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.a1 = load i32, i32* %a, align 4
  %ADD = add i32 %load.a1, 1
  store i32 %ADD, i32* %a, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  %load.a2 = load i32, i32* %a, align 4
  ret i32 %load.a2
}
```

---

## 011 — break (from tests-old/while.md #002)

```ura
fn main() i32:
    a i32 = 0
    while a < 10:
        if a == 3:
            break
        a = a + 1
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
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.a = load i32, i32* %a, align 4
  %LT = icmp slt i32 %load.a, 10
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.a2 = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %load.a2, 3
  br i1 %EQ, label %then1, label %endif

endwhile:                                         ; preds = %then1, %cond
  %load.a4 = load i32, i32* %a, align 4
  ret i32 %load.a4

then1:                                            ; preds = %then
  br label %endwhile

endif:                                            ; preds = %then
  %load.a3 = load i32, i32* %a, align 4
  %ADD = add i32 %load.a3, 1
  store i32 %ADD, i32* %a, align 4
  br label %cond
}
```

---

## 012 — continue (from tests-old/while.md #003)

```ura
fn main() i32:
    a i32 = 0
    while a < 10:
        if a == 3:
            a = a + 2
            continue
        a = a + 1
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
  br label %cond

cond:                                             ; preds = %endif, %then1, %entry
  %load.a = load i32, i32* %a, align 4
  %LT = icmp slt i32 %load.a, 10
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.a2 = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %load.a2, 3
  br i1 %EQ, label %then1, label %endif

endwhile:                                         ; preds = %cond
  %load.a6 = load i32, i32* %a, align 4
  ret i32 %load.a6

then1:                                            ; preds = %then
  %load.a3 = load i32, i32* %a, align 4
  %ADD = add i32 %load.a3, 2
  store i32 %ADD, i32* %a, align 4
  br label %cond

endif:                                            ; preds = %then
  %load.a4 = load i32, i32* %a, align 4
  %ADD5 = add i32 %load.a4, 1
  store i32 %ADD5, i32* %a, align 4
  br label %cond
}
```

---

## 013 — nested whiles (from tests-old/while.md #004)

```ura
// break leaves only the innermost loop
// expect: 3
fn main() i32:
    i i32 = 0
    c i32 = 0
    while i < 3:
        while True: c = c + 1 break
        i = i + 1
    return c
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
  %c = alloca i32, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %c, align 4
  br label %cond

cond:                                             ; preds = %endwhile3, %entry
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 3
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  br label %cond1

endwhile:                                         ; preds = %cond
  %load.c6 = load i32, i32* %c, align 4
  ret i32 %load.c6

cond1:                                            ; preds = %then
  br i1 true, label %then2, label %endwhile3

then2:                                            ; preds = %cond1
  %load.c = load i32, i32* %c, align 4
  %ADD = add i32 %load.c, 1
  store i32 %ADD, i32* %c, align 4
  br label %endwhile3

endwhile3:                                        ; preds = %then2, %cond1
  %load.i4 = load i32, i32* %i, align 4
  %ADD5 = add i32 %load.i4, 1
  store i32 %ADD5, i32* %i, align 4
  br label %cond
}
```

---

## 014 — get crazy (from tests-old/while.md #005)

```ura
fn main() i32:
    i i32 = 0
    c i32 = 0
    while i < 3:
        j i32 = 0
        while j < 4:
            c = c + 1
            j = j + 1
        i = i + 1
    return c
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
  %c = alloca i32, align 4
  %j = alloca i32, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %c, align 4
  br label %cond

cond:                                             ; preds = %endwhile3, %entry
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 3
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  store i32 0, i32* %j, align 4
  br label %cond1

endwhile:                                         ; preds = %cond
  %load.c9 = load i32, i32* %c, align 4
  ret i32 %load.c9

cond1:                                            ; preds = %then2, %then
  %load.j = load i32, i32* %j, align 4
  %LT4 = icmp slt i32 %load.j, 4
  br i1 %LT4, label %then2, label %endwhile3

then2:                                            ; preds = %cond1
  %load.c = load i32, i32* %c, align 4
  %ADD = add i32 %load.c, 1
  store i32 %ADD, i32* %c, align 4
  %load.j5 = load i32, i32* %j, align 4
  %ADD6 = add i32 %load.j5, 1
  store i32 %ADD6, i32* %j, align 4
  br label %cond1

endwhile3:                                        ; preds = %cond1
  %load.i7 = load i32, i32* %i, align 4
  %ADD8 = add i32 %load.i7, 1
  store i32 %ADD8, i32* %i, align 4
  br label %cond
}
```

