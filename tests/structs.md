# structs

## index

- 001 — declare a struct and a local: named type, alloca, zero-init
- 002 — a field whose type is another struct
- 003 — forward reference: a field type declared later in the file
- 004 — a struct declared inside a function body
- 005 — a struct declared inside an if block
- 006 — a struct declared inside a loop body
- 007 — an array-of-struct field is a fat pointer to the named type
- 008 — a struct nested inside another struct's body
- 009 — self reference through an array stays finite
- 010 — an unused struct emits no type: LLVM drops unreferenced named types
- 011 — the same struct name in three nested scopes: innermost wins
- 012 — read and write a field
- 013 — nested field access through a struct field
- 014 — a stack array of structs: Room[N]
- 015 — a heap array of structs: new Room[N], then clean
- 016 — a 2D array of structs: Room[N][M]
- 017 — a 2D heap array of structs frees every inner slice
- 018 — '.len' on arrays of structs, including 2D
- 019 — 'stack' and 'heap' are ordinary identifiers
- 020 — a 'ref?' field may refer to its own struct
- 021 — bind, read and write through a 'ref?' field
- 022 — struct assignment copies by value

## 001 — declare a struct and a local: named type, alloca, zero-init

```ura
// structs/001.ura - declare a struct and a local: named type, alloca, zero-init

struct Player:
    name chars
    hp   int
    mp   int

main():
    p Player
    output("ok\n")
```

```tree
struct Player
├─ name : chars
├─ hp : int
└─ mp : int

fn main() : int
├─ p : STRUCT_CALL
└─ output : void
   └─ chars "ok\n"
```

```out
ok
```

```err
```

```ll

%Player = type { i8*, i32, i32 }

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i32 @main() {
entry:
  %p = alloca %Player, align 8
  store %Player zeroinitializer, %Player* %p, align 8
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 002 — a field whose type is another struct

```ura
// structs/002.ura - a field whose type is another struct

struct Room:
    name  chars
    floor int

struct Dungeon:
    name  chars
    entry Room

main():
    d Dungeon
    output("ok\n")
```

```tree
struct Room
├─ name : chars
└─ floor : int

struct Dungeon
├─ name : chars
└─ entry : STRUCT_CALL

fn main() : int
├─ d : STRUCT_CALL
└─ output : void
   └─ chars "ok\n"
```

```out
ok
```

```err
```

```ll

