# milestone09

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
    io::out($"floor = {r.floor}\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }
%Room = type { i32 }

@str = private unnamed_addr constant [12 x i8] c"floor = %d\0A\00", align 1

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

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
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

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
  ret void
}

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
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %call.Room.create = call %Room @Room.create(i32 7)
  store %Room %call.Room.create, %Room* %r, align 4
  %r.floor = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  %"$r.floor" = load i32, i32* %r.floor, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i32 %"$r.floor")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i32 %"$r.floor")
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  call void @"io::out"(%String %2)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
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
    io::out($"{Room::depth()}\n")
    return 0
```

### stderr

```
error: test.ura:10:22 'Room' has no pub fn 'depth'
   |
10 |     io::out($"{Room::depth()}\n")
   |                      ^^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 003 — method: :: on a type that does not exist

```ura
// error: Unknown type 'Nope'

fn main() i32:
    io::out($"{Nope::create()}\n")
    return 0
```

### stderr

```
error: test.ura:4:16 unknown type 'Nope'
  |
4 |     io::out($"{Nope::create()}\n")
  |                ^^^^
error: aborting due to 1 error
```

### status

```
1
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
    io::out($"room {a.floor} tower {b.height}\n")
    io::out($"zero {Room::zero().floor}\n")
    io::out($"level {Room::level(4)}\n")
    io::out($"method {Room::create(5).depth()}\n")
    n i32 = Room::create(2).depth() + Tower::create(1).height
    io::out($"mixed {n}\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }
%Room = type { i32 }
%Tower = type { i32 }

@str = private unnamed_addr constant [18 x i8] c"room %d tower %d\0A\00", align 1
@str.1 = private unnamed_addr constant [9 x i8] c"zero %d\0A\00", align 1
@str.2 = private unnamed_addr constant [10 x i8] c"level %d\0A\00", align 1
@str.3 = private unnamed_addr constant [11 x i8] c"method %d\0A\00", align 1
@str.4 = private unnamed_addr constant [10 x i8] c"mixed %d\0A\00", align 1

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

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
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

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
  ret void
}

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
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %tmp = alloca %Room, align 8
  %format.size6 = alloca i32, align 4
  %format.text11 = alloca i8*, align 8
  %format.size19 = alloca i32, align 4
  %format.text24 = alloca i8*, align 8
  %tmp33 = alloca %Room, align 8
  %format.size34 = alloca i32, align 4
  %format.text39 = alloca i8*, align 8
  %n = alloca i32, align 4
  %tmp48 = alloca %Room, align 8
  %tmp51 = alloca %Tower, align 8
  %format.size53 = alloca i32, align 4
  %format.text58 = alloca i8*, align 8
  %call.Room.create = call %Room @Room.create(i32 3)
  store %Room %call.Room.create, %Room* %a, align 4
  %call.Tower.create = call %Tower @Tower.create(i32 9)
  store %Tower %call.Tower.create, %Tower* %b, align 4
  %a.floor = getelementptr inbounds %Room, %Room* %a, i32 0, i32 0
  %b.height = getelementptr inbounds %Tower, %Tower* %b, i32 0, i32 0
  %"$a.floor" = load i32, i32* %a.floor, align 4
  %"$b.height" = load i32, i32* %b.height, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str, i32 0, i32 0), i32 %"$a.floor", i32 %"$b.height")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str, i32 0, i32 0), i32 %"$a.floor", i32 %"$b.height")
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  call void @"io::out"(%String %2)
  %call.Room.zero = call %Room @Room.zero()
  store %Room %call.Room.zero, %Room* %tmp, align 4
  %tmp.floor = getelementptr inbounds %Room, %Room* %tmp, i32 0, i32 0
  %"$tmp.floor" = load i32, i32* %tmp.floor, align 4
  %call.snprintf7 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0), i32 %"$tmp.floor")
  store i32 %call.snprintf7, i32* %format.size6, align 4
  %"$format.size8" = load i32, i32* %format.size6, align 4
  %ADD9 = add i32 %"$format.size8", 1
  %AS10 = sext i32 %ADD9 to i64
  %call.calloc12 = call i8* @calloc(i64 %AS10, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc12, i8** %format.text11, align 8
  %"$format.text13" = load i8*, i8** %format.text11, align 8
  %call.snprintf14 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text13", i64 %AS10, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0), i32 %"$tmp.floor")
  %"$format.size15" = load i32, i32* %format.size6, align 4
  %ADD16 = add i32 %"$format.size15", 1
  %"$format.text17" = load i8*, i8** %format.text11, align 8
  %"$format.size18" = load i32, i32* %format.size6, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text17", 0
  %4 = insertvalue %String %3, i32 %"$format.size18", 1
  %5 = insertvalue %String %4, i32 %ADD16, 2
  call void @"io::out"(%String %5)
  %call.Room.level = call i32 @Room.level(i32 4)
  %call.snprintf20 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.2, i32 0, i32 0), i32 %call.Room.level)
  store i32 %call.snprintf20, i32* %format.size19, align 4
  %"$format.size21" = load i32, i32* %format.size19, align 4
  %ADD22 = add i32 %"$format.size21", 1
  %AS23 = sext i32 %ADD22 to i64
  %call.calloc25 = call i8* @calloc(i64 %AS23, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc25, i8** %format.text24, align 8
  %"$format.text26" = load i8*, i8** %format.text24, align 8
  %call.snprintf27 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text26", i64 %AS23, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.2, i32 0, i32 0), i32 %call.Room.level)
  %"$format.size28" = load i32, i32* %format.size19, align 4
  %ADD29 = add i32 %"$format.size28", 1
  %"$format.text30" = load i8*, i8** %format.text24, align 8
  %"$format.size31" = load i32, i32* %format.size19, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text30", 0
  %7 = insertvalue %String %6, i32 %"$format.size31", 1
  %8 = insertvalue %String %7, i32 %ADD29, 2
  call void @"io::out"(%String %8)
  %call.Room.create32 = call %Room @Room.create(i32 5)
  store %Room %call.Room.create32, %Room* %tmp33, align 4
  %call.Room.depth = call i32 @Room.depth(%Room* %tmp33)
  %call.snprintf35 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.3, i32 0, i32 0), i32 %call.Room.depth)
  store i32 %call.snprintf35, i32* %format.size34, align 4
  %"$format.size36" = load i32, i32* %format.size34, align 4
  %ADD37 = add i32 %"$format.size36", 1
  %AS38 = sext i32 %ADD37 to i64
  %call.calloc40 = call i8* @calloc(i64 %AS38, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc40, i8** %format.text39, align 8
  %"$format.text41" = load i8*, i8** %format.text39, align 8
  %call.snprintf42 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text41", i64 %AS38, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.3, i32 0, i32 0), i32 %call.Room.depth)
  %"$format.size43" = load i32, i32* %format.size34, align 4
  %ADD44 = add i32 %"$format.size43", 1
  %"$format.text45" = load i8*, i8** %format.text39, align 8
  %"$format.size46" = load i32, i32* %format.size34, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text45", 0
  %10 = insertvalue %String %9, i32 %"$format.size46", 1
  %11 = insertvalue %String %10, i32 %ADD44, 2
  call void @"io::out"(%String %11)
  %call.Room.create47 = call %Room @Room.create(i32 2)
  store %Room %call.Room.create47, %Room* %tmp48, align 4
  %call.Room.depth49 = call i32 @Room.depth(%Room* %tmp48)
  %call.Tower.create50 = call %Tower @Tower.create(i32 1)
  store %Tower %call.Tower.create50, %Tower* %tmp51, align 4
  %tmp.height = getelementptr inbounds %Tower, %Tower* %tmp51, i32 0, i32 0
  %"$tmp.height" = load i32, i32* %tmp.height, align 4
  %ADD52 = add i32 %call.Room.depth49, %"$tmp.height"
  store i32 %ADD52, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %call.snprintf54 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.4, i32 0, i32 0), i32 %"$n")
  store i32 %call.snprintf54, i32* %format.size53, align 4
  %"$format.size55" = load i32, i32* %format.size53, align 4
  %ADD56 = add i32 %"$format.size55", 1
  %AS57 = sext i32 %ADD56 to i64
  %call.calloc59 = call i8* @calloc(i64 %AS57, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc59, i8** %format.text58, align 8
  %"$format.text60" = load i8*, i8** %format.text58, align 8
  %call.snprintf61 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text60", i64 %AS57, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.4, i32 0, i32 0), i32 %"$n")
  %"$format.size62" = load i32, i32* %format.size53, align 4
  %ADD63 = add i32 %"$format.size62", 1
  %"$format.text64" = load i8*, i8** %format.text58, align 8
  %"$format.size65" = load i32, i32* %format.size53, align 4
  %12 = insertvalue %String zeroinitializer, i8* %"$format.text64", 0
  %13 = insertvalue %String %12, i32 %"$format.size65", 1
  %14 = insertvalue %String %13, i32 %ADD63, 2
  call void @"io::out"(%String %14)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

---

## 005 — operator: operator outside a struct definition

```ura
// error: 'operator' may only be declared inside a struct
use "@/header"

