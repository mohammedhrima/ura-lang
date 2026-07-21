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
- 023 — a struct as a by-value parameter
- 024 — a 'ref' struct parameter mutates the caller's struct
- 025 — returning a struct: the create() constructor convention
- 026 — output() prints a struct and its nested struct
- 027 — output() follows a bound 'ref?' and prints null when unbound
- 028 — output() prints [Circular] instead of looping forever
- 029 — output() expands array fields, including 2D
- 030 — a field declared twice
- 031 — a field whose type does not exist
- 032 — a struct that contains itself by value
- 033 — two structs that contain each other by value
- 034 — a variable of a type that was never declared
- 035 — reading a field the struct does not have
- 036 — reading a field from a scalar
- 037 — '.len' on a scalar
- 038 — a struct declared in a block is not visible outside it
- 039 — a struct declared twice in the same scope
- 040 — a method and a field chain called on a `ref?` field
- 041 — an unknown type as a parameter, a return type, or an array element
- 042 — `pub` inside struct must be followed by `fn`
- 043 — struct redefinition
- 044 — instance method called via `::` instead of `.`
- 045 — accessing an undefined field
- 046 — calling undefined method on struct
- 047 — destructor must take no parameters
- 048 — destructor must return void
- 049 — `pub fn` called via `.` instead of `::`
- 050 — a struct whose first field is a struct is not circular
- 051 — `pub` may only be used inside a struct

## 001 — declare a struct and a local: named type, alloca, zero-init

```ura
// structs/001.ura - declare a struct and a local: named type, alloca, zero-init

struct Player:
    name char[]
    hp   i32
    mp   i32

main():
    p Player
    output("ok\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Player
├─ name : char[]
├─ hp : i32
└─ mp : i32

fn main() : i32
├─ p : STRUCT_CALL
└─ output : void
   └─ char[] "ok\n"
```

```out
ok
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Player = type { { i8*, i64 }, i32, i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %p = alloca %Player, align 8
  store %Player zeroinitializer, %Player* %p, align 8
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 002 — a field whose type is another struct

```ura
// structs/002.ura - a field whose type is another struct

struct Room:
    name  char[]
    floor i32

struct Dungeon:
    name  char[]
    entry Room

