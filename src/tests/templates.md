# templates

## index

- 001 — struct with a type parameter
- 002 — method that takes the type parameter
- 003 — two type parameters
- 004 — two instances of the same template
- 005 — a template inside itself
- 006 — proto inside a template keeps its symbol
- 007 — function called with type arguments
- 008 — sizeof of the type parameter

---

## 001 — struct with a type parameter

```ura
template<T>:
   struct Box:
      value T

fn main() i32:
   b Box<i32>
   b.value = 7
   return b.value
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%"Box<i32>" = type { i32 }

define i32 @main() {
entry:
  %b = alloca %"Box<i32>", align 8
  %b.value = getelementptr inbounds %"Box<i32>", %"Box<i32>"* %b, i32 0, i32 0
  store i32 7, i32* %b.value, align 4
  %b.value1 = getelementptr inbounds %"Box<i32>", %"Box<i32>"* %b, i32 0, i32 0
  %b.value2 = load i32, i32* %b.value1, align 4
  ret i32 %b.value2
}
```

---

## 002 — method that takes the type parameter

```ura
template<T>:
   struct Box:
      value T
      fn set(v T):
         self.value = v

fn main() i32:
   b Box<i32>
   b.set(20)
   return b.value
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%"Box<i32>" = type { i32 }

define void @Box.set(%"Box<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Box<i32>"*, align 8
  %v = alloca i32, align 4
  store %"Box<i32>"* %0, %"Box<i32>"** %self, align 8
  store i32 %1, i32* %v, align 4
  %v1 = load i32, i32* %v, align 4
  %self2 = load %"Box<i32>"*, %"Box<i32>"** %self, align 8
  %self.value = getelementptr inbounds %"Box<i32>", %"Box<i32>"* %self2, i32 0, i32 0
  store i32 %v1, i32* %self.value, align 4
  ret void
}

define i32 @main() {
entry:
  %b = alloca %"Box<i32>", align 8
  call void @Box.set(%"Box<i32>"* %b, i32 20)
  %b.value = getelementptr inbounds %"Box<i32>", %"Box<i32>"* %b, i32 0, i32 0
  %b.value1 = load i32, i32* %b.value, align 4
  ret i32 %b.value1
}
```

---

## 003 — two type parameters

```ura
template<K, V>:
   struct Pair:
      key K
      value V

fn main() i32:
   p Pair<i8, i32>
   p.key = 'z'
   p.value = 5
   return p.value
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%"Pair<i8,i32>" = type { i8, i32 }

define i32 @main() {
entry:
  %p = alloca %"Pair<i8,i32>", align 8
  %p.key = getelementptr inbounds %"Pair<i8,i32>", %"Pair<i8,i32>"* %p, i32 0, i32 0
  store i8 122, i8* %p.key, align 1
  %p.value = getelementptr inbounds %"Pair<i8,i32>", %"Pair<i8,i32>"* %p, i32 0, i32 1
  store i32 5, i32* %p.value, align 4
  %p.value1 = getelementptr inbounds %"Pair<i8,i32>", %"Pair<i8,i32>"* %p, i32 0, i32 1
  %p.value2 = load i32, i32* %p.value1, align 4
  ret i32 %p.value2
}
```

---

## 004 — two instances of the same template

```ura
template<T>:
   struct Box:
      value T

fn main() i32:
   a Box<i32>
   b Box<i8>
   a.value = 3
   b.value = 'a'
   return a.value
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%"Box<i32>" = type { i32 }
%"Box<i8>" = type { i8 }

define i32 @main() {
entry:
  %a = alloca %"Box<i32>", align 8
  %b = alloca %"Box<i8>", align 8
  %a.value = getelementptr inbounds %"Box<i32>", %"Box<i32>"* %a, i32 0, i32 0
  store i32 3, i32* %a.value, align 4
  %b.value = getelementptr inbounds %"Box<i8>", %"Box<i8>"* %b, i32 0, i32 0
  store i8 97, i8* %b.value, align 1
  %a.value1 = getelementptr inbounds %"Box<i32>", %"Box<i32>"* %a, i32 0, i32 0
  %a.value2 = load i32, i32* %a.value1, align 4
  ret i32 %a.value2
}
```

---

## 005 — a template inside itself

```ura
template<T>:
   struct Box:
      value T

fn main() i32:
   n Box<Box<i32>>
   n.value.value = 9
   return n.value.value
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%"Box<Box<i32>>" = type { %"Box<i32>" }
%"Box<i32>" = type { i32 }

define i32 @main() {
entry:
  %n = alloca %"Box<Box<i32>>", align 8
  %n.value = getelementptr inbounds %"Box<Box<i32>>", %"Box<Box<i32>>"* %n, i32 0, i32 0
  %n.value.value = getelementptr inbounds %"Box<i32>", %"Box<i32>"* %n.value, i32 0, i32 0
  store i32 9, i32* %n.value.value, align 4
  %n.value1 = getelementptr inbounds %"Box<Box<i32>>", %"Box<Box<i32>>"* %n, i32 0, i32 0
  %n.value.value2 = getelementptr inbounds %"Box<i32>", %"Box<i32>"* %n.value1, i32 0, i32 0
  %n.value.value3 = load i32, i32* %n.value.value2, align 4
  ret i32 %n.value.value3
}
```

