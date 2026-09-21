# structs

## index

- 001 — init a struct
- 002 — modify struct attribute
- 003 — read struct attribute
- 004 — struct through a ref parameter

---

## 001 — init a struct

```ura
struct User:
   a i32
   b i32

fn main() i32:
   c i32
   u User
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32, i32 }

define i32 @main() {
entry:
  %c = alloca i32, align 4
  %u = alloca %User, align 8
  ret i32 0
}
```

---

## 002 — modify struct attribute

```ura
struct User:
   a i32
   b i32

fn main() i32:
   c i32
   u User
   u.b = 10
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32, i32 }

define i32 @main() {
entry:
  %c = alloca i32, align 4
  %u = alloca %User, align 8
  %b = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  store i32 10, i32* %b, align 4
  ret i32 0
}
```

---

## 003 — read struct attribute

```ura
struct User:
   a i32
   b i32

fn main() i32:
   u User
   u.b = 7
   return u.b
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32, i32 }

define i32 @main() {
entry:
  %u = alloca %User, align 8
  %b = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  store i32 7, i32* %b, align 4
  %b1 = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  %b2 = load i32, i32* %b1, align 4
  ret i32 %b2
}
```

---

## 004 — struct through a ref parameter

```ura
struct User:
   a i32

fn set(p ref(User)):
   dref(p).a = 3

fn main() i32:
   u User
   set(own(u))
   return u.a
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32 }

define void @set(%User* %0) {
entry:
  %p = alloca %User*, align 8
  store %User* %0, %User** %p, align 8
  %p1 = load %User*, %User** %p, align 8
  %a = getelementptr inbounds %User, %User* %p1, i32 0, i32 0
  store i32 3, i32* %a, align 4
  ret void
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  call void @set(%User* %u)
  %a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %a1 = load i32, i32* %a, align 4
  ret i32 %a1
}
```

