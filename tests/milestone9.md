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
- 017 — operator: operator = on reassignment, but not on declaration
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

---

## 001 — method: a pub fn is static: no self, called with ::

```ura
// stdout: floor = 7
// expect: 0

struct Room:
    floor i32

    pub fn make(f i32) Room:
        r Room
        r.floor = f
        return r

fn main() i32:
    r Room = Room::make(7)
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

define %Room @Room.make(i32 %0) {
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
  %arr = alloca [9 x i8], align 1
  %arr1 = alloca [2 x i8], align 1
  %arr2 = alloca [12 x i8], align 1
  %call.Room.make = call %Room @Room.make(i32 7)
  store %Room %call.Room.make, %Room* %r, align 4
  store [9 x i8] c"floor = \00", [9 x i8]* %arr, align 1
  %0 = getelementptr inbounds [9 x i8], [9 x i8]* %arr, i32 0, i32 0
  %r.floor = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr1, align 1
  %1 = getelementptr inbounds [2 x i8], [2 x i8]* %arr1, i32 0, i32 0
  %"$r.floor" = load i32, i32* %r.floor, align 4
  store [12 x i8] c"floor = %d\0A\00", [12 x i8]* %arr2, align 1
  %2 = getelementptr inbounds [12 x i8], [12 x i8]* %arr2, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %2, i32 %"$r.floor")
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
    output(Nope::make(), "\n")
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
  %arr = alloca [6 x i8], align 1
  %arr1 = alloca [8 x i8], align 1
  %arr2 = alloca [2 x i8], align 1
  %arr3 = alloca [18 x i8], align 1
  %arr4 = alloca [6 x i8], align 1
  %tmp = alloca %Room, align 8
  %arr5 = alloca [2 x i8], align 1
  %arr6 = alloca [9 x i8], align 1
  %arr8 = alloca [7 x i8], align 1
  %arr9 = alloca [2 x i8], align 1
  %arr10 = alloca [10 x i8], align 1
  %arr12 = alloca [8 x i8], align 1
  %tmp14 = alloca %Room, align 8
  %arr15 = alloca [2 x i8], align 1
  %arr16 = alloca [11 x i8], align 1
  %n = alloca i32, align 4
  %tmp19 = alloca %Room, align 8
  %tmp22 = alloca %Tower, align 8
  %arr23 = alloca [7 x i8], align 1
  %arr24 = alloca [2 x i8], align 1
  %arr25 = alloca [10 x i8], align 1
  %call.Room.create = call %Room @Room.create(i32 3)
  store %Room %call.Room.create, %Room* %a, align 4
  %call.Tower.create = call %Tower @Tower.create(i32 9)
  store %Tower %call.Tower.create, %Tower* %b, align 4
  store [6 x i8] c"room \00", [6 x i8]* %arr, align 1
  %0 = getelementptr inbounds [6 x i8], [6 x i8]* %arr, i32 0, i32 0
  %a.floor = getelementptr inbounds %Room, %Room* %a, i32 0, i32 0
  store [8 x i8] c" tower \00", [8 x i8]* %arr1, align 1
  %1 = getelementptr inbounds [8 x i8], [8 x i8]* %arr1, i32 0, i32 0
  %b.height = getelementptr inbounds %Tower, %Tower* %b, i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr2, align 1
  %2 = getelementptr inbounds [2 x i8], [2 x i8]* %arr2, i32 0, i32 0
  %"$a.floor" = load i32, i32* %a.floor, align 4
  %"$b.height" = load i32, i32* %b.height, align 4
  store [18 x i8] c"room %d tower %d\0A\00", [18 x i8]* %arr3, align 1
  %3 = getelementptr inbounds [18 x i8], [18 x i8]* %arr3, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %3, i32 %"$a.floor", i32 %"$b.height")
  store [6 x i8] c"zero \00", [6 x i8]* %arr4, align 1
  %4 = getelementptr inbounds [6 x i8], [6 x i8]* %arr4, i32 0, i32 0
  %call.Room.zero = call %Room @Room.zero()
  store %Room %call.Room.zero, %Room* %tmp, align 4
  %tmp.floor = getelementptr inbounds %Room, %Room* %tmp, i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr5, align 1
  %5 = getelementptr inbounds [2 x i8], [2 x i8]* %arr5, i32 0, i32 0
  %"$tmp.floor" = load i32, i32* %tmp.floor, align 4
  store [9 x i8] c"zero %d\0A\00", [9 x i8]* %arr6, align 1
  %6 = getelementptr inbounds [9 x i8], [9 x i8]* %arr6, i32 0, i32 0
  %call.dprintf7 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %6, i32 %"$tmp.floor")
  store [7 x i8] c"level \00", [7 x i8]* %arr8, align 1
  %7 = getelementptr inbounds [7 x i8], [7 x i8]* %arr8, i32 0, i32 0
  %call.Room.level = call i32 @Room.level(i32 4)
  store [2 x i8] c"\0A\00", [2 x i8]* %arr9, align 1
  %8 = getelementptr inbounds [2 x i8], [2 x i8]* %arr9, i32 0, i32 0
  store [10 x i8] c"level %d\0A\00", [10 x i8]* %arr10, align 1
  %9 = getelementptr inbounds [10 x i8], [10 x i8]* %arr10, i32 0, i32 0
  %call.dprintf11 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %9, i32 %call.Room.level)
  store [8 x i8] c"method \00", [8 x i8]* %arr12, align 1
  %10 = getelementptr inbounds [8 x i8], [8 x i8]* %arr12, i32 0, i32 0
  %call.Room.create13 = call %Room @Room.create(i32 5)
  store %Room %call.Room.create13, %Room* %tmp14, align 4
  %call.Room.depth = call i32 @Room.depth(%Room* %tmp14)
  store [2 x i8] c"\0A\00", [2 x i8]* %arr15, align 1
  %11 = getelementptr inbounds [2 x i8], [2 x i8]* %arr15, i32 0, i32 0
  store [11 x i8] c"method %d\0A\00", [11 x i8]* %arr16, align 1
  %12 = getelementptr inbounds [11 x i8], [11 x i8]* %arr16, i32 0, i32 0
  %call.dprintf17 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %12, i32 %call.Room.depth)
  %call.Room.create18 = call %Room @Room.create(i32 2)
  store %Room %call.Room.create18, %Room* %tmp19, align 4
  %call.Room.depth20 = call i32 @Room.depth(%Room* %tmp19)
  %call.Tower.create21 = call %Tower @Tower.create(i32 1)
  store %Tower %call.Tower.create21, %Tower* %tmp22, align 4
  %tmp.height = getelementptr inbounds %Tower, %Tower* %tmp22, i32 0, i32 0
  %"$tmp.height" = load i32, i32* %tmp.height, align 4
  %ADD = add i32 %call.Room.depth20, %"$tmp.height"
  store i32 %ADD, i32* %n, align 4
  store [7 x i8] c"mixed \00", [7 x i8]* %arr23, align 1
  %13 = getelementptr inbounds [7 x i8], [7 x i8]* %arr23, i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr24, align 1
  %14 = getelementptr inbounds [2 x i8], [2 x i8]* %arr24, i32 0, i32 0
  %"$n" = load i32, i32* %n, align 4
  store [10 x i8] c"mixed %d\0A\00", [10 x i8]* %arr25, align 1
  %15 = getelementptr inbounds [10 x i8], [10 x i8]* %arr25, i32 0, i32 0
  %call.dprintf26 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %15, i32 %"$n")
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
  %arr = alloca [2 x i8], align 1
  %arr2 = alloca [2 x i8], align 1
  %arr3 = alloca [7 x i8], align 1
  %call.Vec.create = call %Vec @Vec.create(i32 1, i32 2)
  store %Vec %call.Vec.create, %Vec* %a, align 4
  %call.Vec.create1 = call %Vec @Vec.create(i32 10, i32 20)
  store %Vec %call.Vec.create1, %Vec* %b, align 4
  %"$b" = load %Vec, %Vec* %b, align 4
  %"call.Vec.+" = call %Vec @"Vec.+"(%Vec* %a, %Vec %"$b")
  store %Vec %"call.Vec.+", %Vec* %c, align 4
  %c.x = getelementptr inbounds %Vec, %Vec* %c, i32 0, i32 0
  store [2 x i8] c" \00", [2 x i8]* %arr, align 1
  %0 = getelementptr inbounds [2 x i8], [2 x i8]* %arr, i32 0, i32 0
  %c.y = getelementptr inbounds %Vec, %Vec* %c, i32 0, i32 1
  store [2 x i8] c"\0A\00", [2 x i8]* %arr2, align 1
  %1 = getelementptr inbounds [2 x i8], [2 x i8]* %arr2, i32 0, i32 0
  %"$c.x" = load i32, i32* %c.x, align 4
  %"$c.y" = load i32, i32* %c.y, align 4
  store [7 x i8] c"%d %d\0A\00", [7 x i8]* %arr3, align 1
  %2 = getelementptr inbounds [7 x i8], [7 x i8]* %arr3, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %2, i32 %"$c.x", i32 %"$c.y")
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
  %arr = alloca [2 x i8], align 1
  %tmp3 = alloca %Vec, align 8
  %arr5 = alloca [2 x i8], align 1
  %arr7 = alloca [7 x i8], align 1
  %call.Vec.create = call %Vec @Vec.create(i32 1)
  store %Vec %call.Vec.create, %Vec* %a, align 4
  %call.Vec.create1 = call %Vec @Vec.create(i32 10)
  store %Vec %call.Vec.create1, %Vec* %b, align 4
  %"$b" = load %Vec, %Vec* %b, align 4
  %"call.Vec.+" = call %Vec @"Vec.+"(%Vec* %a, %Vec %"$b")
  store %Vec %"call.Vec.+", %Vec* %tmp, align 4
  %tmp.x = getelementptr inbounds %Vec, %Vec* %tmp, i32 0, i32 0
  store [2 x i8] c" \00", [2 x i8]* %arr, align 1
  %0 = getelementptr inbounds [2 x i8], [2 x i8]* %arr, i32 0, i32 0
  %"call.Vec.+2" = call %Vec @"Vec.+.1"(%Vec* %a, i32 5)
  store %Vec %"call.Vec.+2", %Vec* %tmp3, align 4
  %tmp.x4 = getelementptr inbounds %Vec, %Vec* %tmp3, i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr5, align 1
  %1 = getelementptr inbounds [2 x i8], [2 x i8]* %arr5, i32 0, i32 0
  %"$tmp.x" = load i32, i32* %tmp.x, align 4
  %"$tmp.x6" = load i32, i32* %tmp.x4, align 4
  store [7 x i8] c"%d %d\0A\00", [7 x i8]* %arr7, align 1
  %2 = getelementptr inbounds [7 x i8], [7 x i8]* %arr7, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %2, i32 %"$tmp.x", i32 %"$tmp.x6")
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
  %arr = alloca [2 x i8], align 1
  %arr5 = alloca [2 x i8], align 1
  %bool.text = alloca i8*, align 8
  %arr6 = alloca [5 x i8], align 1
  %arr7 = alloca [6 x i8], align 1
  %bool.text8 = alloca i8*, align 8
  %arr9 = alloca [5 x i8], align 1
  %arr10 = alloca [6 x i8], align 1
  %arr11 = alloca [7 x i8], align 1
  %call.Vec.create = call %Vec @Vec.create(i32 3)
  store %Vec %call.Vec.create, %Vec* %a, align 4
  %call.Vec.create2 = call %Vec @Vec.create(i32 3)
  store %Vec %call.Vec.create2, %Vec* %b, align 4
  %call.Vec.create3 = call %Vec @Vec.create(i32 9)
  store %Vec %call.Vec.create3, %Vec* %c, align 4
  %"$b" = load %Vec, %Vec* %b, align 4
  %"call.Vec.==" = call i1 @"Vec.=="(%Vec* %a, %Vec %"$b")
  store [2 x i8] c" \00", [2 x i8]* %arr, align 1
  %0 = getelementptr inbounds [2 x i8], [2 x i8]* %arr, i32 0, i32 0
  %"$c" = load %Vec, %Vec* %c, align 4
  %"call.Vec.==4" = call i1 @"Vec.=="(%Vec* %a, %Vec %"$c")
  store [2 x i8] c"\0A\00", [2 x i8]* %arr5, align 1
  %1 = getelementptr inbounds [2 x i8], [2 x i8]* %arr5, i32 0, i32 0
  store i8* null, i8** %bool.text, align 8
  br label %if0

if0:                                              ; preds = %entry
  br i1 %"call.Vec.==", label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  store [5 x i8] c"True\00", [5 x i8]* %arr6, align 1
  %2 = getelementptr inbounds [5 x i8], [5 x i8]* %arr6, i32 0, i32 0
  store i8* %2, i8** %bool.text, align 8
  br label %endif

else1:                                            ; preds = %if0
  store [6 x i8] c"False\00", [6 x i8]* %arr7, align 1
  %3 = getelementptr inbounds [6 x i8], [6 x i8]* %arr7, i32 0, i32 0
  store i8* %3, i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  store i8* null, i8** %bool.text8, align 8
  br label %if2

if2:                                              ; preds = %endif
  br i1 %"call.Vec.==4", label %if2.then, label %else3

if2.then:                                         ; preds = %if2
  store [5 x i8] c"True\00", [5 x i8]* %arr9, align 1
  %4 = getelementptr inbounds [5 x i8], [5 x i8]* %arr9, i32 0, i32 0
  store i8* %4, i8** %bool.text8, align 8
  br label %endif1

else3:                                            ; preds = %if2
  store [6 x i8] c"False\00", [6 x i8]* %arr10, align 1
  %5 = getelementptr inbounds [6 x i8], [6 x i8]* %arr10, i32 0, i32 0
  store i8* %5, i8** %bool.text8, align 8
  br label %endif1

endif1:                                           ; preds = %else3, %if2.then
  store [7 x i8] c"%s %s\0A\00", [7 x i8]* %arr11, align 1
  %6 = getelementptr inbounds [7 x i8], [7 x i8]* %arr11, i32 0, i32 0
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %"$bool.text12" = load i8*, i8** %bool.text8, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %6, i8* %"$bool.text", i8* %"$bool.text12")
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
  %arr = alloca [2 x i8], align 1
  %arr1 = alloca [3 x i8], align 1
  %arr3 = alloca [3 x i8], align 1
  %arr4 = alloca [10 x i8], align 1
  store %Vec* %0, %Vec** %self, align 8
  store [2 x i8] c"(\00", [2 x i8]* %arr, align 1
  %1 = getelementptr inbounds [2 x i8], [2 x i8]* %arr, i32 0, i32 0
  %"*self" = load %Vec*, %Vec** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec, %Vec* %"*self", i32 0, i32 0
  store [3 x i8] c", \00", [3 x i8]* %arr1, align 1
  %2 = getelementptr inbounds [3 x i8], [3 x i8]* %arr1, i32 0, i32 0
  %"*self2" = load %Vec*, %Vec** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec, %Vec* %"*self2", i32 0, i32 1
  store [3 x i8] c")\0A\00", [3 x i8]* %arr3, align 1
  %3 = getelementptr inbounds [3 x i8], [3 x i8]* %arr3, i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  store [10 x i8] c"(%d, %d)\0A\00", [10 x i8]* %arr4, align 1
  %4 = getelementptr inbounds [10 x i8], [10 x i8]* %arr4, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %4, i32 %"$*self.x", i32 %"$*self.y")
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Vec, align 8
  %b = alloca %Vec, align 8
  %c = alloca %Vec, align 8
  %arr = alloca [9 x i8], align 1
  %arr3 = alloca [2 x i8], align 1
  %bool.text = alloca i8*, align 8
  %arr4 = alloca [5 x i8], align 1
  %arr5 = alloca [6 x i8], align 1
  %arr6 = alloca [12 x i8], align 1
  %arr7 = alloca [9 x i8], align 1
  %arr10 = alloca [2 x i8], align 1
  %bool.text11 = alloca i8*, align 8
  %arr12 = alloca [5 x i8], align 1
  %arr13 = alloca [6 x i8], align 1
  %arr14 = alloca [12 x i8], align 1
  %tmp = alloca %Vec, align 8
  %call.Vec.create = call %Vec @Vec.create(i32 1, i32 2)
  store %Vec %call.Vec.create, %Vec* %a, align 4
  %call.Vec.create2 = call %Vec @Vec.create(i32 10, i32 20)
  store %Vec %call.Vec.create2, %Vec* %b, align 4
  %"$b" = load %Vec, %Vec* %b, align 4
  %"call.Vec.+" = call %Vec @"Vec.+"(%Vec* %a, %Vec %"$b")
  store %Vec %"call.Vec.+", %Vec* %c, align 4
  call void @Vec.show(%Vec* %c)
  store [9 x i8] c"a == a  \00", [9 x i8]* %arr, align 1
  %0 = getelementptr inbounds [9 x i8], [9 x i8]* %arr, i32 0, i32 0
  %"$a" = load %Vec, %Vec* %a, align 4
  %"call.Vec.==" = call i1 @"Vec.=="(%Vec* %a, %Vec %"$a")
  store [2 x i8] c"\0A\00", [2 x i8]* %arr3, align 1
  %1 = getelementptr inbounds [2 x i8], [2 x i8]* %arr3, i32 0, i32 0
  store i8* null, i8** %bool.text, align 8
  br label %if0

if0:                                              ; preds = %entry
  br i1 %"call.Vec.==", label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  store [5 x i8] c"True\00", [5 x i8]* %arr4, align 1
  %2 = getelementptr inbounds [5 x i8], [5 x i8]* %arr4, i32 0, i32 0
  store i8* %2, i8** %bool.text, align 8
  br label %endif

else1:                                            ; preds = %if0
  store [6 x i8] c"False\00", [6 x i8]* %arr5, align 1
  %3 = getelementptr inbounds [6 x i8], [6 x i8]* %arr5, i32 0, i32 0
  store i8* %3, i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  store [12 x i8] c"a == a  %s\0A\00", [12 x i8]* %arr6, align 1
  %4 = getelementptr inbounds [12 x i8], [12 x i8]* %arr6, i32 0, i32 0
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %4, i8* %"$bool.text")
  store [9 x i8] c"a == b  \00", [9 x i8]* %arr7, align 1
  %5 = getelementptr inbounds [9 x i8], [9 x i8]* %arr7, i32 0, i32 0
  %"$b8" = load %Vec, %Vec* %b, align 4
  %"call.Vec.==9" = call i1 @"Vec.=="(%Vec* %a, %Vec %"$b8")
  store [2 x i8] c"\0A\00", [2 x i8]* %arr10, align 1
  %6 = getelementptr inbounds [2 x i8], [2 x i8]* %arr10, i32 0, i32 0
  store i8* null, i8** %bool.text11, align 8
  br label %if2

if2:                                              ; preds = %endif
  br i1 %"call.Vec.==9", label %if2.then, label %else3

if2.then:                                         ; preds = %if2
  store [5 x i8] c"True\00", [5 x i8]* %arr12, align 1
  %7 = getelementptr inbounds [5 x i8], [5 x i8]* %arr12, i32 0, i32 0
  store i8* %7, i8** %bool.text11, align 8
  br label %endif1

else3:                                            ; preds = %if2
  store [6 x i8] c"False\00", [6 x i8]* %arr13, align 1
  %8 = getelementptr inbounds [6 x i8], [6 x i8]* %arr13, i32 0, i32 0
  store i8* %8, i8** %bool.text11, align 8
  br label %endif1

endif1:                                           ; preds = %else3, %if2.then
  store [12 x i8] c"a == b  %s\0A\00", [12 x i8]* %arr14, align 1
  %9 = getelementptr inbounds [12 x i8], [12 x i8]* %arr14, i32 0, i32 0
  %"$bool.text15" = load i8*, i8** %bool.text11, align 8
  %call.dprintf16 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %9, i8* %"$bool.text15")
  %"$b17" = load %Vec, %Vec* %b, align 4
  %"call.Vec.+18" = call %Vec @"Vec.+"(%Vec* %a, %Vec %"$b17")
  store %Vec %"call.Vec.+18", %Vec* %tmp, align 4
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

## 017 — operator: operator = on reassignment, but not on declaration

```ura
// stdout: decl   5 0
// stdout: assign 5 99
// expect: 0

struct Vec:
    x i32
    tag i32

    operator =(other Vec) void:
        self.x   = other.x
        self.tag = 99

fn main() i32:
    a Vec
    a.x = 5

    // declaration: bitwise copy, tag stays 0
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

define void @"Vec.="(%Vec* %0, %Vec %1) {
entry:
  %self = alloca %Vec*, align 8
  %other = alloca %Vec, align 8
  store %Vec* %0, %Vec** %self, align 8
  store %Vec %1, %Vec* %other, align 4
  %"*self" = load %Vec*, %Vec** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec, %Vec* %"*self", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 0
  %"$other.x" = load i32, i32* %other.x, align 4
  store i32 %"$other.x", i32* %"*self.x", align 4
  %"*self1" = load %Vec*, %Vec** %self, align 8
  %"*self.tag" = getelementptr inbounds %Vec, %Vec* %"*self1", i32 0, i32 1
  store i32 99, i32* %"*self.tag", align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Vec, align 8
  %b = alloca %Vec, align 8
  %arr = alloca [8 x i8], align 1
  %arr1 = alloca [2 x i8], align 1
  %arr2 = alloca [2 x i8], align 1
  %arr3 = alloca [14 x i8], align 1
  %c = alloca %Vec, align 8
  %arr5 = alloca [8 x i8], align 1
  %arr6 = alloca [2 x i8], align 1
  %arr7 = alloca [2 x i8], align 1
  %arr8 = alloca [14 x i8], align 1
  store %Vec zeroinitializer, %Vec* %a, align 4
  %a.x = getelementptr inbounds %Vec, %Vec* %a, i32 0, i32 0
  store i32 5, i32* %a.x, align 4
  %"$a" = load %Vec, %Vec* %a, align 4
  store %Vec %"$a", %Vec* %b, align 4
  store [8 x i8] c"decl   \00", [8 x i8]* %arr, align 1
  %0 = getelementptr inbounds [8 x i8], [8 x i8]* %arr, i32 0, i32 0
  %b.x = getelementptr inbounds %Vec, %Vec* %b, i32 0, i32 0
  store [2 x i8] c" \00", [2 x i8]* %arr1, align 1
  %1 = getelementptr inbounds [2 x i8], [2 x i8]* %arr1, i32 0, i32 0
  %b.tag = getelementptr inbounds %Vec, %Vec* %b, i32 0, i32 1
  store [2 x i8] c"\0A\00", [2 x i8]* %arr2, align 1
  %2 = getelementptr inbounds [2 x i8], [2 x i8]* %arr2, i32 0, i32 0
  %"$b.x" = load i32, i32* %b.x, align 4
  %"$b.tag" = load i32, i32* %b.tag, align 4
  store [14 x i8] c"decl   %d %d\0A\00", [14 x i8]* %arr3, align 1
  %3 = getelementptr inbounds [14 x i8], [14 x i8]* %arr3, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %3, i32 %"$b.x", i32 %"$b.tag")
  store %Vec zeroinitializer, %Vec* %c, align 4
  %"$a4" = load %Vec, %Vec* %a, align 4
  call void @"Vec.="(%Vec* %c, %Vec %"$a4")
  store [8 x i8] c"assign \00", [8 x i8]* %arr5, align 1
  %4 = getelementptr inbounds [8 x i8], [8 x i8]* %arr5, i32 0, i32 0
  %c.x = getelementptr inbounds %Vec, %Vec* %c, i32 0, i32 0
  store [2 x i8] c" \00", [2 x i8]* %arr6, align 1
  %5 = getelementptr inbounds [2 x i8], [2 x i8]* %arr6, i32 0, i32 0
  %c.tag = getelementptr inbounds %Vec, %Vec* %c, i32 0, i32 1
  store [2 x i8] c"\0A\00", [2 x i8]* %arr7, align 1
  %6 = getelementptr inbounds [2 x i8], [2 x i8]* %arr7, i32 0, i32 0
  %"$c.x" = load i32, i32* %c.x, align 4
  %"$c.tag" = load i32, i32* %c.tag, align 4
  store [14 x i8] c"assign %d %d\0A\00", [14 x i8]* %arr8, align 1
  %7 = getelementptr inbounds [14 x i8], [14 x i8]* %arr8, i32 0, i32 0
  %call.dprintf9 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %7, i32 %"$c.x", i32 %"$c.tag")
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
  %arr = alloca [10 x i8], align 1
  %arr2 = alloca [2 x i8], align 1
  %arr3 = alloca [13 x i8], align 1
  %arr4 = alloca [10 x i8], align 1
  %arr6 = alloca [2 x i8], align 1
  %arr8 = alloca [13 x i8], align 1
  store %Vec zeroinitializer, %Vec* %a, align 4
  %a.x = getelementptr inbounds %Vec, %Vec* %a, i32 0, i32 0
  store i32 10, i32* %a.x, align 4
  store %Vec zeroinitializer, %Vec* %b, align 4
  %b.x = getelementptr inbounds %Vec, %Vec* %b, i32 0, i32 0
  store i32 4, i32* %b.x, align 4
  %"$b" = load %Vec, %Vec* %b, align 4
  call void @"Vec.+="(%Vec* %a, %Vec %"$b")
  store [10 x i8] c"after += \00", [10 x i8]* %arr, align 1
  %0 = getelementptr inbounds [10 x i8], [10 x i8]* %arr, i32 0, i32 0
  %a.x1 = getelementptr inbounds %Vec, %Vec* %a, i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr2, align 1
  %1 = getelementptr inbounds [2 x i8], [2 x i8]* %arr2, i32 0, i32 0
  %"$a.x" = load i32, i32* %a.x1, align 4
  store [13 x i8] c"after += %d\0A\00", [13 x i8]* %arr3, align 1
  %2 = getelementptr inbounds [13 x i8], [13 x i8]* %arr3, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %2, i32 %"$a.x")
  call void @"Vec.-="(%Vec* %a, i32 3)
  store [10 x i8] c"after -= \00", [10 x i8]* %arr4, align 1
  %3 = getelementptr inbounds [10 x i8], [10 x i8]* %arr4, i32 0, i32 0
  %a.x5 = getelementptr inbounds %Vec, %Vec* %a, i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr6, align 1
  %4 = getelementptr inbounds [2 x i8], [2 x i8]* %arr6, i32 0, i32 0
  %"$a.x7" = load i32, i32* %a.x5, align 4
  store [13 x i8] c"after -= %d\0A\00", [13 x i8]* %arr8, align 1
  %5 = getelementptr inbounds [13 x i8], [13 x i8]* %arr8, i32 0, i32 0
  %call.dprintf9 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %5, i32 %"$a.x7")
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

    operator =(other Tag) void:
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
// stdout: (1, 2) tag 0
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

    operator =(other Vec) void:
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

    // declaration is a bitwise copy: operator = does not run, tag stays 0
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

