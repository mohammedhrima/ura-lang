# milestone9

## index

- 001 — method: a pub fn is static: no self, called with ::
- 002 — method: calling a non-pub method with ::
- 003 — method: :: on a type that does not exist
- 004 — method: two structs declare the same pub fn, and statics compose
- 005 — operator: operator outside a struct definition
- 006 — operator: operator missing operator symbol
- 007 — operator: adding two structs without an operator + overload
- 008 — operator: operator + on two structs
- 009 — operator: two overloads of + with different right-hand types
- 010 — operator: operator == returning bool
- 011 — operator: no overload for that operator
- 012 — operator: assigning a different struct
- 013 — operator: indexing an array literal directly
- 014 — operator: operators and a method on the same struct
- 015 — operator: assigning a void result
- 016 — operator: a binary operator with mismatched scalar types
- 017 — operator: operator = copies on declaration and on reassignment
- 018 — operator: compound assignment operators
- 019 — operator: a compound operator with no overload
- 020 — operator: return unwinds every scope, and keeps the returned local
- 021 — operator: every operator on one struct, and = from an i32 overload
- 022 — operator: a temporary is destroyed at the end of its statement
- 023 — operator: every overloadable operator on one struct
- 024 — operator: bitwise operator overloads, and one returning a scalar
- 025 — operator: compound bitwise assignment overloads
- 026 — operator: bitwise and shift overloads taking a ref parameter
- 027 — operator: operator output replaces the default struct dump, nested too
- 028 — struct: pub inside struct must be followed by fn
- 029 — struct: instance method called via :: instead of .
- 030 — struct: pub fn called via . instead of ::
- 031 — struct: pub may only be used inside a struct
- 032 — operator: [] returns &i32, so b[0] reads and writes through
- 033 — operator: drop runs at the end of blocks, on continue, and not on a returned local
- 034 — operator: copying a struct with drop needs operator =
- 035 — operator: passing a struct with drop by value
- 036 — struct: from takes attributes and methods of a template, a struct and a grandparent; overrides win
- 037 — struct: from a struct that doesn't exist
- 038 — struct: String from Array<char> in "@/string", temporaries are dropped
- 039 — struct: a plain fn drop() is a normal method, only operator drop runs by itself
- 040 — ownership: nested arrays, an Array attribute, reassignment, drop(x), returning an attribute copies
- 041 — ownership: operator = taking the other side by value

---

## 001 — method: a pub fn is static: no self, called with ::

```ura
// stdout: floor = 7
// expect: 0

struct Room:
    floor i32

    pub fn create(f i32) Room:
        r Room
        r.floor = f
        return r

fn main() i32:
    r Room = Room::create(7)
    output("floor = ", r.floor, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Room = type { i32 }

@str = private unnamed_addr constant [12 x i8] c"floor = %d\0A\00", align 1

define %Room @Room.create(i32 %0) {
entry:
  %f = alloca i32, align 4
  %r = alloca %Room, align 8
  store i32 %0, i32* %f, align 4
  store %Room zeroinitializer, %Room* %r, align 4
  %r.floor = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  %"$f" = load i32, i32* %f, align 4
  store i32 %"$f", i32* %r.floor, align 4
  %"$r" = load %Room, %Room* %r, align 4
  ret %Room %"$r"
}

define i32 @main() {
entry:
  %r = alloca %Room, align 8
  %call.Room.create = call %Room @Room.create(i32 7)
  store %Room %call.Room.create, %Room* %r, align 4
  %r.floor = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  %"$r.floor" = load i32, i32* %r.floor, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i32 %"$r.floor")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 002 — method: calling a non-pub method with ::

```ura
// error: 'Room::depth' needs a receiver; call it on a value, or declare it 'pub fn' to make it static

struct Room:
    floor i32

    fn depth() i32:
        return self.floor

fn main() i32:
    output(Room::depth(), "\n")
    return 0
```

---

## 003 — method: :: on a type that does not exist

```ura
// error: Unknown type 'Nope'

fn main() i32:
    output(Nope::create(), "\n")
    return 0
```

---

## 004 — method: two structs declare the same pub fn, and statics compose

```ura
// stdout: room 3 tower 9
// stdout: zero 0
// stdout: level 40
// stdout: method 5
// stdout: mixed 3
// expect: 0

struct Room:
    floor i32

    pub fn create(f i32) Room:
        r Room
        r.floor = f
        return r

    pub fn zero() Room:
        return Room::create(0)

    pub fn level(f i32) i32:
        return f * 10

    fn depth() i32:
        return self.floor

struct Tower:
    height i32

    pub fn create(h i32) Tower:
        t Tower
        t.height = h
        return t

fn main() i32:
    a Room  = Room::create(3)
    b Tower = Tower::create(9)
    output("room ", a.floor, " tower ", b.height, "\n")
    output("zero ", Room::zero().floor, "\n")
    output("level ", Room::level(4), "\n")
    output("method ", Room::create(5).depth(), "\n")
    n i32 = Room::create(2).depth() + Tower::create(1).height
    output("mixed ", n, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Room = type { i32 }
%Tower = type { i32 }

@str = private unnamed_addr constant [18 x i8] c"room %d tower %d\0A\00", align 1
@str.1 = private unnamed_addr constant [9 x i8] c"zero %d\0A\00", align 1
@str.2 = private unnamed_addr constant [10 x i8] c"level %d\0A\00", align 1
@str.3 = private unnamed_addr constant [11 x i8] c"method %d\0A\00", align 1
@str.4 = private unnamed_addr constant [10 x i8] c"mixed %d\0A\00", align 1

define %Room @Room.create(i32 %0) {
entry:
  %f = alloca i32, align 4
  %r = alloca %Room, align 8
  store i32 %0, i32* %f, align 4
  store %Room zeroinitializer, %Room* %r, align 4
  %r.floor = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  %"$f" = load i32, i32* %f, align 4
  store i32 %"$f", i32* %r.floor, align 4
  %"$r" = load %Room, %Room* %r, align 4
  ret %Room %"$r"
}

define %Room @Room.zero() {
entry:
  %call.Room.create = call %Room @Room.create(i32 0)
  ret %Room %call.Room.create
}

define i32 @Room.level(i32 %0) {
entry:
  %f = alloca i32, align 4
  store i32 %0, i32* %f, align 4
  %"$f" = load i32, i32* %f, align 4
  %MUL = mul i32 %"$f", 10
  ret i32 %MUL
}

define i32 @Room.depth(%Room* %0) {
entry:
  %self = alloca %Room*, align 8
  store %Room* %0, %Room** %self, align 8
  %"*self" = load %Room*, %Room** %self, align 8
  %"*self.floor" = getelementptr inbounds %Room, %Room* %"*self", i32 0, i32 0
  %"$*self.floor" = load i32, i32* %"*self.floor", align 4
  ret i32 %"$*self.floor"
}

define %Tower @Tower.create(i32 %0) {
entry:
  %h = alloca i32, align 4
  %t = alloca %Tower, align 8
  store i32 %0, i32* %h, align 4
  store %Tower zeroinitializer, %Tower* %t, align 4
  %t.height = getelementptr inbounds %Tower, %Tower* %t, i32 0, i32 0
  %"$h" = load i32, i32* %h, align 4
  store i32 %"$h", i32* %t.height, align 4
  %"$t" = load %Tower, %Tower* %t, align 4
  ret %Tower %"$t"
}

define i32 @main() {
entry:
  %a = alloca %Room, align 8
  %b = alloca %Tower, align 8
  %tmp = alloca %Room, align 8
  %tmp4 = alloca %Room, align 8
  %n = alloca i32, align 4
  %tmp7 = alloca %Room, align 8
  %tmp10 = alloca %Tower, align 8
  %call.Room.create = call %Room @Room.create(i32 3)
  store %Room %call.Room.create, %Room* %a, align 4
  %call.Tower.create = call %Tower @Tower.create(i32 9)
  store %Tower %call.Tower.create, %Tower* %b, align 4
  %a.floor = getelementptr inbounds %Room, %Room* %a, i32 0, i32 0
  %b.height = getelementptr inbounds %Tower, %Tower* %b, i32 0, i32 0
  %"$a.floor" = load i32, i32* %a.floor, align 4
  %"$b.height" = load i32, i32* %b.height, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str, i32 0, i32 0), i32 %"$a.floor", i32 %"$b.height")
  %call.Room.zero = call %Room @Room.zero()
  store %Room %call.Room.zero, %Room* %tmp, align 4
  %tmp.floor = getelementptr inbounds %Room, %Room* %tmp, i32 0, i32 0
  %"$tmp.floor" = load i32, i32* %tmp.floor, align 4
  %call.dprintf1 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0), i32 %"$tmp.floor")
  %call.Room.level = call i32 @Room.level(i32 4)
  %call.dprintf2 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.2, i32 0, i32 0), i32 %call.Room.level)
  %call.Room.create3 = call %Room @Room.create(i32 5)
  store %Room %call.Room.create3, %Room* %tmp4, align 4
  %call.Room.depth = call i32 @Room.depth(%Room* %tmp4)
  %call.dprintf5 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.3, i32 0, i32 0), i32 %call.Room.depth)
  %call.Room.create6 = call %Room @Room.create(i32 2)
  store %Room %call.Room.create6, %Room* %tmp7, align 4
  %call.Room.depth8 = call i32 @Room.depth(%Room* %tmp7)
  %call.Tower.create9 = call %Tower @Tower.create(i32 1)
  store %Tower %call.Tower.create9, %Tower* %tmp10, align 4
  %tmp.height = getelementptr inbounds %Tower, %Tower* %tmp10, i32 0, i32 0
  %"$tmp.height" = load i32, i32* %tmp.height, align 4
  %ADD = add i32 %call.Room.depth8, %"$tmp.height"
  store i32 %ADD, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %call.dprintf11 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.4, i32 0, i32 0), i32 %"$n")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 005 — operator: operator outside a struct definition

```ura
// error: 'operator' may only be declared inside a struct
use "@/header"

operator + (a i32, b i32) i32:
    return a + b

fn main() i32:
    output("hi\n")
    return 0
```

### stderr

```
error: test.ura:4:1 'operator' may only be declared inside a struct
  |
4 | operator + (a i32, b i32) i32:
  | ^^^^^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 006 — operator: operator missing operator symbol

```ura
// error: Expected an operator after 'operator' (one of + - * / % & | ^ << >> == != < > <= >= = += -= *= /= %= &= |= ^= <<= >>= drop)
use "@/header"

struct Vec:
    x i32

    operator () Vec:
        return self

fn main() i32:
    v Vec
    v.x = 1
    return 0
```

---

## 007 — operator: adding two structs without an operator + overload

```ura
// error: Struct Vec has no 'operator +' taking Vec; declare one inside the struct
use "@/header"

struct Vec:
    x i32
    y i32

fn main() i32:
    a Vec
    b Vec
    c Vec = a + b
    output(c.x, "\n")
    return 0
```

---

## 008 — operator: operator + on two structs

```ura
// stdout: 11 22
// expect: 0

struct Vec:
    x i32
    y i32

    pub fn create(a i32, b i32) Vec:
        v Vec
        v.x = a
        v.y = b
        return v

    operator +(other Vec) Vec:
        return Vec::create(self.x + other.x, self.y + other.y)

fn main() i32:
    a Vec = Vec::create(1, 2)
    b Vec = Vec::create(10, 20)
    c Vec = a + b
    output(c.x, " ", c.y, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Vec = type { i32, i32 }

@str = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1

define %Vec @Vec.create(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %v = alloca %Vec, align 8
  store i32 %0, i32* %a, align 4
  store i32 %1, i32* %b, align 4
  store %Vec zeroinitializer, %Vec* %v, align 4
  %v.x = getelementptr inbounds %Vec, %Vec* %v, i32 0, i32 0
  %"$a" = load i32, i32* %a, align 4
  store i32 %"$a", i32* %v.x, align 4
  %v.y = getelementptr inbounds %Vec, %Vec* %v, i32 0, i32 1
  %"$b" = load i32, i32* %b, align 4
  store i32 %"$b", i32* %v.y, align 4
  %"$v" = load %Vec, %Vec* %v, align 4
  ret %Vec %"$v"
}

define %Vec @"Vec.+"(%Vec* %0, %Vec %1) {
entry:
  %self = alloca %Vec*, align 8
  %other = alloca %Vec, align 8
  store %Vec* %0, %Vec** %self, align 8
  store %Vec %1, %Vec* %other, align 4
  %"*self" = load %Vec*, %Vec** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec, %Vec* %"*self", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$other.x" = load i32, i32* %other.x, align 4
  %ADD = add i32 %"$*self.x", %"$other.x"
  %"*self1" = load %Vec*, %Vec** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec, %Vec* %"*self1", i32 0, i32 1
  %other.y = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 1
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  %"$other.y" = load i32, i32* %other.y, align 4
  %ADD2 = add i32 %"$*self.y", %"$other.y"
  %call.Vec.create = call %Vec @Vec.create(i32 %ADD, i32 %ADD2)
  ret %Vec %call.Vec.create
}

define i32 @main() {
entry:
  %a = alloca %Vec, align 8
  %b = alloca %Vec, align 8
  %c = alloca %Vec, align 8
  %call.Vec.create = call %Vec @Vec.create(i32 1, i32 2)
  store %Vec %call.Vec.create, %Vec* %a, align 4
  %call.Vec.create1 = call %Vec @Vec.create(i32 10, i32 20)
  store %Vec %call.Vec.create1, %Vec* %b, align 4
  %"$b" = load %Vec, %Vec* %b, align 4
  %"call.Vec.+" = call %Vec @"Vec.+"(%Vec* %a, %Vec %"$b")
  store %Vec %"call.Vec.+", %Vec* %c, align 4
  %c.x = getelementptr inbounds %Vec, %Vec* %c, i32 0, i32 0
  %c.y = getelementptr inbounds %Vec, %Vec* %c, i32 0, i32 1
  %"$c.x" = load i32, i32* %c.x, align 4
  %"$c.y" = load i32, i32* %c.y, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %"$c.x", i32 %"$c.y")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 009 — operator: two overloads of + with different right-hand types

```ura
// stdout: 11 6
// expect: 0

struct Vec:
    x i32

    pub fn create(a i32) Vec:
        v Vec
        v.x = a
        return v

    operator +(other Vec) Vec:
        return Vec::create(self.x + other.x)

    operator +(n i32) Vec:
        return Vec::create(self.x + n)

fn main() i32:
    a Vec = Vec::create(1)
    b Vec = Vec::create(10)
    output((a + b).x, " ", (a + 5).x, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Vec = type { i32 }

@str = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1

define %Vec @Vec.create(i32 %0) {
entry:
  %a = alloca i32, align 4
  %v = alloca %Vec, align 8
  store i32 %0, i32* %a, align 4
  store %Vec zeroinitializer, %Vec* %v, align 4
  %v.x = getelementptr inbounds %Vec, %Vec* %v, i32 0, i32 0
  %"$a" = load i32, i32* %a, align 4
  store i32 %"$a", i32* %v.x, align 4
  %"$v" = load %Vec, %Vec* %v, align 4
  ret %Vec %"$v"
}

define %Vec @"Vec.+"(%Vec* %0, %Vec %1) {
entry:
  %self = alloca %Vec*, align 8
  %other = alloca %Vec, align 8
  store %Vec* %0, %Vec** %self, align 8
  store %Vec %1, %Vec* %other, align 4
  %"*self" = load %Vec*, %Vec** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec, %Vec* %"*self", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$other.x" = load i32, i32* %other.x, align 4
  %ADD = add i32 %"$*self.x", %"$other.x"
  %call.Vec.create = call %Vec @Vec.create(i32 %ADD)
  ret %Vec %call.Vec.create
}

define %Vec @"Vec.+.1"(%Vec* %0, i32 %1) {
entry:
  %self = alloca %Vec*, align 8
  %n = alloca i32, align 4
  store %Vec* %0, %Vec** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %Vec*, %Vec** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec, %Vec* %"*self", i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$*self.x", %"$n"
  %call.Vec.create = call %Vec @Vec.create(i32 %ADD)
  ret %Vec %call.Vec.create
}

define i32 @main() {
entry:
  %a = alloca %Vec, align 8
  %b = alloca %Vec, align 8
  %tmp = alloca %Vec, align 8
  %tmp3 = alloca %Vec, align 8
  %call.Vec.create = call %Vec @Vec.create(i32 1)
  store %Vec %call.Vec.create, %Vec* %a, align 4
  %call.Vec.create1 = call %Vec @Vec.create(i32 10)
  store %Vec %call.Vec.create1, %Vec* %b, align 4
  %"$b" = load %Vec, %Vec* %b, align 4
  %"call.Vec.+" = call %Vec @"Vec.+"(%Vec* %a, %Vec %"$b")
  store %Vec %"call.Vec.+", %Vec* %tmp, align 4
  %tmp.x = getelementptr inbounds %Vec, %Vec* %tmp, i32 0, i32 0
  %"call.Vec.+2" = call %Vec @"Vec.+.1"(%Vec* %a, i32 5)
  store %Vec %"call.Vec.+2", %Vec* %tmp3, align 4
  %tmp.x4 = getelementptr inbounds %Vec, %Vec* %tmp3, i32 0, i32 0
  %"$tmp.x" = load i32, i32* %tmp.x, align 4
  %"$tmp.x5" = load i32, i32* %tmp.x4, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %"$tmp.x", i32 %"$tmp.x5")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 010 — operator: operator == returning bool

```ura
// stdout: True False
// expect: 0

struct Vec:
    x i32

    pub fn create(a i32) Vec:
        v Vec
        v.x = a
        return v

    operator ==(other Vec) bool:
        return self.x == other.x

fn main() i32:
    a Vec = Vec::create(3)
    b Vec = Vec::create(3)
    c Vec = Vec::create(9)
    output(a == b, " ", a == c, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Vec = type { i32 }

@str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.2 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.4 = private unnamed_addr constant [7 x i8] c"%s %s\0A\00", align 1

define %Vec @Vec.create(i32 %0) {
entry:
  %a = alloca i32, align 4
  %v = alloca %Vec, align 8
  store i32 %0, i32* %a, align 4
  store %Vec zeroinitializer, %Vec* %v, align 4
  %v.x = getelementptr inbounds %Vec, %Vec* %v, i32 0, i32 0
  %"$a" = load i32, i32* %a, align 4
  store i32 %"$a", i32* %v.x, align 4
  %"$v" = load %Vec, %Vec* %v, align 4
  ret %Vec %"$v"
}

define i1 @"Vec.=="(%Vec* %0, %Vec %1) {
entry:
  %self = alloca %Vec*, align 8
  %other = alloca %Vec, align 8
  store %Vec* %0, %Vec** %self, align 8
  store %Vec %1, %Vec* %other, align 4
  %"*self" = load %Vec*, %Vec** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec, %Vec* %"*self", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$other.x" = load i32, i32* %other.x, align 4
  %EQ = icmp eq i32 %"$*self.x", %"$other.x"
  ret i1 %EQ
}

define i32 @main() {
entry:
  %a = alloca %Vec, align 8
  %b = alloca %Vec, align 8
  %c = alloca %Vec, align 8
  %bool.text = alloca i8*, align 8
  %bool.text5 = alloca i8*, align 8
  %call.Vec.create = call %Vec @Vec.create(i32 3)
  store %Vec %call.Vec.create, %Vec* %a, align 4
  %call.Vec.create2 = call %Vec @Vec.create(i32 3)
  store %Vec %call.Vec.create2, %Vec* %b, align 4
  %call.Vec.create3 = call %Vec @Vec.create(i32 9)
  store %Vec %call.Vec.create3, %Vec* %c, align 4
  %"$b" = load %Vec, %Vec* %b, align 4
  %"call.Vec.==" = call i1 @"Vec.=="(%Vec* %a, %Vec %"$b")
  %"$c" = load %Vec, %Vec* %c, align 4
  %"call.Vec.==4" = call i1 @"Vec.=="(%Vec* %a, %Vec %"$c")
  store i8* null, i8** %bool.text, align 8
  br label %if0

if0:                                              ; preds = %entry
  br i1 %"call.Vec.==", label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else1:                                            ; preds = %if0
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  store i8* null, i8** %bool.text5, align 8
  br label %if2

if2:                                              ; preds = %endif
  br i1 %"call.Vec.==4", label %if2.then, label %else3

if2.then:                                         ; preds = %if2
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.2, i32 0, i32 0), i8** %bool.text5, align 8
  br label %endif1

else3:                                            ; preds = %if2
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i8** %bool.text5, align 8
  br label %endif1

endif1:                                           ; preds = %else3, %if2.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %"$bool.text6" = load i8*, i8** %bool.text5, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.4, i32 0, i32 0), i8* %"$bool.text", i8* %"$bool.text6")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 011 — operator: no overload for that operator

```ura
// error: Struct Vec has no 'operator -' taking Vec; declare one inside the struct

struct Vec:
    x i32

fn main() i32:
    a Vec
    b Vec
    c Vec = a - b
    return 0
```

---

## 012 — operator: assigning a different struct

```ura
// error: Cannot assign Point to Vec; they are different structs

struct Vec:
    x i32

struct Point:
    x i32

fn main() i32:
    v Vec
    p Point
    v = p
    return 0
```

---

## 013 — operator: indexing an array literal directly

```ura
// stdout: 20
// expect: 0

fn main() i32:
    output([10, 20, 30][1], "\n")
    return 0
```

---

## 014 — operator: operators and a method on the same struct

```ura
// stdout: (11, 22)
// stdout: a == a  True
// stdout: a == b  False
// stdout: (11, 22)
// expect: 0

struct Vec:
    x i32
    y i32

    pub fn create(a i32, b i32) Vec:
        v Vec
        v.x = a
        v.y = b
        return v

    operator +(other Vec) Vec:
        return Vec::create(self.x + other.x, self.y + other.y)

    operator ==(other Vec) bool:
        return self.x == other.x and self.y == other.y

    fn show():
        output("(", self.x, ", ", self.y, ")\n")

fn main() i32:
    a Vec = Vec::create(1, 2)
    b Vec = Vec::create(10, 20)

    c Vec = a + b
    c.show()

    // the same variable on both sides
    output("a == a  ", a == a, "\n")
    output("a == b  ", a == b, "\n")

    // an operator result used directly as a receiver
    (a + b).show()
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Vec = type { i32, i32 }

@str = private unnamed_addr constant [10 x i8] c"(%d, %d)\0A\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.3 = private unnamed_addr constant [12 x i8] c"a == a  %s\0A\00", align 1
@str.4 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.5 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.6 = private unnamed_addr constant [12 x i8] c"a == b  %s\0A\00", align 1

define %Vec @Vec.create(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %v = alloca %Vec, align 8
  store i32 %0, i32* %a, align 4
  store i32 %1, i32* %b, align 4
  store %Vec zeroinitializer, %Vec* %v, align 4
  %v.x = getelementptr inbounds %Vec, %Vec* %v, i32 0, i32 0
  %"$a" = load i32, i32* %a, align 4
  store i32 %"$a", i32* %v.x, align 4
  %v.y = getelementptr inbounds %Vec, %Vec* %v, i32 0, i32 1
  %"$b" = load i32, i32* %b, align 4
  store i32 %"$b", i32* %v.y, align 4
  %"$v" = load %Vec, %Vec* %v, align 4
  ret %Vec %"$v"
}

define %Vec @"Vec.+"(%Vec* %0, %Vec %1) {
entry:
  %self = alloca %Vec*, align 8
  %other = alloca %Vec, align 8
  store %Vec* %0, %Vec** %self, align 8
  store %Vec %1, %Vec* %other, align 4
  %"*self" = load %Vec*, %Vec** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec, %Vec* %"*self", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$other.x" = load i32, i32* %other.x, align 4
  %ADD = add i32 %"$*self.x", %"$other.x"
  %"*self1" = load %Vec*, %Vec** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec, %Vec* %"*self1", i32 0, i32 1
  %other.y = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 1
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  %"$other.y" = load i32, i32* %other.y, align 4
  %ADD2 = add i32 %"$*self.y", %"$other.y"
  %call.Vec.create = call %Vec @Vec.create(i32 %ADD, i32 %ADD2)
  ret %Vec %call.Vec.create
}

define i1 @"Vec.=="(%Vec* %0, %Vec %1) {
entry:
  %self = alloca %Vec*, align 8
  %other = alloca %Vec, align 8
  store %Vec* %0, %Vec** %self, align 8
  store %Vec %1, %Vec* %other, align 4
  %"*self" = load %Vec*, %Vec** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec, %Vec* %"*self", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$other.x" = load i32, i32* %other.x, align 4
  %EQ = icmp eq i32 %"$*self.x", %"$other.x"
  %"*self1" = load %Vec*, %Vec** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec, %Vec* %"*self1", i32 0, i32 1
  %other.y = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 1
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  %"$other.y" = load i32, i32* %other.y, align 4
  %EQ2 = icmp eq i32 %"$*self.y", %"$other.y"
  %AND = and i1 %EQ, %EQ2
  ret i1 %AND
}

define void @Vec.show(%Vec* %0) {
entry:
  %self = alloca %Vec*, align 8
  store %Vec* %0, %Vec** %self, align 8
  %"*self" = load %Vec*, %Vec** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec, %Vec* %"*self", i32 0, i32 0
  %"*self1" = load %Vec*, %Vec** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec, %Vec* %"*self1", i32 0, i32 1
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %"$*self.x", i32 %"$*self.y")
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Vec, align 8
  %b = alloca %Vec, align 8
  %c = alloca %Vec, align 8
  %bool.text = alloca i8*, align 8
  %bool.text5 = alloca i8*, align 8
  %tmp = alloca %Vec, align 8
  %call.Vec.create = call %Vec @Vec.create(i32 1, i32 2)
  store %Vec %call.Vec.create, %Vec* %a, align 4
  %call.Vec.create2 = call %Vec @Vec.create(i32 10, i32 20)
  store %Vec %call.Vec.create2, %Vec* %b, align 4
  %"$b" = load %Vec, %Vec* %b, align 4
  %"call.Vec.+" = call %Vec @"Vec.+"(%Vec* %a, %Vec %"$b")
  store %Vec %"call.Vec.+", %Vec* %c, align 4
  call void @Vec.show(%Vec* %c)
  %"$a" = load %Vec, %Vec* %a, align 4
  %"call.Vec.==" = call i1 @"Vec.=="(%Vec* %a, %Vec %"$a")
  store i8* null, i8** %bool.text, align 8
  br label %if0

if0:                                              ; preds = %entry
  br i1 %"call.Vec.==", label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else1:                                            ; preds = %if0
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.3, i32 0, i32 0), i8* %"$bool.text")
  %"$b3" = load %Vec, %Vec* %b, align 4
  %"call.Vec.==4" = call i1 @"Vec.=="(%Vec* %a, %Vec %"$b3")
  store i8* null, i8** %bool.text5, align 8
  br label %if2

if2:                                              ; preds = %endif
  br i1 %"call.Vec.==4", label %if2.then, label %else3

if2.then:                                         ; preds = %if2
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.4, i32 0, i32 0), i8** %bool.text5, align 8
  br label %endif1

else3:                                            ; preds = %if2
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.5, i32 0, i32 0), i8** %bool.text5, align 8
  br label %endif1

endif1:                                           ; preds = %else3, %if2.then
  %"$bool.text6" = load i8*, i8** %bool.text5, align 8
  %call.dprintf7 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.6, i32 0, i32 0), i8* %"$bool.text6")
  %"$b8" = load %Vec, %Vec* %b, align 4
  %"call.Vec.+9" = call %Vec @"Vec.+"(%Vec* %a, %Vec %"$b8")
  store %Vec %"call.Vec.+9", %Vec* %tmp, align 4
  call void @Vec.show(%Vec* %tmp)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 015 — operator: assigning a void result

```ura
// error: Cannot assign void to Vec

struct Vec:
    x i32

    fn show():
        output(self.x, "\n")

fn main() i32:
    a Vec
    e Vec = a.show()
    return 0
```

---

## 016 — operator: a binary operator with mismatched scalar types

```ura
// error: Cannot use '+' with i32 and array

fn main() i32:
    n i32 = 1
    s char[] = "hi"
    output(n + s, "\n")
    return 0
```

---

## 017 — operator: operator = copies on declaration and on reassignment

```ura
// stdout: decl   5 99
// stdout: assign 5 99
// expect: 0

struct Vec:
    x i32
    tag i32

    operator =(other &Vec) void:
        self.x   = other.x
        self.tag = 99