main():
    d Dungeon
    output("ok\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Room
├─ name : char[]
└─ floor : i32

struct Dungeon
├─ name : char[]
└─ entry : STRUCT_CALL

fn main() : i32
├─ d : STRUCT_CALL
└─ output : void
   └─ char[] "ok\n"
```

```out
ok
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Dungeon = type { { i8*, i64 }, %Room }
%Room = type { { i8*, i64 }, i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %d = alloca %Dungeon, align 8
  store %Dungeon zeroinitializer, %Dungeon* %d, align 8
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 003 — forward reference: a field type declared later in the file

```ura
// structs/003.ura - forward reference: a field type declared later in the file

struct Dungeon:
    name  char[]
    entry Room

struct Room:
    name  char[]
    floor i32

main():
    d Dungeon
    output("ok\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Dungeon
├─ name : char[]
└─ entry : STRUCT_CALL

struct Room
├─ name : char[]
└─ floor : i32

fn main() : i32
├─ d : STRUCT_CALL
└─ output : void
   └─ char[] "ok\n"
```

```out
ok
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Dungeon = type { { i8*, i64 }, %Room }
%Room = type { { i8*, i64 }, i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %d = alloca %Dungeon, align 8
  store %Dungeon zeroinitializer, %Dungeon* %d, align 8
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 004 — a struct declared inside a function body

```ura
// structs/004.ura - a struct declared inside a function body

main():
    struct Local:
        x i32
        y i32

    l Local
    output("ok\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

fn main() : i32
├─ struct Local
│  ├─ x : i32
│  └─ y : i32
├─ l : STRUCT_CALL
└─ output : void
   └─ char[] "ok\n"
```

```out
ok
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Local = type { i32, i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %l = alloca %Local, align 8
  store %Local zeroinitializer, %Local* %l, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 005 — a struct declared inside an if block

```ura
// structs/005.ura - a struct declared inside an if block

main():
    n i32 = 1
    if n == 1:
        struct Inner:
            x i32

        i Inner
        output("ok\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

fn main() : i32
├─ = : i32
│  ├─ n : i32
│  └─ int 1
└─ if
   ├─ condition == : bool
   │  ├─ n : i32
   │  └─ int 1
   ├─ struct Inner
   │  └─ x : i32
   ├─ i : STRUCT_CALL
   └─ output : void
      └─ char[] "ok\n"
```

```out
ok
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Inner = type { i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
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
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
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
            n i32

        t Tick
    output("ok\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

fn main() : i32
├─ for
│  ├─ i : i32
│  ├─ range : i32
│  │  ├─ int 0
│  │  └─ int 2
│  ├─ struct Tick
│  │  └─ n : i32
│  └─ t : STRUCT_CALL
└─ output : void
   └─ char[] "ok\n"
```

```out
ok
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Tick = type { i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i1 = load i32, i32* %i, align 4
  %lt = icmp slt i32 %i1, 2
  %gt = icmp sgt i32 %i1, 2
  %more = select i1 true, i1 %lt, i1 %gt
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
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 007 — an array-of-struct field is a fat pointer to the named type

```ura
// structs/007.ura - an array-of-struct field is a fat pointer to the named type

struct Room:
    floor i32

struct Tree:
    label char[]
    kids  Room[]

main():
    t Tree
    output("ok\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Room
└─ floor : i32

struct Tree
├─ label : char[]
└─ kids : STRUCT_CALL[]

fn main() : i32
├─ t : STRUCT_CALL
└─ output : void
   └─ char[] "ok\n"
```

```out
ok
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Tree = type { { i8*, i64 }, { %Room*, i64 } }
%Room = type { i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %t = alloca %Tree, align 8
  store %Tree zeroinitializer, %Tree* %t, align 8
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 008 — a struct nested inside another struct's body

```ura
// structs/008.ura - a struct nested inside another struct's body

struct Outer:
    name char[]
    struct Config:
        debug bool

    hp i32

main():
    o Outer
    output("ok\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Outer
├─ name : char[]
├─ struct Config
│  └─ debug : bool
└─ hp : i32

fn main() : i32
├─ o : STRUCT_CALL
└─ output : void
   └─ char[] "ok\n"
```

```out
ok
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Outer = type { { i8*, i64 }, i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %o = alloca %Outer, align 8
  store %Outer zeroinitializer, %Outer* %o, align 8
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 009 — self reference through an array stays finite

```ura
// structs/009.ura - self reference through an array stays finite

struct Tree:
    label char[]
    kids  Tree[]

main():
    t Tree
    output("ok\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Tree
├─ label : char[]
└─ kids : STRUCT_CALL[]

fn main() : i32
├─ t : STRUCT_CALL
└─ output : void
   └─ char[] "ok\n"
```

```out
ok
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Tree = type { { i8*, i64 }, { %Tree*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %t = alloca %Tree, align 8
  store %Tree zeroinitializer, %Tree* %t, align 8
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 010 — an unused struct emits no type: LLVM drops unreferenced named types

```ura
// structs/010.ura - an unused struct emits no type

struct Unused:
    x i32
    y i32

main():
    output("ok\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Unused
├─ x : i32
└─ y : i32

fn main() : i32
└─ output : void
   └─ char[] "ok\n"
```

```out
ok
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
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
        x i32
        y i32

    n i32 = 1
    if n == 1:
        struct Local:
            z char[]

        l3 Local

    l Local
    output("ok\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Local
├─ a : char
└─ b : char

fn main() : i32
├─ l0 : STRUCT_CALL
├─ struct Local
│  ├─ x : i32
│  └─ y : i32
├─ = : i32
│  ├─ n : i32
│  └─ int 1
├─ if
│  ├─ condition == : bool
│  │  ├─ n : i32
│  │  └─ int 1
│  ├─ struct Local
│  │  └─ z : char[]
│  └─ l3 : STRUCT_CALL
├─ l : STRUCT_CALL
└─ output : void
   └─ char[] "ok\n"
```

```out
ok
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Local.0 = type { i32, i32 }
%Local.1 = type { { i8*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
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
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
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
    name  char[]
    floor i32

main():
    p Room
    p.name  = "armory"
    p.floor = 3
    output(p.name, " on floor ", p.floor, "\n")
    p.floor = p.floor + 10
    output("floor now ", p.floor, "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Room
├─ name : char[]
└─ floor : i32

fn main() : i32
├─ p : STRUCT_CALL
├─ = : array
│  ├─ .name : char[]
│  │  └─ p : STRUCT_CALL
│  └─ char[] "armory"
├─ = : i32
│  ├─ .floor : i32
│  │  └─ p : STRUCT_CALL
│  └─ int 3
├─ output : void
│  ├─ .name : char[]
│  │  └─ p : STRUCT_CALL
│  ├─ char[] " on floor "
│  ├─ .floor : i32
│  │  └─ p : STRUCT_CALL
│  └─ char[] "\n"
├─ = : i32
│  ├─ .floor : i32
│  │  └─ p : STRUCT_CALL
│  └─ + : i32
│     ├─ .floor : i32
│     │  └─ p : STRUCT_CALL
│     └─ int 10
└─ output : void
   ├─ char[] "floor now "
   ├─ .floor : i32
   │  └─ p : STRUCT_CALL
   └─ char[] "\n"
```

```out
armory on floor 3
floor now 13
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Room = type { { i8*, i64 }, i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [7 x i8] c"armory\00", align 1
@str.1 = private unnamed_addr constant [11 x i8] c" on floor \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [15 x i8] c"%.*s%.*s%d%.*s\00", align 1
@str.3 = private unnamed_addr constant [11 x i8] c"floor now \00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.5 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %p = alloca %Room, align 8
  store %Room zeroinitializer, %Room* %p, align 8
  %name = getelementptr %Room, %Room* %p, i32 0, i32 0
  store { i8*, i64 } { i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i64 6 }, { i8*, i64 }* %name, align 8
  %floor = getelementptr %Room, %Room* %p, i32 0, i32 1
  store i32 3, i32* %floor, align 4
  %name1 = getelementptr %Room, %Room* %p, i32 0, i32 0
  %name2 = load { i8*, i64 }, { i8*, i64 }* %name1, align 8
  %str.len = extractvalue { i8*, i64 } %name2, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %name2, 0
  %floor3 = getelementptr %Room, %Room* %p, i32 0, i32 1
  %floor4 = load i32, i32* %floor3, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @fmt, i32 0, i32 0), i32 %len32, i8* %str.data, i32 10, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.1, i32 0, i32 0), i32 %floor4, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %floor5 = getelementptr %Room, %Room* %p, i32 0, i32 1
  %floor6 = getelementptr %Room, %Room* %p, i32 0, i32 1
  %floor7 = load i32, i32* %floor6, align 4
  %add = add i32 %floor7, 10
  store i32 %add, i32* %floor5, align 4
  %floor8 = getelementptr %Room, %Room* %p, i32 0, i32 1
  %floor9 = load i32, i32* %floor8, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.5, i32 0, i32 0), i32 10, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.3, i32 0, i32 0), i32 %floor9, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 013 — nested field access through a struct field

```ura
// structs/013.ura - nested field access through a struct field

struct Room:
    name  char[]
    floor i32

struct Dungeon:
    name  char[]
    entry Room

main():
    d Dungeon
    d.name = "keep"
    d.entry.name  = "gate"
    d.entry.floor = 1
    output(d.name, " entry ", d.entry.name, " floor ", d.entry.floor, "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Room
├─ name : char[]
└─ floor : i32

struct Dungeon
├─ name : char[]
└─ entry : STRUCT_CALL

fn main() : i32
├─ d : STRUCT_CALL
├─ = : array
│  ├─ .name : char[]
│  │  └─ d : STRUCT_CALL
│  └─ char[] "keep"
├─ = : array
│  ├─ .name : char[]
│  │  └─ .entry : STRUCT_CALL
│  │     └─ d : STRUCT_CALL
│  └─ char[] "gate"
├─ = : i32
│  ├─ .floor : i32
│  │  └─ .entry : STRUCT_CALL
│  │     └─ d : STRUCT_CALL
│  └─ int 1
└─ output : void
   ├─ .name : char[]
   │  └─ d : STRUCT_CALL
   ├─ char[] " entry "
   ├─ .name : char[]
   │  └─ .entry : STRUCT_CALL
   │     └─ d : STRUCT_CALL
   ├─ char[] " floor "
   ├─ .floor : i32
   │  └─ .entry : STRUCT_CALL
   │     └─ d : STRUCT_CALL
   └─ char[] "\n"
```

```out
keep entry gate floor 1
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Dungeon = type { { i8*, i64 }, %Room }
%Room = type { { i8*, i64 }, i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [5 x i8] c"keep\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"gate\00", align 1
@str.2 = private unnamed_addr constant [8 x i8] c" entry \00", align 1
@str.3 = private unnamed_addr constant [8 x i8] c" floor \00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [23 x i8] c"%.*s%.*s%.*s%.*s%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %d = alloca %Dungeon, align 8
  store %Dungeon zeroinitializer, %Dungeon* %d, align 8
  %name = getelementptr %Dungeon, %Dungeon* %d, i32 0, i32 0
  store { i8*, i64 } { i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i64 4 }, { i8*, i64 }* %name, align 8
  %entry1 = getelementptr %Dungeon, %Dungeon* %d, i32 0, i32 1
  %name2 = getelementptr %Room, %Room* %entry1, i32 0, i32 0
  store { i8*, i64 } { i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0), i64 4 }, { i8*, i64 }* %name2, align 8
  %entry3 = getelementptr %Dungeon, %Dungeon* %d, i32 0, i32 1
  %floor = getelementptr %Room, %Room* %entry3, i32 0, i32 1
  store i32 1, i32* %floor, align 4
  %name4 = getelementptr %Dungeon, %Dungeon* %d, i32 0, i32 0
  %name5 = load { i8*, i64 }, { i8*, i64 }* %name4, align 8
  %str.len = extractvalue { i8*, i64 } %name5, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %name5, 0
  %entry6 = getelementptr %Dungeon, %Dungeon* %d, i32 0, i32 1
  %name7 = getelementptr %Room, %Room* %entry6, i32 0, i32 0
  %name8 = load { i8*, i64 }, { i8*, i64 }* %name7, align 8
  %str.len9 = extractvalue { i8*, i64 } %name8, 1
  %len3210 = trunc i64 %str.len9 to i32
  %str.data11 = extractvalue { i8*, i64 } %name8, 0
  %entry12 = getelementptr %Dungeon, %Dungeon* %d, i32 0, i32 1
  %floor13 = getelementptr %Room, %Room* %entry12, i32 0, i32 1
  %floor14 = load i32, i32* %floor13, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @fmt, i32 0, i32 0), i32 %len32, i8* %str.data, i32 7, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.2, i32 0, i32 0), i32 %len3210, i8* %str.data11, i32 7, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.3, i32 0, i32 0), i32 %floor14, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 014 — a stack array of structs: Room[N]

```ura
// structs/014.ura - a stack array of structs: Room[N]

struct Room:
    name  char[]
    floor i32

main():
    s Room[] = Room[2]
    s[0].name  = "lobby"
    s[0].floor = 1
    s[1].name  = "cellar"
    output(s[0].name, "@", s[0].floor, " ", s[1].name, "@", s[1].floor, "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Room
├─ name : char[]
└─ floor : i32

fn main() : i32
├─ = : array
│  ├─ s : STRUCT_CALL[]
│  └─ array : STRUCT_CALL[]
│     └─ int 2
├─ = : array
│  ├─ .name : char[]
│  │  └─ index : STRUCT_CALL
│  │     ├─ s : STRUCT_CALL[]
│  │     └─ int 0
│  └─ char[] "lobby"
├─ = : i32
│  ├─ .floor : i32
│  │  └─ index : STRUCT_CALL
│  │     ├─ s : STRUCT_CALL[]
│  │     └─ int 0
│  └─ int 1
├─ = : array
│  ├─ .name : char[]
│  │  └─ index : STRUCT_CALL
│  │     ├─ s : STRUCT_CALL[]
│  │     └─ int 1
│  └─ char[] "cellar"
└─ output : void
   ├─ .name : char[]
   │  └─ index : STRUCT_CALL
   │     ├─ s : STRUCT_CALL[]
   │     └─ int 0
   ├─ char[] "@"
   ├─ .floor : i32
   │  └─ index : STRUCT_CALL
   │     ├─ s : STRUCT_CALL[]
   │     └─ int 0
   ├─ char[] " "
   ├─ .name : char[]
   │  └─ index : STRUCT_CALL
   │     ├─ s : STRUCT_CALL[]
   │     └─ int 1
   ├─ char[] "@"
   ├─ .floor : i32
   │  └─ index : STRUCT_CALL
   │     ├─ s : STRUCT_CALL[]
   │     └─ int 1
   └─ char[] "\n"
```

```out
lobby@1 cellar@0
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Room = type { { i8*, i64 }, i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [6 x i8] c"lobby\00", align 1
@str.1 = private unnamed_addr constant [7 x i8] c"cellar\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"@\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"@\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [29 x i8] c"%.*s%.*s%d%.*s%.*s%.*s%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %s = alloca { %Room*, i64 }, align 8
  %arr = alloca %Room, i64 2, align 8
  %2 = bitcast %Room* %arr to i8*
  call void @llvm.memset.p0i8.i64(i8* %2, i8 0, i64 48, i1 false)
  %arr.ptr = insertvalue { %Room*, i64 } undef, %Room* %arr, 0
  %arr.len = insertvalue { %Room*, i64 } %arr.ptr, i64 2, 1
  store { %Room*, i64 } %arr.len, { %Room*, i64 }* %s, align 8
  %s1 = load { %Room*, i64 }, { %Room*, i64 }* %s, align 8
  %arr.data = extractvalue { %Room*, i64 } %s1, 0
  %arr.at = getelementptr %Room, %Room* %arr.data, i32 0
  %name = getelementptr %Room, %Room* %arr.at, i32 0, i32 0
  store { i8*, i64 } { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i64 5 }, { i8*, i64 }* %name, align 8
  %s2 = load { %Room*, i64 }, { %Room*, i64 }* %s, align 8
  %arr.data3 = extractvalue { %Room*, i64 } %s2, 0
  %arr.at4 = getelementptr %Room, %Room* %arr.data3, i32 0
  %floor = getelementptr %Room, %Room* %arr.at4, i32 0, i32 1
  store i32 1, i32* %floor, align 4
  %s5 = load { %Room*, i64 }, { %Room*, i64 }* %s, align 8
  %arr.data6 = extractvalue { %Room*, i64 } %s5, 0
  %arr.at7 = getelementptr %Room, %Room* %arr.data6, i32 1
  %name8 = getelementptr %Room, %Room* %arr.at7, i32 0, i32 0
  store { i8*, i64 } { i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0), i64 6 }, { i8*, i64 }* %name8, align 8
  %s9 = load { %Room*, i64 }, { %Room*, i64 }* %s, align 8
  %arr.data10 = extractvalue { %Room*, i64 } %s9, 0
  %arr.at11 = getelementptr %Room, %Room* %arr.data10, i32 0
  %name12 = getelementptr %Room, %Room* %arr.at11, i32 0, i32 0
  %name13 = load { i8*, i64 }, { i8*, i64 }* %name12, align 8
  %str.len = extractvalue { i8*, i64 } %name13, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %name13, 0
  %s14 = load { %Room*, i64 }, { %Room*, i64 }* %s, align 8
  %arr.data15 = extractvalue { %Room*, i64 } %s14, 0
  %arr.at16 = getelementptr %Room, %Room* %arr.data15, i32 0
  %floor17 = getelementptr %Room, %Room* %arr.at16, i32 0, i32 1
  %floor18 = load i32, i32* %floor17, align 4
  %s19 = load { %Room*, i64 }, { %Room*, i64 }* %s, align 8
  %arr.data20 = extractvalue { %Room*, i64 } %s19, 0
  %arr.at21 = getelementptr %Room, %Room* %arr.data20, i32 1
  %name22 = getelementptr %Room, %Room* %arr.at21, i32 0, i32 0
  %name23 = load { i8*, i64 }, { i8*, i64 }* %name22, align 8
  %str.len24 = extractvalue { i8*, i64 } %name23, 1
  %len3225 = trunc i64 %str.len24 to i32
  %str.data26 = extractvalue { i8*, i64 } %name23, 0
  %s27 = load { %Room*, i64 }, { %Room*, i64 }* %s, align 8
  %arr.data28 = extractvalue { %Room*, i64 } %s27, 0
  %arr.at29 = getelementptr %Room, %Room* %arr.data28, i32 1
  %floor30 = getelementptr %Room, %Room* %arr.at29, i32 0, i32 1
  %floor31 = load i32, i32* %floor30, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([29 x i8], [29 x i8]* @fmt, i32 0, i32 0), i32 %len32, i8* %str.data, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0), i32 %floor18, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0), i32 %len3225, i8* %str.data26, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0), i32 %floor31, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
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
    name  char[]
    floor i32

main():
    h Room[] = new Room[2]
    h[0].name  = "attic"
    h[0].floor = 9
    h[1].floor = 2
    output(h[0].name, "@", h[0].floor, " ", h[1].floor, "\n")
    clean h
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Room
├─ name : char[]
└─ floor : i32

fn main() : i32
├─ = : array
│  ├─ h : STRUCT_CALL[]
│  └─ array : STRUCT_CALL[]
│     └─ int 2
├─ = : array
│  ├─ .name : char[]
│  │  └─ index : STRUCT_CALL
│  │     ├─ h : STRUCT_CALL[]
│  │     └─ int 0
│  └─ char[] "attic"
├─ = : i32
│  ├─ .floor : i32
│  │  └─ index : STRUCT_CALL
│  │     ├─ h : STRUCT_CALL[]
│  │     └─ int 0
│  └─ int 9
├─ = : i32
│  ├─ .floor : i32
│  │  └─ index : STRUCT_CALL
│  │     ├─ h : STRUCT_CALL[]
│  │     └─ int 1
│  └─ int 2
├─ output : void
│  ├─ .name : char[]
│  │  └─ index : STRUCT_CALL
│  │     ├─ h : STRUCT_CALL[]
│  │     └─ int 0
│  ├─ char[] "@"
│  ├─ .floor : i32
│  │  └─ index : STRUCT_CALL
│  │     ├─ h : STRUCT_CALL[]
│  │     └─ int 0
│  ├─ char[] " "
│  ├─ .floor : i32
│  │  └─ index : STRUCT_CALL
│  │     ├─ h : STRUCT_CALL[]
│  │     └─ int 1
│  └─ char[] "\n"
└─ clean : void
   └─ h : STRUCT_CALL[]
```

```out
attic@9 2
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Room = type { { i8*, i64 }, i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [6 x i8] c"attic\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"@\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [21 x i8] c"%.*s%.*s%d%.*s%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %h = alloca { %Room*, i64 }, align 8
  %heap = call i8* @calloc(i64 2, i64 24)
  %arr = bitcast i8* %heap to %Room*
  %arr.ptr = insertvalue { %Room*, i64 } undef, %Room* %arr, 0
  %arr.len = insertvalue { %Room*, i64 } %arr.ptr, i64 2, 1
  store { %Room*, i64 } %arr.len, { %Room*, i64 }* %h, align 8
  %h1 = load { %Room*, i64 }, { %Room*, i64 }* %h, align 8
  %arr.data = extractvalue { %Room*, i64 } %h1, 0
  %arr.at = getelementptr %Room, %Room* %arr.data, i32 0
  %name = getelementptr %Room, %Room* %arr.at, i32 0, i32 0
  store { i8*, i64 } { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i64 5 }, { i8*, i64 }* %name, align 8
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
  %name13 = load { i8*, i64 }, { i8*, i64 }* %name12, align 8
  %str.len = extractvalue { i8*, i64 } %name13, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %name13, 0
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
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @fmt, i32 0, i32 0), i32 %len32, i8* %str.data, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i32 %floor18, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0), i32 %floor23, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
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
    name  char[]
    floor i32

main():
    g Room[][] = Room[2][1]
    g[0][0].name  = "grid-a"
    g[0][0].floor = 3
    g[1][0].name  = "grid-b"
    g[1][0].floor = 4
    output(g[0][0].name, "@", g[0][0].floor, " ", g[1][0].name, "@", g[1][0].floor, "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Room
├─ name : char[]
└─ floor : i32

fn main() : i32
├─ = : array
│  ├─ g : STRUCT_CALL[][]
│  └─ array : STRUCT_CALL[][]
│     ├─ int 2
│     └─ int 1
├─ = : array
│  ├─ .name : char[]
│  │  └─ index : STRUCT_CALL
│  │     ├─ index : STRUCT_CALL[]
│  │     │  ├─ g : STRUCT_CALL[][]
│  │     │  └─ int 0
│  │     └─ int 0
│  └─ char[] "grid-a"
├─ = : i32
│  ├─ .floor : i32
│  │  └─ index : STRUCT_CALL
│  │     ├─ index : STRUCT_CALL[]
│  │     │  ├─ g : STRUCT_CALL[][]
│  │     │  └─ int 0
│  │     └─ int 0
│  └─ int 3
├─ = : array
│  ├─ .name : char[]
│  │  └─ index : STRUCT_CALL
│  │     ├─ index : STRUCT_CALL[]
│  │     │  ├─ g : STRUCT_CALL[][]
│  │     │  └─ int 1
│  │     └─ int 0
│  └─ char[] "grid-b"
├─ = : i32
│  ├─ .floor : i32
│  │  └─ index : STRUCT_CALL
│  │     ├─ index : STRUCT_CALL[]
│  │     │  ├─ g : STRUCT_CALL[][]
│  │     │  └─ int 1
│  │     └─ int 0
│  └─ int 4
└─ output : void
   ├─ .name : char[]
   │  └─ index : STRUCT_CALL
   │     ├─ index : STRUCT_CALL[]
   │     │  ├─ g : STRUCT_CALL[][]
   │     │  └─ int 0
   │     └─ int 0
   ├─ char[] "@"
   ├─ .floor : i32
   │  └─ index : STRUCT_CALL
   │     ├─ index : STRUCT_CALL[]
   │     │  ├─ g : STRUCT_CALL[][]
   │     │  └─ int 0
   │     └─ int 0
   ├─ char[] " "
   ├─ .name : char[]
   │  └─ index : STRUCT_CALL
   │     ├─ index : STRUCT_CALL[]
   │     │  ├─ g : STRUCT_CALL[][]
   │     │  └─ int 1
   │     └─ int 0
   ├─ char[] "@"
   ├─ .floor : i32
   │  └─ index : STRUCT_CALL
   │     ├─ index : STRUCT_CALL[]
   │     │  ├─ g : STRUCT_CALL[][]
   │     │  └─ int 1
   │     └─ int 0
   └─ char[] "\n"
```

```out
grid-a@3 grid-b@4
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Room = type { { i8*, i64 }, i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [7 x i8] c"grid-a\00", align 1
@str.1 = private unnamed_addr constant [7 x i8] c"grid-b\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"@\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"@\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [29 x i8] c"%.*s%.*s%d%.*s%.*s%.*s%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
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
  %2 = bitcast %Room* %arr2 to i8*
  call void @llvm.memset.p0i8.i64(i8* %2, i8 0, i64 24, i1 false)
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
  store { i8*, i64 } { i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i64 6 }, { i8*, i64 }* %name, align 8
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
  store { i8*, i64 } { i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0), i64 6 }, { i8*, i64 }* %name21, align 8
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
  %name36 = load { i8*, i64 }, { i8*, i64 }* %name35, align 8
  %str.len = extractvalue { i8*, i64 } %name36, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %name36, 0
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
  %name52 = load { i8*, i64 }, { i8*, i64 }* %name51, align 8
  %str.len53 = extractvalue { i8*, i64 } %name52, 1
  %len3254 = trunc i64 %str.len53 to i32
  %str.data55 = extractvalue { i8*, i64 } %name52, 0
  %g56 = load { { %Room*, i64 }*, i64 }, { { %Room*, i64 }*, i64 }* %g, align 8
  %arr.data57 = extractvalue { { %Room*, i64 }*, i64 } %g56, 0
  %arr.at58 = getelementptr { %Room*, i64 }, { %Room*, i64 }* %arr.data57, i32 1
  %idx59 = load { %Room*, i64 }, { %Room*, i64 }* %arr.at58, align 8
  %arr.data60 = extractvalue { %Room*, i64 } %idx59, 0
  %arr.at61 = getelementptr %Room, %Room* %arr.data60, i32 0
  %floor62 = getelementptr %Room, %Room* %arr.at61, i32 0, i32 1
  %floor63 = load i32, i32* %floor62, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([29 x i8], [29 x i8]* @fmt, i32 0, i32 0), i32 %len32, i8* %str.data, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0), i32 %floor44, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0), i32 %len3254, i8* %str.data55, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0), i32 %floor63, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
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
    floor i32

main():
    g Room[][] = new Room[2][2]
    g[0][0].floor = 1
    g[1][1].floor = 4
    output(g[0][0].floor, " ", g[1][1].floor, "\n")
    clean g
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Room
└─ floor : i32

fn main() : i32
├─ = : array
│  ├─ g : STRUCT_CALL[][]
│  └─ array : STRUCT_CALL[][]
│     ├─ int 2
│     └─ int 2
├─ = : i32
│  ├─ .floor : i32
│  │  └─ index : STRUCT_CALL
│  │     ├─ index : STRUCT_CALL[]
│  │     │  ├─ g : STRUCT_CALL[][]
│  │     │  └─ int 0
│  │     └─ int 0
│  └─ int 1
├─ = : i32
│  ├─ .floor : i32
│  │  └─ index : STRUCT_CALL
│  │     ├─ index : STRUCT_CALL[]
│  │     │  ├─ g : STRUCT_CALL[][]
│  │     │  └─ int 1
│  │     └─ int 1
│  └─ int 4
├─ output : void
│  ├─ .floor : i32
│  │  └─ index : STRUCT_CALL
│  │     ├─ index : STRUCT_CALL[]
│  │     │  ├─ g : STRUCT_CALL[][]
│  │     │  └─ int 0
│  │     └─ int 0
│  ├─ char[] " "
│  ├─ .floor : i32
│  │  └─ index : STRUCT_CALL
│  │     ├─ index : STRUCT_CALL[]
│  │     │  ├─ g : STRUCT_CALL[][]
│  │     │  └─ int 1
│  │     └─ int 1
│  └─ char[] "\n"
└─ clean : void
   └─ g : STRUCT_CALL[][]
```

```out
1 4
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Room = type { i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [13 x i8] c"%d%.*s%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
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
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i32 %floor24, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %floor32, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
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
    floor i32

main():
    s Room[]   = Room[2]
    g Room[][] = Room[4][1]
    output(s.len, " ", g.len, "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Room
└─ floor : i32

fn main() : i32
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
   ├─ .len : u64
   │  └─ s : STRUCT_CALL[]
   ├─ char[] " "
   ├─ .len : u64
   │  └─ g : STRUCT_CALL[][]
   └─ char[] "\n"
```

```out
2 4
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Room = type { i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [17 x i8] c"%llu%.*s%llu%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %s = alloca { %Room*, i64 }, align 8
  %arr = alloca %Room, i64 2, align 8
  %2 = bitcast %Room* %arr to i8*
  call void @llvm.memset.p0i8.i64(i8* %2, i8 0, i64 8, i1 false)
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
  %3 = bitcast %Room* %arr3 to i8*
  call void @llvm.memset.p0i8.i64(i8* %3, i8 0, i64 4, i1 false)
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
  %g10 = load { { %Room*, i64 }*, i64 }, { { %Room*, i64 }*, i64 }* %g, align 8
  %len11 = extractvalue { { %Room*, i64 }*, i64 } %g10, 1
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt, i32 0, i32 0), i64 %len, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i64 %len11, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
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
    stack i32 = 5
    heap  i32 = 7
    output(stack, " ", heap, "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

fn main() : i32
├─ = : i32
│  ├─ stack : i32
│  └─ int 5
├─ = : i32
│  ├─ heap : i32
│  └─ int 7
└─ output : void
   ├─ stack : i32
   ├─ char[] " "
   ├─ heap : i32
   └─ char[] "\n"
```

```out
5 7
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [13 x i8] c"%d%.*s%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %stack = alloca i32, align 4
  store i32 5, i32* %stack, align 4
  %heap = alloca i32, align 4
  store i32 7, i32* %heap, align 4
  %stack1 = load i32, i32* %stack, align 4
  %heap2 = load i32, i32* %heap, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i32 %stack1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %heap2, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 020 — a 'ref?' field may refer to its own struct

```ura
// structs/020.ura - a 'ref?' field may refer to its own struct

struct Node:
    value i32
    ref? next Node

main():
    a Node
    a.value = 1
    output(a.value, "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Node
├─ value : i32
└─ next : STRUCT_CALL

fn main() : i32
├─ a : STRUCT_CALL
├─ = : i32
│  ├─ .value : i32
│  │  └─ a : STRUCT_CALL
│  └─ int 1
└─ output : void
   ├─ .value : i32
   │  └─ a : STRUCT_CALL
   └─ char[] "\n"
```

```out
1
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Node = type { i32, %Node* }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %a = alloca %Node, align 8
  store %Node zeroinitializer, %Node* %a, align 8
  %value = getelementptr %Node, %Node* %a, i32 0, i32 0
  store i32 1, i32* %value, align 4
  %value1 = getelementptr %Node, %Node* %a, i32 0, i32 0
  %value2 = load i32, i32* %value1, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %value2, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 021 — bind, read and write through a 'ref?' field

```ura
// structs/021.ura - bind, read and write through a 'ref?' field

struct Node:
    value i32
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
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Node
├─ value : i32
└─ next : STRUCT_CALL

fn main() : i32
├─ a : STRUCT_CALL
├─ b : STRUCT_CALL
├─ c : STRUCT_CALL
├─ = : i32
│  ├─ .value : i32
│  │  └─ a : STRUCT_CALL
│  └─ int 1
├─ = : i32
│  ├─ .value : i32
│  │  └─ b : STRUCT_CALL
│  └─ int 2
├─ = : i32
│  ├─ .value : i32
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
├─ = : i32
│  ├─ .value : i32
│  │  └─ .next : STRUCT_CALL
│  │     └─ a : STRUCT_CALL
│  └─ int 20
└─ output : void
   ├─ .value : i32
   │  └─ .next : STRUCT_CALL
   │     └─ a : STRUCT_CALL
   ├─ char[] " "
   ├─ .value : i32
   │  └─ .next : STRUCT_CALL
   │     └─ .next : STRUCT_CALL
   │        └─ a : STRUCT_CALL
   ├─ char[] " "
   ├─ .value : i32
   │  └─ b : STRUCT_CALL
   └─ char[] "\n"
```

```out
20 3 20
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Node = type { i32, %Node* }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [19 x i8] c"%d%.*s%d%.*s%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
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
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @fmt, i32 0, i32 0), i32 %value9, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %value15, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i32 %value17, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 022 — struct assignment copies by value

```ura
// structs/022.ura - struct assignment copies by value

struct Room:
    name  char[]
    floor i32

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
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Room
├─ name : char[]
└─ floor : i32

fn main() : i32
├─ a : STRUCT_CALL
├─ = : array
│  ├─ .name : char[]
│  │  └─ a : STRUCT_CALL
│  └─ char[] "hall"
├─ = : i32
│  ├─ .floor : i32
│  │  └─ a : STRUCT_CALL
│  └─ int 2
├─ b : STRUCT_CALL
├─ = : STRUCT_CALL
│  ├─ b : STRUCT_CALL
│  └─ a : STRUCT_CALL
├─ = : i32
│  ├─ .floor : i32
│  │  └─ b : STRUCT_CALL
│  └─ int 9
└─ output : void
   ├─ .name : char[]
   │  └─ b : STRUCT_CALL
   ├─ char[] " "
   ├─ .floor : i32
   │  └─ b : STRUCT_CALL
   ├─ char[] " / "
   ├─ .name : char[]
   │  └─ a : STRUCT_CALL
   ├─ char[] " "
   ├─ .floor : i32
   │  └─ a : STRUCT_CALL
   └─ char[] "\n"
```

```out
hall 9 / hall 2
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Room = type { { i8*, i64 }, i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [5 x i8] c"hall\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c" / \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [29 x i8] c"%.*s%.*s%d%.*s%.*s%.*s%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %a = alloca %Room, align 8
  store %Room zeroinitializer, %Room* %a, align 8
  %name = getelementptr %Room, %Room* %a, i32 0, i32 0
  store { i8*, i64 } { i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i64 4 }, { i8*, i64 }* %name, align 8
  %floor = getelementptr %Room, %Room* %a, i32 0, i32 1
  store i32 2, i32* %floor, align 4
  %b = alloca %Room, align 8
  store %Room zeroinitializer, %Room* %b, align 8
  %a1 = load %Room, %Room* %a, align 8
  store %Room %a1, %Room* %b, align 8
  %floor2 = getelementptr %Room, %Room* %b, i32 0, i32 1
  store i32 9, i32* %floor2, align 4
  %name3 = getelementptr %Room, %Room* %b, i32 0, i32 0
  %name4 = load { i8*, i64 }, { i8*, i64 }* %name3, align 8
  %str.len = extractvalue { i8*, i64 } %name4, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %name4, 0
  %floor5 = getelementptr %Room, %Room* %b, i32 0, i32 1
  %floor6 = load i32, i32* %floor5, align 4
  %name7 = getelementptr %Room, %Room* %a, i32 0, i32 0
  %name8 = load { i8*, i64 }, { i8*, i64 }* %name7, align 8
  %str.len9 = extractvalue { i8*, i64 } %name8, 1
  %len3210 = trunc i64 %str.len9 to i32
  %str.data11 = extractvalue { i8*, i64 } %name8, 0
  %floor12 = getelementptr %Room, %Room* %a, i32 0, i32 1
  %floor13 = load i32, i32* %floor12, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([29 x i8], [29 x i8]* @fmt, i32 0, i32 0), i32 %len32, i8* %str.data, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i32 %floor6, i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 %len3210, i8* %str.data11, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0), i32 %floor13, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 023 — a struct as a by-value parameter

```ura
// structs/023.ura - a struct as a by-value parameter

struct Room:
    name  char[]
    floor i32

fn show(r Room) void:
    output("room ", r.name, "@", r.floor, "\n")

main():
    a Room
    a.name  = "hall"
    a.floor = 2
    show(a)
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Room
├─ name : char[]
└─ floor : i32

fn show(r : STRUCT_CALL) : void
└─ output : void
   ├─ char[] "room "
   ├─ .name : char[]
   │  └─ r : STRUCT_CALL
   ├─ char[] "@"
   ├─ .floor : i32
   │  └─ r : STRUCT_CALL
   └─ char[] "\n"

fn main() : i32
├─ a : STRUCT_CALL
├─ = : array
│  ├─ .name : char[]
│  │  └─ a : STRUCT_CALL
│  └─ char[] "hall"
├─ = : i32
│  ├─ .floor : i32
│  │  └─ a : STRUCT_CALL
│  └─ int 2
└─ call show : void
   └─ a : STRUCT_CALL
```

```out
room hall@2
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Room = type { { i8*, i64 }, i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [6 x i8] c"room \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"@\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [19 x i8] c"%.*s%.*s%.*s%d%.*s\00", align 1
@str.3 = private unnamed_addr constant [5 x i8] c"hall\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define void @show(%Room %0) {
entry:
  %r = alloca %Room, align 8
  store %Room %0, %Room* %r, align 8
  %name = getelementptr %Room, %Room* %r, i32 0, i32 0
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %str.len = extractvalue { i8*, i64 } %name1, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %name1, 0
  %floor = getelementptr %Room, %Room* %r, i32 0, i32 1
  %floor2 = load i32, i32* %floor, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @fmt, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %len32, i8* %str.data, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i32 %floor2, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)

define i32 @main(i32 %0, i8** %1) {
entry:
  %a = alloca %Room, align 8
  store %Room zeroinitializer, %Room* %a, align 8
  %name = getelementptr %Room, %Room* %a, i32 0, i32 0
  store { i8*, i64 } { i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.3, i32 0, i32 0), i64 4 }, { i8*, i64 }* %name, align 8
  %floor = getelementptr %Room, %Room* %a, i32 0, i32 1
  store i32 2, i32* %floor, align 4
  %a1 = load %Room, %Room* %a, align 8
  call void @show(%Room %a1)
  ret i32 0
}
```

## 024 — a 'ref' struct parameter mutates the caller's struct

```ura
// structs/024.ura - a 'ref' struct parameter mutates the caller's struct

struct Room:
    name  char[]
    floor i32

fn bump(ref r Room) void:
    r.floor = r.floor + 1

main():
    a Room
    a.floor = 2
    bump(ref a)
    output("after ", a.floor, "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Room
├─ name : char[]
└─ floor : i32

fn bump(r : STRUCT_CALL) : void
└─ = : i32
   ├─ .floor : i32
   │  └─ r : STRUCT_CALL
   └─ + : i32
      ├─ .floor : i32
      │  └─ r : STRUCT_CALL
      └─ int 1

fn main() : i32
├─ a : STRUCT_CALL
├─ = : i32
│  ├─ .floor : i32
│  │  └─ a : STRUCT_CALL
│  └─ int 2
├─ call bump : void
│  └─ ref : STRUCT_CALL
│     └─ a : STRUCT_CALL
└─ output : void
   ├─ char[] "after "
   ├─ .floor : i32
   │  └─ a : STRUCT_CALL
   └─ char[] "\n"
```

```out
after 3
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Room = type { { i8*, i64 }, i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [7 x i8] c"after \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define void @bump(%Room* %0) {
entry:
  %r = alloca %Room*, align 8
  store %Room* %0, %Room** %r, align 8
  %ref = load %Room*, %Room** %r, align 8
  %floor = getelementptr %Room, %Room* %ref, i32 0, i32 1
  %ref1 = load %Room*, %Room** %r, align 8
  %floor2 = getelementptr %Room, %Room* %ref1, i32 0, i32 1
  %floor3 = load i32, i32* %floor2, align 4
  %add = add i32 %floor3, 1
  store i32 %add, i32* %floor, align 4
  ret void
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %a = alloca %Room, align 8
  store %Room zeroinitializer, %Room* %a, align 8
  %floor = getelementptr %Room, %Room* %a, i32 0, i32 1
  store i32 2, i32* %floor, align 4
  call void @bump(%Room* %a)
  %floor1 = getelementptr %Room, %Room* %a, i32 0, i32 1
  %floor2 = load i32, i32* %floor1, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %floor2, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 025 — returning a struct: the create() constructor convention

```ura
// structs/025.ura - returning a struct: the create() constructor convention

struct Room:
    name  char[]
    floor i32

fn create(f i32) Room:
    r Room
    r.name  = "made"
    r.floor = f
    return r

fn describe(r Room) char[]:
    return r.name

main():
    a Room = create(4)
    output(describe(a), " ", a.floor, "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Room
├─ name : char[]
└─ floor : i32

fn create(f : i32) : STRUCT_CALL
├─ r : STRUCT_CALL
├─ = : array
│  ├─ .name : char[]
│  │  └─ r : STRUCT_CALL
│  └─ char[] "made"
├─ = : i32
│  ├─ .floor : i32
│  │  └─ r : STRUCT_CALL
│  └─ f : i32
└─ return
   └─ r : STRUCT_CALL

fn describe(r : STRUCT_CALL) : char[]
└─ return
   └─ .name : char[]
      └─ r : STRUCT_CALL

fn main() : i32
├─ = : STRUCT_CALL
│  ├─ a : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     └─ int 4
└─ output : void
   ├─ call describe : char[]
   │  └─ a : STRUCT_CALL
   ├─ char[] " "
   ├─ .floor : i32
   │  └─ a : STRUCT_CALL
   └─ char[] "\n"
```

```out
made 4
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Room = type { { i8*, i64 }, i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [5 x i8] c"made\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [15 x i8] c"%.*s%.*s%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define %Room @create(i32 %0) {
entry:
  %f = alloca i32, align 4
  store i32 %0, i32* %f, align 4
  %r = alloca %Room, align 8
  store %Room zeroinitializer, %Room* %r, align 8
  %name = getelementptr %Room, %Room* %r, i32 0, i32 0
  store { i8*, i64 } { i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i64 4 }, { i8*, i64 }* %name, align 8
  %floor = getelementptr %Room, %Room* %r, i32 0, i32 1
  %f1 = load i32, i32* %f, align 4
  store i32 %f1, i32* %floor, align 4
  %r2 = load %Room, %Room* %r, align 8
  ret %Room %r2
}

define { i8*, i64 } @describe(%Room %0) {
entry:
  %r = alloca %Room, align 8
  store %Room %0, %Room* %r, align 8
  %name = getelementptr %Room, %Room* %r, i32 0, i32 0
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  ret { i8*, i64 } %name1
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %a = alloca %Room, align 8
  %call = call %Room @create(i32 4)
  store %Room %call, %Room* %a, align 8
  %a1 = load %Room, %Room* %a, align 8
  %call2 = call { i8*, i64 } @describe(%Room %a1)
  %str.len = extractvalue { i8*, i64 } %call2, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %call2, 0
  %floor = getelementptr %Room, %Room* %a, i32 0, i32 1
  %floor3 = load i32, i32* %floor, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @fmt, i32 0, i32 0), i32 %len32, i8* %str.data, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i32 %floor3, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 026 — output() prints a struct and its nested struct

```ura
// structs/026.ura - output() prints a struct and its nested struct

struct Room:
    name  char[]
    floor i32

struct Dungeon:
    name  char[]
    entry Room

main():
    r Room
    r.name  = "hall"
    r.floor = 2
    output("flat   ", r, "\n")

    d Dungeon
    d.name = "keep"
    d.entry.name  = "gate"
    d.entry.floor = 1
    output("nested ", d, "\n")

    output("two: ", r, " and ", d.entry, " done\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Room
├─ name : char[]
└─ floor : i32

struct Dungeon
├─ name : char[]
└─ entry : STRUCT_CALL

fn main() : i32
├─ r : STRUCT_CALL
├─ = : array
│  ├─ .name : char[]
│  │  └─ r : STRUCT_CALL
│  └─ char[] "hall"
├─ = : i32
│  ├─ .floor : i32
│  │  └─ r : STRUCT_CALL
│  └─ int 2
├─ output : void
│  ├─ char[] "flat   "
│  ├─ r : STRUCT_CALL
│  └─ char[] "\n"
├─ d : STRUCT_CALL
├─ = : array
│  ├─ .name : char[]
│  │  └─ d : STRUCT_CALL
│  └─ char[] "keep"
├─ = : array
│  ├─ .name : char[]
│  │  └─ .entry : STRUCT_CALL
│  │     └─ d : STRUCT_CALL
│  └─ char[] "gate"
├─ = : i32
│  ├─ .floor : i32
│  │  └─ .entry : STRUCT_CALL
│  │     └─ d : STRUCT_CALL
│  └─ int 1
├─ output : void
│  ├─ char[] "nested "
│  ├─ d : STRUCT_CALL
│  └─ char[] "\n"
└─ output : void
   ├─ char[] "two: "
   ├─ r : STRUCT_CALL
   ├─ char[] " and "
   ├─ .entry : STRUCT_CALL
   │  └─ d : STRUCT_CALL
   └─ char[] " done\n"
```

```out
flat   Room{name: hall, floor: 2}
nested Dungeon{name: keep, entry: Room{name: gate, floor: 1}}
two: Room{name: hall, floor: 2} and Room{name: gate, floor: 1} done
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Room = type { { i8*, i64 }, i32 }
%__out_frame = type { i8*, i32, %__out_frame* }
%Dungeon = type { { i8*, i64 }, %Room }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [5 x i8] c"hall\00", align 1
@str.1 = private unnamed_addr constant [8 x i8] c"flat   \00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.2 = private unnamed_addr constant [11 x i8] c"[Circular]\00", align 1
@fmt.3 = private unnamed_addr constant [6 x i8] c"Room{\00", align 1
@fmt.4 = private unnamed_addr constant [7 x i8] c"name: \00", align 1
@fmt.5 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.6 = private unnamed_addr constant [10 x i8] c", floor: \00", align 1
@fmt.7 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@fmt.8 = private unnamed_addr constant [2 x i8] c"}\00", align 1
@str.9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.10 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.11 = private unnamed_addr constant [5 x i8] c"keep\00", align 1
@str.12 = private unnamed_addr constant [5 x i8] c"gate\00", align 1
@str.13 = private unnamed_addr constant [8 x i8] c"nested \00", align 1
@fmt.14 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.15 = private unnamed_addr constant [11 x i8] c"[Circular]\00", align 1
@fmt.16 = private unnamed_addr constant [9 x i8] c"Dungeon{\00", align 1
@fmt.17 = private unnamed_addr constant [7 x i8] c"name: \00", align 1
@fmt.18 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.19 = private unnamed_addr constant [10 x i8] c", entry: \00", align 1
@fmt.20 = private unnamed_addr constant [2 x i8] c"}\00", align 1
@str.21 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.22 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.23 = private unnamed_addr constant [6 x i8] c"two: \00", align 1
@fmt.24 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.25 = private unnamed_addr constant [6 x i8] c" and \00", align 1
@fmt.26 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.27 = private unnamed_addr constant [7 x i8] c" done\0A\00", align 1
@fmt.28 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %r = alloca %Room, align 8
  store %Room zeroinitializer, %Room* %r, align 8
  %name = getelementptr %Room, %Room* %r, i32 0, i32 0
  store { i8*, i64 } { i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i64 4 }, { i8*, i64 }* %name, align 8
  %floor = getelementptr %Room, %Room* %r, i32 0, i32 1
  store i32 2, i32* %floor, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 7, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.1, i32 0, i32 0))
  call void @__out_Room(%Room* %r, %__out_frame* null)
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.10, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.9, i32 0, i32 0))
  %d = alloca %Dungeon, align 8
  store %Dungeon zeroinitializer, %Dungeon* %d, align 8
  %name1 = getelementptr %Dungeon, %Dungeon* %d, i32 0, i32 0
  store { i8*, i64 } { i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.11, i32 0, i32 0), i64 4 }, { i8*, i64 }* %name1, align 8
  %entry2 = getelementptr %Dungeon, %Dungeon* %d, i32 0, i32 1
  %name3 = getelementptr %Room, %Room* %entry2, i32 0, i32 0
  store { i8*, i64 } { i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.12, i32 0, i32 0), i64 4 }, { i8*, i64 }* %name3, align 8
  %entry4 = getelementptr %Dungeon, %Dungeon* %d, i32 0, i32 1
  %floor5 = getelementptr %Room, %Room* %entry4, i32 0, i32 1
  store i32 1, i32* %floor5, align 4
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.14, i32 0, i32 0), i32 7, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.13, i32 0, i32 0))
  call void @__out_Dungeon(%Dungeon* %d, %__out_frame* null)
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.22, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.21, i32 0, i32 0))
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.24, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.23, i32 0, i32 0))
  call void @__out_Room(%Room* %r, %__out_frame* null)
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.26, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.25, i32 0, i32 0))
  %entry6 = getelementptr %Dungeon, %Dungeon* %d, i32 0, i32 1
  call void @__out_Room(%Room* %entry6, %__out_frame* null)
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.28, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.27, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)

define void @__out_Room(%Room* %0, %__out_frame* %1) {
entry:
  %me = bitcast %Room* %0 to i8*
  %walk = alloca %__out_frame*, align 8
  store %__out_frame* %1, %__out_frame** %walk, align 8
  br label %seen.cond

seen.cond:                                        ; preds = %seen.next, %entry
  %q = load %__out_frame*, %__out_frame** %walk, align 8
  %q2i = ptrtoint %__out_frame* %q to i64
  %atroot = icmp eq i64 %q2i, 0
  br i1 %atroot, label %seen.fresh, label %seen.body

seen.body:                                        ; preds = %seen.cond
  %q.ptr = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 0
  %held = load i8*, i8** %q.ptr, align 8
  %q.ty = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 1
  %heldty = load i32, i32* %q.ty, align 4
  %h2i = ptrtoint i8* %held to i64
  %m2i = ptrtoint i8* %me to i64
  %sameptr = icmp eq i64 %h2i, %m2i
  %samety = icmp eq i32 %heldty, 1
  %same = and i1 %sameptr, %samety
  br i1 %same, label %seen.hit, label %seen.next

seen.hit:                                         ; preds = %seen.body
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.2, i32 0, i32 0))
  ret void

seen.next:                                        ; preds = %seen.body
  %q.prev = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 2
  %up = load %__out_frame*, %__out_frame** %q.prev, align 8
  store %__out_frame* %up, %__out_frame** %walk, align 8
  br label %seen.cond

seen.fresh:                                       ; preds = %seen.cond
  %frame = alloca %__out_frame, align 8
  %f.ptr = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 0
  store i8* %me, i8** %f.ptr, align 8
  %f.ty = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 1
  store i32 1, i32* %f.ty, align 4
  %f.prev = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 2
  store %__out_frame* %1, %__out_frame** %f.prev, align 8
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @fmt.3, i32 0, i32 0))
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.4, i32 0, i32 0))
  %name = getelementptr %Room, %Room* %0, i32 0, i32 0
  %arr = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %arr, 0
  %arr.len = extractvalue { i8*, i64 } %arr, 1
  %len32 = trunc i64 %arr.len to i32
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.5, i32 0, i32 0), i32 %len32, i8* %arr.data)
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @fmt.6, i32 0, i32 0))
  %floor = getelementptr %Room, %Room* %0, i32 0, i32 1
  %f = load i32, i32* %floor, align 4
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.7, i32 0, i32 0), i32 %f)
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @fmt.8, i32 0, i32 0))
  ret void
}

define void @__out_Dungeon(%Dungeon* %0, %__out_frame* %1) {
entry:
  %me = bitcast %Dungeon* %0 to i8*
  %walk = alloca %__out_frame*, align 8
  store %__out_frame* %1, %__out_frame** %walk, align 8
  br label %seen.cond

seen.cond:                                        ; preds = %seen.next, %entry
  %q = load %__out_frame*, %__out_frame** %walk, align 8
  %q2i = ptrtoint %__out_frame* %q to i64
  %atroot = icmp eq i64 %q2i, 0
  br i1 %atroot, label %seen.fresh, label %seen.body

seen.body:                                        ; preds = %seen.cond
  %q.ptr = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 0
  %held = load i8*, i8** %q.ptr, align 8
  %q.ty = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 1
  %heldty = load i32, i32* %q.ty, align 4
  %h2i = ptrtoint i8* %held to i64
  %m2i = ptrtoint i8* %me to i64
  %sameptr = icmp eq i64 %h2i, %m2i
  %samety = icmp eq i32 %heldty, 2
  %same = and i1 %sameptr, %samety
  br i1 %same, label %seen.hit, label %seen.next

seen.hit:                                         ; preds = %seen.body
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.15, i32 0, i32 0))
  ret void

seen.next:                                        ; preds = %seen.body
  %q.prev = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 2
  %up = load %__out_frame*, %__out_frame** %q.prev, align 8
  store %__out_frame* %up, %__out_frame** %walk, align 8
  br label %seen.cond

seen.fresh:                                       ; preds = %seen.cond
  %frame = alloca %__out_frame, align 8
  %f.ptr = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 0
  store i8* %me, i8** %f.ptr, align 8
  %f.ty = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 1
  store i32 2, i32* %f.ty, align 4
  %f.prev = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 2
  store %__out_frame* %1, %__out_frame** %f.prev, align 8
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.16, i32 0, i32 0))
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.17, i32 0, i32 0))
  %name = getelementptr %Dungeon, %Dungeon* %0, i32 0, i32 0
  %arr = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %arr, 0
  %arr.len = extractvalue { i8*, i64 } %arr, 1
  %len32 = trunc i64 %arr.len to i32
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.18, i32 0, i32 0), i32 %len32, i8* %arr.data)
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @fmt.19, i32 0, i32 0))
  %entry1 = getelementptr %Dungeon, %Dungeon* %0, i32 0, i32 1
  call void @__out_Room(%Room* %entry1, %__out_frame* %frame)
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @fmt.20, i32 0, i32 0))
  ret void
}
```

## 027 — output() follows a bound 'ref?' and prints null when unbound

```ura
// structs/027.ura - output() follows a bound 'ref?' and prints null when unbound

struct Node:
    value i32
    ref? next Node

main():
    a Node
    a.value = 1
    output("unbound ", a, "\n")

    b Node
    b.value = 2
    a.next = ref b
    output("bound   ", a, "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Node
├─ value : i32
└─ next : STRUCT_CALL

fn main() : i32
├─ a : STRUCT_CALL
├─ = : i32
│  ├─ .value : i32
│  │  └─ a : STRUCT_CALL
│  └─ int 1
├─ output : void
│  ├─ char[] "unbound "
│  ├─ a : STRUCT_CALL
│  └─ char[] "\n"
├─ b : STRUCT_CALL
├─ = : i32
│  ├─ .value : i32
│  │  └─ b : STRUCT_CALL
│  └─ int 2
├─ = : STRUCT_CALL
│  ├─ .next : STRUCT_CALL
│  │  └─ a : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ b : STRUCT_CALL
└─ output : void
   ├─ char[] "bound   "
   ├─ a : STRUCT_CALL
   └─ char[] "\n"
```

```out
unbound Node{value: 1, next: null}
bound   Node{value: 1, next: ref Node{value: 2, next: null}}
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Node = type { i32, %Node* }
%__out_frame = type { i8*, i32, %__out_frame* }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [9 x i8] c"unbound \00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.1 = private unnamed_addr constant [11 x i8] c"[Circular]\00", align 1
@fmt.2 = private unnamed_addr constant [6 x i8] c"Node{\00", align 1
@fmt.3 = private unnamed_addr constant [8 x i8] c"value: \00", align 1
@fmt.4 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@fmt.5 = private unnamed_addr constant [9 x i8] c", next: \00", align 1
@fmt.6 = private unnamed_addr constant [5 x i8] c"null\00", align 1
@fmt.7 = private unnamed_addr constant [5 x i8] c"ref \00", align 1
@fmt.8 = private unnamed_addr constant [2 x i8] c"}\00", align 1
@str.9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.10 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.11 = private unnamed_addr constant [9 x i8] c"bound   \00", align 1
@fmt.12 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.13 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.14 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %a = alloca %Node, align 8
  store %Node zeroinitializer, %Node* %a, align 8
  %value = getelementptr %Node, %Node* %a, i32 0, i32 0
  store i32 1, i32* %value, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0))
  call void @__out_Node(%Node* %a, %__out_frame* null)
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.10, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.9, i32 0, i32 0))
  %b = alloca %Node, align 8
  store %Node zeroinitializer, %Node* %b, align 8
  %value1 = getelementptr %Node, %Node* %b, i32 0, i32 0
  store i32 2, i32* %value1, align 4
  %next = getelementptr %Node, %Node* %a, i32 0, i32 1
  store %Node* %b, %Node** %next, align 8
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.12, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.11, i32 0, i32 0))
  call void @__out_Node(%Node* %a, %__out_frame* null)
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.14, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.13, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)

define void @__out_Node(%Node* %0, %__out_frame* %1) {
entry:
  %me = bitcast %Node* %0 to i8*
  %walk = alloca %__out_frame*, align 8
  store %__out_frame* %1, %__out_frame** %walk, align 8
  br label %seen.cond

seen.cond:                                        ; preds = %seen.next, %entry
  %q = load %__out_frame*, %__out_frame** %walk, align 8
  %q2i = ptrtoint %__out_frame* %q to i64
  %atroot = icmp eq i64 %q2i, 0
  br i1 %atroot, label %seen.fresh, label %seen.body

seen.body:                                        ; preds = %seen.cond
  %q.ptr = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 0
  %held = load i8*, i8** %q.ptr, align 8
  %q.ty = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 1
  %heldty = load i32, i32* %q.ty, align 4
  %h2i = ptrtoint i8* %held to i64
  %m2i = ptrtoint i8* %me to i64
  %sameptr = icmp eq i64 %h2i, %m2i
  %samety = icmp eq i32 %heldty, 1
  %same = and i1 %sameptr, %samety
  br i1 %same, label %seen.hit, label %seen.next

seen.hit:                                         ; preds = %seen.body
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.1, i32 0, i32 0))
  ret void

seen.next:                                        ; preds = %seen.body
  %q.prev = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 2
  %up = load %__out_frame*, %__out_frame** %q.prev, align 8
  store %__out_frame* %up, %__out_frame** %walk, align 8
  br label %seen.cond

seen.fresh:                                       ; preds = %seen.cond
  %frame = alloca %__out_frame, align 8
  %f.ptr = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 0
  store i8* %me, i8** %f.ptr, align 8
  %f.ty = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 1
  store i32 1, i32* %f.ty, align 4
  %f.prev = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 2
  store %__out_frame* %1, %__out_frame** %f.prev, align 8
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @fmt.2, i32 0, i32 0))
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @fmt.3, i32 0, i32 0))
  %value = getelementptr %Node, %Node* %0, i32 0, i32 0
  %f = load i32, i32* %value, align 4
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.4, i32 0, i32 0), i32 %f)
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.5, i32 0, i32 0))
  %next = getelementptr %Node, %Node* %0, i32 0, i32 1
  %ref = load %Node*, %Node** %next, align 8
  %p2i = ptrtoint %Node* %ref to i64
  %isnull = icmp eq i64 %p2i, 0
  br i1 %isnull, label %out.null, label %out.ref

out.null:                                         ; preds = %seen.fresh
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.6, i32 0, i32 0))
  br label %out.refend

out.ref:                                          ; preds = %seen.fresh
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.7, i32 0, i32 0))
  call void @__out_Node(%Node* %ref, %__out_frame* %frame)
  br label %out.refend

out.refend:                                       ; preds = %out.ref, %out.null
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @fmt.8, i32 0, i32 0))
  ret void
}
```

## 028 — output() prints [Circular] instead of looping forever

```ura
// structs/028.ura - output() prints [Circular] instead of looping forever

struct Node:
    value i32
    ref? next Node

main():
    a Node
    b Node
    a.value = 1
    b.value = 2
    a.next = ref b
    b.next = ref a
    output("cycle ", a, "\n")
    output("self  ", b, "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Node
├─ value : i32
└─ next : STRUCT_CALL

fn main() : i32
├─ a : STRUCT_CALL
├─ b : STRUCT_CALL
├─ = : i32
│  ├─ .value : i32
│  │  └─ a : STRUCT_CALL
│  └─ int 1
├─ = : i32
│  ├─ .value : i32
│  │  └─ b : STRUCT_CALL
│  └─ int 2
├─ = : STRUCT_CALL
│  ├─ .next : STRUCT_CALL
│  │  └─ a : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ b : STRUCT_CALL
├─ = : STRUCT_CALL
│  ├─ .next : STRUCT_CALL
│  │  └─ b : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ a : STRUCT_CALL
├─ output : void
│  ├─ char[] "cycle "
│  ├─ a : STRUCT_CALL
│  └─ char[] "\n"
└─ output : void
   ├─ char[] "self  "
   ├─ b : STRUCT_CALL
   └─ char[] "\n"
```

```out
cycle Node{value: 1, next: ref Node{value: 2, next: ref [Circular]}}
self  Node{value: 2, next: ref Node{value: 1, next: ref [Circular]}}
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Node = type { i32, %Node* }
%__out_frame = type { i8*, i32, %__out_frame* }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [7 x i8] c"cycle \00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.1 = private unnamed_addr constant [11 x i8] c"[Circular]\00", align 1
@fmt.2 = private unnamed_addr constant [6 x i8] c"Node{\00", align 1
@fmt.3 = private unnamed_addr constant [8 x i8] c"value: \00", align 1
@fmt.4 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@fmt.5 = private unnamed_addr constant [9 x i8] c", next: \00", align 1
@fmt.6 = private unnamed_addr constant [5 x i8] c"null\00", align 1
@fmt.7 = private unnamed_addr constant [5 x i8] c"ref \00", align 1
@fmt.8 = private unnamed_addr constant [2 x i8] c"}\00", align 1
@str.9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.10 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.11 = private unnamed_addr constant [7 x i8] c"self  \00", align 1
@fmt.12 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.13 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.14 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %a = alloca %Node, align 8
  store %Node zeroinitializer, %Node* %a, align 8
  %b = alloca %Node, align 8
  store %Node zeroinitializer, %Node* %b, align 8
  %value = getelementptr %Node, %Node* %a, i32 0, i32 0
  store i32 1, i32* %value, align 4
  %value1 = getelementptr %Node, %Node* %b, i32 0, i32 0
  store i32 2, i32* %value1, align 4
  %next = getelementptr %Node, %Node* %a, i32 0, i32 1
  store %Node* %b, %Node** %next, align 8
  %next2 = getelementptr %Node, %Node* %b, i32 0, i32 1
  store %Node* %a, %Node** %next2, align 8
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0))
  call void @__out_Node(%Node* %a, %__out_frame* null)
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.10, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.9, i32 0, i32 0))
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.12, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.11, i32 0, i32 0))
  call void @__out_Node(%Node* %b, %__out_frame* null)
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.14, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.13, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)

define void @__out_Node(%Node* %0, %__out_frame* %1) {
entry:
  %me = bitcast %Node* %0 to i8*
  %walk = alloca %__out_frame*, align 8
  store %__out_frame* %1, %__out_frame** %walk, align 8
  br label %seen.cond

seen.cond:                                        ; preds = %seen.next, %entry
  %q = load %__out_frame*, %__out_frame** %walk, align 8
  %q2i = ptrtoint %__out_frame* %q to i64
  %atroot = icmp eq i64 %q2i, 0
  br i1 %atroot, label %seen.fresh, label %seen.body

seen.body:                                        ; preds = %seen.cond
  %q.ptr = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 0
  %held = load i8*, i8** %q.ptr, align 8
  %q.ty = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 1
  %heldty = load i32, i32* %q.ty, align 4
  %h2i = ptrtoint i8* %held to i64
  %m2i = ptrtoint i8* %me to i64
  %sameptr = icmp eq i64 %h2i, %m2i
  %samety = icmp eq i32 %heldty, 1
  %same = and i1 %sameptr, %samety
  br i1 %same, label %seen.hit, label %seen.next

seen.hit:                                         ; preds = %seen.body
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.1, i32 0, i32 0))
  ret void

seen.next:                                        ; preds = %seen.body
  %q.prev = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 2
  %up = load %__out_frame*, %__out_frame** %q.prev, align 8
  store %__out_frame* %up, %__out_frame** %walk, align 8
  br label %seen.cond

seen.fresh:                                       ; preds = %seen.cond
  %frame = alloca %__out_frame, align 8
  %f.ptr = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 0
  store i8* %me, i8** %f.ptr, align 8
  %f.ty = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 1
  store i32 1, i32* %f.ty, align 4
  %f.prev = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 2
  store %__out_frame* %1, %__out_frame** %f.prev, align 8
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @fmt.2, i32 0, i32 0))
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @fmt.3, i32 0, i32 0))
  %value = getelementptr %Node, %Node* %0, i32 0, i32 0
  %f = load i32, i32* %value, align 4
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.4, i32 0, i32 0), i32 %f)
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.5, i32 0, i32 0))
  %next = getelementptr %Node, %Node* %0, i32 0, i32 1
  %ref = load %Node*, %Node** %next, align 8
  %p2i = ptrtoint %Node* %ref to i64
  %isnull = icmp eq i64 %p2i, 0
  br i1 %isnull, label %out.null, label %out.ref

out.null:                                         ; preds = %seen.fresh
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.6, i32 0, i32 0))
  br label %out.refend

out.ref:                                          ; preds = %seen.fresh
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.7, i32 0, i32 0))
  call void @__out_Node(%Node* %ref, %__out_frame* %frame)
  br label %out.refend

out.refend:                                       ; preds = %out.ref, %out.null
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @fmt.8, i32 0, i32 0))
  ret void
}
```

## 029 — output() expands array fields, including 2D

```ura
// structs/029.ura - output() expands array fields, including 2D

struct Room:
    floor i32

struct Flat:
    tag   char[]
    rooms Room[]

struct Grid:
    tag   char[]
    rooms Room[][]

main():
    f Flat
    f.tag   = "flat"
    f.rooms = Room[2]
    f.rooms[0].floor = 3
    f.rooms[1].floor = 4
    output(f, "\n")

    g Grid
    g.tag   = "grid"
    g.rooms = Room[2][2]
    g.rooms[0][0].floor = 1
    g.rooms[0][1].floor = 2
    g.rooms[1][0].floor = 3
    g.rooms[1][1].floor = 4
    output(g, "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Room
└─ floor : i32

struct Flat
├─ tag : char[]
└─ rooms : STRUCT_CALL[]

struct Grid
├─ tag : char[]
└─ rooms : STRUCT_CALL[][]

fn main() : i32
├─ f : STRUCT_CALL
├─ = : array
│  ├─ .tag : char[]
│  │  └─ f : STRUCT_CALL
│  └─ char[] "flat"
├─ = : array
│  ├─ .rooms : STRUCT_CALL[]
│  │  └─ f : STRUCT_CALL
│  └─ array : STRUCT_CALL[]
│     └─ int 2
├─ = : i32
│  ├─ .floor : i32
│  │  └─ index : STRUCT_CALL
│  │     ├─ .rooms : STRUCT_CALL[]
│  │     │  └─ f : STRUCT_CALL
│  │     └─ int 0
│  └─ int 3
├─ = : i32
│  ├─ .floor : i32
│  │  └─ index : STRUCT_CALL
│  │     ├─ .rooms : STRUCT_CALL[]
│  │     │  └─ f : STRUCT_CALL
│  │     └─ int 1
│  └─ int 4
├─ output : void
│  ├─ f : STRUCT_CALL
│  └─ char[] "\n"
├─ g : STRUCT_CALL
├─ = : array
│  ├─ .tag : char[]
│  │  └─ g : STRUCT_CALL
│  └─ char[] "grid"
├─ = : array
│  ├─ .rooms : STRUCT_CALL[][]
│  │  └─ g : STRUCT_CALL
│  └─ array : STRUCT_CALL[][]
│     ├─ int 2
│     └─ int 2
├─ = : i32
│  ├─ .floor : i32
│  │  └─ index : STRUCT_CALL
│  │     ├─ index : STRUCT_CALL[]
│  │     │  ├─ .rooms : STRUCT_CALL[][]
│  │     │  │  └─ g : STRUCT_CALL
│  │     │  └─ int 0
│  │     └─ int 0
│  └─ int 1
├─ = : i32
│  ├─ .floor : i32
│  │  └─ index : STRUCT_CALL
│  │     ├─ index : STRUCT_CALL[]
│  │     │  ├─ .rooms : STRUCT_CALL[][]
│  │     │  │  └─ g : STRUCT_CALL
│  │     │  └─ int 0
│  │     └─ int 1
│  └─ int 2
├─ = : i32
│  ├─ .floor : i32
│  │  └─ index : STRUCT_CALL
│  │     ├─ index : STRUCT_CALL[]
│  │     │  ├─ .rooms : STRUCT_CALL[][]
│  │     │  │  └─ g : STRUCT_CALL
│  │     │  └─ int 1
│  │     └─ int 0
│  └─ int 3
├─ = : i32
│  ├─ .floor : i32
│  │  └─ index : STRUCT_CALL
│  │     ├─ index : STRUCT_CALL[]
│  │     │  ├─ .rooms : STRUCT_CALL[][]
│  │     │  │  └─ g : STRUCT_CALL
│  │     │  └─ int 1
│  │     └─ int 1
│  └─ int 4
└─ output : void
   ├─ g : STRUCT_CALL
   └─ char[] "\n"
```

```out
Flat{tag: flat, rooms: [Room{floor: 3}, Room{floor: 4}]}
Grid{tag: grid, rooms: [[Room{floor: 1}, Room{floor: 2}], [Room{floor: 3}, Room{floor: 4}]]}
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Flat = type { { i8*, i64 }, { %Room*, i64 } }
%Room = type { i32 }
%__out_frame = type { i8*, i32, %__out_frame* }
%Grid = type { { i8*, i64 }, { { %Room*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [5 x i8] c"flat\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"[Circular]\00", align 1
@fmt.1 = private unnamed_addr constant [6 x i8] c"Flat{\00", align 1
@fmt.2 = private unnamed_addr constant [6 x i8] c"tag: \00", align 1
@fmt.3 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.4 = private unnamed_addr constant [10 x i8] c", rooms: \00", align 1
@fmt.5 = private unnamed_addr constant [2 x i8] c"[\00", align 1
@fmt.6 = private unnamed_addr constant [3 x i8] c", \00", align 1
@fmt.7 = private unnamed_addr constant [11 x i8] c"[Circular]\00", align 1
@fmt.8 = private unnamed_addr constant [6 x i8] c"Room{\00", align 1
@fmt.9 = private unnamed_addr constant [8 x i8] c"floor: \00", align 1
@fmt.10 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@fmt.11 = private unnamed_addr constant [2 x i8] c"}\00", align 1
@fmt.12 = private unnamed_addr constant [2 x i8] c"]\00", align 1
@fmt.13 = private unnamed_addr constant [2 x i8] c"}\00", align 1
@str.14 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.15 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.16 = private unnamed_addr constant [5 x i8] c"grid\00", align 1
@fmt.17 = private unnamed_addr constant [11 x i8] c"[Circular]\00", align 1
@fmt.18 = private unnamed_addr constant [6 x i8] c"Grid{\00", align 1
@fmt.19 = private unnamed_addr constant [6 x i8] c"tag: \00", align 1
@fmt.20 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.21 = private unnamed_addr constant [10 x i8] c", rooms: \00", align 1
@fmt.22 = private unnamed_addr constant [2 x i8] c"[\00", align 1
@fmt.23 = private unnamed_addr constant [3 x i8] c", \00", align 1
@fmt.24 = private unnamed_addr constant [2 x i8] c"[\00", align 1
@fmt.25 = private unnamed_addr constant [3 x i8] c", \00", align 1
@fmt.26 = private unnamed_addr constant [2 x i8] c"]\00", align 1
@fmt.27 = private unnamed_addr constant [2 x i8] c"]\00", align 1
@fmt.28 = private unnamed_addr constant [2 x i8] c"}\00", align 1
@str.29 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.30 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %f = alloca %Flat, align 8
  store %Flat zeroinitializer, %Flat* %f, align 8
  %tag = getelementptr %Flat, %Flat* %f, i32 0, i32 0
  store { i8*, i64 } { i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i64 4 }, { i8*, i64 }* %tag, align 8
  %rooms = getelementptr %Flat, %Flat* %f, i32 0, i32 1
  %arr = alloca %Room, i64 2, align 8
  %2 = bitcast %Room* %arr to i8*
  call void @llvm.memset.p0i8.i64(i8* %2, i8 0, i64 8, i1 false)
  %arr.ptr = insertvalue { %Room*, i64 } undef, %Room* %arr, 0
  %arr.len = insertvalue { %Room*, i64 } %arr.ptr, i64 2, 1
  store { %Room*, i64 } %arr.len, { %Room*, i64 }* %rooms, align 8
  %rooms1 = getelementptr %Flat, %Flat* %f, i32 0, i32 1
  %rooms2 = load { %Room*, i64 }, { %Room*, i64 }* %rooms1, align 8
  %arr.data = extractvalue { %Room*, i64 } %rooms2, 0
  %arr.at = getelementptr %Room, %Room* %arr.data, i32 0
  %floor = getelementptr %Room, %Room* %arr.at, i32 0, i32 0
  store i32 3, i32* %floor, align 4
  %rooms3 = getelementptr %Flat, %Flat* %f, i32 0, i32 1
  %rooms4 = load { %Room*, i64 }, { %Room*, i64 }* %rooms3, align 8
  %arr.data5 = extractvalue { %Room*, i64 } %rooms4, 0
  %arr.at6 = getelementptr %Room, %Room* %arr.data5, i32 1
  %floor7 = getelementptr %Room, %Room* %arr.at6, i32 0, i32 0
  store i32 4, i32* %floor7, align 4
  call void @__out_Flat(%Flat* %f, %__out_frame* null)
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.15, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.14, i32 0, i32 0))
  %g = alloca %Grid, align 8
  store %Grid zeroinitializer, %Grid* %g, align 8
  %tag8 = getelementptr %Grid, %Grid* %g, i32 0, i32 0
  store { i8*, i64 } { i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.16, i32 0, i32 0), i64 4 }, { i8*, i64 }* %tag8, align 8
  %rooms9 = getelementptr %Grid, %Grid* %g, i32 0, i32 1
  %arr10 = alloca { %Room*, i64 }, i64 2, align 8
  %i = alloca i64, align 8
  store i64 0, i64* %i, align 4
  br label %arr.cond

arr.cond:                                         ; preds = %arr.body, %entry
  %i11 = load i64, i64* %i, align 4
  %more = icmp slt i64 %i11, 2
  br i1 %more, label %arr.body, label %arr.end

arr.body:                                         ; preds = %arr.cond
  %arr12 = alloca %Room, i64 2, align 8
  %4 = bitcast %Room* %arr12 to i8*
  call void @llvm.memset.p0i8.i64(i8* %4, i8 0, i64 8, i1 false)
  %arr.ptr13 = insertvalue { %Room*, i64 } undef, %Room* %arr12, 0
  %arr.len14 = insertvalue { %Room*, i64 } %arr.ptr13, i64 2, 1
  %i15 = load i64, i64* %i, align 4
  %arr.slot = getelementptr { %Room*, i64 }, { %Room*, i64 }* %arr10, i64 %i15
  store { %Room*, i64 } %arr.len14, { %Room*, i64 }* %arr.slot, align 8
  %next = add i64 %i15, 1
  store i64 %next, i64* %i, align 4
  br label %arr.cond

arr.end:                                          ; preds = %arr.cond
  %arr.ptr16 = insertvalue { { %Room*, i64 }*, i64 } undef, { %Room*, i64 }* %arr10, 0
  %arr.len17 = insertvalue { { %Room*, i64 }*, i64 } %arr.ptr16, i64 2, 1
  store { { %Room*, i64 }*, i64 } %arr.len17, { { %Room*, i64 }*, i64 }* %rooms9, align 8
  %rooms18 = getelementptr %Grid, %Grid* %g, i32 0, i32 1
  %rooms19 = load { { %Room*, i64 }*, i64 }, { { %Room*, i64 }*, i64 }* %rooms18, align 8
  %arr.data20 = extractvalue { { %Room*, i64 }*, i64 } %rooms19, 0
  %arr.at21 = getelementptr { %Room*, i64 }, { %Room*, i64 }* %arr.data20, i32 0
  %idx = load { %Room*, i64 }, { %Room*, i64 }* %arr.at21, align 8
  %arr.data22 = extractvalue { %Room*, i64 } %idx, 0
  %arr.at23 = getelementptr %Room, %Room* %arr.data22, i32 0
  %floor24 = getelementptr %Room, %Room* %arr.at23, i32 0, i32 0
  store i32 1, i32* %floor24, align 4
  %rooms25 = getelementptr %Grid, %Grid* %g, i32 0, i32 1
  %rooms26 = load { { %Room*, i64 }*, i64 }, { { %Room*, i64 }*, i64 }* %rooms25, align 8
  %arr.data27 = extractvalue { { %Room*, i64 }*, i64 } %rooms26, 0
  %arr.at28 = getelementptr { %Room*, i64 }, { %Room*, i64 }* %arr.data27, i32 0
  %idx29 = load { %Room*, i64 }, { %Room*, i64 }* %arr.at28, align 8
  %arr.data30 = extractvalue { %Room*, i64 } %idx29, 0
  %arr.at31 = getelementptr %Room, %Room* %arr.data30, i32 1
  %floor32 = getelementptr %Room, %Room* %arr.at31, i32 0, i32 0
  store i32 2, i32* %floor32, align 4
  %rooms33 = getelementptr %Grid, %Grid* %g, i32 0, i32 1
  %rooms34 = load { { %Room*, i64 }*, i64 }, { { %Room*, i64 }*, i64 }* %rooms33, align 8
  %arr.data35 = extractvalue { { %Room*, i64 }*, i64 } %rooms34, 0
  %arr.at36 = getelementptr { %Room*, i64 }, { %Room*, i64 }* %arr.data35, i32 1
  %idx37 = load { %Room*, i64 }, { %Room*, i64 }* %arr.at36, align 8
  %arr.data38 = extractvalue { %Room*, i64 } %idx37, 0
  %arr.at39 = getelementptr %Room, %Room* %arr.data38, i32 0
  %floor40 = getelementptr %Room, %Room* %arr.at39, i32 0, i32 0
  store i32 3, i32* %floor40, align 4
  %rooms41 = getelementptr %Grid, %Grid* %g, i32 0, i32 1
  %rooms42 = load { { %Room*, i64 }*, i64 }, { { %Room*, i64 }*, i64 }* %rooms41, align 8
  %arr.data43 = extractvalue { { %Room*, i64 }*, i64 } %rooms42, 0
  %arr.at44 = getelementptr { %Room*, i64 }, { %Room*, i64 }* %arr.data43, i32 1
  %idx45 = load { %Room*, i64 }, { %Room*, i64 }* %arr.at44, align 8
  %arr.data46 = extractvalue { %Room*, i64 } %idx45, 0
  %arr.at47 = getelementptr %Room, %Room* %arr.data46, i32 1
  %floor48 = getelementptr %Room, %Room* %arr.at47, i32 0, i32 0
  store i32 4, i32* %floor48, align 4
  call void @__out_Grid(%Grid* %g, %__out_frame* null)
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.30, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.29, i32 0, i32 0))
  ret i32 0
}

; Function Attrs: argmemonly nofree nounwind willreturn writeonly
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #0

define void @__out_Flat(%Flat* %0, %__out_frame* %1) {
entry:
  %me = bitcast %Flat* %0 to i8*
  %walk = alloca %__out_frame*, align 8
  store %__out_frame* %1, %__out_frame** %walk, align 8
  br label %seen.cond

seen.cond:                                        ; preds = %seen.next, %entry
  %q = load %__out_frame*, %__out_frame** %walk, align 8
  %q2i = ptrtoint %__out_frame* %q to i64
  %atroot = icmp eq i64 %q2i, 0
  br i1 %atroot, label %seen.fresh, label %seen.body

seen.body:                                        ; preds = %seen.cond
  %q.ptr = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 0
  %held = load i8*, i8** %q.ptr, align 8
  %q.ty = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 1
  %heldty = load i32, i32* %q.ty, align 4
  %h2i = ptrtoint i8* %held to i64
  %m2i = ptrtoint i8* %me to i64
  %sameptr = icmp eq i64 %h2i, %m2i
  %samety = icmp eq i32 %heldty, 1
  %same = and i1 %sameptr, %samety
  br i1 %same, label %seen.hit, label %seen.next

seen.hit:                                         ; preds = %seen.body
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0))
  ret void

seen.next:                                        ; preds = %seen.body
  %q.prev = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 2
  %up = load %__out_frame*, %__out_frame** %q.prev, align 8
  store %__out_frame* %up, %__out_frame** %walk, align 8
  br label %seen.cond

seen.fresh:                                       ; preds = %seen.cond
  %frame = alloca %__out_frame, align 8
  %f.ptr = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 0
  store i8* %me, i8** %f.ptr, align 8
  %f.ty = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 1
  store i32 1, i32* %f.ty, align 4
  %f.prev = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 2
  store %__out_frame* %1, %__out_frame** %f.prev, align 8
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @fmt.1, i32 0, i32 0))
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @fmt.2, i32 0, i32 0))
  %tag = getelementptr %Flat, %Flat* %0, i32 0, i32 0
  %arr = load { i8*, i64 }, { i8*, i64 }* %tag, align 8
  %arr.data = extractvalue { i8*, i64 } %arr, 0
  %arr.len = extractvalue { i8*, i64 } %arr, 1
  %len32 = trunc i64 %arr.len to i32
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.3, i32 0, i32 0), i32 %len32, i8* %arr.data)
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @fmt.4, i32 0, i32 0))
  %rooms = getelementptr %Flat, %Flat* %0, i32 0, i32 1
  %arr1 = load { %Room*, i64 }, { %Room*, i64 }* %rooms, align 8
  %arr.data2 = extractvalue { %Room*, i64 } %arr1, 0
  %arr.len3 = extractvalue { %Room*, i64 } %arr1, 1
  %oi = alloca i64, align 8
  store i64 0, i64* %oi, align 4
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @fmt.5, i32 0, i32 0))
  br label %out.arr.cond

out.arr.cond:                                     ; preds = %out.arr.item, %seen.fresh
  %i = load i64, i64* %oi, align 4
  %more = icmp slt i64 %i, %arr.len3
  br i1 %more, label %out.arr.body, label %out.arr.end

out.arr.body:                                     ; preds = %out.arr.cond
  %notfirst = icmp sgt i64 %i, 0
  br i1 %notfirst, label %out.arr.sep, label %out.arr.item

out.arr.sep:                                      ; preds = %out.arr.body
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.6, i32 0, i32 0))
  br label %out.arr.item

out.arr.item:                                     ; preds = %out.arr.sep, %out.arr.body
  %at = getelementptr %Room, %Room* %arr.data2, i64 %i
  call void @__out_Room(%Room* %at, %__out_frame* %frame)
  %n = add i64 %i, 1
  store i64 %n, i64* %oi, align 4
  br label %out.arr.cond

out.arr.end:                                      ; preds = %out.arr.cond
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @fmt.12, i32 0, i32 0))
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @fmt.13, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)

define void @__out_Room(%Room* %0, %__out_frame* %1) {
entry:
  %me = bitcast %Room* %0 to i8*
  %walk = alloca %__out_frame*, align 8
  store %__out_frame* %1, %__out_frame** %walk, align 8
  br label %seen.cond

seen.cond:                                        ; preds = %seen.next, %entry
  %q = load %__out_frame*, %__out_frame** %walk, align 8
  %q2i = ptrtoint %__out_frame* %q to i64
  %atroot = icmp eq i64 %q2i, 0
  br i1 %atroot, label %seen.fresh, label %seen.body

seen.body:                                        ; preds = %seen.cond
  %q.ptr = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 0
  %held = load i8*, i8** %q.ptr, align 8
  %q.ty = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 1
  %heldty = load i32, i32* %q.ty, align 4
  %h2i = ptrtoint i8* %held to i64
  %m2i = ptrtoint i8* %me to i64
  %sameptr = icmp eq i64 %h2i, %m2i
  %samety = icmp eq i32 %heldty, 2
  %same = and i1 %sameptr, %samety
  br i1 %same, label %seen.hit, label %seen.next

seen.hit:                                         ; preds = %seen.body
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.7, i32 0, i32 0))
  ret void

seen.next:                                        ; preds = %seen.body
  %q.prev = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 2
  %up = load %__out_frame*, %__out_frame** %q.prev, align 8
  store %__out_frame* %up, %__out_frame** %walk, align 8
  br label %seen.cond

seen.fresh:                                       ; preds = %seen.cond
  %frame = alloca %__out_frame, align 8
  %f.ptr = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 0
  store i8* %me, i8** %f.ptr, align 8
  %f.ty = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 1
  store i32 2, i32* %f.ty, align 4
  %f.prev = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 2
  store %__out_frame* %1, %__out_frame** %f.prev, align 8
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @fmt.8, i32 0, i32 0))
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @fmt.9, i32 0, i32 0))
  %floor = getelementptr %Room, %Room* %0, i32 0, i32 0
  %f = load i32, i32* %floor, align 4
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.10, i32 0, i32 0), i32 %f)
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @fmt.11, i32 0, i32 0))
  ret void
}

define void @__out_Grid(%Grid* %0, %__out_frame* %1) {
entry:
  %me = bitcast %Grid* %0 to i8*
  %walk = alloca %__out_frame*, align 8
  store %__out_frame* %1, %__out_frame** %walk, align 8
  br label %seen.cond

seen.cond:                                        ; preds = %seen.next, %entry
  %q = load %__out_frame*, %__out_frame** %walk, align 8
  %q2i = ptrtoint %__out_frame* %q to i64
  %atroot = icmp eq i64 %q2i, 0
  br i1 %atroot, label %seen.fresh, label %seen.body

seen.body:                                        ; preds = %seen.cond
  %q.ptr = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 0
  %held = load i8*, i8** %q.ptr, align 8
  %q.ty = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 1
  %heldty = load i32, i32* %q.ty, align 4
  %h2i = ptrtoint i8* %held to i64
  %m2i = ptrtoint i8* %me to i64
  %sameptr = icmp eq i64 %h2i, %m2i
  %samety = icmp eq i32 %heldty, 3
  %same = and i1 %sameptr, %samety
  br i1 %same, label %seen.hit, label %seen.next

seen.hit:                                         ; preds = %seen.body
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.17, i32 0, i32 0))
  ret void

seen.next:                                        ; preds = %seen.body
  %q.prev = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 2
  %up = load %__out_frame*, %__out_frame** %q.prev, align 8
  store %__out_frame* %up, %__out_frame** %walk, align 8
  br label %seen.cond

seen.fresh:                                       ; preds = %seen.cond
  %frame = alloca %__out_frame, align 8
  %f.ptr = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 0
  store i8* %me, i8** %f.ptr, align 8
  %f.ty = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 1
  store i32 3, i32* %f.ty, align 4
  %f.prev = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 2
  store %__out_frame* %1, %__out_frame** %f.prev, align 8
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @fmt.18, i32 0, i32 0))
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @fmt.19, i32 0, i32 0))
  %tag = getelementptr %Grid, %Grid* %0, i32 0, i32 0
  %arr = load { i8*, i64 }, { i8*, i64 }* %tag, align 8
  %arr.data = extractvalue { i8*, i64 } %arr, 0
  %arr.len = extractvalue { i8*, i64 } %arr, 1
  %len32 = trunc i64 %arr.len to i32
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.20, i32 0, i32 0), i32 %len32, i8* %arr.data)
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @fmt.21, i32 0, i32 0))
  %rooms = getelementptr %Grid, %Grid* %0, i32 0, i32 1
  %arr1 = load { { %Room*, i64 }*, i64 }, { { %Room*, i64 }*, i64 }* %rooms, align 8
  %arr.data2 = extractvalue { { %Room*, i64 }*, i64 } %arr1, 0
  %arr.len3 = extractvalue { { %Room*, i64 }*, i64 } %arr1, 1
  %oi = alloca i64, align 8
  store i64 0, i64* %oi, align 4
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @fmt.22, i32 0, i32 0))
  br label %out.arr.cond

out.arr.cond:                                     ; preds = %out.arr.end12, %seen.fresh
  %i = load i64, i64* %oi, align 4
  %more = icmp slt i64 %i, %arr.len3
  br i1 %more, label %out.arr.body, label %out.arr.end

out.arr.body:                                     ; preds = %out.arr.cond
  %notfirst = icmp sgt i64 %i, 0
  br i1 %notfirst, label %out.arr.sep, label %out.arr.item

out.arr.sep:                                      ; preds = %out.arr.body
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.23, i32 0, i32 0))
  br label %out.arr.item

out.arr.item:                                     ; preds = %out.arr.sep, %out.arr.body
  %at = getelementptr { %Room*, i64 }, { %Room*, i64 }* %arr.data2, i64 %i
  %arr4 = load { %Room*, i64 }, { %Room*, i64 }* %at, align 8
  %arr.data5 = extractvalue { %Room*, i64 } %arr4, 0
  %arr.len6 = extractvalue { %Room*, i64 } %arr4, 1
  %oi7 = alloca i64, align 8
  store i64 0, i64* %oi7, align 4
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @fmt.24, i32 0, i32 0))
  br label %out.arr.cond8

out.arr.end:                                      ; preds = %out.arr.cond
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @fmt.27, i32 0, i32 0))
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @fmt.28, i32 0, i32 0))
  ret void

out.arr.cond8:                                    ; preds = %out.arr.item11, %out.arr.item
  %i13 = load i64, i64* %oi7, align 4
  %more14 = icmp slt i64 %i13, %arr.len6
  br i1 %more14, label %out.arr.body9, label %out.arr.end12

out.arr.body9:                                    ; preds = %out.arr.cond8
  %notfirst15 = icmp sgt i64 %i13, 0
  br i1 %notfirst15, label %out.arr.sep10, label %out.arr.item11

out.arr.sep10:                                    ; preds = %out.arr.body9
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.25, i32 0, i32 0))
  br label %out.arr.item11

out.arr.item11:                                   ; preds = %out.arr.sep10, %out.arr.body9
  %at16 = getelementptr %Room, %Room* %arr.data5, i64 %i13
  call void @__out_Room(%Room* %at16, %__out_frame* %frame)
  %n = add i64 %i13, 1
  store i64 %n, i64* %oi7, align 4
  br label %out.arr.cond8

out.arr.end12:                                    ; preds = %out.arr.cond8
  %13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @fmt.26, i32 0, i32 0))
  %n17 = add i64 %i, 1
  store i64 %n17, i64* %oi, align 4
  br label %out.arr.cond
}

attributes #0 = { argmemonly nofree nounwind willreturn writeonly }
```

## 030 — a field declared twice

```ura
// structs/030.ura - a field declared twice

struct Player:
    name char[]
    hp   i32
    hp   i32

main():
    p Player
```

```tree
```

```out
```

```err
error: Field 'hp' is already declared in this struct; rename it or remove the duplicate
  030.ura:6:5
  |
6 |     hp   i32
  |     ^^
```

```ll
```

## 031 — a field whose type does not exist

```ura
// structs/031.ura - a field whose type does not exist

struct Dungeon:
    entry Room

main():
    d Dungeon
```

```tree
```

```out
```

```err
error: Unknown type 'Room'
  031.ura:4:5
  |
4 |     entry Room
  |     ^^^^^
```

```ll
```

## 032 — a struct that contains itself by value

```ura
// structs/032.ura - a struct that contains itself by value

struct Node:
    value i32
    next  Node

main():
    n Node
```

```tree
```

```out
```

```err
error: Struct Node cannot contain itself by value; use an array or a 'ref?' so the size stays finite
  032.ura:3:1
  |
3 | struct Node:
  | ^^^^^^
```

```ll
```

## 033 — two structs that contain each other by value

```ura
// structs/033.ura - two structs that contain each other by value

struct A:
    b B

struct B:
    a A

main():
    x A
```

```tree
```

```out
```

```err
error: Struct A cannot contain itself by value; use an array or a 'ref?' so the size stays finite
  033.ura:3:1
  |
3 | struct A:
  | ^^^^^^
error: Struct B cannot contain itself by value; use an array or a 'ref?' so the size stays finite
  033.ura:6:1
  |
6 | struct B:
  | ^^^^^^
```

```ll
```

## 034 — a variable of a type that was never declared

```ura
// structs/034.ura - a variable of a type that was never declared

main():
    p Player
```

```tree
```

```out
```

```err
error: Unknown type 'Player'
  034.ura:4:5
  |
4 |     p Player
  |     ^
```

```ll
```

## 035 — reading a field the struct does not have

```ura
// structs/035.ura - reading a field the struct does not have

struct Player:
    name char[]
    hp   i32

main():
    p Player
    output(p.nope)
```

```tree
```

```out
```

```err
error: Struct Player has no field 'nope'; check the spelling or declare it in the struct
  035.ura:9:13
  |
9 |     output(p.nope)
  |             ^
```

```ll
```

## 036 — reading a field from a scalar

```ura
// structs/036.ura - reading a field from a scalar

main():
    x i32 = 3
    output(x.foo)
```

```tree
```

```out
```

```err
error: Cannot read '.foo' from i32; only a struct has fields
  036.ura:5:13
  |
5 |     output(x.foo)
  |             ^
```

```ll
```

## 037 — '.len' on a scalar

```ura
// structs/037.ura - '.len' on a scalar

main():
    x i32 = 3
    output(x.len)
```

```tree
```

```out
```

```err
error: '.len' is only valid on an array, not i32
  037.ura:5:13
  |
5 |     output(x.len)
  |             ^
```

```ll
```

## 038 — a struct declared in a block is not visible outside it

```ura
// structs/038.ura - a struct declared in a block is not visible outside it

main():
    n i32 = 1
    if n == 1:
        struct Inner:
            x i32

    i Inner
```

```tree
```

```out
```

```err
error: Unknown type 'Inner'
  038.ura:9:5
  |
9 |     i Inner
  |     ^
```

```ll
```

## 039 — a struct declared twice in the same scope

```ura
// structs/039.ura - a struct declared twice in the same scope

struct Player:
    hp i32

struct Player:
    mp i32

main():
    p Player
```

```tree
```

```out
```

```err
error: Redeclaration of struct 'Player'
  039.ura:6:1
  |
6 | struct Player:
  | ^^^^^^
```

```ll
```

## 040 — a method and a field chain called on a `ref?` field

```ura
// structs/040.ura - a method called through a 'ref?' field

struct Item:
    worth i32
    ref? next Item

    pub fn create(w i32) Item:
        it Item
        it.worth = w
        return it

    fn show() void:
        output("worth ", self.worth, "\n")

main():
    head Item = Item::create(5)
    mid  Item = Item::create(7)
    tail Item = Item::create(9)
    head.next = ref mid
    mid.next  = ref tail
    head.show()
    head.next.show()
    head.next.next.show()
    output("chained field ", head.next.next.worth, "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Item
├─ worth : i32
├─ next : STRUCT_CALL
├─ fn Item.create(w : i32) : STRUCT_CALL
│  ├─ it : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ .worth : i32
│  │  │  └─ it : STRUCT_CALL
│  │  └─ w : i32
│  └─ return
│     └─ it : STRUCT_CALL
└─ fn Item.show(self : STRUCT_CALL) : void
   └─ output : void
      ├─ char[] "worth "
      ├─ .worth : i32
      │  └─ self : STRUCT_CALL
      └─ char[] "\n"

fn main() : i32
├─ = : STRUCT_CALL
│  ├─ head : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     └─ int 5
├─ = : STRUCT_CALL
│  ├─ mid : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     └─ int 7
├─ = : STRUCT_CALL
│  ├─ tail : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     └─ int 9
├─ = : STRUCT_CALL
│  ├─ .next : STRUCT_CALL
│  │  └─ head : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ mid : STRUCT_CALL
├─ = : STRUCT_CALL
│  ├─ .next : STRUCT_CALL
│  │  └─ mid : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ tail : STRUCT_CALL
├─ call show : void
│  └─ head : STRUCT_CALL
├─ call show : void
│  └─ .next : STRUCT_CALL
│     └─ head : STRUCT_CALL
├─ call show : void
│  └─ .next : STRUCT_CALL
│     └─ .next : STRUCT_CALL
│        └─ head : STRUCT_CALL
└─ output : void
   ├─ char[] "chained field "
   ├─ .worth : i32
   │  └─ .next : STRUCT_CALL
   │     └─ .next : STRUCT_CALL
   │        └─ head : STRUCT_CALL
   └─ char[] "\n"
```

```out
worth 5
worth 7
worth 9
chained field 9
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Item = type { i32, %Item* }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [7 x i8] c"worth \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.2 = private unnamed_addr constant [15 x i8] c"chained field \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define %Item @Item.create(i32 %0) {
entry:
  %w = alloca i32, align 4
  store i32 %0, i32* %w, align 4
  %it = alloca %Item, align 8
  store %Item zeroinitializer, %Item* %it, align 8
  %worth = getelementptr %Item, %Item* %it, i32 0, i32 0
  %w1 = load i32, i32* %w, align 4
  store i32 %w1, i32* %worth, align 4
  %it2 = load %Item, %Item* %it, align 8
  ret %Item %it2
}

define void @Item.show(%Item* %0) {
entry:
  %self = alloca %Item*, align 8
  store %Item* %0, %Item** %self, align 8
  %ref = load %Item*, %Item** %self, align 8
  %worth = getelementptr %Item, %Item* %ref, i32 0, i32 0
  %worth1 = load i32, i32* %worth, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %worth1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)

define i32 @main(i32 %0, i8** %1) {
entry:
  %head = alloca %Item, align 8
  %call = call %Item @Item.create(i32 5)
  store %Item %call, %Item* %head, align 8
  %mid = alloca %Item, align 8
  %call1 = call %Item @Item.create(i32 7)
  store %Item %call1, %Item* %mid, align 8
  %tail = alloca %Item, align 8
  %call2 = call %Item @Item.create(i32 9)
  store %Item %call2, %Item* %tail, align 8
  %next = getelementptr %Item, %Item* %head, i32 0, i32 1
  store %Item* %mid, %Item** %next, align 8
  %next3 = getelementptr %Item, %Item* %mid, i32 0, i32 1
  store %Item* %tail, %Item** %next3, align 8
  call void @Item.show(%Item* %head)
  %next4 = getelementptr %Item, %Item* %head, i32 0, i32 1
  %ref = load %Item*, %Item** %next4, align 8
  call void @Item.show(%Item* %ref)
  %next5 = getelementptr %Item, %Item* %head, i32 0, i32 1
  %ref6 = load %Item*, %Item** %next5, align 8
  %next7 = getelementptr %Item, %Item* %ref6, i32 0, i32 1
  %ref8 = load %Item*, %Item** %next7, align 8
  call void @Item.show(%Item* %ref8)
  %next9 = getelementptr %Item, %Item* %head, i32 0, i32 1
  %ref10 = load %Item*, %Item** %next9, align 8
  %next11 = getelementptr %Item, %Item* %ref10, i32 0, i32 1
  %ref12 = load %Item*, %Item** %next11, align 8
  %worth = getelementptr %Item, %Item* %ref12, i32 0, i32 0
  %worth13 = load i32, i32* %worth, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.4, i32 0, i32 0), i32 14, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.2, i32 0, i32 0), i32 %worth13, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  ret i32 0
}
```

## 041 — an unknown type as a parameter, a return type, or an array element

```ura
// structs/041.ura - Unknown type in every declaration position

fn process(p Widget) void:
    return

fn make() Widget:
    w Widget
    return w

main():
    arr Widget[] = new Widget[3]
    output("hi\n")
```

```tree
```

```out
```

```err
error: Unknown type 'Widget'
  041.ura:3:12
  |
3 | fn process(p Widget) void:
  |            ^
error: Unknown type 'Widget'
  041.ura:6:1
  |
6 | fn make() Widget:
  | ^^
error: Unknown type 'Widget'
  041.ura:7:5
  |
7 |     w Widget
  |     ^
error: Unknown type 'Widget'
   041.ura:11:5
   |
11 |     arr Widget[] = new Widget[3]
   |     ^^^
error: Unknown type 'Widget'
   041.ura:11:24
   |
11 |     arr Widget[] = new Widget[3]
   |                        ^^^^^^
```

```ll
```

## 042 — `pub` inside struct must be followed by `fn`

```ura
// structs/042.ura - `pub` inside struct must be followed by `fn`
use "@/header"

struct Counter:
    n i32

    pub n i32

main():
    output("hi\n")
```

```tree
```

```out
```

```err
error: Expected 'fn' after 'pub'
  042.ura:7:5
  |
7 |     pub n i32
  |     ^^^
```

```ll
```

## 043 — struct redefinition

```ura
// structs/043.ura - struct redefinition
use "@/header"

struct Point:
    x i32
    y i32

struct Point:
    a i32
    b i32

main():
    output("hi\n")
```

```tree
```

```out
```

```err
error: Redeclaration of struct 'Point'
  043.ura:8:1
  |
8 | struct Point:
  | ^^^^^^
```

```ll
```

## 044 — instance method called via `::` instead of `.`

```ura
// structs/044.ura - instance method called via `::` instead of `.`
use "@/header"

struct Counter:
    n i32

    fn bump() void:
        self.n += 1

main():
    c Counter
    c.n = 0
    Counter::bump()
```

```tree
```

```out
```

```err
error: 'Counter::bump' needs a receiver; call it on a value, or declare it 'pub fn' to make it static
   044.ura:13:5
   |
13 |     Counter::bump()
   |     ^^^^^^^
```

```ll
```

## 045 — accessing an undefined field

```ura
// structs/045.ura - accessing an undefined field
use "@/header"

struct Point:
    x i32
    y i32

main():
    p Point
    p.x = 1
    p.z = 5
```

```tree
```

```out
```

```err
error: Struct Point has no field 'z'; check the spelling or declare it in the struct
   045.ura:11:6
   |
11 |     p.z = 5
   |      ^
```

```ll
```

## 046 — calling undefined method on struct

```ura
// structs/046.ura - calling undefined method on struct
use "@/header"

struct Point:
    x i32
    y i32

main():
    p Point
    p.x = 1
    p.normalize()
```

```tree
```

```out
```

```err
error: Struct Point has no method 'normalize'; check the spelling or declare it in the struct
   046.ura:11:6
   |
11 |     p.normalize()
   |      ^
```

```ll
```

## 047 — destructor must take no parameters

```ura
// structs/047.ura - destructor must take no parameters
use "@/header"

struct Box:
    value i32

    operator drop(extra i32) void:
        return

main():
    b Box
    b.value = 1
```

```tree
```

```out
```

```err
error: Destructor 'drop' takes no parameters; write 'operator drop:'
  047.ura:7:18
  |
7 |     operator drop(extra i32) void:
  |                  ^
```

```ll
```

## 048 — destructor must return void

```ura
// structs/048.ura - destructor must return void
use "@/header"

struct Box:
    value i32

    operator drop i32:
        return 0

main():
    b Box
    b.value = 1
```

```tree
```

```out
```

```err
error: Destructor 'drop' returns void; drop the return type
  048.ura:7:19
  |
7 |     operator drop i32:
  |                   ^^^
```

```ll
```

## 049 — `pub fn` called via `.` instead of `::`

```ura
// structs/049.ura - `pub fn` called via `.` instead of `::`
use "@/header"

struct Maker:
    n i32

    pub fn make() Maker:
        m Maker
        m.n = 1
        return m

main():
    m Maker
    m.n = 0
    m.make()
```

```tree
```

```out
```

```err
error: 'm.make' is a 'pub fn', so it is static and takes no receiver; call it as 'Maker::make()'
   049.ura:15:6
   |
15 |     m.make()
   |      ^
```

```ll
```

## 050 — a struct whose first field is a struct is not circular

```ura
// structs/050.ura - a struct whose first field is a struct is not circular

struct Inner:
    name char[]

struct First:
    tag Inner
    n   i32

struct Last:
    n   i32
    tag Inner

main():
    a First
    a.tag.name = "one"
    a.n = 1
    output("first: ", a, "\n")

    b Last
    b.n = 2
    b.tag.name = "two"
    output("last:  ", b, "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Inner
└─ name : char[]

struct First
├─ tag : STRUCT_CALL
└─ n : i32

struct Last
├─ n : i32
└─ tag : STRUCT_CALL

fn main() : i32
├─ a : STRUCT_CALL
├─ = : array
│  ├─ .name : char[]
│  │  └─ .tag : STRUCT_CALL
│  │     └─ a : STRUCT_CALL
│  └─ char[] "one"
├─ = : i32
│  ├─ .n : i32
│  │  └─ a : STRUCT_CALL
│  └─ int 1
├─ output : void
│  ├─ char[] "first: "
│  ├─ a : STRUCT_CALL
│  └─ char[] "\n"
├─ b : STRUCT_CALL
├─ = : i32
│  ├─ .n : i32
│  │  └─ b : STRUCT_CALL
│  └─ int 2
├─ = : array
│  ├─ .name : char[]
│  │  └─ .tag : STRUCT_CALL
│  │     └─ b : STRUCT_CALL
│  └─ char[] "two"
└─ output : void
   ├─ char[] "last:  "
   ├─ b : STRUCT_CALL
   └─ char[] "\n"
```

```out
first: First{tag: Inner{name: one}, n: 1}
last:  Last{n: 2, tag: Inner{name: two}}
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%First = type { %Inner, i32 }
%Inner = type { { i8*, i64 } }
%__out_frame = type { i8*, i32, %__out_frame* }
%Last = type { i32, %Inner }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [4 x i8] c"one\00", align 1
@str.1 = private unnamed_addr constant [8 x i8] c"first: \00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.2 = private unnamed_addr constant [11 x i8] c"[Circular]\00", align 1
@fmt.3 = private unnamed_addr constant [7 x i8] c"First{\00", align 1
@fmt.4 = private unnamed_addr constant [6 x i8] c"tag: \00", align 1
@fmt.5 = private unnamed_addr constant [11 x i8] c"[Circular]\00", align 1
@fmt.6 = private unnamed_addr constant [7 x i8] c"Inner{\00", align 1
@fmt.7 = private unnamed_addr constant [7 x i8] c"name: \00", align 1
@fmt.8 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.9 = private unnamed_addr constant [2 x i8] c"}\00", align 1
@fmt.10 = private unnamed_addr constant [6 x i8] c", n: \00", align 1
@fmt.11 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@fmt.12 = private unnamed_addr constant [2 x i8] c"}\00", align 1
@str.13 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.14 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.15 = private unnamed_addr constant [4 x i8] c"two\00", align 1
@str.16 = private unnamed_addr constant [8 x i8] c"last:  \00", align 1
@fmt.17 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.18 = private unnamed_addr constant [11 x i8] c"[Circular]\00", align 1
@fmt.19 = private unnamed_addr constant [6 x i8] c"Last{\00", align 1
@fmt.20 = private unnamed_addr constant [4 x i8] c"n: \00", align 1
@fmt.21 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@fmt.22 = private unnamed_addr constant [8 x i8] c", tag: \00", align 1
@fmt.23 = private unnamed_addr constant [2 x i8] c"}\00", align 1
@str.24 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.25 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %a = alloca %First, align 8
  store %First zeroinitializer, %First* %a, align 8
  %tag = getelementptr %First, %First* %a, i32 0, i32 0
  %name = getelementptr %Inner, %Inner* %tag, i32 0, i32 0
  store { i8*, i64 } { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i64 3 }, { i8*, i64 }* %name, align 8
  %n = getelementptr %First, %First* %a, i32 0, i32 1
  store i32 1, i32* %n, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 7, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.1, i32 0, i32 0))
  call void @__out_First(%First* %a, %__out_frame* null)
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.14, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.13, i32 0, i32 0))
  %b = alloca %Last, align 8
  store %Last zeroinitializer, %Last* %b, align 8
  %n1 = getelementptr %Last, %Last* %b, i32 0, i32 0
  store i32 2, i32* %n1, align 4
  %tag2 = getelementptr %Last, %Last* %b, i32 0, i32 1
  %name3 = getelementptr %Inner, %Inner* %tag2, i32 0, i32 0
  store { i8*, i64 } { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.15, i32 0, i32 0), i64 3 }, { i8*, i64 }* %name3, align 8
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.17, i32 0, i32 0), i32 7, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.16, i32 0, i32 0))
  call void @__out_Last(%Last* %b, %__out_frame* null)
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.25, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.24, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)

define void @__out_First(%First* %0, %__out_frame* %1) {
entry:
  %me = bitcast %First* %0 to i8*
  %walk = alloca %__out_frame*, align 8
  store %__out_frame* %1, %__out_frame** %walk, align 8
  br label %seen.cond

seen.cond:                                        ; preds = %seen.next, %entry
  %q = load %__out_frame*, %__out_frame** %walk, align 8
  %q2i = ptrtoint %__out_frame* %q to i64
  %atroot = icmp eq i64 %q2i, 0
  br i1 %atroot, label %seen.fresh, label %seen.body

seen.body:                                        ; preds = %seen.cond
  %q.ptr = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 0
  %held = load i8*, i8** %q.ptr, align 8
  %q.ty = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 1
  %heldty = load i32, i32* %q.ty, align 4
  %h2i = ptrtoint i8* %held to i64
  %m2i = ptrtoint i8* %me to i64
  %sameptr = icmp eq i64 %h2i, %m2i
  %samety = icmp eq i32 %heldty, 1
  %same = and i1 %sameptr, %samety
  br i1 %same, label %seen.hit, label %seen.next

seen.hit:                                         ; preds = %seen.body
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.2, i32 0, i32 0))
  ret void

seen.next:                                        ; preds = %seen.body
  %q.prev = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 2
  %up = load %__out_frame*, %__out_frame** %q.prev, align 8
  store %__out_frame* %up, %__out_frame** %walk, align 8
  br label %seen.cond

seen.fresh:                                       ; preds = %seen.cond
  %frame = alloca %__out_frame, align 8
  %f.ptr = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 0
  store i8* %me, i8** %f.ptr, align 8
  %f.ty = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 1
  store i32 1, i32* %f.ty, align 4
  %f.prev = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 2
  store %__out_frame* %1, %__out_frame** %f.prev, align 8
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.3, i32 0, i32 0))
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @fmt.4, i32 0, i32 0))
  %tag = getelementptr %First, %First* %0, i32 0, i32 0
  call void @__out_Inner(%Inner* %tag, %__out_frame* %frame)
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @fmt.10, i32 0, i32 0))
  %n = getelementptr %First, %First* %0, i32 0, i32 1
  %f = load i32, i32* %n, align 4
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.11, i32 0, i32 0), i32 %f)
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @fmt.12, i32 0, i32 0))
  ret void
}

define void @__out_Inner(%Inner* %0, %__out_frame* %1) {
entry:
  %me = bitcast %Inner* %0 to i8*
  %walk = alloca %__out_frame*, align 8
  store %__out_frame* %1, %__out_frame** %walk, align 8
  br label %seen.cond

seen.cond:                                        ; preds = %seen.next, %entry
  %q = load %__out_frame*, %__out_frame** %walk, align 8
  %q2i = ptrtoint %__out_frame* %q to i64
  %atroot = icmp eq i64 %q2i, 0
  br i1 %atroot, label %seen.fresh, label %seen.body

seen.body:                                        ; preds = %seen.cond
  %q.ptr = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 0
  %held = load i8*, i8** %q.ptr, align 8
  %q.ty = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 1
  %heldty = load i32, i32* %q.ty, align 4
  %h2i = ptrtoint i8* %held to i64
  %m2i = ptrtoint i8* %me to i64
  %sameptr = icmp eq i64 %h2i, %m2i
  %samety = icmp eq i32 %heldty, 2
  %same = and i1 %sameptr, %samety
  br i1 %same, label %seen.hit, label %seen.next

seen.hit:                                         ; preds = %seen.body
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.5, i32 0, i32 0))
  ret void

seen.next:                                        ; preds = %seen.body
  %q.prev = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 2
  %up = load %__out_frame*, %__out_frame** %q.prev, align 8
  store %__out_frame* %up, %__out_frame** %walk, align 8
  br label %seen.cond

seen.fresh:                                       ; preds = %seen.cond
  %frame = alloca %__out_frame, align 8
  %f.ptr = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 0
  store i8* %me, i8** %f.ptr, align 8
  %f.ty = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 1
  store i32 2, i32* %f.ty, align 4
  %f.prev = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 2
  store %__out_frame* %1, %__out_frame** %f.prev, align 8
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.6, i32 0, i32 0))
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.7, i32 0, i32 0))
  %name = getelementptr %Inner, %Inner* %0, i32 0, i32 0
  %arr = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %arr, 0
  %arr.len = extractvalue { i8*, i64 } %arr, 1
  %len32 = trunc i64 %arr.len to i32
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.8, i32 0, i32 0), i32 %len32, i8* %arr.data)
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @fmt.9, i32 0, i32 0))
  ret void
}

define void @__out_Last(%Last* %0, %__out_frame* %1) {
entry:
  %me = bitcast %Last* %0 to i8*
  %walk = alloca %__out_frame*, align 8
  store %__out_frame* %1, %__out_frame** %walk, align 8
  br label %seen.cond

seen.cond:                                        ; preds = %seen.next, %entry
  %q = load %__out_frame*, %__out_frame** %walk, align 8
  %q2i = ptrtoint %__out_frame* %q to i64
  %atroot = icmp eq i64 %q2i, 0
  br i1 %atroot, label %seen.fresh, label %seen.body

seen.body:                                        ; preds = %seen.cond
  %q.ptr = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 0
  %held = load i8*, i8** %q.ptr, align 8
  %q.ty = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 1
  %heldty = load i32, i32* %q.ty, align 4
  %h2i = ptrtoint i8* %held to i64
  %m2i = ptrtoint i8* %me to i64
  %sameptr = icmp eq i64 %h2i, %m2i
  %samety = icmp eq i32 %heldty, 3
  %same = and i1 %sameptr, %samety
  br i1 %same, label %seen.hit, label %seen.next

seen.hit:                                         ; preds = %seen.body
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.18, i32 0, i32 0))
  ret void

seen.next:                                        ; preds = %seen.body
  %q.prev = getelementptr %__out_frame, %__out_frame* %q, i32 0, i32 2
  %up = load %__out_frame*, %__out_frame** %q.prev, align 8
  store %__out_frame* %up, %__out_frame** %walk, align 8
  br label %seen.cond

seen.fresh:                                       ; preds = %seen.cond
  %frame = alloca %__out_frame, align 8
  %f.ptr = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 0
  store i8* %me, i8** %f.ptr, align 8
  %f.ty = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 1
  store i32 3, i32* %f.ty, align 4
  %f.prev = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 2
  store %__out_frame* %1, %__out_frame** %f.prev, align 8
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @fmt.19, i32 0, i32 0))
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @fmt.20, i32 0, i32 0))
  %n = getelementptr %Last, %Last* %0, i32 0, i32 0
  %f = load i32, i32* %n, align 4
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.21, i32 0, i32 0), i32 %f)
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @fmt.22, i32 0, i32 0))
  %tag = getelementptr %Last, %Last* %0, i32 0, i32 1
  call void @__out_Inner(%Inner* %tag, %__out_frame* %frame)
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @fmt.23, i32 0, i32 0))
  ret void
}
```

## 051 — `pub` may only be used inside a struct

```ura
// structs/051.ura - `pub` may only be used inside a struct
use "@/header"

pub fn helper() void:
    return

main():
    helper()
```

```err
error: 'pub' marks a static method; it may only be used inside a struct
  051.ura:4:1
  |
4 | pub fn helper() void:
  | ^^^
```