define void @"Vec.="(%Vec* %0, %Vec %1) {
entry:
  %self = alloca %Vec*, align 8
  %other = alloca %Vec, align 8
  store %Vec* %0, %Vec** %self, align 8
  store %Vec %1, %Vec* %other, align 4
  %"*self" = load %Vec*, %Vec** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec, %Vec* %"*self", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 0
  %"$other.x" = load i32, i32* %other.x, align 4
  store i32 %"$other.x", i32* %"*self.x", align 4
  %"*self1" = load %Vec*, %Vec** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec, %Vec* %"*self1", i32 0, i32 1
  %other.y = getelementptr inbounds %Vec, %Vec* %other, i32 0, i32 1
  %"$other.y" = load i32, i32* %other.y, align 4
  store i32 %"$other.y", i32* %"*self.y", align 4
  %"*self2" = load %Vec*, %Vec** %self, align 8
  %"*self.tag" = getelementptr inbounds %Vec, %Vec* %"*self2", i32 0, i32 2
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
  %arr = alloca [2 x i8], align 1
  %arr1 = alloca [3 x i8], align 1
  %arr3 = alloca [7 x i8], align 1
  %arr5 = alloca [2 x i8], align 1
  %arr6 = alloca [17 x i8], align 1
  store %Vec* %0, %Vec** %self, align 8
  store [2 x i8] c"(\00", [2 x i8]* %arr, align 1
  %1 = getelementptr inbounds [2 x i8], [2 x i8]* %arr, i32 0, i32 0
  %"*self" = load %Vec*, %Vec** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec, %Vec* %"*self", i32 0, i32 0
  store [3 x i8] c", \00", [3 x i8]* %arr1, align 1
  %2 = getelementptr inbounds [3 x i8], [3 x i8]* %arr1, i32 0, i32 0
  %"*self2" = load %Vec*, %Vec** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec, %Vec* %"*self2", i32 0, i32 1
  store [7 x i8] c") tag \00", [7 x i8]* %arr3, align 1
  %3 = getelementptr inbounds [7 x i8], [7 x i8]* %arr3, i32 0, i32 0
  %"*self4" = load %Vec*, %Vec** %self, align 8
  %"*self.tag" = getelementptr inbounds %Vec, %Vec* %"*self4", i32 0, i32 2
  store [2 x i8] c"\0A\00", [2 x i8]* %arr5, align 1
  %4 = getelementptr inbounds [2 x i8], [2 x i8]* %arr5, i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  %"$*self.tag" = load i32, i32* %"*self.tag", align 4
  store [17 x i8] c"(%d, %d) tag %d\0A\00", [17 x i8]* %arr6, align 1
  %5 = getelementptr inbounds [17 x i8], [17 x i8]* %arr6, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %5, i32 %"$*self.x", i32 %"$*self.y", i32 %"$*self.tag")
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Vec, align 8
  %b = alloca %Vec, align 8
  %c = alloca %Vec, align 8
  %d = alloca %Vec, align 8
  %arr = alloca [9 x i8], align 1
  %arr4 = alloca [2 x i8], align 1
  %bool.text = alloca i8*, align 8
  %arr5 = alloca [5 x i8], align 1
  %arr6 = alloca [6 x i8], align 1
  %arr7 = alloca [12 x i8], align 1
  %arr8 = alloca [9 x i8], align 1
  %arr11 = alloca [2 x i8], align 1
  %bool.text12 = alloca i8*, align 8
  %arr13 = alloca [5 x i8], align 1
  %arr14 = alloca [6 x i8], align 1
  %arr15 = alloca [12 x i8], align 1
  %tmp = alloca %Vec, align 8
  %e = alloca %Vec, align 8
  %f = alloca %Vec, align 8
  %g = alloca %Vec, align 8
  %call.Vec.create = call %Vec @Vec.create(i32 1, i32 2)
  store %Vec %call.Vec.create, %Vec* %a, align 4
  %call.Vec.create2 = call %Vec @Vec.create(i32 10, i32 20)
  store %Vec %call.Vec.create2, %Vec* %b, align 4
  %"$b" = load %Vec, %Vec* %b, align 4
  %"call.Vec.+" = call %Vec @"Vec.+"(%Vec* %a, %Vec %"$b")
  store %Vec %"call.Vec.+", %Vec* %c, align 4
  call void @Vec.show(%Vec* %c)
  %"call.Vec.+3" = call %Vec @"Vec.+.1"(%Vec* %a, i32 5)
  store %Vec %"call.Vec.+3", %Vec* %d, align 4
  call void @Vec.show(%Vec* %d)
  store [9 x i8] c"a == a  \00", [9 x i8]* %arr, align 1
  %0 = getelementptr inbounds [9 x i8], [9 x i8]* %arr, i32 0, i32 0
  %"$a" = load %Vec, %Vec* %a, align 4
  %"call.Vec.==" = call i1 @"Vec.=="(%Vec* %a, %Vec %"$a")
  store [2 x i8] c"\0A\00", [2 x i8]* %arr4, align 1
  %1 = getelementptr inbounds [2 x i8], [2 x i8]* %arr4, i32 0, i32 0
  store i8* null, i8** %bool.text, align 8
  br label %if0

if0:                                              ; preds = %entry
  br i1 %"call.Vec.==", label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  store [5 x i8] c"True\00", [5 x i8]* %arr5, align 1
  %2 = getelementptr inbounds [5 x i8], [5 x i8]* %arr5, i32 0, i32 0
  store i8* %2, i8** %bool.text, align 8
  br label %endif

else1:                                            ; preds = %if0
  store [6 x i8] c"False\00", [6 x i8]* %arr6, align 1
  %3 = getelementptr inbounds [6 x i8], [6 x i8]* %arr6, i32 0, i32 0
  store i8* %3, i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  store [12 x i8] c"a == a  %s\0A\00", [12 x i8]* %arr7, align 1
  %4 = getelementptr inbounds [12 x i8], [12 x i8]* %arr7, i32 0, i32 0
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %4, i8* %"$bool.text")
  store [9 x i8] c"a == b  \00", [9 x i8]* %arr8, align 1
  %5 = getelementptr inbounds [9 x i8], [9 x i8]* %arr8, i32 0, i32 0
  %"$b9" = load %Vec, %Vec* %b, align 4
  %"call.Vec.==10" = call i1 @"Vec.=="(%Vec* %a, %Vec %"$b9")
  store [2 x i8] c"\0A\00", [2 x i8]* %arr11, align 1
  %6 = getelementptr inbounds [2 x i8], [2 x i8]* %arr11, i32 0, i32 0
  store i8* null, i8** %bool.text12, align 8
  br label %if2

if2:                                              ; preds = %endif
  br i1 %"call.Vec.==10", label %if2.then, label %else3

if2.then:                                         ; preds = %if2
  store [5 x i8] c"True\00", [5 x i8]* %arr13, align 1
  %7 = getelementptr inbounds [5 x i8], [5 x i8]* %arr13, i32 0, i32 0
  store i8* %7, i8** %bool.text12, align 8
  br label %endif1

else3:                                            ; preds = %if2
  store [6 x i8] c"False\00", [6 x i8]* %arr14, align 1
  %8 = getelementptr inbounds [6 x i8], [6 x i8]* %arr14, i32 0, i32 0
  store i8* %8, i8** %bool.text12, align 8
  br label %endif1

endif1:                                           ; preds = %else3, %if2.then
  store [12 x i8] c"a == b  %s\0A\00", [12 x i8]* %arr15, align 1
  %9 = getelementptr inbounds [12 x i8], [12 x i8]* %arr15, i32 0, i32 0
  %"$bool.text16" = load i8*, i8** %bool.text12, align 8
  %call.dprintf17 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %9, i8* %"$bool.text16")
  %"$b18" = load %Vec, %Vec* %b, align 4
  %"call.Vec.+19" = call %Vec @"Vec.+"(%Vec* %a, %Vec %"$b18")
  store %Vec %"call.Vec.+19", %Vec* %tmp, align 4
  call void @Vec.show(%Vec* %tmp)
  %"$a20" = load %Vec, %Vec* %a, align 4
  store %Vec %"$a20", %Vec* %e, align 4
  call void @Vec.show(%Vec* %e)
  store %Vec zeroinitializer, %Vec* %f, align 4
  %"$a21" = load %Vec, %Vec* %a, align 4
  call void @"Vec.="(%Vec* %f, %Vec %"$a21")
  call void @Vec.show(%Vec* %f)
  %call.Vec.create22 = call %Vec @Vec.create(i32 10, i32 10)
  store %Vec %call.Vec.create22, %Vec* %g, align 4
  %"$b23" = load %Vec, %Vec* %b, align 4
  call void @"Vec.+="(%Vec* %g, %Vec %"$b23")
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

    operator =(other R) void:
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

    operator =(other Vec2) void:
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

define void @"Vec2.="(%Vec2* %0, %Vec2 %1) {
entry:
  %self = alloca %Vec2*, align 8
  %other = alloca %Vec2, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %"*self" = load %Vec2*, %Vec2** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec2, %Vec2* %"*self", i32 0, i32 0
  %other.x = getelementptr inbounds %Vec2, %Vec2* %other, i32 0, i32 0
  %"$other.x" = load i32, i32* %other.x, align 4
  store i32 %"$other.x", i32* %"*self.x", align 4
  %"*self1" = load %Vec2*, %Vec2** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec2, %Vec2* %"*self1", i32 0, i32 1
  %other.y = getelementptr inbounds %Vec2, %Vec2* %other, i32 0, i32 1
  %"$other.y" = load i32, i32* %other.y, align 4
  store i32 %"$other.y", i32* %"*self.y", align 4
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
  %arr = alloca [2 x i8], align 1
  %arr1 = alloca [3 x i8], align 1
  %arr3 = alloca [3 x i8], align 1
  %arr4 = alloca [12 x i8], align 1
  store %Vec2* %0, %Vec2** %self, align 8
  store i8* %1, i8** %label, align 8
  store [2 x i8] c"(\00", [2 x i8]* %arr, align 1
  %2 = getelementptr inbounds [2 x i8], [2 x i8]* %arr, i32 0, i32 0
  %"*self" = load %Vec2*, %Vec2** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec2, %Vec2* %"*self", i32 0, i32 0
  store [3 x i8] c", \00", [3 x i8]* %arr1, align 1
  %3 = getelementptr inbounds [3 x i8], [3 x i8]* %arr1, i32 0, i32 0
  %"*self2" = load %Vec2*, %Vec2** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec2, %Vec2* %"*self2", i32 0, i32 1
  store [3 x i8] c")\0A\00", [3 x i8]* %arr3, align 1
  %4 = getelementptr inbounds [3 x i8], [3 x i8]* %arr3, i32 0, i32 0
  %"$label" = load i8*, i8** %label, align 8
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  store [12 x i8] c"%s(%d, %d)\0A\00", [12 x i8]* %arr4, align 1
  %5 = getelementptr inbounds [12 x i8], [12 x i8]* %arr4, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %5, i8* %"$label", i32 %"$*self.x", i32 %"$*self.y")
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Vec2, align 8
  %b = alloca %Vec2, align 8
  %tmp = alloca %Vec2, align 8
  %arr = alloca [9 x i8], align 1
  %tmp9 = alloca %Vec2, align 8
  %arr10 = alloca [9 x i8], align 1
  %tmp11 = alloca %Vec2, align 8
  %arr12 = alloca [9 x i8], align 1
  %tmp13 = alloca %Vec2, align 8
  %arr14 = alloca [9 x i8], align 1
  %tmp15 = alloca %Vec2, align 8
  %arr16 = alloca [9 x i8], align 1
  %arr17 = alloca [4 x i8], align 1
  %arr19 = alloca [6 x i8], align 1
  %arr21 = alloca [2 x i8], align 1
  %bool.text = alloca i8*, align 8
  %arr22 = alloca [5 x i8], align 1
  %arr23 = alloca [6 x i8], align 1
  %bool.text24 = alloca i8*, align 8
  %arr25 = alloca [5 x i8], align 1
  %arr26 = alloca [6 x i8], align 1
  %arr27 = alloca [14 x i8], align 1
  %arr29 = alloca [4 x i8], align 1
  %arr31 = alloca [6 x i8], align 1
  %arr33 = alloca [2 x i8], align 1
  %bool.text34 = alloca i8*, align 8
  %arr35 = alloca [5 x i8], align 1
  %arr36 = alloca [6 x i8], align 1
  %bool.text37 = alloca i8*, align 8
  %arr38 = alloca [5 x i8], align 1
  %arr39 = alloca [6 x i8], align 1
  %arr40 = alloca [14 x i8], align 1
  %arr44 = alloca [4 x i8], align 1
  %arr46 = alloca [6 x i8], align 1
  %arr48 = alloca [2 x i8], align 1
  %bool.text49 = alloca i8*, align 8
  %arr50 = alloca [5 x i8], align 1
  %arr51 = alloca [6 x i8], align 1
  %bool.text52 = alloca i8*, align 8
  %arr53 = alloca [5 x i8], align 1
  %arr54 = alloca [6 x i8], align 1
  %arr55 = alloca [14 x i8], align 1
  %c = alloca %Vec2, align 8
  %arr60 = alloca [10 x i8], align 1
  %arr62 = alloca [10 x i8], align 1
  %arr64 = alloca [10 x i8], align 1
  %arr65 = alloca [10 x i8], align 1
  %arr66 = alloca [10 x i8], align 1
  %arr67 = alloca [10 x i8], align 1
  %arr70 = alloca [13 x i8], align 1
  %arr71 = alloca [13 x i8], align 1
  %n = alloca i32, align 4
  %d = alloca %Vec2, align 8
  %arr77 = alloca [18 x i8], align 1
  %arr78 = alloca [2 x i8], align 1
  %arr80 = alloca [21 x i8], align 1
  %call.Vec2.create = call %Vec2 @Vec2.create(i32 9, i32 12)
  store %Vec2 %call.Vec2.create, %Vec2* %a, align 4
  %call.Vec2.create7 = call %Vec2 @Vec2.create(i32 2, i32 3)
  store %Vec2 %call.Vec2.create7, %Vec2* %b, align 4
  %"$b" = load %Vec2, %Vec2* %b, align 4
  %"call.Vec2.+" = call %Vec2 @"Vec2.+"(%Vec2* %a, %Vec2 %"$b")
  store %Vec2 %"call.Vec2.+", %Vec2* %tmp, align 4
  store [9 x i8] c"a + b   \00", [9 x i8]* %arr, align 1
  %0 = getelementptr inbounds [9 x i8], [9 x i8]* %arr, i32 0, i32 0
  call void @Vec2.show(%Vec2* %tmp, i8* %0)
  %"$b8" = load %Vec2, %Vec2* %b, align 4
  %call.Vec2.- = call %Vec2 @Vec2.-(%Vec2* %a, %Vec2 %"$b8")
  store %Vec2 %call.Vec2.-, %Vec2* %tmp9, align 4
  store [9 x i8] c"a - b   \00", [9 x i8]* %arr10, align 1
  %1 = getelementptr inbounds [9 x i8], [9 x i8]* %arr10, i32 0, i32 0
  call void @Vec2.show(%Vec2* %tmp9, i8* %1)
  %"call.Vec2.*" = call %Vec2 @"Vec2.*"(%Vec2* %a, i32 3)
  store %Vec2 %"call.Vec2.*", %Vec2* %tmp11, align 4
  store [9 x i8] c"a * 3   \00", [9 x i8]* %arr12, align 1
  %2 = getelementptr inbounds [9 x i8], [9 x i8]* %arr12, i32 0, i32 0
  call void @Vec2.show(%Vec2* %tmp11, i8* %2)
  %"call.Vec2./" = call %Vec2 @"Vec2./"(%Vec2* %a, i32 2)
  store %Vec2 %"call.Vec2./", %Vec2* %tmp13, align 4
  store [9 x i8] c"a / 2   \00", [9 x i8]* %arr14, align 1
  %3 = getelementptr inbounds [9 x i8], [9 x i8]* %arr14, i32 0, i32 0
  call void @Vec2.show(%Vec2* %tmp13, i8* %3)
  %"call.Vec2.%" = call %Vec2 @"Vec2.%"(%Vec2* %a, i32 4)
  store %Vec2 %"call.Vec2.%", %Vec2* %tmp15, align 4
  store [9 x i8] c"a mod 4 \00", [9 x i8]* %arr16, align 1
  %4 = getelementptr inbounds [9 x i8], [9 x i8]* %arr16, i32 0, i32 0
  call void @Vec2.show(%Vec2* %tmp15, i8* %4)
  store [4 x i8] c"== \00", [4 x i8]* %arr17, align 1
  %5 = getelementptr inbounds [4 x i8], [4 x i8]* %arr17, i32 0, i32 0
  %"$b18" = load %Vec2, %Vec2* %b, align 4
  %"call.Vec2.==" = call i1 @"Vec2.=="(%Vec2* %a, %Vec2 %"$b18")
  store [6 x i8] c"  != \00", [6 x i8]* %arr19, align 1
  %6 = getelementptr inbounds [6 x i8], [6 x i8]* %arr19, i32 0, i32 0
  %"$b20" = load %Vec2, %Vec2* %b, align 4
  %"call.Vec2.!=" = call i1 @"Vec2.!="(%Vec2* %a, %Vec2 %"$b20")
  store [2 x i8] c"\0A\00", [2 x i8]* %arr21, align 1
  %7 = getelementptr inbounds [2 x i8], [2 x i8]* %arr21, i32 0, i32 0
  store i8* null, i8** %bool.text, align 8
  br label %if1

if1:                                              ; preds = %entry
  br i1 %"call.Vec2.==", label %if1.then, label %else2

if1.then:                                         ; preds = %if1
  store [5 x i8] c"True\00", [5 x i8]* %arr22, align 1
  %8 = getelementptr inbounds [5 x i8], [5 x i8]* %arr22, i32 0, i32 0
  store i8* %8, i8** %bool.text, align 8
  br label %endif

else2:                                            ; preds = %if1
  store [6 x i8] c"False\00", [6 x i8]* %arr23, align 1
  %9 = getelementptr inbounds [6 x i8], [6 x i8]* %arr23, i32 0, i32 0
  store i8* %9, i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else2, %if1.then
  store i8* null, i8** %bool.text24, align 8
  br label %if3

if3:                                              ; preds = %endif
  br i1 %"call.Vec2.!=", label %if3.then, label %else4

if3.then:                                         ; preds = %if3
  store [5 x i8] c"True\00", [5 x i8]* %arr25, align 1
  %10 = getelementptr inbounds [5 x i8], [5 x i8]* %arr25, i32 0, i32 0
  store i8* %10, i8** %bool.text24, align 8
  br label %endif1

else4:                                            ; preds = %if3
  store [6 x i8] c"False\00", [6 x i8]* %arr26, align 1
  %11 = getelementptr inbounds [6 x i8], [6 x i8]* %arr26, i32 0, i32 0
  store i8* %11, i8** %bool.text24, align 8
  br label %endif1

endif1:                                           ; preds = %else4, %if3.then
  store [14 x i8] c"== %s  != %s\0A\00", [14 x i8]* %arr27, align 1
  %12 = getelementptr inbounds [14 x i8], [14 x i8]* %arr27, i32 0, i32 0
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %"$bool.text28" = load i8*, i8** %bool.text24, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %12, i8* %"$bool.text", i8* %"$bool.text28")
  store [4 x i8] c"<  \00", [4 x i8]* %arr29, align 1
  %13 = getelementptr inbounds [4 x i8], [4 x i8]* %arr29, i32 0, i32 0
  %"$b30" = load %Vec2, %Vec2* %b, align 4
  %"call.Vec2.<" = call i1 @"Vec2.<"(%Vec2* %a, %Vec2 %"$b30")
  store [6 x i8] c"  >  \00", [6 x i8]* %arr31, align 1
  %14 = getelementptr inbounds [6 x i8], [6 x i8]* %arr31, i32 0, i32 0
  %"$b32" = load %Vec2, %Vec2* %b, align 4
  %"call.Vec2.>" = call i1 @"Vec2.>"(%Vec2* %a, %Vec2 %"$b32")
  store [2 x i8] c"\0A\00", [2 x i8]* %arr33, align 1
  %15 = getelementptr inbounds [2 x i8], [2 x i8]* %arr33, i32 0, i32 0
  store i8* null, i8** %bool.text34, align 8
  br label %if5

if5:                                              ; preds = %endif1
  br i1 %"call.Vec2.<", label %if5.then, label %else6

if5.then:                                         ; preds = %if5
  store [5 x i8] c"True\00", [5 x i8]* %arr35, align 1
  %16 = getelementptr inbounds [5 x i8], [5 x i8]* %arr35, i32 0, i32 0
  store i8* %16, i8** %bool.text34, align 8
  br label %endif2

else6:                                            ; preds = %if5
  store [6 x i8] c"False\00", [6 x i8]* %arr36, align 1
  %17 = getelementptr inbounds [6 x i8], [6 x i8]* %arr36, i32 0, i32 0
  store i8* %17, i8** %bool.text34, align 8
  br label %endif2

endif2:                                           ; preds = %else6, %if5.then
  store i8* null, i8** %bool.text37, align 8
  br label %if7

if7:                                              ; preds = %endif2
  br i1 %"call.Vec2.>", label %if7.then, label %else8

if7.then:                                         ; preds = %if7
  store [5 x i8] c"True\00", [5 x i8]* %arr38, align 1
  %18 = getelementptr inbounds [5 x i8], [5 x i8]* %arr38, i32 0, i32 0
  store i8* %18, i8** %bool.text37, align 8
  br label %endif3

else8:                                            ; preds = %if7
  store [6 x i8] c"False\00", [6 x i8]* %arr39, align 1
  %19 = getelementptr inbounds [6 x i8], [6 x i8]* %arr39, i32 0, i32 0
  store i8* %19, i8** %bool.text37, align 8
  br label %endif3