operator + (a i32, b i32) i32:
    return a + b

fn main() i32:
    io::out("hi\n")
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

### stderr

```
error: test.ura:7:15 expected '(' after the function name
  |
7 |     operator () Vec:
  |               ^
help: use operator (()) Vec:
error: test.ura:7:15 expected ':' after the function header
  |
7 |     operator () Vec:
  |               ^
help: use operator (:) Vec:
error: test.ura:7:15 unexpected ')'
  |
7 |     operator () Vec:
  |               ^
error: test.ura:7:15 a struct holds only attributes and methods
  |
7 |     operator () Vec:
  |               ^
error: test.ura:7:17 attribute 'Vec' needs a type
  |
7 |     operator () Vec:
  |                 ^^^
help: like: Vec i32
error: test.ura:7:20 unexpected ':'
  |
7 |     operator () Vec:
  |                    ^
error: test.ura:7:20 a struct holds only attributes and methods
  |
7 |     operator () Vec:
  |                    ^
error: test.ura:8:9 a struct holds only attributes and methods
  |
8 |         return self
  |         ^^^^^^
error: aborting due to 8 errors
```

### status

```
1
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
    io::out($"{c.x}\n")
    return 0
```

### stderr

```
error: test.ura:11:15 'Vec' has no operator '+' that takes 'Vec'
   |
11 |     c Vec = a + b
   |               ^
error: aborting due to 1 error
```

### status

```
1
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
    io::out($"{c.x} {c.y}\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }
%Vec = type { i32, i32 }

@str = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1

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

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
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

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
  ret void
}

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
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
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
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %"$c.x", i32 %"$c.y")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf2 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %"$c.x", i32 %"$c.y")
  %"$format.size3" = load i32, i32* %format.size, align 4
  %ADD4 = add i32 %"$format.size3", 1
  %"$format.text5" = load i8*, i8** %format.text, align 8
  %"$format.size6" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text5", 0
  %1 = insertvalue %String %0, i32 %"$format.size6", 1
  %2 = insertvalue %String %1, i32 %ADD4, 2
  call void @"io::out"(%String %2)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
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
    io::out($"{(a + b).x} {(a + 5).x}\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }
%Vec = type { i32 }

@str = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1

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

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
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

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
  ret void
}

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
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
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
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %"$tmp.x", i32 %"$tmp.x5")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf6 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %"$tmp.x", i32 %"$tmp.x5")
  %"$format.size7" = load i32, i32* %format.size, align 4
  %ADD8 = add i32 %"$format.size7", 1
  %"$format.text9" = load i8*, i8** %format.text, align 8
  %"$format.size10" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text9", 0
  %1 = insertvalue %String %0, i32 %"$format.size10", 1
  %2 = insertvalue %String %1, i32 %ADD8, 2
  call void @"io::out"(%String %2)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
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
    io::out($"{a == b} {a == c}\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }
%Vec = type { i32 }

@str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.2 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.4 = private unnamed_addr constant [7 x i8] c"%s %s\0A\00", align 1

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

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
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

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
  ret void
}

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
  %format.size = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %bool.text5 = alloca i8*, align 8
  %format.text = alloca i8*, align 8
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
  store i32 0, i32* %format.size, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if6

if6:                                              ; preds = %entry
  br i1 %"call.Vec.==", label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  store i8* null, i8** %bool.text5, align 8
  br label %if8

if8:                                              ; preds = %endif
  br i1 %"call.Vec.==4", label %if8.then, label %else9

if8.then:                                         ; preds = %if8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.2, i32 0, i32 0), i8** %bool.text5, align 8
  br label %endif1

else9:                                            ; preds = %if8
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i8** %bool.text5, align 8
  br label %endif1

endif1:                                           ; preds = %else9, %if8.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %"$bool.text6" = load i8*, i8** %bool.text5, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.4, i32 0, i32 0), i8* %"$bool.text", i8* %"$bool.text6")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %"$bool.text7" = load i8*, i8** %bool.text, align 8
  %"$bool.text8" = load i8*, i8** %bool.text5, align 8
  %call.snprintf9 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.4, i32 0, i32 0), i8* %"$bool.text7", i8* %"$bool.text8")
  %"$format.size10" = load i32, i32* %format.size, align 4
  %ADD11 = add i32 %"$format.size10", 1
  %"$format.text12" = load i8*, i8** %format.text, align 8
  %"$format.size13" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text12", 0
  %1 = insertvalue %String %0, i32 %"$format.size13", 1
  %2 = insertvalue %String %1, i32 %ADD11, 2
  call void @"io::out"(%String %2)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
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

### stderr

```
error: test.ura:9:15 'Vec' has no operator '-' that takes 'Vec'
  |
9 |     c Vec = a - b
  |               ^
error: aborting due to 1 error
```

### status

```
1
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

### stderr

```
error: test.ura:12:9 expected 'Vec', found 'Point'
   |
12 |     v = p
   |         ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 013 — operator: indexing an array literal directly

```ura
// stdout: 20
// expect: 0

fn main() i32:
    io::out($"{[10, 20, 30][1]}\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

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

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
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

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
  ret void
}

define i32 @main() {
entry:
  %arr = alloca [3 x i32], align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store [3 x i32] [i32 10, i32 20, i32 30], [3 x i32]* %arr, align 4
  %0 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  %"VAL_CREATE[]" = getelementptr inbounds i32, i32* %0, i32 1
  %"$VAL_CREATE[]" = load i32, i32* %"VAL_CREATE[]", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$VAL_CREATE[]")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$VAL_CREATE[]")
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %1 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %2 = insertvalue %String %1, i32 %"$format.size5", 1
  %3 = insertvalue %String %2, i32 %ADD3, 2
  call void @"io::out"(%String %3)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
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
        io::out($"({self.x}, {self.y})\n")

fn main() i32:
    a Vec = Vec::create(1, 2)
    b Vec = Vec::create(10, 20)

    c Vec = a + b
    c.show()

    // the same variable on both sides
    io::out($"a == a  {a == a}\n")
    io::out($"a == b  {a == b}\n")

    // an operator result used directly as a receiver
    (a + b).show()
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }
%Vec = type { i32, i32 }

@str = private unnamed_addr constant [10 x i8] c"(%d, %d)\0A\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.3 = private unnamed_addr constant [12 x i8] c"a == a  %s\0A\00", align 1
@str.4 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.5 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.6 = private unnamed_addr constant [12 x i8] c"a == b  %s\0A\00", align 1

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

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
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

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
  ret void
}

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
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Vec* %0, %Vec** %self, align 8
  %"*self" = load %Vec*, %Vec** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec, %Vec* %"*self", i32 0, i32 0
  %"*self1" = load %Vec*, %Vec** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec, %Vec* %"*self1", i32 0, i32 1
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %"$*self.x", i32 %"$*self.y")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf2 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %"$*self.x", i32 %"$*self.y")
  %"$format.size3" = load i32, i32* %format.size, align 4
  %ADD4 = add i32 %"$format.size3", 1
  %"$format.text5" = load i8*, i8** %format.text, align 8
  %"$format.size6" = load i32, i32* %format.size, align 4
  %1 = insertvalue %String zeroinitializer, i8* %"$format.text5", 0
  %2 = insertvalue %String %1, i32 %"$format.size6", 1
  %3 = insertvalue %String %2, i32 %ADD4, 2
  call void @"io::out"(%String %3)
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Vec, align 8
  %b = alloca %Vec, align 8
  %c = alloca %Vec, align 8
  %format.size = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text = alloca i8*, align 8
  %format.size11 = alloca i32, align 4
  %bool.text12 = alloca i8*, align 8
  %format.text18 = alloca i8*, align 8
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
  store i32 0, i32* %format.size, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if6

if6:                                              ; preds = %entry
  br i1 %"call.Vec.==", label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.3, i32 0, i32 0), i8* %"$bool.text")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %"$bool.text3" = load i8*, i8** %bool.text, align 8
  %call.snprintf4 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.3, i32 0, i32 0), i8* %"$bool.text3")
  %"$format.size5" = load i32, i32* %format.size, align 4
  %ADD6 = add i32 %"$format.size5", 1
  %"$format.text7" = load i8*, i8** %format.text, align 8
  %"$format.size8" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text7", 0
  %1 = insertvalue %String %0, i32 %"$format.size8", 1
  %2 = insertvalue %String %1, i32 %ADD6, 2
  call void @"io::out"(%String %2)
  %"$b9" = load %Vec, %Vec* %b, align 4
  %"call.Vec.==10" = call i1 @"Vec.=="(%Vec* %a, %Vec %"$b9")
  store i32 0, i32* %format.size11, align 4
  store i8* null, i8** %bool.text12, align 8
  br label %if8

if8:                                              ; preds = %endif
  br i1 %"call.Vec.==10", label %if8.then, label %else9

if8.then:                                         ; preds = %if8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.4, i32 0, i32 0), i8** %bool.text12, align 8
  br label %endif1