%Dungeon = type { i8*, %Room }
%Room = type { i8*, i32 }

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i32 @main() {
entry:
  %d = alloca %Dungeon, align 8
  store %Dungeon zeroinitializer, %Dungeon* %d, align 8
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 003 — forward reference: a field type declared later in the file

```ura
// structs/003.ura - forward reference: a field type declared later in the file

struct Dungeon:
    name  chars
    entry Room

struct Room:
    name  chars
    floor int

main():
    d Dungeon
    output("ok\n")
```

```tree
struct Dungeon
├─ name : chars
└─ entry : STRUCT_CALL

struct Room
├─ name : chars
└─ floor : int

fn main() : int
├─ d : STRUCT_CALL
└─ output : void
   └─ chars "ok\n"
```

```out
ok
```

```err
```

```ll

%Dungeon = type { i8*, %Room }
%Room = type { i8*, i32 }

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i32 @main() {
entry:
  %d = alloca %Dungeon, align 8
  store %Dungeon zeroinitializer, %Dungeon* %d, align 8
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 004 — a struct declared inside a function body

```ura
// structs/004.ura - a struct declared inside a function body

main():
    struct Local:
        x int
        y int

    l Local
    output("ok\n")
```

```tree
fn main() : int
├─ struct Local
│  ├─ x : int
│  └─ y : int
├─ l : STRUCT_CALL
└─ output : void
   └─ chars "ok\n"
```

```out
ok
```

```err
```

```ll

%Local = type { i32, i32 }

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i32 @main() {
entry:
  %l = alloca %Local, align 8
  store %Local zeroinitializer, %Local* %l, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 005 — a struct declared inside an if block

```ura
// structs/005.ura - a struct declared inside an if block

main():
    n int = 1
    if n == 1:
        struct Inner:
            x int

        i Inner
        output("ok\n")
```

```tree
fn main() : int
├─ = : int
│  ├─ n : int
│  └─ int 1
└─ if
   ├─ condition == : bool
   │  ├─ n : int
   │  └─ int 1
   ├─ struct Inner
   │  └─ x : int
   ├─ i : STRUCT_CALL
   └─ output : void
      └─ chars "ok\n"
```

```out
ok
```

```err
```

```ll

%Inner = type { i32 }

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i32 @main() {
entry:
  %n = alloca i32, align 4
  store i32 1, i32* %n, align 4
  %n1 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n1, 1
  br i1 %eq, label %then, label %endif

endif:                                            ; preds = %then, %entry
  ret i32 0

then:                                             ; preds = %entry
  %i = alloca %Inner, align 8
  store %Inner zeroinitializer, %Inner* %i, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  br label %endif
}

declare i32 @printf(i8*, ...)
```

## 006 — a struct declared inside a loop body

```ura
// structs/006.ura - a struct declared inside a loop body

main():
    for i in 0..2:
        struct Tick:
            n int

        t Tick
    output("ok\n")
```

```tree
fn main() : int
├─ for
│  ├─ i : int
│  ├─ range : int
│  │  ├─ int 0
│  │  └─ int 2
│  ├─ struct Tick
│  │  └─ n : int
│  └─ t : STRUCT_CALL
└─ output : void
   └─ chars "ok\n"
```

```out
ok
```

```err
```

```ll

%Tick = type { i32 }

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i32 @main() {
entry:
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i1 = load i32, i32* %i, align 4
  %more = icmp ne i32 %i1, 2
  br i1 %more, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %t = alloca %Tick, align 8
  store %Tick zeroinitializer, %Tick* %t, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %i2 = load i32, i32* %i, align 4
  %next = add i32 %i2, 1
  store i32 %next, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 007 — an array-of-struct field is a fat pointer to the named type

```ura
// structs/007.ura - an array-of-struct field is a fat pointer to the named type

struct Room:
    floor int

struct Tree:
    label chars
    kids  Room[]

main():
    t Tree
    output("ok\n")
```

```tree
struct Room
└─ floor : int

struct Tree
├─ label : chars
└─ kids : STRUCT_CALL[]

fn main() : int
├─ t : STRUCT_CALL
└─ output : void
   └─ chars "ok\n"
```

```out
ok
```

```err
```

```ll

%Tree = type { i8*, { %Room*, i64 } }
%Room = type { i32 }

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i32 @main() {
entry:
  %t = alloca %Tree, align 8
  store %Tree zeroinitializer, %Tree* %t, align 8
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 008 — a struct nested inside another struct's body

```ura
// structs/008.ura - a struct nested inside another struct's body

struct Outer:
    name chars
    struct Config:
        debug bool

    hp int

main():
    o Outer
    output("ok\n")
```

```tree
struct Outer
├─ name : chars
├─ struct Config
│  └─ debug : bool
└─ hp : int

fn main() : int
├─ o : STRUCT_CALL
└─ output : void
   └─ chars "ok\n"
```

```out
ok
```

```err
```

```ll

%Outer = type { i8*, i32 }

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i32 @main() {
entry:
  %o = alloca %Outer, align 8
  store %Outer zeroinitializer, %Outer* %o, align 8
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 009 — self reference through an array stays finite

```ura
// structs/009.ura - self reference through an array stays finite

struct Tree:
    label chars
    kids  Tree[]

main():
    t Tree
    output("ok\n")
```

```tree
struct Tree
├─ label : chars
└─ kids : STRUCT_CALL[]

fn main() : int
├─ t : STRUCT_CALL
└─ output : void
   └─ chars "ok\n"
```

```out
ok
```

```err
```

```ll

%Tree = type { i8*, { %Tree*, i64 } }

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i32 @main() {
entry:
  %t = alloca %Tree, align 8
  store %Tree zeroinitializer, %Tree* %t, align 8
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 010 — an unused struct emits no type: LLVM drops unreferenced named types

```ura
// structs/010.ura - an unused struct emits no type

struct Unused:
    x int
    y int

main():
    output("ok\n")
```

```tree
struct Unused
├─ x : int
└─ y : int

fn main() : int
└─ output : void
   └─ chars "ok\n"
```

```out
ok
```

```err
```

```ll

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i32 @main() {
entry:
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 011 — the same struct name in three nested scopes: innermost wins

```ura
// structs/011.ura - the same struct name in three nested scopes

struct Local:
    a char
    b char

main():
    l0 Local

    struct Local:
        x int
        y int

    n int = 1
    if n == 1:
        struct Local:
            z chars

        l3 Local

    l Local
    output("ok\n")
```

```tree
struct Local
├─ a : char
└─ b : char

fn main() : int
├─ l0 : STRUCT_CALL
├─ struct Local
│  ├─ x : int
│  └─ y : int
├─ = : int
│  ├─ n : int
│  └─ int 1
├─ if
│  ├─ condition == : bool
│  │  ├─ n : int
│  │  └─ int 1
│  ├─ struct Local
│  │  └─ z : chars
│  └─ l3 : STRUCT_CALL
├─ l : STRUCT_CALL
└─ output : void
   └─ chars "ok\n"
```

```out
ok
```

```err
```

```ll

%Local.0 = type { i32, i32 }
%Local.1 = type { i8* }

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i32 @main() {
entry:
  %l0 = alloca %Local.0, align 8
  store %Local.0 zeroinitializer, %Local.0* %l0, align 4
  %n = alloca i32, align 4
  store i32 1, i32* %n, align 4
  %n1 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n1, 1
  br i1 %eq, label %then, label %endif

endif:                                            ; preds = %then, %entry
  %l = alloca %Local.0, align 8
  store %Local.0 zeroinitializer, %Local.0* %l, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret i32 0

then:                                             ; preds = %entry
  %l3 = alloca %Local.1, align 8
  store %Local.1 zeroinitializer, %Local.1* %l3, align 8
  br label %endif
}

declare i32 @printf(i8*, ...)
```

## 012 — read and write a field

```ura
// structs/012.ura - read and write a field

struct Room:
    name  chars
    floor int

main():
    p Room
    p.name  = "armory"
    p.floor = 3
    output(p.name, " on floor ", p.floor, "\n")
    p.floor = p.floor + 10
    output("floor now ", p.floor, "\n")
```

```tree
struct Room
├─ name : chars
└─ floor : int

fn main() : int
├─ p : STRUCT_CALL
├─ = : chars
│  ├─ .name : chars
│  │  └─ p : STRUCT_CALL
│  └─ chars "armory"
├─ = : int
│  ├─ .floor : int
│  │  └─ p : STRUCT_CALL
│  └─ int 3
├─ output : void
│  ├─ .name : chars
│  │  └─ p : STRUCT_CALL
│  ├─ chars " on floor "
│  ├─ .floor : int
│  │  └─ p : STRUCT_CALL
│  └─ chars "\n"
├─ = : int
│  ├─ .floor : int
│  │  └─ p : STRUCT_CALL
│  └─ + : int
│     ├─ .floor : int
│     │  └─ p : STRUCT_CALL
│     └─ int 10
└─ output : void
   ├─ chars "floor now "
   ├─ .floor : int
   │  └─ p : STRUCT_CALL
   └─ chars "\n"
```

```out
armory on floor 3
floor now 13
```

```err
```

```ll

%Room = type { i8*, i32 }

@str = private unnamed_addr constant [7 x i8] c"armory\00", align 1
@str.1 = private unnamed_addr constant [11 x i8] c" on floor \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [9 x i8] c"%s%s%d%s\00", align 1
@str.3 = private unnamed_addr constant [11 x i8] c"floor now \00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.5 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define i32 @main() {
entry:
  %p = alloca %Room, align 8
  store %Room zeroinitializer, %Room* %p, align 8
  %name = getelementptr %Room, %Room* %p, i32 0, i32 0
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i8** %name, align 8
  %floor = getelementptr %Room, %Room* %p, i32 0, i32 1
  store i32 3, i32* %floor, align 4
  %name1 = getelementptr %Room, %Room* %p, i32 0, i32 0
  %name2 = load i8*, i8** %name1, align 8
  %floor3 = getelementptr %Room, %Room* %p, i32 0, i32 1
  %floor4 = load i32, i32* %floor3, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt, i32 0, i32 0), i8* %name2, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.1, i32 0, i32 0), i32 %floor4, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %floor5 = getelementptr %Room, %Room* %p, i32 0, i32 1
  %floor6 = getelementptr %Room, %Room* %p, i32 0, i32 1
  %floor7 = load i32, i32* %floor6, align 4
  %add = add i32 %floor7, 10
  store i32 %add, i32* %floor5, align 4
  %floor8 = getelementptr %Room, %Room* %p, i32 0, i32 1
  %floor9 = load i32, i32* %floor8, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.5, i32 0, i32 0), i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.3, i32 0, i32 0), i32 %floor9, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 013 — nested field access through a struct field

```ura
// structs/013.ura - nested field access through a struct field

struct Room:
    name  chars
    floor int

struct Dungeon:
    name  chars
    entry Room

main():
    d Dungeon
    d.name = "keep"
    d.entry.name  = "gate"
    d.entry.floor = 1
    output(d.name, " entry ", d.entry.name, " floor ", d.entry.floor, "\n")
```

```tree
struct Room
├─ name : chars
└─ floor : int

struct Dungeon
├─ name : chars
└─ entry : STRUCT_CALL

fn main() : int
├─ d : STRUCT_CALL
├─ = : chars
│  ├─ .name : chars
│  │  └─ d : STRUCT_CALL
│  └─ chars "keep"
├─ = : chars
│  ├─ .name : chars
│  │  └─ .entry : STRUCT_CALL
│  │     └─ d : STRUCT_CALL
│  └─ chars "gate"
├─ = : int
│  ├─ .floor : int
│  │  └─ .entry : STRUCT_CALL
│  │     └─ d : STRUCT_CALL
│  └─ int 1
└─ output : void
   ├─ .name : chars
   │  └─ d : STRUCT_CALL
   ├─ chars " entry "
   ├─ .name : chars
   │  └─ .entry : STRUCT_CALL
   │     └─ d : STRUCT_CALL
   ├─ chars " floor "
   ├─ .floor : int
   │  └─ .entry : STRUCT_CALL
   │     └─ d : STRUCT_CALL
   └─ chars "\n"
```

```out
keep entry gate floor 1
```

```err
```

