# milestone03

## index

- 001 — global scope: no main: the file is the program
- 002 — struct: declare: attributes start at zero
- 003 — struct: write an attribute, read it back
- 004 — struct: attributes don't overlap
- 005 — struct: compound assignment on an attribute
- 006 — struct: two variables of one struct are separate
- 007 — struct: attributes of different types
- 008 — struct: nested struct: r.b.x
- 009 — struct: attributes in a loop
- 010 — struct: array attribute
- 011 — struct: print attributes
- 012 — struct: a function returns a struct
- 013 — struct: a ref variable to a struct
- 014 — struct: write through a ref parameter
- 015 — struct: read through a ref parameter
- 016 — method: a method reads self
- 017 — method: a method writes self
- 018 — method: a method reads and writes self
- 019 — method: a method with arguments
- 020 — method: a method calls another method
- 021 — method: a method returns a bool
- 022 — method: a method in a loop
- 023 — method: two structs, one method name
- 024 — method: a method on a nested attribute
- 025 — method: overloaded method
- 026 — method: a method on a ref receiver
- 027 — method: a method through a ref changes the caller

---

## 001 — global scope: no main: the file is the program

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
target triple = "x86_64-unknown-linux-gnu"

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
  br label %while0

while0:                                           ; preds = %endif1, %entry
  br i1 true, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  br label %if1

if1:                                              ; preds = %while0.then
  %"*self" = load %Finder*, %Finder** %self, align 8
  %"*self.size" = getelementptr inbounds %Finder, %Finder* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.size" = load i32, i32* %"*self.size", align 4
  %EQ = icmp eq i32 %"$i", %"$*self.size"
  br i1 %EQ, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  br label %endloop

endif:                                            ; preds = %if1
  br label %if2

if2:                                              ; preds = %endif
  %"*self2" = load %Finder*, %Finder** %self, align 8
  %"*self.data" = getelementptr inbounds %Finder, %Finder* %"*self2", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$i3" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$i3"
  %"$*self.data[]" = load i32, i32* %"*self.data[]", align 4
  %"$v" = load i32, i32* %v, align 4
  %EQ4 = icmp eq i32 %"$*self.data[]", %"$v"
  br i1 %EQ4, label %if2.then, label %endif1

if2.then:                                         ; preds = %if2
  %"$i5" = load i32, i32* %i, align 4
  ret i32 %"$i5"

endif1:                                           ; preds = %if2
  %"$i6" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i6", 1
  store i32 %ADD, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %if1.then, %while0
  ret i32 -1
}

define i32 @main() {
entry:
  %arr = alloca [6 x i32], align 4
  store [6 x i32] [i32 4, i32 8, i32 15, i32 16, i32 23, i32 42], [6 x i32]* %arr, align 4
  %0 = getelementptr inbounds [6 x i32], [6 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** getelementptr inbounds (%Finder, %Finder* @f, i32 0, i32 0), align 8
  store i32 6, i32* getelementptr inbounds (%Finder, %Finder* @f, i32 0, i32 1), align 4
  %call.Finder.index_of = call i32 @Finder.index_of(%Finder* @f, i32 23)
  %MUL = mul i32 %call.Finder.index_of, 10
  %call.Finder.index_of1 = call i32 @Finder.index_of(%Finder* @f, i32 4)
  %ADD = add i32 %MUL, %call.Finder.index_of1
  %call.Finder.index_of2 = call i32 @Finder.index_of(%Finder* @f, i32 99)
  %ADD3 = add i32 %call.Finder.index_of2, 1
  %MUL4 = mul i32 %ADD3, 100
  %ADD5 = add i32 %ADD, %MUL4
  ret i32 %ADD5
}
```

---

## 002 — struct: declare: attributes start at zero

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
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32, i32 }

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %u.b = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  %"$u.a" = load i32, i32* %u.a, align 4
  %"$u.b" = load i32, i32* %u.b, align 4
  %ADD = add i32 %"$u.a", %"$u.b"
  ret i32 %ADD
}
```

---

## 003 — struct: write an attribute, read it back

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
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32, i32 }

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.b = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  store i32 7, i32* %u.b, align 4
  %u.b1 = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  %"$u.b" = load i32, i32* %u.b1, align 4
  ret i32 %"$u.b"
}
```

---

## 004 — struct: attributes don't overlap

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
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32, i32 }

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 3, i32* %u.a, align 4
  %u.b = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  store i32 4, i32* %u.b, align 4
  %u.a1 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a" = load i32, i32* %u.a1, align 4
  %MUL = mul i32 %"$u.a", 10
  %u.b2 = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  %"$u.b" = load i32, i32* %u.b2, align 4
  %ADD = add i32 %MUL, %"$u.b"
  ret i32 %ADD
}
```

