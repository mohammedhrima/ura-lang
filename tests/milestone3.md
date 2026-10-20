# milestone3

## index

- 001 — struct: declare: attributes start at zero
- 002 — struct: write an attribute, read it back
- 003 — struct: attributes don't overlap
- 004 — struct: compound assignment on an attribute
- 005 — struct: two variables of one struct are separate
- 006 — struct: attributes of different types
- 007 — struct: nested struct: r.b.x
- 008 — struct: attributes in a loop
- 009 — struct: array attribute
- 010 — struct: print attributes
- 011 — struct: a function returns a struct
- 012 — struct: a ref variable to a struct
- 013 — struct: write through a ref parameter
- 014 — struct: read through a ref parameter
- 015 — method: a method reads self
- 016 — method: a method writes self
- 017 — method: a method reads and writes self
- 018 — method: a method with arguments
- 019 — method: a method calls another method
- 020 — method: a method returns a bool
- 021 — method: a method in a loop
- 022 — method: two structs, one method name
- 023 — method: a method on a nested attribute
- 024 — method: overloaded method
- 025 — method: a method on a ref receiver
- 026 — method: a method through a ref changes the caller
- 027 — global scope: no main: the file is the program
- 028 — name error: a struct declared twice
- 029 — name error: unknown attribute
- 030 — name error: unknown method
- 031 — name error: self outside a method
- 032 — program: a method called three times through a ref
- 033 — program: methods of a nested struct, called through self
- 034 — program: bubble sort: a struct with an array, methods calling methods
- 035 — program: bank account: methods, a ref parameter, an array of bills
- 036 — program: an array of structs is a copy
- 037 — program: vectors: struct parameters, struct returns, chained calls
- 038 — program: linked list: ref attributes, isNullable, rebinding a ref in a loop
- 039 — program: a recursive method that counts its calls
- 040 — program: a method takes a ref to its own struct type
- 041 — program: stack: push squares, pop until empty
- 042 — program: ring-buffer queue: full, wrap around, drain
- 043 — program: binary tree: recursion through ref attributes
- 044 — program: a linked list over array elements
- 045 — program: search a linked list, -1 when missing
- 046 — program: assigning a struct copies it
- 047 — program: a struct parameter is a copy
- 048 — program: compound assignment through a ref attribute
- 049 — program: a method on an array element
- 050 — program: builder: a method returns a ref to self, chained
- 051 — program: a struct holding an array of structs
- 052 — program: tree walk with a ref out-parameter
- 053 — program: a function returns a ref into a struct, the caller binds it
- 054 — program: fibonacci as a pair: recursion returning a struct, .a on a call
- 055 — program: method calls nested as arguments
- 056 — program: print a linked list backwards with recursion
- 057 — program: sort structs in an array by an attribute
- 058 — program: ping pong: two structs that name each other, before either is declared
- 059 — program: while True with break and return inside a method
- 060 — program: script: a global stack evaluates postfix 3 4 + 2 *
- 061 — program: structs used before declared, a struct local to a function
- 062 — program: a linked list built with chained methods that return refs
- 063 — program: binary search tree: recursive methods through ref attributes
- 064 — program: 2x2 matrix product with methods and ref parameters

---

## 001 — struct: declare: attributes start at zero

```ura
// expect: 0
struct User:
    a i32
    b i32

fn main() i32:
    u User
    return u.a + u.b
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32, i32 }

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %load.u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %load.load.u.a = load i32, i32* %load.u.a, align 4
  %load.u.b = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  %load.load.u.b = load i32, i32* %load.u.b, align 4
  %ADD = add i32 %load.load.u.a, %load.load.u.b
  ret i32 %ADD
}
```

---

## 002 — struct: write an attribute, read it back

```ura
// expect: 7
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
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32, i32 }

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %load.u.b = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  store i32 7, i32* %load.u.b, align 4
  %load.u.b1 = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  %load.load.u.b = load i32, i32* %load.u.b1, align 4
  ret i32 %load.load.u.b
}
```

---

## 003 — struct: attributes don't overlap

```ura
// expect: 34
struct User:
    a i32
    b i32

fn main() i32:
    u User
    u.a = 3
    u.b = 4
    return u.a * 10 + u.b
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32, i32 }

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %load.u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 3, i32* %load.u.a, align 4
  %load.u.b = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  store i32 4, i32* %load.u.b, align 4
  %load.u.a1 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %load.load.u.a = load i32, i32* %load.u.a1, align 4
  %MUL = mul i32 %load.load.u.a, 10
  %load.u.b2 = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  %load.load.u.b = load i32, i32* %load.u.b2, align 4
  %ADD = add i32 %MUL, %load.load.u.b
  ret i32 %ADD
}
```

---

## 004 — struct: compound assignment on an attribute

```ura
// expect: 21   ((5 + 2) * 3)
struct User:
    a i32

fn main() i32:
    u User
    u.a = 5
    u.a += 2
    u.a *= 3
    return u.a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32 }

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %load.u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 5, i32* %load.u.a, align 4
  %load.u.a1 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %old = load i32, i32* %load.u.a1, align 4
  %ADD_ASSIGN = add i32 %old, 2
  store i32 %ADD_ASSIGN, i32* %load.u.a1, align 4
  %load.u.a2 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %old3 = load i32, i32* %load.u.a2, align 4
  %MUL_ASSIGN = mul i32 %old3, 3
  store i32 %MUL_ASSIGN, i32* %load.u.a2, align 4
  %load.u.a4 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %load.load.u.a = load i32, i32* %load.u.a4, align 4
  ret i32 %load.load.u.a
}
```

---

## 005 — struct: two variables of one struct are separate

```ura
// expect: 12
struct User:
    a i32

fn main() i32:
    u User
    v User
    u.a = 1
    v.a = 2
    return u.a * 10 + v.a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32 }

define i32 @main() {
entry:
  %u = alloca %User, align 8
  %v = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  store %User zeroinitializer, %User* %v, align 4
  %load.u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 1, i32* %load.u.a, align 4
  %load.v.a = getelementptr inbounds %User, %User* %v, i32 0, i32 0
  store i32 2, i32* %load.v.a, align 4
  %load.u.a1 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %load.load.u.a = load i32, i32* %load.u.a1, align 4
  %MUL = mul i32 %load.load.u.a, 10
  %load.v.a2 = getelementptr inbounds %User, %User* %v, i32 0, i32 0
  %load.load.v.a = load i32, i32* %load.v.a2, align 4
  %ADD = add i32 %MUL, %load.load.v.a
  ret i32 %ADD
}
```

---

## 006 — struct: attributes of different types

```ura
// expect: 4
struct Item:
    id i32
    used bool
    tag char

fn main() i32:
    it Item
    it.id = 4
    it.used = True
    it.tag = 'x'
    if it.used and it.tag == 'x':
        return it.id
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Item = type { i32, i1, i8 }

define i32 @main() {
entry:
  %it = alloca %Item, align 8
  store %Item zeroinitializer, %Item* %it, align 4
  %load.it.id = getelementptr inbounds %Item, %Item* %it, i32 0, i32 0
  store i32 4, i32* %load.it.id, align 4
  %load.it.used = getelementptr inbounds %Item, %Item* %it, i32 0, i32 1
  store i1 true, i1* %load.it.used, align 1
  %load.it.tag = getelementptr inbounds %Item, %Item* %it, i32 0, i32 2
  store i8 120, i8* %load.it.tag, align 1
  %load.it.used1 = getelementptr inbounds %Item, %Item* %it, i32 0, i32 1
  %load.load.it.used = load i1, i1* %load.it.used1, align 1
  %load.it.tag2 = getelementptr inbounds %Item, %Item* %it, i32 0, i32 2
  %load.load.it.tag = load i8, i8* %load.it.tag2, align 1
  %EQ = icmp eq i8 %load.load.it.tag, 120
  %AND = and i1 %load.load.it.used, %EQ
  br i1 %AND, label %then, label %endif

then:                                             ; preds = %entry
  %load.it.id3 = getelementptr inbounds %Item, %Item* %it, i32 0, i32 0
  %load.load.it.id = load i32, i32* %load.it.id3, align 4
  ret i32 %load.load.it.id

endif:                                            ; preds = %entry
  ret i32 0
}
```

---

## 007 — struct: nested struct: r.b.x

```ura
// expect: 6   (5 - 1 + 2)
struct Point:
    x i32
    y i32

struct Rect:
    a Point
    b Point

fn main() i32:
    r Rect
    r.a.x = 1
    r.b.x = 5
    r.b.y = 2
    return r.b.x - r.a.x + r.b.y
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Rect = type { %Point, %Point }
%Point = type { i32, i32 }

define i32 @main() {
entry:
  %r = alloca %Rect, align 8
  store %Rect zeroinitializer, %Rect* %r, align 4
  %load.r.a = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 0
  %load.load.r.a.x = getelementptr inbounds %Point, %Point* %load.r.a, i32 0, i32 0
  store i32 1, i32* %load.load.r.a.x, align 4
  %load.r.b = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %load.load.r.b.x = getelementptr inbounds %Point, %Point* %load.r.b, i32 0, i32 0
  store i32 5, i32* %load.load.r.b.x, align 4
  %load.r.b1 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %load.load.r.b.y = getelementptr inbounds %Point, %Point* %load.r.b1, i32 0, i32 1
  store i32 2, i32* %load.load.r.b.y, align 4
  %load.r.b2 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %load.load.r.b.x3 = getelementptr inbounds %Point, %Point* %load.r.b2, i32 0, i32 0
  %load.load.load.r.b.x = load i32, i32* %load.load.r.b.x3, align 4
  %load.r.a4 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 0
  %load.load.r.a.x5 = getelementptr inbounds %Point, %Point* %load.r.a4, i32 0, i32 0
  %load.load.load.r.a.x = load i32, i32* %load.load.r.a.x5, align 4
  %SUB = sub i32 %load.load.load.r.b.x, %load.load.load.r.a.x
  %load.r.b6 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %load.load.r.b.y7 = getelementptr inbounds %Point, %Point* %load.r.b6, i32 0, i32 1
  %load.load.load.r.b.y = load i32, i32* %load.load.r.b.y7, align 4
  %ADD = add i32 %SUB, %load.load.load.r.b.y
  ret i32 %ADD
}
```

---

## 008 — struct: attributes in a loop

```ura
// expect: 10   (1 + 2 + 3 + 4)
struct Counter:
    n i32
    total i32

fn main() i32:
    c Counter
    while c.n < 4:
        c.n += 1
        c.total += c.n
    return c.total
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Counter = type { i32, i32 }

define i32 @main() {
entry:
  %c = alloca %Counter, align 8
  store %Counter zeroinitializer, %Counter* %c, align 4
  br label %cond

cond:                                             ; preds = %then, %entry
  %load.c.n = getelementptr inbounds %Counter, %Counter* %c, i32 0, i32 0
  %load.load.c.n = load i32, i32* %load.c.n, align 4
  %LT = icmp slt i32 %load.load.c.n, 4
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.c.n1 = getelementptr inbounds %Counter, %Counter* %c, i32 0, i32 0
  %old = load i32, i32* %load.c.n1, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %load.c.n1, align 4
  %load.c.total = getelementptr inbounds %Counter, %Counter* %c, i32 0, i32 1
  %load.c.n2 = getelementptr inbounds %Counter, %Counter* %c, i32 0, i32 0
  %load.load.c.n3 = load i32, i32* %load.c.n2, align 4
  %old4 = load i32, i32* %load.c.total, align 4
  %ADD_ASSIGN5 = add i32 %old4, %load.load.c.n3
  store i32 %ADD_ASSIGN5, i32* %load.c.total, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  %load.c.total6 = getelementptr inbounds %Counter, %Counter* %c, i32 0, i32 1
  %load.load.c.total = load i32, i32* %load.c.total6, align 4
  ret i32 %load.load.c.total
}
```

---

## 009 — struct: array attribute

```ura
// expect: 12
struct Row:
    cells i32[]

fn main() i32:
    r Row
    r.cells = [1, 2, 3]
    r.cells[0] = 9
    return r.cells[0] + r.cells[2]
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Row = type { i32* }

define i32 @main() {
entry:
  %r = alloca %Row, align 8
  %array = alloca [3 x i32], align 4
  store %Row zeroinitializer, %Row* %r, align 8
  %load.r.cells = getelementptr inbounds %Row, %Row* %r, i32 0, i32 0
  %e = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32 1, i32* %e, align 4
  %e1 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 1
  store i32 2, i32* %e1, align 4
  %e2 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 2
  store i32 3, i32* %e2, align 4
  %array3 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32* %array3, i32** %load.r.cells, align 8
  %load.r.cells4 = getelementptr inbounds %Row, %Row* %r, i32 0, i32 0
  %load.load.r.cells = load i32*, i32** %load.r.cells4, align 8
  %"load.load.r.cells[0]" = getelementptr inbounds i32, i32* %load.load.r.cells, i32 0
  store i32 9, i32* %"load.load.r.cells[0]", align 4
  %load.r.cells5 = getelementptr inbounds %Row, %Row* %r, i32 0, i32 0
  %load.load.r.cells6 = load i32*, i32** %load.r.cells5, align 8
  %"load.load.r.cells[0]7" = getelementptr inbounds i32, i32* %load.load.r.cells6, i32 0
  %"load.load.load.r.cells[0]" = load i32, i32* %"load.load.r.cells[0]7", align 4
  %load.r.cells8 = getelementptr inbounds %Row, %Row* %r, i32 0, i32 0
  %load.load.r.cells9 = load i32*, i32** %load.r.cells8, align 8
  %"load.load.r.cells[2]" = getelementptr inbounds i32, i32* %load.load.r.cells9, i32 2
  %"load.load.load.r.cells[2]" = load i32, i32* %"load.load.r.cells[2]", align 4
  %ADD = add i32 %"load.load.load.r.cells[0]", %"load.load.load.r.cells[2]"
  ret i32 %ADD
}
```

---

## 010 — struct: print attributes

```ura
// stdout: ura 42
// expect: 0
proto printf(fmt i8[], ...) i32

struct User:
    id i32
    name i8[]

fn main() i32:
    u User
    u.id = 42
    u.name = "ura"
    printf("%s %d\n", u.name, u.id)
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32, i8* }

@str = private constant [4 x i8] c"ura\00"
@str.1 = private constant [7 x i8] c"%s %d\0A\00"

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %u = alloca %User, align 8
  %s = alloca [4 x i8], align 1
  %s1 = alloca [7 x i8], align 1
  store %User zeroinitializer, %User* %u, align 8
  %load.u.id = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 42, i32* %load.u.id, align 4
  %load.u.name = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  %dst = getelementptr inbounds [4 x i8], [4 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i64 4, i1 false)
  store i8* %dst, i8** %load.u.name, align 8
  %dst2 = getelementptr inbounds [7 x i8], [7 x i8]* %s1, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst2, i8* align 1 getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0), i64 7, i1 false)
  %load.u.name3 = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  %load.load.u.name = load i8*, i8** %load.u.name3, align 8
  %load.u.id4 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %load.load.u.id = load i32, i32* %load.u.id4, align 4
  %"printf(str,load.load.u.name,load.load.u.id)" = call i32 (i8*, ...) @printf(i8* %dst2, i8* %load.load.u.name, i32 %load.load.u.id)
  ret i32 0
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 011 — struct: a function returns a struct

```ura
// expect: 5
struct User:
    a i32

fn make(a i32) User:
    u User
    u.a = a
    return u

fn main() i32:
    u User = make(5)
    return u.a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32 }

define %User @make(i32 %0) {
entry:
  %a = alloca i32, align 4
  %u = alloca %User, align 8
  store i32 %0, i32* %a, align 4
  store %User zeroinitializer, %User* %u, align 4
  %load.u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %load.a = load i32, i32* %a, align 4
  store i32 %load.a, i32* %load.u.a, align 4
  %load.u = load %User, %User* %u, align 4
  ret %User %load.u
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  %"make(5)" = call %User @make(i32 5)
  store %User %"make(5)", %User* %u, align 4
  %load.u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %load.load.u.a = load i32, i32* %load.u.a, align 4
  ret i32 %load.load.u.a
}
```

---

## 012 — struct: a ref variable to a struct

```ura
// expect: 8
struct User:
    a i32

fn main() i32:
    u User
    p &User = &u
    p.a = 8
    return u.a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32 }

define i32 @main() {
entry:
  %u = alloca %User, align 8
  %p = alloca %User*, align 8
  store %User zeroinitializer, %User* %u, align 4
  store %User* %u, %User** %p, align 8
  %dref.p = load %User*, %User** %p, align 8
  %load.dref.p.a = getelementptr inbounds %User, %User* %dref.p, i32 0, i32 0
  store i32 8, i32* %load.dref.p.a, align 4
  %load.u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %load.load.u.a = load i32, i32* %load.u.a, align 4
  ret i32 %load.load.u.a
}
```

---

## 013 — struct: write through a ref parameter

```ura
// expect: 3
struct User:
    a i32

fn set(p &User):
    p.a = 3

fn main() i32:
    u User
    set(&u)
    return u.a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32 }

define void @set(%User* %0) {
entry:
  %p = alloca %User*, align 8
  store %User* %0, %User** %p, align 8
  %dref.p = load %User*, %User** %p, align 8
  %load.dref.p.a = getelementptr inbounds %User, %User* %dref.p, i32 0, i32 0
  store i32 3, i32* %load.dref.p.a, align 4
  ret void
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  call void @set(%User* %u)
  %load.u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %load.load.u.a = load i32, i32* %load.u.a, align 4
  ret i32 %load.load.u.a
}
```

---

## 014 — struct: read through a ref parameter

```ura
// expect: 7
struct User:
    a i32
    b i32

fn total(p &User) i32:
    return p.a + p.b

fn main() i32:
    u User
    u.a = 2
    u.b = 5
    return total(&u)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32, i32 }

define i32 @total(%User* %0) {
entry:
  %p = alloca %User*, align 8
  store %User* %0, %User** %p, align 8
  %dref.p = load %User*, %User** %p, align 8
  %load.dref.p.a = getelementptr inbounds %User, %User* %dref.p, i32 0, i32 0
  %load.load.dref.p.a = load i32, i32* %load.dref.p.a, align 4
  %dref.p1 = load %User*, %User** %p, align 8
  %load.dref.p.b = getelementptr inbounds %User, %User* %dref.p1, i32 0, i32 1
  %load.load.dref.p.b = load i32, i32* %load.dref.p.b, align 4
  %ADD = add i32 %load.load.dref.p.a, %load.load.dref.p.b
  ret i32 %ADD
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %load.u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 2, i32* %load.u.a, align 4
  %load.u.b = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  store i32 5, i32* %load.u.b, align 4
  %"total(&u)" = call i32 @total(%User* %u)
  ret i32 %"total(&u)"
}
```

---

## 015 — method: a method reads self

```ura
// expect: 4
struct User:
    a i32

    fn get() i32:
        return self.a

fn main() i32:
    u User
    u.a = 4
    return u.get()
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32 }

define i32 @User.get(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %dref.self = load %User*, %User** %self, align 8
  %load.dref.self.a = getelementptr inbounds %User, %User* %dref.self, i32 0, i32 0
  %load.load.dref.self.a = load i32, i32* %load.dref.self.a, align 4
  ret i32 %load.load.dref.self.a
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %load.u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 4, i32* %load.u.a, align 4
  %"get(&load.u)" = call i32 @User.get(%User* %u)
  ret i32 %"get(&load.u)"
}
```

---

## 016 — method: a method writes self

```ura
// expect: 9
struct User:
    a i32

    fn set(n i32):
        self.a = n

fn main() i32:
    u User
    u.set(9)
    return u.a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32 }

define void @User.set(%User* %0, i32 %1) {
entry:
  %self = alloca %User*, align 8
  %n = alloca i32, align 4
  store %User* %0, %User** %self, align 8
  store i32 %1, i32* %n, align 4
  %dref.self = load %User*, %User** %self, align 8
  %load.dref.self.a = getelementptr inbounds %User, %User* %dref.self, i32 0, i32 0
  %load.n = load i32, i32* %n, align 4
  store i32 %load.n, i32* %load.dref.self.a, align 4
  ret void
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  call void @User.set(%User* %u, i32 9)
  %load.u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %load.load.u.a = load i32, i32* %load.u.a, align 4
  ret i32 %load.load.u.a
}
```

---

## 017 — method: a method reads and writes self

```ura
// expect: 7
struct User:
    a i32

    fn bump() i32:
        self.a += 1
        return self.a

fn main() i32:
    u User
    u.a = 5
    u.bump()
    return u.bump()
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32 }

define i32 @User.bump(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %dref.self = load %User*, %User** %self, align 8
  %load.dref.self.a = getelementptr inbounds %User, %User* %dref.self, i32 0, i32 0
  %old = load i32, i32* %load.dref.self.a, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %load.dref.self.a, align 4
  %dref.self1 = load %User*, %User** %self, align 8
  %load.dref.self.a2 = getelementptr inbounds %User, %User* %dref.self1, i32 0, i32 0
  %load.load.dref.self.a = load i32, i32* %load.dref.self.a2, align 4
  ret i32 %load.load.dref.self.a
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %load.u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 5, i32* %load.u.a, align 4
  %"bump(&load.u)" = call i32 @User.bump(%User* %u)
  %"bump(&load.u)1" = call i32 @User.bump(%User* %u)
  ret i32 %"bump(&load.u)1"
}
```

---

## 018 — method: a method with arguments

```ura
// expect: 8
struct User:
    a i32

    fn add(n i32) i32:
        return self.a + n

fn main() i32:
    u User
    u.a = 5
    return u.add(3)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32 }

define i32 @User.add(%User* %0, i32 %1) {
entry:
  %self = alloca %User*, align 8
  %n = alloca i32, align 4
  store %User* %0, %User** %self, align 8
  store i32 %1, i32* %n, align 4
  %dref.self = load %User*, %User** %self, align 8
  %load.dref.self.a = getelementptr inbounds %User, %User* %dref.self, i32 0, i32 0
  %load.load.dref.self.a = load i32, i32* %load.dref.self.a, align 4
  %load.n = load i32, i32* %n, align 4
  %ADD = add i32 %load.load.dref.self.a, %load.n
  ret i32 %ADD
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %load.u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 5, i32* %load.u.a, align 4
  %"add(&load.u,3)" = call i32 @User.add(%User* %u, i32 3)
  ret i32 %"add(&load.u,3)"
}
```

---

## 019 — method: a method calls another method

```ura
// expect: 12
struct User:
    a i32

    fn get() i32:
        return self.a

    fn twice() i32:
        return self.get() * 2

fn main() i32:
    u User
    u.a = 6
    return u.twice()
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32 }

define i32 @User.get(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %dref.self = load %User*, %User** %self, align 8
  %load.dref.self.a = getelementptr inbounds %User, %User* %dref.self, i32 0, i32 0
  %load.load.dref.self.a = load i32, i32* %load.dref.self.a, align 4
  ret i32 %load.load.dref.self.a
}

define i32 @User.twice(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %dref.self = load %User*, %User** %self, align 8
  %"get(&load.dref.self)" = call i32 @User.get(%User* %dref.self)
  %MUL = mul i32 %"get(&load.dref.self)", 2
  ret i32 %MUL
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %load.u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 6, i32* %load.u.a, align 4
  %"twice(&load.u)" = call i32 @User.twice(%User* %u)
  ret i32 %"twice(&load.u)"
}
```

---

## 020 — method: a method returns a bool

```ura
// expect: 1
struct User:
    age i32

    fn adult() bool:
        return self.age >= 18

fn main() i32:
    u User
    u.age = 20
    if u.adult():
        return 1
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32 }

define i1 @User.adult(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %dref.self = load %User*, %User** %self, align 8
  %load.dref.self.age = getelementptr inbounds %User, %User* %dref.self, i32 0, i32 0
  %load.load.dref.self.age = load i32, i32* %load.dref.self.age, align 4
  %GE = icmp sge i32 %load.load.dref.self.age, 18
  ret i1 %GE
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %load.u.age = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 20, i32* %load.u.age, align 4
  %"adult(&load.u)" = call i1 @User.adult(%User* %u)
  br i1 %"adult(&load.u)", label %then, label %endif

then:                                             ; preds = %entry
  ret i32 1

endif:                                            ; preds = %entry
  ret i32 0
}
```

---

## 021 — method: a method in a loop

```ura
// stdout: 3
// expect: 3
proto printf(fmt i8[], ...) i32

struct Counter:
    n i32

    fn tick():
        self.n += 1

fn main() i32:
    c Counter
    i i32 = 0
    while i < 3:
        c.tick()
        i += 1
    printf("%d\n", c.n)
    return c.n
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Counter = type { i32 }

@str = private constant [4 x i8] c"%d\0A\00"

define void @Counter.tick(%Counter* %0) {
entry:
  %self = alloca %Counter*, align 8
  store %Counter* %0, %Counter** %self, align 8
  %dref.self = load %Counter*, %Counter** %self, align 8
  %load.dref.self.n = getelementptr inbounds %Counter, %Counter* %dref.self, i32 0, i32 0
  %old = load i32, i32* %load.dref.self.n, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %load.dref.self.n, align 4
  ret void
}

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %c = alloca %Counter, align 8
  %i = alloca i32, align 4
  %s = alloca [4 x i8], align 1
  store %Counter zeroinitializer, %Counter* %c, align 4
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %then, %entry
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 3
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  call void @Counter.tick(%Counter* %c)
  %old = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  %dst = getelementptr inbounds [4 x i8], [4 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i64 4, i1 false)
  %load.c.n = getelementptr inbounds %Counter, %Counter* %c, i32 0, i32 0
  %load.load.c.n = load i32, i32* %load.c.n, align 4
  %"printf(str,load.load.c.n)" = call i32 (i8*, ...) @printf(i8* %dst, i32 %load.load.c.n)
  %load.c.n1 = getelementptr inbounds %Counter, %Counter* %c, i32 0, i32 0
  %load.load.c.n2 = load i32, i32* %load.c.n1, align 4
  ret i32 %load.load.c.n2
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 022 — method: two structs, one method name