```ll

%Dungeon = type { i8*, %Room }
%Room = type { i8*, i32 }

@str = private unnamed_addr constant [5 x i8] c"keep\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"gate\00", align 1
@str.2 = private unnamed_addr constant [8 x i8] c" entry \00", align 1
@str.3 = private unnamed_addr constant [8 x i8] c" floor \00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [13 x i8] c"%s%s%s%s%d%s\00", align 1

define i32 @main() {
entry:
  %d = alloca %Dungeon, align 8
  store %Dungeon zeroinitializer, %Dungeon* %d, align 8
  %name = getelementptr %Dungeon, %Dungeon* %d, i32 0, i32 0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %name, align 8
  %entry1 = getelementptr %Dungeon, %Dungeon* %d, i32 0, i32 1
  %name2 = getelementptr %Room, %Room* %entry1, i32 0, i32 0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0), i8** %name2, align 8
  %entry3 = getelementptr %Dungeon, %Dungeon* %d, i32 0, i32 1
  %floor = getelementptr %Room, %Room* %entry3, i32 0, i32 1
  store i32 1, i32* %floor, align 4
  %name4 = getelementptr %Dungeon, %Dungeon* %d, i32 0, i32 0
  %name5 = load i8*, i8** %name4, align 8
  %entry6 = getelementptr %Dungeon, %Dungeon* %d, i32 0, i32 1
  %name7 = getelementptr %Room, %Room* %entry6, i32 0, i32 0
  %name8 = load i8*, i8** %name7, align 8
  %entry9 = getelementptr %Dungeon, %Dungeon* %d, i32 0, i32 1
  %floor10 = getelementptr %Room, %Room* %entry9, i32 0, i32 1
  %floor11 = load i32, i32* %floor10, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i8* %name5, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.2, i32 0, i32 0), i8* %name8, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.3, i32 0, i32 0), i32 %floor11, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 014 — a stack array of structs: Room[N]

```ura
// structs/014.ura - a stack array of structs: Room[N]

struct Room:
    name  chars
    floor int

main():
    s Room[] = Room[2]
    s[0].name  = "lobby"
    s[0].floor = 1
    s[1].name  = "cellar"
    output(s[0].name, "@", s[0].floor, " ", s[1].name, "@", s[1].floor, "\n")
```

```tree
struct Room
├─ name : chars
└─ floor : int

fn main() : int
├─ = : array
│  ├─ s : STRUCT_CALL[]
│  └─ array : STRUCT_CALL[]
│     └─ int 2
├─ = : chars
│  ├─ .name : chars
│  │  └─ index : STRUCT_CALL
│  │     ├─ s : STRUCT_CALL[]
│  │     └─ int 0
│  └─ chars "lobby"
├─ = : int
│  ├─ .floor : int
│  │  └─ index : STRUCT_CALL
│  │     ├─ s : STRUCT_CALL[]
│  │     └─ int 0
│  └─ int 1
├─ = : chars
│  ├─ .name : chars
│  │  └─ index : STRUCT_CALL
│  │     ├─ s : STRUCT_CALL[]
│  │     └─ int 1
│  └─ chars "cellar"
└─ output : void
   ├─ .name : chars
   │  └─ index : STRUCT_CALL
   │     ├─ s : STRUCT_CALL[]
   │     └─ int 0
   ├─ chars "@"
   ├─ .floor : int
   │  └─ index : STRUCT_CALL
   │     ├─ s : STRUCT_CALL[]
   │     └─ int 0
   ├─ chars " "
   ├─ .name : chars
   │  └─ index : STRUCT_CALL
   │     ├─ s : STRUCT_CALL[]
   │     └─ int 1
   ├─ chars "@"
   ├─ .floor : int
   │  └─ index : STRUCT_CALL
   │     ├─ s : STRUCT_CALL[]
   │     └─ int 1
   └─ chars "\n"
```

```out
lobby@1 cellar@0
```

```err
```

```ll

%Room = type { i8*, i32 }

@str = private unnamed_addr constant [6 x i8] c"lobby\00", align 1
@str.1 = private unnamed_addr constant [7 x i8] c"cellar\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"@\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"@\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [17 x i8] c"%s%s%d%s%s%s%d%s\00", align 1

define i32 @main() {
entry:
  %s = alloca { %Room*, i64 }, align 8
  %arr = alloca %Room, i64 2, align 8
  %0 = bitcast %Room* %arr to i8*
  call void @llvm.memset.p0i8.i64(i8* %0, i8 0, i64 32, i1 false)
  %arr.ptr = insertvalue { %Room*, i64 } undef, %Room* %arr, 0
  %arr.len = insertvalue { %Room*, i64 } %arr.ptr, i64 2, 1
  store { %Room*, i64 } %arr.len, { %Room*, i64 }* %s, align 8
  %s1 = load { %Room*, i64 }, { %Room*, i64 }* %s, align 8
  %arr.data = extractvalue { %Room*, i64 } %s1, 0
  %arr.at = getelementptr %Room, %Room* %arr.data, i32 0
  %name = getelementptr %Room, %Room* %arr.at, i32 0, i32 0
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i8** %name, align 8
  %s2 = load { %Room*, i64 }, { %Room*, i64 }* %s, align 8
  %arr.data3 = extractvalue { %Room*, i64 } %s2, 0
  %arr.at4 = getelementptr %Room, %Room* %arr.data3, i32 0
  %floor = getelementptr %Room, %Room* %arr.at4, i32 0, i32 1
  store i32 1, i32* %floor, align 4
  %s5 = load { %Room*, i64 }, { %Room*, i64 }* %s, align 8
  %arr.data6 = extractvalue { %Room*, i64 } %s5, 0
  %arr.at7 = getelementptr %Room, %Room* %arr.data6, i32 1
  %name8 = getelementptr %Room, %Room* %arr.at7, i32 0, i32 0
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0), i8** %name8, align 8
  %s9 = load { %Room*, i64 }, { %Room*, i64 }* %s, align 8
  %arr.data10 = extractvalue { %Room*, i64 } %s9, 0
  %arr.at11 = getelementptr %Room, %Room* %arr.data10, i32 0
  %name12 = getelementptr %Room, %Room* %arr.at11, i32 0, i32 0
  %name13 = load i8*, i8** %name12, align 8
  %s14 = load { %Room*, i64 }, { %Room*, i64 }* %s, align 8
  %arr.data15 = extractvalue { %Room*, i64 } %s14, 0
  %arr.at16 = getelementptr %Room, %Room* %arr.data15, i32 0
  %floor17 = getelementptr %Room, %Room* %arr.at16, i32 0, i32 1
  %floor18 = load i32, i32* %floor17, align 4
  %s19 = load { %Room*, i64 }, { %Room*, i64 }* %s, align 8
  %arr.data20 = extractvalue { %Room*, i64 } %s19, 0
  %arr.at21 = getelementptr %Room, %Room* %arr.data20, i32 1
  %name22 = getelementptr %Room, %Room* %arr.at21, i32 0, i32 0
  %name23 = load i8*, i8** %name22, align 8
  %s24 = load { %Room*, i64 }, { %Room*, i64 }* %s, align 8
  %arr.data25 = extractvalue { %Room*, i64 } %s24, 0
  %arr.at26 = getelementptr %Room, %Room* %arr.data25, i32 1
  %floor27 = getelementptr %Room, %Room* %arr.at26, i32 0, i32 1
  %floor28 = load i32, i32* %floor27, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt, i32 0, i32 0), i8* %name13, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0), i32 %floor18, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0), i8* %name23, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0), i32 %floor28, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  ret i32 0
}

; Function Attrs: argmemonly nofree nounwind willreturn writeonly
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #0

declare i32 @printf(i8*, ...)

attributes #0 = { argmemonly nofree nounwind willreturn writeonly }
```

## 015 — a heap array of structs: new Room[N], then clean

```ura
// structs/015.ura - a heap array of structs: new Room[N], then clean

struct Room:
    name  chars
    floor int

main():
    h Room[] = new Room[2]
    h[0].name  = "attic"
    h[0].floor = 9
    h[1].floor = 2
    output(h[0].name, "@", h[0].floor, " ", h[1].floor, "\n")
    clean h
