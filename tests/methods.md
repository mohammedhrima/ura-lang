# methods

## index

- 001 — a method reads self
- 002 — a method mutates through self, and the caller sees it
- 003 — a method with a return value
- 004 — a method calling a sibling method declared below it
- 005 — two structs may declare the same method name
- 006 — a method on an array element
- 007 — a method on a nested struct field
- 008 — calling a method the struct does not have
- 009 — calling a method on a non-struct
- 010 — wrong argument count to a method
- 011 — a `pub fn` is static: no self, called with `::`
- 012 — calling a non-pub method with `::`
- 013 — `::` on a type that does not exist
- 014 — chaining a field access onto a call
- 015 — two structs declare the same `pub fn`, and statics compose

## 001 — a method reads self

```ura
// methods/001.ura - a method reads self

struct Room:
    name  chars
    floor int

    fn describe() void:
        output("room ", self.name, " on floor ", self.floor, "\n")

main():
    r Room
    r.name  = "hall"
    r.floor = 2
    r.describe()
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

struct Room
├─ name : chars
├─ floor : int
└─ fn Room.describe(self : STRUCT_CALL) : void
   └─ output : void
      ├─ chars "room "
      ├─ .name : chars
      │  └─ self : STRUCT_CALL
      ├─ chars " on floor "
      ├─ .floor : int
      │  └─ self : STRUCT_CALL
      └─ chars "\n"

fn main() : int
├─ r : STRUCT_CALL
├─ = : chars
│  ├─ .name : chars
│  │  └─ r : STRUCT_CALL
│  └─ chars "hall"
├─ = : int
│  ├─ .floor : int
│  │  └─ r : STRUCT_CALL
│  └─ int 2
└─ call describe : void
   └─ r : STRUCT_CALL
```

```out
room hall on floor 2
```

```err
```

```ll

%Room = type { i8*, i32 }

@str = private unnamed_addr constant [6 x i8] c"room \00", align 1
@str.1 = private unnamed_addr constant [11 x i8] c" on floor \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%s%s%s%d%s\00", align 1
@str.3 = private unnamed_addr constant [5 x i8] c"hall\00", align 1

define void @Room.describe(%Room* %0) {
entry:
  %self = alloca %Room*, align 8
  store %Room* %0, %Room** %self, align 8
  %ref = load %Room*, %Room** %self, align 8
  %name = getelementptr %Room, %Room* %ref, i32 0, i32 0
  %name1 = load i8*, i8** %name, align 8
  %ref2 = load %Room*, %Room** %self, align 8
  %floor = getelementptr %Room, %Room* %ref2, i32 0, i32 1
  %floor3 = load i32, i32* %floor, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i8* %name1, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.1, i32 0, i32 0), i32 %floor3, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %r = alloca %Room, align 8
  store %Room zeroinitializer, %Room* %r, align 8
  %name = getelementptr %Room, %Room* %r, i32 0, i32 0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.3, i32 0, i32 0), i8** %name, align 8
  %floor = getelementptr %Room, %Room* %r, i32 0, i32 1
  store i32 2, i32* %floor, align 4
  call void @Room.describe(%Room* %r)
  ret i32 0
}
```

## 002 — a method mutates through self, and the caller sees it

```ura
// methods/002.ura - a method mutates through self

struct Room:
    floor int

    fn climb(n int) void:
        self.floor = self.floor + n

main():
    r Room
    r.floor = 2
    r.climb(3)
    output("floor = ", r.floor, "\n")
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

struct Room
├─ floor : int
└─ fn Room.climb(self : STRUCT_CALL, n : int) : void
   └─ = : int
      ├─ .floor : int
      │  └─ self : STRUCT_CALL
      └─ + : int
         ├─ .floor : int
         │  └─ self : STRUCT_CALL
         └─ n : int

fn main() : int
├─ r : STRUCT_CALL
├─ = : int
│  ├─ .floor : int
│  │  └─ r : STRUCT_CALL
│  └─ int 2
├─ call climb : void
│  ├─ r : STRUCT_CALL
│  └─ int 3
└─ output : void
   ├─ chars "floor = "
   ├─ .floor : int
   │  └─ r : STRUCT_CALL
   └─ chars "\n"
```

```out
floor = 5
```

```err
```

