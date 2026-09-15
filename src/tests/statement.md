# statement

## index

- 001 — basic if
- 002 — basic if/elif/else chain

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

