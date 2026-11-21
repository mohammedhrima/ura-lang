# milestone04

## index

- 001 — template: struct with a type parameter
- 002 — template: method that takes the type parameter
- 003 — template: two type parameters
- 004 — template: two instances of the same template
- 005 — template: a template inside itself
- 006 — template: proto inside a template keeps its symbol
- 007 — template: function called with type arguments
- 008 — template: sizeof of the type parameter
- 009 — templates: too many type arguments
- 010 — templates: used without type arguments
- 011 — templates: parameter is not a name
- 012 — templates: body holds something else
- 013 — templates: missing colon
- 014 — template: declared inside a function
- 015 — templates: used outside the scope it is declared in
- 016 — template: several functions in one template, each instance renames all of them; memory_alloc, memory_copy, memory_free

---

## 001 — template: struct with a type parameter

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
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%"Box<i32>" = type { i32 }

define i32 @main() {
entry:
  %b = alloca %"Box<i32>", align 8
  store %"Box<i32>" zeroinitializer, %"Box<i32>"* %b, align 4
  %b.value = getelementptr inbounds %"Box<i32>", %"Box<i32>"* %b, i32 0, i32 0
  store i32 7, i32* %b.value, align 4
  %b.value1 = getelementptr inbounds %"Box<i32>", %"Box<i32>"* %b, i32 0, i32 0
  %"$b.value" = load i32, i32* %b.value1, align 4
  ret i32 %"$b.value"
}
```

---

## 002 — template: method that takes the type parameter

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
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%"Box<i32>" = type { i32 }

define void @"Box<i32>.set"(%"Box<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Box<i32>"*, align 8
  %v = alloca i32, align 4
  store %"Box<i32>"* %0, %"Box<i32>"** %self, align 8
  store i32 %1, i32* %v, align 4
  %"*self" = load %"Box<i32>"*, %"Box<i32>"** %self, align 8
  %"*self.value" = getelementptr inbounds %"Box<i32>", %"Box<i32>"* %"*self", i32 0, i32 0
  %"$v" = load i32, i32* %v, align 4
  store i32 %"$v", i32* %"*self.value", align 4
  ret void
}

define i32 @main() {
entry:
  %b = alloca %"Box<i32>", align 8
  store %"Box<i32>" zeroinitializer, %"Box<i32>"* %b, align 4
  call void @"Box<i32>.set"(%"Box<i32>"* %b, i32 20)
  %b.value = getelementptr inbounds %"Box<i32>", %"Box<i32>"* %b, i32 0, i32 0
  %"$b.value" = load i32, i32* %b.value, align 4
  ret i32 %"$b.value"
}
```

---

## 003 — template: two type parameters

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
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%"Pair<i8,i32>" = type { i8, i32 }

define i32 @main() {
entry:
  %p = alloca %"Pair<i8,i32>", align 8
  store %"Pair<i8,i32>" zeroinitializer, %"Pair<i8,i32>"* %p, align 4
  %p.key = getelementptr inbounds %"Pair<i8,i32>", %"Pair<i8,i32>"* %p, i32 0, i32 0
  store i8 122, i8* %p.key, align 1
  %p.value = getelementptr inbounds %"Pair<i8,i32>", %"Pair<i8,i32>"* %p, i32 0, i32 1
  store i32 5, i32* %p.value, align 4
  %p.value1 = getelementptr inbounds %"Pair<i8,i32>", %"Pair<i8,i32>"* %p, i32 0, i32 1
  %"$p.value" = load i32, i32* %p.value1, align 4
  ret i32 %"$p.value"
}
```

---

## 004 — template: two instances of the same template

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
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%"Box<i32>" = type { i32 }
%"Box<i8>" = type { i8 }

define i32 @main() {
entry:
  %a = alloca %"Box<i32>", align 8
  %b = alloca %"Box<i8>", align 8
  store %"Box<i32>" zeroinitializer, %"Box<i32>"* %a, align 4
  store %"Box<i8>" zeroinitializer, %"Box<i8>"* %b, align 1
  %a.value = getelementptr inbounds %"Box<i32>", %"Box<i32>"* %a, i32 0, i32 0
  store i32 3, i32* %a.value, align 4
  %b.value = getelementptr inbounds %"Box<i8>", %"Box<i8>"* %b, i32 0, i32 0
  store i8 97, i8* %b.value, align 1
  %a.value1 = getelementptr inbounds %"Box<i32>", %"Box<i32>"* %a, i32 0, i32 0
  %"$a.value" = load i32, i32* %a.value1, align 4
  ret i32 %"$a.value"
}
```

---

## 005 — template: a template inside itself

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
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%"Box<Box<i32>>" = type { %"Box<i32>" }
%"Box<i32>" = type { i32 }