```ll

%Room = type { i32 }

@str = private unnamed_addr constant [9 x i8] c"floor = \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define void @Room.climb(%Room* %0, i32 %1) {
entry:
  %self = alloca %Room*, align 8
  store %Room* %0, %Room** %self, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %ref = load %Room*, %Room** %self, align 8
  %floor = getelementptr %Room, %Room* %ref, i32 0, i32 0
  %ref1 = load %Room*, %Room** %self, align 8
  %floor2 = getelementptr %Room, %Room* %ref1, i32 0, i32 0
  %floor3 = load i32, i32* %floor2, align 4
  %n4 = load i32, i32* %n, align 4
  %add = add i32 %floor3, %n4
  store i32 %add, i32* %floor, align 4
  ret void
}

define i32 @main() {
entry:
  %r = alloca %Room, align 8
  store %Room zeroinitializer, %Room* %r, align 4
  %floor = getelementptr %Room, %Room* %r, i32 0, i32 0
  store i32 2, i32* %floor, align 4
  call void @Room.climb(%Room* %r, i32 3)
  %floor1 = getelementptr %Room, %Room* %r, i32 0, i32 0
  %floor2 = load i32, i32* %floor1, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %floor2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 003 — a method with a return value

```ura
// methods/003.ura - a method with a return value

struct Room:
    floor int

    fn depth() int:
        return self.floor * 10

main():
    r Room
    r.floor = 4
    output("depth = ", r.depth(), "\n")
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

struct Room
├─ floor : int
└─ fn Room.depth(self : STRUCT_CALL) : int
   └─ return
      └─ * : int
         ├─ .floor : int
         │  └─ self : STRUCT_CALL
         └─ int 10

fn main() : int
├─ r : STRUCT_CALL
├─ = : int
│  ├─ .floor : int
│  │  └─ r : STRUCT_CALL
│  └─ int 4
└─ output : void
   ├─ chars "depth = "
   ├─ call depth : int
   │  └─ r : STRUCT_CALL
   └─ chars "\n"
```

```out
depth = 40
```

```err
```

```ll

%Room = type { i32 }

@str = private unnamed_addr constant [9 x i8] c"depth = \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define i32 @Room.depth(%Room* %0) {
entry:
  %self = alloca %Room*, align 8
  store %Room* %0, %Room** %self, align 8
  %ref = load %Room*, %Room** %self, align 8
  %floor = getelementptr %Room, %Room* %ref, i32 0, i32 0
  %floor1 = load i32, i32* %floor, align 4
  %mul = mul i32 %floor1, 10
  ret i32 %mul
}

