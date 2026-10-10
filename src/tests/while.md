# while

## index

- 001 — basic while loop
- 002 — break
- 003 — continue
- 004 — nested whiles
- 005 — get crazy

---

## 001 — basic while loop

```ura
fn main() i32:
    a i32 = 0
    while a < 10:
        a = a + 1
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
  br label %cond

cond:                                             ; preds = %then, %entry
  %a1 = load i32, i32* %a, align 4
  %LT = icmp slt i32 %a1, 10
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %a2 = load i32, i32* %a, align 4
  %ADD = add i32 %a2, 1
  store i32 %ADD, i32* %a, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  %a3 = load i32, i32* %a, align 4
  ret i32 %a3
}
```

---

## 002 — break

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
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 0, i32* %a, align 4
  br label %cond

cond:                                             ; preds = %endif, %entry
  %a1 = load i32, i32* %a, align 4
  %LT = icmp slt i32 %a1, 10
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %a3 = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %a3, 3
  br i1 %EQ, label %then2, label %endif

endwhile:                                         ; preds = %then2, %cond
  %a5 = load i32, i32* %a, align 4
  ret i32 %a5

then2:                                            ; preds = %then
  br label %endwhile

endif:                                            ; preds = %then
  %a4 = load i32, i32* %a, align 4
  %ADD = add i32 %a4, 1
  store i32 %ADD, i32* %a, align 4
  br label %cond
}
```

---

## 003 — continue

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
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 0, i32* %a, align 4
  br label %cond

cond:                                             ; preds = %endif, %then2, %entry
  %a1 = load i32, i32* %a, align 4
  %LT = icmp slt i32 %a1, 10
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %a3 = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %a3, 3
  br i1 %EQ, label %then2, label %endif

endwhile:                                         ; preds = %cond
  %a7 = load i32, i32* %a, align 4
  ret i32 %a7

then2:                                            ; preds = %then
  %a4 = load i32, i32* %a, align 4
  %ADD = add i32 %a4, 2
  store i32 %ADD, i32* %a, align 4
  br label %cond

endif:                                            ; preds = %then
  %a5 = load i32, i32* %a, align 4
  %ADD6 = add i32 %a5, 1
  store i32 %ADD6, i32* %a, align 4
  br label %cond
}
```


---

## 004 — nested whiles

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
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %i = alloca i32, align 4
  %c = alloca i32, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %c, align 4
  br label %cond

cond:                                             ; preds = %endwhile4, %entry
  %i1 = load i32, i32* %i, align 4
  %LT = icmp slt i32 %i1, 3
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  br label %cond2

endwhile:                                         ; preds = %cond
  %c8 = load i32, i32* %c, align 4
  ret i32 %c8

cond2:                                            ; preds = %then
  br i1 true, label %then3, label %endwhile4

then3:                                            ; preds = %cond2
  %c5 = load i32, i32* %c, align 4
  %ADD = add i32 %c5, 1
  store i32 %ADD, i32* %c, align 4
  br label %endwhile4

endwhile4:                                        ; preds = %then3, %cond2
  %i6 = load i32, i32* %i, align 4
  %ADD7 = add i32 %i6, 1
  store i32 %ADD7, i32* %i, align 4
  br label %cond
}
```

---

## 005 — get crazy

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
; ModuleID = 'ura-module'
source_filename = "ura-module"
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

cond:                                             ; preds = %endwhile4, %entry
  %i1 = load i32, i32* %i, align 4
  %LT = icmp slt i32 %i1, 3
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  store i32 0, i32* %j, align 4
  br label %cond2

endwhile:                                         ; preds = %cond
  %c12 = load i32, i32* %c, align 4
  ret i32 %c12

cond2:                                            ; preds = %then3, %then
  %j5 = load i32, i32* %j, align 4
  %LT6 = icmp slt i32 %j5, 4
  br i1 %LT6, label %then3, label %endwhile4

then3:                                            ; preds = %cond2
  %c7 = load i32, i32* %c, align 4
  %ADD = add i32 %c7, 1
  store i32 %ADD, i32* %c, align 4
  %j8 = load i32, i32* %j, align 4
  %ADD9 = add i32 %j8, 1
  store i32 %ADD9, i32* %j, align 4
  br label %cond2

endwhile4:                                        ; preds = %cond2
  %i10 = load i32, i32* %i, align 4
  %ADD11 = add i32 %i10, 1
  store i32 %ADD11, i32* %i, align 4
  br label %cond
}
```