define i32 @main() {
entry:
  %n = alloca %"Box<Box<i32>>", align 8
  store %"Box<Box<i32>>" zeroinitializer, %"Box<Box<i32>>"* %n, align 4
  %n.value = getelementptr inbounds %"Box<Box<i32>>", %"Box<Box<i32>>"* %n, i32 0, i32 0
  %n.value.value = getelementptr inbounds %"Box<i32>", %"Box<i32>"* %n.value, i32 0, i32 0
  store i32 9, i32* %n.value.value, align 4
  %n.value1 = getelementptr inbounds %"Box<Box<i32>>", %"Box<Box<i32>>"* %n, i32 0, i32 0
  %n.value.value2 = getelementptr inbounds %"Box<i32>", %"Box<i32>"* %n.value1, i32 0, i32 0
  %"$n.value.value" = load i32, i32* %n.value.value2, align 4
  ret i32 %"$n.value.value"
}
```

---

## 006 — template: proto inside a template keeps its symbol

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
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%"Vec<i32>" = type { i32* }

declare i32* @malloc(i32)

define void @"Vec<i32>.init"(%"Vec<i32>"* %0) {
entry:
  %self = alloca %"Vec<i32>"*, align 8
  store %"Vec<i32>"* %0, %"Vec<i32>"** %self, align 8
  %"*self" = load %"Vec<i32>"*, %"Vec<i32>"** %self, align 8
  %"*self.arr" = getelementptr inbounds %"Vec<i32>", %"Vec<i32>"* %"*self", i32 0, i32 0
  %call.malloc = call i32* @malloc(i32 40)
  store i32* %call.malloc, i32** %"*self.arr", align 8
  ret void
}

define i32 @main() {
entry:
  %v = alloca %"Vec<i32>", align 8
  store %"Vec<i32>" zeroinitializer, %"Vec<i32>"* %v, align 8
  call void @"Vec<i32>.init"(%"Vec<i32>"* %v)
  ret i32 0
}
```

---

## 007 — template: function called with type arguments

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
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @"twice<i32>"(i32 %0) {
entry:
  %v = alloca i32, align 4
  store i32 %0, i32* %v, align 4
  %"$v" = load i32, i32* %v, align 4
  %"$v1" = load i32, i32* %v, align 4
  %ADD = add i32 %"$v", %"$v1"
  ret i32 %ADD
}

define i8 @"twice<i8>"(i8 %0) {
entry:
  %v = alloca i8, align 1
  store i8 %0, i8* %v, align 1
  %"$v" = load i8, i8* %v, align 1
  %"$v1" = load i8, i8* %v, align 1
  %ADD = add i8 %"$v", %"$v1"
  ret i8 %ADD
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i8, align 1
  %c = alloca i32, align 4
  %"call.twice<i32>" = call i32 @"twice<i32>"(i32 5)
  store i32 %"call.twice<i32>", i32* %a, align 4
  %"call.twice<i8>" = call i8 @"twice<i8>"(i8 3)
  store i8 %"call.twice<i8>", i8* %b, align 1
  store i32 0, i32* %c, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$b" = load i8, i8* %b, align 1
  %EQ = icmp eq i8 %"$b", 6
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  store i32 1, i32* %c, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  %"$a" = load i32, i32* %a, align 4
  %"$c" = load i32, i32* %c, align 4
  %ADD = add i32 %"$a", %"$c"
  ret i32 %ADD
}
```

---

## 008 — template: sizeof of the type parameter

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
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%"Vec<i32>" = type { i32* }

declare i32* @malloc(i32)

define void @"Vec<i32>.init"(%"Vec<i32>"* %0) {
entry:
  %self = alloca %"Vec<i32>"*, align 8
  store %"Vec<i32>"* %0, %"Vec<i32>"** %self, align 8
  %"*self" = load %"Vec<i32>"*, %"Vec<i32>"** %self, align 8
  %"*self.arr" = getelementptr inbounds %"Vec<i32>", %"Vec<i32>"* %"*self", i32 0, i32 0
  %call.malloc = call i32* @malloc(i32 mul (i32 ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i32), i32 10))
  store i32* %call.malloc, i32** %"*self.arr", align 8
  ret void
}

define i32 @main() {
entry:
  %x = alloca %"Vec<i32>", align 8
  store %"Vec<i32>" zeroinitializer, %"Vec<i32>"* %x, align 8
  call void @"Vec<i32>.init"(%"Vec<i32>"* %x)
  %x.arr = getelementptr inbounds %"Vec<i32>", %"Vec<i32>"* %x, i32 0, i32 0
  %"$x.arr" = load i32*, i32** %x.arr, align 8
  %"x.arr[]" = getelementptr inbounds i32, i32* %"$x.arr", i32 3
  store i32 7, i32* %"x.arr[]", align 4
  %x.arr1 = getelementptr inbounds %"Vec<i32>", %"Vec<i32>"* %x, i32 0, i32 0
  %"$x.arr2" = load i32*, i32** %x.arr1, align 8
  %"x.arr[]3" = getelementptr inbounds i32, i32* %"$x.arr2", i32 3
  %"$x.arr[]" = load i32, i32* %"x.arr[]3", align 4
  ret i32 %"$x.arr[]"
}
```