---

## 005 — struct: compound assignment on an attribute

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
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32 }

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 5, i32* %u.a, align 4
  %u.a1 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a" = load i32, i32* %u.a1, align 4
  %ADD = add i32 %"$u.a", 2
  store i32 %ADD, i32* %u.a1, align 4
  %u.a2 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a3" = load i32, i32* %u.a2, align 4
  %MUL = mul i32 %"$u.a3", 3
  store i32 %MUL, i32* %u.a2, align 4
  %u.a4 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a5" = load i32, i32* %u.a4, align 4
  ret i32 %"$u.a5"
}
```

---

## 006 — struct: two variables of one struct are separate

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
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32 }

define i32 @main() {
entry:
  %u = alloca %User, align 8
  %v = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  store %User zeroinitializer, %User* %v, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 1, i32* %u.a, align 4
  %v.a = getelementptr inbounds %User, %User* %v, i32 0, i32 0
  store i32 2, i32* %v.a, align 4
  %u.a1 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a" = load i32, i32* %u.a1, align 4
  %MUL = mul i32 %"$u.a", 10
  %v.a2 = getelementptr inbounds %User, %User* %v, i32 0, i32 0
  %"$v.a" = load i32, i32* %v.a2, align 4
  %ADD = add i32 %MUL, %"$v.a"
  ret i32 %ADD
}
```

---

## 007 — struct: attributes of different types

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
target triple = "x86_64-unknown-linux-gnu"

%Item = type { i32, i1, i8 }

define i32 @main() {
entry:
  %it = alloca %Item, align 8
  store %Item zeroinitializer, %Item* %it, align 4
  %it.id = getelementptr inbounds %Item, %Item* %it, i32 0, i32 0
  store i32 4, i32* %it.id, align 4
  %it.used = getelementptr inbounds %Item, %Item* %it, i32 0, i32 1
  store i1 true, i1* %it.used, align 1
  %it.tag = getelementptr inbounds %Item, %Item* %it, i32 0, i32 2
  store i8 120, i8* %it.tag, align 1
  br label %if0

if0:                                              ; preds = %entry
  %it.used1 = getelementptr inbounds %Item, %Item* %it, i32 0, i32 1
  %it.tag2 = getelementptr inbounds %Item, %Item* %it, i32 0, i32 2
  %"$it.tag" = load i8, i8* %it.tag2, align 1
  %EQ = icmp eq i8 %"$it.tag", 120
  %"$it.used" = load i1, i1* %it.used1, align 1
  %AND = and i1 %"$it.used", %EQ
  br i1 %AND, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %it.id3 = getelementptr inbounds %Item, %Item* %it, i32 0, i32 0
  %"$it.id" = load i32, i32* %it.id3, align 4
  ret i32 %"$it.id"

endif:                                            ; preds = %if0
  ret i32 0
}
```

---

## 008 — struct: nested struct: r.b.x

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
target triple = "x86_64-unknown-linux-gnu"

%Rect = type { %Point, %Point }
%Point = type { i32, i32 }

define i32 @main() {
entry:
  %r = alloca %Rect, align 8
  store %Rect zeroinitializer, %Rect* %r, align 4
  %r.a = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 0
  %r.a.x = getelementptr inbounds %Point, %Point* %r.a, i32 0, i32 0
  store i32 1, i32* %r.a.x, align 4
  %r.b = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %r.b.x = getelementptr inbounds %Point, %Point* %r.b, i32 0, i32 0
  store i32 5, i32* %r.b.x, align 4
  %r.b1 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %r.b.y = getelementptr inbounds %Point, %Point* %r.b1, i32 0, i32 1
  store i32 2, i32* %r.b.y, align 4
  %r.b2 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %r.b.x3 = getelementptr inbounds %Point, %Point* %r.b2, i32 0, i32 0
  %r.a4 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 0
  %r.a.x5 = getelementptr inbounds %Point, %Point* %r.a4, i32 0, i32 0
  %"$r.b.x" = load i32, i32* %r.b.x3, align 4
  %"$r.a.x" = load i32, i32* %r.a.x5, align 4
  %SUB = sub i32 %"$r.b.x", %"$r.a.x"
  %r.b6 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %r.b.y7 = getelementptr inbounds %Point, %Point* %r.b6, i32 0, i32 1
  %"$r.b.y" = load i32, i32* %r.b.y7, align 4
  %ADD = add i32 %SUB, %"$r.b.y"
  ret i32 %ADD
}
```