endif3:                                           ; preds = %else8, %if7.then
  store [14 x i8] c"<  %s  >  %s\0A\00", [14 x i8]* %arr40, align 1
  %20 = getelementptr inbounds [14 x i8], [14 x i8]* %arr40, i32 0, i32 0
  %"$bool.text41" = load i8*, i8** %bool.text34, align 8
  %"$bool.text42" = load i8*, i8** %bool.text37, align 8
  %call.dprintf43 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %20, i8* %"$bool.text41", i8* %"$bool.text42")
  store [4 x i8] c"<= \00", [4 x i8]* %arr44, align 1
  %21 = getelementptr inbounds [4 x i8], [4 x i8]* %arr44, i32 0, i32 0
  %"$b45" = load %Vec2, %Vec2* %b, align 4
  %"call.Vec2.<=" = call i1 @"Vec2.<="(%Vec2* %a, %Vec2 %"$b45")
  store [6 x i8] c"  >= \00", [6 x i8]* %arr46, align 1
  %22 = getelementptr inbounds [6 x i8], [6 x i8]* %arr46, i32 0, i32 0
  %"$b47" = load %Vec2, %Vec2* %b, align 4
  %"call.Vec2.>=" = call i1 @"Vec2.>="(%Vec2* %a, %Vec2 %"$b47")
  store [2 x i8] c"\0A\00", [2 x i8]* %arr48, align 1
  %23 = getelementptr inbounds [2 x i8], [2 x i8]* %arr48, i32 0, i32 0
  store i8* null, i8** %bool.text49, align 8
  br label %if9

if9:                                              ; preds = %endif3
  br i1 %"call.Vec2.<=", label %if9.then, label %else10

if9.then:                                         ; preds = %if9
  store [5 x i8] c"True\00", [5 x i8]* %arr50, align 1
  %24 = getelementptr inbounds [5 x i8], [5 x i8]* %arr50, i32 0, i32 0
  store i8* %24, i8** %bool.text49, align 8
  br label %endif4

else10:                                           ; preds = %if9
  store [6 x i8] c"False\00", [6 x i8]* %arr51, align 1
  %25 = getelementptr inbounds [6 x i8], [6 x i8]* %arr51, i32 0, i32 0
  store i8* %25, i8** %bool.text49, align 8
  br label %endif4

endif4:                                           ; preds = %else10, %if9.then
  store i8* null, i8** %bool.text52, align 8
  br label %if11

if11:                                             ; preds = %endif4
  br i1 %"call.Vec2.>=", label %if11.then, label %else12

if11.then:                                        ; preds = %if11
  store [5 x i8] c"True\00", [5 x i8]* %arr53, align 1
  %26 = getelementptr inbounds [5 x i8], [5 x i8]* %arr53, i32 0, i32 0
  store i8* %26, i8** %bool.text52, align 8
  br label %endif5

else12:                                           ; preds = %if11
  store [6 x i8] c"False\00", [6 x i8]* %arr54, align 1
  %27 = getelementptr inbounds [6 x i8], [6 x i8]* %arr54, i32 0, i32 0
  store i8* %27, i8** %bool.text52, align 8
  br label %endif5

endif5:                                           ; preds = %else12, %if11.then
  store [14 x i8] c"<= %s  >= %s\0A\00", [14 x i8]* %arr55, align 1
  %28 = getelementptr inbounds [14 x i8], [14 x i8]* %arr55, i32 0, i32 0
  %"$bool.text56" = load i8*, i8** %bool.text49, align 8
  %"$bool.text57" = load i8*, i8** %bool.text52, align 8
  %call.dprintf58 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %28, i8* %"$bool.text56", i8* %"$bool.text57")
  %call.Vec2.create59 = call %Vec2 @Vec2.create(i32 0, i32 0)
  store %Vec2 %call.Vec2.create59, %Vec2* %c, align 4
  %"$a" = load %Vec2, %Vec2* %a, align 4
  call void @"Vec2.="(%Vec2* %c, %Vec2 %"$a")
  store [10 x i8] c"c = a    \00", [10 x i8]* %arr60, align 1
  %29 = getelementptr inbounds [10 x i8], [10 x i8]* %arr60, i32 0, i32 0
  call void @Vec2.show(%Vec2* %c, i8* %29)
  %"$b61" = load %Vec2, %Vec2* %b, align 4
  call void @"Vec2.+="(%Vec2* %c, %Vec2 %"$b61")
  store [10 x i8] c"c += b   \00", [10 x i8]* %arr62, align 1
  %30 = getelementptr inbounds [10 x i8], [10 x i8]* %arr62, i32 0, i32 0
  call void @Vec2.show(%Vec2* %c, i8* %30)
  %"$b63" = load %Vec2, %Vec2* %b, align 4
  call void @"Vec2.-="(%Vec2* %c, %Vec2 %"$b63")
  store [10 x i8] c"c -= b   \00", [10 x i8]* %arr64, align 1
  %31 = getelementptr inbounds [10 x i8], [10 x i8]* %arr64, i32 0, i32 0
  call void @Vec2.show(%Vec2* %c, i8* %31)
  call void @"Vec2.*="(%Vec2* %c, i32 2)
  store [10 x i8] c"c *= 2   \00", [10 x i8]* %arr65, align 1
  %32 = getelementptr inbounds [10 x i8], [10 x i8]* %arr65, i32 0, i32 0
  call void @Vec2.show(%Vec2* %c, i8* %32)
  call void @"Vec2./="(%Vec2* %c, i32 3)
  store [10 x i8] c"c /= 3   \00", [10 x i8]* %arr66, align 1
  %33 = getelementptr inbounds [10 x i8], [10 x i8]* %arr66, i32 0, i32 0
  call void @Vec2.show(%Vec2* %c, i8* %33)
  call void @"Vec2.%="(%Vec2* %c, i32 5)
  store [10 x i8] c"c %= 5   \00", [10 x i8]* %arr67, align 1
  %34 = getelementptr inbounds [10 x i8], [10 x i8]* %arr67, i32 0, i32 0
  call void @Vec2.show(%Vec2* %c, i8* %34)
  br label %if13

if13:                                             ; preds = %endif5
  %"$b68" = load %Vec2, %Vec2* %b, align 4
  %"call.Vec2.>69" = call i1 @"Vec2.>"(%Vec2* %a, %Vec2 %"$b68")
  br i1 %"call.Vec2.>69", label %if13.then, label %endif6

if13.then:                                        ; preds = %if13
  store [13 x i8] c"a is longer\0A\00", [13 x i8]* %arr70, align 1
  %35 = getelementptr inbounds [13 x i8], [13 x i8]* %arr70, i32 0, i32 0
  store [13 x i8] c"a is longer\0A\00", [13 x i8]* %arr71, align 1
  %36 = getelementptr inbounds [13 x i8], [13 x i8]* %arr71, i32 0, i32 0
  %call.dprintf72 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %36)
  br label %endif6

endif6:                                           ; preds = %if13.then, %if13
  store i32 0, i32* %n, align 4
  %call.Vec2.create73 = call %Vec2 @Vec2.create(i32 1, i32 1)
  store %Vec2 %call.Vec2.create73, %Vec2* %d, align 4
  br label %while14

while14:                                          ; preds = %while14.then, %endif6
  %"$a74" = load %Vec2, %Vec2* %a, align 4
  %"call.Vec2.<75" = call i1 @"Vec2.<"(%Vec2* %d, %Vec2 %"$a74")
  br i1 %"call.Vec2.<75", label %while14.then, label %endloop

while14.then:                                     ; preds = %while14
  %"$b76" = load %Vec2, %Vec2* %b, align 4
  call void @"Vec2.+="(%Vec2* %d, %Vec2 %"$b76")
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 1
  store i32 %ADD, i32* %n, align 4
  br label %while14

endloop:                                          ; preds = %while14
  store [18 x i8] c"steps to pass a: \00", [18 x i8]* %arr77, align 1
  %37 = getelementptr inbounds [18 x i8], [18 x i8]* %arr77, i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr78, align 1
  %38 = getelementptr inbounds [2 x i8], [2 x i8]* %arr78, i32 0, i32 0
  %"$n79" = load i32, i32* %n, align 4
  store [21 x i8] c"steps to pass a: %d\0A\00", [21 x i8]* %arr80, align 1
  %39 = getelementptr inbounds [21 x i8], [21 x i8]* %arr80, i32 0, i32 0
  %call.dprintf81 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %39, i32 %"$n79")
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
// error: 'm.make' is a 'pub fn', so it is static and takes no receiver; call it as 'Maker::make()'
use "@/header"

struct Maker:
    n i32

    pub fn make() Maker:
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
  %arr = alloca [2 x i8], align 1
  %ref16 = alloca i32*, align 8
  %arr18 = alloca [2 x i8], align 1
  %arr21 = alloca [7 x i8], align 1
  %ref23 = alloca i32*, align 8
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
  store [2 x i8] c" \00", [2 x i8]* %arr, align 1
  %0 = getelementptr inbounds [2 x i8], [2 x i8]* %arr, i32 0, i32 0
  %"call.Bag.[]15" = call i32* @"Bag.[]"(%Bag* %b, i32 1)
  store i32* %"call.Bag.[]15", i32** %ref16, align 8
  %"*ref17" = load i32*, i32** %ref16, align 8
  store [2 x i8] c"\0A\00", [2 x i8]* %arr18, align 1
  %1 = getelementptr inbounds [2 x i8], [2 x i8]* %arr18, i32 0, i32 0
  %"$*ref19" = load i32, i32* %"*ref14", align 4
  %"$*ref20" = load i32, i32* %"*ref17", align 4
  store [7 x i8] c"%d %d\0A\00", [7 x i8]* %arr21, align 1
  %2 = getelementptr inbounds [7 x i8], [7 x i8]* %arr21, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %2, i32 %"$*ref19", i32 %"$*ref20")
  %"call.Bag.[]22" = call i32* @"Bag.[]"(%Bag* %b, i32 1)
  store i32* %"call.Bag.[]22", i32** %ref23, align 8
  %"*ref24" = load i32*, i32** %ref23, align 8
  %"$*ref25" = load i32, i32* %"*ref24", align 4
  ret i32 %"$*ref25"
}

declare i32 @dprintf(i32, i8*, ...)
```


---

## 033 — operator: drop runs at the end of blocks, on continue, and not on a returned local

```ura
use "@/array"


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

declare i8* @realloc(i8*, i32)

declare void @free(i8*)

define void @Noisy.drop(%Noisy* %0) {
entry:
  %self = alloca %Noisy*, align 8
  %arr = alloca [6 x i8], align 1
  %arr1 = alloca [2 x i8], align 1
  %arr2 = alloca [9 x i8], align 1
  store %Noisy* %0, %Noisy** %self, align 8
  store [6 x i8] c"drop \00", [6 x i8]* %arr, align 1
  %1 = getelementptr inbounds [6 x i8], [6 x i8]* %arr, i32 0, i32 0
  %"*self" = load %Noisy*, %Noisy** %self, align 8
  %"*self.id" = getelementptr inbounds %Noisy, %Noisy* %"*self", i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr1, align 1
  %2 = getelementptr inbounds [2 x i8], [2 x i8]* %arr1, i32 0, i32 0
  %"$*self.id" = load i32, i32* %"*self.id", align 4
  store [9 x i8] c"drop %d\0A\00", [9 x i8]* %arr2, align 1
  %3 = getelementptr inbounds [9 x i8], [9 x i8]* %arr2, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %3, i32 %"$*self.id")
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

define void @"Array<i32>.reserve"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %cap = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 2
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap", %"$*self.cap"
  br i1 %LE, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret void

endif:                                            ; preds = %if0
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap2" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 2
  %"$cap3" = load i32, i32* %cap, align 4
  store i32 %"$cap3", i32* %"*self.cap2", align 4
  %"*self4" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self4", i32 0, i32 0
  %"*self5" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data6" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self5", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data6", align 8
  %AS = bitcast i32* %"$*self.data" to i8*
  %"$cap7" = load i32, i32* %cap, align 4
  %MUL = mul i32 %"$cap7", ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i32)
  %call.realloc = call i8* @realloc(i8* %AS, i32 %MUL)
  %AS8 = bitcast i8* %call.realloc to i32*
  store i32* %AS8, i32** %"*self.data", align 8
  ret void
}

define void @"Array<i32>.resize"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %size = alloca i32, align 4
  %zero = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %size, align 4
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"$size" = load i32, i32* %size, align 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self", i32 %"$size")
  br label %while1

while1:                                           ; preds = %while1.then, %entry
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$size2" = load i32, i32* %size, align 4
  %LT = icmp slt i32 %"$*self.count", %"$size2"
  br i1 %LT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  store i32 0, i32* %zero, align 4
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 0
  %"*self4" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self4", i32 0, i32 1
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$*self.count6"
  %"$zero" = load i32, i32* %zero, align 4
  store i32 %"$zero", i32* %"*self.data[]", align 4
  %"*self7" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count8" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self7", i32 0, i32 1
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %ADD = add i32 %"$*self.count9", 1
  store i32 %ADD, i32* %"*self.count8", align 4
  br label %while1

endloop:                                          ; preds = %while1
  %"*self10" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self10", i32 0, i32 1
  %"$size12" = load i32, i32* %size, align 4
  store i32 %"$size12", i32* %"*self.count11", align 4
  ret void
}

define void @"Array<i32>.push_back"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %value = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %value, align 4
  br label %if2

if2:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.count", %"$*self.cap"
  br i1 %EQ, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 2
  %"$*self.cap5" = load i32, i32* %"*self.cap4", align 4
  %MUL = mul i32 %"$*self.cap5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if2.then, %if2
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

define void @"Array<i32>.append"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %value = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %value, align 4
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"$value" = load i32, i32* %value, align 4
  call void @"Array<i32>.push_back"(%"Array<i32>"* %"*self", i32 %"$value")
  ret void
}

define void @"Array<i32>.insert"(%"Array<i32>"* %0, i32 %1, i32 %2) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %index = alloca i32, align 4
  %value = alloca i32, align 4
  %i = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %index, align 4
  store i32 %2, i32* %value, align 4
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"$value" = load i32, i32* %value, align 4
  call void @"Array<i32>.push_back"(%"Array<i32>"* %"*self", i32 %"$value")
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  store i32 %SUB, i32* %i, align 4
  br label %while3

while3:                                           ; preds = %while3.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %"$index" = load i32, i32* %index, align 4
  %GT = icmp sgt i32 %"$i", %"$index"
  br i1 %GT, label %while3.then, label %endloop

while3.then:                                      ; preds = %while3
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$i3" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$i3"
  %"*self4" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data5" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self4", i32 0, i32 0
  %"$i6" = load i32, i32* %i, align 4
  %SUB7 = sub i32 %"$i6", 1
  %"$*self.data8" = load i32*, i32** %"*self.data5", align 8
  %"*self.data[]9" = getelementptr inbounds i32, i32* %"$*self.data8", i32 %SUB7
  %"$*self.data[]" = load i32, i32* %"*self.data[]9", align 4
  store i32 %"$*self.data[]", i32* %"*self.data[]", align 4
  %"$i10" = load i32, i32* %i, align 4
  %SUB11 = sub i32 %"$i10", 1
  store i32 %SUB11, i32* %i, align 4
  br label %while3

endloop:                                          ; preds = %while3
  %"*self12" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data13" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self12", i32 0, i32 0
  %"$*self.data14" = load i32*, i32** %"*self.data13", align 8
  %"$index15" = load i32, i32* %index, align 4
  %"*self.data[]16" = getelementptr inbounds i32, i32* %"$*self.data14", i32 %"$index15"
  %"$value17" = load i32, i32* %value, align 4
  store i32 %"$value17", i32* %"*self.data[]16", align 4
  ret void
}

define void @"Array<i32>.push_front"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %value = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %value, align 4
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"$value" = load i32, i32* %value, align 4
  call void @"Array<i32>.insert"(%"Array<i32>"* %"*self", i32 0, i32 %"$value")
  ret void
}

define i32 @"Array<i32>.remove"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %index = alloca i32, align 4
  %value = alloca i32, align 4
  %i = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %index, align 4
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$index"
  %"$*self.data[]" = load i32, i32* %"*self.data[]", align 4
  store i32 %"$*self.data[]", i32* %value, align 4
  %"$index1" = load i32, i32* %index, align 4
  store i32 %"$index1", i32* %i, align 4
  br label %while4

while4:                                           ; preds = %while4.then, %entry
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", %SUB
  br i1 %LT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 0
  %"$*self.data5" = load i32*, i32** %"*self.data4", align 8
  %"$i6" = load i32, i32* %i, align 4
  %"*self.data[]7" = getelementptr inbounds i32, i32* %"$*self.data5", i32 %"$i6"
  %"*self8" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data9" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self8", i32 0, i32 0
  %"$i10" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i10", 1
  %"$*self.data11" = load i32*, i32** %"*self.data9", align 8
  %"*self.data[]12" = getelementptr inbounds i32, i32* %"$*self.data11", i32 %ADD
  %"$*self.data[]13" = load i32, i32* %"*self.data[]12", align 4
  store i32 %"$*self.data[]13", i32* %"*self.data[]7", align 4
  %"$i14" = load i32, i32* %i, align 4
  %ADD15 = add i32 %"$i14", 1
  store i32 %ADD15, i32* %i, align 4
  br label %while4

endloop:                                          ; preds = %while4
  %"*self16" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count17" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self16", i32 0, i32 1
  %"$*self.count18" = load i32, i32* %"*self.count17", align 4
  %SUB19 = sub i32 %"$*self.count18", 1
  store i32 %SUB19, i32* %"*self.count17", align 4
  %"$value" = load i32, i32* %value, align 4
  ret i32 %"$value"
}

define i32 @"Array<i32>.pop_back"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  store i32 %SUB, i32* %"*self.count", align 4
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 0
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 1
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$*self.count4" = load i32, i32* %"*self.count3", align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$*self.count4"
  %"$*self.data[]" = load i32, i32* %"*self.data[]", align 4
  ret i32 %"$*self.data[]"
}

define i32 @"Array<i32>.pop"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"call.Array<i32>.pop_back" = call i32 @"Array<i32>.pop_back"(%"Array<i32>"* %"*self")
  ret i32 %"call.Array<i32>.pop_back"
}

define i32 @"Array<i32>.pop_front"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"call.Array<i32>.remove" = call i32 @"Array<i32>.remove"(%"Array<i32>"* %"*self", i32 0)
  ret i32 %"call.Array<i32>.remove"
}

define i32* @"Array<i32>.front"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 0
  ret i32* %"*self.data[]"
}

define i32* @"Array<i32>.back"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 0
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %SUB
  ret i32* %"*self.data[]"
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

define i32 @"Array<i32>.capacity"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  ret i32 %"$*self.cap"
}

define i1 @"Array<i32>.empty"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %EQ = icmp eq i32 %"$*self.count", 0
  ret i1 %EQ
}

define void @"Array<i32>.clear"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  ret void
}

define void @"Array<i32>.reverse"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %tmp = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 0, i32* %i, align 4
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  store i32 %SUB, i32* %j, align 4
  br label %while5

while5:                                           ; preds = %while5.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %"$j" = load i32, i32* %j, align 4
  %LT = icmp slt i32 %"$i", %"$j"
  br i1 %LT, label %while5.then, label %endloop

while5.then:                                      ; preds = %while5
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$i2" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$i2"
  %"$*self.data[]" = load i32, i32* %"*self.data[]", align 4
  store i32 %"$*self.data[]", i32* %tmp, align 4
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 0
  %"$*self.data5" = load i32*, i32** %"*self.data4", align 8
  %"$i6" = load i32, i32* %i, align 4
  %"*self.data[]7" = getelementptr inbounds i32, i32* %"$*self.data5", i32 %"$i6"
  %"*self8" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data9" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self8", i32 0, i32 0
  %"$*self.data10" = load i32*, i32** %"*self.data9", align 8
  %"$j11" = load i32, i32* %j, align 4
  %"*self.data[]12" = getelementptr inbounds i32, i32* %"$*self.data10", i32 %"$j11"
  %"$*self.data[]13" = load i32, i32* %"*self.data[]12", align 4
  store i32 %"$*self.data[]13", i32* %"*self.data[]7", align 4
  %"*self14" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data15" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self14", i32 0, i32 0
  %"$*self.data16" = load i32*, i32** %"*self.data15", align 8
  %"$j17" = load i32, i32* %j, align 4
  %"*self.data[]18" = getelementptr inbounds i32, i32* %"$*self.data16", i32 %"$j17"
  %"$tmp" = load i32, i32* %tmp, align 4
  store i32 %"$tmp", i32* %"*self.data[]18", align 4
  %"$i19" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i19", 1
  store i32 %ADD, i32* %i, align 4
  %"$j20" = load i32, i32* %j, align 4
  %SUB21 = sub i32 %"$j20", 1
  store i32 %SUB21, i32* %j, align 4
  br label %while5

endloop:                                          ; preds = %while5
  ret void
}

define void @"Array<i32>.release"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %nothing = alloca i32*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %AS = bitcast i32* %"$*self.data" to i8*
  call void @free(i8* %AS)
  store i32* null, i32** %nothing, align 8
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data2" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 0
  %"$nothing" = load i32*, i32** %nothing, align 8
  store i32* %"$nothing", i32** %"*self.data2", align 8
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  %"*self4" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self4", i32 0, i32 2
  store i32 0, i32* %"*self.cap", align 4
  ret void
}

define i32* @"Array<i32>.[]"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %index = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %index, align 4
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 0
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
  br label %while6

while6:                                           ; preds = %while6.then, %entry
  %"*other2" = load %"Array<i32>"*, %"Array<i32>"** %other, align 8
  %"*other.count3" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*other2", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*other.count4" = load i32, i32* %"*other.count3", align 4
  %LT = icmp slt i32 %"$i", %"$*other.count4"
  br i1 %LT, label %while6.then, label %endloop

while6.then:                                      ; preds = %while6
  %"*self5" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*other6" = load %"Array<i32>"*, %"Array<i32>"** %other, align 8
  %"*other.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*other6", i32 0, i32 0
  %"$*other.data" = load i32*, i32** %"*other.data", align 8
  %"$i7" = load i32, i32* %i, align 4
  %"*other.data[]" = getelementptr inbounds i32, i32* %"$*other.data", i32 %"$i7"
  %"$*other.data[]" = load i32, i32* %"*other.data[]", align 4
  call void @"Array<i32>.push_back"(%"Array<i32>"* %"*self5", i32 %"$*other.data[]")
  %"$i8" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i8", 1
  store i32 %ADD, i32* %i, align 4
  br label %while6

endloop:                                          ; preds = %while6
  ret void
}

define void @"Array<i32>.drop"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %AS = bitcast i32* %"$*self.data" to i8*
  call void @free(i8* %AS)
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Noisy, align 8
  %i = alloca i32, align 4
  %b = alloca %Noisy, align 8
  %arr = alloca [6 x i8], align 1
  %arr6 = alloca [2 x i8], align 1
  %arr8 = alloca [9 x i8], align 1
  %c = alloca %Noisy, align 8
  %x = alloca %"Array<i32>", align 8
  %y = alloca %"Array<i32>", align 8
  %arr10 = alloca [2 x i8], align 1
  %arr12 = alloca [2 x i8], align 1
  %ref = alloca i32*, align 8
  %arr13 = alloca [2 x i8], align 1
  %arr14 = alloca [10 x i8], align 1
  %call.make = call %Noisy @make(i32 1)
  store %Noisy %call.make, %Noisy* %a, align 4
  store i32 0, i32* %i, align 4
  br label %while7

while7:                                           ; preds = %endif, %if8.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while7.then, label %endloop

while7.then:                                      ; preds = %while7
  store %Noisy zeroinitializer, %Noisy* %b, align 4
  %b.id = getelementptr inbounds %Noisy, %Noisy* %b, i32 0, i32 0
  %"$i2" = load i32, i32* %i, align 4
  %ADD = add i32 10, %"$i2"
  store i32 %ADD, i32* %b.id, align 4
  %"$i3" = load i32, i32* %i, align 4
  %ADD4 = add i32 %"$i3", 1
  store i32 %ADD4, i32* %i, align 4
  br label %if8

if8:                                              ; preds = %while7.then
  %"$i5" = load i32, i32* %i, align 4
  %EQ = icmp eq i32 %"$i5", 2
  br i1 %EQ, label %if8.then, label %endif

if8.then:                                         ; preds = %if8
  call void @Noisy.drop(%Noisy* %b)
  br label %while7

endif:                                            ; preds = %if8
  store [6 x i8] c"loop \00", [6 x i8]* %arr, align 1
  %0 = getelementptr inbounds [6 x i8], [6 x i8]* %arr, i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr6, align 1
  %1 = getelementptr inbounds [2 x i8], [2 x i8]* %arr6, i32 0, i32 0
  %"$i7" = load i32, i32* %i, align 4
  store [9 x i8] c"loop %d\0A\00", [9 x i8]* %arr8, align 1
  %2 = getelementptr inbounds [9 x i8], [9 x i8]* %arr8, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %2, i32 %"$i7")
  call void @Noisy.drop(%Noisy* %b)
  br label %while7

endloop:                                          ; preds = %while7
  br label %if9

if9:                                              ; preds = %endloop
  %a.id = getelementptr inbounds %Noisy, %Noisy* %a, i32 0, i32 0
  %"$a.id" = load i32, i32* %a.id, align 4
  %EQ9 = icmp eq i32 %"$a.id", 1
  br i1 %EQ9, label %if9.then, label %endif1