```

```tree
struct Room
├─ name : chars
└─ floor : int

fn main() : int
├─ = : array
│  ├─ h : STRUCT_CALL[]
│  └─ array : STRUCT_CALL[]
│     └─ int 2
├─ = : chars
│  ├─ .name : chars
│  │  └─ index : STRUCT_CALL
│  │     ├─ h : STRUCT_CALL[]
│  │     └─ int 0
│  └─ chars "attic"
├─ = : int
│  ├─ .floor : int
│  │  └─ index : STRUCT_CALL
│  │     ├─ h : STRUCT_CALL[]
│  │     └─ int 0
│  └─ int 9
├─ = : int
│  ├─ .floor : int
│  │  └─ index : STRUCT_CALL
│  │     ├─ h : STRUCT_CALL[]
│  │     └─ int 1
│  └─ int 2
├─ output : void
│  ├─ .name : chars
│  │  └─ index : STRUCT_CALL
│  │     ├─ h : STRUCT_CALL[]
│  │     └─ int 0
│  ├─ chars "@"
│  ├─ .floor : int
│  │  └─ index : STRUCT_CALL
│  │     ├─ h : STRUCT_CALL[]
│  │     └─ int 0
│  ├─ chars " "
│  ├─ .floor : int
│  │  └─ index : STRUCT_CALL
│  │     ├─ h : STRUCT_CALL[]
│  │     └─ int 1
│  └─ chars "\n"
└─ clean : void
   └─ h : STRUCT_CALL[]
```

```out
attic@9 2
```

```err
```

```ll

%Room = type { i8*, i32 }

@str = private unnamed_addr constant [6 x i8] c"attic\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"@\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [13 x i8] c"%s%s%d%s%d%s\00", align 1

define i32 @main() {
entry:
  %h = alloca { %Room*, i64 }, align 8
  %heap = call i8* @calloc(i64 2, i64 16)
  %arr = bitcast i8* %heap to %Room*
  %arr.ptr = insertvalue { %Room*, i64 } undef, %Room* %arr, 0
  %arr.len = insertvalue { %Room*, i64 } %arr.ptr, i64 2, 1
  store { %Room*, i64 } %arr.len, { %Room*, i64 }* %h, align 8
  %h1 = load { %Room*, i64 }, { %Room*, i64 }* %h, align 8
  %arr.data = extractvalue { %Room*, i64 } %h1, 0
  %arr.at = getelementptr %Room, %Room* %arr.data, i32 0
  %name = getelementptr %Room, %Room* %arr.at, i32 0, i32 0
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i8** %name, align 8
  %h2 = load { %Room*, i64 }, { %Room*, i64 }* %h, align 8
  %arr.data3 = extractvalue { %Room*, i64 } %h2, 0
  %arr.at4 = getelementptr %Room, %Room* %arr.data3, i32 0
  %floor = getelementptr %Room, %Room* %arr.at4, i32 0, i32 1
  store i32 9, i32* %floor, align 4
  %h5 = load { %Room*, i64 }, { %Room*, i64 }* %h, align 8
  %arr.data6 = extractvalue { %Room*, i64 } %h5, 0
  %arr.at7 = getelementptr %Room, %Room* %arr.data6, i32 1
  %floor8 = getelementptr %Room, %Room* %arr.at7, i32 0, i32 1
  store i32 2, i32* %floor8, align 4
  %h9 = load { %Room*, i64 }, { %Room*, i64 }* %h, align 8
  %arr.data10 = extractvalue { %Room*, i64 } %h9, 0
  %arr.at11 = getelementptr %Room, %Room* %arr.data10, i32 0
  %name12 = getelementptr %Room, %Room* %arr.at11, i32 0, i32 0
  %name13 = load i8*, i8** %name12, align 8
  %h14 = load { %Room*, i64 }, { %Room*, i64 }* %h, align 8
  %arr.data15 = extractvalue { %Room*, i64 } %h14, 0
  %arr.at16 = getelementptr %Room, %Room* %arr.data15, i32 0
  %floor17 = getelementptr %Room, %Room* %arr.at16, i32 0, i32 1
  %floor18 = load i32, i32* %floor17, align 4
  %h19 = load { %Room*, i64 }, { %Room*, i64 }* %h, align 8
  %arr.data20 = extractvalue { %Room*, i64 } %h19, 0
  %arr.at21 = getelementptr %Room, %Room* %arr.data20, i32 1
  %floor22 = getelementptr %Room, %Room* %arr.at21, i32 0, i32 1
  %floor23 = load i32, i32* %floor22, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i8* %name13, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i32 %floor18, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0), i32 %floor23, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %arr24 = load { %Room*, i64 }, { %Room*, i64 }* %h, align 8
  %arr.data25 = extractvalue { %Room*, i64 } %arr24, 0
  %free.ptr = bitcast %Room* %arr.data25 to i8*
  call void @free(i8* %free.ptr)
  store { %Room*, i64 } zeroinitializer, { %Room*, i64 }* %h, align 8
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i32 @printf(i8*, ...)

declare void @free(i8*)
```

## 016 — a 2D array of structs: Room[N][M]

```ura
// structs/016.ura - a 2D array of structs: Room[N][M]

struct Room:
    name  chars
    floor int

main():
    g Room[][] = Room[2][1]
    g[0][0].name  = "grid-a"
    g[0][0].floor = 3
    g[1][0].name  = "grid-b"
    g[1][0].floor = 4
    output(g[0][0].name, "@", g[0][0].floor, " ", g[1][0].name, "@", g[1][0].floor, "\n")
```

```tree
struct Room
├─ name : chars
└─ floor : int

fn main() : int
├─ = : array
│  ├─ g : STRUCT_CALL[][]
│  └─ array : STRUCT_CALL[][]
│     ├─ int 2
│     └─ int 1
├─ = : chars
│  ├─ .name : chars
│  │  └─ index : STRUCT_CALL
│  │     ├─ index : STRUCT_CALL[]
│  │     │  ├─ g : STRUCT_CALL[][]
│  │     │  └─ int 0
│  │     └─ int 0
│  └─ chars "grid-a"
├─ = : int
│  ├─ .floor : int
│  │  └─ index : STRUCT_CALL
│  │     ├─ index : STRUCT_CALL[]
│  │     │  ├─ g : STRUCT_CALL[][]
│  │     │  └─ int 0
│  │     └─ int 0
│  └─ int 3
├─ = : chars
│  ├─ .name : chars
│  │  └─ index : STRUCT_CALL
│  │     ├─ index : STRUCT_CALL[]
│  │     │  ├─ g : STRUCT_CALL[][]
│  │     │  └─ int 1
│  │     └─ int 0
│  └─ chars "grid-b"
├─ = : int
│  ├─ .floor : int
│  │  └─ index : STRUCT_CALL
│  │     ├─ index : STRUCT_CALL[]
│  │     │  ├─ g : STRUCT_CALL[][]
│  │     │  └─ int 1
│  │     └─ int 0
│  └─ int 4
└─ output : void
   ├─ .name : chars
   │  └─ index : STRUCT_CALL
   │     ├─ index : STRUCT_CALL[]
   │     │  ├─ g : STRUCT_CALL[][]
   │     │  └─ int 0
   │     └─ int 0
   ├─ chars "@"
   ├─ .floor : int
   │  └─ index : STRUCT_CALL
   │     ├─ index : STRUCT_CALL[]
   │     │  ├─ g : STRUCT_CALL[][]
   │     │  └─ int 0
   │     └─ int 0
   ├─ chars " "
   ├─ .name : chars
   │  └─ index : STRUCT_CALL
   │     ├─ index : STRUCT_CALL[]
   │     │  ├─ g : STRUCT_CALL[][]
   │     │  └─ int 1
   │     └─ int 0
   ├─ chars "@"
   ├─ .floor : int
   │  └─ index : STRUCT_CALL
   │     ├─ index : STRUCT_CALL[]
   │     │  ├─ g : STRUCT_CALL[][]
   │     │  └─ int 1
   │     └─ int 0
   └─ chars "\n"