fn main() i32:
    a Vec
    a.x = 5

    // declaration: copies through operator =, tag becomes 99
    b Vec = a
    output("decl   ", b.x, " ", b.tag, "\n")

    // reassignment: calls operator =, tag becomes 99
    c Vec
    c = a
    output("assign ", c.x, " ", c.tag, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Vec = type { i32, i32 }

@str = private unnamed_addr constant [14 x i8] c"decl   %d %d\0A\00", align 1
@str.1 = private unnamed_addr constant [14 x i8] c"assign %d %d\0A\00", align 1

define void @"Vec.="(%Vec* %0, %Vec* %1) {
entry:
  %self = alloca %Vec*, align 8
  %other = alloca %Vec*, align 8
  store %Vec* %0, %Vec** %self, align 8
  store %Vec* %1, %Vec** %other, align 8
  %"*self" = load %Vec*, %Vec** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec, %Vec* %"*self", i32 0, i32 0
  %"*other" = load %Vec*, %Vec** %other, align 8
  %"*other.x" = getelementptr inbounds %Vec, %Vec* %"*other", i32 0, i32 0
  %"$*other.x" = load i32, i32* %"*other.x", align 4
  store i32 %"$*other.x", i32* %"*self.x", align 4
  %"*self1" = load %Vec*, %Vec** %self, align 8
  %"*self.tag" = getelementptr inbounds %Vec, %Vec* %"*self1", i32 0, i32 1
  store i32 99, i32* %"*self.tag", align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Vec, align 8
  %b = alloca %Vec, align 8
  %copy = alloca %Vec, align 8
  %c = alloca %Vec, align 8
  %copy1 = alloca %Vec, align 8
  store %Vec zeroinitializer, %Vec* %a, align 4
  %a.x = getelementptr inbounds %Vec, %Vec* %a, i32 0, i32 0
  store i32 5, i32* %a.x, align 4
  store %Vec zeroinitializer, %Vec* %copy, align 4
  call void @"Vec.="(%Vec* %copy, %Vec* %a)
  %"$copy" = load %Vec, %Vec* %copy, align 4
  store %Vec %"$copy", %Vec* %b, align 4
  %b.x = getelementptr inbounds %Vec, %Vec* %b, i32 0, i32 0
  %b.tag = getelementptr inbounds %Vec, %Vec* %b, i32 0, i32 1
  %"$b.x" = load i32, i32* %b.x, align 4
  %"$b.tag" = load i32, i32* %b.tag, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str, i32 0, i32 0), i32 %"$b.x", i32 %"$b.tag")
  store %Vec zeroinitializer, %Vec* %copy1, align 4
  call void @"Vec.="(%Vec* %copy1, %Vec* %a)
  %"$copy2" = load %Vec, %Vec* %copy1, align 4
  store %Vec %"$copy2", %Vec* %c, align 4
  %c.x = getelementptr inbounds %Vec, %Vec* %c, i32 0, i32 0
  %c.tag = getelementptr inbounds %Vec, %Vec* %c, i32 0, i32 1
  %"$c.x" = load i32, i32* %c.x, align 4
  %"$c.tag" = load i32, i32* %c.tag, align 4
  %call.dprintf3 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.1, i32 0, i32 0), i32 %"$c.x", i32 %"$c.tag")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 018 — operator: compound assignment operators

```ura
// stdout: after += 14
// stdout: after -= 11
// expect: 0

struct Vec:
    x i32

    operator +=(other Vec) void:
        self.x = self.x + other.x

    operator -=(n i32) void:
        self.x = self.x - n

fn main() i32:
    a Vec
    a.x = 10
    b Vec
    b.x = 4

    a += b
    output("after += ", a.x, "\n")

    a -= 3
    output("after -= ", a.x, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Vec = type { i32 }

@str = private unnamed_addr constant [13 x i8] c"after += %d\0A\00", align 1
@str.1 = private unnamed_addr constant [13 x i8] c"after -= %d\0A\00", align 1

define void @"Vec.+="(%Vec* %0, %Vec %1) {
entry:
  %self = alloca %Vec*, align 8
  %other = alloca %Vec, align 8
  store %Vec* %0, %Vec** %self, align 8
  store %Vec %1, %Vec* %other, align 4
  %"*self" = load %Vec*, %Vec** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec, %Vec* %"*self", i32 0, i32 0
  %"*self1" = load %Vec*, %Vec** %self, align 8
  %"*self.x2" = getelementptr inbounds %Vec, %Vec* %"*self1", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x2", align 4
  %"$other.x" = load i32, i32* %other.x, align 4
  %ADD = add i32 %"$*self.x", %"$other.x"
  store i32 %ADD, i32* %"*self.x", align 4
  ret void
}

define void @"Vec.-="(%Vec* %0, i32 %1) {
entry:
  %self = alloca %Vec*, align 8
  %n = alloca i32, align 4
  store %Vec* %0, %Vec** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %Vec*, %Vec** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec, %Vec* %"*self", i32 0, i32 0
  %"*self1" = load %Vec*, %Vec** %self, align 8
  %"*self.x2" = getelementptr inbounds %Vec, %Vec* %"*self1", i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x2", align 4
  %"$n" = load i32, i32* %n, align 4
  %SUB = sub i32 %"$*self.x", %"$n"
  store i32 %SUB, i32* %"*self.x", align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Vec, align 8
  %b = alloca %Vec, align 8
  store %Vec zeroinitializer, %Vec* %a, align 4
  %a.x = getelementptr inbounds %Vec, %Vec* %a, i32 0, i32 0
  store i32 10, i32* %a.x, align 4
  store %Vec zeroinitializer, %Vec* %b, align 4
  %b.x = getelementptr inbounds %Vec, %Vec* %b, i32 0, i32 0
  store i32 4, i32* %b.x, align 4
  %"$b" = load %Vec, %Vec* %b, align 4
  call void @"Vec.+="(%Vec* %a, %Vec %"$b")
  %a.x1 = getelementptr inbounds %Vec, %Vec* %a, i32 0, i32 0
  %"$a.x" = load i32, i32* %a.x1, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str, i32 0, i32 0), i32 %"$a.x")
  call void @"Vec.-="(%Vec* %a, i32 3)
  %a.x2 = getelementptr inbounds %Vec, %Vec* %a, i32 0, i32 0
  %"$a.x3" = load i32, i32* %a.x2, align 4
  %call.dprintf4 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.1, i32 0, i32 0), i32 %"$a.x3")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 019 — operator: a compound operator with no overload

```ura
// error: Struct Vec has no 'operator *=' taking Vec; declare one inside the struct

struct Vec:
    x i32

fn main() i32:
    a Vec
    b Vec
    a *= b
    return 0
```

---

## 020 — operator: return unwinds every scope, and keeps the returned local

```ura
// stdout: drop 10
// stdout: drop 1
// stdout: early 7
// stdout: g.id 99
// stdout: drop 99
// expect: 0

struct Tag:
    id i32

    pub fn create(n i32) Tag:
        t Tag
        t.id = n
        return t

    operator =(other &Tag) void:
        self.id = other.id

    operator drop:
        output("drop ", self.id, "\n")

fn early() i32:
    a Tag = Tag::create(1)
    if 1 < 2:
        b Tag = Tag::create(10)
        return 7
    return 0

fn giveback() Tag:
    t Tag = Tag::create(99)
    return t

fn main() i32:
    r i32 = early()
    output("early ", r, "\n")
    g Tag = giveback()
    output("g.id ", g.id, "\n")
    return 0
```

---

## 021 — operator: every operator on one struct, and = from an i32 overload

```ura
// stdout: (11, 22) tag 0
// stdout: (6, 7) tag 0
// stdout: a == a  True
// stdout: a == b  False
// stdout: (11, 22) tag 0
// stdout: (1, 2) tag 99
// stdout: (1, 2) tag 99
// stdout: (20, 30) tag 0
// stdout: (17, 27) tag 0
// expect: 0

struct Vec:
    x i32
    y i32
    tag i32

    pub fn create(a i32, b i32) Vec:
        v Vec
        v.x = a
        v.y = b
        return v

    operator +(other Vec) Vec:
        return Vec::create(self.x + other.x, self.y + other.y)

    operator +(n i32) Vec:
        return Vec::create(self.x + n, self.y + n)

    operator ==(other Vec) bool:
        return self.x == other.x and self.y == other.y

    operator =(other &Vec) void:
        self.x   = other.x
        self.y   = other.y
        self.tag = 99

    operator +=(other Vec) void:
        self.x = self.x + other.x
        self.y = self.y + other.y

    operator -=(n i32) void:
        self.x = self.x - n
        self.y = self.y - n

    fn show():
        output("(", self.x, ", ", self.y, ") tag ", self.tag, "\n")

fn main() i32:
    a Vec = Vec::create(1, 2)
    b Vec = Vec::create(10, 20)

    c Vec = a + b
    c.show()

    // the i32 overload's result as a declaration initialiser
    d Vec = a + 5
    d.show()

    output("a == a  ", a == a, "\n")
    output("a == b  ", a == b, "\n")

    (a + b).show()

    // a declaration copies through operator =, tag becomes 99
    e Vec = a
    e.show()

    // reassignment does call operator =, so tag becomes 99
    f Vec
    f = a
    f.show()

    g Vec = Vec::create(10, 10)
    g += b
    g.show()
    g -= 3
    g.show()
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Vec = type { i32, i32, i32 }

@str = private unnamed_addr constant [17 x i8] c"(%d, %d) tag %d\0A\00", align 1
@str.2 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.4 = private unnamed_addr constant [12 x i8] c"a == a  %s\0A\00", align 1
@str.5 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.6 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.7 = private unnamed_addr constant [12 x i8] c"a == b  %s\0A\00", align 1

define %Vec @Vec.create(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %v = alloca %Vec, align 8
  store i32 %0, i32* %a, align 4
  store i32 %1, i32* %b, align 4
  store %Vec zeroinitializer, %Vec* %v, align 4
  %v.x = getelementptr inbounds %Vec, %Vec* %v, i32 0, i32 0
  %"$a" = load i32, i32* %a, align 4
  store i32 %"$a", i32* %v.x, align 4
  %v.y = getelementptr inbounds %Vec, %Vec* %v, i32 0, i32 1
  %"$b" = load i32, i32* %b, align 4
  store i32 %"$b", i32* %v.y, align 4
  %"$v" = load %Vec, %Vec* %v, align 4
  ret %Vec %"$v"
}

define %Vec @"Vec.+"(%Vec* %0, %Vec %1) {
entry:
  %self = alloca %Vec*, align 8
  %other = alloca %Vec, align 8
  store %Vec* %0, %Vec** %self, align 8
  store %Vec %1, %Vec* %other, align 4
  %"*self" = load %Vec*, %Vec** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec, %Vec* %"*self", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$other.x" = load i32, i32* %other.x, align 4
  %ADD = add i32 %"$*self.x", %"$other.x"
  %"*self1" = load %Vec*, %Vec** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec, %Vec* %"*self1", i32 0, i32 1
  %other.y = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 1
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  %"$other.y" = load i32, i32* %other.y, align 4
  %ADD2 = add i32 %"$*self.y", %"$other.y"
  %call.Vec.create = call %Vec @Vec.create(i32 %ADD, i32 %ADD2)
  ret %Vec %call.Vec.create
}

define %Vec @"Vec.+.1"(%Vec* %0, i32 %1) {
entry:
  %self = alloca %Vec*, align 8
  %n = alloca i32, align 4
  store %Vec* %0, %Vec** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %Vec*, %Vec** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec, %Vec* %"*self", i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$*self.x", %"$n"
  %"*self1" = load %Vec*, %Vec** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec, %Vec* %"*self1", i32 0, i32 1
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  %"$n2" = load i32, i32* %n, align 4
  %ADD3 = add i32 %"$*self.y", %"$n2"
  %call.Vec.create = call %Vec @Vec.create(i32 %ADD, i32 %ADD3)
  ret %Vec %call.Vec.create
}

define i1 @"Vec.=="(%Vec* %0, %Vec %1) {
entry:
  %self = alloca %Vec*, align 8
  %other = alloca %Vec, align 8
  store %Vec* %0, %Vec** %self, align 8
  store %Vec %1, %Vec* %other, align 4
  %"*self" = load %Vec*, %Vec** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec, %Vec* %"*self", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$other.x" = load i32, i32* %other.x, align 4
  %EQ = icmp eq i32 %"$*self.x", %"$other.x"
  %"*self1" = load %Vec*, %Vec** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec, %Vec* %"*self1", i32 0, i32 1
  %other.y = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 1
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  %"$other.y" = load i32, i32* %other.y, align 4
  %EQ2 = icmp eq i32 %"$*self.y", %"$other.y"
  %AND = and i1 %EQ, %EQ2
  ret i1 %AND
}

define void @"Vec.="(%Vec* %0, %Vec* %1) {
entry:
  %self = alloca %Vec*, align 8
  %other = alloca %Vec*, align 8
  store %Vec* %0, %Vec** %self, align 8
  store %Vec* %1, %Vec** %other, align 8
  %"*self" = load %Vec*, %Vec** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec, %Vec* %"*self", i32 0, i32 0
  %"*other" = load %Vec*, %Vec** %other, align 8
  %"*other.x" = getelementptr inbounds %Vec, %Vec* %"*other", i32 0, i32 0
  %"$*other.x" = load i32, i32* %"*other.x", align 4
  store i32 %"$*other.x", i32* %"*self.x", align 4
  %"*self1" = load %Vec*, %Vec** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec, %Vec* %"*self1", i32 0, i32 1
  %"*other2" = load %Vec*, %Vec** %other, align 8
  %"*other.y" = getelementptr inbounds %Vec, %Vec* %"*other2", i32 0, i32 1
  %"$*other.y" = load i32, i32* %"*other.y", align 4
  store i32 %"$*other.y", i32* %"*self.y", align 4
  %"*self3" = load %Vec*, %Vec** %self, align 8
  %"*self.tag" = getelementptr inbounds %Vec, %Vec* %"*self3", i32 0, i32 2
  store i32 99, i32* %"*self.tag", align 4
  ret void
}

define void @"Vec.+="(%Vec* %0, %Vec %1) {
entry:
  %self = alloca %Vec*, align 8
  %other = alloca %Vec, align 8
  store %Vec* %0, %Vec** %self, align 8
  store %Vec %1, %Vec* %other, align 4
  %"*self" = load %Vec*, %Vec** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec, %Vec* %"*self", i32 0, i32 0
  %"*self1" = load %Vec*, %Vec** %self, align 8
  %"*self.x2" = getelementptr inbounds %Vec, %Vec* %"*self1", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x2", align 4
  %"$other.x" = load i32, i32* %other.x, align 4
  %ADD = add i32 %"$*self.x", %"$other.x"
  store i32 %ADD, i32* %"*self.x", align 4
  %"*self3" = load %Vec*, %Vec** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec, %Vec* %"*self3", i32 0, i32 1
  %"*self4" = load %Vec*, %Vec** %self, align 8
  %"*self.y5" = getelementptr inbounds %Vec, %Vec* %"*self4", i32 0, i32 1
  %other.y = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 1
  %"$*self.y" = load i32, i32* %"*self.y5", align 4
  %"$other.y" = load i32, i32* %other.y, align 4
  %ADD6 = add i32 %"$*self.y", %"$other.y"
  store i32 %ADD6, i32* %"*self.y", align 4
  ret void
}

define void @"Vec.-="(%Vec* %0, i32 %1) {
entry:
  %self = alloca %Vec*, align 8
  %n = alloca i32, align 4
  store %Vec* %0, %Vec** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %Vec*, %Vec** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec, %Vec* %"*self", i32 0, i32 0
  %"*self1" = load %Vec*, %Vec** %self, align 8
  %"*self.x2" = getelementptr inbounds %Vec, %Vec* %"*self1", i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x2", align 4
  %"$n" = load i32, i32* %n, align 4
  %SUB = sub i32 %"$*self.x", %"$n"
  store i32 %SUB, i32* %"*self.x", align 4
  %"*self3" = load %Vec*, %Vec** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec, %Vec* %"*self3", i32 0, i32 1
  %"*self4" = load %Vec*, %Vec** %self, align 8
  %"*self.y5" = getelementptr inbounds %Vec, %Vec* %"*self4", i32 0, i32 1
  %"$*self.y" = load i32, i32* %"*self.y5", align 4
  %"$n6" = load i32, i32* %n, align 4
  %SUB7 = sub i32 %"$*self.y", %"$n6"
  store i32 %SUB7, i32* %"*self.y", align 4
  ret void
}

define void @Vec.show(%Vec* %0) {
entry:
  %self = alloca %Vec*, align 8
  store %Vec* %0, %Vec** %self, align 8
  %"*self" = load %Vec*, %Vec** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec, %Vec* %"*self", i32 0, i32 0
  %"*self1" = load %Vec*, %Vec** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec, %Vec* %"*self1", i32 0, i32 1
  %"*self2" = load %Vec*, %Vec** %self, align 8
  %"*self.tag" = getelementptr inbounds %Vec, %Vec* %"*self2", i32 0, i32 2
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  %"$*self.tag" = load i32, i32* %"*self.tag", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str, i32 0, i32 0), i32 %"$*self.x", i32 %"$*self.y", i32 %"$*self.tag")
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Vec, align 8
  %b = alloca %Vec, align 8
  %c = alloca %Vec, align 8
  %copy = alloca %Vec, align 8
  %d = alloca %Vec, align 8
  %copy4 = alloca %Vec, align 8
  %bool.text = alloca i8*, align 8
  %copy6 = alloca %Vec, align 8
  %bool.text9 = alloca i8*, align 8
  %copy12 = alloca %Vec, align 8
  %tmp = alloca %Vec, align 8
  %e = alloca %Vec, align 8
  %copy15 = alloca %Vec, align 8
  %f = alloca %Vec, align 8
  %copy17 = alloca %Vec, align 8
  %g = alloca %Vec, align 8
  %copy20 = alloca %Vec, align 8
  %call.Vec.create = call %Vec @Vec.create(i32 1, i32 2)
  store %Vec %call.Vec.create, %Vec* %a, align 4
  %call.Vec.create2 = call %Vec @Vec.create(i32 10, i32 20)
  store %Vec %call.Vec.create2, %Vec* %b, align 4
  store %Vec zeroinitializer, %Vec* %copy, align 4
  call void @"Vec.="(%Vec* %copy, %Vec* %b)
  %"$copy" = load %Vec, %Vec* %copy, align 4
  %"call.Vec.+" = call %Vec @"Vec.+"(%Vec* %a, %Vec %"$copy")
  store %Vec %"call.Vec.+", %Vec* %c, align 4
  call void @Vec.show(%Vec* %c)
  %"call.Vec.+3" = call %Vec @"Vec.+.1"(%Vec* %a, i32 5)
  store %Vec %"call.Vec.+3", %Vec* %d, align 4
  call void @Vec.show(%Vec* %d)
  store %Vec zeroinitializer, %Vec* %copy4, align 4
  call void @"Vec.="(%Vec* %copy4, %Vec* %a)
  %"$copy5" = load %Vec, %Vec* %copy4, align 4
  %"call.Vec.==" = call i1 @"Vec.=="(%Vec* %a, %Vec %"$copy5")
  store i8* null, i8** %bool.text, align 8
  br label %if0

if0:                                              ; preds = %entry
  br i1 %"call.Vec.==", label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else1:                                            ; preds = %if0
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.4, i32 0, i32 0), i8* %"$bool.text")
  store %Vec zeroinitializer, %Vec* %copy6, align 4
  call void @"Vec.="(%Vec* %copy6, %Vec* %b)
  %"$copy7" = load %Vec, %Vec* %copy6, align 4
  %"call.Vec.==8" = call i1 @"Vec.=="(%Vec* %a, %Vec %"$copy7")
  store i8* null, i8** %bool.text9, align 8
  br label %if2

if2:                                              ; preds = %endif
  br i1 %"call.Vec.==8", label %if2.then, label %else3

if2.then:                                         ; preds = %if2
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.5, i32 0, i32 0), i8** %bool.text9, align 8
  br label %endif1

else3:                                            ; preds = %if2
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.6, i32 0, i32 0), i8** %bool.text9, align 8
  br label %endif1

endif1:                                           ; preds = %else3, %if2.then
  %"$bool.text10" = load i8*, i8** %bool.text9, align 8
  %call.dprintf11 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.7, i32 0, i32 0), i8* %"$bool.text10")
  store %Vec zeroinitializer, %Vec* %copy12, align 4
  call void @"Vec.="(%Vec* %copy12, %Vec* %b)
  %"$copy13" = load %Vec, %Vec* %copy12, align 4
  %"call.Vec.+14" = call %Vec @"Vec.+"(%Vec* %a, %Vec %"$copy13")
  store %Vec %"call.Vec.+14", %Vec* %tmp, align 4
  call void @Vec.show(%Vec* %tmp)
  store %Vec zeroinitializer, %Vec* %copy15, align 4
  call void @"Vec.="(%Vec* %copy15, %Vec* %a)
  %"$copy16" = load %Vec, %Vec* %copy15, align 4
  store %Vec %"$copy16", %Vec* %e, align 4
  call void @Vec.show(%Vec* %e)
  store %Vec zeroinitializer, %Vec* %copy17, align 4
  call void @"Vec.="(%Vec* %copy17, %Vec* %a)
  %"$copy18" = load %Vec, %Vec* %copy17, align 4
  store %Vec %"$copy18", %Vec* %f, align 4
  call void @Vec.show(%Vec* %f)
  %call.Vec.create19 = call %Vec @Vec.create(i32 10, i32 10)
  store %Vec %call.Vec.create19, %Vec* %g, align 4
  store %Vec zeroinitializer, %Vec* %copy20, align 4
  call void @"Vec.="(%Vec* %copy20, %Vec* %b)
  %"$copy21" = load %Vec, %Vec* %copy20, align 4
  call void @"Vec.+="(%Vec* %g, %Vec %"$copy21")
  call void @Vec.show(%Vec* %g)
  call void @"Vec.-="(%Vec* %g, i32 3)
  call void @Vec.show(%Vec* %g)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 022 — operator: a temporary is destroyed at the end of its statement

```ura
// stdout: show 3
// stdout: DROP 3
// stdout: --
// stdout: 3
// stdout: DROP 3
// stdout: --
// stdout: show 4
// stdout: DROP 4
// stdout: DROP 3
// stdout: --
// stdout: show 3
// stdout: DROP 3
// stdout: show 3
// stdout: DROP 3
// stdout: --
// stdout: DROP 3
// stdout: 3
// stdout: --
// stdout: 9
// stdout: DROP 9
// stdout: --
// stdout: 7
// stdout: end
// stdout: DROP 2
// stdout: DROP 1
// expect: 0

struct Plain:
    x i32

    pub fn create(n i32) Plain:
        p Plain
        p.x = n
        return p

struct R:
    id i32

    pub fn create(n i32) R:
        r R
        r.id = n
        return r

    operator =(other &R) void:
        self.id = other.id

    operator +(other R) R:
        return R::create(self.id + other.id)

    fn show():
        output("show ", self.id, "\n")

    operator drop:
        output("DROP ", self.id, "\n")

fn via_return(a R, b R) i32:
    return (a + b).id

fn main() i32:
    a R = R::create(1)
    b R = R::create(2)
    (a + b).show()
    output("--\n")
    output((a + b).id, "\n")
    output("--\n")
    // a chain makes two temporaries; both die, in reverse
    (a + b + a).show()
    output("--\n")
    i i32 = 0
    while i < 2:
        (a + b).show()
        i += 1
    output("--\n")
    // dropped before the ret, not after it
    output(via_return(a, b), "\n")
    output("--\n")
    output(R::create(9).id, "\n")
    output("--\n")
    // a struct with no destructor is never recorded as a temporary
    output(Plain::create(7).x, "\n")
    output("end\n")
    return 0
```

---

## 023 — operator: every overloadable operator on one struct