---

## 009 — struct: attributes in a loop

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
target triple = "x86_64-unknown-linux-gnu"

%Counter = type { i32, i32 }

define i32 @main() {
entry:
  %c = alloca %Counter, align 8
  store %Counter zeroinitializer, %Counter* %c, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %c.n = getelementptr inbounds %Counter, %Counter* %c, i32 0, i32 0
  %"$c.n" = load i32, i32* %c.n, align 4
  %LT = icmp slt i32 %"$c.n", 4
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %c.n1 = getelementptr inbounds %Counter, %Counter* %c, i32 0, i32 0
  %"$c.n2" = load i32, i32* %c.n1, align 4
  %ADD = add i32 %"$c.n2", 1
  store i32 %ADD, i32* %c.n1, align 4
  %c.total = getelementptr inbounds %Counter, %Counter* %c, i32 0, i32 1
  %c.n3 = getelementptr inbounds %Counter, %Counter* %c, i32 0, i32 0
  %"$c.n4" = load i32, i32* %c.n3, align 4
  %"$c.total" = load i32, i32* %c.total, align 4
  %ADD5 = add i32 %"$c.total", %"$c.n4"
  store i32 %ADD5, i32* %c.total, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %c.total6 = getelementptr inbounds %Counter, %Counter* %c, i32 0, i32 1
  %"$c.total7" = load i32, i32* %c.total6, align 4
  ret i32 %"$c.total7"
}
```

---

## 010 — struct: array attribute

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
target triple = "x86_64-unknown-linux-gnu"

%Row = type { i32* }

define i32 @main() {
entry:
  %r = alloca %Row, align 8
  %arr = alloca [3 x i32], align 4
  store %Row zeroinitializer, %Row* %r, align 8
  %r.cells = getelementptr inbounds %Row, %Row* %r, i32 0, i32 0
  store [3 x i32] [i32 1, i32 2, i32 3], [3 x i32]* %arr, align 4
  %0 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %r.cells, align 8
  %r.cells1 = getelementptr inbounds %Row, %Row* %r, i32 0, i32 0
  %"$r.cells" = load i32*, i32** %r.cells1, align 8
  %"r.cells[]" = getelementptr inbounds i32, i32* %"$r.cells", i32 0
  store i32 9, i32* %"r.cells[]", align 4
  %r.cells2 = getelementptr inbounds %Row, %Row* %r, i32 0, i32 0
  %"$r.cells3" = load i32*, i32** %r.cells2, align 8
  %"r.cells[]4" = getelementptr inbounds i32, i32* %"$r.cells3", i32 0
  %r.cells5 = getelementptr inbounds %Row, %Row* %r, i32 0, i32 0
  %"$r.cells6" = load i32*, i32** %r.cells5, align 8
  %"r.cells[]7" = getelementptr inbounds i32, i32* %"$r.cells6", i32 2
  %"$r.cells[]" = load i32, i32* %"r.cells[]4", align 4
  %"$r.cells[]8" = load i32, i32* %"r.cells[]7", align 4
  %ADD = add i32 %"$r.cells[]", %"$r.cells[]8"
  ret i32 %ADD
}
```

---

## 011 — struct: print attributes

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
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32, i8* }

@str = private unnamed_addr constant [4 x i8] c"ura\00", align 1
@str.1 = private unnamed_addr constant [7 x i8] c"%s %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 8
  %u.id = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 42, i32* %u.id, align 4
  %u.name = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  store i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i8** %u.name, align 8
  %u.name1 = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  %u.id2 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.name" = load i8*, i8** %u.name1, align 8
  %"$u.id" = load i32, i32* %u.id2, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0), i8* %"$u.name", i32 %"$u.id")
  ret i32 0
}
```

---

## 012 — struct: a function returns a struct

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
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32 }

define %User @make(i32 %0) {
entry:
  %a = alloca i32, align 4
  %u = alloca %User, align 8
  store i32 %0, i32* %a, align 4
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$a" = load i32, i32* %a, align 4
  store i32 %"$a", i32* %u.a, align 4
  %"$u" = load %User, %User* %u, align 4
  ret %User %"$u"
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  %call.make = call %User @make(i32 5)
  store %User %call.make, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a" = load i32, i32* %u.a, align 4
  ret i32 %"$u.a"
}
```

