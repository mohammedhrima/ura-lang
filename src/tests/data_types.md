# data_types

## index

- 001 — declare variable

---

## 001 — declare variable

```ura
// pointer syntax
// p ref(i32) = own(a)

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

