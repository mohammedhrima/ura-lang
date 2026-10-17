# structs

## index

- 001 — declare: attributes start at zero
- 002 — write an attribute, read it back
- 003 — attributes don't overlap
- 004 — compound assignment on an attribute
- 005 — two variables of one struct are separate
- 006 — attributes of different types
- 007 — nested struct: r.b.x
- 008 — attributes in a loop
- 009 — array attribute
- 010 — print attributes
- 011 — a function returns a struct
- 012 — a ref variable to a struct
- 013 — write through a ref parameter
- 014 — read through a ref parameter

---

## 001 — declare: attributes start at zero

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

## 002 — write an attribute, read it back

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

## 003 — attributes don't overlap

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

## 004 — compound assignment on an attribute

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

## 005 — two variables of one struct are separate

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

## 006 — attributes of different types

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

## 007 — nested struct: r.b.x

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

## 008 — attributes in a loop

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

## 009 — array attribute

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

## 010 — print attributes

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

## 011 — a function returns a struct

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

## 012 — a ref variable to a struct

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

## 013 — write through a ref parameter

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

## 014 — read through a ref parameter

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