```

```out
grid-a@3 grid-b@4
```

```err
```

```ll

%Room = type { i8*, i32 }

@str = private unnamed_addr constant [7 x i8] c"grid-a\00", align 1
@str.1 = private unnamed_addr constant [7 x i8] c"grid-b\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"@\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"@\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [17 x i8] c"%s%s%d%s%s%s%d%s\00", align 1

define i32 @main() {
entry:
  %g = alloca { { %Room*, i64 }*, i64 }, align 8
  %arr = alloca { %Room*, i64 }, i64 2, align 8
  %i = alloca i64, align 8
  store i64 0, i64* %i, align 4
  br label %arr.cond

arr.cond:                                         ; preds = %arr.body, %entry
  %i1 = load i64, i64* %i, align 4
  %more = icmp slt i64 %i1, 2
  br i1 %more, label %arr.body, label %arr.end

arr.body:                                         ; preds = %arr.cond
  %arr2 = alloca %Room, i64 1, align 8
  %0 = bitcast %Room* %arr2 to i8*
  call void @llvm.memset.p0i8.i64(i8* %0, i8 0, i64 16, i1 false)
  %arr.ptr = insertvalue { %Room*, i64 } undef, %Room* %arr2, 0
  %arr.len = insertvalue { %Room*, i64 } %arr.ptr, i64 1, 1
  %i3 = load i64, i64* %i, align 4
  %arr.slot = getelementptr { %Room*, i64 }, { %Room*, i64 }* %arr, i64 %i3
  store { %Room*, i64 } %arr.len, { %Room*, i64 }* %arr.slot, align 8
  %next = add i64 %i3, 1
  store i64 %next, i64* %i, align 4
  br label %arr.cond

arr.end:                                          ; preds = %arr.cond
  %arr.ptr4 = insertvalue { { %Room*, i64 }*, i64 } undef, { %Room*, i64 }* %arr, 0
  %arr.len5 = insertvalue { { %Room*, i64 }*, i64 } %arr.ptr4, i64 2, 1
  store { { %Room*, i64 }*, i64 } %arr.len5, { { %Room*, i64 }*, i64 }* %g, align 8
  %g6 = load { { %Room*, i64 }*, i64 }, { { %Room*, i64 }*, i64 }* %g, align 8
  %arr.data = extractvalue { { %Room*, i64 }*, i64 } %g6, 0
  %arr.at = getelementptr { %Room*, i64 }, { %Room*, i64 }* %arr.data, i32 0
  %idx = load { %Room*, i64 }, { %Room*, i64 }* %arr.at, align 8
  %arr.data7 = extractvalue { %Room*, i64 } %idx, 0
  %arr.at8 = getelementptr %Room, %Room* %arr.data7, i32 0
  %name = getelementptr %Room, %Room* %arr.at8, i32 0, i32 0
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i8** %name, align 8
  %g9 = load { { %Room*, i64 }*, i64 }, { { %Room*, i64 }*, i64 }* %g, align 8
  %arr.data10 = extractvalue { { %Room*, i64 }*, i64 } %g9, 0
  %arr.at11 = getelementptr { %Room*, i64 }, { %Room*, i64 }* %arr.data10, i32 0
  %idx12 = load { %Room*, i64 }, { %Room*, i64 }* %arr.at11, align 8
  %arr.data13 = extractvalue { %Room*, i64 } %idx12, 0
  %arr.at14 = getelementptr %Room, %Room* %arr.data13, i32 0
  %floor = getelementptr %Room, %Room* %arr.at14, i32 0, i32 1
  store i32 3, i32* %floor, align 4
  %g15 = load { { %Room*, i64 }*, i64 }, { { %Room*, i64 }*, i64 }* %g, align 8
  %arr.data16 = extractvalue { { %Room*, i64 }*, i64 } %g15, 0
  %arr.at17 = getelementptr { %Room*, i64 }, { %Room*, i64 }* %arr.data16, i32 1
  %idx18 = load { %Room*, i64 }, { %Room*, i64 }* %arr.at17, align 8
  %arr.data19 = extractvalue { %Room*, i64 } %idx18, 0
  %arr.at20 = getelementptr %Room, %Room* %arr.data19, i32 0
  %name21 = getelementptr %Room, %Room* %arr.at20, i32 0, i32 0
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0), i8** %name21, align 8
  %g22 = load { { %Room*, i64 }*, i64 }, { { %Room*, i64 }*, i64 }* %g, align 8
  %arr.data23 = extractvalue { { %Room*, i64 }*, i64 } %g22, 0
  %arr.at24 = getelementptr { %Room*, i64 }, { %Room*, i64 }* %arr.data23, i32 1
  %idx25 = load { %Room*, i64 }, { %Room*, i64 }* %arr.at24, align 8
  %arr.data26 = extractvalue { %Room*, i64 } %idx25, 0
  %arr.at27 = getelementptr %Room, %Room* %arr.data26, i32 0
  %floor28 = getelementptr %Room, %Room* %arr.at27, i32 0, i32 1
  store i32 4, i32* %floor28, align 4
  %g29 = load { { %Room*, i64 }*, i64 }, { { %Room*, i64 }*, i64 }* %g, align 8
  %arr.data30 = extractvalue { { %Room*, i64 }*, i64 } %g29, 0
  %arr.at31 = getelementptr { %Room*, i64 }, { %Room*, i64 }* %arr.data30, i32 0
  %idx32 = load { %Room*, i64 }, { %Room*, i64 }* %arr.at31, align 8
  %arr.data33 = extractvalue { %Room*, i64 } %idx32, 0
  %arr.at34 = getelementptr %Room, %Room* %arr.data33, i32 0
  %name35 = getelementptr %Room, %Room* %arr.at34, i32 0, i32 0
  %name36 = load i8*, i8** %name35, align 8
  %g37 = load { { %Room*, i64 }*, i64 }, { { %Room*, i64 }*, i64 }* %g, align 8
  %arr.data38 = extractvalue { { %Room*, i64 }*, i64 } %g37, 0
  %arr.at39 = getelementptr { %Room*, i64 }, { %Room*, i64 }* %arr.data38, i32 0
  %idx40 = load { %Room*, i64 }, { %Room*, i64 }* %arr.at39, align 8
  %arr.data41 = extractvalue { %Room*, i64 } %idx40, 0
  %arr.at42 = getelementptr %Room, %Room* %arr.data41, i32 0
  %floor43 = getelementptr %Room, %Room* %arr.at42, i32 0, i32 1
  %floor44 = load i32, i32* %floor43, align 4
  %g45 = load { { %Room*, i64 }*, i64 }, { { %Room*, i64 }*, i64 }* %g, align 8
  %arr.data46 = extractvalue { { %Room*, i64 }*, i64 } %g45, 0
  %arr.at47 = getelementptr { %Room*, i64 }, { %Room*, i64 }* %arr.data46, i32 1
  %idx48 = load { %Room*, i64 }, { %Room*, i64 }* %arr.at47, align 8
  %arr.data49 = extractvalue { %Room*, i64 } %idx48, 0
  %arr.at50 = getelementptr %Room, %Room* %arr.data49, i32 0
  %name51 = getelementptr %Room, %Room* %arr.at50, i32 0, i32 0
  %name52 = load i8*, i8** %name51, align 8
  %g53 = load { { %Room*, i64 }*, i64 }, { { %Room*, i64 }*, i64 }* %g, align 8
  %arr.data54 = extractvalue { { %Room*, i64 }*, i64 } %g53, 0
  %arr.at55 = getelementptr { %Room*, i64 }, { %Room*, i64 }* %arr.data54, i32 1
  %idx56 = load { %Room*, i64 }, { %Room*, i64 }* %arr.at55, align 8
  %arr.data57 = extractvalue { %Room*, i64 } %idx56, 0
  %arr.at58 = getelementptr %Room, %Room* %arr.data57, i32 0
  %floor59 = getelementptr %Room, %Room* %arr.at58, i32 0, i32 1
  %floor60 = load i32, i32* %floor59, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt, i32 0, i32 0), i8* %name36, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0), i32 %floor44, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0), i8* %name52, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0), i32 %floor60, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  ret i32 0
}