if9.then:                                         ; preds = %if9
  store %Noisy zeroinitializer, %Noisy* %c, align 4
  %c.id = getelementptr inbounds %Noisy, %Noisy* %c, i32 0, i32 0
  store i32 5, i32* %c.id, align 4
  call void @Noisy.drop(%Noisy* %c)
  br label %endif1

endif1:                                           ; preds = %if9.then, %if9
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %x, align 8
  call void @"Array<i32>.append"(%"Array<i32>"* %x, i32 4)
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %y, align 8
  call void @"Array<i32>.="(%"Array<i32>"* %y, %"Array<i32>"* %x)
  call void @"Array<i32>.append"(%"Array<i32>"* %y, i32 5)
  %"call.Array<i32>.size" = call i32 @"Array<i32>.size"(%"Array<i32>"* %x)
  store [2 x i8] c" \00", [2 x i8]* %arr10, align 1
  %3 = getelementptr inbounds [2 x i8], [2 x i8]* %arr10, i32 0, i32 0
  %"call.Array<i32>.size11" = call i32 @"Array<i32>.size"(%"Array<i32>"* %y)
  store [2 x i8] c" \00", [2 x i8]* %arr12, align 1
  %4 = getelementptr inbounds [2 x i8], [2 x i8]* %arr12, i32 0, i32 0
  %"call.Array<i32>.[]" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %y, i32 1)
  store i32* %"call.Array<i32>.[]", i32** %ref, align 8
  %"*ref" = load i32*, i32** %ref, align 8
  store [2 x i8] c"\0A\00", [2 x i8]* %arr13, align 1
  %5 = getelementptr inbounds [2 x i8], [2 x i8]* %arr13, i32 0, i32 0
  %"$*ref" = load i32, i32* %"*ref", align 4
  store [10 x i8] c"%d %d %d\0A\00", [10 x i8]* %arr14, align 1
  %6 = getelementptr inbounds [10 x i8], [10 x i8]* %arr14, i32 0, i32 0
  %call.dprintf15 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %6, i32 %"call.Array<i32>.size", i32 %"call.Array<i32>.size11", i32 %"$*ref")
  call void @"Array<i32>.drop"(%"Array<i32>"* %y)
  call void @"Array<i32>.drop"(%"Array<i32>"* %x)
  call void @Noisy.drop(%Noisy* %a)
  ret i32 0
}

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
error: test.ura:9:14 'Buffer' has operator drop, a copy would free it twice
  |
9 |     b Buffer = a
  |              ^
help: define operator =(other &Buffer) to copy it
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
error: test.ura:12:17 'Buffer' has operator drop, passing a copy would free it twice
   |
12 |     return take(a)
   |                 ^
help: take it as &Buffer
error: aborting due to 1 error
```

### status

```
1
```


---

## 036 — struct: from takes attributes and methods of a template, a struct and a grandparent; overrides win

```ura
use "@/array"

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
    t.push_back('u')
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

%Named = type { i8* }
%"Array<i8>" = type { i8*, i32, i32 }
%Text = type { i8*, i32, i32, i8*, i32 }
%Title = type { i8*, i32, i32, i8*, i32 }

@TYPEOF = private unnamed_addr constant [5 x i8] c"Text\00", align 1

declare i8* @realloc(i8*, i32)

declare void @free(i8*)

define i8* @Named.name(%Named* %0) {
entry:
  %self = alloca %Named*, align 8
  store %Named* %0, %Named** %self, align 8
  %"*self" = load %Named*, %Named** %self, align 8
  %"*self.label" = getelementptr inbounds %Named, %Named* %"*self", i32 0, i32 0
  %"$*self.label" = load i8*, i8** %"*self.label", align 8
  ret i8* %"$*self.label"
}

define void @"Array<i8>.reserve"(%"Array<i8>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %cap = alloca i32, align 4
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap", %"$*self.cap"
  br i1 %LE, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret void

endif:                                            ; preds = %if0
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap2" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 2
  %"$cap3" = load i32, i32* %cap, align 4
  store i32 %"$cap3", i32* %"*self.cap2", align 4
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 0
  %"*self5" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data6" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self5", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data6", align 8
  %"$cap7" = load i32, i32* %cap, align 4
  %MUL = mul i32 %"$cap7", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %call.realloc = call i8* @realloc(i8* %"$*self.data", i32 %MUL)
  store i8* %call.realloc, i8** %"*self.data", align 8
  ret void
}

define void @"Array<i8>.resize"(%"Array<i8>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %size = alloca i32, align 4
  %zero = alloca i8, align 1
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store i32 %1, i32* %size, align 4
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"$size" = load i32, i32* %size, align 4
  call void @"Array<i8>.reserve"(%"Array<i8>"* %"*self", i32 %"$size")
  br label %while1

while1:                                           ; preds = %while1.then, %entry
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$size2" = load i32, i32* %size, align 4
  %LT = icmp slt i32 %"$*self.count", %"$size2"
  br i1 %LT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  store i8 0, i8* %zero, align 1
  %"*self3" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self3", i32 0, i32 0
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count6"
  %"$zero" = load i8, i8* %zero, align 1
  store i8 %"$zero", i8* %"*self.data[]", align 1
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count8" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 1
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %ADD = add i32 %"$*self.count9", 1
  store i32 %ADD, i32* %"*self.count8", align 4
  br label %while1

endloop:                                          ; preds = %while1
  %"*self10" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self10", i32 0, i32 1
  %"$size12" = load i32, i32* %size, align 4
  store i32 %"$size12", i32* %"*self.count11", align 4
  ret void
}

define void @"Array<i8>.push_back"(%"Array<i8>"* %0, i8 %1) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %value = alloca i8, align 1
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store i8 %1, i8* %value, align 1
  br label %if2

if2:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.count", %"$*self.cap"
  br i1 %EQ, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self3" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap4" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self3", i32 0, i32 2
  %"$*self.cap5" = load i32, i32* %"*self.cap4", align 4
  %MUL = mul i32 %"$*self.cap5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<i8>.reserve"(%"Array<i8>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  %"*self6" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self6", i32 0, i32 0
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count8" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count9"
  %"$value" = load i8, i8* %value, align 1
  store i8 %"$value", i8* %"*self.data[]", align 1
  %"*self10" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  ret void
}

define void @"Array<i8>.append"(%"Array<i8>"* %0, i8 %1) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %value = alloca i8, align 1
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store i8 %1, i8* %value, align 1
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"$value" = load i8, i8* %value, align 1
  call void @"Array<i8>.push_back"(%"Array<i8>"* %"*self", i8 %"$value")
  ret void
}

define void @"Array<i8>.insert"(%"Array<i8>"* %0, i32 %1, i8 %2) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %index = alloca i32, align 4
  %value = alloca i8, align 1
  %i = alloca i32, align 4
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store i32 %1, i32* %index, align 4
  store i8 %2, i8* %value, align 1
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"$value" = load i8, i8* %value, align 1
  call void @"Array<i8>.push_back"(%"Array<i8>"* %"*self", i8 %"$value")
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  store i32 %SUB, i32* %i, align 4
  br label %while3

while3:                                           ; preds = %while3.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %"$index" = load i32, i32* %index, align 4
  %GT = icmp sgt i32 %"$i", %"$index"
  br i1 %GT, label %while3.then, label %endloop

while3.then:                                      ; preds = %while3
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i3" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i3"
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data5" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 0
  %"$i6" = load i32, i32* %i, align 4
  %SUB7 = sub i32 %"$i6", 1
  %"$*self.data8" = load i8*, i8** %"*self.data5", align 8
  %"*self.data[]9" = getelementptr inbounds i8, i8* %"$*self.data8", i32 %SUB7
  %"$*self.data[]" = load i8, i8* %"*self.data[]9", align 1
  store i8 %"$*self.data[]", i8* %"*self.data[]", align 1
  %"$i10" = load i32, i32* %i, align 4
  %SUB11 = sub i32 %"$i10", 1
  store i32 %SUB11, i32* %i, align 4
  br label %while3

endloop:                                          ; preds = %while3
  %"*self12" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data13" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self12", i32 0, i32 0
  %"$*self.data14" = load i8*, i8** %"*self.data13", align 8
  %"$index15" = load i32, i32* %index, align 4
  %"*self.data[]16" = getelementptr inbounds i8, i8* %"$*self.data14", i32 %"$index15"
  %"$value17" = load i8, i8* %value, align 1
  store i8 %"$value17", i8* %"*self.data[]16", align 1
  ret void
}

define void @"Array<i8>.push_front"(%"Array<i8>"* %0, i8 %1) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %value = alloca i8, align 1
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store i8 %1, i8* %value, align 1
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"$value" = load i8, i8* %value, align 1
  call void @"Array<i8>.insert"(%"Array<i8>"* %"*self", i32 0, i8 %"$value")
  ret void
}

define i8 @"Array<i8>.remove"(%"Array<i8>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %index = alloca i32, align 4
  %value = alloca i8, align 1
  %i = alloca i32, align 4
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store i32 %1, i32* %index, align 4
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$index"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  store i8 %"$*self.data[]", i8* %value, align 1
  %"$index1" = load i32, i32* %index, align 4
  store i32 %"$index1", i32* %i, align 4
  br label %while4

while4:                                           ; preds = %while4.then, %entry
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", %SUB
  br i1 %LT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
  %"*self3" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data4" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self3", i32 0, i32 0
  %"$*self.data5" = load i8*, i8** %"*self.data4", align 8
  %"$i6" = load i32, i32* %i, align 4
  %"*self.data[]7" = getelementptr inbounds i8, i8* %"$*self.data5", i32 %"$i6"
  %"*self8" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data9" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self8", i32 0, i32 0
  %"$i10" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i10", 1
  %"$*self.data11" = load i8*, i8** %"*self.data9", align 8
  %"*self.data[]12" = getelementptr inbounds i8, i8* %"$*self.data11", i32 %ADD
  %"$*self.data[]13" = load i8, i8* %"*self.data[]12", align 1
  store i8 %"$*self.data[]13", i8* %"*self.data[]7", align 1
  %"$i14" = load i32, i32* %i, align 4
  %ADD15 = add i32 %"$i14", 1
  store i32 %ADD15, i32* %i, align 4
  br label %while4

endloop:                                          ; preds = %while4
  %"*self16" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count17" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self16", i32 0, i32 1
  %"$*self.count18" = load i32, i32* %"*self.count17", align 4
  %SUB19 = sub i32 %"$*self.count18", 1
  store i32 %SUB19, i32* %"*self.count17", align 4
  %"$value" = load i8, i8* %value, align 1
  ret i8 %"$value"
}

define i8 @"Array<i8>.pop_back"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  store i32 %SUB, i32* %"*self.count", align 4
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 0
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count4" = load i32, i32* %"*self.count3", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count4"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  ret i8 %"$*self.data[]"
}

define i8 @"Array<i8>.pop"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"call.Array<i8>.pop_back" = call i8 @"Array<i8>.pop_back"(%"Array<i8>"* %"*self")
  ret i8 %"call.Array<i8>.pop_back"
}

define i8 @"Array<i8>.pop_front"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"call.Array<i8>.remove" = call i8 @"Array<i8>.remove"(%"Array<i8>"* %"*self", i32 0)
  ret i8 %"call.Array<i8>.remove"
}

define i8* @"Array<i8>.front"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 0
  ret i8* %"*self.data[]"
}

define i8* @"Array<i8>.back"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 0
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %SUB
  ret i8* %"*self.data[]"
}

define i32 @"Array<i8>.size"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i32 @"Array<i8>.capacity"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  ret i32 %"$*self.cap"
}

define i1 @"Array<i8>.empty"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %EQ = icmp eq i32 %"$*self.count", 0
  ret i1 %EQ
}

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  ret void
}

define void @"Array<i8>.reverse"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %tmp = alloca i8, align 1
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store i32 0, i32* %i, align 4
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  store i32 %SUB, i32* %j, align 4
  br label %while5

while5:                                           ; preds = %while5.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %"$j" = load i32, i32* %j, align 4
  %LT = icmp slt i32 %"$i", %"$j"
  br i1 %LT, label %while5.then, label %endloop

while5.then:                                      ; preds = %while5
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i2" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i2"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  store i8 %"$*self.data[]", i8* %tmp, align 1
  %"*self3" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data4" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self3", i32 0, i32 0
  %"$*self.data5" = load i8*, i8** %"*self.data4", align 8
  %"$i6" = load i32, i32* %i, align 4
  %"*self.data[]7" = getelementptr inbounds i8, i8* %"$*self.data5", i32 %"$i6"
  %"*self8" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data9" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self8", i32 0, i32 0
  %"$*self.data10" = load i8*, i8** %"*self.data9", align 8
  %"$j11" = load i32, i32* %j, align 4
  %"*self.data[]12" = getelementptr inbounds i8, i8* %"$*self.data10", i32 %"$j11"
  %"$*self.data[]13" = load i8, i8* %"*self.data[]12", align 1
  store i8 %"$*self.data[]13", i8* %"*self.data[]7", align 1
  %"*self14" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data15" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self14", i32 0, i32 0
  %"$*self.data16" = load i8*, i8** %"*self.data15", align 8
  %"$j17" = load i32, i32* %j, align 4
  %"*self.data[]18" = getelementptr inbounds i8, i8* %"$*self.data16", i32 %"$j17"
  %"$tmp" = load i8, i8* %tmp, align 1
  store i8 %"$tmp", i8* %"*self.data[]18", align 1
  %"$i19" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i19", 1
  store i32 %ADD, i32* %i, align 4
  %"$j20" = load i32, i32* %j, align 4
  %SUB21 = sub i32 %"$j20", 1
  store i32 %SUB21, i32* %j, align 4
  br label %while5

endloop:                                          ; preds = %while5
  ret void
}

define void @"Array<i8>.release"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %nothing = alloca i8*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @free(i8* %"$*self.data")
  store i8* null, i8** %nothing, align 8
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data2" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 0
  %"$nothing" = load i8*, i8** %nothing, align 8
  store i8* %"$nothing", i8** %"*self.data2", align 8
  %"*self3" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self3", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 2
  store i32 0, i32* %"*self.cap", align 4
  ret void
}

define i8* @"Array<i8>.[]"(%"Array<i8>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %index = alloca i32, align 4
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store i32 %1, i32* %index, align 4
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$index"
  ret i8* %"*self.data[]"
}

define void @"Array<i8>.="(%"Array<i8>"* %0, %"Array<i8>"* %1) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %other = alloca %"Array<i8>"*, align 8
  %i = alloca i32, align 4
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store %"Array<i8>"* %1, %"Array<i8>"** %other, align 8
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*self")
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*other" = load %"Array<i8>"*, %"Array<i8>"** %other, align 8
  %"*other.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*other", i32 0, i32 1
  %"$*other.count" = load i32, i32* %"*other.count", align 4
  call void @"Array<i8>.reserve"(%"Array<i8>"* %"*self1", i32 %"$*other.count")
  store i32 0, i32* %i, align 4
  br label %while6

while6:                                           ; preds = %while6.then, %entry
  %"*other2" = load %"Array<i8>"*, %"Array<i8>"** %other, align 8
  %"*other.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*other2", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*other.count4" = load i32, i32* %"*other.count3", align 4
  %LT = icmp slt i32 %"$i", %"$*other.count4"
  br i1 %LT, label %while6.then, label %endloop

while6.then:                                      ; preds = %while6
  %"*self5" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*other6" = load %"Array<i8>"*, %"Array<i8>"** %other, align 8
  %"*other.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*other6", i32 0, i32 0
  %"$*other.data" = load i8*, i8** %"*other.data", align 8
  %"$i7" = load i32, i32* %i, align 4
  %"*other.data[]" = getelementptr inbounds i8, i8* %"$*other.data", i32 %"$i7"
  %"$*other.data[]" = load i8, i8* %"*other.data[]", align 1
  call void @"Array<i8>.push_back"(%"Array<i8>"* %"*self5", i8 %"$*other.data[]")
  %"$i8" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i8", 1
  store i32 %ADD, i32* %i, align 4
  br label %while6

endloop:                                          ; preds = %while6
  ret void
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @free(i8* %"$*self.data")
  ret void
}

define void @Text.reserve(%Text* %0, i32 %1) {
entry:
  %self = alloca %Text*, align 8
  %cap = alloca i32, align 4
  store %Text* %0, %Text** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if7

if7:                                              ; preds = %entry
  %"*self" = load %Text*, %Text** %self, align 8
  %"*self.cap" = getelementptr inbounds %Text, %Text* %"*self", i32 0, i32 2
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap", %"$*self.cap"
  br i1 %LE, label %if7.then, label %endif

if7.then:                                         ; preds = %if7
  ret void

endif:                                            ; preds = %if7
  %"*self1" = load %Text*, %Text** %self, align 8
  %"*self.cap2" = getelementptr inbounds %Text, %Text* %"*self1", i32 0, i32 2
  %"$cap3" = load i32, i32* %cap, align 4
  store i32 %"$cap3", i32* %"*self.cap2", align 4
  %"*self4" = load %Text*, %Text** %self, align 8
  %"*self.data" = getelementptr inbounds %Text, %Text* %"*self4", i32 0, i32 0
  %"*self5" = load %Text*, %Text** %self, align 8
  %"*self.data6" = getelementptr inbounds %Text, %Text* %"*self5", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data6", align 8
  %"$cap7" = load i32, i32* %cap, align 4
  %MUL = mul i32 %"$cap7", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %call.realloc = call i8* @realloc(i8* %"$*self.data", i32 %MUL)
  store i8* %call.realloc, i8** %"*self.data", align 8
  ret void
}

define void @Text.resize(%Text* %0, i32 %1) {
entry:
  %self = alloca %Text*, align 8
  %size = alloca i32, align 4
  %zero = alloca i8, align 1
  store %Text* %0, %Text** %self, align 8
  store i32 %1, i32* %size, align 4
  %"*self" = load %Text*, %Text** %self, align 8
  %"$size" = load i32, i32* %size, align 4
  call void @Text.reserve(%Text* %"*self", i32 %"$size")
  br label %while8

while8:                                           ; preds = %while8.then, %entry
  %"*self1" = load %Text*, %Text** %self, align 8
  %"*self.count" = getelementptr inbounds %Text, %Text* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$size2" = load i32, i32* %size, align 4
  %LT = icmp slt i32 %"$*self.count", %"$size2"
  br i1 %LT, label %while8.then, label %endloop

while8.then:                                      ; preds = %while8
  store i8 0, i8* %zero, align 1
  %"*self3" = load %Text*, %Text** %self, align 8
  %"*self.data" = getelementptr inbounds %Text, %Text* %"*self3", i32 0, i32 0
  %"*self4" = load %Text*, %Text** %self, align 8
  %"*self.count5" = getelementptr inbounds %Text, %Text* %"*self4", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count6"
  %"$zero" = load i8, i8* %zero, align 1
  store i8 %"$zero", i8* %"*self.data[]", align 1
  %"*self7" = load %Text*, %Text** %self, align 8
  %"*self.count8" = getelementptr inbounds %Text, %Text* %"*self7", i32 0, i32 1
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %ADD = add i32 %"$*self.count9", 1
  store i32 %ADD, i32* %"*self.count8", align 4
  br label %while8

endloop:                                          ; preds = %while8
  %"*self10" = load %Text*, %Text** %self, align 8
  %"*self.count11" = getelementptr inbounds %Text, %Text* %"*self10", i32 0, i32 1
  %"$size12" = load i32, i32* %size, align 4
  store i32 %"$size12", i32* %"*self.count11", align 4
  ret void
}

define void @Text.append(%Text* %0, i8 %1) {
entry:
  %self = alloca %Text*, align 8
  %value = alloca i8, align 1
  store %Text* %0, %Text** %self, align 8
  store i8 %1, i8* %value, align 1
  %"*self" = load %Text*, %Text** %self, align 8
  %"$value" = load i8, i8* %value, align 1
  call void @Text.push_back(%Text* %"*self", i8 %"$value")
  ret void
}

define void @Text.insert(%Text* %0, i32 %1, i8 %2) {
entry:
  %self = alloca %Text*, align 8
  %index = alloca i32, align 4
  %value = alloca i8, align 1
  %i = alloca i32, align 4
  store %Text* %0, %Text** %self, align 8
  store i32 %1, i32* %index, align 4
  store i8 %2, i8* %value, align 1
  %"*self" = load %Text*, %Text** %self, align 8
  %"$value" = load i8, i8* %value, align 1
  call void @Text.push_back(%Text* %"*self", i8 %"$value")
  %"*self1" = load %Text*, %Text** %self, align 8
  %"*self.count" = getelementptr inbounds %Text, %Text* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  store i32 %SUB, i32* %i, align 4
  br label %while9

while9:                                           ; preds = %while9.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %"$index" = load i32, i32* %index, align 4
  %GT = icmp sgt i32 %"$i", %"$index"
  br i1 %GT, label %while9.then, label %endloop

while9.then:                                      ; preds = %while9
  %"*self2" = load %Text*, %Text** %self, align 8
  %"*self.data" = getelementptr inbounds %Text, %Text* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i3" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i3"
  %"*self4" = load %Text*, %Text** %self, align 8
  %"*self.data5" = getelementptr inbounds %Text, %Text* %"*self4", i32 0, i32 0
  %"$i6" = load i32, i32* %i, align 4
  %SUB7 = sub i32 %"$i6", 1
  %"$*self.data8" = load i8*, i8** %"*self.data5", align 8
  %"*self.data[]9" = getelementptr inbounds i8, i8* %"$*self.data8", i32 %SUB7
  %"$*self.data[]" = load i8, i8* %"*self.data[]9", align 1
  store i8 %"$*self.data[]", i8* %"*self.data[]", align 1
  %"$i10" = load i32, i32* %i, align 4
  %SUB11 = sub i32 %"$i10", 1
  store i32 %SUB11, i32* %i, align 4
  br label %while9

endloop:                                          ; preds = %while9
  %"*self12" = load %Text*, %Text** %self, align 8
  %"*self.data13" = getelementptr inbounds %Text, %Text* %"*self12", i32 0, i32 0
  %"$*self.data14" = load i8*, i8** %"*self.data13", align 8
  %"$index15" = load i32, i32* %index, align 4
  %"*self.data[]16" = getelementptr inbounds i8, i8* %"$*self.data14", i32 %"$index15"
  %"$value17" = load i8, i8* %value, align 1
  store i8 %"$value17", i8* %"*self.data[]16", align 1
  ret void
}

define void @Text.push_front(%Text* %0, i8 %1) {
entry:
  %self = alloca %Text*, align 8
  %value = alloca i8, align 1
  store %Text* %0, %Text** %self, align 8
  store i8 %1, i8* %value, align 1
  %"*self" = load %Text*, %Text** %self, align 8
  %"$value" = load i8, i8* %value, align 1
  call void @Text.insert(%Text* %"*self", i32 0, i8 %"$value")
  ret void
}

define i8 @Text.remove(%Text* %0, i32 %1) {
entry:
  %self = alloca %Text*, align 8
  %index = alloca i32, align 4
  %value = alloca i8, align 1
  %i = alloca i32, align 4
  store %Text* %0, %Text** %self, align 8
  store i32 %1, i32* %index, align 4
  %"*self" = load %Text*, %Text** %self, align 8
  %"*self.data" = getelementptr inbounds %Text, %Text* %"*self", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$index"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  store i8 %"$*self.data[]", i8* %value, align 1
  %"$index1" = load i32, i32* %index, align 4
  store i32 %"$index1", i32* %i, align 4
  br label %while10

while10:                                          ; preds = %while10.then, %entry
  %"*self2" = load %Text*, %Text** %self, align 8
  %"*self.count" = getelementptr inbounds %Text, %Text* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", %SUB
  br i1 %LT, label %while10.then, label %endloop

while10.then:                                     ; preds = %while10
  %"*self3" = load %Text*, %Text** %self, align 8
  %"*self.data4" = getelementptr inbounds %Text, %Text* %"*self3", i32 0, i32 0
  %"$*self.data5" = load i8*, i8** %"*self.data4", align 8
  %"$i6" = load i32, i32* %i, align 4
  %"*self.data[]7" = getelementptr inbounds i8, i8* %"$*self.data5", i32 %"$i6"
  %"*self8" = load %Text*, %Text** %self, align 8
  %"*self.data9" = getelementptr inbounds %Text, %Text* %"*self8", i32 0, i32 0
  %"$i10" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i10", 1
  %"$*self.data11" = load i8*, i8** %"*self.data9", align 8
  %"*self.data[]12" = getelementptr inbounds i8, i8* %"$*self.data11", i32 %ADD
  %"$*self.data[]13" = load i8, i8* %"*self.data[]12", align 1
  store i8 %"$*self.data[]13", i8* %"*self.data[]7", align 1
  %"$i14" = load i32, i32* %i, align 4
  %ADD15 = add i32 %"$i14", 1
  store i32 %ADD15, i32* %i, align 4
  br label %while10

endloop:                                          ; preds = %while10
  %"*self16" = load %Text*, %Text** %self, align 8
  %"*self.count17" = getelementptr inbounds %Text, %Text* %"*self16", i32 0, i32 1
  %"$*self.count18" = load i32, i32* %"*self.count17", align 4
  %SUB19 = sub i32 %"$*self.count18", 1
  store i32 %SUB19, i32* %"*self.count17", align 4
  %"$value" = load i8, i8* %value, align 1
  ret i8 %"$value"
}