define i32 @main() {
entry:
  %r = alloca %Room, align 8
  store %Room zeroinitializer, %Room* %r, align 4
  %floor = getelementptr %Room, %Room* %r, i32 0, i32 0
  store i32 4, i32* %floor, align 4
  %call = call i32 @Room.depth(%Room* %r)
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %call, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 004 — a method calling a sibling method declared below it

```ura
// methods/004.ura - a method calling a sibling declared below it

struct Room:
    floor int

    fn show() void:
        output("floor ", self.twice(), "\n")

    fn twice() int:
        return self.floor * 2

main():
    r Room
    r.floor = 5
    r.show()
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

struct Room
├─ floor : int
├─ fn Room.show(self : STRUCT_CALL) : void
│  └─ output : void
│     ├─ chars "floor "
│     ├─ call twice : int
│     │  └─ self : STRUCT_CALL
│     └─ chars "\n"
└─ fn Room.twice(self : STRUCT_CALL) : int
   └─ return
      └─ * : int
         ├─ .floor : int
         │  └─ self : STRUCT_CALL
         └─ int 2

fn main() : int
├─ r : STRUCT_CALL
├─ = : int
│  ├─ .floor : int
│  │  └─ r : STRUCT_CALL
│  └─ int 5
└─ call show : void
   └─ r : STRUCT_CALL
```

```out
floor 10
```

```err
```

```ll

%Room = type { i32 }

@str = private unnamed_addr constant [7 x i8] c"floor \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define void @Room.show(%Room* %0) {
entry:
  %self = alloca %Room*, align 8
  store %Room* %0, %Room** %self, align 8
  %ref = load %Room*, %Room** %self, align 8
  %call = call i32 @Room.twice(%Room* %ref)
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %call, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret void
}

define i32 @Room.twice(%Room* %0) {
entry:
  %self = alloca %Room*, align 8
  store %Room* %0, %Room** %self, align 8
  %ref = load %Room*, %Room** %self, align 8
  %floor = getelementptr %Room, %Room* %ref, i32 0, i32 0
  %floor1 = load i32, i32* %floor, align 4
  %mul = mul i32 %floor1, 2
  ret i32 %mul
}

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %r = alloca %Room, align 8
  store %Room zeroinitializer, %Room* %r, align 4
  %floor = getelementptr %Room, %Room* %r, i32 0, i32 0
  store i32 5, i32* %floor, align 4
  call void @Room.show(%Room* %r)
  ret i32 0
}
```

## 005 — two structs may declare the same method name

```ura
// methods/005.ura - two structs may declare the same method name

struct Room:
    floor int

    fn describe() void:
        output("room on floor ", self.floor, "\n")

struct Tower:
    height int

    fn describe() void:
        output("tower of height ", self.height, "\n")

main():
    r Room
    r.floor = 1
    t Tower
    t.height = 9
    r.describe()
    t.describe()
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

struct Room
├─ floor : int
└─ fn Room.describe(self : STRUCT_CALL) : void
   └─ output : void
      ├─ chars "room on floor "
      ├─ .floor : int
      │  └─ self : STRUCT_CALL
      └─ chars "\n"

struct Tower
├─ height : int
└─ fn Tower.describe(self : STRUCT_CALL) : void
   └─ output : void
      ├─ chars "tower of height "
      ├─ .height : int
      │  └─ self : STRUCT_CALL
      └─ chars "\n"

fn main() : int
├─ r : STRUCT_CALL
├─ = : int
│  ├─ .floor : int
│  │  └─ r : STRUCT_CALL
│  └─ int 1
├─ t : STRUCT_CALL
├─ = : int
│  ├─ .height : int
│  │  └─ t : STRUCT_CALL
│  └─ int 9
├─ call describe : void
│  └─ r : STRUCT_CALL
└─ call describe : void
   └─ t : STRUCT_CALL
```

```out
room on floor 1
tower of height 9
```

```err
```

```ll

%Room = type { i32 }
%Tower = type { i32 }

@str = private unnamed_addr constant [15 x i8] c"room on floor \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.2 = private unnamed_addr constant [17 x i8] c"tower of height \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define void @Room.describe(%Room* %0) {
entry:
  %self = alloca %Room*, align 8
  store %Room* %0, %Room** %self, align 8
  %ref = load %Room*, %Room** %self, align 8
  %floor = getelementptr %Room, %Room* %ref, i32 0, i32 0
  %floor1 = load i32, i32* %floor, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str, i32 0, i32 0), i32 %floor1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)

define void @Tower.describe(%Tower* %0) {
entry:
  %self = alloca %Tower*, align 8
  store %Tower* %0, %Tower** %self, align 8
  %ref = load %Tower*, %Tower** %self, align 8
  %height = getelementptr %Tower, %Tower* %ref, i32 0, i32 0
  %height1 = load i32, i32* %height, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.4, i32 0, i32 0), i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.2, i32 0, i32 0), i32 %height1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  ret void
}

define i32 @main() {
entry:
  %r = alloca %Room, align 8
  store %Room zeroinitializer, %Room* %r, align 4
  %floor = getelementptr %Room, %Room* %r, i32 0, i32 0
  store i32 1, i32* %floor, align 4
  %t = alloca %Tower, align 8
  store %Tower zeroinitializer, %Tower* %t, align 4
  %height = getelementptr %Tower, %Tower* %t, i32 0, i32 0
  store i32 9, i32* %height, align 4
  call void @Room.describe(%Room* %r)
  call void @Tower.describe(%Tower* %t)
  ret i32 0
}
```

## 006 — a method on an array element

```ura
// methods/006.ura - a method on an array element

struct Room:
    floor int

    fn bump() void:
        self.floor = self.floor + 1

main():
    rooms Room[] = Room[2]
    rooms[0].floor = 7
    rooms[0].bump()
    rooms[1].bump()
    output(rooms[0].floor, " ", rooms[1].floor, "\n")
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

struct Room
├─ floor : int
└─ fn Room.bump(self : STRUCT_CALL) : void
   └─ = : int
      ├─ .floor : int
      │  └─ self : STRUCT_CALL
      └─ + : int
         ├─ .floor : int
         │  └─ self : STRUCT_CALL
         └─ int 1

fn main() : int
├─ = : array
│  ├─ rooms : STRUCT_CALL[]
│  └─ array : STRUCT_CALL[]
│     └─ int 2
├─ = : int
│  ├─ .floor : int
│  │  └─ index : STRUCT_CALL
│  │     ├─ rooms : STRUCT_CALL[]
│  │     └─ int 0
│  └─ int 7
├─ call bump : void
│  └─ index : STRUCT_CALL
│     ├─ rooms : STRUCT_CALL[]
│     └─ int 0
├─ call bump : void
│  └─ index : STRUCT_CALL
│     ├─ rooms : STRUCT_CALL[]
│     └─ int 1
└─ output : void
   ├─ .floor : int
   │  └─ index : STRUCT_CALL
   │     ├─ rooms : STRUCT_CALL[]
   │     └─ int 0
   ├─ chars " "
   ├─ .floor : int
   │  └─ index : STRUCT_CALL
   │     ├─ rooms : STRUCT_CALL[]
   │     └─ int 1
   └─ chars "\n"
```

```out
8 1
```

```err
```

```ll

%Room = type { i32 }

@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [9 x i8] c"%d%s%d%s\00", align 1

define void @Room.bump(%Room* %0) {
entry:
  %self = alloca %Room*, align 8
  store %Room* %0, %Room** %self, align 8
  %ref = load %Room*, %Room** %self, align 8
  %floor = getelementptr %Room, %Room* %ref, i32 0, i32 0
  %ref1 = load %Room*, %Room** %self, align 8
  %floor2 = getelementptr %Room, %Room* %ref1, i32 0, i32 0
  %floor3 = load i32, i32* %floor2, align 4
  %add = add i32 %floor3, 1
  store i32 %add, i32* %floor, align 4
  ret void
}

define i32 @main() {
entry:
  %rooms = alloca { %Room*, i64 }, align 8
  %arr = alloca %Room, i64 2, align 8
  %0 = bitcast %Room* %arr to i8*
  call void @llvm.memset.p0i8.i64(i8* %0, i8 0, i64 8, i1 false)
  %arr.ptr = insertvalue { %Room*, i64 } undef, %Room* %arr, 0
  %arr.len = insertvalue { %Room*, i64 } %arr.ptr, i64 2, 1
  store { %Room*, i64 } %arr.len, { %Room*, i64 }* %rooms, align 8
  %rooms1 = load { %Room*, i64 }, { %Room*, i64 }* %rooms, align 8
  %arr.data = extractvalue { %Room*, i64 } %rooms1, 0
  %arr.at = getelementptr %Room, %Room* %arr.data, i32 0
  %floor = getelementptr %Room, %Room* %arr.at, i32 0, i32 0
  store i32 7, i32* %floor, align 4
  %rooms2 = load { %Room*, i64 }, { %Room*, i64 }* %rooms, align 8
  %arr.data3 = extractvalue { %Room*, i64 } %rooms2, 0
  %arr.at4 = getelementptr %Room, %Room* %arr.data3, i32 0
  call void @Room.bump(%Room* %arr.at4)
  %rooms5 = load { %Room*, i64 }, { %Room*, i64 }* %rooms, align 8
  %arr.data6 = extractvalue { %Room*, i64 } %rooms5, 0
  %arr.at7 = getelementptr %Room, %Room* %arr.data6, i32 1
  call void @Room.bump(%Room* %arr.at7)
  %rooms8 = load { %Room*, i64 }, { %Room*, i64 }* %rooms, align 8
  %arr.data9 = extractvalue { %Room*, i64 } %rooms8, 0
  %arr.at10 = getelementptr %Room, %Room* %arr.data9, i32 0
  %floor11 = getelementptr %Room, %Room* %arr.at10, i32 0, i32 0
  %floor12 = load i32, i32* %floor11, align 4
  %rooms13 = load { %Room*, i64 }, { %Room*, i64 }* %rooms, align 8
  %arr.data14 = extractvalue { %Room*, i64 } %rooms13, 0
  %arr.at15 = getelementptr %Room, %Room* %arr.data14, i32 1
  %floor16 = getelementptr %Room, %Room* %arr.at15, i32 0, i32 0
  %floor17 = load i32, i32* %floor16, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt, i32 0, i32 0), i32 %floor12, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %floor17, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

; Function Attrs: argmemonly nofree nounwind willreturn writeonly
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #0

declare i32 @printf(i8*, ...)

attributes #0 = { argmemonly nofree nounwind willreturn writeonly }
```

## 007 — a method on a nested struct field

```ura
// methods/007.ura - a method on a nested struct field

struct Room:
    floor int

    fn depth() int:
        return self.floor

struct Dungeon:
    entry Room

main():
    d Dungeon
    d.entry.floor = 3
    output("entry depth ", d.entry.depth(), "\n")
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

struct Room
├─ floor : int
└─ fn Room.depth(self : STRUCT_CALL) : int
   └─ return
      └─ .floor : int
         └─ self : STRUCT_CALL

struct Dungeon
└─ entry : STRUCT_CALL

fn main() : int
├─ d : STRUCT_CALL
├─ = : int
│  ├─ .floor : int
│  │  └─ .entry : STRUCT_CALL
│  │     └─ d : STRUCT_CALL
│  └─ int 3
└─ output : void
   ├─ chars "entry depth "
   ├─ call depth : int
   │  └─ .entry : STRUCT_CALL
   │     └─ d : STRUCT_CALL
   └─ chars "\n"
```

```out
entry depth 3
```

```err
```

```ll

%Room = type { i32 }
%Dungeon = type { %Room }

@str = private unnamed_addr constant [13 x i8] c"entry depth \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define i32 @Room.depth(%Room* %0) {
entry:
  %self = alloca %Room*, align 8
  store %Room* %0, %Room** %self, align 8
  %ref = load %Room*, %Room** %self, align 8
  %floor = getelementptr %Room, %Room* %ref, i32 0, i32 0
  %floor1 = load i32, i32* %floor, align 4
  ret i32 %floor1
}

define i32 @main() {
entry:
  %d = alloca %Dungeon, align 8
  store %Dungeon zeroinitializer, %Dungeon* %d, align 4
  %entry1 = getelementptr %Dungeon, %Dungeon* %d, i32 0, i32 0
  %floor = getelementptr %Room, %Room* %entry1, i32 0, i32 0
  store i32 3, i32* %floor, align 4
  %entry2 = getelementptr %Dungeon, %Dungeon* %d, i32 0, i32 0
  %call = call i32 @Room.depth(%Room* %entry2)
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str, i32 0, i32 0), i32 %call, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 008 — calling a method the struct does not have

```ura
// methods/008.ura - calling a method the struct does not have

struct Room:
    floor int

main():
    r Room
    r.nope()
```

```tree
```

```out
```

```err
error: Struct Room has no method 'nope'; check the spelling or declare it in the struct
  008.ura:8:6
  |
8 |     r.nope()
  |      ^
```

```ll
```

## 009 — calling a method on a non-struct

```ura
// methods/009.ura - calling a method on a non-struct

main():
    x int = 3
    x.nope()
```

```tree
```

```out
```

```err
error: Cannot call '.nope()' on int; only a struct has methods
  009.ura:5:6
  |
5 |     x.nope()
  |      ^
```

```ll
```

## 010 — wrong argument count to a method

```ura
// methods/010.ura - wrong argument count to a method

struct Room:
    floor int

    fn climb(n int) void:
        self.floor = self.floor + n

main():
    r Room
    r.climb()
```

```tree
```

```out
```

```err
error: Wrong number of arguments to 'climb'
   010.ura:11:6
   |
11 |     r.climb()
   |      ^
```

```ll
```

## 011 — a `pub fn` is static: no self, called with `::`

```ura
// methods/011.ura - a pub fn is static, called with ::

struct Room:
    floor int

    pub fn make(f int) Room:
        r Room
        r.floor = f
        return r

main():
    r Room = Room::make(7)
    output("floor = ", r.floor, "\n")
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

struct Room
├─ floor : int
└─ fn Room.make(f : int) : STRUCT_CALL
   ├─ r : STRUCT_CALL
   ├─ = : int
   │  ├─ .floor : int
   │  │  └─ r : STRUCT_CALL
   │  └─ f : int
   └─ return
      └─ r : STRUCT_CALL

fn main() : int
├─ = : STRUCT_CALL
│  ├─ r : STRUCT_CALL
│  └─ call make : STRUCT_CALL
│     └─ int 7
└─ output : void
   ├─ chars "floor = "
   ├─ .floor : int
   │  └─ r : STRUCT_CALL
   └─ chars "\n"
```

```out
floor = 7
```

```err
```

```ll

%Room = type { i32 }

@str = private unnamed_addr constant [9 x i8] c"floor = \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define %Room @Room.make(i32 %0) {
entry:
  %f = alloca i32, align 4
  store i32 %0, i32* %f, align 4
  %r = alloca %Room, align 8
  store %Room zeroinitializer, %Room* %r, align 4
  %floor = getelementptr %Room, %Room* %r, i32 0, i32 0
  %f1 = load i32, i32* %f, align 4
  store i32 %f1, i32* %floor, align 4
  %r2 = load %Room, %Room* %r, align 4
  ret %Room %r2
}