```ura
// stdout: a + b   (11, 15)
// stdout: a - b   (7, 9)
// stdout: a * 3   (27, 36)
// stdout: a / 2   (4, 6)
// stdout: a mod 4 (1, 0)
// stdout: == False  != True
// stdout: <  False  >  True
// stdout: <= False  >= True
// stdout: c = a    (9, 12)
// stdout: c += b   (11, 15)
// stdout: c -= b   (9, 12)
// stdout: c *= 2   (18, 24)
// stdout: c /= 3   (6, 8)
// stdout: c %= 5   (1, 3)
// stdout: a is longer
// stdout: steps to pass a: 4
// expect: 0

struct Vec2:
    x i32
    y i32

    pub fn create(a i32, b i32) Vec2:
        v Vec2
        v.x = a
        v.y = b
        return v

    operator +(other Vec2) Vec2:
        return Vec2::create(self.x + other.x, self.y + other.y)

    operator -(other Vec2) Vec2:
        return Vec2::create(self.x - other.x, self.y - other.y)

    operator *(n i32) Vec2:
        return Vec2::create(self.x * n, self.y * n)

    operator /(n i32) Vec2:
        return Vec2::create(self.x / n, self.y / n)

    operator %(n i32) Vec2:
        return Vec2::create(self.x % n, self.y % n)

    operator ==(other Vec2) bool:
        return self.x == other.x and self.y == other.y

    operator !=(other Vec2) bool:
        return self.x != other.x or self.y != other.y

    operator <(other Vec2) bool:
        return self.taxicab() < other.taxicab()

    operator >(other Vec2) bool:
        return self.taxicab() > other.taxicab()

    operator <=(other Vec2) bool:
        return self.taxicab() <= other.taxicab()

    operator >=(other Vec2) bool:
        return self.taxicab() >= other.taxicab()

    operator =(other &Vec2) void:
        self.x = other.x
        self.y = other.y

    operator +=(other Vec2) void:
        self.x = self.x + other.x
        self.y = self.y + other.y

    operator -=(other Vec2) void:
        self.x = self.x - other.x
        self.y = self.y - other.y

    operator *=(n i32) void:
        self.x = self.x * n
        self.y = self.y * n

    operator /=(n i32) void:
        self.x = self.x / n
        self.y = self.y / n

    operator %=(n i32) void:
        self.x = self.x % n
        self.y = self.y % n

    fn taxicab() i32:
        return self.abs(self.x) + self.abs(self.y)

    fn abs(n i32) i32:
        if n < 0:
            return 0 - n
        return n

    fn show(label char[]):
        output(label, "(", self.x, ", ", self.y, ")\n")

fn main() i32:
    a Vec2 = Vec2::create(9, 12)
    b Vec2 = Vec2::create(2, 3)

    (a + b).show("a + b   ")
    (a - b).show("a - b   ")
    (a * 3).show("a * 3   ")
    (a / 2).show("a / 2   ")
    (a % 4).show("a mod 4 ")

    output("== ", a == b, "  != ", a != b, "\n")
    output("<  ", a < b,  "  >  ", a > b, "\n")
    output("<= ", a <= b, "  >= ", a >= b, "\n")

    c Vec2 = Vec2::create(0, 0)
    c = a
    c.show("c = a    ")
    c += b
    c.show("c += b   ")
    c -= b
    c.show("c -= b   ")
    c *= 2
    c.show("c *= 2   ")
    c /= 3
    c.show("c /= 3   ")
    c %= 5
    c.show("c %= 5   ")

    // an overloaded comparison driving a branch and a loop
    if a > b:
        output("a is longer\n")
    n i32 = 0
    d Vec2 = Vec2::create(1, 1)
    while d < a:
        d += b
        n += 1
    output("steps to pass a: ", n, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Vec2 = type { i32, i32 }

@str = private unnamed_addr constant [12 x i8] c"%s(%d, %d)\0A\00", align 1
@str.1 = private unnamed_addr constant [9 x i8] c"a + b   \00", align 1
@str.2 = private unnamed_addr constant [9 x i8] c"a - b   \00", align 1
@str.3 = private unnamed_addr constant [9 x i8] c"a * 3   \00", align 1
@str.4 = private unnamed_addr constant [9 x i8] c"a / 2   \00", align 1
@str.5 = private unnamed_addr constant [9 x i8] c"a mod 4 \00", align 1
@str.6 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.7 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.8 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.9 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.10 = private unnamed_addr constant [14 x i8] c"== %s  != %s\0A\00", align 1
@str.11 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.12 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.13 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.14 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.15 = private unnamed_addr constant [14 x i8] c"<  %s  >  %s\0A\00", align 1
@str.16 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.17 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.18 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.19 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.20 = private unnamed_addr constant [14 x i8] c"<= %s  >= %s\0A\00", align 1
@str.21 = private unnamed_addr constant [10 x i8] c"c = a    \00", align 1
@str.22 = private unnamed_addr constant [10 x i8] c"c += b   \00", align 1
@str.23 = private unnamed_addr constant [10 x i8] c"c -= b   \00", align 1
@str.24 = private unnamed_addr constant [10 x i8] c"c *= 2   \00", align 1
@str.25 = private unnamed_addr constant [10 x i8] c"c /= 3   \00", align 1
@str.26 = private unnamed_addr constant [10 x i8] c"c %= 5   \00", align 1
@str.27 = private unnamed_addr constant [13 x i8] c"a is longer\0A\00", align 1
@str.28 = private unnamed_addr constant [21 x i8] c"steps to pass a: %d\0A\00", align 1

define %Vec2 @Vec2.create(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %v = alloca %Vec2, align 8
  store i32 %0, i32* %a, align 4
  store i32 %1, i32* %b, align 4
  store %Vec2 zeroinitializer, %Vec2* %v, align 4
  %v.x = getelementptr inbounds %Vec2, %Vec2* %v, i32 0, i32 0
  %"$a" = load i32, i32* %a, align 4
  store i32 %"$a", i32* %v.x, align 4
  %v.y = getelementptr inbounds %Vec2, %Vec2* %v, i32 0, i32 1
  %"$b" = load i32, i32* %b, align 4
  store i32 %"$b", i32* %v.y, align 4
  %"$v" = load %Vec2, %Vec2* %v, align 4
  ret %Vec2 %"$v"
}

define %Vec2 @"Vec2.+"(%Vec2* %0, %Vec2 %1) {
entry:
  %self = alloca %Vec2*, align 8
  %other = alloca %Vec2, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %"*self" = load %Vec2*, %Vec2** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec2, %Vec2* %"*self", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec2, %Vec2* %other, i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$other.x" = load i32, i32* %other.x, align 4
  %ADD = add i32 %"$*self.x", %"$other.x"
  %"*self1" = load %Vec2*, %Vec2** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec2, %Vec2* %"*self1", i32 0, i32 1
  %other.y = getelementptr inbounds %Vec2, %Vec2* %other, i32 0, i32 1
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  %"$other.y" = load i32, i32* %other.y, align 4
  %ADD2 = add i32 %"$*self.y", %"$other.y"
  %call.Vec2.create = call %Vec2 @Vec2.create(i32 %ADD, i32 %ADD2)
  ret %Vec2 %call.Vec2.create
}

define %Vec2 @Vec2.-(%Vec2* %0, %Vec2 %1) {
entry:
  %self = alloca %Vec2*, align 8
  %other = alloca %Vec2, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %"*self" = load %Vec2*, %Vec2** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec2, %Vec2* %"*self", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec2, %Vec2* %other, i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$other.x" = load i32, i32* %other.x, align 4
  %SUB = sub i32 %"$*self.x", %"$other.x"
  %"*self1" = load %Vec2*, %Vec2** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec2, %Vec2* %"*self1", i32 0, i32 1
  %other.y = getelementptr inbounds %Vec2, %Vec2* %other, i32 0, i32 1
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  %"$other.y" = load i32, i32* %other.y, align 4
  %SUB2 = sub i32 %"$*self.y", %"$other.y"
  %call.Vec2.create = call %Vec2 @Vec2.create(i32 %SUB, i32 %SUB2)
  ret %Vec2 %call.Vec2.create
}

define %Vec2 @"Vec2.*"(%Vec2* %0, i32 %1) {
entry:
  %self = alloca %Vec2*, align 8
  %n = alloca i32, align 4
  store %Vec2* %0, %Vec2** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %Vec2*, %Vec2** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec2, %Vec2* %"*self", i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$n" = load i32, i32* %n, align 4
  %MUL = mul i32 %"$*self.x", %"$n"
  %"*self1" = load %Vec2*, %Vec2** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec2, %Vec2* %"*self1", i32 0, i32 1
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  %"$n2" = load i32, i32* %n, align 4
  %MUL3 = mul i32 %"$*self.y", %"$n2"
  %call.Vec2.create = call %Vec2 @Vec2.create(i32 %MUL, i32 %MUL3)
  ret %Vec2 %call.Vec2.create
}

define %Vec2 @"Vec2./"(%Vec2* %0, i32 %1) {
entry:
  %self = alloca %Vec2*, align 8
  %n = alloca i32, align 4
  store %Vec2* %0, %Vec2** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %Vec2*, %Vec2** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec2, %Vec2* %"*self", i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$n" = load i32, i32* %n, align 4
  %DIV = sdiv i32 %"$*self.x", %"$n"
  %"*self1" = load %Vec2*, %Vec2** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec2, %Vec2* %"*self1", i32 0, i32 1
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  %"$n2" = load i32, i32* %n, align 4
  %DIV3 = sdiv i32 %"$*self.y", %"$n2"
  %call.Vec2.create = call %Vec2 @Vec2.create(i32 %DIV, i32 %DIV3)
  ret %Vec2 %call.Vec2.create
}

define %Vec2 @"Vec2.%"(%Vec2* %0, i32 %1) {
entry:
  %self = alloca %Vec2*, align 8
  %n = alloca i32, align 4
  store %Vec2* %0, %Vec2** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %Vec2*, %Vec2** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec2, %Vec2* %"*self", i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$n" = load i32, i32* %n, align 4
  %MOD = srem i32 %"$*self.x", %"$n"
  %"*self1" = load %Vec2*, %Vec2** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec2, %Vec2* %"*self1", i32 0, i32 1
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  %"$n2" = load i32, i32* %n, align 4
  %MOD3 = srem i32 %"$*self.y", %"$n2"
  %call.Vec2.create = call %Vec2 @Vec2.create(i32 %MOD, i32 %MOD3)
  ret %Vec2 %call.Vec2.create
}

define i1 @"Vec2.=="(%Vec2* %0, %Vec2 %1) {
entry:
  %self = alloca %Vec2*, align 8
  %other = alloca %Vec2, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %"*self" = load %Vec2*, %Vec2** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec2, %Vec2* %"*self", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec2, %Vec2* %other, i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$other.x" = load i32, i32* %other.x, align 4
  %EQ = icmp eq i32 %"$*self.x", %"$other.x"
  %"*self1" = load %Vec2*, %Vec2** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec2, %Vec2* %"*self1", i32 0, i32 1
  %other.y = getelementptr inbounds %Vec2, %Vec2* %other, i32 0, i32 1
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  %"$other.y" = load i32, i32* %other.y, align 4
  %EQ2 = icmp eq i32 %"$*self.y", %"$other.y"
  %AND = and i1 %EQ, %EQ2
  ret i1 %AND
}

define i1 @"Vec2.!="(%Vec2* %0, %Vec2 %1) {
entry:
  %self = alloca %Vec2*, align 8
  %other = alloca %Vec2, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %"*self" = load %Vec2*, %Vec2** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec2, %Vec2* %"*self", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec2, %Vec2* %other, i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$other.x" = load i32, i32* %other.x, align 4
  %NQ = icmp ne i32 %"$*self.x", %"$other.x"
  %"*self1" = load %Vec2*, %Vec2** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec2, %Vec2* %"*self1", i32 0, i32 1
  %other.y = getelementptr inbounds %Vec2, %Vec2* %other, i32 0, i32 1
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  %"$other.y" = load i32, i32* %other.y, align 4
  %NQ2 = icmp ne i32 %"$*self.y", %"$other.y"
  %OR = or i1 %NQ, %NQ2
  ret i1 %OR
}

define i1 @"Vec2.<"(%Vec2* %0, %Vec2 %1) {
entry:
  %self = alloca %Vec2*, align 8
  %other = alloca %Vec2, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %"*self" = load %Vec2*, %Vec2** %self, align 8
  %call.Vec2.taxicab = call i32 @Vec2.taxicab(%Vec2* %"*self")
  %call.Vec2.taxicab1 = call i32 @Vec2.taxicab(%Vec2* %other)
  %LT = icmp slt i32 %call.Vec2.taxicab, %call.Vec2.taxicab1
  ret i1 %LT
}

define i1 @"Vec2.>"(%Vec2* %0, %Vec2 %1) {
entry:
  %self = alloca %Vec2*, align 8
  %other = alloca %Vec2, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %"*self" = load %Vec2*, %Vec2** %self, align 8
  %call.Vec2.taxicab = call i32 @Vec2.taxicab(%Vec2* %"*self")
  %call.Vec2.taxicab1 = call i32 @Vec2.taxicab(%Vec2* %other)
  %GT = icmp sgt i32 %call.Vec2.taxicab, %call.Vec2.taxicab1
  ret i1 %GT
}

define i1 @"Vec2.<="(%Vec2* %0, %Vec2 %1) {
entry:
  %self = alloca %Vec2*, align 8
  %other = alloca %Vec2, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %"*self" = load %Vec2*, %Vec2** %self, align 8
  %call.Vec2.taxicab = call i32 @Vec2.taxicab(%Vec2* %"*self")
  %call.Vec2.taxicab1 = call i32 @Vec2.taxicab(%Vec2* %other)
  %LE = icmp sle i32 %call.Vec2.taxicab, %call.Vec2.taxicab1
  ret i1 %LE
}

define i1 @"Vec2.>="(%Vec2* %0, %Vec2 %1) {
entry:
  %self = alloca %Vec2*, align 8
  %other = alloca %Vec2, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %"*self" = load %Vec2*, %Vec2** %self, align 8
  %call.Vec2.taxicab = call i32 @Vec2.taxicab(%Vec2* %"*self")
  %call.Vec2.taxicab1 = call i32 @Vec2.taxicab(%Vec2* %other)
  %GE = icmp sge i32 %call.Vec2.taxicab, %call.Vec2.taxicab1
  ret i1 %GE
}

define void @"Vec2.="(%Vec2* %0, %Vec2* %1) {
entry:
  %self = alloca %Vec2*, align 8
  %other = alloca %Vec2*, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  store %Vec2* %1, %Vec2** %other, align 8
  %"*self" = load %Vec2*, %Vec2** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec2, %Vec2* %"*self", i32 0, i32 0
  %"*other" = load %Vec2*, %Vec2** %other, align 8
  %"*other.x" = getelementptr inbounds %Vec2, %Vec2* %"*other", i32 0, i32 0
  %"$*other.x" = load i32, i32* %"*other.x", align 4
  store i32 %"$*other.x", i32* %"*self.x", align 4
  %"*self1" = load %Vec2*, %Vec2** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec2, %Vec2* %"*self1", i32 0, i32 1
  %"*other2" = load %Vec2*, %Vec2** %other, align 8
  %"*other.y" = getelementptr inbounds %Vec2, %Vec2* %"*other2", i32 0, i32 1
  %"$*other.y" = load i32, i32* %"*other.y", align 4
  store i32 %"$*other.y", i32* %"*self.y", align 4
  ret void
}

define void @"Vec2.+="(%Vec2* %0, %Vec2 %1) {
entry:
  %self = alloca %Vec2*, align 8
  %other = alloca %Vec2, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %"*self" = load %Vec2*, %Vec2** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec2, %Vec2* %"*self", i32 0, i32 0
  %"*self1" = load %Vec2*, %Vec2** %self, align 8
  %"*self.x2" = getelementptr inbounds %Vec2, %Vec2* %"*self1", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec2, %Vec2* %other, i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x2", align 4
  %"$other.x" = load i32, i32* %other.x, align 4
  %ADD = add i32 %"$*self.x", %"$other.x"
  store i32 %ADD, i32* %"*self.x", align 4
  %"*self3" = load %Vec2*, %Vec2** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec2, %Vec2* %"*self3", i32 0, i32 1
  %"*self4" = load %Vec2*, %Vec2** %self, align 8
  %"*self.y5" = getelementptr inbounds %Vec2, %Vec2* %"*self4", i32 0, i32 1
  %other.y = getelementptr inbounds %Vec2, %Vec2* %other, i32 0, i32 1
  %"$*self.y" = load i32, i32* %"*self.y5", align 4
  %"$other.y" = load i32, i32* %other.y, align 4
  %ADD6 = add i32 %"$*self.y", %"$other.y"
  store i32 %ADD6, i32* %"*self.y", align 4
  ret void
}

define void @"Vec2.-="(%Vec2* %0, %Vec2 %1) {
entry:
  %self = alloca %Vec2*, align 8
  %other = alloca %Vec2, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %"*self" = load %Vec2*, %Vec2** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec2, %Vec2* %"*self", i32 0, i32 0
  %"*self1" = load %Vec2*, %Vec2** %self, align 8
  %"*self.x2" = getelementptr inbounds %Vec2, %Vec2* %"*self1", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec2, %Vec2* %other, i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x2", align 4
  %"$other.x" = load i32, i32* %other.x, align 4
  %SUB = sub i32 %"$*self.x", %"$other.x"
  store i32 %SUB, i32* %"*self.x", align 4
  %"*self3" = load %Vec2*, %Vec2** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec2, %Vec2* %"*self3", i32 0, i32 1
  %"*self4" = load %Vec2*, %Vec2** %self, align 8
  %"*self.y5" = getelementptr inbounds %Vec2, %Vec2* %"*self4", i32 0, i32 1
  %other.y = getelementptr inbounds %Vec2, %Vec2* %other, i32 0, i32 1
  %"$*self.y" = load i32, i32* %"*self.y5", align 4
  %"$other.y" = load i32, i32* %other.y, align 4
  %SUB6 = sub i32 %"$*self.y", %"$other.y"
  store i32 %SUB6, i32* %"*self.y", align 4
  ret void
}

define void @"Vec2.*="(%Vec2* %0, i32 %1) {
entry:
  %self = alloca %Vec2*, align 8
  %n = alloca i32, align 4
  store %Vec2* %0, %Vec2** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %Vec2*, %Vec2** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec2, %Vec2* %"*self", i32 0, i32 0
  %"*self1" = load %Vec2*, %Vec2** %self, align 8
  %"*self.x2" = getelementptr inbounds %Vec2, %Vec2* %"*self1", i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x2", align 4
  %"$n" = load i32, i32* %n, align 4
  %MUL = mul i32 %"$*self.x", %"$n"
  store i32 %MUL, i32* %"*self.x", align 4
  %"*self3" = load %Vec2*, %Vec2** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec2, %Vec2* %"*self3", i32 0, i32 1
  %"*self4" = load %Vec2*, %Vec2** %self, align 8
  %"*self.y5" = getelementptr inbounds %Vec2, %Vec2* %"*self4", i32 0, i32 1
  %"$*self.y" = load i32, i32* %"*self.y5", align 4
  %"$n6" = load i32, i32* %n, align 4
  %MUL7 = mul i32 %"$*self.y", %"$n6"
  store i32 %MUL7, i32* %"*self.y", align 4
  ret void
}

define void @"Vec2./="(%Vec2* %0, i32 %1) {
entry:
  %self = alloca %Vec2*, align 8
  %n = alloca i32, align 4
  store %Vec2* %0, %Vec2** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %Vec2*, %Vec2** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec2, %Vec2* %"*self", i32 0, i32 0
  %"*self1" = load %Vec2*, %Vec2** %self, align 8
  %"*self.x2" = getelementptr inbounds %Vec2, %Vec2* %"*self1", i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x2", align 4
  %"$n" = load i32, i32* %n, align 4
  %DIV = sdiv i32 %"$*self.x", %"$n"
  store i32 %DIV, i32* %"*self.x", align 4
  %"*self3" = load %Vec2*, %Vec2** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec2, %Vec2* %"*self3", i32 0, i32 1
  %"*self4" = load %Vec2*, %Vec2** %self, align 8
  %"*self.y5" = getelementptr inbounds %Vec2, %Vec2* %"*self4", i32 0, i32 1
  %"$*self.y" = load i32, i32* %"*self.y5", align 4
  %"$n6" = load i32, i32* %n, align 4
  %DIV7 = sdiv i32 %"$*self.y", %"$n6"
  store i32 %DIV7, i32* %"*self.y", align 4
  ret void
}

define void @"Vec2.%="(%Vec2* %0, i32 %1) {
entry:
  %self = alloca %Vec2*, align 8
  %n = alloca i32, align 4
  store %Vec2* %0, %Vec2** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %Vec2*, %Vec2** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec2, %Vec2* %"*self", i32 0, i32 0
  %"*self1" = load %Vec2*, %Vec2** %self, align 8
  %"*self.x2" = getelementptr inbounds %Vec2, %Vec2* %"*self1", i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x2", align 4
  %"$n" = load i32, i32* %n, align 4
  %MOD = srem i32 %"$*self.x", %"$n"
  store i32 %MOD, i32* %"*self.x", align 4
  %"*self3" = load %Vec2*, %Vec2** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec2, %Vec2* %"*self3", i32 0, i32 1
  %"*self4" = load %Vec2*, %Vec2** %self, align 8
  %"*self.y5" = getelementptr inbounds %Vec2, %Vec2* %"*self4", i32 0, i32 1
  %"$*self.y" = load i32, i32* %"*self.y5", align 4
  %"$n6" = load i32, i32* %n, align 4
  %MOD7 = srem i32 %"$*self.y", %"$n6"
  store i32 %MOD7, i32* %"*self.y", align 4
  ret void
}

define i32 @Vec2.taxicab(%Vec2* %0) {
entry:
  %self = alloca %Vec2*, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  %"*self" = load %Vec2*, %Vec2** %self, align 8
  %"*self1" = load %Vec2*, %Vec2** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec2, %Vec2* %"*self1", i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %call.Vec2.abs = call i32 @Vec2.abs(%Vec2* %"*self", i32 %"$*self.x")
  %"*self2" = load %Vec2*, %Vec2** %self, align 8
  %"*self3" = load %Vec2*, %Vec2** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec2, %Vec2* %"*self3", i32 0, i32 1
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  %call.Vec2.abs4 = call i32 @Vec2.abs(%Vec2* %"*self2", i32 %"$*self.y")
  %ADD = add i32 %call.Vec2.abs, %call.Vec2.abs4
  ret i32 %ADD
}

define i32 @Vec2.abs(%Vec2* %0, i32 %1) {
entry:
  %self = alloca %Vec2*, align 8
  %n = alloca i32, align 4
  store %Vec2* %0, %Vec2** %self, align 8
  store i32 %1, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %LT = icmp slt i32 %"$n", 0
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$n1" = load i32, i32* %n, align 4
  %SUB = sub i32 0, %"$n1"
  ret i32 %SUB

endif:                                            ; preds = %if0
  %"$n2" = load i32, i32* %n, align 4
  ret i32 %"$n2"
}

define void @Vec2.show(%Vec2* %0, i8* %1) {
entry:
  %self = alloca %Vec2*, align 8
  %label = alloca i8*, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  store i8* %1, i8** %label, align 8
  %"*self" = load %Vec2*, %Vec2** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec2, %Vec2* %"*self", i32 0, i32 0
  %"*self1" = load %Vec2*, %Vec2** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec2, %Vec2* %"*self1", i32 0, i32 1
  %"$label" = load i8*, i8** %label, align 8
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i8* %"$label", i32 %"$*self.x", i32 %"$*self.y")
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Vec2, align 8
  %b = alloca %Vec2, align 8
  %copy = alloca %Vec2, align 8
  %tmp = alloca %Vec2, align 8
  %copy8 = alloca %Vec2, align 8
  %tmp10 = alloca %Vec2, align 8
  %tmp11 = alloca %Vec2, align 8
  %tmp12 = alloca %Vec2, align 8
  %tmp13 = alloca %Vec2, align 8
  %copy14 = alloca %Vec2, align 8
  %copy16 = alloca %Vec2, align 8
  %bool.text = alloca i8*, align 8
  %bool.text18 = alloca i8*, align 8
  %copy20 = alloca %Vec2, align 8
  %copy22 = alloca %Vec2, align 8
  %bool.text24 = alloca i8*, align 8
  %bool.text25 = alloca i8*, align 8
  %copy29 = alloca %Vec2, align 8
  %copy31 = alloca %Vec2, align 8
  %bool.text33 = alloca i8*, align 8
  %bool.text34 = alloca i8*, align 8
  %c = alloca %Vec2, align 8
  %copy39 = alloca %Vec2, align 8
  %copy41 = alloca %Vec2, align 8
  %copy43 = alloca %Vec2, align 8
  %copy45 = alloca %Vec2, align 8
  %n = alloca i32, align 4
  %d = alloca %Vec2, align 8
  %copy50 = alloca %Vec2, align 8
  %copy53 = alloca %Vec2, align 8
  %call.Vec2.create = call %Vec2 @Vec2.create(i32 9, i32 12)
  store %Vec2 %call.Vec2.create, %Vec2* %a, align 4
  %call.Vec2.create7 = call %Vec2 @Vec2.create(i32 2, i32 3)
  store %Vec2 %call.Vec2.create7, %Vec2* %b, align 4
  store %Vec2 zeroinitializer, %Vec2* %copy, align 4
  call void @"Vec2.="(%Vec2* %copy, %Vec2* %b)
  %"$copy" = load %Vec2, %Vec2* %copy, align 4
  %"call.Vec2.+" = call %Vec2 @"Vec2.+"(%Vec2* %a, %Vec2 %"$copy")
  store %Vec2 %"call.Vec2.+", %Vec2* %tmp, align 4
  call void @Vec2.show(%Vec2* %tmp, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0))
  store %Vec2 zeroinitializer, %Vec2* %copy8, align 4
  call void @"Vec2.="(%Vec2* %copy8, %Vec2* %b)
  %"$copy9" = load %Vec2, %Vec2* %copy8, align 4
  %call.Vec2.- = call %Vec2 @Vec2.-(%Vec2* %a, %Vec2 %"$copy9")
  store %Vec2 %call.Vec2.-, %Vec2* %tmp10, align 4
  call void @Vec2.show(%Vec2* %tmp10, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.2, i32 0, i32 0))
  %"call.Vec2.*" = call %Vec2 @"Vec2.*"(%Vec2* %a, i32 3)
  store %Vec2 %"call.Vec2.*", %Vec2* %tmp11, align 4
  call void @Vec2.show(%Vec2* %tmp11, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.3, i32 0, i32 0))
  %"call.Vec2./" = call %Vec2 @"Vec2./"(%Vec2* %a, i32 2)
  store %Vec2 %"call.Vec2./", %Vec2* %tmp12, align 4
  call void @Vec2.show(%Vec2* %tmp12, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.4, i32 0, i32 0))
  %"call.Vec2.%" = call %Vec2 @"Vec2.%"(%Vec2* %a, i32 4)
  store %Vec2 %"call.Vec2.%", %Vec2* %tmp13, align 4
  call void @Vec2.show(%Vec2* %tmp13, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.5, i32 0, i32 0))
  store %Vec2 zeroinitializer, %Vec2* %copy14, align 4
  call void @"Vec2.="(%Vec2* %copy14, %Vec2* %b)
  %"$copy15" = load %Vec2, %Vec2* %copy14, align 4
  %"call.Vec2.==" = call i1 @"Vec2.=="(%Vec2* %a, %Vec2 %"$copy15")
  store %Vec2 zeroinitializer, %Vec2* %copy16, align 4
  call void @"Vec2.="(%Vec2* %copy16, %Vec2* %b)
  %"$copy17" = load %Vec2, %Vec2* %copy16, align 4
  %"call.Vec2.!=" = call i1 @"Vec2.!="(%Vec2* %a, %Vec2 %"$copy17")
  store i8* null, i8** %bool.text, align 8
  br label %if1

if1:                                              ; preds = %entry
  br i1 %"call.Vec2.==", label %if1.then, label %else2

if1.then:                                         ; preds = %if1
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.6, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else2:                                            ; preds = %if1
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.7, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else2, %if1.then
  store i8* null, i8** %bool.text18, align 8
  br label %if3

if3:                                              ; preds = %endif
  br i1 %"call.Vec2.!=", label %if3.then, label %else4

if3.then:                                         ; preds = %if3
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.8, i32 0, i32 0), i8** %bool.text18, align 8
  br label %endif1

else4:                                            ; preds = %if3
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.9, i32 0, i32 0), i8** %bool.text18, align 8
  br label %endif1

endif1:                                           ; preds = %else4, %if3.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %"$bool.text19" = load i8*, i8** %bool.text18, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.10, i32 0, i32 0), i8* %"$bool.text", i8* %"$bool.text19")
  store %Vec2 zeroinitializer, %Vec2* %copy20, align 4
  call void @"Vec2.="(%Vec2* %copy20, %Vec2* %b)
  %"$copy21" = load %Vec2, %Vec2* %copy20, align 4
  %"call.Vec2.<" = call i1 @"Vec2.<"(%Vec2* %a, %Vec2 %"$copy21")
  store %Vec2 zeroinitializer, %Vec2* %copy22, align 4
  call void @"Vec2.="(%Vec2* %copy22, %Vec2* %b)
  %"$copy23" = load %Vec2, %Vec2* %copy22, align 4
  %"call.Vec2.>" = call i1 @"Vec2.>"(%Vec2* %a, %Vec2 %"$copy23")
  store i8* null, i8** %bool.text24, align 8
  br label %if5

if5:                                              ; preds = %endif1
  br i1 %"call.Vec2.<", label %if5.then, label %else6

if5.then:                                         ; preds = %if5
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.11, i32 0, i32 0), i8** %bool.text24, align 8
  br label %endif2

else6:                                            ; preds = %if5
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.12, i32 0, i32 0), i8** %bool.text24, align 8
  br label %endif2

endif2:                                           ; preds = %else6, %if5.then
  store i8* null, i8** %bool.text25, align 8
  br label %if7

if7:                                              ; preds = %endif2
  br i1 %"call.Vec2.>", label %if7.then, label %else8

if7.then:                                         ; preds = %if7
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.13, i32 0, i32 0), i8** %bool.text25, align 8
  br label %endif3

else8:                                            ; preds = %if7
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.14, i32 0, i32 0), i8** %bool.text25, align 8
  br label %endif3

endif3:                                           ; preds = %else8, %if7.then
  %"$bool.text26" = load i8*, i8** %bool.text24, align 8
  %"$bool.text27" = load i8*, i8** %bool.text25, align 8
  %call.dprintf28 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.15, i32 0, i32 0), i8* %"$bool.text26", i8* %"$bool.text27")
  store %Vec2 zeroinitializer, %Vec2* %copy29, align 4
  call void @"Vec2.="(%Vec2* %copy29, %Vec2* %b)
  %"$copy30" = load %Vec2, %Vec2* %copy29, align 4
  %"call.Vec2.<=" = call i1 @"Vec2.<="(%Vec2* %a, %Vec2 %"$copy30")
  store %Vec2 zeroinitializer, %Vec2* %copy31, align 4
  call void @"Vec2.="(%Vec2* %copy31, %Vec2* %b)
  %"$copy32" = load %Vec2, %Vec2* %copy31, align 4
  %"call.Vec2.>=" = call i1 @"Vec2.>="(%Vec2* %a, %Vec2 %"$copy32")
  store i8* null, i8** %bool.text33, align 8
  br label %if9

if9:                                              ; preds = %endif3
  br i1 %"call.Vec2.<=", label %if9.then, label %else10

if9.then:                                         ; preds = %if9
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.16, i32 0, i32 0), i8** %bool.text33, align 8
  br label %endif4

else10:                                           ; preds = %if9
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.17, i32 0, i32 0), i8** %bool.text33, align 8
  br label %endif4

endif4:                                           ; preds = %else10, %if9.then
  store i8* null, i8** %bool.text34, align 8
  br label %if11

if11:                                             ; preds = %endif4
  br i1 %"call.Vec2.>=", label %if11.then, label %else12

if11.then:                                        ; preds = %if11
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.18, i32 0, i32 0), i8** %bool.text34, align 8
  br label %endif5

else12:                                           ; preds = %if11
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.19, i32 0, i32 0), i8** %bool.text34, align 8
  br label %endif5

endif5:                                           ; preds = %else12, %if11.then
  %"$bool.text35" = load i8*, i8** %bool.text33, align 8
  %"$bool.text36" = load i8*, i8** %bool.text34, align 8
  %call.dprintf37 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.20, i32 0, i32 0), i8* %"$bool.text35", i8* %"$bool.text36")
  %call.Vec2.create38 = call %Vec2 @Vec2.create(i32 0, i32 0)
  store %Vec2 %call.Vec2.create38, %Vec2* %c, align 4
  store %Vec2 zeroinitializer, %Vec2* %copy39, align 4
  call void @"Vec2.="(%Vec2* %copy39, %Vec2* %a)
  %"$copy40" = load %Vec2, %Vec2* %copy39, align 4
  store %Vec2 %"$copy40", %Vec2* %c, align 4
  call void @Vec2.show(%Vec2* %c, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.21, i32 0, i32 0))
  store %Vec2 zeroinitializer, %Vec2* %copy41, align 4
  call void @"Vec2.="(%Vec2* %copy41, %Vec2* %b)
  %"$copy42" = load %Vec2, %Vec2* %copy41, align 4
  call void @"Vec2.+="(%Vec2* %c, %Vec2 %"$copy42")
  call void @Vec2.show(%Vec2* %c, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.22, i32 0, i32 0))
  store %Vec2 zeroinitializer, %Vec2* %copy43, align 4
  call void @"Vec2.="(%Vec2* %copy43, %Vec2* %b)
  %"$copy44" = load %Vec2, %Vec2* %copy43, align 4
  call void @"Vec2.-="(%Vec2* %c, %Vec2 %"$copy44")
  call void @Vec2.show(%Vec2* %c, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.23, i32 0, i32 0))
  call void @"Vec2.*="(%Vec2* %c, i32 2)
  call void @Vec2.show(%Vec2* %c, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.24, i32 0, i32 0))
  call void @"Vec2./="(%Vec2* %c, i32 3)
  call void @Vec2.show(%Vec2* %c, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.25, i32 0, i32 0))
  call void @"Vec2.%="(%Vec2* %c, i32 5)
  call void @Vec2.show(%Vec2* %c, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.26, i32 0, i32 0))
  br label %if13

if13:                                             ; preds = %endif5
  store %Vec2 zeroinitializer, %Vec2* %copy45, align 4
  call void @"Vec2.="(%Vec2* %copy45, %Vec2* %b)
  %"$copy46" = load %Vec2, %Vec2* %copy45, align 4
  %"call.Vec2.>47" = call i1 @"Vec2.>"(%Vec2* %a, %Vec2 %"$copy46")
  br i1 %"call.Vec2.>47", label %if13.then, label %endif6

if13.then:                                        ; preds = %if13
  %call.dprintf48 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.27, i32 0, i32 0))
  br label %endif6

endif6:                                           ; preds = %if13.then, %if13
  store i32 0, i32* %n, align 4
  %call.Vec2.create49 = call %Vec2 @Vec2.create(i32 1, i32 1)
  store %Vec2 %call.Vec2.create49, %Vec2* %d, align 4
  br label %while14

while14:                                          ; preds = %while14.then, %endif6
  store %Vec2 zeroinitializer, %Vec2* %copy50, align 4
  call void @"Vec2.="(%Vec2* %copy50, %Vec2* %a)
  %"$copy51" = load %Vec2, %Vec2* %copy50, align 4
  %"call.Vec2.<52" = call i1 @"Vec2.<"(%Vec2* %d, %Vec2 %"$copy51")
  br i1 %"call.Vec2.<52", label %while14.then, label %endloop

while14.then:                                     ; preds = %while14
  store %Vec2 zeroinitializer, %Vec2* %copy53, align 4
  call void @"Vec2.="(%Vec2* %copy53, %Vec2* %b)
  %"$copy54" = load %Vec2, %Vec2* %copy53, align 4
  call void @"Vec2.+="(%Vec2* %d, %Vec2 %"$copy54")
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 1
  store i32 %ADD, i32* %n, align 4
  br label %while14

endloop:                                          ; preds = %while14
  %"$n55" = load i32, i32* %n, align 4
  %call.dprintf56 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.28, i32 0, i32 0), i32 %"$n55")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 024 — operator: bitwise operator overloads, and one returning a scalar

```ura
// stdout: and 8  or 14  xor 6
// stdout: shl 48  shr 3
// stdout: dot 23
// expect: 0

