# mod

## index

- 001 — a module function is called with `::`
- 002 — nested modules: `a::b::f()`
- 003 — an enum inside a module, member read with `::`
- 004 — a module may be referenced before it is declared
- 005 — a module function calls a sibling unqualified
- 006 — a struct declared in a module, used as `mod::Type`
- 007 — a `pub` static method on a struct inside a module
- 008 — calling a module member with `.` is an error
- 009 — an unknown member of a known module is an error
- 010 — only declarations are allowed in a module body
- 011 — a module call is arity-checked

## 001 — a module function is called with `::`

```ura
// mod/001 — a module function is called with ::
mod lib:
    fn foo() i32:
        return 1

main():
    output(lib::foo(), "\n")
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

mod lib
└─ fn lib.foo() : i32
   └─ return
      └─ int 1

fn main() : i32
└─ output : void
   ├─ call foo : i32
   └─ char[] "\n"
```

```out
1
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

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

define i32 @lib.foo() {
entry:
  ret i32 1
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %call = call i32 @lib.foo()
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %call, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 002 — nested modules: `a::b::f()`

```ura
// mod/002 — nested modules a::b::f()
mod a:
    mod b:
        fn f() i32:
            return 42

main():
    output(a::b::f(), "\n")
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

mod a
└─ mod a.b
   └─ fn a.b.f() : i32
      └─ return
         └─ int 42

fn main() : i32
└─ output : void
   ├─ call f : i32
   └─ char[] "\n"
```

```out
42
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

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