else9:                                            ; preds = %if8
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.5, i32 0, i32 0), i8** %bool.text12, align 8
  br label %endif1

endif1:                                           ; preds = %else9, %if8.then
  %"$bool.text13" = load i8*, i8** %bool.text12, align 8
  %call.snprintf14 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.6, i32 0, i32 0), i8* %"$bool.text13")
  store i32 %call.snprintf14, i32* %format.size11, align 4
  %"$format.size15" = load i32, i32* %format.size11, align 4
  %ADD16 = add i32 %"$format.size15", 1
  %AS17 = sext i32 %ADD16 to i64
  %call.calloc19 = call i8* @calloc(i64 %AS17, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc19, i8** %format.text18, align 8
  %"$format.text20" = load i8*, i8** %format.text18, align 8
  %"$bool.text21" = load i8*, i8** %bool.text12, align 8
  %call.snprintf22 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text20", i64 %AS17, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.6, i32 0, i32 0), i8* %"$bool.text21")
  %"$format.size23" = load i32, i32* %format.size11, align 4
  %ADD24 = add i32 %"$format.size23", 1
  %"$format.text25" = load i8*, i8** %format.text18, align 8
  %"$format.size26" = load i32, i32* %format.size11, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text25", 0
  %4 = insertvalue %String %3, i32 %"$format.size26", 1
  %5 = insertvalue %String %4, i32 %ADD24, 2
  call void @"io::out"(%String %5)
  %"$b27" = load %Vec, %Vec* %b, align 4
  %"call.Vec.+28" = call %Vec @"Vec.+"(%Vec* %a, %Vec %"$b27")
  store %Vec %"call.Vec.+28", %Vec* %tmp, align 4
  call void @Vec.show(%Vec* %tmp)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

---

## 015 — operator: assigning a void result

```ura
// error: Cannot assign void to Vec

struct Vec:
    x i32

    fn show():
        io::out($"{self.x}\n")

fn main() i32:
    a Vec
    e Vec = a.show()
    return 0
```

### stderr

```
error: test.ura:11:14 expected 'Vec', found 'void'
   |
11 |     e Vec = a.show()
   |              ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 016 — operator: a binary operator with mismatched scalar types

```ura
// error: Cannot use '+' with i32 and array

fn main() i32:
    n i32 = 1
    s char[] = "hi"
    io::out($"{n + s}\n")
    return 0
```

### stderr

```
error: test.ura:6:18 can't apply '+' to 'i32' and 'i8[]'
  |
6 |     io::out($"{n + s}\n")
  |                  ^
error: aborting due to 1 error
```

### status

```
1
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
    io::out($"decl   {b.x} {b.tag}\n")

    // reassignment: calls operator =, tag becomes 99
    c Vec
    c = a
    io::out($"assign {c.x} {c.tag}\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }
%Vec = type { i32, i32 }

@str = private unnamed_addr constant [14 x i8] c"decl   %d %d\0A\00", align 1
@str.1 = private unnamed_addr constant [14 x i8] c"assign %d %d\0A\00", align 1

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

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
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

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
  ret void
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
  %"*self.tag" = getelementptr inbounds %Vec, %Vec* %"*self1", i32 0, i32 1
  store i32 99, i32* %"*self.tag", align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Vec, align 8
  %b = alloca %Vec, align 8
  %copy = alloca %Vec, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %c = alloca %Vec, align 8
  %copy6 = alloca %Vec, align 8
  %format.size8 = alloca i32, align 4
  %format.text13 = alloca i8*, align 8
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
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str, i32 0, i32 0), i32 %"$b.x", i32 %"$b.tag")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str, i32 0, i32 0), i32 %"$b.x", i32 %"$b.tag")
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  call void @"io::out"(%String %2)
  store %Vec zeroinitializer, %Vec* %copy6, align 4
  call void @"Vec.="(%Vec* %copy6, %Vec* %a)
  %"$copy7" = load %Vec, %Vec* %copy6, align 4
  store %Vec %"$copy7", %Vec* %c, align 4
  %c.x = getelementptr inbounds %Vec, %Vec* %c, i32 0, i32 0
  %c.tag = getelementptr inbounds %Vec, %Vec* %c, i32 0, i32 1
  %"$c.x" = load i32, i32* %c.x, align 4
  %"$c.tag" = load i32, i32* %c.tag, align 4
  %call.snprintf9 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.1, i32 0, i32 0), i32 %"$c.x", i32 %"$c.tag")
  store i32 %call.snprintf9, i32* %format.size8, align 4
  %"$format.size10" = load i32, i32* %format.size8, align 4
  %ADD11 = add i32 %"$format.size10", 1
  %AS12 = sext i32 %ADD11 to i64
  %call.calloc14 = call i8* @calloc(i64 %AS12, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc14, i8** %format.text13, align 8
  %"$format.text15" = load i8*, i8** %format.text13, align 8
  %call.snprintf16 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text15", i64 %AS12, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.1, i32 0, i32 0), i32 %"$c.x", i32 %"$c.tag")
  %"$format.size17" = load i32, i32* %format.size8, align 4
  %ADD18 = add i32 %"$format.size17", 1
  %"$format.text19" = load i8*, i8** %format.text13, align 8
  %"$format.size20" = load i32, i32* %format.size8, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text19", 0
  %4 = insertvalue %String %3, i32 %"$format.size20", 1
  %5 = insertvalue %String %4, i32 %ADD18, 2
  call void @"io::out"(%String %5)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
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
    io::out($"after += {a.x}\n")

    a -= 3
    io::out($"after -= {a.x}\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }
%Vec = type { i32 }

@str = private unnamed_addr constant [13 x i8] c"after += %d\0A\00", align 1
@str.1 = private unnamed_addr constant [13 x i8] c"after -= %d\0A\00", align 1

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

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
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

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
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
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %format.size8 = alloca i32, align 4
  %format.text14 = alloca i8*, align 8
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
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str, i32 0, i32 0), i32 %"$a.x")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf2 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str, i32 0, i32 0), i32 %"$a.x")
  %"$format.size3" = load i32, i32* %format.size, align 4
  %ADD4 = add i32 %"$format.size3", 1
  %"$format.text5" = load i8*, i8** %format.text, align 8
  %"$format.size6" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text5", 0
  %1 = insertvalue %String %0, i32 %"$format.size6", 1
  %2 = insertvalue %String %1, i32 %ADD4, 2
  call void @"io::out"(%String %2)
  call void @"Vec.-="(%Vec* %a, i32 3)
  %a.x7 = getelementptr inbounds %Vec, %Vec* %a, i32 0, i32 0
  %"$a.x9" = load i32, i32* %a.x7, align 4
  %call.snprintf10 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.1, i32 0, i32 0), i32 %"$a.x9")
  store i32 %call.snprintf10, i32* %format.size8, align 4
  %"$format.size11" = load i32, i32* %format.size8, align 4
  %ADD12 = add i32 %"$format.size11", 1
  %AS13 = sext i32 %ADD12 to i64
  %call.calloc15 = call i8* @calloc(i64 %AS13, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc15, i8** %format.text14, align 8
  %"$format.text16" = load i8*, i8** %format.text14, align 8
  %call.snprintf17 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text16", i64 %AS13, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.1, i32 0, i32 0), i32 %"$a.x9")
  %"$format.size18" = load i32, i32* %format.size8, align 4
  %ADD19 = add i32 %"$format.size18", 1
  %"$format.text20" = load i8*, i8** %format.text14, align 8
  %"$format.size21" = load i32, i32* %format.size8, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text20", 0
  %4 = insertvalue %String %3, i32 %"$format.size21", 1
  %5 = insertvalue %String %4, i32 %ADD19, 2
  call void @"io::out"(%String %5)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
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

### stderr

```
error: test.ura:9:7 'Vec' has no operator '*=' that takes 'Vec'
  |
9 |     a *= b
  |       ^^
error: aborting due to 1 error
```

### status

```
1
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
        io::out($"drop {self.id}\n")

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
    io::out($"early {r}\n")
    g Tag = giveback()
    io::out($"g.id {g.id}\n")
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
        io::out($"({self.x}, {self.y}) tag {self.tag}\n")

fn main() i32:
    a Vec = Vec::create(1, 2)
    b Vec = Vec::create(10, 20)

    c Vec = a + b
    c.show()

    // the i32 overload's result as a declaration initialiser
    d Vec = a + 5
    d.show()

    io::out($"a == a  {a == a}\n")
    io::out($"a == b  {a == b}\n")

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
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }
%Vec = type { i32, i32, i32 }

@str = private unnamed_addr constant [17 x i8] c"(%d, %d) tag %d\0A\00", align 1
@str.2 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.4 = private unnamed_addr constant [12 x i8] c"a == a  %s\0A\00", align 1
@str.5 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.6 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.7 = private unnamed_addr constant [12 x i8] c"a == b  %s\0A\00", align 1

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

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
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

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
  ret void
}

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
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
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
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str, i32 0, i32 0), i32 %"$*self.x", i32 %"$*self.y", i32 %"$*self.tag")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf3 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str, i32 0, i32 0), i32 %"$*self.x", i32 %"$*self.y", i32 %"$*self.tag")
  %"$format.size4" = load i32, i32* %format.size, align 4
  %ADD5 = add i32 %"$format.size4", 1
  %"$format.text6" = load i8*, i8** %format.text, align 8
  %"$format.size7" = load i32, i32* %format.size, align 4
  %1 = insertvalue %String zeroinitializer, i8* %"$format.text6", 0
  %2 = insertvalue %String %1, i32 %"$format.size7", 1
  %3 = insertvalue %String %2, i32 %ADD5, 2
  call void @"io::out"(%String %3)
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
  %format.size = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text = alloca i8*, align 8
  %copy12 = alloca %Vec, align 8
  %format.size15 = alloca i32, align 4
  %bool.text16 = alloca i8*, align 8
  %format.text22 = alloca i8*, align 8
  %copy31 = alloca %Vec, align 8
  %tmp = alloca %Vec, align 8
  %e = alloca %Vec, align 8
  %copy34 = alloca %Vec, align 8
  %f = alloca %Vec, align 8
  %copy36 = alloca %Vec, align 8
  %g = alloca %Vec, align 8
  %copy39 = alloca %Vec, align 8
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
  store i32 0, i32* %format.size, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if6

if6:                                              ; preds = %entry
  br i1 %"call.Vec.==", label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.4, i32 0, i32 0), i8* %"$bool.text")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %"$bool.text6" = load i8*, i8** %bool.text, align 8
  %call.snprintf7 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.4, i32 0, i32 0), i8* %"$bool.text6")
  %"$format.size8" = load i32, i32* %format.size, align 4
  %ADD9 = add i32 %"$format.size8", 1
  %"$format.text10" = load i8*, i8** %format.text, align 8
  %"$format.size11" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text10", 0
  %1 = insertvalue %String %0, i32 %"$format.size11", 1
  %2 = insertvalue %String %1, i32 %ADD9, 2
  call void @"io::out"(%String %2)
  store %Vec zeroinitializer, %Vec* %copy12, align 4
  call void @"Vec.="(%Vec* %copy12, %Vec* %b)
  %"$copy13" = load %Vec, %Vec* %copy12, align 4
  %"call.Vec.==14" = call i1 @"Vec.=="(%Vec* %a, %Vec %"$copy13")
  store i32 0, i32* %format.size15, align 4
  store i8* null, i8** %bool.text16, align 8
  br label %if8

if8:                                              ; preds = %endif
  br i1 %"call.Vec.==14", label %if8.then, label %else9

if8.then:                                         ; preds = %if8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.5, i32 0, i32 0), i8** %bool.text16, align 8
  br label %endif1

else9:                                            ; preds = %if8
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.6, i32 0, i32 0), i8** %bool.text16, align 8
  br label %endif1

endif1:                                           ; preds = %else9, %if8.then
  %"$bool.text17" = load i8*, i8** %bool.text16, align 8
  %call.snprintf18 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.7, i32 0, i32 0), i8* %"$bool.text17")
  store i32 %call.snprintf18, i32* %format.size15, align 4
  %"$format.size19" = load i32, i32* %format.size15, align 4
  %ADD20 = add i32 %"$format.size19", 1
  %AS21 = sext i32 %ADD20 to i64
  %call.calloc23 = call i8* @calloc(i64 %AS21, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc23, i8** %format.text22, align 8
  %"$format.text24" = load i8*, i8** %format.text22, align 8
  %"$bool.text25" = load i8*, i8** %bool.text16, align 8
  %call.snprintf26 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text24", i64 %AS21, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.7, i32 0, i32 0), i8* %"$bool.text25")
  %"$format.size27" = load i32, i32* %format.size15, align 4
  %ADD28 = add i32 %"$format.size27", 1
  %"$format.text29" = load i8*, i8** %format.text22, align 8
  %"$format.size30" = load i32, i32* %format.size15, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text29", 0
  %4 = insertvalue %String %3, i32 %"$format.size30", 1
  %5 = insertvalue %String %4, i32 %ADD28, 2
  call void @"io::out"(%String %5)
  store %Vec zeroinitializer, %Vec* %copy31, align 4
  call void @"Vec.="(%Vec* %copy31, %Vec* %b)
  %"$copy32" = load %Vec, %Vec* %copy31, align 4
  %"call.Vec.+33" = call %Vec @"Vec.+"(%Vec* %a, %Vec %"$copy32")
  store %Vec %"call.Vec.+33", %Vec* %tmp, align 4
  call void @Vec.show(%Vec* %tmp)
  store %Vec zeroinitializer, %Vec* %copy34, align 4
  call void @"Vec.="(%Vec* %copy34, %Vec* %a)
  %"$copy35" = load %Vec, %Vec* %copy34, align 4
  store %Vec %"$copy35", %Vec* %e, align 4
  call void @Vec.show(%Vec* %e)
  store %Vec zeroinitializer, %Vec* %copy36, align 4
  call void @"Vec.="(%Vec* %copy36, %Vec* %a)
  %"$copy37" = load %Vec, %Vec* %copy36, align 4
  store %Vec %"$copy37", %Vec* %f, align 4
  call void @Vec.show(%Vec* %f)
  %call.Vec.create38 = call %Vec @Vec.create(i32 10, i32 10)
  store %Vec %call.Vec.create38, %Vec* %g, align 4
  store %Vec zeroinitializer, %Vec* %copy39, align 4
  call void @"Vec.="(%Vec* %copy39, %Vec* %b)
  %"$copy40" = load %Vec, %Vec* %copy39, align 4
  call void @"Vec.+="(%Vec* %g, %Vec %"$copy40")
  call void @Vec.show(%Vec* %g)
  call void @"Vec.-="(%Vec* %g, i32 3)
  call void @Vec.show(%Vec* %g)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
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
        io::out($"show {self.id}\n")

    operator drop:
        io::out($"DROP {self.id}\n")

fn via_return(a R, b R) i32:
    return (a + b).id

fn main() i32:
    a R = R::create(1)
    b R = R::create(2)
    (a + b).show()
    io::out("--\n")
    io::out($"{(a + b).id}\n")
    io::out("--\n")
    // a chain makes two temporaries; both die, in reverse
    (a + b + a).show()
    io::out("--\n")
    i i32 = 0
    while i < 2:
        (a + b).show()
        i += 1
    io::out("--\n")
    // dropped before the ret, not after it
    io::out($"{via_return(a, b)}\n")
    io::out("--\n")
    io::out($"{R::create(9).id}\n")
    io::out("--\n")
    // a struct with no destructor is never recorded as a temporary
    io::out($"{Plain::create(7).x}\n")
    io::out("end\n")
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
        io::out($"{label}({self.x}, {self.y})\n")

fn main() i32:
    a Vec2 = Vec2::create(9, 12)
    b Vec2 = Vec2::create(2, 3)

    (a + b).show("a + b   ")
    (a - b).show("a - b   ")
    (a * 3).show("a * 3   ")
    (a / 2).show("a / 2   ")
    (a % 4).show("a mod 4 ")

    io::out($"== {a == b}  != {a != b}\n")
    io::out($"<  {a < b}  >  {a > b}\n")
    io::out($"<= {a <= b}  >= {a >= b}\n")

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
        io::out("a is longer\n")
    n i32 = 0
    d Vec2 = Vec2::create(1, 1)
    while d < a:
        d += b
        n += 1
    io::out($"steps to pass a: {n}\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }
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

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
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

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
  ret void
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
  br label %if6

if6:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %LT = icmp slt i32 %"$n", 0
  br i1 %LT, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"$n1" = load i32, i32* %n, align 4
  %SUB = sub i32 0, %"$n1"
  ret i32 %SUB

endif:                                            ; preds = %if6
  %"$n2" = load i32, i32* %n, align 4
  ret i32 %"$n2"
}

define void @Vec2.show(%Vec2* %0, i8* %1) {
entry:
  %self = alloca %Vec2*, align 8
  %label = alloca i8*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  store i8* %1, i8** %label, align 8
  %"*self" = load %Vec2*, %Vec2** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec2, %Vec2* %"*self", i32 0, i32 0
  %"*self1" = load %Vec2*, %Vec2** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec2, %Vec2* %"*self1", i32 0, i32 1
  %"$label" = load i8*, i8** %label, align 8
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i8* %"$label", i32 %"$*self.x", i32 %"$*self.y")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf2 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i8* %"$label", i32 %"$*self.x", i32 %"$*self.y")
  %"$format.size3" = load i32, i32* %format.size, align 4
  %ADD4 = add i32 %"$format.size3", 1
  %"$format.text5" = load i8*, i8** %format.text, align 8
  %"$format.size6" = load i32, i32* %format.size, align 4
  %2 = insertvalue %String zeroinitializer, i8* %"$format.text5", 0
  %3 = insertvalue %String %2, i32 %"$format.size6", 1
  %4 = insertvalue %String %3, i32 %ADD4, 2
  call void @"io::out"(%String %4)
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
  %format.size = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %bool.text18 = alloca i8*, align 8
  %format.text = alloca i8*, align 8
  %copy27 = alloca %Vec2, align 8
  %copy29 = alloca %Vec2, align 8
  %format.size31 = alloca i32, align 4
  %bool.text32 = alloca i8*, align 8
  %bool.text33 = alloca i8*, align 8
  %format.text40 = alloca i8*, align 8
  %copy50 = alloca %Vec2, align 8
  %copy52 = alloca %Vec2, align 8
  %format.size54 = alloca i32, align 4
  %bool.text55 = alloca i8*, align 8
  %bool.text56 = alloca i8*, align 8
  %format.text63 = alloca i8*, align 8
  %c = alloca %Vec2, align 8
  %copy74 = alloca %Vec2, align 8
  %copy76 = alloca %Vec2, align 8
  %copy78 = alloca %Vec2, align 8
  %copy80 = alloca %Vec2, align 8
  %n = alloca i32, align 4
  %d = alloca %Vec2, align 8
  %copy84 = alloca %Vec2, align 8
  %copy87 = alloca %Vec2, align 8
  %format.size90 = alloca i32, align 4
  %format.text96 = alloca i8*, align 8
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
  store i32 0, i32* %format.size, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if7

if7:                                              ; preds = %entry
  br i1 %"call.Vec2.==", label %if7.then, label %else8

if7.then:                                         ; preds = %if7
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.6, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else8:                                            ; preds = %if7
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.7, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else8, %if7.then
  store i8* null, i8** %bool.text18, align 8
  br label %if9

if9:                                              ; preds = %endif
  br i1 %"call.Vec2.!=", label %if9.then, label %else10

if9.then:                                         ; preds = %if9
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.8, i32 0, i32 0), i8** %bool.text18, align 8
  br label %endif1

else10:                                           ; preds = %if9
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.9, i32 0, i32 0), i8** %bool.text18, align 8
  br label %endif1

endif1:                                           ; preds = %else10, %if9.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %"$bool.text19" = load i8*, i8** %bool.text18, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.10, i32 0, i32 0), i8* %"$bool.text", i8* %"$bool.text19")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %"$bool.text20" = load i8*, i8** %bool.text, align 8
  %"$bool.text21" = load i8*, i8** %bool.text18, align 8
  %call.snprintf22 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.10, i32 0, i32 0), i8* %"$bool.text20", i8* %"$bool.text21")
  %"$format.size23" = load i32, i32* %format.size, align 4
  %ADD24 = add i32 %"$format.size23", 1
  %"$format.text25" = load i8*, i8** %format.text, align 8
  %"$format.size26" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text25", 0
  %1 = insertvalue %String %0, i32 %"$format.size26", 1
  %2 = insertvalue %String %1, i32 %ADD24, 2
  call void @"io::out"(%String %2)
  store %Vec2 zeroinitializer, %Vec2* %copy27, align 4
  call void @"Vec2.="(%Vec2* %copy27, %Vec2* %b)
  %"$copy28" = load %Vec2, %Vec2* %copy27, align 4
  %"call.Vec2.<" = call i1 @"Vec2.<"(%Vec2* %a, %Vec2 %"$copy28")
  store %Vec2 zeroinitializer, %Vec2* %copy29, align 4
  call void @"Vec2.="(%Vec2* %copy29, %Vec2* %b)
  %"$copy30" = load %Vec2, %Vec2* %copy29, align 4
  %"call.Vec2.>" = call i1 @"Vec2.>"(%Vec2* %a, %Vec2 %"$copy30")
  store i32 0, i32* %format.size31, align 4
  store i8* null, i8** %bool.text32, align 8
  br label %if11

if11:                                             ; preds = %endif1
  br i1 %"call.Vec2.<", label %if11.then, label %else12

if11.then:                                        ; preds = %if11
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.11, i32 0, i32 0), i8** %bool.text32, align 8
  br label %endif2

else12:                                           ; preds = %if11
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.12, i32 0, i32 0), i8** %bool.text32, align 8
  br label %endif2

endif2:                                           ; preds = %else12, %if11.then
  store i8* null, i8** %bool.text33, align 8
  br label %if13

if13:                                             ; preds = %endif2
  br i1 %"call.Vec2.>", label %if13.then, label %else14

if13.then:                                        ; preds = %if13
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.13, i32 0, i32 0), i8** %bool.text33, align 8
  br label %endif3

else14:                                           ; preds = %if13
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.14, i32 0, i32 0), i8** %bool.text33, align 8
  br label %endif3

endif3:                                           ; preds = %else14, %if13.then
  %"$bool.text34" = load i8*, i8** %bool.text32, align 8
  %"$bool.text35" = load i8*, i8** %bool.text33, align 8
  %call.snprintf36 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.15, i32 0, i32 0), i8* %"$bool.text34", i8* %"$bool.text35")
  store i32 %call.snprintf36, i32* %format.size31, align 4
  %"$format.size37" = load i32, i32* %format.size31, align 4
  %ADD38 = add i32 %"$format.size37", 1
  %AS39 = sext i32 %ADD38 to i64
  %call.calloc41 = call i8* @calloc(i64 %AS39, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc41, i8** %format.text40, align 8
  %"$format.text42" = load i8*, i8** %format.text40, align 8
  %"$bool.text43" = load i8*, i8** %bool.text32, align 8
  %"$bool.text44" = load i8*, i8** %bool.text33, align 8
  %call.snprintf45 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text42", i64 %AS39, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.15, i32 0, i32 0), i8* %"$bool.text43", i8* %"$bool.text44")
  %"$format.size46" = load i32, i32* %format.size31, align 4
  %ADD47 = add i32 %"$format.size46", 1
  %"$format.text48" = load i8*, i8** %format.text40, align 8
  %"$format.size49" = load i32, i32* %format.size31, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text48", 0
  %4 = insertvalue %String %3, i32 %"$format.size49", 1
  %5 = insertvalue %String %4, i32 %ADD47, 2
  call void @"io::out"(%String %5)
  store %Vec2 zeroinitializer, %Vec2* %copy50, align 4
  call void @"Vec2.="(%Vec2* %copy50, %Vec2* %b)
  %"$copy51" = load %Vec2, %Vec2* %copy50, align 4
  %"call.Vec2.<=" = call i1 @"Vec2.<="(%Vec2* %a, %Vec2 %"$copy51")
  store %Vec2 zeroinitializer, %Vec2* %copy52, align 4
  call void @"Vec2.="(%Vec2* %copy52, %Vec2* %b)
  %"$copy53" = load %Vec2, %Vec2* %copy52, align 4
  %"call.Vec2.>=" = call i1 @"Vec2.>="(%Vec2* %a, %Vec2 %"$copy53")
  store i32 0, i32* %format.size54, align 4
  store i8* null, i8** %bool.text55, align 8
  br label %if15

if15:                                             ; preds = %endif3
  br i1 %"call.Vec2.<=", label %if15.then, label %else16

if15.then:                                        ; preds = %if15
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.16, i32 0, i32 0), i8** %bool.text55, align 8
  br label %endif4

else16:                                           ; preds = %if15
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.17, i32 0, i32 0), i8** %bool.text55, align 8
  br label %endif4

endif4:                                           ; preds = %else16, %if15.then
  store i8* null, i8** %bool.text56, align 8
  br label %if17

if17:                                             ; preds = %endif4
  br i1 %"call.Vec2.>=", label %if17.then, label %else18

if17.then:                                        ; preds = %if17
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.18, i32 0, i32 0), i8** %bool.text56, align 8
  br label %endif5

else18:                                           ; preds = %if17
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.19, i32 0, i32 0), i8** %bool.text56, align 8
  br label %endif5

endif5:                                           ; preds = %else18, %if17.then
  %"$bool.text57" = load i8*, i8** %bool.text55, align 8
  %"$bool.text58" = load i8*, i8** %bool.text56, align 8
  %call.snprintf59 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.20, i32 0, i32 0), i8* %"$bool.text57", i8* %"$bool.text58")
  store i32 %call.snprintf59, i32* %format.size54, align 4
  %"$format.size60" = load i32, i32* %format.size54, align 4
  %ADD61 = add i32 %"$format.size60", 1
  %AS62 = sext i32 %ADD61 to i64
  %call.calloc64 = call i8* @calloc(i64 %AS62, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc64, i8** %format.text63, align 8
  %"$format.text65" = load i8*, i8** %format.text63, align 8
  %"$bool.text66" = load i8*, i8** %bool.text55, align 8
  %"$bool.text67" = load i8*, i8** %bool.text56, align 8
  %call.snprintf68 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text65", i64 %AS62, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.20, i32 0, i32 0), i8* %"$bool.text66", i8* %"$bool.text67")
  %"$format.size69" = load i32, i32* %format.size54, align 4
  %ADD70 = add i32 %"$format.size69", 1
  %"$format.text71" = load i8*, i8** %format.text63, align 8
  %"$format.size72" = load i32, i32* %format.size54, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text71", 0
  %7 = insertvalue %String %6, i32 %"$format.size72", 1
  %8 = insertvalue %String %7, i32 %ADD70, 2
  call void @"io::out"(%String %8)
  %call.Vec2.create73 = call %Vec2 @Vec2.create(i32 0, i32 0)
  store %Vec2 %call.Vec2.create73, %Vec2* %c, align 4
  store %Vec2 zeroinitializer, %Vec2* %copy74, align 4
  call void @"Vec2.="(%Vec2* %copy74, %Vec2* %a)
  %"$copy75" = load %Vec2, %Vec2* %copy74, align 4
  store %Vec2 %"$copy75", %Vec2* %c, align 4
  call void @Vec2.show(%Vec2* %c, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.21, i32 0, i32 0))
  store %Vec2 zeroinitializer, %Vec2* %copy76, align 4
  call void @"Vec2.="(%Vec2* %copy76, %Vec2* %b)
  %"$copy77" = load %Vec2, %Vec2* %copy76, align 4
  call void @"Vec2.+="(%Vec2* %c, %Vec2 %"$copy77")
  call void @Vec2.show(%Vec2* %c, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.22, i32 0, i32 0))
  store %Vec2 zeroinitializer, %Vec2* %copy78, align 4
  call void @"Vec2.="(%Vec2* %copy78, %Vec2* %b)
  %"$copy79" = load %Vec2, %Vec2* %copy78, align 4
  call void @"Vec2.-="(%Vec2* %c, %Vec2 %"$copy79")
  call void @Vec2.show(%Vec2* %c, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.23, i32 0, i32 0))
  call void @"Vec2.*="(%Vec2* %c, i32 2)
  call void @Vec2.show(%Vec2* %c, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.24, i32 0, i32 0))
  call void @"Vec2./="(%Vec2* %c, i32 3)
  call void @Vec2.show(%Vec2* %c, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.25, i32 0, i32 0))
  call void @"Vec2.%="(%Vec2* %c, i32 5)
  call void @Vec2.show(%Vec2* %c, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.26, i32 0, i32 0))
  br label %if19

if19:                                             ; preds = %endif5
  store %Vec2 zeroinitializer, %Vec2* %copy80, align 4
  call void @"Vec2.="(%Vec2* %copy80, %Vec2* %b)
  %"$copy81" = load %Vec2, %Vec2* %copy80, align 4
  %"call.Vec2.>82" = call i1 @"Vec2.>"(%Vec2* %a, %Vec2 %"$copy81")
  br i1 %"call.Vec2.>82", label %if19.then, label %endif6

if19.then:                                        ; preds = %if19
  call void @"io::out"(%String { i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.27, i32 0, i32 0), i32 12, i32 0 })
  br label %endif6

endif6:                                           ; preds = %if19.then, %if19
  store i32 0, i32* %n, align 4
  %call.Vec2.create83 = call %Vec2 @Vec2.create(i32 1, i32 1)
  store %Vec2 %call.Vec2.create83, %Vec2* %d, align 4
  br label %while20

while20:                                          ; preds = %while20.then, %endif6
  store %Vec2 zeroinitializer, %Vec2* %copy84, align 4
  call void @"Vec2.="(%Vec2* %copy84, %Vec2* %a)
  %"$copy85" = load %Vec2, %Vec2* %copy84, align 4
  %"call.Vec2.<86" = call i1 @"Vec2.<"(%Vec2* %d, %Vec2 %"$copy85")
  br i1 %"call.Vec2.<86", label %while20.then, label %endloop

while20.then:                                     ; preds = %while20
  store %Vec2 zeroinitializer, %Vec2* %copy87, align 4
  call void @"Vec2.="(%Vec2* %copy87, %Vec2* %b)
  %"$copy88" = load %Vec2, %Vec2* %copy87, align 4
  call void @"Vec2.+="(%Vec2* %d, %Vec2 %"$copy88")
  %"$n" = load i32, i32* %n, align 4
  %ADD89 = add i32 %"$n", 1
  store i32 %ADD89, i32* %n, align 4
  br label %while20

endloop:                                          ; preds = %while20
  %"$n91" = load i32, i32* %n, align 4
  %call.snprintf92 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.28, i32 0, i32 0), i32 %"$n91")
  store i32 %call.snprintf92, i32* %format.size90, align 4
  %"$format.size93" = load i32, i32* %format.size90, align 4
  %ADD94 = add i32 %"$format.size93", 1
  %AS95 = sext i32 %ADD94 to i64
  %call.calloc97 = call i8* @calloc(i64 %AS95, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc97, i8** %format.text96, align 8
  %"$format.text98" = load i8*, i8** %format.text96, align 8
  %call.snprintf99 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text98", i64 %AS95, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str.28, i32 0, i32 0), i32 %"$n91")
  %"$format.size100" = load i32, i32* %format.size90, align 4
  %ADD101 = add i32 %"$format.size100", 1
  %"$format.text102" = load i8*, i8** %format.text96, align 8
  %"$format.size103" = load i32, i32* %format.size90, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text102", 0
  %10 = insertvalue %String %9, i32 %"$format.size103", 1
  %11 = insertvalue %String %10, i32 %ADD101, 2
  call void @"io::out"(%String %11)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
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
    io::out($"and {(a & b).bits}  or {(a | b).bits}")
    io::out($"  xor {(a ^ b).bits}\n")
    io::out($"shl {(a << 2).bits}  shr {(a >> 2).bits}\n")

    p Vec2 = Vec2::create(2, 3)
    q Vec2 = Vec2::create(4, 5)
    io::out($"dot {p * q}\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }
%Flags = type { i32 }
%Vec2 = type { i32, i32 }

@str = private unnamed_addr constant [14 x i8] c"and %d  or %d\00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c"  xor %d\0A\00", align 1
@str.2 = private unnamed_addr constant [16 x i8] c"shl %d  shr %d\0A\00", align 1
@str.3 = private unnamed_addr constant [8 x i8] c"dot %d\0A\00", align 1

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

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
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

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
  ret void
}

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
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %tmp12 = alloca %Flags, align 8
  %format.size14 = alloca i32, align 4
  %format.text20 = alloca i8*, align 8
  %tmp28 = alloca %Flags, align 8
  %tmp30 = alloca %Flags, align 8
  %format.size32 = alloca i32, align 4
  %format.text39 = alloca i8*, align 8
  %p = alloca %Vec2, align 8
  %q = alloca %Vec2, align 8
  %format.size48 = alloca i32, align 4
  %format.text53 = alloca i8*, align 8
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
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str, i32 0, i32 0), i32 %"$tmp.bits", i32 %"$tmp.bits5")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf6 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str, i32 0, i32 0), i32 %"$tmp.bits", i32 %"$tmp.bits5")
  %"$format.size7" = load i32, i32* %format.size, align 4
  %ADD8 = add i32 %"$format.size7", 1
  %"$format.text9" = load i8*, i8** %format.text, align 8
  %"$format.size10" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text9", 0
  %1 = insertvalue %String %0, i32 %"$format.size10", 1
  %2 = insertvalue %String %1, i32 %ADD8, 2
  call void @"io::out"(%String %2)
  %"$b11" = load %Flags, %Flags* %b, align 4
  %"call.Flags.^" = call %Flags @"Flags.^"(%Flags* %a, %Flags %"$b11")
  store %Flags %"call.Flags.^", %Flags* %tmp12, align 4
  %tmp.bits13 = getelementptr inbounds %Flags, %Flags* %tmp12, i32 0, i32 0
  %"$tmp.bits15" = load i32, i32* %tmp.bits13, align 4
  %call.snprintf16 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i32 %"$tmp.bits15")
  store i32 %call.snprintf16, i32* %format.size14, align 4
  %"$format.size17" = load i32, i32* %format.size14, align 4
  %ADD18 = add i32 %"$format.size17", 1
  %AS19 = sext i32 %ADD18 to i64
  %call.calloc21 = call i8* @calloc(i64 %AS19, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc21, i8** %format.text20, align 8
  %"$format.text22" = load i8*, i8** %format.text20, align 8
  %call.snprintf23 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text22", i64 %AS19, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i32 %"$tmp.bits15")
  %"$format.size24" = load i32, i32* %format.size14, align 4
  %ADD25 = add i32 %"$format.size24", 1
  %"$format.text26" = load i8*, i8** %format.text20, align 8
  %"$format.size27" = load i32, i32* %format.size14, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text26", 0
  %4 = insertvalue %String %3, i32 %"$format.size27", 1
  %5 = insertvalue %String %4, i32 %ADD25, 2
  call void @"io::out"(%String %5)
  %"call.Flags.<<" = call %Flags @"Flags.<<"(%Flags* %a, i32 2)
  store %Flags %"call.Flags.<<", %Flags* %tmp28, align 4
  %tmp.bits29 = getelementptr inbounds %Flags, %Flags* %tmp28, i32 0, i32 0
  %"call.Flags.>>" = call %Flags @"Flags.>>"(%Flags* %a, i32 2)
  store %Flags %"call.Flags.>>", %Flags* %tmp30, align 4
  %tmp.bits31 = getelementptr inbounds %Flags, %Flags* %tmp30, i32 0, i32 0
  %"$tmp.bits33" = load i32, i32* %tmp.bits29, align 4
  %"$tmp.bits34" = load i32, i32* %tmp.bits31, align 4
  %call.snprintf35 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.2, i32 0, i32 0), i32 %"$tmp.bits33", i32 %"$tmp.bits34")
  store i32 %call.snprintf35, i32* %format.size32, align 4
  %"$format.size36" = load i32, i32* %format.size32, align 4
  %ADD37 = add i32 %"$format.size36", 1
  %AS38 = sext i32 %ADD37 to i64
  %call.calloc40 = call i8* @calloc(i64 %AS38, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc40, i8** %format.text39, align 8
  %"$format.text41" = load i8*, i8** %format.text39, align 8
  %call.snprintf42 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text41", i64 %AS38, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.2, i32 0, i32 0), i32 %"$tmp.bits33", i32 %"$tmp.bits34")
  %"$format.size43" = load i32, i32* %format.size32, align 4
  %ADD44 = add i32 %"$format.size43", 1
  %"$format.text45" = load i8*, i8** %format.text39, align 8
  %"$format.size46" = load i32, i32* %format.size32, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text45", 0
  %7 = insertvalue %String %6, i32 %"$format.size46", 1
  %8 = insertvalue %String %7, i32 %ADD44, 2
  call void @"io::out"(%String %8)
  %call.Vec2.create = call %Vec2 @Vec2.create(i32 2, i32 3)
  store %Vec2 %call.Vec2.create, %Vec2* %p, align 4
  %call.Vec2.create47 = call %Vec2 @Vec2.create(i32 4, i32 5)
  store %Vec2 %call.Vec2.create47, %Vec2* %q, align 4
  %"$q" = load %Vec2, %Vec2* %q, align 4
  %"call.Vec2.*" = call i32 @"Vec2.*"(%Vec2* %p, %Vec2 %"$q")
  %call.snprintf49 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.3, i32 0, i32 0), i32 %"call.Vec2.*")
  store i32 %call.snprintf49, i32* %format.size48, align 4
  %"$format.size50" = load i32, i32* %format.size48, align 4
  %ADD51 = add i32 %"$format.size50", 1
  %AS52 = sext i32 %ADD51 to i64
  %call.calloc54 = call i8* @calloc(i64 %AS52, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc54, i8** %format.text53, align 8
  %"$format.text55" = load i8*, i8** %format.text53, align 8
  %call.snprintf56 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text55", i64 %AS52, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.3, i32 0, i32 0), i32 %"call.Vec2.*")
  %"$format.size57" = load i32, i32* %format.size48, align 4
  %ADD58 = add i32 %"$format.size57", 1
  %"$format.text59" = load i8*, i8** %format.text53, align 8
  %"$format.size60" = load i32, i32* %format.size48, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text59", 0
  %10 = insertvalue %String %9, i32 %"$format.size60", 1
  %11 = insertvalue %String %10, i32 %ADD58, 2
  call void @"io::out"(%String %11)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
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
    io::out($"{f.bits} ")
    f |= 3
    io::out($"{f.bits} ")
    f ^= 5
    io::out($"{f.bits} ")
    f <<= 2
    io::out($"{f.bits} ")
    f >>= 1
    io::out($"{f.bits}\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }
%Flags = type { i32 }

@str = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str.3 = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str.4 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

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

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
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

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
  ret void
}

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
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %format.size8 = alloca i32, align 4
  %format.text14 = alloca i8*, align 8
  %format.size23 = alloca i32, align 4
  %format.text29 = alloca i8*, align 8
  %format.size38 = alloca i32, align 4
  %format.text44 = alloca i8*, align 8
  %format.size53 = alloca i32, align 4
  %format.text59 = alloca i8*, align 8
  store %Flags zeroinitializer, %Flags* %f, align 4
  %f.bits = getelementptr inbounds %Flags, %Flags* %f, i32 0, i32 0
  store i32 12, i32* %f.bits, align 4
  call void @"Flags.&="(%Flags* %f, i32 10)
  %f.bits1 = getelementptr inbounds %Flags, %Flags* %f, i32 0, i32 0
  %"$f.bits" = load i32, i32* %f.bits1, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$f.bits")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf2 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$f.bits")
  %"$format.size3" = load i32, i32* %format.size, align 4
  %ADD4 = add i32 %"$format.size3", 1
  %"$format.text5" = load i8*, i8** %format.text, align 8
  %"$format.size6" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text5", 0
  %1 = insertvalue %String %0, i32 %"$format.size6", 1
  %2 = insertvalue %String %1, i32 %ADD4, 2
  call void @"io::out"(%String %2)
  call void @"Flags.|="(%Flags* %f, i32 3)
  %f.bits7 = getelementptr inbounds %Flags, %Flags* %f, i32 0, i32 0
  %"$f.bits9" = load i32, i32* %f.bits7, align 4
  %call.snprintf10 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 %"$f.bits9")
  store i32 %call.snprintf10, i32* %format.size8, align 4
  %"$format.size11" = load i32, i32* %format.size8, align 4
  %ADD12 = add i32 %"$format.size11", 1
  %AS13 = sext i32 %ADD12 to i64
  %call.calloc15 = call i8* @calloc(i64 %AS13, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc15, i8** %format.text14, align 8
  %"$format.text16" = load i8*, i8** %format.text14, align 8
  %call.snprintf17 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text16", i64 %AS13, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 %"$f.bits9")
  %"$format.size18" = load i32, i32* %format.size8, align 4
  %ADD19 = add i32 %"$format.size18", 1
  %"$format.text20" = load i8*, i8** %format.text14, align 8
  %"$format.size21" = load i32, i32* %format.size8, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text20", 0
  %4 = insertvalue %String %3, i32 %"$format.size21", 1
  %5 = insertvalue %String %4, i32 %ADD19, 2
  call void @"io::out"(%String %5)
  call void @"Flags.^="(%Flags* %f, i32 5)
  %f.bits22 = getelementptr inbounds %Flags, %Flags* %f, i32 0, i32 0
  %"$f.bits24" = load i32, i32* %f.bits22, align 4
  %call.snprintf25 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 %"$f.bits24")
  store i32 %call.snprintf25, i32* %format.size23, align 4
  %"$format.size26" = load i32, i32* %format.size23, align 4
  %ADD27 = add i32 %"$format.size26", 1
  %AS28 = sext i32 %ADD27 to i64
  %call.calloc30 = call i8* @calloc(i64 %AS28, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc30, i8** %format.text29, align 8
  %"$format.text31" = load i8*, i8** %format.text29, align 8
  %call.snprintf32 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text31", i64 %AS28, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 %"$f.bits24")
  %"$format.size33" = load i32, i32* %format.size23, align 4
  %ADD34 = add i32 %"$format.size33", 1
  %"$format.text35" = load i8*, i8** %format.text29, align 8
  %"$format.size36" = load i32, i32* %format.size23, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text35", 0
  %7 = insertvalue %String %6, i32 %"$format.size36", 1
  %8 = insertvalue %String %7, i32 %ADD34, 2
  call void @"io::out"(%String %8)
  call void @"Flags.<<="(%Flags* %f, i32 2)
  %f.bits37 = getelementptr inbounds %Flags, %Flags* %f, i32 0, i32 0
  %"$f.bits39" = load i32, i32* %f.bits37, align 4
  %call.snprintf40 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.3, i32 0, i32 0), i32 %"$f.bits39")
  store i32 %call.snprintf40, i32* %format.size38, align 4
  %"$format.size41" = load i32, i32* %format.size38, align 4
  %ADD42 = add i32 %"$format.size41", 1
  %AS43 = sext i32 %ADD42 to i64
  %call.calloc45 = call i8* @calloc(i64 %AS43, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc45, i8** %format.text44, align 8
  %"$format.text46" = load i8*, i8** %format.text44, align 8
  %call.snprintf47 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text46", i64 %AS43, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.3, i32 0, i32 0), i32 %"$f.bits39")
  %"$format.size48" = load i32, i32* %format.size38, align 4
  %ADD49 = add i32 %"$format.size48", 1
  %"$format.text50" = load i8*, i8** %format.text44, align 8
  %"$format.size51" = load i32, i32* %format.size38, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text50", 0
  %10 = insertvalue %String %9, i32 %"$format.size51", 1
  %11 = insertvalue %String %10, i32 %ADD49, 2
  call void @"io::out"(%String %11)
  call void @"Flags.>>="(%Flags* %f, i32 1)
  %f.bits52 = getelementptr inbounds %Flags, %Flags* %f, i32 0, i32 0
  %"$f.bits54" = load i32, i32* %f.bits52, align 4
  %call.snprintf55 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i32 %"$f.bits54")
  store i32 %call.snprintf55, i32* %format.size53, align 4
  %"$format.size56" = load i32, i32* %format.size53, align 4
  %ADD57 = add i32 %"$format.size56", 1
  %AS58 = sext i32 %ADD57 to i64
  %call.calloc60 = call i8* @calloc(i64 %AS58, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc60, i8** %format.text59, align 8
  %"$format.text61" = load i8*, i8** %format.text59, align 8
  %call.snprintf62 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text61", i64 %AS58, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i32 %"$f.bits54")
  %"$format.size63" = load i32, i32* %format.size53, align 4
  %ADD64 = add i32 %"$format.size63", 1
  %"$format.text65" = load i8*, i8** %format.text59, align 8
  %"$format.size66" = load i32, i32* %format.size53, align 4
  %12 = insertvalue %String zeroinitializer, i8* %"$format.text65", 0
  %13 = insertvalue %String %12, i32 %"$format.size66", 1
  %14 = insertvalue %String %13, i32 %ADD64, 2
  call void @"io::out"(%String %14)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
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
    io::out($"a & b:  {band.bits}\n")

    bor Flags = a | b
    io::out($"a | b:  {bor.bits}\n")

    bxor Flags = a ^ b
    io::out($"a ^ b:  {bxor.bits}\n")

    lsh Flags = a << 2
    io::out($"a << 2: {lsh.bits}\n")

    rsh Flags = a >> 1
    io::out($"a >> 1: {rsh.bits}\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }
%Flags = type { i32 }

@str = private unnamed_addr constant [12 x i8] c"a & b:  %d\0A\00", align 1
@str.1 = private unnamed_addr constant [12 x i8] c"a | b:  %d\0A\00", align 1
@str.2 = private unnamed_addr constant [12 x i8] c"a ^ b:  %d\0A\00", align 1
@str.3 = private unnamed_addr constant [12 x i8] c"a << 2: %d\0A\00", align 1
@str.4 = private unnamed_addr constant [12 x i8] c"a >> 1: %d\0A\00", align 1

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

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
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

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
  ret void
}

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
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %bor = alloca %Flags, align 8
  %format.size6 = alloca i32, align 4
  %format.text11 = alloca i8*, align 8
  %bxor = alloca %Flags, align 8
  %format.size19 = alloca i32, align 4
  %format.text24 = alloca i8*, align 8
  %lsh = alloca %Flags, align 8
  %format.size32 = alloca i32, align 4
  %format.text37 = alloca i8*, align 8
  %rsh = alloca %Flags, align 8
  %format.size45 = alloca i32, align 4
  %format.text50 = alloca i8*, align 8
  store %Flags zeroinitializer, %Flags* %a, align 4
  call void @"Flags.="(%Flags* %a, i32 12)
  store %Flags zeroinitializer, %Flags* %b, align 4
  call void @"Flags.="(%Flags* %b, i32 10)
  %"call.Flags.&" = call %Flags @"Flags.&"(%Flags* %a, %Flags* %b)
  store %Flags %"call.Flags.&", %Flags* %band, align 4
  %band.bits = getelementptr inbounds %Flags, %Flags* %band, i32 0, i32 0
  %"$band.bits" = load i32, i32* %band.bits, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i32 %"$band.bits")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i32 %"$band.bits")
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  call void @"io::out"(%String %2)
  %"call.Flags.|" = call %Flags @"Flags.|"(%Flags* %a, %Flags* %b)
  store %Flags %"call.Flags.|", %Flags* %bor, align 4
  %bor.bits = getelementptr inbounds %Flags, %Flags* %bor, i32 0, i32 0
  %"$bor.bits" = load i32, i32* %bor.bits, align 4
  %call.snprintf7 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.1, i32 0, i32 0), i32 %"$bor.bits")
  store i32 %call.snprintf7, i32* %format.size6, align 4
  %"$format.size8" = load i32, i32* %format.size6, align 4
  %ADD9 = add i32 %"$format.size8", 1
  %AS10 = sext i32 %ADD9 to i64
  %call.calloc12 = call i8* @calloc(i64 %AS10, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc12, i8** %format.text11, align 8
  %"$format.text13" = load i8*, i8** %format.text11, align 8
  %call.snprintf14 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text13", i64 %AS10, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.1, i32 0, i32 0), i32 %"$bor.bits")
  %"$format.size15" = load i32, i32* %format.size6, align 4
  %ADD16 = add i32 %"$format.size15", 1
  %"$format.text17" = load i8*, i8** %format.text11, align 8
  %"$format.size18" = load i32, i32* %format.size6, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text17", 0
  %4 = insertvalue %String %3, i32 %"$format.size18", 1
  %5 = insertvalue %String %4, i32 %ADD16, 2
  call void @"io::out"(%String %5)
  %"call.Flags.^" = call %Flags @"Flags.^"(%Flags* %a, %Flags* %b)
  store %Flags %"call.Flags.^", %Flags* %bxor, align 4
  %bxor.bits = getelementptr inbounds %Flags, %Flags* %bxor, i32 0, i32 0
  %"$bxor.bits" = load i32, i32* %bxor.bits, align 4
  %call.snprintf20 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.2, i32 0, i32 0), i32 %"$bxor.bits")
  store i32 %call.snprintf20, i32* %format.size19, align 4
  %"$format.size21" = load i32, i32* %format.size19, align 4
  %ADD22 = add i32 %"$format.size21", 1
  %AS23 = sext i32 %ADD22 to i64
  %call.calloc25 = call i8* @calloc(i64 %AS23, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc25, i8** %format.text24, align 8
  %"$format.text26" = load i8*, i8** %format.text24, align 8
  %call.snprintf27 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text26", i64 %AS23, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.2, i32 0, i32 0), i32 %"$bxor.bits")
  %"$format.size28" = load i32, i32* %format.size19, align 4
  %ADD29 = add i32 %"$format.size28", 1
  %"$format.text30" = load i8*, i8** %format.text24, align 8
  %"$format.size31" = load i32, i32* %format.size19, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text30", 0
  %7 = insertvalue %String %6, i32 %"$format.size31", 1
  %8 = insertvalue %String %7, i32 %ADD29, 2
  call void @"io::out"(%String %8)
  %"call.Flags.<<" = call %Flags @"Flags.<<"(%Flags* %a, i32 2)
  store %Flags %"call.Flags.<<", %Flags* %lsh, align 4
  %lsh.bits = getelementptr inbounds %Flags, %Flags* %lsh, i32 0, i32 0
  %"$lsh.bits" = load i32, i32* %lsh.bits, align 4
  %call.snprintf33 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.3, i32 0, i32 0), i32 %"$lsh.bits")
  store i32 %call.snprintf33, i32* %format.size32, align 4
  %"$format.size34" = load i32, i32* %format.size32, align 4
  %ADD35 = add i32 %"$format.size34", 1
  %AS36 = sext i32 %ADD35 to i64
  %call.calloc38 = call i8* @calloc(i64 %AS36, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc38, i8** %format.text37, align 8
  %"$format.text39" = load i8*, i8** %format.text37, align 8
  %call.snprintf40 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text39", i64 %AS36, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.3, i32 0, i32 0), i32 %"$lsh.bits")
  %"$format.size41" = load i32, i32* %format.size32, align 4
  %ADD42 = add i32 %"$format.size41", 1
  %"$format.text43" = load i8*, i8** %format.text37, align 8
  %"$format.size44" = load i32, i32* %format.size32, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text43", 0
  %10 = insertvalue %String %9, i32 %"$format.size44", 1
  %11 = insertvalue %String %10, i32 %ADD42, 2
  call void @"io::out"(%String %11)
  %"call.Flags.>>" = call %Flags @"Flags.>>"(%Flags* %a, i32 1)
  store %Flags %"call.Flags.>>", %Flags* %rsh, align 4
  %rsh.bits = getelementptr inbounds %Flags, %Flags* %rsh, i32 0, i32 0
  %"$rsh.bits" = load i32, i32* %rsh.bits, align 4
  %call.snprintf46 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.4, i32 0, i32 0), i32 %"$rsh.bits")
  store i32 %call.snprintf46, i32* %format.size45, align 4
  %"$format.size47" = load i32, i32* %format.size45, align 4
  %ADD48 = add i32 %"$format.size47", 1
  %AS49 = sext i32 %ADD48 to i64
  %call.calloc51 = call i8* @calloc(i64 %AS49, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc51, i8** %format.text50, align 8
  %"$format.text52" = load i8*, i8** %format.text50, align 8
  %call.snprintf53 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text52", i64 %AS49, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.4, i32 0, i32 0), i32 %"$rsh.bits")
  %"$format.size54" = load i32, i32* %format.size45, align 4
  %ADD55 = add i32 %"$format.size54", 1
  %"$format.text56" = load i8*, i8** %format.text50, align 8
  %"$format.size57" = load i32, i32* %format.size45, align 4
  %12 = insertvalue %String zeroinitializer, i8* %"$format.text56", 0
  %13 = insertvalue %String %12, i32 %"$format.size57", 1
  %14 = insertvalue %String %13, i32 %ADD55, 2
  call void @"io::out"(%String %14)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```