struct Flags:
    bits i32

    pub fn create(n i32) Flags:
        f Flags
        f.bits = n
        return f

    operator &(other Flags) Flags:
        return Flags::create(self.bits & other.bits)

    operator |(other Flags) Flags:
        return Flags::create(self.bits | other.bits)

    operator ^(other Flags) Flags:
        return Flags::create(self.bits ^ other.bits)

    operator <<(n i32) Flags:
        return Flags::create(self.bits << n)

    operator >>(n i32) Flags:
        return Flags::create(self.bits >> n)

struct Vec2:
    x i32
    y i32

    pub fn create(a i32, b i32) Vec2:
        v Vec2
        v.x = a
        v.y = b
        return v

    // an overload may return something other than the struct
    operator *(other Vec2) i32:
        return self.x * other.x + self.y * other.y

fn main() i32:
    a Flags = Flags::create(12)
    b Flags = Flags::create(10)
    output("and ", (a & b).bits, "  or ", (a | b).bits)
    output("  xor ", (a ^ b).bits, "\n")
    output("shl ", (a << 2).bits, "  shr ", (a >> 2).bits, "\n")

    p Vec2 = Vec2::create(2, 3)
    q Vec2 = Vec2::create(4, 5)
    output("dot ", p * q, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Flags = type { i32 }
%Vec2 = type { i32, i32 }

@str = private unnamed_addr constant [14 x i8] c"and %d  or %d\00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c"  xor %d\0A\00", align 1
@str.2 = private unnamed_addr constant [16 x i8] c"shl %d  shr %d\0A\00", align 1
@str.3 = private unnamed_addr constant [8 x i8] c"dot %d\0A\00", align 1

define %Flags @Flags.create(i32 %0) {
entry:
  %n = alloca i32, align 4
  %f = alloca %Flags, align 8
  store i32 %0, i32* %n, align 4
  store %Flags zeroinitializer, %Flags* %f, align 4
  %f.bits = getelementptr inbounds %Flags, %Flags* %f, i32 0, i32 0
  %"$n" = load i32, i32* %n, align 4
  store i32 %"$n", i32* %f.bits, align 4
  %"$f" = load %Flags, %Flags* %f, align 4
  ret %Flags %"$f"
}

define %Flags @"Flags.&"(%Flags* %0, %Flags %1) {
entry:
  %self = alloca %Flags*, align 8
  %other = alloca %Flags, align 8
  store %Flags* %0, %Flags** %self, align 8
  store %Flags %1, %Flags* %other, align 4
  %"*self" = load %Flags*, %Flags** %self, align 8
  %"*self.bits" = getelementptr inbounds %Flags, %Flags* %"*self", i32 0, i32 0
  %other.bits = getelementptr inbounds %Flags, %Flags* %other, i32 0, i32 0
  %"$*self.bits" = load i32, i32* %"*self.bits", align 4
  %"$other.bits" = load i32, i32* %other.bits, align 4
  %BAND = and i32 %"$*self.bits", %"$other.bits"
  %call.Flags.create = call %Flags @Flags.create(i32 %BAND)
  ret %Flags %call.Flags.create
}

define %Flags @"Flags.|"(%Flags* %0, %Flags %1) {
entry:
  %self = alloca %Flags*, align 8
  %other = alloca %Flags, align 8
  store %Flags* %0, %Flags** %self, align 8
  store %Flags %1, %Flags* %other, align 4
  %"*self" = load %Flags*, %Flags** %self, align 8
  %"*self.bits" = getelementptr inbounds %Flags, %Flags* %"*self", i32 0, i32 0
  %other.bits = getelementptr inbounds %Flags, %Flags* %other, i32 0, i32 0
  %"$*self.bits" = load i32, i32* %"*self.bits", align 4
  %"$other.bits" = load i32, i32* %other.bits, align 4
  %BOR = or i32 %"$*self.bits", %"$other.bits"
  %call.Flags.create = call %Flags @Flags.create(i32 %BOR)
  ret %Flags %call.Flags.create
}

define %Flags @"Flags.^"(%Flags* %0, %Flags %1) {
entry:
  %self = alloca %Flags*, align 8
  %other = alloca %Flags, align 8
  store %Flags* %0, %Flags** %self, align 8
  store %Flags %1, %Flags* %other, align 4
  %"*self" = load %Flags*, %Flags** %self, align 8
  %"*self.bits" = getelementptr inbounds %Flags, %Flags* %"*self", i32 0, i32 0
  %other.bits = getelementptr inbounds %Flags, %Flags* %other, i32 0, i32 0
  %"$*self.bits" = load i32, i32* %"*self.bits", align 4
  %"$other.bits" = load i32, i32* %other.bits, align 4
  %BXOR = xor i32 %"$*self.bits", %"$other.bits"
  %call.Flags.create = call %Flags @Flags.create(i32 %BXOR)
  ret %Flags %call.Flags.create
}

define %Flags @"Flags.<<"(%Flags* %0, i32 %1) {
entry:
  %self = alloca %Flags*, align 8
  %n = alloca i32, align 4
  store %Flags* %0, %Flags** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %Flags*, %Flags** %self, align 8
  %"*self.bits" = getelementptr inbounds %Flags, %Flags* %"*self", i32 0, i32 0
  %"$*self.bits" = load i32, i32* %"*self.bits", align 4
  %"$n" = load i32, i32* %n, align 4
  %LSHIFT = shl i32 %"$*self.bits", %"$n"
  %call.Flags.create = call %Flags @Flags.create(i32 %LSHIFT)
  ret %Flags %call.Flags.create
}

define %Flags @"Flags.>>"(%Flags* %0, i32 %1) {
entry:
  %self = alloca %Flags*, align 8
  %n = alloca i32, align 4
  store %Flags* %0, %Flags** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %Flags*, %Flags** %self, align 8
  %"*self.bits" = getelementptr inbounds %Flags, %Flags* %"*self", i32 0, i32 0
  %"$*self.bits" = load i32, i32* %"*self.bits", align 4
  %"$n" = load i32, i32* %n, align 4
  %RSHIFT = ashr i32 %"$*self.bits", %"$n"
  %call.Flags.create = call %Flags @Flags.create(i32 %RSHIFT)
  ret %Flags %call.Flags.create
}

define %Vec2 @Vec2.create(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %v = alloca %Vec2, align 8
  store i32 %0, i32* %a, align 4
  store i32 %1, i32* %b, align 4
  store %Vec2 zeroinitializer, %Vec2* %v, align 4
  %v.x = getelementptr inbounds %Vec2, %Vec2* %v, i32 0, i32 0
  %"$a" = load i32, i32* %a, align 4
  store i32 %"$a", i32* %v.x, align 4
  %v.y = getelementptr inbounds %Vec2, %Vec2* %v, i32 0, i32 1
  %"$b" = load i32, i32* %b, align 4
  store i32 %"$b", i32* %v.y, align 4
  %"$v" = load %Vec2, %Vec2* %v, align 4
  ret %Vec2 %"$v"
}

define i32 @"Vec2.*"(%Vec2* %0, %Vec2 %1) {
entry:
  %self = alloca %Vec2*, align 8
  %other = alloca %Vec2, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %"*self" = load %Vec2*, %Vec2** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec2, %Vec2* %"*self", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec2, %Vec2* %other, i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$other.x" = load i32, i32* %other.x, align 4
  %MUL = mul i32 %"$*self.x", %"$other.x"
  %"*self1" = load %Vec2*, %Vec2** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec2, %Vec2* %"*self1", i32 0, i32 1
  %other.y = getelementptr inbounds %Vec2, %Vec2* %other, i32 0, i32 1
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  %"$other.y" = load i32, i32* %other.y, align 4
  %MUL2 = mul i32 %"$*self.y", %"$other.y"
  %ADD = add i32 %MUL, %MUL2
  ret i32 %ADD
}

define i32 @main() {
entry:
  %a = alloca %Flags, align 8
  %b = alloca %Flags, align 8
  %tmp = alloca %Flags, align 8
  %tmp3 = alloca %Flags, align 8
  %tmp7 = alloca %Flags, align 8
  %tmp11 = alloca %Flags, align 8
  %tmp13 = alloca %Flags, align 8
  %p = alloca %Vec2, align 8
  %q = alloca %Vec2, align 8
  %call.Flags.create = call %Flags @Flags.create(i32 12)
  store %Flags %call.Flags.create, %Flags* %a, align 4
  %call.Flags.create1 = call %Flags @Flags.create(i32 10)
  store %Flags %call.Flags.create1, %Flags* %b, align 4
  %"$b" = load %Flags, %Flags* %b, align 4
  %"call.Flags.&" = call %Flags @"Flags.&"(%Flags* %a, %Flags %"$b")
  store %Flags %"call.Flags.&", %Flags* %tmp, align 4
  %tmp.bits = getelementptr inbounds %Flags, %Flags* %tmp, i32 0, i32 0
  %"$b2" = load %Flags, %Flags* %b, align 4
  %"call.Flags.|" = call %Flags @"Flags.|"(%Flags* %a, %Flags %"$b2")
  store %Flags %"call.Flags.|", %Flags* %tmp3, align 4
  %tmp.bits4 = getelementptr inbounds %Flags, %Flags* %tmp3, i32 0, i32 0
  %"$tmp.bits" = load i32, i32* %tmp.bits, align 4
  %"$tmp.bits5" = load i32, i32* %tmp.bits4, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str, i32 0, i32 0), i32 %"$tmp.bits", i32 %"$tmp.bits5")
  %"$b6" = load %Flags, %Flags* %b, align 4
  %"call.Flags.^" = call %Flags @"Flags.^"(%Flags* %a, %Flags %"$b6")
  store %Flags %"call.Flags.^", %Flags* %tmp7, align 4
  %tmp.bits8 = getelementptr inbounds %Flags, %Flags* %tmp7, i32 0, i32 0
  %"$tmp.bits9" = load i32, i32* %tmp.bits8, align 4
  %call.dprintf10 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i32 %"$tmp.bits9")
  %"call.Flags.<<" = call %Flags @"Flags.<<"(%Flags* %a, i32 2)
  store %Flags %"call.Flags.<<", %Flags* %tmp11, align 4
  %tmp.bits12 = getelementptr inbounds %Flags, %Flags* %tmp11, i32 0, i32 0
  %"call.Flags.>>" = call %Flags @"Flags.>>"(%Flags* %a, i32 2)
  store %Flags %"call.Flags.>>", %Flags* %tmp13, align 4
  %tmp.bits14 = getelementptr inbounds %Flags, %Flags* %tmp13, i32 0, i32 0
  %"$tmp.bits15" = load i32, i32* %tmp.bits12, align 4
  %"$tmp.bits16" = load i32, i32* %tmp.bits14, align 4
  %call.dprintf17 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.2, i32 0, i32 0), i32 %"$tmp.bits15", i32 %"$tmp.bits16")
  %call.Vec2.create = call %Vec2 @Vec2.create(i32 2, i32 3)
  store %Vec2 %call.Vec2.create, %Vec2* %p, align 4
  %call.Vec2.create18 = call %Vec2 @Vec2.create(i32 4, i32 5)
  store %Vec2 %call.Vec2.create18, %Vec2* %q, align 4
  %"$q" = load %Vec2, %Vec2* %q, align 4
  %"call.Vec2.*" = call i32 @"Vec2.*"(%Vec2* %p, %Vec2 %"$q")
  %call.dprintf19 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.3, i32 0, i32 0), i32 %"call.Vec2.*")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 025 — operator: compound bitwise assignment overloads

```ura
// stdout: 8 11 14 56 28
// expect: 0

struct Flags:
    bits i32

    operator &=(n i32) void:
        self.bits = self.bits & n

    operator |=(n i32) void:
        self.bits = self.bits | n

    operator ^=(n i32) void:
        self.bits = self.bits ^ n

    operator <<=(n i32) void:
        self.bits = self.bits << n

    operator >>=(n i32) void:
        self.bits = self.bits >> n

fn main() i32:
    f Flags
    f.bits = 12
    f &= 10
    output(f.bits, " ")
    f |= 3
    output(f.bits, " ")
    f ^= 5
    output(f.bits, " ")
    f <<= 2
    output(f.bits, " ")
    f >>= 1
    output(f.bits, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Flags = type { i32 }

@str = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str.3 = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str.4 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define void @"Flags.&="(%Flags* %0, i32 %1) {
entry:
  %self = alloca %Flags*, align 8
  %n = alloca i32, align 4
  store %Flags* %0, %Flags** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %Flags*, %Flags** %self, align 8
  %"*self.bits" = getelementptr inbounds %Flags, %Flags* %"*self", i32 0, i32 0
  %"*self1" = load %Flags*, %Flags** %self, align 8
  %"*self.bits2" = getelementptr inbounds %Flags, %Flags* %"*self1", i32 0, i32 0
  %"$*self.bits" = load i32, i32* %"*self.bits2", align 4
  %"$n" = load i32, i32* %n, align 4
  %BAND = and i32 %"$*self.bits", %"$n"
  store i32 %BAND, i32* %"*self.bits", align 4
  ret void
}

define void @"Flags.|="(%Flags* %0, i32 %1) {
entry:
  %self = alloca %Flags*, align 8
  %n = alloca i32, align 4
  store %Flags* %0, %Flags** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %Flags*, %Flags** %self, align 8
  %"*self.bits" = getelementptr inbounds %Flags, %Flags* %"*self", i32 0, i32 0
  %"*self1" = load %Flags*, %Flags** %self, align 8
  %"*self.bits2" = getelementptr inbounds %Flags, %Flags* %"*self1", i32 0, i32 0
  %"$*self.bits" = load i32, i32* %"*self.bits2", align 4
  %"$n" = load i32, i32* %n, align 4
  %BOR = or i32 %"$*self.bits", %"$n"
  store i32 %BOR, i32* %"*self.bits", align 4
  ret void
}

define void @"Flags.^="(%Flags* %0, i32 %1) {
entry:
  %self = alloca %Flags*, align 8
  %n = alloca i32, align 4
  store %Flags* %0, %Flags** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %Flags*, %Flags** %self, align 8
  %"*self.bits" = getelementptr inbounds %Flags, %Flags* %"*self", i32 0, i32 0
  %"*self1" = load %Flags*, %Flags** %self, align 8
  %"*self.bits2" = getelementptr inbounds %Flags, %Flags* %"*self1", i32 0, i32 0
  %"$*self.bits" = load i32, i32* %"*self.bits2", align 4
  %"$n" = load i32, i32* %n, align 4
  %BXOR = xor i32 %"$*self.bits", %"$n"
  store i32 %BXOR, i32* %"*self.bits", align 4
  ret void
}

define void @"Flags.<<="(%Flags* %0, i32 %1) {
entry:
  %self = alloca %Flags*, align 8
  %n = alloca i32, align 4
  store %Flags* %0, %Flags** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %Flags*, %Flags** %self, align 8
  %"*self.bits" = getelementptr inbounds %Flags, %Flags* %"*self", i32 0, i32 0
  %"*self1" = load %Flags*, %Flags** %self, align 8
  %"*self.bits2" = getelementptr inbounds %Flags, %Flags* %"*self1", i32 0, i32 0
  %"$*self.bits" = load i32, i32* %"*self.bits2", align 4
  %"$n" = load i32, i32* %n, align 4
  %LSHIFT = shl i32 %"$*self.bits", %"$n"
  store i32 %LSHIFT, i32* %"*self.bits", align 4
  ret void
}

define void @"Flags.>>="(%Flags* %0, i32 %1) {
entry:
  %self = alloca %Flags*, align 8
  %n = alloca i32, align 4
  store %Flags* %0, %Flags** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %Flags*, %Flags** %self, align 8
  %"*self.bits" = getelementptr inbounds %Flags, %Flags* %"*self", i32 0, i32 0
  %"*self1" = load %Flags*, %Flags** %self, align 8
  %"*self.bits2" = getelementptr inbounds %Flags, %Flags* %"*self1", i32 0, i32 0
  %"$*self.bits" = load i32, i32* %"*self.bits2", align 4
  %"$n" = load i32, i32* %n, align 4
  %RSHIFT = ashr i32 %"$*self.bits", %"$n"
  store i32 %RSHIFT, i32* %"*self.bits", align 4
  ret void
}

define i32 @main() {
entry:
  %f = alloca %Flags, align 8
  store %Flags zeroinitializer, %Flags* %f, align 4
  %f.bits = getelementptr inbounds %Flags, %Flags* %f, i32 0, i32 0
  store i32 12, i32* %f.bits, align 4
  call void @"Flags.&="(%Flags* %f, i32 10)
  %f.bits1 = getelementptr inbounds %Flags, %Flags* %f, i32 0, i32 0
  %"$f.bits" = load i32, i32* %f.bits1, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$f.bits")
  call void @"Flags.|="(%Flags* %f, i32 3)
  %f.bits2 = getelementptr inbounds %Flags, %Flags* %f, i32 0, i32 0
  %"$f.bits3" = load i32, i32* %f.bits2, align 4
  %call.dprintf4 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 %"$f.bits3")
  call void @"Flags.^="(%Flags* %f, i32 5)
  %f.bits5 = getelementptr inbounds %Flags, %Flags* %f, i32 0, i32 0
  %"$f.bits6" = load i32, i32* %f.bits5, align 4
  %call.dprintf7 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 %"$f.bits6")
  call void @"Flags.<<="(%Flags* %f, i32 2)
  %f.bits8 = getelementptr inbounds %Flags, %Flags* %f, i32 0, i32 0
  %"$f.bits9" = load i32, i32* %f.bits8, align 4
  %call.dprintf10 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.3, i32 0, i32 0), i32 %"$f.bits9")
  call void @"Flags.>>="(%Flags* %f, i32 1)
  %f.bits11 = getelementptr inbounds %Flags, %Flags* %f, i32 0, i32 0
  %"$f.bits12" = load i32, i32* %f.bits11, align 4
  %call.dprintf13 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i32 %"$f.bits12")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 026 — operator: bitwise and shift overloads taking a ref parameter

```ura
// stdout: a & b:  8
// stdout: a | b:  14
// stdout: a ^ b:  6
// stdout: a << 2: 48
// stdout: a >> 1: 6
// expect: 0

struct Flags:
    bits i32

    operator = (n i32) void:
        self.bits = n

    operator = (f &Flags) void:
        self.bits = f.bits

    operator & (f &Flags) Flags:
        res Flags
        res.bits = self.bits & f.bits
        return res

    operator | (f &Flags) Flags:
        res Flags
        res.bits = self.bits | f.bits
        return res

    operator ^ (f &Flags) Flags:
        res Flags
        res.bits = self.bits ^ f.bits
        return res

    operator << (n i32) Flags:
        res Flags
        res.bits = self.bits << n
        return res

    operator >> (n i32) Flags:
        res Flags
        res.bits = self.bits >> n
        return res

fn main() i32:
    a Flags
    a = 12

    b Flags
    b = 10

    band Flags = a & b
    output("a & b:  ", band.bits, "\n")

    bor Flags = a | b
    output("a | b:  ", bor.bits, "\n")

    bxor Flags = a ^ b
    output("a ^ b:  ", bxor.bits, "\n")

    lsh Flags = a << 2
    output("a << 2: ", lsh.bits, "\n")

    rsh Flags = a >> 1
    output("a >> 1: ", rsh.bits, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Flags = type { i32 }

@str = private unnamed_addr constant [12 x i8] c"a & b:  %d\0A\00", align 1
@str.1 = private unnamed_addr constant [12 x i8] c"a | b:  %d\0A\00", align 1
@str.2 = private unnamed_addr constant [12 x i8] c"a ^ b:  %d\0A\00", align 1
@str.3 = private unnamed_addr constant [12 x i8] c"a << 2: %d\0A\00", align 1
@str.4 = private unnamed_addr constant [12 x i8] c"a >> 1: %d\0A\00", align 1

define void @"Flags.="(%Flags* %0, i32 %1) {
entry:
  %self = alloca %Flags*, align 8
  %n = alloca i32, align 4
  store %Flags* %0, %Flags** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %Flags*, %Flags** %self, align 8
  %"*self.bits" = getelementptr inbounds %Flags, %Flags* %"*self", i32 0, i32 0
  %"$n" = load i32, i32* %n, align 4
  store i32 %"$n", i32* %"*self.bits", align 4
  ret void
}

define %Flags @"Flags.&"(%Flags* %0, %Flags* %1) {
entry:
  %self = alloca %Flags*, align 8
  %f = alloca %Flags*, align 8
  %res = alloca %Flags, align 8
  store %Flags* %0, %Flags** %self, align 8
  store %Flags* %1, %Flags** %f, align 8
  store %Flags zeroinitializer, %Flags* %res, align 4
  %res.bits = getelementptr inbounds %Flags, %Flags* %res, i32 0, i32 0
  %"*self" = load %Flags*, %Flags** %self, align 8
  %"*self.bits" = getelementptr inbounds %Flags, %Flags* %"*self", i32 0, i32 0
  %"*f" = load %Flags*, %Flags** %f, align 8
  %"*f.bits" = getelementptr inbounds %Flags, %Flags* %"*f", i32 0, i32 0
  %"$*self.bits" = load i32, i32* %"*self.bits", align 4
  %"$*f.bits" = load i32, i32* %"*f.bits", align 4
  %BAND = and i32 %"$*self.bits", %"$*f.bits"
  store i32 %BAND, i32* %res.bits, align 4
  %"$res" = load %Flags, %Flags* %res, align 4
  ret %Flags %"$res"
}

define %Flags @"Flags.|"(%Flags* %0, %Flags* %1) {
entry:
  %self = alloca %Flags*, align 8
  %f = alloca %Flags*, align 8
  %res = alloca %Flags, align 8
  store %Flags* %0, %Flags** %self, align 8
  store %Flags* %1, %Flags** %f, align 8
  store %Flags zeroinitializer, %Flags* %res, align 4
  %res.bits = getelementptr inbounds %Flags, %Flags* %res, i32 0, i32 0
  %"*self" = load %Flags*, %Flags** %self, align 8
  %"*self.bits" = getelementptr inbounds %Flags, %Flags* %"*self", i32 0, i32 0
  %"*f" = load %Flags*, %Flags** %f, align 8
  %"*f.bits" = getelementptr inbounds %Flags, %Flags* %"*f", i32 0, i32 0
  %"$*self.bits" = load i32, i32* %"*self.bits", align 4
  %"$*f.bits" = load i32, i32* %"*f.bits", align 4
  %BOR = or i32 %"$*self.bits", %"$*f.bits"
  store i32 %BOR, i32* %res.bits, align 4
  %"$res" = load %Flags, %Flags* %res, align 4
  ret %Flags %"$res"
}

define %Flags @"Flags.^"(%Flags* %0, %Flags* %1) {
entry:
  %self = alloca %Flags*, align 8
  %f = alloca %Flags*, align 8
  %res = alloca %Flags, align 8
  store %Flags* %0, %Flags** %self, align 8
  store %Flags* %1, %Flags** %f, align 8
  store %Flags zeroinitializer, %Flags* %res, align 4
  %res.bits = getelementptr inbounds %Flags, %Flags* %res, i32 0, i32 0
  %"*self" = load %Flags*, %Flags** %self, align 8
  %"*self.bits" = getelementptr inbounds %Flags, %Flags* %"*self", i32 0, i32 0
  %"*f" = load %Flags*, %Flags** %f, align 8
  %"*f.bits" = getelementptr inbounds %Flags, %Flags* %"*f", i32 0, i32 0
  %"$*self.bits" = load i32, i32* %"*self.bits", align 4
  %"$*f.bits" = load i32, i32* %"*f.bits", align 4
  %BXOR = xor i32 %"$*self.bits", %"$*f.bits"
  store i32 %BXOR, i32* %res.bits, align 4
  %"$res" = load %Flags, %Flags* %res, align 4
  ret %Flags %"$res"
}

define %Flags @"Flags.<<"(%Flags* %0, i32 %1) {
entry:
  %self = alloca %Flags*, align 8
  %n = alloca i32, align 4
  %res = alloca %Flags, align 8
  store %Flags* %0, %Flags** %self, align 8
  store i32 %1, i32* %n, align 4
  store %Flags zeroinitializer, %Flags* %res, align 4
  %res.bits = getelementptr inbounds %Flags, %Flags* %res, i32 0, i32 0
  %"*self" = load %Flags*, %Flags** %self, align 8
  %"*self.bits" = getelementptr inbounds %Flags, %Flags* %"*self", i32 0, i32 0
  %"$*self.bits" = load i32, i32* %"*self.bits", align 4
  %"$n" = load i32, i32* %n, align 4
  %LSHIFT = shl i32 %"$*self.bits", %"$n"
  store i32 %LSHIFT, i32* %res.bits, align 4
  %"$res" = load %Flags, %Flags* %res, align 4
  ret %Flags %"$res"
}

define %Flags @"Flags.>>"(%Flags* %0, i32 %1) {
entry:
  %self = alloca %Flags*, align 8
  %n = alloca i32, align 4
  %res = alloca %Flags, align 8
  store %Flags* %0, %Flags** %self, align 8
  store i32 %1, i32* %n, align 4
  store %Flags zeroinitializer, %Flags* %res, align 4
  %res.bits = getelementptr inbounds %Flags, %Flags* %res, i32 0, i32 0
  %"*self" = load %Flags*, %Flags** %self, align 8
  %"*self.bits" = getelementptr inbounds %Flags, %Flags* %"*self", i32 0, i32 0
  %"$*self.bits" = load i32, i32* %"*self.bits", align 4
  %"$n" = load i32, i32* %n, align 4
  %RSHIFT = ashr i32 %"$*self.bits", %"$n"
  store i32 %RSHIFT, i32* %res.bits, align 4
  %"$res" = load %Flags, %Flags* %res, align 4
  ret %Flags %"$res"
}

define i32 @main() {
entry:
  %a = alloca %Flags, align 8
  %b = alloca %Flags, align 8
  %band = alloca %Flags, align 8
  %bor = alloca %Flags, align 8
  %bxor = alloca %Flags, align 8
  %lsh = alloca %Flags, align 8
  %rsh = alloca %Flags, align 8
  store %Flags zeroinitializer, %Flags* %a, align 4
  call void @"Flags.="(%Flags* %a, i32 12)
  store %Flags zeroinitializer, %Flags* %b, align 4
  call void @"Flags.="(%Flags* %b, i32 10)
  %"call.Flags.&" = call %Flags @"Flags.&"(%Flags* %a, %Flags* %b)
  store %Flags %"call.Flags.&", %Flags* %band, align 4
  %band.bits = getelementptr inbounds %Flags, %Flags* %band, i32 0, i32 0
  %"$band.bits" = load i32, i32* %band.bits, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i32 %"$band.bits")
  %"call.Flags.|" = call %Flags @"Flags.|"(%Flags* %a, %Flags* %b)
  store %Flags %"call.Flags.|", %Flags* %bor, align 4
  %bor.bits = getelementptr inbounds %Flags, %Flags* %bor, i32 0, i32 0
  %"$bor.bits" = load i32, i32* %bor.bits, align 4
  %call.dprintf1 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.1, i32 0, i32 0), i32 %"$bor.bits")
  %"call.Flags.^" = call %Flags @"Flags.^"(%Flags* %a, %Flags* %b)
  store %Flags %"call.Flags.^", %Flags* %bxor, align 4
  %bxor.bits = getelementptr inbounds %Flags, %Flags* %bxor, i32 0, i32 0
  %"$bxor.bits" = load i32, i32* %bxor.bits, align 4
  %call.dprintf2 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.2, i32 0, i32 0), i32 %"$bxor.bits")
  %"call.Flags.<<" = call %Flags @"Flags.<<"(%Flags* %a, i32 2)
  store %Flags %"call.Flags.<<", %Flags* %lsh, align 4
  %lsh.bits = getelementptr inbounds %Flags, %Flags* %lsh, i32 0, i32 0
  %"$lsh.bits" = load i32, i32* %lsh.bits, align 4
  %call.dprintf3 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.3, i32 0, i32 0), i32 %"$lsh.bits")
  %"call.Flags.>>" = call %Flags @"Flags.>>"(%Flags* %a, i32 1)
  store %Flags %"call.Flags.>>", %Flags* %rsh, align 4
  %rsh.bits = getelementptr inbounds %Flags, %Flags* %rsh, i32 0, i32 0
  %"$rsh.bits" = load i32, i32* %rsh.bits, align 4
  %call.dprintf4 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.4, i32 0, i32 0), i32 %"$rsh.bits")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 027 — operator: operator output replaces the default struct dump, nested too