define i8 @Text.pop_back(%Text* %0) {
entry:
  %self = alloca %Text*, align 8
  store %Text* %0, %Text** %self, align 8
  %"*self" = load %Text*, %Text** %self, align 8
  %"*self.count" = getelementptr inbounds %Text, %Text* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  store i32 %SUB, i32* %"*self.count", align 4
  %"*self1" = load %Text*, %Text** %self, align 8
  %"*self.data" = getelementptr inbounds %Text, %Text* %"*self1", i32 0, i32 0
  %"*self2" = load %Text*, %Text** %self, align 8
  %"*self.count3" = getelementptr inbounds %Text, %Text* %"*self2", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count4" = load i32, i32* %"*self.count3", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count4"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  ret i8 %"$*self.data[]"
}

define i8 @Text.pop(%Text* %0) {
entry:
  %self = alloca %Text*, align 8
  store %Text* %0, %Text** %self, align 8
  %"*self" = load %Text*, %Text** %self, align 8
  %call.Text.pop_back = call i8 @Text.pop_back(%Text* %"*self")
  ret i8 %call.Text.pop_back
}

define i8 @Text.pop_front(%Text* %0) {
entry:
  %self = alloca %Text*, align 8
  store %Text* %0, %Text** %self, align 8
  %"*self" = load %Text*, %Text** %self, align 8
  %call.Text.remove = call i8 @Text.remove(%Text* %"*self", i32 0)
  ret i8 %call.Text.remove
}

define i8* @Text.front(%Text* %0) {
entry:
  %self = alloca %Text*, align 8
  store %Text* %0, %Text** %self, align 8
  %"*self" = load %Text*, %Text** %self, align 8
  %"*self.data" = getelementptr inbounds %Text, %Text* %"*self", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 0
  ret i8* %"*self.data[]"
}