```ura
// expect: 21   (1 + 2 * 10)
struct A:
    v i32

    fn get() i32:
        return self.v

struct B:
    v i32

    fn get() i32:
        return self.v * 10

fn main() i32:
    a A
    b B
    a.v = 1
    b.v = 2
    return a.get() + b.get()
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%A = type { i32 }
%B = type { i32 }

define i32 @A.get(%A* %0) {
entry:
  %self = alloca %A*, align 8
  store %A* %0, %A** %self, align 8
  %dref.self = load %A*, %A** %self, align 8
  %load.dref.self.v = getelementptr inbounds %A, %A* %dref.self, i32 0, i32 0
  %load.load.dref.self.v = load i32, i32* %load.dref.self.v, align 4
  ret i32 %load.load.dref.self.v
}

define i32 @B.get(%B* %0) {
entry:
  %self = alloca %B*, align 8
  store %B* %0, %B** %self, align 8
  %dref.self = load %B*, %B** %self, align 8
  %load.dref.self.v = getelementptr inbounds %B, %B* %dref.self, i32 0, i32 0
  %load.load.dref.self.v = load i32, i32* %load.dref.self.v, align 4
  %MUL = mul i32 %load.load.dref.self.v, 10
  ret i32 %MUL
}

define i32 @main() {
entry:
  %a = alloca %A, align 8
  %b = alloca %B, align 8
  store %A zeroinitializer, %A* %a, align 4
  store %B zeroinitializer, %B* %b, align 4
  %load.a.v = getelementptr inbounds %A, %A* %a, i32 0, i32 0
  store i32 1, i32* %load.a.v, align 4
  %load.b.v = getelementptr inbounds %B, %B* %b, i32 0, i32 0
  store i32 2, i32* %load.b.v, align 4
  %"get(&load.a)" = call i32 @A.get(%A* %a)
  %"get(&load.b)" = call i32 @B.get(%B* %b)
  %ADD = add i32 %"get(&load.a)", %"get(&load.b)"
  ret i32 %ADD
}
```

---

## 023 — method: a method on a nested attribute

```ura
// expect: 7
struct Point:
    x i32

    fn get() i32:
        return self.x

struct Rect:
    a Point
    b Point

fn main() i32:
    r Rect
    r.b.x = 7
    return r.b.get()
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Point = type { i32 }
%Rect = type { %Point, %Point }

define i32 @Point.get(%Point* %0) {
entry:
  %self = alloca %Point*, align 8
  store %Point* %0, %Point** %self, align 8
  %dref.self = load %Point*, %Point** %self, align 8
  %load.dref.self.x = getelementptr inbounds %Point, %Point* %dref.self, i32 0, i32 0
  %load.load.dref.self.x = load i32, i32* %load.dref.self.x, align 4
  ret i32 %load.load.dref.self.x
}

define i32 @main() {
entry:
  %r = alloca %Rect, align 8
  store %Rect zeroinitializer, %Rect* %r, align 4
  %load.r.b = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %load.load.r.b.x = getelementptr inbounds %Point, %Point* %load.r.b, i32 0, i32 0
  store i32 7, i32* %load.load.r.b.x, align 4
  %load.r.b1 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %"get(&load.load.r.b)" = call i32 @Point.get(%Point* %load.r.b1)
  ret i32 %"get(&load.load.r.b)"
}
```

---

## 024 — method: overloaded method

```ura
// expect: 6   (1 + 5)
struct User:
    a i32

    fn set(n i32):
        self.a = n

    fn set():
        self.a = 1

fn main() i32:
    u User
    u.set()
    first i32 = u.a
    u.set(5)
    return first + u.a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32 }

define void @User.set(%User* %0, i32 %1) {
entry:
  %self = alloca %User*, align 8
  %n = alloca i32, align 4
  store %User* %0, %User** %self, align 8
  store i32 %1, i32* %n, align 4
  %dref.self = load %User*, %User** %self, align 8
  %load.dref.self.a = getelementptr inbounds %User, %User* %dref.self, i32 0, i32 0
  %load.n = load i32, i32* %n, align 4
  store i32 %load.n, i32* %load.dref.self.a, align 4
  ret void
}

define void @User.set.1(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %dref.self = load %User*, %User** %self, align 8
  %load.dref.self.a = getelementptr inbounds %User, %User* %dref.self, i32 0, i32 0
  store i32 1, i32* %load.dref.self.a, align 4
  ret void
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  %first = alloca i32, align 4
  store %User zeroinitializer, %User* %u, align 4
  call void @User.set.1(%User* %u)
  %load.u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %load.load.u.a = load i32, i32* %load.u.a, align 4
  store i32 %load.load.u.a, i32* %first, align 4
  call void @User.set(%User* %u, i32 5)
  %load.first = load i32, i32* %first, align 4
  %load.u.a1 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %load.load.u.a2 = load i32, i32* %load.u.a1, align 4
  %ADD = add i32 %load.first, %load.load.u.a2
  ret i32 %ADD
}
```

---

## 025 — method: a method on a ref receiver

```ura
// expect: 4
struct User:
    a i32

    fn get() i32:
        return self.a

fn via(p &User) i32:
    return p.get()

fn main() i32:
    u User
    u.a = 4
    return via(&u)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32 }

define i32 @User.get(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %dref.self = load %User*, %User** %self, align 8
  %load.dref.self.a = getelementptr inbounds %User, %User* %dref.self, i32 0, i32 0
  %load.load.dref.self.a = load i32, i32* %load.dref.self.a, align 4
  ret i32 %load.load.dref.self.a
}

define i32 @via(%User* %0) {
entry:
  %p = alloca %User*, align 8
  store %User* %0, %User** %p, align 8
  %dref.p = load %User*, %User** %p, align 8
  %"get(&load.dref.p)" = call i32 @User.get(%User* %dref.p)
  ret i32 %"get(&load.dref.p)"
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %load.u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 4, i32* %load.u.a, align 4
  %"via(&u)" = call i32 @via(%User* %u)
  ret i32 %"via(&u)"
}
```

---

## 026 — method: a method through a ref changes the caller

```ura
// expect: 11
struct User:
    a i32

    fn set(n i32):
        self.a = n

fn reset(p &User):
    p.set(11)

fn main() i32:
    u User
    u.a = 3
    reset(&u)
    return u.a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32 }

define void @User.set(%User* %0, i32 %1) {
entry:
  %self = alloca %User*, align 8
  %n = alloca i32, align 4
  store %User* %0, %User** %self, align 8
  store i32 %1, i32* %n, align 4
  %dref.self = load %User*, %User** %self, align 8
  %load.dref.self.a = getelementptr inbounds %User, %User* %dref.self, i32 0, i32 0
  %load.n = load i32, i32* %n, align 4
  store i32 %load.n, i32* %load.dref.self.a, align 4
  ret void
}

define void @reset(%User* %0) {
entry:
  %p = alloca %User*, align 8
  store %User* %0, %User** %p, align 8
  %dref.p = load %User*, %User** %p, align 8
  call void @User.set(%User* %dref.p, i32 11)
  ret void
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %load.u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 3, i32* %load.u.a, align 4
  call void @reset(%User* %u)
  %load.u.a1 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %load.load.u.a = load i32, i32* %load.u.a1, align 4
  ret i32 %load.load.u.a
}
```

---

## 027 — global scope: no main: the file is the program

```ura
// expect: 40   (23 at 4 * 10, 4 at 0, 99 missing: 0)
struct Finder:
    data i32[]
    size i32

    fn index_of(v i32) i32:
        i i32 = 0
        while True:
            if i == self.size: break
            if self.data[i] == v: return i
            i += 1
        return -1

f Finder
f.data = [4, 8, 15, 16, 23, 42]
f.size = 6
return f.index_of(23) * 10 + f.index_of(4) + (f.index_of(99) + 1) * 100
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Finder = type { i32*, i32 }

@f = global %Finder zeroinitializer

define i32 @Finder.index_of(%Finder* %0, i32 %1) {
entry:
  %self = alloca %Finder*, align 8
  %v = alloca i32, align 4
  %i = alloca i32, align 4
  store %Finder* %0, %Finder** %self, align 8
  store i32 %1, i32* %v, align 4
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %endif2, %entry
  br i1 true, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.i = load i32, i32* %i, align 4
  %dref.self = load %Finder*, %Finder** %self, align 8
  %load.dref.self.size = getelementptr inbounds %Finder, %Finder* %dref.self, i32 0, i32 1
  %load.load.dref.self.size = load i32, i32* %load.dref.self.size, align 4
  %EQ = icmp eq i32 %load.i, %load.load.dref.self.size
  br i1 %EQ, label %then1, label %endif

endwhile:                                         ; preds = %then1, %cond
  ret i32 -1

then1:                                            ; preds = %then
  br label %endwhile

endif:                                            ; preds = %then
  %dref.self4 = load %Finder*, %Finder** %self, align 8
  %load.dref.self.data = getelementptr inbounds %Finder, %Finder* %dref.self4, i32 0, i32 0
  %load.load.dref.self.data = load i32*, i32** %load.dref.self.data, align 8
  %load.i5 = load i32, i32* %i, align 4
  %"load.load.dref.self.data[load.i]" = getelementptr inbounds i32, i32* %load.load.dref.self.data, i32 %load.i5
  %"load.load.load.dref.self.data[load.i]" = load i32, i32* %"load.load.dref.self.data[load.i]", align 4
  %load.v = load i32, i32* %v, align 4
  %EQ6 = icmp eq i32 %"load.load.load.dref.self.data[load.i]", %load.v
  br i1 %EQ6, label %then3, label %endif2

then3:                                            ; preds = %endif
  %load.i7 = load i32, i32* %i, align 4
  ret i32 %load.i7

endif2:                                           ; preds = %endif
  %old = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  br label %cond
}

define i32 @main() {
entry:
  %array = alloca [6 x i32], align 4
  store %Finder zeroinitializer, %Finder* @f, align 8
  %e = getelementptr inbounds [6 x i32], [6 x i32]* %array, i32 0, i32 0
  store i32 4, i32* %e, align 4
  %e1 = getelementptr inbounds [6 x i32], [6 x i32]* %array, i32 0, i32 1
  store i32 8, i32* %e1, align 4
  %e2 = getelementptr inbounds [6 x i32], [6 x i32]* %array, i32 0, i32 2
  store i32 15, i32* %e2, align 4
  %e3 = getelementptr inbounds [6 x i32], [6 x i32]* %array, i32 0, i32 3
  store i32 16, i32* %e3, align 4
  %e4 = getelementptr inbounds [6 x i32], [6 x i32]* %array, i32 0, i32 4
  store i32 23, i32* %e4, align 4
  %e5 = getelementptr inbounds [6 x i32], [6 x i32]* %array, i32 0, i32 5
  store i32 42, i32* %e5, align 4
  %array6 = getelementptr inbounds [6 x i32], [6 x i32]* %array, i32 0, i32 0
  store i32* %array6, i32** getelementptr inbounds (%Finder, %Finder* @f, i32 0, i32 0), align 8
  store i32 6, i32* getelementptr inbounds (%Finder, %Finder* @f, i32 0, i32 1), align 4
  %"index_of(&load.f,23)" = call i32 @Finder.index_of(%Finder* @f, i32 23)
  %MUL = mul i32 %"index_of(&load.f,23)", 10
  %"index_of(&load.f,4)" = call i32 @Finder.index_of(%Finder* @f, i32 4)
  %ADD = add i32 %MUL, %"index_of(&load.f,4)"
  %"index_of(&load.f,99)" = call i32 @Finder.index_of(%Finder* @f, i32 99)
  %ADD7 = add i32 %"index_of(&load.f,99)", 1
  %MUL8 = mul i32 %ADD7, 100
  %ADD9 = add i32 %ADD, %MUL8
  ret i32 %ADD9
}
```

---

## 028 — name error: a struct declared twice

```ura
// error: struct 'User' is already declared, on line 2
struct User:
    a i32

struct User:
    b i32

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:5:8 struct 'User' is already declared, on line 2
  |
5 | struct User:
  |        ^^^^
```

### status

```
1
```

---

## 029 — name error: unknown attribute

```ura
// error: User has no attribute 'age'
// help: User has: name, id
struct User:
    name i8[]
    id i32

fn main() i32:
    u User
    return u.age
```

### stderr

```
error: test.ura:9:14 User has no attribute 'age'
  |
9 |     return u.age
  |              ^^^
help: User has: name, id
```

### status

```
1
```

---

## 030 — name error: unknown method

```ura
// error: User has no method 'run'
// help: User has: get
struct User:
    a i32

    fn get() i32:
        return self.a

fn main() i32:
    u User
    return u.run()
```

### stderr

```
error: test.ura:11:14 User has no method 'run'
   |
11 |     return u.run()
   |              ^^^
help: User has: get
```

### status

```
1
```

---

## 031 — name error: self outside a method

```ura
// error: 'self' only exists inside a method
fn get() i32:
    return self.a

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:3:12 'self' only exists inside a method
  |
3 |     return self.a
  |            ^^^^
```

### status

```
1
```

---

## 032 — program: a method called three times through a ref

```ura
// expect: 21
struct Pair:
    a i32
    b i32

    fn swap():
        t i32 = self.a
        self.a = self.b
        self.b = t

fn thrice(p &Pair):
    p.swap()
    p.swap()
    p.swap()

fn main() i32:
    x Pair
    x.a = 1
    x.b = 2
    thrice(&x)
    return x.a * 10 + x.b
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Pair = type { i32, i32 }

define void @Pair.swap(%Pair* %0) {
entry:
  %self = alloca %Pair*, align 8
  %t = alloca i32, align 4
  store %Pair* %0, %Pair** %self, align 8
  %dref.self = load %Pair*, %Pair** %self, align 8
  %load.dref.self.a = getelementptr inbounds %Pair, %Pair* %dref.self, i32 0, i32 0
  %load.load.dref.self.a = load i32, i32* %load.dref.self.a, align 4
  store i32 %load.load.dref.self.a, i32* %t, align 4
  %dref.self1 = load %Pair*, %Pair** %self, align 8
  %load.dref.self.a2 = getelementptr inbounds %Pair, %Pair* %dref.self1, i32 0, i32 0
  %dref.self3 = load %Pair*, %Pair** %self, align 8
  %load.dref.self.b = getelementptr inbounds %Pair, %Pair* %dref.self3, i32 0, i32 1
  %load.load.dref.self.b = load i32, i32* %load.dref.self.b, align 4
  store i32 %load.load.dref.self.b, i32* %load.dref.self.a2, align 4
  %dref.self4 = load %Pair*, %Pair** %self, align 8
  %load.dref.self.b5 = getelementptr inbounds %Pair, %Pair* %dref.self4, i32 0, i32 1
  %load.t = load i32, i32* %t, align 4
  store i32 %load.t, i32* %load.dref.self.b5, align 4
  ret void
}

define void @thrice(%Pair* %0) {
entry:
  %p = alloca %Pair*, align 8
  store %Pair* %0, %Pair** %p, align 8
  %dref.p = load %Pair*, %Pair** %p, align 8
  call void @Pair.swap(%Pair* %dref.p)
  %dref.p1 = load %Pair*, %Pair** %p, align 8
  call void @Pair.swap(%Pair* %dref.p1)
  %dref.p2 = load %Pair*, %Pair** %p, align 8
  call void @Pair.swap(%Pair* %dref.p2)
  ret void
}

define i32 @main() {
entry:
  %x = alloca %Pair, align 8
  store %Pair zeroinitializer, %Pair* %x, align 4
  %load.x.a = getelementptr inbounds %Pair, %Pair* %x, i32 0, i32 0
  store i32 1, i32* %load.x.a, align 4
  %load.x.b = getelementptr inbounds %Pair, %Pair* %x, i32 0, i32 1
  store i32 2, i32* %load.x.b, align 4
  call void @thrice(%Pair* %x)
  %load.x.a1 = getelementptr inbounds %Pair, %Pair* %x, i32 0, i32 0
  %load.load.x.a = load i32, i32* %load.x.a1, align 4
  %MUL = mul i32 %load.load.x.a, 10
  %load.x.b2 = getelementptr inbounds %Pair, %Pair* %x, i32 0, i32 1
  %load.load.x.b = load i32, i32* %load.x.b2, align 4
  %ADD = add i32 %MUL, %load.load.x.b
  ret i32 %ADD
}
```

---

## 033 — program: methods of a nested struct, called through self

```ura
// expect: 30   (area 4 * 5, plus a.x 10)
struct Point:
    x i32
    y i32

    fn move(dx i32, dy i32):
        self.x += dx
        self.y += dy

struct Rect:
    a Point
    b Point

    fn shift(d i32):
        self.a.move(d, d)
        self.b.move(d, d)

    fn area() i32:
        return (self.b.x - self.a.x) * (self.b.y - self.a.y)

fn main() i32:
    r Rect
    r.b.move(4, 5)
    r.shift(10)
    return r.area() + r.a.x
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Point = type { i32, i32 }
%Rect = type { %Point, %Point }

define void @Point.move(%Point* %0, i32 %1, i32 %2) {
entry:
  %self = alloca %Point*, align 8
  %dx = alloca i32, align 4
  %dy = alloca i32, align 4
  store %Point* %0, %Point** %self, align 8
  store i32 %1, i32* %dx, align 4
  store i32 %2, i32* %dy, align 4
  %dref.self = load %Point*, %Point** %self, align 8
  %load.dref.self.x = getelementptr inbounds %Point, %Point* %dref.self, i32 0, i32 0
  %load.dx = load i32, i32* %dx, align 4
  %old = load i32, i32* %load.dref.self.x, align 4
  %ADD_ASSIGN = add i32 %old, %load.dx
  store i32 %ADD_ASSIGN, i32* %load.dref.self.x, align 4
  %dref.self1 = load %Point*, %Point** %self, align 8
  %load.dref.self.y = getelementptr inbounds %Point, %Point* %dref.self1, i32 0, i32 1
  %load.dy = load i32, i32* %dy, align 4
  %old2 = load i32, i32* %load.dref.self.y, align 4
  %ADD_ASSIGN3 = add i32 %old2, %load.dy
  store i32 %ADD_ASSIGN3, i32* %load.dref.self.y, align 4
  ret void
}

define void @Rect.shift(%Rect* %0, i32 %1) {
entry:
  %self = alloca %Rect*, align 8
  %d = alloca i32, align 4
  store %Rect* %0, %Rect** %self, align 8
  store i32 %1, i32* %d, align 4
  %dref.self = load %Rect*, %Rect** %self, align 8
  %load.dref.self.a = getelementptr inbounds %Rect, %Rect* %dref.self, i32 0, i32 0
  %load.d = load i32, i32* %d, align 4
  %load.d1 = load i32, i32* %d, align 4
  call void @Point.move(%Point* %load.dref.self.a, i32 %load.d, i32 %load.d1)
  %dref.self2 = load %Rect*, %Rect** %self, align 8
  %load.dref.self.b = getelementptr inbounds %Rect, %Rect* %dref.self2, i32 0, i32 1
  %load.d3 = load i32, i32* %d, align 4
  %load.d4 = load i32, i32* %d, align 4
  call void @Point.move(%Point* %load.dref.self.b, i32 %load.d3, i32 %load.d4)
  ret void
}

define i32 @Rect.area(%Rect* %0) {
entry:
  %self = alloca %Rect*, align 8
  store %Rect* %0, %Rect** %self, align 8
  %dref.self = load %Rect*, %Rect** %self, align 8
  %load.dref.self.b = getelementptr inbounds %Rect, %Rect* %dref.self, i32 0, i32 1
  %load.load.dref.self.b.x = getelementptr inbounds %Point, %Point* %load.dref.self.b, i32 0, i32 0
  %load.load.load.dref.self.b.x = load i32, i32* %load.load.dref.self.b.x, align 4
  %dref.self1 = load %Rect*, %Rect** %self, align 8
  %load.dref.self.a = getelementptr inbounds %Rect, %Rect* %dref.self1, i32 0, i32 0
  %load.load.dref.self.a.x = getelementptr inbounds %Point, %Point* %load.dref.self.a, i32 0, i32 0
  %load.load.load.dref.self.a.x = load i32, i32* %load.load.dref.self.a.x, align 4
  %SUB = sub i32 %load.load.load.dref.self.b.x, %load.load.load.dref.self.a.x
  %dref.self2 = load %Rect*, %Rect** %self, align 8
  %load.dref.self.b3 = getelementptr inbounds %Rect, %Rect* %dref.self2, i32 0, i32 1
  %load.load.dref.self.b.y = getelementptr inbounds %Point, %Point* %load.dref.self.b3, i32 0, i32 1
  %load.load.load.dref.self.b.y = load i32, i32* %load.load.dref.self.b.y, align 4
  %dref.self4 = load %Rect*, %Rect** %self, align 8
  %load.dref.self.a5 = getelementptr inbounds %Rect, %Rect* %dref.self4, i32 0, i32 0
  %load.load.dref.self.a.y = getelementptr inbounds %Point, %Point* %load.dref.self.a5, i32 0, i32 1
  %load.load.load.dref.self.a.y = load i32, i32* %load.load.dref.self.a.y, align 4
  %SUB6 = sub i32 %load.load.load.dref.self.b.y, %load.load.load.dref.self.a.y
  %MUL = mul i32 %SUB, %SUB6
  ret i32 %MUL
}

define i32 @main() {
entry:
  %r = alloca %Rect, align 8
  store %Rect zeroinitializer, %Rect* %r, align 4
  %load.r.b = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  call void @Point.move(%Point* %load.r.b, i32 4, i32 5)
  call void @Rect.shift(%Rect* %r, i32 10)
  %"area(&load.r)" = call i32 @Rect.area(%Rect* %r)
  %load.r.a = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 0
  %load.load.r.a.x = getelementptr inbounds %Point, %Point* %load.r.a, i32 0, i32 0
  %load.load.load.r.a.x = load i32, i32* %load.load.r.a.x, align 4
  %ADD = add i32 %"area(&load.r)", %load.load.load.r.a.x
  ret i32 %ADD
}
```

---

## 034 — program: bubble sort: a struct with an array, methods calling methods