---

## 013 — struct: a ref variable to a struct

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
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32 }

define i32 @main() {
entry:
  %u = alloca %User, align 8
  %p = alloca %User*, align 8
  store %User zeroinitializer, %User* %u, align 4
  store %User* %u, %User** %p, align 8
  %"*p" = load %User*, %User** %p, align 8
  %"*p.a" = getelementptr inbounds %User, %User* %"*p", i32 0, i32 0
  store i32 8, i32* %"*p.a", align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a" = load i32, i32* %u.a, align 4
  ret i32 %"$u.a"
}
```

---

## 014 — struct: write through a ref parameter

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
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32 }

define void @set(%User* %0) {
entry:
  %p = alloca %User*, align 8
  store %User* %0, %User** %p, align 8
  %"*p" = load %User*, %User** %p, align 8
  %"*p.a" = getelementptr inbounds %User, %User* %"*p", i32 0, i32 0
  store i32 3, i32* %"*p.a", align 4
  ret void
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  call void @set(%User* %u)
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a" = load i32, i32* %u.a, align 4
  ret i32 %"$u.a"
}
```

---

## 015 — struct: read through a ref parameter

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
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32, i32 }

define i32 @total(%User* %0) {
entry:
  %p = alloca %User*, align 8
  store %User* %0, %User** %p, align 8
  %"*p" = load %User*, %User** %p, align 8
  %"*p.a" = getelementptr inbounds %User, %User* %"*p", i32 0, i32 0
  %"*p1" = load %User*, %User** %p, align 8
  %"*p.b" = getelementptr inbounds %User, %User* %"*p1", i32 0, i32 1
  %"$*p.a" = load i32, i32* %"*p.a", align 4
  %"$*p.b" = load i32, i32* %"*p.b", align 4
  %ADD = add i32 %"$*p.a", %"$*p.b"
  ret i32 %ADD
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 2, i32* %u.a, align 4
  %u.b = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  store i32 5, i32* %u.b, align 4
  %call.total = call i32 @total(%User* %u)
  ret i32 %call.total
}
```

---

## 016 — method: a method reads self

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
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32 }

define i32 @User.get(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %"*self" = load %User*, %User** %self, align 8
  %"*self.a" = getelementptr inbounds %User, %User* %"*self", i32 0, i32 0
  %"$*self.a" = load i32, i32* %"*self.a", align 4
  ret i32 %"$*self.a"
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 4, i32* %u.a, align 4
  %call.User.get = call i32 @User.get(%User* %u)
  ret i32 %call.User.get
}
```

---

## 017 — method: a method writes self

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
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32 }

define void @User.set(%User* %0, i32 %1) {
entry:
  %self = alloca %User*, align 8
  %n = alloca i32, align 4
  store %User* %0, %User** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %User*, %User** %self, align 8
  %"*self.a" = getelementptr inbounds %User, %User* %"*self", i32 0, i32 0
  %"$n" = load i32, i32* %n, align 4
  store i32 %"$n", i32* %"*self.a", align 4
  ret void
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  call void @User.set(%User* %u, i32 9)
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a" = load i32, i32* %u.a, align 4
  ret i32 %"$u.a"
}
```

---

## 018 — method: a method reads and writes self

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
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32 }

define i32 @User.bump(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %"*self" = load %User*, %User** %self, align 8
  %"*self.a" = getelementptr inbounds %User, %User* %"*self", i32 0, i32 0
  %"$*self.a" = load i32, i32* %"*self.a", align 4
  %ADD = add i32 %"$*self.a", 1
  store i32 %ADD, i32* %"*self.a", align 4
  %"*self1" = load %User*, %User** %self, align 8
  %"*self.a2" = getelementptr inbounds %User, %User* %"*self1", i32 0, i32 0
  %"$*self.a3" = load i32, i32* %"*self.a2", align 4
  ret i32 %"$*self.a3"
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 5, i32* %u.a, align 4
  %call.User.bump = call i32 @User.bump(%User* %u)
  %call.User.bump1 = call i32 @User.bump(%User* %u)
  ret i32 %call.User.bump1
}
```

---

## 019 — method: a method with arguments

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
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32 }