define i8* @Text.back(%Text* %0) {
entry:
  %self = alloca %Text*, align 8
  store %Text* %0, %Text** %self, align 8
  %"*self" = load %Text*, %Text** %self, align 8
  %"*self.data" = getelementptr inbounds %Text, %Text* %"*self", i32 0, i32 0
  %"*self1" = load %Text*, %Text** %self, align 8
  %"*self.count" = getelementptr inbounds %Text, %Text* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %SUB
  ret i8* %"*self.data[]"
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

define i32 @Text.capacity(%Text* %0) {
entry:
  %self = alloca %Text*, align 8
  store %Text* %0, %Text** %self, align 8
  %"*self" = load %Text*, %Text** %self, align 8
  %"*self.cap" = getelementptr inbounds %Text, %Text* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  ret i32 %"$*self.cap"
}

define i1 @Text.empty(%Text* %0) {
entry:
  %self = alloca %Text*, align 8
  store %Text* %0, %Text** %self, align 8
  %"*self" = load %Text*, %Text** %self, align 8
  %"*self.count" = getelementptr inbounds %Text, %Text* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %EQ = icmp eq i32 %"$*self.count", 0
  ret i1 %EQ
}

define void @Text.clear(%Text* %0) {
entry:
  %self = alloca %Text*, align 8
  store %Text* %0, %Text** %self, align 8
  %"*self" = load %Text*, %Text** %self, align 8
  %"*self.count" = getelementptr inbounds %Text, %Text* %"*self", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  ret void
}

define void @Text.reverse(%Text* %0) {
entry:
  %self = alloca %Text*, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %tmp = alloca i8, align 1
  store %Text* %0, %Text** %self, align 8
  store i32 0, i32* %i, align 4
  %"*self" = load %Text*, %Text** %self, align 8
  %"*self.count" = getelementptr inbounds %Text, %Text* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  store i32 %SUB, i32* %j, align 4
  br label %while11

while11:                                          ; preds = %while11.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %"$j" = load i32, i32* %j, align 4
  %LT = icmp slt i32 %"$i", %"$j"
  br i1 %LT, label %while11.then, label %endloop

while11.then:                                     ; preds = %while11
  %"*self1" = load %Text*, %Text** %self, align 8
  %"*self.data" = getelementptr inbounds %Text, %Text* %"*self1", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i2" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i2"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  store i8 %"$*self.data[]", i8* %tmp, align 1
  %"*self3" = load %Text*, %Text** %self, align 8
  %"*self.data4" = getelementptr inbounds %Text, %Text* %"*self3", i32 0, i32 0
  %"$*self.data5" = load i8*, i8** %"*self.data4", align 8
  %"$i6" = load i32, i32* %i, align 4
  %"*self.data[]7" = getelementptr inbounds i8, i8* %"$*self.data5", i32 %"$i6"
  %"*self8" = load %Text*, %Text** %self, align 8
  %"*self.data9" = getelementptr inbounds %Text, %Text* %"*self8", i32 0, i32 0
  %"$*self.data10" = load i8*, i8** %"*self.data9", align 8
  %"$j11" = load i32, i32* %j, align 4
  %"*self.data[]12" = getelementptr inbounds i8, i8* %"$*self.data10", i32 %"$j11"
  %"$*self.data[]13" = load i8, i8* %"*self.data[]12", align 1
  store i8 %"$*self.data[]13", i8* %"*self.data[]7", align 1
  %"*self14" = load %Text*, %Text** %self, align 8
  %"*self.data15" = getelementptr inbounds %Text, %Text* %"*self14", i32 0, i32 0
  %"$*self.data16" = load i8*, i8** %"*self.data15", align 8
  %"$j17" = load i32, i32* %j, align 4
  %"*self.data[]18" = getelementptr inbounds i8, i8* %"$*self.data16", i32 %"$j17"
  %"$tmp" = load i8, i8* %tmp, align 1
  store i8 %"$tmp", i8* %"*self.data[]18", align 1
  %"$i19" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i19", 1
  store i32 %ADD, i32* %i, align 4
  %"$j20" = load i32, i32* %j, align 4
  %SUB21 = sub i32 %"$j20", 1
  store i32 %SUB21, i32* %j, align 4
  br label %while11

endloop:                                          ; preds = %while11
  ret void
}

define void @Text.release(%Text* %0) {
entry:
  %self = alloca %Text*, align 8
  %nothing = alloca i8*, align 8
  store %Text* %0, %Text** %self, align 8
  %"*self" = load %Text*, %Text** %self, align 8
  %"*self.data" = getelementptr inbounds %Text, %Text* %"*self", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @free(i8* %"$*self.data")
  store i8* null, i8** %nothing, align 8
  %"*self1" = load %Text*, %Text** %self, align 8
  %"*self.data2" = getelementptr inbounds %Text, %Text* %"*self1", i32 0, i32 0
  %"$nothing" = load i8*, i8** %nothing, align 8
  store i8* %"$nothing", i8** %"*self.data2", align 8
  %"*self3" = load %Text*, %Text** %self, align 8
  %"*self.count" = getelementptr inbounds %Text, %Text* %"*self3", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  %"*self4" = load %Text*, %Text** %self, align 8
  %"*self.cap" = getelementptr inbounds %Text, %Text* %"*self4", i32 0, i32 2
  store i32 0, i32* %"*self.cap", align 4
  ret void
}

define i8* @"Text.[]"(%Text* %0, i32 %1) {
entry:
  %self = alloca %Text*, align 8
  %index = alloca i32, align 4
  store %Text* %0, %Text** %self, align 8
  store i32 %1, i32* %index, align 4
  %"*self" = load %Text*, %Text** %self, align 8
  %"*self.data" = getelementptr inbounds %Text, %Text* %"*self", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$index"
  ret i8* %"*self.data[]"
}

define void @"Text.="(%Text* %0, %Text* %1) {
entry:
  %self = alloca %Text*, align 8
  %other = alloca %Text*, align 8
  %i = alloca i32, align 4
  store %Text* %0, %Text** %self, align 8
  store %Text* %1, %Text** %other, align 8
  %"*self" = load %Text*, %Text** %self, align 8
  call void @Text.clear(%Text* %"*self")
  %"*self1" = load %Text*, %Text** %self, align 8
  %"*other" = load %Text*, %Text** %other, align 8
  %"*other.count" = getelementptr inbounds %Text, %Text* %"*other", i32 0, i32 1
  %"$*other.count" = load i32, i32* %"*other.count", align 4
  call void @Text.reserve(%Text* %"*self1", i32 %"$*other.count")
  store i32 0, i32* %i, align 4
  br label %while12

while12:                                          ; preds = %while12.then, %entry
  %"*other2" = load %Text*, %Text** %other, align 8
  %"*other.count3" = getelementptr inbounds %Text, %Text* %"*other2", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*other.count4" = load i32, i32* %"*other.count3", align 4
  %LT = icmp slt i32 %"$i", %"$*other.count4"
  br i1 %LT, label %while12.then, label %endloop

while12.then:                                     ; preds = %while12
  %"*self5" = load %Text*, %Text** %self, align 8
  %"*other6" = load %Text*, %Text** %other, align 8
  %"*other.data" = getelementptr inbounds %Text, %Text* %"*other6", i32 0, i32 0
  %"$*other.data" = load i8*, i8** %"*other.data", align 8
  %"$i7" = load i32, i32* %i, align 4
  %"*other.data[]" = getelementptr inbounds i8, i8* %"$*other.data", i32 %"$i7"
  %"$*other.data[]" = load i8, i8* %"*other.data[]", align 1
  call void @Text.push_back(%Text* %"*self5", i8 %"$*other.data[]")
  %"$i8" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i8", 1
  store i32 %ADD, i32* %i, align 4
  br label %while12

endloop:                                          ; preds = %while12
  ret void
}

define void @Text.drop(%Text* %0) {
entry:
  %self = alloca %Text*, align 8
  store %Text* %0, %Text** %self, align 8
  %"*self" = load %Text*, %Text** %self, align 8
  %"*self.data" = getelementptr inbounds %Text, %Text* %"*self", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @free(i8* %"$*self.data")
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

define void @Text.push_back(%Text* %0, i8 %1) {
entry:
  %self = alloca %Text*, align 8
  %c = alloca i8, align 1
  store %Text* %0, %Text** %self, align 8
  store i8 %1, i8* %c, align 1
  br label %if13

if13:                                             ; preds = %entry
  %"*self" = load %Text*, %Text** %self, align 8
  %"*self.count" = getelementptr inbounds %Text, %Text* %"*self", i32 0, i32 1
  %"*self2" = load %Text*, %Text** %self, align 8
  %"*self.cap" = getelementptr inbounds %Text, %Text* %"*self2", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.count", %"$*self.cap"
  br i1 %EQ, label %if13.then, label %endif

if13.then:                                        ; preds = %if13
  %"*self3" = load %Text*, %Text** %self, align 8
  %"*self4" = load %Text*, %Text** %self, align 8
  %"*self.cap5" = getelementptr inbounds %Text, %Text* %"*self4", i32 0, i32 2
  %"$*self.cap6" = load i32, i32* %"*self.cap5", align 4
  %MUL = mul i32 %"$*self.cap6", 2
  %ADD = add i32 %MUL, 4
  call void @Text.reserve(%Text* %"*self3", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if13.then, %if13
  %"*self7" = load %Text*, %Text** %self, align 8
  %"*self.data" = getelementptr inbounds %Text, %Text* %"*self7", i32 0, i32 0
  %"*self8" = load %Text*, %Text** %self, align 8
  %"*self.count9" = getelementptr inbounds %Text, %Text* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  %"$c" = load i8, i8* %c, align 1
  store i8 %"$c", i8* %"*self.data[]", align 1
  %"*self11" = load %Text*, %Text** %self, align 8
  %"*self.count12" = getelementptr inbounds %Text, %Text* %"*self11", i32 0, i32 1
  %"$*self.count13" = load i32, i32* %"*self.count12", align 4
  %ADD14 = add i32 %"$*self.count13", 1
  store i32 %ADD14, i32* %"*self.count12", align 4
  br label %if14

if14:                                             ; preds = %endif
  %"$c15" = load i8, i8* %c, align 1
  %EQ16 = icmp eq i8 %"$c15", 32
  br i1 %EQ16, label %if14.then, label %endif1

if14.then:                                        ; preds = %if14
  %"*self17" = load %Text*, %Text** %self, align 8
  %"*self.words" = getelementptr inbounds %Text, %Text* %"*self17", i32 0, i32 4
  %"$*self.words" = load i32, i32* %"*self.words", align 4
  %ADD18 = add i32 %"$*self.words", 1
  store i32 %ADD18, i32* %"*self.words", align 4
  br label %endif1

endif1:                                           ; preds = %if14.then, %if14
  ret void
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
  br label %while15

while15:                                          ; preds = %endif, %entry
  %"*self" = load %Text*, %Text** %self, align 8
  %call.Text.size = call i32 @Text.size(%Text* %"*self")
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", %call.Text.size
  br i1 %LT, label %while15.then, label %endloop

while15.then:                                     ; preds = %while15
  %"*self1" = load %Text*, %Text** %self, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"call.Text.[]" = call i8* @"Text.[]"(%Text* %"*self1", i32 %"$i2")
  store i8* %"call.Text.[]", i8** %ref, align 8
  %"*ref" = load i8*, i8** %ref, align 8
  %"$*ref" = load i8, i8* %"*ref", align 1
  store i8 %"$*ref", i8* %c, align 1
  br label %if16

if16:                                             ; preds = %while15.then
  %"$c" = load i8, i8* %c, align 1
  %GE = icmp sge i8 %"$c", 97
  %"$c3" = load i8, i8* %c, align 1
  %LE = icmp sle i8 %"$c3", 122
  %AND = and i1 %GE, %LE
  br i1 %AND, label %if16.then, label %endif

if16.then:                                        ; preds = %if16
  %"$c4" = load i8, i8* %c, align 1
  %AS = sext i8 %"$c4" to i32
  %SUB = sub i32 %AS, 32
  %AS5 = trunc i32 %SUB to i8
  store i8 %AS5, i8* %c, align 1
  br label %endif

endif:                                            ; preds = %if16.then, %if16
  %"$c6" = load i8, i8* %c, align 1
  call void @Text.append(%Text* %res, i8 %"$c6")
  %"$i7" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i7", 1
  store i32 %ADD, i32* %i, align 4
  br label %while15

endloop:                                          ; preds = %while15
  %"$res" = load %Text, %Text* %res, align 8
  ret %Text %"$res"
}

define void @Title.reserve(%Title* %0, i32 %1) {
entry:
  %self = alloca %Title*, align 8
  %cap = alloca i32, align 4
  store %Title* %0, %Title** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if17

if17:                                             ; preds = %entry
  %"*self" = load %Title*, %Title** %self, align 8
  %"*self.cap" = getelementptr inbounds %Title, %Title* %"*self", i32 0, i32 2
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap", %"$*self.cap"
  br i1 %LE, label %if17.then, label %endif

if17.then:                                        ; preds = %if17
  ret void

endif:                                            ; preds = %if17
  %"*self1" = load %Title*, %Title** %self, align 8
  %"*self.cap2" = getelementptr inbounds %Title, %Title* %"*self1", i32 0, i32 2
  %"$cap3" = load i32, i32* %cap, align 4
  store i32 %"$cap3", i32* %"*self.cap2", align 4
  %"*self4" = load %Title*, %Title** %self, align 8
  %"*self.data" = getelementptr inbounds %Title, %Title* %"*self4", i32 0, i32 0
  %"*self5" = load %Title*, %Title** %self, align 8
  %"*self.data6" = getelementptr inbounds %Title, %Title* %"*self5", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data6", align 8
  %"$cap7" = load i32, i32* %cap, align 4
  %MUL = mul i32 %"$cap7", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %call.realloc = call i8* @realloc(i8* %"$*self.data", i32 %MUL)
  store i8* %call.realloc, i8** %"*self.data", align 8
  ret void
}

define void @Title.resize(%Title* %0, i32 %1) {
entry:
  %self = alloca %Title*, align 8
  %size = alloca i32, align 4
  %zero = alloca i8, align 1
  store %Title* %0, %Title** %self, align 8
  store i32 %1, i32* %size, align 4
  %"*self" = load %Title*, %Title** %self, align 8
  %"$size" = load i32, i32* %size, align 4
  call void @Title.reserve(%Title* %"*self", i32 %"$size")
  br label %while18

while18:                                          ; preds = %while18.then, %entry
  %"*self1" = load %Title*, %Title** %self, align 8
  %"*self.count" = getelementptr inbounds %Title, %Title* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$size2" = load i32, i32* %size, align 4
  %LT = icmp slt i32 %"$*self.count", %"$size2"
  br i1 %LT, label %while18.then, label %endloop

while18.then:                                     ; preds = %while18
  store i8 0, i8* %zero, align 1
  %"*self3" = load %Title*, %Title** %self, align 8
  %"*self.data" = getelementptr inbounds %Title, %Title* %"*self3", i32 0, i32 0
  %"*self4" = load %Title*, %Title** %self, align 8
  %"*self.count5" = getelementptr inbounds %Title, %Title* %"*self4", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count6"
  %"$zero" = load i8, i8* %zero, align 1
  store i8 %"$zero", i8* %"*self.data[]", align 1
  %"*self7" = load %Title*, %Title** %self, align 8
  %"*self.count8" = getelementptr inbounds %Title, %Title* %"*self7", i32 0, i32 1
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %ADD = add i32 %"$*self.count9", 1
  store i32 %ADD, i32* %"*self.count8", align 4
  br label %while18

endloop:                                          ; preds = %while18
  %"*self10" = load %Title*, %Title** %self, align 8
  %"*self.count11" = getelementptr inbounds %Title, %Title* %"*self10", i32 0, i32 1
  %"$size12" = load i32, i32* %size, align 4
  store i32 %"$size12", i32* %"*self.count11", align 4
  ret void
}

define void @Title.append(%Title* %0, i8 %1) {
entry:
  %self = alloca %Title*, align 8
  %value = alloca i8, align 1
  store %Title* %0, %Title** %self, align 8
  store i8 %1, i8* %value, align 1
  %"*self" = load %Title*, %Title** %self, align 8
  %"$value" = load i8, i8* %value, align 1
  call void @Title.push_back(%Title* %"*self", i8 %"$value")
  ret void
}

define void @Title.insert(%Title* %0, i32 %1, i8 %2) {
entry:
  %self = alloca %Title*, align 8
  %index = alloca i32, align 4
  %value = alloca i8, align 1
  %i = alloca i32, align 4
  store %Title* %0, %Title** %self, align 8
  store i32 %1, i32* %index, align 4
  store i8 %2, i8* %value, align 1
  %"*self" = load %Title*, %Title** %self, align 8
  %"$value" = load i8, i8* %value, align 1
  call void @Title.push_back(%Title* %"*self", i8 %"$value")
  %"*self1" = load %Title*, %Title** %self, align 8
  %"*self.count" = getelementptr inbounds %Title, %Title* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  store i32 %SUB, i32* %i, align 4
  br label %while19

while19:                                          ; preds = %while19.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %"$index" = load i32, i32* %index, align 4
  %GT = icmp sgt i32 %"$i", %"$index"
  br i1 %GT, label %while19.then, label %endloop

while19.then:                                     ; preds = %while19
  %"*self2" = load %Title*, %Title** %self, align 8
  %"*self.data" = getelementptr inbounds %Title, %Title* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i3" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i3"
  %"*self4" = load %Title*, %Title** %self, align 8
  %"*self.data5" = getelementptr inbounds %Title, %Title* %"*self4", i32 0, i32 0
  %"$i6" = load i32, i32* %i, align 4
  %SUB7 = sub i32 %"$i6", 1
  %"$*self.data8" = load i8*, i8** %"*self.data5", align 8
  %"*self.data[]9" = getelementptr inbounds i8, i8* %"$*self.data8", i32 %SUB7
  %"$*self.data[]" = load i8, i8* %"*self.data[]9", align 1
  store i8 %"$*self.data[]", i8* %"*self.data[]", align 1
  %"$i10" = load i32, i32* %i, align 4
  %SUB11 = sub i32 %"$i10", 1
  store i32 %SUB11, i32* %i, align 4
  br label %while19

endloop:                                          ; preds = %while19
  %"*self12" = load %Title*, %Title** %self, align 8
  %"*self.data13" = getelementptr inbounds %Title, %Title* %"*self12", i32 0, i32 0
  %"$*self.data14" = load i8*, i8** %"*self.data13", align 8
  %"$index15" = load i32, i32* %index, align 4
  %"*self.data[]16" = getelementptr inbounds i8, i8* %"$*self.data14", i32 %"$index15"
  %"$value17" = load i8, i8* %value, align 1
  store i8 %"$value17", i8* %"*self.data[]16", align 1
  ret void
}

define void @Title.push_front(%Title* %0, i8 %1) {
entry:
  %self = alloca %Title*, align 8
  %value = alloca i8, align 1
  store %Title* %0, %Title** %self, align 8
  store i8 %1, i8* %value, align 1
  %"*self" = load %Title*, %Title** %self, align 8
  %"$value" = load i8, i8* %value, align 1
  call void @Title.insert(%Title* %"*self", i32 0, i8 %"$value")
  ret void
}

define i8 @Title.remove(%Title* %0, i32 %1) {
entry:
  %self = alloca %Title*, align 8
  %index = alloca i32, align 4
  %value = alloca i8, align 1
  %i = alloca i32, align 4
  store %Title* %0, %Title** %self, align 8
  store i32 %1, i32* %index, align 4
  %"*self" = load %Title*, %Title** %self, align 8
  %"*self.data" = getelementptr inbounds %Title, %Title* %"*self", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$index"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  store i8 %"$*self.data[]", i8* %value, align 1
  %"$index1" = load i32, i32* %index, align 4
  store i32 %"$index1", i32* %i, align 4
  br label %while20

while20:                                          ; preds = %while20.then, %entry
  %"*self2" = load %Title*, %Title** %self, align 8
  %"*self.count" = getelementptr inbounds %Title, %Title* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", %SUB
  br i1 %LT, label %while20.then, label %endloop

while20.then:                                     ; preds = %while20
  %"*self3" = load %Title*, %Title** %self, align 8
  %"*self.data4" = getelementptr inbounds %Title, %Title* %"*self3", i32 0, i32 0
  %"$*self.data5" = load i8*, i8** %"*self.data4", align 8
  %"$i6" = load i32, i32* %i, align 4
  %"*self.data[]7" = getelementptr inbounds i8, i8* %"$*self.data5", i32 %"$i6"
  %"*self8" = load %Title*, %Title** %self, align 8
  %"*self.data9" = getelementptr inbounds %Title, %Title* %"*self8", i32 0, i32 0
  %"$i10" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i10", 1
  %"$*self.data11" = load i8*, i8** %"*self.data9", align 8
  %"*self.data[]12" = getelementptr inbounds i8, i8* %"$*self.data11", i32 %ADD
  %"$*self.data[]13" = load i8, i8* %"*self.data[]12", align 1
  store i8 %"$*self.data[]13", i8* %"*self.data[]7", align 1
  %"$i14" = load i32, i32* %i, align 4
  %ADD15 = add i32 %"$i14", 1
  store i32 %ADD15, i32* %i, align 4
  br label %while20

endloop:                                          ; preds = %while20
  %"*self16" = load %Title*, %Title** %self, align 8
  %"*self.count17" = getelementptr inbounds %Title, %Title* %"*self16", i32 0, i32 1
  %"$*self.count18" = load i32, i32* %"*self.count17", align 4
  %SUB19 = sub i32 %"$*self.count18", 1
  store i32 %SUB19, i32* %"*self.count17", align 4
  %"$value" = load i8, i8* %value, align 1
  ret i8 %"$value"
}

define i8 @Title.pop_back(%Title* %0) {
entry:
  %self = alloca %Title*, align 8
  store %Title* %0, %Title** %self, align 8
  %"*self" = load %Title*, %Title** %self, align 8
  %"*self.count" = getelementptr inbounds %Title, %Title* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  store i32 %SUB, i32* %"*self.count", align 4
  %"*self1" = load %Title*, %Title** %self, align 8
  %"*self.data" = getelementptr inbounds %Title, %Title* %"*self1", i32 0, i32 0
  %"*self2" = load %Title*, %Title** %self, align 8
  %"*self.count3" = getelementptr inbounds %Title, %Title* %"*self2", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count4" = load i32, i32* %"*self.count3", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count4"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  ret i8 %"$*self.data[]"
}

define i8 @Title.pop(%Title* %0) {
entry:
  %self = alloca %Title*, align 8
  store %Title* %0, %Title** %self, align 8
  %"*self" = load %Title*, %Title** %self, align 8
  %call.Title.pop_back = call i8 @Title.pop_back(%Title* %"*self")
  ret i8 %call.Title.pop_back
}

define i8 @Title.pop_front(%Title* %0) {
entry:
  %self = alloca %Title*, align 8
  store %Title* %0, %Title** %self, align 8
  %"*self" = load %Title*, %Title** %self, align 8
  %call.Title.remove = call i8 @Title.remove(%Title* %"*self", i32 0)
  ret i8 %call.Title.remove
}

define i8* @Title.front(%Title* %0) {
entry:
  %self = alloca %Title*, align 8
  store %Title* %0, %Title** %self, align 8
  %"*self" = load %Title*, %Title** %self, align 8
  %"*self.data" = getelementptr inbounds %Title, %Title* %"*self", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 0
  ret i8* %"*self.data[]"
}

define i8* @Title.back(%Title* %0) {
entry:
  %self = alloca %Title*, align 8
  store %Title* %0, %Title** %self, align 8
  %"*self" = load %Title*, %Title** %self, align 8
  %"*self.data" = getelementptr inbounds %Title, %Title* %"*self", i32 0, i32 0
  %"*self1" = load %Title*, %Title** %self, align 8
  %"*self.count" = getelementptr inbounds %Title, %Title* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %SUB
  ret i8* %"*self.data[]"
}

define i32 @Title.size(%Title* %0) {
entry:
  %self = alloca %Title*, align 8
  store %Title* %0, %Title** %self, align 8
  %"*self" = load %Title*, %Title** %self, align 8
  %"*self.count" = getelementptr inbounds %Title, %Title* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i32 @Title.capacity(%Title* %0) {
entry:
  %self = alloca %Title*, align 8
  store %Title* %0, %Title** %self, align 8
  %"*self" = load %Title*, %Title** %self, align 8
  %"*self.cap" = getelementptr inbounds %Title, %Title* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  ret i32 %"$*self.cap"
}

define i1 @Title.empty(%Title* %0) {
entry:
  %self = alloca %Title*, align 8
  store %Title* %0, %Title** %self, align 8
  %"*self" = load %Title*, %Title** %self, align 8
  %"*self.count" = getelementptr inbounds %Title, %Title* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %EQ = icmp eq i32 %"$*self.count", 0
  ret i1 %EQ
}

define void @Title.clear(%Title* %0) {
entry:
  %self = alloca %Title*, align 8
  store %Title* %0, %Title** %self, align 8
  %"*self" = load %Title*, %Title** %self, align 8
  %"*self.count" = getelementptr inbounds %Title, %Title* %"*self", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  ret void
}

define void @Title.reverse(%Title* %0) {
entry:
  %self = alloca %Title*, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %tmp = alloca i8, align 1
  store %Title* %0, %Title** %self, align 8
  store i32 0, i32* %i, align 4
  %"*self" = load %Title*, %Title** %self, align 8
  %"*self.count" = getelementptr inbounds %Title, %Title* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  store i32 %SUB, i32* %j, align 4
  br label %while21

while21:                                          ; preds = %while21.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %"$j" = load i32, i32* %j, align 4
  %LT = icmp slt i32 %"$i", %"$j"
  br i1 %LT, label %while21.then, label %endloop

while21.then:                                     ; preds = %while21
  %"*self1" = load %Title*, %Title** %self, align 8
  %"*self.data" = getelementptr inbounds %Title, %Title* %"*self1", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i2" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i2"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  store i8 %"$*self.data[]", i8* %tmp, align 1
  %"*self3" = load %Title*, %Title** %self, align 8
  %"*self.data4" = getelementptr inbounds %Title, %Title* %"*self3", i32 0, i32 0
  %"$*self.data5" = load i8*, i8** %"*self.data4", align 8
  %"$i6" = load i32, i32* %i, align 4
  %"*self.data[]7" = getelementptr inbounds i8, i8* %"$*self.data5", i32 %"$i6"
  %"*self8" = load %Title*, %Title** %self, align 8
  %"*self.data9" = getelementptr inbounds %Title, %Title* %"*self8", i32 0, i32 0
  %"$*self.data10" = load i8*, i8** %"*self.data9", align 8
  %"$j11" = load i32, i32* %j, align 4
  %"*self.data[]12" = getelementptr inbounds i8, i8* %"$*self.data10", i32 %"$j11"
  %"$*self.data[]13" = load i8, i8* %"*self.data[]12", align 1
  store i8 %"$*self.data[]13", i8* %"*self.data[]7", align 1
  %"*self14" = load %Title*, %Title** %self, align 8
  %"*self.data15" = getelementptr inbounds %Title, %Title* %"*self14", i32 0, i32 0
  %"$*self.data16" = load i8*, i8** %"*self.data15", align 8
  %"$j17" = load i32, i32* %j, align 4
  %"*self.data[]18" = getelementptr inbounds i8, i8* %"$*self.data16", i32 %"$j17"
  %"$tmp" = load i8, i8* %tmp, align 1
  store i8 %"$tmp", i8* %"*self.data[]18", align 1
  %"$i19" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i19", 1
  store i32 %ADD, i32* %i, align 4
  %"$j20" = load i32, i32* %j, align 4
  %SUB21 = sub i32 %"$j20", 1
  store i32 %SUB21, i32* %j, align 4
  br label %while21

endloop:                                          ; preds = %while21
  ret void
}

define void @Title.release(%Title* %0) {
entry:
  %self = alloca %Title*, align 8
  %nothing = alloca i8*, align 8
  store %Title* %0, %Title** %self, align 8
  %"*self" = load %Title*, %Title** %self, align 8
  %"*self.data" = getelementptr inbounds %Title, %Title* %"*self", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @free(i8* %"$*self.data")
  store i8* null, i8** %nothing, align 8
  %"*self1" = load %Title*, %Title** %self, align 8
  %"*self.data2" = getelementptr inbounds %Title, %Title* %"*self1", i32 0, i32 0
  %"$nothing" = load i8*, i8** %nothing, align 8
  store i8* %"$nothing", i8** %"*self.data2", align 8
  %"*self3" = load %Title*, %Title** %self, align 8
  %"*self.count" = getelementptr inbounds %Title, %Title* %"*self3", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  %"*self4" = load %Title*, %Title** %self, align 8
  %"*self.cap" = getelementptr inbounds %Title, %Title* %"*self4", i32 0, i32 2
  store i32 0, i32* %"*self.cap", align 4
  ret void
}

define i8* @"Title.[]"(%Title* %0, i32 %1) {
entry:
  %self = alloca %Title*, align 8
  %index = alloca i32, align 4
  store %Title* %0, %Title** %self, align 8
  store i32 %1, i32* %index, align 4
  %"*self" = load %Title*, %Title** %self, align 8
  %"*self.data" = getelementptr inbounds %Title, %Title* %"*self", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$index"
  ret i8* %"*self.data[]"
}

define void @"Title.="(%Title* %0, %Title* %1) {
entry:
  %self = alloca %Title*, align 8
  %other = alloca %Title*, align 8
  %i = alloca i32, align 4
  store %Title* %0, %Title** %self, align 8
  store %Title* %1, %Title** %other, align 8
  %"*self" = load %Title*, %Title** %self, align 8
  call void @Title.clear(%Title* %"*self")
  %"*self1" = load %Title*, %Title** %self, align 8
  %"*other" = load %Title*, %Title** %other, align 8
  %"*other.count" = getelementptr inbounds %Title, %Title* %"*other", i32 0, i32 1
  %"$*other.count" = load i32, i32* %"*other.count", align 4
  call void @Title.reserve(%Title* %"*self1", i32 %"$*other.count")
  store i32 0, i32* %i, align 4
  br label %while22

while22:                                          ; preds = %while22.then, %entry
  %"*other2" = load %Title*, %Title** %other, align 8
  %"*other.count3" = getelementptr inbounds %Title, %Title* %"*other2", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*other.count4" = load i32, i32* %"*other.count3", align 4
  %LT = icmp slt i32 %"$i", %"$*other.count4"
  br i1 %LT, label %while22.then, label %endloop

while22.then:                                     ; preds = %while22
  %"*self5" = load %Title*, %Title** %self, align 8
  %"*other6" = load %Title*, %Title** %other, align 8
  %"*other.data" = getelementptr inbounds %Title, %Title* %"*other6", i32 0, i32 0
  %"$*other.data" = load i8*, i8** %"*other.data", align 8
  %"$i7" = load i32, i32* %i, align 4
  %"*other.data[]" = getelementptr inbounds i8, i8* %"$*other.data", i32 %"$i7"
  %"$*other.data[]" = load i8, i8* %"*other.data[]", align 1
  call void @Title.push_back(%Title* %"*self5", i8 %"$*other.data[]")
  %"$i8" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i8", 1
  store i32 %ADD, i32* %i, align 4
  br label %while22

endloop:                                          ; preds = %while22
  ret void
}

define void @Title.drop(%Title* %0) {
entry:
  %self = alloca %Title*, align 8
  store %Title* %0, %Title** %self, align 8
  %"*self" = load %Title*, %Title** %self, align 8
  %"*self.data" = getelementptr inbounds %Title, %Title* %"*self", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @free(i8* %"$*self.data")
  ret void
}

define i8* @Title.name(%Title* %0) {
entry:
  %self = alloca %Title*, align 8
  store %Title* %0, %Title** %self, align 8
  %"*self" = load %Title*, %Title** %self, align 8
  %"*self.label" = getelementptr inbounds %Title, %Title* %"*self", i32 0, i32 3
  %"$*self.label" = load i8*, i8** %"*self.label", align 8
  ret i8* %"$*self.label"
}

define void @Title.push_back(%Title* %0, i8 %1) {
entry:
  %self = alloca %Title*, align 8
  %c = alloca i8, align 1
  store %Title* %0, %Title** %self, align 8
  store i8 %1, i8* %c, align 1
  br label %if23

if23:                                             ; preds = %entry
  %"*self" = load %Title*, %Title** %self, align 8
  %"*self.count" = getelementptr inbounds %Title, %Title* %"*self", i32 0, i32 1
  %"*self2" = load %Title*, %Title** %self, align 8
  %"*self.cap" = getelementptr inbounds %Title, %Title* %"*self2", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.count", %"$*self.cap"
  br i1 %EQ, label %if23.then, label %endif

if23.then:                                        ; preds = %if23
  %"*self3" = load %Title*, %Title** %self, align 8
  %"*self4" = load %Title*, %Title** %self, align 8
  %"*self.cap5" = getelementptr inbounds %Title, %Title* %"*self4", i32 0, i32 2
  %"$*self.cap6" = load i32, i32* %"*self.cap5", align 4
  %MUL = mul i32 %"$*self.cap6", 2
  %ADD = add i32 %MUL, 4
  call void @Title.reserve(%Title* %"*self3", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if23.then, %if23
  %"*self7" = load %Title*, %Title** %self, align 8
  %"*self.data" = getelementptr inbounds %Title, %Title* %"*self7", i32 0, i32 0
  %"*self8" = load %Title*, %Title** %self, align 8
  %"*self.count9" = getelementptr inbounds %Title, %Title* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  %"$c" = load i8, i8* %c, align 1
  store i8 %"$c", i8* %"*self.data[]", align 1
  %"*self11" = load %Title*, %Title** %self, align 8
  %"*self.count12" = getelementptr inbounds %Title, %Title* %"*self11", i32 0, i32 1
  %"$*self.count13" = load i32, i32* %"*self.count12", align 4
  %ADD14 = add i32 %"$*self.count13", 1
  store i32 %ADD14, i32* %"*self.count12", align 4
  br label %if24

if24:                                             ; preds = %endif
  %"$c15" = load i8, i8* %c, align 1
  %EQ16 = icmp eq i8 %"$c15", 32
  br i1 %EQ16, label %if24.then, label %endif1

if24.then:                                        ; preds = %if24
  %"*self17" = load %Title*, %Title** %self, align 8
  %"*self.words" = getelementptr inbounds %Title, %Title* %"*self17", i32 0, i32 4
  %"$*self.words" = load i32, i32* %"*self.words", align 4
  %ADD18 = add i32 %"$*self.words", 1
  store i32 %ADD18, i32* %"*self.words", align 4
  br label %endif1

endif1:                                           ; preds = %if24.then, %if24
  ret void
}

define %Title @Title.shout(%Title* %0) {
entry:
  %self = alloca %Title*, align 8
  %res = alloca %Title, align 8
  %i = alloca i32, align 4
  %c = alloca i8, align 1
  %ref = alloca i8*, align 8
  store %Title* %0, %Title** %self, align 8
  store %Title zeroinitializer, %Title* %res, align 8
  store i32 0, i32* %i, align 4
  br label %while25

while25:                                          ; preds = %endif, %entry
  %"*self" = load %Title*, %Title** %self, align 8
  %call.Title.size = call i32 @Title.size(%Title* %"*self")
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", %call.Title.size
  br i1 %LT, label %while25.then, label %endloop

while25.then:                                     ; preds = %while25
  %"*self1" = load %Title*, %Title** %self, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"call.Title.[]" = call i8* @"Title.[]"(%Title* %"*self1", i32 %"$i2")
  store i8* %"call.Title.[]", i8** %ref, align 8
  %"*ref" = load i8*, i8** %ref, align 8
  %"$*ref" = load i8, i8* %"*ref", align 1
  store i8 %"$*ref", i8* %c, align 1
  br label %if26

if26:                                             ; preds = %while25.then
  %"$c" = load i8, i8* %c, align 1
  %GE = icmp sge i8 %"$c", 97
  %"$c3" = load i8, i8* %c, align 1
  %LE = icmp sle i8 %"$c3", 122
  %AND = and i1 %GE, %LE
  br i1 %AND, label %if26.then, label %endif

if26.then:                                        ; preds = %if26
  %"$c4" = load i8, i8* %c, align 1
  %AS = sext i8 %"$c4" to i32
  %SUB = sub i32 %AS, 32
  %AS5 = trunc i32 %SUB to i8
  store i8 %AS5, i8* %c, align 1
  br label %endif

endif:                                            ; preds = %if26.then, %if26
  %"$c6" = load i8, i8* %c, align 1
  call void @Title.append(%Title* %res, i8 %"$c6")
  %"$i7" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i7", 1
  store i32 %ADD, i32* %i, align 4
  br label %while25

endloop:                                          ; preds = %while25
  %"$res" = load %Title, %Title* %res, align 8
  ret %Title %"$res"
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
  %arr = alloca [9 x i8], align 1
  %loud = alloca %Text, align 8
  %title = alloca %Title, align 8
  %arr1 = alloca [2 x i8], align 1
  %arr2 = alloca [2 x i8], align 1
  %ref = alloca i8*, align 8
  %ref4 = alloca i8*, align 8
  %ref7 = alloca i8*, align 8
  %arr9 = alloca [2 x i8], align 1
  %arr10 = alloca [2 x i8], align 1
  %arr11 = alloca [2 x i8], align 1
  %arr12 = alloca [2 x i8], align 1
  %arr18 = alloca [23 x i8], align 1
  store %Text zeroinitializer, %Text* %t, align 8
  %t.label = getelementptr inbounds %Text, %Text* %t, i32 0, i32 3
  store [9 x i8] c"greeting\00", [9 x i8]* %arr, align 1
  %0 = getelementptr inbounds [9 x i8], [9 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %t.label, align 8
  call void @Text.append(%Text* %t, i8 104)
  call void @Text.append(%Text* %t, i8 105)
  call void @Text.append(%Text* %t, i8 32)
  call void @Text.push_back(%Text* %t, i8 117)
  %call.Text.shout = call %Text @Text.shout(%Text* %t)
  store %Text %call.Text.shout, %Text* %loud, align 8
  store %Title zeroinitializer, %Title* %title, align 8
  call void @Title.append(%Title* %title, i8 122)
  %call.Text.size = call i32 @Text.size(%Text* %t)
  store [2 x i8] c" \00", [2 x i8]* %arr1, align 1
  %1 = getelementptr inbounds [2 x i8], [2 x i8]* %arr1, i32 0, i32 0
  %t.words = getelementptr inbounds %Text, %Text* %t, i32 0, i32 4
  store [2 x i8] c" \00", [2 x i8]* %arr2, align 1
  %2 = getelementptr inbounds [2 x i8], [2 x i8]* %arr2, i32 0, i32 0
  %"call.Text.[]" = call i8* @"Text.[]"(%Text* %loud, i32 0)
  store i8* %"call.Text.[]", i8** %ref, align 8
  %"*ref" = load i8*, i8** %ref, align 8
  %"call.Text.[]3" = call i8* @"Text.[]"(%Text* %loud, i32 1)
  store i8* %"call.Text.[]3", i8** %ref4, align 8
  %"*ref5" = load i8*, i8** %ref4, align 8
  %"call.Text.[]6" = call i8* @"Text.[]"(%Text* %loud, i32 3)
  store i8* %"call.Text.[]6", i8** %ref7, align 8
  %"*ref8" = load i8*, i8** %ref7, align 8
  store [2 x i8] c" \00", [2 x i8]* %arr9, align 1
  %3 = getelementptr inbounds [2 x i8], [2 x i8]* %arr9, i32 0, i32 0
  %call.Text.name = call i8* @Text.name(%Text* %t)
  store [2 x i8] c" \00", [2 x i8]* %arr10, align 1
  %4 = getelementptr inbounds [2 x i8], [2 x i8]* %arr10, i32 0, i32 0
  %call.Title.first = call i8 @Title.first(%Title* %title)
  store [2 x i8] c" \00", [2 x i8]* %arr11, align 1
  %5 = getelementptr inbounds [2 x i8], [2 x i8]* %arr11, i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr12, align 1
  %6 = getelementptr inbounds [2 x i8], [2 x i8]* %arr12, i32 0, i32 0
  %"$t.words" = load i32, i32* %t.words, align 4
  %"$*ref" = load i8, i8* %"*ref", align 1
  %AS = sext i8 %"$*ref" to i32
  %"$*ref13" = load i8, i8* %"*ref5", align 1
  %AS14 = sext i8 %"$*ref13" to i32
  %"$*ref15" = load i8, i8* %"*ref8", align 1
  %AS16 = sext i8 %"$*ref15" to i32
  %AS17 = sext i8 %call.Title.first to i32
  store [23 x i8] c"%d %d %c%c%c %s %c %s\0A\00", [23 x i8]* %arr18, align 1
  %7 = getelementptr inbounds [23 x i8], [23 x i8]* %arr18, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %7, i32 %call.Text.size, i32 %"$t.words", i32 %AS, i32 %AS14, i32 %AS16, i8* %call.Text.name, i32 %AS17, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @TYPEOF, i32 0, i32 0))
  call void @Title.drop(%Title* %title)
  call void @Text.drop(%Text* %loud)
  call void @Text.drop(%Text* %t)
  ret i32 0
}

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
    output("[", t.c_str(), "] ", t.size(), "\n")
    output(t.upper().c_str(), " ", t.lower().c_str(), " ", t.reverse().c_str(), "\n")
    output(t.find("World"), " ", t.contains("lo W"), " ", t.starts_with("Hell"), " ", t.ends_with("ld"), " ", t.find("xyz"), "\n")
    r String = t.replace("World", "ura")
    output(r.c_str(), " ", r.substr(1, 3).c_str(), " ", r.repeat(2).c_str(), "\n")
    a String = String::from("abc")
    b String = "abd"
    c String = a + "-" + b
    c += "!"
    output(c.c_str(), " ", a < b, " ", a == "abc", " ", a != b, " ", c.size(), "\n")
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

%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@TYPEOF = private unnamed_addr constant [7 x i8] c"String\00", align 1

declare i64 @strlen(i8*)

declare i32 @strnlen(i8*, i32)

declare i32 @strcmp(i8*, i8*)

declare i32 @strncmp(i8*, i8*, i32)

declare i32 @strcasecmp(i8*, i8*)

declare i32 @strncasecmp(i8*, i8*, i32)

declare i32 @strcoll(i8*, i8*)

declare i32 @strxfrm(i8*, i8*, i32)

declare i8* @strcpy(i8*, i8*)

declare i8* @strncpy(i8*, i8*, i32)

declare i8* @strdup(i8*)

declare i8* @strndup(i8*, i32)

declare i8* @strcat(i8*, i8*)

declare i8* @strncat(i8*, i8*, i32)

declare i8* @strchr(i8*, i32)

declare i8* @strrchr(i8*, i32)

declare i8* @strstr(i8*, i8*)

declare i8* @strcasestr(i8*, i8*)

declare i32 @strspn(i8*, i8*)

declare i32 @strcspn(i8*, i8*)

declare i8* @strpbrk(i8*, i8*)

declare i8* @strtok(i8*, i8*)

declare i8* @strtok_r(i8*, i8*, i8*)

declare i8* @strerror(i32)

declare i32 @strerror_r(i32, i8*, i64)

declare i8* @memchr(i8*, i32, i32)

declare i8* @memrchr(i8*, i32, i32)

declare i32 @memcmp(i8*, i8*, i32)

declare i8* @memcpy(i8*, i8*, i32)

declare i8* @memmove(i8*, i8*, i32)

declare i8* @memset(i8*, i32, i32)

declare i8* @memccpy(i8*, i8*, i32, i32)

declare void @bzero(i8*, i64)

declare i32 @strlcpy(i8*, i8*, i32)

declare i32 @strlcat(i8*, i8*, i32)

declare i8* @strsignal(i32)

declare i8* @memset_s(i8*, i32, i32, i32)

declare i32 @atoi(i8*)

declare i8* @realloc(i8*, i32)

declare void @free(i8*)

define void @"Array<i8>.reserve"(%"Array<i8>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %cap = alloca i32, align 4
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap", %"$*self.cap"
  br i1 %LE, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret void

endif:                                            ; preds = %if0
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap2" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 2
  %"$cap3" = load i32, i32* %cap, align 4
  store i32 %"$cap3", i32* %"*self.cap2", align 4
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 0
  %"*self5" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data6" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self5", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data6", align 8
  %"$cap7" = load i32, i32* %cap, align 4
  %MUL = mul i32 %"$cap7", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %call.realloc = call i8* @realloc(i8* %"$*self.data", i32 %MUL)
  store i8* %call.realloc, i8** %"*self.data", align 8
  ret void
}

define void @"Array<i8>.resize"(%"Array<i8>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %size = alloca i32, align 4
  %zero = alloca i8, align 1
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store i32 %1, i32* %size, align 4
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"$size" = load i32, i32* %size, align 4
  call void @"Array<i8>.reserve"(%"Array<i8>"* %"*self", i32 %"$size")
  br label %while1

while1:                                           ; preds = %while1.then, %entry
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$size2" = load i32, i32* %size, align 4
  %LT = icmp slt i32 %"$*self.count", %"$size2"
  br i1 %LT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  store i8 0, i8* %zero, align 1
  %"*self3" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self3", i32 0, i32 0
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count6"
  %"$zero" = load i8, i8* %zero, align 1
  store i8 %"$zero", i8* %"*self.data[]", align 1
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count8" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 1
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %ADD = add i32 %"$*self.count9", 1
  store i32 %ADD, i32* %"*self.count8", align 4
  br label %while1

endloop:                                          ; preds = %while1
  %"*self10" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self10", i32 0, i32 1
  %"$size12" = load i32, i32* %size, align 4
  store i32 %"$size12", i32* %"*self.count11", align 4
  ret void
}

define void @"Array<i8>.push_back"(%"Array<i8>"* %0, i8 %1) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %value = alloca i8, align 1
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store i8 %1, i8* %value, align 1
  br label %if2

if2:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.count", %"$*self.cap"
  br i1 %EQ, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self3" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap4" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self3", i32 0, i32 2
  %"$*self.cap5" = load i32, i32* %"*self.cap4", align 4
  %MUL = mul i32 %"$*self.cap5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<i8>.reserve"(%"Array<i8>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  %"*self6" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self6", i32 0, i32 0
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count8" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count9"
  %"$value" = load i8, i8* %value, align 1
  store i8 %"$value", i8* %"*self.data[]", align 1
  %"*self10" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  ret void
}

define void @"Array<i8>.append"(%"Array<i8>"* %0, i8 %1) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %value = alloca i8, align 1
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store i8 %1, i8* %value, align 1
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"$value" = load i8, i8* %value, align 1
  call void @"Array<i8>.push_back"(%"Array<i8>"* %"*self", i8 %"$value")
  ret void
}

define void @"Array<i8>.insert"(%"Array<i8>"* %0, i32 %1, i8 %2) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %index = alloca i32, align 4
  %value = alloca i8, align 1
  %i = alloca i32, align 4
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store i32 %1, i32* %index, align 4
  store i8 %2, i8* %value, align 1
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"$value" = load i8, i8* %value, align 1
  call void @"Array<i8>.push_back"(%"Array<i8>"* %"*self", i8 %"$value")
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  store i32 %SUB, i32* %i, align 4
  br label %while3

while3:                                           ; preds = %while3.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %"$index" = load i32, i32* %index, align 4
  %GT = icmp sgt i32 %"$i", %"$index"
  br i1 %GT, label %while3.then, label %endloop

while3.then:                                      ; preds = %while3
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i3" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i3"
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data5" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 0
  %"$i6" = load i32, i32* %i, align 4
  %SUB7 = sub i32 %"$i6", 1
  %"$*self.data8" = load i8*, i8** %"*self.data5", align 8
  %"*self.data[]9" = getelementptr inbounds i8, i8* %"$*self.data8", i32 %SUB7
  %"$*self.data[]" = load i8, i8* %"*self.data[]9", align 1
  store i8 %"$*self.data[]", i8* %"*self.data[]", align 1
  %"$i10" = load i32, i32* %i, align 4
  %SUB11 = sub i32 %"$i10", 1
  store i32 %SUB11, i32* %i, align 4
  br label %while3

endloop:                                          ; preds = %while3
  %"*self12" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data13" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self12", i32 0, i32 0
  %"$*self.data14" = load i8*, i8** %"*self.data13", align 8
  %"$index15" = load i32, i32* %index, align 4
  %"*self.data[]16" = getelementptr inbounds i8, i8* %"$*self.data14", i32 %"$index15"
  %"$value17" = load i8, i8* %value, align 1
  store i8 %"$value17", i8* %"*self.data[]16", align 1
  ret void
}

define void @"Array<i8>.push_front"(%"Array<i8>"* %0, i8 %1) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %value = alloca i8, align 1
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store i8 %1, i8* %value, align 1
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"$value" = load i8, i8* %value, align 1
  call void @"Array<i8>.insert"(%"Array<i8>"* %"*self", i32 0, i8 %"$value")
  ret void
}

define i8 @"Array<i8>.remove"(%"Array<i8>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %index = alloca i32, align 4
  %value = alloca i8, align 1
  %i = alloca i32, align 4
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store i32 %1, i32* %index, align 4
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$index"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  store i8 %"$*self.data[]", i8* %value, align 1
  %"$index1" = load i32, i32* %index, align 4
  store i32 %"$index1", i32* %i, align 4
  br label %while4

while4:                                           ; preds = %while4.then, %entry
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", %SUB
  br i1 %LT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
  %"*self3" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data4" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self3", i32 0, i32 0
  %"$*self.data5" = load i8*, i8** %"*self.data4", align 8
  %"$i6" = load i32, i32* %i, align 4
  %"*self.data[]7" = getelementptr inbounds i8, i8* %"$*self.data5", i32 %"$i6"
  %"*self8" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data9" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self8", i32 0, i32 0
  %"$i10" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i10", 1
  %"$*self.data11" = load i8*, i8** %"*self.data9", align 8
  %"*self.data[]12" = getelementptr inbounds i8, i8* %"$*self.data11", i32 %ADD
  %"$*self.data[]13" = load i8, i8* %"*self.data[]12", align 1
  store i8 %"$*self.data[]13", i8* %"*self.data[]7", align 1
  %"$i14" = load i32, i32* %i, align 4
  %ADD15 = add i32 %"$i14", 1
  store i32 %ADD15, i32* %i, align 4
  br label %while4

endloop:                                          ; preds = %while4
  %"*self16" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count17" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self16", i32 0, i32 1
  %"$*self.count18" = load i32, i32* %"*self.count17", align 4
  %SUB19 = sub i32 %"$*self.count18", 1
  store i32 %SUB19, i32* %"*self.count17", align 4
  %"$value" = load i8, i8* %value, align 1
  ret i8 %"$value"
}

define i8 @"Array<i8>.pop_back"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  store i32 %SUB, i32* %"*self.count", align 4
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 0
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count4" = load i32, i32* %"*self.count3", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count4"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  ret i8 %"$*self.data[]"
}