```ura
// stdout: 1 3 4 5 9
// expect: 19
proto printf(fmt i8[], ...) i32

struct List:
    items i32[]
    size i32

    fn swap(i i32, j i32):
        t i32 = self.items[i]
        self.items[i] = self.items[j]
        self.items[j] = t

    fn sort():
        i i32 = 0
        while i < self.size:
            j i32 = 0
            while j < self.size - 1 - i:
                if self.items[j] > self.items[j + 1]:
                    self.swap(j, j + 1)
                j += 1
            i += 1

    fn show():
        i i32 = 0
        while i < self.size:
            if i > 0: printf(" ")
            printf("%d", self.items[i])
            i += 1
        printf("\n")

fn main() i32:
    l List
    l.items = [5, 3, 9, 1, 4]
    l.size = 5
    l.sort()
    l.show()
    return l.items[0] * 10 + l.items[4]
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%List = type { i32*, i32 }

@str = private constant [2 x i8] c" \00"
@str.1 = private constant [3 x i8] c"%d\00"
@str.2 = private constant [2 x i8] c"\0A\00"

define void @List.swap(%List* %0, i32 %1, i32 %2) {
entry:
  %self = alloca %List*, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %t = alloca i32, align 4
  store %List* %0, %List** %self, align 8
  store i32 %1, i32* %i, align 4
  store i32 %2, i32* %j, align 4
  %dref.self = load %List*, %List** %self, align 8
  %load.dref.self.items = getelementptr inbounds %List, %List* %dref.self, i32 0, i32 0
  %load.load.dref.self.items = load i32*, i32** %load.dref.self.items, align 8
  %load.i = load i32, i32* %i, align 4
  %"load.load.dref.self.items[load.i]" = getelementptr inbounds i32, i32* %load.load.dref.self.items, i32 %load.i
  %"load.load.load.dref.self.items[load.i]" = load i32, i32* %"load.load.dref.self.items[load.i]", align 4
  store i32 %"load.load.load.dref.self.items[load.i]", i32* %t, align 4
  %dref.self1 = load %List*, %List** %self, align 8
  %load.dref.self.items2 = getelementptr inbounds %List, %List* %dref.self1, i32 0, i32 0
  %load.load.dref.self.items3 = load i32*, i32** %load.dref.self.items2, align 8
  %load.i4 = load i32, i32* %i, align 4
  %"load.load.dref.self.items[load.i]5" = getelementptr inbounds i32, i32* %load.load.dref.self.items3, i32 %load.i4
  %dref.self6 = load %List*, %List** %self, align 8
  %load.dref.self.items7 = getelementptr inbounds %List, %List* %dref.self6, i32 0, i32 0
  %load.load.dref.self.items8 = load i32*, i32** %load.dref.self.items7, align 8
  %load.j = load i32, i32* %j, align 4
  %"load.load.dref.self.items[load.j]" = getelementptr inbounds i32, i32* %load.load.dref.self.items8, i32 %load.j
  %"load.load.load.dref.self.items[load.j]" = load i32, i32* %"load.load.dref.self.items[load.j]", align 4
  store i32 %"load.load.load.dref.self.items[load.j]", i32* %"load.load.dref.self.items[load.i]5", align 4
  %dref.self9 = load %List*, %List** %self, align 8
  %load.dref.self.items10 = getelementptr inbounds %List, %List* %dref.self9, i32 0, i32 0
  %load.load.dref.self.items11 = load i32*, i32** %load.dref.self.items10, align 8
  %load.j12 = load i32, i32* %j, align 4
  %"load.load.dref.self.items[load.j]13" = getelementptr inbounds i32, i32* %load.load.dref.self.items11, i32 %load.j12
  %load.t = load i32, i32* %t, align 4
  store i32 %load.t, i32* %"load.load.dref.self.items[load.j]13", align 4
  ret void
}

define void @List.sort(%List* %0) {
entry:
  %self = alloca %List*, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  store %List* %0, %List** %self, align 8
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %endwhile3, %entry
  %load.i = load i32, i32* %i, align 4
  %dref.self = load %List*, %List** %self, align 8
  %load.dref.self.size = getelementptr inbounds %List, %List* %dref.self, i32 0, i32 1
  %load.load.dref.self.size = load i32, i32* %load.dref.self.size, align 4
  %LT = icmp slt i32 %load.i, %load.load.dref.self.size
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  store i32 0, i32* %j, align 4
  br label %cond1

endwhile:                                         ; preds = %cond
  ret void

cond1:                                            ; preds = %endif, %then
  %load.j = load i32, i32* %j, align 4
  %dref.self4 = load %List*, %List** %self, align 8
  %load.dref.self.size5 = getelementptr inbounds %List, %List* %dref.self4, i32 0, i32 1
  %load.load.dref.self.size6 = load i32, i32* %load.dref.self.size5, align 4
  %SUB = sub i32 %load.load.dref.self.size6, 1
  %load.i7 = load i32, i32* %i, align 4
  %SUB8 = sub i32 %SUB, %load.i7
  %LT9 = icmp slt i32 %load.j, %SUB8
  br i1 %LT9, label %then2, label %endwhile3

then2:                                            ; preds = %cond1
  %dref.self11 = load %List*, %List** %self, align 8
  %load.dref.self.items = getelementptr inbounds %List, %List* %dref.self11, i32 0, i32 0
  %load.load.dref.self.items = load i32*, i32** %load.dref.self.items, align 8
  %load.j12 = load i32, i32* %j, align 4
  %"load.load.dref.self.items[load.j]" = getelementptr inbounds i32, i32* %load.load.dref.self.items, i32 %load.j12
  %"load.load.load.dref.self.items[load.j]" = load i32, i32* %"load.load.dref.self.items[load.j]", align 4
  %dref.self13 = load %List*, %List** %self, align 8
  %load.dref.self.items14 = getelementptr inbounds %List, %List* %dref.self13, i32 0, i32 0
  %load.load.dref.self.items15 = load i32*, i32** %load.dref.self.items14, align 8
  %load.j16 = load i32, i32* %j, align 4
  %ADD = add i32 %load.j16, 1
  %"load.load.dref.self.items[load.j + 1]" = getelementptr inbounds i32, i32* %load.load.dref.self.items15, i32 %ADD
  %"load.load.load.dref.self.items[load.j + 1]" = load i32, i32* %"load.load.dref.self.items[load.j + 1]", align 4
  %GT = icmp sgt i32 %"load.load.load.dref.self.items[load.j]", %"load.load.load.dref.self.items[load.j + 1]"
  br i1 %GT, label %then10, label %endif

endwhile3:                                        ; preds = %cond1
  %old21 = load i32, i32* %i, align 4
  %ADD_ASSIGN22 = add i32 %old21, 1
  store i32 %ADD_ASSIGN22, i32* %i, align 4
  br label %cond

then10:                                           ; preds = %then2
  %dref.self17 = load %List*, %List** %self, align 8
  %load.j18 = load i32, i32* %j, align 4
  %load.j19 = load i32, i32* %j, align 4
  %ADD20 = add i32 %load.j19, 1
  call void @List.swap(%List* %dref.self17, i32 %load.j18, i32 %ADD20)
  br label %endif

endif:                                            ; preds = %then10, %then2
  %old = load i32, i32* %j, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %j, align 4
  br label %cond1
}

define void @List.show(%List* %0) {
entry:
  %self = alloca %List*, align 8
  %i = alloca i32, align 4
  %s = alloca [2 x i8], align 1
  %s3 = alloca [3 x i8], align 1
  %s7 = alloca [2 x i8], align 1
  store %List* %0, %List** %self, align 8
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.i = load i32, i32* %i, align 4
  %dref.self = load %List*, %List** %self, align 8
  %load.dref.self.size = getelementptr inbounds %List, %List* %dref.self, i32 0, i32 1
  %load.load.dref.self.size = load i32, i32* %load.dref.self.size, align 4
  %LT = icmp slt i32 %load.i, %load.load.dref.self.size
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.i2 = load i32, i32* %i, align 4
  %GT = icmp sgt i32 %load.i2, 0
  br i1 %GT, label %then1, label %endif

endwhile:                                         ; preds = %cond
  %dst8 = getelementptr inbounds [2 x i8], [2 x i8]* %s7, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst8, i8* align 1 getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0), i64 2, i1 false)
  %"printf(str)9" = call i32 (i8*, ...) @printf(i8* %dst8)
  ret void

then1:                                            ; preds = %then
  %dst = getelementptr inbounds [2 x i8], [2 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i64 2, i1 false)
  %"printf(str)" = call i32 (i8*, ...) @printf(i8* %dst)
  br label %endif

endif:                                            ; preds = %then1, %then
  %dst4 = getelementptr inbounds [3 x i8], [3 x i8]* %s3, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst4, i8* align 1 getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0), i64 3, i1 false)
  %dref.self5 = load %List*, %List** %self, align 8
  %load.dref.self.items = getelementptr inbounds %List, %List* %dref.self5, i32 0, i32 0
  %load.load.dref.self.items = load i32*, i32** %load.dref.self.items, align 8
  %load.i6 = load i32, i32* %i, align 4
  %"load.load.dref.self.items[load.i]" = getelementptr inbounds i32, i32* %load.load.dref.self.items, i32 %load.i6
  %"load.load.load.dref.self.items[load.i]" = load i32, i32* %"load.load.dref.self.items[load.i]", align 4
  %"printf(str,load.load.load.dref.self.items[load.i])" = call i32 (i8*, ...) @printf(i8* %dst4, i32 %"load.load.load.dref.self.items[load.i]")
  %old = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  br label %cond
}

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %l = alloca %List, align 8
  %array = alloca [5 x i32], align 4
  store %List zeroinitializer, %List* %l, align 8
  %load.l.items = getelementptr inbounds %List, %List* %l, i32 0, i32 0
  %e = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 0
  store i32 5, i32* %e, align 4
  %e1 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 1
  store i32 3, i32* %e1, align 4
  %e2 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 2
  store i32 9, i32* %e2, align 4
  %e3 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 3
  store i32 1, i32* %e3, align 4
  %e4 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 4
  store i32 4, i32* %e4, align 4
  %array5 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 0
  store i32* %array5, i32** %load.l.items, align 8
  %load.l.size = getelementptr inbounds %List, %List* %l, i32 0, i32 1
  store i32 5, i32* %load.l.size, align 4
  call void @List.sort(%List* %l)
  call void @List.show(%List* %l)
  %load.l.items6 = getelementptr inbounds %List, %List* %l, i32 0, i32 0
  %load.load.l.items = load i32*, i32** %load.l.items6, align 8
  %"load.load.l.items[0]" = getelementptr inbounds i32, i32* %load.load.l.items, i32 0
  %"load.load.load.l.items[0]" = load i32, i32* %"load.load.l.items[0]", align 4
  %MUL = mul i32 %"load.load.load.l.items[0]", 10
  %load.l.items7 = getelementptr inbounds %List, %List* %l, i32 0, i32 0
  %load.load.l.items8 = load i32*, i32** %load.l.items7, align 8
  %"load.load.l.items[4]" = getelementptr inbounds i32, i32* %load.load.l.items8, i32 4
  %"load.load.load.l.items[4]" = load i32, i32* %"load.load.l.items[4]", align 4
  %ADD = add i32 %MUL, %"load.load.load.l.items[4]"
  ret i32 %ADD
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 035 — program: bank account: methods, a ref parameter, an array of bills

```ura
// stdout: balance 10, ops 4, failed 1
// expect: 10
proto printf(fmt i8[], ...) i32

struct Account:
    balance i32
    ops i32

    fn deposit(n i32):
        self.balance += n
        self.ops += 1

    fn withdraw(n i32) bool:
        if n > self.balance:
            return False
        self.balance -= n
        self.ops += 1
        return True

fn pay_all(acc &Account, bills i32[], count i32) i32:
    failed i32 = 0
    i i32 = 0
    while i < count:
        if acc.withdraw(bills[i]) == False:
            failed += 1
        i += 1
    return failed

fn main() i32:
    a Account
    a.deposit(100)
    bills i32[] = [30, 50, 40, 10]
    failed i32 = pay_all(&a, bills, 4)
    printf("balance %d, ops %d, failed %d\n", a.balance, a.ops, failed)
    return a.balance
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Account = type { i32, i32 }

@str = private constant [31 x i8] c"balance %d, ops %d, failed %d\0A\00"

define void @Account.deposit(%Account* %0, i32 %1) {
entry:
  %self = alloca %Account*, align 8
  %n = alloca i32, align 4
  store %Account* %0, %Account** %self, align 8
  store i32 %1, i32* %n, align 4
  %dref.self = load %Account*, %Account** %self, align 8
  %load.dref.self.balance = getelementptr inbounds %Account, %Account* %dref.self, i32 0, i32 0
  %load.n = load i32, i32* %n, align 4
  %old = load i32, i32* %load.dref.self.balance, align 4
  %ADD_ASSIGN = add i32 %old, %load.n
  store i32 %ADD_ASSIGN, i32* %load.dref.self.balance, align 4
  %dref.self1 = load %Account*, %Account** %self, align 8
  %load.dref.self.ops = getelementptr inbounds %Account, %Account* %dref.self1, i32 0, i32 1
  %old2 = load i32, i32* %load.dref.self.ops, align 4
  %ADD_ASSIGN3 = add i32 %old2, 1
  store i32 %ADD_ASSIGN3, i32* %load.dref.self.ops, align 4
  ret void
}

define i1 @Account.withdraw(%Account* %0, i32 %1) {
entry:
  %self = alloca %Account*, align 8
  %n = alloca i32, align 4
  store %Account* %0, %Account** %self, align 8
  store i32 %1, i32* %n, align 4
  %load.n = load i32, i32* %n, align 4
  %dref.self = load %Account*, %Account** %self, align 8
  %load.dref.self.balance = getelementptr inbounds %Account, %Account* %dref.self, i32 0, i32 0
  %load.load.dref.self.balance = load i32, i32* %load.dref.self.balance, align 4
  %GT = icmp sgt i32 %load.n, %load.load.dref.self.balance
  br i1 %GT, label %then, label %endif

then:                                             ; preds = %entry
  ret i1 false

endif:                                            ; preds = %entry
  %dref.self1 = load %Account*, %Account** %self, align 8
  %load.dref.self.balance2 = getelementptr inbounds %Account, %Account* %dref.self1, i32 0, i32 0
  %load.n3 = load i32, i32* %n, align 4
  %old = load i32, i32* %load.dref.self.balance2, align 4
  %SUB_ASSIGN = sub i32 %old, %load.n3
  store i32 %SUB_ASSIGN, i32* %load.dref.self.balance2, align 4
  %dref.self4 = load %Account*, %Account** %self, align 8
  %load.dref.self.ops = getelementptr inbounds %Account, %Account* %dref.self4, i32 0, i32 1
  %old5 = load i32, i32* %load.dref.self.ops, align 4
  %ADD_ASSIGN = add i32 %old5, 1
  store i32 %ADD_ASSIGN, i32* %load.dref.self.ops, align 4
  ret i1 true
}

declare i32 @printf(i8*, ...)

define i32 @pay_all(%Account* %0, i32* %1, i32 %2) {
entry:
  %acc = alloca %Account*, align 8
  %bills = alloca i32*, align 8
  %count = alloca i32, align 4
  %failed = alloca i32, align 4
  %i = alloca i32, align 4
  store %Account* %0, %Account** %acc, align 8
  store i32* %1, i32** %bills, align 8
  store i32 %2, i32* %count, align 4
  store i32 0, i32* %failed, align 4
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.i = load i32, i32* %i, align 4
  %load.count = load i32, i32* %count, align 4
  %LT = icmp slt i32 %load.i, %load.count
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %dref.acc = load %Account*, %Account** %acc, align 8
  %load.bills = load i32*, i32** %bills, align 8
  %load.i2 = load i32, i32* %i, align 4
  %"load.bills[load.i]" = getelementptr inbounds i32, i32* %load.bills, i32 %load.i2
  %"load.load.bills[load.i]" = load i32, i32* %"load.bills[load.i]", align 4
  %"withdraw(&load.dref.acc,load.load.bills[load.i])" = call i1 @Account.withdraw(%Account* %dref.acc, i32 %"load.load.bills[load.i]")
  %EQ = icmp eq i1 %"withdraw(&load.dref.acc,load.load.bills[load.i])", false
  br i1 %EQ, label %then1, label %endif

endwhile:                                         ; preds = %cond
  %load.failed = load i32, i32* %failed, align 4
  ret i32 %load.failed

then1:                                            ; preds = %then
  %old = load i32, i32* %failed, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %failed, align 4
  br label %endif

endif:                                            ; preds = %then1, %then
  %old3 = load i32, i32* %i, align 4
  %ADD_ASSIGN4 = add i32 %old3, 1
  store i32 %ADD_ASSIGN4, i32* %i, align 4
  br label %cond
}