define i32 @User.add(%User* %0, i32 %1) {
entry:
  %self = alloca %User*, align 8
  %n = alloca i32, align 4
  store %User* %0, %User** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %User*, %User** %self, align 8
  %"*self.a" = getelementptr inbounds %User, %User* %"*self", i32 0, i32 0
  %"$*self.a" = load i32, i32* %"*self.a", align 4
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$*self.a", %"$n"
  ret i32 %ADD
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 5, i32* %u.a, align 4
  %call.User.add = call i32 @User.add(%User* %u, i32 3)
  ret i32 %call.User.add
}
```

---

## 020 — method: a method calls another method

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
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32 }

define i32 @User.get(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %"*self" = load %User*, %User** %self, align 8
  %"*self.a" = getelementptr inbounds %User, %User* %"*self", i32 0, i32 0
  %"$*self.a" = load i32, i32* %"*self.a", align 4
  ret i32 %"$*self.a"
}

define i32 @User.twice(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %"*self" = load %User*, %User** %self, align 8
  %call.User.get = call i32 @User.get(%User* %"*self")
  %MUL = mul i32 %call.User.get, 2
  ret i32 %MUL
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 6, i32* %u.a, align 4
  %call.User.twice = call i32 @User.twice(%User* %u)
  ret i32 %call.User.twice
}
```

---

## 021 — method: a method returns a bool

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
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32 }

define i1 @User.adult(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %"*self" = load %User*, %User** %self, align 8
  %"*self.age" = getelementptr inbounds %User, %User* %"*self", i32 0, i32 0
  %"$*self.age" = load i32, i32* %"*self.age", align 4
  %GE = icmp sge i32 %"$*self.age", 18
  ret i1 %GE
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.age = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 20, i32* %u.age, align 4
  br label %if0

if0:                                              ; preds = %entry
  %call.User.adult = call i1 @User.adult(%User* %u)
  br i1 %call.User.adult, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  ret i32 0
}
```

---

## 022 — method: a method in a loop

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
target triple = "x86_64-unknown-linux-gnu"

%Counter = type { i32 }

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @Counter.tick(%Counter* %0) {
entry:
  %self = alloca %Counter*, align 8
  store %Counter* %0, %Counter** %self, align 8
  %"*self" = load %Counter*, %Counter** %self, align 8
  %"*self.n" = getelementptr inbounds %Counter, %Counter* %"*self", i32 0, i32 0
  %"$*self.n" = load i32, i32* %"*self.n", align 4
  %ADD = add i32 %"$*self.n", 1
  store i32 %ADD, i32* %"*self.n", align 4
  ret void
}

define i32 @main() {
entry:
  %c = alloca %Counter, align 8
  %i = alloca i32, align 4
  store %Counter zeroinitializer, %Counter* %c, align 4
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  call void @Counter.tick(%Counter* %c)
  %"$i1" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i1", 1
  store i32 %ADD, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %c.n = getelementptr inbounds %Counter, %Counter* %c, i32 0, i32 0
  %"$c.n" = load i32, i32* %c.n, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$c.n")
  %c.n2 = getelementptr inbounds %Counter, %Counter* %c, i32 0, i32 0
  %"$c.n3" = load i32, i32* %c.n2, align 4
  ret i32 %"$c.n3"
}
```

---

## 023 — method: two structs, one method name

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
target triple = "x86_64-unknown-linux-gnu"

%A = type { i32 }
%B = type { i32 }

define i32 @A.get(%A* %0) {
entry:
  %self = alloca %A*, align 8
  store %A* %0, %A** %self, align 8
  %"*self" = load %A*, %A** %self, align 8
  %"*self.v" = getelementptr inbounds %A, %A* %"*self", i32 0, i32 0
  %"$*self.v" = load i32, i32* %"*self.v", align 4
  ret i32 %"$*self.v"
}

define i32 @B.get(%B* %0) {
entry:
  %self = alloca %B*, align 8
  store %B* %0, %B** %self, align 8
  %"*self" = load %B*, %B** %self, align 8
  %"*self.v" = getelementptr inbounds %B, %B* %"*self", i32 0, i32 0
  %"$*self.v" = load i32, i32* %"*self.v", align 4
  %MUL = mul i32 %"$*self.v", 10
  ret i32 %MUL
}