```ura
// stdout: direct: custom
// stdout: plain:  Plain{a: 1}
// stdout: nested: Holder{tag: inner, p: Plain{a: 2}}
// expect: 0

struct Tag:
    name char[]
    n    i32

    operator output() char[]:
        return self.name

struct Plain:
    a i32

struct Holder:
    tag Tag
    p   Plain

fn main() i32:
    t Tag
    t.name = "custom"
    t.n    = 7
    output("direct: ", t, "\n")

    p Plain
    p.a = 1
    output("plain:  ", p, "\n")

    h Holder
    h.tag.name = "inner"
    h.p.a      = 2
    output("nested: ", h, "\n")
    return 0
```

---

## 028 — struct: pub inside struct must be followed by fn

```ura
// error: Expected 'fn' after 'pub'
use "@/header"

struct Counter:
    n i32

    pub n i32

fn main() i32:
    output("hi\n")
    return 0
```

---

## 029 — struct: instance method called via :: instead of .

```ura
// error: 'Counter::bump' needs a receiver; call it on a value, or declare it 'pub fn' to make it static
use "@/header"

struct Counter:
    n i32

    fn bump():
        self.n += 1

fn main() i32:
    c Counter
    c.n = 0
    Counter::bump()
    return 0
```

---

## 030 — struct: pub fn called via . instead of ::

```ura
// error: 'm.make' is a 'pub fn', so it is static and takes no receiver; call it as 'Maker::create()'
use "@/header"

struct Maker:
    n i32

    pub fn create() Maker:
        m Maker
        m.n = 1
        return m

fn main() i32:
    m Maker
    m.n = 0
    m.make()
    return 0
```

---

## 031 — struct: pub may only be used inside a struct

```ura
// error: 'pub' marks a static method; it may only be used inside a struct
use "@/header"

pub fn helper():
    return

fn main() i32:
    helper()
    return 0
```

### stderr

```
error: test.ura:4:5 'pub' may only be used inside a struct
  |
4 | pub fn helper():
  |     ^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 032 — operator: [] returns &i32, so b[0] reads and writes through

```ura
proto malloc(size i32) i32[]

struct Bag:
    data i32[]

    operator [](index i32) &i32:
        return &self.data[index]

fn main() i32:
    b Bag
    b.data = malloc(40)
    b[0] = 5
    b[1] = b[0] + 2
    b[1] += 1
    output(b[0], " ", b[1], "\n")
    return b[1]
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Bag = type { i32* }

@str = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1

declare i32* @malloc(i32)

define i32* @"Bag.[]"(%Bag* %0, i32 %1) {
entry:
  %self = alloca %Bag*, align 8
  %index = alloca i32, align 4
  store %Bag* %0, %Bag** %self, align 8
  store i32 %1, i32* %index, align 4
  %"*self" = load %Bag*, %Bag** %self, align 8
  %"*self.data" = getelementptr inbounds %Bag, %Bag* %"*self", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$index"
  ret i32* %"*self.data[]"
}

define i32 @main() {
entry:
  %b = alloca %Bag, align 8
  %ref = alloca i32*, align 8
  %ref2 = alloca i32*, align 8
  %ref5 = alloca i32*, align 8
  %ref8 = alloca i32*, align 8
  %ref13 = alloca i32*, align 8
  %ref16 = alloca i32*, align 8
  %ref21 = alloca i32*, align 8
  store %Bag zeroinitializer, %Bag* %b, align 8
  %b.data = getelementptr inbounds %Bag, %Bag* %b, i32 0, i32 0
  %call.malloc = call i32* @malloc(i32 40)
  store i32* %call.malloc, i32** %b.data, align 8
  %"call.Bag.[]" = call i32* @"Bag.[]"(%Bag* %b, i32 0)
  store i32* %"call.Bag.[]", i32** %ref, align 8
  %"*ref" = load i32*, i32** %ref, align 8
  store i32 5, i32* %"*ref", align 4
  %"call.Bag.[]1" = call i32* @"Bag.[]"(%Bag* %b, i32 1)
  store i32* %"call.Bag.[]1", i32** %ref2, align 8
  %"*ref3" = load i32*, i32** %ref2, align 8
  %"call.Bag.[]4" = call i32* @"Bag.[]"(%Bag* %b, i32 0)
  store i32* %"call.Bag.[]4", i32** %ref5, align 8
  %"*ref6" = load i32*, i32** %ref5, align 8
  %"$*ref" = load i32, i32* %"*ref6", align 4
  %ADD = add i32 %"$*ref", 2
  store i32 %ADD, i32* %"*ref3", align 4
  %"call.Bag.[]7" = call i32* @"Bag.[]"(%Bag* %b, i32 1)
  store i32* %"call.Bag.[]7", i32** %ref8, align 8
  %"*ref9" = load i32*, i32** %ref8, align 8
  %"$*ref10" = load i32, i32* %"*ref9", align 4
  %ADD11 = add i32 %"$*ref10", 1
  store i32 %ADD11, i32* %"*ref9", align 4
  %"call.Bag.[]12" = call i32* @"Bag.[]"(%Bag* %b, i32 0)
  store i32* %"call.Bag.[]12", i32** %ref13, align 8
  %"*ref14" = load i32*, i32** %ref13, align 8
  %"call.Bag.[]15" = call i32* @"Bag.[]"(%Bag* %b, i32 1)
  store i32* %"call.Bag.[]15", i32** %ref16, align 8
  %"*ref17" = load i32*, i32** %ref16, align 8
  %"$*ref18" = load i32, i32* %"*ref14", align 4
  %"$*ref19" = load i32, i32* %"*ref17", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %"$*ref18", i32 %"$*ref19")
  %"call.Bag.[]20" = call i32* @"Bag.[]"(%Bag* %b, i32 1)
  store i32* %"call.Bag.[]20", i32** %ref21, align 8
  %"*ref22" = load i32*, i32** %ref21, align 8
  %"$*ref23" = load i32, i32* %"*ref22", align 4
  ret i32 %"$*ref23"
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 033 — operator: drop runs at the end of blocks, on continue, and not on a returned local

```ura
struct Noisy:
    id i32

    operator drop():
        output("drop ", self.id, "\n")

fn make(id i32) Noisy:
    n Noisy
    n.id = id
    return n

fn main() i32:
    a Noisy = make(1)
    i i32 = 0
    while i < 3:
        b Noisy
        b.id = 10 + i
        i += 1
        if i == 2:
            continue
        output("loop ", i, "\n")
    if a.id == 1:
        c Noisy
        c.id = 5
    x Array<i32>
    x.append(4)
    y Array<i32> = x
    y.append(5)
    output(x.size(), " ", y.size(), " ", y[1], "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Noisy = type { i32 }
%"Array<i32>" = type { i32*, i32, i32 }

@str = private unnamed_addr constant [9 x i8] c"drop %d\0A\00", align 1
@str.1 = private unnamed_addr constant [9 x i8] c"loop %d\0A\00", align 1
@str.2 = private unnamed_addr constant [10 x i8] c"%d %d %d\0A\00", align 1

define void @Noisy.drop(%Noisy* %0) {
entry:
  %self = alloca %Noisy*, align 8
  store %Noisy* %0, %Noisy** %self, align 8
  %"*self" = load %Noisy*, %Noisy** %self, align 8
  %"*self.id" = getelementptr inbounds %Noisy, %Noisy* %"*self", i32 0, i32 0
  %"$*self.id" = load i32, i32* %"*self.id", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %"$*self.id")
  ret void
}

define %Noisy @make(i32 %0) {
entry:
  %id = alloca i32, align 4
  %n = alloca %Noisy, align 8
  store i32 %0, i32* %id, align 4
  store %Noisy zeroinitializer, %Noisy* %n, align 4
  %n.id = getelementptr inbounds %Noisy, %Noisy* %n, i32 0, i32 0
  %"$id" = load i32, i32* %id, align 4
  store i32 %"$id", i32* %n.id, align 4
  %"$n" = load %Noisy, %Noisy* %n, align 4
  ret %Noisy %"$n"
}

define i32* @"new<i32>"(i32 %0) {
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

define void @"Array<i32>.reserve"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i32*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self6" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  ret void

endif1:                                           ; preds = %if1
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i32>" = call i32* @"new<i32>"(i32 %"$cap8")
  store i32* %"call.new<i32>", i32** %bigger, align 8
  %"*self9" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self9", i32 0, i32 0
  %"*self10" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self10", i32 0, i32 1
  %"$bigger" = load i32*, i32** %bigger, align 8
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i32>"(i32* %"$bigger", i32* %"$*self.data", i32 %"$*self.count12")
  br label %if2

if2:                                              ; preds = %endif1
  %"*self13" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap14" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*self16" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data17" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i32*, i32** %"*self.data17", align 8
  call void @"free<i32>"(i32* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"*self19" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data20" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self19", i32 0, i32 0
  %"$bigger21" = load i32*, i32** %bigger, align 8
  store i32* %"$bigger21", i32** %"*self.data20", align 8
  %"*self22" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap23" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @"Array<i32>.own"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %AND = and i1 %EQ, %GT
  br i1 %AND, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self2", i32 %"$*self.count5")
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  ret void
}

define void @"Array<i32>.clear"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  br label %if4

if4:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if4.then, label %endif

if4.then:                                         ; preds = %if4
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if4.then, %if4
  br label %while5

while5:                                           ; preds = %while5.then, %endif
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while5.then, label %endloop

while5.then:                                      ; preds = %while5
  %"*self4" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$*self.count10"
  store i32 0, i32* %"*self.data[]", align 4
  br label %while5

endloop:                                          ; preds = %while5
  ret void
}

define void @"Array<i32>.append"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %value = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %value, align 4
  br label %if6

if6:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  %"*self6" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self6", i32 0, i32 0
  %"*self7" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count8" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self7", i32 0, i32 1
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$*self.count9"
  %"$value" = load i32, i32* %value, align 4
  store i32 %"$value", i32* %"*self.data[]", align 4
  %"*self10" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  ret void
}

define i32 @"Array<i32>.size"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i32* @"Array<i32>.[]"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %index = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %index, align 4
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  call void @"Array<i32>.own"(%"Array<i32>"* %"*self")
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$index"
  ret i32* %"*self.data[]"
}

define void @"Array<i32>.="(%"Array<i32>"* %0, %"Array<i32>"* %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %other = alloca %"Array<i32>"*, align 8
  %i = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store %"Array<i32>"* %1, %"Array<i32>"** %other, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  call void @"Array<i32>.clear"(%"Array<i32>"* %"*self")
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*other" = load %"Array<i32>"*, %"Array<i32>"** %other, align 8
  %"*other.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*other", i32 0, i32 1
  %"$*other.count" = load i32, i32* %"*other.count", align 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self1", i32 %"$*other.count")
  store i32 0, i32* %i, align 4
  br label %while7

while7:                                           ; preds = %while7.then, %entry
  %"*other2" = load %"Array<i32>"*, %"Array<i32>"** %other, align 8
  %"*other.count3" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*other2", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*other.count4" = load i32, i32* %"*other.count3", align 4
  %LT = icmp slt i32 %"$i", %"$*other.count4"
  br i1 %LT, label %while7.then, label %endloop

while7.then:                                      ; preds = %while7
  %"*self5" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self5", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$i6" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$i6"
  %"*other7" = load %"Array<i32>"*, %"Array<i32>"** %other, align 8
  %"*other.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*other7", i32 0, i32 0
  %"$*other.data" = load i32*, i32** %"*other.data", align 8
  %"$i8" = load i32, i32* %i, align 4
  %"*other.data[]" = getelementptr inbounds i32, i32* %"$*other.data", i32 %"$i8"
  %"$*other.data[]" = load i32, i32* %"*other.data[]", align 4
  store i32 %"$*other.data[]", i32* %"*self.data[]", align 4
  %"$i9" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i9", 1
  store i32 %ADD, i32* %i, align 4
  br label %while7

endloop:                                          ; preds = %while7
  %"*self10" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self10", i32 0, i32 1
  %"*other11" = load %"Array<i32>"*, %"Array<i32>"** %other, align 8
  %"*other.count12" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*other11", i32 0, i32 1
  %"$*other.count13" = load i32, i32* %"*other.count12", align 4
  store i32 %"$*other.count13", i32* %"*self.count", align 4
  ret void
}

define void @"Array<i32>.drop"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  call void @"Array<i32>.clear"(%"Array<i32>"* %"*self")
  br label %if8

if8:                                              ; preds = %entry
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if8.then, label %endif

if8.then:                                         ; preds = %if8
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  call void @"free<i32>"(i32* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if8.then, %if8
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Noisy, align 8
  %i = alloca i32, align 4
  %b = alloca %Noisy, align 8
  %c = alloca %Noisy, align 8
  %x = alloca %"Array<i32>", align 8
  %y = alloca %"Array<i32>", align 8
  %copy = alloca %"Array<i32>", align 8
  %ref = alloca i32*, align 8
  %call.make = call %Noisy @make(i32 1)
  store %Noisy %call.make, %Noisy* %a, align 4
  store i32 0, i32* %i, align 4
  br label %while9

while9:                                           ; preds = %endif, %if10.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while9.then, label %endloop

while9.then:                                      ; preds = %while9
  store %Noisy zeroinitializer, %Noisy* %b, align 4
  %b.id = getelementptr inbounds %Noisy, %Noisy* %b, i32 0, i32 0
  %"$i2" = load i32, i32* %i, align 4
  %ADD = add i32 10, %"$i2"
  store i32 %ADD, i32* %b.id, align 4
  %"$i3" = load i32, i32* %i, align 4
  %ADD4 = add i32 %"$i3", 1
  store i32 %ADD4, i32* %i, align 4
  br label %if10

if10:                                             ; preds = %while9.then
  %"$i5" = load i32, i32* %i, align 4
  %EQ = icmp eq i32 %"$i5", 2
  br i1 %EQ, label %if10.then, label %endif

if10.then:                                        ; preds = %if10
  call void @Noisy.drop(%Noisy* %b)
  br label %while9

endif:                                            ; preds = %if10
  %"$i6" = load i32, i32* %i, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0), i32 %"$i6")
  call void @Noisy.drop(%Noisy* %b)
  br label %while9

endloop:                                          ; preds = %while9
  br label %if11

if11:                                             ; preds = %endloop
  %a.id = getelementptr inbounds %Noisy, %Noisy* %a, i32 0, i32 0
  %"$a.id" = load i32, i32* %a.id, align 4
  %EQ7 = icmp eq i32 %"$a.id", 1
  br i1 %EQ7, label %if11.then, label %endif1

if11.then:                                        ; preds = %if11
  store %Noisy zeroinitializer, %Noisy* %c, align 4
  %c.id = getelementptr inbounds %Noisy, %Noisy* %c, i32 0, i32 0
  store i32 5, i32* %c.id, align 4
  call void @Noisy.drop(%Noisy* %c)
  br label %endif1

endif1:                                           ; preds = %if11.then, %if11
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %x, align 8
  call void @"Array<i32>.append"(%"Array<i32>"* %x, i32 4)
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %copy, align 8
  call void @"Array<i32>.="(%"Array<i32>"* %copy, %"Array<i32>"* %x)
  %"$copy" = load %"Array<i32>", %"Array<i32>"* %copy, align 8
  store %"Array<i32>" %"$copy", %"Array<i32>"* %y, align 8
  call void @"Array<i32>.append"(%"Array<i32>"* %y, i32 5)
  %"call.Array<i32>.size" = call i32 @"Array<i32>.size"(%"Array<i32>"* %x)
  %"call.Array<i32>.size8" = call i32 @"Array<i32>.size"(%"Array<i32>"* %y)
  %"call.Array<i32>.[]" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %y, i32 1)
  store i32* %"call.Array<i32>.[]", i32** %ref, align 8
  %"*ref" = load i32*, i32** %ref, align 8
  %"$*ref" = load i32, i32* %"*ref", align 4
  %call.dprintf9 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.2, i32 0, i32 0), i32 %"call.Array<i32>.size", i32 %"call.Array<i32>.size8", i32 %"$*ref")
  call void @"Array<i32>.drop"(%"Array<i32>"* %y)
  call void @"Array<i32>.drop"(%"Array<i32>"* %x)
  call void @Noisy.drop(%Noisy* %a)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 034 — operator: copying a struct with drop needs operator =

```ura
struct Buffer:
    size i32

    operator drop():
        output("drop\n")

fn main() i32:
    a Buffer
    b Buffer = a
    return 0
```

### stderr

```
error: test.ura:9:16 'Buffer' can't be copied: it needs a drop and has no operator =
  |
9 |     b Buffer = a
  |                ^
help: define operator =(other &Buffer), or take it as &Buffer
error: aborting due to 1 error
```

### status

```
1
```

---

## 035 — operator: passing a struct with drop by value

```ura
struct Buffer:
    size i32

    operator drop():
        output("drop\n")

fn take(b Buffer) i32:
    return b.size

fn main() i32:
    a Buffer
    return take(a)
```

### stderr

```
error: test.ura:12:17 'Buffer' can't be copied: it needs a drop and has no operator =
   |
12 |     return take(a)
   |                 ^
help: define operator =(other &Buffer), or take it as &Buffer
error: aborting due to 1 error
```

### status

```
1
```

---

## 036 — struct: from takes attributes and methods of a template, a struct and a grandparent; overrides win

```ura
struct Named:
    label i8[]

    fn name() i8[]:
        return self.label

struct Text from Array<char> and Named:
    words i32

    fn push_back(c char):
        if self.count == self.cap:
            self.reserve(self.cap * 2 + 4)
        self.data[self.count] = c
        self.count += 1
        if c == ' ':
            self.words += 1

    fn shout() Text:
        res Text
        i i32 = 0
        while i < self.size():
            c char = self[i]
            if c >= 'a' and c <= 'z':
                c = (c as i32 - 32) as char
            res.append(c)
            i += 1
        return res

struct Title from Text:
    fn first() char:
        return self[0]