; Function Attrs: argmemonly nofree nounwind willreturn writeonly
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #0

declare i32 @printf(i8*, ...)

attributes #0 = { argmemonly nofree nounwind willreturn writeonly }
```

## 017 — a 2D heap array of structs frees every inner slice

```ura
// structs/017.ura - a 2D heap array of structs frees every inner slice

struct Room:
    floor int

main():
    g Room[][] = new Room[2][2]
    g[0][0].floor = 1
    g[1][1].floor = 4
    output(g[0][0].floor, " ", g[1][1].floor, "\n")
    clean g
```

```tree
struct Room
└─ floor : int

fn main() : int
├─ = : array
│  ├─ g : STRUCT_CALL[][]
│  └─ array : STRUCT_CALL[][]
│     ├─ int 2
│     └─ int 2
├─ = : int
│  ├─ .floor : int
│  │  └─ index : STRUCT_CALL
│  │     ├─ index : STRUCT_CALL[]
│  │     │  ├─ g : STRUCT_CALL[][]
│  │     │  └─ int 0
│  │     └─ int 0
│  └─ int 1
├─ = : int
│  ├─ .floor : int
│  │  └─ index : STRUCT_CALL
│  │     ├─ index : STRUCT_CALL[]
│  │     │  ├─ g : STRUCT_CALL[][]
│  │     │  └─ int 1
│  │     └─ int 1
│  └─ int 4
├─ output : void
│  ├─ .floor : int
│  │  └─ index : STRUCT_CALL
│  │     ├─ index : STRUCT_CALL[]
│  │     │  ├─ g : STRUCT_CALL[][]
│  │     │  └─ int 0
│  │     └─ int 0
│  ├─ chars " "
│  ├─ .floor : int
│  │  └─ index : STRUCT_CALL
│  │     ├─ index : STRUCT_CALL[]
│  │     │  ├─ g : STRUCT_CALL[][]
│  │     │  └─ int 1
│  │     └─ int 1
│  └─ chars "\n"
└─ clean : void
   └─ g : STRUCT_CALL[][]
```

```out
1 4
```

```err
```

```ll

%Room = type { i32 }

@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [9 x i8] c"%d%s%d%s\00", align 1

define i32 @main() {
entry:
  %g = alloca { { %Room*, i64 }*, i64 }, align 8
  %heap = call i8* @calloc(i64 2, i64 16)
  %arr = bitcast i8* %heap to { %Room*, i64 }*
  %i = alloca i64, align 8
  store i64 0, i64* %i, align 4
  br label %arr.cond

arr.cond:                                         ; preds = %arr.body, %entry
  %i1 = load i64, i64* %i, align 4
  %more = icmp slt i64 %i1, 2
  br i1 %more, label %arr.body, label %arr.end

arr.body:                                         ; preds = %arr.cond
  %heap2 = call i8* @calloc(i64 2, i64 4)
  %arr3 = bitcast i8* %heap2 to %Room*
  %arr.ptr = insertvalue { %Room*, i64 } undef, %Room* %arr3, 0
  %arr.len = insertvalue { %Room*, i64 } %arr.ptr, i64 2, 1
  %i4 = load i64, i64* %i, align 4
  %arr.slot = getelementptr { %Room*, i64 }, { %Room*, i64 }* %arr, i64 %i4
  store { %Room*, i64 } %arr.len, { %Room*, i64 }* %arr.slot, align 8
  %next = add i64 %i4, 1
  store i64 %next, i64* %i, align 4
  br label %arr.cond

arr.end:                                          ; preds = %arr.cond
  %arr.ptr5 = insertvalue { { %Room*, i64 }*, i64 } undef, { %Room*, i64 }* %arr, 0
  %arr.len6 = insertvalue { { %Room*, i64 }*, i64 } %arr.ptr5, i64 2, 1
  store { { %Room*, i64 }*, i64 } %arr.len6, { { %Room*, i64 }*, i64 }* %g, align 8
  %g7 = load { { %Room*, i64 }*, i64 }, { { %Room*, i64 }*, i64 }* %g, align 8
  %arr.data = extractvalue { { %Room*, i64 }*, i64 } %g7, 0
  %arr.at = getelementptr { %Room*, i64 }, { %Room*, i64 }* %arr.data, i32 0
  %idx = load { %Room*, i64 }, { %Room*, i64 }* %arr.at, align 8
  %arr.data8 = extractvalue { %Room*, i64 } %idx, 0
  %arr.at9 = getelementptr %Room, %Room* %arr.data8, i32 0
  %floor = getelementptr %Room, %Room* %arr.at9, i32 0, i32 0
  store i32 1, i32* %floor, align 4
  %g10 = load { { %Room*, i64 }*, i64 }, { { %Room*, i64 }*, i64 }* %g, align 8
  %arr.data11 = extractvalue { { %Room*, i64 }*, i64 } %g10, 0
  %arr.at12 = getelementptr { %Room*, i64 }, { %Room*, i64 }* %arr.data11, i32 1
  %idx13 = load { %Room*, i64 }, { %Room*, i64 }* %arr.at12, align 8
  %arr.data14 = extractvalue { %Room*, i64 } %idx13, 0
  %arr.at15 = getelementptr %Room, %Room* %arr.data14, i32 1
  %floor16 = getelementptr %Room, %Room* %arr.at15, i32 0, i32 0
  store i32 4, i32* %floor16, align 4
  %g17 = load { { %Room*, i64 }*, i64 }, { { %Room*, i64 }*, i64 }* %g, align 8
  %arr.data18 = extractvalue { { %Room*, i64 }*, i64 } %g17, 0
  %arr.at19 = getelementptr { %Room*, i64 }, { %Room*, i64 }* %arr.data18, i32 0
  %idx20 = load { %Room*, i64 }, { %Room*, i64 }* %arr.at19, align 8
  %arr.data21 = extractvalue { %Room*, i64 } %idx20, 0
  %arr.at22 = getelementptr %Room, %Room* %arr.data21, i32 0
  %floor23 = getelementptr %Room, %Room* %arr.at22, i32 0, i32 0
  %floor24 = load i32, i32* %floor23, align 4
  %g25 = load { { %Room*, i64 }*, i64 }, { { %Room*, i64 }*, i64 }* %g, align 8
  %arr.data26 = extractvalue { { %Room*, i64 }*, i64 } %g25, 0
  %arr.at27 = getelementptr { %Room*, i64 }, { %Room*, i64 }* %arr.data26, i32 1
  %idx28 = load { %Room*, i64 }, { %Room*, i64 }* %arr.at27, align 8
  %arr.data29 = extractvalue { %Room*, i64 } %idx28, 0
  %arr.at30 = getelementptr %Room, %Room* %arr.data29, i32 1
  %floor31 = getelementptr %Room, %Room* %arr.at30, i32 0, i32 0
  %floor32 = load i32, i32* %floor31, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt, i32 0, i32 0), i32 %floor24, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %floor32, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %arr33 = load { { %Room*, i64 }*, i64 }, { { %Room*, i64 }*, i64 }* %g, align 8
  %arr.data34 = extractvalue { { %Room*, i64 }*, i64 } %arr33, 0
  %arr.len35 = extractvalue { { %Room*, i64 }*, i64 } %arr33, 1
  %i36 = alloca i64, align 8
  store i64 0, i64* %i36, align 4
  br label %free.cond