define i32 @main() {
entry:
  %a = alloca %A, align 8
  %b = alloca %B, align 8
  store %A zeroinitializer, %A* %a, align 4
  store %B zeroinitializer, %B* %b, align 4
  %a.v = getelementptr inbounds %A, %A* %a, i32 0, i32 0
  store i32 1, i32* %a.v, align 4
  %b.v = getelementptr inbounds %B, %B* %b, i32 0, i32 0
  store i32 2, i32* %b.v, align 4
  %call.A.get = call i32 @A.get(%A* %a)
  %call.B.get = call i32 @B.get(%B* %b)
  %ADD = add i32 %call.A.get, %call.B.get
  ret i32 %ADD
}
```

---

## 024 — method: a method on a nested attribute

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
target triple = "x86_64-unknown-linux-gnu"

%Point = type { i32 }
%Rect = type { %Point, %Point }

define i32 @Point.get(%Point* %0) {
entry:
  %self = alloca %Point*, align 8
  store %Point* %0, %Point** %self, align 8
  %"*self" = load %Point*, %Point** %self, align 8
  %"*self.x" = getelementptr inbounds %Point, %Point* %"*self", i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  ret i32 %"$*self.x"
}

define i32 @main() {
entry:
  %r = alloca %Rect, align 8
  store %Rect zeroinitializer, %Rect* %r, align 4
  %r.b = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %r.b.x = getelementptr inbounds %Point, %Point* %r.b, i32 0, i32 0
  store i32 7, i32* %r.b.x, align 4
  %r.b1 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %call.Point.get = call i32 @Point.get(%Point* %r.b1)
  ret i32 %call.Point.get
}
```

---

## 025 — method: overloaded method

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
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32 }

define void @User.set(%User* %0, i32 %1) {
entry:
  %self = alloca %User*, align 8
  %n = alloca i32, align 4
  store %User* %0, %User** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %User*, %User** %self, align 8
  %"*self.a" = getelementptr inbounds %User, %User* %"*self", i32 0, i32 0
  %"$n" = load i32, i32* %n, align 4
  store i32 %"$n", i32* %"*self.a", align 4
  ret void
}

define void @User.set.1(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %"*self" = load %User*, %User** %self, align 8
  %"*self.a" = getelementptr inbounds %User, %User* %"*self", i32 0, i32 0
  store i32 1, i32* %"*self.a", align 4
  ret void
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  %first = alloca i32, align 4
  store %User zeroinitializer, %User* %u, align 4
  call void @User.set.1(%User* %u)
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a" = load i32, i32* %u.a, align 4
  store i32 %"$u.a", i32* %first, align 4
  call void @User.set(%User* %u, i32 5)
  %u.a1 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$first" = load i32, i32* %first, align 4
  %"$u.a2" = load i32, i32* %u.a1, align 4
  %ADD = add i32 %"$first", %"$u.a2"
  ret i32 %ADD
}
```

---

## 026 — method: a method on a ref receiver

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
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32 }

define i32 @User.get(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %"*self" = load %User*, %User** %self, align 8
  %"*self.a" = getelementptr inbounds %User, %User* %"*self", i32 0, i32 0
  %"$*self.a" = load i32, i32* %"*self.a", align 4
  ret i32 %"$*self.a"
}

define i32 @via(%User* %0) {
entry:
  %p = alloca %User*, align 8
  store %User* %0, %User** %p, align 8
  %"*p" = load %User*, %User** %p, align 8
  %call.User.get = call i32 @User.get(%User* %"*p")
  ret i32 %call.User.get
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 4, i32* %u.a, align 4
  %call.via = call i32 @via(%User* %u)
  ret i32 %call.via
}
```

---

## 027 — method: a method through a ref changes the caller

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
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32 }

define void @User.set(%User* %0, i32 %1) {
entry:
  %self = alloca %User*, align 8
  %n = alloca i32, align 4
  store %User* %0, %User** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %User*, %User** %self, align 8
  %"*self.a" = getelementptr inbounds %User, %User* %"*self", i32 0, i32 0
  %"$n" = load i32, i32* %n, align 4
  store i32 %"$n", i32* %"*self.a", align 4
  ret void
}

define void @reset(%User* %0) {
entry:
  %p = alloca %User*, align 8
  store %User* %0, %User** %p, align 8
  %"*p" = load %User*, %User** %p, align 8
  call void @User.set(%User* %"*p", i32 11)
  ret void
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 3, i32* %u.a, align 4
  call void @reset(%User* %u)
  %u.a1 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a" = load i32, i32* %u.a1, align 4
  ret i32 %"$u.a"
}
```