define i8 @"Array<i8>.pop"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"call.Array<i8>.pop_back" = call i8 @"Array<i8>.pop_back"(%"Array<i8>"* %"*self")
  ret i8 %"call.Array<i8>.pop_back"
}

define i8 @"Array<i8>.pop_front"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"call.Array<i8>.remove" = call i8 @"Array<i8>.remove"(%"Array<i8>"* %"*self", i32 0)
  ret i8 %"call.Array<i8>.remove"
}

define i8* @"Array<i8>.front"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 0
  ret i8* %"*self.data[]"
}

define i8* @"Array<i8>.back"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 0
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %SUB
  ret i8* %"*self.data[]"
}

define i32 @"Array<i8>.size"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i32 @"Array<i8>.capacity"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  ret i32 %"$*self.cap"
}

define i1 @"Array<i8>.empty"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %EQ = icmp eq i32 %"$*self.count", 0
  ret i1 %EQ
}

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  ret void
}

define void @"Array<i8>.reverse"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %tmp = alloca i8, align 1
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store i32 0, i32* %i, align 4
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  store i32 %SUB, i32* %j, align 4
  br label %while5

while5:                                           ; preds = %while5.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %"$j" = load i32, i32* %j, align 4
  %LT = icmp slt i32 %"$i", %"$j"
  br i1 %LT, label %while5.then, label %endloop

while5.then:                                      ; preds = %while5
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i2" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i2"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  store i8 %"$*self.data[]", i8* %tmp, align 1
  %"*self3" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data4" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self3", i32 0, i32 0
  %"$*self.data5" = load i8*, i8** %"*self.data4", align 8
  %"$i6" = load i32, i32* %i, align 4
  %"*self.data[]7" = getelementptr inbounds i8, i8* %"$*self.data5", i32 %"$i6"
  %"*self8" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data9" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self8", i32 0, i32 0
  %"$*self.data10" = load i8*, i8** %"*self.data9", align 8
  %"$j11" = load i32, i32* %j, align 4
  %"*self.data[]12" = getelementptr inbounds i8, i8* %"$*self.data10", i32 %"$j11"
  %"$*self.data[]13" = load i8, i8* %"*self.data[]12", align 1
  store i8 %"$*self.data[]13", i8* %"*self.data[]7", align 1
  %"*self14" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data15" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self14", i32 0, i32 0
  %"$*self.data16" = load i8*, i8** %"*self.data15", align 8
  %"$j17" = load i32, i32* %j, align 4
  %"*self.data[]18" = getelementptr inbounds i8, i8* %"$*self.data16", i32 %"$j17"
  %"$tmp" = load i8, i8* %tmp, align 1
  store i8 %"$tmp", i8* %"*self.data[]18", align 1
  %"$i19" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i19", 1
  store i32 %ADD, i32* %i, align 4
  %"$j20" = load i32, i32* %j, align 4
  %SUB21 = sub i32 %"$j20", 1
  store i32 %SUB21, i32* %j, align 4
  br label %while5

endloop:                                          ; preds = %while5
  ret void
}

define void @"Array<i8>.release"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %nothing = alloca i8*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @free(i8* %"$*self.data")
  store i8* null, i8** %nothing, align 8
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data2" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 0
  %"$nothing" = load i8*, i8** %nothing, align 8
  store i8* %"$nothing", i8** %"*self.data2", align 8
  %"*self3" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self3", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 2
  store i32 0, i32* %"*self.cap", align 4
  ret void
}

define i8* @"Array<i8>.[]"(%"Array<i8>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %index = alloca i32, align 4
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store i32 %1, i32* %index, align 4
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$index"
  ret i8* %"*self.data[]"
}

define void @"Array<i8>.="(%"Array<i8>"* %0, %"Array<i8>"* %1) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %other = alloca %"Array<i8>"*, align 8
  %i = alloca i32, align 4
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store %"Array<i8>"* %1, %"Array<i8>"** %other, align 8
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*self")
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*other" = load %"Array<i8>"*, %"Array<i8>"** %other, align 8
  %"*other.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*other", i32 0, i32 1
  %"$*other.count" = load i32, i32* %"*other.count", align 4
  call void @"Array<i8>.reserve"(%"Array<i8>"* %"*self1", i32 %"$*other.count")
  store i32 0, i32* %i, align 4
  br label %while6

while6:                                           ; preds = %while6.then, %entry
  %"*other2" = load %"Array<i8>"*, %"Array<i8>"** %other, align 8
  %"*other.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*other2", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*other.count4" = load i32, i32* %"*other.count3", align 4
  %LT = icmp slt i32 %"$i", %"$*other.count4"
  br i1 %LT, label %while6.then, label %endloop

while6.then:                                      ; preds = %while6
  %"*self5" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*other6" = load %"Array<i8>"*, %"Array<i8>"** %other, align 8
  %"*other.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*other6", i32 0, i32 0
  %"$*other.data" = load i8*, i8** %"*other.data", align 8
  %"$i7" = load i32, i32* %i, align 4
  %"*other.data[]" = getelementptr inbounds i8, i8* %"$*other.data", i32 %"$i7"
  %"$*other.data[]" = load i8, i8* %"*other.data[]", align 1
  call void @"Array<i8>.push_back"(%"Array<i8>"* %"*self5", i8 %"$*other.data[]")
  %"$i8" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i8", 1
  store i32 %ADD, i32* %i, align 4
  br label %while6

endloop:                                          ; preds = %while6
  ret void
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @free(i8* %"$*self.data")
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if7

if7:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap", %"$*self.cap"
  br i1 %LE, label %if7.then, label %endif

if7.then:                                         ; preds = %if7
  ret void

endif:                                            ; preds = %if7
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap2" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$cap3" = load i32, i32* %cap, align 4
  store i32 %"$cap3", i32* %"*self.cap2", align 4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 0
  %"*self5" = load %String*, %String** %self, align 8
  %"*self.data6" = getelementptr inbounds %String, %String* %"*self5", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data6", align 8
  %"$cap7" = load i32, i32* %cap, align 4
  %MUL = mul i32 %"$cap7", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %call.realloc = call i8* @realloc(i8* %"$*self.data", i32 %MUL)
  store i8* %call.realloc, i8** %"*self.data", align 8
  ret void
}

define void @String.resize(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %size = alloca i32, align 4
  %zero = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %size, align 4
  %"*self" = load %String*, %String** %self, align 8
  %"$size" = load i32, i32* %size, align 4
  call void @String.reserve(%String* %"*self", i32 %"$size")
  br label %while8

while8:                                           ; preds = %while8.then, %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$size2" = load i32, i32* %size, align 4
  %LT = icmp slt i32 %"$*self.count", %"$size2"
  br i1 %LT, label %while8.then, label %endloop

while8.then:                                      ; preds = %while8
  store i8 0, i8* %zero, align 1
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 0
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.count5" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count6"
  %"$zero" = load i8, i8* %zero, align 1
  store i8 %"$zero", i8* %"*self.data[]", align 1
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.count8" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 1
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %ADD = add i32 %"$*self.count9", 1
  store i32 %ADD, i32* %"*self.count8", align 4
  br label %while8

endloop:                                          ; preds = %while8
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$size12" = load i32, i32* %size, align 4
  store i32 %"$size12", i32* %"*self.count11", align 4
  ret void
}

