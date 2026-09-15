# while

## index

- 001 — basic while loop
- 002 — break
- 003 — continue

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

### stdout

```

```

### stderr

```

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

### stdout

```

```

### stderr

```

```

