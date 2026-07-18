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