---

## 009 — templates: too many type arguments

```ura
template<T>:
   struct Box:
      value T

fn main() i32:
   b Box<i32, i8>
   return 0
```

### stderr

```
error: test.ura:6:6 'Box' takes 1 type argument, got 2
  |
6 |    b Box<i32, i8>
  |      ^^^
help: it is declared 'template<T>'
error: aborting due to 1 error
```

### status

```
1
```

---

## 010 — templates: used without type arguments

```ura
template<T>:
   struct Box:
      value T

fn main() i32:
   b Box
   return 0
```

### stderr

```
error: test.ura:6:6 expected '<' after 'Box'
  |
6 |    b Box
  |      ^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 011 — templates: parameter is not a name

```ura
template<3>:
   struct Box:
      value i32

fn main() i32:
   return 0
```

### stderr

```
error: test.ura:1:10 expected names after template
  |
1 | template<3>:
  |          ^
help: use template<T, V>
error: aborting due to 1 error
```

### status

```
1
```

---

## 012 — templates: body holds something else

```ura
template<T>:
   value T

fn main() i32:
   return 0
```

### stderr

```
error: test.ura:2:4 expected struct/fn after template
  |
2 |    value T
  |    ^^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 013 — templates: missing colon

```ura
template<T>
   struct Box:
      value T

fn main() i32:
   return 0
```

### stderr

```
error: test.ura:1:11 expected ':' after 'template'
  |
1 | template<T>
  |           ^
help: use template<T>:
error: aborting due to 1 error
```

### status

```
1
```


---

## 014 — template: declared inside a function

```ura
fn main() i32:
    template<T>:
        struct Box:
            value T
    b Box<i32>
    b.value = 4
    return b.value
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%"Box<i32>" = type { i32 }

define i32 @main() {
entry:
  %b = alloca %"Box<i32>", align 8
  store %"Box<i32>" zeroinitializer, %"Box<i32>"* %b, align 4
  %b.value = getelementptr inbounds %"Box<i32>", %"Box<i32>"* %b, i32 0, i32 0
  store i32 4, i32* %b.value, align 4
  %b.value1 = getelementptr inbounds %"Box<i32>", %"Box<i32>"* %b, i32 0, i32 0
  %"$b.value" = load i32, i32* %b.value1, align 4
  ret i32 %"$b.value"
}
```

---

## 015 — templates: used outside the scope it is declared in

```ura
fn f() i32:
    template<T>:
        struct Box:
            value T
    return 0

fn main() i32:
    b Box<i32>
    return 0
```

### stderr

```
error: test.ura:8:7 no template named 'Box'
  |
8 |     b Box<i32>
  |       ^^^
help: declare it: template<T>: struct Box:
error: aborting due to 1 error
```

### status

```
1
```

---

## 016 — template: several functions in one template, each instance renames all of them; memory_alloc, memory_copy, memory_free