free.cond:                                        ; preds = %free.body, %arr.end
  %i37 = load i64, i64* %i36, align 4
  %more38 = icmp slt i64 %i37, %arr.len35
  br i1 %more38, label %free.body, label %free.end

free.body:                                        ; preds = %free.cond
  %free.slot = getelementptr { %Room*, i64 }, { %Room*, i64 }* %arr.data34, i64 %i37
  %inner = load { %Room*, i64 }, { %Room*, i64 }* %free.slot, align 8
  %arr.data39 = extractvalue { %Room*, i64 } %inner, 0
  %free.ptr = bitcast %Room* %arr.data39 to i8*
  call void @free(i8* %free.ptr)
  %i40 = load i64, i64* %i36, align 4
  %next41 = add i64 %i40, 1
  store i64 %next41, i64* %i36, align 4
  br label %free.cond

free.end:                                         ; preds = %free.cond
  %free.ptr42 = bitcast { %Room*, i64 }* %arr.data34 to i8*
  call void @free(i8* %free.ptr42)
  store { { %Room*, i64 }*, i64 } zeroinitializer, { { %Room*, i64 }*, i64 }* %g, align 8
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i32 @printf(i8*, ...)

declare void @free(i8*)
```

## 018 — '.len' on arrays of structs, including 2D

```ura
// structs/018.ura - '.len' on arrays of structs, including 2D

struct Room:
    floor int

main():
    s Room[]   = Room[2]
    g Room[][] = Room[4][1]
    output(s.len, " ", g.len, "\n")
```

```tree
struct Room
└─ floor : int

fn main() : int
├─ = : array
│  ├─ s : STRUCT_CALL[]
│  └─ array : STRUCT_CALL[]
│     └─ int 2
├─ = : array
│  ├─ g : STRUCT_CALL[][]
│  └─ array : STRUCT_CALL[][]
│     ├─ int 4
│     └─ int 1
└─ output : void
   ├─ .len : int
   │  └─ s : STRUCT_CALL[]
   ├─ chars " "
   ├─ .len : int
   │  └─ g : STRUCT_CALL[][]
   └─ chars "\n"
```

```out
2 4
```

```err
```

```ll

%Room = type { i32 }

@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [9 x i8] c"%d%s%d%s\00", align 1

define i32 @main() {
entry:
  %s = alloca { %Room*, i64 }, align 8
  %arr = alloca %Room, i64 2, align 8
  %0 = bitcast %Room* %arr to i8*
  call void @llvm.memset.p0i8.i64(i8* %0, i8 0, i64 8, i1 false)
  %arr.ptr = insertvalue { %Room*, i64 } undef, %Room* %arr, 0
  %arr.len = insertvalue { %Room*, i64 } %arr.ptr, i64 2, 1
  store { %Room*, i64 } %arr.len, { %Room*, i64 }* %s, align 8
  %g = alloca { { %Room*, i64 }*, i64 }, align 8
  %arr1 = alloca { %Room*, i64 }, i64 4, align 8
  %i = alloca i64, align 8
  store i64 0, i64* %i, align 4
  br label %arr.cond

arr.cond:                                         ; preds = %arr.body, %entry
  %i2 = load i64, i64* %i, align 4
  %more = icmp slt i64 %i2, 4
  br i1 %more, label %arr.body, label %arr.end

arr.body:                                         ; preds = %arr.cond
  %arr3 = alloca %Room, i64 1, align 8
  %1 = bitcast %Room* %arr3 to i8*
  call void @llvm.memset.p0i8.i64(i8* %1, i8 0, i64 4, i1 false)
  %arr.ptr4 = insertvalue { %Room*, i64 } undef, %Room* %arr3, 0
  %arr.len5 = insertvalue { %Room*, i64 } %arr.ptr4, i64 1, 1
  %i6 = load i64, i64* %i, align 4
  %arr.slot = getelementptr { %Room*, i64 }, { %Room*, i64 }* %arr1, i64 %i6
  store { %Room*, i64 } %arr.len5, { %Room*, i64 }* %arr.slot, align 8
  %next = add i64 %i6, 1
  store i64 %next, i64* %i, align 4
  br label %arr.cond

arr.end:                                          ; preds = %arr.cond
  %arr.ptr7 = insertvalue { { %Room*, i64 }*, i64 } undef, { %Room*, i64 }* %arr1, 0
  %arr.len8 = insertvalue { { %Room*, i64 }*, i64 } %arr.ptr7, i64 4, 1
  store { { %Room*, i64 }*, i64 } %arr.len8, { { %Room*, i64 }*, i64 }* %g, align 8
  %s9 = load { %Room*, i64 }, { %Room*, i64 }* %s, align 8
  %len = extractvalue { %Room*, i64 } %s9, 1
  %len10 = trunc i64 %len to i32
  %g11 = load { { %Room*, i64 }*, i64 }, { { %Room*, i64 }*, i64 }* %g, align 8
  %len12 = extractvalue { { %Room*, i64 }*, i64 } %g11, 1
  %len13 = trunc i64 %len12 to i32
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt, i32 0, i32 0), i32 %len10, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %len13, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

; Function Attrs: argmemonly nofree nounwind willreturn writeonly
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #0

declare i32 @printf(i8*, ...)

attributes #0 = { argmemonly nofree nounwind willreturn writeonly }
```

## 019 — 'stack' and 'heap' are ordinary identifiers

```ura
// structs/019.ura - 'stack' and 'heap' are ordinary identifiers

main():
    stack int = 5
    heap  int = 7
    output(stack, " ", heap, "\n")
```

```tree
fn main() : int
├─ = : int
│  ├─ stack : int
│  └─ int 5
├─ = : int
│  ├─ heap : int
│  └─ int 7
└─ output : void
   ├─ stack : int
   ├─ chars " "
   ├─ heap : int
   └─ chars "\n"
```

```out
5 7
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [9 x i8] c"%d%s%d%s\00", align 1

define i32 @main() {
entry:
  %stack = alloca i32, align 4
  store i32 5, i32* %stack, align 4
  %heap = alloca i32, align 4
  store i32 7, i32* %heap, align 4
  %stack1 = load i32, i32* %stack, align 4
  %heap2 = load i32, i32* %heap, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt, i32 0, i32 0), i32 %stack1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %heap2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 020 — a 'ref?' field may refer to its own struct

```ura
// structs/020.ura - a 'ref?' field may refer to its own struct

struct Node:
    value int
    ref? next Node

main():
    a Node
    a.value = 1
    output(a.value, "\n")
```

```tree
struct Node
├─ value : int
└─ next : STRUCT_CALL

fn main() : int
├─ a : STRUCT_CALL
├─ = : int
│  ├─ .value : int
│  │  └─ a : STRUCT_CALL
│  └─ int 1
└─ output : void
   ├─ .value : int
   │  └─ a : STRUCT_CALL
   └─ chars "\n"
```