fn main() i32:
    t Text
    t.label = "greeting"
    t.append('h')
    t.append('i')
    t.append(' ')
    t.append('u')
    loud Text = t.shout()
    title Title
    title.append('z')
    output(t.size(), " ", t.words, " ", loud[0], loud[1], loud[3], " ", t.name(), " ", title.first(), " ", typeof(loud), "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Text = type { i8*, i32, i32, i8*, i32 }
%Title = type { i8*, i32, i32, i8*, i32 }

@str = private unnamed_addr constant [9 x i8] c"greeting\00", align 1
@TYPEOF = private unnamed_addr constant [5 x i8] c"Text\00", align 1
@str.1 = private unnamed_addr constant [23 x i8] c"%d %d %c%c%c %s %c %s\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @Text.reserve(%Text* %0, i32 %1) {
entry:
  %self = alloca %Text*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %Text* %0, %Text** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %Text*, %Text** %self, align 8
  %"*self.count" = getelementptr inbounds %Text, %Text* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self3" = load %Text*, %Text** %self, align 8
  %"*self.count4" = getelementptr inbounds %Text, %Text* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self6" = load %Text*, %Text** %self, align 8
  %"*self.cap" = getelementptr inbounds %Text, %Text* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  ret void

endif1:                                           ; preds = %if1
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %Text*, %Text** %self, align 8
  %"*self.data" = getelementptr inbounds %Text, %Text* %"*self9", i32 0, i32 0
  %"*self10" = load %Text*, %Text** %self, align 8
  %"*self.count11" = getelementptr inbounds %Text, %Text* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if2

if2:                                              ; preds = %endif1
  %"*self13" = load %Text*, %Text** %self, align 8
  %"*self.cap14" = getelementptr inbounds %Text, %Text* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*self16" = load %Text*, %Text** %self, align 8
  %"*self.data17" = getelementptr inbounds %Text, %Text* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"*self19" = load %Text*, %Text** %self, align 8
  %"*self.data20" = getelementptr inbounds %Text, %Text* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %Text*, %Text** %self, align 8
  %"*self.cap23" = getelementptr inbounds %Text, %Text* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @Text.own(%Text* %0) {
entry:
  %self = alloca %Text*, align 8
  store %Text* %0, %Text** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %Text*, %Text** %self, align 8
  %"*self.cap" = getelementptr inbounds %Text, %Text* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  %"*self1" = load %Text*, %Text** %self, align 8
  %"*self.count" = getelementptr inbounds %Text, %Text* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %AND = and i1 %EQ, %GT
  br i1 %AND, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self2" = load %Text*, %Text** %self, align 8
  %"*self3" = load %Text*, %Text** %self, align 8
  %"*self.count4" = getelementptr inbounds %Text, %Text* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  call void @Text.reserve(%Text* %"*self2", i32 %"$*self.count5")
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  ret void
}

define void @Text.clear(%Text* %0) {
entry:
  %self = alloca %Text*, align 8
  store %Text* %0, %Text** %self, align 8
  br label %if4

if4:                                              ; preds = %entry
  %"*self" = load %Text*, %Text** %self, align 8
  %"*self.cap" = getelementptr inbounds %Text, %Text* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if4.then, label %endif

if4.then:                                         ; preds = %if4
  %"*self1" = load %Text*, %Text** %self, align 8
  %"*self.count" = getelementptr inbounds %Text, %Text* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if4.then, %if4
  br label %while5

while5:                                           ; preds = %while5.then, %endif
  %"*self2" = load %Text*, %Text** %self, align 8
  %"*self.count3" = getelementptr inbounds %Text, %Text* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while5.then, label %endloop

while5.then:                                      ; preds = %while5
  %"*self4" = load %Text*, %Text** %self, align 8
  %"*self.count5" = getelementptr inbounds %Text, %Text* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %Text*, %Text** %self, align 8
  %"*self.data" = getelementptr inbounds %Text, %Text* %"*self7", i32 0, i32 0
  %"*self8" = load %Text*, %Text** %self, align 8
  %"*self.count9" = getelementptr inbounds %Text, %Text* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while5

endloop:                                          ; preds = %while5
  ret void
}

define void @Text.append(%Text* %0, i8 %1) {
entry:
  %self = alloca %Text*, align 8
  %value = alloca i8, align 1
  store %Text* %0, %Text** %self, align 8
  store i8 %1, i8* %value, align 1
  br label %if6

if6:                                              ; preds = %entry
  %"*self" = load %Text*, %Text** %self, align 8
  %"*self.count" = getelementptr inbounds %Text, %Text* %"*self", i32 0, i32 1
  %"*self1" = load %Text*, %Text** %self, align 8
  %"*self.cap" = getelementptr inbounds %Text, %Text* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*self2" = load %Text*, %Text** %self, align 8
  %"*self3" = load %Text*, %Text** %self, align 8
  %"*self.count4" = getelementptr inbounds %Text, %Text* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @Text.reserve(%Text* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  %"*self6" = load %Text*, %Text** %self, align 8
  %"*self.data" = getelementptr inbounds %Text, %Text* %"*self6", i32 0, i32 0
  %"*self7" = load %Text*, %Text** %self, align 8
  %"*self.count8" = getelementptr inbounds %Text, %Text* %"*self7", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count9"
  %"$value" = load i8, i8* %value, align 1
  store i8 %"$value", i8* %"*self.data[]", align 1
  %"*self10" = load %Text*, %Text** %self, align 8
  %"*self.count11" = getelementptr inbounds %Text, %Text* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  ret void
}

define i32 @Text.size(%Text* %0) {
entry:
  %self = alloca %Text*, align 8
  store %Text* %0, %Text** %self, align 8
  %"*self" = load %Text*, %Text** %self, align 8
  %"*self.count" = getelementptr inbounds %Text, %Text* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i8* @"Text.[]"(%Text* %0, i32 %1) {
entry:
  %self = alloca %Text*, align 8
  %index = alloca i32, align 4
  store %Text* %0, %Text** %self, align 8
  store i32 %1, i32* %index, align 4
  %"*self" = load %Text*, %Text** %self, align 8
  call void @Text.own(%Text* %"*self")
  %"*self1" = load %Text*, %Text** %self, align 8
  %"*self.data" = getelementptr inbounds %Text, %Text* %"*self1", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$index"
  ret i8* %"*self.data[]"
}

define void @Text.drop(%Text* %0) {
entry:
  %self = alloca %Text*, align 8
  store %Text* %0, %Text** %self, align 8
  %"*self" = load %Text*, %Text** %self, align 8
  call void @Text.clear(%Text* %"*self")
  br label %if7

if7:                                              ; preds = %entry
  %"*self1" = load %Text*, %Text** %self, align 8
  %"*self.cap" = getelementptr inbounds %Text, %Text* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if7.then, label %endif

if7.then:                                         ; preds = %if7
  %"*self2" = load %Text*, %Text** %self, align 8
  %"*self.data" = getelementptr inbounds %Text, %Text* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if7.then, %if7
  ret void
}

define i8* @Text.name(%Text* %0) {
entry:
  %self = alloca %Text*, align 8
  store %Text* %0, %Text** %self, align 8
  %"*self" = load %Text*, %Text** %self, align 8
  %"*self.label" = getelementptr inbounds %Text, %Text* %"*self", i32 0, i32 3
  %"$*self.label" = load i8*, i8** %"*self.label", align 8
  ret i8* %"$*self.label"
}

define %Text @Text.shout(%Text* %0) {
entry:
  %self = alloca %Text*, align 8
  %res = alloca %Text, align 8
  %i = alloca i32, align 4
  %c = alloca i8, align 1
  %ref = alloca i8*, align 8
  store %Text* %0, %Text** %self, align 8
  store %Text zeroinitializer, %Text* %res, align 8
  store i32 0, i32* %i, align 4
  br label %while8

while8:                                           ; preds = %endif, %entry
  %"*self" = load %Text*, %Text** %self, align 8
  %call.Text.size = call i32 @Text.size(%Text* %"*self")
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", %call.Text.size
  br i1 %LT, label %while8.then, label %endloop

while8.then:                                      ; preds = %while8
  %"*self1" = load %Text*, %Text** %self, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"call.Text.[]" = call i8* @"Text.[]"(%Text* %"*self1", i32 %"$i2")
  store i8* %"call.Text.[]", i8** %ref, align 8
  %"*ref" = load i8*, i8** %ref, align 8
  %"$*ref" = load i8, i8* %"*ref", align 1
  store i8 %"$*ref", i8* %c, align 1
  br label %if9

if9:                                              ; preds = %while8.then
  %"$c" = load i8, i8* %c, align 1
  %GE = icmp sge i8 %"$c", 97
  %"$c3" = load i8, i8* %c, align 1
  %LE = icmp sle i8 %"$c3", 122
  %AND = and i1 %GE, %LE
  br i1 %AND, label %if9.then, label %endif

if9.then:                                         ; preds = %if9
  %"$c4" = load i8, i8* %c, align 1
  %AS = sext i8 %"$c4" to i32
  %SUB = sub i32 %AS, 32
  %AS5 = trunc i32 %SUB to i8
  store i8 %AS5, i8* %c, align 1
  br label %endif

endif:                                            ; preds = %if9.then, %if9
  %"$c6" = load i8, i8* %c, align 1
  call void @Text.append(%Text* %res, i8 %"$c6")
  %"$i7" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i7", 1
  store i32 %ADD, i32* %i, align 4
  br label %while8

endloop:                                          ; preds = %while8
  %"$res" = load %Text, %Text* %res, align 8
  ret %Text %"$res"
}

define void @Title.reserve(%Title* %0, i32 %1) {
entry:
  %self = alloca %Title*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %Title* %0, %Title** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if10

if10:                                             ; preds = %entry
  %"*self" = load %Title*, %Title** %self, align 8
  %"*self.count" = getelementptr inbounds %Title, %Title* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if10.then, label %endif

if10.then:                                        ; preds = %if10
  %"*self3" = load %Title*, %Title** %self, align 8
  %"*self.count4" = getelementptr inbounds %Title, %Title* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if10.then, %if10
  br label %if11

if11:                                             ; preds = %endif
  %"*self6" = load %Title*, %Title** %self, align 8
  %"*self.cap" = getelementptr inbounds %Title, %Title* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if11.then, label %endif1

if11.then:                                        ; preds = %if11
  ret void

endif1:                                           ; preds = %if11
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %Title*, %Title** %self, align 8
  %"*self.data" = getelementptr inbounds %Title, %Title* %"*self9", i32 0, i32 0
  %"*self10" = load %Title*, %Title** %self, align 8
  %"*self.count11" = getelementptr inbounds %Title, %Title* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if12

if12:                                             ; preds = %endif1
  %"*self13" = load %Title*, %Title** %self, align 8
  %"*self.cap14" = getelementptr inbounds %Title, %Title* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if12.then, label %endif2

if12.then:                                        ; preds = %if12
  %"*self16" = load %Title*, %Title** %self, align 8
  %"*self.data17" = getelementptr inbounds %Title, %Title* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if12.then, %if12
  %"*self19" = load %Title*, %Title** %self, align 8
  %"*self.data20" = getelementptr inbounds %Title, %Title* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %Title*, %Title** %self, align 8
  %"*self.cap23" = getelementptr inbounds %Title, %Title* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @Title.own(%Title* %0) {
entry:
  %self = alloca %Title*, align 8
  store %Title* %0, %Title** %self, align 8
  br label %if13

if13:                                             ; preds = %entry
  %"*self" = load %Title*, %Title** %self, align 8
  %"*self.cap" = getelementptr inbounds %Title, %Title* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  %"*self1" = load %Title*, %Title** %self, align 8
  %"*self.count" = getelementptr inbounds %Title, %Title* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %AND = and i1 %EQ, %GT
  br i1 %AND, label %if13.then, label %endif

if13.then:                                        ; preds = %if13
  %"*self2" = load %Title*, %Title** %self, align 8
  %"*self3" = load %Title*, %Title** %self, align 8
  %"*self.count4" = getelementptr inbounds %Title, %Title* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  call void @Title.reserve(%Title* %"*self2", i32 %"$*self.count5")
  br label %endif

endif:                                            ; preds = %if13.then, %if13
  ret void
}

define void @Title.clear(%Title* %0) {
entry:
  %self = alloca %Title*, align 8
  store %Title* %0, %Title** %self, align 8
  br label %if14

if14:                                             ; preds = %entry
  %"*self" = load %Title*, %Title** %self, align 8
  %"*self.cap" = getelementptr inbounds %Title, %Title* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if14.then, label %endif

if14.then:                                        ; preds = %if14
  %"*self1" = load %Title*, %Title** %self, align 8
  %"*self.count" = getelementptr inbounds %Title, %Title* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if14.then, %if14
  br label %while15

while15:                                          ; preds = %while15.then, %endif
  %"*self2" = load %Title*, %Title** %self, align 8
  %"*self.count3" = getelementptr inbounds %Title, %Title* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while15.then, label %endloop

while15.then:                                     ; preds = %while15
  %"*self4" = load %Title*, %Title** %self, align 8
  %"*self.count5" = getelementptr inbounds %Title, %Title* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %Title*, %Title** %self, align 8
  %"*self.data" = getelementptr inbounds %Title, %Title* %"*self7", i32 0, i32 0
  %"*self8" = load %Title*, %Title** %self, align 8
  %"*self.count9" = getelementptr inbounds %Title, %Title* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while15

endloop:                                          ; preds = %while15
  ret void
}

define void @Title.append(%Title* %0, i8 %1) {
entry:
  %self = alloca %Title*, align 8
  %value = alloca i8, align 1
  store %Title* %0, %Title** %self, align 8
  store i8 %1, i8* %value, align 1
  br label %if16

if16:                                             ; preds = %entry
  %"*self" = load %Title*, %Title** %self, align 8
  %"*self.count" = getelementptr inbounds %Title, %Title* %"*self", i32 0, i32 1
  %"*self1" = load %Title*, %Title** %self, align 8
  %"*self.cap" = getelementptr inbounds %Title, %Title* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if16.then, label %endif

if16.then:                                        ; preds = %if16
  %"*self2" = load %Title*, %Title** %self, align 8
  %"*self3" = load %Title*, %Title** %self, align 8
  %"*self.count4" = getelementptr inbounds %Title, %Title* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @Title.reserve(%Title* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if16.then, %if16
  %"*self6" = load %Title*, %Title** %self, align 8
  %"*self.data" = getelementptr inbounds %Title, %Title* %"*self6", i32 0, i32 0
  %"*self7" = load %Title*, %Title** %self, align 8
  %"*self.count8" = getelementptr inbounds %Title, %Title* %"*self7", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count9"
  %"$value" = load i8, i8* %value, align 1
  store i8 %"$value", i8* %"*self.data[]", align 1
  %"*self10" = load %Title*, %Title** %self, align 8
  %"*self.count11" = getelementptr inbounds %Title, %Title* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  ret void
}

define i8* @"Title.[]"(%Title* %0, i32 %1) {
entry:
  %self = alloca %Title*, align 8
  %index = alloca i32, align 4
  store %Title* %0, %Title** %self, align 8
  store i32 %1, i32* %index, align 4
  %"*self" = load %Title*, %Title** %self, align 8
  call void @Title.own(%Title* %"*self")
  %"*self1" = load %Title*, %Title** %self, align 8
  %"*self.data" = getelementptr inbounds %Title, %Title* %"*self1", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$index"
  ret i8* %"*self.data[]"
}

define void @Title.drop(%Title* %0) {
entry:
  %self = alloca %Title*, align 8
  store %Title* %0, %Title** %self, align 8
  %"*self" = load %Title*, %Title** %self, align 8
  call void @Title.clear(%Title* %"*self")
  br label %if17

if17:                                             ; preds = %entry
  %"*self1" = load %Title*, %Title** %self, align 8
  %"*self.cap" = getelementptr inbounds %Title, %Title* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if17.then, label %endif

if17.then:                                        ; preds = %if17
  %"*self2" = load %Title*, %Title** %self, align 8
  %"*self.data" = getelementptr inbounds %Title, %Title* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if17.then, %if17
  ret void
}

define i8 @Title.first(%Title* %0) {
entry:
  %self = alloca %Title*, align 8
  %ref = alloca i8*, align 8
  store %Title* %0, %Title** %self, align 8
  %"*self" = load %Title*, %Title** %self, align 8
  %"call.Title.[]" = call i8* @"Title.[]"(%Title* %"*self", i32 0)
  store i8* %"call.Title.[]", i8** %ref, align 8
  %"*ref" = load i8*, i8** %ref, align 8
  %"$*ref" = load i8, i8* %"*ref", align 1
  ret i8 %"$*ref"
}

define i32 @main() {
entry:
  %t = alloca %Text, align 8
  %loud = alloca %Text, align 8
  %title = alloca %Title, align 8
  %ref = alloca i8*, align 8
  %ref2 = alloca i8*, align 8
  %ref5 = alloca i8*, align 8
  store %Text zeroinitializer, %Text* %t, align 8
  %t.label = getelementptr inbounds %Text, %Text* %t, i32 0, i32 3
  store i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i8** %t.label, align 8
  call void @Text.append(%Text* %t, i8 104)
  call void @Text.append(%Text* %t, i8 105)
  call void @Text.append(%Text* %t, i8 32)
  call void @Text.append(%Text* %t, i8 117)
  %call.Text.shout = call %Text @Text.shout(%Text* %t)
  store %Text %call.Text.shout, %Text* %loud, align 8
  store %Title zeroinitializer, %Title* %title, align 8
  call void @Title.append(%Title* %title, i8 122)
  %call.Text.size = call i32 @Text.size(%Text* %t)
  %t.words = getelementptr inbounds %Text, %Text* %t, i32 0, i32 4
  %"call.Text.[]" = call i8* @"Text.[]"(%Text* %loud, i32 0)
  store i8* %"call.Text.[]", i8** %ref, align 8
  %"*ref" = load i8*, i8** %ref, align 8
  %"call.Text.[]1" = call i8* @"Text.[]"(%Text* %loud, i32 1)
  store i8* %"call.Text.[]1", i8** %ref2, align 8
  %"*ref3" = load i8*, i8** %ref2, align 8
  %"call.Text.[]4" = call i8* @"Text.[]"(%Text* %loud, i32 3)
  store i8* %"call.Text.[]4", i8** %ref5, align 8
  %"*ref6" = load i8*, i8** %ref5, align 8
  %call.Text.name = call i8* @Text.name(%Text* %t)
  %call.Title.first = call i8 @Title.first(%Title* %title)
  %"$t.words" = load i32, i32* %t.words, align 4
  %"$*ref" = load i8, i8* %"*ref", align 1
  %AS = sext i8 %"$*ref" to i32
  %"$*ref7" = load i8, i8* %"*ref3", align 1
  %AS8 = sext i8 %"$*ref7" to i32
  %"$*ref9" = load i8, i8* %"*ref6", align 1
  %AS10 = sext i8 %"$*ref9" to i32
  %AS11 = sext i8 %call.Title.first to i32
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.1, i32 0, i32 0), i32 %call.Text.size, i32 %"$t.words", i32 %AS, i32 %AS8, i32 %AS10, i8* %call.Text.name, i32 %AS11, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @TYPEOF, i32 0, i32 0))
  call void @Title.drop(%Title* %title)
  call void @Text.drop(%Text* %loud)
  call void @Text.drop(%Text* %t)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 037 — struct: from a struct that doesn't exist

```ura
struct Text from Nothing:
    size i32

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:1:18 no struct named 'Nothing' to take from
  |
1 | struct Text from Nothing:
  |                  ^^^^^^^
error: aborting due to 1 error
```

### status

```
1
```


---

## 038 — struct: String from Array<char> in "@/string", temporaries are dropped

```ura
use "@/string"

fn main() i32:
    s String = "  Hello World  "
    t String = s.trim()
    output("[", t.chars(), "] ", t.size(), "\n")
    output(t.upper().chars(), " ", t.lower().chars(), " ", t.reverse().chars(), "\n")
    output(t.find("World"), " ", t.contains("lo W"), " ", t.starts_with("Hell"), " ", t.ends_with("ld"), " ", t.find("xyz"), "\n")
    r String = t.replace("World", "ura")
    output(r.chars(), " ", r.substr(1, 3).chars(), " ", r.repeat(2).chars(), "\n")
    a String = String::create("abc")
    b String = "abd"
    c String = a + "-" + b
    c += "!"
    output(c.chars(), " ", a < b, " ", a == "abc", " ", a != b, " ", c.size(), "\n")
    n String = "42"
    output(n.to_int() + 1, " ", typeof(n), "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [16 x i8] c"  Hello World  \00", align 1
@str.3 = private unnamed_addr constant [9 x i8] c"[%s] %d\0A\00", align 1
@str.4 = private unnamed_addr constant [10 x i8] c"%s %s %s\0A\00", align 1
@str.5 = private unnamed_addr constant [6 x i8] c"World\00", align 1
@str.6 = private unnamed_addr constant [5 x i8] c"lo W\00", align 1
@str.7 = private unnamed_addr constant [5 x i8] c"Hell\00", align 1
@str.8 = private unnamed_addr constant [3 x i8] c"ld\00", align 1
@str.9 = private unnamed_addr constant [4 x i8] c"xyz\00", align 1
@str.10 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.11 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.12 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.13 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.14 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.15 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.16 = private unnamed_addr constant [16 x i8] c"%d %s %s %s %d\0A\00", align 1
@str.17 = private unnamed_addr constant [6 x i8] c"World\00", align 1
@str.18 = private unnamed_addr constant [4 x i8] c"ura\00", align 1
@str.19 = private unnamed_addr constant [10 x i8] c"%s %s %s\0A\00", align 1
@str.20 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@str.21 = private unnamed_addr constant [4 x i8] c"abd\00", align 1
@str.22 = private unnamed_addr constant [2 x i8] c"-\00", align 1
@str.23 = private unnamed_addr constant [2 x i8] c"!\00", align 1
@str.24 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@str.25 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.26 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.27 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.28 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.29 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.30 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.31 = private unnamed_addr constant [16 x i8] c"%s %s %s %s %d\0A\00", align 1
@str.32 = private unnamed_addr constant [3 x i8] c"42\00", align 1
@TYPEOF = private unnamed_addr constant [7 x i8] c"String\00", align 1
@str.33 = private unnamed_addr constant [7 x i8] c"%d %s\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  ret void

endif1:                                           ; preds = %if1
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if2

if2:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %while4

while4:                                           ; preds = %while4.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.count5" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.count9" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while4

endloop:                                          ; preds = %while4
  ret void
}

define void @String.append(%String* %0, i8 %1) {
entry:
  %self = alloca %String*, align 8
  %value = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store i8 %1, i8* %value, align 1
  br label %if5

if5:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @String.reserve(%String* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if5.then, %if5
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.count8" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count9"
  %"$value" = load i8, i8* %value, align 1
  store i8 %"$value", i8* %"*self.data[]", align 1
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  ret void
}

define i32 @String.size(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @"String.="(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  %other = alloca %String*, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store %String* %1, %String** %other, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  %"*self1" = load %String*, %String** %self, align 8
  %"*other" = load %String*, %String** %other, align 8
  %"*other.count" = getelementptr inbounds %String, %String* %"*other", i32 0, i32 1
  %"$*other.count" = load i32, i32* %"*other.count", align 4
  call void @String.reserve(%String* %"*self1", i32 %"$*other.count")
  store i32 0, i32* %i, align 4
  br label %while6

while6:                                           ; preds = %while6.then, %entry
  %"*other2" = load %String*, %String** %other, align 8
  %"*other.count3" = getelementptr inbounds %String, %String* %"*other2", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*other.count4" = load i32, i32* %"*other.count3", align 4
  %LT = icmp slt i32 %"$i", %"$*other.count4"
  br i1 %LT, label %while6.then, label %endloop

while6.then:                                      ; preds = %while6
  %"*self5" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self5", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i6" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i6"
  %"*other7" = load %String*, %String** %other, align 8
  %"*other.data" = getelementptr inbounds %String, %String* %"*other7", i32 0, i32 0
  %"$*other.data" = load i8*, i8** %"*other.data", align 8
  %"$i8" = load i32, i32* %i, align 4
  %"*other.data[]" = getelementptr inbounds i8, i8* %"$*other.data", i32 %"$i8"
  %"$*other.data[]" = load i8, i8* %"*other.data[]", align 1
  store i8 %"$*other.data[]", i8* %"*self.data[]", align 1
  %"$i9" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i9", 1
  store i32 %ADD, i32* %i, align 4
  br label %while6

endloop:                                          ; preds = %while6
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"*other11" = load %String*, %String** %other, align 8
  %"*other.count12" = getelementptr inbounds %String, %String* %"*other11", i32 0, i32 1
  %"$*other.count13" = load i32, i32* %"*other.count12", align 4
  store i32 %"$*other.count13", i32* %"*self.count", align 4
  ret void
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if7

if7:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if7.then, label %endif

if7.then:                                         ; preds = %if7
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if7.then, %if7
  ret void
}

define %String @String.create(i8* %0) {
entry:
  %text = alloca i8*, align 8
  %res = alloca %String, align 8
  store i8* %0, i8** %text, align 8
  store %String zeroinitializer, %String* %res, align 8
  %"$text" = load i8*, i8** %text, align 8
  call void @String.join(%String* %res, i8* %"$text")
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define i32 @String.length_of(i8* %0) {
entry:
  %text = alloca i8*, align 8
  %n = alloca i32, align 4
  store i8* %0, i8** %text, align 8
  store i32 0, i32* %n, align 4
  br label %while8

while8:                                           ; preds = %while8.then, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$n" = load i32, i32* %n, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$n"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while8.then, label %endloop

while8.then:                                      ; preds = %while8
  %"$n1" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n1", 1
  store i32 %ADD, i32* %n, align 4
  br label %while8

endloop:                                          ; preds = %while8
  %"$n2" = load i32, i32* %n, align 4
  ret i32 %"$n2"
}

define void @String.join(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  store i32 0, i32* %i, align 4
  br label %while9

while9:                                           ; preds = %while9.then, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$i" = load i32, i32* %i, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$i"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while9.then, label %endloop

while9.then:                                      ; preds = %while9
  %"*self" = load %String*, %String** %self, align 8
  %"$text1" = load i8*, i8** %text, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"text[]3" = getelementptr inbounds i8, i8* %"$text1", i32 %"$i2"
  %"$text[]4" = load i8, i8* %"text[]3", align 1
  call void @String.append(%String* %"*self", i8 %"$text[]4")
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while9

endloop:                                          ; preds = %while9
  ret void
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

define i32 @String.to_int(%String* %0) {
entry:
  %self = alloca %String*, align 8
  %i = alloca i32, align 4
  %sign = alloca i32, align 4
  %n = alloca i32, align 4
  %c = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store i32 0, i32* %i, align 4
  store i32 1, i32* %sign, align 4
  br label %if10

if10:                                             ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 0
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  %EQ = icmp eq i8 %"$*self.data[]", 45
  %AND = and i1 %GT, %EQ
  br i1 %AND, label %if10.then, label %endif

if10.then:                                        ; preds = %if10
  store i32 -1, i32* %sign, align 4
  store i32 1, i32* %i, align 4
  br label %endif

endif:                                            ; preds = %if10.then, %if10
  store i32 0, i32* %n, align 4
  br label %while11

while11:                                          ; preds = %endif1, %endif
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count5"
  br i1 %LT, label %while11.then, label %endloop

while11.then:                                     ; preds = %while11
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  %"$i9" = load i32, i32* %i, align 4
  %"*self.data[]10" = getelementptr inbounds i8, i8* %"$*self.data8", i32 %"$i9"
  %"$*self.data[]11" = load i8, i8* %"*self.data[]10", align 1
  store i8 %"$*self.data[]11", i8* %c, align 1
  br label %if12

if12:                                             ; preds = %while11.then
  %"$c" = load i8, i8* %c, align 1
  %LT12 = icmp slt i8 %"$c", 48
  %"$c13" = load i8, i8* %c, align 1
  %GT14 = icmp sgt i8 %"$c13", 57
  %OR = or i1 %LT12, %GT14
  br i1 %OR, label %if12.then, label %endif1

if12.then:                                        ; preds = %if12
  br label %endloop

endif1:                                           ; preds = %if12
  %"$n" = load i32, i32* %n, align 4
  %MUL = mul i32 %"$n", 10
  %"$c15" = load i8, i8* %c, align 1
  %AS = sext i8 %"$c15" to i32
  %SUB = sub i32 %AS, 48
  %ADD = add i32 %MUL, %SUB
  store i32 %ADD, i32* %n, align 4
  %"$i16" = load i32, i32* %i, align 4
  %ADD17 = add i32 %"$i16", 1
  store i32 %ADD17, i32* %i, align 4
  br label %while11

endloop:                                          ; preds = %if12.then, %while11
  %"$n18" = load i32, i32* %n, align 4
  %"$sign" = load i32, i32* %sign, align 4
  %MUL19 = mul i32 %"$n18", %"$sign"
  ret i32 %MUL19
}

define i1 @String.matches_at(%String* %0, i32 %1, i8* %2) {
entry:
  %self = alloca %String*, align 8
  %index = alloca i32, align 4
  %text = alloca i8*, align 8
  %j = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %index, align 4
  store i8* %2, i8** %text, align 8
  store i32 0, i32* %j, align 4
  br label %while13

while13:                                          ; preds = %endif1, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$j" = load i32, i32* %j, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$j"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while13.then, label %endloop

while13.then:                                     ; preds = %while13
  br label %if14

if14:                                             ; preds = %while13.then
  %"$index" = load i32, i32* %index, align 4
  %"$j2" = load i32, i32* %j, align 4
  %ADD = add i32 %"$index", %"$j2"
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GE = icmp sge i32 %ADD, %"$*self.count"
  br i1 %GE, label %if14.then, label %endif

if14.then:                                        ; preds = %if14
  ret i1 false

endif:                                            ; preds = %if14
  br label %if15

if15:                                             ; preds = %endif
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 0
  %"$index4" = load i32, i32* %index, align 4
  %"$j5" = load i32, i32* %j, align 4
  %ADD6 = add i32 %"$index4", %"$j5"
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %ADD6
  %"$text7" = load i8*, i8** %text, align 8
  %"$j8" = load i32, i32* %j, align 4
  %"text[]9" = getelementptr inbounds i8, i8* %"$text7", i32 %"$j8"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  %"$text[]10" = load i8, i8* %"text[]9", align 1
  %NQ11 = icmp ne i8 %"$*self.data[]", %"$text[]10"
  br i1 %NQ11, label %if15.then, label %endif1

if15.then:                                        ; preds = %if15
  ret i1 false

endif1:                                           ; preds = %if15
  %"$j12" = load i32, i32* %j, align 4
  %ADD13 = add i32 %"$j12", 1
  store i32 %ADD13, i32* %j, align 4
  br label %while13

endloop:                                          ; preds = %while13
  ret i1 true
}

define i32 @String.find(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  store i32 0, i32* %i, align 4
  br label %while16

while16:                                          ; preds = %endif, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count"
  br i1 %LT, label %while16.then, label %endloop

while16.then:                                     ; preds = %while16
  br label %if17

if17:                                             ; preds = %while16.then
  %"*self1" = load %String*, %String** %self, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"$text" = load i8*, i8** %text, align 8
  %call.String.matches_at = call i1 @String.matches_at(%String* %"*self1", i32 %"$i2", i8* %"$text")
  br i1 %call.String.matches_at, label %if17.then, label %endif

if17.then:                                        ; preds = %if17
  %"$i3" = load i32, i32* %i, align 4
  ret i32 %"$i3"

endif:                                            ; preds = %if17
  %"$i4" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i4", 1
  store i32 %ADD, i32* %i, align 4
  br label %while16

endloop:                                          ; preds = %while16
  ret i32 -1
}

define i1 @String.contains(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"$text" = load i8*, i8** %text, align 8
  %call.String.find = call i32 @String.find(%String* %"*self", i8* %"$text")
  %NQ = icmp ne i32 %call.String.find, -1
  ret i1 %NQ
}

define i1 @String.starts_with(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"$text" = load i8*, i8** %text, align 8
  %call.String.matches_at = call i1 @String.matches_at(%String* %"*self", i32 0, i8* %"$text")
  ret i1 %call.String.matches_at
}

define i1 @String.ends_with(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  %n = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  %"$text" = load i8*, i8** %text, align 8
  %call.String.length_of = call i32 @String.length_of(i8* %"$text")
  store i32 %call.String.length_of, i32* %n, align 4
  br label %if18

if18:                                             ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$n" = load i32, i32* %n, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$n", %"$*self.count"
  br i1 %GT, label %if18.then, label %endif

if18.then:                                        ; preds = %if18
  ret i1 false

endif:                                            ; preds = %if18
  %"*self1" = load %String*, %String** %self, align 8
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count4" = load i32, i32* %"*self.count3", align 4
  %"$n5" = load i32, i32* %n, align 4
  %SUB = sub i32 %"$*self.count4", %"$n5"
  %"$text6" = load i8*, i8** %text, align 8
  %call.String.matches_at = call i1 @String.matches_at(%String* %"*self1", i32 %SUB, i8* %"$text6")
  ret i1 %call.String.matches_at
}

define %String @String.substr(%String* %0, i32 %1, i32 %2) {
entry:
  %self = alloca %String*, align 8
  %start = alloca i32, align 4
  %length = alloca i32, align 4
  %res = alloca %String, align 8
  %stop = alloca i32, align 4
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %start, align 4
  store i32 %2, i32* %length, align 4
  store %String zeroinitializer, %String* %res, align 8
  br label %if19

if19:                                             ; preds = %entry
  %"$start" = load i32, i32* %start, align 4
  %LT = icmp slt i32 %"$start", 0
  br i1 %LT, label %if19.then, label %endif

if19.then:                                        ; preds = %if19
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"

endif:                                            ; preds = %if19
  %"$start2" = load i32, i32* %start, align 4
  %"$length" = load i32, i32* %length, align 4
  %ADD = add i32 %"$start2", %"$length"
  store i32 %ADD, i32* %stop, align 4
  br label %if20

if20:                                             ; preds = %endif
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$stop" = load i32, i32* %stop, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$stop", %"$*self.count"
  br i1 %GT, label %if20.then, label %endif1

if20.then:                                        ; preds = %if20
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %stop, align 4
  br label %endif1

endif1:                                           ; preds = %if20.then, %if20
  %"$start6" = load i32, i32* %start, align 4
  store i32 %"$start6", i32* %i, align 4
  br label %while21

while21:                                          ; preds = %while21.then, %endif1
  %"$i" = load i32, i32* %i, align 4
  %"$stop7" = load i32, i32* %stop, align 4
  %LT8 = icmp slt i32 %"$i", %"$stop7"
  br i1 %LT8, label %while21.then, label %endloop

while21.then:                                     ; preds = %while21
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i10" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i10"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  call void @String.append(%String* %res, i8 %"$*self.data[]")
  %"$i11" = load i32, i32* %i, align 4
  %ADD12 = add i32 %"$i11", 1
  store i32 %ADD12, i32* %i, align 4
  br label %while21

endloop:                                          ; preds = %while21
  %"$res13" = load %String, %String* %res, align 8
  ret %String %"$res13"
}

define %String @String.upper(%String* %0) {
entry:
  %self = alloca %String*, align 8
  %res = alloca %String, align 8
  %i = alloca i32, align 4
  %c = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store %String zeroinitializer, %String* %res, align 8
  store i32 0, i32* %i, align 4
  br label %while22

while22:                                          ; preds = %endif, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count"
  br i1 %LT, label %while22.then, label %endloop

while22.then:                                     ; preds = %while22
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i2" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i2"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  store i8 %"$*self.data[]", i8* %c, align 1
  br label %if23

if23:                                             ; preds = %while22.then
  %"$c" = load i8, i8* %c, align 1
  %GE = icmp sge i8 %"$c", 97
  %"$c3" = load i8, i8* %c, align 1
  %LE = icmp sle i8 %"$c3", 122
  %AND = and i1 %GE, %LE
  br i1 %AND, label %if23.then, label %endif

if23.then:                                        ; preds = %if23
  %"$c4" = load i8, i8* %c, align 1
  %AS = sext i8 %"$c4" to i32
  %SUB = sub i32 %AS, 32
  %AS5 = trunc i32 %SUB to i8
  store i8 %AS5, i8* %c, align 1
  br label %endif

endif:                                            ; preds = %if23.then, %if23
  %"$c6" = load i8, i8* %c, align 1
  call void @String.append(%String* %res, i8 %"$c6")
  %"$i7" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i7", 1
  store i32 %ADD, i32* %i, align 4
  br label %while22

endloop:                                          ; preds = %while22
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define %String @String.lower(%String* %0) {
entry:
  %self = alloca %String*, align 8
  %res = alloca %String, align 8
  %i = alloca i32, align 4
  %c = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store %String zeroinitializer, %String* %res, align 8
  store i32 0, i32* %i, align 4
  br label %while24

while24:                                          ; preds = %endif, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count"
  br i1 %LT, label %while24.then, label %endloop

while24.then:                                     ; preds = %while24
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i2" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i2"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  store i8 %"$*self.data[]", i8* %c, align 1
  br label %if25

if25:                                             ; preds = %while24.then
  %"$c" = load i8, i8* %c, align 1
  %GE = icmp sge i8 %"$c", 65
  %"$c3" = load i8, i8* %c, align 1
  %LE = icmp sle i8 %"$c3", 90
  %AND = and i1 %GE, %LE
  br i1 %AND, label %if25.then, label %endif

if25.then:                                        ; preds = %if25
  %"$c4" = load i8, i8* %c, align 1
  %AS = sext i8 %"$c4" to i32
  %ADD = add i32 %AS, 32
  %AS5 = trunc i32 %ADD to i8
  store i8 %AS5, i8* %c, align 1
  br label %endif

endif:                                            ; preds = %if25.then, %if25
  %"$c6" = load i8, i8* %c, align 1
  call void @String.append(%String* %res, i8 %"$c6")
  %"$i7" = load i32, i32* %i, align 4
  %ADD8 = add i32 %"$i7", 1
  store i32 %ADD8, i32* %i, align 4
  br label %while24

endloop:                                          ; preds = %while24
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define i1 @String.is_space(%String* %0, i8 %1) {
entry:
  %self = alloca %String*, align 8
  %c = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store i8 %1, i8* %c, align 1
  %"$c" = load i8, i8* %c, align 1
  %EQ = icmp eq i8 %"$c", 32
  %"$c1" = load i8, i8* %c, align 1
  %EQ2 = icmp eq i8 %"$c1", 9
  %OR = or i1 %EQ, %EQ2
  %"$c3" = load i8, i8* %c, align 1
  %EQ4 = icmp eq i8 %"$c3", 10
  %OR5 = or i1 %OR, %EQ4
  %"$c6" = load i8, i8* %c, align 1
  %EQ7 = icmp eq i8 %"$c6", 13
  %OR8 = or i1 %OR5, %EQ7
  ret i1 %OR8
}

define %String @String.trim(%String* %0) {
entry:
  %self = alloca %String*, align 8
  %start = alloca i32, align 4
  %stop = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i32 0, i32* %start, align 4
  br label %while26

while26:                                          ; preds = %endif, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$start" = load i32, i32* %start, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$start", %"$*self.count"
  br i1 %LT, label %while26.then, label %endloop

while26.then:                                     ; preds = %while26
  br label %if27

if27:                                             ; preds = %while26.then
  %"*self3" = load %String*, %String** %self, align 8
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$start5" = load i32, i32* %start, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$start5"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  %call.String.is_space = call i1 @String.is_space(%String* %"*self3", i8 %"$*self.data[]")
  %NOT = xor i1 %call.String.is_space, true
  br i1 %NOT, label %if27.then, label %endif

if27.then:                                        ; preds = %if27
  br label %endloop

endif:                                            ; preds = %if27
  %"$start6" = load i32, i32* %start, align 4
  %ADD = add i32 %"$start6", 1
  store i32 %ADD, i32* %start, align 4
  br label %while26

endloop:                                          ; preds = %if27.then, %while26
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.count8" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 1
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  store i32 %"$*self.count9", i32* %stop, align 4
  br label %while28

while28:                                          ; preds = %endif1, %endloop
  %"$stop" = load i32, i32* %stop, align 4
  %"$start10" = load i32, i32* %start, align 4
  %GT = icmp sgt i32 %"$stop", %"$start10"
  br i1 %GT, label %while28.then, label %endloop2

while28.then:                                     ; preds = %while28
  br label %if29

if29:                                             ; preds = %while28.then
  %"*self11" = load %String*, %String** %self, align 8
  %"*self12" = load %String*, %String** %self, align 8
  %"*self.data13" = getelementptr inbounds %String, %String* %"*self12", i32 0, i32 0
  %"$stop14" = load i32, i32* %stop, align 4
  %SUB = sub i32 %"$stop14", 1
  %"$*self.data15" = load i8*, i8** %"*self.data13", align 8
  %"*self.data[]16" = getelementptr inbounds i8, i8* %"$*self.data15", i32 %SUB
  %"$*self.data[]17" = load i8, i8* %"*self.data[]16", align 1
  %call.String.is_space18 = call i1 @String.is_space(%String* %"*self11", i8 %"$*self.data[]17")
  %NOT19 = xor i1 %call.String.is_space18, true
  br i1 %NOT19, label %if29.then, label %endif1

if29.then:                                        ; preds = %if29
  br label %endloop2

endif1:                                           ; preds = %if29
  %"$stop20" = load i32, i32* %stop, align 4
  %SUB21 = sub i32 %"$stop20", 1
  store i32 %SUB21, i32* %stop, align 4
  br label %while28

endloop2:                                         ; preds = %if29.then, %while28
  %"*self22" = load %String*, %String** %self, align 8
  %"$stop23" = load i32, i32* %stop, align 4
  %"$start24" = load i32, i32* %start, align 4
  %SUB25 = sub i32 %"$stop23", %"$start24"
  %"$start26" = load i32, i32* %start, align 4
  %call.String.substr = call %String @String.substr(%String* %"*self22", i32 %"$start26", i32 %SUB25)
  ret %String %call.String.substr
}

define %String @String.replace(%String* %0, i8* %1, i8* %2) {
entry:
  %self = alloca %String*, align 8
  %old = alloca i8*, align 8
  %fresh = alloca i8*, align 8
  %res = alloca %String, align 8
  %n = alloca i32, align 4
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %old, align 8
  store i8* %2, i8** %fresh, align 8
  store %String zeroinitializer, %String* %res, align 8
  %"$old" = load i8*, i8** %old, align 8
  %call.String.length_of = call i32 @String.length_of(i8* %"$old")
  store i32 %call.String.length_of, i32* %n, align 4
  store i32 0, i32* %i, align 4
  br label %while30

while30:                                          ; preds = %endif, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count"
  br i1 %LT, label %while30.then, label %endloop

while30.then:                                     ; preds = %while30
  br label %if31

if31:                                             ; preds = %while30.then
  %"$n" = load i32, i32* %n, align 4
  %GT = icmp sgt i32 %"$n", 0
  %"*self1" = load %String*, %String** %self, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"$old3" = load i8*, i8** %old, align 8
  %call.String.matches_at = call i1 @String.matches_at(%String* %"*self1", i32 %"$i2", i8* %"$old3")
  %AND = and i1 %GT, %call.String.matches_at
  br i1 %AND, label %if31.then, label %else32

if31.then:                                        ; preds = %if31
  %"$fresh" = load i8*, i8** %fresh, align 8
  call void @String.join(%String* %res, i8* %"$fresh")
  %"$n4" = load i32, i32* %n, align 4
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", %"$n4"
  store i32 %ADD, i32* %i, align 4
  br label %endif

else32:                                           ; preds = %if31
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i7" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i7"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  call void @String.append(%String* %res, i8 %"$*self.data[]")
  %"$i8" = load i32, i32* %i, align 4
  %ADD9 = add i32 %"$i8", 1
  store i32 %ADD9, i32* %i, align 4
  br label %endif

endif:                                            ; preds = %else32, %if31.then
  br label %while30

endloop:                                          ; preds = %while30
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define %String @String.repeat(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %times = alloca i32, align 4
  %res = alloca %String, align 8
  %k = alloca i32, align 4
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %times, align 4
  store %String zeroinitializer, %String* %res, align 8
  store i32 0, i32* %k, align 4
  br label %while33

while33:                                          ; preds = %endloop, %entry
  %"$k" = load i32, i32* %k, align 4
  %"$times" = load i32, i32* %times, align 4
  %LT = icmp slt i32 %"$k", %"$times"
  br i1 %LT, label %while33.then, label %endloop1

while33.then:                                     ; preds = %while33
  store i32 0, i32* %i, align 4
  br label %while34

while34:                                          ; preds = %while34.then, %while33.then
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT2 = icmp slt i32 %"$i", %"$*self.count"
  br i1 %LT2, label %while34.then, label %endloop

while34.then:                                     ; preds = %while34
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i4" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i4"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  call void @String.append(%String* %res, i8 %"$*self.data[]")
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while34

endloop:                                          ; preds = %while34
  %"$k6" = load i32, i32* %k, align 4
  %ADD7 = add i32 %"$k6", 1
  store i32 %ADD7, i32* %k, align 4
  br label %while33

endloop1:                                         ; preds = %while33
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define %String @String.reverse(%String* %0) {
entry:
  %self = alloca %String*, align 8
  %res = alloca %String, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store %String zeroinitializer, %String* %res, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  store i32 %SUB, i32* %i, align 4
  br label %while35

while35:                                          ; preds = %while35.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %GE = icmp sge i32 %"$i", 0
  br i1 %GE, label %while35.then, label %endloop

while35.then:                                     ; preds = %while35
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i2" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i2"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  call void @String.append(%String* %res, i8 %"$*self.data[]")
  %"$i3" = load i32, i32* %i, align 4
  %SUB4 = sub i32 %"$i3", 1
  store i32 %SUB4, i32* %i, align 4
  br label %while35

endloop:                                          ; preds = %while35
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define i32 @String.compare(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  %other = alloca %String*, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store %String* %1, %String** %other, align 8
  store i32 0, i32* %i, align 4
  br label %while36

while36:                                          ; preds = %endif, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count"
  %"*other" = load %String*, %String** %other, align 8
  %"*other.count" = getelementptr inbounds %String, %String* %"*other", i32 0, i32 1
  %"$i1" = load i32, i32* %i, align 4
  %"$*other.count" = load i32, i32* %"*other.count", align 4
  %LT2 = icmp slt i32 %"$i1", %"$*other.count"
  %AND = and i1 %LT, %LT2
  br i1 %AND, label %while36.then, label %endloop

while36.then:                                     ; preds = %while36
  br label %if37

if37:                                             ; preds = %while36.then
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i4" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i4"
  %"*other5" = load %String*, %String** %other, align 8
  %"*other.data" = getelementptr inbounds %String, %String* %"*other5", i32 0, i32 0
  %"$*other.data" = load i8*, i8** %"*other.data", align 8
  %"$i6" = load i32, i32* %i, align 4
  %"*other.data[]" = getelementptr inbounds i8, i8* %"$*other.data", i32 %"$i6"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  %"$*other.data[]" = load i8, i8* %"*other.data[]", align 1
  %NQ = icmp ne i8 %"$*self.data[]", %"$*other.data[]"
  br i1 %NQ, label %if37.then, label %endif

if37.then:                                        ; preds = %if37
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data8" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"$*self.data9" = load i8*, i8** %"*self.data8", align 8
  %"$i10" = load i32, i32* %i, align 4
  %"*self.data[]11" = getelementptr inbounds i8, i8* %"$*self.data9", i32 %"$i10"
  %"$*self.data[]12" = load i8, i8* %"*self.data[]11", align 1
  %AS = sext i8 %"$*self.data[]12" to i32
  %"*other13" = load %String*, %String** %other, align 8
  %"*other.data14" = getelementptr inbounds %String, %String* %"*other13", i32 0, i32 0
  %"$*other.data15" = load i8*, i8** %"*other.data14", align 8
  %"$i16" = load i32, i32* %i, align 4
  %"*other.data[]17" = getelementptr inbounds i8, i8* %"$*other.data15", i32 %"$i16"
  %"$*other.data[]18" = load i8, i8* %"*other.data[]17", align 1
  %AS19 = sext i8 %"$*other.data[]18" to i32
  %SUB = sub i32 %AS, %AS19
  ret i32 %SUB

endif:                                            ; preds = %if37
  %"$i20" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i20", 1
  store i32 %ADD, i32* %i, align 4
  br label %while36

endloop:                                          ; preds = %while36
  %"*self21" = load %String*, %String** %self, align 8
  %"*self.count22" = getelementptr inbounds %String, %String* %"*self21", i32 0, i32 1
  %"*other23" = load %String*, %String** %other, align 8
  %"*other.count24" = getelementptr inbounds %String, %String* %"*other23", i32 0, i32 1
  %"$*self.count25" = load i32, i32* %"*self.count22", align 4
  %"$*other.count26" = load i32, i32* %"*other.count24", align 4
  %SUB27 = sub i32 %"$*self.count25", %"$*other.count26"
  ret i32 %SUB27
}

define i1 @"String.=="(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  %other = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  store %String* %1, %String** %other, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*other" = load %String*, %String** %other, align 8
  %call.String.compare = call i32 @String.compare(%String* %"*self", %String* %"*other")
  %EQ = icmp eq i32 %call.String.compare, 0
  ret i1 %EQ
}

define i1 @"String.!="(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  %other = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  store %String* %1, %String** %other, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*other" = load %String*, %String** %other, align 8
  %call.String.compare = call i32 @String.compare(%String* %"*self", %String* %"*other")
  %NQ = icmp ne i32 %call.String.compare, 0
  ret i1 %NQ
}

define i1 @"String.<"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  %other = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  store %String* %1, %String** %other, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*other" = load %String*, %String** %other, align 8
  %call.String.compare = call i32 @String.compare(%String* %"*self", %String* %"*other")
  %LT = icmp slt i32 %call.String.compare, 0
  ret i1 %LT
}

define void @"String.+="(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"$text" = load i8*, i8** %text, align 8
  call void @String.join(%String* %"*self", i8* %"$text")
  ret void
}

define void @"String.+=.1"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  %other = alloca %String*, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store %String* %1, %String** %other, align 8
  store i32 0, i32* %i, align 4
  br label %while38

while38:                                          ; preds = %while38.then, %entry
  %"*other" = load %String*, %String** %other, align 8
  %"*other.count" = getelementptr inbounds %String, %String* %"*other", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*other.count" = load i32, i32* %"*other.count", align 4
  %LT = icmp slt i32 %"$i", %"$*other.count"
  br i1 %LT, label %while38.then, label %endloop

while38.then:                                     ; preds = %while38
  %"*self" = load %String*, %String** %self, align 8
  %"*other1" = load %String*, %String** %other, align 8
  %"*other.data" = getelementptr inbounds %String, %String* %"*other1", i32 0, i32 0
  %"$*other.data" = load i8*, i8** %"*other.data", align 8
  %"$i2" = load i32, i32* %i, align 4
  %"*other.data[]" = getelementptr inbounds i8, i8* %"$*other.data", i32 %"$i2"
  %"$*other.data[]" = load i8, i8* %"*other.data[]", align 1
  call void @String.append(%String* %"*self", i8 %"$*other.data[]")
  %"$i3" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i3", 1
  store i32 %ADD, i32* %i, align 4
  br label %while38

endloop:                                          ; preds = %while38
  ret void
}

define %String @"String.+"(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  %res = alloca %String, align 8
  %copy = alloca %String, align 8
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  store %String zeroinitializer, %String* %copy, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @"String.="(%String* %copy, %String* %"*self")
  %"$copy" = load %String, %String* %copy, align 8
  store %String %"$copy", %String* %res, align 8
  %"$text" = load i8*, i8** %text, align 8
  call void @String.join(%String* %res, i8* %"$text")
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define %String @"String.+.2"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  %other = alloca %String*, align 8
  %res = alloca %String, align 8
  %copy = alloca %String, align 8
  store %String* %0, %String** %self, align 8
  store %String* %1, %String** %other, align 8
  store %String zeroinitializer, %String* %copy, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @"String.="(%String* %copy, %String* %"*self")
  %"$copy" = load %String, %String* %copy, align 8
  store %String %"$copy", %String* %res, align 8
  %"*other" = load %String*, %String** %other, align 8
  call void @"String.+=.1"(%String* %res, %String* %"*other")
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define i32 @main() {
entry:
  %s = alloca %String, align 8
  %t = alloca %String, align 8
  %tmp = alloca %String, align 8
  %tmp7 = alloca %String, align 8
  %tmp9 = alloca %String, align 8
  %bool.text = alloca i8*, align 8
  %bool.text13 = alloca i8*, align 8
  %bool.text14 = alloca i8*, align 8
  %r = alloca %String, align 8
  %tmp19 = alloca %String, align 8
  %tmp21 = alloca %String, align 8
  %a = alloca %String, align 8
  %b = alloca %String, align 8
  %c = alloca %String, align 8
  %tmp24 = alloca %String, align 8
  %tmp27 = alloca %String, align 8
  %bool.text29 = alloca i8*, align 8
  %bool.text30 = alloca i8*, align 8
  %bool.text31 = alloca i8*, align 8
  %n = alloca %String, align 8
  store %String { i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str, i32 0, i32 0), i32 15, i32 0 }, %String* %s, align 8
  %call.String.trim = call %String @String.trim(%String* %s)
  store %String %call.String.trim, %String* %t, align 8
  %call.String.chars = call i8* @String.chars(%String* %t)
  %call.String.size = call i32 @String.size(%String* %t)
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.3, i32 0, i32 0), i8* %call.String.chars, i32 %call.String.size)
  %call.String.upper = call %String @String.upper(%String* %t)
  store %String %call.String.upper, %String* %tmp, align 8
  %call.String.chars6 = call i8* @String.chars(%String* %tmp)
  %call.String.lower = call %String @String.lower(%String* %t)
  store %String %call.String.lower, %String* %tmp7, align 8
  %call.String.chars8 = call i8* @String.chars(%String* %tmp7)
  %call.String.reverse = call %String @String.reverse(%String* %t)
  store %String %call.String.reverse, %String* %tmp9, align 8
  %call.String.chars10 = call i8* @String.chars(%String* %tmp9)
  %call.dprintf11 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.4, i32 0, i32 0), i8* %call.String.chars6, i8* %call.String.chars8, i8* %call.String.chars10)
  call void @String.drop(%String* %tmp9)
  call void @String.drop(%String* %tmp7)
  call void @String.drop(%String* %tmp)
  %call.String.find = call i32 @String.find(%String* %t, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.5, i32 0, i32 0))
  %call.String.contains = call i1 @String.contains(%String* %t, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.6, i32 0, i32 0))
  %call.String.starts_with = call i1 @String.starts_with(%String* %t, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.7, i32 0, i32 0))
  %call.String.ends_with = call i1 @String.ends_with(%String* %t, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.8, i32 0, i32 0))
  %call.String.find12 = call i32 @String.find(%String* %t, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.9, i32 0, i32 0))
  store i8* null, i8** %bool.text, align 8
  br label %if39

if39:                                             ; preds = %entry
  br i1 %call.String.contains, label %if39.then, label %else40

if39.then:                                        ; preds = %if39
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.10, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else40:                                           ; preds = %if39
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.11, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else40, %if39.then
  store i8* null, i8** %bool.text13, align 8
  br label %if41

if41:                                             ; preds = %endif
  br i1 %call.String.starts_with, label %if41.then, label %else42

if41.then:                                        ; preds = %if41
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.12, i32 0, i32 0), i8** %bool.text13, align 8
  br label %endif1

else42:                                           ; preds = %if41
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.13, i32 0, i32 0), i8** %bool.text13, align 8
  br label %endif1

endif1:                                           ; preds = %else42, %if41.then
  store i8* null, i8** %bool.text14, align 8
  br label %if43

if43:                                             ; preds = %endif1
  br i1 %call.String.ends_with, label %if43.then, label %else44

if43.then:                                        ; preds = %if43
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.14, i32 0, i32 0), i8** %bool.text14, align 8
  br label %endif2

else44:                                           ; preds = %if43
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.15, i32 0, i32 0), i8** %bool.text14, align 8
  br label %endif2

endif2:                                           ; preds = %else44, %if43.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %"$bool.text15" = load i8*, i8** %bool.text13, align 8
  %"$bool.text16" = load i8*, i8** %bool.text14, align 8
  %call.dprintf17 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.16, i32 0, i32 0), i32 %call.String.find, i8* %"$bool.text", i8* %"$bool.text15", i8* %"$bool.text16", i32 %call.String.find12)
  %call.String.replace = call %String @String.replace(%String* %t, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.17, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.18, i32 0, i32 0))
  store %String %call.String.replace, %String* %r, align 8
  %call.String.chars18 = call i8* @String.chars(%String* %r)
  %call.String.substr = call %String @String.substr(%String* %r, i32 1, i32 3)
  store %String %call.String.substr, %String* %tmp19, align 8
  %call.String.chars20 = call i8* @String.chars(%String* %tmp19)
  %call.String.repeat = call %String @String.repeat(%String* %r, i32 2)
  store %String %call.String.repeat, %String* %tmp21, align 8
  %call.String.chars22 = call i8* @String.chars(%String* %tmp21)
  %call.dprintf23 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.19, i32 0, i32 0), i8* %call.String.chars18, i8* %call.String.chars20, i8* %call.String.chars22)
  call void @String.drop(%String* %tmp21)
  call void @String.drop(%String* %tmp19)
  %call.String.create = call %String @String.create(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.20, i32 0, i32 0))
  store %String %call.String.create, %String* %a, align 8
  store %String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.21, i32 0, i32 0), i32 3, i32 0 }, %String* %b, align 8
  %"call.String.+" = call %String @"String.+"(%String* %a, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.22, i32 0, i32 0))
  store %String %"call.String.+", %String* %tmp24, align 8
  %"call.String.+25" = call %String @"String.+.2"(%String* %tmp24, %String* %b)
  store %String %"call.String.+25", %String* %c, align 8
  call void @String.drop(%String* %tmp24)
  call void @"String.+="(%String* %c, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.23, i32 0, i32 0))
  %call.String.chars26 = call i8* @String.chars(%String* %c)
  %"call.String.<" = call i1 @"String.<"(%String* %a, %String* %b)
  store %String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.24, i32 0, i32 0), i32 3, i32 0 }, %String* %tmp27, align 8
  %"call.String.==" = call i1 @"String.=="(%String* %a, %String* %tmp27)
  %"call.String.!=" = call i1 @"String.!="(%String* %a, %String* %b)
  %call.String.size28 = call i32 @String.size(%String* %c)
  store i8* null, i8** %bool.text29, align 8
  br label %if45

if45:                                             ; preds = %endif2
  br i1 %"call.String.<", label %if45.then, label %else46

if45.then:                                        ; preds = %if45
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.25, i32 0, i32 0), i8** %bool.text29, align 8
  br label %endif3

else46:                                           ; preds = %if45
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.26, i32 0, i32 0), i8** %bool.text29, align 8
  br label %endif3

endif3:                                           ; preds = %else46, %if45.then
  store i8* null, i8** %bool.text30, align 8
  br label %if47

if47:                                             ; preds = %endif3
  br i1 %"call.String.==", label %if47.then, label %else48

if47.then:                                        ; preds = %if47
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.27, i32 0, i32 0), i8** %bool.text30, align 8
  br label %endif4

else48:                                           ; preds = %if47
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.28, i32 0, i32 0), i8** %bool.text30, align 8
  br label %endif4

endif4:                                           ; preds = %else48, %if47.then
  store i8* null, i8** %bool.text31, align 8
  br label %if49

if49:                                             ; preds = %endif4
  br i1 %"call.String.!=", label %if49.then, label %else50

if49.then:                                        ; preds = %if49
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.29, i32 0, i32 0), i8** %bool.text31, align 8
  br label %endif5

else50:                                           ; preds = %if49
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.30, i32 0, i32 0), i8** %bool.text31, align 8
  br label %endif5

endif5:                                           ; preds = %else50, %if49.then
  %"$bool.text32" = load i8*, i8** %bool.text29, align 8
  %"$bool.text33" = load i8*, i8** %bool.text30, align 8
  %"$bool.text34" = load i8*, i8** %bool.text31, align 8
  %call.dprintf35 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.31, i32 0, i32 0), i8* %call.String.chars26, i8* %"$bool.text32", i8* %"$bool.text33", i8* %"$bool.text34", i32 %call.String.size28)
  call void @String.drop(%String* %tmp27)
  store %String { i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.32, i32 0, i32 0), i32 2, i32 0 }, %String* %n, align 8
  %call.String.to_int = call i32 @String.to_int(%String* %n)
  %ADD = add i32 %call.String.to_int, 1
  %call.dprintf36 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.33, i32 0, i32 0), i32 %ADD, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @TYPEOF, i32 0, i32 0))
  call void @String.drop(%String* %n)
  call void @String.drop(%String* %c)
  call void @String.drop(%String* %b)
  call void @String.drop(%String* %a)
  call void @String.drop(%String* %r)
  call void @String.drop(%String* %t)
  call void @String.drop(%String* %s)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 039 — struct: a plain fn drop() is a normal method, only operator drop runs by itself

```ura
struct Plain:
    id i32

    fn drop():
        output("plain drop ", self.id, "\n")

struct Owned:
    id i32

    operator drop():
        output("owned drop ", self.id, "\n")

fn main() i32:
    p Plain
    p.id = 1
    o Owned
    o.id = 2
    p.drop()
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Plain = type { i32 }
%Owned = type { i32 }

@str = private unnamed_addr constant [15 x i8] c"plain drop %d\0A\00", align 1
@str.1 = private unnamed_addr constant [15 x i8] c"owned drop %d\0A\00", align 1

define void @Plain.drop(%Plain* %0) {
entry:
  %self = alloca %Plain*, align 8
  store %Plain* %0, %Plain** %self, align 8
  %"*self" = load %Plain*, %Plain** %self, align 8
  %"*self.id" = getelementptr inbounds %Plain, %Plain* %"*self", i32 0, i32 0
  %"$*self.id" = load i32, i32* %"*self.id", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str, i32 0, i32 0), i32 %"$*self.id")
  ret void
}

define void @Owned.drop(%Owned* %0) {
entry:
  %self = alloca %Owned*, align 8
  store %Owned* %0, %Owned** %self, align 8
  %"*self" = load %Owned*, %Owned** %self, align 8
  %"*self.id" = getelementptr inbounds %Owned, %Owned* %"*self", i32 0, i32 0
  %"$*self.id" = load i32, i32* %"*self.id", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.1, i32 0, i32 0), i32 %"$*self.id")
  ret void
}