```ura
// stdout: 10 20 30
// stdout: 0 7
// expect: 0
template<T>:
    fn alloc(count i32) T[]:
        return memory_alloc(count * sizeof(T)) as T[]

    fn copy(dst T[], src T[], count i32):
        memory_copy(dst as i8[], src as i8[], count * sizeof(T))

    fn free(data T[]):
        memory_free(data as i8[])

struct P:
    x i32
    y i32

fn main() i32:
    a i32[] = alloc<i32>(3)
    b i32[] = alloc<i32>(3)
    a[0] = 10
    a[1] = 20
    a[2] = 30
    copy<i32>(b, a, 3)
    output(b[0], " ", b[1], " ", b[2], "\n")
    ps P[] = alloc<P>(2)
    ps[1].y = 7
    output(ps[0].x, " ", ps[1].y, "\n")
    free<i32>(a)
    free<i32>(b)
    free<P>(ps)
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%P = type { i32, i32 }

@str = private unnamed_addr constant [10 x i8] c"%d %d %d\0A\00", align 1
@str.1 = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1

define i32* @"alloc<i32>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to i32*
  ret i32* %AS1
}

define void @"copy<i32>"(i32* %0, i32* %1, i32 %2) {
entry:
  %dst = alloca i32*, align 8
  %src = alloca i32*, align 8
  %count = alloca i32, align 4
  store i32* %0, i32** %dst, align 8
  store i32* %1, i32** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i32*, i32** %dst, align 8
  %AS = bitcast i32* %"$dst" to i8*
  %"$src" = load i32*, i32** %src, align 8
  %AS1 = bitcast i32* %"$src" to i8*
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i32)
  %AS2 = sext i32 %MUL to i64
  %MUL3 = mul i64 %AS2, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %AS, i8* %AS1, i64 %MUL3)
  ret void
}

define void @"free<i32>"(i32* %0) {
entry:
  %data = alloca i32*, align 8
  store i32* %0, i32** %data, align 8
  %"$data" = load i32*, i32** %data, align 8
  %AS = bitcast i32* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define %P* @"alloc<P>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", trunc (i64 mul nuw (i64 ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i64), i64 2) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to %P*
  ret %P* %AS1
}

define void @"free<P>"(%P* %0) {
entry:
  %data = alloca %P*, align 8
  store %P* %0, %P** %data, align 8
  %"$data" = load %P*, %P** %data, align 8
  %AS = bitcast %P* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %b = alloca i32*, align 8
  %ps = alloca %P*, align 8
  %"call.alloc<i32>" = call i32* @"alloc<i32>"(i32 3)
  store i32* %"call.alloc<i32>", i32** %a, align 8
  %"call.alloc<i32>1" = call i32* @"alloc<i32>"(i32 3)
  store i32* %"call.alloc<i32>1", i32** %b, align 8
  %"$a" = load i32*, i32** %a, align 8
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 0
  store i32 10, i32* %"a[]", align 4
  %"$a2" = load i32*, i32** %a, align 8
  %"a[]3" = getelementptr inbounds i32, i32* %"$a2", i32 1
  store i32 20, i32* %"a[]3", align 4
  %"$a4" = load i32*, i32** %a, align 8
  %"a[]5" = getelementptr inbounds i32, i32* %"$a4", i32 2
  store i32 30, i32* %"a[]5", align 4
  %"$b" = load i32*, i32** %b, align 8
  %"$a6" = load i32*, i32** %a, align 8
  call void @"copy<i32>"(i32* %"$b", i32* %"$a6", i32 3)
  %"$b7" = load i32*, i32** %b, align 8
  %"b[]" = getelementptr inbounds i32, i32* %"$b7", i32 0
  %"$b8" = load i32*, i32** %b, align 8
  %"b[]9" = getelementptr inbounds i32, i32* %"$b8", i32 1
  %"$b10" = load i32*, i32** %b, align 8
  %"b[]11" = getelementptr inbounds i32, i32* %"$b10", i32 2
  %"$b[]" = load i32, i32* %"b[]", align 4
  %"$b[]12" = load i32, i32* %"b[]9", align 4
  %"$b[]13" = load i32, i32* %"b[]11", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %"$b[]", i32 %"$b[]12", i32 %"$b[]13")
  %"call.alloc<P>" = call %P* @"alloc<P>"(i32 2)
  store %P* %"call.alloc<P>", %P** %ps, align 8
  %"$ps" = load %P*, %P** %ps, align 8
  %"ps[]" = getelementptr inbounds %P, %P* %"$ps", i32 1
  %"ps[].y" = getelementptr inbounds %P, %P* %"ps[]", i32 0, i32 1
  store i32 7, i32* %"ps[].y", align 4
  %"$ps14" = load %P*, %P** %ps, align 8
  %"ps[]15" = getelementptr inbounds %P, %P* %"$ps14", i32 0
  %"ps[].x" = getelementptr inbounds %P, %P* %"ps[]15", i32 0, i32 0
  %"$ps16" = load %P*, %P** %ps, align 8
  %"ps[]17" = getelementptr inbounds %P, %P* %"$ps16", i32 1
  %"ps[].y18" = getelementptr inbounds %P, %P* %"ps[]17", i32 0, i32 1
  %"$ps[].x" = load i32, i32* %"ps[].x", align 4
  %"$ps[].y" = load i32, i32* %"ps[].y18", align 4
  %call.dprintf19 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0), i32 %"$ps[].x", i32 %"$ps[].y")
  %"$a20" = load i32*, i32** %a, align 8
  call void @"free<i32>"(i32* %"$a20")
  %"$b21" = load i32*, i32** %b, align 8
  call void @"free<i32>"(i32* %"$b21")
  %"$ps22" = load %P*, %P** %ps, align 8
  call void @"free<P>"(%P* %"$ps22")
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