define i32 @main() {
entry:
  %r = alloca %Room, align 8
  %call = call %Room @Room.make(i32 7)
  store %Room %call, %Room* %r, align 4
  %floor = getelementptr %Room, %Room* %r, i32 0, i32 0
  %floor1 = load i32, i32* %floor, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %floor1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 012 — calling a non-pub method with `::`

```ura
// methods/012.ura - calling a non-pub method with ::

struct Room:
    floor int

    fn depth() int:
        return self.floor

main():
    output(Room::depth(), "\n")
```

```tree
```

```out
```

```err
error: 'Room::depth' needs a receiver; call it on a value, or declare it 'pub fn' to make it static
   012.ura:10:12
   |
10 |     output(Room::depth(), "\n")
   |            ^^^^
```

```ll
```

## 013 — `::` on a type that does not exist

```ura
// methods/013.ura - :: on a type that does not exist

main():
    output(Nope::make(), "\n")
```

```tree
```

```out
```

```err
error: Unknown type 'Nope'
  013.ura:4:12
  |
4 |     output(Nope::make(), "\n")
  |            ^^^^
```

```ll
```

## 014 — chaining a field access onto a call

```ura
// methods/014.ura - chaining a field access onto a call

struct Room:
    floor int

    pub fn create(f int) Room:
        r Room
        r.floor = f
        return r

fn make(f int) Room:
    r Room
    r.floor = f
    return r

main():
    output("static ", Room::create(7).floor, "\n")
    output("plain  ", make(3).floor, "\n")
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

struct Room
├─ floor : int
└─ fn Room.create(f : int) : STRUCT_CALL
   ├─ r : STRUCT_CALL
   ├─ = : int
   │  ├─ .floor : int
   │  │  └─ r : STRUCT_CALL
   │  └─ f : int
   └─ return
      └─ r : STRUCT_CALL

fn make(f : int) : STRUCT_CALL
├─ r : STRUCT_CALL
├─ = : int
│  ├─ .floor : int
│  │  └─ r : STRUCT_CALL
│  └─ f : int
└─ return
   └─ r : STRUCT_CALL

fn main() : int
├─ output : void
│  ├─ chars "static "
│  ├─ .floor : int
│  │  └─ call create : STRUCT_CALL
│  │     └─ int 7
│  └─ chars "\n"
└─ output : void
   ├─ chars "plain  "
   ├─ .floor : int
   │  └─ call make : STRUCT_CALL
   │     └─ int 3
   └─ chars "\n"
```

```out
static 7
plain  3
```

```err
```

```ll

%Room = type { i32 }

@str = private unnamed_addr constant [8 x i8] c"static \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.2 = private unnamed_addr constant [8 x i8] c"plain  \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define %Room @Room.create(i32 %0) {
entry:
  %f = alloca i32, align 4
  store i32 %0, i32* %f, align 4
  %r = alloca %Room, align 8
  store %Room zeroinitializer, %Room* %r, align 4
  %floor = getelementptr %Room, %Room* %r, i32 0, i32 0
  %f1 = load i32, i32* %f, align 4
  store i32 %f1, i32* %floor, align 4
  %r2 = load %Room, %Room* %r, align 4
  ret %Room %r2
}

define %Room @make(i32 %0) {
entry:
  %f = alloca i32, align 4
  store i32 %0, i32* %f, align 4
  %r = alloca %Room, align 8
  store %Room zeroinitializer, %Room* %r, align 4
  %floor = getelementptr %Room, %Room* %r, i32 0, i32 0
  %f1 = load i32, i32* %f, align 4
  store i32 %f1, i32* %floor, align 4
  %r2 = load %Room, %Room* %r, align 4
  ret %Room %r2
}

define i32 @main() {
entry:
  %call = call %Room @Room.create(i32 7)
  %out.tmp = alloca %Room, align 8
  store %Room %call, %Room* %out.tmp, align 4
  %floor = getelementptr %Room, %Room* %out.tmp, i32 0, i32 0
  %floor1 = load i32, i32* %floor, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str, i32 0, i32 0), i32 %floor1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %call2 = call %Room @make(i32 3)
  %out.tmp3 = alloca %Room, align 8
  store %Room %call2, %Room* %out.tmp3, align 4
  %floor4 = getelementptr %Room, %Room* %out.tmp3, i32 0, i32 0
  %floor5 = load i32, i32* %floor4, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.4, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.2, i32 0, i32 0), i32 %floor5, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 015 — two structs declare the same `pub fn`, and statics compose

```ura
// methods/015.ura - two structs with the same pub fn name

struct Room:
    floor int

    pub fn create(f int) Room:
        r Room
        r.floor = f
        return r

    pub fn zero() Room:
        return Room::create(0)

    pub fn level(f int) int:
        return f * 10

    fn depth() int:
        return self.floor

struct Tower:
    height int

    pub fn create(h int) Tower:
        t Tower
        t.height = h
        return t

main():
    a Room  = Room::create(3)
    b Tower = Tower::create(9)
    output("room ", a.floor, " tower ", b.height, "\n")
    output("zero ", Room::zero().floor, "\n")
    output("level ", Room::level(4), "\n")
    output("method ", Room::create(5).depth(), "\n")
    n int = Room::create(2).depth() + Tower::create(1).height
    output("mixed ", n, "\n")
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

struct Room
├─ floor : int
├─ fn Room.create(f : int) : STRUCT_CALL
│  ├─ r : STRUCT_CALL
│  ├─ = : int
│  │  ├─ .floor : int
│  │  │  └─ r : STRUCT_CALL
│  │  └─ f : int
│  └─ return
│     └─ r : STRUCT_CALL
├─ fn Room.zero() : STRUCT_CALL
│  └─ return
│     └─ call create : STRUCT_CALL
│        └─ int 0
├─ fn Room.level(f : int) : int
│  └─ return
│     └─ * : int
│        ├─ f : int
│        └─ int 10
└─ fn Room.depth(self : STRUCT_CALL) : int
   └─ return
      └─ .floor : int
         └─ self : STRUCT_CALL

struct Tower
├─ height : int
└─ fn Tower.create(h : int) : STRUCT_CALL
   ├─ t : STRUCT_CALL
   ├─ = : int
   │  ├─ .height : int
   │  │  └─ t : STRUCT_CALL
   │  └─ h : int
   └─ return
      └─ t : STRUCT_CALL

fn main() : int
├─ = : STRUCT_CALL
│  ├─ a : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     └─ int 3
├─ = : STRUCT_CALL
│  ├─ b : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     └─ int 9
├─ output : void
│  ├─ chars "room "
│  ├─ .floor : int
│  │  └─ a : STRUCT_CALL
│  ├─ chars " tower "
│  ├─ .height : int
│  │  └─ b : STRUCT_CALL
│  └─ chars "\n"
├─ output : void
│  ├─ chars "zero "
│  ├─ .floor : int
│  │  └─ call zero : STRUCT_CALL
│  └─ chars "\n"
├─ output : void
│  ├─ chars "level "
│  ├─ call level : int
│  │  └─ int 4
│  └─ chars "\n"
├─ output : void
│  ├─ chars "method "
│  ├─ call depth : int
│  │  └─ call create : STRUCT_CALL
│  │     └─ int 5
│  └─ chars "\n"
├─ = : int
│  ├─ n : int
│  └─ + : int
│     ├─ call depth : int
│     │  └─ call create : STRUCT_CALL
│     │     └─ int 2
│     └─ .height : int
│        └─ call create : STRUCT_CALL
│           └─ int 1
└─ output : void
   ├─ chars "mixed "
   ├─ n : int
   └─ chars "\n"
```

```out
room 3 tower 9
zero 0
level 40
method 5
mixed 3
```

```err
```

```ll

%Room = type { i32 }
%Tower = type { i32 }

@str = private unnamed_addr constant [6 x i8] c"room \00", align 1
@str.1 = private unnamed_addr constant [8 x i8] c" tower \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%s%d%s%d%s\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"zero \00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.5 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.6 = private unnamed_addr constant [7 x i8] c"level \00", align 1
@str.7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.8 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.9 = private unnamed_addr constant [8 x i8] c"method \00", align 1
@str.10 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.11 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.12 = private unnamed_addr constant [7 x i8] c"mixed \00", align 1
@str.13 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.14 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define %Room @Room.create(i32 %0) {
entry:
  %f = alloca i32, align 4
  store i32 %0, i32* %f, align 4
  %r = alloca %Room, align 8
  store %Room zeroinitializer, %Room* %r, align 4
  %floor = getelementptr %Room, %Room* %r, i32 0, i32 0
  %f1 = load i32, i32* %f, align 4
  store i32 %f1, i32* %floor, align 4
  %r2 = load %Room, %Room* %r, align 4
  ret %Room %r2
}

define %Room @Room.zero() {
entry:
  %call = call %Room @Room.create(i32 0)
  ret %Room %call
}

define i32 @Room.level(i32 %0) {
entry:
  %f = alloca i32, align 4
  store i32 %0, i32* %f, align 4
  %f1 = load i32, i32* %f, align 4
  %mul = mul i32 %f1, 10
  ret i32 %mul
}

define i32 @Room.depth(%Room* %0) {
entry:
  %self = alloca %Room*, align 8
  store %Room* %0, %Room** %self, align 8
  %ref = load %Room*, %Room** %self, align 8
  %floor = getelementptr %Room, %Room* %ref, i32 0, i32 0
  %floor1 = load i32, i32* %floor, align 4
  ret i32 %floor1
}

define %Tower @Tower.create(i32 %0) {
entry:
  %h = alloca i32, align 4
  store i32 %0, i32* %h, align 4
  %t = alloca %Tower, align 8
  store %Tower zeroinitializer, %Tower* %t, align 4
  %height = getelementptr %Tower, %Tower* %t, i32 0, i32 0
  %h1 = load i32, i32* %h, align 4
  store i32 %h1, i32* %height, align 4
  %t2 = load %Tower, %Tower* %t, align 4
  ret %Tower %t2
}

define i32 @main() {
entry:
  %a = alloca %Room, align 8
  %call = call %Room @Room.create(i32 3)
  store %Room %call, %Room* %a, align 4
  %b = alloca %Tower, align 8
  %call1 = call %Tower @Tower.create(i32 9)
  store %Tower %call1, %Tower* %b, align 4
  %floor = getelementptr %Room, %Room* %a, i32 0, i32 0
  %floor2 = load i32, i32* %floor, align 4
  %height = getelementptr %Tower, %Tower* %b, i32 0, i32 0
  %height3 = load i32, i32* %height, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %floor2, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.1, i32 0, i32 0), i32 %height3, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %call4 = call %Room @Room.zero()
  %out.tmp = alloca %Room, align 8
  store %Room %call4, %Room* %out.tmp, align 4
  %floor5 = getelementptr %Room, %Room* %out.tmp, i32 0, i32 0
  %floor6 = load i32, i32* %floor5, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.5, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i32 %floor6, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  %call7 = call i32 @Room.level(i32 4)
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.8, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.6, i32 0, i32 0), i32 %call7, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  %call8 = call %Room @Room.create(i32 5)
  %out.tmp9 = alloca %Room, align 8
  store %Room %call8, %Room* %out.tmp9, align 4
  %call10 = call i32 @Room.depth(%Room* %out.tmp9)
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.11, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.9, i32 0, i32 0), i32 %call10, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.10, i32 0, i32 0))
  %n = alloca i32, align 4
  %call11 = call %Room @Room.create(i32 2)
  %out.tmp12 = alloca %Room, align 8
  store %Room %call11, %Room* %out.tmp12, align 4
  %call13 = call i32 @Room.depth(%Room* %out.tmp12)
  %call14 = call %Tower @Tower.create(i32 1)
  %out.tmp15 = alloca %Tower, align 8
  store %Tower %call14, %Tower* %out.tmp15, align 4
  %height16 = getelementptr %Tower, %Tower* %out.tmp15, i32 0, i32 0
  %height17 = load i32, i32* %height16, align 4
  %add = add i32 %call13, %height17
  store i32 %add, i32* %n, align 4
  %n18 = load i32, i32* %n, align 4
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.14, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.12, i32 0, i32 0), i32 %n18, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.13, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```