define i32 @main() {
entry:
  %p = alloca %Plain, align 8
  %o = alloca %Owned, align 8
  store %Plain zeroinitializer, %Plain* %p, align 4
  %p.id = getelementptr inbounds %Plain, %Plain* %p, i32 0, i32 0
  store i32 1, i32* %p.id, align 4
  store %Owned zeroinitializer, %Owned* %o, align 4
  %o.id = getelementptr inbounds %Owned, %Owned* %o, i32 0, i32 0
  store i32 2, i32* %o.id, align 4
  call void @Plain.drop(%Plain* %p)
  call void @Owned.drop(%Owned* %o)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 040 — ownership: nested arrays, an Array attribute, reassignment, drop(x), returning an attribute copies

```ura
struct Bag:
    items Array<i32>
    label i32

    fn first_items() Array<i32>:
        return self.items

fn make(n i32) Array<i32>:
    a Array<i32>
    i i32 = 0
    while i < n:
        a.append(i)
        i += 1
    return a

fn total(a Array<i32>) i32:
    sum i32 = 0
    i i32 = 0
    while i < a.size():
        sum += a[i]
        i += 1
    return sum

fn main() i32:
    grid Array<Array<i32>>
    row Array<i32> = make(3)
    grid.append(row)
    grid.append(make(2))
    grid[0].append(9)
    copy Array<i32> = grid[0]
    copy.append(5)
    output(grid.size(), " ", grid[0].size(), " ", grid[1].size(), " ", copy.size(), " ", row.size(), "\n")
    bag Bag
    bag.items = make(4)
    bag.items = make(2)
    items Array<i32> = bag.first_items()
    items.append(7)
    output(bag.items.size(), " ", items.size(), " ", total(items), " ", total(grid[0]), "\n")
    popped Array<i32> = grid.pop()
    drop(row)
    output(popped.size(), " ", row.size(), " ", grid.size(), "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%"Array<i32>" = type { i32*, i32, i32 }
%Bag = type { %"Array<i32>", i32 }
%"Array<Array<i32>>" = type { %"Array<i32>"*, i32, i32 }

@str = private unnamed_addr constant [16 x i8] c"%d %d %d %d %d\0A\00", align 1
@str.2 = private unnamed_addr constant [13 x i8] c"%d %d %d %d\0A\00", align 1
@str.3 = private unnamed_addr constant [10 x i8] c"%d %d %d\0A\00", align 1

define i32* @"new<i32>"(i32 %0) {
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

define void @"Array<i32>.reserve"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i32*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self6" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  ret void

endif1:                                           ; preds = %if1
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i32>" = call i32* @"new<i32>"(i32 %"$cap8")
  store i32* %"call.new<i32>", i32** %bigger, align 8
  %"*self9" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self9", i32 0, i32 0
  %"*self10" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self10", i32 0, i32 1
  %"$bigger" = load i32*, i32** %bigger, align 8
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i32>"(i32* %"$bigger", i32* %"$*self.data", i32 %"$*self.count12")
  br label %if2

if2:                                              ; preds = %endif1
  %"*self13" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap14" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*self16" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data17" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i32*, i32** %"*self.data17", align 8
  call void @"free<i32>"(i32* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"*self19" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data20" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self19", i32 0, i32 0
  %"$bigger21" = load i32*, i32** %bigger, align 8
  store i32* %"$bigger21", i32** %"*self.data20", align 8
  %"*self22" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap23" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @"Array<i32>.own"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %AND = and i1 %EQ, %GT
  br i1 %AND, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self2", i32 %"$*self.count5")
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  ret void
}

define void @"Array<i32>.clear"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  br label %if4

if4:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if4.then, label %endif

if4.then:                                         ; preds = %if4
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if4.then, %if4
  br label %while5

while5:                                           ; preds = %while5.then, %endif
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while5.then, label %endloop

while5.then:                                      ; preds = %while5
  %"*self4" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$*self.count10"
  store i32 0, i32* %"*self.data[]", align 4
  br label %while5

endloop:                                          ; preds = %while5
  ret void
}

define void @"Array<i32>.append"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %value = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %value, align 4
  br label %if6

if6:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  %"*self6" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self6", i32 0, i32 0
  %"*self7" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count8" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self7", i32 0, i32 1
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$*self.count9"
  %"$value" = load i32, i32* %value, align 4
  store i32 %"$value", i32* %"*self.data[]", align 4
  %"*self10" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  ret void
}

define i32 @"Array<i32>.size"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i32* @"Array<i32>.[]"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %index = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %index, align 4
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  call void @"Array<i32>.own"(%"Array<i32>"* %"*self")
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$index"
  ret i32* %"*self.data[]"
}

define void @"Array<i32>.="(%"Array<i32>"* %0, %"Array<i32>"* %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %other = alloca %"Array<i32>"*, align 8
  %i = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store %"Array<i32>"* %1, %"Array<i32>"** %other, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  call void @"Array<i32>.clear"(%"Array<i32>"* %"*self")
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*other" = load %"Array<i32>"*, %"Array<i32>"** %other, align 8
  %"*other.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*other", i32 0, i32 1
  %"$*other.count" = load i32, i32* %"*other.count", align 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self1", i32 %"$*other.count")
  store i32 0, i32* %i, align 4
  br label %while7

while7:                                           ; preds = %while7.then, %entry
  %"*other2" = load %"Array<i32>"*, %"Array<i32>"** %other, align 8
  %"*other.count3" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*other2", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*other.count4" = load i32, i32* %"*other.count3", align 4
  %LT = icmp slt i32 %"$i", %"$*other.count4"
  br i1 %LT, label %while7.then, label %endloop

while7.then:                                      ; preds = %while7
  %"*self5" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self5", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$i6" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$i6"
  %"*other7" = load %"Array<i32>"*, %"Array<i32>"** %other, align 8
  %"*other.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*other7", i32 0, i32 0
  %"$*other.data" = load i32*, i32** %"*other.data", align 8
  %"$i8" = load i32, i32* %i, align 4
  %"*other.data[]" = getelementptr inbounds i32, i32* %"$*other.data", i32 %"$i8"
  %"$*other.data[]" = load i32, i32* %"*other.data[]", align 4
  store i32 %"$*other.data[]", i32* %"*self.data[]", align 4
  %"$i9" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i9", 1
  store i32 %ADD, i32* %i, align 4
  br label %while7

endloop:                                          ; preds = %while7
  %"*self10" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self10", i32 0, i32 1
  %"*other11" = load %"Array<i32>"*, %"Array<i32>"** %other, align 8
  %"*other.count12" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*other11", i32 0, i32 1
  %"$*other.count13" = load i32, i32* %"*other.count12", align 4
  store i32 %"$*other.count13", i32* %"*self.count", align 4
  ret void
}

define void @"Array<i32>.drop"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  call void @"Array<i32>.clear"(%"Array<i32>"* %"*self")
  br label %if8

if8:                                              ; preds = %entry
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if8.then, label %endif

if8.then:                                         ; preds = %if8
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  call void @"free<i32>"(i32* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if8.then, %if8
  ret void
}

define %"Array<i32>" @Bag.first_items(%Bag* %0) {
entry:
  %self = alloca %Bag*, align 8
  %copy = alloca %"Array<i32>", align 8
  store %Bag* %0, %Bag** %self, align 8
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %copy, align 8
  %"*self" = load %Bag*, %Bag** %self, align 8
  %"*self.items" = getelementptr inbounds %Bag, %Bag* %"*self", i32 0, i32 0
  call void @"Array<i32>.="(%"Array<i32>"* %copy, %"Array<i32>"* %"*self.items")
  %"$copy" = load %"Array<i32>", %"Array<i32>"* %copy, align 8
  ret %"Array<i32>" %"$copy"
}

define %"Array<i32>" @make(i32 %0) {
entry:
  %n = alloca i32, align 4
  %a = alloca %"Array<i32>", align 8
  %i = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %a, align 8
  store i32 0, i32* %i, align 4
  br label %while9

while9:                                           ; preds = %while9.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %"$n" = load i32, i32* %n, align 4
  %LT = icmp slt i32 %"$i", %"$n"
  br i1 %LT, label %while9.then, label %endloop

while9.then:                                      ; preds = %while9
  %"$i1" = load i32, i32* %i, align 4
  call void @"Array<i32>.append"(%"Array<i32>"* %a, i32 %"$i1")
  %"$i2" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i2", 1
  store i32 %ADD, i32* %i, align 4
  br label %while9

endloop:                                          ; preds = %while9
  %"$a" = load %"Array<i32>", %"Array<i32>"* %a, align 8
  ret %"Array<i32>" %"$a"
}

define i32 @total(%"Array<i32>" %0) {
entry:
  %a = alloca %"Array<i32>", align 8
  %sum = alloca i32, align 4
  %i = alloca i32, align 4
  %ref = alloca i32*, align 8
  store %"Array<i32>" %0, %"Array<i32>"* %a, align 8
  store i32 0, i32* %sum, align 4
  store i32 0, i32* %i, align 4
  br label %while10

while10:                                          ; preds = %while10.then, %entry
  %"call.Array<i32>.size" = call i32 @"Array<i32>.size"(%"Array<i32>"* %a)
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", %"call.Array<i32>.size"
  br i1 %LT, label %while10.then, label %endloop

while10.then:                                     ; preds = %while10
  %"$i1" = load i32, i32* %i, align 4
  %"call.Array<i32>.[]" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %a, i32 %"$i1")
  store i32* %"call.Array<i32>.[]", i32** %ref, align 8
  %"*ref" = load i32*, i32** %ref, align 8
  %"$*ref" = load i32, i32* %"*ref", align 4
  %"$sum" = load i32, i32* %sum, align 4
  %ADD = add i32 %"$sum", %"$*ref"
  store i32 %ADD, i32* %sum, align 4
  %"$i2" = load i32, i32* %i, align 4
  %ADD3 = add i32 %"$i2", 1
  store i32 %ADD3, i32* %i, align 4
  br label %while10

endloop:                                          ; preds = %while10
  %"$sum4" = load i32, i32* %sum, align 4
  call void @"Array<i32>.drop"(%"Array<i32>"* %a)
  ret i32 %"$sum4"
}

define %"Array<i32>"* @"new<Array<i32>>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (%"Array<i32>"* getelementptr (%"Array<i32>", %"Array<i32>"* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to %"Array<i32>"*
  ret %"Array<i32>"* %AS1
}

define void @"copy<Array<i32>>"(%"Array<i32>"* %0, %"Array<i32>"* %1, i32 %2) {
entry:
  %dst = alloca %"Array<i32>"*, align 8
  %src = alloca %"Array<i32>"*, align 8
  %count = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %dst, align 8
  store %"Array<i32>"* %1, %"Array<i32>"** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load %"Array<i32>"*, %"Array<i32>"** %dst, align 8
  %AS = bitcast %"Array<i32>"* %"$dst" to i8*
  %"$src" = load %"Array<i32>"*, %"Array<i32>"** %src, align 8
  %AS1 = bitcast %"Array<i32>"* %"$src" to i8*
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (%"Array<i32>"* getelementptr (%"Array<i32>", %"Array<i32>"* null, i32 1) to i32)
  %AS2 = sext i32 %MUL to i64
  %MUL3 = mul i64 %AS2, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %AS, i8* %AS1, i64 %MUL3)
  ret void
}

define void @"free<Array<i32>>"(%"Array<i32>"* %0) {
entry:
  %data = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %data, align 8
  %"$data" = load %"Array<i32>"*, %"Array<i32>"** %data, align 8
  %AS = bitcast %"Array<i32>"* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define void @"Array<Array<i32>>.reserve"(%"Array<Array<i32>>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca %"Array<i32>"*, align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if11

if11:                                             ; preds = %entry
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if11.then, label %endif

if11.then:                                        ; preds = %if11
  %"*self3" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if11.then, %if11
  br label %if12

if12:                                             ; preds = %endif
  %"*self6" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if12.then, label %endif1

if12.then:                                        ; preds = %if12
  ret void

endif1:                                           ; preds = %if12
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<Array<i32>>" = call %"Array<i32>"* @"new<Array<i32>>"(i32 %"$cap8")
  store %"Array<i32>"* %"call.new<Array<i32>>", %"Array<i32>"** %bigger, align 8
  %"*self9" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self9", i32 0, i32 0
  %"*self10" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self10", i32 0, i32 1
  %"$bigger" = load %"Array<i32>"*, %"Array<i32>"** %bigger, align 8
  %"$*self.data" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<Array<i32>>"(%"Array<i32>"* %"$bigger", %"Array<i32>"* %"$*self.data", i32 %"$*self.count12")
  br label %if13

if13:                                             ; preds = %endif1
  %"*self13" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.cap14" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if13.then, label %endif2

if13.then:                                        ; preds = %if13
  %"*self16" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data17" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self16", i32 0, i32 0
  %"$*self.data18" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data17", align 8
  call void @"free<Array<i32>>"(%"Array<i32>"* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if13.then, %if13
  %"*self19" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data20" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self19", i32 0, i32 0
  %"$bigger21" = load %"Array<i32>"*, %"Array<i32>"** %bigger, align 8
  store %"Array<i32>"* %"$bigger21", %"Array<i32>"** %"*self.data20", align 8
  %"*self22" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.cap23" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @"Array<Array<i32>>.own"(%"Array<Array<i32>>"* %0) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  br label %if14

if14:                                             ; preds = %entry
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  %"*self1" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %AND = and i1 %EQ, %GT
  br i1 %AND, label %if14.then, label %endif

if14.then:                                        ; preds = %if14
  %"*self2" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self3" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  call void @"Array<Array<i32>>.reserve"(%"Array<Array<i32>>"* %"*self2", i32 %"$*self.count5")
  br label %endif

endif:                                            ; preds = %if14.then, %if14
  ret void
}

define void @"Array<Array<i32>>.clear"(%"Array<Array<i32>>"* %0) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  br label %if15

if15:                                             ; preds = %entry
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if15.then, label %endif

if15.then:                                        ; preds = %if15
  %"*self1" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if15.then, %if15
  br label %while16

while16:                                          ; preds = %while16.then, %endif
  %"*self2" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while16.then, label %endloop

while16.then:                                     ; preds = %while16
  %"*self4" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"$*self.data", i32 %"$*self.count10"
  call void @"Array<i32>.drop"(%"Array<i32>"* %"*self.data[]")
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %"*self.data[]", align 8
  br label %while16

endloop:                                          ; preds = %while16
  ret void
}

define void @"Array<Array<i32>>.append"(%"Array<Array<i32>>"* %0, %"Array<i32>" %1) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  %value = alloca %"Array<i32>", align 8
  %copy = alloca %"Array<i32>", align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  store %"Array<i32>" %1, %"Array<i32>"* %value, align 8
  br label %if17

if17:                                             ; preds = %entry
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if17.then, label %endif

if17.then:                                        ; preds = %if17
  %"*self2" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self3" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<Array<i32>>.reserve"(%"Array<Array<i32>>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if17.then, %if17
  %"*self6" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self6", i32 0, i32 0
  %"*self7" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count8" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self7", i32 0, i32 1
  %"$*self.data" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"$*self.data", i32 %"$*self.count9"
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %copy, align 8
  call void @"Array<i32>.="(%"Array<i32>"* %copy, %"Array<i32>"* %value)
  %"$copy" = load %"Array<i32>", %"Array<i32>"* %copy, align 8
  call void @"Array<i32>.drop"(%"Array<i32>"* %"*self.data[]")
  store %"Array<i32>" %"$copy", %"Array<i32>"* %"*self.data[]", align 8
  %"*self10" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  call void @"Array<i32>.drop"(%"Array<i32>"* %value)
  ret void
}

define %"Array<i32>" @"Array<Array<i32>>.pop"(%"Array<Array<i32>>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  %index = alloca i32, align 4
  %value = alloca %"Array<i32>", align 8
  %copy = alloca %"Array<i32>", align 8
  %i = alloca i32, align 4
  %copy15 = alloca %"Array<i32>", align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  store i32 %1, i32* %index, align 4
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %value, align 8
  br label %if18

if18:                                             ; preds = %entry
  %"$index" = load i32, i32* %index, align 4
  %LT = icmp slt i32 %"$index", 0
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self", i32 0, i32 1
  %"$index1" = load i32, i32* %index, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GE = icmp sge i32 %"$index1", %"$*self.count"
  %OR = or i1 %LT, %GE
  br i1 %OR, label %if18.then, label %endif

if18.then:                                        ; preds = %if18
  %"$value" = load %"Array<i32>", %"Array<i32>"* %value, align 8
  ret %"Array<i32>" %"$value"

endif:                                            ; preds = %if18
  %"*self2" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  call void @"Array<Array<i32>>.own"(%"Array<Array<i32>>"* %"*self2")
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %copy, align 8
  %"*self3" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self3", i32 0, i32 0
  %"$*self.data" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data", align 8
  %"$index4" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"$*self.data", i32 %"$index4"
  call void @"Array<i32>.="(%"Array<i32>"* %copy, %"Array<i32>"* %"*self.data[]")
  %"$copy" = load %"Array<i32>", %"Array<i32>"* %copy, align 8
  call void @"Array<i32>.drop"(%"Array<i32>"* %value)
  store %"Array<i32>" %"$copy", %"Array<i32>"* %value, align 8
  %"$index5" = load i32, i32* %index, align 4
  store i32 %"$index5", i32* %i, align 4
  br label %while19

while19:                                          ; preds = %while19.then, %endif
  %"*self6" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count7" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self6", i32 0, i32 1
  %"$*self.count8" = load i32, i32* %"*self.count7", align 4
  %SUB = sub i32 %"$*self.count8", 1
  %"$i" = load i32, i32* %i, align 4
  %LT9 = icmp slt i32 %"$i", %SUB
  br i1 %LT9, label %while19.then, label %endloop

while19.then:                                     ; preds = %while19
  %"*self10" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data11" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self10", i32 0, i32 0
  %"$*self.data12" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data11", align 8
  %"$i13" = load i32, i32* %i, align 4
  %"*self.data[]14" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"$*self.data12", i32 %"$i13"
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %copy15, align 8
  %"*self16" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data17" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self16", i32 0, i32 0
  %"$i18" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i18", 1
  %"$*self.data19" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data17", align 8
  %"*self.data[]20" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"$*self.data19", i32 %ADD
  call void @"Array<i32>.="(%"Array<i32>"* %copy15, %"Array<i32>"* %"*self.data[]20")
  %"$copy21" = load %"Array<i32>", %"Array<i32>"* %copy15, align 8
  call void @"Array<i32>.drop"(%"Array<i32>"* %"*self.data[]14")
  store %"Array<i32>" %"$copy21", %"Array<i32>"* %"*self.data[]14", align 8
  %"$i22" = load i32, i32* %i, align 4
  %ADD23 = add i32 %"$i22", 1
  store i32 %ADD23, i32* %i, align 4
  br label %while19

endloop:                                          ; preds = %while19
  %"*self24" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count25" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self24", i32 0, i32 1
  %"$*self.count26" = load i32, i32* %"*self.count25", align 4
  %SUB27 = sub i32 %"$*self.count26", 1
  store i32 %SUB27, i32* %"*self.count25", align 4
  %"*self28" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data29" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self28", i32 0, i32 0
  %"*self30" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count31" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self30", i32 0, i32 1
  %"$*self.data32" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data29", align 8
  %"$*self.count33" = load i32, i32* %"*self.count31", align 4
  %"*self.data[]34" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"$*self.data32", i32 %"$*self.count33"
  call void @"Array<i32>.drop"(%"Array<i32>"* %"*self.data[]34")
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %"*self.data[]34", align 8
  %"$value35" = load %"Array<i32>", %"Array<i32>"* %value, align 8
  ret %"Array<i32>" %"$value35"
}

define %"Array<i32>" @"Array<Array<i32>>.pop.1"(%"Array<Array<i32>>"* %0) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self1" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  %"call.Array<Array<i32>>.pop" = call %"Array<i32>" @"Array<Array<i32>>.pop"(%"Array<Array<i32>>"* %"*self", i32 %SUB)
  ret %"Array<i32>" %"call.Array<Array<i32>>.pop"
}

define i32 @"Array<Array<i32>>.size"(%"Array<Array<i32>>"* %0) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define %"Array<i32>"* @"Array<Array<i32>>.[]"(%"Array<Array<i32>>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  %index = alloca i32, align 4
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  store i32 %1, i32* %index, align 4
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  call void @"Array<Array<i32>>.own"(%"Array<Array<i32>>"* %"*self")
  %"*self1" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self1", i32 0, i32 0
  %"$*self.data" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"$*self.data", i32 %"$index"
  ret %"Array<i32>"* %"*self.data[]"
}

define void @"Array<Array<i32>>.drop"(%"Array<Array<i32>>"* %0) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  call void @"Array<Array<i32>>.clear"(%"Array<Array<i32>>"* %"*self")
  br label %if20

if20:                                             ; preds = %entry
  %"*self1" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if20.then, label %endif

if20.then:                                        ; preds = %if20
  %"*self2" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self2", i32 0, i32 0
  %"$*self.data" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data", align 8
  call void @"free<Array<i32>>"(%"Array<i32>"* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if20.then, %if20
  ret void
}

define i32 @main() {
entry:
  %grid = alloca %"Array<Array<i32>>", align 8
  %row = alloca %"Array<i32>", align 8
  %copy = alloca %"Array<i32>", align 8
  %ref = alloca %"Array<i32>"*, align 8
  %copy2 = alloca %"Array<i32>", align 8
  %copy3 = alloca %"Array<i32>", align 8
  %ref5 = alloca %"Array<i32>"*, align 8
  %ref9 = alloca %"Array<i32>"*, align 8
  %ref12 = alloca %"Array<i32>"*, align 8
  %bag = alloca %Bag, align 8
  %items = alloca %"Array<i32>", align 8
  %copy23 = alloca %"Array<i32>", align 8
  %copy25 = alloca %"Array<i32>", align 8
  %ref27 = alloca %"Array<i32>"*, align 8
  %popped = alloca %"Array<i32>", align 8
  store %"Array<Array<i32>>" zeroinitializer, %"Array<Array<i32>>"* %grid, align 8
  %call.make = call %"Array<i32>" @make(i32 3)
  store %"Array<i32>" %call.make, %"Array<i32>"* %row, align 8
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %copy, align 8
  call void @"Array<i32>.="(%"Array<i32>"* %copy, %"Array<i32>"* %row)
  %"$copy" = load %"Array<i32>", %"Array<i32>"* %copy, align 8
  call void @"Array<Array<i32>>.append"(%"Array<Array<i32>>"* %grid, %"Array<i32>" %"$copy")
  %call.make1 = call %"Array<i32>" @make(i32 2)
  call void @"Array<Array<i32>>.append"(%"Array<Array<i32>>"* %grid, %"Array<i32>" %call.make1)
  %"call.Array<Array<i32>>.[]" = call %"Array<i32>"* @"Array<Array<i32>>.[]"(%"Array<Array<i32>>"* %grid, i32 0)
  store %"Array<i32>"* %"call.Array<Array<i32>>.[]", %"Array<i32>"** %ref, align 8
  %"*ref" = load %"Array<i32>"*, %"Array<i32>"** %ref, align 8
  call void @"Array<i32>.append"(%"Array<i32>"* %"*ref", i32 9)
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %copy3, align 8
  %"call.Array<Array<i32>>.[]4" = call %"Array<i32>"* @"Array<Array<i32>>.[]"(%"Array<Array<i32>>"* %grid, i32 0)
  store %"Array<i32>"* %"call.Array<Array<i32>>.[]4", %"Array<i32>"** %ref5, align 8
  %"*ref6" = load %"Array<i32>"*, %"Array<i32>"** %ref5, align 8
  call void @"Array<i32>.="(%"Array<i32>"* %copy3, %"Array<i32>"* %"*ref6")
  %"$copy7" = load %"Array<i32>", %"Array<i32>"* %copy3, align 8
  store %"Array<i32>" %"$copy7", %"Array<i32>"* %copy2, align 8
  call void @"Array<i32>.append"(%"Array<i32>"* %copy2, i32 5)
  %"call.Array<Array<i32>>.size" = call i32 @"Array<Array<i32>>.size"(%"Array<Array<i32>>"* %grid)
  %"call.Array<Array<i32>>.[]8" = call %"Array<i32>"* @"Array<Array<i32>>.[]"(%"Array<Array<i32>>"* %grid, i32 0)
  store %"Array<i32>"* %"call.Array<Array<i32>>.[]8", %"Array<i32>"** %ref9, align 8
  %"*ref10" = load %"Array<i32>"*, %"Array<i32>"** %ref9, align 8
  %"call.Array<i32>.size" = call i32 @"Array<i32>.size"(%"Array<i32>"* %"*ref10")
  %"call.Array<Array<i32>>.[]11" = call %"Array<i32>"* @"Array<Array<i32>>.[]"(%"Array<Array<i32>>"* %grid, i32 1)
  store %"Array<i32>"* %"call.Array<Array<i32>>.[]11", %"Array<i32>"** %ref12, align 8
  %"*ref13" = load %"Array<i32>"*, %"Array<i32>"** %ref12, align 8
  %"call.Array<i32>.size14" = call i32 @"Array<i32>.size"(%"Array<i32>"* %"*ref13")
  %"call.Array<i32>.size15" = call i32 @"Array<i32>.size"(%"Array<i32>"* %copy2)
  %"call.Array<i32>.size16" = call i32 @"Array<i32>.size"(%"Array<i32>"* %row)
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str, i32 0, i32 0), i32 %"call.Array<Array<i32>>.size", i32 %"call.Array<i32>.size", i32 %"call.Array<i32>.size14", i32 %"call.Array<i32>.size15", i32 %"call.Array<i32>.size16")
  store %Bag zeroinitializer, %Bag* %bag, align 8
  %bag.items = getelementptr inbounds %Bag, %Bag* %bag, i32 0, i32 0
  %call.make17 = call %"Array<i32>" @make(i32 4)
  call void @"Array<i32>.drop"(%"Array<i32>"* %bag.items)
  store %"Array<i32>" %call.make17, %"Array<i32>"* %bag.items, align 8
  %bag.items18 = getelementptr inbounds %Bag, %Bag* %bag, i32 0, i32 0
  %call.make19 = call %"Array<i32>" @make(i32 2)
  call void @"Array<i32>.drop"(%"Array<i32>"* %bag.items18)
  store %"Array<i32>" %call.make19, %"Array<i32>"* %bag.items18, align 8
  %call.Bag.first_items = call %"Array<i32>" @Bag.first_items(%Bag* %bag)
  store %"Array<i32>" %call.Bag.first_items, %"Array<i32>"* %items, align 8
  call void @"Array<i32>.append"(%"Array<i32>"* %items, i32 7)
  %bag.items20 = getelementptr inbounds %Bag, %Bag* %bag, i32 0, i32 0
  %"call.Array<i32>.size21" = call i32 @"Array<i32>.size"(%"Array<i32>"* %bag.items20)
  %"call.Array<i32>.size22" = call i32 @"Array<i32>.size"(%"Array<i32>"* %items)
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %copy23, align 8
  call void @"Array<i32>.="(%"Array<i32>"* %copy23, %"Array<i32>"* %items)
  %"$copy24" = load %"Array<i32>", %"Array<i32>"* %copy23, align 8
  %call.total = call i32 @total(%"Array<i32>" %"$copy24")
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %copy25, align 8
  %"call.Array<Array<i32>>.[]26" = call %"Array<i32>"* @"Array<Array<i32>>.[]"(%"Array<Array<i32>>"* %grid, i32 0)
  store %"Array<i32>"* %"call.Array<Array<i32>>.[]26", %"Array<i32>"** %ref27, align 8
  %"*ref28" = load %"Array<i32>"*, %"Array<i32>"** %ref27, align 8
  call void @"Array<i32>.="(%"Array<i32>"* %copy25, %"Array<i32>"* %"*ref28")
  %"$copy29" = load %"Array<i32>", %"Array<i32>"* %copy25, align 8
  %call.total30 = call i32 @total(%"Array<i32>" %"$copy29")
  %call.dprintf31 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.2, i32 0, i32 0), i32 %"call.Array<i32>.size21", i32 %"call.Array<i32>.size22", i32 %call.total, i32 %call.total30)
  %"call.Array<Array<i32>>.pop" = call %"Array<i32>" @"Array<Array<i32>>.pop.1"(%"Array<Array<i32>>"* %grid)
  store %"Array<i32>" %"call.Array<Array<i32>>.pop", %"Array<i32>"* %popped, align 8
  call void @"Array<i32>.drop"(%"Array<i32>"* %row)
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %row, align 8
  %"call.Array<i32>.size32" = call i32 @"Array<i32>.size"(%"Array<i32>"* %popped)
  %"call.Array<i32>.size33" = call i32 @"Array<i32>.size"(%"Array<i32>"* %row)
  %"call.Array<Array<i32>>.size34" = call i32 @"Array<Array<i32>>.size"(%"Array<Array<i32>>"* %grid)
  %call.dprintf35 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.3, i32 0, i32 0), i32 %"call.Array<i32>.size32", i32 %"call.Array<i32>.size33", i32 %"call.Array<Array<i32>>.size34")
  call void @"Array<i32>.drop"(%"Array<i32>"* %popped)
  call void @"Array<i32>.drop"(%"Array<i32>"* %items)
  %bag.items36 = getelementptr inbounds %Bag, %Bag* %bag, i32 0, i32 0
  call void @"Array<i32>.drop"(%"Array<i32>"* %bag.items36)
  call void @"Array<i32>.drop"(%"Array<i32>"* %copy2)
  call void @"Array<i32>.drop"(%"Array<i32>"* %row)
  call void @"Array<Array<i32>>.drop"(%"Array<Array<i32>>"* %grid)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 041 — ownership: operator = taking the other side by value

```ura
struct Vec:
    x i32

    operator =(other Vec):
        self.x = other.x

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:4:5 operator = takes the other side by reference, a copy would need itself
  |
4 |     operator =(other Vec):
  |     ^^^^^^^^
help: use operator =(other &Vec)
error: aborting due to 1 error
```

### status

```
1
```