---

## 006 — proto inside a template keeps its symbol

```ura
template<T>:
   proto malloc(size i32) T[]

   struct Vec:
      arr T[]
      fn init():
         self.arr = malloc(40)

fn main() i32:
   v Vec<i32>
   v.init()
   return 0
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%"Vec<i32>" = type { i32* }

declare i32* @malloc(i32)

define void @Vec.init(%"Vec<i32>"* %0) {
entry:
  %self = alloca %"Vec<i32>"*, align 8
  store %"Vec<i32>"* %0, %"Vec<i32>"** %self, align 8
  %"malloc<i32>" = call i32* @malloc(i32 40)
  %self1 = load %"Vec<i32>"*, %"Vec<i32>"** %self, align 8
  %self.arr = getelementptr inbounds %"Vec<i32>", %"Vec<i32>"* %self1, i32 0, i32 0
  store i32* %"malloc<i32>", i32** %self.arr, align 8
  ret void
}

define i32 @main() {
entry:
  %v = alloca %"Vec<i32>", align 8
  call void @Vec.init(%"Vec<i32>"* %v)
  ret i32 0
}
```

---

## 007 — function called with type arguments

```ura
template<T>:
   fn twice(v T) T:
      return v + v

fn main() i32:
   a i32 = twice<i32>(5)
   b i8 = twice<i8>(3)
   c i32 = 0
   if b == 6:
      c = 1
   return a + c
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @"twice<i32>"(i32 %0) {
entry:
  %v = alloca i32, align 4
  store i32 %0, i32* %v, align 4
  %v1 = load i32, i32* %v, align 4
  %v2 = load i32, i32* %v, align 4
  %ADD = add i32 %v1, %v2
  ret i32 %ADD
}

define i8 @"twice<i8>"(i8 %0) {
entry:
  %v = alloca i8, align 1
  store i8 %0, i8* %v, align 1
  %v1 = load i8, i8* %v, align 1
  %v2 = load i8, i8* %v, align 1
  %ADD = add i8 %v1, %v2
  ret i8 %ADD
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i8, align 1
  %c = alloca i32, align 4
  %"twice<i32>" = call i32 @"twice<i32>"(i32 5)
  store i32 %"twice<i32>", i32* %a, align 4
  %"twice<i8>" = call i8 @"twice<i8>"(i8 3)
  store i8 %"twice<i8>", i8* %b, align 1
  store i32 0, i32* %c, align 4
  %b1 = load i8, i8* %b, align 1
  %EQ = icmp eq i8 %b1, 6
  br i1 %EQ, label %then, label %endif

then:                                             ; preds = %entry
  store i32 1, i32* %c, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %a2 = load i32, i32* %a, align 4
  %c3 = load i32, i32* %c, align 4
  %ADD = add i32 %a2, %c3
  ret i32 %ADD
}
```

---

## 008 — sizeof of the type parameter

```ura
template<T>:
   proto malloc(size i32) T[]

   struct Vec:
      arr T[]
      fn init():
         self.arr = malloc(sizeof(T) * 10)

fn main() i32:
   x Vec<i32>
   x.init()
   x.arr[3] = 7
   return x.arr[3]
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%"Vec<i32>" = type { i32* }

declare i32* @malloc(i32)

define void @Vec.init(%"Vec<i32>"* %0) {
entry:
  %self = alloca %"Vec<i32>"*, align 8
  store %"Vec<i32>"* %0, %"Vec<i32>"** %self, align 8
  %"malloc<i32>" = call i32* @malloc(i32 40)
  %self1 = load %"Vec<i32>"*, %"Vec<i32>"** %self, align 8
  %self.arr = getelementptr inbounds %"Vec<i32>", %"Vec<i32>"* %self1, i32 0, i32 0
  store i32* %"malloc<i32>", i32** %self.arr, align 8
  ret void
}

define i32 @main() {
entry:
  %x = alloca %"Vec<i32>", align 8
  call void @Vec.init(%"Vec<i32>"* %x)
  %x.arr = getelementptr inbounds %"Vec<i32>", %"Vec<i32>"* %x, i32 0, i32 0
  %x.arr1 = load i32*, i32** %x.arr, align 8
  %0 = getelementptr inbounds i32, i32* %x.arr1, i32 3
  store i32 7, i32* %0, align 4
  %x.arr2 = getelementptr inbounds %"Vec<i32>", %"Vec<i32>"* %x, i32 0, i32 0
  %x.arr3 = load i32*, i32** %x.arr2, align 8
  %1 = getelementptr inbounds i32, i32* %x.arr3, i32 3
  %2 = load i32, i32* %1, align 4
  ret i32 %2
}
```

