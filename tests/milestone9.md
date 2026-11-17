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