define void @String.push_back(%String* %0, i8 %1) {
entry:
  %self = alloca %String*, align 8
  %value = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store i8 %1, i8* %value, align 1
  br label %if9

if9:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.count", %"$*self.cap"
  br i1 %EQ, label %if9.then, label %endif

if9.then:                                         ; preds = %if9
  %"*self2" = load %String*, %String** %self, align 8
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.cap4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 2
  %"$*self.cap5" = load i32, i32* %"*self.cap4", align 4
  %MUL = mul i32 %"$*self.cap5", 2
  %ADD = add i32 %MUL, 4
  call void @String.reserve(%String* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if9.then, %if9
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

define void @String.append(%String* %0, i8 %1) {
entry:
  %self = alloca %String*, align 8
  %value = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store i8 %1, i8* %value, align 1
  %"*self" = load %String*, %String** %self, align 8
  %"$value" = load i8, i8* %value, align 1
  call void @String.push_back(%String* %"*self", i8 %"$value")
  ret void
}

define void @String.insert(%String* %0, i32 %1, i8 %2) {
entry:
  %self = alloca %String*, align 8
  %index = alloca i32, align 4
  %value = alloca i8, align 1
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %index, align 4
  store i8 %2, i8* %value, align 1
  %"*self" = load %String*, %String** %self, align 8
  %"$value" = load i8, i8* %value, align 1
  call void @String.push_back(%String* %"*self", i8 %"$value")
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  store i32 %SUB, i32* %i, align 4
  br label %while10

while10:                                          ; preds = %while10.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %"$index" = load i32, i32* %index, align 4
  %GT = icmp sgt i32 %"$i", %"$index"
  br i1 %GT, label %while10.then, label %endloop

while10.then:                                     ; preds = %while10
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i3" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i3"
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.data5" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 0
  %"$i6" = load i32, i32* %i, align 4
  %SUB7 = sub i32 %"$i6", 1
  %"$*self.data8" = load i8*, i8** %"*self.data5", align 8
  %"*self.data[]9" = getelementptr inbounds i8, i8* %"$*self.data8", i32 %SUB7
  %"$*self.data[]" = load i8, i8* %"*self.data[]9", align 1
  store i8 %"$*self.data[]", i8* %"*self.data[]", align 1
  %"$i10" = load i32, i32* %i, align 4
  %SUB11 = sub i32 %"$i10", 1
  store i32 %SUB11, i32* %i, align 4
  br label %while10

endloop:                                          ; preds = %while10
  %"*self12" = load %String*, %String** %self, align 8
  %"*self.data13" = getelementptr inbounds %String, %String* %"*self12", i32 0, i32 0
  %"$*self.data14" = load i8*, i8** %"*self.data13", align 8
  %"$index15" = load i32, i32* %index, align 4
  %"*self.data[]16" = getelementptr inbounds i8, i8* %"$*self.data14", i32 %"$index15"
  %"$value17" = load i8, i8* %value, align 1
  store i8 %"$value17", i8* %"*self.data[]16", align 1
  ret void
}

define void @String.push_front(%String* %0, i8 %1) {
entry:
  %self = alloca %String*, align 8
  %value = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store i8 %1, i8* %value, align 1
  %"*self" = load %String*, %String** %self, align 8
  %"$value" = load i8, i8* %value, align 1
  call void @String.insert(%String* %"*self", i32 0, i8 %"$value")
  ret void
}

define i8 @String.remove(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %index = alloca i32, align 4
  %value = alloca i8, align 1
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %index, align 4
  %"*self" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$index"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  store i8 %"$*self.data[]", i8* %value, align 1
  %"$index1" = load i32, i32* %index, align 4
  store i32 %"$index1", i32* %i, align 4
  br label %while11

while11:                                          ; preds = %while11.then, %entry
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", %SUB
  br i1 %LT, label %while11.then, label %endloop

while11.then:                                     ; preds = %while11
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.data4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 0
  %"$*self.data5" = load i8*, i8** %"*self.data4", align 8
  %"$i6" = load i32, i32* %i, align 4
  %"*self.data[]7" = getelementptr inbounds i8, i8* %"$*self.data5", i32 %"$i6"
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.data9" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 0
  %"$i10" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i10", 1
  %"$*self.data11" = load i8*, i8** %"*self.data9", align 8
  %"*self.data[]12" = getelementptr inbounds i8, i8* %"$*self.data11", i32 %ADD
  %"$*self.data[]13" = load i8, i8* %"*self.data[]12", align 1
  store i8 %"$*self.data[]13", i8* %"*self.data[]7", align 1
  %"$i14" = load i32, i32* %i, align 4
  %ADD15 = add i32 %"$i14", 1
  store i32 %ADD15, i32* %i, align 4
  br label %while11

endloop:                                          ; preds = %while11
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.count17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 1
  %"$*self.count18" = load i32, i32* %"*self.count17", align 4
  %SUB19 = sub i32 %"$*self.count18", 1
  store i32 %SUB19, i32* %"*self.count17", align 4
  %"$value" = load i8, i8* %value, align 1
  ret i8 %"$value"
}

define i8 @String.pop_back(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  store i32 %SUB, i32* %"*self.count", align 4
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 0
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count4" = load i32, i32* %"*self.count3", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count4"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  ret i8 %"$*self.data[]"
}

define i8 @String.pop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %call.String.pop_back = call i8 @String.pop_back(%String* %"*self")
  ret i8 %call.String.pop_back
}

define i8 @String.pop_front(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %call.String.remove = call i8 @String.remove(%String* %"*self", i32 0)
  ret i8 %call.String.remove
}

define i8* @String.front(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 0
  ret i8* %"*self.data[]"
}

define i8* @String.back(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 0
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %SUB
  ret i8* %"*self.data[]"
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

define i32 @String.capacity(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  ret i32 %"$*self.cap"
}

define i1 @String.empty(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %EQ = icmp eq i32 %"$*self.count", 0
  ret i1 %EQ
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  ret void
}

define void @String.release(%String* %0) {
entry:
  %self = alloca %String*, align 8
  %nothing = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @free(i8* %"$*self.data")
  store i8* null, i8** %nothing, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.data2" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 0
  %"$nothing" = load i8*, i8** %nothing, align 8
  store i8* %"$nothing", i8** %"*self.data2", align 8
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 2
  store i32 0, i32* %"*self.cap", align 4
  ret void
}

define i8* @"String.[]"(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %index = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %index, align 4
  %"*self" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$index"
  ret i8* %"*self.data[]"
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
  br label %while12

while12:                                          ; preds = %while12.then, %entry
  %"*other2" = load %String*, %String** %other, align 8
  %"*other.count3" = getelementptr inbounds %String, %String* %"*other2", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*other.count4" = load i32, i32* %"*other.count3", align 4
  %LT = icmp slt i32 %"$i", %"$*other.count4"
  br i1 %LT, label %while12.then, label %endloop

while12.then:                                     ; preds = %while12
  %"*self5" = load %String*, %String** %self, align 8
  %"*other6" = load %String*, %String** %other, align 8
  %"*other.data" = getelementptr inbounds %String, %String* %"*other6", i32 0, i32 0
  %"$*other.data" = load i8*, i8** %"*other.data", align 8
  %"$i7" = load i32, i32* %i, align 4
  %"*other.data[]" = getelementptr inbounds i8, i8* %"$*other.data", i32 %"$i7"
  %"$*other.data[]" = load i8, i8* %"*other.data[]", align 1
  call void @String.push_back(%String* %"*self5", i8 %"$*other.data[]")
  %"$i8" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i8", 1
  store i32 %ADD, i32* %i, align 4
  br label %while12

endloop:                                          ; preds = %while12
  ret void
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @free(i8* %"$*self.data")
  ret void
}

define %String @String.from(i8* %0) {
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

define void @String.join(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  store i32 0, i32* %i, align 4
  br label %while13

while13:                                          ; preds = %while13.then, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$i" = load i32, i32* %i, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$i"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while13.then, label %endloop

while13.then:                                     ; preds = %while13
  %"*self" = load %String*, %String** %self, align 8
  %"$text1" = load i8*, i8** %text, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"text[]3" = getelementptr inbounds i8, i8* %"$text1", i32 %"$i2"
  %"$text[]4" = load i8, i8* %"text[]3", align 1
  call void @String.push_back(%String* %"*self", i8 %"$text[]4")
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while13

endloop:                                          ; preds = %while13
  ret void
}

define void @String.assign(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  %"*self1" = load %String*, %String** %self, align 8
  %"$text" = load i8*, i8** %text, align 8
  call void @String.join(%String* %"*self1", i8* %"$text")
  ret void
}

define i8* @String.c_str(%String* %0) {
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
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %call.String.c_str = call i8* @String.c_str(%String* %"*self")
  %call.atoi = call i32 @atoi(i8* %call.String.c_str)
  ret i32 %call.atoi
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
  br label %while14

while14:                                          ; preds = %endif1, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$j" = load i32, i32* %j, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$j"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while14.then, label %endloop

while14.then:                                     ; preds = %while14
  br label %if15

if15:                                             ; preds = %while14.then
  %"$index" = load i32, i32* %index, align 4
  %"$j2" = load i32, i32* %j, align 4
  %ADD = add i32 %"$index", %"$j2"
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GE = icmp sge i32 %ADD, %"$*self.count"
  br i1 %GE, label %if15.then, label %endif

if15.then:                                        ; preds = %if15
  ret i1 false

endif:                                            ; preds = %if15
  br label %if16

if16:                                             ; preds = %endif
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
  br i1 %NQ11, label %if16.then, label %endif1

if16.then:                                        ; preds = %if16
  ret i1 false

endif1:                                           ; preds = %if16
  %"$j12" = load i32, i32* %j, align 4
  %ADD13 = add i32 %"$j12", 1
  store i32 %ADD13, i32* %j, align 4
  br label %while14

endloop:                                          ; preds = %while14
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
  br label %while17

while17:                                          ; preds = %endif, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count"
  br i1 %LT, label %while17.then, label %endloop

while17.then:                                     ; preds = %while17
  br label %if18

if18:                                             ; preds = %while17.then
  %"*self1" = load %String*, %String** %self, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"$text" = load i8*, i8** %text, align 8
  %call.String.matches_at = call i1 @String.matches_at(%String* %"*self1", i32 %"$i2", i8* %"$text")
  br i1 %call.String.matches_at, label %if18.then, label %endif

if18.then:                                        ; preds = %if18
  %"$i3" = load i32, i32* %i, align 4
  ret i32 %"$i3"

endif:                                            ; preds = %if18
  %"$i4" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i4", 1
  store i32 %ADD, i32* %i, align 4
  br label %while17

endloop:                                          ; preds = %while17
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
  %call.strlen = call i64 @strlen(i8* %"$text")
  %AS = trunc i64 %call.strlen to i32
  store i32 %AS, i32* %n, align 4
  br label %if19

if19:                                             ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$n" = load i32, i32* %n, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$n", %"$*self.count"
  br i1 %GT, label %if19.then, label %endif

if19.then:                                        ; preds = %if19
  ret i1 false

endif:                                            ; preds = %if19
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
  %"$start" = load i32, i32* %start, align 4
  %"$length" = load i32, i32* %length, align 4
  %ADD = add i32 %"$start", %"$length"
  store i32 %ADD, i32* %stop, align 4
  br label %if20

if20:                                             ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$stop" = load i32, i32* %stop, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$stop", %"$*self.count"
  br i1 %GT, label %if20.then, label %endif

if20.then:                                        ; preds = %if20
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count2" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count3" = load i32, i32* %"*self.count2", align 4
  store i32 %"$*self.count3", i32* %stop, align 4
  br label %endif

endif:                                            ; preds = %if20.then, %if20
  %"$start4" = load i32, i32* %start, align 4
  store i32 %"$start4", i32* %i, align 4
  br label %while21

while21:                                          ; preds = %while21.then, %endif
  %"$i" = load i32, i32* %i, align 4
  %"$stop5" = load i32, i32* %stop, align 4
  %LT = icmp slt i32 %"$i", %"$stop5"
  br i1 %LT, label %while21.then, label %endloop

while21.then:                                     ; preds = %while21
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i7" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i7"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  call void @String.push_back(%String* %res, i8 %"$*self.data[]")
  %"$i8" = load i32, i32* %i, align 4
  %ADD9 = add i32 %"$i8", 1
  store i32 %ADD9, i32* %i, align 4
  br label %while21

endloop:                                          ; preds = %while21
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
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
  call void @String.push_back(%String* %res, i8 %"$c6")
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
  call void @String.push_back(%String* %res, i8 %"$c6")
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
  %call.strlen = call i64 @strlen(i8* %"$old")
  %AS = trunc i64 %call.strlen to i32
  store i32 %AS, i32* %n, align 4
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
  call void @String.push_back(%String* %res, i8 %"$*self.data[]")
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
  call void @String.push_back(%String* %res, i8 %"$*self.data[]")
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
  call void @String.push_back(%String* %res, i8 %"$*self.data[]")
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

define i1 @"String.>"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  %other = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  store %String* %1, %String** %other, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*other" = load %String*, %String** %other, align 8
  %call.String.compare = call i32 @String.compare(%String* %"*self", %String* %"*other")
  %GT = icmp sgt i32 %call.String.compare, 0
  ret i1 %GT
}

define i1 @"String.==.1"(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$text" = load i8*, i8** %text, align 8
  %call.strlen = call i64 @strlen(i8* %"$text")
  %AS = trunc i64 %call.strlen to i32
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %EQ = icmp eq i32 %"$*self.count", %AS
  %"*self1" = load %String*, %String** %self, align 8
  %"$text2" = load i8*, i8** %text, align 8
  %call.String.matches_at = call i1 @String.matches_at(%String* %"*self1", i32 0, i8* %"$text2")
  %AND = and i1 %EQ, %call.String.matches_at
  ret i1 %AND
}

define i1 @"String.!=.2"(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"$text" = load i8*, i8** %text, align 8
  %"call.String.==" = call i1 @"String.==.1"(%String* %"*self", i8* %"$text")
  %NOT = xor i1 %"call.String.==", true
  ret i1 %NOT
}

define void @"String.=.3"(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"$text" = load i8*, i8** %text, align 8
  call void @String.assign(%String* %"*self", i8* %"$text")
  ret void
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

define void @"String.+=.4"(%String* %0, %String* %1) {
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
  call void @String.push_back(%String* %"*self", i8 %"$*other.data[]")
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
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  store %String zeroinitializer, %String* %res, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @"String.="(%String* %res, %String* %"*self")
  %"$text" = load i8*, i8** %text, align 8
  call void @String.join(%String* %res, i8* %"$text")
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define %String @"String.+.5"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  %other = alloca %String*, align 8
  %res = alloca %String, align 8
  store %String* %0, %String** %self, align 8
  store %String* %1, %String** %other, align 8
  store %String zeroinitializer, %String* %res, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @"String.="(%String* %res, %String* %"*self")
  %"*other" = load %String*, %String** %other, align 8
  call void @"String.+=.4"(%String* %res, %String* %"*other")
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define i32 @main() {
entry:
  %s = alloca %String, align 8
  %arr = alloca [16 x i8], align 1
  %t = alloca %String, align 8
  %arr6 = alloca [2 x i8], align 1
  %arr7 = alloca [3 x i8], align 1
  %arr8 = alloca [2 x i8], align 1
  %arr9 = alloca [9 x i8], align 1
  %tmp = alloca %String, align 8
  %arr11 = alloca [2 x i8], align 1
  %tmp12 = alloca %String, align 8
  %arr14 = alloca [2 x i8], align 1
  %tmp15 = alloca %String, align 8
  %arr17 = alloca [2 x i8], align 1
  %arr18 = alloca [10 x i8], align 1
  %arr20 = alloca [6 x i8], align 1
  %arr21 = alloca [2 x i8], align 1
  %arr22 = alloca [5 x i8], align 1
  %arr23 = alloca [2 x i8], align 1
  %arr24 = alloca [5 x i8], align 1
  %arr25 = alloca [2 x i8], align 1
  %arr26 = alloca [3 x i8], align 1
  %arr27 = alloca [2 x i8], align 1
  %arr28 = alloca [4 x i8], align 1
  %arr30 = alloca [2 x i8], align 1
  %bool.text = alloca i8*, align 8
  %arr31 = alloca [5 x i8], align 1
  %arr32 = alloca [6 x i8], align 1
  %bool.text33 = alloca i8*, align 8
  %arr34 = alloca [5 x i8], align 1
  %arr35 = alloca [6 x i8], align 1
  %bool.text36 = alloca i8*, align 8
  %arr37 = alloca [5 x i8], align 1
  %arr38 = alloca [6 x i8], align 1
  %arr39 = alloca [16 x i8], align 1
  %r = alloca %String, align 8
  %arr43 = alloca [6 x i8], align 1
  %arr44 = alloca [4 x i8], align 1
  %arr46 = alloca [2 x i8], align 1
  %tmp47 = alloca %String, align 8
  %arr49 = alloca [2 x i8], align 1
  %tmp50 = alloca %String, align 8
  %arr52 = alloca [2 x i8], align 1
  %arr53 = alloca [10 x i8], align 1
  %a = alloca %String, align 8
  %arr55 = alloca [4 x i8], align 1
  %b = alloca %String, align 8
  %arr56 = alloca [4 x i8], align 1
  %c = alloca %String, align 8
  %arr57 = alloca [2 x i8], align 1
  %tmp58 = alloca %String, align 8
  %arr60 = alloca [2 x i8], align 1
  %arr62 = alloca [2 x i8], align 1
  %arr63 = alloca [2 x i8], align 1
  %arr64 = alloca [4 x i8], align 1
  %arr65 = alloca [2 x i8], align 1
  %arr66 = alloca [2 x i8], align 1
  %arr68 = alloca [2 x i8], align 1
  %bool.text69 = alloca i8*, align 8
  %arr70 = alloca [5 x i8], align 1
  %arr71 = alloca [6 x i8], align 1
  %bool.text72 = alloca i8*, align 8
  %arr73 = alloca [5 x i8], align 1
  %arr74 = alloca [6 x i8], align 1
  %bool.text75 = alloca i8*, align 8
  %arr76 = alloca [5 x i8], align 1
  %arr77 = alloca [6 x i8], align 1
  %arr78 = alloca [16 x i8], align 1
  %n = alloca %String, align 8
  %arr83 = alloca [3 x i8], align 1
  %arr84 = alloca [2 x i8], align 1
  %arr85 = alloca [2 x i8], align 1
  %arr86 = alloca [7 x i8], align 1
  store %String zeroinitializer, %String* %s, align 8
  store [16 x i8] c"  Hello World  \00", [16 x i8]* %arr, align 1
  %0 = getelementptr inbounds [16 x i8], [16 x i8]* %arr, i32 0, i32 0
  call void @"String.=.3"(%String* %s, i8* %0)
  %call.String.trim = call %String @String.trim(%String* %s)
  store %String %call.String.trim, %String* %t, align 8
  store [2 x i8] c"[\00", [2 x i8]* %arr6, align 1
  %1 = getelementptr inbounds [2 x i8], [2 x i8]* %arr6, i32 0, i32 0
  %call.String.c_str = call i8* @String.c_str(%String* %t)
  store [3 x i8] c"] \00", [3 x i8]* %arr7, align 1
  %2 = getelementptr inbounds [3 x i8], [3 x i8]* %arr7, i32 0, i32 0
  %call.String.size = call i32 @String.size(%String* %t)
  store [2 x i8] c"\0A\00", [2 x i8]* %arr8, align 1
  %3 = getelementptr inbounds [2 x i8], [2 x i8]* %arr8, i32 0, i32 0
  store [9 x i8] c"[%s] %d\0A\00", [9 x i8]* %arr9, align 1
  %4 = getelementptr inbounds [9 x i8], [9 x i8]* %arr9, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %4, i8* %call.String.c_str, i32 %call.String.size)
  %call.String.upper = call %String @String.upper(%String* %t)
  store %String %call.String.upper, %String* %tmp, align 8
  %call.String.c_str10 = call i8* @String.c_str(%String* %tmp)
  store [2 x i8] c" \00", [2 x i8]* %arr11, align 1
  %5 = getelementptr inbounds [2 x i8], [2 x i8]* %arr11, i32 0, i32 0
  %call.String.lower = call %String @String.lower(%String* %t)
  store %String %call.String.lower, %String* %tmp12, align 8
  %call.String.c_str13 = call i8* @String.c_str(%String* %tmp12)
  store [2 x i8] c" \00", [2 x i8]* %arr14, align 1
  %6 = getelementptr inbounds [2 x i8], [2 x i8]* %arr14, i32 0, i32 0
  %call.String.reverse = call %String @String.reverse(%String* %t)
  store %String %call.String.reverse, %String* %tmp15, align 8
  %call.String.c_str16 = call i8* @String.c_str(%String* %tmp15)
  store [2 x i8] c"\0A\00", [2 x i8]* %arr17, align 1
  %7 = getelementptr inbounds [2 x i8], [2 x i8]* %arr17, i32 0, i32 0
  store [10 x i8] c"%s %s %s\0A\00", [10 x i8]* %arr18, align 1
  %8 = getelementptr inbounds [10 x i8], [10 x i8]* %arr18, i32 0, i32 0
  %call.dprintf19 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %8, i8* %call.String.c_str10, i8* %call.String.c_str13, i8* %call.String.c_str16)
  call void @String.drop(%String* %tmp15)
  call void @String.drop(%String* %tmp12)
  call void @String.drop(%String* %tmp)
  store [6 x i8] c"World\00", [6 x i8]* %arr20, align 1
  %9 = getelementptr inbounds [6 x i8], [6 x i8]* %arr20, i32 0, i32 0
  %call.String.find = call i32 @String.find(%String* %t, i8* %9)
  store [2 x i8] c" \00", [2 x i8]* %arr21, align 1
  %10 = getelementptr inbounds [2 x i8], [2 x i8]* %arr21, i32 0, i32 0
  store [5 x i8] c"lo W\00", [5 x i8]* %arr22, align 1
  %11 = getelementptr inbounds [5 x i8], [5 x i8]* %arr22, i32 0, i32 0
  %call.String.contains = call i1 @String.contains(%String* %t, i8* %11)
  store [2 x i8] c" \00", [2 x i8]* %arr23, align 1
  %12 = getelementptr inbounds [2 x i8], [2 x i8]* %arr23, i32 0, i32 0
  store [5 x i8] c"Hell\00", [5 x i8]* %arr24, align 1
  %13 = getelementptr inbounds [5 x i8], [5 x i8]* %arr24, i32 0, i32 0
  %call.String.starts_with = call i1 @String.starts_with(%String* %t, i8* %13)
  store [2 x i8] c" \00", [2 x i8]* %arr25, align 1
  %14 = getelementptr inbounds [2 x i8], [2 x i8]* %arr25, i32 0, i32 0
  store [3 x i8] c"ld\00", [3 x i8]* %arr26, align 1
  %15 = getelementptr inbounds [3 x i8], [3 x i8]* %arr26, i32 0, i32 0
  %call.String.ends_with = call i1 @String.ends_with(%String* %t, i8* %15)
  store [2 x i8] c" \00", [2 x i8]* %arr27, align 1
  %16 = getelementptr inbounds [2 x i8], [2 x i8]* %arr27, i32 0, i32 0
  store [4 x i8] c"xyz\00", [4 x i8]* %arr28, align 1
  %17 = getelementptr inbounds [4 x i8], [4 x i8]* %arr28, i32 0, i32 0
  %call.String.find29 = call i32 @String.find(%String* %t, i8* %17)
  store [2 x i8] c"\0A\00", [2 x i8]* %arr30, align 1
  %18 = getelementptr inbounds [2 x i8], [2 x i8]* %arr30, i32 0, i32 0
  store i8* null, i8** %bool.text, align 8
  br label %if39

if39:                                             ; preds = %entry
  br i1 %call.String.contains, label %if39.then, label %else40

if39.then:                                        ; preds = %if39
  store [5 x i8] c"True\00", [5 x i8]* %arr31, align 1
  %19 = getelementptr inbounds [5 x i8], [5 x i8]* %arr31, i32 0, i32 0
  store i8* %19, i8** %bool.text, align 8
  br label %endif

else40:                                           ; preds = %if39
  store [6 x i8] c"False\00", [6 x i8]* %arr32, align 1
  %20 = getelementptr inbounds [6 x i8], [6 x i8]* %arr32, i32 0, i32 0
  store i8* %20, i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else40, %if39.then
  store i8* null, i8** %bool.text33, align 8
  br label %if41

if41:                                             ; preds = %endif
  br i1 %call.String.starts_with, label %if41.then, label %else42

if41.then:                                        ; preds = %if41
  store [5 x i8] c"True\00", [5 x i8]* %arr34, align 1
  %21 = getelementptr inbounds [5 x i8], [5 x i8]* %arr34, i32 0, i32 0
  store i8* %21, i8** %bool.text33, align 8
  br label %endif1

else42:                                           ; preds = %if41
  store [6 x i8] c"False\00", [6 x i8]* %arr35, align 1
  %22 = getelementptr inbounds [6 x i8], [6 x i8]* %arr35, i32 0, i32 0
  store i8* %22, i8** %bool.text33, align 8
  br label %endif1

endif1:                                           ; preds = %else42, %if41.then
  store i8* null, i8** %bool.text36, align 8
  br label %if43

if43:                                             ; preds = %endif1
  br i1 %call.String.ends_with, label %if43.then, label %else44

if43.then:                                        ; preds = %if43
  store [5 x i8] c"True\00", [5 x i8]* %arr37, align 1
  %23 = getelementptr inbounds [5 x i8], [5 x i8]* %arr37, i32 0, i32 0
  store i8* %23, i8** %bool.text36, align 8
  br label %endif2

else44:                                           ; preds = %if43
  store [6 x i8] c"False\00", [6 x i8]* %arr38, align 1
  %24 = getelementptr inbounds [6 x i8], [6 x i8]* %arr38, i32 0, i32 0
  store i8* %24, i8** %bool.text36, align 8
  br label %endif2

endif2:                                           ; preds = %else44, %if43.then
  store [16 x i8] c"%d %s %s %s %d\0A\00", [16 x i8]* %arr39, align 1
  %25 = getelementptr inbounds [16 x i8], [16 x i8]* %arr39, i32 0, i32 0
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %"$bool.text40" = load i8*, i8** %bool.text33, align 8
  %"$bool.text41" = load i8*, i8** %bool.text36, align 8
  %call.dprintf42 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %25, i32 %call.String.find, i8* %"$bool.text", i8* %"$bool.text40", i8* %"$bool.text41", i32 %call.String.find29)
  store [6 x i8] c"World\00", [6 x i8]* %arr43, align 1
  %26 = getelementptr inbounds [6 x i8], [6 x i8]* %arr43, i32 0, i32 0
  store [4 x i8] c"ura\00", [4 x i8]* %arr44, align 1
  %27 = getelementptr inbounds [4 x i8], [4 x i8]* %arr44, i32 0, i32 0
  %call.String.replace = call %String @String.replace(%String* %t, i8* %26, i8* %27)
  store %String %call.String.replace, %String* %r, align 8
  %call.String.c_str45 = call i8* @String.c_str(%String* %r)
  store [2 x i8] c" \00", [2 x i8]* %arr46, align 1
  %28 = getelementptr inbounds [2 x i8], [2 x i8]* %arr46, i32 0, i32 0
  %call.String.substr = call %String @String.substr(%String* %r, i32 1, i32 3)
  store %String %call.String.substr, %String* %tmp47, align 8
  %call.String.c_str48 = call i8* @String.c_str(%String* %tmp47)
  store [2 x i8] c" \00", [2 x i8]* %arr49, align 1
  %29 = getelementptr inbounds [2 x i8], [2 x i8]* %arr49, i32 0, i32 0
  %call.String.repeat = call %String @String.repeat(%String* %r, i32 2)
  store %String %call.String.repeat, %String* %tmp50, align 8
  %call.String.c_str51 = call i8* @String.c_str(%String* %tmp50)
  store [2 x i8] c"\0A\00", [2 x i8]* %arr52, align 1
  %30 = getelementptr inbounds [2 x i8], [2 x i8]* %arr52, i32 0, i32 0
  store [10 x i8] c"%s %s %s\0A\00", [10 x i8]* %arr53, align 1
  %31 = getelementptr inbounds [10 x i8], [10 x i8]* %arr53, i32 0, i32 0
  %call.dprintf54 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %31, i8* %call.String.c_str45, i8* %call.String.c_str48, i8* %call.String.c_str51)
  call void @String.drop(%String* %tmp50)
  call void @String.drop(%String* %tmp47)
  store [4 x i8] c"abc\00", [4 x i8]* %arr55, align 1
  %32 = getelementptr inbounds [4 x i8], [4 x i8]* %arr55, i32 0, i32 0
  %call.String.from = call %String @String.from(i8* %32)
  store %String %call.String.from, %String* %a, align 8
  store %String zeroinitializer, %String* %b, align 8
  store [4 x i8] c"abd\00", [4 x i8]* %arr56, align 1
  %33 = getelementptr inbounds [4 x i8], [4 x i8]* %arr56, i32 0, i32 0
  call void @"String.=.3"(%String* %b, i8* %33)
  store [2 x i8] c"-\00", [2 x i8]* %arr57, align 1
  %34 = getelementptr inbounds [2 x i8], [2 x i8]* %arr57, i32 0, i32 0
  %"call.String.+" = call %String @"String.+"(%String* %a, i8* %34)
  store %String %"call.String.+", %String* %tmp58, align 8
  %"call.String.+59" = call %String @"String.+.5"(%String* %tmp58, %String* %b)
  store %String %"call.String.+59", %String* %c, align 8
  call void @String.drop(%String* %tmp58)
  store [2 x i8] c"!\00", [2 x i8]* %arr60, align 1
  %35 = getelementptr inbounds [2 x i8], [2 x i8]* %arr60, i32 0, i32 0
  call void @"String.+="(%String* %c, i8* %35)
  %call.String.c_str61 = call i8* @String.c_str(%String* %c)
  store [2 x i8] c" \00", [2 x i8]* %arr62, align 1
  %36 = getelementptr inbounds [2 x i8], [2 x i8]* %arr62, i32 0, i32 0
  %"call.String.<" = call i1 @"String.<"(%String* %a, %String* %b)
  store [2 x i8] c" \00", [2 x i8]* %arr63, align 1
  %37 = getelementptr inbounds [2 x i8], [2 x i8]* %arr63, i32 0, i32 0
  store [4 x i8] c"abc\00", [4 x i8]* %arr64, align 1
  %38 = getelementptr inbounds [4 x i8], [4 x i8]* %arr64, i32 0, i32 0
  %"call.String.==" = call i1 @"String.==.1"(%String* %a, i8* %38)
  store [2 x i8] c" \00", [2 x i8]* %arr65, align 1
  %39 = getelementptr inbounds [2 x i8], [2 x i8]* %arr65, i32 0, i32 0
  %"call.String.!=" = call i1 @"String.!="(%String* %a, %String* %b)
  store [2 x i8] c" \00", [2 x i8]* %arr66, align 1
  %40 = getelementptr inbounds [2 x i8], [2 x i8]* %arr66, i32 0, i32 0
  %call.String.size67 = call i32 @String.size(%String* %c)
  store [2 x i8] c"\0A\00", [2 x i8]* %arr68, align 1
  %41 = getelementptr inbounds [2 x i8], [2 x i8]* %arr68, i32 0, i32 0
  store i8* null, i8** %bool.text69, align 8
  br label %if45

if45:                                             ; preds = %endif2
  br i1 %"call.String.<", label %if45.then, label %else46

if45.then:                                        ; preds = %if45
  store [5 x i8] c"True\00", [5 x i8]* %arr70, align 1
  %42 = getelementptr inbounds [5 x i8], [5 x i8]* %arr70, i32 0, i32 0
  store i8* %42, i8** %bool.text69, align 8
  br label %endif3

else46:                                           ; preds = %if45
  store [6 x i8] c"False\00", [6 x i8]* %arr71, align 1
  %43 = getelementptr inbounds [6 x i8], [6 x i8]* %arr71, i32 0, i32 0
  store i8* %43, i8** %bool.text69, align 8
  br label %endif3

endif3:                                           ; preds = %else46, %if45.then
  store i8* null, i8** %bool.text72, align 8
  br label %if47

if47:                                             ; preds = %endif3
  br i1 %"call.String.==", label %if47.then, label %else48

if47.then:                                        ; preds = %if47
  store [5 x i8] c"True\00", [5 x i8]* %arr73, align 1
  %44 = getelementptr inbounds [5 x i8], [5 x i8]* %arr73, i32 0, i32 0
  store i8* %44, i8** %bool.text72, align 8
  br label %endif4

else48:                                           ; preds = %if47
  store [6 x i8] c"False\00", [6 x i8]* %arr74, align 1
  %45 = getelementptr inbounds [6 x i8], [6 x i8]* %arr74, i32 0, i32 0
  store i8* %45, i8** %bool.text72, align 8
  br label %endif4

endif4:                                           ; preds = %else48, %if47.then
  store i8* null, i8** %bool.text75, align 8
  br label %if49

if49:                                             ; preds = %endif4
  br i1 %"call.String.!=", label %if49.then, label %else50

if49.then:                                        ; preds = %if49
  store [5 x i8] c"True\00", [5 x i8]* %arr76, align 1
  %46 = getelementptr inbounds [5 x i8], [5 x i8]* %arr76, i32 0, i32 0
  store i8* %46, i8** %bool.text75, align 8
  br label %endif5

else50:                                           ; preds = %if49
  store [6 x i8] c"False\00", [6 x i8]* %arr77, align 1
  %47 = getelementptr inbounds [6 x i8], [6 x i8]* %arr77, i32 0, i32 0
  store i8* %47, i8** %bool.text75, align 8
  br label %endif5

endif5:                                           ; preds = %else50, %if49.then
  store [16 x i8] c"%s %s %s %s %d\0A\00", [16 x i8]* %arr78, align 1
  %48 = getelementptr inbounds [16 x i8], [16 x i8]* %arr78, i32 0, i32 0
  %"$bool.text79" = load i8*, i8** %bool.text69, align 8
  %"$bool.text80" = load i8*, i8** %bool.text72, align 8
  %"$bool.text81" = load i8*, i8** %bool.text75, align 8
  %call.dprintf82 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %48, i8* %call.String.c_str61, i8* %"$bool.text79", i8* %"$bool.text80", i8* %"$bool.text81", i32 %call.String.size67)
  store %String zeroinitializer, %String* %n, align 8
  store [3 x i8] c"42\00", [3 x i8]* %arr83, align 1
  %49 = getelementptr inbounds [3 x i8], [3 x i8]* %arr83, i32 0, i32 0
  call void @"String.=.3"(%String* %n, i8* %49)
  %call.String.to_int = call i32 @String.to_int(%String* %n)
  %ADD = add i32 %call.String.to_int, 1
  store [2 x i8] c" \00", [2 x i8]* %arr84, align 1
  %50 = getelementptr inbounds [2 x i8], [2 x i8]* %arr84, i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr85, align 1
  %51 = getelementptr inbounds [2 x i8], [2 x i8]* %arr85, i32 0, i32 0
  store [7 x i8] c"%d %s\0A\00", [7 x i8]* %arr86, align 1
  %52 = getelementptr inbounds [7 x i8], [7 x i8]* %arr86, i32 0, i32 0
  %call.dprintf87 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %52, i32 %ADD, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @TYPEOF, i32 0, i32 0))
  call void @String.drop(%String* %n)
  call void @String.drop(%String* %c)
  call void @String.drop(%String* %b)
  call void @String.drop(%String* %a)
  call void @String.drop(%String* %r)
  call void @String.drop(%String* %t)
  call void @String.drop(%String* %s)
  ret i32 0
}

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

define void @Plain.drop(%Plain* %0) {
entry:
  %self = alloca %Plain*, align 8
  %arr = alloca [12 x i8], align 1
  %arr1 = alloca [2 x i8], align 1
  %arr2 = alloca [15 x i8], align 1
  store %Plain* %0, %Plain** %self, align 8
  store [12 x i8] c"plain drop \00", [12 x i8]* %arr, align 1
  %1 = getelementptr inbounds [12 x i8], [12 x i8]* %arr, i32 0, i32 0
  %"*self" = load %Plain*, %Plain** %self, align 8
  %"*self.id" = getelementptr inbounds %Plain, %Plain* %"*self", i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr1, align 1
  %2 = getelementptr inbounds [2 x i8], [2 x i8]* %arr1, i32 0, i32 0
  %"$*self.id" = load i32, i32* %"*self.id", align 4
  store [15 x i8] c"plain drop %d\0A\00", [15 x i8]* %arr2, align 1
  %3 = getelementptr inbounds [15 x i8], [15 x i8]* %arr2, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %3, i32 %"$*self.id")
  ret void
}

define void @Owned.drop(%Owned* %0) {
entry:
  %self = alloca %Owned*, align 8
  %arr = alloca [12 x i8], align 1
  %arr1 = alloca [2 x i8], align 1
  %arr2 = alloca [15 x i8], align 1
  store %Owned* %0, %Owned** %self, align 8
  store [12 x i8] c"owned drop \00", [12 x i8]* %arr, align 1
  %1 = getelementptr inbounds [12 x i8], [12 x i8]* %arr, i32 0, i32 0
  %"*self" = load %Owned*, %Owned** %self, align 8
  %"*self.id" = getelementptr inbounds %Owned, %Owned* %"*self", i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr1, align 1
  %2 = getelementptr inbounds [2 x i8], [2 x i8]* %arr1, i32 0, i32 0
  %"$*self.id" = load i32, i32* %"*self.id", align 4
  store [15 x i8] c"owned drop %d\0A\00", [15 x i8]* %arr2, align 1
  %3 = getelementptr inbounds [15 x i8], [15 x i8]* %arr2, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %3, i32 %"$*self.id")
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