define i32 @main() {
entry:
  %a = alloca %Account, align 8
  %bills = alloca i32*, align 8
  %array = alloca [4 x i32], align 4
  %failed = alloca i32, align 4
  %s = alloca [31 x i8], align 1
  store %Account zeroinitializer, %Account* %a, align 4
  call void @Account.deposit(%Account* %a, i32 100)
  %e = getelementptr inbounds [4 x i32], [4 x i32]* %array, i32 0, i32 0
  store i32 30, i32* %e, align 4
  %e1 = getelementptr inbounds [4 x i32], [4 x i32]* %array, i32 0, i32 1
  store i32 50, i32* %e1, align 4
  %e2 = getelementptr inbounds [4 x i32], [4 x i32]* %array, i32 0, i32 2
  store i32 40, i32* %e2, align 4
  %e3 = getelementptr inbounds [4 x i32], [4 x i32]* %array, i32 0, i32 3
  store i32 10, i32* %e3, align 4
  %array4 = getelementptr inbounds [4 x i32], [4 x i32]* %array, i32 0, i32 0
  store i32* %array4, i32** %bills, align 8
  %load.bills = load i32*, i32** %bills, align 8
  %"pay_all(&a,load.bills,4)" = call i32 @pay_all(%Account* %a, i32* %load.bills, i32 4)
  store i32 %"pay_all(&a,load.bills,4)", i32* %failed, align 4
  %dst = getelementptr inbounds [31 x i8], [31 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([31 x i8], [31 x i8]* @str, i32 0, i32 0), i64 31, i1 false)
  %load.a.balance = getelementptr inbounds %Account, %Account* %a, i32 0, i32 0
  %load.load.a.balance = load i32, i32* %load.a.balance, align 4
  %load.a.ops = getelementptr inbounds %Account, %Account* %a, i32 0, i32 1
  %load.load.a.ops = load i32, i32* %load.a.ops, align 4
  %load.failed = load i32, i32* %failed, align 4
  %"printf(str,load.load.a.balance,load.load.a.ops,load.failed)" = call i32 (i8*, ...) @printf(i8* %dst, i32 %load.load.a.balance, i32 %load.load.a.ops, i32 %load.failed)
  %load.a.balance5 = getelementptr inbounds %Account, %Account* %a, i32 0, i32 0
  %load.load.a.balance6 = load i32, i32* %load.a.balance5, align 4
  ret i32 %load.load.a.balance6
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 036 — program: an array of structs is a copy

```ura
// expect: 10   (1 + 7 + 2: q itself stays 2)
struct Point:
    x i32
    y i32

fn main() i32:
    p Point
    q Point
    p.x = 1
    q.x = 2
    ps Point[] = [p, q]
    ps[1].x = 7
    return ps[0].x + ps[1].x + q.x
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Point = type { i32, i32 }

define i32 @main() {
entry:
  %p = alloca %Point, align 8
  %q = alloca %Point, align 8
  %ps = alloca %Point*, align 8
  %array = alloca [2 x %Point], align 8
  store %Point zeroinitializer, %Point* %p, align 4
  store %Point zeroinitializer, %Point* %q, align 4
  %load.p.x = getelementptr inbounds %Point, %Point* %p, i32 0, i32 0
  store i32 1, i32* %load.p.x, align 4
  %load.q.x = getelementptr inbounds %Point, %Point* %q, i32 0, i32 0
  store i32 2, i32* %load.q.x, align 4
  %load.p = load %Point, %Point* %p, align 4
  %load.q = load %Point, %Point* %q, align 4
  %e = getelementptr inbounds [2 x %Point], [2 x %Point]* %array, i32 0, i32 0
  store %Point %load.p, %Point* %e, align 4
  %e1 = getelementptr inbounds [2 x %Point], [2 x %Point]* %array, i32 0, i32 1
  store %Point %load.q, %Point* %e1, align 4
  %array2 = getelementptr inbounds [2 x %Point], [2 x %Point]* %array, i32 0, i32 0
  store %Point* %array2, %Point** %ps, align 8
  %load.ps = load %Point*, %Point** %ps, align 8
  %"load.ps[1]" = getelementptr inbounds %Point, %Point* %load.ps, i32 1
  %"load.load.ps[1].x" = getelementptr inbounds %Point, %Point* %"load.ps[1]", i32 0, i32 0
  store i32 7, i32* %"load.load.ps[1].x", align 4
  %load.ps3 = load %Point*, %Point** %ps, align 8
  %"load.ps[0]" = getelementptr inbounds %Point, %Point* %load.ps3, i32 0
  %"load.load.ps[0].x" = getelementptr inbounds %Point, %Point* %"load.ps[0]", i32 0, i32 0
  %"load.load.load.ps[0].x" = load i32, i32* %"load.load.ps[0].x", align 4
  %load.ps4 = load %Point*, %Point** %ps, align 8
  %"load.ps[1]5" = getelementptr inbounds %Point, %Point* %load.ps4, i32 1
  %"load.load.ps[1].x6" = getelementptr inbounds %Point, %Point* %"load.ps[1]5", i32 0, i32 0
  %"load.load.load.ps[1].x" = load i32, i32* %"load.load.ps[1].x6", align 4
  %ADD = add i32 %"load.load.load.ps[0].x", %"load.load.load.ps[1].x"
  %load.q.x7 = getelementptr inbounds %Point, %Point* %q, i32 0, i32 0
  %load.load.q.x = load i32, i32* %load.q.x7, align 4
  %ADD8 = add i32 %ADD, %load.load.q.x
  ret i32 %ADD8
}
```

---

## 037 — program: vectors: struct parameters, struct returns, chained calls

```ura
// expect: 27   (c = (7, 10), c . a = 7 + 20)
struct Vec:
    x i32
    y i32

    fn add(o Vec) Vec:
        r Vec
        r.x = self.x + o.x
        r.y = self.y + o.y
        return r

    fn dot(o Vec) i32:
        return self.x * o.x + self.y * o.y

fn make(x i32, y i32) Vec:
    v Vec
    v.x = x
    v.y = y
    return v

fn main() i32:
    a Vec = make(1, 2)
    b Vec = make(3, 4)
    c Vec = a.add(b).add(b)
    return c.dot(a)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Vec = type { i32, i32 }

define %Vec @Vec.add(%Vec* %0, %Vec %1) {
entry:
  %self = alloca %Vec*, align 8
  %o = alloca %Vec, align 8
  %r = alloca %Vec, align 8
  store %Vec* %0, %Vec** %self, align 8
  store %Vec %1, %Vec* %o, align 4
  store %Vec zeroinitializer, %Vec* %r, align 4
  %load.r.x = getelementptr inbounds %Vec, %Vec* %r, i32 0, i32 0
  %dref.self = load %Vec*, %Vec** %self, align 8
  %load.dref.self.x = getelementptr inbounds %Vec, %Vec* %dref.self, i32 0, i32 0
  %load.load.dref.self.x = load i32, i32* %load.dref.self.x, align 4
  %load.o.x = getelementptr inbounds %Vec, %Vec* %o, i32 0, i32 0
  %load.load.o.x = load i32, i32* %load.o.x, align 4
  %ADD = add i32 %load.load.dref.self.x, %load.load.o.x
  store i32 %ADD, i32* %load.r.x, align 4
  %load.r.y = getelementptr inbounds %Vec, %Vec* %r, i32 0, i32 1
  %dref.self1 = load %Vec*, %Vec** %self, align 8
  %load.dref.self.y = getelementptr inbounds %Vec, %Vec* %dref.self1, i32 0, i32 1
  %load.load.dref.self.y = load i32, i32* %load.dref.self.y, align 4
  %load.o.y = getelementptr inbounds %Vec, %Vec* %o, i32 0, i32 1
  %load.load.o.y = load i32, i32* %load.o.y, align 4
  %ADD2 = add i32 %load.load.dref.self.y, %load.load.o.y
  store i32 %ADD2, i32* %load.r.y, align 4
  %load.r = load %Vec, %Vec* %r, align 4
  ret %Vec %load.r
}

define i32 @Vec.dot(%Vec* %0, %Vec %1) {
entry:
  %self = alloca %Vec*, align 8
  %o = alloca %Vec, align 8
  store %Vec* %0, %Vec** %self, align 8
  store %Vec %1, %Vec* %o, align 4
  %dref.self = load %Vec*, %Vec** %self, align 8
  %load.dref.self.x = getelementptr inbounds %Vec, %Vec* %dref.self, i32 0, i32 0
  %load.load.dref.self.x = load i32, i32* %load.dref.self.x, align 4
  %load.o.x = getelementptr inbounds %Vec, %Vec* %o, i32 0, i32 0
  %load.load.o.x = load i32, i32* %load.o.x, align 4
  %MUL = mul i32 %load.load.dref.self.x, %load.load.o.x
  %dref.self1 = load %Vec*, %Vec** %self, align 8
  %load.dref.self.y = getelementptr inbounds %Vec, %Vec* %dref.self1, i32 0, i32 1
  %load.load.dref.self.y = load i32, i32* %load.dref.self.y, align 4
  %load.o.y = getelementptr inbounds %Vec, %Vec* %o, i32 0, i32 1
  %load.load.o.y = load i32, i32* %load.o.y, align 4
  %MUL2 = mul i32 %load.load.dref.self.y, %load.load.o.y
  %ADD = add i32 %MUL, %MUL2
  ret i32 %ADD
}

define %Vec @make(i32 %0, i32 %1) {
entry:
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  %v = alloca %Vec, align 8
  store i32 %0, i32* %x, align 4
  store i32 %1, i32* %y, align 4
  store %Vec zeroinitializer, %Vec* %v, align 4
  %load.v.x = getelementptr inbounds %Vec, %Vec* %v, i32 0, i32 0
  %load.x = load i32, i32* %x, align 4
  store i32 %load.x, i32* %load.v.x, align 4
  %load.v.y = getelementptr inbounds %Vec, %Vec* %v, i32 0, i32 1
  %load.y = load i32, i32* %y, align 4
  store i32 %load.y, i32* %load.v.y, align 4
  %load.v = load %Vec, %Vec* %v, align 4
  ret %Vec %load.v
}

define i32 @main() {
entry:
  %a = alloca %Vec, align 8
  %b = alloca %Vec, align 8
  %c = alloca %Vec, align 8
  %tmp = alloca %Vec, align 8
  %"make(1,2)" = call %Vec @make(i32 1, i32 2)
  store %Vec %"make(1,2)", %Vec* %a, align 4
  %"make(3,4)" = call %Vec @make(i32 3, i32 4)
  store %Vec %"make(3,4)", %Vec* %b, align 4
  %load.b = load %Vec, %Vec* %b, align 4
  %"add(&load.a,load.b)" = call %Vec @Vec.add(%Vec* %a, %Vec %load.b)
  store %Vec %"add(&load.a,load.b)", %Vec* %tmp, align 4
  %load.b1 = load %Vec, %Vec* %b, align 4
  %"add(&add(&load.a,load.b),load.b)" = call %Vec @Vec.add(%Vec* %tmp, %Vec %load.b1)
  store %Vec %"add(&add(&load.a,load.b),load.b)", %Vec* %c, align 4
  %load.a = load %Vec, %Vec* %a, align 4
  %"dot(&load.c,load.a)" = call i32 @Vec.dot(%Vec* %c, %Vec %load.a)
  ret i32 %"dot(&load.c,load.a)"
}
```

---

## 038 — program: linked list: ref attributes, isNullable, rebinding a ref in a loop

```ura
// expect: 36   (1 + 2 + 3, plus 3 * 10)
struct Node:
    value i32
    next &Node

fn main() i32:
    c Node
    c.value = 3 // c.next is never set: it starts null
    b Node
    b.value = 2
    b.next = &c
    a Node
    a.value = 1
    a.next = &b
    sum i32 = 0
    p &Node = &a
    while not isNullable(p):
        sum += p.value
        p = &p.next
    return sum + a.next.next.value * 10
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Node = type { i32, %Node* }

define i32 @main() {
entry:
  %c = alloca %Node, align 8
  %b = alloca %Node, align 8
  %a = alloca %Node, align 8
  %sum = alloca i32, align 4
  %p = alloca %Node*, align 8
  store %Node zeroinitializer, %Node* %c, align 8
  %load.c.value = getelementptr inbounds %Node, %Node* %c, i32 0, i32 0
  store i32 3, i32* %load.c.value, align 4
  store %Node zeroinitializer, %Node* %b, align 8
  %load.b.value = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  store i32 2, i32* %load.b.value, align 4
  %load.b.next = getelementptr inbounds %Node, %Node* %b, i32 0, i32 1
  store %Node* %c, %Node** %load.b.next, align 8
  store %Node zeroinitializer, %Node* %a, align 8
  %load.a.value = getelementptr inbounds %Node, %Node* %a, i32 0, i32 0
  store i32 1, i32* %load.a.value, align 4
  %load.a.next = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  store %Node* %b, %Node** %load.a.next, align 8
  store i32 0, i32* %sum, align 4
  store %Node* %a, %Node** %p, align 8
  br label %cond

cond:                                             ; preds = %then, %entry
  %dref.p = load %Node*, %Node** %p, align 8
  %"isNullable(dref.p)" = icmp eq %Node* %dref.p, null
  %NOT = xor i1 %"isNullable(dref.p)", true
  br i1 %NOT, label %then, label %endwhile

then:                                             ; preds = %cond
  %dref.p1 = load %Node*, %Node** %p, align 8
  %load.dref.p.value = getelementptr inbounds %Node, %Node* %dref.p1, i32 0, i32 0
  %load.load.dref.p.value = load i32, i32* %load.dref.p.value, align 4
  %old = load i32, i32* %sum, align 4
  %ADD_ASSIGN = add i32 %old, %load.load.dref.p.value
  store i32 %ADD_ASSIGN, i32* %sum, align 4
  %dref.p2 = load %Node*, %Node** %p, align 8
  %load.dref.p.next = getelementptr inbounds %Node, %Node* %dref.p2, i32 0, i32 1
  %dref.load.dref.p.next = load %Node*, %Node** %load.dref.p.next, align 8
  store %Node* %dref.load.dref.p.next, %Node** %p, align 8
  br label %cond

endwhile:                                         ; preds = %cond
  %load.sum = load i32, i32* %sum, align 4
  %load.a.next3 = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  %dref.load.a.next = load %Node*, %Node** %load.a.next3, align 8
  %load.dref.load.a.next.next = getelementptr inbounds %Node, %Node* %dref.load.a.next, i32 0, i32 1
  %dref.load.dref.load.a.next.next = load %Node*, %Node** %load.dref.load.a.next.next, align 8
  %load.dref.load.dref.load.a.next.next.value = getelementptr inbounds %Node, %Node* %dref.load.dref.load.a.next.next, i32 0, i32 0
  %load.load.dref.load.dref.load.a.next.next.value = load i32, i32* %load.dref.load.dref.load.a.next.next.value, align 4
  %MUL = mul i32 %load.load.dref.load.dref.load.a.next.next.value, 10
  %ADD = add i32 %load.sum, %MUL
  ret i32 %ADD
}
```

---

## 039 — program: a recursive method that counts its calls

```ura
// expect: 125   (5! = 120, plus 5 calls)
struct Math:
    calls i32

    fn fact(n i32) i32:
        self.calls += 1
        if n <= 1: return 1
        return n * self.fact(n - 1)

fn main() i32:
    m Math
    r i32 = m.fact(5)
    return r + m.calls
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Math = type { i32 }

define i32 @Math.fact(%Math* %0, i32 %1) {
entry:
  %self = alloca %Math*, align 8
  %n = alloca i32, align 4
  store %Math* %0, %Math** %self, align 8
  store i32 %1, i32* %n, align 4
  %dref.self = load %Math*, %Math** %self, align 8
  %load.dref.self.calls = getelementptr inbounds %Math, %Math* %dref.self, i32 0, i32 0
  %old = load i32, i32* %load.dref.self.calls, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %load.dref.self.calls, align 4
  %load.n = load i32, i32* %n, align 4
  %LE = icmp sle i32 %load.n, 1
  br i1 %LE, label %then, label %endif

then:                                             ; preds = %entry
  ret i32 1

endif:                                            ; preds = %entry
  %load.n1 = load i32, i32* %n, align 4
  %dref.self2 = load %Math*, %Math** %self, align 8
  %load.n3 = load i32, i32* %n, align 4
  %SUB = sub i32 %load.n3, 1
  %"fact(&load.dref.self,load.n - 1)" = call i32 @Math.fact(%Math* %dref.self2, i32 %SUB)
  %MUL = mul i32 %load.n1, %"fact(&load.dref.self,load.n - 1)"
  ret i32 %MUL
}

define i32 @main() {
entry:
  %m = alloca %Math, align 8
  %r = alloca i32, align 4
  store %Math zeroinitializer, %Math* %m, align 4
  %"fact(&load.m,5)" = call i32 @Math.fact(%Math* %m, i32 5)
  store i32 %"fact(&load.m,5)", i32* %r, align 4
  %load.r = load i32, i32* %r, align 4
  %load.m.calls = getelementptr inbounds %Math, %Math* %m, i32 0, i32 0
  %load.load.m.calls = load i32, i32* %load.m.calls, align 4
  %ADD = add i32 %load.r, %load.load.m.calls
  ret i32 %ADD
}
```

---

## 040 — program: a method takes a ref to its own struct type

```ura
// expect: 100   (a = 10, b = 0)
struct Counter:
    n i32

    fn absorb(other &Counter):
        self.n += other.n
        other.n = 0

fn main() i32:
    a Counter
    b Counter
    a.n = 4
    b.n = 6
    a.absorb(&b)
    a.absorb(&b)
    return a.n * 10 + b.n
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Counter = type { i32 }

define void @Counter.absorb(%Counter* %0, %Counter* %1) {
entry:
  %self = alloca %Counter*, align 8
  %other = alloca %Counter*, align 8
  store %Counter* %0, %Counter** %self, align 8
  store %Counter* %1, %Counter** %other, align 8
  %dref.self = load %Counter*, %Counter** %self, align 8
  %load.dref.self.n = getelementptr inbounds %Counter, %Counter* %dref.self, i32 0, i32 0
  %dref.other = load %Counter*, %Counter** %other, align 8
  %load.dref.other.n = getelementptr inbounds %Counter, %Counter* %dref.other, i32 0, i32 0
  %load.load.dref.other.n = load i32, i32* %load.dref.other.n, align 4
  %old = load i32, i32* %load.dref.self.n, align 4
  %ADD_ASSIGN = add i32 %old, %load.load.dref.other.n
  store i32 %ADD_ASSIGN, i32* %load.dref.self.n, align 4
  %dref.other1 = load %Counter*, %Counter** %other, align 8
  %load.dref.other.n2 = getelementptr inbounds %Counter, %Counter* %dref.other1, i32 0, i32 0
  store i32 0, i32* %load.dref.other.n2, align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Counter, align 8
  %b = alloca %Counter, align 8
  store %Counter zeroinitializer, %Counter* %a, align 4
  store %Counter zeroinitializer, %Counter* %b, align 4
  %load.a.n = getelementptr inbounds %Counter, %Counter* %a, i32 0, i32 0
  store i32 4, i32* %load.a.n, align 4
  %load.b.n = getelementptr inbounds %Counter, %Counter* %b, i32 0, i32 0
  store i32 6, i32* %load.b.n, align 4
  call void @Counter.absorb(%Counter* %a, %Counter* %b)
  call void @Counter.absorb(%Counter* %a, %Counter* %b)
  %load.a.n1 = getelementptr inbounds %Counter, %Counter* %a, i32 0, i32 0
  %load.load.a.n = load i32, i32* %load.a.n1, align 4
  %MUL = mul i32 %load.load.a.n, 10
  %load.b.n2 = getelementptr inbounds %Counter, %Counter* %b, i32 0, i32 0
  %load.load.b.n = load i32, i32* %load.b.n2, align 4
  %ADD = add i32 %MUL, %load.load.b.n
  ret i32 %ADD
}
```

---

## 041 — program: stack: push squares, pop until empty

```ura
// expect: 173   (pops 25 16 9 4 1, last digits folded: ((((5*2+6)*2+9)*2+4)*2+1)
struct Stack:
    items i32[]
    top i32

    fn push(v i32):
        self.items[self.top] = v
        self.top += 1

    fn pop() i32:
        self.top -= 1
        return self.items[self.top]

    fn empty() bool:
        return self.top == 0

fn main() i32:
    s Stack
    s.items = [0, 0, 0, 0, 0, 0, 0, 0]
    i i32 = 1
    while i <= 5:
        s.push(i * i)
        i += 1
    total i32 = 0
    while not s.empty():
        total = total * 2 + s.pop() % 10
    return total
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Stack = type { i32*, i32 }

define void @Stack.push(%Stack* %0, i32 %1) {
entry:
  %self = alloca %Stack*, align 8
  %v = alloca i32, align 4
  store %Stack* %0, %Stack** %self, align 8
  store i32 %1, i32* %v, align 4
  %dref.self = load %Stack*, %Stack** %self, align 8
  %load.dref.self.items = getelementptr inbounds %Stack, %Stack* %dref.self, i32 0, i32 0
  %load.load.dref.self.items = load i32*, i32** %load.dref.self.items, align 8
  %dref.self1 = load %Stack*, %Stack** %self, align 8
  %load.dref.self.top = getelementptr inbounds %Stack, %Stack* %dref.self1, i32 0, i32 1
  %load.load.dref.self.top = load i32, i32* %load.dref.self.top, align 4
  %"load.load.dref.self.items[load.load.dref.self.top]" = getelementptr inbounds i32, i32* %load.load.dref.self.items, i32 %load.load.dref.self.top
  %load.v = load i32, i32* %v, align 4
  store i32 %load.v, i32* %"load.load.dref.self.items[load.load.dref.self.top]", align 4
  %dref.self2 = load %Stack*, %Stack** %self, align 8
  %load.dref.self.top3 = getelementptr inbounds %Stack, %Stack* %dref.self2, i32 0, i32 1
  %old = load i32, i32* %load.dref.self.top3, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %load.dref.self.top3, align 4
  ret void
}

define i32 @Stack.pop(%Stack* %0) {
entry:
  %self = alloca %Stack*, align 8
  store %Stack* %0, %Stack** %self, align 8
  %dref.self = load %Stack*, %Stack** %self, align 8
  %load.dref.self.top = getelementptr inbounds %Stack, %Stack* %dref.self, i32 0, i32 1
  %old = load i32, i32* %load.dref.self.top, align 4
  %SUB_ASSIGN = sub i32 %old, 1
  store i32 %SUB_ASSIGN, i32* %load.dref.self.top, align 4
  %dref.self1 = load %Stack*, %Stack** %self, align 8
  %load.dref.self.items = getelementptr inbounds %Stack, %Stack* %dref.self1, i32 0, i32 0
  %load.load.dref.self.items = load i32*, i32** %load.dref.self.items, align 8
  %dref.self2 = load %Stack*, %Stack** %self, align 8
  %load.dref.self.top3 = getelementptr inbounds %Stack, %Stack* %dref.self2, i32 0, i32 1
  %load.load.dref.self.top = load i32, i32* %load.dref.self.top3, align 4
  %"load.load.dref.self.items[load.load.dref.self.top]" = getelementptr inbounds i32, i32* %load.load.dref.self.items, i32 %load.load.dref.self.top
  %"load.load.load.dref.self.items[load.load.dref.self.top]" = load i32, i32* %"load.load.dref.self.items[load.load.dref.self.top]", align 4
  ret i32 %"load.load.load.dref.self.items[load.load.dref.self.top]"
}

define i1 @Stack.empty(%Stack* %0) {
entry:
  %self = alloca %Stack*, align 8
  store %Stack* %0, %Stack** %self, align 8
  %dref.self = load %Stack*, %Stack** %self, align 8
  %load.dref.self.top = getelementptr inbounds %Stack, %Stack* %dref.self, i32 0, i32 1
  %load.load.dref.self.top = load i32, i32* %load.dref.self.top, align 4
  %EQ = icmp eq i32 %load.load.dref.self.top, 0
  ret i1 %EQ
}

define i32 @main() {
entry:
  %s = alloca %Stack, align 8
  %array = alloca [8 x i32], align 4
  %i = alloca i32, align 4
  %total = alloca i32, align 4
  store %Stack zeroinitializer, %Stack* %s, align 8
  %load.s.items = getelementptr inbounds %Stack, %Stack* %s, i32 0, i32 0
  %e = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 0
  store i32 0, i32* %e, align 4
  %e1 = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 1
  store i32 0, i32* %e1, align 4
  %e2 = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 2
  store i32 0, i32* %e2, align 4
  %e3 = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 3
  store i32 0, i32* %e3, align 4
  %e4 = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 4
  store i32 0, i32* %e4, align 4
  %e5 = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 5
  store i32 0, i32* %e5, align 4
  %e6 = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 6
  store i32 0, i32* %e6, align 4
  %e7 = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 7
  store i32 0, i32* %e7, align 4
  %array8 = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 0
  store i32* %array8, i32** %load.s.items, align 8
  store i32 1, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %then, %entry
  %load.i = load i32, i32* %i, align 4
  %LE = icmp sle i32 %load.i, 5
  br i1 %LE, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.i9 = load i32, i32* %i, align 4
  %load.i10 = load i32, i32* %i, align 4
  %MUL = mul i32 %load.i9, %load.i10
  call void @Stack.push(%Stack* %s, i32 %MUL)
  %old = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  store i32 0, i32* %total, align 4
  br label %cond11

cond11:                                           ; preds = %then12, %endwhile
  %"empty(&load.s)" = call i1 @Stack.empty(%Stack* %s)
  %NOT = xor i1 %"empty(&load.s)", true
  br i1 %NOT, label %then12, label %endwhile13

then12:                                           ; preds = %cond11
  %load.total = load i32, i32* %total, align 4
  %MUL14 = mul i32 %load.total, 2
  %"pop(&load.s)" = call i32 @Stack.pop(%Stack* %s)
  %MOD = srem i32 %"pop(&load.s)", 10
  %ADD = add i32 %MUL14, %MOD
  store i32 %ADD, i32* %total, align 4
  br label %cond11

endwhile13:                                       ; preds = %cond11
  %load.total15 = load i32, i32* %total, align 4
  ret i32 %load.total15
}
```

---

## 042 — program: ring-buffer queue: full, wrap around, drain

```ura
// expect: 242   (first 1 * 100, sum 2 + 3 + 9 = 14 * 10, 2 dropped)
struct Queue:
    items i32[]
    head i32
    tail i32
    size i32
    cap i32

    fn put(v i32) bool:
        if self.size == self.cap: return False
        self.items[self.tail] = v
        self.tail = (self.tail + 1) % self.cap
        self.size += 1
        return True

    fn take() i32:
        v i32 = self.items[self.head]
        self.head = (self.head + 1) % self.cap
        self.size -= 1
        return v

fn main() i32:
    q Queue
    q.items = [0, 0, 0]
    q.cap = 3
    dropped i32 = 0
    i i32 = 1
    while i <= 5:
        if !q.put(i): dropped += 1
        i += 1
    first i32 = q.take()
    q.put(9)
    sum i32 = 0
    while q.size > 0:
        sum += q.take()
    return first * 100 + sum * 10 + dropped
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Queue = type { i32*, i32, i32, i32, i32 }

define i1 @Queue.put(%Queue* %0, i32 %1) {
entry:
  %self = alloca %Queue*, align 8
  %v = alloca i32, align 4
  store %Queue* %0, %Queue** %self, align 8
  store i32 %1, i32* %v, align 4
  %dref.self = load %Queue*, %Queue** %self, align 8
  %load.dref.self.size = getelementptr inbounds %Queue, %Queue* %dref.self, i32 0, i32 3
  %load.load.dref.self.size = load i32, i32* %load.dref.self.size, align 4
  %dref.self1 = load %Queue*, %Queue** %self, align 8
  %load.dref.self.cap = getelementptr inbounds %Queue, %Queue* %dref.self1, i32 0, i32 4
  %load.load.dref.self.cap = load i32, i32* %load.dref.self.cap, align 4
  %EQ = icmp eq i32 %load.load.dref.self.size, %load.load.dref.self.cap
  br i1 %EQ, label %then, label %endif

then:                                             ; preds = %entry
  ret i1 false

endif:                                            ; preds = %entry
  %dref.self2 = load %Queue*, %Queue** %self, align 8
  %load.dref.self.items = getelementptr inbounds %Queue, %Queue* %dref.self2, i32 0, i32 0
  %load.load.dref.self.items = load i32*, i32** %load.dref.self.items, align 8
  %dref.self3 = load %Queue*, %Queue** %self, align 8
  %load.dref.self.tail = getelementptr inbounds %Queue, %Queue* %dref.self3, i32 0, i32 2
  %load.load.dref.self.tail = load i32, i32* %load.dref.self.tail, align 4
  %"load.load.dref.self.items[load.load.dref.self.tail]" = getelementptr inbounds i32, i32* %load.load.dref.self.items, i32 %load.load.dref.self.tail
  %load.v = load i32, i32* %v, align 4
  store i32 %load.v, i32* %"load.load.dref.self.items[load.load.dref.self.tail]", align 4
  %dref.self4 = load %Queue*, %Queue** %self, align 8
  %load.dref.self.tail5 = getelementptr inbounds %Queue, %Queue* %dref.self4, i32 0, i32 2
  %dref.self6 = load %Queue*, %Queue** %self, align 8
  %load.dref.self.tail7 = getelementptr inbounds %Queue, %Queue* %dref.self6, i32 0, i32 2
  %load.load.dref.self.tail8 = load i32, i32* %load.dref.self.tail7, align 4
  %ADD = add i32 %load.load.dref.self.tail8, 1
  %dref.self9 = load %Queue*, %Queue** %self, align 8
  %load.dref.self.cap10 = getelementptr inbounds %Queue, %Queue* %dref.self9, i32 0, i32 4
  %load.load.dref.self.cap11 = load i32, i32* %load.dref.self.cap10, align 4
  %MOD = srem i32 %ADD, %load.load.dref.self.cap11
  store i32 %MOD, i32* %load.dref.self.tail5, align 4
  %dref.self12 = load %Queue*, %Queue** %self, align 8
  %load.dref.self.size13 = getelementptr inbounds %Queue, %Queue* %dref.self12, i32 0, i32 3
  %old = load i32, i32* %load.dref.self.size13, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %load.dref.self.size13, align 4
  ret i1 true
}

define i32 @Queue.take(%Queue* %0) {
entry:
  %self = alloca %Queue*, align 8
  %v = alloca i32, align 4
  store %Queue* %0, %Queue** %self, align 8
  %dref.self = load %Queue*, %Queue** %self, align 8
  %load.dref.self.items = getelementptr inbounds %Queue, %Queue* %dref.self, i32 0, i32 0
  %load.load.dref.self.items = load i32*, i32** %load.dref.self.items, align 8
  %dref.self1 = load %Queue*, %Queue** %self, align 8
  %load.dref.self.head = getelementptr inbounds %Queue, %Queue* %dref.self1, i32 0, i32 1
  %load.load.dref.self.head = load i32, i32* %load.dref.self.head, align 4
  %"load.load.dref.self.items[load.load.dref.self.head]" = getelementptr inbounds i32, i32* %load.load.dref.self.items, i32 %load.load.dref.self.head
  %"load.load.load.dref.self.items[load.load.dref.self.head]" = load i32, i32* %"load.load.dref.self.items[load.load.dref.self.head]", align 4
  store i32 %"load.load.load.dref.self.items[load.load.dref.self.head]", i32* %v, align 4
  %dref.self2 = load %Queue*, %Queue** %self, align 8
  %load.dref.self.head3 = getelementptr inbounds %Queue, %Queue* %dref.self2, i32 0, i32 1
  %dref.self4 = load %Queue*, %Queue** %self, align 8
  %load.dref.self.head5 = getelementptr inbounds %Queue, %Queue* %dref.self4, i32 0, i32 1
  %load.load.dref.self.head6 = load i32, i32* %load.dref.self.head5, align 4
  %ADD = add i32 %load.load.dref.self.head6, 1
  %dref.self7 = load %Queue*, %Queue** %self, align 8
  %load.dref.self.cap = getelementptr inbounds %Queue, %Queue* %dref.self7, i32 0, i32 4
  %load.load.dref.self.cap = load i32, i32* %load.dref.self.cap, align 4
  %MOD = srem i32 %ADD, %load.load.dref.self.cap
  store i32 %MOD, i32* %load.dref.self.head3, align 4
  %dref.self8 = load %Queue*, %Queue** %self, align 8
  %load.dref.self.size = getelementptr inbounds %Queue, %Queue* %dref.self8, i32 0, i32 3
  %old = load i32, i32* %load.dref.self.size, align 4
  %SUB_ASSIGN = sub i32 %old, 1
  store i32 %SUB_ASSIGN, i32* %load.dref.self.size, align 4
  %load.v = load i32, i32* %v, align 4
  ret i32 %load.v
}

define i32 @main() {
entry:
  %q = alloca %Queue, align 8
  %array = alloca [3 x i32], align 4
  %dropped = alloca i32, align 4
  %i = alloca i32, align 4
  %first = alloca i32, align 4
  %sum = alloca i32, align 4
  store %Queue zeroinitializer, %Queue* %q, align 8
  %load.q.items = getelementptr inbounds %Queue, %Queue* %q, i32 0, i32 0
  %e = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32 0, i32* %e, align 4
  %e1 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 1
  store i32 0, i32* %e1, align 4
  %e2 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 2
  store i32 0, i32* %e2, align 4
  %array3 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32* %array3, i32** %load.q.items, align 8
  %load.q.cap = getelementptr inbounds %Queue, %Queue* %q, i32 0, i32 4
  store i32 3, i32* %load.q.cap, align 4
  store i32 0, i32* %dropped, align 4
  store i32 1, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.i = load i32, i32* %i, align 4
  %LE = icmp sle i32 %load.i, 5
  br i1 %LE, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.i5 = load i32, i32* %i, align 4
  %"put(&load.q,load.i)" = call i1 @Queue.put(%Queue* %q, i32 %load.i5)
  %NOT = xor i1 %"put(&load.q,load.i)", true
  br i1 %NOT, label %then4, label %endif

endwhile:                                         ; preds = %cond
  %"take(&load.q)" = call i32 @Queue.take(%Queue* %q)
  store i32 %"take(&load.q)", i32* %first, align 4
  %"put(&load.q,9)" = call i1 @Queue.put(%Queue* %q, i32 9)
  store i32 0, i32* %sum, align 4
  br label %cond8

then4:                                            ; preds = %then
  %old = load i32, i32* %dropped, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %dropped, align 4
  br label %endif

endif:                                            ; preds = %then4, %then
  %old6 = load i32, i32* %i, align 4
  %ADD_ASSIGN7 = add i32 %old6, 1
  store i32 %ADD_ASSIGN7, i32* %i, align 4
  br label %cond

cond8:                                            ; preds = %then9, %endwhile
  %load.q.size = getelementptr inbounds %Queue, %Queue* %q, i32 0, i32 3
  %load.load.q.size = load i32, i32* %load.q.size, align 4
  %GT = icmp sgt i32 %load.load.q.size, 0
  br i1 %GT, label %then9, label %endwhile10

then9:                                            ; preds = %cond8
  %"take(&load.q)11" = call i32 @Queue.take(%Queue* %q)
  %old12 = load i32, i32* %sum, align 4
  %ADD_ASSIGN13 = add i32 %old12, %"take(&load.q)11"
  store i32 %ADD_ASSIGN13, i32* %sum, align 4
  br label %cond8

endwhile10:                                       ; preds = %cond8
  %load.first = load i32, i32* %first, align 4
  %MUL = mul i32 %load.first, 100
  %load.sum = load i32, i32* %sum, align 4
  %MUL14 = mul i32 %load.sum, 10
  %ADD = add i32 %MUL, %MUL14
  %load.dropped = load i32, i32* %dropped, align 4
  %ADD15 = add i32 %ADD, %load.dropped
  ret i32 %ADD15
}
```

---

## 043 — program: binary tree: recursion through ref attributes

```ura
// expect: 103   (sum 10 * 10, depth 3)
struct Tree:
    value i32
    left &Tree
    right &Tree

fn sum(t &Tree) i32:
    if isNullable(t): return 0
    return t.value + sum(&t.left) + sum(&t.right)

fn depth(t &Tree) i32:
    if isNullable(t): return 0
    l i32 = depth(&t.left)
    r i32 = depth(&t.right)
    if l > r: return l + 1
    return r + 1

fn main() i32:
    a Tree
    b Tree
    c Tree
    d Tree
    a.value = 1
    b.value = 2
    c.value = 3
    d.value = 4
    a.left = &b
    a.right = &c
    b.left = &d
    return sum(&a) * 10 + depth(&a)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Tree = type { i32, %Tree*, %Tree* }

define i32 @sum(%Tree* %0) {
entry:
  %t = alloca %Tree*, align 8
  store %Tree* %0, %Tree** %t, align 8
  %dref.t = load %Tree*, %Tree** %t, align 8
  %"isNullable(dref.t)" = icmp eq %Tree* %dref.t, null
  br i1 %"isNullable(dref.t)", label %then, label %endif

then:                                             ; preds = %entry
  ret i32 0

endif:                                            ; preds = %entry
  %dref.t1 = load %Tree*, %Tree** %t, align 8
  %load.dref.t.value = getelementptr inbounds %Tree, %Tree* %dref.t1, i32 0, i32 0
  %load.load.dref.t.value = load i32, i32* %load.dref.t.value, align 4
  %dref.t2 = load %Tree*, %Tree** %t, align 8
  %load.dref.t.left = getelementptr inbounds %Tree, %Tree* %dref.t2, i32 0, i32 1
  %dref.load.dref.t.left = load %Tree*, %Tree** %load.dref.t.left, align 8
  %"sum(&dref.load.dref.t.left)" = call i32 @sum(%Tree* %dref.load.dref.t.left)
  %ADD = add i32 %load.load.dref.t.value, %"sum(&dref.load.dref.t.left)"
  %dref.t3 = load %Tree*, %Tree** %t, align 8
  %load.dref.t.right = getelementptr inbounds %Tree, %Tree* %dref.t3, i32 0, i32 2
  %dref.load.dref.t.right = load %Tree*, %Tree** %load.dref.t.right, align 8
  %"sum(&dref.load.dref.t.right)" = call i32 @sum(%Tree* %dref.load.dref.t.right)
  %ADD4 = add i32 %ADD, %"sum(&dref.load.dref.t.right)"
  ret i32 %ADD4
}

define i32 @depth(%Tree* %0) {
entry:
  %t = alloca %Tree*, align 8
  %l = alloca i32, align 4
  %r = alloca i32, align 4
  store %Tree* %0, %Tree** %t, align 8
  %dref.t = load %Tree*, %Tree** %t, align 8
  %"isNullable(dref.t)" = icmp eq %Tree* %dref.t, null
  br i1 %"isNullable(dref.t)", label %then, label %endif

then:                                             ; preds = %entry
  ret i32 0

endif:                                            ; preds = %entry
  %dref.t1 = load %Tree*, %Tree** %t, align 8
  %load.dref.t.left = getelementptr inbounds %Tree, %Tree* %dref.t1, i32 0, i32 1
  %dref.load.dref.t.left = load %Tree*, %Tree** %load.dref.t.left, align 8
  %"depth(&dref.load.dref.t.left)" = call i32 @depth(%Tree* %dref.load.dref.t.left)
  store i32 %"depth(&dref.load.dref.t.left)", i32* %l, align 4
  %dref.t2 = load %Tree*, %Tree** %t, align 8
  %load.dref.t.right = getelementptr inbounds %Tree, %Tree* %dref.t2, i32 0, i32 2
  %dref.load.dref.t.right = load %Tree*, %Tree** %load.dref.t.right, align 8
  %"depth(&dref.load.dref.t.right)" = call i32 @depth(%Tree* %dref.load.dref.t.right)
  store i32 %"depth(&dref.load.dref.t.right)", i32* %r, align 4
  %load.l = load i32, i32* %l, align 4
  %load.r = load i32, i32* %r, align 4
  %GT = icmp sgt i32 %load.l, %load.r
  br i1 %GT, label %then4, label %endif3

then4:                                            ; preds = %endif
  %load.l5 = load i32, i32* %l, align 4
  %ADD = add i32 %load.l5, 1
  ret i32 %ADD

endif3:                                           ; preds = %endif
  %load.r6 = load i32, i32* %r, align 4
  %ADD7 = add i32 %load.r6, 1
  ret i32 %ADD7
}

define i32 @main() {
entry:
  %a = alloca %Tree, align 8
  %b = alloca %Tree, align 8
  %c = alloca %Tree, align 8
  %d = alloca %Tree, align 8
  store %Tree zeroinitializer, %Tree* %a, align 8
  store %Tree zeroinitializer, %Tree* %b, align 8
  store %Tree zeroinitializer, %Tree* %c, align 8
  store %Tree zeroinitializer, %Tree* %d, align 8
  %load.a.value = getelementptr inbounds %Tree, %Tree* %a, i32 0, i32 0
  store i32 1, i32* %load.a.value, align 4
  %load.b.value = getelementptr inbounds %Tree, %Tree* %b, i32 0, i32 0
  store i32 2, i32* %load.b.value, align 4
  %load.c.value = getelementptr inbounds %Tree, %Tree* %c, i32 0, i32 0
  store i32 3, i32* %load.c.value, align 4
  %load.d.value = getelementptr inbounds %Tree, %Tree* %d, i32 0, i32 0
  store i32 4, i32* %load.d.value, align 4
  %load.a.left = getelementptr inbounds %Tree, %Tree* %a, i32 0, i32 1
  store %Tree* %b, %Tree** %load.a.left, align 8
  %load.a.right = getelementptr inbounds %Tree, %Tree* %a, i32 0, i32 2
  store %Tree* %c, %Tree** %load.a.right, align 8
  %load.b.left = getelementptr inbounds %Tree, %Tree* %b, i32 0, i32 1
  store %Tree* %d, %Tree** %load.b.left, align 8
  %"sum(&a)" = call i32 @sum(%Tree* %a)
  %MUL = mul i32 %"sum(&a)", 10
  %"depth(&a)" = call i32 @depth(%Tree* %a)
  %ADD = add i32 %MUL, %"depth(&a)"
  ret i32 %ADD
}
```

---

## 044 — program: a linked list over array elements

```ura
// expect: 33   (length 3, plus the third value 30)
struct Node:
    value i32
    next &Node

fn length(n &Node) i32:
    if isNullable(n): return 0
    return 1 + length(&n.next)

fn main() i32:
    a Node
    b Node
    c Node
    nodes Node[] = [a, b, c]
    i i32 = 0
    while i < 3:
        nodes[i].value = (i + 1) * 10
        if i < 2: nodes[i].next = &nodes[i + 1]
        i += 1
    return length(&nodes[0]) + nodes[0].next.next.value
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Node = type { i32, %Node* }

define i32 @length(%Node* %0) {
entry:
  %n = alloca %Node*, align 8
  store %Node* %0, %Node** %n, align 8
  %dref.n = load %Node*, %Node** %n, align 8
  %"isNullable(dref.n)" = icmp eq %Node* %dref.n, null
  br i1 %"isNullable(dref.n)", label %then, label %endif

then:                                             ; preds = %entry
  ret i32 0

endif:                                            ; preds = %entry
  %dref.n1 = load %Node*, %Node** %n, align 8
  %load.dref.n.next = getelementptr inbounds %Node, %Node* %dref.n1, i32 0, i32 1
  %dref.load.dref.n.next = load %Node*, %Node** %load.dref.n.next, align 8
  %"length(&dref.load.dref.n.next)" = call i32 @length(%Node* %dref.load.dref.n.next)
  %ADD = add i32 1, %"length(&dref.load.dref.n.next)"
  ret i32 %ADD
}

define i32 @main() {
entry:
  %a = alloca %Node, align 8
  %b = alloca %Node, align 8
  %c = alloca %Node, align 8
  %nodes = alloca %Node*, align 8
  %array = alloca [3 x %Node], align 8
  %i = alloca i32, align 4
  store %Node zeroinitializer, %Node* %a, align 8
  store %Node zeroinitializer, %Node* %b, align 8
  store %Node zeroinitializer, %Node* %c, align 8
  %load.a = load %Node, %Node* %a, align 8
  %load.b = load %Node, %Node* %b, align 8
  %load.c = load %Node, %Node* %c, align 8
  %e = getelementptr inbounds [3 x %Node], [3 x %Node]* %array, i32 0, i32 0
  store %Node %load.a, %Node* %e, align 8
  %e1 = getelementptr inbounds [3 x %Node], [3 x %Node]* %array, i32 0, i32 1
  store %Node %load.b, %Node* %e1, align 8
  %e2 = getelementptr inbounds [3 x %Node], [3 x %Node]* %array, i32 0, i32 2
  store %Node %load.c, %Node* %e2, align 8
  %array3 = getelementptr inbounds [3 x %Node], [3 x %Node]* %array, i32 0, i32 0
  store %Node* %array3, %Node** %nodes, align 8
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 3
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.nodes = load %Node*, %Node** %nodes, align 8
  %load.i4 = load i32, i32* %i, align 4
  %"load.nodes[load.i]" = getelementptr inbounds %Node, %Node* %load.nodes, i32 %load.i4
  %"load.load.nodes[load.i].value" = getelementptr inbounds %Node, %Node* %"load.nodes[load.i]", i32 0, i32 0
  %load.i5 = load i32, i32* %i, align 4
  %ADD = add i32 %load.i5, 1
  %MUL = mul i32 %ADD, 10
  store i32 %MUL, i32* %"load.load.nodes[load.i].value", align 4
  %load.i7 = load i32, i32* %i, align 4
  %LT8 = icmp slt i32 %load.i7, 2
  br i1 %LT8, label %then6, label %endif

endwhile:                                         ; preds = %cond
  %load.nodes15 = load %Node*, %Node** %nodes, align 8
  %"load.nodes[0]" = getelementptr inbounds %Node, %Node* %load.nodes15, i32 0
  %"length(&load.nodes[0])" = call i32 @length(%Node* %"load.nodes[0]")
  %load.nodes16 = load %Node*, %Node** %nodes, align 8
  %"load.nodes[0]17" = getelementptr inbounds %Node, %Node* %load.nodes16, i32 0
  %"load.load.nodes[0].next" = getelementptr inbounds %Node, %Node* %"load.nodes[0]17", i32 0, i32 1
  %"dref.load.load.nodes[0].next" = load %Node*, %Node** %"load.load.nodes[0].next", align 8
  %"load.dref.load.load.nodes[0].next.next" = getelementptr inbounds %Node, %Node* %"dref.load.load.nodes[0].next", i32 0, i32 1
  %"dref.load.dref.load.load.nodes[0].next.next" = load %Node*, %Node** %"load.dref.load.load.nodes[0].next.next", align 8
  %"load.dref.load.dref.load.load.nodes[0].next.next.value" = getelementptr inbounds %Node, %Node* %"dref.load.dref.load.load.nodes[0].next.next", i32 0, i32 0
  %"load.load.dref.load.dref.load.load.nodes[0].next.next.value" = load i32, i32* %"load.dref.load.dref.load.load.nodes[0].next.next.value", align 4
  %ADD18 = add i32 %"length(&load.nodes[0])", %"load.load.dref.load.dref.load.load.nodes[0].next.next.value"
  ret i32 %ADD18

then6:                                            ; preds = %then
  %load.nodes9 = load %Node*, %Node** %nodes, align 8
  %load.i10 = load i32, i32* %i, align 4
  %"load.nodes[load.i]11" = getelementptr inbounds %Node, %Node* %load.nodes9, i32 %load.i10
  %"load.load.nodes[load.i].next" = getelementptr inbounds %Node, %Node* %"load.nodes[load.i]11", i32 0, i32 1
  %load.nodes12 = load %Node*, %Node** %nodes, align 8
  %load.i13 = load i32, i32* %i, align 4
  %ADD14 = add i32 %load.i13, 1
  %"load.nodes[load.i + 1]" = getelementptr inbounds %Node, %Node* %load.nodes12, i32 %ADD14
  store %Node* %"load.nodes[load.i + 1]", %Node** %"load.load.nodes[load.i].next", align 8
  br label %endif

endif:                                            ; preds = %then6, %then
  %old = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  br label %cond
}
```

---

## 045 — program: search a linked list, -1 when missing

```ura
// expect: 121   (13 at 2 * 10, 8 at 1, 99 missing: 100)
struct Node:
    value i32
    next &Node

fn find(head &Node, v i32) i32:
    i i32 = 0
    p &Node = &head
    while !isNullable(p):
        if p.value == v: return i
        p = &p.next
        i += 1
    return -1

fn main() i32:
    a Node
    b Node
    c Node
    a.value = 5
    b.value = 8
    c.value = 13
    a.next = &b
    b.next = &c
    r i32 = find(&a, 13) * 10 + find(&a, 8)
    if find(&a, 99) == -1: r += 100
    return r
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Node = type { i32, %Node* }

define i32 @find(%Node* %0, i32 %1) {
entry:
  %head = alloca %Node*, align 8
  %v = alloca i32, align 4
  %i = alloca i32, align 4
  %p = alloca %Node*, align 8
  store %Node* %0, %Node** %head, align 8
  store i32 %1, i32* %v, align 4
  store i32 0, i32* %i, align 4
  %dref.head = load %Node*, %Node** %head, align 8
  store %Node* %dref.head, %Node** %p, align 8
  br label %cond

cond:                                             ; preds = %endif, %entry
  %dref.p = load %Node*, %Node** %p, align 8
  %"isNullable(dref.p)" = icmp eq %Node* %dref.p, null
  %NOT = xor i1 %"isNullable(dref.p)", true
  br i1 %NOT, label %then, label %endwhile

then:                                             ; preds = %cond
  %dref.p2 = load %Node*, %Node** %p, align 8
  %load.dref.p.value = getelementptr inbounds %Node, %Node* %dref.p2, i32 0, i32 0
  %load.load.dref.p.value = load i32, i32* %load.dref.p.value, align 4
  %load.v = load i32, i32* %v, align 4
  %EQ = icmp eq i32 %load.load.dref.p.value, %load.v
  br i1 %EQ, label %then1, label %endif

endwhile:                                         ; preds = %cond
  ret i32 -1

then1:                                            ; preds = %then
  %load.i = load i32, i32* %i, align 4
  ret i32 %load.i

endif:                                            ; preds = %then
  %dref.p3 = load %Node*, %Node** %p, align 8
  %load.dref.p.next = getelementptr inbounds %Node, %Node* %dref.p3, i32 0, i32 1
  %dref.load.dref.p.next = load %Node*, %Node** %load.dref.p.next, align 8
  store %Node* %dref.load.dref.p.next, %Node** %p, align 8
  %old = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  br label %cond
}

define i32 @main() {
entry:
  %a = alloca %Node, align 8
  %b = alloca %Node, align 8
  %c = alloca %Node, align 8
  %r = alloca i32, align 4
  store %Node zeroinitializer, %Node* %a, align 8
  store %Node zeroinitializer, %Node* %b, align 8
  store %Node zeroinitializer, %Node* %c, align 8
  %load.a.value = getelementptr inbounds %Node, %Node* %a, i32 0, i32 0
  store i32 5, i32* %load.a.value, align 4
  %load.b.value = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  store i32 8, i32* %load.b.value, align 4
  %load.c.value = getelementptr inbounds %Node, %Node* %c, i32 0, i32 0
  store i32 13, i32* %load.c.value, align 4
  %load.a.next = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  store %Node* %b, %Node** %load.a.next, align 8
  %load.b.next = getelementptr inbounds %Node, %Node* %b, i32 0, i32 1
  store %Node* %c, %Node** %load.b.next, align 8
  %"find(&a,13)" = call i32 @find(%Node* %a, i32 13)
  %MUL = mul i32 %"find(&a,13)", 10
  %"find(&a,8)" = call i32 @find(%Node* %a, i32 8)
  %ADD = add i32 %MUL, %"find(&a,8)"
  store i32 %ADD, i32* %r, align 4
  %"find(&a,99)" = call i32 @find(%Node* %a, i32 99)
  %EQ = icmp eq i32 %"find(&a,99)", -1
  br i1 %EQ, label %then, label %endif

then:                                             ; preds = %entry
  %old = load i32, i32* %r, align 4
  %ADD_ASSIGN = add i32 %old, 100
  store i32 %ADD_ASSIGN, i32* %r, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %load.r = load i32, i32* %r, align 4
  ret i32 %load.r
}
```

---

## 046 — program: assigning a struct copies it

```ura
// expect: 63   (1 + 20 + 10 + 2 + 10 + 20)
struct Point:
    x i32
    y i32

fn main() i32:
    a Point
    a.x = 1
    a.y = 2
    b Point = a
    b.x = 10
    c Point
    c = b
    c.y = 20
    return a.x + a.y * 10 + b.x + b.y + c.x + c.y
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Point = type { i32, i32 }

define i32 @main() {
entry:
  %a = alloca %Point, align 8
  %b = alloca %Point, align 8
  %c = alloca %Point, align 8
  store %Point zeroinitializer, %Point* %a, align 4
  %load.a.x = getelementptr inbounds %Point, %Point* %a, i32 0, i32 0
  store i32 1, i32* %load.a.x, align 4
  %load.a.y = getelementptr inbounds %Point, %Point* %a, i32 0, i32 1
  store i32 2, i32* %load.a.y, align 4
  %load.a = load %Point, %Point* %a, align 4
  store %Point %load.a, %Point* %b, align 4
  %load.b.x = getelementptr inbounds %Point, %Point* %b, i32 0, i32 0
  store i32 10, i32* %load.b.x, align 4
  store %Point zeroinitializer, %Point* %c, align 4
  %load.b = load %Point, %Point* %b, align 4
  store %Point %load.b, %Point* %c, align 4
  %load.c.y = getelementptr inbounds %Point, %Point* %c, i32 0, i32 1
  store i32 20, i32* %load.c.y, align 4
  %load.a.x1 = getelementptr inbounds %Point, %Point* %a, i32 0, i32 0
  %load.load.a.x = load i32, i32* %load.a.x1, align 4
  %load.a.y2 = getelementptr inbounds %Point, %Point* %a, i32 0, i32 1
  %load.load.a.y = load i32, i32* %load.a.y2, align 4
  %MUL = mul i32 %load.load.a.y, 10
  %ADD = add i32 %load.load.a.x, %MUL
  %load.b.x3 = getelementptr inbounds %Point, %Point* %b, i32 0, i32 0
  %load.load.b.x = load i32, i32* %load.b.x3, align 4
  %ADD4 = add i32 %ADD, %load.load.b.x
  %load.b.y = getelementptr inbounds %Point, %Point* %b, i32 0, i32 1
  %load.load.b.y = load i32, i32* %load.b.y, align 4
  %ADD5 = add i32 %ADD4, %load.load.b.y
  %load.c.x = getelementptr inbounds %Point, %Point* %c, i32 0, i32 0
  %load.load.c.x = load i32, i32* %load.c.x, align 4
  %ADD6 = add i32 %ADD5, %load.load.c.x
  %load.c.y7 = getelementptr inbounds %Point, %Point* %c, i32 0, i32 1
  %load.load.c.y = load i32, i32* %load.c.y7, align 4
  %ADD8 = add i32 %ADD6, %load.load.c.y
  ret i32 %ADD8
}
```

---

## 047 — program: a struct parameter is a copy

```ura
// expect: 61
struct Point:
    x i32

fn bump(p Point) i32:
    p.x += 5
    return p.x

fn main() i32:
    a Point
    a.x = 1
    r i32 = bump(a)
    return r * 10 + a.x
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Point = type { i32 }

define i32 @bump(%Point %0) {
entry:
  %p = alloca %Point, align 8
  store %Point %0, %Point* %p, align 4
  %load.p.x = getelementptr inbounds %Point, %Point* %p, i32 0, i32 0
  %old = load i32, i32* %load.p.x, align 4
  %ADD_ASSIGN = add i32 %old, 5
  store i32 %ADD_ASSIGN, i32* %load.p.x, align 4
  %load.p.x1 = getelementptr inbounds %Point, %Point* %p, i32 0, i32 0
  %load.load.p.x = load i32, i32* %load.p.x1, align 4
  ret i32 %load.load.p.x
}

define i32 @main() {
entry:
  %a = alloca %Point, align 8
  %r = alloca i32, align 4
  store %Point zeroinitializer, %Point* %a, align 4
  %load.a.x = getelementptr inbounds %Point, %Point* %a, i32 0, i32 0
  store i32 1, i32* %load.a.x, align 4
  %load.a = load %Point, %Point* %a, align 4
  %"bump(load.a)" = call i32 @bump(%Point %load.a)
  store i32 %"bump(load.a)", i32* %r, align 4
  %load.r = load i32, i32* %r, align 4
  %MUL = mul i32 %load.r, 10
  %load.a.x1 = getelementptr inbounds %Point, %Point* %a, i32 0, i32 0
  %load.load.a.x = load i32, i32* %load.a.x1, align 4
  %ADD = add i32 %MUL, %load.load.a.x
  ret i32 %ADD
}
```

---

## 048 — program: compound assignment through a ref attribute

```ura
// expect: 14   ((0 + 5) * 3 - 1)
struct Node:
    value i32
    next &Node

fn main() i32:
    a Node
    b Node
    a.next = &b
    a.next.value += 5
    a.next.value *= 3
    b.value -= 1
    return b.value
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Node = type { i32, %Node* }

define i32 @main() {
entry:
  %a = alloca %Node, align 8
  %b = alloca %Node, align 8
  store %Node zeroinitializer, %Node* %a, align 8
  store %Node zeroinitializer, %Node* %b, align 8
  %load.a.next = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  store %Node* %b, %Node** %load.a.next, align 8
  %load.a.next1 = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  %dref.load.a.next = load %Node*, %Node** %load.a.next1, align 8
  %load.dref.load.a.next.value = getelementptr inbounds %Node, %Node* %dref.load.a.next, i32 0, i32 0
  %old = load i32, i32* %load.dref.load.a.next.value, align 4
  %ADD_ASSIGN = add i32 %old, 5
  store i32 %ADD_ASSIGN, i32* %load.dref.load.a.next.value, align 4
  %load.a.next2 = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  %dref.load.a.next3 = load %Node*, %Node** %load.a.next2, align 8
  %load.dref.load.a.next.value4 = getelementptr inbounds %Node, %Node* %dref.load.a.next3, i32 0, i32 0
  %old5 = load i32, i32* %load.dref.load.a.next.value4, align 4
  %MUL_ASSIGN = mul i32 %old5, 3
  store i32 %MUL_ASSIGN, i32* %load.dref.load.a.next.value4, align 4
  %load.b.value = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  %old6 = load i32, i32* %load.b.value, align 4
  %SUB_ASSIGN = sub i32 %old6, 1
  store i32 %SUB_ASSIGN, i32* %load.b.value, align 4
  %load.b.value7 = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  %load.load.b.value = load i32, i32* %load.b.value7, align 4
  ret i32 %load.load.b.value
}
```

---

## 049 — program: a method on an array element

```ura
// expect: 45   (4 * 10 + 5)
struct Counter:
    n i32

    fn add(k i32):
        self.n += k

fn main() i32:
    a Counter
    b Counter
    cs Counter[] = [a, b]
    i i32 = 0
    while i < 2:
        cs[i].add(i + 3)
        cs[i].add(1)
        i += 1
    return cs[0].n * 10 + cs[1].n
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Counter = type { i32 }

define void @Counter.add(%Counter* %0, i32 %1) {
entry:
  %self = alloca %Counter*, align 8
  %k = alloca i32, align 4
  store %Counter* %0, %Counter** %self, align 8
  store i32 %1, i32* %k, align 4
  %dref.self = load %Counter*, %Counter** %self, align 8
  %load.dref.self.n = getelementptr inbounds %Counter, %Counter* %dref.self, i32 0, i32 0
  %load.k = load i32, i32* %k, align 4
  %old = load i32, i32* %load.dref.self.n, align 4
  %ADD_ASSIGN = add i32 %old, %load.k
  store i32 %ADD_ASSIGN, i32* %load.dref.self.n, align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Counter, align 8
  %b = alloca %Counter, align 8
  %cs = alloca %Counter*, align 8
  %array = alloca [2 x %Counter], align 8
  %i = alloca i32, align 4
  store %Counter zeroinitializer, %Counter* %a, align 4
  store %Counter zeroinitializer, %Counter* %b, align 4
  %load.a = load %Counter, %Counter* %a, align 4
  %load.b = load %Counter, %Counter* %b, align 4
  %e = getelementptr inbounds [2 x %Counter], [2 x %Counter]* %array, i32 0, i32 0
  store %Counter %load.a, %Counter* %e, align 4
  %e1 = getelementptr inbounds [2 x %Counter], [2 x %Counter]* %array, i32 0, i32 1
  store %Counter %load.b, %Counter* %e1, align 4
  %array2 = getelementptr inbounds [2 x %Counter], [2 x %Counter]* %array, i32 0, i32 0
  store %Counter* %array2, %Counter** %cs, align 8
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %then, %entry
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 2
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.cs = load %Counter*, %Counter** %cs, align 8
  %load.i3 = load i32, i32* %i, align 4
  %"load.cs[load.i]" = getelementptr inbounds %Counter, %Counter* %load.cs, i32 %load.i3
  %load.i4 = load i32, i32* %i, align 4
  %ADD = add i32 %load.i4, 3
  call void @Counter.add(%Counter* %"load.cs[load.i]", i32 %ADD)
  %load.cs5 = load %Counter*, %Counter** %cs, align 8
  %load.i6 = load i32, i32* %i, align 4
  %"load.cs[load.i]7" = getelementptr inbounds %Counter, %Counter* %load.cs5, i32 %load.i6
  call void @Counter.add(%Counter* %"load.cs[load.i]7", i32 1)
  %old = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  %load.cs8 = load %Counter*, %Counter** %cs, align 8
  %"load.cs[0]" = getelementptr inbounds %Counter, %Counter* %load.cs8, i32 0
  %"load.load.cs[0].n" = getelementptr inbounds %Counter, %Counter* %"load.cs[0]", i32 0, i32 0
  %"load.load.load.cs[0].n" = load i32, i32* %"load.load.cs[0].n", align 4
  %MUL = mul i32 %"load.load.load.cs[0].n", 10
  %load.cs9 = load %Counter*, %Counter** %cs, align 8
  %"load.cs[1]" = getelementptr inbounds %Counter, %Counter* %load.cs9, i32 1
  %"load.load.cs[1].n" = getelementptr inbounds %Counter, %Counter* %"load.cs[1]", i32 0, i32 0
  %"load.load.load.cs[1].n" = load i32, i32* %"load.load.cs[1].n", align 4
  %ADD10 = add i32 %MUL, %"load.load.load.cs[1].n"
  ret i32 %ADD10
}
```

---

## 050 — program: builder: a method returns a ref to self, chained

```ura
// expect: 6
struct Builder:
    total i32

    fn add(v i32) &Builder:
        self.total += v
        return &self

fn main() i32:
    b Builder
    b.add(1).add(2).add(3)
    return b.total
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Builder = type { i32 }

define %Builder* @Builder.add(%Builder* %0, i32 %1) {
entry:
  %self = alloca %Builder*, align 8
  %v = alloca i32, align 4
  store %Builder* %0, %Builder** %self, align 8
  store i32 %1, i32* %v, align 4
  %dref.self = load %Builder*, %Builder** %self, align 8
  %load.dref.self.total = getelementptr inbounds %Builder, %Builder* %dref.self, i32 0, i32 0
  %load.v = load i32, i32* %v, align 4
  %old = load i32, i32* %load.dref.self.total, align 4
  %ADD_ASSIGN = add i32 %old, %load.v
  store i32 %ADD_ASSIGN, i32* %load.dref.self.total, align 4
  %dref.self1 = load %Builder*, %Builder** %self, align 8
  ret %Builder* %dref.self1
}

define i32 @main() {
entry:
  %b = alloca %Builder, align 8
  store %Builder zeroinitializer, %Builder* %b, align 4
  %"add(&load.b,1)" = call %Builder* @Builder.add(%Builder* %b, i32 1)
  %"add(add(&load.b,1),2)" = call %Builder* @Builder.add(%Builder* %"add(&load.b,1)", i32 2)
  %"add(add(add(&load.b,1),2),3)" = call %Builder* @Builder.add(%Builder* %"add(add(&load.b,1),2)", i32 3)
  %load.b.total = getelementptr inbounds %Builder, %Builder* %b, i32 0, i32 0
  %load.load.b.total = load i32, i32* %load.b.total, align 4
  ret i32 %load.load.b.total
}
```

---

## 051 — program: a struct holding an array of structs

```ura
// expect: 12   (9 - (-3))
struct Point:
    x i32
    y i32

struct Shape:
    points Point[]
    count i32

    fn width() i32:
        lo i32 = self.points[0].x
        hi i32 = lo
        i i32 = 1
        while i < self.count:
            x i32 = self.points[i].x
            if x < lo: lo = x
            if x > hi: hi = x
            i += 1
        return hi - lo

fn main() i32:
    a Point
    b Point
    c Point
    a.x = 4
    b.x = -3
    c.x = 9
    s Shape
    s.points = [a, b, c]
    s.count = 3
    return s.width()
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Shape = type { %Point*, i32 }
%Point = type { i32, i32 }

define i32 @Shape.width(%Shape* %0) {
entry:
  %self = alloca %Shape*, align 8
  %lo = alloca i32, align 4
  %hi = alloca i32, align 4
  %i = alloca i32, align 4
  %x = alloca i32, align 4
  store %Shape* %0, %Shape** %self, align 8
  %dref.self = load %Shape*, %Shape** %self, align 8
  %load.dref.self.points = getelementptr inbounds %Shape, %Shape* %dref.self, i32 0, i32 0
  %load.load.dref.self.points = load %Point*, %Point** %load.dref.self.points, align 8
  %"load.load.dref.self.points[0]" = getelementptr inbounds %Point, %Point* %load.load.dref.self.points, i32 0
  %"load.load.load.dref.self.points[0].x" = getelementptr inbounds %Point, %Point* %"load.load.dref.self.points[0]", i32 0, i32 0
  %"load.load.load.load.dref.self.points[0].x" = load i32, i32* %"load.load.load.dref.self.points[0].x", align 4
  store i32 %"load.load.load.load.dref.self.points[0].x", i32* %lo, align 4
  %load.lo = load i32, i32* %lo, align 4
  store i32 %load.lo, i32* %hi, align 4
  store i32 1, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %endif10, %entry
  %load.i = load i32, i32* %i, align 4
  %dref.self1 = load %Shape*, %Shape** %self, align 8
  %load.dref.self.count = getelementptr inbounds %Shape, %Shape* %dref.self1, i32 0, i32 1
  %load.load.dref.self.count = load i32, i32* %load.dref.self.count, align 4
  %LT = icmp slt i32 %load.i, %load.load.dref.self.count
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %dref.self2 = load %Shape*, %Shape** %self, align 8
  %load.dref.self.points3 = getelementptr inbounds %Shape, %Shape* %dref.self2, i32 0, i32 0
  %load.load.dref.self.points4 = load %Point*, %Point** %load.dref.self.points3, align 8
  %load.i5 = load i32, i32* %i, align 4
  %"load.load.dref.self.points[load.i]" = getelementptr inbounds %Point, %Point* %load.load.dref.self.points4, i32 %load.i5
  %"load.load.load.dref.self.points[load.i].x" = getelementptr inbounds %Point, %Point* %"load.load.dref.self.points[load.i]", i32 0, i32 0
  %"load.load.load.load.dref.self.points[load.i].x" = load i32, i32* %"load.load.load.dref.self.points[load.i].x", align 4
  store i32 %"load.load.load.load.dref.self.points[load.i].x", i32* %x, align 4
  %load.x = load i32, i32* %x, align 4
  %load.lo7 = load i32, i32* %lo, align 4
  %LT8 = icmp slt i32 %load.x, %load.lo7
  br i1 %LT8, label %then6, label %endif

endwhile:                                         ; preds = %cond
  %load.hi14 = load i32, i32* %hi, align 4
  %load.lo15 = load i32, i32* %lo, align 4
  %SUB = sub i32 %load.hi14, %load.lo15
  ret i32 %SUB

then6:                                            ; preds = %then
  %load.x9 = load i32, i32* %x, align 4
  store i32 %load.x9, i32* %lo, align 4
  br label %endif

endif:                                            ; preds = %then6, %then
  %load.x12 = load i32, i32* %x, align 4
  %load.hi = load i32, i32* %hi, align 4
  %GT = icmp sgt i32 %load.x12, %load.hi
  br i1 %GT, label %then11, label %endif10

then11:                                           ; preds = %endif
  %load.x13 = load i32, i32* %x, align 4
  store i32 %load.x13, i32* %hi, align 4
  br label %endif10

endif10:                                          ; preds = %then11, %endif
  %old = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  br label %cond
}

define i32 @main() {
entry:
  %a = alloca %Point, align 8
  %b = alloca %Point, align 8
  %c = alloca %Point, align 8
  %s = alloca %Shape, align 8
  %array = alloca [3 x %Point], align 8
  store %Point zeroinitializer, %Point* %a, align 4
  store %Point zeroinitializer, %Point* %b, align 4
  store %Point zeroinitializer, %Point* %c, align 4
  %load.a.x = getelementptr inbounds %Point, %Point* %a, i32 0, i32 0
  store i32 4, i32* %load.a.x, align 4
  %load.b.x = getelementptr inbounds %Point, %Point* %b, i32 0, i32 0
  store i32 -3, i32* %load.b.x, align 4
  %load.c.x = getelementptr inbounds %Point, %Point* %c, i32 0, i32 0
  store i32 9, i32* %load.c.x, align 4
  store %Shape zeroinitializer, %Shape* %s, align 8
  %load.s.points = getelementptr inbounds %Shape, %Shape* %s, i32 0, i32 0
  %load.a = load %Point, %Point* %a, align 4
  %load.b = load %Point, %Point* %b, align 4
  %load.c = load %Point, %Point* %c, align 4
  %e = getelementptr inbounds [3 x %Point], [3 x %Point]* %array, i32 0, i32 0
  store %Point %load.a, %Point* %e, align 4
  %e1 = getelementptr inbounds [3 x %Point], [3 x %Point]* %array, i32 0, i32 1
  store %Point %load.b, %Point* %e1, align 4
  %e2 = getelementptr inbounds [3 x %Point], [3 x %Point]* %array, i32 0, i32 2
  store %Point %load.c, %Point* %e2, align 4
  %array3 = getelementptr inbounds [3 x %Point], [3 x %Point]* %array, i32 0, i32 0
  store %Point* %array3, %Point** %load.s.points, align 8
  %load.s.count = getelementptr inbounds %Shape, %Shape* %s, i32 0, i32 1
  store i32 3, i32* %load.s.count, align 4
  %"width(&load.s)" = call i32 @Shape.width(%Shape* %s)
  ret i32 %"width(&load.s)"
}
```

---

## 052 — program: tree walk with a ref out-parameter

```ura
// expect: 183   (sum 18 * 10, 3 nodes)
struct Tree:
    value i32
    left &Tree
    right &Tree

fn walk(t &Tree, count &i32) i32:
    if isNullable(t): return 0
    count += 1
    return t.value + walk(&t.left, &count) + walk(&t.right, &count)

fn main() i32:
    a Tree
    b Tree
    c Tree
    a.value = 5
    b.value = 6
    c.value = 7
    a.left = &b
    b.right = &c
    n i32 = 0
    s i32 = walk(&a, &n)
    return s * 10 + n
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Tree = type { i32, %Tree*, %Tree* }

define i32 @walk(%Tree* %0, i32* %1) {
entry:
  %t = alloca %Tree*, align 8
  %count = alloca i32*, align 8
  store %Tree* %0, %Tree** %t, align 8
  store i32* %1, i32** %count, align 8
  %dref.t = load %Tree*, %Tree** %t, align 8
  %"isNullable(dref.t)" = icmp eq %Tree* %dref.t, null
  br i1 %"isNullable(dref.t)", label %then, label %endif

then:                                             ; preds = %entry
  ret i32 0

endif:                                            ; preds = %entry
  %dref.count = load i32*, i32** %count, align 8
  %old = load i32, i32* %dref.count, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %dref.count, align 4
  %dref.t1 = load %Tree*, %Tree** %t, align 8
  %load.dref.t.value = getelementptr inbounds %Tree, %Tree* %dref.t1, i32 0, i32 0
  %load.load.dref.t.value = load i32, i32* %load.dref.t.value, align 4
  %dref.t2 = load %Tree*, %Tree** %t, align 8
  %load.dref.t.left = getelementptr inbounds %Tree, %Tree* %dref.t2, i32 0, i32 1
  %dref.load.dref.t.left = load %Tree*, %Tree** %load.dref.t.left, align 8
  %dref.count3 = load i32*, i32** %count, align 8
  %"walk(&dref.load.dref.t.left,&dref.count)" = call i32 @walk(%Tree* %dref.load.dref.t.left, i32* %dref.count3)
  %ADD = add i32 %load.load.dref.t.value, %"walk(&dref.load.dref.t.left,&dref.count)"
  %dref.t4 = load %Tree*, %Tree** %t, align 8
  %load.dref.t.right = getelementptr inbounds %Tree, %Tree* %dref.t4, i32 0, i32 2
  %dref.load.dref.t.right = load %Tree*, %Tree** %load.dref.t.right, align 8
  %dref.count5 = load i32*, i32** %count, align 8
  %"walk(&dref.load.dref.t.right,&dref.count)" = call i32 @walk(%Tree* %dref.load.dref.t.right, i32* %dref.count5)
  %ADD6 = add i32 %ADD, %"walk(&dref.load.dref.t.right,&dref.count)"
  ret i32 %ADD6
}

define i32 @main() {
entry:
  %a = alloca %Tree, align 8
  %b = alloca %Tree, align 8
  %c = alloca %Tree, align 8
  %n = alloca i32, align 4
  %s = alloca i32, align 4
  store %Tree zeroinitializer, %Tree* %a, align 8
  store %Tree zeroinitializer, %Tree* %b, align 8
  store %Tree zeroinitializer, %Tree* %c, align 8
  %load.a.value = getelementptr inbounds %Tree, %Tree* %a, i32 0, i32 0
  store i32 5, i32* %load.a.value, align 4
  %load.b.value = getelementptr inbounds %Tree, %Tree* %b, i32 0, i32 0
  store i32 6, i32* %load.b.value, align 4
  %load.c.value = getelementptr inbounds %Tree, %Tree* %c, i32 0, i32 0
  store i32 7, i32* %load.c.value, align 4
  %load.a.left = getelementptr inbounds %Tree, %Tree* %a, i32 0, i32 1
  store %Tree* %b, %Tree** %load.a.left, align 8
  %load.b.right = getelementptr inbounds %Tree, %Tree* %b, i32 0, i32 2
  store %Tree* %c, %Tree** %load.b.right, align 8
  store i32 0, i32* %n, align 4
  %"walk(&a,&n)" = call i32 @walk(%Tree* %a, i32* %n)
  store i32 %"walk(&a,&n)", i32* %s, align 4
  %load.s = load i32, i32* %s, align 4
  %MUL = mul i32 %load.s, 10
  %load.n = load i32, i32* %n, align 4
  %ADD = add i32 %MUL, %load.n
  ret i32 %ADD
}
```

---

## 053 — program: a function returns a ref into a struct, the caller binds it

```ura
// expect: 103   (r binds to x.b: 3 + 100)
struct Pair:
    a i32
    b i32

fn bigger(p &Pair) &i32:
    if p.a > p.b: return &p.a
    return &p.b

fn main() i32:
    x Pair
    x.a = 3
    x.b = 7
    r &i32 = bigger(&x)
    r = 100
    return x.a + x.b
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Pair = type { i32, i32 }

define i32* @bigger(%Pair* %0) {
entry:
  %p = alloca %Pair*, align 8
  store %Pair* %0, %Pair** %p, align 8
  %dref.p = load %Pair*, %Pair** %p, align 8
  %load.dref.p.a = getelementptr inbounds %Pair, %Pair* %dref.p, i32 0, i32 0
  %load.load.dref.p.a = load i32, i32* %load.dref.p.a, align 4
  %dref.p1 = load %Pair*, %Pair** %p, align 8
  %load.dref.p.b = getelementptr inbounds %Pair, %Pair* %dref.p1, i32 0, i32 1
  %load.load.dref.p.b = load i32, i32* %load.dref.p.b, align 4
  %GT = icmp sgt i32 %load.load.dref.p.a, %load.load.dref.p.b
  br i1 %GT, label %then, label %endif

then:                                             ; preds = %entry
  %dref.p2 = load %Pair*, %Pair** %p, align 8
  %load.dref.p.a3 = getelementptr inbounds %Pair, %Pair* %dref.p2, i32 0, i32 0
  ret i32* %load.dref.p.a3

endif:                                            ; preds = %entry
  %dref.p4 = load %Pair*, %Pair** %p, align 8
  %load.dref.p.b5 = getelementptr inbounds %Pair, %Pair* %dref.p4, i32 0, i32 1
  ret i32* %load.dref.p.b5
}

define i32 @main() {
entry:
  %x = alloca %Pair, align 8
  %r = alloca i32*, align 8
  store %Pair zeroinitializer, %Pair* %x, align 4
  %load.x.a = getelementptr inbounds %Pair, %Pair* %x, i32 0, i32 0
  store i32 3, i32* %load.x.a, align 4
  %load.x.b = getelementptr inbounds %Pair, %Pair* %x, i32 0, i32 1
  store i32 7, i32* %load.x.b, align 4
  %"bigger(&x)" = call i32* @bigger(%Pair* %x)
  store i32* %"bigger(&x)", i32** %r, align 8
  %dref.r = load i32*, i32** %r, align 8
  store i32 100, i32* %dref.r, align 4
  %load.x.a1 = getelementptr inbounds %Pair, %Pair* %x, i32 0, i32 0
  %load.load.x.a = load i32, i32* %load.x.a1, align 4
  %load.x.b2 = getelementptr inbounds %Pair, %Pair* %x, i32 0, i32 1
  %load.load.x.b = load i32, i32* %load.x.b2, align 4
  %ADD = add i32 %load.load.x.a, %load.load.x.b
  ret i32 %ADD
}
```

---

## 054 — program: fibonacci as a pair: recursion returning a struct, .a on a call

```ura
// expect: 55
struct Pair:
    a i32
    b i32

fn fib(n i32) Pair:
    r Pair
    if n == 0:
        r.a = 0
        r.b = 1
        return r
    p Pair = fib(n - 1)
    r.a = p.b
    r.b = p.a + p.b
    return r

fn main() i32:
    return fib(10).a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Pair = type { i32, i32 }

define %Pair @fib(i32 %0) {
entry:
  %n = alloca i32, align 4
  %r = alloca %Pair, align 8
  %p = alloca %Pair, align 8
  store i32 %0, i32* %n, align 4
  store %Pair zeroinitializer, %Pair* %r, align 4
  %load.n = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %load.n, 0
  br i1 %EQ, label %then, label %endif

then:                                             ; preds = %entry
  %load.r.a = getelementptr inbounds %Pair, %Pair* %r, i32 0, i32 0
  store i32 0, i32* %load.r.a, align 4
  %load.r.b = getelementptr inbounds %Pair, %Pair* %r, i32 0, i32 1
  store i32 1, i32* %load.r.b, align 4
  %load.r = load %Pair, %Pair* %r, align 4
  ret %Pair %load.r

endif:                                            ; preds = %entry
  %load.n1 = load i32, i32* %n, align 4
  %SUB = sub i32 %load.n1, 1
  %"fib(load.n - 1)" = call %Pair @fib(i32 %SUB)
  store %Pair %"fib(load.n - 1)", %Pair* %p, align 4
  %load.r.a2 = getelementptr inbounds %Pair, %Pair* %r, i32 0, i32 0
  %load.p.b = getelementptr inbounds %Pair, %Pair* %p, i32 0, i32 1
  %load.load.p.b = load i32, i32* %load.p.b, align 4
  store i32 %load.load.p.b, i32* %load.r.a2, align 4
  %load.r.b3 = getelementptr inbounds %Pair, %Pair* %r, i32 0, i32 1
  %load.p.a = getelementptr inbounds %Pair, %Pair* %p, i32 0, i32 0
  %load.load.p.a = load i32, i32* %load.p.a, align 4
  %load.p.b4 = getelementptr inbounds %Pair, %Pair* %p, i32 0, i32 1
  %load.load.p.b5 = load i32, i32* %load.p.b4, align 4
  %ADD = add i32 %load.load.p.a, %load.load.p.b5
  store i32 %ADD, i32* %load.r.b3, align 4
  %load.r6 = load %Pair, %Pair* %r, align 4
  ret %Pair %load.r6
}

define i32 @main() {
entry:
  %tmp = alloca %Pair, align 8
  %"fib(10)" = call %Pair @fib(i32 10)
  store %Pair %"fib(10)", %Pair* %tmp, align 4
  %"fib(10).a" = getelementptr inbounds %Pair, %Pair* %tmp, i32 0, i32 0
  %"load.fib(10).a" = load i32, i32* %"fib(10).a", align 4
  ret i32 %"load.fib(10).a"
}
```

---

## 055 — program: method calls nested as arguments

```ura
// expect: 160   (a 15 * 10, b 10)
struct Acc:
    v i32

    fn get() i32:
        return self.v

    fn add(k i32) i32:
        self.v += k
        return self.v

fn main() i32:
    a Acc
    b Acc
    b.v = 5
    a.add(b.add(a.add(b.get())))
    return a.v * 10 + b.v
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Acc = type { i32 }

define i32 @Acc.get(%Acc* %0) {
entry:
  %self = alloca %Acc*, align 8
  store %Acc* %0, %Acc** %self, align 8
  %dref.self = load %Acc*, %Acc** %self, align 8
  %load.dref.self.v = getelementptr inbounds %Acc, %Acc* %dref.self, i32 0, i32 0
  %load.load.dref.self.v = load i32, i32* %load.dref.self.v, align 4
  ret i32 %load.load.dref.self.v
}

define i32 @Acc.add(%Acc* %0, i32 %1) {
entry:
  %self = alloca %Acc*, align 8
  %k = alloca i32, align 4
  store %Acc* %0, %Acc** %self, align 8
  store i32 %1, i32* %k, align 4
  %dref.self = load %Acc*, %Acc** %self, align 8
  %load.dref.self.v = getelementptr inbounds %Acc, %Acc* %dref.self, i32 0, i32 0
  %load.k = load i32, i32* %k, align 4
  %old = load i32, i32* %load.dref.self.v, align 4
  %ADD_ASSIGN = add i32 %old, %load.k
  store i32 %ADD_ASSIGN, i32* %load.dref.self.v, align 4
  %dref.self1 = load %Acc*, %Acc** %self, align 8
  %load.dref.self.v2 = getelementptr inbounds %Acc, %Acc* %dref.self1, i32 0, i32 0
  %load.load.dref.self.v = load i32, i32* %load.dref.self.v2, align 4
  ret i32 %load.load.dref.self.v
}

define i32 @main() {
entry:
  %a = alloca %Acc, align 8
  %b = alloca %Acc, align 8
  store %Acc zeroinitializer, %Acc* %a, align 4
  store %Acc zeroinitializer, %Acc* %b, align 4
  %load.b.v = getelementptr inbounds %Acc, %Acc* %b, i32 0, i32 0
  store i32 5, i32* %load.b.v, align 4
  %"get(&load.b)" = call i32 @Acc.get(%Acc* %b)
  %"add(&load.a,get(&load.b))" = call i32 @Acc.add(%Acc* %a, i32 %"get(&load.b)")
  %"add(&load.b,add(&load.a,get(&load.b)))" = call i32 @Acc.add(%Acc* %b, i32 %"add(&load.a,get(&load.b))")
  %"add(&load.a,add(&load.b,add(&load.a,get(&load.b))))" = call i32 @Acc.add(%Acc* %a, i32 %"add(&load.b,add(&load.a,get(&load.b)))")
  %load.a.v = getelementptr inbounds %Acc, %Acc* %a, i32 0, i32 0
  %load.load.a.v = load i32, i32* %load.a.v, align 4
  %MUL = mul i32 %load.load.a.v, 10
  %load.b.v1 = getelementptr inbounds %Acc, %Acc* %b, i32 0, i32 0
  %load.load.b.v = load i32, i32* %load.b.v1, align 4
  %ADD = add i32 %MUL, %load.load.b.v
  ret i32 %ADD
}
```

---

## 056 — program: print a linked list backwards with recursion

```ura
// stdout: 321
// expect: 6
proto printf(fmt i8[], ...) i32

struct Node:
    value i32
    next &Node

fn print_rev(n &Node):
    if isNullable(n): return
    print_rev(&n.next)
    printf("%d", n.value)

fn main() i32:
    a Node
    b Node
    c Node
    a.value = 1
    b.value = 2
    c.value = 3
    a.next = &b
    b.next = &c
    print_rev(&a)
    printf("\n")
    return a.value + b.value + c.value
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Node = type { i32, %Node* }

@str = private constant [3 x i8] c"%d\00"
@str.1 = private constant [2 x i8] c"\0A\00"

declare i32 @printf(i8*, ...)

define void @print_rev(%Node* %0) {
entry:
  %n = alloca %Node*, align 8
  %s = alloca [3 x i8], align 1
  store %Node* %0, %Node** %n, align 8
  %dref.n = load %Node*, %Node** %n, align 8
  %"isNullable(dref.n)" = icmp eq %Node* %dref.n, null
  br i1 %"isNullable(dref.n)", label %then, label %endif

then:                                             ; preds = %entry
  ret void

endif:                                            ; preds = %entry
  %dref.n1 = load %Node*, %Node** %n, align 8
  %load.dref.n.next = getelementptr inbounds %Node, %Node* %dref.n1, i32 0, i32 1
  %dref.load.dref.n.next = load %Node*, %Node** %load.dref.n.next, align 8
  call void @print_rev(%Node* %dref.load.dref.n.next)
  %dst = getelementptr inbounds [3 x i8], [3 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i64 3, i1 false)
  %dref.n2 = load %Node*, %Node** %n, align 8
  %load.dref.n.value = getelementptr inbounds %Node, %Node* %dref.n2, i32 0, i32 0
  %load.load.dref.n.value = load i32, i32* %load.dref.n.value, align 4
  %"printf(str,load.load.dref.n.value)" = call i32 (i8*, ...) @printf(i8* %dst, i32 %load.load.dref.n.value)
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Node, align 8
  %b = alloca %Node, align 8
  %c = alloca %Node, align 8
  %s = alloca [2 x i8], align 1
  store %Node zeroinitializer, %Node* %a, align 8
  store %Node zeroinitializer, %Node* %b, align 8
  store %Node zeroinitializer, %Node* %c, align 8
  %load.a.value = getelementptr inbounds %Node, %Node* %a, i32 0, i32 0
  store i32 1, i32* %load.a.value, align 4
  %load.b.value = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  store i32 2, i32* %load.b.value, align 4
  %load.c.value = getelementptr inbounds %Node, %Node* %c, i32 0, i32 0
  store i32 3, i32* %load.c.value, align 4
  %load.a.next = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  store %Node* %b, %Node** %load.a.next, align 8
  %load.b.next = getelementptr inbounds %Node, %Node* %b, i32 0, i32 1
  store %Node* %c, %Node** %load.b.next, align 8
  call void @print_rev(%Node* %a)
  %dst = getelementptr inbounds [2 x i8], [2 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i64 2, i1 false)
  %"printf(str)" = call i32 (i8*, ...) @printf(i8* %dst)
  %load.a.value1 = getelementptr inbounds %Node, %Node* %a, i32 0, i32 0
  %load.load.a.value = load i32, i32* %load.a.value1, align 4
  %load.b.value2 = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  %load.load.b.value = load i32, i32* %load.b.value2, align 4
  %ADD = add i32 %load.load.a.value, %load.load.b.value
  %load.c.value3 = getelementptr inbounds %Node, %Node* %c, i32 0, i32 0
  %load.load.c.value = load i32, i32* %load.c.value3, align 4
  %ADD4 = add i32 %ADD, %load.load.c.value
  ret i32 %ADD4
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 057 — program: sort structs in an array by an attribute

```ura
// stdout: abc
// expect: 123
proto printf(fmt i8[], ...) i32

struct Item:
    key i32
    tag char

fn main() i32:
    a Item
    b Item
    c Item
    a.key = 3
    a.tag = 'c'
    b.key = 1
    b.tag = 'a'
    c.key = 2
    c.tag = 'b'
    items Item[] = [a, b, c]
    i i32 = 0
    while i < 3:
        j i32 = i + 1
        while j < 3:
            if items[j].key < items[i].key:
                t Item = items[i]
                items[i] = items[j]
                items[j] = t
            j += 1
        i += 1
    printf("%c%c%c\n", items[0].tag, items[1].tag, items[2].tag)
    return items[0].key * 100 + items[1].key * 10 + items[2].key
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Item = type { i32, i8 }

@str = private constant [8 x i8] c"%c%c%c\0A\00"

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %a = alloca %Item, align 8
  %b = alloca %Item, align 8
  %c = alloca %Item, align 8
  %items = alloca %Item*, align 8
  %array = alloca [3 x %Item], align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %t = alloca %Item, align 8
  %s = alloca [8 x i8], align 1
  store %Item zeroinitializer, %Item* %a, align 4
  store %Item zeroinitializer, %Item* %b, align 4
  store %Item zeroinitializer, %Item* %c, align 4
  %load.a.key = getelementptr inbounds %Item, %Item* %a, i32 0, i32 0
  store i32 3, i32* %load.a.key, align 4
  %load.a.tag = getelementptr inbounds %Item, %Item* %a, i32 0, i32 1
  store i8 99, i8* %load.a.tag, align 1
  %load.b.key = getelementptr inbounds %Item, %Item* %b, i32 0, i32 0
  store i32 1, i32* %load.b.key, align 4
  %load.b.tag = getelementptr inbounds %Item, %Item* %b, i32 0, i32 1
  store i8 97, i8* %load.b.tag, align 1
  %load.c.key = getelementptr inbounds %Item, %Item* %c, i32 0, i32 0
  store i32 2, i32* %load.c.key, align 4
  %load.c.tag = getelementptr inbounds %Item, %Item* %c, i32 0, i32 1
  store i8 98, i8* %load.c.tag, align 1
  %load.a = load %Item, %Item* %a, align 4
  %load.b = load %Item, %Item* %b, align 4
  %load.c = load %Item, %Item* %c, align 4
  %e = getelementptr inbounds [3 x %Item], [3 x %Item]* %array, i32 0, i32 0
  store %Item %load.a, %Item* %e, align 4
  %e1 = getelementptr inbounds [3 x %Item], [3 x %Item]* %array, i32 0, i32 1
  store %Item %load.b, %Item* %e1, align 4
  %e2 = getelementptr inbounds [3 x %Item], [3 x %Item]* %array, i32 0, i32 2
  store %Item %load.c, %Item* %e2, align 4
  %array3 = getelementptr inbounds [3 x %Item], [3 x %Item]* %array, i32 0, i32 0
  store %Item* %array3, %Item** %items, align 8
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %endwhile7, %entry
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 3
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.i4 = load i32, i32* %i, align 4
  %ADD = add i32 %load.i4, 1
  store i32 %ADD, i32* %j, align 4
  br label %cond5

endwhile:                                         ; preds = %cond
  %dst = getelementptr inbounds [8 x i8], [8 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([8 x i8], [8 x i8]* @str, i32 0, i32 0), i64 8, i1 false)
  %load.items28 = load %Item*, %Item** %items, align 8
  %"load.items[0]" = getelementptr inbounds %Item, %Item* %load.items28, i32 0
  %"load.load.items[0].tag" = getelementptr inbounds %Item, %Item* %"load.items[0]", i32 0, i32 1
  %"load.load.load.items[0].tag" = load i8, i8* %"load.load.items[0].tag", align 1
  %load.items29 = load %Item*, %Item** %items, align 8
  %"load.items[1]" = getelementptr inbounds %Item, %Item* %load.items29, i32 1
  %"load.load.items[1].tag" = getelementptr inbounds %Item, %Item* %"load.items[1]", i32 0, i32 1
  %"load.load.load.items[1].tag" = load i8, i8* %"load.load.items[1].tag", align 1
  %load.items30 = load %Item*, %Item** %items, align 8
  %"load.items[2]" = getelementptr inbounds %Item, %Item* %load.items30, i32 2
  %"load.load.items[2].tag" = getelementptr inbounds %Item, %Item* %"load.items[2]", i32 0, i32 1
  %"load.load.load.items[2].tag" = load i8, i8* %"load.load.items[2].tag", align 1
  %"printf(str,load.load.load.items[0].tag,load.load.load.items[1].tag,load.load.load.items[2].tag)" = call i32 (i8*, ...) @printf(i8* %dst, i8 %"load.load.load.items[0].tag", i8 %"load.load.load.items[1].tag", i8 %"load.load.load.items[2].tag")
  %load.items31 = load %Item*, %Item** %items, align 8
  %"load.items[0]32" = getelementptr inbounds %Item, %Item* %load.items31, i32 0
  %"load.load.items[0].key" = getelementptr inbounds %Item, %Item* %"load.items[0]32", i32 0, i32 0
  %"load.load.load.items[0].key" = load i32, i32* %"load.load.items[0].key", align 4
  %MUL = mul i32 %"load.load.load.items[0].key", 100
  %load.items33 = load %Item*, %Item** %items, align 8
  %"load.items[1]34" = getelementptr inbounds %Item, %Item* %load.items33, i32 1
  %"load.load.items[1].key" = getelementptr inbounds %Item, %Item* %"load.items[1]34", i32 0, i32 0
  %"load.load.load.items[1].key" = load i32, i32* %"load.load.items[1].key", align 4
  %MUL35 = mul i32 %"load.load.load.items[1].key", 10
  %ADD36 = add i32 %MUL, %MUL35
  %load.items37 = load %Item*, %Item** %items, align 8
  %"load.items[2]38" = getelementptr inbounds %Item, %Item* %load.items37, i32 2
  %"load.load.items[2].key" = getelementptr inbounds %Item, %Item* %"load.items[2]38", i32 0, i32 0
  %"load.load.load.items[2].key" = load i32, i32* %"load.load.items[2].key", align 4
  %ADD39 = add i32 %ADD36, %"load.load.load.items[2].key"
  ret i32 %ADD39

cond5:                                            ; preds = %endif, %then
  %load.j = load i32, i32* %j, align 4
  %LT8 = icmp slt i32 %load.j, 3
  br i1 %LT8, label %then6, label %endwhile7

then6:                                            ; preds = %cond5
  %load.items = load %Item*, %Item** %items, align 8
  %load.j10 = load i32, i32* %j, align 4
  %"load.items[load.j]" = getelementptr inbounds %Item, %Item* %load.items, i32 %load.j10
  %"load.load.items[load.j].key" = getelementptr inbounds %Item, %Item* %"load.items[load.j]", i32 0, i32 0
  %"load.load.load.items[load.j].key" = load i32, i32* %"load.load.items[load.j].key", align 4
  %load.items11 = load %Item*, %Item** %items, align 8
  %load.i12 = load i32, i32* %i, align 4
  %"load.items[load.i]" = getelementptr inbounds %Item, %Item* %load.items11, i32 %load.i12
  %"load.load.items[load.i].key" = getelementptr inbounds %Item, %Item* %"load.items[load.i]", i32 0, i32 0
  %"load.load.load.items[load.i].key" = load i32, i32* %"load.load.items[load.i].key", align 4
  %LT13 = icmp slt i32 %"load.load.load.items[load.j].key", %"load.load.load.items[load.i].key"
  br i1 %LT13, label %then9, label %endif

endwhile7:                                        ; preds = %cond5
  %old26 = load i32, i32* %i, align 4
  %ADD_ASSIGN27 = add i32 %old26, 1
  store i32 %ADD_ASSIGN27, i32* %i, align 4
  br label %cond

then9:                                            ; preds = %then6
  %load.items14 = load %Item*, %Item** %items, align 8
  %load.i15 = load i32, i32* %i, align 4
  %"load.items[load.i]16" = getelementptr inbounds %Item, %Item* %load.items14, i32 %load.i15
  %"load.load.items[load.i]" = load %Item, %Item* %"load.items[load.i]16", align 4
  store %Item %"load.load.items[load.i]", %Item* %t, align 4
  %load.items17 = load %Item*, %Item** %items, align 8
  %load.i18 = load i32, i32* %i, align 4
  %"load.items[load.i]19" = getelementptr inbounds %Item, %Item* %load.items17, i32 %load.i18
  %load.items20 = load %Item*, %Item** %items, align 8
  %load.j21 = load i32, i32* %j, align 4
  %"load.items[load.j]22" = getelementptr inbounds %Item, %Item* %load.items20, i32 %load.j21
  %"load.load.items[load.j]" = load %Item, %Item* %"load.items[load.j]22", align 4
  store %Item %"load.load.items[load.j]", %Item* %"load.items[load.i]19", align 4
  %load.items23 = load %Item*, %Item** %items, align 8
  %load.j24 = load i32, i32* %j, align 4
  %"load.items[load.j]25" = getelementptr inbounds %Item, %Item* %load.items23, i32 %load.j24
  %load.t = load %Item, %Item* %t, align 4
  store %Item %load.t, %Item* %"load.items[load.j]25", align 4
  br label %endif

endif:                                            ; preds = %then9, %then6
  %old = load i32, i32* %j, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %j, align 4
  br label %cond5
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 058 — program: ping pong: two structs that name each other, before either is declared

```ura
// expect: 33
struct Ping:
    hits i32

    fn hit(o &Pong, n i32):
        self.hits += 1
        if n > 0: o.hit(&self, n - 1)

struct Pong:
    hits i32

    fn hit(o &Ping, n i32):
        self.hits += 1
        if n > 0: o.hit(&self, n - 1)

fn main() i32:
    a Ping
    b Pong
    a.hit(&b, 5)
    return a.hits * 10 + b.hits
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Ping = type { i32 }
%Pong = type { i32 }

define void @Ping.hit(%Ping* %0, %Pong* %1, i32 %2) {
entry:
  %self = alloca %Ping*, align 8
  %o = alloca %Pong*, align 8
  %n = alloca i32, align 4
  store %Ping* %0, %Ping** %self, align 8
  store %Pong* %1, %Pong** %o, align 8
  store i32 %2, i32* %n, align 4
  %dref.self = load %Ping*, %Ping** %self, align 8
  %load.dref.self.hits = getelementptr inbounds %Ping, %Ping* %dref.self, i32 0, i32 0
  %old = load i32, i32* %load.dref.self.hits, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %load.dref.self.hits, align 4
  %load.n = load i32, i32* %n, align 4
  %GT = icmp sgt i32 %load.n, 0
  br i1 %GT, label %then, label %endif

then:                                             ; preds = %entry
  %dref.o = load %Pong*, %Pong** %o, align 8
  %dref.self1 = load %Ping*, %Ping** %self, align 8
  %load.n2 = load i32, i32* %n, align 4
  %SUB = sub i32 %load.n2, 1
  call void @Pong.hit(%Pong* %dref.o, %Ping* %dref.self1, i32 %SUB)
  br label %endif

endif:                                            ; preds = %then, %entry
  ret void
}

define void @Pong.hit(%Pong* %0, %Ping* %1, i32 %2) {
entry:
  %self = alloca %Pong*, align 8
  %o = alloca %Ping*, align 8
  %n = alloca i32, align 4
  store %Pong* %0, %Pong** %self, align 8
  store %Ping* %1, %Ping** %o, align 8
  store i32 %2, i32* %n, align 4
  %dref.self = load %Pong*, %Pong** %self, align 8
  %load.dref.self.hits = getelementptr inbounds %Pong, %Pong* %dref.self, i32 0, i32 0
  %old = load i32, i32* %load.dref.self.hits, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %load.dref.self.hits, align 4
  %load.n = load i32, i32* %n, align 4
  %GT = icmp sgt i32 %load.n, 0
  br i1 %GT, label %then, label %endif

then:                                             ; preds = %entry
  %dref.o = load %Ping*, %Ping** %o, align 8
  %dref.self1 = load %Pong*, %Pong** %self, align 8
  %load.n2 = load i32, i32* %n, align 4
  %SUB = sub i32 %load.n2, 1
  call void @Ping.hit(%Ping* %dref.o, %Pong* %dref.self1, i32 %SUB)
  br label %endif

endif:                                            ; preds = %then, %entry
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Ping, align 8
  %b = alloca %Pong, align 8
  store %Ping zeroinitializer, %Ping* %a, align 4
  store %Pong zeroinitializer, %Pong* %b, align 4
  call void @Ping.hit(%Ping* %a, %Pong* %b, i32 5)
  %load.a.hits = getelementptr inbounds %Ping, %Ping* %a, i32 0, i32 0
  %load.load.a.hits = load i32, i32* %load.a.hits, align 4
  %MUL = mul i32 %load.load.a.hits, 10
  %load.b.hits = getelementptr inbounds %Pong, %Pong* %b, i32 0, i32 0
  %load.load.b.hits = load i32, i32* %load.b.hits, align 4
  %ADD = add i32 %MUL, %load.load.b.hits
  ret i32 %ADD
}
```

---

## 059 — program: while True with break and return inside a method

```ura
// expect: 40   (23 at 4 * 10, 4 at 0, 99 missing: 0)
struct Finder:
    data i32[]
    size i32

    fn index_of(v i32) i32:
        i i32 = 0
        while True:
            if i == self.size: break
            if self.data[i] == v: return i
            i += 1
        return -1

fn main() i32:
    f Finder
    f.data = [4, 8, 15, 16, 23, 42]
    f.size = 6
    return f.index_of(23) * 10 + f.index_of(4) + (f.index_of(99) + 1) * 100
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Finder = type { i32*, i32 }

define i32 @Finder.index_of(%Finder* %0, i32 %1) {
entry:
  %self = alloca %Finder*, align 8
  %v = alloca i32, align 4
  %i = alloca i32, align 4
  store %Finder* %0, %Finder** %self, align 8
  store i32 %1, i32* %v, align 4
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %endif2, %entry
  br i1 true, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.i = load i32, i32* %i, align 4
  %dref.self = load %Finder*, %Finder** %self, align 8
  %load.dref.self.size = getelementptr inbounds %Finder, %Finder* %dref.self, i32 0, i32 1
  %load.load.dref.self.size = load i32, i32* %load.dref.self.size, align 4
  %EQ = icmp eq i32 %load.i, %load.load.dref.self.size
  br i1 %EQ, label %then1, label %endif

endwhile:                                         ; preds = %then1, %cond
  ret i32 -1

then1:                                            ; preds = %then
  br label %endwhile

endif:                                            ; preds = %then
  %dref.self4 = load %Finder*, %Finder** %self, align 8
  %load.dref.self.data = getelementptr inbounds %Finder, %Finder* %dref.self4, i32 0, i32 0
  %load.load.dref.self.data = load i32*, i32** %load.dref.self.data, align 8
  %load.i5 = load i32, i32* %i, align 4
  %"load.load.dref.self.data[load.i]" = getelementptr inbounds i32, i32* %load.load.dref.self.data, i32 %load.i5
  %"load.load.load.dref.self.data[load.i]" = load i32, i32* %"load.load.dref.self.data[load.i]", align 4
  %load.v = load i32, i32* %v, align 4
  %EQ6 = icmp eq i32 %"load.load.load.dref.self.data[load.i]", %load.v
  br i1 %EQ6, label %then3, label %endif2

then3:                                            ; preds = %endif
  %load.i7 = load i32, i32* %i, align 4
  ret i32 %load.i7

endif2:                                           ; preds = %endif
  %old = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  br label %cond
}

define i32 @main() {
entry:
  %f = alloca %Finder, align 8
  %array = alloca [6 x i32], align 4
  store %Finder zeroinitializer, %Finder* %f, align 8
  %load.f.data = getelementptr inbounds %Finder, %Finder* %f, i32 0, i32 0
  %e = getelementptr inbounds [6 x i32], [6 x i32]* %array, i32 0, i32 0
  store i32 4, i32* %e, align 4
  %e1 = getelementptr inbounds [6 x i32], [6 x i32]* %array, i32 0, i32 1
  store i32 8, i32* %e1, align 4
  %e2 = getelementptr inbounds [6 x i32], [6 x i32]* %array, i32 0, i32 2
  store i32 15, i32* %e2, align 4
  %e3 = getelementptr inbounds [6 x i32], [6 x i32]* %array, i32 0, i32 3
  store i32 16, i32* %e3, align 4
  %e4 = getelementptr inbounds [6 x i32], [6 x i32]* %array, i32 0, i32 4
  store i32 23, i32* %e4, align 4
  %e5 = getelementptr inbounds [6 x i32], [6 x i32]* %array, i32 0, i32 5
  store i32 42, i32* %e5, align 4
  %array6 = getelementptr inbounds [6 x i32], [6 x i32]* %array, i32 0, i32 0
  store i32* %array6, i32** %load.f.data, align 8
  %load.f.size = getelementptr inbounds %Finder, %Finder* %f, i32 0, i32 1
  store i32 6, i32* %load.f.size, align 4
  %"index_of(&load.f,23)" = call i32 @Finder.index_of(%Finder* %f, i32 23)
  %MUL = mul i32 %"index_of(&load.f,23)", 10
  %"index_of(&load.f,4)" = call i32 @Finder.index_of(%Finder* %f, i32 4)
  %ADD = add i32 %MUL, %"index_of(&load.f,4)"
  %"index_of(&load.f,99)" = call i32 @Finder.index_of(%Finder* %f, i32 99)
  %ADD7 = add i32 %"index_of(&load.f,99)", 1
  %MUL8 = mul i32 %ADD7, 100
  %ADD9 = add i32 %ADD, %MUL8
  ret i32 %ADD9
}
```

---

## 060 — program: script: a global stack evaluates postfix 3 4 + 2 *

```ura
// expect: 14
struct Stack:
    items i32[]
    top i32

stack Stack
stack.items = [0, 0, 0, 0, 0, 0, 0, 0]

fn push(v i32):
    stack.items[stack.top] = v
    stack.top += 1

fn pop() i32:
    stack.top -= 1
    return stack.items[stack.top]

push(3)
push(4)
push(pop() + pop())
push(2)
push(pop() * pop())
return pop()
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Stack = type { i32*, i32 }

@stack = global %Stack zeroinitializer

define void @push(i32 %0) {
entry:
  %v = alloca i32, align 4
  store i32 %0, i32* %v, align 4
  %load.load.stack.items = load i32*, i32** getelementptr inbounds (%Stack, %Stack* @stack, i32 0, i32 0), align 8
  %load.load.stack.top = load i32, i32* getelementptr inbounds (%Stack, %Stack* @stack, i32 0, i32 1), align 4
  %"load.load.stack.items[load.load.stack.top]" = getelementptr inbounds i32, i32* %load.load.stack.items, i32 %load.load.stack.top
  %load.v = load i32, i32* %v, align 4
  store i32 %load.v, i32* %"load.load.stack.items[load.load.stack.top]", align 4
  %old = load i32, i32* getelementptr inbounds (%Stack, %Stack* @stack, i32 0, i32 1), align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* getelementptr inbounds (%Stack, %Stack* @stack, i32 0, i32 1), align 4
  ret void
}

define i32 @pop() {
entry:
  %old = load i32, i32* getelementptr inbounds (%Stack, %Stack* @stack, i32 0, i32 1), align 4
  %SUB_ASSIGN = sub i32 %old, 1
  store i32 %SUB_ASSIGN, i32* getelementptr inbounds (%Stack, %Stack* @stack, i32 0, i32 1), align 4
  %load.load.stack.items = load i32*, i32** getelementptr inbounds (%Stack, %Stack* @stack, i32 0, i32 0), align 8
  %load.load.stack.top = load i32, i32* getelementptr inbounds (%Stack, %Stack* @stack, i32 0, i32 1), align 4
  %"load.load.stack.items[load.load.stack.top]" = getelementptr inbounds i32, i32* %load.load.stack.items, i32 %load.load.stack.top
  %"load.load.load.stack.items[load.load.stack.top]" = load i32, i32* %"load.load.stack.items[load.load.stack.top]", align 4
  ret i32 %"load.load.load.stack.items[load.load.stack.top]"
}

define i32 @main() {
entry:
  %array = alloca [8 x i32], align 4
  store %Stack zeroinitializer, %Stack* @stack, align 8
  %e = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 0
  store i32 0, i32* %e, align 4
  %e1 = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 1
  store i32 0, i32* %e1, align 4
  %e2 = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 2
  store i32 0, i32* %e2, align 4
  %e3 = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 3
  store i32 0, i32* %e3, align 4
  %e4 = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 4
  store i32 0, i32* %e4, align 4
  %e5 = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 5
  store i32 0, i32* %e5, align 4
  %e6 = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 6
  store i32 0, i32* %e6, align 4
  %e7 = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 7
  store i32 0, i32* %e7, align 4
  %array8 = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 0
  store i32* %array8, i32** getelementptr inbounds (%Stack, %Stack* @stack, i32 0, i32 0), align 8
  call void @push(i32 3)
  call void @push(i32 4)
  %"pop()" = call i32 @pop()
  %"pop()9" = call i32 @pop()
  %ADD = add i32 %"pop()", %"pop()9"
  call void @push(i32 %ADD)
  call void @push(i32 2)
  %"pop()10" = call i32 @pop()
  %"pop()11" = call i32 @pop()
  %MUL = mul i32 %"pop()10", %"pop()11"
  call void @push(i32 %MUL)
  %"pop()12" = call i32 @pop()
  ret i32 %"pop()12"
}
```

---

## 061 — program: structs used before declared, a struct local to a function

```ura
// expect: 203   (area 5 * 4 = 20 * 10, plus 3)
origin Point
origin.x = 2

fn area(r &Rect) i32:
    return (r.b.x - r.a.x) * (r.b.y - r.a.y)

fn make() i32:
    struct Local:
        v i32
    l Local
    l.v = 3
    return l.v

struct Rect:
    a Point
    b Point

struct Point:
    x i32
    y i32

r Rect
r.a = origin
r.b.x = 7
r.b.y = 4
return area(&r) * 10 + make()
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Point = type { i32, i32 }
%Rect = type { %Point, %Point }
%Local = type { i32 }

@origin = global %Point zeroinitializer
@r = global %Rect zeroinitializer

define i32 @area(%Rect* %0) {
entry:
  %r = alloca %Rect*, align 8
  store %Rect* %0, %Rect** %r, align 8
  %dref.r = load %Rect*, %Rect** %r, align 8
  %load.dref.r.b = getelementptr inbounds %Rect, %Rect* %dref.r, i32 0, i32 1
  %load.load.dref.r.b.x = getelementptr inbounds %Point, %Point* %load.dref.r.b, i32 0, i32 0
  %load.load.load.dref.r.b.x = load i32, i32* %load.load.dref.r.b.x, align 4
  %dref.r1 = load %Rect*, %Rect** %r, align 8
  %load.dref.r.a = getelementptr inbounds %Rect, %Rect* %dref.r1, i32 0, i32 0
  %load.load.dref.r.a.x = getelementptr inbounds %Point, %Point* %load.dref.r.a, i32 0, i32 0
  %load.load.load.dref.r.a.x = load i32, i32* %load.load.dref.r.a.x, align 4
  %SUB = sub i32 %load.load.load.dref.r.b.x, %load.load.load.dref.r.a.x
  %dref.r2 = load %Rect*, %Rect** %r, align 8
  %load.dref.r.b3 = getelementptr inbounds %Rect, %Rect* %dref.r2, i32 0, i32 1
  %load.load.dref.r.b.y = getelementptr inbounds %Point, %Point* %load.dref.r.b3, i32 0, i32 1
  %load.load.load.dref.r.b.y = load i32, i32* %load.load.dref.r.b.y, align 4
  %dref.r4 = load %Rect*, %Rect** %r, align 8
  %load.dref.r.a5 = getelementptr inbounds %Rect, %Rect* %dref.r4, i32 0, i32 0
  %load.load.dref.r.a.y = getelementptr inbounds %Point, %Point* %load.dref.r.a5, i32 0, i32 1
  %load.load.load.dref.r.a.y = load i32, i32* %load.load.dref.r.a.y, align 4
  %SUB6 = sub i32 %load.load.load.dref.r.b.y, %load.load.load.dref.r.a.y
  %MUL = mul i32 %SUB, %SUB6
  ret i32 %MUL
}

define i32 @make() {
entry:
  %l = alloca %Local, align 8
  store %Local zeroinitializer, %Local* %l, align 4
  %load.l.v = getelementptr inbounds %Local, %Local* %l, i32 0, i32 0
  store i32 3, i32* %load.l.v, align 4
  %load.l.v1 = getelementptr inbounds %Local, %Local* %l, i32 0, i32 0
  %load.load.l.v = load i32, i32* %load.l.v1, align 4
  ret i32 %load.load.l.v
}

define i32 @main() {
entry:
  store %Point zeroinitializer, %Point* @origin, align 4
  store i32 2, i32* getelementptr inbounds (%Point, %Point* @origin, i32 0, i32 0), align 4
  store %Rect zeroinitializer, %Rect* @r, align 4
  %load.origin = load %Point, %Point* @origin, align 4
  store %Point %load.origin, %Point* getelementptr inbounds (%Rect, %Rect* @r, i32 0, i32 0), align 4
  store i32 7, i32* getelementptr inbounds (%Rect, %Rect* @r, i32 0, i32 1, i32 0), align 4
  store i32 4, i32* getelementptr inbounds (%Rect, %Rect* @r, i32 0, i32 1, i32 1), align 4
  %"area(&r)" = call i32 @area(%Rect* @r)
  %MUL = mul i32 %"area(&r)", 10
  %"make()" = call i32 @make()
  %ADD = add i32 %MUL, %"make()"
  ret i32 %ADD
}
```

---

## 062 — program: a linked list built with chained methods that return refs

```ura
// expect: 44   (length 4 * 10, last value 4)
struct Node:
    value i32
    next &Node

    fn link(n &Node) &Node:
        self.next = &n
        return &n

    fn length() i32:
        n i32 = 1
        p &Node = &self
        while not isNullable(p.next):
            n += 1
            p = &p.next
        return n

a Node
b Node
c Node
d Node
d.value = 4
a.link(&b).link(&c).link(&d)
return a.length() * 10 + a.next.next.next.value
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Node = type { i32, %Node* }

@a = global %Node zeroinitializer
@b = global %Node zeroinitializer
@c = global %Node zeroinitializer
@d = global %Node zeroinitializer

define %Node* @Node.link(%Node* %0, %Node* %1) {
entry:
  %self = alloca %Node*, align 8
  %n = alloca %Node*, align 8
  store %Node* %0, %Node** %self, align 8
  store %Node* %1, %Node** %n, align 8
  %dref.self = load %Node*, %Node** %self, align 8
  %load.dref.self.next = getelementptr inbounds %Node, %Node* %dref.self, i32 0, i32 1
  %dref.n = load %Node*, %Node** %n, align 8
  store %Node* %dref.n, %Node** %load.dref.self.next, align 8
  %dref.n1 = load %Node*, %Node** %n, align 8
  ret %Node* %dref.n1
}

define i32 @Node.length(%Node* %0) {
entry:
  %self = alloca %Node*, align 8
  %n = alloca i32, align 4
  %p = alloca %Node*, align 8
  store %Node* %0, %Node** %self, align 8
  store i32 1, i32* %n, align 4
  %dref.self = load %Node*, %Node** %self, align 8
  store %Node* %dref.self, %Node** %p, align 8
  br label %cond

cond:                                             ; preds = %then, %entry
  %dref.p = load %Node*, %Node** %p, align 8
  %load.dref.p.next = getelementptr inbounds %Node, %Node* %dref.p, i32 0, i32 1
  %dref.load.dref.p.next = load %Node*, %Node** %load.dref.p.next, align 8
  %"isNullable(dref.load.dref.p.next)" = icmp eq %Node* %dref.load.dref.p.next, null
  %NOT = xor i1 %"isNullable(dref.load.dref.p.next)", true
  br i1 %NOT, label %then, label %endwhile

then:                                             ; preds = %cond
  %old = load i32, i32* %n, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %n, align 4
  %dref.p1 = load %Node*, %Node** %p, align 8
  %load.dref.p.next2 = getelementptr inbounds %Node, %Node* %dref.p1, i32 0, i32 1
  %dref.load.dref.p.next3 = load %Node*, %Node** %load.dref.p.next2, align 8
  store %Node* %dref.load.dref.p.next3, %Node** %p, align 8
  br label %cond

endwhile:                                         ; preds = %cond
  %load.n = load i32, i32* %n, align 4
  ret i32 %load.n
}

define i32 @main() {
entry:
  store %Node zeroinitializer, %Node* @a, align 8
  store %Node zeroinitializer, %Node* @b, align 8
  store %Node zeroinitializer, %Node* @c, align 8
  store %Node zeroinitializer, %Node* @d, align 8
  store i32 4, i32* getelementptr inbounds (%Node, %Node* @d, i32 0, i32 0), align 4
  %"link(&load.a,&b)" = call %Node* @Node.link(%Node* @a, %Node* @b)
  %"link(link(&load.a,&b),&c)" = call %Node* @Node.link(%Node* %"link(&load.a,&b)", %Node* @c)
  %"link(link(link(&load.a,&b),&c),&d)" = call %Node* @Node.link(%Node* %"link(link(&load.a,&b),&c)", %Node* @d)
  %"length(&load.a)" = call i32 @Node.length(%Node* @a)
  %MUL = mul i32 %"length(&load.a)", 10
  %dref.load.a.next = load %Node*, %Node** getelementptr inbounds (%Node, %Node* @a, i32 0, i32 1), align 8
  %load.dref.load.a.next.next = getelementptr inbounds %Node, %Node* %dref.load.a.next, i32 0, i32 1
  %dref.load.dref.load.a.next.next = load %Node*, %Node** %load.dref.load.a.next.next, align 8
  %load.dref.load.dref.load.a.next.next.next = getelementptr inbounds %Node, %Node* %dref.load.dref.load.a.next.next, i32 0, i32 1
  %dref.load.dref.load.dref.load.a.next.next.next = load %Node*, %Node** %load.dref.load.dref.load.a.next.next.next, align 8
  %load.dref.load.dref.load.dref.load.a.next.next.next.value = getelementptr inbounds %Node, %Node* %dref.load.dref.load.dref.load.a.next.next.next, i32 0, i32 0
  %load.load.dref.load.dref.load.dref.load.a.next.next.next.value = load i32, i32* %load.dref.load.dref.load.dref.load.a.next.next.next.value, align 4
  %ADD = add i32 %MUL, %load.load.dref.load.dref.load.dref.load.a.next.next.next.value
  ret i32 %ADD
}
```

---

## 063 — program: binary search tree: recursive methods through ref attributes

```ura
// expect: 213   (sum 210, depth 3)
struct Tree:
    value i32
    left &Tree
    right &Tree

    fn insert(t &Tree):
        if t.value < self.value:
            if isNullable(self.left): self.left = &t
            else: self.left.insert(&t)
        else:
            if isNullable(self.right): self.right = &t
            else: self.right.insert(&t)

    fn sum() i32:
        s i32 = self.value
        if not isNullable(self.left): s += self.left.sum()
        if not isNullable(self.right): s += self.right.sum()
        return s

    fn depth() i32:
        l i32 = 0
        r i32 = 0
        if not isNullable(self.left): l = self.left.depth()
        if not isNullable(self.right): r = self.right.depth()
        if l > r: return l + 1
        return r + 1

a Tree
b Tree
c Tree
d Tree
e Tree
a.value = 50
b.value = 30
c.value = 70
d.value = 20
e.value = 40
a.insert(&b)
a.insert(&c)
a.insert(&d)
a.insert(&e)
return a.sum() + a.depth()
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Tree = type { i32, %Tree*, %Tree* }

@a = global %Tree zeroinitializer
@b = global %Tree zeroinitializer
@c = global %Tree zeroinitializer
@d = global %Tree zeroinitializer
@e = global %Tree zeroinitializer

define void @Tree.insert(%Tree* %0, %Tree* %1) {
entry:
  %self = alloca %Tree*, align 8
  %t = alloca %Tree*, align 8
  store %Tree* %0, %Tree** %self, align 8
  store %Tree* %1, %Tree** %t, align 8
  %dref.t = load %Tree*, %Tree** %t, align 8
  %load.dref.t.value = getelementptr inbounds %Tree, %Tree* %dref.t, i32 0, i32 0
  %load.load.dref.t.value = load i32, i32* %load.dref.t.value, align 4
  %dref.self = load %Tree*, %Tree** %self, align 8
  %load.dref.self.value = getelementptr inbounds %Tree, %Tree* %dref.self, i32 0, i32 0
  %load.load.dref.self.value = load i32, i32* %load.dref.self.value, align 4
  %LT = icmp slt i32 %load.load.dref.t.value, %load.load.dref.self.value
  br i1 %LT, label %then, label %next

then:                                             ; preds = %entry
  %dref.self4 = load %Tree*, %Tree** %self, align 8
  %load.dref.self.left = getelementptr inbounds %Tree, %Tree* %dref.self4, i32 0, i32 1
  %dref.load.dref.self.left = load %Tree*, %Tree** %load.dref.self.left, align 8
  %"isNullable(dref.load.dref.self.left)" = icmp eq %Tree* %dref.load.dref.self.left, null
  br i1 %"isNullable(dref.load.dref.self.left)", label %then2, label %next3

next:                                             ; preds = %entry
  %dref.self15 = load %Tree*, %Tree** %self, align 8
  %load.dref.self.right = getelementptr inbounds %Tree, %Tree* %dref.self15, i32 0, i32 2
  %dref.load.dref.self.right = load %Tree*, %Tree** %load.dref.self.right, align 8
  %"isNullable(dref.load.dref.self.right)" = icmp eq %Tree* %dref.load.dref.self.right, null
  br i1 %"isNullable(dref.load.dref.self.right)", label %then13, label %next14

then2:                                            ; preds = %then
  %dref.self5 = load %Tree*, %Tree** %self, align 8
  %load.dref.self.left6 = getelementptr inbounds %Tree, %Tree* %dref.self5, i32 0, i32 1
  %dref.t7 = load %Tree*, %Tree** %t, align 8
  store %Tree* %dref.t7, %Tree** %load.dref.self.left6, align 8
  br label %endif1

next3:                                            ; preds = %then
  %dref.self8 = load %Tree*, %Tree** %self, align 8
  %load.dref.self.left9 = getelementptr inbounds %Tree, %Tree* %dref.self8, i32 0, i32 1
  %dref.load.dref.self.left10 = load %Tree*, %Tree** %load.dref.self.left9, align 8
  %dref.t11 = load %Tree*, %Tree** %t, align 8
  call void @Tree.insert(%Tree* %dref.load.dref.self.left10, %Tree* %dref.t11)
  br label %endif1

endif1:                                           ; preds = %next3, %then2
  br label %endif

then13:                                           ; preds = %next
  %dref.self16 = load %Tree*, %Tree** %self, align 8
  %load.dref.self.right17 = getelementptr inbounds %Tree, %Tree* %dref.self16, i32 0, i32 2
  %dref.t18 = load %Tree*, %Tree** %t, align 8
  store %Tree* %dref.t18, %Tree** %load.dref.self.right17, align 8
  br label %endif12

next14:                                           ; preds = %next
  %dref.self19 = load %Tree*, %Tree** %self, align 8
  %load.dref.self.right20 = getelementptr inbounds %Tree, %Tree* %dref.self19, i32 0, i32 2
  %dref.load.dref.self.right21 = load %Tree*, %Tree** %load.dref.self.right20, align 8
  %dref.t22 = load %Tree*, %Tree** %t, align 8
  call void @Tree.insert(%Tree* %dref.load.dref.self.right21, %Tree* %dref.t22)
  br label %endif12

endif12:                                          ; preds = %next14, %then13
  br label %endif

endif:                                            ; preds = %endif12, %endif1
  ret void
}

define i32 @Tree.sum(%Tree* %0) {
entry:
  %self = alloca %Tree*, align 8
  %s = alloca i32, align 4
  store %Tree* %0, %Tree** %self, align 8
  %dref.self = load %Tree*, %Tree** %self, align 8
  %load.dref.self.value = getelementptr inbounds %Tree, %Tree* %dref.self, i32 0, i32 0
  %load.load.dref.self.value = load i32, i32* %load.dref.self.value, align 4
  store i32 %load.load.dref.self.value, i32* %s, align 4
  %dref.self1 = load %Tree*, %Tree** %self, align 8
  %load.dref.self.left = getelementptr inbounds %Tree, %Tree* %dref.self1, i32 0, i32 1
  %dref.load.dref.self.left = load %Tree*, %Tree** %load.dref.self.left, align 8
  %"isNullable(dref.load.dref.self.left)" = icmp eq %Tree* %dref.load.dref.self.left, null
  %NOT = xor i1 %"isNullable(dref.load.dref.self.left)", true
  br i1 %NOT, label %then, label %endif

then:                                             ; preds = %entry
  %dref.self2 = load %Tree*, %Tree** %self, align 8
  %load.dref.self.left3 = getelementptr inbounds %Tree, %Tree* %dref.self2, i32 0, i32 1
  %dref.load.dref.self.left4 = load %Tree*, %Tree** %load.dref.self.left3, align 8
  %"sum(&load.dref.load.dref.self.left)" = call i32 @Tree.sum(%Tree* %dref.load.dref.self.left4)
  %old = load i32, i32* %s, align 4
  %ADD_ASSIGN = add i32 %old, %"sum(&load.dref.load.dref.self.left)"
  store i32 %ADD_ASSIGN, i32* %s, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %dref.self7 = load %Tree*, %Tree** %self, align 8
  %load.dref.self.right = getelementptr inbounds %Tree, %Tree* %dref.self7, i32 0, i32 2
  %dref.load.dref.self.right = load %Tree*, %Tree** %load.dref.self.right, align 8
  %"isNullable(dref.load.dref.self.right)" = icmp eq %Tree* %dref.load.dref.self.right, null
  %NOT8 = xor i1 %"isNullable(dref.load.dref.self.right)", true
  br i1 %NOT8, label %then6, label %endif5

then6:                                            ; preds = %endif
  %dref.self9 = load %Tree*, %Tree** %self, align 8
  %load.dref.self.right10 = getelementptr inbounds %Tree, %Tree* %dref.self9, i32 0, i32 2
  %dref.load.dref.self.right11 = load %Tree*, %Tree** %load.dref.self.right10, align 8
  %"sum(&load.dref.load.dref.self.right)" = call i32 @Tree.sum(%Tree* %dref.load.dref.self.right11)
  %old12 = load i32, i32* %s, align 4
  %ADD_ASSIGN13 = add i32 %old12, %"sum(&load.dref.load.dref.self.right)"
  store i32 %ADD_ASSIGN13, i32* %s, align 4
  br label %endif5

endif5:                                           ; preds = %then6, %endif
  %load.s = load i32, i32* %s, align 4
  ret i32 %load.s
}

define i32 @Tree.depth(%Tree* %0) {
entry:
  %self = alloca %Tree*, align 8
  %l = alloca i32, align 4
  %r = alloca i32, align 4
  store %Tree* %0, %Tree** %self, align 8
  store i32 0, i32* %l, align 4
  store i32 0, i32* %r, align 4
  %dref.self = load %Tree*, %Tree** %self, align 8
  %load.dref.self.left = getelementptr inbounds %Tree, %Tree* %dref.self, i32 0, i32 1
  %dref.load.dref.self.left = load %Tree*, %Tree** %load.dref.self.left, align 8
  %"isNullable(dref.load.dref.self.left)" = icmp eq %Tree* %dref.load.dref.self.left, null
  %NOT = xor i1 %"isNullable(dref.load.dref.self.left)", true
  br i1 %NOT, label %then, label %endif

then:                                             ; preds = %entry
  %dref.self1 = load %Tree*, %Tree** %self, align 8
  %load.dref.self.left2 = getelementptr inbounds %Tree, %Tree* %dref.self1, i32 0, i32 1
  %dref.load.dref.self.left3 = load %Tree*, %Tree** %load.dref.self.left2, align 8
  %"depth(&load.dref.load.dref.self.left)" = call i32 @Tree.depth(%Tree* %dref.load.dref.self.left3)
  store i32 %"depth(&load.dref.load.dref.self.left)", i32* %l, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %dref.self6 = load %Tree*, %Tree** %self, align 8
  %load.dref.self.right = getelementptr inbounds %Tree, %Tree* %dref.self6, i32 0, i32 2
  %dref.load.dref.self.right = load %Tree*, %Tree** %load.dref.self.right, align 8
  %"isNullable(dref.load.dref.self.right)" = icmp eq %Tree* %dref.load.dref.self.right, null
  %NOT7 = xor i1 %"isNullable(dref.load.dref.self.right)", true
  br i1 %NOT7, label %then5, label %endif4

then5:                                            ; preds = %endif
  %dref.self8 = load %Tree*, %Tree** %self, align 8
  %load.dref.self.right9 = getelementptr inbounds %Tree, %Tree* %dref.self8, i32 0, i32 2
  %dref.load.dref.self.right10 = load %Tree*, %Tree** %load.dref.self.right9, align 8
  %"depth(&load.dref.load.dref.self.right)" = call i32 @Tree.depth(%Tree* %dref.load.dref.self.right10)
  store i32 %"depth(&load.dref.load.dref.self.right)", i32* %r, align 4
  br label %endif4

endif4:                                           ; preds = %then5, %endif
  %load.l = load i32, i32* %l, align 4
  %load.r = load i32, i32* %r, align 4
  %GT = icmp sgt i32 %load.l, %load.r
  br i1 %GT, label %then12, label %endif11

then12:                                           ; preds = %endif4
  %load.l13 = load i32, i32* %l, align 4
  %ADD = add i32 %load.l13, 1
  ret i32 %ADD

endif11:                                          ; preds = %endif4
  %load.r14 = load i32, i32* %r, align 4
  %ADD15 = add i32 %load.r14, 1
  ret i32 %ADD15
}

define i32 @main() {
entry:
  store %Tree zeroinitializer, %Tree* @a, align 8
  store %Tree zeroinitializer, %Tree* @b, align 8
  store %Tree zeroinitializer, %Tree* @c, align 8
  store %Tree zeroinitializer, %Tree* @d, align 8
  store %Tree zeroinitializer, %Tree* @e, align 8
  store i32 50, i32* getelementptr inbounds (%Tree, %Tree* @a, i32 0, i32 0), align 4
  store i32 30, i32* getelementptr inbounds (%Tree, %Tree* @b, i32 0, i32 0), align 4
  store i32 70, i32* getelementptr inbounds (%Tree, %Tree* @c, i32 0, i32 0), align 4
  store i32 20, i32* getelementptr inbounds (%Tree, %Tree* @d, i32 0, i32 0), align 4
  store i32 40, i32* getelementptr inbounds (%Tree, %Tree* @e, i32 0, i32 0), align 4
  call void @Tree.insert(%Tree* @a, %Tree* @b)
  call void @Tree.insert(%Tree* @a, %Tree* @c)
  call void @Tree.insert(%Tree* @a, %Tree* @d)
  call void @Tree.insert(%Tree* @a, %Tree* @e)
  %"sum(&load.a)" = call i32 @Tree.sum(%Tree* @a)
  %"depth(&load.a)" = call i32 @Tree.depth(%Tree* @a)
  %ADD = add i32 %"sum(&load.a)", %"depth(&load.a)"
  ret i32 %ADD
}
```

---

## 064 — program: 2x2 matrix product with methods and ref parameters

```ura
// expect: 134   (19 + 22 + 43 + 50)
struct Mat:
    v i32[]

    fn at(r i32, c i32) i32:
        return self.v[r * 2 + c]

    fn mul(o &Mat, out &Mat):
        r i32 = 0
        while r < 2:
            c i32 = 0
            while c < 2:
                s i32 = 0
                k i32 = 0
                while k < 2:
                    s += self.at(r, k) * o.at(k, c)
                    k += 1
                out.v[r * 2 + c] = s
                c += 1
            r += 1

a Mat
b Mat
c Mat
a.v = [1, 2, 3, 4]
b.v = [5, 6, 7, 8]
c.v = [0, 0, 0, 0]
a.mul(&b, &c)
return c.v[0] + c.v[1] + c.v[2] + c.v[3]
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Mat = type { i32* }

@a = global %Mat zeroinitializer
@b = global %Mat zeroinitializer
@c = global %Mat zeroinitializer

define i32 @Mat.at(%Mat* %0, i32 %1, i32 %2) {
entry:
  %self = alloca %Mat*, align 8
  %r = alloca i32, align 4
  %c = alloca i32, align 4
  store %Mat* %0, %Mat** %self, align 8
  store i32 %1, i32* %r, align 4
  store i32 %2, i32* %c, align 4
  %dref.self = load %Mat*, %Mat** %self, align 8
  %load.dref.self.v = getelementptr inbounds %Mat, %Mat* %dref.self, i32 0, i32 0
  %load.load.dref.self.v = load i32*, i32** %load.dref.self.v, align 8
  %load.r = load i32, i32* %r, align 4
  %MUL = mul i32 %load.r, 2
  %load.c = load i32, i32* %c, align 4
  %ADD = add i32 %MUL, %load.c
  %"load.load.dref.self.v[load.r * 2 + load.c]" = getelementptr inbounds i32, i32* %load.load.dref.self.v, i32 %ADD
  %"load.load.load.dref.self.v[load.r * 2 + load.c]" = load i32, i32* %"load.load.dref.self.v[load.r * 2 + load.c]", align 4
  ret i32 %"load.load.load.dref.self.v[load.r * 2 + load.c]"
}

define void @Mat.mul(%Mat* %0, %Mat* %1, %Mat* %2) {
entry:
  %self = alloca %Mat*, align 8
  %o = alloca %Mat*, align 8
  %out = alloca %Mat*, align 8
  %r = alloca i32, align 4
  %c = alloca i32, align 4
  %s = alloca i32, align 4
  %k = alloca i32, align 4
  store %Mat* %0, %Mat** %self, align 8
  store %Mat* %1, %Mat** %o, align 8
  store %Mat* %2, %Mat** %out, align 8
  store i32 0, i32* %r, align 4
  br label %cond

cond:                                             ; preds = %endwhile3, %entry
  %load.r = load i32, i32* %r, align 4
  %LT = icmp slt i32 %load.r, 2
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  store i32 0, i32* %c, align 4
  br label %cond1

endwhile:                                         ; preds = %cond
  ret void

cond1:                                            ; preds = %endwhile7, %then
  %load.c = load i32, i32* %c, align 4
  %LT4 = icmp slt i32 %load.c, 2
  br i1 %LT4, label %then2, label %endwhile3

then2:                                            ; preds = %cond1
  store i32 0, i32* %s, align 4
  store i32 0, i32* %k, align 4
  br label %cond5

endwhile3:                                        ; preds = %cond1
  %old20 = load i32, i32* %r, align 4
  %ADD_ASSIGN21 = add i32 %old20, 1
  store i32 %ADD_ASSIGN21, i32* %r, align 4
  br label %cond

cond5:                                            ; preds = %then6, %then2
  %load.k = load i32, i32* %k, align 4
  %LT8 = icmp slt i32 %load.k, 2
  br i1 %LT8, label %then6, label %endwhile7

then6:                                            ; preds = %cond5
  %dref.self = load %Mat*, %Mat** %self, align 8
  %load.r9 = load i32, i32* %r, align 4
  %load.k10 = load i32, i32* %k, align 4
  %"at(&load.dref.self,load.r,load.k)" = call i32 @Mat.at(%Mat* %dref.self, i32 %load.r9, i32 %load.k10)
  %dref.o = load %Mat*, %Mat** %o, align 8
  %load.k11 = load i32, i32* %k, align 4
  %load.c12 = load i32, i32* %c, align 4
  %"at(&load.dref.o,load.k,load.c)" = call i32 @Mat.at(%Mat* %dref.o, i32 %load.k11, i32 %load.c12)
  %MUL = mul i32 %"at(&load.dref.self,load.r,load.k)", %"at(&load.dref.o,load.k,load.c)"
  %old = load i32, i32* %s, align 4
  %ADD_ASSIGN = add i32 %old, %MUL
  store i32 %ADD_ASSIGN, i32* %s, align 4
  %old13 = load i32, i32* %k, align 4
  %ADD_ASSIGN14 = add i32 %old13, 1
  store i32 %ADD_ASSIGN14, i32* %k, align 4
  br label %cond5

endwhile7:                                        ; preds = %cond5
  %dref.out = load %Mat*, %Mat** %out, align 8
  %load.dref.out.v = getelementptr inbounds %Mat, %Mat* %dref.out, i32 0, i32 0
  %load.load.dref.out.v = load i32*, i32** %load.dref.out.v, align 8
  %load.r15 = load i32, i32* %r, align 4
  %MUL16 = mul i32 %load.r15, 2
  %load.c17 = load i32, i32* %c, align 4
  %ADD = add i32 %MUL16, %load.c17
  %"load.load.dref.out.v[load.r * 2 + load.c]" = getelementptr inbounds i32, i32* %load.load.dref.out.v, i32 %ADD
  %load.s = load i32, i32* %s, align 4
  store i32 %load.s, i32* %"load.load.dref.out.v[load.r * 2 + load.c]", align 4
  %old18 = load i32, i32* %c, align 4
  %ADD_ASSIGN19 = add i32 %old18, 1
  store i32 %ADD_ASSIGN19, i32* %c, align 4
  br label %cond1
}

define i32 @main() {
entry:
  %array = alloca [4 x i32], align 4
  %array5 = alloca [4 x i32], align 4
  %array11 = alloca [4 x i32], align 4
  store %Mat zeroinitializer, %Mat* @a, align 8
  store %Mat zeroinitializer, %Mat* @b, align 8
  store %Mat zeroinitializer, %Mat* @c, align 8
  %e = getelementptr inbounds [4 x i32], [4 x i32]* %array, i32 0, i32 0
  store i32 1, i32* %e, align 4
  %e1 = getelementptr inbounds [4 x i32], [4 x i32]* %array, i32 0, i32 1
  store i32 2, i32* %e1, align 4
  %e2 = getelementptr inbounds [4 x i32], [4 x i32]* %array, i32 0, i32 2
  store i32 3, i32* %e2, align 4
  %e3 = getelementptr inbounds [4 x i32], [4 x i32]* %array, i32 0, i32 3
  store i32 4, i32* %e3, align 4
  %array4 = getelementptr inbounds [4 x i32], [4 x i32]* %array, i32 0, i32 0
  store i32* %array4, i32** getelementptr inbounds (%Mat, %Mat* @a, i32 0, i32 0), align 8
  %e6 = getelementptr inbounds [4 x i32], [4 x i32]* %array5, i32 0, i32 0
  store i32 5, i32* %e6, align 4
  %e7 = getelementptr inbounds [4 x i32], [4 x i32]* %array5, i32 0, i32 1
  store i32 6, i32* %e7, align 4
  %e8 = getelementptr inbounds [4 x i32], [4 x i32]* %array5, i32 0, i32 2
  store i32 7, i32* %e8, align 4
  %e9 = getelementptr inbounds [4 x i32], [4 x i32]* %array5, i32 0, i32 3
  store i32 8, i32* %e9, align 4
  %array10 = getelementptr inbounds [4 x i32], [4 x i32]* %array5, i32 0, i32 0
  store i32* %array10, i32** getelementptr inbounds (%Mat, %Mat* @b, i32 0, i32 0), align 8
  %e12 = getelementptr inbounds [4 x i32], [4 x i32]* %array11, i32 0, i32 0
  store i32 0, i32* %e12, align 4
  %e13 = getelementptr inbounds [4 x i32], [4 x i32]* %array11, i32 0, i32 1
  store i32 0, i32* %e13, align 4
  %e14 = getelementptr inbounds [4 x i32], [4 x i32]* %array11, i32 0, i32 2
  store i32 0, i32* %e14, align 4
  %e15 = getelementptr inbounds [4 x i32], [4 x i32]* %array11, i32 0, i32 3
  store i32 0, i32* %e15, align 4
  %array16 = getelementptr inbounds [4 x i32], [4 x i32]* %array11, i32 0, i32 0
  store i32* %array16, i32** getelementptr inbounds (%Mat, %Mat* @c, i32 0, i32 0), align 8
  call void @Mat.mul(%Mat* @a, %Mat* @b, %Mat* @c)
  %load.load.c.v = load i32*, i32** getelementptr inbounds (%Mat, %Mat* @c, i32 0, i32 0), align 8
  %"load.load.c.v[0]" = getelementptr inbounds i32, i32* %load.load.c.v, i32 0
  %"load.load.load.c.v[0]" = load i32, i32* %"load.load.c.v[0]", align 4
  %load.load.c.v17 = load i32*, i32** getelementptr inbounds (%Mat, %Mat* @c, i32 0, i32 0), align 8
  %"load.load.c.v[1]" = getelementptr inbounds i32, i32* %load.load.c.v17, i32 1
  %"load.load.load.c.v[1]" = load i32, i32* %"load.load.c.v[1]", align 4
  %ADD = add i32 %"load.load.load.c.v[0]", %"load.load.load.c.v[1]"
  %load.load.c.v18 = load i32*, i32** getelementptr inbounds (%Mat, %Mat* @c, i32 0, i32 0), align 8
  %"load.load.c.v[2]" = getelementptr inbounds i32, i32* %load.load.c.v18, i32 2
  %"load.load.load.c.v[2]" = load i32, i32* %"load.load.c.v[2]", align 4
  %ADD19 = add i32 %ADD, %"load.load.load.c.v[2]"
  %load.load.c.v20 = load i32*, i32** getelementptr inbounds (%Mat, %Mat* @c, i32 0, i32 0), align 8
  %"load.load.c.v[3]" = getelementptr inbounds i32, i32* %load.load.c.v20, i32 3
  %"load.load.load.c.v[3]" = load i32, i32* %"load.load.c.v[3]", align 4
  %ADD21 = add i32 %ADD19, %"load.load.load.c.v[3]"
  ret i32 %ADD21
}
```