```out
1
```

```err
```

```ll

%Node = type { i32, %Node* }

@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1

define i32 @main() {
entry:
  %a = alloca %Node, align 8
  store %Node zeroinitializer, %Node* %a, align 8
  %value = getelementptr %Node, %Node* %a, i32 0, i32 0
  store i32 1, i32* %value, align 4
  %value1 = getelementptr %Node, %Node* %a, i32 0, i32 0
  %value2 = load i32, i32* %value1, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 %value2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 021 — bind, read and write through a 'ref?' field

```ura
// structs/021.ura - bind, read and write through a 'ref?' field

struct Node:
    value int
    ref? next Node

main():
    a Node
    b Node
    c Node
    a.value = 1
    b.value = 2
    c.value = 3
    a.next = ref b
    b.next = ref c
    a.next.value = 20
    output(a.next.value, " ", a.next.next.value, " ", b.value, "\n")
```

```tree
struct Node
├─ value : int
└─ next : STRUCT_CALL

fn main() : int
├─ a : STRUCT_CALL
├─ b : STRUCT_CALL
├─ c : STRUCT_CALL
├─ = : int
│  ├─ .value : int
│  │  └─ a : STRUCT_CALL
│  └─ int 1
├─ = : int
│  ├─ .value : int
│  │  └─ b : STRUCT_CALL
│  └─ int 2
├─ = : int
│  ├─ .value : int
│  │  └─ c : STRUCT_CALL
│  └─ int 3
├─ = : STRUCT_CALL
│  ├─ .next : STRUCT_CALL
│  │  └─ a : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ b : STRUCT_CALL
├─ = : STRUCT_CALL
│  ├─ .next : STRUCT_CALL
│  │  └─ b : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ c : STRUCT_CALL
├─ = : int
│  ├─ .value : int
│  │  └─ .next : STRUCT_CALL
│  │     └─ a : STRUCT_CALL
│  └─ int 20
└─ output : void
   ├─ .value : int
   │  └─ .next : STRUCT_CALL
   │     └─ a : STRUCT_CALL
   ├─ chars " "
   ├─ .value : int
   │  └─ .next : STRUCT_CALL
   │     └─ .next : STRUCT_CALL
   │        └─ a : STRUCT_CALL
   ├─ chars " "
   ├─ .value : int
   │  └─ b : STRUCT_CALL
   └─ chars "\n"
```

```out
20 3 20
```

```err
```

```ll

%Node = type { i32, %Node* }

@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [13 x i8] c"%d%s%d%s%d%s\00", align 1

define i32 @main() {
entry:
  %a = alloca %Node, align 8
  store %Node zeroinitializer, %Node* %a, align 8
  %b = alloca %Node, align 8
  store %Node zeroinitializer, %Node* %b, align 8
  %c = alloca %Node, align 8
  store %Node zeroinitializer, %Node* %c, align 8
  %value = getelementptr %Node, %Node* %a, i32 0, i32 0
  store i32 1, i32* %value, align 4
  %value1 = getelementptr %Node, %Node* %b, i32 0, i32 0
  store i32 2, i32* %value1, align 4
  %value2 = getelementptr %Node, %Node* %c, i32 0, i32 0
  store i32 3, i32* %value2, align 4
  %next = getelementptr %Node, %Node* %a, i32 0, i32 1
  store %Node* %b, %Node** %next, align 8
  %next3 = getelementptr %Node, %Node* %b, i32 0, i32 1
  store %Node* %c, %Node** %next3, align 8
  %next4 = getelementptr %Node, %Node* %a, i32 0, i32 1
  %ref = load %Node*, %Node** %next4, align 8
  %value5 = getelementptr %Node, %Node* %ref, i32 0, i32 0
  store i32 20, i32* %value5, align 4
  %next6 = getelementptr %Node, %Node* %a, i32 0, i32 1
  %ref7 = load %Node*, %Node** %next6, align 8
  %value8 = getelementptr %Node, %Node* %ref7, i32 0, i32 0
  %value9 = load i32, i32* %value8, align 4
  %next10 = getelementptr %Node, %Node* %a, i32 0, i32 1
  %ref11 = load %Node*, %Node** %next10, align 8
  %next12 = getelementptr %Node, %Node* %ref11, i32 0, i32 1
  %ref13 = load %Node*, %Node** %next12, align 8
  %value14 = getelementptr %Node, %Node* %ref13, i32 0, i32 0
  %value15 = load i32, i32* %value14, align 4
  %value16 = getelementptr %Node, %Node* %b, i32 0, i32 0
  %value17 = load i32, i32* %value16, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i32 %value9, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %value15, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i32 %value17, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 022 — struct assignment copies by value

```ura
// structs/022.ura - struct assignment copies by value

struct Room:
    name  chars
    floor int

main():
    a Room
    a.name  = "hall"
    a.floor = 2
    b Room
    b = a
    b.floor = 9
    output(b.name, " ", b.floor, " / ", a.name, " ", a.floor, "\n")
```

```tree
struct Room
├─ name : chars
└─ floor : int

fn main() : int
├─ a : STRUCT_CALL
├─ = : chars
│  ├─ .name : chars
│  │  └─ a : STRUCT_CALL
│  └─ chars "hall"
├─ = : int
│  ├─ .floor : int
│  │  └─ a : STRUCT_CALL
│  └─ int 2
├─ b : STRUCT_CALL
├─ = : STRUCT_CALL
│  ├─ b : STRUCT_CALL
│  └─ a : STRUCT_CALL
├─ = : int
│  ├─ .floor : int
│  │  └─ b : STRUCT_CALL
│  └─ int 9
└─ output : void
   ├─ .name : chars
   │  └─ b : STRUCT_CALL
   ├─ chars " "
   ├─ .floor : int
   │  └─ b : STRUCT_CALL
   ├─ chars " / "
   ├─ .name : chars
   │  └─ a : STRUCT_CALL
   ├─ chars " "
   ├─ .floor : int
   │  └─ a : STRUCT_CALL
   └─ chars "\n"
```

```out
hall 9 / hall 2
```

```err
```

```ll

%Room = type { i8*, i32 }

@str = private unnamed_addr constant [5 x i8] c"hall\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c" / \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [17 x i8] c"%s%s%d%s%s%s%d%s\00", align 1

define i32 @main() {
entry:
  %a = alloca %Room, align 8
  store %Room zeroinitializer, %Room* %a, align 8
  %name = getelementptr %Room, %Room* %a, i32 0, i32 0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %name, align 8
  %floor = getelementptr %Room, %Room* %a, i32 0, i32 1
  store i32 2, i32* %floor, align 4
  %b = alloca %Room, align 8
  store %Room zeroinitializer, %Room* %b, align 8
  %a1 = load %Room, %Room* %a, align 8
  store %Room %a1, %Room* %b, align 8
  %floor2 = getelementptr %Room, %Room* %b, i32 0, i32 1
  store i32 9, i32* %floor2, align 4
  %name3 = getelementptr %Room, %Room* %b, i32 0, i32 0
  %name4 = load i8*, i8** %name3, align 8
  %floor5 = getelementptr %Room, %Room* %b, i32 0, i32 1
  %floor6 = load i32, i32* %floor5, align 4
  %name7 = getelementptr %Room, %Room* %a, i32 0, i32 0
  %name8 = load i8*, i8** %name7, align 8
  %floor9 = getelementptr %Room, %Room* %a, i32 0, i32 1
  %floor10 = load i32, i32* %floor9, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt, i32 0, i32 0), i8* %name4, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i32 %floor6, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i8* %name8, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0), i32 %floor10, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```