define i32 @a.b.f() {
entry:
  ret i32 42
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %call = call i32 @a.b.f()
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %call, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 003 — an enum inside a module, member read with `::`

```ura
// mod/003 — an enum inside a module, member read with ::
mod col:
    enum Color:
        RED, GREEN, BLUE

main():
    output(col::RED, " ", col::BLUE, "\n")
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

mod col
└─ ENUM_DEF
   ├─ ENUM_CALL
   ├─ ENUM_CALL
   └─ ENUM_CALL

fn main() : i32
└─ output : void
   ├─ int 0
   ├─ char[] " "
   ├─ int 2
   └─ char[] "\n"
```

```out
0 2
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
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i32 0, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 2, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 004 — a module may be referenced before it is declared

```ura
// mod/004 — a module may be referenced before it is declared
main():
    output(later::hi(), "\n")

mod later:
    fn hi() i32:
        return 7
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
└─ output : void
   ├─ call hi : i32
   └─ char[] "\n"

mod later
└─ fn later.hi() : i32
   └─ return
      └─ int 7
```

```out
7
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

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
  %call = call i32 @later.hi()
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %call, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

define i32 @later.hi() {
entry:
  ret i32 7
}

declare i32 @printf(i8*, ...)
```

## 005 — a module function calls a sibling unqualified

```ura
// mod/005 — a module function calls a sibling unqualified
mod m:
    fn helper() i32:
        return 5
    fn use_it() i32:
        return helper() + 1

main():
    output(m::use_it(), "\n")
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

mod m
├─ fn m.helper() : i32
│  └─ return
│     └─ int 5
└─ fn m.use_it() : i32
   └─ return
      └─ + : i32
         ├─ call helper : i32
         └─ int 1

fn main() : i32
└─ output : void
   ├─ call use_it : i32
   └─ char[] "\n"
```

```out
6
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

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

define i32 @m.helper() {
entry:
  ret i32 5
}

define i32 @m.use_it() {
entry:
  %call = call i32 @m.helper()
  %add = add i32 %call, 1
  ret i32 %add
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %call = call i32 @m.use_it()
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %call, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 006 — a struct declared in a module, used as `mod::Type`

```ura
// mod/006 — a struct declared in a module, used as mod::Type
mod g:
    struct Point:
        x i32

main():
    p g::Point
    p.x = 9
    output(p.x, "\n")
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

mod g
└─ struct g.Point
   └─ x : i32

fn main() : i32
├─ p : STRUCT_CALL
├─ = : i32
│  ├─ .x : i32
│  │  └─ p : STRUCT_CALL
│  └─ int 9
└─ output : void
   ├─ .x : i32
   │  └─ p : STRUCT_CALL
   └─ char[] "\n"
```

```out
9
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%g.Point = type { i32 }

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
  %p = alloca %g.Point, align 8
  store %g.Point zeroinitializer, %g.Point* %p, align 4
  %x = getelementptr %g.Point, %g.Point* %p, i32 0, i32 0
  store i32 9, i32* %x, align 4
  %x1 = getelementptr %g.Point, %g.Point* %p, i32 0, i32 0
  %x2 = load i32, i32* %x1, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %x2, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 007 — a `pub` static method on a struct inside a module

```ura
// mod/007 — a pub static method on a struct inside a module
mod g:
    struct Point:
        x i32
        pub fn make(v i32) Point:
            p Point
            p.x = v
            return p

main():
    p g::Point = g::Point::make(3)
    output(p.x, "\n")
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

mod g
└─ struct g.Point
   ├─ x : i32
   └─ fn g.Point.make(v : i32) : STRUCT_CALL
      ├─ p : STRUCT_CALL
      ├─ = : i32
      │  ├─ .x : i32
      │  │  └─ p : STRUCT_CALL
      │  └─ v : i32
      └─ return
         └─ p : STRUCT_CALL

fn main() : i32
├─ = : STRUCT_CALL
│  ├─ p : STRUCT_CALL
│  └─ call make : STRUCT_CALL
│     └─ int 3
└─ output : void
   ├─ .x : i32
   │  └─ p : STRUCT_CALL
   └─ char[] "\n"
```

```out
3
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%g.Point = type { i32 }

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

define %g.Point @g.Point.make(i32 %0) {
entry:
  %v = alloca i32, align 4
  store i32 %0, i32* %v, align 4
  %p = alloca %g.Point, align 8
  store %g.Point zeroinitializer, %g.Point* %p, align 4
  %x = getelementptr %g.Point, %g.Point* %p, i32 0, i32 0
  %v1 = load i32, i32* %v, align 4
  store i32 %v1, i32* %x, align 4
  %p2 = load %g.Point, %g.Point* %p, align 4
  ret %g.Point %p2
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %p = alloca %g.Point, align 8
  %call = call %g.Point @g.Point.make(i32 3)
  store %g.Point %call, %g.Point* %p, align 4
  %x = getelementptr %g.Point, %g.Point* %p, i32 0, i32 0
  %x1 = load i32, i32* %x, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %x1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 008 — calling a module member with `.` is an error

```ura
// mod/008 — calling a module member with . is an error
mod lib:
    fn foo() i32:
        return 1

main():
    output(lib.foo(), "\n")
```

```tree
```

```out
```

```err
error: 'lib' is a module; reach its members with '::' instead of '.'
  008.ura:7:12
  |
7 |     output(lib.foo(), "\n")
  |            ^^^
error: Cannot call '.foo()' on unknown; only a struct has methods
  008.ura:7:15
  |
7 |     output(lib.foo(), "\n")
  |               ^
```

```ll
```

## 009 — an unknown member of a known module is an error

```ura
// mod/009 — an unknown member of a known module is an error
mod lib:
    fn foo() i32:
        return 1

main():
    output(lib::bar(), "\n")
```

```tree
```

```out
```

```err
error: Module 'lib' has no member 'bar'
  009.ura:7:12
  |
7 |     output(lib::bar(), "\n")
  |            ^^^
```

```ll
```

## 010 — only declarations are allowed in a module body

```ura
// mod/010 — only declarations are allowed in a module body
mod lib:
    x i32 = 5

main():
    output("hi\n")
```

```tree
```

```out
```

```err
error: Only 'fn', 'struct', 'enum' and 'mod' can be declared in a module
  010.ura:3:11
  |
3 |     x i32 = 5
  |           ^
```

```ll
```

## 011 — a module call is arity-checked

```ura
// mod/011 — a module call is arity-checked
mod lib:
    fn foo(a i32) i32:
        return a

main():
    output(lib::foo(1, 2), "\n")
```

```tree
```

```out
```

```err
error: Wrong number of arguments to 'foo'
  011.ura:7:12
  |
7 |     output(lib::foo(1, 2), "\n")
  |            ^^^
```

```ll
```
