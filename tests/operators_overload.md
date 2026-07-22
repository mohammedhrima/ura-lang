# operators_overload

## index

- 001 — operator + on two structs
- 002 — two overloads of + with different right-hand types
- 003 — operator == returning bool
- 004 — no overload for that operator
- 005 — assigning a different struct
- 006 — indexing an array literal directly
- 007 — operators and a method on the same struct
- 008 — assigning a void result
- 009 — a binary operator with mismatched scalar types
- 010 — operator = on reassignment, but not on declaration
- 011 — compound assignment operators
- 012 — a compound operator with no overload
- 013 — operator drop declares a destructor named `Vec.drop`
- 014 — 'clean' does not destroy a struct, even one with `operator drop`
- 015 — calling `.drop()` yourself is rejected
- 016 — `operator drop` beside `operator =` does not warn
- 017 — `return` unwinds every scope, and keeps the returned local
- 018 — a destructor fires on `break` and on `continue`
- 019 — every operator on one struct, and `=` from an i32 overload
- 020 — drop on fall-through: an if block, each iteration, reverse order
- 021 — a struct drops the structs it owns, at any depth, but not what it borrows
- 022 — a temporary is destroyed at the end of its statement
- 023 — every overloadable operator on one struct
- 024 — bitwise operator overloads, and one returning a scalar
- 025 — compound bitwise assignment overloads
- 026 — bitwise and shift overloads taking a ref parameter
- 027 — a heap-owning struct: create, deep-copy operator = and drop
- 028 — a growable string struct: nullable buffer, grow, join and drop
- 029 — operator output replaces the default struct dump, nested too
- 030 — a struct owning heap buffers: deep-copy operator = and drop

## 001 — operator + on two structs

```ura
// operators_overload/001.ura - operator + on two structs

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

main():
    a Vec = Vec::create(1, 2)
    b Vec = Vec::create(10, 20)
    c Vec = a + b
    output(c.x, " ", c.y, "\n")
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

struct Vec
├─ x : i32
├─ y : i32
├─ fn Vec.create(a : i32, b : i32) : STRUCT_CALL
│  ├─ v : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ .x : i32
│  │  │  └─ v : STRUCT_CALL
│  │  └─ a : i32
│  ├─ = : i32
│  │  ├─ .y : i32
│  │  │  └─ v : STRUCT_CALL
│  │  └─ b : i32
│  └─ return
│     └─ v : STRUCT_CALL
└─ fn Vec.+.Vec(self : STRUCT_CALL, other : STRUCT_CALL) : STRUCT_CALL
   └─ return
      └─ call create : STRUCT_CALL
         ├─ + : i32
         │  ├─ .x : i32
         │  │  └─ self : STRUCT_CALL
         │  └─ .x : i32
         │     └─ other : STRUCT_CALL
         └─ + : i32
            ├─ .y : i32
            │  └─ self : STRUCT_CALL
            └─ .y : i32
               └─ other : STRUCT_CALL

fn main() : i32
├─ = : STRUCT_CALL
│  ├─ a : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     ├─ int 1
│     └─ int 2
├─ = : STRUCT_CALL
│  ├─ b : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     ├─ int 10
│     └─ int 20
├─ = : STRUCT_CALL
│  ├─ c : STRUCT_CALL
│  └─ + : STRUCT_CALL
│     ├─ a : STRUCT_CALL
│     └─ b : STRUCT_CALL
└─ output : void
   ├─ .x : i32
   │  └─ c : STRUCT_CALL
   ├─ char[] " "
   ├─ .y : i32
   │  └─ c : STRUCT_CALL
   └─ char[] "\n"
```

```out
11 22
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Vec = type { i32, i32 }

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

define %Vec @Vec.create(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  %b = alloca i32, align 4
  store i32 %1, i32* %b, align 4
  %v = alloca %Vec, align 8
  store %Vec zeroinitializer, %Vec* %v, align 4
  %x = getelementptr %Vec, %Vec* %v, i32 0, i32 0
  %a1 = load i32, i32* %a, align 4
  store i32 %a1, i32* %x, align 4
  %y = getelementptr %Vec, %Vec* %v, i32 0, i32 1
  %b2 = load i32, i32* %b, align 4
  store i32 %b2, i32* %y, align 4
  %v3 = load %Vec, %Vec* %v, align 4
  ret %Vec %v3
}

define %Vec @"Vec.+.Vec"(%Vec* %0, %Vec %1) {
entry:
  %self = alloca %Vec*, align 8
  store %Vec* %0, %Vec** %self, align 8
  %other = alloca %Vec, align 8
  store %Vec %1, %Vec* %other, align 4
  %ref = load %Vec*, %Vec** %self, align 8
  %x = getelementptr %Vec, %Vec* %ref, i32 0, i32 0
  %x1 = load i32, i32* %x, align 4
  %x2 = getelementptr %Vec, %Vec* %other, i32 0, i32 0
  %x3 = load i32, i32* %x2, align 4
  %add = add i32 %x1, %x3
  %ref4 = load %Vec*, %Vec** %self, align 8
  %y = getelementptr %Vec, %Vec* %ref4, i32 0, i32 1
  %y5 = load i32, i32* %y, align 4
  %y6 = getelementptr %Vec, %Vec* %other, i32 0, i32 1
  %y7 = load i32, i32* %y6, align 4
  %add8 = add i32 %y5, %y7
  %call = call %Vec @Vec.create(i32 %add, i32 %add8)
  ret %Vec %call
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %a = alloca %Vec, align 8
  %call = call %Vec @Vec.create(i32 1, i32 2)
  store %Vec %call, %Vec* %a, align 4
  %b = alloca %Vec, align 8
  %call1 = call %Vec @Vec.create(i32 10, i32 20)
  store %Vec %call1, %Vec* %b, align 4
  %c = alloca %Vec, align 8
  %b2 = load %Vec, %Vec* %b, align 4
  %op = call %Vec @"Vec.+.Vec"(%Vec* %a, %Vec %b2)
  store %Vec %op, %Vec* %c, align 4
  %x = getelementptr %Vec, %Vec* %c, i32 0, i32 0
  %x3 = load i32, i32* %x, align 4
  %y = getelementptr %Vec, %Vec* %c, i32 0, i32 1
  %y4 = load i32, i32* %y, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i32 %x3, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %y4, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 002 — two overloads of + with different right-hand types

```ura
// operators_overload/002.ura - two overloads of + with different RHS types

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

main():
    a Vec = Vec::create(1)
    b Vec = Vec::create(10)
    output((a + b).x, " ", (a + 5).x, "\n")
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

struct Vec
├─ x : i32
├─ fn Vec.create(a : i32) : STRUCT_CALL
│  ├─ v : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ .x : i32
│  │  │  └─ v : STRUCT_CALL
│  │  └─ a : i32
│  └─ return
│     └─ v : STRUCT_CALL
├─ fn Vec.+.Vec(self : STRUCT_CALL, other : STRUCT_CALL) : STRUCT_CALL
│  └─ return
│     └─ call create : STRUCT_CALL
│        └─ + : i32
│           ├─ .x : i32
│           │  └─ self : STRUCT_CALL
│           └─ .x : i32
│              └─ other : STRUCT_CALL
└─ fn Vec.+.i32(self : STRUCT_CALL, n : i32) : STRUCT_CALL
   └─ return
      └─ call create : STRUCT_CALL
         └─ + : i32
            ├─ .x : i32
            │  └─ self : STRUCT_CALL
            └─ n : i32

fn main() : i32
├─ = : STRUCT_CALL
│  ├─ a : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     └─ int 1
├─ = : STRUCT_CALL
│  ├─ b : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     └─ int 10
└─ output : void
   ├─ .x : i32
   │  └─ + : STRUCT_CALL
   │     ├─ a : STRUCT_CALL
   │     └─ b : STRUCT_CALL
   ├─ char[] " "
   ├─ .x : i32
   │  └─ + : STRUCT_CALL
   │     ├─ a : STRUCT_CALL
   │     └─ int 5
   └─ char[] "\n"
```

```out
11 6
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Vec = type { i32 }

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

define %Vec @Vec.create(i32 %0) {
entry:
  %a = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  %v = alloca %Vec, align 8
  store %Vec zeroinitializer, %Vec* %v, align 4
  %x = getelementptr %Vec, %Vec* %v, i32 0, i32 0
  %a1 = load i32, i32* %a, align 4
  store i32 %a1, i32* %x, align 4
  %v2 = load %Vec, %Vec* %v, align 4
  ret %Vec %v2
}

define %Vec @"Vec.+.Vec"(%Vec* %0, %Vec %1) {
entry:
  %self = alloca %Vec*, align 8
  store %Vec* %0, %Vec** %self, align 8
  %other = alloca %Vec, align 8
  store %Vec %1, %Vec* %other, align 4
  %ref = load %Vec*, %Vec** %self, align 8
  %x = getelementptr %Vec, %Vec* %ref, i32 0, i32 0
  %x1 = load i32, i32* %x, align 4
  %x2 = getelementptr %Vec, %Vec* %other, i32 0, i32 0
  %x3 = load i32, i32* %x2, align 4
  %add = add i32 %x1, %x3
  %call = call %Vec @Vec.create(i32 %add)
  ret %Vec %call
}

define %Vec @"Vec.+.i32"(%Vec* %0, i32 %1) {
entry:
  %self = alloca %Vec*, align 8
  store %Vec* %0, %Vec** %self, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %ref = load %Vec*, %Vec** %self, align 8
  %x = getelementptr %Vec, %Vec* %ref, i32 0, i32 0
  %x1 = load i32, i32* %x, align 4
  %n2 = load i32, i32* %n, align 4
  %add = add i32 %x1, %n2
  %call = call %Vec @Vec.create(i32 %add)
  ret %Vec %call
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %a = alloca %Vec, align 8
  %call = call %Vec @Vec.create(i32 1)
  store %Vec %call, %Vec* %a, align 4
  %b = alloca %Vec, align 8
  %call1 = call %Vec @Vec.create(i32 10)
  store %Vec %call1, %Vec* %b, align 4
  %b2 = load %Vec, %Vec* %b, align 4
  %op = call %Vec @"Vec.+.Vec"(%Vec* %a, %Vec %b2)
  %out.tmp = alloca %Vec, align 8
  store %Vec %op, %Vec* %out.tmp, align 4
  %x = getelementptr %Vec, %Vec* %out.tmp, i32 0, i32 0
  %x3 = load i32, i32* %x, align 4
  %op4 = call %Vec @"Vec.+.i32"(%Vec* %a, i32 5)
  %out.tmp5 = alloca %Vec, align 8
  store %Vec %op4, %Vec* %out.tmp5, align 4
  %x6 = getelementptr %Vec, %Vec* %out.tmp5, i32 0, i32 0
  %x7 = load i32, i32* %x6, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i32 %x3, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %x7, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 003 — operator == returning bool

```ura
// operators_overload/003.ura - operator == returning bool

struct Vec:
    x i32

    pub fn create(a i32) Vec:
        v Vec
        v.x = a
        return v

    operator ==(other Vec) bool:
        return self.x == other.x

main():
    a Vec = Vec::create(3)
    b Vec = Vec::create(3)
    c Vec = Vec::create(9)
    output(a == b, " ", a == c, "\n")
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

struct Vec
├─ x : i32
├─ fn Vec.create(a : i32) : STRUCT_CALL
│  ├─ v : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ .x : i32
│  │  │  └─ v : STRUCT_CALL
│  │  └─ a : i32
│  └─ return
│     └─ v : STRUCT_CALL
└─ fn Vec.==.Vec(self : STRUCT_CALL, other : STRUCT_CALL) : bool
   └─ return
      └─ == : bool
         ├─ .x : i32
         │  └─ self : STRUCT_CALL
         └─ .x : i32
            └─ other : STRUCT_CALL

fn main() : i32
├─ = : STRUCT_CALL
│  ├─ a : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     └─ int 3
├─ = : STRUCT_CALL
│  ├─ b : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     └─ int 3
├─ = : STRUCT_CALL
│  ├─ c : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     └─ int 9
└─ output : void
   ├─ == : bool
   │  ├─ a : STRUCT_CALL
   │  └─ b : STRUCT_CALL
   ├─ char[] " "
   ├─ == : bool
   │  ├─ a : STRUCT_CALL
   │  └─ c : STRUCT_CALL
   └─ char[] "\n"
```

```out
True False
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Vec = type { i32 }

@os = internal global %Os zeroinitializer
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@true_str.1 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.2 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [13 x i8] c"%s%.*s%s%.*s\00", align 1

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

define %Vec @Vec.create(i32 %0) {
entry:
  %a = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  %v = alloca %Vec, align 8
  store %Vec zeroinitializer, %Vec* %v, align 4
  %x = getelementptr %Vec, %Vec* %v, i32 0, i32 0
  %a1 = load i32, i32* %a, align 4
  store i32 %a1, i32* %x, align 4
  %v2 = load %Vec, %Vec* %v, align 4
  ret %Vec %v2
}

define i1 @"Vec.==.Vec"(%Vec* %0, %Vec %1) {
entry:
  %self = alloca %Vec*, align 8
  store %Vec* %0, %Vec** %self, align 8
  %other = alloca %Vec, align 8
  store %Vec %1, %Vec* %other, align 4
  %ref = load %Vec*, %Vec** %self, align 8
  %x = getelementptr %Vec, %Vec* %ref, i32 0, i32 0
  %x1 = load i32, i32* %x, align 4
  %x2 = getelementptr %Vec, %Vec* %other, i32 0, i32 0
  %x3 = load i32, i32* %x2, align 4
  %eq = icmp eq i32 %x1, %x3
  ret i1 %eq
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %a = alloca %Vec, align 8
  %call = call %Vec @Vec.create(i32 3)
  store %Vec %call, %Vec* %a, align 4
  %b = alloca %Vec, align 8
  %call1 = call %Vec @Vec.create(i32 3)
  store %Vec %call1, %Vec* %b, align 4
  %c = alloca %Vec, align 8
  %call2 = call %Vec @Vec.create(i32 9)
  store %Vec %call2, %Vec* %c, align 4
  %b3 = load %Vec, %Vec* %b, align 4
  %op = call i1 @"Vec.==.Vec"(%Vec* %a, %Vec %b3)
  %bool_str = select i1 %op, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %c4 = load %Vec, %Vec* %c, align 4
  %op5 = call i1 @"Vec.==.Vec"(%Vec* %a, %Vec %c4)
  %bool_str6 = select i1 %op5, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.1, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.2, i32 0, i32 0)
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i8* %bool_str, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i8* %bool_str6, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 004 — no overload for that operator

```ura
// operators_overload/004.ura - no overload for that operator

struct Vec:
    x i32

main():
    a Vec
    b Vec
    c Vec = a - b
```

```tree
```

```out
```

```err
error: Struct Vec has no 'operator -' taking Vec; declare one inside the struct
  004.ura:9:15
  |
9 |     c Vec = a - b
  |               ^
```

```ll
```

## 005 — assigning a different struct

```ura
// operators_overload/005.ura - assigning a different struct

struct Vec:
    x i32

struct Point:
    x i32

main():
    v Vec
    p Point
    v = p
```

```tree
```

```out
```

```err
error: Cannot assign Point to Vec; they are different structs
   005.ura:12:7
   |
12 |     v = p
   |       ^
```

```ll
```

## 006 — indexing an array literal directly

```ura
// operators_overload/006.ura - indexing an array literal directly

main():
    output([10, 20, 30][1], "\n")
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
   ├─ index : i32
   │  ├─ array : i32[]
   │  │  ├─ int 10
   │  │  ├─ int 20
   │  │  └─ int 30
   │  └─ int 1
   └─ char[] "\n"
```

```out
20
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
  %arr = alloca i32, i64 3, align 4
  %arr.init = getelementptr i32, i32* %arr, i64 0
  store i32 10, i32* %arr.init, align 4
  %arr.init1 = getelementptr i32, i32* %arr, i64 1
  store i32 20, i32* %arr.init1, align 4
  %arr.init2 = getelementptr i32, i32* %arr, i64 2
  store i32 30, i32* %arr.init2, align 4
  %arr.ptr = insertvalue { i32*, i64 } undef, i32* %arr, 0
  %arr.len = insertvalue { i32*, i64 } %arr.ptr, i64 3, 1
  %arr.data = extractvalue { i32*, i64 } %arr.len, 0
  %arr.at = getelementptr i32, i32* %arr.data, i32 1
  %idx = load i32, i32* %arr.at, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %idx, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 007 — operators and a method on the same struct

```ura
// operators_overload/007.ura - operators and a method on the same struct

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

    fn show() void:
        output("(", self.x, ", ", self.y, ")\n")

main():
    a Vec = Vec::create(1, 2)
    b Vec = Vec::create(10, 20)

    c Vec = a + b
    c.show()

    // the same variable on both sides
    output("a == a  ", a == a, "\n")
    output("a == b  ", a == b, "\n")

    // an operator result used directly as a receiver
    (a + b).show()
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

struct Vec
├─ x : i32
├─ y : i32
├─ fn Vec.create(a : i32, b : i32) : STRUCT_CALL
│  ├─ v : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ .x : i32
│  │  │  └─ v : STRUCT_CALL
│  │  └─ a : i32
│  ├─ = : i32
│  │  ├─ .y : i32
│  │  │  └─ v : STRUCT_CALL
│  │  └─ b : i32
│  └─ return
│     └─ v : STRUCT_CALL
├─ fn Vec.+.Vec(self : STRUCT_CALL, other : STRUCT_CALL) : STRUCT_CALL
│  └─ return
│     └─ call create : STRUCT_CALL
│        ├─ + : i32
│        │  ├─ .x : i32
│        │  │  └─ self : STRUCT_CALL
│        │  └─ .x : i32
│        │     └─ other : STRUCT_CALL
│        └─ + : i32
│           ├─ .y : i32
│           │  └─ self : STRUCT_CALL
│           └─ .y : i32
│              └─ other : STRUCT_CALL
├─ fn Vec.==.Vec(self : STRUCT_CALL, other : STRUCT_CALL) : bool
│  └─ return
│     └─ and : bool
│        ├─ == : bool
│        │  ├─ .x : i32
│        │  │  └─ self : STRUCT_CALL
│        │  └─ .x : i32
│        │     └─ other : STRUCT_CALL
│        └─ == : bool
│           ├─ .y : i32
│           │  └─ self : STRUCT_CALL
│           └─ .y : i32
│              └─ other : STRUCT_CALL
└─ fn Vec.show(self : STRUCT_CALL) : void
   └─ output : void
      ├─ char[] "("
      ├─ .x : i32
      │  └─ self : STRUCT_CALL
      ├─ char[] ", "
      ├─ .y : i32
      │  └─ self : STRUCT_CALL
      └─ char[] ")\n"

fn main() : i32
├─ = : STRUCT_CALL
│  ├─ a : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     ├─ int 1
│     └─ int 2
├─ = : STRUCT_CALL
│  ├─ b : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     ├─ int 10
│     └─ int 20
├─ = : STRUCT_CALL
│  ├─ c : STRUCT_CALL
│  └─ + : STRUCT_CALL
│     ├─ a : STRUCT_CALL
│     └─ b : STRUCT_CALL
├─ call show : void
│  └─ c : STRUCT_CALL
├─ output : void
│  ├─ char[] "a == a  "
│  ├─ == : bool
│  │  ├─ a : STRUCT_CALL
│  │  └─ a : STRUCT_CALL
│  └─ char[] "\n"
├─ output : void
│  ├─ char[] "a == b  "
│  ├─ == : bool
│  │  ├─ a : STRUCT_CALL
│  │  └─ b : STRUCT_CALL
│  └─ char[] "\n"
└─ call show : void
   └─ + : STRUCT_CALL
      ├─ a : STRUCT_CALL
      └─ b : STRUCT_CALL
```

```out
(11, 22)
a == a  True
a == b  False
(11, 22)
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Vec = type { i32, i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c"(\00", align 1
@str.1 = private unnamed_addr constant [3 x i8] c", \00", align 1
@str.2 = private unnamed_addr constant [3 x i8] c")\0A\00", align 1
@fmt = private unnamed_addr constant [17 x i8] c"%.*s%d%.*s%d%.*s\00", align 1
@str.3 = private unnamed_addr constant [9 x i8] c"a == a  \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.5 = private unnamed_addr constant [11 x i8] c"%.*s%s%.*s\00", align 1
@str.6 = private unnamed_addr constant [9 x i8] c"a == b  \00", align 1
@true_str.7 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.8 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.10 = private unnamed_addr constant [11 x i8] c"%.*s%s%.*s\00", align 1

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

define %Vec @Vec.create(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  %b = alloca i32, align 4
  store i32 %1, i32* %b, align 4
  %v = alloca %Vec, align 8
  store %Vec zeroinitializer, %Vec* %v, align 4
  %x = getelementptr %Vec, %Vec* %v, i32 0, i32 0
  %a1 = load i32, i32* %a, align 4
  store i32 %a1, i32* %x, align 4
  %y = getelementptr %Vec, %Vec* %v, i32 0, i32 1
  %b2 = load i32, i32* %b, align 4
  store i32 %b2, i32* %y, align 4
  %v3 = load %Vec, %Vec* %v, align 4
  ret %Vec %v3
}

define %Vec @"Vec.+.Vec"(%Vec* %0, %Vec %1) {
entry:
  %self = alloca %Vec*, align 8
  store %Vec* %0, %Vec** %self, align 8
  %other = alloca %Vec, align 8
  store %Vec %1, %Vec* %other, align 4
  %ref = load %Vec*, %Vec** %self, align 8
  %x = getelementptr %Vec, %Vec* %ref, i32 0, i32 0
  %x1 = load i32, i32* %x, align 4
  %x2 = getelementptr %Vec, %Vec* %other, i32 0, i32 0
  %x3 = load i32, i32* %x2, align 4
  %add = add i32 %x1, %x3
  %ref4 = load %Vec*, %Vec** %self, align 8
  %y = getelementptr %Vec, %Vec* %ref4, i32 0, i32 1
  %y5 = load i32, i32* %y, align 4
  %y6 = getelementptr %Vec, %Vec* %other, i32 0, i32 1
  %y7 = load i32, i32* %y6, align 4
  %add8 = add i32 %y5, %y7
  %call = call %Vec @Vec.create(i32 %add, i32 %add8)
  ret %Vec %call
}

define i1 @"Vec.==.Vec"(%Vec* %0, %Vec %1) {
entry:
  %self = alloca %Vec*, align 8
  store %Vec* %0, %Vec** %self, align 8
  %other = alloca %Vec, align 8
  store %Vec %1, %Vec* %other, align 4
  %ref = load %Vec*, %Vec** %self, align 8
  %x = getelementptr %Vec, %Vec* %ref, i32 0, i32 0
  %x1 = load i32, i32* %x, align 4
  %x2 = getelementptr %Vec, %Vec* %other, i32 0, i32 0
  %x3 = load i32, i32* %x2, align 4
  %eq = icmp eq i32 %x1, %x3
  %ref4 = load %Vec*, %Vec** %self, align 8
  %y = getelementptr %Vec, %Vec* %ref4, i32 0, i32 1
  %y5 = load i32, i32* %y, align 4
  %y6 = getelementptr %Vec, %Vec* %other, i32 0, i32 1
  %y7 = load i32, i32* %y6, align 4
  %eq8 = icmp eq i32 %y5, %y7
  %and = and i1 %eq, %eq8
  ret i1 %and
}

define void @Vec.show(%Vec* %0) {
entry:
  %self = alloca %Vec*, align 8
  store %Vec* %0, %Vec** %self, align 8
  %ref = load %Vec*, %Vec** %self, align 8
  %x = getelementptr %Vec, %Vec* %ref, i32 0, i32 0
  %x1 = load i32, i32* %x, align 4
  %ref2 = load %Vec*, %Vec** %self, align 8
  %y = getelementptr %Vec, %Vec* %ref2, i32 0, i32 1
  %y3 = load i32, i32* %y, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %x1, i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0), i32 %y3, i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.2, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)

define i32 @main(i32 %0, i8** %1) {
entry:
  %a = alloca %Vec, align 8
  %call = call %Vec @Vec.create(i32 1, i32 2)
  store %Vec %call, %Vec* %a, align 4
  %b = alloca %Vec, align 8
  %call1 = call %Vec @Vec.create(i32 10, i32 20)
  store %Vec %call1, %Vec* %b, align 4
  %c = alloca %Vec, align 8
  %b2 = load %Vec, %Vec* %b, align 4
  %op = call %Vec @"Vec.+.Vec"(%Vec* %a, %Vec %b2)
  store %Vec %op, %Vec* %c, align 4
  call void @Vec.show(%Vec* %c)
  %a3 = load %Vec, %Vec* %a, align 4
  %op4 = call i1 @"Vec.==.Vec"(%Vec* %a, %Vec %a3)
  %bool_str = select i1 %op4, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.5, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.3, i32 0, i32 0), i8* %bool_str, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  %b5 = load %Vec, %Vec* %b, align 4
  %op6 = call i1 @"Vec.==.Vec"(%Vec* %a, %Vec %b5)
  %bool_str7 = select i1 %op6, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.7, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.8, i32 0, i32 0)
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.10, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.6, i32 0, i32 0), i8* %bool_str7, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.9, i32 0, i32 0))
  %b8 = load %Vec, %Vec* %b, align 4
  %op9 = call %Vec @"Vec.+.Vec"(%Vec* %a, %Vec %b8)
  %out.tmp = alloca %Vec, align 8
  store %Vec %op9, %Vec* %out.tmp, align 4
  call void @Vec.show(%Vec* %out.tmp)
  ret i32 0
}
```

## 008 — assigning a void result

```ura
// operators_overload/008.ura - assigning a void result

struct Vec:
    x i32

    fn show() void:
        output(self.x, "\n")

main():
    a Vec
    e Vec = a.show()
```

```tree
```

```out
```

```err
error: Cannot assign void to Vec
   008.ura:11:11
   |
11 |     e Vec = a.show()
   |           ^
```

```ll
```

## 009 — a binary operator with mismatched scalar types

```ura
// operators_overload/009.ura - a binary operator with mismatched scalar types

main():
    n i32 = 1
    s char[] = "hi"
    output(n + s, "\n")
```

```tree
```

```out
```

```err
error: Cannot use '+' with i32 and array
  009.ura:6:14
  |
6 |     output(n + s, "\n")
  |              ^
```

```ll
```

## 010 — operator = on reassignment, but not on declaration

```ura
// operators_overload/010.ura - operator = on reassignment, not declaration

struct Vec:
    x i32
    tag i32

    operator =(other Vec) void:
        self.x   = other.x
        self.tag = 99

main():
    a Vec
    a.x = 5

    // declaration: bitwise copy, tag stays 0
    b Vec = a
    output("decl   ", b.x, " ", b.tag, "\n")

    // reassignment: calls operator =, tag becomes 99
    c Vec
    c = a
    output("assign ", c.x, " ", c.tag, "\n")
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

struct Vec
├─ x : i32
├─ tag : i32
└─ fn Vec.=.Vec(self : STRUCT_CALL, other : STRUCT_CALL) : void
   ├─ = : i32
   │  ├─ .x : i32
   │  │  └─ self : STRUCT_CALL
   │  └─ .x : i32
   │     └─ other : STRUCT_CALL
   └─ = : i32
      ├─ .tag : i32
      │  └─ self : STRUCT_CALL
      └─ int 99

fn main() : i32
├─ a : STRUCT_CALL
├─ = : i32
│  ├─ .x : i32
│  │  └─ a : STRUCT_CALL
│  └─ int 5
├─ = : STRUCT_CALL
│  ├─ b : STRUCT_CALL
│  └─ a : STRUCT_CALL
├─ output : void
│  ├─ char[] "decl   "
│  ├─ .x : i32
│  │  └─ b : STRUCT_CALL
│  ├─ char[] " "
│  ├─ .tag : i32
│  │  └─ b : STRUCT_CALL
│  └─ char[] "\n"
├─ c : STRUCT_CALL
├─ = : void
│  ├─ c : STRUCT_CALL
│  └─ a : STRUCT_CALL
└─ output : void
   ├─ char[] "assign "
   ├─ .x : i32
   │  └─ c : STRUCT_CALL
   ├─ char[] " "
   ├─ .tag : i32
   │  └─ c : STRUCT_CALL
   └─ char[] "\n"
```

```out
decl   5 0
assign 5 99
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Vec = type { i32, i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [8 x i8] c"decl   \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [17 x i8] c"%.*s%d%.*s%d%.*s\00", align 1
@str.3 = private unnamed_addr constant [8 x i8] c"assign \00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [17 x i8] c"%.*s%d%.*s%d%.*s\00", align 1

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

define void @"Vec.=.Vec"(%Vec* %0, %Vec %1) {
entry:
  %self = alloca %Vec*, align 8
  store %Vec* %0, %Vec** %self, align 8
  %other = alloca %Vec, align 8
  store %Vec %1, %Vec* %other, align 4
  %ref = load %Vec*, %Vec** %self, align 8
  %x = getelementptr %Vec, %Vec* %ref, i32 0, i32 0
  %x1 = getelementptr %Vec, %Vec* %other, i32 0, i32 0
  %x2 = load i32, i32* %x1, align 4
  store i32 %x2, i32* %x, align 4
  %ref3 = load %Vec*, %Vec** %self, align 8
  %tag = getelementptr %Vec, %Vec* %ref3, i32 0, i32 1
  store i32 99, i32* %tag, align 4
  ret void
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %a = alloca %Vec, align 8
  store %Vec zeroinitializer, %Vec* %a, align 4
  %x = getelementptr %Vec, %Vec* %a, i32 0, i32 0
  store i32 5, i32* %x, align 4
  %b = alloca %Vec, align 8
  %a1 = load %Vec, %Vec* %a, align 4
  store %Vec %a1, %Vec* %b, align 4
  %x2 = getelementptr %Vec, %Vec* %b, i32 0, i32 0
  %x3 = load i32, i32* %x2, align 4
  %tag = getelementptr %Vec, %Vec* %b, i32 0, i32 1
  %tag4 = load i32, i32* %tag, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt, i32 0, i32 0), i32 7, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str, i32 0, i32 0), i32 %x3, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i32 %tag4, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %c = alloca %Vec, align 8
  store %Vec zeroinitializer, %Vec* %c, align 4
  %a5 = load %Vec, %Vec* %a, align 4
  call void @"Vec.=.Vec"(%Vec* %c, %Vec %a5)
  %x6 = getelementptr %Vec, %Vec* %c, i32 0, i32 0
  %x7 = load i32, i32* %x6, align 4
  %tag8 = getelementptr %Vec, %Vec* %c, i32 0, i32 1
  %tag9 = load i32, i32* %tag8, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt.6, i32 0, i32 0), i32 7, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.3, i32 0, i32 0), i32 %x7, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0), i32 %tag9, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 011 — compound assignment operators

```ura
// operators_overload/011.ura - compound assignment operators

struct Vec:
    x i32

    operator +=(other Vec) void:
        self.x = self.x + other.x

    operator -=(n i32) void:
        self.x = self.x - n

main():
    a Vec
    a.x = 10
    b Vec
    b.x = 4

    a += b
    output("after += ", a.x, "\n")

    a -= 3
    output("after -= ", a.x, "\n")
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

struct Vec
├─ x : i32
├─ fn Vec.+=.Vec(self : STRUCT_CALL, other : STRUCT_CALL) : void
│  └─ = : i32
│     ├─ .x : i32
│     │  └─ self : STRUCT_CALL
│     └─ + : i32
│        ├─ .x : i32
│        │  └─ self : STRUCT_CALL
│        └─ .x : i32
│           └─ other : STRUCT_CALL
└─ fn Vec.-=.i32(self : STRUCT_CALL, n : i32) : void
   └─ = : i32
      ├─ .x : i32
      │  └─ self : STRUCT_CALL
      └─ - : i32
         ├─ .x : i32
         │  └─ self : STRUCT_CALL
         └─ n : i32

fn main() : i32
├─ a : STRUCT_CALL
├─ = : i32
│  ├─ .x : i32
│  │  └─ a : STRUCT_CALL
│  └─ int 10
├─ b : STRUCT_CALL
├─ = : i32
│  ├─ .x : i32
│  │  └─ b : STRUCT_CALL
│  └─ int 4
├─ += : void
│  ├─ a : STRUCT_CALL
│  └─ b : STRUCT_CALL
├─ output : void
│  ├─ char[] "after += "
│  ├─ .x : i32
│  │  └─ a : STRUCT_CALL
│  └─ char[] "\n"
├─ -= : void
│  ├─ a : STRUCT_CALL
│  └─ int 3
└─ output : void
   ├─ char[] "after -= "
   ├─ .x : i32
   │  └─ a : STRUCT_CALL
   └─ char[] "\n"
```

```out
after += 14
after -= 11
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Vec = type { i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [10 x i8] c"after += \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.2 = private unnamed_addr constant [10 x i8] c"after -= \00", align 1
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

define void @"Vec.+=.Vec"(%Vec* %0, %Vec %1) {
entry:
  %self = alloca %Vec*, align 8
  store %Vec* %0, %Vec** %self, align 8
  %other = alloca %Vec, align 8
  store %Vec %1, %Vec* %other, align 4
  %ref = load %Vec*, %Vec** %self, align 8
  %x = getelementptr %Vec, %Vec* %ref, i32 0, i32 0
  %ref1 = load %Vec*, %Vec** %self, align 8
  %x2 = getelementptr %Vec, %Vec* %ref1, i32 0, i32 0
  %x3 = load i32, i32* %x2, align 4
  %x4 = getelementptr %Vec, %Vec* %other, i32 0, i32 0
  %x5 = load i32, i32* %x4, align 4
  %add = add i32 %x3, %x5
  store i32 %add, i32* %x, align 4
  ret void
}

define void @"Vec.-=.i32"(%Vec* %0, i32 %1) {
entry:
  %self = alloca %Vec*, align 8
  store %Vec* %0, %Vec** %self, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %ref = load %Vec*, %Vec** %self, align 8
  %x = getelementptr %Vec, %Vec* %ref, i32 0, i32 0
  %ref1 = load %Vec*, %Vec** %self, align 8
  %x2 = getelementptr %Vec, %Vec* %ref1, i32 0, i32 0
  %x3 = load i32, i32* %x2, align 4
  %n4 = load i32, i32* %n, align 4
  %sub = sub i32 %x3, %n4
  store i32 %sub, i32* %x, align 4
  ret void
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %a = alloca %Vec, align 8
  store %Vec zeroinitializer, %Vec* %a, align 4
  %x = getelementptr %Vec, %Vec* %a, i32 0, i32 0
  store i32 10, i32* %x, align 4
  %b = alloca %Vec, align 8
  store %Vec zeroinitializer, %Vec* %b, align 4
  %x1 = getelementptr %Vec, %Vec* %b, i32 0, i32 0
  store i32 4, i32* %x1, align 4
  %b2 = load %Vec, %Vec* %b, align 4
  call void @"Vec.+=.Vec"(%Vec* %a, %Vec %b2)
  %x3 = getelementptr %Vec, %Vec* %a, i32 0, i32 0
  %x4 = load i32, i32* %x3, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 9, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %x4, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  call void @"Vec.-=.i32"(%Vec* %a, i32 3)
  %x5 = getelementptr %Vec, %Vec* %a, i32 0, i32 0
  %x6 = load i32, i32* %x5, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.4, i32 0, i32 0), i32 9, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.2, i32 0, i32 0), i32 %x6, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 012 — a compound operator with no overload

```ura
// operators_overload/012.ura - a compound operator with no overload

struct Vec:
    x i32

main():
    a Vec
    b Vec
    a *= b
```

```tree
```

```out
```

```err
error: Struct Vec has no 'operator *=' taking Vec; declare one inside the struct
  012.ura:9:7
  |
9 |     a *= b
  |       ^^
```

```ll
```

## 013 — operator drop declares a destructor named `Vec.drop`

```ura
// operators_overload/013.ura - operator drop declares Vec.drop

struct Vec:
    x i32

    operator drop:
        output("dropping ", self.x, "\n")

main():
    v Vec
    v.x = 7
    output("done\n")
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

struct Vec
├─ x : i32
└─ fn Vec.drop(self : STRUCT_CALL) : void
   └─ output : void
      ├─ char[] "dropping "
      ├─ .x : i32
      │  └─ self : STRUCT_CALL
      └─ char[] "\n"

fn main() : i32
├─ v : STRUCT_CALL
├─ = : i32
│  ├─ .x : i32
│  │  └─ v : STRUCT_CALL
│  └─ int 7
└─ output : void
   └─ char[] "done\n"
```

```out
done
dropping 7
```

```err
warning: Struct Vec has 'operator drop' but no 'operator ='; copying it makes two owners of the same memory, and both will be destroyed
  013.ura:3:1
  |
3 | struct Vec:
  | ^^^^^^
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Vec = type { i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [10 x i8] c"dropping \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"done\0A\00", align 1
@fmt.3 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

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

define void @Vec.drop(%Vec* %0) {
entry:
  %self = alloca %Vec*, align 8
  store %Vec* %0, %Vec** %self, align 8
  %ref = load %Vec*, %Vec** %self, align 8
  %x = getelementptr %Vec, %Vec* %ref, i32 0, i32 0
  %x1 = load i32, i32* %x, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 9, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %x1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)

define i32 @main(i32 %0, i8** %1) {
entry:
  %v = alloca %Vec, align 8
  store %Vec zeroinitializer, %Vec* %v, align 4
  %x = getelementptr %Vec, %Vec* %v, i32 0, i32 0
  store i32 7, i32* %x, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.3, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0))
  call void @Vec.drop(%Vec* %v)
  ret i32 0
}
```

## 014 — 'clean' does not destroy a struct, even one with `operator drop`

```ura
// operators_overload/014.ura - clean does not destroy a struct

struct Vec:
    x i32

    operator drop:
        output("dropping\n")

main():
    v Vec
    clean v
```

```tree
```

```out
```

```err
warning: Struct Vec has 'operator drop' but no 'operator ='; copying it makes two owners of the same memory, and both will be destroyed
  014.ura:3:1
  |
3 | struct Vec:
  | ^^^^^^
error: 'clean' frees a heap array, not the struct Vec; a struct is destroyed at end of scope by its 'operator drop'
   014.ura:11:5
   |
11 |     clean v
   |     ^^^^^
```

```ll
```

## 015 — calling `.drop()` yourself is rejected

```ura
// operators_overload/015.ura - calling .drop() yourself is rejected

struct Vec:
    x i32

    operator drop:
        output("dropping\n")

main():
    v Vec
    v.drop()
```

```tree
```

```out
```

```err
warning: Struct Vec has 'operator drop' but no 'operator ='; copying it makes two owners of the same memory, and both will be destroyed
  015.ura:3:1
  |
3 | struct Vec:
  | ^^^^^^
error: Cannot call 'Vec.drop()' yourself; the compiler runs 'operator drop' when the value leaves scope
   015.ura:11:6
   |
11 |     v.drop()
   |      ^
```

```ll
```

## 016 — `operator drop` beside `operator =` does not warn

```ura
// operators_overload/016.ura - drop beside operator = does not warn

struct Vec:
    x i32

    operator =(other Vec) void:
        self.x = other.x

    operator drop:
        output("drop ", self.x, "\n")

main():
    a Vec
    a.x = 5
    output("done\n")
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

struct Vec
├─ x : i32
├─ fn Vec.=.Vec(self : STRUCT_CALL, other : STRUCT_CALL) : void
│  └─ = : i32
│     ├─ .x : i32
│     │  └─ self : STRUCT_CALL
│     └─ .x : i32
│        └─ other : STRUCT_CALL
└─ fn Vec.drop(self : STRUCT_CALL) : void
   └─ output : void
      ├─ char[] "drop "
      ├─ .x : i32
      │  └─ self : STRUCT_CALL
      └─ char[] "\n"

fn main() : i32
├─ a : STRUCT_CALL
├─ = : i32
│  ├─ .x : i32
│  │  └─ a : STRUCT_CALL
│  └─ int 5
└─ output : void
   └─ char[] "done\n"
```

```out
done
drop 5
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Vec = type { i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [6 x i8] c"drop \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"done\0A\00", align 1
@fmt.3 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

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

define void @"Vec.=.Vec"(%Vec* %0, %Vec %1) {
entry:
  %self = alloca %Vec*, align 8
  store %Vec* %0, %Vec** %self, align 8
  %other = alloca %Vec, align 8
  store %Vec %1, %Vec* %other, align 4
  %ref = load %Vec*, %Vec** %self, align 8
  %x = getelementptr %Vec, %Vec* %ref, i32 0, i32 0
  %x1 = getelementptr %Vec, %Vec* %other, i32 0, i32 0
  %x2 = load i32, i32* %x1, align 4
  store i32 %x2, i32* %x, align 4
  ret void
}

define void @Vec.drop(%Vec* %0) {
entry:
  %self = alloca %Vec*, align 8
  store %Vec* %0, %Vec** %self, align 8
  %ref = load %Vec*, %Vec** %self, align 8
  %x = getelementptr %Vec, %Vec* %ref, i32 0, i32 0
  %x1 = load i32, i32* %x, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %x1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)

define i32 @main(i32 %0, i8** %1) {
entry:
  %a = alloca %Vec, align 8
  store %Vec zeroinitializer, %Vec* %a, align 4
  %x = getelementptr %Vec, %Vec* %a, i32 0, i32 0
  store i32 5, i32* %x, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.3, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0))
  call void @Vec.drop(%Vec* %a)
  ret i32 0
}
```

## 017 — `return` unwinds every scope, and keeps the returned local

```ura
// operators_overload/017.ura - return unwinds scopes, keeps the returned local

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

main():
    r i32 = early()
    output("early ", r, "\n")
    g Tag = giveback()
    output("g.id ", g.id, "\n")
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

struct Tag
├─ id : i32
├─ fn Tag.create(n : i32) : STRUCT_CALL
│  ├─ t : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ .id : i32
│  │  │  └─ t : STRUCT_CALL
│  │  └─ n : i32
│  └─ return
│     └─ t : STRUCT_CALL
├─ fn Tag.=.Tag(self : STRUCT_CALL, other : STRUCT_CALL) : void
│  └─ = : i32
│     ├─ .id : i32
│     │  └─ self : STRUCT_CALL
│     └─ .id : i32
│        └─ other : STRUCT_CALL
└─ fn Tag.drop(self : STRUCT_CALL) : void
   └─ output : void
      ├─ char[] "drop "
      ├─ .id : i32
      │  └─ self : STRUCT_CALL
      └─ char[] "\n"

fn early() : i32
├─ = : STRUCT_CALL
│  ├─ a : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     └─ int 1
├─ if
│  ├─ condition < : bool
│  │  ├─ int 1
│  │  └─ int 2
│  ├─ = : STRUCT_CALL
│  │  ├─ b : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  │     └─ int 10
│  └─ return
│     └─ int 7
└─ return
   └─ int 0

fn giveback() : STRUCT_CALL
├─ = : STRUCT_CALL
│  ├─ t : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     └─ int 99
└─ return
   └─ t : STRUCT_CALL

fn main() : i32
├─ = : i32
│  ├─ r : i32
│  └─ call early : i32
├─ output : void
│  ├─ char[] "early "
│  ├─ r : i32
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ g : STRUCT_CALL
│  └─ call giveback : STRUCT_CALL
└─ output : void
   ├─ char[] "g.id "
   ├─ .id : i32
   │  └─ g : STRUCT_CALL
   └─ char[] "\n"
```

```out
drop 10
drop 1
early 7
g.id 99
drop 99
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Tag = type { i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [6 x i8] c"drop \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.2 = private unnamed_addr constant [7 x i8] c"early \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.5 = private unnamed_addr constant [6 x i8] c"g.id \00", align 1
@str.6 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.7 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1

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

define %Tag @Tag.create(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %t = alloca %Tag, align 8
  store %Tag zeroinitializer, %Tag* %t, align 4
  %id = getelementptr %Tag, %Tag* %t, i32 0, i32 0
  %n1 = load i32, i32* %n, align 4
  store i32 %n1, i32* %id, align 4
  %t2 = load %Tag, %Tag* %t, align 4
  ret %Tag %t2
}

define void @"Tag.=.Tag"(%Tag* %0, %Tag %1) {
entry:
  %self = alloca %Tag*, align 8
  store %Tag* %0, %Tag** %self, align 8
  %other = alloca %Tag, align 8
  store %Tag %1, %Tag* %other, align 4
  %ref = load %Tag*, %Tag** %self, align 8
  %id = getelementptr %Tag, %Tag* %ref, i32 0, i32 0
  %id1 = getelementptr %Tag, %Tag* %other, i32 0, i32 0
  %id2 = load i32, i32* %id1, align 4
  store i32 %id2, i32* %id, align 4
  ret void
}

define void @Tag.drop(%Tag* %0) {
entry:
  %self = alloca %Tag*, align 8
  store %Tag* %0, %Tag** %self, align 8
  %ref = load %Tag*, %Tag** %self, align 8
  %id = getelementptr %Tag, %Tag* %ref, i32 0, i32 0
  %id1 = load i32, i32* %id, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %id1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)

define i32 @early() {
entry:
  %a = alloca %Tag, align 8
  %call = call %Tag @Tag.create(i32 1)
  store %Tag %call, %Tag* %a, align 4
  br i1 true, label %then, label %endif

endif:                                            ; preds = %entry
  call void @Tag.drop(%Tag* %a)
  ret i32 0

then:                                             ; preds = %entry
  %b = alloca %Tag, align 8
  %call1 = call %Tag @Tag.create(i32 10)
  store %Tag %call1, %Tag* %b, align 4
  call void @Tag.drop(%Tag* %b)
  call void @Tag.drop(%Tag* %a)
  ret i32 7
}

define %Tag @giveback() {
entry:
  %t = alloca %Tag, align 8
  %call = call %Tag @Tag.create(i32 99)
  store %Tag %call, %Tag* %t, align 4
  %t1 = load %Tag, %Tag* %t, align 4
  ret %Tag %t1
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %r = alloca i32, align 4
  %call = call i32 @early()
  store i32 %call, i32* %r, align 4
  %r1 = load i32, i32* %r, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.4, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.2, i32 0, i32 0), i32 %r1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %g = alloca %Tag, align 8
  %call2 = call %Tag @giveback()
  store %Tag %call2, %Tag* %g, align 4
  %id = getelementptr %Tag, %Tag* %g, i32 0, i32 0
  %id3 = load i32, i32* %id, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.7, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.5, i32 0, i32 0), i32 %id3, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.6, i32 0, i32 0))
  call void @Tag.drop(%Tag* %g)
  ret i32 0
}
```

## 018 — a destructor fires on `break` and on `continue`

```ura
// operators_overload/018.ura - destructor fires on break and continue

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

main():
    i i32 = 0
    while i < 3:
        b Tag = Tag::create(100 + i)
        if i == 1:
            break
        i = i + 1
    output("after break\n")

    k i32 = 0
    while k < 2:
        c Tag = Tag::create(200 + k)
        k = k + 1
        continue
    output("after continue\n")
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

struct Tag
├─ id : i32
├─ fn Tag.create(n : i32) : STRUCT_CALL
│  ├─ t : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ .id : i32
│  │  │  └─ t : STRUCT_CALL
│  │  └─ n : i32
│  └─ return
│     └─ t : STRUCT_CALL
├─ fn Tag.=.Tag(self : STRUCT_CALL, other : STRUCT_CALL) : void
│  └─ = : i32
│     ├─ .id : i32
│     │  └─ self : STRUCT_CALL
│     └─ .id : i32
│        └─ other : STRUCT_CALL
└─ fn Tag.drop(self : STRUCT_CALL) : void
   └─ output : void
      ├─ char[] "drop "
      ├─ .id : i32
      │  └─ self : STRUCT_CALL
      └─ char[] "\n"

fn main() : i32
├─ = : i32
│  ├─ i : i32
│  └─ int 0
├─ while
│  ├─ condition < : bool
│  │  ├─ i : i32
│  │  └─ int 3
│  ├─ = : STRUCT_CALL
│  │  ├─ b : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  │     └─ + : i32
│  │        ├─ int 100
│  │        └─ i : i32
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ i : i32
│  │  │  └─ int 1
│  │  └─ break
│  └─ = : i32
│     ├─ i : i32
│     └─ + : i32
│        ├─ i : i32
│        └─ int 1
├─ output : void
│  └─ char[] "after break\n"
├─ = : i32
│  ├─ k : i32
│  └─ int 0
├─ while
│  ├─ condition < : bool
│  │  ├─ k : i32
│  │  └─ int 2
│  ├─ = : STRUCT_CALL
│  │  ├─ c : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  │     └─ + : i32
│  │        ├─ int 200
│  │        └─ k : i32
│  ├─ = : i32
│  │  ├─ k : i32
│  │  └─ + : i32
│  │     ├─ k : i32
│  │     └─ int 1
│  └─ continue
└─ output : void
   └─ char[] "after continue\n"
```

```out
drop 100
drop 101
after break
drop 200
drop 201
after continue
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Tag = type { i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [6 x i8] c"drop \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.2 = private unnamed_addr constant [13 x i8] c"after break\0A\00", align 1
@fmt.3 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.4 = private unnamed_addr constant [16 x i8] c"after continue\0A\00", align 1
@fmt.5 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

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

define %Tag @Tag.create(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %t = alloca %Tag, align 8
  store %Tag zeroinitializer, %Tag* %t, align 4
  %id = getelementptr %Tag, %Tag* %t, i32 0, i32 0
  %n1 = load i32, i32* %n, align 4
  store i32 %n1, i32* %id, align 4
  %t2 = load %Tag, %Tag* %t, align 4
  ret %Tag %t2
}

define void @"Tag.=.Tag"(%Tag* %0, %Tag %1) {
entry:
  %self = alloca %Tag*, align 8
  store %Tag* %0, %Tag** %self, align 8
  %other = alloca %Tag, align 8
  store %Tag %1, %Tag* %other, align 4
  %ref = load %Tag*, %Tag** %self, align 8
  %id = getelementptr %Tag, %Tag* %ref, i32 0, i32 0
  %id1 = getelementptr %Tag, %Tag* %other, i32 0, i32 0
  %id2 = load i32, i32* %id1, align 4
  store i32 %id2, i32* %id, align 4
  ret void
}

define void @Tag.drop(%Tag* %0) {
entry:
  %self = alloca %Tag*, align 8
  store %Tag* %0, %Tag** %self, align 8
  %ref = load %Tag*, %Tag** %self, align 8
  %id = getelementptr %Tag, %Tag* %ref, i32 0, i32 0
  %id1 = load i32, i32* %id, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %id1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)

define i32 @main(i32 %0, i8** %1) {
entry:
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %endif, %entry
  %i1 = load i32, i32* %i, align 4
  %lt = icmp slt i32 %i1, 3
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %b = alloca %Tag, align 8
  %i2 = load i32, i32* %i, align 4
  %add = add i32 100, %i2
  %call = call %Tag @Tag.create(i32 %add)
  store %Tag %call, %Tag* %b, align 4
  %i3 = load i32, i32* %i, align 4
  %eq = icmp eq i32 %i3, 1
  br i1 %eq, label %then, label %endif

while.end:                                        ; preds = %then, %while.cond
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.3, i32 0, i32 0), i32 12, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.2, i32 0, i32 0))
  %k = alloca i32, align 4
  store i32 0, i32* %k, align 4
  br label %while.cond6

endif:                                            ; preds = %while.body
  %i4 = load i32, i32* %i, align 4
  %add5 = add i32 %i4, 1
  store i32 %add5, i32* %i, align 4
  call void @Tag.drop(%Tag* %b)
  br label %while.cond

then:                                             ; preds = %while.body
  call void @Tag.drop(%Tag* %b)
  br label %while.end

while.cond6:                                      ; preds = %while.body7, %while.end
  %k9 = load i32, i32* %k, align 4
  %lt10 = icmp slt i32 %k9, 2
  br i1 %lt10, label %while.body7, label %while.end8

while.body7:                                      ; preds = %while.cond6
  %c = alloca %Tag, align 8
  %k11 = load i32, i32* %k, align 4
  %add12 = add i32 200, %k11
  %call13 = call %Tag @Tag.create(i32 %add12)
  store %Tag %call13, %Tag* %c, align 4
  %k14 = load i32, i32* %k, align 4
  %add15 = add i32 %k14, 1
  store i32 %add15, i32* %k, align 4
  call void @Tag.drop(%Tag* %c)
  br label %while.cond6

while.end8:                                       ; preds = %while.cond6
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.5, i32 0, i32 0), i32 15, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.4, i32 0, i32 0))
  ret i32 0
}
```

## 019 — every operator on one struct, and `=` from an i32 overload

```ura
// operators_overload/019.ura - every operator on one struct

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

    fn show() void:
        output("(", self.x, ", ", self.y, ") tag ", self.tag, "\n")

main():
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

struct Vec
├─ x : i32
├─ y : i32
├─ tag : i32
├─ fn Vec.create(a : i32, b : i32) : STRUCT_CALL
│  ├─ v : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ .x : i32
│  │  │  └─ v : STRUCT_CALL
│  │  └─ a : i32
│  ├─ = : i32
│  │  ├─ .y : i32
│  │  │  └─ v : STRUCT_CALL
│  │  └─ b : i32
│  └─ return
│     └─ v : STRUCT_CALL
├─ fn Vec.+.Vec(self : STRUCT_CALL, other : STRUCT_CALL) : STRUCT_CALL
│  └─ return
│     └─ call create : STRUCT_CALL
│        ├─ + : i32
│        │  ├─ .x : i32
│        │  │  └─ self : STRUCT_CALL
│        │  └─ .x : i32
│        │     └─ other : STRUCT_CALL
│        └─ + : i32
│           ├─ .y : i32
│           │  └─ self : STRUCT_CALL
│           └─ .y : i32
│              └─ other : STRUCT_CALL
├─ fn Vec.+.i32(self : STRUCT_CALL, n : i32) : STRUCT_CALL
│  └─ return
│     └─ call create : STRUCT_CALL
│        ├─ + : i32
│        │  ├─ .x : i32
│        │  │  └─ self : STRUCT_CALL
│        │  └─ n : i32
│        └─ + : i32
│           ├─ .y : i32
│           │  └─ self : STRUCT_CALL
│           └─ n : i32
├─ fn Vec.==.Vec(self : STRUCT_CALL, other : STRUCT_CALL) : bool
│  └─ return
│     └─ and : bool
│        ├─ == : bool
│        │  ├─ .x : i32
│        │  │  └─ self : STRUCT_CALL
│        │  └─ .x : i32
│        │     └─ other : STRUCT_CALL
│        └─ == : bool
│           ├─ .y : i32
│           │  └─ self : STRUCT_CALL
│           └─ .y : i32
│              └─ other : STRUCT_CALL
├─ fn Vec.=.Vec(self : STRUCT_CALL, other : STRUCT_CALL) : void
│  ├─ = : i32
│  │  ├─ .x : i32
│  │  │  └─ self : STRUCT_CALL
│  │  └─ .x : i32
│  │     └─ other : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ .y : i32
│  │  │  └─ self : STRUCT_CALL
│  │  └─ .y : i32
│  │     └─ other : STRUCT_CALL
│  └─ = : i32
│     ├─ .tag : i32
│     │  └─ self : STRUCT_CALL
│     └─ int 99
├─ fn Vec.+=.Vec(self : STRUCT_CALL, other : STRUCT_CALL) : void
│  ├─ = : i32
│  │  ├─ .x : i32
│  │  │  └─ self : STRUCT_CALL
│  │  └─ + : i32
│  │     ├─ .x : i32
│  │     │  └─ self : STRUCT_CALL
│  │     └─ .x : i32
│  │        └─ other : STRUCT_CALL
│  └─ = : i32
│     ├─ .y : i32
│     │  └─ self : STRUCT_CALL
│     └─ + : i32
│        ├─ .y : i32
│        │  └─ self : STRUCT_CALL
│        └─ .y : i32
│           └─ other : STRUCT_CALL
├─ fn Vec.-=.i32(self : STRUCT_CALL, n : i32) : void
│  ├─ = : i32
│  │  ├─ .x : i32
│  │  │  └─ self : STRUCT_CALL
│  │  └─ - : i32
│  │     ├─ .x : i32
│  │     │  └─ self : STRUCT_CALL
│  │     └─ n : i32
│  └─ = : i32
│     ├─ .y : i32
│     │  └─ self : STRUCT_CALL
│     └─ - : i32
│        ├─ .y : i32
│        │  └─ self : STRUCT_CALL
│        └─ n : i32
└─ fn Vec.show(self : STRUCT_CALL) : void
   └─ output : void
      ├─ char[] "("
      ├─ .x : i32
      │  └─ self : STRUCT_CALL
      ├─ char[] ", "
      ├─ .y : i32
      │  └─ self : STRUCT_CALL
      ├─ char[] ") tag "
      ├─ .tag : i32
      │  └─ self : STRUCT_CALL
      └─ char[] "\n"

fn main() : i32
├─ = : STRUCT_CALL
│  ├─ a : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     ├─ int 1
│     └─ int 2
├─ = : STRUCT_CALL
│  ├─ b : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     ├─ int 10
│     └─ int 20
├─ = : STRUCT_CALL
│  ├─ c : STRUCT_CALL
│  └─ + : STRUCT_CALL
│     ├─ a : STRUCT_CALL
│     └─ b : STRUCT_CALL
├─ call show : void
│  └─ c : STRUCT_CALL
├─ = : STRUCT_CALL
│  ├─ d : STRUCT_CALL
│  └─ + : STRUCT_CALL
│     ├─ a : STRUCT_CALL
│     └─ int 5
├─ call show : void
│  └─ d : STRUCT_CALL
├─ output : void
│  ├─ char[] "a == a  "
│  ├─ == : bool
│  │  ├─ a : STRUCT_CALL
│  │  └─ a : STRUCT_CALL
│  └─ char[] "\n"
├─ output : void
│  ├─ char[] "a == b  "
│  ├─ == : bool
│  │  ├─ a : STRUCT_CALL
│  │  └─ b : STRUCT_CALL
│  └─ char[] "\n"
├─ call show : void
│  └─ + : STRUCT_CALL
│     ├─ a : STRUCT_CALL
│     └─ b : STRUCT_CALL
├─ = : STRUCT_CALL
│  ├─ e : STRUCT_CALL
│  └─ a : STRUCT_CALL
├─ call show : void
│  └─ e : STRUCT_CALL
├─ f : STRUCT_CALL
├─ = : void
│  ├─ f : STRUCT_CALL
│  └─ a : STRUCT_CALL
├─ call show : void
│  └─ f : STRUCT_CALL
├─ = : STRUCT_CALL
│  ├─ g : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     ├─ int 10
│     └─ int 10
├─ += : void
│  ├─ g : STRUCT_CALL
│  └─ b : STRUCT_CALL
├─ call show : void
│  └─ g : STRUCT_CALL
├─ -= : void
│  ├─ g : STRUCT_CALL
│  └─ int 3
└─ call show : void
   └─ g : STRUCT_CALL
```

```out
(11, 22) tag 0
(6, 7) tag 0
a == a  True
a == b  False
(11, 22) tag 0
(1, 2) tag 0
(1, 2) tag 99
(20, 30) tag 0
(17, 27) tag 0
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Vec = type { i32, i32, i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c"(\00", align 1
@str.1 = private unnamed_addr constant [3 x i8] c", \00", align 1
@str.2 = private unnamed_addr constant [7 x i8] c") tag \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [23 x i8] c"%.*s%d%.*s%d%.*s%d%.*s\00", align 1
@str.4 = private unnamed_addr constant [9 x i8] c"a == a  \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [11 x i8] c"%.*s%s%.*s\00", align 1
@str.7 = private unnamed_addr constant [9 x i8] c"a == b  \00", align 1
@true_str.8 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.9 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.10 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.11 = private unnamed_addr constant [11 x i8] c"%.*s%s%.*s\00", align 1

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

define %Vec @Vec.create(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  %b = alloca i32, align 4
  store i32 %1, i32* %b, align 4
  %v = alloca %Vec, align 8
  store %Vec zeroinitializer, %Vec* %v, align 4
  %x = getelementptr %Vec, %Vec* %v, i32 0, i32 0
  %a1 = load i32, i32* %a, align 4
  store i32 %a1, i32* %x, align 4
  %y = getelementptr %Vec, %Vec* %v, i32 0, i32 1
  %b2 = load i32, i32* %b, align 4
  store i32 %b2, i32* %y, align 4
  %v3 = load %Vec, %Vec* %v, align 4
  ret %Vec %v3
}

define %Vec @"Vec.+.Vec"(%Vec* %0, %Vec %1) {
entry:
  %self = alloca %Vec*, align 8
  store %Vec* %0, %Vec** %self, align 8
  %other = alloca %Vec, align 8
  store %Vec %1, %Vec* %other, align 4
  %ref = load %Vec*, %Vec** %self, align 8
  %x = getelementptr %Vec, %Vec* %ref, i32 0, i32 0
  %x1 = load i32, i32* %x, align 4
  %x2 = getelementptr %Vec, %Vec* %other, i32 0, i32 0
  %x3 = load i32, i32* %x2, align 4
  %add = add i32 %x1, %x3
  %ref4 = load %Vec*, %Vec** %self, align 8
  %y = getelementptr %Vec, %Vec* %ref4, i32 0, i32 1
  %y5 = load i32, i32* %y, align 4
  %y6 = getelementptr %Vec, %Vec* %other, i32 0, i32 1
  %y7 = load i32, i32* %y6, align 4
  %add8 = add i32 %y5, %y7
  %call = call %Vec @Vec.create(i32 %add, i32 %add8)
  ret %Vec %call
}

define %Vec @"Vec.+.i32"(%Vec* %0, i32 %1) {
entry:
  %self = alloca %Vec*, align 8
  store %Vec* %0, %Vec** %self, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %ref = load %Vec*, %Vec** %self, align 8
  %x = getelementptr %Vec, %Vec* %ref, i32 0, i32 0
  %x1 = load i32, i32* %x, align 4
  %n2 = load i32, i32* %n, align 4
  %add = add i32 %x1, %n2
  %ref3 = load %Vec*, %Vec** %self, align 8
  %y = getelementptr %Vec, %Vec* %ref3, i32 0, i32 1
  %y4 = load i32, i32* %y, align 4
  %n5 = load i32, i32* %n, align 4
  %add6 = add i32 %y4, %n5
  %call = call %Vec @Vec.create(i32 %add, i32 %add6)
  ret %Vec %call
}

define i1 @"Vec.==.Vec"(%Vec* %0, %Vec %1) {
entry:
  %self = alloca %Vec*, align 8
  store %Vec* %0, %Vec** %self, align 8
  %other = alloca %Vec, align 8
  store %Vec %1, %Vec* %other, align 4
  %ref = load %Vec*, %Vec** %self, align 8
  %x = getelementptr %Vec, %Vec* %ref, i32 0, i32 0
  %x1 = load i32, i32* %x, align 4
  %x2 = getelementptr %Vec, %Vec* %other, i32 0, i32 0
  %x3 = load i32, i32* %x2, align 4
  %eq = icmp eq i32 %x1, %x3
  %ref4 = load %Vec*, %Vec** %self, align 8
  %y = getelementptr %Vec, %Vec* %ref4, i32 0, i32 1
  %y5 = load i32, i32* %y, align 4
  %y6 = getelementptr %Vec, %Vec* %other, i32 0, i32 1
  %y7 = load i32, i32* %y6, align 4
  %eq8 = icmp eq i32 %y5, %y7
  %and = and i1 %eq, %eq8
  ret i1 %and
}

define void @"Vec.=.Vec"(%Vec* %0, %Vec %1) {
entry:
  %self = alloca %Vec*, align 8
  store %Vec* %0, %Vec** %self, align 8
  %other = alloca %Vec, align 8
  store %Vec %1, %Vec* %other, align 4
  %ref = load %Vec*, %Vec** %self, align 8
  %x = getelementptr %Vec, %Vec* %ref, i32 0, i32 0
  %x1 = getelementptr %Vec, %Vec* %other, i32 0, i32 0
  %x2 = load i32, i32* %x1, align 4
  store i32 %x2, i32* %x, align 4
  %ref3 = load %Vec*, %Vec** %self, align 8
  %y = getelementptr %Vec, %Vec* %ref3, i32 0, i32 1
  %y4 = getelementptr %Vec, %Vec* %other, i32 0, i32 1
  %y5 = load i32, i32* %y4, align 4
  store i32 %y5, i32* %y, align 4
  %ref6 = load %Vec*, %Vec** %self, align 8
  %tag = getelementptr %Vec, %Vec* %ref6, i32 0, i32 2
  store i32 99, i32* %tag, align 4
  ret void
}

define void @"Vec.+=.Vec"(%Vec* %0, %Vec %1) {
entry:
  %self = alloca %Vec*, align 8
  store %Vec* %0, %Vec** %self, align 8
  %other = alloca %Vec, align 8
  store %Vec %1, %Vec* %other, align 4
  %ref = load %Vec*, %Vec** %self, align 8
  %x = getelementptr %Vec, %Vec* %ref, i32 0, i32 0
  %ref1 = load %Vec*, %Vec** %self, align 8
  %x2 = getelementptr %Vec, %Vec* %ref1, i32 0, i32 0
  %x3 = load i32, i32* %x2, align 4
  %x4 = getelementptr %Vec, %Vec* %other, i32 0, i32 0
  %x5 = load i32, i32* %x4, align 4
  %add = add i32 %x3, %x5
  store i32 %add, i32* %x, align 4
  %ref6 = load %Vec*, %Vec** %self, align 8
  %y = getelementptr %Vec, %Vec* %ref6, i32 0, i32 1
  %ref7 = load %Vec*, %Vec** %self, align 8
  %y8 = getelementptr %Vec, %Vec* %ref7, i32 0, i32 1
  %y9 = load i32, i32* %y8, align 4
  %y10 = getelementptr %Vec, %Vec* %other, i32 0, i32 1
  %y11 = load i32, i32* %y10, align 4
  %add12 = add i32 %y9, %y11
  store i32 %add12, i32* %y, align 4
  ret void
}

define void @"Vec.-=.i32"(%Vec* %0, i32 %1) {
entry:
  %self = alloca %Vec*, align 8
  store %Vec* %0, %Vec** %self, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %ref = load %Vec*, %Vec** %self, align 8
  %x = getelementptr %Vec, %Vec* %ref, i32 0, i32 0
  %ref1 = load %Vec*, %Vec** %self, align 8
  %x2 = getelementptr %Vec, %Vec* %ref1, i32 0, i32 0
  %x3 = load i32, i32* %x2, align 4
  %n4 = load i32, i32* %n, align 4
  %sub = sub i32 %x3, %n4
  store i32 %sub, i32* %x, align 4
  %ref5 = load %Vec*, %Vec** %self, align 8
  %y = getelementptr %Vec, %Vec* %ref5, i32 0, i32 1
  %ref6 = load %Vec*, %Vec** %self, align 8
  %y7 = getelementptr %Vec, %Vec* %ref6, i32 0, i32 1
  %y8 = load i32, i32* %y7, align 4
  %n9 = load i32, i32* %n, align 4
  %sub10 = sub i32 %y8, %n9
  store i32 %sub10, i32* %y, align 4
  ret void
}

define void @Vec.show(%Vec* %0) {
entry:
  %self = alloca %Vec*, align 8
  store %Vec* %0, %Vec** %self, align 8
  %ref = load %Vec*, %Vec** %self, align 8
  %x = getelementptr %Vec, %Vec* %ref, i32 0, i32 0
  %x1 = load i32, i32* %x, align 4
  %ref2 = load %Vec*, %Vec** %self, align 8
  %y = getelementptr %Vec, %Vec* %ref2, i32 0, i32 1
  %y3 = load i32, i32* %y, align 4
  %ref4 = load %Vec*, %Vec** %self, align 8
  %tag = getelementptr %Vec, %Vec* %ref4, i32 0, i32 2
  %tag5 = load i32, i32* %tag, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @fmt, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %x1, i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0), i32 %y3, i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.2, i32 0, i32 0), i32 %tag5, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)

define i32 @main(i32 %0, i8** %1) {
entry:
  %a = alloca %Vec, align 8
  %call = call %Vec @Vec.create(i32 1, i32 2)
  store %Vec %call, %Vec* %a, align 4
  %b = alloca %Vec, align 8
  %call1 = call %Vec @Vec.create(i32 10, i32 20)
  store %Vec %call1, %Vec* %b, align 4
  %c = alloca %Vec, align 8
  %b2 = load %Vec, %Vec* %b, align 4
  %op = call %Vec @"Vec.+.Vec"(%Vec* %a, %Vec %b2)
  store %Vec %op, %Vec* %c, align 4
  call void @Vec.show(%Vec* %c)
  %d = alloca %Vec, align 8
  %op3 = call %Vec @"Vec.+.i32"(%Vec* %a, i32 5)
  store %Vec %op3, %Vec* %d, align 4
  call void @Vec.show(%Vec* %d)
  %a4 = load %Vec, %Vec* %a, align 4
  %op5 = call i1 @"Vec.==.Vec"(%Vec* %a, %Vec %a4)
  %bool_str = select i1 %op5, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.6, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.4, i32 0, i32 0), i8* %bool_str, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  %b6 = load %Vec, %Vec* %b, align 4
  %op7 = call i1 @"Vec.==.Vec"(%Vec* %a, %Vec %b6)
  %bool_str8 = select i1 %op7, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.8, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.9, i32 0, i32 0)
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.11, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.7, i32 0, i32 0), i8* %bool_str8, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.10, i32 0, i32 0))
  %b9 = load %Vec, %Vec* %b, align 4
  %op10 = call %Vec @"Vec.+.Vec"(%Vec* %a, %Vec %b9)
  %out.tmp = alloca %Vec, align 8
  store %Vec %op10, %Vec* %out.tmp, align 4
  call void @Vec.show(%Vec* %out.tmp)
  %e = alloca %Vec, align 8
  %a11 = load %Vec, %Vec* %a, align 4
  store %Vec %a11, %Vec* %e, align 4
  call void @Vec.show(%Vec* %e)
  %f = alloca %Vec, align 8
  store %Vec zeroinitializer, %Vec* %f, align 4
  %a12 = load %Vec, %Vec* %a, align 4
  call void @"Vec.=.Vec"(%Vec* %f, %Vec %a12)
  call void @Vec.show(%Vec* %f)
  %g = alloca %Vec, align 8
  %call13 = call %Vec @Vec.create(i32 10, i32 10)
  store %Vec %call13, %Vec* %g, align 4
  %b14 = load %Vec, %Vec* %b, align 4
  call void @"Vec.+=.Vec"(%Vec* %g, %Vec %b14)
  call void @Vec.show(%Vec* %g)
  call void @"Vec.-=.i32"(%Vec* %g, i32 3)
  call void @Vec.show(%Vec* %g)
  ret i32 0
}
```

## 020 — drop on fall-through: an if block, each iteration, reverse order

```ura
// operators_overload/020.ura - fall-through drops, no return or break

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

main():
    if 1 < 2:
        x Tag = Tag::create(1)
        output("inside\n")
    output("after if\n")

    i i32 = 0
    while i < 3:
        y Tag = Tag::create(10 + i)
        i += 1
    output("after loop\n")

    p Tag = Tag::create(7)
    q Tag = Tag::create(8)
    output("end of main\n")
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

struct Tag
├─ id : i32
├─ fn Tag.create(n : i32) : STRUCT_CALL
│  ├─ t : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ .id : i32
│  │  │  └─ t : STRUCT_CALL
│  │  └─ n : i32
│  └─ return
│     └─ t : STRUCT_CALL
├─ fn Tag.=.Tag(self : STRUCT_CALL, other : STRUCT_CALL) : void
│  └─ = : i32
│     ├─ .id : i32
│     │  └─ self : STRUCT_CALL
│     └─ .id : i32
│        └─ other : STRUCT_CALL
└─ fn Tag.drop(self : STRUCT_CALL) : void
   └─ output : void
      ├─ char[] "drop "
      ├─ .id : i32
      │  └─ self : STRUCT_CALL
      └─ char[] "\n"

fn main() : i32
├─ if
│  ├─ condition < : bool
│  │  ├─ int 1
│  │  └─ int 2
│  ├─ = : STRUCT_CALL
│  │  ├─ x : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  │     └─ int 1
│  └─ output : void
│     └─ char[] "inside\n"
├─ output : void
│  └─ char[] "after if\n"
├─ = : i32
│  ├─ i : i32
│  └─ int 0
├─ while
│  ├─ condition < : bool
│  │  ├─ i : i32
│  │  └─ int 3
│  ├─ = : STRUCT_CALL
│  │  ├─ y : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  │     └─ + : i32
│  │        ├─ int 10
│  │        └─ i : i32
│  └─ += : i32
│     ├─ i : i32
│     └─ int 1
├─ output : void
│  └─ char[] "after loop\n"
├─ = : STRUCT_CALL
│  ├─ p : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     └─ int 7
├─ = : STRUCT_CALL
│  ├─ q : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     └─ int 8
└─ output : void
   └─ char[] "end of main\n"
```

```out
inside
drop 1
after if
drop 10
drop 11
drop 12
after loop
end of main
drop 8
drop 7
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Tag = type { i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [6 x i8] c"drop \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.2 = private unnamed_addr constant [8 x i8] c"inside\0A\00", align 1
@fmt.3 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.4 = private unnamed_addr constant [10 x i8] c"after if\0A\00", align 1
@fmt.5 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.6 = private unnamed_addr constant [12 x i8] c"after loop\0A\00", align 1
@fmt.7 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.8 = private unnamed_addr constant [13 x i8] c"end of main\0A\00", align 1
@fmt.9 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

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

define %Tag @Tag.create(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %t = alloca %Tag, align 8
  store %Tag zeroinitializer, %Tag* %t, align 4
  %id = getelementptr %Tag, %Tag* %t, i32 0, i32 0
  %n1 = load i32, i32* %n, align 4
  store i32 %n1, i32* %id, align 4
  %t2 = load %Tag, %Tag* %t, align 4
  ret %Tag %t2
}

define void @"Tag.=.Tag"(%Tag* %0, %Tag %1) {
entry:
  %self = alloca %Tag*, align 8
  store %Tag* %0, %Tag** %self, align 8
  %other = alloca %Tag, align 8
  store %Tag %1, %Tag* %other, align 4
  %ref = load %Tag*, %Tag** %self, align 8
  %id = getelementptr %Tag, %Tag* %ref, i32 0, i32 0
  %id1 = getelementptr %Tag, %Tag* %other, i32 0, i32 0
  %id2 = load i32, i32* %id1, align 4
  store i32 %id2, i32* %id, align 4
  ret void
}

define void @Tag.drop(%Tag* %0) {
entry:
  %self = alloca %Tag*, align 8
  store %Tag* %0, %Tag** %self, align 8
  %ref = load %Tag*, %Tag** %self, align 8
  %id = getelementptr %Tag, %Tag* %ref, i32 0, i32 0
  %id1 = load i32, i32* %id, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %id1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)

define i32 @main(i32 %0, i8** %1) {
entry:
  br i1 true, label %then, label %endif

endif:                                            ; preds = %then, %entry
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.5, i32 0, i32 0), i32 9, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.4, i32 0, i32 0))
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then:                                             ; preds = %entry
  %x = alloca %Tag, align 8
  %call = call %Tag @Tag.create(i32 1)
  store %Tag %call, %Tag* %x, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.3, i32 0, i32 0), i32 7, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.2, i32 0, i32 0))
  call void @Tag.drop(%Tag* %x)
  br label %endif

while.cond:                                       ; preds = %while.body, %endif
  %i1 = load i32, i32* %i, align 4
  %lt = icmp slt i32 %i1, 3
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %y = alloca %Tag, align 8
  %i2 = load i32, i32* %i, align 4
  %add = add i32 10, %i2
  %call3 = call %Tag @Tag.create(i32 %add)
  store %Tag %call3, %Tag* %y, align 4
  %cur = load i32, i32* %i, align 4
  %add4 = add i32 %cur, 1
  store i32 %add4, i32* %i, align 4
  call void @Tag.drop(%Tag* %y)
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.7, i32 0, i32 0), i32 11, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.6, i32 0, i32 0))
  %p = alloca %Tag, align 8
  %call5 = call %Tag @Tag.create(i32 7)
  store %Tag %call5, %Tag* %p, align 4
  %q = alloca %Tag, align 8
  %call6 = call %Tag @Tag.create(i32 8)
  store %Tag %call6, %Tag* %q, align 4
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.9, i32 0, i32 0), i32 12, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.8, i32 0, i32 0))
  call void @Tag.drop(%Tag* %q)
  call void @Tag.drop(%Tag* %p)
  ret i32 0
}
```

## 021 — a struct drops the structs it owns, at any depth, but not what it borrows

```ura
// operators_overload/021.ura - owned struct fields drop recursively

struct Res:
    id i32

    pub fn create(n i32) Res:
        r Res
        r.id = n
        return r

    operator =(other Res) void:
        self.id = other.id

    operator drop:
        output("drop ", self.id, "\n")

struct L3:
    res Res
struct L2:
    inner L3

// Owner declares no destructor of its own — everything below still drops
struct Owner:
    mid  L2
    also Res
    ref? borrowed Res

    pub fn create() Owner:
        o Owner
        o.mid.inner.res = Res::create(3)
        o.also          = Res::create(1)
        return o

main():
    lent Res = Res::create(9)
    o Owner = Owner::create()
    o.borrowed = ref lent
    output("built\n")
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

struct Res
├─ id : i32
├─ fn Res.create(n : i32) : STRUCT_CALL
│  ├─ r : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ .id : i32
│  │  │  └─ r : STRUCT_CALL
│  │  └─ n : i32
│  └─ return
│     └─ r : STRUCT_CALL
├─ fn Res.=.Res(self : STRUCT_CALL, other : STRUCT_CALL) : void
│  └─ = : i32
│     ├─ .id : i32
│     │  └─ self : STRUCT_CALL
│     └─ .id : i32
│        └─ other : STRUCT_CALL
└─ fn Res.drop(self : STRUCT_CALL) : void
   └─ output : void
      ├─ char[] "drop "
      ├─ .id : i32
      │  └─ self : STRUCT_CALL
      └─ char[] "\n"

struct L3
└─ res : STRUCT_CALL

struct L2
└─ inner : STRUCT_CALL

struct Owner
├─ mid : STRUCT_CALL
├─ also : STRUCT_CALL
├─ borrowed : STRUCT_CALL
└─ fn Owner.create() : STRUCT_CALL
   ├─ o : STRUCT_CALL
   ├─ = : void
   │  ├─ .res : STRUCT_CALL
   │  │  └─ .inner : STRUCT_CALL
   │  │     └─ .mid : STRUCT_CALL
   │  │        └─ o : STRUCT_CALL
   │  └─ call create : STRUCT_CALL
   │     └─ int 3
   ├─ = : void
   │  ├─ .also : STRUCT_CALL
   │  │  └─ o : STRUCT_CALL
   │  └─ call create : STRUCT_CALL
   │     └─ int 1
   └─ return
      └─ o : STRUCT_CALL

fn main() : i32
├─ = : STRUCT_CALL
│  ├─ lent : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     └─ int 9
├─ = : STRUCT_CALL
│  ├─ o : STRUCT_CALL
│  └─ call create : STRUCT_CALL
├─ = : STRUCT_CALL
│  ├─ .borrowed : STRUCT_CALL
│  │  └─ o : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ lent : STRUCT_CALL
└─ output : void
   └─ char[] "built\n"
```

```out
built
drop 1
drop 3
drop 9
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Res = type { i32 }
%Owner = type { %L2, %Res, %Res* }
%L2 = type { %L3 }
%L3 = type { %Res }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [6 x i8] c"drop \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.2 = private unnamed_addr constant [7 x i8] c"built\0A\00", align 1
@fmt.3 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

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

define %Res @Res.create(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %r = alloca %Res, align 8
  store %Res zeroinitializer, %Res* %r, align 4
  %id = getelementptr %Res, %Res* %r, i32 0, i32 0
  %n1 = load i32, i32* %n, align 4
  store i32 %n1, i32* %id, align 4
  %r2 = load %Res, %Res* %r, align 4
  ret %Res %r2
}

define void @"Res.=.Res"(%Res* %0, %Res %1) {
entry:
  %self = alloca %Res*, align 8
  store %Res* %0, %Res** %self, align 8
  %other = alloca %Res, align 8
  store %Res %1, %Res* %other, align 4
  %ref = load %Res*, %Res** %self, align 8
  %id = getelementptr %Res, %Res* %ref, i32 0, i32 0
  %id1 = getelementptr %Res, %Res* %other, i32 0, i32 0
  %id2 = load i32, i32* %id1, align 4
  store i32 %id2, i32* %id, align 4
  ret void
}

define void @Res.drop(%Res* %0) {
entry:
  %self = alloca %Res*, align 8
  store %Res* %0, %Res** %self, align 8
  %ref = load %Res*, %Res** %self, align 8
  %id = getelementptr %Res, %Res* %ref, i32 0, i32 0
  %id1 = load i32, i32* %id, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %id1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)

define %Owner @Owner.create() {
entry:
  %o = alloca %Owner, align 8
  store %Owner zeroinitializer, %Owner* %o, align 8
  %mid = getelementptr %Owner, %Owner* %o, i32 0, i32 0
  %inner = getelementptr %L2, %L2* %mid, i32 0, i32 0
  %res = getelementptr %L3, %L3* %inner, i32 0, i32 0
  %call = call %Res @Res.create(i32 3)
  call void @"Res.=.Res"(%Res* %res, %Res %call)
  %also = getelementptr %Owner, %Owner* %o, i32 0, i32 1
  %call1 = call %Res @Res.create(i32 1)
  call void @"Res.=.Res"(%Res* %also, %Res %call1)
  %o2 = load %Owner, %Owner* %o, align 8
  ret %Owner %o2
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %lent = alloca %Res, align 8
  %call = call %Res @Res.create(i32 9)
  store %Res %call, %Res* %lent, align 4
  %o = alloca %Owner, align 8
  %call1 = call %Owner @Owner.create()
  store %Owner %call1, %Owner* %o, align 8
  %borrowed = getelementptr %Owner, %Owner* %o, i32 0, i32 2
  store %Res* %lent, %Res** %borrowed, align 8
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.3, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.2, i32 0, i32 0))
  %also = getelementptr %Owner, %Owner* %o, i32 0, i32 1
  call void @Res.drop(%Res* %also)
  %mid = getelementptr %Owner, %Owner* %o, i32 0, i32 0
  %inner = getelementptr %L2, %L2* %mid, i32 0, i32 0
  %res = getelementptr %L3, %L3* %inner, i32 0, i32 0
  call void @Res.drop(%Res* %res)
  call void @Res.drop(%Res* %lent)
  ret i32 0
}
```

## 022 — a temporary is destroyed at the end of its statement

```ura
// operators_overload/022.ura - temporaries die at end of statement

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

    fn show() void:
        output("show ", self.id, "\n")

    operator drop:
        output("DROP ", self.id, "\n")

fn via_return(a R, b R) i32:
    return (a + b).id

main():
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

struct Plain
├─ x : i32
└─ fn Plain.create(n : i32) : STRUCT_CALL
   ├─ p : STRUCT_CALL
   ├─ = : i32
   │  ├─ .x : i32
   │  │  └─ p : STRUCT_CALL
   │  └─ n : i32
   └─ return
      └─ p : STRUCT_CALL

struct R
├─ id : i32
├─ fn R.create(n : i32) : STRUCT_CALL
│  ├─ r : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ .id : i32
│  │  │  └─ r : STRUCT_CALL
│  │  └─ n : i32
│  └─ return
│     └─ r : STRUCT_CALL
├─ fn R.=.R(self : STRUCT_CALL, other : STRUCT_CALL) : void
│  └─ = : i32
│     ├─ .id : i32
│     │  └─ self : STRUCT_CALL
│     └─ .id : i32
│        └─ other : STRUCT_CALL
├─ fn R.+.R(self : STRUCT_CALL, other : STRUCT_CALL) : STRUCT_CALL
│  └─ return
│     └─ call create : STRUCT_CALL
│        └─ + : i32
│           ├─ .id : i32
│           │  └─ self : STRUCT_CALL
│           └─ .id : i32
│              └─ other : STRUCT_CALL
├─ fn R.show(self : STRUCT_CALL) : void
│  └─ output : void
│     ├─ char[] "show "
│     ├─ .id : i32
│     │  └─ self : STRUCT_CALL
│     └─ char[] "\n"
└─ fn R.drop(self : STRUCT_CALL) : void
   └─ output : void
      ├─ char[] "DROP "
      ├─ .id : i32
      │  └─ self : STRUCT_CALL
      └─ char[] "\n"

fn via_return(a : STRUCT_CALL, b : STRUCT_CALL) : i32
└─ return
   └─ .id : i32
      └─ + : STRUCT_CALL
         ├─ a : STRUCT_CALL
         └─ b : STRUCT_CALL

fn main() : i32
├─ = : STRUCT_CALL
│  ├─ a : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     └─ int 1
├─ = : STRUCT_CALL
│  ├─ b : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     └─ int 2
├─ call show : void
│  └─ + : STRUCT_CALL
│     ├─ a : STRUCT_CALL
│     └─ b : STRUCT_CALL
├─ output : void
│  └─ char[] "--\n"
├─ output : void
│  ├─ .id : i32
│  │  └─ + : STRUCT_CALL
│  │     ├─ a : STRUCT_CALL
│  │     └─ b : STRUCT_CALL
│  └─ char[] "\n"
├─ output : void
│  └─ char[] "--\n"
├─ call show : void
│  └─ + : STRUCT_CALL
│     ├─ + : STRUCT_CALL
│     │  ├─ a : STRUCT_CALL
│     │  └─ b : STRUCT_CALL
│     └─ a : STRUCT_CALL
├─ output : void
│  └─ char[] "--\n"
├─ = : i32
│  ├─ i : i32
│  └─ int 0
├─ while
│  ├─ condition < : bool
│  │  ├─ i : i32
│  │  └─ int 2
│  ├─ call show : void
│  │  └─ + : STRUCT_CALL
│  │     ├─ a : STRUCT_CALL
│  │     └─ b : STRUCT_CALL
│  └─ += : i32
│     ├─ i : i32
│     └─ int 1
├─ output : void
│  └─ char[] "--\n"
├─ output : void
│  ├─ call via_return : i32
│  │  ├─ a : STRUCT_CALL
│  │  └─ b : STRUCT_CALL
│  └─ char[] "\n"
├─ output : void
│  └─ char[] "--\n"
├─ output : void
│  ├─ .id : i32
│  │  └─ call create : STRUCT_CALL
│  │     └─ int 9
│  └─ char[] "\n"
├─ output : void
│  └─ char[] "--\n"
├─ output : void
│  ├─ .x : i32
│  │  └─ call create : STRUCT_CALL
│  │     └─ int 7
│  └─ char[] "\n"
└─ output : void
   └─ char[] "end\n"
```

```out
show 3
DROP 3
--
3
DROP 3
--
show 4
DROP 4
DROP 3
--
show 3
DROP 3
show 3
DROP 3
--
DROP 3
3
--
9
DROP 9
--
7
end
DROP 2
DROP 1
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Plain = type { i32 }
%R = type { i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [6 x i8] c"show \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"DROP \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.5 = private unnamed_addr constant [4 x i8] c"--\0A\00", align 1
@fmt.6 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.8 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.9 = private unnamed_addr constant [4 x i8] c"--\0A\00", align 1
@fmt.10 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.11 = private unnamed_addr constant [4 x i8] c"--\0A\00", align 1
@fmt.12 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.13 = private unnamed_addr constant [4 x i8] c"--\0A\00", align 1
@fmt.14 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.15 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.16 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.17 = private unnamed_addr constant [4 x i8] c"--\0A\00", align 1
@fmt.18 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.19 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.20 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.21 = private unnamed_addr constant [4 x i8] c"--\0A\00", align 1
@fmt.22 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.23 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.24 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.25 = private unnamed_addr constant [5 x i8] c"end\0A\00", align 1
@fmt.26 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

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

define %Plain @Plain.create(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %p = alloca %Plain, align 8
  store %Plain zeroinitializer, %Plain* %p, align 4
  %x = getelementptr %Plain, %Plain* %p, i32 0, i32 0
  %n1 = load i32, i32* %n, align 4
  store i32 %n1, i32* %x, align 4
  %p2 = load %Plain, %Plain* %p, align 4
  ret %Plain %p2
}

define %R @R.create(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %r = alloca %R, align 8
  store %R zeroinitializer, %R* %r, align 4
  %id = getelementptr %R, %R* %r, i32 0, i32 0
  %n1 = load i32, i32* %n, align 4
  store i32 %n1, i32* %id, align 4
  %r2 = load %R, %R* %r, align 4
  ret %R %r2
}

define void @"R.=.R"(%R* %0, %R %1) {
entry:
  %self = alloca %R*, align 8
  store %R* %0, %R** %self, align 8
  %other = alloca %R, align 8
  store %R %1, %R* %other, align 4
  %ref = load %R*, %R** %self, align 8
  %id = getelementptr %R, %R* %ref, i32 0, i32 0
  %id1 = getelementptr %R, %R* %other, i32 0, i32 0
  %id2 = load i32, i32* %id1, align 4
  store i32 %id2, i32* %id, align 4
  ret void
}

define %R @"R.+.R"(%R* %0, %R %1) {
entry:
  %self = alloca %R*, align 8
  store %R* %0, %R** %self, align 8
  %other = alloca %R, align 8
  store %R %1, %R* %other, align 4
  %ref = load %R*, %R** %self, align 8
  %id = getelementptr %R, %R* %ref, i32 0, i32 0
  %id1 = load i32, i32* %id, align 4
  %id2 = getelementptr %R, %R* %other, i32 0, i32 0
  %id3 = load i32, i32* %id2, align 4
  %add = add i32 %id1, %id3
  %call = call %R @R.create(i32 %add)
  ret %R %call
}

define void @R.show(%R* %0) {
entry:
  %self = alloca %R*, align 8
  store %R* %0, %R** %self, align 8
  %ref = load %R*, %R** %self, align 8
  %id = getelementptr %R, %R* %ref, i32 0, i32 0
  %id1 = load i32, i32* %id, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %id1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)

define void @R.drop(%R* %0) {
entry:
  %self = alloca %R*, align 8
  store %R* %0, %R** %self, align 8
  %ref = load %R*, %R** %self, align 8
  %id = getelementptr %R, %R* %ref, i32 0, i32 0
  %id1 = load i32, i32* %id, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.4, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0), i32 %id1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  ret void
}

define i32 @via_return(%R %0, %R %1) {
entry:
  %a = alloca %R, align 8
  store %R %0, %R* %a, align 4
  %b = alloca %R, align 8
  store %R %1, %R* %b, align 4
  %b1 = load %R, %R* %b, align 4
  %op = call %R @"R.+.R"(%R* %a, %R %b1)
  %out.tmp = alloca %R, align 8
  store %R %op, %R* %out.tmp, align 4
  %id = getelementptr %R, %R* %out.tmp, i32 0, i32 0
  %id2 = load i32, i32* %id, align 4
  call void @R.drop(%R* %out.tmp)
  ret i32 %id2
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %a = alloca %R, align 8
  %call = call %R @R.create(i32 1)
  store %R %call, %R* %a, align 4
  %b = alloca %R, align 8
  %call1 = call %R @R.create(i32 2)
  store %R %call1, %R* %b, align 4
  %b2 = load %R, %R* %b, align 4
  %op = call %R @"R.+.R"(%R* %a, %R %b2)
  %out.tmp = alloca %R, align 8
  store %R %op, %R* %out.tmp, align 4
  call void @R.show(%R* %out.tmp)
  call void @R.drop(%R* %out.tmp)
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.6, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.5, i32 0, i32 0))
  %b3 = load %R, %R* %b, align 4
  %op4 = call %R @"R.+.R"(%R* %a, %R %b3)
  %out.tmp5 = alloca %R, align 8
  store %R %op4, %R* %out.tmp5, align 4
  %id = getelementptr %R, %R* %out.tmp5, i32 0, i32 0
  %id6 = load i32, i32* %id, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.8, i32 0, i32 0), i32 %id6, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  call void @R.drop(%R* %out.tmp5)
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.10, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.9, i32 0, i32 0))
  %b7 = load %R, %R* %b, align 4
  %op8 = call %R @"R.+.R"(%R* %a, %R %b7)
  %out.tmp9 = alloca %R, align 8
  store %R %op8, %R* %out.tmp9, align 4
  %a10 = load %R, %R* %a, align 4
  %op11 = call %R @"R.+.R"(%R* %out.tmp9, %R %a10)
  %out.tmp12 = alloca %R, align 8
  store %R %op11, %R* %out.tmp12, align 4
  call void @R.show(%R* %out.tmp12)
  call void @R.drop(%R* %out.tmp12)
  call void @R.drop(%R* %out.tmp9)
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.12, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.11, i32 0, i32 0))
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %i13 = load i32, i32* %i, align 4
  %lt = icmp slt i32 %i13, 2
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %b14 = load %R, %R* %b, align 4
  %op15 = call %R @"R.+.R"(%R* %a, %R %b14)
  %out.tmp16 = alloca %R, align 8
  store %R %op15, %R* %out.tmp16, align 4
  call void @R.show(%R* %out.tmp16)
  call void @R.drop(%R* %out.tmp16)
  %cur = load i32, i32* %i, align 4
  %add = add i32 %cur, 1
  store i32 %add, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.14, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.13, i32 0, i32 0))
  %a17 = load %R, %R* %a, align 4
  %b18 = load %R, %R* %b, align 4
  %call19 = call i32 @via_return(%R %a17, %R %b18)
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.16, i32 0, i32 0), i32 %call19, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.15, i32 0, i32 0))
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.18, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.17, i32 0, i32 0))
  %call20 = call %R @R.create(i32 9)
  %out.tmp21 = alloca %R, align 8
  store %R %call20, %R* %out.tmp21, align 4
  %id22 = getelementptr %R, %R* %out.tmp21, i32 0, i32 0
  %id23 = load i32, i32* %id22, align 4
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.20, i32 0, i32 0), i32 %id23, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.19, i32 0, i32 0))
  call void @R.drop(%R* %out.tmp21)
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.22, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.21, i32 0, i32 0))
  %call24 = call %Plain @Plain.create(i32 7)
  %out.tmp25 = alloca %Plain, align 8
  store %Plain %call24, %Plain* %out.tmp25, align 4
  %x = getelementptr %Plain, %Plain* %out.tmp25, i32 0, i32 0
  %x26 = load i32, i32* %x, align 4
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.24, i32 0, i32 0), i32 %x26, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.23, i32 0, i32 0))
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.26, i32 0, i32 0), i32 4, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.25, i32 0, i32 0))
  call void @R.drop(%R* %b)
  call void @R.drop(%R* %a)
  ret i32 0
}
```

## 023 — every overloadable operator on one struct

```ura
// operators_overload/023.ura - the full overload surface

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

    fn show(label char[]) void:
        output(label, "(", self.x, ", ", self.y, ")\n")

main():
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

struct Vec2
├─ x : i32
├─ y : i32
├─ fn Vec2.create(a : i32, b : i32) : STRUCT_CALL
│  ├─ v : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ .x : i32
│  │  │  └─ v : STRUCT_CALL
│  │  └─ a : i32
│  ├─ = : i32
│  │  ├─ .y : i32
│  │  │  └─ v : STRUCT_CALL
│  │  └─ b : i32
│  └─ return
│     └─ v : STRUCT_CALL
├─ fn Vec2.+.Vec2(self : STRUCT_CALL, other : STRUCT_CALL) : STRUCT_CALL
│  └─ return
│     └─ call create : STRUCT_CALL
│        ├─ + : i32
│        │  ├─ .x : i32
│        │  │  └─ self : STRUCT_CALL
│        │  └─ .x : i32
│        │     └─ other : STRUCT_CALL
│        └─ + : i32
│           ├─ .y : i32
│           │  └─ self : STRUCT_CALL
│           └─ .y : i32
│              └─ other : STRUCT_CALL
├─ fn Vec2.-.Vec2(self : STRUCT_CALL, other : STRUCT_CALL) : STRUCT_CALL
│  └─ return
│     └─ call create : STRUCT_CALL
│        ├─ - : i32
│        │  ├─ .x : i32
│        │  │  └─ self : STRUCT_CALL
│        │  └─ .x : i32
│        │     └─ other : STRUCT_CALL
│        └─ - : i32
│           ├─ .y : i32
│           │  └─ self : STRUCT_CALL
│           └─ .y : i32
│              └─ other : STRUCT_CALL
├─ fn Vec2.*.i32(self : STRUCT_CALL, n : i32) : STRUCT_CALL
│  └─ return
│     └─ call create : STRUCT_CALL
│        ├─ * : i32
│        │  ├─ .x : i32
│        │  │  └─ self : STRUCT_CALL
│        │  └─ n : i32
│        └─ * : i32
│           ├─ .y : i32
│           │  └─ self : STRUCT_CALL
│           └─ n : i32
├─ fn Vec2./.i32(self : STRUCT_CALL, n : i32) : STRUCT_CALL
│  └─ return
│     └─ call create : STRUCT_CALL
│        ├─ / : i32
│        │  ├─ .x : i32
│        │  │  └─ self : STRUCT_CALL
│        │  └─ n : i32
│        └─ / : i32
│           ├─ .y : i32
│           │  └─ self : STRUCT_CALL
│           └─ n : i32
├─ fn Vec2.%.i32(self : STRUCT_CALL, n : i32) : STRUCT_CALL
│  └─ return
│     └─ call create : STRUCT_CALL
│        ├─ % : i32
│        │  ├─ .x : i32
│        │  │  └─ self : STRUCT_CALL
│        │  └─ n : i32
│        └─ % : i32
│           ├─ .y : i32
│           │  └─ self : STRUCT_CALL
│           └─ n : i32
├─ fn Vec2.==.Vec2(self : STRUCT_CALL, other : STRUCT_CALL) : bool
│  └─ return
│     └─ and : bool
│        ├─ == : bool
│        │  ├─ .x : i32
│        │  │  └─ self : STRUCT_CALL
│        │  └─ .x : i32
│        │     └─ other : STRUCT_CALL
│        └─ == : bool
│           ├─ .y : i32
│           │  └─ self : STRUCT_CALL
│           └─ .y : i32
│              └─ other : STRUCT_CALL
├─ fn Vec2.!=.Vec2(self : STRUCT_CALL, other : STRUCT_CALL) : bool
│  └─ return
│     └─ or : bool
│        ├─ != : bool
│        │  ├─ .x : i32
│        │  │  └─ self : STRUCT_CALL
│        │  └─ .x : i32
│        │     └─ other : STRUCT_CALL
│        └─ != : bool
│           ├─ .y : i32
│           │  └─ self : STRUCT_CALL
│           └─ .y : i32
│              └─ other : STRUCT_CALL
├─ fn Vec2.<.Vec2(self : STRUCT_CALL, other : STRUCT_CALL) : bool
│  └─ return
│     └─ < : bool
│        ├─ call taxicab : i32
│        │  └─ self : STRUCT_CALL
│        └─ call taxicab : i32
│           └─ other : STRUCT_CALL
├─ fn Vec2.>.Vec2(self : STRUCT_CALL, other : STRUCT_CALL) : bool
│  └─ return
│     └─ > : bool
│        ├─ call taxicab : i32
│        │  └─ self : STRUCT_CALL
│        └─ call taxicab : i32
│           └─ other : STRUCT_CALL
├─ fn Vec2.<=.Vec2(self : STRUCT_CALL, other : STRUCT_CALL) : bool
│  └─ return
│     └─ <= : bool
│        ├─ call taxicab : i32
│        │  └─ self : STRUCT_CALL
│        └─ call taxicab : i32
│           └─ other : STRUCT_CALL
├─ fn Vec2.>=.Vec2(self : STRUCT_CALL, other : STRUCT_CALL) : bool
│  └─ return
│     └─ >= : bool
│        ├─ call taxicab : i32
│        │  └─ self : STRUCT_CALL
│        └─ call taxicab : i32
│           └─ other : STRUCT_CALL
├─ fn Vec2.=.Vec2(self : STRUCT_CALL, other : STRUCT_CALL) : void
│  ├─ = : i32
│  │  ├─ .x : i32
│  │  │  └─ self : STRUCT_CALL
│  │  └─ .x : i32
│  │     └─ other : STRUCT_CALL
│  └─ = : i32
│     ├─ .y : i32
│     │  └─ self : STRUCT_CALL
│     └─ .y : i32
│        └─ other : STRUCT_CALL
├─ fn Vec2.+=.Vec2(self : STRUCT_CALL, other : STRUCT_CALL) : void
│  ├─ = : i32
│  │  ├─ .x : i32
│  │  │  └─ self : STRUCT_CALL
│  │  └─ + : i32
│  │     ├─ .x : i32
│  │     │  └─ self : STRUCT_CALL
│  │     └─ .x : i32
│  │        └─ other : STRUCT_CALL
│  └─ = : i32
│     ├─ .y : i32
│     │  └─ self : STRUCT_CALL
│     └─ + : i32
│        ├─ .y : i32
│        │  └─ self : STRUCT_CALL
│        └─ .y : i32
│           └─ other : STRUCT_CALL
├─ fn Vec2.-=.Vec2(self : STRUCT_CALL, other : STRUCT_CALL) : void
│  ├─ = : i32
│  │  ├─ .x : i32
│  │  │  └─ self : STRUCT_CALL
│  │  └─ - : i32
│  │     ├─ .x : i32
│  │     │  └─ self : STRUCT_CALL
│  │     └─ .x : i32
│  │        └─ other : STRUCT_CALL
│  └─ = : i32
│     ├─ .y : i32
│     │  └─ self : STRUCT_CALL
│     └─ - : i32
│        ├─ .y : i32
│        │  └─ self : STRUCT_CALL
│        └─ .y : i32
│           └─ other : STRUCT_CALL
├─ fn Vec2.*=.i32(self : STRUCT_CALL, n : i32) : void
│  ├─ = : i32
│  │  ├─ .x : i32
│  │  │  └─ self : STRUCT_CALL
│  │  └─ * : i32
│  │     ├─ .x : i32
│  │     │  └─ self : STRUCT_CALL
│  │     └─ n : i32
│  └─ = : i32
│     ├─ .y : i32
│     │  └─ self : STRUCT_CALL
│     └─ * : i32
│        ├─ .y : i32
│        │  └─ self : STRUCT_CALL
│        └─ n : i32
├─ fn Vec2./=.i32(self : STRUCT_CALL, n : i32) : void
│  ├─ = : i32
│  │  ├─ .x : i32
│  │  │  └─ self : STRUCT_CALL
│  │  └─ / : i32
│  │     ├─ .x : i32
│  │     │  └─ self : STRUCT_CALL
│  │     └─ n : i32
│  └─ = : i32
│     ├─ .y : i32
│     │  └─ self : STRUCT_CALL
│     └─ / : i32
│        ├─ .y : i32
│        │  └─ self : STRUCT_CALL
│        └─ n : i32
├─ fn Vec2.%=.i32(self : STRUCT_CALL, n : i32) : void
│  ├─ = : i32
│  │  ├─ .x : i32
│  │  │  └─ self : STRUCT_CALL
│  │  └─ % : i32
│  │     ├─ .x : i32
│  │     │  └─ self : STRUCT_CALL
│  │     └─ n : i32
│  └─ = : i32
│     ├─ .y : i32
│     │  └─ self : STRUCT_CALL
│     └─ % : i32
│        ├─ .y : i32
│        │  └─ self : STRUCT_CALL
│        └─ n : i32
├─ fn Vec2.taxicab(self : STRUCT_CALL) : i32
│  └─ return
│     └─ + : i32
│        ├─ call abs : i32
│        │  ├─ self : STRUCT_CALL
│        │  └─ .x : i32
│        │     └─ self : STRUCT_CALL
│        └─ call abs : i32
│           ├─ self : STRUCT_CALL
│           └─ .y : i32
│              └─ self : STRUCT_CALL
├─ fn Vec2.abs(self : STRUCT_CALL, n : i32) : i32
│  ├─ if
│  │  ├─ condition < : bool
│  │  │  ├─ n : i32
│  │  │  └─ int 0
│  │  └─ return
│  │     └─ - : i32
│  │        ├─ int 0
│  │        └─ n : i32
│  └─ return
│     └─ n : i32
└─ fn Vec2.show(self : STRUCT_CALL, label : array) : void
   └─ output : void
      ├─ label : char[]
      ├─ char[] "("
      ├─ .x : i32
      │  └─ self : STRUCT_CALL
      ├─ char[] ", "
      ├─ .y : i32
      │  └─ self : STRUCT_CALL
      └─ char[] ")\n"

fn main() : i32
├─ = : STRUCT_CALL
│  ├─ a : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     ├─ int 9
│     └─ int 12
├─ = : STRUCT_CALL
│  ├─ b : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     ├─ int 2
│     └─ int 3
├─ call show : void
│  ├─ + : STRUCT_CALL
│  │  ├─ a : STRUCT_CALL
│  │  └─ b : STRUCT_CALL
│  └─ char[] "a + b   "
├─ call show : void
│  ├─ - : STRUCT_CALL
│  │  ├─ a : STRUCT_CALL
│  │  └─ b : STRUCT_CALL
│  └─ char[] "a - b   "
├─ call show : void
│  ├─ * : STRUCT_CALL
│  │  ├─ a : STRUCT_CALL
│  │  └─ int 3
│  └─ char[] "a * 3   "
├─ call show : void
│  ├─ / : STRUCT_CALL
│  │  ├─ a : STRUCT_CALL
│  │  └─ int 2
│  └─ char[] "a / 2   "
├─ call show : void
│  ├─ % : STRUCT_CALL
│  │  ├─ a : STRUCT_CALL
│  │  └─ int 4
│  └─ char[] "a mod 4 "
├─ output : void
│  ├─ char[] "== "
│  ├─ == : bool
│  │  ├─ a : STRUCT_CALL
│  │  └─ b : STRUCT_CALL
│  ├─ char[] "  != "
│  ├─ != : bool
│  │  ├─ a : STRUCT_CALL
│  │  └─ b : STRUCT_CALL
│  └─ char[] "\n"
├─ output : void
│  ├─ char[] "<  "
│  ├─ < : bool
│  │  ├─ a : STRUCT_CALL
│  │  └─ b : STRUCT_CALL
│  ├─ char[] "  >  "
│  ├─ > : bool
│  │  ├─ a : STRUCT_CALL
│  │  └─ b : STRUCT_CALL
│  └─ char[] "\n"
├─ output : void
│  ├─ char[] "<= "
│  ├─ <= : bool
│  │  ├─ a : STRUCT_CALL
│  │  └─ b : STRUCT_CALL
│  ├─ char[] "  >= "
│  ├─ >= : bool
│  │  ├─ a : STRUCT_CALL
│  │  └─ b : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ c : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     ├─ int 0
│     └─ int 0
├─ = : void
│  ├─ c : STRUCT_CALL
│  └─ a : STRUCT_CALL
├─ call show : void
│  ├─ c : STRUCT_CALL
│  └─ char[] "c = a    "
├─ += : void
│  ├─ c : STRUCT_CALL
│  └─ b : STRUCT_CALL
├─ call show : void
│  ├─ c : STRUCT_CALL
│  └─ char[] "c += b   "
├─ -= : void
│  ├─ c : STRUCT_CALL
│  └─ b : STRUCT_CALL
├─ call show : void
│  ├─ c : STRUCT_CALL
│  └─ char[] "c -= b   "
├─ *= : void
│  ├─ c : STRUCT_CALL
│  └─ int 2
├─ call show : void
│  ├─ c : STRUCT_CALL
│  └─ char[] "c *= 2   "
├─ /= : void
│  ├─ c : STRUCT_CALL
│  └─ int 3
├─ call show : void
│  ├─ c : STRUCT_CALL
│  └─ char[] "c /= 3   "
├─ %= : void
│  ├─ c : STRUCT_CALL
│  └─ int 5
├─ call show : void
│  ├─ c : STRUCT_CALL
│  └─ char[] "c %= 5   "
├─ if
│  ├─ condition > : bool
│  │  ├─ a : STRUCT_CALL
│  │  └─ b : STRUCT_CALL
│  └─ output : void
│     └─ char[] "a is longer\n"
├─ = : i32
│  ├─ n : i32
│  └─ int 0
├─ = : STRUCT_CALL
│  ├─ d : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     ├─ int 1
│     └─ int 1
├─ while
│  ├─ condition < : bool
│  │  ├─ d : STRUCT_CALL
│  │  └─ a : STRUCT_CALL
│  ├─ += : void
│  │  ├─ d : STRUCT_CALL
│  │  └─ b : STRUCT_CALL
│  └─ += : i32
│     ├─ n : i32
│     └─ int 1
└─ output : void
   ├─ char[] "steps to pass a: "
   ├─ n : i32
   └─ char[] "\n"
```

```out
a + b   (11, 15)
a - b   (7, 9)
a * 3   (27, 36)
a / 2   (4, 6)
a mod 4 (1, 0)
== False  != True
<  False  >  True
<= False  >= True
c = a    (9, 12)
c += b   (11, 15)
c -= b   (9, 12)
c *= 2   (18, 24)
c /= 3   (6, 8)
c %= 5   (1, 3)
a is longer
steps to pass a: 4
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Vec2 = type { i32, i32 }

@os = internal global %Os zeroinitializer
@trap_msg = private unnamed_addr constant [215 x i8] c"runtime error: Division by zero\0A   023.ura:23:36\0A   |\0A23 |         return Vec2::create(self.x / n, self.y / n)\0A   |                                    ^\0A\00", align 1
@trap_msg.1 = private unnamed_addr constant [227 x i8] c"runtime error: Division by zero\0A   023.ura:23:48\0A   |\0A23 |         return Vec2::create(self.x / n, self.y / n)\0A   |                                                ^\0A\00", align 1
@trap_msg.2 = private unnamed_addr constant [213 x i8] c"runtime error: Modulo by zero\0A   023.ura:26:36\0A   |\0A26 |         return Vec2::create(self.x % n, self.y % n)\0A   |                                    ^\0A\00", align 1
@trap_msg.3 = private unnamed_addr constant [225 x i8] c"runtime error: Modulo by zero\0A   023.ura:26:48\0A   |\0A26 |         return Vec2::create(self.x % n, self.y % n)\0A   |                                                ^\0A\00", align 1
@trap_msg.4 = private unnamed_addr constant [180 x i8] c"runtime error: Division by zero\0A   023.ura:63:25\0A   |\0A63 |         self.x = self.x / n\0A   |                         ^\0A\00", align 1
@trap_msg.5 = private unnamed_addr constant [180 x i8] c"runtime error: Division by zero\0A   023.ura:64:25\0A   |\0A64 |         self.y = self.y / n\0A   |                         ^\0A\00", align 1
@trap_msg.6 = private unnamed_addr constant [178 x i8] c"runtime error: Modulo by zero\0A   023.ura:67:25\0A   |\0A67 |         self.x = self.x % n\0A   |                         ^\0A\00", align 1
@trap_msg.7 = private unnamed_addr constant [178 x i8] c"runtime error: Modulo by zero\0A   023.ura:68:25\0A   |\0A68 |         self.y = self.y % n\0A   |                         ^\0A\00", align 1
@str = private unnamed_addr constant [2 x i8] c"(\00", align 1
@str.8 = private unnamed_addr constant [3 x i8] c", \00", align 1
@str.9 = private unnamed_addr constant [3 x i8] c")\0A\00", align 1
@fmt = private unnamed_addr constant [21 x i8] c"%.*s%.*s%d%.*s%d%.*s\00", align 1
@str.10 = private unnamed_addr constant [9 x i8] c"a + b   \00", align 1
@str.11 = private unnamed_addr constant [9 x i8] c"a - b   \00", align 1
@str.12 = private unnamed_addr constant [9 x i8] c"a * 3   \00", align 1
@str.13 = private unnamed_addr constant [9 x i8] c"a / 2   \00", align 1
@str.14 = private unnamed_addr constant [9 x i8] c"a mod 4 \00", align 1
@str.15 = private unnamed_addr constant [4 x i8] c"== \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.16 = private unnamed_addr constant [6 x i8] c"  != \00", align 1
@true_str.17 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.18 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.19 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.20 = private unnamed_addr constant [17 x i8] c"%.*s%s%.*s%s%.*s\00", align 1
@str.21 = private unnamed_addr constant [4 x i8] c"<  \00", align 1
@true_str.22 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.23 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.24 = private unnamed_addr constant [6 x i8] c"  >  \00", align 1
@true_str.25 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.26 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.27 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.28 = private unnamed_addr constant [17 x i8] c"%.*s%s%.*s%s%.*s\00", align 1
@str.29 = private unnamed_addr constant [4 x i8] c"<= \00", align 1
@true_str.30 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.31 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.32 = private unnamed_addr constant [6 x i8] c"  >= \00", align 1
@true_str.33 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.34 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.35 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.36 = private unnamed_addr constant [17 x i8] c"%.*s%s%.*s%s%.*s\00", align 1
@str.37 = private unnamed_addr constant [10 x i8] c"c = a    \00", align 1
@str.38 = private unnamed_addr constant [10 x i8] c"c += b   \00", align 1
@str.39 = private unnamed_addr constant [10 x i8] c"c -= b   \00", align 1
@str.40 = private unnamed_addr constant [10 x i8] c"c *= 2   \00", align 1
@str.41 = private unnamed_addr constant [10 x i8] c"c /= 3   \00", align 1
@str.42 = private unnamed_addr constant [10 x i8] c"c %= 5   \00", align 1
@str.43 = private unnamed_addr constant [13 x i8] c"a is longer\0A\00", align 1
@fmt.44 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.45 = private unnamed_addr constant [18 x i8] c"steps to pass a: \00", align 1
@str.46 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.47 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1

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

define %Vec2 @Vec2.create(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  %b = alloca i32, align 4
  store i32 %1, i32* %b, align 4
  %v = alloca %Vec2, align 8
  store %Vec2 zeroinitializer, %Vec2* %v, align 4
  %x = getelementptr %Vec2, %Vec2* %v, i32 0, i32 0
  %a1 = load i32, i32* %a, align 4
  store i32 %a1, i32* %x, align 4
  %y = getelementptr %Vec2, %Vec2* %v, i32 0, i32 1
  %b2 = load i32, i32* %b, align 4
  store i32 %b2, i32* %y, align 4
  %v3 = load %Vec2, %Vec2* %v, align 4
  ret %Vec2 %v3
}

define %Vec2 @"Vec2.+.Vec2"(%Vec2* %0, %Vec2 %1) {
entry:
  %self = alloca %Vec2*, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  %other = alloca %Vec2, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %ref = load %Vec2*, %Vec2** %self, align 8
  %x = getelementptr %Vec2, %Vec2* %ref, i32 0, i32 0
  %x1 = load i32, i32* %x, align 4
  %x2 = getelementptr %Vec2, %Vec2* %other, i32 0, i32 0
  %x3 = load i32, i32* %x2, align 4
  %add = add i32 %x1, %x3
  %ref4 = load %Vec2*, %Vec2** %self, align 8
  %y = getelementptr %Vec2, %Vec2* %ref4, i32 0, i32 1
  %y5 = load i32, i32* %y, align 4
  %y6 = getelementptr %Vec2, %Vec2* %other, i32 0, i32 1
  %y7 = load i32, i32* %y6, align 4
  %add8 = add i32 %y5, %y7
  %call = call %Vec2 @Vec2.create(i32 %add, i32 %add8)
  ret %Vec2 %call
}

define %Vec2 @Vec2.-.Vec2(%Vec2* %0, %Vec2 %1) {
entry:
  %self = alloca %Vec2*, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  %other = alloca %Vec2, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %ref = load %Vec2*, %Vec2** %self, align 8
  %x = getelementptr %Vec2, %Vec2* %ref, i32 0, i32 0
  %x1 = load i32, i32* %x, align 4
  %x2 = getelementptr %Vec2, %Vec2* %other, i32 0, i32 0
  %x3 = load i32, i32* %x2, align 4
  %sub = sub i32 %x1, %x3
  %ref4 = load %Vec2*, %Vec2** %self, align 8
  %y = getelementptr %Vec2, %Vec2* %ref4, i32 0, i32 1
  %y5 = load i32, i32* %y, align 4
  %y6 = getelementptr %Vec2, %Vec2* %other, i32 0, i32 1
  %y7 = load i32, i32* %y6, align 4
  %sub8 = sub i32 %y5, %y7
  %call = call %Vec2 @Vec2.create(i32 %sub, i32 %sub8)
  ret %Vec2 %call
}

define %Vec2 @"Vec2.*.i32"(%Vec2* %0, i32 %1) {
entry:
  %self = alloca %Vec2*, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %ref = load %Vec2*, %Vec2** %self, align 8
  %x = getelementptr %Vec2, %Vec2* %ref, i32 0, i32 0
  %x1 = load i32, i32* %x, align 4
  %n2 = load i32, i32* %n, align 4
  %mul = mul i32 %x1, %n2
  %ref3 = load %Vec2*, %Vec2** %self, align 8
  %y = getelementptr %Vec2, %Vec2* %ref3, i32 0, i32 1
  %y4 = load i32, i32* %y, align 4
  %n5 = load i32, i32* %n, align 4
  %mul6 = mul i32 %y4, %n5
  %call = call %Vec2 @Vec2.create(i32 %mul, i32 %mul6)
  ret %Vec2 %call
}

define %Vec2 @"Vec2./.i32"(%Vec2* %0, i32 %1) {
entry:
  %self = alloca %Vec2*, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %ref = load %Vec2*, %Vec2** %self, align 8
  %x = getelementptr %Vec2, %Vec2* %ref, i32 0, i32 0
  %x1 = load i32, i32* %x, align 4
  %n2 = load i32, i32* %n, align 4
  %iszero = icmp eq i32 %n2, 0
  br i1 %iszero, label %trap, label %cont

trap:                                             ; preds = %entry
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([215 x i8], [215 x i8]* @trap_msg, i32 0, i32 0), i64 214)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %div = sdiv i32 %x1, %n2
  %ref3 = load %Vec2*, %Vec2** %self, align 8
  %y = getelementptr %Vec2, %Vec2* %ref3, i32 0, i32 1
  %y4 = load i32, i32* %y, align 4
  %n5 = load i32, i32* %n, align 4
  %iszero6 = icmp eq i32 %n5, 0
  br i1 %iszero6, label %trap7, label %cont8

trap7:                                            ; preds = %cont
  %3 = call i64 @write(i32 2, i8* getelementptr inbounds ([227 x i8], [227 x i8]* @trap_msg.1, i32 0, i32 0), i64 226)
  call void @exit(i32 1)
  unreachable

cont8:                                            ; preds = %cont
  %div9 = sdiv i32 %y4, %n5
  %call = call %Vec2 @Vec2.create(i32 %div, i32 %div9)
  ret %Vec2 %call
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)

define %Vec2 @"Vec2.%.i32"(%Vec2* %0, i32 %1) {
entry:
  %self = alloca %Vec2*, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %ref = load %Vec2*, %Vec2** %self, align 8
  %x = getelementptr %Vec2, %Vec2* %ref, i32 0, i32 0
  %x1 = load i32, i32* %x, align 4
  %n2 = load i32, i32* %n, align 4
  %iszero = icmp eq i32 %n2, 0
  br i1 %iszero, label %trap, label %cont

trap:                                             ; preds = %entry
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([213 x i8], [213 x i8]* @trap_msg.2, i32 0, i32 0), i64 212)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %mod = srem i32 %x1, %n2
  %ref3 = load %Vec2*, %Vec2** %self, align 8
  %y = getelementptr %Vec2, %Vec2* %ref3, i32 0, i32 1
  %y4 = load i32, i32* %y, align 4
  %n5 = load i32, i32* %n, align 4
  %iszero6 = icmp eq i32 %n5, 0
  br i1 %iszero6, label %trap7, label %cont8

trap7:                                            ; preds = %cont
  %3 = call i64 @write(i32 2, i8* getelementptr inbounds ([225 x i8], [225 x i8]* @trap_msg.3, i32 0, i32 0), i64 224)
  call void @exit(i32 1)
  unreachable

cont8:                                            ; preds = %cont
  %mod9 = srem i32 %y4, %n5
  %call = call %Vec2 @Vec2.create(i32 %mod, i32 %mod9)
  ret %Vec2 %call
}

define i1 @"Vec2.==.Vec2"(%Vec2* %0, %Vec2 %1) {
entry:
  %self = alloca %Vec2*, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  %other = alloca %Vec2, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %ref = load %Vec2*, %Vec2** %self, align 8
  %x = getelementptr %Vec2, %Vec2* %ref, i32 0, i32 0
  %x1 = load i32, i32* %x, align 4
  %x2 = getelementptr %Vec2, %Vec2* %other, i32 0, i32 0
  %x3 = load i32, i32* %x2, align 4
  %eq = icmp eq i32 %x1, %x3
  %ref4 = load %Vec2*, %Vec2** %self, align 8
  %y = getelementptr %Vec2, %Vec2* %ref4, i32 0, i32 1
  %y5 = load i32, i32* %y, align 4
  %y6 = getelementptr %Vec2, %Vec2* %other, i32 0, i32 1
  %y7 = load i32, i32* %y6, align 4
  %eq8 = icmp eq i32 %y5, %y7
  %and = and i1 %eq, %eq8
  ret i1 %and
}

define i1 @"Vec2.!=.Vec2"(%Vec2* %0, %Vec2 %1) {
entry:
  %self = alloca %Vec2*, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  %other = alloca %Vec2, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %ref = load %Vec2*, %Vec2** %self, align 8
  %x = getelementptr %Vec2, %Vec2* %ref, i32 0, i32 0
  %x1 = load i32, i32* %x, align 4
  %x2 = getelementptr %Vec2, %Vec2* %other, i32 0, i32 0
  %x3 = load i32, i32* %x2, align 4
  %ne = icmp ne i32 %x1, %x3
  %ref4 = load %Vec2*, %Vec2** %self, align 8
  %y = getelementptr %Vec2, %Vec2* %ref4, i32 0, i32 1
  %y5 = load i32, i32* %y, align 4
  %y6 = getelementptr %Vec2, %Vec2* %other, i32 0, i32 1
  %y7 = load i32, i32* %y6, align 4
  %ne8 = icmp ne i32 %y5, %y7
  %or = or i1 %ne, %ne8
  ret i1 %or
}

define i1 @"Vec2.<.Vec2"(%Vec2* %0, %Vec2 %1) {
entry:
  %self = alloca %Vec2*, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  %other = alloca %Vec2, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %ref = load %Vec2*, %Vec2** %self, align 8
  %call = call i32 @Vec2.taxicab(%Vec2* %ref)
  %call1 = call i32 @Vec2.taxicab(%Vec2* %other)
  %lt = icmp slt i32 %call, %call1
  ret i1 %lt
}

define i32 @Vec2.taxicab(%Vec2* %0) {
entry:
  %self = alloca %Vec2*, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  %ref = load %Vec2*, %Vec2** %self, align 8
  %ref1 = load %Vec2*, %Vec2** %self, align 8
  %x = getelementptr %Vec2, %Vec2* %ref1, i32 0, i32 0
  %x2 = load i32, i32* %x, align 4
  %call = call i32 @Vec2.abs(%Vec2* %ref, i32 %x2)
  %ref3 = load %Vec2*, %Vec2** %self, align 8
  %ref4 = load %Vec2*, %Vec2** %self, align 8
  %y = getelementptr %Vec2, %Vec2* %ref4, i32 0, i32 1
  %y5 = load i32, i32* %y, align 4
  %call6 = call i32 @Vec2.abs(%Vec2* %ref3, i32 %y5)
  %add = add i32 %call, %call6
  ret i32 %add
}

define i1 @"Vec2.>.Vec2"(%Vec2* %0, %Vec2 %1) {
entry:
  %self = alloca %Vec2*, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  %other = alloca %Vec2, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %ref = load %Vec2*, %Vec2** %self, align 8
  %call = call i32 @Vec2.taxicab(%Vec2* %ref)
  %call1 = call i32 @Vec2.taxicab(%Vec2* %other)
  %gt = icmp sgt i32 %call, %call1
  ret i1 %gt
}

define i1 @"Vec2.<=.Vec2"(%Vec2* %0, %Vec2 %1) {
entry:
  %self = alloca %Vec2*, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  %other = alloca %Vec2, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %ref = load %Vec2*, %Vec2** %self, align 8
  %call = call i32 @Vec2.taxicab(%Vec2* %ref)
  %call1 = call i32 @Vec2.taxicab(%Vec2* %other)
  %le = icmp sle i32 %call, %call1
  ret i1 %le
}

define i1 @"Vec2.>=.Vec2"(%Vec2* %0, %Vec2 %1) {
entry:
  %self = alloca %Vec2*, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  %other = alloca %Vec2, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %ref = load %Vec2*, %Vec2** %self, align 8
  %call = call i32 @Vec2.taxicab(%Vec2* %ref)
  %call1 = call i32 @Vec2.taxicab(%Vec2* %other)
  %ge = icmp sge i32 %call, %call1
  ret i1 %ge
}

define void @"Vec2.=.Vec2"(%Vec2* %0, %Vec2 %1) {
entry:
  %self = alloca %Vec2*, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  %other = alloca %Vec2, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %ref = load %Vec2*, %Vec2** %self, align 8
  %x = getelementptr %Vec2, %Vec2* %ref, i32 0, i32 0
  %x1 = getelementptr %Vec2, %Vec2* %other, i32 0, i32 0
  %x2 = load i32, i32* %x1, align 4
  store i32 %x2, i32* %x, align 4
  %ref3 = load %Vec2*, %Vec2** %self, align 8
  %y = getelementptr %Vec2, %Vec2* %ref3, i32 0, i32 1
  %y4 = getelementptr %Vec2, %Vec2* %other, i32 0, i32 1
  %y5 = load i32, i32* %y4, align 4
  store i32 %y5, i32* %y, align 4
  ret void
}

define void @"Vec2.+=.Vec2"(%Vec2* %0, %Vec2 %1) {
entry:
  %self = alloca %Vec2*, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  %other = alloca %Vec2, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %ref = load %Vec2*, %Vec2** %self, align 8
  %x = getelementptr %Vec2, %Vec2* %ref, i32 0, i32 0
  %ref1 = load %Vec2*, %Vec2** %self, align 8
  %x2 = getelementptr %Vec2, %Vec2* %ref1, i32 0, i32 0
  %x3 = load i32, i32* %x2, align 4
  %x4 = getelementptr %Vec2, %Vec2* %other, i32 0, i32 0
  %x5 = load i32, i32* %x4, align 4
  %add = add i32 %x3, %x5
  store i32 %add, i32* %x, align 4
  %ref6 = load %Vec2*, %Vec2** %self, align 8
  %y = getelementptr %Vec2, %Vec2* %ref6, i32 0, i32 1
  %ref7 = load %Vec2*, %Vec2** %self, align 8
  %y8 = getelementptr %Vec2, %Vec2* %ref7, i32 0, i32 1
  %y9 = load i32, i32* %y8, align 4
  %y10 = getelementptr %Vec2, %Vec2* %other, i32 0, i32 1
  %y11 = load i32, i32* %y10, align 4
  %add12 = add i32 %y9, %y11
  store i32 %add12, i32* %y, align 4
  ret void
}

define void @"Vec2.-=.Vec2"(%Vec2* %0, %Vec2 %1) {
entry:
  %self = alloca %Vec2*, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  %other = alloca %Vec2, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %ref = load %Vec2*, %Vec2** %self, align 8
  %x = getelementptr %Vec2, %Vec2* %ref, i32 0, i32 0
  %ref1 = load %Vec2*, %Vec2** %self, align 8
  %x2 = getelementptr %Vec2, %Vec2* %ref1, i32 0, i32 0
  %x3 = load i32, i32* %x2, align 4
  %x4 = getelementptr %Vec2, %Vec2* %other, i32 0, i32 0
  %x5 = load i32, i32* %x4, align 4
  %sub = sub i32 %x3, %x5
  store i32 %sub, i32* %x, align 4
  %ref6 = load %Vec2*, %Vec2** %self, align 8
  %y = getelementptr %Vec2, %Vec2* %ref6, i32 0, i32 1
  %ref7 = load %Vec2*, %Vec2** %self, align 8
  %y8 = getelementptr %Vec2, %Vec2* %ref7, i32 0, i32 1
  %y9 = load i32, i32* %y8, align 4
  %y10 = getelementptr %Vec2, %Vec2* %other, i32 0, i32 1
  %y11 = load i32, i32* %y10, align 4
  %sub12 = sub i32 %y9, %y11
  store i32 %sub12, i32* %y, align 4
  ret void
}

define void @"Vec2.*=.i32"(%Vec2* %0, i32 %1) {
entry:
  %self = alloca %Vec2*, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %ref = load %Vec2*, %Vec2** %self, align 8
  %x = getelementptr %Vec2, %Vec2* %ref, i32 0, i32 0
  %ref1 = load %Vec2*, %Vec2** %self, align 8
  %x2 = getelementptr %Vec2, %Vec2* %ref1, i32 0, i32 0
  %x3 = load i32, i32* %x2, align 4
  %n4 = load i32, i32* %n, align 4
  %mul = mul i32 %x3, %n4
  store i32 %mul, i32* %x, align 4
  %ref5 = load %Vec2*, %Vec2** %self, align 8
  %y = getelementptr %Vec2, %Vec2* %ref5, i32 0, i32 1
  %ref6 = load %Vec2*, %Vec2** %self, align 8
  %y7 = getelementptr %Vec2, %Vec2* %ref6, i32 0, i32 1
  %y8 = load i32, i32* %y7, align 4
  %n9 = load i32, i32* %n, align 4
  %mul10 = mul i32 %y8, %n9
  store i32 %mul10, i32* %y, align 4
  ret void
}

define void @"Vec2./=.i32"(%Vec2* %0, i32 %1) {
entry:
  %self = alloca %Vec2*, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %ref = load %Vec2*, %Vec2** %self, align 8
  %x = getelementptr %Vec2, %Vec2* %ref, i32 0, i32 0
  %ref1 = load %Vec2*, %Vec2** %self, align 8
  %x2 = getelementptr %Vec2, %Vec2* %ref1, i32 0, i32 0
  %x3 = load i32, i32* %x2, align 4
  %n4 = load i32, i32* %n, align 4
  %iszero = icmp eq i32 %n4, 0
  br i1 %iszero, label %trap, label %cont

trap:                                             ; preds = %entry
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([180 x i8], [180 x i8]* @trap_msg.4, i32 0, i32 0), i64 179)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %div = sdiv i32 %x3, %n4
  store i32 %div, i32* %x, align 4
  %ref5 = load %Vec2*, %Vec2** %self, align 8
  %y = getelementptr %Vec2, %Vec2* %ref5, i32 0, i32 1
  %ref6 = load %Vec2*, %Vec2** %self, align 8
  %y7 = getelementptr %Vec2, %Vec2* %ref6, i32 0, i32 1
  %y8 = load i32, i32* %y7, align 4
  %n9 = load i32, i32* %n, align 4
  %iszero10 = icmp eq i32 %n9, 0
  br i1 %iszero10, label %trap11, label %cont12

trap11:                                           ; preds = %cont
  %3 = call i64 @write(i32 2, i8* getelementptr inbounds ([180 x i8], [180 x i8]* @trap_msg.5, i32 0, i32 0), i64 179)
  call void @exit(i32 1)
  unreachable

cont12:                                           ; preds = %cont
  %div13 = sdiv i32 %y8, %n9
  store i32 %div13, i32* %y, align 4
  ret void
}

define void @"Vec2.%=.i32"(%Vec2* %0, i32 %1) {
entry:
  %self = alloca %Vec2*, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %ref = load %Vec2*, %Vec2** %self, align 8
  %x = getelementptr %Vec2, %Vec2* %ref, i32 0, i32 0
  %ref1 = load %Vec2*, %Vec2** %self, align 8
  %x2 = getelementptr %Vec2, %Vec2* %ref1, i32 0, i32 0
  %x3 = load i32, i32* %x2, align 4
  %n4 = load i32, i32* %n, align 4
  %iszero = icmp eq i32 %n4, 0
  br i1 %iszero, label %trap, label %cont

trap:                                             ; preds = %entry
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([178 x i8], [178 x i8]* @trap_msg.6, i32 0, i32 0), i64 177)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %mod = srem i32 %x3, %n4
  store i32 %mod, i32* %x, align 4
  %ref5 = load %Vec2*, %Vec2** %self, align 8
  %y = getelementptr %Vec2, %Vec2* %ref5, i32 0, i32 1
  %ref6 = load %Vec2*, %Vec2** %self, align 8
  %y7 = getelementptr %Vec2, %Vec2* %ref6, i32 0, i32 1
  %y8 = load i32, i32* %y7, align 4
  %n9 = load i32, i32* %n, align 4
  %iszero10 = icmp eq i32 %n9, 0
  br i1 %iszero10, label %trap11, label %cont12

trap11:                                           ; preds = %cont
  %3 = call i64 @write(i32 2, i8* getelementptr inbounds ([178 x i8], [178 x i8]* @trap_msg.7, i32 0, i32 0), i64 177)
  call void @exit(i32 1)
  unreachable

cont12:                                           ; preds = %cont
  %mod13 = srem i32 %y8, %n9
  store i32 %mod13, i32* %y, align 4
  ret void
}

define i32 @Vec2.abs(%Vec2* %0, i32 %1) {
entry:
  %self = alloca %Vec2*, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %n1 = load i32, i32* %n, align 4
  %lt = icmp slt i32 %n1, 0
  br i1 %lt, label %then, label %endif

endif:                                            ; preds = %entry
  %n3 = load i32, i32* %n, align 4
  ret i32 %n3

then:                                             ; preds = %entry
  %n2 = load i32, i32* %n, align 4
  %sub = sub i32 0, %n2
  ret i32 %sub
}

define void @Vec2.show(%Vec2* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Vec2*, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  %label = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %label, align 8
  %label1 = load { i8*, i64 }, { i8*, i64 }* %label, align 8
  %str.len = extractvalue { i8*, i64 } %label1, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %label1, 0
  %ref = load %Vec2*, %Vec2** %self, align 8
  %x = getelementptr %Vec2, %Vec2* %ref, i32 0, i32 0
  %x2 = load i32, i32* %x, align 4
  %ref3 = load %Vec2*, %Vec2** %self, align 8
  %y = getelementptr %Vec2, %Vec2* %ref3, i32 0, i32 1
  %y4 = load i32, i32* %y, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @fmt, i32 0, i32 0), i32 %len32, i8* %str.data, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %x2, i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.8, i32 0, i32 0), i32 %y4, i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.9, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)

define i32 @main(i32 %0, i8** %1) {
entry:
  %a = alloca %Vec2, align 8
  %call = call %Vec2 @Vec2.create(i32 9, i32 12)
  store %Vec2 %call, %Vec2* %a, align 4
  %b = alloca %Vec2, align 8
  %call1 = call %Vec2 @Vec2.create(i32 2, i32 3)
  store %Vec2 %call1, %Vec2* %b, align 4
  %b2 = load %Vec2, %Vec2* %b, align 4
  %op = call %Vec2 @"Vec2.+.Vec2"(%Vec2* %a, %Vec2 %b2)
  %out.tmp = alloca %Vec2, align 8
  store %Vec2 %op, %Vec2* %out.tmp, align 4
  call void @Vec2.show(%Vec2* %out.tmp, { i8*, i64 } { i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.10, i32 0, i32 0), i64 8 })
  %b3 = load %Vec2, %Vec2* %b, align 4
  %op4 = call %Vec2 @Vec2.-.Vec2(%Vec2* %a, %Vec2 %b3)
  %out.tmp5 = alloca %Vec2, align 8
  store %Vec2 %op4, %Vec2* %out.tmp5, align 4
  call void @Vec2.show(%Vec2* %out.tmp5, { i8*, i64 } { i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.11, i32 0, i32 0), i64 8 })
  %op6 = call %Vec2 @"Vec2.*.i32"(%Vec2* %a, i32 3)
  %out.tmp7 = alloca %Vec2, align 8
  store %Vec2 %op6, %Vec2* %out.tmp7, align 4
  call void @Vec2.show(%Vec2* %out.tmp7, { i8*, i64 } { i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.12, i32 0, i32 0), i64 8 })
  %op8 = call %Vec2 @"Vec2./.i32"(%Vec2* %a, i32 2)
  %out.tmp9 = alloca %Vec2, align 8
  store %Vec2 %op8, %Vec2* %out.tmp9, align 4
  call void @Vec2.show(%Vec2* %out.tmp9, { i8*, i64 } { i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.13, i32 0, i32 0), i64 8 })
  %op10 = call %Vec2 @"Vec2.%.i32"(%Vec2* %a, i32 4)
  %out.tmp11 = alloca %Vec2, align 8
  store %Vec2 %op10, %Vec2* %out.tmp11, align 4
  call void @Vec2.show(%Vec2* %out.tmp11, { i8*, i64 } { i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.14, i32 0, i32 0), i64 8 })
  %b12 = load %Vec2, %Vec2* %b, align 4
  %op13 = call i1 @"Vec2.==.Vec2"(%Vec2* %a, %Vec2 %b12)
  %bool_str = select i1 %op13, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %b14 = load %Vec2, %Vec2* %b, align 4
  %op15 = call i1 @"Vec2.!=.Vec2"(%Vec2* %a, %Vec2 %b14)
  %bool_str16 = select i1 %op15, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.17, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.18, i32 0, i32 0)
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt.20, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.15, i32 0, i32 0), i8* %bool_str, i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.16, i32 0, i32 0), i8* %bool_str16, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.19, i32 0, i32 0))
  %b17 = load %Vec2, %Vec2* %b, align 4
  %op18 = call i1 @"Vec2.<.Vec2"(%Vec2* %a, %Vec2 %b17)
  %bool_str19 = select i1 %op18, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.22, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.23, i32 0, i32 0)
  %b20 = load %Vec2, %Vec2* %b, align 4
  %op21 = call i1 @"Vec2.>.Vec2"(%Vec2* %a, %Vec2 %b20)
  %bool_str22 = select i1 %op21, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.25, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.26, i32 0, i32 0)
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt.28, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.21, i32 0, i32 0), i8* %bool_str19, i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.24, i32 0, i32 0), i8* %bool_str22, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.27, i32 0, i32 0))
  %b23 = load %Vec2, %Vec2* %b, align 4
  %op24 = call i1 @"Vec2.<=.Vec2"(%Vec2* %a, %Vec2 %b23)
  %bool_str25 = select i1 %op24, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.30, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.31, i32 0, i32 0)
  %b26 = load %Vec2, %Vec2* %b, align 4
  %op27 = call i1 @"Vec2.>=.Vec2"(%Vec2* %a, %Vec2 %b26)
  %bool_str28 = select i1 %op27, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.33, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.34, i32 0, i32 0)
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt.36, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.29, i32 0, i32 0), i8* %bool_str25, i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.32, i32 0, i32 0), i8* %bool_str28, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.35, i32 0, i32 0))
  %c = alloca %Vec2, align 8
  %call29 = call %Vec2 @Vec2.create(i32 0, i32 0)
  store %Vec2 %call29, %Vec2* %c, align 4
  %a30 = load %Vec2, %Vec2* %a, align 4
  call void @"Vec2.=.Vec2"(%Vec2* %c, %Vec2 %a30)
  call void @Vec2.show(%Vec2* %c, { i8*, i64 } { i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.37, i32 0, i32 0), i64 9 })
  %b31 = load %Vec2, %Vec2* %b, align 4
  call void @"Vec2.+=.Vec2"(%Vec2* %c, %Vec2 %b31)
  call void @Vec2.show(%Vec2* %c, { i8*, i64 } { i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.38, i32 0, i32 0), i64 9 })
  %b32 = load %Vec2, %Vec2* %b, align 4
  call void @"Vec2.-=.Vec2"(%Vec2* %c, %Vec2 %b32)
  call void @Vec2.show(%Vec2* %c, { i8*, i64 } { i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.39, i32 0, i32 0), i64 9 })
  call void @"Vec2.*=.i32"(%Vec2* %c, i32 2)
  call void @Vec2.show(%Vec2* %c, { i8*, i64 } { i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.40, i32 0, i32 0), i64 9 })
  call void @"Vec2./=.i32"(%Vec2* %c, i32 3)
  call void @Vec2.show(%Vec2* %c, { i8*, i64 } { i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.41, i32 0, i32 0), i64 9 })
  call void @"Vec2.%=.i32"(%Vec2* %c, i32 5)
  call void @Vec2.show(%Vec2* %c, { i8*, i64 } { i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.42, i32 0, i32 0), i64 9 })
  %b33 = load %Vec2, %Vec2* %b, align 4
  %op34 = call i1 @"Vec2.>.Vec2"(%Vec2* %a, %Vec2 %b33)
  br i1 %op34, label %then, label %endif

endif:                                            ; preds = %then, %entry
  %n = alloca i32, align 4
  store i32 0, i32* %n, align 4
  %d = alloca %Vec2, align 8
  %call35 = call %Vec2 @Vec2.create(i32 1, i32 1)
  store %Vec2 %call35, %Vec2* %d, align 4
  br label %while.cond

then:                                             ; preds = %entry
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.44, i32 0, i32 0), i32 12, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.43, i32 0, i32 0))
  br label %endif

while.cond:                                       ; preds = %while.body, %endif
  %a36 = load %Vec2, %Vec2* %a, align 4
  %op37 = call i1 @"Vec2.<.Vec2"(%Vec2* %d, %Vec2 %a36)
  br i1 %op37, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %b38 = load %Vec2, %Vec2* %b, align 4
  call void @"Vec2.+=.Vec2"(%Vec2* %d, %Vec2 %b38)
  %cur = load i32, i32* %n, align 4
  %add = add i32 %cur, 1
  store i32 %add, i32* %n, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %n39 = load i32, i32* %n, align 4
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.47, i32 0, i32 0), i32 17, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str.45, i32 0, i32 0), i32 %n39, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.46, i32 0, i32 0))
  ret i32 0
}
```

## 024 — bitwise operator overloads, and one returning a scalar

```ura
// operators_overload/024.ura - bitwise overloads + a scalar return

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

main():
    a Flags = Flags::create(12)
    b Flags = Flags::create(10)
    output("and ", (a & b).bits, "  or ", (a | b).bits)
    output("  xor ", (a ^ b).bits, "\n")
    output("shl ", (a << 2).bits, "  shr ", (a >> 2).bits, "\n")

    p Vec2 = Vec2::create(2, 3)
    q Vec2 = Vec2::create(4, 5)
    output("dot ", p * q, "\n")
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

struct Flags
├─ bits : i32
├─ fn Flags.create(n : i32) : STRUCT_CALL
│  ├─ f : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ .bits : i32
│  │  │  └─ f : STRUCT_CALL
│  │  └─ n : i32
│  └─ return
│     └─ f : STRUCT_CALL
├─ fn Flags.&.Flags(self : STRUCT_CALL, other : STRUCT_CALL) : STRUCT_CALL
│  └─ return
│     └─ call create : STRUCT_CALL
│        └─ & : i32
│           ├─ .bits : i32
│           │  └─ self : STRUCT_CALL
│           └─ .bits : i32
│              └─ other : STRUCT_CALL
├─ fn Flags.|.Flags(self : STRUCT_CALL, other : STRUCT_CALL) : STRUCT_CALL
│  └─ return
│     └─ call create : STRUCT_CALL
│        └─ | : i32
│           ├─ .bits : i32
│           │  └─ self : STRUCT_CALL
│           └─ .bits : i32
│              └─ other : STRUCT_CALL
├─ fn Flags.^.Flags(self : STRUCT_CALL, other : STRUCT_CALL) : STRUCT_CALL
│  └─ return
│     └─ call create : STRUCT_CALL
│        └─ ^ : i32
│           ├─ .bits : i32
│           │  └─ self : STRUCT_CALL
│           └─ .bits : i32
│              └─ other : STRUCT_CALL
├─ fn Flags.<<.i32(self : STRUCT_CALL, n : i32) : STRUCT_CALL
│  └─ return
│     └─ call create : STRUCT_CALL
│        └─ << : i32
│           ├─ .bits : i32
│           │  └─ self : STRUCT_CALL
│           └─ n : i32
└─ fn Flags.>>.i32(self : STRUCT_CALL, n : i32) : STRUCT_CALL
   └─ return
      └─ call create : STRUCT_CALL
         └─ >> : i32
            ├─ .bits : i32
            │  └─ self : STRUCT_CALL
            └─ n : i32

struct Vec2
├─ x : i32
├─ y : i32
├─ fn Vec2.create(a : i32, b : i32) : STRUCT_CALL
│  ├─ v : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ .x : i32
│  │  │  └─ v : STRUCT_CALL
│  │  └─ a : i32
│  ├─ = : i32
│  │  ├─ .y : i32
│  │  │  └─ v : STRUCT_CALL
│  │  └─ b : i32
│  └─ return
│     └─ v : STRUCT_CALL
└─ fn Vec2.*.Vec2(self : STRUCT_CALL, other : STRUCT_CALL) : i32
   └─ return
      └─ + : i32
         ├─ * : i32
         │  ├─ .x : i32
         │  │  └─ self : STRUCT_CALL
         │  └─ .x : i32
         │     └─ other : STRUCT_CALL
         └─ * : i32
            ├─ .y : i32
            │  └─ self : STRUCT_CALL
            └─ .y : i32
               └─ other : STRUCT_CALL

fn main() : i32
├─ = : STRUCT_CALL
│  ├─ a : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     └─ int 12
├─ = : STRUCT_CALL
│  ├─ b : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     └─ int 10
├─ output : void
│  ├─ char[] "and "
│  ├─ .bits : i32
│  │  └─ & : STRUCT_CALL
│  │     ├─ a : STRUCT_CALL
│  │     └─ b : STRUCT_CALL
│  ├─ char[] "  or "
│  └─ .bits : i32
│     └─ | : STRUCT_CALL
│        ├─ a : STRUCT_CALL
│        └─ b : STRUCT_CALL
├─ output : void
│  ├─ char[] "  xor "
│  ├─ .bits : i32
│  │  └─ ^ : STRUCT_CALL
│  │     ├─ a : STRUCT_CALL
│  │     └─ b : STRUCT_CALL
│  └─ char[] "\n"
├─ output : void
│  ├─ char[] "shl "
│  ├─ .bits : i32
│  │  └─ << : STRUCT_CALL
│  │     ├─ a : STRUCT_CALL
│  │     └─ int 2
│  ├─ char[] "  shr "
│  ├─ .bits : i32
│  │  └─ >> : STRUCT_CALL
│  │     ├─ a : STRUCT_CALL
│  │     └─ int 2
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ p : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     ├─ int 2
│     └─ int 3
├─ = : STRUCT_CALL
│  ├─ q : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     ├─ int 4
│     └─ int 5
└─ output : void
   ├─ char[] "dot "
   ├─ * : i32
   │  ├─ p : STRUCT_CALL
   │  └─ q : STRUCT_CALL
   └─ char[] "\n"
```

```out
and 8  or 14  xor 6
shl 48  shr 3
dot 23
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Flags = type { i32 }
%Vec2 = type { i32, i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [5 x i8] c"and \00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"  or \00", align 1
@fmt = private unnamed_addr constant [13 x i8] c"%.*s%d%.*s%d\00", align 1
@str.2 = private unnamed_addr constant [7 x i8] c"  xor \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.5 = private unnamed_addr constant [5 x i8] c"shl \00", align 1
@str.6 = private unnamed_addr constant [7 x i8] c"  shr \00", align 1
@str.7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.8 = private unnamed_addr constant [17 x i8] c"%.*s%d%.*s%d%.*s\00", align 1
@str.9 = private unnamed_addr constant [5 x i8] c"dot \00", align 1
@str.10 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.11 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1

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

define %Flags @Flags.create(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %f = alloca %Flags, align 8
  store %Flags zeroinitializer, %Flags* %f, align 4
  %bits = getelementptr %Flags, %Flags* %f, i32 0, i32 0
  %n1 = load i32, i32* %n, align 4
  store i32 %n1, i32* %bits, align 4
  %f2 = load %Flags, %Flags* %f, align 4
  ret %Flags %f2
}

define %Flags @"Flags.&.Flags"(%Flags* %0, %Flags %1) {
entry:
  %self = alloca %Flags*, align 8
  store %Flags* %0, %Flags** %self, align 8
  %other = alloca %Flags, align 8
  store %Flags %1, %Flags* %other, align 4
  %ref = load %Flags*, %Flags** %self, align 8
  %bits = getelementptr %Flags, %Flags* %ref, i32 0, i32 0
  %bits1 = load i32, i32* %bits, align 4
  %bits2 = getelementptr %Flags, %Flags* %other, i32 0, i32 0
  %bits3 = load i32, i32* %bits2, align 4
  %band = and i32 %bits1, %bits3
  %call = call %Flags @Flags.create(i32 %band)
  ret %Flags %call
}

define %Flags @"Flags.|.Flags"(%Flags* %0, %Flags %1) {
entry:
  %self = alloca %Flags*, align 8
  store %Flags* %0, %Flags** %self, align 8
  %other = alloca %Flags, align 8
  store %Flags %1, %Flags* %other, align 4
  %ref = load %Flags*, %Flags** %self, align 8
  %bits = getelementptr %Flags, %Flags* %ref, i32 0, i32 0
  %bits1 = load i32, i32* %bits, align 4
  %bits2 = getelementptr %Flags, %Flags* %other, i32 0, i32 0
  %bits3 = load i32, i32* %bits2, align 4
  %bor = or i32 %bits1, %bits3
  %call = call %Flags @Flags.create(i32 %bor)
  ret %Flags %call
}

define %Flags @"Flags.^.Flags"(%Flags* %0, %Flags %1) {
entry:
  %self = alloca %Flags*, align 8
  store %Flags* %0, %Flags** %self, align 8
  %other = alloca %Flags, align 8
  store %Flags %1, %Flags* %other, align 4
  %ref = load %Flags*, %Flags** %self, align 8
  %bits = getelementptr %Flags, %Flags* %ref, i32 0, i32 0
  %bits1 = load i32, i32* %bits, align 4
  %bits2 = getelementptr %Flags, %Flags* %other, i32 0, i32 0
  %bits3 = load i32, i32* %bits2, align 4
  %bxor = xor i32 %bits1, %bits3
  %call = call %Flags @Flags.create(i32 %bxor)
  ret %Flags %call
}

define %Flags @"Flags.<<.i32"(%Flags* %0, i32 %1) {
entry:
  %self = alloca %Flags*, align 8
  store %Flags* %0, %Flags** %self, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %ref = load %Flags*, %Flags** %self, align 8
  %bits = getelementptr %Flags, %Flags* %ref, i32 0, i32 0
  %bits1 = load i32, i32* %bits, align 4
  %n2 = load i32, i32* %n, align 4
  %shl = shl i32 %bits1, %n2
  %call = call %Flags @Flags.create(i32 %shl)
  ret %Flags %call
}

define %Flags @"Flags.>>.i32"(%Flags* %0, i32 %1) {
entry:
  %self = alloca %Flags*, align 8
  store %Flags* %0, %Flags** %self, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %ref = load %Flags*, %Flags** %self, align 8
  %bits = getelementptr %Flags, %Flags* %ref, i32 0, i32 0
  %bits1 = load i32, i32* %bits, align 4
  %n2 = load i32, i32* %n, align 4
  %shr = ashr i32 %bits1, %n2
  %call = call %Flags @Flags.create(i32 %shr)
  ret %Flags %call
}

define %Vec2 @Vec2.create(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  %b = alloca i32, align 4
  store i32 %1, i32* %b, align 4
  %v = alloca %Vec2, align 8
  store %Vec2 zeroinitializer, %Vec2* %v, align 4
  %x = getelementptr %Vec2, %Vec2* %v, i32 0, i32 0
  %a1 = load i32, i32* %a, align 4
  store i32 %a1, i32* %x, align 4
  %y = getelementptr %Vec2, %Vec2* %v, i32 0, i32 1
  %b2 = load i32, i32* %b, align 4
  store i32 %b2, i32* %y, align 4
  %v3 = load %Vec2, %Vec2* %v, align 4
  ret %Vec2 %v3
}

define i32 @"Vec2.*.Vec2"(%Vec2* %0, %Vec2 %1) {
entry:
  %self = alloca %Vec2*, align 8
  store %Vec2* %0, %Vec2** %self, align 8
  %other = alloca %Vec2, align 8
  store %Vec2 %1, %Vec2* %other, align 4
  %ref = load %Vec2*, %Vec2** %self, align 8
  %x = getelementptr %Vec2, %Vec2* %ref, i32 0, i32 0
  %x1 = load i32, i32* %x, align 4
  %x2 = getelementptr %Vec2, %Vec2* %other, i32 0, i32 0
  %x3 = load i32, i32* %x2, align 4
  %mul = mul i32 %x1, %x3
  %ref4 = load %Vec2*, %Vec2** %self, align 8
  %y = getelementptr %Vec2, %Vec2* %ref4, i32 0, i32 1
  %y5 = load i32, i32* %y, align 4
  %y6 = getelementptr %Vec2, %Vec2* %other, i32 0, i32 1
  %y7 = load i32, i32* %y6, align 4
  %mul8 = mul i32 %y5, %y7
  %add = add i32 %mul, %mul8
  ret i32 %add
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %a = alloca %Flags, align 8
  %call = call %Flags @Flags.create(i32 12)
  store %Flags %call, %Flags* %a, align 4
  %b = alloca %Flags, align 8
  %call1 = call %Flags @Flags.create(i32 10)
  store %Flags %call1, %Flags* %b, align 4
  %b2 = load %Flags, %Flags* %b, align 4
  %op = call %Flags @"Flags.&.Flags"(%Flags* %a, %Flags %b2)
  %out.tmp = alloca %Flags, align 8
  store %Flags %op, %Flags* %out.tmp, align 4
  %bits = getelementptr %Flags, %Flags* %out.tmp, i32 0, i32 0
  %bits3 = load i32, i32* %bits, align 4
  %b4 = load %Flags, %Flags* %b, align 4
  %op5 = call %Flags @"Flags.|.Flags"(%Flags* %a, %Flags %b4)
  %out.tmp6 = alloca %Flags, align 8
  store %Flags %op5, %Flags* %out.tmp6, align 4
  %bits7 = getelementptr %Flags, %Flags* %out.tmp6, i32 0, i32 0
  %bits8 = load i32, i32* %bits7, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i32 4, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i32 %bits3, i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i32 %bits8)
  %b9 = load %Flags, %Flags* %b, align 4
  %op10 = call %Flags @"Flags.^.Flags"(%Flags* %a, %Flags %b9)
  %out.tmp11 = alloca %Flags, align 8
  store %Flags %op10, %Flags* %out.tmp11, align 4
  %bits12 = getelementptr %Flags, %Flags* %out.tmp11, i32 0, i32 0
  %bits13 = load i32, i32* %bits12, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.4, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.2, i32 0, i32 0), i32 %bits13, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %op14 = call %Flags @"Flags.<<.i32"(%Flags* %a, i32 2)
  %out.tmp15 = alloca %Flags, align 8
  store %Flags %op14, %Flags* %out.tmp15, align 4
  %bits16 = getelementptr %Flags, %Flags* %out.tmp15, i32 0, i32 0
  %bits17 = load i32, i32* %bits16, align 4
  %op18 = call %Flags @"Flags.>>.i32"(%Flags* %a, i32 2)
  %out.tmp19 = alloca %Flags, align 8
  store %Flags %op18, %Flags* %out.tmp19, align 4
  %bits20 = getelementptr %Flags, %Flags* %out.tmp19, i32 0, i32 0
  %bits21 = load i32, i32* %bits20, align 4
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt.8, i32 0, i32 0), i32 4, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.5, i32 0, i32 0), i32 %bits17, i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.6, i32 0, i32 0), i32 %bits21, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  %p = alloca %Vec2, align 8
  %call22 = call %Vec2 @Vec2.create(i32 2, i32 3)
  store %Vec2 %call22, %Vec2* %p, align 4
  %q = alloca %Vec2, align 8
  %call23 = call %Vec2 @Vec2.create(i32 4, i32 5)
  store %Vec2 %call23, %Vec2* %q, align 4
  %q24 = load %Vec2, %Vec2* %q, align 4
  %op25 = call i32 @"Vec2.*.Vec2"(%Vec2* %p, %Vec2 %q24)
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.11, i32 0, i32 0), i32 4, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.9, i32 0, i32 0), i32 %op25, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.10, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 025 — compound bitwise assignment overloads

```ura
// operators_overload/025.ura - &= |= ^= <<= >>= as overloads

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

main():
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

struct Flags
├─ bits : i32
├─ fn Flags.&=.i32(self : STRUCT_CALL, n : i32) : void
│  └─ = : i32
│     ├─ .bits : i32
│     │  └─ self : STRUCT_CALL
│     └─ & : i32
│        ├─ .bits : i32
│        │  └─ self : STRUCT_CALL
│        └─ n : i32
├─ fn Flags.|=.i32(self : STRUCT_CALL, n : i32) : void
│  └─ = : i32
│     ├─ .bits : i32
│     │  └─ self : STRUCT_CALL
│     └─ | : i32
│        ├─ .bits : i32
│        │  └─ self : STRUCT_CALL
│        └─ n : i32
├─ fn Flags.^=.i32(self : STRUCT_CALL, n : i32) : void
│  └─ = : i32
│     ├─ .bits : i32
│     │  └─ self : STRUCT_CALL
│     └─ ^ : i32
│        ├─ .bits : i32
│        │  └─ self : STRUCT_CALL
│        └─ n : i32
├─ fn Flags.<<=.i32(self : STRUCT_CALL, n : i32) : void
│  └─ = : i32
│     ├─ .bits : i32
│     │  └─ self : STRUCT_CALL
│     └─ << : i32
│        ├─ .bits : i32
│        │  └─ self : STRUCT_CALL
│        └─ n : i32
└─ fn Flags.>>=.i32(self : STRUCT_CALL, n : i32) : void
   └─ = : i32
      ├─ .bits : i32
      │  └─ self : STRUCT_CALL
      └─ >> : i32
         ├─ .bits : i32
         │  └─ self : STRUCT_CALL
         └─ n : i32

fn main() : i32
├─ f : STRUCT_CALL
├─ = : i32
│  ├─ .bits : i32
│  │  └─ f : STRUCT_CALL
│  └─ int 12
├─ &= : void
│  ├─ f : STRUCT_CALL
│  └─ int 10
├─ output : void
│  ├─ .bits : i32
│  │  └─ f : STRUCT_CALL
│  └─ char[] " "
├─ |= : void
│  ├─ f : STRUCT_CALL
│  └─ int 3
├─ output : void
│  ├─ .bits : i32
│  │  └─ f : STRUCT_CALL
│  └─ char[] " "
├─ ^= : void
│  ├─ f : STRUCT_CALL
│  └─ int 5
├─ output : void
│  ├─ .bits : i32
│  │  └─ f : STRUCT_CALL
│  └─ char[] " "
├─ <<= : void
│  ├─ f : STRUCT_CALL
│  └─ int 2
├─ output : void
│  ├─ .bits : i32
│  │  └─ f : STRUCT_CALL
│  └─ char[] " "
├─ >>= : void
│  ├─ f : STRUCT_CALL
│  └─ int 1
└─ output : void
   ├─ .bits : i32
   │  └─ f : STRUCT_CALL
   └─ char[] "\n"
```

```out
8 11 14 56 28
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Flags = type { i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c" \00", align 1
@fmt.2 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c" \00", align 1
@fmt.4 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c" \00", align 1
@fmt.6 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.8 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1

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

define void @"Flags.&=.i32"(%Flags* %0, i32 %1) {
entry:
  %self = alloca %Flags*, align 8
  store %Flags* %0, %Flags** %self, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %ref = load %Flags*, %Flags** %self, align 8
  %bits = getelementptr %Flags, %Flags* %ref, i32 0, i32 0
  %ref1 = load %Flags*, %Flags** %self, align 8
  %bits2 = getelementptr %Flags, %Flags* %ref1, i32 0, i32 0
  %bits3 = load i32, i32* %bits2, align 4
  %n4 = load i32, i32* %n, align 4
  %band = and i32 %bits3, %n4
  store i32 %band, i32* %bits, align 4
  ret void
}

define void @"Flags.|=.i32"(%Flags* %0, i32 %1) {
entry:
  %self = alloca %Flags*, align 8
  store %Flags* %0, %Flags** %self, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %ref = load %Flags*, %Flags** %self, align 8
  %bits = getelementptr %Flags, %Flags* %ref, i32 0, i32 0
  %ref1 = load %Flags*, %Flags** %self, align 8
  %bits2 = getelementptr %Flags, %Flags* %ref1, i32 0, i32 0
  %bits3 = load i32, i32* %bits2, align 4
  %n4 = load i32, i32* %n, align 4
  %bor = or i32 %bits3, %n4
  store i32 %bor, i32* %bits, align 4
  ret void
}

define void @"Flags.^=.i32"(%Flags* %0, i32 %1) {
entry:
  %self = alloca %Flags*, align 8
  store %Flags* %0, %Flags** %self, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %ref = load %Flags*, %Flags** %self, align 8
  %bits = getelementptr %Flags, %Flags* %ref, i32 0, i32 0
  %ref1 = load %Flags*, %Flags** %self, align 8
  %bits2 = getelementptr %Flags, %Flags* %ref1, i32 0, i32 0
  %bits3 = load i32, i32* %bits2, align 4
  %n4 = load i32, i32* %n, align 4
  %bxor = xor i32 %bits3, %n4
  store i32 %bxor, i32* %bits, align 4
  ret void
}

define void @"Flags.<<=.i32"(%Flags* %0, i32 %1) {
entry:
  %self = alloca %Flags*, align 8
  store %Flags* %0, %Flags** %self, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %ref = load %Flags*, %Flags** %self, align 8
  %bits = getelementptr %Flags, %Flags* %ref, i32 0, i32 0
  %ref1 = load %Flags*, %Flags** %self, align 8
  %bits2 = getelementptr %Flags, %Flags* %ref1, i32 0, i32 0
  %bits3 = load i32, i32* %bits2, align 4
  %n4 = load i32, i32* %n, align 4
  %shl = shl i32 %bits3, %n4
  store i32 %shl, i32* %bits, align 4
  ret void
}

define void @"Flags.>>=.i32"(%Flags* %0, i32 %1) {
entry:
  %self = alloca %Flags*, align 8
  store %Flags* %0, %Flags** %self, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %ref = load %Flags*, %Flags** %self, align 8
  %bits = getelementptr %Flags, %Flags* %ref, i32 0, i32 0
  %ref1 = load %Flags*, %Flags** %self, align 8
  %bits2 = getelementptr %Flags, %Flags* %ref1, i32 0, i32 0
  %bits3 = load i32, i32* %bits2, align 4
  %n4 = load i32, i32* %n, align 4
  %shr = ashr i32 %bits3, %n4
  store i32 %shr, i32* %bits, align 4
  ret void
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %f = alloca %Flags, align 8
  store %Flags zeroinitializer, %Flags* %f, align 4
  %bits = getelementptr %Flags, %Flags* %f, i32 0, i32 0
  store i32 12, i32* %bits, align 4
  call void @"Flags.&=.i32"(%Flags* %f, i32 10)
  %bits1 = getelementptr %Flags, %Flags* %f, i32 0, i32 0
  %bits2 = load i32, i32* %bits1, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %bits2, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  call void @"Flags.|=.i32"(%Flags* %f, i32 3)
  %bits3 = getelementptr %Flags, %Flags* %f, i32 0, i32 0
  %bits4 = load i32, i32* %bits3, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.2, i32 0, i32 0), i32 %bits4, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  call void @"Flags.^=.i32"(%Flags* %f, i32 5)
  %bits5 = getelementptr %Flags, %Flags* %f, i32 0, i32 0
  %bits6 = load i32, i32* %bits5, align 4
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.4, i32 0, i32 0), i32 %bits6, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  call void @"Flags.<<=.i32"(%Flags* %f, i32 2)
  %bits7 = getelementptr %Flags, %Flags* %f, i32 0, i32 0
  %bits8 = load i32, i32* %bits7, align 4
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.6, i32 0, i32 0), i32 %bits8, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  call void @"Flags.>>=.i32"(%Flags* %f, i32 1)
  %bits9 = getelementptr %Flags, %Flags* %f, i32 0, i32 0
  %bits10 = load i32, i32* %bits9, align 4
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.8, i32 0, i32 0), i32 %bits10, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 026 — bitwise and shift overloads taking a ref parameter

```ura
// operators_overload/026.ura - bitwise and shift overloads taking a ref parameter

struct Flags:
    bits i32

    operator = (n i32) void:
        self.bits = n

    operator = (ref f Flags) void:
        self.bits = f.bits

    operator & (ref f Flags) Flags:
        res Flags
        res.bits = self.bits & f.bits
        return res

    operator | (ref f Flags) Flags:
        res Flags
        res.bits = self.bits | f.bits
        return res

    operator ^ (ref f Flags) Flags:
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

main():
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

struct Flags
├─ bits : i32
├─ fn Flags.=.i32(self : STRUCT_CALL, n : i32) : void
│  └─ = : i32
│     ├─ .bits : i32
│     │  └─ self : STRUCT_CALL
│     └─ n : i32
├─ fn Flags.=.ref.Flags(self : STRUCT_CALL, f : STRUCT_CALL) : void
│  └─ = : i32
│     ├─ .bits : i32
│     │  └─ self : STRUCT_CALL
│     └─ .bits : i32
│        └─ f : STRUCT_CALL
├─ fn Flags.&.ref.Flags(self : STRUCT_CALL, f : STRUCT_CALL) : STRUCT_CALL
│  ├─ res : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ .bits : i32
│  │  │  └─ res : STRUCT_CALL
│  │  └─ & : i32
│  │     ├─ .bits : i32
│  │     │  └─ self : STRUCT_CALL
│  │     └─ .bits : i32
│  │        └─ f : STRUCT_CALL
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn Flags.|.ref.Flags(self : STRUCT_CALL, f : STRUCT_CALL) : STRUCT_CALL
│  ├─ res : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ .bits : i32
│  │  │  └─ res : STRUCT_CALL
│  │  └─ | : i32
│  │     ├─ .bits : i32
│  │     │  └─ self : STRUCT_CALL
│  │     └─ .bits : i32
│  │        └─ f : STRUCT_CALL
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn Flags.^.ref.Flags(self : STRUCT_CALL, f : STRUCT_CALL) : STRUCT_CALL
│  ├─ res : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ .bits : i32
│  │  │  └─ res : STRUCT_CALL
│  │  └─ ^ : i32
│  │     ├─ .bits : i32
│  │     │  └─ self : STRUCT_CALL
│  │     └─ .bits : i32
│  │        └─ f : STRUCT_CALL
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn Flags.<<.i32(self : STRUCT_CALL, n : i32) : STRUCT_CALL
│  ├─ res : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ .bits : i32
│  │  │  └─ res : STRUCT_CALL
│  │  └─ << : i32
│  │     ├─ .bits : i32
│  │     │  └─ self : STRUCT_CALL
│  │     └─ n : i32
│  └─ return
│     └─ res : STRUCT_CALL
└─ fn Flags.>>.i32(self : STRUCT_CALL, n : i32) : STRUCT_CALL
   ├─ res : STRUCT_CALL
   ├─ = : i32
   │  ├─ .bits : i32
   │  │  └─ res : STRUCT_CALL
   │  └─ >> : i32
   │     ├─ .bits : i32
   │     │  └─ self : STRUCT_CALL
   │     └─ n : i32
   └─ return
      └─ res : STRUCT_CALL

fn main() : i32
├─ a : STRUCT_CALL
├─ = : void
│  ├─ a : STRUCT_CALL
│  └─ int 12
├─ b : STRUCT_CALL
├─ = : void
│  ├─ b : STRUCT_CALL
│  └─ int 10
├─ = : STRUCT_CALL
│  ├─ band : STRUCT_CALL
│  └─ & : STRUCT_CALL
│     ├─ a : STRUCT_CALL
│     └─ b : STRUCT_CALL
├─ output : void
│  ├─ char[] "a & b:  "
│  ├─ .bits : i32
│  │  └─ band : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ bor : STRUCT_CALL
│  └─ | : STRUCT_CALL
│     ├─ a : STRUCT_CALL
│     └─ b : STRUCT_CALL
├─ output : void
│  ├─ char[] "a | b:  "
│  ├─ .bits : i32
│  │  └─ bor : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ bxor : STRUCT_CALL
│  └─ ^ : STRUCT_CALL
│     ├─ a : STRUCT_CALL
│     └─ b : STRUCT_CALL
├─ output : void
│  ├─ char[] "a ^ b:  "
│  ├─ .bits : i32
│  │  └─ bxor : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ lsh : STRUCT_CALL
│  └─ << : STRUCT_CALL
│     ├─ a : STRUCT_CALL
│     └─ int 2
├─ output : void
│  ├─ char[] "a << 2: "
│  ├─ .bits : i32
│  │  └─ lsh : STRUCT_CALL
│  └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ rsh : STRUCT_CALL
│  └─ >> : STRUCT_CALL
│     ├─ a : STRUCT_CALL
│     └─ int 1
└─ output : void
   ├─ char[] "a >> 1: "
   ├─ .bits : i32
   │  └─ rsh : STRUCT_CALL
   └─ char[] "\n"
```

```out
a & b:  8
a | b:  14
a ^ b:  6
a << 2: 48
a >> 1: 6
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Flags = type { i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [9 x i8] c"a & b:  \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.2 = private unnamed_addr constant [9 x i8] c"a | b:  \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.5 = private unnamed_addr constant [9 x i8] c"a ^ b:  \00", align 1
@str.6 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.7 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.8 = private unnamed_addr constant [9 x i8] c"a << 2: \00", align 1
@str.9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.10 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.11 = private unnamed_addr constant [9 x i8] c"a >> 1: \00", align 1
@str.12 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.13 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1

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

define void @"Flags.=.i32"(%Flags* %0, i32 %1) {
entry:
  %self = alloca %Flags*, align 8
  store %Flags* %0, %Flags** %self, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %ref = load %Flags*, %Flags** %self, align 8
  %bits = getelementptr %Flags, %Flags* %ref, i32 0, i32 0
  %n1 = load i32, i32* %n, align 4
  store i32 %n1, i32* %bits, align 4
  ret void
}

define void @"Flags.=.ref.Flags"(%Flags* %0, %Flags* %1) {
entry:
  %self = alloca %Flags*, align 8
  store %Flags* %0, %Flags** %self, align 8
  %f = alloca %Flags*, align 8
  store %Flags* %1, %Flags** %f, align 8
  %ref = load %Flags*, %Flags** %self, align 8
  %bits = getelementptr %Flags, %Flags* %ref, i32 0, i32 0
  %ref1 = load %Flags*, %Flags** %f, align 8
  %bits2 = getelementptr %Flags, %Flags* %ref1, i32 0, i32 0
  %bits3 = load i32, i32* %bits2, align 4
  store i32 %bits3, i32* %bits, align 4
  ret void
}

define %Flags @"Flags.&.ref.Flags"(%Flags* %0, %Flags* %1) {
entry:
  %self = alloca %Flags*, align 8
  store %Flags* %0, %Flags** %self, align 8
  %f = alloca %Flags*, align 8
  store %Flags* %1, %Flags** %f, align 8
  %res = alloca %Flags, align 8
  store %Flags zeroinitializer, %Flags* %res, align 4
  %bits = getelementptr %Flags, %Flags* %res, i32 0, i32 0
  %ref = load %Flags*, %Flags** %self, align 8
  %bits1 = getelementptr %Flags, %Flags* %ref, i32 0, i32 0
  %bits2 = load i32, i32* %bits1, align 4
  %ref3 = load %Flags*, %Flags** %f, align 8
  %bits4 = getelementptr %Flags, %Flags* %ref3, i32 0, i32 0
  %bits5 = load i32, i32* %bits4, align 4
  %band = and i32 %bits2, %bits5
  store i32 %band, i32* %bits, align 4
  %res6 = load %Flags, %Flags* %res, align 4
  ret %Flags %res6
}

define %Flags @"Flags.|.ref.Flags"(%Flags* %0, %Flags* %1) {
entry:
  %self = alloca %Flags*, align 8
  store %Flags* %0, %Flags** %self, align 8
  %f = alloca %Flags*, align 8
  store %Flags* %1, %Flags** %f, align 8
  %res = alloca %Flags, align 8
  store %Flags zeroinitializer, %Flags* %res, align 4
  %bits = getelementptr %Flags, %Flags* %res, i32 0, i32 0
  %ref = load %Flags*, %Flags** %self, align 8
  %bits1 = getelementptr %Flags, %Flags* %ref, i32 0, i32 0
  %bits2 = load i32, i32* %bits1, align 4
  %ref3 = load %Flags*, %Flags** %f, align 8
  %bits4 = getelementptr %Flags, %Flags* %ref3, i32 0, i32 0
  %bits5 = load i32, i32* %bits4, align 4
  %bor = or i32 %bits2, %bits5
  store i32 %bor, i32* %bits, align 4
  %res6 = load %Flags, %Flags* %res, align 4
  ret %Flags %res6
}

define %Flags @"Flags.^.ref.Flags"(%Flags* %0, %Flags* %1) {
entry:
  %self = alloca %Flags*, align 8
  store %Flags* %0, %Flags** %self, align 8
  %f = alloca %Flags*, align 8
  store %Flags* %1, %Flags** %f, align 8
  %res = alloca %Flags, align 8
  store %Flags zeroinitializer, %Flags* %res, align 4
  %bits = getelementptr %Flags, %Flags* %res, i32 0, i32 0
  %ref = load %Flags*, %Flags** %self, align 8
  %bits1 = getelementptr %Flags, %Flags* %ref, i32 0, i32 0
  %bits2 = load i32, i32* %bits1, align 4
  %ref3 = load %Flags*, %Flags** %f, align 8
  %bits4 = getelementptr %Flags, %Flags* %ref3, i32 0, i32 0
  %bits5 = load i32, i32* %bits4, align 4
  %bxor = xor i32 %bits2, %bits5
  store i32 %bxor, i32* %bits, align 4
  %res6 = load %Flags, %Flags* %res, align 4
  ret %Flags %res6
}

define %Flags @"Flags.<<.i32"(%Flags* %0, i32 %1) {
entry:
  %self = alloca %Flags*, align 8
  store %Flags* %0, %Flags** %self, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %res = alloca %Flags, align 8
  store %Flags zeroinitializer, %Flags* %res, align 4
  %bits = getelementptr %Flags, %Flags* %res, i32 0, i32 0
  %ref = load %Flags*, %Flags** %self, align 8
  %bits1 = getelementptr %Flags, %Flags* %ref, i32 0, i32 0
  %bits2 = load i32, i32* %bits1, align 4
  %n3 = load i32, i32* %n, align 4
  %shl = shl i32 %bits2, %n3
  store i32 %shl, i32* %bits, align 4
  %res4 = load %Flags, %Flags* %res, align 4
  ret %Flags %res4
}

define %Flags @"Flags.>>.i32"(%Flags* %0, i32 %1) {
entry:
  %self = alloca %Flags*, align 8
  store %Flags* %0, %Flags** %self, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %res = alloca %Flags, align 8
  store %Flags zeroinitializer, %Flags* %res, align 4
  %bits = getelementptr %Flags, %Flags* %res, i32 0, i32 0
  %ref = load %Flags*, %Flags** %self, align 8
  %bits1 = getelementptr %Flags, %Flags* %ref, i32 0, i32 0
  %bits2 = load i32, i32* %bits1, align 4
  %n3 = load i32, i32* %n, align 4
  %shr = ashr i32 %bits2, %n3
  store i32 %shr, i32* %bits, align 4
  %res4 = load %Flags, %Flags* %res, align 4
  ret %Flags %res4
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %a = alloca %Flags, align 8
  store %Flags zeroinitializer, %Flags* %a, align 4
  call void @"Flags.=.i32"(%Flags* %a, i32 12)
  %b = alloca %Flags, align 8
  store %Flags zeroinitializer, %Flags* %b, align 4
  call void @"Flags.=.i32"(%Flags* %b, i32 10)
  %band = alloca %Flags, align 8
  %op = call %Flags @"Flags.&.ref.Flags"(%Flags* %a, %Flags* %b)
  store %Flags %op, %Flags* %band, align 4
  %bits = getelementptr %Flags, %Flags* %band, i32 0, i32 0
  %bits1 = load i32, i32* %bits, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %bits1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %bor = alloca %Flags, align 8
  %op2 = call %Flags @"Flags.|.ref.Flags"(%Flags* %a, %Flags* %b)
  store %Flags %op2, %Flags* %bor, align 4
  %bits3 = getelementptr %Flags, %Flags* %bor, i32 0, i32 0
  %bits4 = load i32, i32* %bits3, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.4, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.2, i32 0, i32 0), i32 %bits4, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %bxor = alloca %Flags, align 8
  %op5 = call %Flags @"Flags.^.ref.Flags"(%Flags* %a, %Flags* %b)
  store %Flags %op5, %Flags* %bxor, align 4
  %bits6 = getelementptr %Flags, %Flags* %bxor, i32 0, i32 0
  %bits7 = load i32, i32* %bits6, align 4
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.7, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.5, i32 0, i32 0), i32 %bits7, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.6, i32 0, i32 0))
  %lsh = alloca %Flags, align 8
  %op8 = call %Flags @"Flags.<<.i32"(%Flags* %a, i32 2)
  store %Flags %op8, %Flags* %lsh, align 4
  %bits9 = getelementptr %Flags, %Flags* %lsh, i32 0, i32 0
  %bits10 = load i32, i32* %bits9, align 4
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.10, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.8, i32 0, i32 0), i32 %bits10, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.9, i32 0, i32 0))
  %rsh = alloca %Flags, align 8
  %op11 = call %Flags @"Flags.>>.i32"(%Flags* %a, i32 1)
  store %Flags %op11, %Flags* %rsh, align 4
  %bits12 = getelementptr %Flags, %Flags* %rsh, i32 0, i32 0
  %bits13 = load i32, i32* %bits12, align 4
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.13, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.11, i32 0, i32 0), i32 %bits13, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.12, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 027 — a heap-owning struct: create, deep-copy operator = and drop

```ura
// operators_overload/027.ura - a heap-owning struct: create, deep-copy operator = and drop

use "@/string"

struct Buffer:
    value char[]

    pub fn create() Buffer:
        b Buffer
        b.value = new char[64]
        return b

    operator drop:
        clean self.value

    operator = (s char[]) void:
        strcpy(self.value, s)

    operator = (ref b Buffer) void:
        strcpy(self.value, b.value)

main():
    b1 Buffer = Buffer::create()
    b1 = "hello"

    b2 Buffer = Buffer::create()
    b2 = b1

    strcpy(b2.value, "world")

    output("b1: ", b1.value, "\n")
    output("b2: ", b2.value, "\n")
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

proto fn strlen(s : pointer) : i64

proto fn strnlen(s : pointer, maxlen : i32) : i32

proto fn strcmp(a : pointer, b : pointer) : i32

proto fn strncmp(a : pointer, b : pointer, n : i32) : i32

proto fn strcasecmp(a : pointer, b : pointer) : i32

proto fn strncasecmp(a : pointer, b : pointer, n : i32) : i32

proto fn strcoll(a : pointer, b : pointer) : i32

proto fn strxfrm(dest : pointer, src : pointer, n : i32) : i32

proto fn strcpy(dest : pointer, src : pointer) : pointer

proto fn strncpy(dest : pointer, src : pointer, n : i32) : pointer

proto fn strdup(s : pointer) : pointer

proto fn strndup(s : pointer, n : i32) : pointer

proto fn strcat(dest : pointer, src : pointer) : pointer

proto fn strncat(dest : pointer, src : pointer, n : i32) : pointer

proto fn strchr(s : pointer, c : i32) : pointer

proto fn strrchr(s : pointer, c : i32) : pointer

proto fn strstr(haystack : pointer, needle : pointer) : pointer

proto fn strcasestr(haystack : pointer, needle : pointer) : pointer

proto fn strspn(s : pointer, accept : pointer) : i32

proto fn strcspn(s : pointer, reject : pointer) : i32

proto fn strpbrk(s : pointer, accept : pointer) : pointer

proto fn strtok(s : pointer, delim : pointer) : pointer

proto fn strtok_r(s : pointer, delim : pointer, saveptr : pointer) : pointer

proto fn strerror(errnum : i32) : pointer

proto fn strerror_r(errnum : i32, buf : pointer, buflen : i64) : i32

proto fn memchr(ptr : pointer, c : i32, n : i32) : pointer

proto fn memrchr(ptr : pointer, c : i32, n : i32) : pointer

proto fn memcmp(a : pointer, b : pointer, n : i32) : i32

proto fn memcpy(dest : pointer, src : pointer, n : i32) : pointer

proto fn memmove(dest : pointer, src : pointer, n : i32) : pointer

proto fn memset(ptr : pointer, value : i32, n : i32) : pointer

proto fn memccpy(dest : pointer, src : pointer, c : i32, n : i32) : pointer

proto fn bzero(s : pointer, size : i64) : void

proto fn strlcpy(dest : pointer, src : pointer, size : i32) : i32

proto fn strlcat(dest : pointer, src : pointer, size : i32) : i32

proto fn strsignal(sig : i32) : pointer

proto fn memset_s(ptr : pointer, size : i32, value : i32, n : i32) : pointer

proto fn atoi(s : pointer) : i32

struct String
├─ value : char[]
├─ count : i32
├─ fn String.create() : STRUCT_CALL
│  ├─ s : STRUCT_CALL
│  ├─ = : array
│  │  ├─ .value : char[]
│  │  │  └─ s : STRUCT_CALL
│  │  └─ array : char[]
│  │     └─ int 16
│  ├─ = : i32
│  │  ├─ .count : i32
│  │  │  └─ s : STRUCT_CALL
│  │  └─ int 0
│  └─ return
│     └─ s : STRUCT_CALL
├─ fn String.from(str : array) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ s : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  ├─ call assign : void
│  │  ├─ s : STRUCT_CALL
│  │  └─ str : char[]
│  └─ return
│     └─ s : STRUCT_CALL
├─ fn String.from_int(n : i32) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ s : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ n : i32
│  │  │  └─ int 0
│  │  ├─ call push : void
│  │  │  ├─ s : STRUCT_CALL
│  │  │  └─ char '0'
│  │  └─ return
│  │     └─ s : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ num : i32
│  │  └─ n : i32
│  ├─ if
│  │  ├─ condition < : bool
│  │  │  ├─ num : i32
│  │  │  └─ int 0
│  │  ├─ call push : void
│  │  │  ├─ s : STRUCT_CALL
│  │  │  └─ char '-'
│  │  └─ = : i32
│  │     ├─ num : i32
│  │     └─ - : i32
│  │        ├─ int 0
│  │        └─ num : i32
│  ├─ = : array
│  │  ├─ digits : char[]
│  │  └─ array : char[]
│  │     └─ int 12
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ int 0
│  ├─ while
│  │  ├─ condition > : bool
│  │  │  ├─ num : i32
│  │  │  └─ int 0
│  │  ├─ = : char
│  │  │  ├─ index : char
│  │  │  │  ├─ digits : char[]
│  │  │  │  └─ i : i32
│  │  │  └─ cast : char
│  │  │     └─ + : i32
│  │  │        ├─ % : i32
│  │  │        │  ├─ num : i32
│  │  │        │  └─ int 10
│  │  │        └─ int 48
│  │  ├─ = : i32
│  │  │  ├─ num : i32
│  │  │  └─ / : i32
│  │  │     ├─ num : i32
│  │  │     └─ int 10
│  │  └─ += : i32
│  │     ├─ i : i32
│  │     └─ int 1
│  ├─ while
│  │  ├─ condition > : bool
│  │  │  ├─ i : i32
│  │  │  └─ int 0
│  │  ├─ -= : i32
│  │  │  ├─ i : i32
│  │  │  └─ int 1
│  │  └─ call push : void
│  │     ├─ s : STRUCT_CALL
│  │     └─ index : char
│  │        ├─ digits : char[]
│  │        └─ i : i32
│  ├─ clean : void
│  │  └─ digits : char[]
│  └─ return
│     └─ s : STRUCT_CALL
├─ fn String.drop(self : STRUCT_CALL) : void
│  └─ clean : void
│     └─ .value : char[]
│        └─ self : STRUCT_CALL
├─ fn String.cap(self : STRUCT_CALL) : i32
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ .value : char[]
│  │  │  │  └─ self : STRUCT_CALL
│  │  │  └─ NULL_LIT : char[]
│  │  └─ return
│  │     └─ int 0
│  └─ return
│     └─ cast : i32
│        └─ .len : u64
│           └─ .value : char[]
│              └─ self : STRUCT_CALL
├─ fn String.grow(self : STRUCT_CALL, want : i32) : void
│  ├─ = : i32
│  │  ├─ room : i32
│  │  └─ call cap : i32
│  │     └─ self : STRUCT_CALL
│  ├─ if
│  │  ├─ condition >= : bool
│  │  │  ├─ room : i32
│  │  │  └─ want : i32
│  │  └─ return
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ room : i32
│  │  │  └─ int 0
│  │  └─ = : i32
│  │     ├─ room : i32
│  │     └─ int 16
│  ├─ while
│  │  ├─ condition < : bool
│  │  │  ├─ room : i32
│  │  │  └─ want : i32
│  │  └─ *= : i32
│  │     ├─ room : i32
│  │     └─ int 2
│  ├─ = : array
│  │  ├─ bigger : char[]
│  │  └─ array : char[]
│  │     └─ room : i32
│  ├─ if
│  │  ├─ condition > : bool
│  │  │  ├─ .count : i32
│  │  │  │  └─ self : STRUCT_CALL
│  │  │  └─ int 0
│  │  └─ call memcpy : pointer
│  │     ├─ bigger : char[]
│  │     ├─ FALLBACK : char[]
│  │     │  ├─ .value : char[]
│  │     │  │  └─ self : STRUCT_CALL
│  │     │  └─ char[] ""
│  │     └─ .count : i32
│  │        └─ self : STRUCT_CALL
│  ├─ = : char
│  │  ├─ index : char
│  │  │  ├─ bigger : char[]
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  └─ cast : char
│  │     └─ int 0
│  ├─ clean : void
│  │  └─ .value : char[]
│  │     └─ self : STRUCT_CALL
│  └─ = : array
│     ├─ .value : char[]
│     │  └─ self : STRUCT_CALL
│     └─ bigger : char[]
├─ fn String.assign(self : STRUCT_CALL, str : array) : void
│  ├─ = : i32
│  │  ├─ n : i32
│  │  └─ int 0
│  ├─ if
│  │  ├─ condition != : bool
│  │  │  ├─ str : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  └─ = : i32
│  │     ├─ n : i32
│  │     └─ cast : i32
│  │        └─ call strlen : i64
│  │           └─ str : char[]
│  ├─ call grow : void
│  │  ├─ self : STRUCT_CALL
│  │  └─ + : i32
│  │     ├─ n : i32
│  │     └─ int 1
│  ├─ if
│  │  ├─ condition > : bool
│  │  │  ├─ n : i32
│  │  │  └─ int 0
│  │  └─ call memcpy : pointer
│  │     ├─ FALLBACK : char[]
│  │     │  ├─ .value : char[]
│  │     │  │  └─ self : STRUCT_CALL
│  │     │  └─ char[] ""
│  │     ├─ FALLBACK : char[]
│  │     │  ├─ str : char[]
│  │     │  └─ char[] ""
│  │     └─ n : i32
│  ├─ = : char
│  │  ├─ index : char
│  │  │  ├─ .value : char[]
│  │  │  │  └─ self : STRUCT_CALL
│  │  │  └─ n : i32
│  │  └─ cast : char
│  │     └─ int 0
│  └─ = : i32
│     ├─ .count : i32
│     │  └─ self : STRUCT_CALL
│     └─ n : i32
├─ fn String.join(self : STRUCT_CALL, str : array) : void
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ str : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  └─ return
│  ├─ = : i32
│  │  ├─ n : i32
│  │  └─ cast : i32
│  │     └─ call strlen : i64
│  │        └─ str : char[]
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ n : i32
│  │  │  └─ int 0
│  │  └─ return
│  ├─ call grow : void
│  │  ├─ self : STRUCT_CALL
│  │  └─ + : i32
│  │     ├─ + : i32
│  │     │  ├─ .count : i32
│  │     │  │  └─ self : STRUCT_CALL
│  │     │  └─ n : i32
│  │     └─ int 1
│  ├─ call memcpy : pointer
│  │  ├─ index : char[]
│  │  │  ├─ .value : char[]
│  │  │  │  └─ self : STRUCT_CALL
│  │  │  └─ range : i32
│  │  │     ├─ .count : i32
│  │  │     │  └─ self : STRUCT_CALL
│  │  │     └─ + : i32
│  │  │        ├─ .count : i32
│  │  │        │  └─ self : STRUCT_CALL
│  │  │        └─ n : i32
│  │  ├─ str : char[]
│  │  └─ n : i32
│  ├─ += : i32
│  │  ├─ .count : i32
│  │  │  └─ self : STRUCT_CALL
│  │  └─ n : i32
│  └─ = : char
│     ├─ index : char
│     │  ├─ .value : char[]
│     │  │  └─ self : STRUCT_CALL
│     │  └─ .count : i32
│     │     └─ self : STRUCT_CALL
│     └─ cast : char
│        └─ int 0
├─ fn String.push(self : STRUCT_CALL, c : char) : void
│  ├─ call grow : void
│  │  ├─ self : STRUCT_CALL
│  │  └─ + : i32
│  │     ├─ .count : i32
│  │     │  └─ self : STRUCT_CALL
│  │     └─ int 2
│  ├─ = : char
│  │  ├─ index : char
│  │  │  ├─ .value : char[]
│  │  │  │  └─ self : STRUCT_CALL
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  └─ c : char
│  ├─ += : i32
│  │  ├─ .count : i32
│  │  │  └─ self : STRUCT_CALL
│  │  └─ int 1
│  └─ = : char
│     ├─ index : char
│     │  ├─ .value : char[]
│     │  │  └─ self : STRUCT_CALL
│     │  └─ .count : i32
│     │     └─ self : STRUCT_CALL
│     └─ cast : char
│        └─ int 0
├─ fn String.pop(self : STRUCT_CALL) : char
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ .count : i32
│  │  │  │  └─ self : STRUCT_CALL
│  │  │  └─ int 0
│  │  └─ return
│  │     └─ cast : char
│  │        └─ int 0
│  ├─ -= : i32
│  │  ├─ .count : i32
│  │  │  └─ self : STRUCT_CALL
│  │  └─ int 1
│  ├─ = : char
│  │  ├─ c : char
│  │  └─ index : char
│  │     ├─ .value : char[]
│  │     │  └─ self : STRUCT_CALL
│  │     └─ .count : i32
│  │        └─ self : STRUCT_CALL
│  ├─ = : char
│  │  ├─ index : char
│  │  │  ├─ .value : char[]
│  │  │  │  └─ self : STRUCT_CALL
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  └─ cast : char
│  │     └─ int 0
│  └─ return
│     └─ c : char
├─ fn String.clear(self : STRUCT_CALL) : void
│  ├─ = : i32
│  │  ├─ .count : i32
│  │  │  └─ self : STRUCT_CALL
│  │  └─ int 0
│  └─ if
│     ├─ condition != : bool
│     │  ├─ .value : char[]
│     │  │  └─ self : STRUCT_CALL
│     │  └─ NULL_LIT : char[]
│     └─ = : char
│        ├─ index : char
│        │  ├─ .value : char[]
│        │  │  └─ self : STRUCT_CALL
│        │  └─ int 0
│        └─ cast : char
│           └─ int 0
├─ fn String.len(self : STRUCT_CALL) : i32
│  └─ return
│     └─ .count : i32
│        └─ self : STRUCT_CALL
├─ fn String.empty(self : STRUCT_CALL) : bool
│  └─ return
│     └─ == : bool
│        ├─ .count : i32
│        │  └─ self : STRUCT_CALL
│        └─ int 0
├─ fn String.c_str(self : STRUCT_CALL) : char[]
│  └─ return
│     └─ FALLBACK : char[]
│        ├─ .value : char[]
│        │  └─ self : STRUCT_CALL
│        └─ char[] ""
├─ fn String.at(self : STRUCT_CALL, i : i32) : char
│  ├─ if
│  │  ├─ condition or : bool
│  │  │  ├─ < : bool
│  │  │  │  ├─ i : i32
│  │  │  │  └─ int 0
│  │  │  └─ >= : bool
│  │  │     ├─ i : i32
│  │  │     └─ .count : i32
│  │  │        └─ self : STRUCT_CALL
│  │  └─ return
│  │     └─ cast : char
│  │        └─ int 0
│  └─ return
│     └─ index : char
│        ├─ .value : char[]
│        │  └─ self : STRUCT_CALL
│        └─ i : i32
├─ fn String.find(self : STRUCT_CALL, needle : array) : i32
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ needle : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  └─ return
│  │     └─ - : i32
│  │        ├─ int 0
│  │        └─ int 1
│  ├─ = : i32
│  │  ├─ n : i32
│  │  └─ cast : i32
│  │     └─ call strlen : i64
│  │        └─ needle : char[]
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ n : i32
│  │  │  └─ int 0
│  │  └─ return
│  │     └─ int 0
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ int 0
│  ├─ while
│  │  ├─ condition <= : bool
│  │  │  ├─ + : i32
│  │  │  │  ├─ i : i32
│  │  │  │  └─ n : i32
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  ├─ if
│  │  │  ├─ condition == : bool
│  │  │  │  ├─ call strncmp : i32
│  │  │  │  │  ├─ index : char[]
│  │  │  │  │  │  ├─ .value : char[]
│  │  │  │  │  │  │  └─ self : STRUCT_CALL
│  │  │  │  │  │  └─ range : i32
│  │  │  │  │  │     ├─ i : i32
│  │  │  │  │  │     └─ + : i32
│  │  │  │  │  │        ├─ i : i32
│  │  │  │  │  │        └─ n : i32
│  │  │  │  │  ├─ needle : char[]
│  │  │  │  │  └─ n : i32
│  │  │  │  └─ int 0
│  │  │  └─ return
│  │  │     └─ i : i32
│  │  └─ += : i32
│  │     ├─ i : i32
│  │     └─ int 1
│  └─ return
│     └─ - : i32
│        ├─ int 0
│        └─ int 1
├─ fn String.contains(self : STRUCT_CALL, needle : array) : bool
│  └─ return
│     └─ != : bool
│        ├─ call find : i32
│        │  ├─ self : STRUCT_CALL
│        │  └─ needle : char[]
│        └─ - : i32
│           ├─ int 0
│           └─ int 1
├─ fn String.starts_with(self : STRUCT_CALL, prefix : array) : bool
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ prefix : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  └─ return
│  │     └─ bool True
│  ├─ = : i32
│  │  ├─ n : i32
│  │  └─ cast : i32
│  │     └─ call strlen : i64
│  │        └─ prefix : char[]
│  ├─ if
│  │  ├─ condition > : bool
│  │  │  ├─ n : i32
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  └─ return
│  │     └─ bool False
│  └─ return
│     └─ == : bool
│        ├─ call strncmp : i32
│        │  ├─ index : char[]
│        │  │  ├─ .value : char[]
│        │  │  │  └─ self : STRUCT_CALL
│        │  │  └─ range : i32
│        │  │     ├─ int 0
│        │  │     └─ n : i32
│        │  ├─ prefix : char[]
│        │  └─ n : i32
│        └─ int 0
├─ fn String.ends_with(self : STRUCT_CALL, suffix : array) : bool
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ suffix : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  └─ return
│  │     └─ bool True
│  ├─ = : i32
│  │  ├─ n : i32
│  │  └─ cast : i32
│  │     └─ call strlen : i64
│  │        └─ suffix : char[]
│  ├─ if
│  │  ├─ condition > : bool
│  │  │  ├─ n : i32
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  └─ return
│  │     └─ bool False
│  └─ return
│     └─ == : bool
│        ├─ call strncmp : i32
│        │  ├─ index : char[]
│        │  │  ├─ .value : char[]
│        │  │  │  └─ self : STRUCT_CALL
│        │  │  └─ range : i32
│        │  │     ├─ - : i32
│        │  │     │  ├─ .count : i32
│        │  │     │  │  └─ self : STRUCT_CALL
│        │  │     │  └─ n : i32
│        │  │     └─ .count : i32
│        │  │        └─ self : STRUCT_CALL
│        │  ├─ suffix : char[]
│        │  └─ n : i32
│        └─ int 0
├─ fn String.substr(self : STRUCT_CALL, start : i32, length : i32) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ res : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  ├─ if
│  │  ├─ condition or : bool
│  │  │  ├─ < : bool
│  │  │  │  ├─ start : i32
│  │  │  │  └─ int 0
│  │  │  └─ >= : bool
│  │  │     ├─ start : i32
│  │  │     └─ .count : i32
│  │  │        └─ self : STRUCT_CALL
│  │  └─ return
│  │     └─ res : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ stop : i32
│  │  └─ + : i32
│  │     ├─ start : i32
│  │     └─ length : i32
│  ├─ if
│  │  ├─ condition > : bool
│  │  │  ├─ stop : i32
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  └─ = : i32
│  │     ├─ stop : i32
│  │     └─ .count : i32
│  │        └─ self : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ start : i32
│  ├─ while
│  │  ├─ condition < : bool
│  │  │  ├─ i : i32
│  │  │  └─ stop : i32
│  │  ├─ call push : void
│  │  │  ├─ res : STRUCT_CALL
│  │  │  └─ index : char
│  │  │     ├─ .value : char[]
│  │  │     │  └─ self : STRUCT_CALL
│  │  │     └─ i : i32
│  │  └─ += : i32
│  │     ├─ i : i32
│  │     └─ int 1
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn String.upper(self : STRUCT_CALL) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ res : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ int 0
│  ├─ while
│  │  ├─ condition < : bool
│  │  │  ├─ i : i32
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  ├─ = : char
│  │  │  ├─ c : char
│  │  │  └─ index : char
│  │  │     ├─ .value : char[]
│  │  │     │  └─ self : STRUCT_CALL
│  │  │     └─ i : i32
│  │  ├─ if
│  │  │  ├─ condition and : bool
│  │  │  │  ├─ >= : bool
│  │  │  │  │  ├─ c : char
│  │  │  │  │  └─ char 'a'
│  │  │  │  └─ <= : bool
│  │  │  │     ├─ c : char
│  │  │  │     └─ char 'z'
│  │  │  ├─ call push : void
│  │  │  │  ├─ res : STRUCT_CALL
│  │  │  │  └─ cast : char
│  │  │  │     └─ - : i32
│  │  │  │        ├─ cast : i32
│  │  │  │        │  └─ c : char
│  │  │  │        └─ int 32
│  │  │  └─ else
│  │  │     └─ call push : void
│  │  │        ├─ res : STRUCT_CALL
│  │  │        └─ c : char
│  │  └─ += : i32
│  │     ├─ i : i32
│  │     └─ int 1
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn String.lower(self : STRUCT_CALL) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ res : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ int 0
│  ├─ while
│  │  ├─ condition < : bool
│  │  │  ├─ i : i32
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  ├─ = : char
│  │  │  ├─ c : char
│  │  │  └─ index : char
│  │  │     ├─ .value : char[]
│  │  │     │  └─ self : STRUCT_CALL
│  │  │     └─ i : i32
│  │  ├─ if
│  │  │  ├─ condition and : bool
│  │  │  │  ├─ >= : bool
│  │  │  │  │  ├─ c : char
│  │  │  │  │  └─ char 'A'
│  │  │  │  └─ <= : bool
│  │  │  │     ├─ c : char
│  │  │  │     └─ char 'Z'
│  │  │  ├─ call push : void
│  │  │  │  ├─ res : STRUCT_CALL
│  │  │  │  └─ cast : char
│  │  │  │     └─ + : i32
│  │  │  │        ├─ cast : i32
│  │  │  │        │  └─ c : char
│  │  │  │        └─ int 32
│  │  │  └─ else
│  │  │     └─ call push : void
│  │  │        ├─ res : STRUCT_CALL
│  │  │        └─ c : char
│  │  └─ += : i32
│  │     ├─ i : i32
│  │     └─ int 1
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn String.trim(self : STRUCT_CALL) : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ start : i32
│  │  └─ int 0
│  ├─ while
│  │  ├─ condition < : bool
│  │  │  ├─ start : i32
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  ├─ = : char
│  │  │  ├─ c : char
│  │  │  └─ index : char
│  │  │     ├─ .value : char[]
│  │  │     │  └─ self : STRUCT_CALL
│  │  │     └─ start : i32
│  │  └─ if
│  │     ├─ condition or : bool
│  │     │  ├─ or : bool
│  │     │  │  ├─ or : bool
│  │     │  │  │  ├─ == : bool
│  │     │  │  │  │  ├─ c : char
│  │     │  │  │  │  └─ char ' '
│  │     │  │  │  └─ == : bool
│  │     │  │  │     ├─ c : char
│  │     │  │  │     └─ char '\t'
│  │     │  │  └─ == : bool
│  │     │  │     ├─ c : char
│  │     │  │     └─ char '\n'
│  │     │  └─ == : bool
│  │     │     ├─ c : char
│  │     │     └─ char '\r'
│  │     ├─ += : i32
│  │     │  ├─ start : i32
│  │     │  └─ int 1
│  │     └─ else
│  │        └─ break
│  ├─ = : i32
│  │  ├─ stop : i32
│  │  └─ .count : i32
│  │     └─ self : STRUCT_CALL
│  ├─ while
│  │  ├─ condition > : bool
│  │  │  ├─ stop : i32
│  │  │  └─ start : i32
│  │  ├─ = : char
│  │  │  ├─ c : char
│  │  │  └─ index : char
│  │  │     ├─ .value : char[]
│  │  │     │  └─ self : STRUCT_CALL
│  │  │     └─ - : i32
│  │  │        ├─ stop : i32
│  │  │        └─ int 1
│  │  └─ if
│  │     ├─ condition or : bool
│  │     │  ├─ or : bool
│  │     │  │  ├─ or : bool
│  │     │  │  │  ├─ == : bool
│  │     │  │  │  │  ├─ c : char
│  │     │  │  │  │  └─ char ' '
│  │     │  │  │  └─ == : bool
│  │     │  │  │     ├─ c : char
│  │     │  │  │     └─ char '\t'
│  │     │  │  └─ == : bool
│  │     │  │     ├─ c : char
│  │     │  │     └─ char '\n'
│  │     │  └─ == : bool
│  │     │     ├─ c : char
│  │     │     └─ char '\r'
│  │     ├─ -= : i32
│  │     │  ├─ stop : i32
│  │     │  └─ int 1
│  │     └─ else
│  │        └─ break
│  └─ return
│     └─ call substr : STRUCT_CALL
│        ├─ self : STRUCT_CALL
│        ├─ start : i32
│        └─ - : i32
│           ├─ stop : i32
│           └─ start : i32
├─ fn String.replace(self : STRUCT_CALL, old : array, fresh : array) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ res : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ old : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  ├─ call assign : void
│  │  │  ├─ res : STRUCT_CALL
│  │  │  └─ .value : char[]
│  │  │     └─ self : STRUCT_CALL
│  │  └─ return
│  │     └─ res : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ n : i32
│  │  └─ cast : i32
│  │     └─ call strlen : i64
│  │        └─ old : char[]
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ n : i32
│  │  │  └─ int 0
│  │  ├─ call assign : void
│  │  │  ├─ res : STRUCT_CALL
│  │  │  └─ .value : char[]
│  │  │     └─ self : STRUCT_CALL
│  │  └─ return
│  │     └─ res : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ int 0
│  ├─ while
│  │  ├─ condition < : bool
│  │  │  ├─ i : i32
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  ├─ = : bool
│  │  │  ├─ hit : bool
│  │  │  └─ bool False
│  │  ├─ if
│  │  │  ├─ condition <= : bool
│  │  │  │  ├─ + : i32
│  │  │  │  │  ├─ i : i32
│  │  │  │  │  └─ n : i32
│  │  │  │  └─ .count : i32
│  │  │  │     └─ self : STRUCT_CALL
│  │  │  └─ = : bool
│  │  │     ├─ hit : bool
│  │  │     └─ == : bool
│  │  │        ├─ call strncmp : i32
│  │  │        │  ├─ index : char[]
│  │  │        │  │  ├─ .value : char[]
│  │  │        │  │  │  └─ self : STRUCT_CALL
│  │  │        │  │  └─ range : i32
│  │  │        │  │     ├─ i : i32
│  │  │        │  │     └─ + : i32
│  │  │        │  │        ├─ i : i32
│  │  │        │  │        └─ n : i32
│  │  │        │  ├─ old : char[]
│  │  │        │  └─ n : i32
│  │  │        └─ int 0
│  │  └─ if
│  │     ├─ condition hit : bool
│  │     ├─ call join : void
│  │     │  ├─ res : STRUCT_CALL
│  │     │  └─ fresh : char[]
│  │     ├─ += : i32
│  │     │  ├─ i : i32
│  │     │  └─ n : i32
│  │     └─ else
│  │        ├─ call push : void
│  │        │  ├─ res : STRUCT_CALL
│  │        │  └─ index : char
│  │        │     ├─ .value : char[]
│  │        │     │  └─ self : STRUCT_CALL
│  │        │     └─ i : i32
│  │        └─ += : i32
│  │           ├─ i : i32
│  │           └─ int 1
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn String.repeat(self : STRUCT_CALL, n : i32) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ res : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ int 0
│  ├─ while
│  │  ├─ condition < : bool
│  │  │  ├─ i : i32
│  │  │  └─ n : i32
│  │  ├─ call join : void
│  │  │  ├─ res : STRUCT_CALL
│  │  │  └─ .value : char[]
│  │  │     └─ self : STRUCT_CALL
│  │  └─ += : i32
│  │     ├─ i : i32
│  │     └─ int 1
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn String.reverse(self : STRUCT_CALL) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ res : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ - : i32
│  │     ├─ .count : i32
│  │     │  └─ self : STRUCT_CALL
│  │     └─ int 1
│  ├─ while
│  │  ├─ condition >= : bool
│  │  │  ├─ i : i32
│  │  │  └─ int 0
│  │  ├─ call push : void
│  │  │  ├─ res : STRUCT_CALL
│  │  │  └─ index : char
│  │  │     ├─ .value : char[]
│  │  │     │  └─ self : STRUCT_CALL
│  │  │     └─ i : i32
│  │  └─ -= : i32
│  │     ├─ i : i32
│  │     └─ int 1
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn String.compare(self : STRUCT_CALL, other : STRUCT_CALL) : i32
│  └─ return
│     └─ call strcmp : i32
│        ├─ FALLBACK : char[]
│        │  ├─ .value : char[]
│        │  │  └─ self : STRUCT_CALL
│        │  └─ char[] ""
│        └─ FALLBACK : char[]
│           ├─ .value : char[]
│           │  └─ other : STRUCT_CALL
│           └─ char[] ""
├─ fn String.to_int(self : STRUCT_CALL) : i32
│  └─ return
│     └─ call atoi : i32
│        └─ FALLBACK : char[]
│           ├─ .value : char[]
│           │  └─ self : STRUCT_CALL
│           └─ char[] ""
├─ fn String.=.array(self : STRUCT_CALL, str : array) : void
│  └─ call assign : void
│     ├─ self : STRUCT_CALL
│     └─ str : char[]
├─ fn String.=.ref.String(self : STRUCT_CALL, v : STRUCT_CALL) : void
│  └─ call assign : void
│     ├─ self : STRUCT_CALL
│     └─ .value : char[]
│        └─ v : STRUCT_CALL
├─ fn String.+.ref.String(self : STRUCT_CALL, v : STRUCT_CALL) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ res : STRUCT_CALL
│  │  └─ call from : STRUCT_CALL
│  │     └─ .value : char[]
│  │        └─ self : STRUCT_CALL
│  ├─ call join : void
│  │  ├─ res : STRUCT_CALL
│  │  └─ .value : char[]
│  │     └─ v : STRUCT_CALL
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn String.+.array(self : STRUCT_CALL, str : array) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ res : STRUCT_CALL
│  │  └─ call from : STRUCT_CALL
│  │     └─ .value : char[]
│  │        └─ self : STRUCT_CALL
│  ├─ call join : void
│  │  ├─ res : STRUCT_CALL
│  │  └─ str : char[]
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn String.+=.ref.String(self : STRUCT_CALL, v : STRUCT_CALL) : void
│  └─ call join : void
│     ├─ self : STRUCT_CALL
│     └─ .value : char[]
│        └─ v : STRUCT_CALL
├─ fn String.+=.array(self : STRUCT_CALL, str : array) : void
│  └─ call join : void
│     ├─ self : STRUCT_CALL
│     └─ str : char[]
├─ fn String.==.ref.String(self : STRUCT_CALL, v : STRUCT_CALL) : bool
│  └─ return
│     └─ == : bool
│        ├─ call compare : i32
│        │  ├─ self : STRUCT_CALL
│        │  └─ ref : STRUCT_CALL
│        │     └─ v : STRUCT_CALL
│        └─ int 0
├─ fn String.!=.ref.String(self : STRUCT_CALL, v : STRUCT_CALL) : bool
│  └─ return
│     └─ != : bool
│        ├─ call compare : i32
│        │  ├─ self : STRUCT_CALL
│        │  └─ ref : STRUCT_CALL
│        │     └─ v : STRUCT_CALL
│        └─ int 0
├─ fn String.==.array(self : STRUCT_CALL, str : array) : bool
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ str : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  └─ return
│  │     └─ == : bool
│  │        ├─ .count : i32
│  │        │  └─ self : STRUCT_CALL
│  │        └─ int 0
│  └─ return
│     └─ == : bool
│        ├─ call strcmp : i32
│        │  ├─ FALLBACK : char[]
│        │  │  ├─ .value : char[]
│        │  │  │  └─ self : STRUCT_CALL
│        │  │  └─ char[] ""
│        │  └─ str : char[]
│        └─ int 0
├─ fn String.!=.array(self : STRUCT_CALL, str : array) : bool
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ str : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  └─ return
│  │     └─ != : bool
│  │        ├─ .count : i32
│  │        │  └─ self : STRUCT_CALL
│  │        └─ int 0
│  └─ return
│     └─ != : bool
│        ├─ call strcmp : i32
│        │  ├─ FALLBACK : char[]
│        │  │  ├─ .value : char[]
│        │  │  │  └─ self : STRUCT_CALL
│        │  │  └─ char[] ""
│        │  └─ str : char[]
│        └─ int 0
├─ fn String.<.ref.String(self : STRUCT_CALL, v : STRUCT_CALL) : bool
│  └─ return
│     └─ < : bool
│        ├─ call compare : i32
│        │  ├─ self : STRUCT_CALL
│        │  └─ ref : STRUCT_CALL
│        │     └─ v : STRUCT_CALL
│        └─ int 0
├─ fn String.>.ref.String(self : STRUCT_CALL, v : STRUCT_CALL) : bool
│  └─ return
│     └─ > : bool
│        ├─ call compare : i32
│        │  ├─ self : STRUCT_CALL
│        │  └─ ref : STRUCT_CALL
│        │     └─ v : STRUCT_CALL
│        └─ int 0
├─ fn String.<.array(self : STRUCT_CALL, str : array) : bool
│  └─ return
│     └─ < : bool
│        ├─ call strcmp : i32
│        │  ├─ FALLBACK : char[]
│        │  │  ├─ .value : char[]
│        │  │  │  └─ self : STRUCT_CALL
│        │  │  └─ char[] ""
│        │  └─ FALLBACK : char[]
│        │     ├─ str : char[]
│        │     └─ char[] ""
│        └─ int 0
├─ fn String.>.array(self : STRUCT_CALL, str : array) : bool
│  └─ return
│     └─ > : bool
│        ├─ call strcmp : i32
│        │  ├─ FALLBACK : char[]
│        │  │  ├─ .value : char[]
│        │  │  │  └─ self : STRUCT_CALL
│        │  │  └─ char[] ""
│        │  └─ FALLBACK : char[]
│        │     ├─ str : char[]
│        │     └─ char[] ""
│        └─ int 0
└─ fn String.output(self : STRUCT_CALL) : char[]
   └─ return
      └─ FALLBACK : char[]
         ├─ .value : char[]
         │  └─ self : STRUCT_CALL
         └─ char[] ""

struct Buffer
├─ value : char[]
├─ fn Buffer.create() : STRUCT_CALL
│  ├─ b : STRUCT_CALL
│  ├─ = : array
│  │  ├─ .value : char[]
│  │  │  └─ b : STRUCT_CALL
│  │  └─ array : char[]
│  │     └─ int 64
│  └─ return
│     └─ b : STRUCT_CALL
├─ fn Buffer.drop(self : STRUCT_CALL) : void
│  └─ clean : void
│     └─ .value : char[]
│        └─ self : STRUCT_CALL
├─ fn Buffer.=.array(self : STRUCT_CALL, s : array) : void
│  └─ call strcpy : pointer
│     ├─ .value : char[]
│     │  └─ self : STRUCT_CALL
│     └─ s : char[]
└─ fn Buffer.=.ref.Buffer(self : STRUCT_CALL, b : STRUCT_CALL) : void
   └─ call strcpy : pointer
      ├─ .value : char[]
      │  └─ self : STRUCT_CALL
      └─ .value : char[]
         └─ b : STRUCT_CALL

fn main() : i32
├─ = : STRUCT_CALL
│  ├─ b1 : STRUCT_CALL
│  └─ call create : STRUCT_CALL
├─ = : void
│  ├─ b1 : STRUCT_CALL
│  └─ char[] "hello"
├─ = : STRUCT_CALL
│  ├─ b2 : STRUCT_CALL
│  └─ call create : STRUCT_CALL
├─ = : void
│  ├─ b2 : STRUCT_CALL
│  └─ b1 : STRUCT_CALL
├─ call strcpy : pointer
│  ├─ .value : char[]
│  │  └─ b2 : STRUCT_CALL
│  └─ char[] "world"
├─ output : void
│  ├─ char[] "b1: "
│  ├─ .value : char[]
│  │  └─ b1 : STRUCT_CALL
│  └─ char[] "\n"
└─ output : void
   ├─ char[] "b2: "
   ├─ .value : char[]
   │  └─ b2 : STRUCT_CALL
   └─ char[] "\n"
```

```out
b1: hello
b2: world
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%String = type { { i8*, i64 }, i32 }
%Buffer = type { { i8*, i64 } }

@os = internal global %Os zeroinitializer
@trap_msg = private unnamed_addr constant [153 x i8] c"runtime error: Modulo by zero\0A   ura-lib/string.ura:97:30\0A   |\0A97 |             digits[i] = (num % 10 + 48) as char\0A   |                              ^\0A\00", align 1
@trap_msg.1 = private unnamed_addr constant [127 x i8] c"runtime error: Division by zero\0A   ura-lib/string.ura:98:23\0A   |\0A98 |             num = num / 10\0A   |                       ^\0A\00", align 1
@str = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.2 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.3 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.4 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.5 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.6 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.7 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.8 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.9 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.10 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.11 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.12 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.13 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.14 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.15 = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@str.16 = private unnamed_addr constant [6 x i8] c"world\00", align 1
@str.17 = private unnamed_addr constant [5 x i8] c"b1: \00", align 1
@str.18 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [13 x i8] c"%.*s%.*s%.*s\00", align 1
@str.19 = private unnamed_addr constant [5 x i8] c"b2: \00", align 1
@str.20 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.21 = private unnamed_addr constant [13 x i8] c"%.*s%.*s%.*s\00", align 1

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

define %String @String.create() {
entry:
  %s = alloca %String, align 8
  store %String zeroinitializer, %String* %s, align 8
  %value = getelementptr %String, %String* %s, i32 0, i32 0
  %heap = call i8* @calloc(i64 16, i64 1)
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %heap, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 16, 1
  store { i8*, i64 } %arr.len, { i8*, i64 }* %value, align 8
  %count = getelementptr %String, %String* %s, i32 0, i32 1
  store i32 0, i32* %count, align 4
  %s1 = load %String, %String* %s, align 8
  ret %String %s1
}

declare i8* @calloc(i64, i64)

define %String @String.from({ i8*, i64 } %0) {
entry:
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %0, { i8*, i64 }* %str, align 8
  %s = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %s, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  call void @String.assign(%String* %s, { i8*, i64 } %str1)
  %s2 = load %String, %String* %s, align 8
  ret %String %s2
}

define void @String.assign(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %n = alloca i32, align 4
  store i32 0, i32* %n, align 4
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr = extractvalue { i8*, i64 } %str1, 0
  %nullcmp = icmp ne i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %then, %entry
  %ref = load %String*, %String** %self, align 8
  %n3 = load i32, i32* %n, align 4
  %add = add i32 %n3, 1
  call void @String.grow(%String* %ref, i32 %add)
  %n6 = load i32, i32* %n, align 4
  %gt = icmp sgt i32 %n6, 0
  br i1 %gt, label %then5, label %endif4

then:                                             ; preds = %entry
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data = extractvalue { i8*, i64 } %str2, 0
  %call = call i64 @strlen(i8* %arr.data)
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  br label %endif

endif4:                                           ; preds = %then5, %endif
  %ref18 = load %String*, %String** %self, align 8
  %value19 = getelementptr %String, %String* %ref18, i32 0, i32 0
  %value20 = load { i8*, i64 }, { i8*, i64 }* %value19, align 8
  %arr.data21 = extractvalue { i8*, i64 } %value20, 0
  %n22 = load i32, i32* %n, align 4
  %arr.at = getelementptr i8, i8* %arr.data21, i32 %n22
  store i8 0, i8* %arr.at, align 1
  %ref23 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref23, i32 0, i32 1
  %n24 = load i32, i32* %n, align 4
  store i32 %n24, i32* %count, align 4
  ret void

then5:                                            ; preds = %endif
  %ref7 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref7, i32 0, i32 0
  %value8 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr9 = extractvalue { i8*, i64 } %value8, 0
  %isnull = icmp eq i8* %opt.ptr9, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.2, i32 0, i32 0), i64 0 }, { i8*, i64 } %value8
  %arr.data10 = extractvalue { i8*, i64 } %fallback, 0
  %str11 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr12 = extractvalue { i8*, i64 } %str11, 0
  %isnull13 = icmp eq i8* %opt.ptr12, null
  %fallback14 = select i1 %isnull13, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.3, i32 0, i32 0), i64 0 }, { i8*, i64 } %str11
  %arr.data15 = extractvalue { i8*, i64 } %fallback14, 0
  %n16 = load i32, i32* %n, align 4
  %call17 = call i8* @memcpy(i8* %arr.data10, i8* %arr.data15, i32 %n16)
  br label %endif4
}

define %String @String.from_int(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %s = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %s, align 8
  %n1 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n1, 0
  br i1 %eq, label %then, label %endif

endif:                                            ; preds = %entry
  %num = alloca i32, align 4
  %n3 = load i32, i32* %n, align 4
  store i32 %n3, i32* %num, align 4
  %num6 = load i32, i32* %num, align 4
  %lt = icmp slt i32 %num6, 0
  br i1 %lt, label %then5, label %endif4

then:                                             ; preds = %entry
  call void @String.push(%String* %s, i8 48)
  %s2 = load %String, %String* %s, align 8
  ret %String %s2

endif4:                                           ; preds = %then5, %endif
  %digits = alloca { i8*, i64 }, align 8
  %heap = call i8* @calloc(i64 12, i64 1)
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %heap, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 12, 1
  store { i8*, i64 } %arr.len, { i8*, i64 }* %digits, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then5:                                            ; preds = %endif
  call void @String.push(%String* %s, i8 45)
  %num7 = load i32, i32* %num, align 4
  %sub = sub i32 0, %num7
  store i32 %sub, i32* %num, align 4
  br label %endif4

while.cond:                                       ; preds = %cont14, %endif4
  %num8 = load i32, i32* %num, align 4
  %gt = icmp sgt i32 %num8, 0
  br i1 %gt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %digits9 = load { i8*, i64 }, { i8*, i64 }* %digits, align 8
  %arr.data = extractvalue { i8*, i64 } %digits9, 0
  %i10 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %i10
  %num11 = load i32, i32* %num, align 4
  br i1 false, label %trap, label %cont

while.end:                                        ; preds = %while.cond
  br label %while.cond16

trap:                                             ; preds = %while.body
  %1 = call i64 @write(i32 2, i8* getelementptr inbounds ([153 x i8], [153 x i8]* @trap_msg, i32 0, i32 0), i64 152)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %while.body
  %mod = srem i32 %num11, 10
  %add = add i32 %mod, 48
  %cast = trunc i32 %add to i8
  store i8 %cast, i8* %arr.at, align 1
  %num12 = load i32, i32* %num, align 4
  br i1 false, label %trap13, label %cont14

trap13:                                           ; preds = %cont
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([127 x i8], [127 x i8]* @trap_msg.1, i32 0, i32 0), i64 126)
  call void @exit(i32 1)
  unreachable

cont14:                                           ; preds = %cont
  %div = sdiv i32 %num12, 10
  store i32 %div, i32* %num, align 4
  %cur = load i32, i32* %i, align 4
  %add15 = add i32 %cur, 1
  store i32 %add15, i32* %i, align 4
  br label %while.cond

while.cond16:                                     ; preds = %while.body17, %while.end
  %i19 = load i32, i32* %i, align 4
  %gt20 = icmp sgt i32 %i19, 0
  br i1 %gt20, label %while.body17, label %while.end18

while.body17:                                     ; preds = %while.cond16
  %cur21 = load i32, i32* %i, align 4
  %sub22 = sub i32 %cur21, 1
  store i32 %sub22, i32* %i, align 4
  %digits23 = load { i8*, i64 }, { i8*, i64 }* %digits, align 8
  %arr.data24 = extractvalue { i8*, i64 } %digits23, 0
  %i25 = load i32, i32* %i, align 4
  %arr.at26 = getelementptr i8, i8* %arr.data24, i32 %i25
  %idx = load i8, i8* %arr.at26, align 1
  call void @String.push(%String* %s, i8 %idx)
  br label %while.cond16

while.end18:                                      ; preds = %while.cond16
  %arr = load { i8*, i64 }, { i8*, i64 }* %digits, align 8
  %arr.data27 = extractvalue { i8*, i64 } %arr, 0
  call void @free(i8* %arr.data27)
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %digits, align 8
  %s28 = load %String, %String* %s, align 8
  ret %String %s28
}

define void @String.push(%String* %0, i8 %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %c = alloca i8, align 1
  store i8 %1, i8* %c, align 1
  %ref = load %String*, %String** %self, align 8
  %ref1 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref1, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %add = add i32 %count2, 2
  call void @String.grow(%String* %ref, i32 %add)
  %ref3 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref3, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value4, 0
  %ref5 = load %String*, %String** %self, align 8
  %count6 = getelementptr %String, %String* %ref5, i32 0, i32 1
  %count7 = load i32, i32* %count6, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %count7
  %c8 = load i8, i8* %c, align 1
  store i8 %c8, i8* %arr.at, align 1
  %ref9 = load %String*, %String** %self, align 8
  %count10 = getelementptr %String, %String* %ref9, i32 0, i32 1
  %cur = load i32, i32* %count10, align 4
  %add11 = add i32 %cur, 1
  store i32 %add11, i32* %count10, align 4
  %ref12 = load %String*, %String** %self, align 8
  %value13 = getelementptr %String, %String* %ref12, i32 0, i32 0
  %value14 = load { i8*, i64 }, { i8*, i64 }* %value13, align 8
  %arr.data15 = extractvalue { i8*, i64 } %value14, 0
  %ref16 = load %String*, %String** %self, align 8
  %count17 = getelementptr %String, %String* %ref16, i32 0, i32 1
  %count18 = load i32, i32* %count17, align 4
  %arr.at19 = getelementptr i8, i8* %arr.data15, i32 %count18
  store i8 0, i8* %arr.at19, align 1
  ret void
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)

declare void @free(i8*)

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %arr = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %arr, 0
  call void @free(i8* %arr.data)
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %value, align 8
  ret void
}

define i32 @String.cap(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %ref2 = load %String*, %String** %self, align 8
  %value3 = getelementptr %String, %String* %ref2, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value3, align 8
  %len = extractvalue { i8*, i64 } %value4, 1
  %cast = trunc i64 %len to i32
  ret i32 %cast

then:                                             ; preds = %entry
  ret i32 0
}

define void @String.grow(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %want = alloca i32, align 4
  store i32 %1, i32* %want, align 4
  %room = alloca i32, align 4
  %ref = load %String*, %String** %self, align 8
  %call = call i32 @String.cap(%String* %ref)
  store i32 %call, i32* %room, align 4
  %room1 = load i32, i32* %room, align 4
  %want2 = load i32, i32* %want, align 4
  %ge = icmp sge i32 %room1, %want2
  br i1 %ge, label %then, label %endif

endif:                                            ; preds = %entry
  %room5 = load i32, i32* %room, align 4
  %eq = icmp eq i32 %room5, 0
  br i1 %eq, label %then4, label %endif3

then:                                             ; preds = %entry
  ret void

endif3:                                           ; preds = %then4, %endif
  br label %while.cond

then4:                                            ; preds = %endif
  store i32 16, i32* %room, align 4
  br label %endif3

while.cond:                                       ; preds = %while.body, %endif3
  %room6 = load i32, i32* %room, align 4
  %want7 = load i32, i32* %want, align 4
  %lt = icmp slt i32 %room6, %want7
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %cur = load i32, i32* %room, align 4
  %mul = mul i32 %cur, 2
  store i32 %mul, i32* %room, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %bigger = alloca { i8*, i64 }, align 8
  %room8 = load i32, i32* %room, align 4
  %n = sext i32 %room8 to i64
  %heap = call i8* @calloc(i64 %n, i64 1)
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %heap, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %n, 1
  store { i8*, i64 } %arr.len, { i8*, i64 }* %bigger, align 8
  %ref11 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref11, i32 0, i32 1
  %count12 = load i32, i32* %count, align 4
  %gt = icmp sgt i32 %count12, 0
  br i1 %gt, label %then10, label %endif9

endif9:                                           ; preds = %then10, %while.end
  %bigger21 = load { i8*, i64 }, { i8*, i64 }* %bigger, align 8
  %arr.data22 = extractvalue { i8*, i64 } %bigger21, 0
  %ref23 = load %String*, %String** %self, align 8
  %count24 = getelementptr %String, %String* %ref23, i32 0, i32 1
  %count25 = load i32, i32* %count24, align 4
  %arr.at = getelementptr i8, i8* %arr.data22, i32 %count25
  store i8 0, i8* %arr.at, align 1
  %ref26 = load %String*, %String** %self, align 8
  %value27 = getelementptr %String, %String* %ref26, i32 0, i32 0
  %arr = load { i8*, i64 }, { i8*, i64 }* %value27, align 8
  %arr.data28 = extractvalue { i8*, i64 } %arr, 0
  call void @free(i8* %arr.data28)
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %value27, align 8
  %ref29 = load %String*, %String** %self, align 8
  %value30 = getelementptr %String, %String* %ref29, i32 0, i32 0
  %bigger31 = load { i8*, i64 }, { i8*, i64 }* %bigger, align 8
  store { i8*, i64 } %bigger31, { i8*, i64 }* %value30, align 8
  ret void

then10:                                           ; preds = %while.end
  %bigger13 = load { i8*, i64 }, { i8*, i64 }* %bigger, align 8
  %arr.data = extractvalue { i8*, i64 } %bigger13, 0
  %ref14 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref14, i32 0, i32 0
  %value15 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value15, 0
  %isnull = icmp eq i8* %opt.ptr, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str, i32 0, i32 0), i64 0 }, { i8*, i64 } %value15
  %arr.data16 = extractvalue { i8*, i64 } %fallback, 0
  %ref17 = load %String*, %String** %self, align 8
  %count18 = getelementptr %String, %String* %ref17, i32 0, i32 1
  %count19 = load i32, i32* %count18, align 4
  %call20 = call i8* @memcpy(i8* %arr.data, i8* %arr.data16, i32 %count19)
  br label %endif9
}

declare i8* @memcpy(i8*, i8*, i32)

declare i64 @strlen(i8*)

define void @String.join(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr = extractvalue { i8*, i64 } %str1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %n = alloca i32, align 4
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data = extractvalue { i8*, i64 } %str2, 0
  %call = call i64 @strlen(i8* %arr.data)
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n5, 0
  br i1 %eq, label %then4, label %endif3

then:                                             ; preds = %entry
  ret void

endif3:                                           ; preds = %endif
  %ref = load %String*, %String** %self, align 8
  %ref6 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref6, i32 0, i32 1
  %count7 = load i32, i32* %count, align 4
  %n8 = load i32, i32* %n, align 4
  %add = add i32 %count7, %n8
  %add9 = add i32 %add, 1
  call void @String.grow(%String* %ref, i32 %add9)
  %ref10 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref10, i32 0, i32 0
  %value11 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data12 = extractvalue { i8*, i64 } %value11, 0
  %ref13 = load %String*, %String** %self, align 8
  %count14 = getelementptr %String, %String* %ref13, i32 0, i32 1
  %count15 = load i32, i32* %count14, align 4
  %ref16 = load %String*, %String** %self, align 8
  %count17 = getelementptr %String, %String* %ref16, i32 0, i32 1
  %count18 = load i32, i32* %count17, align 4
  %n19 = load i32, i32* %n, align 4
  %add20 = add i32 %count18, %n19
  %start = sext i32 %count15 to i64
  %end = sext i32 %add20 to i64
  %slice.data = getelementptr i8, i8* %arr.data12, i64 %start
  %slice.len = sub i64 %end, %start
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %slice.data, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %slice.len, 1
  %arr.data21 = extractvalue { i8*, i64 } %arr.len, 0
  %str22 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data23 = extractvalue { i8*, i64 } %str22, 0
  %n24 = load i32, i32* %n, align 4
  %call25 = call i8* @memcpy(i8* %arr.data21, i8* %arr.data23, i32 %n24)
  %ref26 = load %String*, %String** %self, align 8
  %count27 = getelementptr %String, %String* %ref26, i32 0, i32 1
  %n28 = load i32, i32* %n, align 4
  %cur = load i32, i32* %count27, align 4
  %add29 = add i32 %cur, %n28
  store i32 %add29, i32* %count27, align 4
  %ref30 = load %String*, %String** %self, align 8
  %value31 = getelementptr %String, %String* %ref30, i32 0, i32 0
  %value32 = load { i8*, i64 }, { i8*, i64 }* %value31, align 8
  %arr.data33 = extractvalue { i8*, i64 } %value32, 0
  %ref34 = load %String*, %String** %self, align 8
  %count35 = getelementptr %String, %String* %ref34, i32 0, i32 1
  %count36 = load i32, i32* %count35, align 4
  %arr.at = getelementptr i8, i8* %arr.data33, i32 %count36
  store i8 0, i8* %arr.at, align 1
  ret void

then4:                                            ; preds = %endif
  ret void
}

define i8 @String.pop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count1 = load i32, i32* %count, align 4
  %eq = icmp eq i32 %count1, 0
  br i1 %eq, label %then, label %endif

endif:                                            ; preds = %entry
  %ref2 = load %String*, %String** %self, align 8
  %count3 = getelementptr %String, %String* %ref2, i32 0, i32 1
  %cur = load i32, i32* %count3, align 4
  %sub = sub i32 %cur, 1
  store i32 %sub, i32* %count3, align 4
  %c = alloca i8, align 1
  %ref4 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref4, i32 0, i32 0
  %value5 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value5, 0
  %ref6 = load %String*, %String** %self, align 8
  %count7 = getelementptr %String, %String* %ref6, i32 0, i32 1
  %count8 = load i32, i32* %count7, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %count8
  %idx = load i8, i8* %arr.at, align 1
  store i8 %idx, i8* %c, align 1
  %ref9 = load %String*, %String** %self, align 8
  %value10 = getelementptr %String, %String* %ref9, i32 0, i32 0
  %value11 = load { i8*, i64 }, { i8*, i64 }* %value10, align 8
  %arr.data12 = extractvalue { i8*, i64 } %value11, 0
  %ref13 = load %String*, %String** %self, align 8
  %count14 = getelementptr %String, %String* %ref13, i32 0, i32 1
  %count15 = load i32, i32* %count14, align 4
  %arr.at16 = getelementptr i8, i8* %arr.data12, i32 %count15
  store i8 0, i8* %arr.at16, align 1
  %c17 = load i8, i8* %c, align 1
  ret i8 %c17

then:                                             ; preds = %entry
  ret i8 0
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  store i32 0, i32* %count, align 4
  %ref1 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref1, i32 0, i32 0
  %value2 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value2, 0
  %nullcmp = icmp ne i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %then, %entry
  ret void

then:                                             ; preds = %entry
  %ref3 = load %String*, %String** %self, align 8
  %value4 = getelementptr %String, %String* %ref3, i32 0, i32 0
  %value5 = load { i8*, i64 }, { i8*, i64 }* %value4, align 8
  %arr.data = extractvalue { i8*, i64 } %value5, 0
  %arr.at = getelementptr i8, i8* %arr.data, i32 0
  store i8 0, i8* %arr.at, align 1
  br label %endif
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count1 = load i32, i32* %count, align 4
  ret i32 %count1
}

define i1 @String.empty(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count1 = load i32, i32* %count, align 4
  %eq = icmp eq i32 %count1, 0
  ret i1 %eq
}

define { i8*, i64 } @String.c_str(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value1, 0
  %isnull = icmp eq i8* %opt.ptr, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.4, i32 0, i32 0), i64 0 }, { i8*, i64 } %value1
  ret { i8*, i64 } %fallback
}

define i8 @String.at(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %i = alloca i32, align 4
  store i32 %1, i32* %i, align 4
  %i1 = load i32, i32* %i, align 4
  %lt = icmp slt i32 %i1, 0
  %i2 = load i32, i32* %i, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count3 = load i32, i32* %count, align 4
  %ge = icmp sge i32 %i2, %count3
  %or = or i1 %lt, %ge
  br i1 %or, label %then, label %endif

endif:                                            ; preds = %entry
  %ref4 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref4, i32 0, i32 0
  %value5 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value5, 0
  %i6 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %i6
  %idx = load i8, i8* %arr.at, align 1
  ret i8 %idx

then:                                             ; preds = %entry
  ret i8 0
}

define i32 @String.find(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %needle = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %needle, align 8
  %needle1 = load { i8*, i64 }, { i8*, i64 }* %needle, align 8
  %opt.ptr = extractvalue { i8*, i64 } %needle1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %n = alloca i32, align 4
  %needle2 = load { i8*, i64 }, { i8*, i64 }* %needle, align 8
  %arr.data = extractvalue { i8*, i64 } %needle2, 0
  %call = call i64 @strlen(i8* %arr.data)
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n5, 0
  br i1 %eq, label %then4, label %endif3

then:                                             ; preds = %entry
  ret i32 -1

endif3:                                           ; preds = %endif
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then4:                                            ; preds = %endif
  ret i32 0

while.cond:                                       ; preds = %endif9, %endif3
  %i6 = load i32, i32* %i, align 4
  %n7 = load i32, i32* %n, align 4
  %add = add i32 %i6, %n7
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count8 = load i32, i32* %count, align 4
  %le = icmp sle i32 %add, %count8
  br i1 %le, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref11 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref11, i32 0, i32 0
  %value12 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data13 = extractvalue { i8*, i64 } %value12, 0
  %i14 = load i32, i32* %i, align 4
  %i15 = load i32, i32* %i, align 4
  %n16 = load i32, i32* %n, align 4
  %add17 = add i32 %i15, %n16
  %start = sext i32 %i14 to i64
  %end = sext i32 %add17 to i64
  %slice.data = getelementptr i8, i8* %arr.data13, i64 %start
  %slice.len = sub i64 %end, %start
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %slice.data, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %slice.len, 1
  %arr.data18 = extractvalue { i8*, i64 } %arr.len, 0
  %needle19 = load { i8*, i64 }, { i8*, i64 }* %needle, align 8
  %arr.data20 = extractvalue { i8*, i64 } %needle19, 0
  %n21 = load i32, i32* %n, align 4
  %call22 = call i32 @strncmp(i8* %arr.data18, i8* %arr.data20, i32 %n21)
  %eq23 = icmp eq i32 %call22, 0
  br i1 %eq23, label %then10, label %endif9

while.end:                                        ; preds = %while.cond
  ret i32 -1

endif9:                                           ; preds = %while.body
  %cur = load i32, i32* %i, align 4
  %add25 = add i32 %cur, 1
  store i32 %add25, i32* %i, align 4
  br label %while.cond

then10:                                           ; preds = %while.body
  %i24 = load i32, i32* %i, align 4
  ret i32 %i24
}

declare i32 @strncmp(i8*, i8*, i32)

define i1 @String.contains(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %needle = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %needle, align 8
  %ref = load %String*, %String** %self, align 8
  %needle1 = load { i8*, i64 }, { i8*, i64 }* %needle, align 8
  %call = call i32 @String.find(%String* %ref, { i8*, i64 } %needle1)
  %ne = icmp ne i32 %call, -1
  ret i1 %ne
}

define i1 @String.starts_with(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %prefix = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %prefix, align 8
  %prefix1 = load { i8*, i64 }, { i8*, i64 }* %prefix, align 8
  %opt.ptr = extractvalue { i8*, i64 } %prefix1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %n = alloca i32, align 4
  %prefix2 = load { i8*, i64 }, { i8*, i64 }* %prefix, align 8
  %arr.data = extractvalue { i8*, i64 } %prefix2, 0
  %call = call i64 @strlen(i8* %arr.data)
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count6 = load i32, i32* %count, align 4
  %gt = icmp sgt i32 %n5, %count6
  br i1 %gt, label %then4, label %endif3

then:                                             ; preds = %entry
  ret i1 true

endif3:                                           ; preds = %endif
  %ref7 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref7, i32 0, i32 0
  %value8 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data9 = extractvalue { i8*, i64 } %value8, 0
  %n10 = load i32, i32* %n, align 4
  %end = sext i32 %n10 to i64
  %slice.data = getelementptr i8, i8* %arr.data9, i64 0
  %slice.len = sub i64 %end, 0
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %slice.data, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %slice.len, 1
  %arr.data11 = extractvalue { i8*, i64 } %arr.len, 0
  %prefix12 = load { i8*, i64 }, { i8*, i64 }* %prefix, align 8
  %arr.data13 = extractvalue { i8*, i64 } %prefix12, 0
  %n14 = load i32, i32* %n, align 4
  %call15 = call i32 @strncmp(i8* %arr.data11, i8* %arr.data13, i32 %n14)
  %eq = icmp eq i32 %call15, 0
  ret i1 %eq

then4:                                            ; preds = %endif
  ret i1 false
}

define i1 @String.ends_with(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %suffix = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %suffix, align 8
  %suffix1 = load { i8*, i64 }, { i8*, i64 }* %suffix, align 8
  %opt.ptr = extractvalue { i8*, i64 } %suffix1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %n = alloca i32, align 4
  %suffix2 = load { i8*, i64 }, { i8*, i64 }* %suffix, align 8
  %arr.data = extractvalue { i8*, i64 } %suffix2, 0
  %call = call i64 @strlen(i8* %arr.data)
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count6 = load i32, i32* %count, align 4
  %gt = icmp sgt i32 %n5, %count6
  br i1 %gt, label %then4, label %endif3

then:                                             ; preds = %entry
  ret i1 true

endif3:                                           ; preds = %endif
  %ref7 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref7, i32 0, i32 0
  %value8 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data9 = extractvalue { i8*, i64 } %value8, 0
  %ref10 = load %String*, %String** %self, align 8
  %count11 = getelementptr %String, %String* %ref10, i32 0, i32 1
  %count12 = load i32, i32* %count11, align 4
  %n13 = load i32, i32* %n, align 4
  %sub = sub i32 %count12, %n13
  %ref14 = load %String*, %String** %self, align 8
  %count15 = getelementptr %String, %String* %ref14, i32 0, i32 1
  %count16 = load i32, i32* %count15, align 4
  %start = sext i32 %sub to i64
  %end = sext i32 %count16 to i64
  %slice.data = getelementptr i8, i8* %arr.data9, i64 %start
  %slice.len = sub i64 %end, %start
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %slice.data, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %slice.len, 1
  %arr.data17 = extractvalue { i8*, i64 } %arr.len, 0
  %suffix18 = load { i8*, i64 }, { i8*, i64 }* %suffix, align 8
  %arr.data19 = extractvalue { i8*, i64 } %suffix18, 0
  %n20 = load i32, i32* %n, align 4
  %call21 = call i32 @strncmp(i8* %arr.data17, i8* %arr.data19, i32 %n20)
  %eq = icmp eq i32 %call21, 0
  ret i1 %eq

then4:                                            ; preds = %endif
  ret i1 false
}

define %String @String.substr(%String* %0, i32 %1, i32 %2) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %start = alloca i32, align 4
  store i32 %1, i32* %start, align 4
  %length = alloca i32, align 4
  store i32 %2, i32* %length, align 4
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %start1 = load i32, i32* %start, align 4
  %lt = icmp slt i32 %start1, 0
  %start2 = load i32, i32* %start, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count3 = load i32, i32* %count, align 4
  %ge = icmp sge i32 %start2, %count3
  %or = or i1 %lt, %ge
  br i1 %or, label %then, label %endif

endif:                                            ; preds = %entry
  %stop = alloca i32, align 4
  %start5 = load i32, i32* %start, align 4
  %length6 = load i32, i32* %length, align 4
  %add = add i32 %start5, %length6
  store i32 %add, i32* %stop, align 4
  %stop9 = load i32, i32* %stop, align 4
  %ref10 = load %String*, %String** %self, align 8
  %count11 = getelementptr %String, %String* %ref10, i32 0, i32 1
  %count12 = load i32, i32* %count11, align 4
  %gt = icmp sgt i32 %stop9, %count12
  br i1 %gt, label %then8, label %endif7

then:                                             ; preds = %entry
  %res4 = load %String, %String* %res, align 8
  ret %String %res4

endif7:                                           ; preds = %then8, %endif
  %i = alloca i32, align 4
  %start16 = load i32, i32* %start, align 4
  store i32 %start16, i32* %i, align 4
  br label %while.cond

then8:                                            ; preds = %endif
  %ref13 = load %String*, %String** %self, align 8
  %count14 = getelementptr %String, %String* %ref13, i32 0, i32 1
  %count15 = load i32, i32* %count14, align 4
  store i32 %count15, i32* %stop, align 4
  br label %endif7

while.cond:                                       ; preds = %while.body, %endif7
  %i17 = load i32, i32* %i, align 4
  %stop18 = load i32, i32* %stop, align 4
  %lt19 = icmp slt i32 %i17, %stop18
  br i1 %lt19, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref20 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref20, i32 0, i32 0
  %value21 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value21, 0
  %i22 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %i22
  %idx = load i8, i8* %arr.at, align 1
  call void @String.push(%String* %res, i8 %idx)
  %cur = load i32, i32* %i, align 4
  %add23 = add i32 %cur, 1
  store i32 %add23, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %res24 = load %String, %String* %res, align 8
  ret %String %res24
}

define %String @String.upper(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %endif, %entry
  %i1 = load i32, i32* %i, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %lt = icmp slt i32 %i1, %count2
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %c = alloca i8, align 1
  %ref3 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref3, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value4, 0
  %i5 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %i5
  %idx = load i8, i8* %arr.at, align 1
  store i8 %idx, i8* %c, align 1
  %c6 = load i8, i8* %c, align 1
  %ge = icmp sge i8 %c6, 97
  %c7 = load i8, i8* %c, align 1
  %le = icmp sle i8 %c7, 122
  %and = and i1 %ge, %le
  br i1 %and, label %then, label %next

while.end:                                        ; preds = %while.cond
  %res11 = load %String, %String* %res, align 8
  ret %String %res11

endif:                                            ; preds = %next, %then
  %cur = load i32, i32* %i, align 4
  %add = add i32 %cur, 1
  store i32 %add, i32* %i, align 4
  br label %while.cond

then:                                             ; preds = %while.body
  %c8 = load i8, i8* %c, align 1
  %cast = sext i8 %c8 to i32
  %sub = sub i32 %cast, 32
  %cast9 = trunc i32 %sub to i8
  call void @String.push(%String* %res, i8 %cast9)
  br label %endif

next:                                             ; preds = %while.body
  %c10 = load i8, i8* %c, align 1
  call void @String.push(%String* %res, i8 %c10)
  br label %endif
}

define %String @String.lower(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %endif, %entry
  %i1 = load i32, i32* %i, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %lt = icmp slt i32 %i1, %count2
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %c = alloca i8, align 1
  %ref3 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref3, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value4, 0
  %i5 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %i5
  %idx = load i8, i8* %arr.at, align 1
  store i8 %idx, i8* %c, align 1
  %c6 = load i8, i8* %c, align 1
  %ge = icmp sge i8 %c6, 65
  %c7 = load i8, i8* %c, align 1
  %le = icmp sle i8 %c7, 90
  %and = and i1 %ge, %le
  br i1 %and, label %then, label %next

while.end:                                        ; preds = %while.cond
  %res12 = load %String, %String* %res, align 8
  ret %String %res12

endif:                                            ; preds = %next, %then
  %cur = load i32, i32* %i, align 4
  %add11 = add i32 %cur, 1
  store i32 %add11, i32* %i, align 4
  br label %while.cond

then:                                             ; preds = %while.body
  %c8 = load i8, i8* %c, align 1
  %cast = sext i8 %c8 to i32
  %add = add i32 %cast, 32
  %cast9 = trunc i32 %add to i8
  call void @String.push(%String* %res, i8 %cast9)
  br label %endif

next:                                             ; preds = %while.body
  %c10 = load i8, i8* %c, align 1
  call void @String.push(%String* %res, i8 %c10)
  br label %endif
}

define %String @String.trim(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %start = alloca i32, align 4
  store i32 0, i32* %start, align 4
  br label %while.cond

while.cond:                                       ; preds = %endif, %entry
  %start1 = load i32, i32* %start, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %lt = icmp slt i32 %start1, %count2
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %c = alloca i8, align 1
  %ref3 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref3, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value4, 0
  %start5 = load i32, i32* %start, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %start5
  %idx = load i8, i8* %arr.at, align 1
  store i8 %idx, i8* %c, align 1
  %c6 = load i8, i8* %c, align 1
  %eq = icmp eq i8 %c6, 32
  %c7 = load i8, i8* %c, align 1
  %eq8 = icmp eq i8 %c7, 9
  %or = or i1 %eq, %eq8
  %c9 = load i8, i8* %c, align 1
  %eq10 = icmp eq i8 %c9, 10
  %or11 = or i1 %or, %eq10
  %c12 = load i8, i8* %c, align 1
  %eq13 = icmp eq i8 %c12, 13
  %or14 = or i1 %or11, %eq13
  br i1 %or14, label %then, label %next

while.end:                                        ; preds = %next, %while.cond
  %stop = alloca i32, align 4
  %ref15 = load %String*, %String** %self, align 8
  %count16 = getelementptr %String, %String* %ref15, i32 0, i32 1
  %count17 = load i32, i32* %count16, align 4
  store i32 %count17, i32* %stop, align 4
  br label %while.cond18

endif:                                            ; preds = %then
  br label %while.cond

then:                                             ; preds = %while.body
  %cur = load i32, i32* %start, align 4
  %add = add i32 %cur, 1
  store i32 %add, i32* %start, align 4
  br label %endif

next:                                             ; preds = %while.body
  br label %while.end

while.cond18:                                     ; preds = %endif31, %while.end
  %stop21 = load i32, i32* %stop, align 4
  %start22 = load i32, i32* %start, align 4
  %gt = icmp sgt i32 %stop21, %start22
  br i1 %gt, label %while.body19, label %while.end20

while.body19:                                     ; preds = %while.cond18
  %c23 = alloca i8, align 1
  %ref24 = load %String*, %String** %self, align 8
  %value25 = getelementptr %String, %String* %ref24, i32 0, i32 0
  %value26 = load { i8*, i64 }, { i8*, i64 }* %value25, align 8
  %arr.data27 = extractvalue { i8*, i64 } %value26, 0
  %stop28 = load i32, i32* %stop, align 4
  %sub = sub i32 %stop28, 1
  %arr.at29 = getelementptr i8, i8* %arr.data27, i32 %sub
  %idx30 = load i8, i8* %arr.at29, align 1
  store i8 %idx30, i8* %c23, align 1
  %c34 = load i8, i8* %c23, align 1
  %eq35 = icmp eq i8 %c34, 32
  %c36 = load i8, i8* %c23, align 1
  %eq37 = icmp eq i8 %c36, 9
  %or38 = or i1 %eq35, %eq37
  %c39 = load i8, i8* %c23, align 1
  %eq40 = icmp eq i8 %c39, 10
  %or41 = or i1 %or38, %eq40
  %c42 = load i8, i8* %c23, align 1
  %eq43 = icmp eq i8 %c42, 13
  %or44 = or i1 %or41, %eq43
  br i1 %or44, label %then32, label %next33

while.end20:                                      ; preds = %next33, %while.cond18
  %ref47 = load %String*, %String** %self, align 8
  %start48 = load i32, i32* %start, align 4
  %stop49 = load i32, i32* %stop, align 4
  %start50 = load i32, i32* %start, align 4
  %sub51 = sub i32 %stop49, %start50
  %call = call %String @String.substr(%String* %ref47, i32 %start48, i32 %sub51)
  ret %String %call

endif31:                                          ; preds = %then32
  br label %while.cond18

then32:                                           ; preds = %while.body19
  %cur45 = load i32, i32* %stop, align 4
  %sub46 = sub i32 %cur45, 1
  store i32 %sub46, i32* %stop, align 4
  br label %endif31

next33:                                           ; preds = %while.body19
  br label %while.end20
}

define %String @String.replace(%String* %0, { i8*, i64 } %1, { i8*, i64 } %2) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %old = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %old, align 8
  %fresh = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %2, { i8*, i64 }* %fresh, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %old1 = load { i8*, i64 }, { i8*, i64 }* %old, align 8
  %opt.ptr = extractvalue { i8*, i64 } %old1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %n = alloca i32, align 4
  %old4 = load { i8*, i64 }, { i8*, i64 }* %old, align 8
  %arr.data = extractvalue { i8*, i64 } %old4, 0
  %call5 = call i64 @strlen(i8* %arr.data)
  %cast = trunc i64 %call5 to i32
  store i32 %cast, i32* %n, align 4
  %n8 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n8, 0
  br i1 %eq, label %then7, label %endif6

then:                                             ; preds = %entry
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value2 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  call void @String.assign(%String* %res, { i8*, i64 } %value2)
  %res3 = load %String, %String* %res, align 8
  ret %String %res3

endif6:                                           ; preds = %endif
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then7:                                            ; preds = %endif
  %ref9 = load %String*, %String** %self, align 8
  %value10 = getelementptr %String, %String* %ref9, i32 0, i32 0
  %value11 = load { i8*, i64 }, { i8*, i64 }* %value10, align 8
  call void @String.assign(%String* %res, { i8*, i64 } %value11)
  %res12 = load %String, %String* %res, align 8
  ret %String %res12

while.cond:                                       ; preds = %endif37, %endif6
  %i13 = load i32, i32* %i, align 4
  %ref14 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref14, i32 0, i32 1
  %count15 = load i32, i32* %count, align 4
  %lt = icmp slt i32 %i13, %count15
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %hit = alloca i1, align 1
  store i1 false, i1* %hit, align 1
  %i18 = load i32, i32* %i, align 4
  %n19 = load i32, i32* %n, align 4
  %add = add i32 %i18, %n19
  %ref20 = load %String*, %String** %self, align 8
  %count21 = getelementptr %String, %String* %ref20, i32 0, i32 1
  %count22 = load i32, i32* %count21, align 4
  %le = icmp sle i32 %add, %count22
  br i1 %le, label %then17, label %endif16

while.end:                                        ; preds = %while.cond
  %res50 = load %String, %String* %res, align 8
  ret %String %res50

endif16:                                          ; preds = %then17, %while.body
  %hit39 = load i1, i1* %hit, align 1
  br i1 %hit39, label %then38, label %next

then17:                                           ; preds = %while.body
  %ref23 = load %String*, %String** %self, align 8
  %value24 = getelementptr %String, %String* %ref23, i32 0, i32 0
  %value25 = load { i8*, i64 }, { i8*, i64 }* %value24, align 8
  %arr.data26 = extractvalue { i8*, i64 } %value25, 0
  %i27 = load i32, i32* %i, align 4
  %i28 = load i32, i32* %i, align 4
  %n29 = load i32, i32* %n, align 4
  %add30 = add i32 %i28, %n29
  %start = sext i32 %i27 to i64
  %end = sext i32 %add30 to i64
  %slice.data = getelementptr i8, i8* %arr.data26, i64 %start
  %slice.len = sub i64 %end, %start
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %slice.data, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %slice.len, 1
  %arr.data31 = extractvalue { i8*, i64 } %arr.len, 0
  %old32 = load { i8*, i64 }, { i8*, i64 }* %old, align 8
  %arr.data33 = extractvalue { i8*, i64 } %old32, 0
  %n34 = load i32, i32* %n, align 4
  %call35 = call i32 @strncmp(i8* %arr.data31, i8* %arr.data33, i32 %n34)
  %eq36 = icmp eq i32 %call35, 0
  store i1 %eq36, i1* %hit, align 1
  br label %endif16

endif37:                                          ; preds = %next, %then38
  br label %while.cond

then38:                                           ; preds = %endif16
  %fresh40 = load { i8*, i64 }, { i8*, i64 }* %fresh, align 8
  call void @String.join(%String* %res, { i8*, i64 } %fresh40)
  %n41 = load i32, i32* %n, align 4
  %cur = load i32, i32* %i, align 4
  %add42 = add i32 %cur, %n41
  store i32 %add42, i32* %i, align 4
  br label %endif37

next:                                             ; preds = %endif16
  %ref43 = load %String*, %String** %self, align 8
  %value44 = getelementptr %String, %String* %ref43, i32 0, i32 0
  %value45 = load { i8*, i64 }, { i8*, i64 }* %value44, align 8
  %arr.data46 = extractvalue { i8*, i64 } %value45, 0
  %i47 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data46, i32 %i47
  %idx = load i8, i8* %arr.at, align 1
  call void @String.push(%String* %res, i8 %idx)
  %cur48 = load i32, i32* %i, align 4
  %add49 = add i32 %cur48, 1
  store i32 %add49, i32* %i, align 4
  br label %endif37
}

define %String @String.repeat(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %i1 = load i32, i32* %i, align 4
  %n2 = load i32, i32* %n, align 4
  %lt = icmp slt i32 %i1, %n2
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value3 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  call void @String.join(%String* %res, { i8*, i64 } %value3)
  %cur = load i32, i32* %i, align 4
  %add = add i32 %cur, 1
  store i32 %add, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %res4 = load %String, %String* %res, align 8
  ret %String %res4
}

define %String @String.reverse(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count1 = load i32, i32* %count, align 4
  %sub = sub i32 %count1, 1
  store i32 %sub, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %i2 = load i32, i32* %i, align 4
  %ge = icmp sge i32 %i2, 0
  br i1 %ge, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref3 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref3, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value4, 0
  %i5 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %i5
  %idx = load i8, i8* %arr.at, align 1
  call void @String.push(%String* %res, i8 %idx)
  %cur = load i32, i32* %i, align 4
  %sub6 = sub i32 %cur, 1
  store i32 %sub6, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %res7 = load %String, %String* %res, align 8
  ret %String %res7
}

define i32 @String.compare(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %other = alloca %String*, align 8
  store %String* %1, %String** %other, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value1, 0
  %isnull = icmp eq i8* %opt.ptr, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.5, i32 0, i32 0), i64 0 }, { i8*, i64 } %value1
  %arr.data = extractvalue { i8*, i64 } %fallback, 0
  %ref2 = load %String*, %String** %other, align 8
  %value3 = getelementptr %String, %String* %ref2, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value3, align 8
  %opt.ptr5 = extractvalue { i8*, i64 } %value4, 0
  %isnull6 = icmp eq i8* %opt.ptr5, null
  %fallback7 = select i1 %isnull6, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.6, i32 0, i32 0), i64 0 }, { i8*, i64 } %value4
  %arr.data8 = extractvalue { i8*, i64 } %fallback7, 0
  %call = call i32 @strcmp(i8* %arr.data, i8* %arr.data8)
  ret i32 %call
}

declare i32 @strcmp(i8*, i8*)

define i32 @String.to_int(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value1, 0
  %isnull = icmp eq i8* %opt.ptr, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.7, i32 0, i32 0), i64 0 }, { i8*, i64 } %value1
  %arr.data = extractvalue { i8*, i64 } %fallback, 0
  %call = call i32 @atoi(i8* %arr.data)
  ret i32 %call
}

declare i32 @atoi(i8*)

define void @"String.=.array"(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %ref = load %String*, %String** %self, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  call void @String.assign(%String* %ref, { i8*, i64 } %str1)
  ret void
}

define void @"String.=.ref.String"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %v = alloca %String*, align 8
  store %String* %1, %String** %v, align 8
  %ref = load %String*, %String** %self, align 8
  %ref1 = load %String*, %String** %v, align 8
  %value = getelementptr %String, %String* %ref1, i32 0, i32 0
  %value2 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  call void @String.assign(%String* %ref, { i8*, i64 } %value2)
  ret void
}

define %String @"String.+.ref.String"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %v = alloca %String*, align 8
  store %String* %1, %String** %v, align 8
  %res = alloca %String, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %call = call %String @String.from({ i8*, i64 } %value1)
  store %String %call, %String* %res, align 8
  %ref2 = load %String*, %String** %v, align 8
  %value3 = getelementptr %String, %String* %ref2, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value3, align 8
  call void @String.join(%String* %res, { i8*, i64 } %value4)
  %res5 = load %String, %String* %res, align 8
  ret %String %res5
}

define %String @"String.+.array"(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %res = alloca %String, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %call = call %String @String.from({ i8*, i64 } %value1)
  store %String %call, %String* %res, align 8
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  call void @String.join(%String* %res, { i8*, i64 } %str2)
  %res3 = load %String, %String* %res, align 8
  ret %String %res3
}

define void @"String.+=.ref.String"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %v = alloca %String*, align 8
  store %String* %1, %String** %v, align 8
  %ref = load %String*, %String** %self, align 8
  %ref1 = load %String*, %String** %v, align 8
  %value = getelementptr %String, %String* %ref1, i32 0, i32 0
  %value2 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  call void @String.join(%String* %ref, { i8*, i64 } %value2)
  ret void
}

define void @"String.+=.array"(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %ref = load %String*, %String** %self, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  call void @String.join(%String* %ref, { i8*, i64 } %str1)
  ret void
}

define i1 @"String.==.ref.String"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %v = alloca %String*, align 8
  store %String* %1, %String** %v, align 8
  %ref = load %String*, %String** %self, align 8
  %ref1 = load %String*, %String** %v, align 8
  %call = call i32 @String.compare(%String* %ref, %String* %ref1)
  %eq = icmp eq i32 %call, 0
  ret i1 %eq
}

define i1 @"String.!=.ref.String"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %v = alloca %String*, align 8
  store %String* %1, %String** %v, align 8
  %ref = load %String*, %String** %self, align 8
  %ref1 = load %String*, %String** %v, align 8
  %call = call i32 @String.compare(%String* %ref, %String* %ref1)
  %ne = icmp ne i32 %call, 0
  ret i1 %ne
}

define i1 @"String.==.array"(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr = extractvalue { i8*, i64 } %str1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %ref3 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref3, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr5 = extractvalue { i8*, i64 } %value4, 0
  %isnull = icmp eq i8* %opt.ptr5, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.8, i32 0, i32 0), i64 0 }, { i8*, i64 } %value4
  %arr.data = extractvalue { i8*, i64 } %fallback, 0
  %str6 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data7 = extractvalue { i8*, i64 } %str6, 0
  %call = call i32 @strcmp(i8* %arr.data, i8* %arr.data7)
  %eq8 = icmp eq i32 %call, 0
  ret i1 %eq8

then:                                             ; preds = %entry
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %eq = icmp eq i32 %count2, 0
  ret i1 %eq
}

define i1 @"String.!=.array"(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr = extractvalue { i8*, i64 } %str1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %ref3 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref3, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr5 = extractvalue { i8*, i64 } %value4, 0
  %isnull = icmp eq i8* %opt.ptr5, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.9, i32 0, i32 0), i64 0 }, { i8*, i64 } %value4
  %arr.data = extractvalue { i8*, i64 } %fallback, 0
  %str6 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data7 = extractvalue { i8*, i64 } %str6, 0
  %call = call i32 @strcmp(i8* %arr.data, i8* %arr.data7)
  %ne8 = icmp ne i32 %call, 0
  ret i1 %ne8

then:                                             ; preds = %entry
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %ne = icmp ne i32 %count2, 0
  ret i1 %ne
}

define i1 @"String.<.ref.String"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %v = alloca %String*, align 8
  store %String* %1, %String** %v, align 8
  %ref = load %String*, %String** %self, align 8
  %ref1 = load %String*, %String** %v, align 8
  %call = call i32 @String.compare(%String* %ref, %String* %ref1)
  %lt = icmp slt i32 %call, 0
  ret i1 %lt
}

define i1 @"String.>.ref.String"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %v = alloca %String*, align 8
  store %String* %1, %String** %v, align 8
  %ref = load %String*, %String** %self, align 8
  %ref1 = load %String*, %String** %v, align 8
  %call = call i32 @String.compare(%String* %ref, %String* %ref1)
  %gt = icmp sgt i32 %call, 0
  ret i1 %gt
}

define i1 @"String.<.array"(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value1, 0
  %isnull = icmp eq i8* %opt.ptr, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.10, i32 0, i32 0), i64 0 }, { i8*, i64 } %value1
  %arr.data = extractvalue { i8*, i64 } %fallback, 0
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr3 = extractvalue { i8*, i64 } %str2, 0
  %isnull4 = icmp eq i8* %opt.ptr3, null
  %fallback5 = select i1 %isnull4, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.11, i32 0, i32 0), i64 0 }, { i8*, i64 } %str2
  %arr.data6 = extractvalue { i8*, i64 } %fallback5, 0
  %call = call i32 @strcmp(i8* %arr.data, i8* %arr.data6)
  %lt = icmp slt i32 %call, 0
  ret i1 %lt
}

define i1 @"String.>.array"(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value1, 0
  %isnull = icmp eq i8* %opt.ptr, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.12, i32 0, i32 0), i64 0 }, { i8*, i64 } %value1
  %arr.data = extractvalue { i8*, i64 } %fallback, 0
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr3 = extractvalue { i8*, i64 } %str2, 0
  %isnull4 = icmp eq i8* %opt.ptr3, null
  %fallback5 = select i1 %isnull4, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.13, i32 0, i32 0), i64 0 }, { i8*, i64 } %str2
  %arr.data6 = extractvalue { i8*, i64 } %fallback5, 0
  %call = call i32 @strcmp(i8* %arr.data, i8* %arr.data6)
  %gt = icmp sgt i32 %call, 0
  ret i1 %gt
}

define { i8*, i64 } @String.output(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value1, 0
  %isnull = icmp eq i8* %opt.ptr, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.14, i32 0, i32 0), i64 0 }, { i8*, i64 } %value1
  ret { i8*, i64 } %fallback
}

define %Buffer @Buffer.create() {
entry:
  %b = alloca %Buffer, align 8
  store %Buffer zeroinitializer, %Buffer* %b, align 8
  %value = getelementptr %Buffer, %Buffer* %b, i32 0, i32 0
  %heap = call i8* @calloc(i64 64, i64 1)
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %heap, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 64, 1
  store { i8*, i64 } %arr.len, { i8*, i64 }* %value, align 8
  %b1 = load %Buffer, %Buffer* %b, align 8
  ret %Buffer %b1
}

define void @Buffer.drop(%Buffer* %0) {
entry:
  %self = alloca %Buffer*, align 8
  store %Buffer* %0, %Buffer** %self, align 8
  %ref = load %Buffer*, %Buffer** %self, align 8
  %value = getelementptr %Buffer, %Buffer* %ref, i32 0, i32 0
  %arr = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %arr, 0
  call void @free(i8* %arr.data)
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %value, align 8
  ret void
}

define void @"Buffer.=.array"(%Buffer* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Buffer*, align 8
  store %Buffer* %0, %Buffer** %self, align 8
  %s = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %s, align 8
  %ref = load %Buffer*, %Buffer** %self, align 8
  %value = getelementptr %Buffer, %Buffer* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value1, 0
  %s2 = load { i8*, i64 }, { i8*, i64 }* %s, align 8
  %arr.data3 = extractvalue { i8*, i64 } %s2, 0
  %call = call i8* @strcpy(i8* %arr.data, i8* %arr.data3)
  ret void
}

declare i8* @strcpy(i8*, i8*)

define void @"Buffer.=.ref.Buffer"(%Buffer* %0, %Buffer* %1) {
entry:
  %self = alloca %Buffer*, align 8
  store %Buffer* %0, %Buffer** %self, align 8
  %b = alloca %Buffer*, align 8
  store %Buffer* %1, %Buffer** %b, align 8
  %ref = load %Buffer*, %Buffer** %self, align 8
  %value = getelementptr %Buffer, %Buffer* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value1, 0
  %ref2 = load %Buffer*, %Buffer** %b, align 8
  %value3 = getelementptr %Buffer, %Buffer* %ref2, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value3, align 8
  %arr.data5 = extractvalue { i8*, i64 } %value4, 0
  %call = call i8* @strcpy(i8* %arr.data, i8* %arr.data5)
  ret void
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %b1 = alloca %Buffer, align 8
  %call = call %Buffer @Buffer.create()
  store %Buffer %call, %Buffer* %b1, align 8
  call void @"Buffer.=.array"(%Buffer* %b1, { i8*, i64 } { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.15, i32 0, i32 0), i64 5 })
  %b2 = alloca %Buffer, align 8
  %call1 = call %Buffer @Buffer.create()
  store %Buffer %call1, %Buffer* %b2, align 8
  call void @"Buffer.=.ref.Buffer"(%Buffer* %b2, %Buffer* %b1)
  %value = getelementptr %Buffer, %Buffer* %b2, i32 0, i32 0
  %value2 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value2, 0
  %call3 = call i8* @strcpy(i8* %arr.data, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.16, i32 0, i32 0))
  %value4 = getelementptr %Buffer, %Buffer* %b1, i32 0, i32 0
  %value5 = load { i8*, i64 }, { i8*, i64 }* %value4, align 8
  %str.len = extractvalue { i8*, i64 } %value5, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %value5, 0
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i32 4, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.17, i32 0, i32 0), i32 %len32, i8* %str.data, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.18, i32 0, i32 0))
  %value6 = getelementptr %Buffer, %Buffer* %b2, i32 0, i32 0
  %value7 = load { i8*, i64 }, { i8*, i64 }* %value6, align 8
  %str.len8 = extractvalue { i8*, i64 } %value7, 1
  %len329 = trunc i64 %str.len8 to i32
  %str.data10 = extractvalue { i8*, i64 } %value7, 0
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt.21, i32 0, i32 0), i32 4, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.19, i32 0, i32 0), i32 %len329, i8* %str.data10, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.20, i32 0, i32 0))
  call void @Buffer.drop(%Buffer* %b2)
  call void @Buffer.drop(%Buffer* %b1)
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 028 — a growable string struct: nullable buffer, grow, join and drop

```ura
// operators_overload/028.ura - a growable string struct: nullable buffer, grow, join and drop

use "@/header"

struct String2:
    value char[]?
    count i32
    size  i32

    pub fn create() String2:
        s String2
        s.size  = 10
        s.value = new char[10]
        return s

    operator drop:
        clean self.value

    fn grow(want i32) void:
        if self.size >= want: return
        while self.size < want:
            self.size *= 2
        bigger char[] = new char[self.size]
        strcpy(bigger, self.value)
        clean self.value
        self.value = bigger

    fn assign(str char[]?) void:
        if str == null:
            self.assign("")
            return
        len i32 = strlen(str) as i32
        self.grow(len + 1)
        bzero(self.value, self.size as i64)
        strcpy(self.value, str)
        self.count = len

    fn join(str char[]?) void:
        if str == null: return
        len i32 = strlen(str) as i32
        self.grow(self.count + len + 1)
        strcat(self.value, str)
        self.count += len

    operator = (str char[]?) void:
        self.assign(str)

    operator = (ref v String2) void:
        self.assign(v.value)

fn add(ref v String2, ref w String2) String2:
    res String2 = String2::create()
    res = v
    res.join(w.value)
    return res

main():
    s0 String2 = String2::create()
    s0 = "1x2345"

    s1 String2 = String2::create()
    s1 = "6x789"

    s2 String2 = String2::create()
    s2 = add(ref s0, ref s1)

    output("s0:    ", s0.value, " (", s0.count, ")\n")
    output("s1:    ", s1.value, " (", s1.count, ")\n")
    output("s0+s1: ", s2.value, " (", s2.count, ")\n")
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

proto fn isalnum(c : i32) : i32

proto fn isalpha(c : i32) : i32

proto fn isascii(c : i32) : i32

proto fn isblank(c : i32) : i32

proto fn iscntrl(c : i32) : i32

proto fn isdigit(c : i32) : i32

proto fn isgraph(c : i32) : i32

proto fn islower(c : i32) : i32

proto fn isprint(c : i32) : i32

proto fn ispunct(c : i32) : i32

proto fn isspace(c : i32) : i32

proto fn isupper(c : i32) : i32

proto fn isxdigit(c : i32) : i32

proto fn tolower(c : i32) : i32

proto fn toupper(c : i32) : i32

proto fn strerror(errnum : i32) : pointer

proto fn strerror_r(errnum : i32, buf : pointer, buflen : i64) : i32

proto fn perror(s : pointer) : void

= : i32
├─ EPERM : i32
└─ int 1

= : i32
├─ ENOENT : i32
└─ int 2

= : i32
├─ ESRCH : i32
└─ int 3

= : i32
├─ EINTR : i32
└─ int 4

= : i32
├─ EIO : i32
└─ int 5

= : i32
├─ ENXIO : i32
└─ int 6

= : i32
├─ E2BIG : i32
└─ int 7

= : i32
├─ ENOEXEC : i32
└─ int 8

= : i32
├─ EBADF : i32
└─ int 9

= : i32
├─ ECHILD : i32
└─ int 10

= : i32
├─ EACCES : i32
└─ int 13

= : i32
├─ EFAULT : i32
└─ int 14

= : i32
├─ ENOTBLK : i32
└─ int 15

= : i32
├─ EBUSY : i32
└─ int 16

= : i32
├─ EEXIST : i32
└─ int 17

= : i32
├─ EXDEV : i32
└─ int 18

= : i32
├─ ENODEV : i32
└─ int 19

= : i32
├─ ENOTDIR : i32
└─ int 20

= : i32
├─ EISDIR : i32
└─ int 21

= : i32
├─ EINVAL : i32
└─ int 22

= : i32
├─ ENFILE : i32
└─ int 23

= : i32
├─ EMFILE : i32
└─ int 24

= : i32
├─ ENOTTY : i32
└─ int 25

= : i32
├─ ETXTBSY : i32
└─ int 26

= : i32
├─ EFBIG : i32
└─ int 27

= : i32
├─ ENOSPC : i32
└─ int 28

= : i32
├─ ESPIPE : i32
└─ int 29

= : i32
├─ EROFS : i32
└─ int 30

= : i32
├─ EMLINK : i32
└─ int 31

= : i32
├─ EPIPE : i32
└─ int 32

= : i32
├─ EDOM : i32
└─ int 33

= : i32
├─ ERANGE : i32
└─ int 34

proto fn open(path : pointer, flags : i32, mode : i32) : i32

proto fn openat(dirfd : i32, path : pointer, flags : i32, mode : i32) : i32

proto fn creat(path : pointer, mode : i32) : i32

proto fn fcntl(fd : i32, cmd : i32, arg : i32) : i32

= : i32
├─ O_RDONLY : i32
└─ int 0

= : i32
├─ O_WRONLY : i32
└─ int 1

= : i32
├─ O_RDWR : i32
└─ int 2

= : i32
├─ FD_CLOEXEC : i32
└─ int 1

proto fn fopen(path : pointer, mode : pointer) : pointer

proto fn freopen(path : pointer, mode : pointer, file : pointer) : pointer

proto fn fclose(file : pointer) : i32

proto fn fflush(file : pointer) : i32

proto fn fgetc(file : pointer) : i32

proto fn fputc(c : i32, file : pointer) : i32

proto fn getc(file : pointer) : i32

proto fn putc(c : i32, file : pointer) : i32

proto fn getchar() : i32

proto fn putchar(c : char) : i32

proto fn ungetc(c : i32, file : pointer) : i32

proto fn fgets(buffer : pointer, size : i32, file : pointer) : pointer

proto fn fputs(str : pointer, file : pointer) : i32

proto fn puts(str : pointer) : i32

proto fn gets(buffer : pointer) : pointer

proto fn fread(ptr : pointer, size : i32, nmemb : i32, file : pointer) : i32

proto fn fwrite(ptr : pointer, size : i32, nmemb : i32, file : pointer) : i32

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn read(fd : i32, ptr : pointer, len : i64) : i64

proto fn fseek(file : pointer, offset : i64, whence : i32) : i32

proto fn ftell(file : pointer) : i64

proto fn rewind(file : pointer) : void

proto fn fgetpos(file : pointer, pos : pointer) : i32

proto fn fsetpos(file : pointer, pos : pointer) : i32

proto fn feof(file : pointer) : i32

proto fn ferror(file : pointer) : i32

proto fn clearerr(file : pointer) : void

proto fn perror(str : pointer) : void

proto fn setvbuf(file : pointer, buffer : pointer, mode : i32, size : i32) : i32

proto fn setbuf(file : pointer, buffer : pointer) : void

proto fn remove(path : pointer) : i32

proto fn rename(oldpath : pointer, newpath : pointer) : i32

proto fn tmpfile() : pointer

proto fn tmpnam(str : pointer) : pointer

proto fn fileno(file : pointer) : i32

proto fn fdopen(fd : i32, mode : pointer) : pointer

proto fn printf(format : pointer, ...) : i32

proto fn fprintf(file : pointer, format : pointer, ...) : i32

proto fn sprintf(buffer : pointer, format : pointer, ...) : i32

proto fn snprintf(buffer : pointer, size : i32, format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn vprintf(format : pointer, args : pointer) : i32

proto fn vfprintf(file : pointer, format : pointer, args : pointer) : i32

proto fn vsprintf(buffer : pointer, format : pointer, args : pointer) : i32

proto fn vsnprintf(buffer : pointer, size : i32, format : pointer, args : pointer) : i32

proto fn vdprintf(fd : i32, format : pointer, args : pointer) : i32

proto fn scanf(format : pointer, ...) : i32

proto fn fscanf(file : pointer, format : pointer, ...) : i32

proto fn sscanf(buffer : pointer, format : pointer, ...) : i32

proto fn vscanf(format : pointer, args : pointer) : i32

proto fn vfscanf(file : pointer, format : pointer, args : pointer) : i32

proto fn vsscanf(buffer : pointer, format : pointer, args : pointer) : i32

proto fn sin(x : f64) : f64

proto fn cos(x : f64) : f64

proto fn tan(x : f64) : f64

proto fn asin(x : f64) : f64

proto fn acos(x : f64) : f64

proto fn atan(x : f64) : f64

proto fn atan2(y : f64, x : f64) : f64

proto fn sinh(x : f64) : f64

proto fn cosh(x : f64) : f64

proto fn tanh(x : f64) : f64

proto fn asinh(x : f64) : f64

proto fn acosh(x : f64) : f64

proto fn atanh(x : f64) : f64

proto fn exp(x : f64) : f64

proto fn exp2(x : f64) : f64

proto fn expm1(x : f64) : f64

proto fn log(x : f64) : f64

proto fn log2(x : f64) : f64

proto fn log10(x : f64) : f64

proto fn log1p(x : f64) : f64

proto fn pow(x : f64, y : f64) : f64

proto fn sqrt(x : f64) : f64

proto fn cbrt(x : f64) : f64

proto fn hypot(x : f64, y : f64) : f64

proto fn ceil(x : f64) : f64

proto fn floor(x : f64) : f64

proto fn trunc(x : f64) : f64

proto fn round(x : f64) : f64

proto fn fmod(x : f64, y : f64) : f64

proto fn remainder(x : f64, y : f64) : f64

proto fn fabs(x : f64) : f64

proto fn copysign(x : f64, y : f64) : f64

proto fn fmax(x : f64, y : f64) : f64

proto fn fmin(x : f64, y : f64) : f64

proto fn fdim(x : f64, y : f64) : f64

proto fn isnan(x : f64) : i32

proto fn isinf(x : f64) : i32

proto fn isfinite(x : f64) : i32

proto fn signbit(x : f64) : i32

proto fn tgamma(x : f64) : f64

proto fn lgamma(x : f64) : f64

proto fn erf(x : f64) : f64

proto fn erfc(x : f64) : f64

proto fn malloc(size : i32) : pointer

proto fn calloc(len : i64, size : i64) : pointer

proto fn realloc(ptr : pointer, newSize : i32) : pointer

proto fn free(ptr : pointer) : void

proto fn aligned_alloc(alignment : i32, size : i32) : pointer

struct Pollfd
├─ fd : i32
├─ events : i16
└─ revents : i16

proto fn socket(domain : i32, type : i32, protocol : i32) : i32

proto fn bind(fd : i32, addr : pointer, addrlen : i32) : i32

proto fn listen(fd : i32, backlog : i32) : i32

proto fn accept(fd : i32, addr : pointer, addrlen : pointer) : i32

proto fn connect(fd : i32, addr : pointer, addrlen : i32) : i32

proto fn shutdown(fd : i32, how : i32) : i32

proto fn send(fd : i32, buf : pointer, len : i64, flags : i32) : i64

proto fn recv(fd : i32, buf : pointer, len : i64, flags : i32) : i64

proto fn sendto(fd : i32, buf : pointer, len : i64, flags : i32, addr : pointer, addrlen : i32) : i64

proto fn recvfrom(fd : i32, buf : pointer, len : i64, flags : i32, addr : pointer, addrlen : pointer) : i64

proto fn sendmsg(fd : i32, msg : pointer, flags : i32) : i64

proto fn recvmsg(fd : i32, msg : pointer, flags : i32) : i64

proto fn setsockopt(fd : i32, level : i32, optname : i32, optval : pointer, optlen : i32) : i32

proto fn getsockopt(fd : i32, level : i32, optname : i32, optval : pointer, optlen : pointer) : i32

proto fn getsockname(fd : i32, addr : pointer, addrlen : pointer) : i32

proto fn getpeername(fd : i32, addr : pointer, addrlen : pointer) : i32

proto fn htons(hostshort : i32) : i32

proto fn htonl(hostlong : i32) : i32

proto fn ntohs(netshort : i32) : i32

proto fn ntohl(netlong : i32) : i32

proto fn inet_addr(cp : pointer) : i32

proto fn inet_ntoa(addr : u32) : pointer

proto fn inet_pton(af : i32, src : pointer, dst : pointer) : i32

proto fn inet_ntop(af : i32, src : pointer, dst : pointer, size : i32) : pointer

proto fn gethostbyname(name : pointer) : pointer

proto fn gethostbyaddr(addr : pointer, len : i32, type : i32) : pointer

proto fn getaddrinfo(node : pointer, service : pointer, hints : pointer, res : pointer) : i32

proto fn freeaddrinfo(res : pointer) : void

proto fn gai_strerror(errcode : i32) : pointer

proto fn getnameinfo(addr : pointer, addrlen : i32, host : pointer, hostlen : i32, serv : pointer, servlen : i32, flags : i32) : i32

proto fn select(nfds : i32, readfds : pointer, writefds : pointer, exceptfds : pointer, timeout : pointer) : i32

proto fn poll(fds : pointer, nfds : i32, timeout : i32) : i32

proto fn ioctl(fd : i32, request : i32, arg : pointer) : i32

= : i32
├─ IPPROTO_IP : i32
└─ int 0

= : i32
├─ IPPROTO_TCP : i32
└─ int 6

= : i32
├─ IPPROTO_UDP : i32
└─ int 17

= : i32
├─ IPPROTO_IPV6 : i32
└─ int 41

= : i32
├─ SHUT_RD : i32
└─ int 0

= : i32
├─ SHUT_WR : i32
└─ int 1

= : i32
├─ SHUT_RDWR : i32
└─ int 2

= : i32
├─ POLLIN : i32
└─ int 1

= : i32
├─ POLLPRI : i32
└─ int 2

= : i32
├─ POLLOUT : i32
└─ int 4

= : i32
├─ POLLERR : i32
└─ int 8

= : i32
├─ POLLHUP : i32
└─ int 16

= : i32
├─ POLLNVAL : i32
└─ int 32

= : i32
├─ INADDR_ANY : i32
└─ int 0

= : i32
├─ INADDR_LOOPBACK : i32
└─ int 2130706433

= : i32
├─ INADDR_BROADCAST : i32
└─ - : i32
   ├─ int 0
   └─ int 1

proto fn signal(sig : i32, handler : pointer) : pointer

proto fn raise(sig : i32) : i32

proto fn kill(pid : i32, sig : i32) : i32

proto fn sigaction(sig : i32, act : pointer, oldact : pointer) : i32

proto fn sigprocmask(how : i32, set : pointer, oldset : pointer) : i32

proto fn sigsuspend(mask : pointer) : i32

proto fn strsignal(sig : i32) : pointer

= : i32
├─ SIGHUP : i32
└─ int 1

= : i32
├─ SIGINT : i32
└─ int 2

= : i32
├─ SIGQUIT : i32
└─ int 3

= : i32
├─ SIGILL : i32
└─ int 4

= : i32
├─ SIGTRAP : i32
└─ int 5

= : i32
├─ SIGABRT : i32
└─ int 6

= : i32
├─ SIGFPE : i32
└─ int 8

= : i32
├─ SIGKILL : i32
└─ int 9

= : i32
├─ SIGSEGV : i32
└─ int 11

= : i32
├─ SIGPIPE : i32
└─ int 13

= : i32
├─ SIGALRM : i32
└─ int 14

= : i32
├─ SIGTERM : i32
└─ int 15

proto fn stat(path : pointer, buf : pointer) : i32

proto fn fstat(fd : i32, buf : pointer) : i32

proto fn lstat(path : pointer, buf : pointer) : i32

proto fn mkdir(path : pointer, mode : i32) : i32

proto fn chmod(path : pointer, mode : i32) : i32

proto fn fchmod(fd : i32, mode : i32) : i32

proto fn umask(cmask : i32) : i32

= : i32
├─ S_IFMT : i32
└─ int 61440

= : i32
├─ S_IFSOCK : i32
└─ int 49152

= : i32
├─ S_IFLNK : i32
└─ int 40960

= : i32
├─ S_IFREG : i32
└─ int 32768

= : i32
├─ S_IFBLK : i32
└─ int 24576

= : i32
├─ S_IFDIR : i32
└─ int 16384

= : i32
├─ S_IFCHR : i32
└─ int 8192

= : i32
├─ S_IFIFO : i32
└─ int 4096

= : i32
├─ S_ISUID : i32
└─ int 2048

= : i32
├─ S_ISGID : i32
└─ int 1024

= : i32
├─ S_ISVTX : i32
└─ int 512

proto fn abort() : void

proto fn exit(code : i32) : void

proto fn quick_exit(code : i32) : void

proto fn _Exit(code : i32) : void

proto fn getenv(name : pointer) : pointer

proto fn setenv(name : pointer, value : pointer, overwrite : i32) : i32

proto fn unsetenv(name : pointer) : i32

proto fn putenv(string : pointer) : i32

proto fn system(command : pointer) : i32

proto fn rand() : i32

proto fn srand(seed : i32) : void

proto fn bsearch(key : pointer, base : pointer, nmemb : i32, size : i32, compar : pointer) : pointer

proto fn qsort(base : pointer, nmemb : i32, size : i32, compar : pointer) : void

proto fn abs(n : i32) : i32

proto fn labs(n : i64) : i64

proto fn div(numer : i32, denom : i32) : pointer

proto fn ldiv(numer : i64, denom : i64) : pointer

proto fn atoi(str : pointer) : i32

proto fn atol(str : pointer) : i64

proto fn atof(str : pointer) : f64

proto fn strtol(str : pointer, endptr : pointer, base : i32) : i64

proto fn strtoul(str : pointer, endptr : pointer, base : i32) : i64

proto fn strtof(str : pointer, endptr : pointer) : f32

proto fn strtod(str : pointer, endptr : pointer) : f64

proto fn strtoll(str : pointer, endptr : pointer, base : i32) : i64

proto fn strtoull(str : pointer, endptr : pointer, base : i32) : i64

proto fn mblen(s : pointer, n : i32) : i32

proto fn mbtowc(pwc : pointer, s : pointer, n : i32) : i32

proto fn wctomb(s : pointer, wchar : i32) : i32

proto fn mbstowcs(dest : pointer, src : pointer, n : i32) : i32

proto fn wcstombs(dest : pointer, src : pointer, n : i32) : i32

proto fn strlen(s : pointer) : i64

proto fn strnlen(s : pointer, maxlen : i32) : i32

proto fn strcmp(a : pointer, b : pointer) : i32

proto fn strncmp(a : pointer, b : pointer, n : i32) : i32

proto fn strcasecmp(a : pointer, b : pointer) : i32

proto fn strncasecmp(a : pointer, b : pointer, n : i32) : i32

proto fn strcoll(a : pointer, b : pointer) : i32

proto fn strxfrm(dest : pointer, src : pointer, n : i32) : i32

proto fn strcpy(dest : pointer, src : pointer) : pointer

proto fn strncpy(dest : pointer, src : pointer, n : i32) : pointer

proto fn strdup(s : pointer) : pointer

proto fn strndup(s : pointer, n : i32) : pointer

proto fn strcat(dest : pointer, src : pointer) : pointer

proto fn strncat(dest : pointer, src : pointer, n : i32) : pointer

proto fn strchr(s : pointer, c : i32) : pointer

proto fn strrchr(s : pointer, c : i32) : pointer

proto fn strstr(haystack : pointer, needle : pointer) : pointer

proto fn strcasestr(haystack : pointer, needle : pointer) : pointer

proto fn strspn(s : pointer, accept : pointer) : i32

proto fn strcspn(s : pointer, reject : pointer) : i32

proto fn strpbrk(s : pointer, accept : pointer) : pointer

proto fn strtok(s : pointer, delim : pointer) : pointer

proto fn strtok_r(s : pointer, delim : pointer, saveptr : pointer) : pointer

proto fn strerror(errnum : i32) : pointer

proto fn strerror_r(errnum : i32, buf : pointer, buflen : i64) : i32

proto fn memchr(ptr : pointer, c : i32, n : i32) : pointer

proto fn memrchr(ptr : pointer, c : i32, n : i32) : pointer

proto fn memcmp(a : pointer, b : pointer, n : i32) : i32

proto fn memcpy(dest : pointer, src : pointer, n : i32) : pointer

proto fn memmove(dest : pointer, src : pointer, n : i32) : pointer

proto fn memset(ptr : pointer, value : i32, n : i32) : pointer

proto fn memccpy(dest : pointer, src : pointer, c : i32, n : i32) : pointer

proto fn bzero(s : pointer, size : i64) : void

proto fn strlcpy(dest : pointer, src : pointer, size : i32) : i32

proto fn strlcat(dest : pointer, src : pointer, size : i32) : i32

proto fn strsignal(sig : i32) : pointer

proto fn memset_s(ptr : pointer, size : i32, value : i32, n : i32) : pointer

proto fn atoi(s : pointer) : i32

struct String
├─ value : char[]
├─ count : i32
├─ fn String.create() : STRUCT_CALL
│  ├─ s : STRUCT_CALL
│  ├─ = : array
│  │  ├─ .value : char[]
│  │  │  └─ s : STRUCT_CALL
│  │  └─ array : char[]
│  │     └─ int 16
│  ├─ = : i32
│  │  ├─ .count : i32
│  │  │  └─ s : STRUCT_CALL
│  │  └─ int 0
│  └─ return
│     └─ s : STRUCT_CALL
├─ fn String.from(str : array) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ s : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  ├─ call assign : void
│  │  ├─ s : STRUCT_CALL
│  │  └─ str : char[]
│  └─ return
│     └─ s : STRUCT_CALL
├─ fn String.from_int(n : i32) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ s : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ n : i32
│  │  │  └─ int 0
│  │  ├─ call push : void
│  │  │  ├─ s : STRUCT_CALL
│  │  │  └─ char '0'
│  │  └─ return
│  │     └─ s : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ num : i32
│  │  └─ n : i32
│  ├─ if
│  │  ├─ condition < : bool
│  │  │  ├─ num : i32
│  │  │  └─ int 0
│  │  ├─ call push : void
│  │  │  ├─ s : STRUCT_CALL
│  │  │  └─ char '-'
│  │  └─ = : i32
│  │     ├─ num : i32
│  │     └─ - : i32
│  │        ├─ int 0
│  │        └─ num : i32
│  ├─ = : array
│  │  ├─ digits : char[]
│  │  └─ array : char[]
│  │     └─ int 12
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ int 0
│  ├─ while
│  │  ├─ condition > : bool
│  │  │  ├─ num : i32
│  │  │  └─ int 0
│  │  ├─ = : char
│  │  │  ├─ index : char
│  │  │  │  ├─ digits : char[]
│  │  │  │  └─ i : i32
│  │  │  └─ cast : char
│  │  │     └─ + : i32
│  │  │        ├─ % : i32
│  │  │        │  ├─ num : i32
│  │  │        │  └─ int 10
│  │  │        └─ int 48
│  │  ├─ = : i32
│  │  │  ├─ num : i32
│  │  │  └─ / : i32
│  │  │     ├─ num : i32
│  │  │     └─ int 10
│  │  └─ += : i32
│  │     ├─ i : i32
│  │     └─ int 1
│  ├─ while
│  │  ├─ condition > : bool
│  │  │  ├─ i : i32
│  │  │  └─ int 0
│  │  ├─ -= : i32
│  │  │  ├─ i : i32
│  │  │  └─ int 1
│  │  └─ call push : void
│  │     ├─ s : STRUCT_CALL
│  │     └─ index : char
│  │        ├─ digits : char[]
│  │        └─ i : i32
│  ├─ clean : void
│  │  └─ digits : char[]
│  └─ return
│     └─ s : STRUCT_CALL
├─ fn String.drop(self : STRUCT_CALL) : void
│  └─ clean : void
│     └─ .value : char[]
│        └─ self : STRUCT_CALL
├─ fn String.cap(self : STRUCT_CALL) : i32
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ .value : char[]
│  │  │  │  └─ self : STRUCT_CALL
│  │  │  └─ NULL_LIT : char[]
│  │  └─ return
│  │     └─ int 0
│  └─ return
│     └─ cast : i32
│        └─ .len : u64
│           └─ .value : char[]
│              └─ self : STRUCT_CALL
├─ fn String.grow(self : STRUCT_CALL, want : i32) : void
│  ├─ = : i32
│  │  ├─ room : i32
│  │  └─ call cap : i32
│  │     └─ self : STRUCT_CALL
│  ├─ if
│  │  ├─ condition >= : bool
│  │  │  ├─ room : i32
│  │  │  └─ want : i32
│  │  └─ return
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ room : i32
│  │  │  └─ int 0
│  │  └─ = : i32
│  │     ├─ room : i32
│  │     └─ int 16
│  ├─ while
│  │  ├─ condition < : bool
│  │  │  ├─ room : i32
│  │  │  └─ want : i32
│  │  └─ *= : i32
│  │     ├─ room : i32
│  │     └─ int 2
│  ├─ = : array
│  │  ├─ bigger : char[]
│  │  └─ array : char[]
│  │     └─ room : i32
│  ├─ if
│  │  ├─ condition > : bool
│  │  │  ├─ .count : i32
│  │  │  │  └─ self : STRUCT_CALL
│  │  │  └─ int 0
│  │  └─ call memcpy : pointer
│  │     ├─ bigger : char[]
│  │     ├─ FALLBACK : char[]
│  │     │  ├─ .value : char[]
│  │     │  │  └─ self : STRUCT_CALL
│  │     │  └─ char[] ""
│  │     └─ .count : i32
│  │        └─ self : STRUCT_CALL
│  ├─ = : char
│  │  ├─ index : char
│  │  │  ├─ bigger : char[]
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  └─ cast : char
│  │     └─ int 0
│  ├─ clean : void
│  │  └─ .value : char[]
│  │     └─ self : STRUCT_CALL
│  └─ = : array
│     ├─ .value : char[]
│     │  └─ self : STRUCT_CALL
│     └─ bigger : char[]
├─ fn String.assign(self : STRUCT_CALL, str : array) : void
│  ├─ = : i32
│  │  ├─ n : i32
│  │  └─ int 0
│  ├─ if
│  │  ├─ condition != : bool
│  │  │  ├─ str : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  └─ = : i32
│  │     ├─ n : i32
│  │     └─ cast : i32
│  │        └─ call strlen : i64
│  │           └─ str : char[]
│  ├─ call grow : void
│  │  ├─ self : STRUCT_CALL
│  │  └─ + : i32
│  │     ├─ n : i32
│  │     └─ int 1
│  ├─ if
│  │  ├─ condition > : bool
│  │  │  ├─ n : i32
│  │  │  └─ int 0
│  │  └─ call memcpy : pointer
│  │     ├─ FALLBACK : char[]
│  │     │  ├─ .value : char[]
│  │     │  │  └─ self : STRUCT_CALL
│  │     │  └─ char[] ""
│  │     ├─ FALLBACK : char[]
│  │     │  ├─ str : char[]
│  │     │  └─ char[] ""
│  │     └─ n : i32
│  ├─ = : char
│  │  ├─ index : char
│  │  │  ├─ .value : char[]
│  │  │  │  └─ self : STRUCT_CALL
│  │  │  └─ n : i32
│  │  └─ cast : char
│  │     └─ int 0
│  └─ = : i32
│     ├─ .count : i32
│     │  └─ self : STRUCT_CALL
│     └─ n : i32
├─ fn String.join(self : STRUCT_CALL, str : array) : void
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ str : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  └─ return
│  ├─ = : i32
│  │  ├─ n : i32
│  │  └─ cast : i32
│  │     └─ call strlen : i64
│  │        └─ str : char[]
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ n : i32
│  │  │  └─ int 0
│  │  └─ return
│  ├─ call grow : void
│  │  ├─ self : STRUCT_CALL
│  │  └─ + : i32
│  │     ├─ + : i32
│  │     │  ├─ .count : i32
│  │     │  │  └─ self : STRUCT_CALL
│  │     │  └─ n : i32
│  │     └─ int 1
│  ├─ call memcpy : pointer
│  │  ├─ index : char[]
│  │  │  ├─ .value : char[]
│  │  │  │  └─ self : STRUCT_CALL
│  │  │  └─ range : i32
│  │  │     ├─ .count : i32
│  │  │     │  └─ self : STRUCT_CALL
│  │  │     └─ + : i32
│  │  │        ├─ .count : i32
│  │  │        │  └─ self : STRUCT_CALL
│  │  │        └─ n : i32
│  │  ├─ str : char[]
│  │  └─ n : i32
│  ├─ += : i32
│  │  ├─ .count : i32
│  │  │  └─ self : STRUCT_CALL
│  │  └─ n : i32
│  └─ = : char
│     ├─ index : char
│     │  ├─ .value : char[]
│     │  │  └─ self : STRUCT_CALL
│     │  └─ .count : i32
│     │     └─ self : STRUCT_CALL
│     └─ cast : char
│        └─ int 0
├─ fn String.push(self : STRUCT_CALL, c : char) : void
│  ├─ call grow : void
│  │  ├─ self : STRUCT_CALL
│  │  └─ + : i32
│  │     ├─ .count : i32
│  │     │  └─ self : STRUCT_CALL
│  │     └─ int 2
│  ├─ = : char
│  │  ├─ index : char
│  │  │  ├─ .value : char[]
│  │  │  │  └─ self : STRUCT_CALL
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  └─ c : char
│  ├─ += : i32
│  │  ├─ .count : i32
│  │  │  └─ self : STRUCT_CALL
│  │  └─ int 1
│  └─ = : char
│     ├─ index : char
│     │  ├─ .value : char[]
│     │  │  └─ self : STRUCT_CALL
│     │  └─ .count : i32
│     │     └─ self : STRUCT_CALL
│     └─ cast : char
│        └─ int 0
├─ fn String.pop(self : STRUCT_CALL) : char
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ .count : i32
│  │  │  │  └─ self : STRUCT_CALL
│  │  │  └─ int 0
│  │  └─ return
│  │     └─ cast : char
│  │        └─ int 0
│  ├─ -= : i32
│  │  ├─ .count : i32
│  │  │  └─ self : STRUCT_CALL
│  │  └─ int 1
│  ├─ = : char
│  │  ├─ c : char
│  │  └─ index : char
│  │     ├─ .value : char[]
│  │     │  └─ self : STRUCT_CALL
│  │     └─ .count : i32
│  │        └─ self : STRUCT_CALL
│  ├─ = : char
│  │  ├─ index : char
│  │  │  ├─ .value : char[]
│  │  │  │  └─ self : STRUCT_CALL
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  └─ cast : char
│  │     └─ int 0
│  └─ return
│     └─ c : char
├─ fn String.clear(self : STRUCT_CALL) : void
│  ├─ = : i32
│  │  ├─ .count : i32
│  │  │  └─ self : STRUCT_CALL
│  │  └─ int 0
│  └─ if
│     ├─ condition != : bool
│     │  ├─ .value : char[]
│     │  │  └─ self : STRUCT_CALL
│     │  └─ NULL_LIT : char[]
│     └─ = : char
│        ├─ index : char
│        │  ├─ .value : char[]
│        │  │  └─ self : STRUCT_CALL
│        │  └─ int 0
│        └─ cast : char
│           └─ int 0
├─ fn String.len(self : STRUCT_CALL) : i32
│  └─ return
│     └─ .count : i32
│        └─ self : STRUCT_CALL
├─ fn String.empty(self : STRUCT_CALL) : bool
│  └─ return
│     └─ == : bool
│        ├─ .count : i32
│        │  └─ self : STRUCT_CALL
│        └─ int 0
├─ fn String.c_str(self : STRUCT_CALL) : char[]
│  └─ return
│     └─ FALLBACK : char[]
│        ├─ .value : char[]
│        │  └─ self : STRUCT_CALL
│        └─ char[] ""
├─ fn String.at(self : STRUCT_CALL, i : i32) : char
│  ├─ if
│  │  ├─ condition or : bool
│  │  │  ├─ < : bool
│  │  │  │  ├─ i : i32
│  │  │  │  └─ int 0
│  │  │  └─ >= : bool
│  │  │     ├─ i : i32
│  │  │     └─ .count : i32
│  │  │        └─ self : STRUCT_CALL
│  │  └─ return
│  │     └─ cast : char
│  │        └─ int 0
│  └─ return
│     └─ index : char
│        ├─ .value : char[]
│        │  └─ self : STRUCT_CALL
│        └─ i : i32
├─ fn String.find(self : STRUCT_CALL, needle : array) : i32
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ needle : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  └─ return
│  │     └─ - : i32
│  │        ├─ int 0
│  │        └─ int 1
│  ├─ = : i32
│  │  ├─ n : i32
│  │  └─ cast : i32
│  │     └─ call strlen : i64
│  │        └─ needle : char[]
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ n : i32
│  │  │  └─ int 0
│  │  └─ return
│  │     └─ int 0
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ int 0
│  ├─ while
│  │  ├─ condition <= : bool
│  │  │  ├─ + : i32
│  │  │  │  ├─ i : i32
│  │  │  │  └─ n : i32
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  ├─ if
│  │  │  ├─ condition == : bool
│  │  │  │  ├─ call strncmp : i32
│  │  │  │  │  ├─ index : char[]
│  │  │  │  │  │  ├─ .value : char[]
│  │  │  │  │  │  │  └─ self : STRUCT_CALL
│  │  │  │  │  │  └─ range : i32
│  │  │  │  │  │     ├─ i : i32
│  │  │  │  │  │     └─ + : i32
│  │  │  │  │  │        ├─ i : i32
│  │  │  │  │  │        └─ n : i32
│  │  │  │  │  ├─ needle : char[]
│  │  │  │  │  └─ n : i32
│  │  │  │  └─ int 0
│  │  │  └─ return
│  │  │     └─ i : i32
│  │  └─ += : i32
│  │     ├─ i : i32
│  │     └─ int 1
│  └─ return
│     └─ - : i32
│        ├─ int 0
│        └─ int 1
├─ fn String.contains(self : STRUCT_CALL, needle : array) : bool
│  └─ return
│     └─ != : bool
│        ├─ call find : i32
│        │  ├─ self : STRUCT_CALL
│        │  └─ needle : char[]
│        └─ - : i32
│           ├─ int 0
│           └─ int 1
├─ fn String.starts_with(self : STRUCT_CALL, prefix : array) : bool
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ prefix : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  └─ return
│  │     └─ bool True
│  ├─ = : i32
│  │  ├─ n : i32
│  │  └─ cast : i32
│  │     └─ call strlen : i64
│  │        └─ prefix : char[]
│  ├─ if
│  │  ├─ condition > : bool
│  │  │  ├─ n : i32
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  └─ return
│  │     └─ bool False
│  └─ return
│     └─ == : bool
│        ├─ call strncmp : i32
│        │  ├─ index : char[]
│        │  │  ├─ .value : char[]
│        │  │  │  └─ self : STRUCT_CALL
│        │  │  └─ range : i32
│        │  │     ├─ int 0
│        │  │     └─ n : i32
│        │  ├─ prefix : char[]
│        │  └─ n : i32
│        └─ int 0
├─ fn String.ends_with(self : STRUCT_CALL, suffix : array) : bool
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ suffix : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  └─ return
│  │     └─ bool True
│  ├─ = : i32
│  │  ├─ n : i32
│  │  └─ cast : i32
│  │     └─ call strlen : i64
│  │        └─ suffix : char[]
│  ├─ if
│  │  ├─ condition > : bool
│  │  │  ├─ n : i32
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  └─ return
│  │     └─ bool False
│  └─ return
│     └─ == : bool
│        ├─ call strncmp : i32
│        │  ├─ index : char[]
│        │  │  ├─ .value : char[]
│        │  │  │  └─ self : STRUCT_CALL
│        │  │  └─ range : i32
│        │  │     ├─ - : i32
│        │  │     │  ├─ .count : i32
│        │  │     │  │  └─ self : STRUCT_CALL
│        │  │     │  └─ n : i32
│        │  │     └─ .count : i32
│        │  │        └─ self : STRUCT_CALL
│        │  ├─ suffix : char[]
│        │  └─ n : i32
│        └─ int 0
├─ fn String.substr(self : STRUCT_CALL, start : i32, length : i32) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ res : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  ├─ if
│  │  ├─ condition or : bool
│  │  │  ├─ < : bool
│  │  │  │  ├─ start : i32
│  │  │  │  └─ int 0
│  │  │  └─ >= : bool
│  │  │     ├─ start : i32
│  │  │     └─ .count : i32
│  │  │        └─ self : STRUCT_CALL
│  │  └─ return
│  │     └─ res : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ stop : i32
│  │  └─ + : i32
│  │     ├─ start : i32
│  │     └─ length : i32
│  ├─ if
│  │  ├─ condition > : bool
│  │  │  ├─ stop : i32
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  └─ = : i32
│  │     ├─ stop : i32
│  │     └─ .count : i32
│  │        └─ self : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ start : i32
│  ├─ while
│  │  ├─ condition < : bool
│  │  │  ├─ i : i32
│  │  │  └─ stop : i32
│  │  ├─ call push : void
│  │  │  ├─ res : STRUCT_CALL
│  │  │  └─ index : char
│  │  │     ├─ .value : char[]
│  │  │     │  └─ self : STRUCT_CALL
│  │  │     └─ i : i32
│  │  └─ += : i32
│  │     ├─ i : i32
│  │     └─ int 1
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn String.upper(self : STRUCT_CALL) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ res : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ int 0
│  ├─ while
│  │  ├─ condition < : bool
│  │  │  ├─ i : i32
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  ├─ = : char
│  │  │  ├─ c : char
│  │  │  └─ index : char
│  │  │     ├─ .value : char[]
│  │  │     │  └─ self : STRUCT_CALL
│  │  │     └─ i : i32
│  │  ├─ if
│  │  │  ├─ condition and : bool
│  │  │  │  ├─ >= : bool
│  │  │  │  │  ├─ c : char
│  │  │  │  │  └─ char 'a'
│  │  │  │  └─ <= : bool
│  │  │  │     ├─ c : char
│  │  │  │     └─ char 'z'
│  │  │  ├─ call push : void
│  │  │  │  ├─ res : STRUCT_CALL
│  │  │  │  └─ cast : char
│  │  │  │     └─ - : i32
│  │  │  │        ├─ cast : i32
│  │  │  │        │  └─ c : char
│  │  │  │        └─ int 32
│  │  │  └─ else
│  │  │     └─ call push : void
│  │  │        ├─ res : STRUCT_CALL
│  │  │        └─ c : char
│  │  └─ += : i32
│  │     ├─ i : i32
│  │     └─ int 1
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn String.lower(self : STRUCT_CALL) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ res : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ int 0
│  ├─ while
│  │  ├─ condition < : bool
│  │  │  ├─ i : i32
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  ├─ = : char
│  │  │  ├─ c : char
│  │  │  └─ index : char
│  │  │     ├─ .value : char[]
│  │  │     │  └─ self : STRUCT_CALL
│  │  │     └─ i : i32
│  │  ├─ if
│  │  │  ├─ condition and : bool
│  │  │  │  ├─ >= : bool
│  │  │  │  │  ├─ c : char
│  │  │  │  │  └─ char 'A'
│  │  │  │  └─ <= : bool
│  │  │  │     ├─ c : char
│  │  │  │     └─ char 'Z'
│  │  │  ├─ call push : void
│  │  │  │  ├─ res : STRUCT_CALL
│  │  │  │  └─ cast : char
│  │  │  │     └─ + : i32
│  │  │  │        ├─ cast : i32
│  │  │  │        │  └─ c : char
│  │  │  │        └─ int 32
│  │  │  └─ else
│  │  │     └─ call push : void
│  │  │        ├─ res : STRUCT_CALL
│  │  │        └─ c : char
│  │  └─ += : i32
│  │     ├─ i : i32
│  │     └─ int 1
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn String.trim(self : STRUCT_CALL) : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ start : i32
│  │  └─ int 0
│  ├─ while
│  │  ├─ condition < : bool
│  │  │  ├─ start : i32
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  ├─ = : char
│  │  │  ├─ c : char
│  │  │  └─ index : char
│  │  │     ├─ .value : char[]
│  │  │     │  └─ self : STRUCT_CALL
│  │  │     └─ start : i32
│  │  └─ if
│  │     ├─ condition or : bool
│  │     │  ├─ or : bool
│  │     │  │  ├─ or : bool
│  │     │  │  │  ├─ == : bool
│  │     │  │  │  │  ├─ c : char
│  │     │  │  │  │  └─ char ' '
│  │     │  │  │  └─ == : bool
│  │     │  │  │     ├─ c : char
│  │     │  │  │     └─ char '\t'
│  │     │  │  └─ == : bool
│  │     │  │     ├─ c : char
│  │     │  │     └─ char '\n'
│  │     │  └─ == : bool
│  │     │     ├─ c : char
│  │     │     └─ char '\r'
│  │     ├─ += : i32
│  │     │  ├─ start : i32
│  │     │  └─ int 1
│  │     └─ else
│  │        └─ break
│  ├─ = : i32
│  │  ├─ stop : i32
│  │  └─ .count : i32
│  │     └─ self : STRUCT_CALL
│  ├─ while
│  │  ├─ condition > : bool
│  │  │  ├─ stop : i32
│  │  │  └─ start : i32
│  │  ├─ = : char
│  │  │  ├─ c : char
│  │  │  └─ index : char
│  │  │     ├─ .value : char[]
│  │  │     │  └─ self : STRUCT_CALL
│  │  │     └─ - : i32
│  │  │        ├─ stop : i32
│  │  │        └─ int 1
│  │  └─ if
│  │     ├─ condition or : bool
│  │     │  ├─ or : bool
│  │     │  │  ├─ or : bool
│  │     │  │  │  ├─ == : bool
│  │     │  │  │  │  ├─ c : char
│  │     │  │  │  │  └─ char ' '
│  │     │  │  │  └─ == : bool
│  │     │  │  │     ├─ c : char
│  │     │  │  │     └─ char '\t'
│  │     │  │  └─ == : bool
│  │     │  │     ├─ c : char
│  │     │  │     └─ char '\n'
│  │     │  └─ == : bool
│  │     │     ├─ c : char
│  │     │     └─ char '\r'
│  │     ├─ -= : i32
│  │     │  ├─ stop : i32
│  │     │  └─ int 1
│  │     └─ else
│  │        └─ break
│  └─ return
│     └─ call substr : STRUCT_CALL
│        ├─ self : STRUCT_CALL
│        ├─ start : i32
│        └─ - : i32
│           ├─ stop : i32
│           └─ start : i32
├─ fn String.replace(self : STRUCT_CALL, old : array, fresh : array) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ res : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ old : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  ├─ call assign : void
│  │  │  ├─ res : STRUCT_CALL
│  │  │  └─ .value : char[]
│  │  │     └─ self : STRUCT_CALL
│  │  └─ return
│  │     └─ res : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ n : i32
│  │  └─ cast : i32
│  │     └─ call strlen : i64
│  │        └─ old : char[]
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ n : i32
│  │  │  └─ int 0
│  │  ├─ call assign : void
│  │  │  ├─ res : STRUCT_CALL
│  │  │  └─ .value : char[]
│  │  │     └─ self : STRUCT_CALL
│  │  └─ return
│  │     └─ res : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ int 0
│  ├─ while
│  │  ├─ condition < : bool
│  │  │  ├─ i : i32
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  ├─ = : bool
│  │  │  ├─ hit : bool
│  │  │  └─ bool False
│  │  ├─ if
│  │  │  ├─ condition <= : bool
│  │  │  │  ├─ + : i32
│  │  │  │  │  ├─ i : i32
│  │  │  │  │  └─ n : i32
│  │  │  │  └─ .count : i32
│  │  │  │     └─ self : STRUCT_CALL
│  │  │  └─ = : bool
│  │  │     ├─ hit : bool
│  │  │     └─ == : bool
│  │  │        ├─ call strncmp : i32
│  │  │        │  ├─ index : char[]
│  │  │        │  │  ├─ .value : char[]
│  │  │        │  │  │  └─ self : STRUCT_CALL
│  │  │        │  │  └─ range : i32
│  │  │        │  │     ├─ i : i32
│  │  │        │  │     └─ + : i32
│  │  │        │  │        ├─ i : i32
│  │  │        │  │        └─ n : i32
│  │  │        │  ├─ old : char[]
│  │  │        │  └─ n : i32
│  │  │        └─ int 0
│  │  └─ if
│  │     ├─ condition hit : bool
│  │     ├─ call join : void
│  │     │  ├─ res : STRUCT_CALL
│  │     │  └─ fresh : char[]
│  │     ├─ += : i32
│  │     │  ├─ i : i32
│  │     │  └─ n : i32
│  │     └─ else
│  │        ├─ call push : void
│  │        │  ├─ res : STRUCT_CALL
│  │        │  └─ index : char
│  │        │     ├─ .value : char[]
│  │        │     │  └─ self : STRUCT_CALL
│  │        │     └─ i : i32
│  │        └─ += : i32
│  │           ├─ i : i32
│  │           └─ int 1
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn String.repeat(self : STRUCT_CALL, n : i32) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ res : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ int 0
│  ├─ while
│  │  ├─ condition < : bool
│  │  │  ├─ i : i32
│  │  │  └─ n : i32
│  │  ├─ call join : void
│  │  │  ├─ res : STRUCT_CALL
│  │  │  └─ .value : char[]
│  │  │     └─ self : STRUCT_CALL
│  │  └─ += : i32
│  │     ├─ i : i32
│  │     └─ int 1
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn String.reverse(self : STRUCT_CALL) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ res : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ - : i32
│  │     ├─ .count : i32
│  │     │  └─ self : STRUCT_CALL
│  │     └─ int 1
│  ├─ while
│  │  ├─ condition >= : bool
│  │  │  ├─ i : i32
│  │  │  └─ int 0
│  │  ├─ call push : void
│  │  │  ├─ res : STRUCT_CALL
│  │  │  └─ index : char
│  │  │     ├─ .value : char[]
│  │  │     │  └─ self : STRUCT_CALL
│  │  │     └─ i : i32
│  │  └─ -= : i32
│  │     ├─ i : i32
│  │     └─ int 1
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn String.compare(self : STRUCT_CALL, other : STRUCT_CALL) : i32
│  └─ return
│     └─ call strcmp : i32
│        ├─ FALLBACK : char[]
│        │  ├─ .value : char[]
│        │  │  └─ self : STRUCT_CALL
│        │  └─ char[] ""
│        └─ FALLBACK : char[]
│           ├─ .value : char[]
│           │  └─ other : STRUCT_CALL
│           └─ char[] ""
├─ fn String.to_int(self : STRUCT_CALL) : i32
│  └─ return
│     └─ call atoi : i32
│        └─ FALLBACK : char[]
│           ├─ .value : char[]
│           │  └─ self : STRUCT_CALL
│           └─ char[] ""
├─ fn String.=.array(self : STRUCT_CALL, str : array) : void
│  └─ call assign : void
│     ├─ self : STRUCT_CALL
│     └─ str : char[]
├─ fn String.=.ref.String(self : STRUCT_CALL, v : STRUCT_CALL) : void
│  └─ call assign : void
│     ├─ self : STRUCT_CALL
│     └─ .value : char[]
│        └─ v : STRUCT_CALL
├─ fn String.+.ref.String(self : STRUCT_CALL, v : STRUCT_CALL) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ res : STRUCT_CALL
│  │  └─ call from : STRUCT_CALL
│  │     └─ .value : char[]
│  │        └─ self : STRUCT_CALL
│  ├─ call join : void
│  │  ├─ res : STRUCT_CALL
│  │  └─ .value : char[]
│  │     └─ v : STRUCT_CALL
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn String.+.array(self : STRUCT_CALL, str : array) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ res : STRUCT_CALL
│  │  └─ call from : STRUCT_CALL
│  │     └─ .value : char[]
│  │        └─ self : STRUCT_CALL
│  ├─ call join : void
│  │  ├─ res : STRUCT_CALL
│  │  └─ str : char[]
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn String.+=.ref.String(self : STRUCT_CALL, v : STRUCT_CALL) : void
│  └─ call join : void
│     ├─ self : STRUCT_CALL
│     └─ .value : char[]
│        └─ v : STRUCT_CALL
├─ fn String.+=.array(self : STRUCT_CALL, str : array) : void
│  └─ call join : void
│     ├─ self : STRUCT_CALL
│     └─ str : char[]
├─ fn String.==.ref.String(self : STRUCT_CALL, v : STRUCT_CALL) : bool
│  └─ return
│     └─ == : bool
│        ├─ call compare : i32
│        │  ├─ self : STRUCT_CALL
│        │  └─ ref : STRUCT_CALL
│        │     └─ v : STRUCT_CALL
│        └─ int 0
├─ fn String.!=.ref.String(self : STRUCT_CALL, v : STRUCT_CALL) : bool
│  └─ return
│     └─ != : bool
│        ├─ call compare : i32
│        │  ├─ self : STRUCT_CALL
│        │  └─ ref : STRUCT_CALL
│        │     └─ v : STRUCT_CALL
│        └─ int 0
├─ fn String.==.array(self : STRUCT_CALL, str : array) : bool
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ str : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  └─ return
│  │     └─ == : bool
│  │        ├─ .count : i32
│  │        │  └─ self : STRUCT_CALL
│  │        └─ int 0
│  └─ return
│     └─ == : bool
│        ├─ call strcmp : i32
│        │  ├─ FALLBACK : char[]
│        │  │  ├─ .value : char[]
│        │  │  │  └─ self : STRUCT_CALL
│        │  │  └─ char[] ""
│        │  └─ str : char[]
│        └─ int 0
├─ fn String.!=.array(self : STRUCT_CALL, str : array) : bool
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ str : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  └─ return
│  │     └─ != : bool
│  │        ├─ .count : i32
│  │        │  └─ self : STRUCT_CALL
│  │        └─ int 0
│  └─ return
│     └─ != : bool
│        ├─ call strcmp : i32
│        │  ├─ FALLBACK : char[]
│        │  │  ├─ .value : char[]
│        │  │  │  └─ self : STRUCT_CALL
│        │  │  └─ char[] ""
│        │  └─ str : char[]
│        └─ int 0
├─ fn String.<.ref.String(self : STRUCT_CALL, v : STRUCT_CALL) : bool
│  └─ return
│     └─ < : bool
│        ├─ call compare : i32
│        │  ├─ self : STRUCT_CALL
│        │  └─ ref : STRUCT_CALL
│        │     └─ v : STRUCT_CALL
│        └─ int 0
├─ fn String.>.ref.String(self : STRUCT_CALL, v : STRUCT_CALL) : bool
│  └─ return
│     └─ > : bool
│        ├─ call compare : i32
│        │  ├─ self : STRUCT_CALL
│        │  └─ ref : STRUCT_CALL
│        │     └─ v : STRUCT_CALL
│        └─ int 0
├─ fn String.<.array(self : STRUCT_CALL, str : array) : bool
│  └─ return
│     └─ < : bool
│        ├─ call strcmp : i32
│        │  ├─ FALLBACK : char[]
│        │  │  ├─ .value : char[]
│        │  │  │  └─ self : STRUCT_CALL
│        │  │  └─ char[] ""
│        │  └─ FALLBACK : char[]
│        │     ├─ str : char[]
│        │     └─ char[] ""
│        └─ int 0
├─ fn String.>.array(self : STRUCT_CALL, str : array) : bool
│  └─ return
│     └─ > : bool
│        ├─ call strcmp : i32
│        │  ├─ FALLBACK : char[]
│        │  │  ├─ .value : char[]
│        │  │  │  └─ self : STRUCT_CALL
│        │  │  └─ char[] ""
│        │  └─ FALLBACK : char[]
│        │     ├─ str : char[]
│        │     └─ char[] ""
│        └─ int 0
└─ fn String.output(self : STRUCT_CALL) : char[]
   └─ return
      └─ FALLBACK : char[]
         ├─ .value : char[]
         │  └─ self : STRUCT_CALL
         └─ char[] ""

struct Timespec
├─ tv_sec : i64
└─ tv_nsec : i64

struct Timeval
├─ tv_sec : i64
├─ tv_usec : i32
└─ __pad : i32

struct Tm
├─ tm_sec : i32
├─ tm_min : i32
├─ tm_hour : i32
├─ tm_mday : i32
├─ tm_mon : i32
├─ tm_year : i32
├─ tm_wday : i32
├─ tm_yday : i32
├─ tm_isdst : i32
├─ tm_gmtoff : i64
└─ tm_zone : pointer

proto fn time(timer : pointer) : i64

proto fn difftime(time1 : i64, time0 : i64) : f64

proto fn clock() : i64

proto fn gmtime(timer : i64) : pointer

proto fn localtime(timer : i64) : pointer

proto fn mktime(timeptr : pointer) : i64

proto fn gmtime_r(timer : i64, result : pointer) : pointer

proto fn localtime_r(timer : i64, result : pointer) : pointer

proto fn strftime(s : pointer, maxsize : i64, format : pointer, timeptr : pointer) : i64

proto fn asctime(timeptr : pointer) : pointer

proto fn ctime(timer : pointer) : pointer

proto fn asctime_r(timeptr : pointer, buf : pointer) : pointer

proto fn ctime_r(timer : pointer, buf : pointer) : pointer

proto fn clock_gettime(clk_id : i32, tp : pointer) : i32

proto fn clock_settime(clk_id : i32, tp : pointer) : i32

proto fn nanosleep(req : pointer, rem : pointer) : i32

proto fn gettimeofday(tv : pointer, tz : pointer) : i32

proto fn getpid() : i32

proto fn getppid() : i32

proto fn getuid() : i32

proto fn geteuid() : i32

proto fn getgid() : i32

proto fn getegid() : i32

proto fn setuid(uid : i32) : i32

proto fn setgid(gid : i32) : i32

proto fn fork() : i32

proto fn execv(path : pointer, argv : pointer) : i32

proto fn execve(path : pointer, argv : pointer, envp : pointer) : i32

proto fn execvp(file : pointer, argv : pointer) : i32

proto fn _exit(code : i32) : void

proto fn getcwd(buf : pointer, size : i64) : pointer

proto fn chdir(path : pointer) : i32

proto fn fchdir(fd : i32) : i32

proto fn access(path : pointer, mode : i32) : i32

proto fn unlink(path : pointer) : i32

proto fn rmdir(path : pointer) : i32

proto fn link(oldpath : pointer, newpath : pointer) : i32

proto fn symlink(target : pointer, linkpath : pointer) : i32

proto fn readlink(path : pointer, buf : pointer, bufsize : i64) : i64

proto fn rename(oldpath : pointer, newpath : pointer) : i32

proto fn truncate(path : pointer, length : i64) : i32

proto fn ftruncate(fd : i32, length : i64) : i32

proto fn close(fd : i32) : i32

proto fn dup(fd : i32) : i32

proto fn dup2(oldfd : i32, newfd : i32) : i32

proto fn pipe(pipefd : pointer) : i32

proto fn lseek(fd : i32, offset : i64, whence : i32) : i64

proto fn isatty(fd : i32) : i32

proto fn ttyname(fd : i32) : pointer

proto fn sleep(seconds : i32) : i32

proto fn usleep(microseconds : i32) : i32

proto fn gethostname(name : pointer, len : i64) : i32

proto fn sethostname(name : pointer, len : i64) : i32

proto fn alarm(seconds : i32) : i32

proto fn pause() : i32

proto fn sync() : void

proto fn fsync(fd : i32) : i32

proto fn fdatasync(fd : i32) : i32

= : i32
├─ F_OK : i32
└─ int 0

= : i32
├─ X_OK : i32
└─ int 1

= : i32
├─ W_OK : i32
└─ int 2

= : i32
├─ R_OK : i32
└─ int 4

= : i32
├─ SEEK_SET : i32
└─ int 0

= : i32
├─ SEEK_CUR : i32
└─ int 1

= : i32
├─ SEEK_END : i32
└─ int 2

= : i32
├─ STDIN_FILENO : i32
└─ int 0

= : i32
├─ STDOUT_FILENO : i32
└─ int 1

= : i32
├─ STDERR_FILENO : i32
└─ int 2

struct String2
├─ value : char[]
├─ count : i32
├─ size : i32
├─ fn String2.create() : STRUCT_CALL
│  ├─ s : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ .size : i32
│  │  │  └─ s : STRUCT_CALL
│  │  └─ int 10
│  ├─ = : array
│  │  ├─ .value : char[]
│  │  │  └─ s : STRUCT_CALL
│  │  └─ array : char[]
│  │     └─ int 10
│  └─ return
│     └─ s : STRUCT_CALL
├─ fn String2.drop(self : STRUCT_CALL) : void
│  └─ clean : void
│     └─ .value : char[]
│        └─ self : STRUCT_CALL
├─ fn String2.grow(self : STRUCT_CALL, want : i32) : void
│  ├─ if
│  │  ├─ condition >= : bool
│  │  │  ├─ .size : i32
│  │  │  │  └─ self : STRUCT_CALL
│  │  │  └─ want : i32
│  │  └─ return
│  ├─ while
│  │  ├─ condition < : bool
│  │  │  ├─ .size : i32
│  │  │  │  └─ self : STRUCT_CALL
│  │  │  └─ want : i32
│  │  └─ *= : i32
│  │     ├─ .size : i32
│  │     │  └─ self : STRUCT_CALL
│  │     └─ int 2
│  ├─ = : array
│  │  ├─ bigger : char[]
│  │  └─ array : char[]
│  │     └─ .size : i32
│  │        └─ self : STRUCT_CALL
│  ├─ call strcpy : pointer
│  │  ├─ bigger : char[]
│  │  └─ .value : char[]
│  │     └─ self : STRUCT_CALL
│  ├─ clean : void
│  │  └─ .value : char[]
│  │     └─ self : STRUCT_CALL
│  └─ = : array
│     ├─ .value : char[]
│     │  └─ self : STRUCT_CALL
│     └─ bigger : char[]
├─ fn String2.assign(self : STRUCT_CALL, str : array) : void
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ str : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  ├─ call assign : void
│  │  │  ├─ self : STRUCT_CALL
│  │  │  └─ char[] ""
│  │  └─ return
│  ├─ = : i32
│  │  ├─ len : i32
│  │  └─ cast : i32
│  │     └─ call strlen : i64
│  │        └─ str : char[]
│  ├─ call grow : void
│  │  ├─ self : STRUCT_CALL
│  │  └─ + : i32
│  │     ├─ len : i32
│  │     └─ int 1
│  ├─ call bzero : void
│  │  ├─ .value : char[]
│  │  │  └─ self : STRUCT_CALL
│  │  └─ cast : i64
│  │     └─ .size : i32
│  │        └─ self : STRUCT_CALL
│  ├─ call strcpy : pointer
│  │  ├─ .value : char[]
│  │  │  └─ self : STRUCT_CALL
│  │  └─ str : char[]
│  └─ = : i32
│     ├─ .count : i32
│     │  └─ self : STRUCT_CALL
│     └─ len : i32
├─ fn String2.join(self : STRUCT_CALL, str : array) : void
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ str : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  └─ return
│  ├─ = : i32
│  │  ├─ len : i32
│  │  └─ cast : i32
│  │     └─ call strlen : i64
│  │        └─ str : char[]
│  ├─ call grow : void
│  │  ├─ self : STRUCT_CALL
│  │  └─ + : i32
│  │     ├─ + : i32
│  │     │  ├─ .count : i32
│  │     │  │  └─ self : STRUCT_CALL
│  │     │  └─ len : i32
│  │     └─ int 1
│  ├─ call strcat : pointer
│  │  ├─ .value : char[]
│  │  │  └─ self : STRUCT_CALL
│  │  └─ str : char[]
│  └─ += : i32
│     ├─ .count : i32
│     │  └─ self : STRUCT_CALL
│     └─ len : i32
├─ fn String2.=.array(self : STRUCT_CALL, str : array) : void
│  └─ call assign : void
│     ├─ self : STRUCT_CALL
│     └─ str : char[]
└─ fn String2.=.ref.String2(self : STRUCT_CALL, v : STRUCT_CALL) : void
   └─ call assign : void
      ├─ self : STRUCT_CALL
      └─ .value : char[]
         └─ v : STRUCT_CALL

fn add(v : STRUCT_CALL, w : STRUCT_CALL) : STRUCT_CALL
├─ = : STRUCT_CALL
│  ├─ res : STRUCT_CALL
│  └─ call create : STRUCT_CALL
├─ = : void
│  ├─ res : STRUCT_CALL
│  └─ v : STRUCT_CALL
├─ call join : void
│  ├─ res : STRUCT_CALL
│  └─ .value : char[]
│     └─ w : STRUCT_CALL
└─ return
   └─ res : STRUCT_CALL

fn main() : i32
├─ = : STRUCT_CALL
│  ├─ s0 : STRUCT_CALL
│  └─ call create : STRUCT_CALL
├─ = : void
│  ├─ s0 : STRUCT_CALL
│  └─ char[] "1x2345"
├─ = : STRUCT_CALL
│  ├─ s1 : STRUCT_CALL
│  └─ call create : STRUCT_CALL
├─ = : void
│  ├─ s1 : STRUCT_CALL
│  └─ char[] "6x789"
├─ = : STRUCT_CALL
│  ├─ s2 : STRUCT_CALL
│  └─ call create : STRUCT_CALL
├─ = : void
│  ├─ s2 : STRUCT_CALL
│  └─ call add : STRUCT_CALL
│     ├─ ref : STRUCT_CALL
│     │  └─ s0 : STRUCT_CALL
│     └─ ref : STRUCT_CALL
│        └─ s1 : STRUCT_CALL
├─ output : void
│  ├─ char[] "s0:    "
│  ├─ .value : char[]
│  │  └─ s0 : STRUCT_CALL
│  ├─ char[] " ("
│  ├─ .count : i32
│  │  └─ s0 : STRUCT_CALL
│  └─ char[] ")\n"
├─ output : void
│  ├─ char[] "s1:    "
│  ├─ .value : char[]
│  │  └─ s1 : STRUCT_CALL
│  ├─ char[] " ("
│  ├─ .count : i32
│  │  └─ s1 : STRUCT_CALL
│  └─ char[] ")\n"
└─ output : void
   ├─ char[] "s0+s1: "
   ├─ .value : char[]
   │  └─ s2 : STRUCT_CALL
   ├─ char[] " ("
   ├─ .count : i32
   │  └─ s2 : STRUCT_CALL
   └─ char[] ")\n"
```

```out
s0:    1x2345 (6)
s1:    6x789 (5)
s0+s1: 1x23456x789 (11)
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%String = type { { i8*, i64 }, i32 }
%String2 = type { { i8*, i64 }, i32, i32 }

@os = internal global %Os zeroinitializer
@EPERM = internal global i32 0
@ENOENT = internal global i32 0
@ESRCH = internal global i32 0
@EINTR = internal global i32 0
@EIO = internal global i32 0
@ENXIO = internal global i32 0
@E2BIG = internal global i32 0
@ENOEXEC = internal global i32 0
@EBADF = internal global i32 0
@ECHILD = internal global i32 0
@EACCES = internal global i32 0
@EFAULT = internal global i32 0
@ENOTBLK = internal global i32 0
@EBUSY = internal global i32 0
@EEXIST = internal global i32 0
@EXDEV = internal global i32 0
@ENODEV = internal global i32 0
@ENOTDIR = internal global i32 0
@EISDIR = internal global i32 0
@EINVAL = internal global i32 0
@ENFILE = internal global i32 0
@EMFILE = internal global i32 0
@ENOTTY = internal global i32 0
@ETXTBSY = internal global i32 0
@EFBIG = internal global i32 0
@ENOSPC = internal global i32 0
@ESPIPE = internal global i32 0
@EROFS = internal global i32 0
@EMLINK = internal global i32 0
@EPIPE = internal global i32 0
@EDOM = internal global i32 0
@ERANGE = internal global i32 0
@O_RDONLY = internal global i32 0
@O_WRONLY = internal global i32 0
@O_RDWR = internal global i32 0
@FD_CLOEXEC = internal global i32 0
@IPPROTO_IP = internal global i32 0
@IPPROTO_TCP = internal global i32 0
@IPPROTO_UDP = internal global i32 0
@IPPROTO_IPV6 = internal global i32 0
@SHUT_RD = internal global i32 0
@SHUT_WR = internal global i32 0
@SHUT_RDWR = internal global i32 0
@POLLIN = internal global i32 0
@POLLPRI = internal global i32 0
@POLLOUT = internal global i32 0
@POLLERR = internal global i32 0
@POLLHUP = internal global i32 0
@POLLNVAL = internal global i32 0
@INADDR_ANY = internal global i32 0
@INADDR_LOOPBACK = internal global i32 0
@INADDR_BROADCAST = internal global i32 0
@SIGHUP = internal global i32 0
@SIGINT = internal global i32 0
@SIGQUIT = internal global i32 0
@SIGILL = internal global i32 0
@SIGTRAP = internal global i32 0
@SIGABRT = internal global i32 0
@SIGFPE = internal global i32 0
@SIGKILL = internal global i32 0
@SIGSEGV = internal global i32 0
@SIGPIPE = internal global i32 0
@SIGALRM = internal global i32 0
@SIGTERM = internal global i32 0
@S_IFMT = internal global i32 0
@S_IFSOCK = internal global i32 0
@S_IFLNK = internal global i32 0
@S_IFREG = internal global i32 0
@S_IFBLK = internal global i32 0
@S_IFDIR = internal global i32 0
@S_IFCHR = internal global i32 0
@S_IFIFO = internal global i32 0
@S_ISUID = internal global i32 0
@S_ISGID = internal global i32 0
@S_ISVTX = internal global i32 0
@F_OK = internal global i32 0
@X_OK = internal global i32 0
@W_OK = internal global i32 0
@R_OK = internal global i32 0
@SEEK_SET = internal global i32 0
@SEEK_CUR = internal global i32 0
@SEEK_END = internal global i32 0
@STDIN_FILENO = internal global i32 0
@STDOUT_FILENO = internal global i32 0
@STDERR_FILENO = internal global i32 0
@trap_msg = private unnamed_addr constant [153 x i8] c"runtime error: Modulo by zero\0A   ura-lib/string.ura:97:30\0A   |\0A97 |             digits[i] = (num % 10 + 48) as char\0A   |                              ^\0A\00", align 1
@trap_msg.1 = private unnamed_addr constant [127 x i8] c"runtime error: Division by zero\0A   ura-lib/string.ura:98:23\0A   |\0A98 |             num = num / 10\0A   |                       ^\0A\00", align 1
@str = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.2 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.3 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.4 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.5 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.6 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.7 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.8 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.9 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.10 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.11 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.12 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.13 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.14 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.15 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.16 = private unnamed_addr constant [7 x i8] c"1x2345\00", align 1
@str.17 = private unnamed_addr constant [6 x i8] c"6x789\00", align 1
@str.18 = private unnamed_addr constant [8 x i8] c"s0:    \00", align 1
@str.19 = private unnamed_addr constant [3 x i8] c" (\00", align 1
@str.20 = private unnamed_addr constant [3 x i8] c")\0A\00", align 1
@fmt = private unnamed_addr constant [19 x i8] c"%.*s%.*s%.*s%d%.*s\00", align 1
@str.21 = private unnamed_addr constant [8 x i8] c"s1:    \00", align 1
@str.22 = private unnamed_addr constant [3 x i8] c" (\00", align 1
@str.23 = private unnamed_addr constant [3 x i8] c")\0A\00", align 1
@fmt.24 = private unnamed_addr constant [19 x i8] c"%.*s%.*s%.*s%d%.*s\00", align 1
@str.25 = private unnamed_addr constant [8 x i8] c"s0+s1: \00", align 1
@str.26 = private unnamed_addr constant [3 x i8] c" (\00", align 1
@str.27 = private unnamed_addr constant [3 x i8] c")\0A\00", align 1
@fmt.28 = private unnamed_addr constant [19 x i8] c"%.*s%.*s%.*s%d%.*s\00", align 1

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

define %String @String.create() {
entry:
  %s = alloca %String, align 8
  store %String zeroinitializer, %String* %s, align 8
  %value = getelementptr %String, %String* %s, i32 0, i32 0
  %heap = call i8* @calloc(i64 16, i64 1)
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %heap, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 16, 1
  store { i8*, i64 } %arr.len, { i8*, i64 }* %value, align 8
  %count = getelementptr %String, %String* %s, i32 0, i32 1
  store i32 0, i32* %count, align 4
  %s1 = load %String, %String* %s, align 8
  ret %String %s1
}

declare i8* @calloc(i64, i64)

define %String @String.from({ i8*, i64 } %0) {
entry:
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %0, { i8*, i64 }* %str, align 8
  %s = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %s, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  call void @String.assign(%String* %s, { i8*, i64 } %str1)
  %s2 = load %String, %String* %s, align 8
  ret %String %s2
}

define void @String.assign(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %n = alloca i32, align 4
  store i32 0, i32* %n, align 4
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr = extractvalue { i8*, i64 } %str1, 0
  %nullcmp = icmp ne i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %then, %entry
  %ref = load %String*, %String** %self, align 8
  %n3 = load i32, i32* %n, align 4
  %add = add i32 %n3, 1
  call void @String.grow(%String* %ref, i32 %add)
  %n6 = load i32, i32* %n, align 4
  %gt = icmp sgt i32 %n6, 0
  br i1 %gt, label %then5, label %endif4

then:                                             ; preds = %entry
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data = extractvalue { i8*, i64 } %str2, 0
  %call = call i64 @strlen(i8* %arr.data)
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  br label %endif

endif4:                                           ; preds = %then5, %endif
  %ref18 = load %String*, %String** %self, align 8
  %value19 = getelementptr %String, %String* %ref18, i32 0, i32 0
  %value20 = load { i8*, i64 }, { i8*, i64 }* %value19, align 8
  %arr.data21 = extractvalue { i8*, i64 } %value20, 0
  %n22 = load i32, i32* %n, align 4
  %arr.at = getelementptr i8, i8* %arr.data21, i32 %n22
  store i8 0, i8* %arr.at, align 1
  %ref23 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref23, i32 0, i32 1
  %n24 = load i32, i32* %n, align 4
  store i32 %n24, i32* %count, align 4
  ret void

then5:                                            ; preds = %endif
  %ref7 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref7, i32 0, i32 0
  %value8 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr9 = extractvalue { i8*, i64 } %value8, 0
  %isnull = icmp eq i8* %opt.ptr9, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.2, i32 0, i32 0), i64 0 }, { i8*, i64 } %value8
  %arr.data10 = extractvalue { i8*, i64 } %fallback, 0
  %str11 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr12 = extractvalue { i8*, i64 } %str11, 0
  %isnull13 = icmp eq i8* %opt.ptr12, null
  %fallback14 = select i1 %isnull13, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.3, i32 0, i32 0), i64 0 }, { i8*, i64 } %str11
  %arr.data15 = extractvalue { i8*, i64 } %fallback14, 0
  %n16 = load i32, i32* %n, align 4
  %call17 = call i8* @memcpy(i8* %arr.data10, i8* %arr.data15, i32 %n16)
  br label %endif4
}

define %String @String.from_int(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %s = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %s, align 8
  %n1 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n1, 0
  br i1 %eq, label %then, label %endif

endif:                                            ; preds = %entry
  %num = alloca i32, align 4
  %n3 = load i32, i32* %n, align 4
  store i32 %n3, i32* %num, align 4
  %num6 = load i32, i32* %num, align 4
  %lt = icmp slt i32 %num6, 0
  br i1 %lt, label %then5, label %endif4

then:                                             ; preds = %entry
  call void @String.push(%String* %s, i8 48)
  %s2 = load %String, %String* %s, align 8
  ret %String %s2

endif4:                                           ; preds = %then5, %endif
  %digits = alloca { i8*, i64 }, align 8
  %heap = call i8* @calloc(i64 12, i64 1)
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %heap, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 12, 1
  store { i8*, i64 } %arr.len, { i8*, i64 }* %digits, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then5:                                            ; preds = %endif
  call void @String.push(%String* %s, i8 45)
  %num7 = load i32, i32* %num, align 4
  %sub = sub i32 0, %num7
  store i32 %sub, i32* %num, align 4
  br label %endif4

while.cond:                                       ; preds = %cont14, %endif4
  %num8 = load i32, i32* %num, align 4
  %gt = icmp sgt i32 %num8, 0
  br i1 %gt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %digits9 = load { i8*, i64 }, { i8*, i64 }* %digits, align 8
  %arr.data = extractvalue { i8*, i64 } %digits9, 0
  %i10 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %i10
  %num11 = load i32, i32* %num, align 4
  br i1 false, label %trap, label %cont

while.end:                                        ; preds = %while.cond
  br label %while.cond16

trap:                                             ; preds = %while.body
  %1 = call i64 @write(i32 2, i8* getelementptr inbounds ([153 x i8], [153 x i8]* @trap_msg, i32 0, i32 0), i64 152)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %while.body
  %mod = srem i32 %num11, 10
  %add = add i32 %mod, 48
  %cast = trunc i32 %add to i8
  store i8 %cast, i8* %arr.at, align 1
  %num12 = load i32, i32* %num, align 4
  br i1 false, label %trap13, label %cont14

trap13:                                           ; preds = %cont
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([127 x i8], [127 x i8]* @trap_msg.1, i32 0, i32 0), i64 126)
  call void @exit(i32 1)
  unreachable

cont14:                                           ; preds = %cont
  %div = sdiv i32 %num12, 10
  store i32 %div, i32* %num, align 4
  %cur = load i32, i32* %i, align 4
  %add15 = add i32 %cur, 1
  store i32 %add15, i32* %i, align 4
  br label %while.cond

while.cond16:                                     ; preds = %while.body17, %while.end
  %i19 = load i32, i32* %i, align 4
  %gt20 = icmp sgt i32 %i19, 0
  br i1 %gt20, label %while.body17, label %while.end18

while.body17:                                     ; preds = %while.cond16
  %cur21 = load i32, i32* %i, align 4
  %sub22 = sub i32 %cur21, 1
  store i32 %sub22, i32* %i, align 4
  %digits23 = load { i8*, i64 }, { i8*, i64 }* %digits, align 8
  %arr.data24 = extractvalue { i8*, i64 } %digits23, 0
  %i25 = load i32, i32* %i, align 4
  %arr.at26 = getelementptr i8, i8* %arr.data24, i32 %i25
  %idx = load i8, i8* %arr.at26, align 1
  call void @String.push(%String* %s, i8 %idx)
  br label %while.cond16

while.end18:                                      ; preds = %while.cond16
  %arr = load { i8*, i64 }, { i8*, i64 }* %digits, align 8
  %arr.data27 = extractvalue { i8*, i64 } %arr, 0
  call void @free(i8* %arr.data27)
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %digits, align 8
  %s28 = load %String, %String* %s, align 8
  ret %String %s28
}

define void @String.push(%String* %0, i8 %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %c = alloca i8, align 1
  store i8 %1, i8* %c, align 1
  %ref = load %String*, %String** %self, align 8
  %ref1 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref1, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %add = add i32 %count2, 2
  call void @String.grow(%String* %ref, i32 %add)
  %ref3 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref3, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value4, 0
  %ref5 = load %String*, %String** %self, align 8
  %count6 = getelementptr %String, %String* %ref5, i32 0, i32 1
  %count7 = load i32, i32* %count6, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %count7
  %c8 = load i8, i8* %c, align 1
  store i8 %c8, i8* %arr.at, align 1
  %ref9 = load %String*, %String** %self, align 8
  %count10 = getelementptr %String, %String* %ref9, i32 0, i32 1
  %cur = load i32, i32* %count10, align 4
  %add11 = add i32 %cur, 1
  store i32 %add11, i32* %count10, align 4
  %ref12 = load %String*, %String** %self, align 8
  %value13 = getelementptr %String, %String* %ref12, i32 0, i32 0
  %value14 = load { i8*, i64 }, { i8*, i64 }* %value13, align 8
  %arr.data15 = extractvalue { i8*, i64 } %value14, 0
  %ref16 = load %String*, %String** %self, align 8
  %count17 = getelementptr %String, %String* %ref16, i32 0, i32 1
  %count18 = load i32, i32* %count17, align 4
  %arr.at19 = getelementptr i8, i8* %arr.data15, i32 %count18
  store i8 0, i8* %arr.at19, align 1
  ret void
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)

declare void @free(i8*)

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %arr = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %arr, 0
  call void @free(i8* %arr.data)
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %value, align 8
  ret void
}

define i32 @String.cap(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %ref2 = load %String*, %String** %self, align 8
  %value3 = getelementptr %String, %String* %ref2, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value3, align 8
  %len = extractvalue { i8*, i64 } %value4, 1
  %cast = trunc i64 %len to i32
  ret i32 %cast

then:                                             ; preds = %entry
  ret i32 0
}

define void @String.grow(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %want = alloca i32, align 4
  store i32 %1, i32* %want, align 4
  %room = alloca i32, align 4
  %ref = load %String*, %String** %self, align 8
  %call = call i32 @String.cap(%String* %ref)
  store i32 %call, i32* %room, align 4
  %room1 = load i32, i32* %room, align 4
  %want2 = load i32, i32* %want, align 4
  %ge = icmp sge i32 %room1, %want2
  br i1 %ge, label %then, label %endif

endif:                                            ; preds = %entry
  %room5 = load i32, i32* %room, align 4
  %eq = icmp eq i32 %room5, 0
  br i1 %eq, label %then4, label %endif3

then:                                             ; preds = %entry
  ret void

endif3:                                           ; preds = %then4, %endif
  br label %while.cond

then4:                                            ; preds = %endif
  store i32 16, i32* %room, align 4
  br label %endif3

while.cond:                                       ; preds = %while.body, %endif3
  %room6 = load i32, i32* %room, align 4
  %want7 = load i32, i32* %want, align 4
  %lt = icmp slt i32 %room6, %want7
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %cur = load i32, i32* %room, align 4
  %mul = mul i32 %cur, 2
  store i32 %mul, i32* %room, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %bigger = alloca { i8*, i64 }, align 8
  %room8 = load i32, i32* %room, align 4
  %n = sext i32 %room8 to i64
  %heap = call i8* @calloc(i64 %n, i64 1)
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %heap, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %n, 1
  store { i8*, i64 } %arr.len, { i8*, i64 }* %bigger, align 8
  %ref11 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref11, i32 0, i32 1
  %count12 = load i32, i32* %count, align 4
  %gt = icmp sgt i32 %count12, 0
  br i1 %gt, label %then10, label %endif9

endif9:                                           ; preds = %then10, %while.end
  %bigger21 = load { i8*, i64 }, { i8*, i64 }* %bigger, align 8
  %arr.data22 = extractvalue { i8*, i64 } %bigger21, 0
  %ref23 = load %String*, %String** %self, align 8
  %count24 = getelementptr %String, %String* %ref23, i32 0, i32 1
  %count25 = load i32, i32* %count24, align 4
  %arr.at = getelementptr i8, i8* %arr.data22, i32 %count25
  store i8 0, i8* %arr.at, align 1
  %ref26 = load %String*, %String** %self, align 8
  %value27 = getelementptr %String, %String* %ref26, i32 0, i32 0
  %arr = load { i8*, i64 }, { i8*, i64 }* %value27, align 8
  %arr.data28 = extractvalue { i8*, i64 } %arr, 0
  call void @free(i8* %arr.data28)
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %value27, align 8
  %ref29 = load %String*, %String** %self, align 8
  %value30 = getelementptr %String, %String* %ref29, i32 0, i32 0
  %bigger31 = load { i8*, i64 }, { i8*, i64 }* %bigger, align 8
  store { i8*, i64 } %bigger31, { i8*, i64 }* %value30, align 8
  ret void

then10:                                           ; preds = %while.end
  %bigger13 = load { i8*, i64 }, { i8*, i64 }* %bigger, align 8
  %arr.data = extractvalue { i8*, i64 } %bigger13, 0
  %ref14 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref14, i32 0, i32 0
  %value15 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value15, 0
  %isnull = icmp eq i8* %opt.ptr, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str, i32 0, i32 0), i64 0 }, { i8*, i64 } %value15
  %arr.data16 = extractvalue { i8*, i64 } %fallback, 0
  %ref17 = load %String*, %String** %self, align 8
  %count18 = getelementptr %String, %String* %ref17, i32 0, i32 1
  %count19 = load i32, i32* %count18, align 4
  %call20 = call i8* @memcpy(i8* %arr.data, i8* %arr.data16, i32 %count19)
  br label %endif9
}

declare i8* @memcpy(i8*, i8*, i32)

declare i64 @strlen(i8*)

define void @String.join(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr = extractvalue { i8*, i64 } %str1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %n = alloca i32, align 4
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data = extractvalue { i8*, i64 } %str2, 0
  %call = call i64 @strlen(i8* %arr.data)
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n5, 0
  br i1 %eq, label %then4, label %endif3

then:                                             ; preds = %entry
  ret void

endif3:                                           ; preds = %endif
  %ref = load %String*, %String** %self, align 8
  %ref6 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref6, i32 0, i32 1
  %count7 = load i32, i32* %count, align 4
  %n8 = load i32, i32* %n, align 4
  %add = add i32 %count7, %n8
  %add9 = add i32 %add, 1
  call void @String.grow(%String* %ref, i32 %add9)
  %ref10 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref10, i32 0, i32 0
  %value11 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data12 = extractvalue { i8*, i64 } %value11, 0
  %ref13 = load %String*, %String** %self, align 8
  %count14 = getelementptr %String, %String* %ref13, i32 0, i32 1
  %count15 = load i32, i32* %count14, align 4
  %ref16 = load %String*, %String** %self, align 8
  %count17 = getelementptr %String, %String* %ref16, i32 0, i32 1
  %count18 = load i32, i32* %count17, align 4
  %n19 = load i32, i32* %n, align 4
  %add20 = add i32 %count18, %n19
  %start = sext i32 %count15 to i64
  %end = sext i32 %add20 to i64
  %slice.data = getelementptr i8, i8* %arr.data12, i64 %start
  %slice.len = sub i64 %end, %start
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %slice.data, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %slice.len, 1
  %arr.data21 = extractvalue { i8*, i64 } %arr.len, 0
  %str22 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data23 = extractvalue { i8*, i64 } %str22, 0
  %n24 = load i32, i32* %n, align 4
  %call25 = call i8* @memcpy(i8* %arr.data21, i8* %arr.data23, i32 %n24)
  %ref26 = load %String*, %String** %self, align 8
  %count27 = getelementptr %String, %String* %ref26, i32 0, i32 1
  %n28 = load i32, i32* %n, align 4
  %cur = load i32, i32* %count27, align 4
  %add29 = add i32 %cur, %n28
  store i32 %add29, i32* %count27, align 4
  %ref30 = load %String*, %String** %self, align 8
  %value31 = getelementptr %String, %String* %ref30, i32 0, i32 0
  %value32 = load { i8*, i64 }, { i8*, i64 }* %value31, align 8
  %arr.data33 = extractvalue { i8*, i64 } %value32, 0
  %ref34 = load %String*, %String** %self, align 8
  %count35 = getelementptr %String, %String* %ref34, i32 0, i32 1
  %count36 = load i32, i32* %count35, align 4
  %arr.at = getelementptr i8, i8* %arr.data33, i32 %count36
  store i8 0, i8* %arr.at, align 1
  ret void

then4:                                            ; preds = %endif
  ret void
}

define i8 @String.pop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count1 = load i32, i32* %count, align 4
  %eq = icmp eq i32 %count1, 0
  br i1 %eq, label %then, label %endif

endif:                                            ; preds = %entry
  %ref2 = load %String*, %String** %self, align 8
  %count3 = getelementptr %String, %String* %ref2, i32 0, i32 1
  %cur = load i32, i32* %count3, align 4
  %sub = sub i32 %cur, 1
  store i32 %sub, i32* %count3, align 4
  %c = alloca i8, align 1
  %ref4 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref4, i32 0, i32 0
  %value5 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value5, 0
  %ref6 = load %String*, %String** %self, align 8
  %count7 = getelementptr %String, %String* %ref6, i32 0, i32 1
  %count8 = load i32, i32* %count7, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %count8
  %idx = load i8, i8* %arr.at, align 1
  store i8 %idx, i8* %c, align 1
  %ref9 = load %String*, %String** %self, align 8
  %value10 = getelementptr %String, %String* %ref9, i32 0, i32 0
  %value11 = load { i8*, i64 }, { i8*, i64 }* %value10, align 8
  %arr.data12 = extractvalue { i8*, i64 } %value11, 0
  %ref13 = load %String*, %String** %self, align 8
  %count14 = getelementptr %String, %String* %ref13, i32 0, i32 1
  %count15 = load i32, i32* %count14, align 4
  %arr.at16 = getelementptr i8, i8* %arr.data12, i32 %count15
  store i8 0, i8* %arr.at16, align 1
  %c17 = load i8, i8* %c, align 1
  ret i8 %c17

then:                                             ; preds = %entry
  ret i8 0
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  store i32 0, i32* %count, align 4
  %ref1 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref1, i32 0, i32 0
  %value2 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value2, 0
  %nullcmp = icmp ne i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %then, %entry
  ret void

then:                                             ; preds = %entry
  %ref3 = load %String*, %String** %self, align 8
  %value4 = getelementptr %String, %String* %ref3, i32 0, i32 0
  %value5 = load { i8*, i64 }, { i8*, i64 }* %value4, align 8
  %arr.data = extractvalue { i8*, i64 } %value5, 0
  %arr.at = getelementptr i8, i8* %arr.data, i32 0
  store i8 0, i8* %arr.at, align 1
  br label %endif
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count1 = load i32, i32* %count, align 4
  ret i32 %count1
}

define i1 @String.empty(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count1 = load i32, i32* %count, align 4
  %eq = icmp eq i32 %count1, 0
  ret i1 %eq
}

define { i8*, i64 } @String.c_str(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value1, 0
  %isnull = icmp eq i8* %opt.ptr, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.4, i32 0, i32 0), i64 0 }, { i8*, i64 } %value1
  ret { i8*, i64 } %fallback
}

define i8 @String.at(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %i = alloca i32, align 4
  store i32 %1, i32* %i, align 4
  %i1 = load i32, i32* %i, align 4
  %lt = icmp slt i32 %i1, 0
  %i2 = load i32, i32* %i, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count3 = load i32, i32* %count, align 4
  %ge = icmp sge i32 %i2, %count3
  %or = or i1 %lt, %ge
  br i1 %or, label %then, label %endif

endif:                                            ; preds = %entry
  %ref4 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref4, i32 0, i32 0
  %value5 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value5, 0
  %i6 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %i6
  %idx = load i8, i8* %arr.at, align 1
  ret i8 %idx

then:                                             ; preds = %entry
  ret i8 0
}

define i32 @String.find(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %needle = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %needle, align 8
  %needle1 = load { i8*, i64 }, { i8*, i64 }* %needle, align 8
  %opt.ptr = extractvalue { i8*, i64 } %needle1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %n = alloca i32, align 4
  %needle2 = load { i8*, i64 }, { i8*, i64 }* %needle, align 8
  %arr.data = extractvalue { i8*, i64 } %needle2, 0
  %call = call i64 @strlen(i8* %arr.data)
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n5, 0
  br i1 %eq, label %then4, label %endif3

then:                                             ; preds = %entry
  ret i32 -1

endif3:                                           ; preds = %endif
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then4:                                            ; preds = %endif
  ret i32 0

while.cond:                                       ; preds = %endif9, %endif3
  %i6 = load i32, i32* %i, align 4
  %n7 = load i32, i32* %n, align 4
  %add = add i32 %i6, %n7
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count8 = load i32, i32* %count, align 4
  %le = icmp sle i32 %add, %count8
  br i1 %le, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref11 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref11, i32 0, i32 0
  %value12 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data13 = extractvalue { i8*, i64 } %value12, 0
  %i14 = load i32, i32* %i, align 4
  %i15 = load i32, i32* %i, align 4
  %n16 = load i32, i32* %n, align 4
  %add17 = add i32 %i15, %n16
  %start = sext i32 %i14 to i64
  %end = sext i32 %add17 to i64
  %slice.data = getelementptr i8, i8* %arr.data13, i64 %start
  %slice.len = sub i64 %end, %start
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %slice.data, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %slice.len, 1
  %arr.data18 = extractvalue { i8*, i64 } %arr.len, 0
  %needle19 = load { i8*, i64 }, { i8*, i64 }* %needle, align 8
  %arr.data20 = extractvalue { i8*, i64 } %needle19, 0
  %n21 = load i32, i32* %n, align 4
  %call22 = call i32 @strncmp(i8* %arr.data18, i8* %arr.data20, i32 %n21)
  %eq23 = icmp eq i32 %call22, 0
  br i1 %eq23, label %then10, label %endif9

while.end:                                        ; preds = %while.cond
  ret i32 -1

endif9:                                           ; preds = %while.body
  %cur = load i32, i32* %i, align 4
  %add25 = add i32 %cur, 1
  store i32 %add25, i32* %i, align 4
  br label %while.cond

then10:                                           ; preds = %while.body
  %i24 = load i32, i32* %i, align 4
  ret i32 %i24
}

declare i32 @strncmp(i8*, i8*, i32)

define i1 @String.contains(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %needle = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %needle, align 8
  %ref = load %String*, %String** %self, align 8
  %needle1 = load { i8*, i64 }, { i8*, i64 }* %needle, align 8
  %call = call i32 @String.find(%String* %ref, { i8*, i64 } %needle1)
  %ne = icmp ne i32 %call, -1
  ret i1 %ne
}

define i1 @String.starts_with(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %prefix = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %prefix, align 8
  %prefix1 = load { i8*, i64 }, { i8*, i64 }* %prefix, align 8
  %opt.ptr = extractvalue { i8*, i64 } %prefix1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %n = alloca i32, align 4
  %prefix2 = load { i8*, i64 }, { i8*, i64 }* %prefix, align 8
  %arr.data = extractvalue { i8*, i64 } %prefix2, 0
  %call = call i64 @strlen(i8* %arr.data)
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count6 = load i32, i32* %count, align 4
  %gt = icmp sgt i32 %n5, %count6
  br i1 %gt, label %then4, label %endif3

then:                                             ; preds = %entry
  ret i1 true

endif3:                                           ; preds = %endif
  %ref7 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref7, i32 0, i32 0
  %value8 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data9 = extractvalue { i8*, i64 } %value8, 0
  %n10 = load i32, i32* %n, align 4
  %end = sext i32 %n10 to i64
  %slice.data = getelementptr i8, i8* %arr.data9, i64 0
  %slice.len = sub i64 %end, 0
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %slice.data, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %slice.len, 1
  %arr.data11 = extractvalue { i8*, i64 } %arr.len, 0
  %prefix12 = load { i8*, i64 }, { i8*, i64 }* %prefix, align 8
  %arr.data13 = extractvalue { i8*, i64 } %prefix12, 0
  %n14 = load i32, i32* %n, align 4
  %call15 = call i32 @strncmp(i8* %arr.data11, i8* %arr.data13, i32 %n14)
  %eq = icmp eq i32 %call15, 0
  ret i1 %eq

then4:                                            ; preds = %endif
  ret i1 false
}

define i1 @String.ends_with(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %suffix = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %suffix, align 8
  %suffix1 = load { i8*, i64 }, { i8*, i64 }* %suffix, align 8
  %opt.ptr = extractvalue { i8*, i64 } %suffix1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %n = alloca i32, align 4
  %suffix2 = load { i8*, i64 }, { i8*, i64 }* %suffix, align 8
  %arr.data = extractvalue { i8*, i64 } %suffix2, 0
  %call = call i64 @strlen(i8* %arr.data)
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count6 = load i32, i32* %count, align 4
  %gt = icmp sgt i32 %n5, %count6
  br i1 %gt, label %then4, label %endif3

then:                                             ; preds = %entry
  ret i1 true

endif3:                                           ; preds = %endif
  %ref7 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref7, i32 0, i32 0
  %value8 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data9 = extractvalue { i8*, i64 } %value8, 0
  %ref10 = load %String*, %String** %self, align 8
  %count11 = getelementptr %String, %String* %ref10, i32 0, i32 1
  %count12 = load i32, i32* %count11, align 4
  %n13 = load i32, i32* %n, align 4
  %sub = sub i32 %count12, %n13
  %ref14 = load %String*, %String** %self, align 8
  %count15 = getelementptr %String, %String* %ref14, i32 0, i32 1
  %count16 = load i32, i32* %count15, align 4
  %start = sext i32 %sub to i64
  %end = sext i32 %count16 to i64
  %slice.data = getelementptr i8, i8* %arr.data9, i64 %start
  %slice.len = sub i64 %end, %start
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %slice.data, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %slice.len, 1
  %arr.data17 = extractvalue { i8*, i64 } %arr.len, 0
  %suffix18 = load { i8*, i64 }, { i8*, i64 }* %suffix, align 8
  %arr.data19 = extractvalue { i8*, i64 } %suffix18, 0
  %n20 = load i32, i32* %n, align 4
  %call21 = call i32 @strncmp(i8* %arr.data17, i8* %arr.data19, i32 %n20)
  %eq = icmp eq i32 %call21, 0
  ret i1 %eq

then4:                                            ; preds = %endif
  ret i1 false
}

define %String @String.substr(%String* %0, i32 %1, i32 %2) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %start = alloca i32, align 4
  store i32 %1, i32* %start, align 4
  %length = alloca i32, align 4
  store i32 %2, i32* %length, align 4
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %start1 = load i32, i32* %start, align 4
  %lt = icmp slt i32 %start1, 0
  %start2 = load i32, i32* %start, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count3 = load i32, i32* %count, align 4
  %ge = icmp sge i32 %start2, %count3
  %or = or i1 %lt, %ge
  br i1 %or, label %then, label %endif

endif:                                            ; preds = %entry
  %stop = alloca i32, align 4
  %start5 = load i32, i32* %start, align 4
  %length6 = load i32, i32* %length, align 4
  %add = add i32 %start5, %length6
  store i32 %add, i32* %stop, align 4
  %stop9 = load i32, i32* %stop, align 4
  %ref10 = load %String*, %String** %self, align 8
  %count11 = getelementptr %String, %String* %ref10, i32 0, i32 1
  %count12 = load i32, i32* %count11, align 4
  %gt = icmp sgt i32 %stop9, %count12
  br i1 %gt, label %then8, label %endif7

then:                                             ; preds = %entry
  %res4 = load %String, %String* %res, align 8
  ret %String %res4

endif7:                                           ; preds = %then8, %endif
  %i = alloca i32, align 4
  %start16 = load i32, i32* %start, align 4
  store i32 %start16, i32* %i, align 4
  br label %while.cond

then8:                                            ; preds = %endif
  %ref13 = load %String*, %String** %self, align 8
  %count14 = getelementptr %String, %String* %ref13, i32 0, i32 1
  %count15 = load i32, i32* %count14, align 4
  store i32 %count15, i32* %stop, align 4
  br label %endif7

while.cond:                                       ; preds = %while.body, %endif7
  %i17 = load i32, i32* %i, align 4
  %stop18 = load i32, i32* %stop, align 4
  %lt19 = icmp slt i32 %i17, %stop18
  br i1 %lt19, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref20 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref20, i32 0, i32 0
  %value21 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value21, 0
  %i22 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %i22
  %idx = load i8, i8* %arr.at, align 1
  call void @String.push(%String* %res, i8 %idx)
  %cur = load i32, i32* %i, align 4
  %add23 = add i32 %cur, 1
  store i32 %add23, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %res24 = load %String, %String* %res, align 8
  ret %String %res24
}

define %String @String.upper(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %endif, %entry
  %i1 = load i32, i32* %i, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %lt = icmp slt i32 %i1, %count2
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %c = alloca i8, align 1
  %ref3 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref3, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value4, 0
  %i5 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %i5
  %idx = load i8, i8* %arr.at, align 1
  store i8 %idx, i8* %c, align 1
  %c6 = load i8, i8* %c, align 1
  %ge = icmp sge i8 %c6, 97
  %c7 = load i8, i8* %c, align 1
  %le = icmp sle i8 %c7, 122
  %and = and i1 %ge, %le
  br i1 %and, label %then, label %next

while.end:                                        ; preds = %while.cond
  %res11 = load %String, %String* %res, align 8
  ret %String %res11

endif:                                            ; preds = %next, %then
  %cur = load i32, i32* %i, align 4
  %add = add i32 %cur, 1
  store i32 %add, i32* %i, align 4
  br label %while.cond

then:                                             ; preds = %while.body
  %c8 = load i8, i8* %c, align 1
  %cast = sext i8 %c8 to i32
  %sub = sub i32 %cast, 32
  %cast9 = trunc i32 %sub to i8
  call void @String.push(%String* %res, i8 %cast9)
  br label %endif

next:                                             ; preds = %while.body
  %c10 = load i8, i8* %c, align 1
  call void @String.push(%String* %res, i8 %c10)
  br label %endif
}

define %String @String.lower(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %endif, %entry
  %i1 = load i32, i32* %i, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %lt = icmp slt i32 %i1, %count2
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %c = alloca i8, align 1
  %ref3 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref3, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value4, 0
  %i5 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %i5
  %idx = load i8, i8* %arr.at, align 1
  store i8 %idx, i8* %c, align 1
  %c6 = load i8, i8* %c, align 1
  %ge = icmp sge i8 %c6, 65
  %c7 = load i8, i8* %c, align 1
  %le = icmp sle i8 %c7, 90
  %and = and i1 %ge, %le
  br i1 %and, label %then, label %next

while.end:                                        ; preds = %while.cond
  %res12 = load %String, %String* %res, align 8
  ret %String %res12

endif:                                            ; preds = %next, %then
  %cur = load i32, i32* %i, align 4
  %add11 = add i32 %cur, 1
  store i32 %add11, i32* %i, align 4
  br label %while.cond

then:                                             ; preds = %while.body
  %c8 = load i8, i8* %c, align 1
  %cast = sext i8 %c8 to i32
  %add = add i32 %cast, 32
  %cast9 = trunc i32 %add to i8
  call void @String.push(%String* %res, i8 %cast9)
  br label %endif

next:                                             ; preds = %while.body
  %c10 = load i8, i8* %c, align 1
  call void @String.push(%String* %res, i8 %c10)
  br label %endif
}

define %String @String.trim(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %start = alloca i32, align 4
  store i32 0, i32* %start, align 4
  br label %while.cond

while.cond:                                       ; preds = %endif, %entry
  %start1 = load i32, i32* %start, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %lt = icmp slt i32 %start1, %count2
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %c = alloca i8, align 1
  %ref3 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref3, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value4, 0
  %start5 = load i32, i32* %start, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %start5
  %idx = load i8, i8* %arr.at, align 1
  store i8 %idx, i8* %c, align 1
  %c6 = load i8, i8* %c, align 1
  %eq = icmp eq i8 %c6, 32
  %c7 = load i8, i8* %c, align 1
  %eq8 = icmp eq i8 %c7, 9
  %or = or i1 %eq, %eq8
  %c9 = load i8, i8* %c, align 1
  %eq10 = icmp eq i8 %c9, 10
  %or11 = or i1 %or, %eq10
  %c12 = load i8, i8* %c, align 1
  %eq13 = icmp eq i8 %c12, 13
  %or14 = or i1 %or11, %eq13
  br i1 %or14, label %then, label %next

while.end:                                        ; preds = %next, %while.cond
  %stop = alloca i32, align 4
  %ref15 = load %String*, %String** %self, align 8
  %count16 = getelementptr %String, %String* %ref15, i32 0, i32 1
  %count17 = load i32, i32* %count16, align 4
  store i32 %count17, i32* %stop, align 4
  br label %while.cond18

endif:                                            ; preds = %then
  br label %while.cond

then:                                             ; preds = %while.body
  %cur = load i32, i32* %start, align 4
  %add = add i32 %cur, 1
  store i32 %add, i32* %start, align 4
  br label %endif

next:                                             ; preds = %while.body
  br label %while.end

while.cond18:                                     ; preds = %endif31, %while.end
  %stop21 = load i32, i32* %stop, align 4
  %start22 = load i32, i32* %start, align 4
  %gt = icmp sgt i32 %stop21, %start22
  br i1 %gt, label %while.body19, label %while.end20

while.body19:                                     ; preds = %while.cond18
  %c23 = alloca i8, align 1
  %ref24 = load %String*, %String** %self, align 8
  %value25 = getelementptr %String, %String* %ref24, i32 0, i32 0
  %value26 = load { i8*, i64 }, { i8*, i64 }* %value25, align 8
  %arr.data27 = extractvalue { i8*, i64 } %value26, 0
  %stop28 = load i32, i32* %stop, align 4
  %sub = sub i32 %stop28, 1
  %arr.at29 = getelementptr i8, i8* %arr.data27, i32 %sub
  %idx30 = load i8, i8* %arr.at29, align 1
  store i8 %idx30, i8* %c23, align 1
  %c34 = load i8, i8* %c23, align 1
  %eq35 = icmp eq i8 %c34, 32
  %c36 = load i8, i8* %c23, align 1
  %eq37 = icmp eq i8 %c36, 9
  %or38 = or i1 %eq35, %eq37
  %c39 = load i8, i8* %c23, align 1
  %eq40 = icmp eq i8 %c39, 10
  %or41 = or i1 %or38, %eq40
  %c42 = load i8, i8* %c23, align 1
  %eq43 = icmp eq i8 %c42, 13
  %or44 = or i1 %or41, %eq43
  br i1 %or44, label %then32, label %next33

while.end20:                                      ; preds = %next33, %while.cond18
  %ref47 = load %String*, %String** %self, align 8
  %start48 = load i32, i32* %start, align 4
  %stop49 = load i32, i32* %stop, align 4
  %start50 = load i32, i32* %start, align 4
  %sub51 = sub i32 %stop49, %start50
  %call = call %String @String.substr(%String* %ref47, i32 %start48, i32 %sub51)
  ret %String %call

endif31:                                          ; preds = %then32
  br label %while.cond18

then32:                                           ; preds = %while.body19
  %cur45 = load i32, i32* %stop, align 4
  %sub46 = sub i32 %cur45, 1
  store i32 %sub46, i32* %stop, align 4
  br label %endif31

next33:                                           ; preds = %while.body19
  br label %while.end20
}

define %String @String.replace(%String* %0, { i8*, i64 } %1, { i8*, i64 } %2) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %old = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %old, align 8
  %fresh = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %2, { i8*, i64 }* %fresh, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %old1 = load { i8*, i64 }, { i8*, i64 }* %old, align 8
  %opt.ptr = extractvalue { i8*, i64 } %old1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %n = alloca i32, align 4
  %old4 = load { i8*, i64 }, { i8*, i64 }* %old, align 8
  %arr.data = extractvalue { i8*, i64 } %old4, 0
  %call5 = call i64 @strlen(i8* %arr.data)
  %cast = trunc i64 %call5 to i32
  store i32 %cast, i32* %n, align 4
  %n8 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n8, 0
  br i1 %eq, label %then7, label %endif6

then:                                             ; preds = %entry
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value2 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  call void @String.assign(%String* %res, { i8*, i64 } %value2)
  %res3 = load %String, %String* %res, align 8
  ret %String %res3

endif6:                                           ; preds = %endif
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then7:                                            ; preds = %endif
  %ref9 = load %String*, %String** %self, align 8
  %value10 = getelementptr %String, %String* %ref9, i32 0, i32 0
  %value11 = load { i8*, i64 }, { i8*, i64 }* %value10, align 8
  call void @String.assign(%String* %res, { i8*, i64 } %value11)
  %res12 = load %String, %String* %res, align 8
  ret %String %res12

while.cond:                                       ; preds = %endif37, %endif6
  %i13 = load i32, i32* %i, align 4
  %ref14 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref14, i32 0, i32 1
  %count15 = load i32, i32* %count, align 4
  %lt = icmp slt i32 %i13, %count15
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %hit = alloca i1, align 1
  store i1 false, i1* %hit, align 1
  %i18 = load i32, i32* %i, align 4
  %n19 = load i32, i32* %n, align 4
  %add = add i32 %i18, %n19
  %ref20 = load %String*, %String** %self, align 8
  %count21 = getelementptr %String, %String* %ref20, i32 0, i32 1
  %count22 = load i32, i32* %count21, align 4
  %le = icmp sle i32 %add, %count22
  br i1 %le, label %then17, label %endif16

while.end:                                        ; preds = %while.cond
  %res50 = load %String, %String* %res, align 8
  ret %String %res50

endif16:                                          ; preds = %then17, %while.body
  %hit39 = load i1, i1* %hit, align 1
  br i1 %hit39, label %then38, label %next

then17:                                           ; preds = %while.body
  %ref23 = load %String*, %String** %self, align 8
  %value24 = getelementptr %String, %String* %ref23, i32 0, i32 0
  %value25 = load { i8*, i64 }, { i8*, i64 }* %value24, align 8
  %arr.data26 = extractvalue { i8*, i64 } %value25, 0
  %i27 = load i32, i32* %i, align 4
  %i28 = load i32, i32* %i, align 4
  %n29 = load i32, i32* %n, align 4
  %add30 = add i32 %i28, %n29
  %start = sext i32 %i27 to i64
  %end = sext i32 %add30 to i64
  %slice.data = getelementptr i8, i8* %arr.data26, i64 %start
  %slice.len = sub i64 %end, %start
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %slice.data, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %slice.len, 1
  %arr.data31 = extractvalue { i8*, i64 } %arr.len, 0
  %old32 = load { i8*, i64 }, { i8*, i64 }* %old, align 8
  %arr.data33 = extractvalue { i8*, i64 } %old32, 0
  %n34 = load i32, i32* %n, align 4
  %call35 = call i32 @strncmp(i8* %arr.data31, i8* %arr.data33, i32 %n34)
  %eq36 = icmp eq i32 %call35, 0
  store i1 %eq36, i1* %hit, align 1
  br label %endif16

endif37:                                          ; preds = %next, %then38
  br label %while.cond

then38:                                           ; preds = %endif16
  %fresh40 = load { i8*, i64 }, { i8*, i64 }* %fresh, align 8
  call void @String.join(%String* %res, { i8*, i64 } %fresh40)
  %n41 = load i32, i32* %n, align 4
  %cur = load i32, i32* %i, align 4
  %add42 = add i32 %cur, %n41
  store i32 %add42, i32* %i, align 4
  br label %endif37

next:                                             ; preds = %endif16
  %ref43 = load %String*, %String** %self, align 8
  %value44 = getelementptr %String, %String* %ref43, i32 0, i32 0
  %value45 = load { i8*, i64 }, { i8*, i64 }* %value44, align 8
  %arr.data46 = extractvalue { i8*, i64 } %value45, 0
  %i47 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data46, i32 %i47
  %idx = load i8, i8* %arr.at, align 1
  call void @String.push(%String* %res, i8 %idx)
  %cur48 = load i32, i32* %i, align 4
  %add49 = add i32 %cur48, 1
  store i32 %add49, i32* %i, align 4
  br label %endif37
}

define %String @String.repeat(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %i1 = load i32, i32* %i, align 4
  %n2 = load i32, i32* %n, align 4
  %lt = icmp slt i32 %i1, %n2
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value3 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  call void @String.join(%String* %res, { i8*, i64 } %value3)
  %cur = load i32, i32* %i, align 4
  %add = add i32 %cur, 1
  store i32 %add, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %res4 = load %String, %String* %res, align 8
  ret %String %res4
}

define %String @String.reverse(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count1 = load i32, i32* %count, align 4
  %sub = sub i32 %count1, 1
  store i32 %sub, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %i2 = load i32, i32* %i, align 4
  %ge = icmp sge i32 %i2, 0
  br i1 %ge, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref3 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref3, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value4, 0
  %i5 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %i5
  %idx = load i8, i8* %arr.at, align 1
  call void @String.push(%String* %res, i8 %idx)
  %cur = load i32, i32* %i, align 4
  %sub6 = sub i32 %cur, 1
  store i32 %sub6, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %res7 = load %String, %String* %res, align 8
  ret %String %res7
}

define i32 @String.compare(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %other = alloca %String*, align 8
  store %String* %1, %String** %other, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value1, 0
  %isnull = icmp eq i8* %opt.ptr, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.5, i32 0, i32 0), i64 0 }, { i8*, i64 } %value1
  %arr.data = extractvalue { i8*, i64 } %fallback, 0
  %ref2 = load %String*, %String** %other, align 8
  %value3 = getelementptr %String, %String* %ref2, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value3, align 8
  %opt.ptr5 = extractvalue { i8*, i64 } %value4, 0
  %isnull6 = icmp eq i8* %opt.ptr5, null
  %fallback7 = select i1 %isnull6, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.6, i32 0, i32 0), i64 0 }, { i8*, i64 } %value4
  %arr.data8 = extractvalue { i8*, i64 } %fallback7, 0
  %call = call i32 @strcmp(i8* %arr.data, i8* %arr.data8)
  ret i32 %call
}

declare i32 @strcmp(i8*, i8*)

define i32 @String.to_int(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value1, 0
  %isnull = icmp eq i8* %opt.ptr, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.7, i32 0, i32 0), i64 0 }, { i8*, i64 } %value1
  %arr.data = extractvalue { i8*, i64 } %fallback, 0
  %call = call i32 @atoi(i8* %arr.data)
  ret i32 %call
}

declare i32 @atoi(i8*)

define void @"String.=.array"(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %ref = load %String*, %String** %self, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  call void @String.assign(%String* %ref, { i8*, i64 } %str1)
  ret void
}

define void @"String.=.ref.String"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %v = alloca %String*, align 8
  store %String* %1, %String** %v, align 8
  %ref = load %String*, %String** %self, align 8
  %ref1 = load %String*, %String** %v, align 8
  %value = getelementptr %String, %String* %ref1, i32 0, i32 0
  %value2 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  call void @String.assign(%String* %ref, { i8*, i64 } %value2)
  ret void
}

define %String @"String.+.ref.String"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %v = alloca %String*, align 8
  store %String* %1, %String** %v, align 8
  %res = alloca %String, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %call = call %String @String.from({ i8*, i64 } %value1)
  store %String %call, %String* %res, align 8
  %ref2 = load %String*, %String** %v, align 8
  %value3 = getelementptr %String, %String* %ref2, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value3, align 8
  call void @String.join(%String* %res, { i8*, i64 } %value4)
  %res5 = load %String, %String* %res, align 8
  ret %String %res5
}

define %String @"String.+.array"(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %res = alloca %String, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %call = call %String @String.from({ i8*, i64 } %value1)
  store %String %call, %String* %res, align 8
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  call void @String.join(%String* %res, { i8*, i64 } %str2)
  %res3 = load %String, %String* %res, align 8
  ret %String %res3
}

define void @"String.+=.ref.String"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %v = alloca %String*, align 8
  store %String* %1, %String** %v, align 8
  %ref = load %String*, %String** %self, align 8
  %ref1 = load %String*, %String** %v, align 8
  %value = getelementptr %String, %String* %ref1, i32 0, i32 0
  %value2 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  call void @String.join(%String* %ref, { i8*, i64 } %value2)
  ret void
}

define void @"String.+=.array"(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %ref = load %String*, %String** %self, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  call void @String.join(%String* %ref, { i8*, i64 } %str1)
  ret void
}

define i1 @"String.==.ref.String"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %v = alloca %String*, align 8
  store %String* %1, %String** %v, align 8
  %ref = load %String*, %String** %self, align 8
  %ref1 = load %String*, %String** %v, align 8
  %call = call i32 @String.compare(%String* %ref, %String* %ref1)
  %eq = icmp eq i32 %call, 0
  ret i1 %eq
}

define i1 @"String.!=.ref.String"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %v = alloca %String*, align 8
  store %String* %1, %String** %v, align 8
  %ref = load %String*, %String** %self, align 8
  %ref1 = load %String*, %String** %v, align 8
  %call = call i32 @String.compare(%String* %ref, %String* %ref1)
  %ne = icmp ne i32 %call, 0
  ret i1 %ne
}

define i1 @"String.==.array"(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr = extractvalue { i8*, i64 } %str1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %ref3 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref3, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr5 = extractvalue { i8*, i64 } %value4, 0
  %isnull = icmp eq i8* %opt.ptr5, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.8, i32 0, i32 0), i64 0 }, { i8*, i64 } %value4
  %arr.data = extractvalue { i8*, i64 } %fallback, 0
  %str6 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data7 = extractvalue { i8*, i64 } %str6, 0
  %call = call i32 @strcmp(i8* %arr.data, i8* %arr.data7)
  %eq8 = icmp eq i32 %call, 0
  ret i1 %eq8

then:                                             ; preds = %entry
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %eq = icmp eq i32 %count2, 0
  ret i1 %eq
}

define i1 @"String.!=.array"(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr = extractvalue { i8*, i64 } %str1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %ref3 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref3, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr5 = extractvalue { i8*, i64 } %value4, 0
  %isnull = icmp eq i8* %opt.ptr5, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.9, i32 0, i32 0), i64 0 }, { i8*, i64 } %value4
  %arr.data = extractvalue { i8*, i64 } %fallback, 0
  %str6 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data7 = extractvalue { i8*, i64 } %str6, 0
  %call = call i32 @strcmp(i8* %arr.data, i8* %arr.data7)
  %ne8 = icmp ne i32 %call, 0
  ret i1 %ne8

then:                                             ; preds = %entry
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %ne = icmp ne i32 %count2, 0
  ret i1 %ne
}

define i1 @"String.<.ref.String"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %v = alloca %String*, align 8
  store %String* %1, %String** %v, align 8
  %ref = load %String*, %String** %self, align 8
  %ref1 = load %String*, %String** %v, align 8
  %call = call i32 @String.compare(%String* %ref, %String* %ref1)
  %lt = icmp slt i32 %call, 0
  ret i1 %lt
}

define i1 @"String.>.ref.String"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %v = alloca %String*, align 8
  store %String* %1, %String** %v, align 8
  %ref = load %String*, %String** %self, align 8
  %ref1 = load %String*, %String** %v, align 8
  %call = call i32 @String.compare(%String* %ref, %String* %ref1)
  %gt = icmp sgt i32 %call, 0
  ret i1 %gt
}

define i1 @"String.<.array"(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value1, 0
  %isnull = icmp eq i8* %opt.ptr, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.10, i32 0, i32 0), i64 0 }, { i8*, i64 } %value1
  %arr.data = extractvalue { i8*, i64 } %fallback, 0
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr3 = extractvalue { i8*, i64 } %str2, 0
  %isnull4 = icmp eq i8* %opt.ptr3, null
  %fallback5 = select i1 %isnull4, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.11, i32 0, i32 0), i64 0 }, { i8*, i64 } %str2
  %arr.data6 = extractvalue { i8*, i64 } %fallback5, 0
  %call = call i32 @strcmp(i8* %arr.data, i8* %arr.data6)
  %lt = icmp slt i32 %call, 0
  ret i1 %lt
}

define i1 @"String.>.array"(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value1, 0
  %isnull = icmp eq i8* %opt.ptr, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.12, i32 0, i32 0), i64 0 }, { i8*, i64 } %value1
  %arr.data = extractvalue { i8*, i64 } %fallback, 0
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr3 = extractvalue { i8*, i64 } %str2, 0
  %isnull4 = icmp eq i8* %opt.ptr3, null
  %fallback5 = select i1 %isnull4, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.13, i32 0, i32 0), i64 0 }, { i8*, i64 } %str2
  %arr.data6 = extractvalue { i8*, i64 } %fallback5, 0
  %call = call i32 @strcmp(i8* %arr.data, i8* %arr.data6)
  %gt = icmp sgt i32 %call, 0
  ret i1 %gt
}

define { i8*, i64 } @String.output(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value1, 0
  %isnull = icmp eq i8* %opt.ptr, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.14, i32 0, i32 0), i64 0 }, { i8*, i64 } %value1
  ret { i8*, i64 } %fallback
}

define %String2 @String2.create() {
entry:
  %s = alloca %String2, align 8
  store %String2 zeroinitializer, %String2* %s, align 8
  %size = getelementptr %String2, %String2* %s, i32 0, i32 2
  store i32 10, i32* %size, align 4
  %value = getelementptr %String2, %String2* %s, i32 0, i32 0
  %heap = call i8* @calloc(i64 10, i64 1)
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %heap, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 10, 1
  store { i8*, i64 } %arr.len, { i8*, i64 }* %value, align 8
  %s1 = load %String2, %String2* %s, align 8
  ret %String2 %s1
}

define void @String2.drop(%String2* %0) {
entry:
  %self = alloca %String2*, align 8
  store %String2* %0, %String2** %self, align 8
  %ref = load %String2*, %String2** %self, align 8
  %value = getelementptr %String2, %String2* %ref, i32 0, i32 0
  %arr = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %arr, 0
  call void @free(i8* %arr.data)
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %value, align 8
  ret void
}

define void @String2.grow(%String2* %0, i32 %1) {
entry:
  %self = alloca %String2*, align 8
  store %String2* %0, %String2** %self, align 8
  %want = alloca i32, align 4
  store i32 %1, i32* %want, align 4
  %ref = load %String2*, %String2** %self, align 8
  %size = getelementptr %String2, %String2* %ref, i32 0, i32 2
  %size1 = load i32, i32* %size, align 4
  %want2 = load i32, i32* %want, align 4
  %ge = icmp sge i32 %size1, %want2
  br i1 %ge, label %then, label %endif

endif:                                            ; preds = %entry
  br label %while.cond

then:                                             ; preds = %entry
  ret void

while.cond:                                       ; preds = %while.body, %endif
  %ref3 = load %String2*, %String2** %self, align 8
  %size4 = getelementptr %String2, %String2* %ref3, i32 0, i32 2
  %size5 = load i32, i32* %size4, align 4
  %want6 = load i32, i32* %want, align 4
  %lt = icmp slt i32 %size5, %want6
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref7 = load %String2*, %String2** %self, align 8
  %size8 = getelementptr %String2, %String2* %ref7, i32 0, i32 2
  %cur = load i32, i32* %size8, align 4
  %mul = mul i32 %cur, 2
  store i32 %mul, i32* %size8, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %bigger = alloca { i8*, i64 }, align 8
  %ref9 = load %String2*, %String2** %self, align 8
  %size10 = getelementptr %String2, %String2* %ref9, i32 0, i32 2
  %size11 = load i32, i32* %size10, align 4
  %n = sext i32 %size11 to i64
  %heap = call i8* @calloc(i64 %n, i64 1)
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %heap, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %n, 1
  store { i8*, i64 } %arr.len, { i8*, i64 }* %bigger, align 8
  %bigger12 = load { i8*, i64 }, { i8*, i64 }* %bigger, align 8
  %arr.data = extractvalue { i8*, i64 } %bigger12, 0
  %ref13 = load %String2*, %String2** %self, align 8
  %value = getelementptr %String2, %String2* %ref13, i32 0, i32 0
  %value14 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data15 = extractvalue { i8*, i64 } %value14, 0
  %call = call i8* @strcpy(i8* %arr.data, i8* %arr.data15)
  %ref16 = load %String2*, %String2** %self, align 8
  %value17 = getelementptr %String2, %String2* %ref16, i32 0, i32 0
  %arr = load { i8*, i64 }, { i8*, i64 }* %value17, align 8
  %arr.data18 = extractvalue { i8*, i64 } %arr, 0
  call void @free(i8* %arr.data18)
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %value17, align 8
  %ref19 = load %String2*, %String2** %self, align 8
  %value20 = getelementptr %String2, %String2* %ref19, i32 0, i32 0
  %bigger21 = load { i8*, i64 }, { i8*, i64 }* %bigger, align 8
  store { i8*, i64 } %bigger21, { i8*, i64 }* %value20, align 8
  ret void
}

declare i8* @strcpy(i8*, i8*)

define void @String2.assign(%String2* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String2*, align 8
  store %String2* %0, %String2** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr = extractvalue { i8*, i64 } %str1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %len = alloca i32, align 4
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data = extractvalue { i8*, i64 } %str2, 0
  %call = call i64 @strlen(i8* %arr.data)
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %len, align 4
  %ref3 = load %String2*, %String2** %self, align 8
  %len4 = load i32, i32* %len, align 4
  %add = add i32 %len4, 1
  call void @String2.grow(%String2* %ref3, i32 %add)
  %ref5 = load %String2*, %String2** %self, align 8
  %value = getelementptr %String2, %String2* %ref5, i32 0, i32 0
  %value6 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data7 = extractvalue { i8*, i64 } %value6, 0
  %ref8 = load %String2*, %String2** %self, align 8
  %size = getelementptr %String2, %String2* %ref8, i32 0, i32 2
  %size9 = load i32, i32* %size, align 4
  %cast10 = sext i32 %size9 to i64
  call void @bzero(i8* %arr.data7, i64 %cast10)
  %ref11 = load %String2*, %String2** %self, align 8
  %value12 = getelementptr %String2, %String2* %ref11, i32 0, i32 0
  %value13 = load { i8*, i64 }, { i8*, i64 }* %value12, align 8
  %arr.data14 = extractvalue { i8*, i64 } %value13, 0
  %str15 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data16 = extractvalue { i8*, i64 } %str15, 0
  %call17 = call i8* @strcpy(i8* %arr.data14, i8* %arr.data16)
  %ref18 = load %String2*, %String2** %self, align 8
  %count = getelementptr %String2, %String2* %ref18, i32 0, i32 1
  %len19 = load i32, i32* %len, align 4
  store i32 %len19, i32* %count, align 4
  ret void

then:                                             ; preds = %entry
  %ref = load %String2*, %String2** %self, align 8
  call void @String2.assign(%String2* %ref, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.15, i32 0, i32 0), i64 0 })
  ret void
}

declare void @bzero(i8*, i64)

define void @String2.join(%String2* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String2*, align 8
  store %String2* %0, %String2** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr = extractvalue { i8*, i64 } %str1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %len = alloca i32, align 4
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data = extractvalue { i8*, i64 } %str2, 0
  %call = call i64 @strlen(i8* %arr.data)
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %len, align 4
  %ref = load %String2*, %String2** %self, align 8
  %ref3 = load %String2*, %String2** %self, align 8
  %count = getelementptr %String2, %String2* %ref3, i32 0, i32 1
  %count4 = load i32, i32* %count, align 4
  %len5 = load i32, i32* %len, align 4
  %add = add i32 %count4, %len5
  %add6 = add i32 %add, 1
  call void @String2.grow(%String2* %ref, i32 %add6)
  %ref7 = load %String2*, %String2** %self, align 8
  %value = getelementptr %String2, %String2* %ref7, i32 0, i32 0
  %value8 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data9 = extractvalue { i8*, i64 } %value8, 0
  %str10 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data11 = extractvalue { i8*, i64 } %str10, 0
  %call12 = call i8* @strcat(i8* %arr.data9, i8* %arr.data11)
  %ref13 = load %String2*, %String2** %self, align 8
  %count14 = getelementptr %String2, %String2* %ref13, i32 0, i32 1
  %len15 = load i32, i32* %len, align 4
  %cur = load i32, i32* %count14, align 4
  %add16 = add i32 %cur, %len15
  store i32 %add16, i32* %count14, align 4
  ret void

then:                                             ; preds = %entry
  ret void
}

declare i8* @strcat(i8*, i8*)

define void @"String2.=.array"(%String2* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String2*, align 8
  store %String2* %0, %String2** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %ref = load %String2*, %String2** %self, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  call void @String2.assign(%String2* %ref, { i8*, i64 } %str1)
  ret void
}

define void @"String2.=.ref.String2"(%String2* %0, %String2* %1) {
entry:
  %self = alloca %String2*, align 8
  store %String2* %0, %String2** %self, align 8
  %v = alloca %String2*, align 8
  store %String2* %1, %String2** %v, align 8
  %ref = load %String2*, %String2** %self, align 8
  %ref1 = load %String2*, %String2** %v, align 8
  %value = getelementptr %String2, %String2* %ref1, i32 0, i32 0
  %value2 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  call void @String2.assign(%String2* %ref, { i8*, i64 } %value2)
  ret void
}

define %String2 @add(%String2* %0, %String2* %1) {
entry:
  %v = alloca %String2*, align 8
  store %String2* %0, %String2** %v, align 8
  %w = alloca %String2*, align 8
  store %String2* %1, %String2** %w, align 8
  %res = alloca %String2, align 8
  %call = call %String2 @String2.create()
  store %String2 %call, %String2* %res, align 8
  %ref = load %String2*, %String2** %v, align 8
  call void @"String2.=.ref.String2"(%String2* %res, %String2* %ref)
  %ref1 = load %String2*, %String2** %w, align 8
  %value = getelementptr %String2, %String2* %ref1, i32 0, i32 0
  %value2 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  call void @String2.join(%String2* %res, { i8*, i64 } %value2)
  %res3 = load %String2, %String2* %res, align 8
  ret %String2 %res3
}

define i32 @main(i32 %0, i8** %1) {
entry:
  store i32 1, i32* @EPERM, align 4
  store i32 2, i32* @ENOENT, align 4
  store i32 3, i32* @ESRCH, align 4
  store i32 4, i32* @EINTR, align 4
  store i32 5, i32* @EIO, align 4
  store i32 6, i32* @ENXIO, align 4
  store i32 7, i32* @E2BIG, align 4
  store i32 8, i32* @ENOEXEC, align 4
  store i32 9, i32* @EBADF, align 4
  store i32 10, i32* @ECHILD, align 4
  store i32 13, i32* @EACCES, align 4
  store i32 14, i32* @EFAULT, align 4
  store i32 15, i32* @ENOTBLK, align 4
  store i32 16, i32* @EBUSY, align 4
  store i32 17, i32* @EEXIST, align 4
  store i32 18, i32* @EXDEV, align 4
  store i32 19, i32* @ENODEV, align 4
  store i32 20, i32* @ENOTDIR, align 4
  store i32 21, i32* @EISDIR, align 4
  store i32 22, i32* @EINVAL, align 4
  store i32 23, i32* @ENFILE, align 4
  store i32 24, i32* @EMFILE, align 4
  store i32 25, i32* @ENOTTY, align 4
  store i32 26, i32* @ETXTBSY, align 4
  store i32 27, i32* @EFBIG, align 4
  store i32 28, i32* @ENOSPC, align 4
  store i32 29, i32* @ESPIPE, align 4
  store i32 30, i32* @EROFS, align 4
  store i32 31, i32* @EMLINK, align 4
  store i32 32, i32* @EPIPE, align 4
  store i32 33, i32* @EDOM, align 4
  store i32 34, i32* @ERANGE, align 4
  store i32 0, i32* @O_RDONLY, align 4
  store i32 1, i32* @O_WRONLY, align 4
  store i32 2, i32* @O_RDWR, align 4
  store i32 1, i32* @FD_CLOEXEC, align 4
  store i32 0, i32* @IPPROTO_IP, align 4
  store i32 6, i32* @IPPROTO_TCP, align 4
  store i32 17, i32* @IPPROTO_UDP, align 4
  store i32 41, i32* @IPPROTO_IPV6, align 4
  store i32 0, i32* @SHUT_RD, align 4
  store i32 1, i32* @SHUT_WR, align 4
  store i32 2, i32* @SHUT_RDWR, align 4
  store i32 1, i32* @POLLIN, align 4
  store i32 2, i32* @POLLPRI, align 4
  store i32 4, i32* @POLLOUT, align 4
  store i32 8, i32* @POLLERR, align 4
  store i32 16, i32* @POLLHUP, align 4
  store i32 32, i32* @POLLNVAL, align 4
  store i32 0, i32* @INADDR_ANY, align 4
  store i32 2130706433, i32* @INADDR_LOOPBACK, align 4
  store i32 -1, i32* @INADDR_BROADCAST, align 4
  store i32 1, i32* @SIGHUP, align 4
  store i32 2, i32* @SIGINT, align 4
  store i32 3, i32* @SIGQUIT, align 4
  store i32 4, i32* @SIGILL, align 4
  store i32 5, i32* @SIGTRAP, align 4
  store i32 6, i32* @SIGABRT, align 4
  store i32 8, i32* @SIGFPE, align 4
  store i32 9, i32* @SIGKILL, align 4
  store i32 11, i32* @SIGSEGV, align 4
  store i32 13, i32* @SIGPIPE, align 4
  store i32 14, i32* @SIGALRM, align 4
  store i32 15, i32* @SIGTERM, align 4
  store i32 61440, i32* @S_IFMT, align 4
  store i32 49152, i32* @S_IFSOCK, align 4
  store i32 40960, i32* @S_IFLNK, align 4
  store i32 32768, i32* @S_IFREG, align 4
  store i32 24576, i32* @S_IFBLK, align 4
  store i32 16384, i32* @S_IFDIR, align 4
  store i32 8192, i32* @S_IFCHR, align 4
  store i32 4096, i32* @S_IFIFO, align 4
  store i32 2048, i32* @S_ISUID, align 4
  store i32 1024, i32* @S_ISGID, align 4
  store i32 512, i32* @S_ISVTX, align 4
  store i32 0, i32* @F_OK, align 4
  store i32 1, i32* @X_OK, align 4
  store i32 2, i32* @W_OK, align 4
  store i32 4, i32* @R_OK, align 4
  store i32 0, i32* @SEEK_SET, align 4
  store i32 1, i32* @SEEK_CUR, align 4
  store i32 2, i32* @SEEK_END, align 4
  store i32 0, i32* @STDIN_FILENO, align 4
  store i32 1, i32* @STDOUT_FILENO, align 4
  store i32 2, i32* @STDERR_FILENO, align 4
  %s0 = alloca %String2, align 8
  %call = call %String2 @String2.create()
  store %String2 %call, %String2* %s0, align 8
  call void @"String2.=.array"(%String2* %s0, { i8*, i64 } { i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.16, i32 0, i32 0), i64 6 })
  %s1 = alloca %String2, align 8
  %call1 = call %String2 @String2.create()
  store %String2 %call1, %String2* %s1, align 8
  call void @"String2.=.array"(%String2* %s1, { i8*, i64 } { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.17, i32 0, i32 0), i64 5 })
  %s2 = alloca %String2, align 8
  %call2 = call %String2 @String2.create()
  store %String2 %call2, %String2* %s2, align 8
  %call3 = call %String2 @add(%String2* %s0, %String2* %s1)
  %op.tmp = alloca %String2, align 8
  store %String2 %call3, %String2* %op.tmp, align 8
  call void @"String2.=.ref.String2"(%String2* %s2, %String2* %op.tmp)
  %value = getelementptr %String2, %String2* %s0, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %str.len = extractvalue { i8*, i64 } %value4, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %value4, 0
  %count = getelementptr %String2, %String2* %s0, i32 0, i32 1
  %count5 = load i32, i32* %count, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @fmt, i32 0, i32 0), i32 7, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.18, i32 0, i32 0), i32 %len32, i8* %str.data, i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.19, i32 0, i32 0), i32 %count5, i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.20, i32 0, i32 0))
  %value6 = getelementptr %String2, %String2* %s1, i32 0, i32 0
  %value7 = load { i8*, i64 }, { i8*, i64 }* %value6, align 8
  %str.len8 = extractvalue { i8*, i64 } %value7, 1
  %len329 = trunc i64 %str.len8 to i32
  %str.data10 = extractvalue { i8*, i64 } %value7, 0
  %count11 = getelementptr %String2, %String2* %s1, i32 0, i32 1
  %count12 = load i32, i32* %count11, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @fmt.24, i32 0, i32 0), i32 7, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.21, i32 0, i32 0), i32 %len329, i8* %str.data10, i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.22, i32 0, i32 0), i32 %count12, i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.23, i32 0, i32 0))
  %value13 = getelementptr %String2, %String2* %s2, i32 0, i32 0
  %value14 = load { i8*, i64 }, { i8*, i64 }* %value13, align 8
  %str.len15 = extractvalue { i8*, i64 } %value14, 1
  %len3216 = trunc i64 %str.len15 to i32
  %str.data17 = extractvalue { i8*, i64 } %value14, 0
  %count18 = getelementptr %String2, %String2* %s2, i32 0, i32 1
  %count19 = load i32, i32* %count18, align 4
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @fmt.28, i32 0, i32 0), i32 7, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.25, i32 0, i32 0), i32 %len3216, i8* %str.data17, i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.26, i32 0, i32 0), i32 %count19, i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.27, i32 0, i32 0))
  call void @String2.drop(%String2* %s2)
  call void @String2.drop(%String2* %s1)
  call void @String2.drop(%String2* %s0)
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 029 — operator output replaces the default struct dump, nested too

```ura
// operators_overload/029.ura - operator output replaces the default struct dump, nested too

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

main():
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

struct Tag
├─ name : char[]
├─ n : i32
└─ fn Tag.output(self : STRUCT_CALL) : char[]
   └─ return
      └─ .name : char[]
         └─ self : STRUCT_CALL

struct Plain
└─ a : i32

struct Holder
├─ tag : STRUCT_CALL
└─ p : STRUCT_CALL

fn main() : i32
├─ t : STRUCT_CALL
├─ = : array
│  ├─ .name : char[]
│  │  └─ t : STRUCT_CALL
│  └─ char[] "custom"
├─ = : i32
│  ├─ .n : i32
│  │  └─ t : STRUCT_CALL
│  └─ int 7
├─ output : void
│  ├─ char[] "direct: "
│  ├─ t : STRUCT_CALL
│  └─ char[] "\n"
├─ p : STRUCT_CALL
├─ = : i32
│  ├─ .a : i32
│  │  └─ p : STRUCT_CALL
│  └─ int 1
├─ output : void
│  ├─ char[] "plain:  "
│  ├─ p : STRUCT_CALL
│  └─ char[] "\n"
├─ h : STRUCT_CALL
├─ = : array
│  ├─ .name : char[]
│  │  └─ .tag : STRUCT_CALL
│  │     └─ h : STRUCT_CALL
│  └─ char[] "inner"
├─ = : i32
│  ├─ .a : i32
│  │  └─ .p : STRUCT_CALL
│  │     └─ h : STRUCT_CALL
│  └─ int 2
└─ output : void
   ├─ char[] "nested: "
   ├─ h : STRUCT_CALL
   └─ char[] "\n"
```

```out
direct: custom
plain:  Plain{a: 1}
nested: Holder{tag: inner, p: Plain{a: 2}}
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Tag = type { { i8*, i64 }, i32 }
%Plain = type { i32 }
%__out_frame = type { i8*, i32, %__out_frame* }
%Holder = type { %Tag, %Plain }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [7 x i8] c"custom\00", align 1
@str.1 = private unnamed_addr constant [9 x i8] c"direct: \00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.2 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.5 = private unnamed_addr constant [9 x i8] c"plain:  \00", align 1
@fmt.6 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.7 = private unnamed_addr constant [11 x i8] c"[Circular]\00", align 1
@fmt.8 = private unnamed_addr constant [7 x i8] c"Plain{\00", align 1
@fmt.9 = private unnamed_addr constant [4 x i8] c"a: \00", align 1
@fmt.10 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@fmt.11 = private unnamed_addr constant [2 x i8] c"}\00", align 1
@str.12 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.13 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.14 = private unnamed_addr constant [6 x i8] c"inner\00", align 1
@str.15 = private unnamed_addr constant [9 x i8] c"nested: \00", align 1
@fmt.16 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.17 = private unnamed_addr constant [11 x i8] c"[Circular]\00", align 1
@fmt.18 = private unnamed_addr constant [8 x i8] c"Holder{\00", align 1
@fmt.19 = private unnamed_addr constant [6 x i8] c"tag: \00", align 1
@fmt.20 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.21 = private unnamed_addr constant [6 x i8] c", p: \00", align 1
@fmt.22 = private unnamed_addr constant [2 x i8] c"}\00", align 1
@str.23 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.24 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

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

define { i8*, i64 } @Tag.output(%Tag* %0) {
entry:
  %self = alloca %Tag*, align 8
  store %Tag* %0, %Tag** %self, align 8
  %ref = load %Tag*, %Tag** %self, align 8
  %name = getelementptr %Tag, %Tag* %ref, i32 0, i32 0
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  ret { i8*, i64 } %name1
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %t = alloca %Tag, align 8
  store %Tag zeroinitializer, %Tag* %t, align 8
  %name = getelementptr %Tag, %Tag* %t, i32 0, i32 0
  store { i8*, i64 } { i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i64 6 }, { i8*, i64 }* %name, align 8
  %n = getelementptr %Tag, %Tag* %t, i32 0, i32 1
  store i32 7, i32* %n, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0))
  %out = call { i8*, i64 } @Tag.output(%Tag* %t)
  %out.len = extractvalue { i8*, i64 } %out, 1
  %out.data = extractvalue { i8*, i64 } %out, 0
  %len32 = trunc i64 %out.len to i32
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.2, i32 0, i32 0), i32 %len32, i8* %out.data)
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.4, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %p = alloca %Plain, align 8
  store %Plain zeroinitializer, %Plain* %p, align 4
  %a = getelementptr %Plain, %Plain* %p, i32 0, i32 0
  store i32 1, i32* %a, align 4
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.6, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.5, i32 0, i32 0))
  call void @__out_Plain(%Plain* %p, %__out_frame* null)
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.13, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.12, i32 0, i32 0))
  %h = alloca %Holder, align 8
  store %Holder zeroinitializer, %Holder* %h, align 8
  %tag = getelementptr %Holder, %Holder* %h, i32 0, i32 0
  %name1 = getelementptr %Tag, %Tag* %tag, i32 0, i32 0
  store { i8*, i64 } { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.14, i32 0, i32 0), i64 5 }, { i8*, i64 }* %name1, align 8
  %p2 = getelementptr %Holder, %Holder* %h, i32 0, i32 1
  %a3 = getelementptr %Plain, %Plain* %p2, i32 0, i32 0
  store i32 2, i32* %a3, align 4
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.16, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.15, i32 0, i32 0))
  call void @__out_Holder(%Holder* %h, %__out_frame* null)
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.24, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.23, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)

define void @__out_Plain(%Plain* %0, %__out_frame* %1) {
entry:
  %me = bitcast %Plain* %0 to i8*
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
  store i32 1, i32* %f.ty, align 4
  %f.prev = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 2
  store %__out_frame* %1, %__out_frame** %f.prev, align 8
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.8, i32 0, i32 0))
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @fmt.9, i32 0, i32 0))
  %a = getelementptr %Plain, %Plain* %0, i32 0, i32 0
  %f = load i32, i32* %a, align 4
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.10, i32 0, i32 0), i32 %f)
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @fmt.11, i32 0, i32 0))
  ret void
}

define void @__out_Holder(%Holder* %0, %__out_frame* %1) {
entry:
  %me = bitcast %Holder* %0 to i8*
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
  store i32 2, i32* %f.ty, align 4
  %f.prev = getelementptr %__out_frame, %__out_frame* %frame, i32 0, i32 2
  store %__out_frame* %1, %__out_frame** %f.prev, align 8
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @fmt.18, i32 0, i32 0))
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @fmt.19, i32 0, i32 0))
  %tag = getelementptr %Holder, %Holder* %0, i32 0, i32 0
  %out = call { i8*, i64 } @Tag.output(%Tag* %tag)
  %out.len = extractvalue { i8*, i64 } %out, 1
  %out.data = extractvalue { i8*, i64 } %out, 0
  %len32 = trunc i64 %out.len to i32
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.20, i32 0, i32 0), i32 %len32, i8* %out.data)
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @fmt.21, i32 0, i32 0))
  %p = getelementptr %Holder, %Holder* %0, i32 0, i32 1
  call void @__out_Plain(%Plain* %p, %__out_frame* %frame)
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @fmt.22, i32 0, i32 0))
  ret void
}
```

## 030 — a struct owning heap buffers: deep-copy operator = and drop

```ura
// operators_overload/030.ura - a struct owning heap buffers: deep-copy operator = and drop
use "@/header"

struct Hero:
    name  char[]?
    title char[]?

    pub fn create() Hero:
        h Hero
        h.name  = new char[32]
        h.title = new char[32]
        return h

    fn setup(name char[], title char[]) void:
        strcpy(self.name,  name)
        strcpy(self.title, title)

    fn announce() void:
        output(self.title, " ", self.name, " enters!\n")

    operator = (ref other Hero) void:
        clean self.name
        clean self.title
        self.name  = new char[32]
        self.title = new char[32]
        strcpy(self.name,  other.name)
        strcpy(self.title, other.title)

    operator drop:
        clean self.name
        clean self.title

main():
    hero Hero = Hero::create()
    hero.setup("Aldric", "The Bold")
    hero.announce()

    other Hero = Hero::create()
    other.setup("Borin", "The Grim")
    hero = other
    hero.announce()

    // the copy owns its own buffers, so mutating the source leaves it alone
    other.setup("Cass", "The Quick")
    hero.announce()
    other.announce()
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

proto fn isalnum(c : i32) : i32

proto fn isalpha(c : i32) : i32

proto fn isascii(c : i32) : i32

proto fn isblank(c : i32) : i32

proto fn iscntrl(c : i32) : i32

proto fn isdigit(c : i32) : i32

proto fn isgraph(c : i32) : i32

proto fn islower(c : i32) : i32

proto fn isprint(c : i32) : i32

proto fn ispunct(c : i32) : i32

proto fn isspace(c : i32) : i32

proto fn isupper(c : i32) : i32

proto fn isxdigit(c : i32) : i32

proto fn tolower(c : i32) : i32

proto fn toupper(c : i32) : i32

proto fn strerror(errnum : i32) : pointer

proto fn strerror_r(errnum : i32, buf : pointer, buflen : i64) : i32

proto fn perror(s : pointer) : void

= : i32
├─ EPERM : i32
└─ int 1

= : i32
├─ ENOENT : i32
└─ int 2

= : i32
├─ ESRCH : i32
└─ int 3

= : i32
├─ EINTR : i32
└─ int 4

= : i32
├─ EIO : i32
└─ int 5

= : i32
├─ ENXIO : i32
└─ int 6

= : i32
├─ E2BIG : i32
└─ int 7

= : i32
├─ ENOEXEC : i32
└─ int 8

= : i32
├─ EBADF : i32
└─ int 9

= : i32
├─ ECHILD : i32
└─ int 10

= : i32
├─ EACCES : i32
└─ int 13

= : i32
├─ EFAULT : i32
└─ int 14

= : i32
├─ ENOTBLK : i32
└─ int 15

= : i32
├─ EBUSY : i32
└─ int 16

= : i32
├─ EEXIST : i32
└─ int 17

= : i32
├─ EXDEV : i32
└─ int 18

= : i32
├─ ENODEV : i32
└─ int 19

= : i32
├─ ENOTDIR : i32
└─ int 20

= : i32
├─ EISDIR : i32
└─ int 21

= : i32
├─ EINVAL : i32
└─ int 22

= : i32
├─ ENFILE : i32
└─ int 23

= : i32
├─ EMFILE : i32
└─ int 24

= : i32
├─ ENOTTY : i32
└─ int 25

= : i32
├─ ETXTBSY : i32
└─ int 26

= : i32
├─ EFBIG : i32
└─ int 27

= : i32
├─ ENOSPC : i32
└─ int 28

= : i32
├─ ESPIPE : i32
└─ int 29

= : i32
├─ EROFS : i32
└─ int 30

= : i32
├─ EMLINK : i32
└─ int 31

= : i32
├─ EPIPE : i32
└─ int 32

= : i32
├─ EDOM : i32
└─ int 33

= : i32
├─ ERANGE : i32
└─ int 34

proto fn open(path : pointer, flags : i32, mode : i32) : i32

proto fn openat(dirfd : i32, path : pointer, flags : i32, mode : i32) : i32

proto fn creat(path : pointer, mode : i32) : i32

proto fn fcntl(fd : i32, cmd : i32, arg : i32) : i32

= : i32
├─ O_RDONLY : i32
└─ int 0

= : i32
├─ O_WRONLY : i32
└─ int 1

= : i32
├─ O_RDWR : i32
└─ int 2

= : i32
├─ FD_CLOEXEC : i32
└─ int 1

proto fn fopen(path : pointer, mode : pointer) : pointer

proto fn freopen(path : pointer, mode : pointer, file : pointer) : pointer

proto fn fclose(file : pointer) : i32

proto fn fflush(file : pointer) : i32

proto fn fgetc(file : pointer) : i32

proto fn fputc(c : i32, file : pointer) : i32

proto fn getc(file : pointer) : i32

proto fn putc(c : i32, file : pointer) : i32

proto fn getchar() : i32

proto fn putchar(c : char) : i32

proto fn ungetc(c : i32, file : pointer) : i32

proto fn fgets(buffer : pointer, size : i32, file : pointer) : pointer

proto fn fputs(str : pointer, file : pointer) : i32

proto fn puts(str : pointer) : i32

proto fn gets(buffer : pointer) : pointer

proto fn fread(ptr : pointer, size : i32, nmemb : i32, file : pointer) : i32

proto fn fwrite(ptr : pointer, size : i32, nmemb : i32, file : pointer) : i32

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn read(fd : i32, ptr : pointer, len : i64) : i64

proto fn fseek(file : pointer, offset : i64, whence : i32) : i32

proto fn ftell(file : pointer) : i64

proto fn rewind(file : pointer) : void

proto fn fgetpos(file : pointer, pos : pointer) : i32

proto fn fsetpos(file : pointer, pos : pointer) : i32

proto fn feof(file : pointer) : i32

proto fn ferror(file : pointer) : i32

proto fn clearerr(file : pointer) : void

proto fn perror(str : pointer) : void

proto fn setvbuf(file : pointer, buffer : pointer, mode : i32, size : i32) : i32

proto fn setbuf(file : pointer, buffer : pointer) : void

proto fn remove(path : pointer) : i32

proto fn rename(oldpath : pointer, newpath : pointer) : i32

proto fn tmpfile() : pointer

proto fn tmpnam(str : pointer) : pointer

proto fn fileno(file : pointer) : i32

proto fn fdopen(fd : i32, mode : pointer) : pointer

proto fn printf(format : pointer, ...) : i32

proto fn fprintf(file : pointer, format : pointer, ...) : i32

proto fn sprintf(buffer : pointer, format : pointer, ...) : i32

proto fn snprintf(buffer : pointer, size : i32, format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn vprintf(format : pointer, args : pointer) : i32

proto fn vfprintf(file : pointer, format : pointer, args : pointer) : i32

proto fn vsprintf(buffer : pointer, format : pointer, args : pointer) : i32

proto fn vsnprintf(buffer : pointer, size : i32, format : pointer, args : pointer) : i32

proto fn vdprintf(fd : i32, format : pointer, args : pointer) : i32

proto fn scanf(format : pointer, ...) : i32

proto fn fscanf(file : pointer, format : pointer, ...) : i32

proto fn sscanf(buffer : pointer, format : pointer, ...) : i32

proto fn vscanf(format : pointer, args : pointer) : i32

proto fn vfscanf(file : pointer, format : pointer, args : pointer) : i32

proto fn vsscanf(buffer : pointer, format : pointer, args : pointer) : i32

proto fn sin(x : f64) : f64

proto fn cos(x : f64) : f64

proto fn tan(x : f64) : f64

proto fn asin(x : f64) : f64

proto fn acos(x : f64) : f64

proto fn atan(x : f64) : f64

proto fn atan2(y : f64, x : f64) : f64

proto fn sinh(x : f64) : f64

proto fn cosh(x : f64) : f64

proto fn tanh(x : f64) : f64

proto fn asinh(x : f64) : f64

proto fn acosh(x : f64) : f64

proto fn atanh(x : f64) : f64

proto fn exp(x : f64) : f64

proto fn exp2(x : f64) : f64

proto fn expm1(x : f64) : f64

proto fn log(x : f64) : f64

proto fn log2(x : f64) : f64

proto fn log10(x : f64) : f64

proto fn log1p(x : f64) : f64

proto fn pow(x : f64, y : f64) : f64

proto fn sqrt(x : f64) : f64

proto fn cbrt(x : f64) : f64

proto fn hypot(x : f64, y : f64) : f64

proto fn ceil(x : f64) : f64

proto fn floor(x : f64) : f64

proto fn trunc(x : f64) : f64

proto fn round(x : f64) : f64

proto fn fmod(x : f64, y : f64) : f64

proto fn remainder(x : f64, y : f64) : f64

proto fn fabs(x : f64) : f64

proto fn copysign(x : f64, y : f64) : f64

proto fn fmax(x : f64, y : f64) : f64

proto fn fmin(x : f64, y : f64) : f64

proto fn fdim(x : f64, y : f64) : f64

proto fn isnan(x : f64) : i32

proto fn isinf(x : f64) : i32

proto fn isfinite(x : f64) : i32

proto fn signbit(x : f64) : i32

proto fn tgamma(x : f64) : f64

proto fn lgamma(x : f64) : f64

proto fn erf(x : f64) : f64

proto fn erfc(x : f64) : f64

proto fn malloc(size : i32) : pointer

proto fn calloc(len : i64, size : i64) : pointer

proto fn realloc(ptr : pointer, newSize : i32) : pointer

proto fn free(ptr : pointer) : void

proto fn aligned_alloc(alignment : i32, size : i32) : pointer

struct Pollfd
├─ fd : i32
├─ events : i16
└─ revents : i16

proto fn socket(domain : i32, type : i32, protocol : i32) : i32

proto fn bind(fd : i32, addr : pointer, addrlen : i32) : i32

proto fn listen(fd : i32, backlog : i32) : i32

proto fn accept(fd : i32, addr : pointer, addrlen : pointer) : i32

proto fn connect(fd : i32, addr : pointer, addrlen : i32) : i32

proto fn shutdown(fd : i32, how : i32) : i32

proto fn send(fd : i32, buf : pointer, len : i64, flags : i32) : i64

proto fn recv(fd : i32, buf : pointer, len : i64, flags : i32) : i64

proto fn sendto(fd : i32, buf : pointer, len : i64, flags : i32, addr : pointer, addrlen : i32) : i64

proto fn recvfrom(fd : i32, buf : pointer, len : i64, flags : i32, addr : pointer, addrlen : pointer) : i64

proto fn sendmsg(fd : i32, msg : pointer, flags : i32) : i64

proto fn recvmsg(fd : i32, msg : pointer, flags : i32) : i64

proto fn setsockopt(fd : i32, level : i32, optname : i32, optval : pointer, optlen : i32) : i32

proto fn getsockopt(fd : i32, level : i32, optname : i32, optval : pointer, optlen : pointer) : i32

proto fn getsockname(fd : i32, addr : pointer, addrlen : pointer) : i32

proto fn getpeername(fd : i32, addr : pointer, addrlen : pointer) : i32

proto fn htons(hostshort : i32) : i32

proto fn htonl(hostlong : i32) : i32

proto fn ntohs(netshort : i32) : i32

proto fn ntohl(netlong : i32) : i32

proto fn inet_addr(cp : pointer) : i32

proto fn inet_ntoa(addr : u32) : pointer

proto fn inet_pton(af : i32, src : pointer, dst : pointer) : i32

proto fn inet_ntop(af : i32, src : pointer, dst : pointer, size : i32) : pointer

proto fn gethostbyname(name : pointer) : pointer

proto fn gethostbyaddr(addr : pointer, len : i32, type : i32) : pointer

proto fn getaddrinfo(node : pointer, service : pointer, hints : pointer, res : pointer) : i32

proto fn freeaddrinfo(res : pointer) : void

proto fn gai_strerror(errcode : i32) : pointer

proto fn getnameinfo(addr : pointer, addrlen : i32, host : pointer, hostlen : i32, serv : pointer, servlen : i32, flags : i32) : i32

proto fn select(nfds : i32, readfds : pointer, writefds : pointer, exceptfds : pointer, timeout : pointer) : i32

proto fn poll(fds : pointer, nfds : i32, timeout : i32) : i32

proto fn ioctl(fd : i32, request : i32, arg : pointer) : i32

= : i32
├─ IPPROTO_IP : i32
└─ int 0

= : i32
├─ IPPROTO_TCP : i32
└─ int 6

= : i32
├─ IPPROTO_UDP : i32
└─ int 17

= : i32
├─ IPPROTO_IPV6 : i32
└─ int 41

= : i32
├─ SHUT_RD : i32
└─ int 0

= : i32
├─ SHUT_WR : i32
└─ int 1

= : i32
├─ SHUT_RDWR : i32
└─ int 2

= : i32
├─ POLLIN : i32
└─ int 1

= : i32
├─ POLLPRI : i32
└─ int 2

= : i32
├─ POLLOUT : i32
└─ int 4

= : i32
├─ POLLERR : i32
└─ int 8

= : i32
├─ POLLHUP : i32
└─ int 16

= : i32
├─ POLLNVAL : i32
└─ int 32

= : i32
├─ INADDR_ANY : i32
└─ int 0

= : i32
├─ INADDR_LOOPBACK : i32
└─ int 2130706433

= : i32
├─ INADDR_BROADCAST : i32
└─ - : i32
   ├─ int 0
   └─ int 1

proto fn signal(sig : i32, handler : pointer) : pointer

proto fn raise(sig : i32) : i32

proto fn kill(pid : i32, sig : i32) : i32

proto fn sigaction(sig : i32, act : pointer, oldact : pointer) : i32

proto fn sigprocmask(how : i32, set : pointer, oldset : pointer) : i32

proto fn sigsuspend(mask : pointer) : i32

proto fn strsignal(sig : i32) : pointer

= : i32
├─ SIGHUP : i32
└─ int 1

= : i32
├─ SIGINT : i32
└─ int 2

= : i32
├─ SIGQUIT : i32
└─ int 3

= : i32
├─ SIGILL : i32
└─ int 4

= : i32
├─ SIGTRAP : i32
└─ int 5

= : i32
├─ SIGABRT : i32
└─ int 6

= : i32
├─ SIGFPE : i32
└─ int 8

= : i32
├─ SIGKILL : i32
└─ int 9

= : i32
├─ SIGSEGV : i32
└─ int 11

= : i32
├─ SIGPIPE : i32
└─ int 13

= : i32
├─ SIGALRM : i32
└─ int 14

= : i32
├─ SIGTERM : i32
└─ int 15

proto fn stat(path : pointer, buf : pointer) : i32

proto fn fstat(fd : i32, buf : pointer) : i32

proto fn lstat(path : pointer, buf : pointer) : i32

proto fn mkdir(path : pointer, mode : i32) : i32

proto fn chmod(path : pointer, mode : i32) : i32

proto fn fchmod(fd : i32, mode : i32) : i32

proto fn umask(cmask : i32) : i32

= : i32
├─ S_IFMT : i32
└─ int 61440

= : i32
├─ S_IFSOCK : i32
└─ int 49152

= : i32
├─ S_IFLNK : i32
└─ int 40960

= : i32
├─ S_IFREG : i32
└─ int 32768

= : i32
├─ S_IFBLK : i32
└─ int 24576

= : i32
├─ S_IFDIR : i32
└─ int 16384

= : i32
├─ S_IFCHR : i32
└─ int 8192

= : i32
├─ S_IFIFO : i32
└─ int 4096

= : i32
├─ S_ISUID : i32
└─ int 2048

= : i32
├─ S_ISGID : i32
└─ int 1024

= : i32
├─ S_ISVTX : i32
└─ int 512

proto fn abort() : void

proto fn exit(code : i32) : void

proto fn quick_exit(code : i32) : void

proto fn _Exit(code : i32) : void

proto fn getenv(name : pointer) : pointer

proto fn setenv(name : pointer, value : pointer, overwrite : i32) : i32

proto fn unsetenv(name : pointer) : i32

proto fn putenv(string : pointer) : i32

proto fn system(command : pointer) : i32

proto fn rand() : i32

proto fn srand(seed : i32) : void

proto fn bsearch(key : pointer, base : pointer, nmemb : i32, size : i32, compar : pointer) : pointer

proto fn qsort(base : pointer, nmemb : i32, size : i32, compar : pointer) : void

proto fn abs(n : i32) : i32

proto fn labs(n : i64) : i64

proto fn div(numer : i32, denom : i32) : pointer

proto fn ldiv(numer : i64, denom : i64) : pointer

proto fn atoi(str : pointer) : i32

proto fn atol(str : pointer) : i64

proto fn atof(str : pointer) : f64

proto fn strtol(str : pointer, endptr : pointer, base : i32) : i64

proto fn strtoul(str : pointer, endptr : pointer, base : i32) : i64

proto fn strtof(str : pointer, endptr : pointer) : f32

proto fn strtod(str : pointer, endptr : pointer) : f64

proto fn strtoll(str : pointer, endptr : pointer, base : i32) : i64

proto fn strtoull(str : pointer, endptr : pointer, base : i32) : i64

proto fn mblen(s : pointer, n : i32) : i32

proto fn mbtowc(pwc : pointer, s : pointer, n : i32) : i32

proto fn wctomb(s : pointer, wchar : i32) : i32

proto fn mbstowcs(dest : pointer, src : pointer, n : i32) : i32

proto fn wcstombs(dest : pointer, src : pointer, n : i32) : i32

proto fn strlen(s : pointer) : i64

proto fn strnlen(s : pointer, maxlen : i32) : i32

proto fn strcmp(a : pointer, b : pointer) : i32

proto fn strncmp(a : pointer, b : pointer, n : i32) : i32

proto fn strcasecmp(a : pointer, b : pointer) : i32

proto fn strncasecmp(a : pointer, b : pointer, n : i32) : i32

proto fn strcoll(a : pointer, b : pointer) : i32

proto fn strxfrm(dest : pointer, src : pointer, n : i32) : i32

proto fn strcpy(dest : pointer, src : pointer) : pointer

proto fn strncpy(dest : pointer, src : pointer, n : i32) : pointer

proto fn strdup(s : pointer) : pointer

proto fn strndup(s : pointer, n : i32) : pointer

proto fn strcat(dest : pointer, src : pointer) : pointer

proto fn strncat(dest : pointer, src : pointer, n : i32) : pointer

proto fn strchr(s : pointer, c : i32) : pointer

proto fn strrchr(s : pointer, c : i32) : pointer

proto fn strstr(haystack : pointer, needle : pointer) : pointer

proto fn strcasestr(haystack : pointer, needle : pointer) : pointer

proto fn strspn(s : pointer, accept : pointer) : i32

proto fn strcspn(s : pointer, reject : pointer) : i32

proto fn strpbrk(s : pointer, accept : pointer) : pointer

proto fn strtok(s : pointer, delim : pointer) : pointer

proto fn strtok_r(s : pointer, delim : pointer, saveptr : pointer) : pointer

proto fn strerror(errnum : i32) : pointer

proto fn strerror_r(errnum : i32, buf : pointer, buflen : i64) : i32

proto fn memchr(ptr : pointer, c : i32, n : i32) : pointer

proto fn memrchr(ptr : pointer, c : i32, n : i32) : pointer

proto fn memcmp(a : pointer, b : pointer, n : i32) : i32

proto fn memcpy(dest : pointer, src : pointer, n : i32) : pointer

proto fn memmove(dest : pointer, src : pointer, n : i32) : pointer

proto fn memset(ptr : pointer, value : i32, n : i32) : pointer

proto fn memccpy(dest : pointer, src : pointer, c : i32, n : i32) : pointer

proto fn bzero(s : pointer, size : i64) : void

proto fn strlcpy(dest : pointer, src : pointer, size : i32) : i32

proto fn strlcat(dest : pointer, src : pointer, size : i32) : i32

proto fn strsignal(sig : i32) : pointer

proto fn memset_s(ptr : pointer, size : i32, value : i32, n : i32) : pointer

proto fn atoi(s : pointer) : i32

struct String
├─ value : char[]
├─ count : i32
├─ fn String.create() : STRUCT_CALL
│  ├─ s : STRUCT_CALL
│  ├─ = : array
│  │  ├─ .value : char[]
│  │  │  └─ s : STRUCT_CALL
│  │  └─ array : char[]
│  │     └─ int 16
│  ├─ = : i32
│  │  ├─ .count : i32
│  │  │  └─ s : STRUCT_CALL
│  │  └─ int 0
│  └─ return
│     └─ s : STRUCT_CALL
├─ fn String.from(str : array) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ s : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  ├─ call assign : void
│  │  ├─ s : STRUCT_CALL
│  │  └─ str : char[]
│  └─ return
│     └─ s : STRUCT_CALL
├─ fn String.from_int(n : i32) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ s : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ n : i32
│  │  │  └─ int 0
│  │  ├─ call push : void
│  │  │  ├─ s : STRUCT_CALL
│  │  │  └─ char '0'
│  │  └─ return
│  │     └─ s : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ num : i32
│  │  └─ n : i32
│  ├─ if
│  │  ├─ condition < : bool
│  │  │  ├─ num : i32
│  │  │  └─ int 0
│  │  ├─ call push : void
│  │  │  ├─ s : STRUCT_CALL
│  │  │  └─ char '-'
│  │  └─ = : i32
│  │     ├─ num : i32
│  │     └─ - : i32
│  │        ├─ int 0
│  │        └─ num : i32
│  ├─ = : array
│  │  ├─ digits : char[]
│  │  └─ array : char[]
│  │     └─ int 12
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ int 0
│  ├─ while
│  │  ├─ condition > : bool
│  │  │  ├─ num : i32
│  │  │  └─ int 0
│  │  ├─ = : char
│  │  │  ├─ index : char
│  │  │  │  ├─ digits : char[]
│  │  │  │  └─ i : i32
│  │  │  └─ cast : char
│  │  │     └─ + : i32
│  │  │        ├─ % : i32
│  │  │        │  ├─ num : i32
│  │  │        │  └─ int 10
│  │  │        └─ int 48
│  │  ├─ = : i32
│  │  │  ├─ num : i32
│  │  │  └─ / : i32
│  │  │     ├─ num : i32
│  │  │     └─ int 10
│  │  └─ += : i32
│  │     ├─ i : i32
│  │     └─ int 1
│  ├─ while
│  │  ├─ condition > : bool
│  │  │  ├─ i : i32
│  │  │  └─ int 0
│  │  ├─ -= : i32
│  │  │  ├─ i : i32
│  │  │  └─ int 1
│  │  └─ call push : void
│  │     ├─ s : STRUCT_CALL
│  │     └─ index : char
│  │        ├─ digits : char[]
│  │        └─ i : i32
│  ├─ clean : void
│  │  └─ digits : char[]
│  └─ return
│     └─ s : STRUCT_CALL
├─ fn String.drop(self : STRUCT_CALL) : void
│  └─ clean : void
│     └─ .value : char[]
│        └─ self : STRUCT_CALL
├─ fn String.cap(self : STRUCT_CALL) : i32
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ .value : char[]
│  │  │  │  └─ self : STRUCT_CALL
│  │  │  └─ NULL_LIT : char[]
│  │  └─ return
│  │     └─ int 0
│  └─ return
│     └─ cast : i32
│        └─ .len : u64
│           └─ .value : char[]
│              └─ self : STRUCT_CALL
├─ fn String.grow(self : STRUCT_CALL, want : i32) : void
│  ├─ = : i32
│  │  ├─ room : i32
│  │  └─ call cap : i32
│  │     └─ self : STRUCT_CALL
│  ├─ if
│  │  ├─ condition >= : bool
│  │  │  ├─ room : i32
│  │  │  └─ want : i32
│  │  └─ return
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ room : i32
│  │  │  └─ int 0
│  │  └─ = : i32
│  │     ├─ room : i32
│  │     └─ int 16
│  ├─ while
│  │  ├─ condition < : bool
│  │  │  ├─ room : i32
│  │  │  └─ want : i32
│  │  └─ *= : i32
│  │     ├─ room : i32
│  │     └─ int 2
│  ├─ = : array
│  │  ├─ bigger : char[]
│  │  └─ array : char[]
│  │     └─ room : i32
│  ├─ if
│  │  ├─ condition > : bool
│  │  │  ├─ .count : i32
│  │  │  │  └─ self : STRUCT_CALL
│  │  │  └─ int 0
│  │  └─ call memcpy : pointer
│  │     ├─ bigger : char[]
│  │     ├─ FALLBACK : char[]
│  │     │  ├─ .value : char[]
│  │     │  │  └─ self : STRUCT_CALL
│  │     │  └─ char[] ""
│  │     └─ .count : i32
│  │        └─ self : STRUCT_CALL
│  ├─ = : char
│  │  ├─ index : char
│  │  │  ├─ bigger : char[]
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  └─ cast : char
│  │     └─ int 0
│  ├─ clean : void
│  │  └─ .value : char[]
│  │     └─ self : STRUCT_CALL
│  └─ = : array
│     ├─ .value : char[]
│     │  └─ self : STRUCT_CALL
│     └─ bigger : char[]
├─ fn String.assign(self : STRUCT_CALL, str : array) : void
│  ├─ = : i32
│  │  ├─ n : i32
│  │  └─ int 0
│  ├─ if
│  │  ├─ condition != : bool
│  │  │  ├─ str : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  └─ = : i32
│  │     ├─ n : i32
│  │     └─ cast : i32
│  │        └─ call strlen : i64
│  │           └─ str : char[]
│  ├─ call grow : void
│  │  ├─ self : STRUCT_CALL
│  │  └─ + : i32
│  │     ├─ n : i32
│  │     └─ int 1
│  ├─ if
│  │  ├─ condition > : bool
│  │  │  ├─ n : i32
│  │  │  └─ int 0
│  │  └─ call memcpy : pointer
│  │     ├─ FALLBACK : char[]
│  │     │  ├─ .value : char[]
│  │     │  │  └─ self : STRUCT_CALL
│  │     │  └─ char[] ""
│  │     ├─ FALLBACK : char[]
│  │     │  ├─ str : char[]
│  │     │  └─ char[] ""
│  │     └─ n : i32
│  ├─ = : char
│  │  ├─ index : char
│  │  │  ├─ .value : char[]
│  │  │  │  └─ self : STRUCT_CALL
│  │  │  └─ n : i32
│  │  └─ cast : char
│  │     └─ int 0
│  └─ = : i32
│     ├─ .count : i32
│     │  └─ self : STRUCT_CALL
│     └─ n : i32
├─ fn String.join(self : STRUCT_CALL, str : array) : void
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ str : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  └─ return
│  ├─ = : i32
│  │  ├─ n : i32
│  │  └─ cast : i32
│  │     └─ call strlen : i64
│  │        └─ str : char[]
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ n : i32
│  │  │  └─ int 0
│  │  └─ return
│  ├─ call grow : void
│  │  ├─ self : STRUCT_CALL
│  │  └─ + : i32
│  │     ├─ + : i32
│  │     │  ├─ .count : i32
│  │     │  │  └─ self : STRUCT_CALL
│  │     │  └─ n : i32
│  │     └─ int 1
│  ├─ call memcpy : pointer
│  │  ├─ index : char[]
│  │  │  ├─ .value : char[]
│  │  │  │  └─ self : STRUCT_CALL
│  │  │  └─ range : i32
│  │  │     ├─ .count : i32
│  │  │     │  └─ self : STRUCT_CALL
│  │  │     └─ + : i32
│  │  │        ├─ .count : i32
│  │  │        │  └─ self : STRUCT_CALL
│  │  │        └─ n : i32
│  │  ├─ str : char[]
│  │  └─ n : i32
│  ├─ += : i32
│  │  ├─ .count : i32
│  │  │  └─ self : STRUCT_CALL
│  │  └─ n : i32
│  └─ = : char
│     ├─ index : char
│     │  ├─ .value : char[]
│     │  │  └─ self : STRUCT_CALL
│     │  └─ .count : i32
│     │     └─ self : STRUCT_CALL
│     └─ cast : char
│        └─ int 0
├─ fn String.push(self : STRUCT_CALL, c : char) : void
│  ├─ call grow : void
│  │  ├─ self : STRUCT_CALL
│  │  └─ + : i32
│  │     ├─ .count : i32
│  │     │  └─ self : STRUCT_CALL
│  │     └─ int 2
│  ├─ = : char
│  │  ├─ index : char
│  │  │  ├─ .value : char[]
│  │  │  │  └─ self : STRUCT_CALL
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  └─ c : char
│  ├─ += : i32
│  │  ├─ .count : i32
│  │  │  └─ self : STRUCT_CALL
│  │  └─ int 1
│  └─ = : char
│     ├─ index : char
│     │  ├─ .value : char[]
│     │  │  └─ self : STRUCT_CALL
│     │  └─ .count : i32
│     │     └─ self : STRUCT_CALL
│     └─ cast : char
│        └─ int 0
├─ fn String.pop(self : STRUCT_CALL) : char
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ .count : i32
│  │  │  │  └─ self : STRUCT_CALL
│  │  │  └─ int 0
│  │  └─ return
│  │     └─ cast : char
│  │        └─ int 0
│  ├─ -= : i32
│  │  ├─ .count : i32
│  │  │  └─ self : STRUCT_CALL
│  │  └─ int 1
│  ├─ = : char
│  │  ├─ c : char
│  │  └─ index : char
│  │     ├─ .value : char[]
│  │     │  └─ self : STRUCT_CALL
│  │     └─ .count : i32
│  │        └─ self : STRUCT_CALL
│  ├─ = : char
│  │  ├─ index : char
│  │  │  ├─ .value : char[]
│  │  │  │  └─ self : STRUCT_CALL
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  └─ cast : char
│  │     └─ int 0
│  └─ return
│     └─ c : char
├─ fn String.clear(self : STRUCT_CALL) : void
│  ├─ = : i32
│  │  ├─ .count : i32
│  │  │  └─ self : STRUCT_CALL
│  │  └─ int 0
│  └─ if
│     ├─ condition != : bool
│     │  ├─ .value : char[]
│     │  │  └─ self : STRUCT_CALL
│     │  └─ NULL_LIT : char[]
│     └─ = : char
│        ├─ index : char
│        │  ├─ .value : char[]
│        │  │  └─ self : STRUCT_CALL
│        │  └─ int 0
│        └─ cast : char
│           └─ int 0
├─ fn String.len(self : STRUCT_CALL) : i32
│  └─ return
│     └─ .count : i32
│        └─ self : STRUCT_CALL
├─ fn String.empty(self : STRUCT_CALL) : bool
│  └─ return
│     └─ == : bool
│        ├─ .count : i32
│        │  └─ self : STRUCT_CALL
│        └─ int 0
├─ fn String.c_str(self : STRUCT_CALL) : char[]
│  └─ return
│     └─ FALLBACK : char[]
│        ├─ .value : char[]
│        │  └─ self : STRUCT_CALL
│        └─ char[] ""
├─ fn String.at(self : STRUCT_CALL, i : i32) : char
│  ├─ if
│  │  ├─ condition or : bool
│  │  │  ├─ < : bool
│  │  │  │  ├─ i : i32
│  │  │  │  └─ int 0
│  │  │  └─ >= : bool
│  │  │     ├─ i : i32
│  │  │     └─ .count : i32
│  │  │        └─ self : STRUCT_CALL
│  │  └─ return
│  │     └─ cast : char
│  │        └─ int 0
│  └─ return
│     └─ index : char
│        ├─ .value : char[]
│        │  └─ self : STRUCT_CALL
│        └─ i : i32
├─ fn String.find(self : STRUCT_CALL, needle : array) : i32
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ needle : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  └─ return
│  │     └─ - : i32
│  │        ├─ int 0
│  │        └─ int 1
│  ├─ = : i32
│  │  ├─ n : i32
│  │  └─ cast : i32
│  │     └─ call strlen : i64
│  │        └─ needle : char[]
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ n : i32
│  │  │  └─ int 0
│  │  └─ return
│  │     └─ int 0
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ int 0
│  ├─ while
│  │  ├─ condition <= : bool
│  │  │  ├─ + : i32
│  │  │  │  ├─ i : i32
│  │  │  │  └─ n : i32
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  ├─ if
│  │  │  ├─ condition == : bool
│  │  │  │  ├─ call strncmp : i32
│  │  │  │  │  ├─ index : char[]
│  │  │  │  │  │  ├─ .value : char[]
│  │  │  │  │  │  │  └─ self : STRUCT_CALL
│  │  │  │  │  │  └─ range : i32
│  │  │  │  │  │     ├─ i : i32
│  │  │  │  │  │     └─ + : i32
│  │  │  │  │  │        ├─ i : i32
│  │  │  │  │  │        └─ n : i32
│  │  │  │  │  ├─ needle : char[]
│  │  │  │  │  └─ n : i32
│  │  │  │  └─ int 0
│  │  │  └─ return
│  │  │     └─ i : i32
│  │  └─ += : i32
│  │     ├─ i : i32
│  │     └─ int 1
│  └─ return
│     └─ - : i32
│        ├─ int 0
│        └─ int 1
├─ fn String.contains(self : STRUCT_CALL, needle : array) : bool
│  └─ return
│     └─ != : bool
│        ├─ call find : i32
│        │  ├─ self : STRUCT_CALL
│        │  └─ needle : char[]
│        └─ - : i32
│           ├─ int 0
│           └─ int 1
├─ fn String.starts_with(self : STRUCT_CALL, prefix : array) : bool
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ prefix : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  └─ return
│  │     └─ bool True
│  ├─ = : i32
│  │  ├─ n : i32
│  │  └─ cast : i32
│  │     └─ call strlen : i64
│  │        └─ prefix : char[]
│  ├─ if
│  │  ├─ condition > : bool
│  │  │  ├─ n : i32
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  └─ return
│  │     └─ bool False
│  └─ return
│     └─ == : bool
│        ├─ call strncmp : i32
│        │  ├─ index : char[]
│        │  │  ├─ .value : char[]
│        │  │  │  └─ self : STRUCT_CALL
│        │  │  └─ range : i32
│        │  │     ├─ int 0
│        │  │     └─ n : i32
│        │  ├─ prefix : char[]
│        │  └─ n : i32
│        └─ int 0
├─ fn String.ends_with(self : STRUCT_CALL, suffix : array) : bool
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ suffix : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  └─ return
│  │     └─ bool True
│  ├─ = : i32
│  │  ├─ n : i32
│  │  └─ cast : i32
│  │     └─ call strlen : i64
│  │        └─ suffix : char[]
│  ├─ if
│  │  ├─ condition > : bool
│  │  │  ├─ n : i32
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  └─ return
│  │     └─ bool False
│  └─ return
│     └─ == : bool
│        ├─ call strncmp : i32
│        │  ├─ index : char[]
│        │  │  ├─ .value : char[]
│        │  │  │  └─ self : STRUCT_CALL
│        │  │  └─ range : i32
│        │  │     ├─ - : i32
│        │  │     │  ├─ .count : i32
│        │  │     │  │  └─ self : STRUCT_CALL
│        │  │     │  └─ n : i32
│        │  │     └─ .count : i32
│        │  │        └─ self : STRUCT_CALL
│        │  ├─ suffix : char[]
│        │  └─ n : i32
│        └─ int 0
├─ fn String.substr(self : STRUCT_CALL, start : i32, length : i32) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ res : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  ├─ if
│  │  ├─ condition or : bool
│  │  │  ├─ < : bool
│  │  │  │  ├─ start : i32
│  │  │  │  └─ int 0
│  │  │  └─ >= : bool
│  │  │     ├─ start : i32
│  │  │     └─ .count : i32
│  │  │        └─ self : STRUCT_CALL
│  │  └─ return
│  │     └─ res : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ stop : i32
│  │  └─ + : i32
│  │     ├─ start : i32
│  │     └─ length : i32
│  ├─ if
│  │  ├─ condition > : bool
│  │  │  ├─ stop : i32
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  └─ = : i32
│  │     ├─ stop : i32
│  │     └─ .count : i32
│  │        └─ self : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ start : i32
│  ├─ while
│  │  ├─ condition < : bool
│  │  │  ├─ i : i32
│  │  │  └─ stop : i32
│  │  ├─ call push : void
│  │  │  ├─ res : STRUCT_CALL
│  │  │  └─ index : char
│  │  │     ├─ .value : char[]
│  │  │     │  └─ self : STRUCT_CALL
│  │  │     └─ i : i32
│  │  └─ += : i32
│  │     ├─ i : i32
│  │     └─ int 1
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn String.upper(self : STRUCT_CALL) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ res : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ int 0
│  ├─ while
│  │  ├─ condition < : bool
│  │  │  ├─ i : i32
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  ├─ = : char
│  │  │  ├─ c : char
│  │  │  └─ index : char
│  │  │     ├─ .value : char[]
│  │  │     │  └─ self : STRUCT_CALL
│  │  │     └─ i : i32
│  │  ├─ if
│  │  │  ├─ condition and : bool
│  │  │  │  ├─ >= : bool
│  │  │  │  │  ├─ c : char
│  │  │  │  │  └─ char 'a'
│  │  │  │  └─ <= : bool
│  │  │  │     ├─ c : char
│  │  │  │     └─ char 'z'
│  │  │  ├─ call push : void
│  │  │  │  ├─ res : STRUCT_CALL
│  │  │  │  └─ cast : char
│  │  │  │     └─ - : i32
│  │  │  │        ├─ cast : i32
│  │  │  │        │  └─ c : char
│  │  │  │        └─ int 32
│  │  │  └─ else
│  │  │     └─ call push : void
│  │  │        ├─ res : STRUCT_CALL
│  │  │        └─ c : char
│  │  └─ += : i32
│  │     ├─ i : i32
│  │     └─ int 1
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn String.lower(self : STRUCT_CALL) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ res : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ int 0
│  ├─ while
│  │  ├─ condition < : bool
│  │  │  ├─ i : i32
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  ├─ = : char
│  │  │  ├─ c : char
│  │  │  └─ index : char
│  │  │     ├─ .value : char[]
│  │  │     │  └─ self : STRUCT_CALL
│  │  │     └─ i : i32
│  │  ├─ if
│  │  │  ├─ condition and : bool
│  │  │  │  ├─ >= : bool
│  │  │  │  │  ├─ c : char
│  │  │  │  │  └─ char 'A'
│  │  │  │  └─ <= : bool
│  │  │  │     ├─ c : char
│  │  │  │     └─ char 'Z'
│  │  │  ├─ call push : void
│  │  │  │  ├─ res : STRUCT_CALL
│  │  │  │  └─ cast : char
│  │  │  │     └─ + : i32
│  │  │  │        ├─ cast : i32
│  │  │  │        │  └─ c : char
│  │  │  │        └─ int 32
│  │  │  └─ else
│  │  │     └─ call push : void
│  │  │        ├─ res : STRUCT_CALL
│  │  │        └─ c : char
│  │  └─ += : i32
│  │     ├─ i : i32
│  │     └─ int 1
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn String.trim(self : STRUCT_CALL) : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ start : i32
│  │  └─ int 0
│  ├─ while
│  │  ├─ condition < : bool
│  │  │  ├─ start : i32
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  ├─ = : char
│  │  │  ├─ c : char
│  │  │  └─ index : char
│  │  │     ├─ .value : char[]
│  │  │     │  └─ self : STRUCT_CALL
│  │  │     └─ start : i32
│  │  └─ if
│  │     ├─ condition or : bool
│  │     │  ├─ or : bool
│  │     │  │  ├─ or : bool
│  │     │  │  │  ├─ == : bool
│  │     │  │  │  │  ├─ c : char
│  │     │  │  │  │  └─ char ' '
│  │     │  │  │  └─ == : bool
│  │     │  │  │     ├─ c : char
│  │     │  │  │     └─ char '\t'
│  │     │  │  └─ == : bool
│  │     │  │     ├─ c : char
│  │     │  │     └─ char '\n'
│  │     │  └─ == : bool
│  │     │     ├─ c : char
│  │     │     └─ char '\r'
│  │     ├─ += : i32
│  │     │  ├─ start : i32
│  │     │  └─ int 1
│  │     └─ else
│  │        └─ break
│  ├─ = : i32
│  │  ├─ stop : i32
│  │  └─ .count : i32
│  │     └─ self : STRUCT_CALL
│  ├─ while
│  │  ├─ condition > : bool
│  │  │  ├─ stop : i32
│  │  │  └─ start : i32
│  │  ├─ = : char
│  │  │  ├─ c : char
│  │  │  └─ index : char
│  │  │     ├─ .value : char[]
│  │  │     │  └─ self : STRUCT_CALL
│  │  │     └─ - : i32
│  │  │        ├─ stop : i32
│  │  │        └─ int 1
│  │  └─ if
│  │     ├─ condition or : bool
│  │     │  ├─ or : bool
│  │     │  │  ├─ or : bool
│  │     │  │  │  ├─ == : bool
│  │     │  │  │  │  ├─ c : char
│  │     │  │  │  │  └─ char ' '
│  │     │  │  │  └─ == : bool
│  │     │  │  │     ├─ c : char
│  │     │  │  │     └─ char '\t'
│  │     │  │  └─ == : bool
│  │     │  │     ├─ c : char
│  │     │  │     └─ char '\n'
│  │     │  └─ == : bool
│  │     │     ├─ c : char
│  │     │     └─ char '\r'
│  │     ├─ -= : i32
│  │     │  ├─ stop : i32
│  │     │  └─ int 1
│  │     └─ else
│  │        └─ break
│  └─ return
│     └─ call substr : STRUCT_CALL
│        ├─ self : STRUCT_CALL
│        ├─ start : i32
│        └─ - : i32
│           ├─ stop : i32
│           └─ start : i32
├─ fn String.replace(self : STRUCT_CALL, old : array, fresh : array) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ res : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ old : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  ├─ call assign : void
│  │  │  ├─ res : STRUCT_CALL
│  │  │  └─ .value : char[]
│  │  │     └─ self : STRUCT_CALL
│  │  └─ return
│  │     └─ res : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ n : i32
│  │  └─ cast : i32
│  │     └─ call strlen : i64
│  │        └─ old : char[]
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ n : i32
│  │  │  └─ int 0
│  │  ├─ call assign : void
│  │  │  ├─ res : STRUCT_CALL
│  │  │  └─ .value : char[]
│  │  │     └─ self : STRUCT_CALL
│  │  └─ return
│  │     └─ res : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ int 0
│  ├─ while
│  │  ├─ condition < : bool
│  │  │  ├─ i : i32
│  │  │  └─ .count : i32
│  │  │     └─ self : STRUCT_CALL
│  │  ├─ = : bool
│  │  │  ├─ hit : bool
│  │  │  └─ bool False
│  │  ├─ if
│  │  │  ├─ condition <= : bool
│  │  │  │  ├─ + : i32
│  │  │  │  │  ├─ i : i32
│  │  │  │  │  └─ n : i32
│  │  │  │  └─ .count : i32
│  │  │  │     └─ self : STRUCT_CALL
│  │  │  └─ = : bool
│  │  │     ├─ hit : bool
│  │  │     └─ == : bool
│  │  │        ├─ call strncmp : i32
│  │  │        │  ├─ index : char[]
│  │  │        │  │  ├─ .value : char[]
│  │  │        │  │  │  └─ self : STRUCT_CALL
│  │  │        │  │  └─ range : i32
│  │  │        │  │     ├─ i : i32
│  │  │        │  │     └─ + : i32
│  │  │        │  │        ├─ i : i32
│  │  │        │  │        └─ n : i32
│  │  │        │  ├─ old : char[]
│  │  │        │  └─ n : i32
│  │  │        └─ int 0
│  │  └─ if
│  │     ├─ condition hit : bool
│  │     ├─ call join : void
│  │     │  ├─ res : STRUCT_CALL
│  │     │  └─ fresh : char[]
│  │     ├─ += : i32
│  │     │  ├─ i : i32
│  │     │  └─ n : i32
│  │     └─ else
│  │        ├─ call push : void
│  │        │  ├─ res : STRUCT_CALL
│  │        │  └─ index : char
│  │        │     ├─ .value : char[]
│  │        │     │  └─ self : STRUCT_CALL
│  │        │     └─ i : i32
│  │        └─ += : i32
│  │           ├─ i : i32
│  │           └─ int 1
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn String.repeat(self : STRUCT_CALL, n : i32) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ res : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ int 0
│  ├─ while
│  │  ├─ condition < : bool
│  │  │  ├─ i : i32
│  │  │  └─ n : i32
│  │  ├─ call join : void
│  │  │  ├─ res : STRUCT_CALL
│  │  │  └─ .value : char[]
│  │  │     └─ self : STRUCT_CALL
│  │  └─ += : i32
│  │     ├─ i : i32
│  │     └─ int 1
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn String.reverse(self : STRUCT_CALL) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ res : STRUCT_CALL
│  │  └─ call create : STRUCT_CALL
│  ├─ = : i32
│  │  ├─ i : i32
│  │  └─ - : i32
│  │     ├─ .count : i32
│  │     │  └─ self : STRUCT_CALL
│  │     └─ int 1
│  ├─ while
│  │  ├─ condition >= : bool
│  │  │  ├─ i : i32
│  │  │  └─ int 0
│  │  ├─ call push : void
│  │  │  ├─ res : STRUCT_CALL
│  │  │  └─ index : char
│  │  │     ├─ .value : char[]
│  │  │     │  └─ self : STRUCT_CALL
│  │  │     └─ i : i32
│  │  └─ -= : i32
│  │     ├─ i : i32
│  │     └─ int 1
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn String.compare(self : STRUCT_CALL, other : STRUCT_CALL) : i32
│  └─ return
│     └─ call strcmp : i32
│        ├─ FALLBACK : char[]
│        │  ├─ .value : char[]
│        │  │  └─ self : STRUCT_CALL
│        │  └─ char[] ""
│        └─ FALLBACK : char[]
│           ├─ .value : char[]
│           │  └─ other : STRUCT_CALL
│           └─ char[] ""
├─ fn String.to_int(self : STRUCT_CALL) : i32
│  └─ return
│     └─ call atoi : i32
│        └─ FALLBACK : char[]
│           ├─ .value : char[]
│           │  └─ self : STRUCT_CALL
│           └─ char[] ""
├─ fn String.=.array(self : STRUCT_CALL, str : array) : void
│  └─ call assign : void
│     ├─ self : STRUCT_CALL
│     └─ str : char[]
├─ fn String.=.ref.String(self : STRUCT_CALL, v : STRUCT_CALL) : void
│  └─ call assign : void
│     ├─ self : STRUCT_CALL
│     └─ .value : char[]
│        └─ v : STRUCT_CALL
├─ fn String.+.ref.String(self : STRUCT_CALL, v : STRUCT_CALL) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ res : STRUCT_CALL
│  │  └─ call from : STRUCT_CALL
│  │     └─ .value : char[]
│  │        └─ self : STRUCT_CALL
│  ├─ call join : void
│  │  ├─ res : STRUCT_CALL
│  │  └─ .value : char[]
│  │     └─ v : STRUCT_CALL
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn String.+.array(self : STRUCT_CALL, str : array) : STRUCT_CALL
│  ├─ = : STRUCT_CALL
│  │  ├─ res : STRUCT_CALL
│  │  └─ call from : STRUCT_CALL
│  │     └─ .value : char[]
│  │        └─ self : STRUCT_CALL
│  ├─ call join : void
│  │  ├─ res : STRUCT_CALL
│  │  └─ str : char[]
│  └─ return
│     └─ res : STRUCT_CALL
├─ fn String.+=.ref.String(self : STRUCT_CALL, v : STRUCT_CALL) : void
│  └─ call join : void
│     ├─ self : STRUCT_CALL
│     └─ .value : char[]
│        └─ v : STRUCT_CALL
├─ fn String.+=.array(self : STRUCT_CALL, str : array) : void
│  └─ call join : void
│     ├─ self : STRUCT_CALL
│     └─ str : char[]
├─ fn String.==.ref.String(self : STRUCT_CALL, v : STRUCT_CALL) : bool
│  └─ return
│     └─ == : bool
│        ├─ call compare : i32
│        │  ├─ self : STRUCT_CALL
│        │  └─ ref : STRUCT_CALL
│        │     └─ v : STRUCT_CALL
│        └─ int 0
├─ fn String.!=.ref.String(self : STRUCT_CALL, v : STRUCT_CALL) : bool
│  └─ return
│     └─ != : bool
│        ├─ call compare : i32
│        │  ├─ self : STRUCT_CALL
│        │  └─ ref : STRUCT_CALL
│        │     └─ v : STRUCT_CALL
│        └─ int 0
├─ fn String.==.array(self : STRUCT_CALL, str : array) : bool
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ str : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  └─ return
│  │     └─ == : bool
│  │        ├─ .count : i32
│  │        │  └─ self : STRUCT_CALL
│  │        └─ int 0
│  └─ return
│     └─ == : bool
│        ├─ call strcmp : i32
│        │  ├─ FALLBACK : char[]
│        │  │  ├─ .value : char[]
│        │  │  │  └─ self : STRUCT_CALL
│        │  │  └─ char[] ""
│        │  └─ str : char[]
│        └─ int 0
├─ fn String.!=.array(self : STRUCT_CALL, str : array) : bool
│  ├─ if
│  │  ├─ condition == : bool
│  │  │  ├─ str : char[]
│  │  │  └─ NULL_LIT : char[]
│  │  └─ return
│  │     └─ != : bool
│  │        ├─ .count : i32
│  │        │  └─ self : STRUCT_CALL
│  │        └─ int 0
│  └─ return
│     └─ != : bool
│        ├─ call strcmp : i32
│        │  ├─ FALLBACK : char[]
│        │  │  ├─ .value : char[]
│        │  │  │  └─ self : STRUCT_CALL
│        │  │  └─ char[] ""
│        │  └─ str : char[]
│        └─ int 0
├─ fn String.<.ref.String(self : STRUCT_CALL, v : STRUCT_CALL) : bool
│  └─ return
│     └─ < : bool
│        ├─ call compare : i32
│        │  ├─ self : STRUCT_CALL
│        │  └─ ref : STRUCT_CALL
│        │     └─ v : STRUCT_CALL
│        └─ int 0
├─ fn String.>.ref.String(self : STRUCT_CALL, v : STRUCT_CALL) : bool
│  └─ return
│     └─ > : bool
│        ├─ call compare : i32
│        │  ├─ self : STRUCT_CALL
│        │  └─ ref : STRUCT_CALL
│        │     └─ v : STRUCT_CALL
│        └─ int 0
├─ fn String.<.array(self : STRUCT_CALL, str : array) : bool
│  └─ return
│     └─ < : bool
│        ├─ call strcmp : i32
│        │  ├─ FALLBACK : char[]
│        │  │  ├─ .value : char[]
│        │  │  │  └─ self : STRUCT_CALL
│        │  │  └─ char[] ""
│        │  └─ FALLBACK : char[]
│        │     ├─ str : char[]
│        │     └─ char[] ""
│        └─ int 0
├─ fn String.>.array(self : STRUCT_CALL, str : array) : bool
│  └─ return
│     └─ > : bool
│        ├─ call strcmp : i32
│        │  ├─ FALLBACK : char[]
│        │  │  ├─ .value : char[]
│        │  │  │  └─ self : STRUCT_CALL
│        │  │  └─ char[] ""
│        │  └─ FALLBACK : char[]
│        │     ├─ str : char[]
│        │     └─ char[] ""
│        └─ int 0
└─ fn String.output(self : STRUCT_CALL) : char[]
   └─ return
      └─ FALLBACK : char[]
         ├─ .value : char[]
         │  └─ self : STRUCT_CALL
         └─ char[] ""

struct Timespec
├─ tv_sec : i64
└─ tv_nsec : i64

struct Timeval
├─ tv_sec : i64
├─ tv_usec : i32
└─ __pad : i32

struct Tm
├─ tm_sec : i32
├─ tm_min : i32
├─ tm_hour : i32
├─ tm_mday : i32
├─ tm_mon : i32
├─ tm_year : i32
├─ tm_wday : i32
├─ tm_yday : i32
├─ tm_isdst : i32
├─ tm_gmtoff : i64
└─ tm_zone : pointer

proto fn time(timer : pointer) : i64

proto fn difftime(time1 : i64, time0 : i64) : f64

proto fn clock() : i64

proto fn gmtime(timer : i64) : pointer

proto fn localtime(timer : i64) : pointer

proto fn mktime(timeptr : pointer) : i64

proto fn gmtime_r(timer : i64, result : pointer) : pointer

proto fn localtime_r(timer : i64, result : pointer) : pointer

proto fn strftime(s : pointer, maxsize : i64, format : pointer, timeptr : pointer) : i64

proto fn asctime(timeptr : pointer) : pointer

proto fn ctime(timer : pointer) : pointer

proto fn asctime_r(timeptr : pointer, buf : pointer) : pointer

proto fn ctime_r(timer : pointer, buf : pointer) : pointer

proto fn clock_gettime(clk_id : i32, tp : pointer) : i32

proto fn clock_settime(clk_id : i32, tp : pointer) : i32

proto fn nanosleep(req : pointer, rem : pointer) : i32

proto fn gettimeofday(tv : pointer, tz : pointer) : i32

proto fn getpid() : i32

proto fn getppid() : i32

proto fn getuid() : i32

proto fn geteuid() : i32

proto fn getgid() : i32

proto fn getegid() : i32

proto fn setuid(uid : i32) : i32

proto fn setgid(gid : i32) : i32

proto fn fork() : i32

proto fn execv(path : pointer, argv : pointer) : i32

proto fn execve(path : pointer, argv : pointer, envp : pointer) : i32

proto fn execvp(file : pointer, argv : pointer) : i32

proto fn _exit(code : i32) : void

proto fn getcwd(buf : pointer, size : i64) : pointer

proto fn chdir(path : pointer) : i32

proto fn fchdir(fd : i32) : i32

proto fn access(path : pointer, mode : i32) : i32

proto fn unlink(path : pointer) : i32

proto fn rmdir(path : pointer) : i32

proto fn link(oldpath : pointer, newpath : pointer) : i32

proto fn symlink(target : pointer, linkpath : pointer) : i32

proto fn readlink(path : pointer, buf : pointer, bufsize : i64) : i64

proto fn rename(oldpath : pointer, newpath : pointer) : i32

proto fn truncate(path : pointer, length : i64) : i32

proto fn ftruncate(fd : i32, length : i64) : i32

proto fn close(fd : i32) : i32

proto fn dup(fd : i32) : i32

proto fn dup2(oldfd : i32, newfd : i32) : i32

proto fn pipe(pipefd : pointer) : i32

proto fn lseek(fd : i32, offset : i64, whence : i32) : i64

proto fn isatty(fd : i32) : i32

proto fn ttyname(fd : i32) : pointer

proto fn sleep(seconds : i32) : i32

proto fn usleep(microseconds : i32) : i32

proto fn gethostname(name : pointer, len : i64) : i32

proto fn sethostname(name : pointer, len : i64) : i32

proto fn alarm(seconds : i32) : i32

proto fn pause() : i32

proto fn sync() : void

proto fn fsync(fd : i32) : i32

proto fn fdatasync(fd : i32) : i32

= : i32
├─ F_OK : i32
└─ int 0

= : i32
├─ X_OK : i32
└─ int 1

= : i32
├─ W_OK : i32
└─ int 2

= : i32
├─ R_OK : i32
└─ int 4

= : i32
├─ SEEK_SET : i32
└─ int 0

= : i32
├─ SEEK_CUR : i32
└─ int 1

= : i32
├─ SEEK_END : i32
└─ int 2

= : i32
├─ STDIN_FILENO : i32
└─ int 0

= : i32
├─ STDOUT_FILENO : i32
└─ int 1

= : i32
├─ STDERR_FILENO : i32
└─ int 2

struct Hero
├─ name : char[]
├─ title : char[]
├─ fn Hero.create() : STRUCT_CALL
│  ├─ h : STRUCT_CALL
│  ├─ = : array
│  │  ├─ .name : char[]
│  │  │  └─ h : STRUCT_CALL
│  │  └─ array : char[]
│  │     └─ int 32
│  ├─ = : array
│  │  ├─ .title : char[]
│  │  │  └─ h : STRUCT_CALL
│  │  └─ array : char[]
│  │     └─ int 32
│  └─ return
│     └─ h : STRUCT_CALL
├─ fn Hero.setup(self : STRUCT_CALL, name : array, title : array) : void
│  ├─ call strcpy : pointer
│  │  ├─ .name : char[]
│  │  │  └─ self : STRUCT_CALL
│  │  └─ name : char[]
│  └─ call strcpy : pointer
│     ├─ .title : char[]
│     │  └─ self : STRUCT_CALL
│     └─ title : char[]
├─ fn Hero.announce(self : STRUCT_CALL) : void
│  └─ output : void
│     ├─ .title : char[]
│     │  └─ self : STRUCT_CALL
│     ├─ char[] " "
│     ├─ .name : char[]
│     │  └─ self : STRUCT_CALL
│     └─ char[] " enters!\n"
├─ fn Hero.=.ref.Hero(self : STRUCT_CALL, other : STRUCT_CALL) : void
│  ├─ clean : void
│  │  └─ .name : char[]
│  │     └─ self : STRUCT_CALL
│  ├─ clean : void
│  │  └─ .title : char[]
│  │     └─ self : STRUCT_CALL
│  ├─ = : array
│  │  ├─ .name : char[]
│  │  │  └─ self : STRUCT_CALL
│  │  └─ array : char[]
│  │     └─ int 32
│  ├─ = : array
│  │  ├─ .title : char[]
│  │  │  └─ self : STRUCT_CALL
│  │  └─ array : char[]
│  │     └─ int 32
│  ├─ call strcpy : pointer
│  │  ├─ .name : char[]
│  │  │  └─ self : STRUCT_CALL
│  │  └─ .name : char[]
│  │     └─ other : STRUCT_CALL
│  └─ call strcpy : pointer
│     ├─ .title : char[]
│     │  └─ self : STRUCT_CALL
│     └─ .title : char[]
│        └─ other : STRUCT_CALL
└─ fn Hero.drop(self : STRUCT_CALL) : void
   ├─ clean : void
   │  └─ .name : char[]
   │     └─ self : STRUCT_CALL
   └─ clean : void
      └─ .title : char[]
         └─ self : STRUCT_CALL

fn main() : i32
├─ = : STRUCT_CALL
│  ├─ hero : STRUCT_CALL
│  └─ call create : STRUCT_CALL
├─ call setup : void
│  ├─ hero : STRUCT_CALL
│  ├─ char[] "Aldric"
│  └─ char[] "The Bold"
├─ call announce : void
│  └─ hero : STRUCT_CALL
├─ = : STRUCT_CALL
│  ├─ other : STRUCT_CALL
│  └─ call create : STRUCT_CALL
├─ call setup : void
│  ├─ other : STRUCT_CALL
│  ├─ char[] "Borin"
│  └─ char[] "The Grim"
├─ = : void
│  ├─ hero : STRUCT_CALL
│  └─ other : STRUCT_CALL
├─ call announce : void
│  └─ hero : STRUCT_CALL
├─ call setup : void
│  ├─ other : STRUCT_CALL
│  ├─ char[] "Cass"
│  └─ char[] "The Quick"
├─ call announce : void
│  └─ hero : STRUCT_CALL
└─ call announce : void
   └─ other : STRUCT_CALL
```

```out
The Bold Aldric enters!
The Grim Borin enters!
The Grim Borin enters!
The Quick Cass enters!
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%String = type { { i8*, i64 }, i32 }
%Hero = type { { i8*, i64 }, { i8*, i64 } }

@os = internal global %Os zeroinitializer
@EPERM = internal global i32 0
@ENOENT = internal global i32 0
@ESRCH = internal global i32 0
@EINTR = internal global i32 0
@EIO = internal global i32 0
@ENXIO = internal global i32 0
@E2BIG = internal global i32 0
@ENOEXEC = internal global i32 0
@EBADF = internal global i32 0
@ECHILD = internal global i32 0
@EACCES = internal global i32 0
@EFAULT = internal global i32 0
@ENOTBLK = internal global i32 0
@EBUSY = internal global i32 0
@EEXIST = internal global i32 0
@EXDEV = internal global i32 0
@ENODEV = internal global i32 0
@ENOTDIR = internal global i32 0
@EISDIR = internal global i32 0
@EINVAL = internal global i32 0
@ENFILE = internal global i32 0
@EMFILE = internal global i32 0
@ENOTTY = internal global i32 0
@ETXTBSY = internal global i32 0
@EFBIG = internal global i32 0
@ENOSPC = internal global i32 0
@ESPIPE = internal global i32 0
@EROFS = internal global i32 0
@EMLINK = internal global i32 0
@EPIPE = internal global i32 0
@EDOM = internal global i32 0
@ERANGE = internal global i32 0
@O_RDONLY = internal global i32 0
@O_WRONLY = internal global i32 0
@O_RDWR = internal global i32 0
@FD_CLOEXEC = internal global i32 0
@IPPROTO_IP = internal global i32 0
@IPPROTO_TCP = internal global i32 0
@IPPROTO_UDP = internal global i32 0
@IPPROTO_IPV6 = internal global i32 0
@SHUT_RD = internal global i32 0
@SHUT_WR = internal global i32 0
@SHUT_RDWR = internal global i32 0
@POLLIN = internal global i32 0
@POLLPRI = internal global i32 0
@POLLOUT = internal global i32 0
@POLLERR = internal global i32 0
@POLLHUP = internal global i32 0
@POLLNVAL = internal global i32 0
@INADDR_ANY = internal global i32 0
@INADDR_LOOPBACK = internal global i32 0
@INADDR_BROADCAST = internal global i32 0
@SIGHUP = internal global i32 0
@SIGINT = internal global i32 0
@SIGQUIT = internal global i32 0
@SIGILL = internal global i32 0
@SIGTRAP = internal global i32 0
@SIGABRT = internal global i32 0
@SIGFPE = internal global i32 0
@SIGKILL = internal global i32 0
@SIGSEGV = internal global i32 0
@SIGPIPE = internal global i32 0
@SIGALRM = internal global i32 0
@SIGTERM = internal global i32 0
@S_IFMT = internal global i32 0
@S_IFSOCK = internal global i32 0
@S_IFLNK = internal global i32 0
@S_IFREG = internal global i32 0
@S_IFBLK = internal global i32 0
@S_IFDIR = internal global i32 0
@S_IFCHR = internal global i32 0
@S_IFIFO = internal global i32 0
@S_ISUID = internal global i32 0
@S_ISGID = internal global i32 0
@S_ISVTX = internal global i32 0
@F_OK = internal global i32 0
@X_OK = internal global i32 0
@W_OK = internal global i32 0
@R_OK = internal global i32 0
@SEEK_SET = internal global i32 0
@SEEK_CUR = internal global i32 0
@SEEK_END = internal global i32 0
@STDIN_FILENO = internal global i32 0
@STDOUT_FILENO = internal global i32 0
@STDERR_FILENO = internal global i32 0
@trap_msg = private unnamed_addr constant [153 x i8] c"runtime error: Modulo by zero\0A   ura-lib/string.ura:97:30\0A   |\0A97 |             digits[i] = (num % 10 + 48) as char\0A   |                              ^\0A\00", align 1
@trap_msg.1 = private unnamed_addr constant [127 x i8] c"runtime error: Division by zero\0A   ura-lib/string.ura:98:23\0A   |\0A98 |             num = num / 10\0A   |                       ^\0A\00", align 1
@str = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.2 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.3 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.4 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.5 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.6 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.7 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.8 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.9 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.10 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.11 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.12 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.13 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.14 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.15 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.16 = private unnamed_addr constant [10 x i8] c" enters!\0A\00", align 1
@fmt = private unnamed_addr constant [17 x i8] c"%.*s%.*s%.*s%.*s\00", align 1
@str.17 = private unnamed_addr constant [7 x i8] c"Aldric\00", align 1
@str.18 = private unnamed_addr constant [9 x i8] c"The Bold\00", align 1
@str.19 = private unnamed_addr constant [6 x i8] c"Borin\00", align 1
@str.20 = private unnamed_addr constant [9 x i8] c"The Grim\00", align 1
@str.21 = private unnamed_addr constant [5 x i8] c"Cass\00", align 1
@str.22 = private unnamed_addr constant [10 x i8] c"The Quick\00", align 1

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

define %String @String.create() {
entry:
  %s = alloca %String, align 8
  store %String zeroinitializer, %String* %s, align 8
  %value = getelementptr %String, %String* %s, i32 0, i32 0
  %heap = call i8* @calloc(i64 16, i64 1)
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %heap, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 16, 1
  store { i8*, i64 } %arr.len, { i8*, i64 }* %value, align 8
  %count = getelementptr %String, %String* %s, i32 0, i32 1
  store i32 0, i32* %count, align 4
  %s1 = load %String, %String* %s, align 8
  ret %String %s1
}

declare i8* @calloc(i64, i64)

define %String @String.from({ i8*, i64 } %0) {
entry:
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %0, { i8*, i64 }* %str, align 8
  %s = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %s, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  call void @String.assign(%String* %s, { i8*, i64 } %str1)
  %s2 = load %String, %String* %s, align 8
  ret %String %s2
}

define void @String.assign(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %n = alloca i32, align 4
  store i32 0, i32* %n, align 4
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr = extractvalue { i8*, i64 } %str1, 0
  %nullcmp = icmp ne i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %then, %entry
  %ref = load %String*, %String** %self, align 8
  %n3 = load i32, i32* %n, align 4
  %add = add i32 %n3, 1
  call void @String.grow(%String* %ref, i32 %add)
  %n6 = load i32, i32* %n, align 4
  %gt = icmp sgt i32 %n6, 0
  br i1 %gt, label %then5, label %endif4

then:                                             ; preds = %entry
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data = extractvalue { i8*, i64 } %str2, 0
  %call = call i64 @strlen(i8* %arr.data)
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  br label %endif

endif4:                                           ; preds = %then5, %endif
  %ref18 = load %String*, %String** %self, align 8
  %value19 = getelementptr %String, %String* %ref18, i32 0, i32 0
  %value20 = load { i8*, i64 }, { i8*, i64 }* %value19, align 8
  %arr.data21 = extractvalue { i8*, i64 } %value20, 0
  %n22 = load i32, i32* %n, align 4
  %arr.at = getelementptr i8, i8* %arr.data21, i32 %n22
  store i8 0, i8* %arr.at, align 1
  %ref23 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref23, i32 0, i32 1
  %n24 = load i32, i32* %n, align 4
  store i32 %n24, i32* %count, align 4
  ret void

then5:                                            ; preds = %endif
  %ref7 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref7, i32 0, i32 0
  %value8 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr9 = extractvalue { i8*, i64 } %value8, 0
  %isnull = icmp eq i8* %opt.ptr9, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.2, i32 0, i32 0), i64 0 }, { i8*, i64 } %value8
  %arr.data10 = extractvalue { i8*, i64 } %fallback, 0
  %str11 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr12 = extractvalue { i8*, i64 } %str11, 0
  %isnull13 = icmp eq i8* %opt.ptr12, null
  %fallback14 = select i1 %isnull13, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.3, i32 0, i32 0), i64 0 }, { i8*, i64 } %str11
  %arr.data15 = extractvalue { i8*, i64 } %fallback14, 0
  %n16 = load i32, i32* %n, align 4
  %call17 = call i8* @memcpy(i8* %arr.data10, i8* %arr.data15, i32 %n16)
  br label %endif4
}

define %String @String.from_int(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %s = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %s, align 8
  %n1 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n1, 0
  br i1 %eq, label %then, label %endif

endif:                                            ; preds = %entry
  %num = alloca i32, align 4
  %n3 = load i32, i32* %n, align 4
  store i32 %n3, i32* %num, align 4
  %num6 = load i32, i32* %num, align 4
  %lt = icmp slt i32 %num6, 0
  br i1 %lt, label %then5, label %endif4

then:                                             ; preds = %entry
  call void @String.push(%String* %s, i8 48)
  %s2 = load %String, %String* %s, align 8
  ret %String %s2

endif4:                                           ; preds = %then5, %endif
  %digits = alloca { i8*, i64 }, align 8
  %heap = call i8* @calloc(i64 12, i64 1)
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %heap, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 12, 1
  store { i8*, i64 } %arr.len, { i8*, i64 }* %digits, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then5:                                            ; preds = %endif
  call void @String.push(%String* %s, i8 45)
  %num7 = load i32, i32* %num, align 4
  %sub = sub i32 0, %num7
  store i32 %sub, i32* %num, align 4
  br label %endif4

while.cond:                                       ; preds = %cont14, %endif4
  %num8 = load i32, i32* %num, align 4
  %gt = icmp sgt i32 %num8, 0
  br i1 %gt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %digits9 = load { i8*, i64 }, { i8*, i64 }* %digits, align 8
  %arr.data = extractvalue { i8*, i64 } %digits9, 0
  %i10 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %i10
  %num11 = load i32, i32* %num, align 4
  br i1 false, label %trap, label %cont

while.end:                                        ; preds = %while.cond
  br label %while.cond16

trap:                                             ; preds = %while.body
  %1 = call i64 @write(i32 2, i8* getelementptr inbounds ([153 x i8], [153 x i8]* @trap_msg, i32 0, i32 0), i64 152)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %while.body
  %mod = srem i32 %num11, 10
  %add = add i32 %mod, 48
  %cast = trunc i32 %add to i8
  store i8 %cast, i8* %arr.at, align 1
  %num12 = load i32, i32* %num, align 4
  br i1 false, label %trap13, label %cont14

trap13:                                           ; preds = %cont
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([127 x i8], [127 x i8]* @trap_msg.1, i32 0, i32 0), i64 126)
  call void @exit(i32 1)
  unreachable

cont14:                                           ; preds = %cont
  %div = sdiv i32 %num12, 10
  store i32 %div, i32* %num, align 4
  %cur = load i32, i32* %i, align 4
  %add15 = add i32 %cur, 1
  store i32 %add15, i32* %i, align 4
  br label %while.cond

while.cond16:                                     ; preds = %while.body17, %while.end
  %i19 = load i32, i32* %i, align 4
  %gt20 = icmp sgt i32 %i19, 0
  br i1 %gt20, label %while.body17, label %while.end18

while.body17:                                     ; preds = %while.cond16
  %cur21 = load i32, i32* %i, align 4
  %sub22 = sub i32 %cur21, 1
  store i32 %sub22, i32* %i, align 4
  %digits23 = load { i8*, i64 }, { i8*, i64 }* %digits, align 8
  %arr.data24 = extractvalue { i8*, i64 } %digits23, 0
  %i25 = load i32, i32* %i, align 4
  %arr.at26 = getelementptr i8, i8* %arr.data24, i32 %i25
  %idx = load i8, i8* %arr.at26, align 1
  call void @String.push(%String* %s, i8 %idx)
  br label %while.cond16

while.end18:                                      ; preds = %while.cond16
  %arr = load { i8*, i64 }, { i8*, i64 }* %digits, align 8
  %arr.data27 = extractvalue { i8*, i64 } %arr, 0
  call void @free(i8* %arr.data27)
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %digits, align 8
  %s28 = load %String, %String* %s, align 8
  ret %String %s28
}

define void @String.push(%String* %0, i8 %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %c = alloca i8, align 1
  store i8 %1, i8* %c, align 1
  %ref = load %String*, %String** %self, align 8
  %ref1 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref1, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %add = add i32 %count2, 2
  call void @String.grow(%String* %ref, i32 %add)
  %ref3 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref3, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value4, 0
  %ref5 = load %String*, %String** %self, align 8
  %count6 = getelementptr %String, %String* %ref5, i32 0, i32 1
  %count7 = load i32, i32* %count6, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %count7
  %c8 = load i8, i8* %c, align 1
  store i8 %c8, i8* %arr.at, align 1
  %ref9 = load %String*, %String** %self, align 8
  %count10 = getelementptr %String, %String* %ref9, i32 0, i32 1
  %cur = load i32, i32* %count10, align 4
  %add11 = add i32 %cur, 1
  store i32 %add11, i32* %count10, align 4
  %ref12 = load %String*, %String** %self, align 8
  %value13 = getelementptr %String, %String* %ref12, i32 0, i32 0
  %value14 = load { i8*, i64 }, { i8*, i64 }* %value13, align 8
  %arr.data15 = extractvalue { i8*, i64 } %value14, 0
  %ref16 = load %String*, %String** %self, align 8
  %count17 = getelementptr %String, %String* %ref16, i32 0, i32 1
  %count18 = load i32, i32* %count17, align 4
  %arr.at19 = getelementptr i8, i8* %arr.data15, i32 %count18
  store i8 0, i8* %arr.at19, align 1
  ret void
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)

declare void @free(i8*)

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %arr = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %arr, 0
  call void @free(i8* %arr.data)
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %value, align 8
  ret void
}

define i32 @String.cap(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %ref2 = load %String*, %String** %self, align 8
  %value3 = getelementptr %String, %String* %ref2, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value3, align 8
  %len = extractvalue { i8*, i64 } %value4, 1
  %cast = trunc i64 %len to i32
  ret i32 %cast

then:                                             ; preds = %entry
  ret i32 0
}

define void @String.grow(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %want = alloca i32, align 4
  store i32 %1, i32* %want, align 4
  %room = alloca i32, align 4
  %ref = load %String*, %String** %self, align 8
  %call = call i32 @String.cap(%String* %ref)
  store i32 %call, i32* %room, align 4
  %room1 = load i32, i32* %room, align 4
  %want2 = load i32, i32* %want, align 4
  %ge = icmp sge i32 %room1, %want2
  br i1 %ge, label %then, label %endif

endif:                                            ; preds = %entry
  %room5 = load i32, i32* %room, align 4
  %eq = icmp eq i32 %room5, 0
  br i1 %eq, label %then4, label %endif3

then:                                             ; preds = %entry
  ret void

endif3:                                           ; preds = %then4, %endif
  br label %while.cond

then4:                                            ; preds = %endif
  store i32 16, i32* %room, align 4
  br label %endif3

while.cond:                                       ; preds = %while.body, %endif3
  %room6 = load i32, i32* %room, align 4
  %want7 = load i32, i32* %want, align 4
  %lt = icmp slt i32 %room6, %want7
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %cur = load i32, i32* %room, align 4
  %mul = mul i32 %cur, 2
  store i32 %mul, i32* %room, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %bigger = alloca { i8*, i64 }, align 8
  %room8 = load i32, i32* %room, align 4
  %n = sext i32 %room8 to i64
  %heap = call i8* @calloc(i64 %n, i64 1)
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %heap, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %n, 1
  store { i8*, i64 } %arr.len, { i8*, i64 }* %bigger, align 8
  %ref11 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref11, i32 0, i32 1
  %count12 = load i32, i32* %count, align 4
  %gt = icmp sgt i32 %count12, 0
  br i1 %gt, label %then10, label %endif9

endif9:                                           ; preds = %then10, %while.end
  %bigger21 = load { i8*, i64 }, { i8*, i64 }* %bigger, align 8
  %arr.data22 = extractvalue { i8*, i64 } %bigger21, 0
  %ref23 = load %String*, %String** %self, align 8
  %count24 = getelementptr %String, %String* %ref23, i32 0, i32 1
  %count25 = load i32, i32* %count24, align 4
  %arr.at = getelementptr i8, i8* %arr.data22, i32 %count25
  store i8 0, i8* %arr.at, align 1
  %ref26 = load %String*, %String** %self, align 8
  %value27 = getelementptr %String, %String* %ref26, i32 0, i32 0
  %arr = load { i8*, i64 }, { i8*, i64 }* %value27, align 8
  %arr.data28 = extractvalue { i8*, i64 } %arr, 0
  call void @free(i8* %arr.data28)
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %value27, align 8
  %ref29 = load %String*, %String** %self, align 8
  %value30 = getelementptr %String, %String* %ref29, i32 0, i32 0
  %bigger31 = load { i8*, i64 }, { i8*, i64 }* %bigger, align 8
  store { i8*, i64 } %bigger31, { i8*, i64 }* %value30, align 8
  ret void

then10:                                           ; preds = %while.end
  %bigger13 = load { i8*, i64 }, { i8*, i64 }* %bigger, align 8
  %arr.data = extractvalue { i8*, i64 } %bigger13, 0
  %ref14 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref14, i32 0, i32 0
  %value15 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value15, 0
  %isnull = icmp eq i8* %opt.ptr, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str, i32 0, i32 0), i64 0 }, { i8*, i64 } %value15
  %arr.data16 = extractvalue { i8*, i64 } %fallback, 0
  %ref17 = load %String*, %String** %self, align 8
  %count18 = getelementptr %String, %String* %ref17, i32 0, i32 1
  %count19 = load i32, i32* %count18, align 4
  %call20 = call i8* @memcpy(i8* %arr.data, i8* %arr.data16, i32 %count19)
  br label %endif9
}

declare i8* @memcpy(i8*, i8*, i32)

declare i64 @strlen(i8*)

define void @String.join(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr = extractvalue { i8*, i64 } %str1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %n = alloca i32, align 4
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data = extractvalue { i8*, i64 } %str2, 0
  %call = call i64 @strlen(i8* %arr.data)
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n5, 0
  br i1 %eq, label %then4, label %endif3

then:                                             ; preds = %entry
  ret void

endif3:                                           ; preds = %endif
  %ref = load %String*, %String** %self, align 8
  %ref6 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref6, i32 0, i32 1
  %count7 = load i32, i32* %count, align 4
  %n8 = load i32, i32* %n, align 4
  %add = add i32 %count7, %n8
  %add9 = add i32 %add, 1
  call void @String.grow(%String* %ref, i32 %add9)
  %ref10 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref10, i32 0, i32 0
  %value11 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data12 = extractvalue { i8*, i64 } %value11, 0
  %ref13 = load %String*, %String** %self, align 8
  %count14 = getelementptr %String, %String* %ref13, i32 0, i32 1
  %count15 = load i32, i32* %count14, align 4
  %ref16 = load %String*, %String** %self, align 8
  %count17 = getelementptr %String, %String* %ref16, i32 0, i32 1
  %count18 = load i32, i32* %count17, align 4
  %n19 = load i32, i32* %n, align 4
  %add20 = add i32 %count18, %n19
  %start = sext i32 %count15 to i64
  %end = sext i32 %add20 to i64
  %slice.data = getelementptr i8, i8* %arr.data12, i64 %start
  %slice.len = sub i64 %end, %start
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %slice.data, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %slice.len, 1
  %arr.data21 = extractvalue { i8*, i64 } %arr.len, 0
  %str22 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data23 = extractvalue { i8*, i64 } %str22, 0
  %n24 = load i32, i32* %n, align 4
  %call25 = call i8* @memcpy(i8* %arr.data21, i8* %arr.data23, i32 %n24)
  %ref26 = load %String*, %String** %self, align 8
  %count27 = getelementptr %String, %String* %ref26, i32 0, i32 1
  %n28 = load i32, i32* %n, align 4
  %cur = load i32, i32* %count27, align 4
  %add29 = add i32 %cur, %n28
  store i32 %add29, i32* %count27, align 4
  %ref30 = load %String*, %String** %self, align 8
  %value31 = getelementptr %String, %String* %ref30, i32 0, i32 0
  %value32 = load { i8*, i64 }, { i8*, i64 }* %value31, align 8
  %arr.data33 = extractvalue { i8*, i64 } %value32, 0
  %ref34 = load %String*, %String** %self, align 8
  %count35 = getelementptr %String, %String* %ref34, i32 0, i32 1
  %count36 = load i32, i32* %count35, align 4
  %arr.at = getelementptr i8, i8* %arr.data33, i32 %count36
  store i8 0, i8* %arr.at, align 1
  ret void

then4:                                            ; preds = %endif
  ret void
}

define i8 @String.pop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count1 = load i32, i32* %count, align 4
  %eq = icmp eq i32 %count1, 0
  br i1 %eq, label %then, label %endif

endif:                                            ; preds = %entry
  %ref2 = load %String*, %String** %self, align 8
  %count3 = getelementptr %String, %String* %ref2, i32 0, i32 1
  %cur = load i32, i32* %count3, align 4
  %sub = sub i32 %cur, 1
  store i32 %sub, i32* %count3, align 4
  %c = alloca i8, align 1
  %ref4 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref4, i32 0, i32 0
  %value5 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value5, 0
  %ref6 = load %String*, %String** %self, align 8
  %count7 = getelementptr %String, %String* %ref6, i32 0, i32 1
  %count8 = load i32, i32* %count7, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %count8
  %idx = load i8, i8* %arr.at, align 1
  store i8 %idx, i8* %c, align 1
  %ref9 = load %String*, %String** %self, align 8
  %value10 = getelementptr %String, %String* %ref9, i32 0, i32 0
  %value11 = load { i8*, i64 }, { i8*, i64 }* %value10, align 8
  %arr.data12 = extractvalue { i8*, i64 } %value11, 0
  %ref13 = load %String*, %String** %self, align 8
  %count14 = getelementptr %String, %String* %ref13, i32 0, i32 1
  %count15 = load i32, i32* %count14, align 4
  %arr.at16 = getelementptr i8, i8* %arr.data12, i32 %count15
  store i8 0, i8* %arr.at16, align 1
  %c17 = load i8, i8* %c, align 1
  ret i8 %c17

then:                                             ; preds = %entry
  ret i8 0
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  store i32 0, i32* %count, align 4
  %ref1 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref1, i32 0, i32 0
  %value2 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value2, 0
  %nullcmp = icmp ne i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %then, %entry
  ret void

then:                                             ; preds = %entry
  %ref3 = load %String*, %String** %self, align 8
  %value4 = getelementptr %String, %String* %ref3, i32 0, i32 0
  %value5 = load { i8*, i64 }, { i8*, i64 }* %value4, align 8
  %arr.data = extractvalue { i8*, i64 } %value5, 0
  %arr.at = getelementptr i8, i8* %arr.data, i32 0
  store i8 0, i8* %arr.at, align 1
  br label %endif
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count1 = load i32, i32* %count, align 4
  ret i32 %count1
}

define i1 @String.empty(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count1 = load i32, i32* %count, align 4
  %eq = icmp eq i32 %count1, 0
  ret i1 %eq
}

define { i8*, i64 } @String.c_str(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value1, 0
  %isnull = icmp eq i8* %opt.ptr, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.4, i32 0, i32 0), i64 0 }, { i8*, i64 } %value1
  ret { i8*, i64 } %fallback
}

define i8 @String.at(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %i = alloca i32, align 4
  store i32 %1, i32* %i, align 4
  %i1 = load i32, i32* %i, align 4
  %lt = icmp slt i32 %i1, 0
  %i2 = load i32, i32* %i, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count3 = load i32, i32* %count, align 4
  %ge = icmp sge i32 %i2, %count3
  %or = or i1 %lt, %ge
  br i1 %or, label %then, label %endif

endif:                                            ; preds = %entry
  %ref4 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref4, i32 0, i32 0
  %value5 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value5, 0
  %i6 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %i6
  %idx = load i8, i8* %arr.at, align 1
  ret i8 %idx

then:                                             ; preds = %entry
  ret i8 0
}

define i32 @String.find(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %needle = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %needle, align 8
  %needle1 = load { i8*, i64 }, { i8*, i64 }* %needle, align 8
  %opt.ptr = extractvalue { i8*, i64 } %needle1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %n = alloca i32, align 4
  %needle2 = load { i8*, i64 }, { i8*, i64 }* %needle, align 8
  %arr.data = extractvalue { i8*, i64 } %needle2, 0
  %call = call i64 @strlen(i8* %arr.data)
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n5, 0
  br i1 %eq, label %then4, label %endif3

then:                                             ; preds = %entry
  ret i32 -1

endif3:                                           ; preds = %endif
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then4:                                            ; preds = %endif
  ret i32 0

while.cond:                                       ; preds = %endif9, %endif3
  %i6 = load i32, i32* %i, align 4
  %n7 = load i32, i32* %n, align 4
  %add = add i32 %i6, %n7
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count8 = load i32, i32* %count, align 4
  %le = icmp sle i32 %add, %count8
  br i1 %le, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref11 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref11, i32 0, i32 0
  %value12 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data13 = extractvalue { i8*, i64 } %value12, 0
  %i14 = load i32, i32* %i, align 4
  %i15 = load i32, i32* %i, align 4
  %n16 = load i32, i32* %n, align 4
  %add17 = add i32 %i15, %n16
  %start = sext i32 %i14 to i64
  %end = sext i32 %add17 to i64
  %slice.data = getelementptr i8, i8* %arr.data13, i64 %start
  %slice.len = sub i64 %end, %start
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %slice.data, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %slice.len, 1
  %arr.data18 = extractvalue { i8*, i64 } %arr.len, 0
  %needle19 = load { i8*, i64 }, { i8*, i64 }* %needle, align 8
  %arr.data20 = extractvalue { i8*, i64 } %needle19, 0
  %n21 = load i32, i32* %n, align 4
  %call22 = call i32 @strncmp(i8* %arr.data18, i8* %arr.data20, i32 %n21)
  %eq23 = icmp eq i32 %call22, 0
  br i1 %eq23, label %then10, label %endif9

while.end:                                        ; preds = %while.cond
  ret i32 -1

endif9:                                           ; preds = %while.body
  %cur = load i32, i32* %i, align 4
  %add25 = add i32 %cur, 1
  store i32 %add25, i32* %i, align 4
  br label %while.cond

then10:                                           ; preds = %while.body
  %i24 = load i32, i32* %i, align 4
  ret i32 %i24
}

declare i32 @strncmp(i8*, i8*, i32)

define i1 @String.contains(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %needle = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %needle, align 8
  %ref = load %String*, %String** %self, align 8
  %needle1 = load { i8*, i64 }, { i8*, i64 }* %needle, align 8
  %call = call i32 @String.find(%String* %ref, { i8*, i64 } %needle1)
  %ne = icmp ne i32 %call, -1
  ret i1 %ne
}

define i1 @String.starts_with(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %prefix = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %prefix, align 8
  %prefix1 = load { i8*, i64 }, { i8*, i64 }* %prefix, align 8
  %opt.ptr = extractvalue { i8*, i64 } %prefix1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %n = alloca i32, align 4
  %prefix2 = load { i8*, i64 }, { i8*, i64 }* %prefix, align 8
  %arr.data = extractvalue { i8*, i64 } %prefix2, 0
  %call = call i64 @strlen(i8* %arr.data)
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count6 = load i32, i32* %count, align 4
  %gt = icmp sgt i32 %n5, %count6
  br i1 %gt, label %then4, label %endif3

then:                                             ; preds = %entry
  ret i1 true

endif3:                                           ; preds = %endif
  %ref7 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref7, i32 0, i32 0
  %value8 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data9 = extractvalue { i8*, i64 } %value8, 0
  %n10 = load i32, i32* %n, align 4
  %end = sext i32 %n10 to i64
  %slice.data = getelementptr i8, i8* %arr.data9, i64 0
  %slice.len = sub i64 %end, 0
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %slice.data, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %slice.len, 1
  %arr.data11 = extractvalue { i8*, i64 } %arr.len, 0
  %prefix12 = load { i8*, i64 }, { i8*, i64 }* %prefix, align 8
  %arr.data13 = extractvalue { i8*, i64 } %prefix12, 0
  %n14 = load i32, i32* %n, align 4
  %call15 = call i32 @strncmp(i8* %arr.data11, i8* %arr.data13, i32 %n14)
  %eq = icmp eq i32 %call15, 0
  ret i1 %eq

then4:                                            ; preds = %endif
  ret i1 false
}

define i1 @String.ends_with(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %suffix = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %suffix, align 8
  %suffix1 = load { i8*, i64 }, { i8*, i64 }* %suffix, align 8
  %opt.ptr = extractvalue { i8*, i64 } %suffix1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %n = alloca i32, align 4
  %suffix2 = load { i8*, i64 }, { i8*, i64 }* %suffix, align 8
  %arr.data = extractvalue { i8*, i64 } %suffix2, 0
  %call = call i64 @strlen(i8* %arr.data)
  %cast = trunc i64 %call to i32
  store i32 %cast, i32* %n, align 4
  %n5 = load i32, i32* %n, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count6 = load i32, i32* %count, align 4
  %gt = icmp sgt i32 %n5, %count6
  br i1 %gt, label %then4, label %endif3

then:                                             ; preds = %entry
  ret i1 true

endif3:                                           ; preds = %endif
  %ref7 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref7, i32 0, i32 0
  %value8 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data9 = extractvalue { i8*, i64 } %value8, 0
  %ref10 = load %String*, %String** %self, align 8
  %count11 = getelementptr %String, %String* %ref10, i32 0, i32 1
  %count12 = load i32, i32* %count11, align 4
  %n13 = load i32, i32* %n, align 4
  %sub = sub i32 %count12, %n13
  %ref14 = load %String*, %String** %self, align 8
  %count15 = getelementptr %String, %String* %ref14, i32 0, i32 1
  %count16 = load i32, i32* %count15, align 4
  %start = sext i32 %sub to i64
  %end = sext i32 %count16 to i64
  %slice.data = getelementptr i8, i8* %arr.data9, i64 %start
  %slice.len = sub i64 %end, %start
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %slice.data, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %slice.len, 1
  %arr.data17 = extractvalue { i8*, i64 } %arr.len, 0
  %suffix18 = load { i8*, i64 }, { i8*, i64 }* %suffix, align 8
  %arr.data19 = extractvalue { i8*, i64 } %suffix18, 0
  %n20 = load i32, i32* %n, align 4
  %call21 = call i32 @strncmp(i8* %arr.data17, i8* %arr.data19, i32 %n20)
  %eq = icmp eq i32 %call21, 0
  ret i1 %eq

then4:                                            ; preds = %endif
  ret i1 false
}

define %String @String.substr(%String* %0, i32 %1, i32 %2) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %start = alloca i32, align 4
  store i32 %1, i32* %start, align 4
  %length = alloca i32, align 4
  store i32 %2, i32* %length, align 4
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %start1 = load i32, i32* %start, align 4
  %lt = icmp slt i32 %start1, 0
  %start2 = load i32, i32* %start, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count3 = load i32, i32* %count, align 4
  %ge = icmp sge i32 %start2, %count3
  %or = or i1 %lt, %ge
  br i1 %or, label %then, label %endif

endif:                                            ; preds = %entry
  %stop = alloca i32, align 4
  %start5 = load i32, i32* %start, align 4
  %length6 = load i32, i32* %length, align 4
  %add = add i32 %start5, %length6
  store i32 %add, i32* %stop, align 4
  %stop9 = load i32, i32* %stop, align 4
  %ref10 = load %String*, %String** %self, align 8
  %count11 = getelementptr %String, %String* %ref10, i32 0, i32 1
  %count12 = load i32, i32* %count11, align 4
  %gt = icmp sgt i32 %stop9, %count12
  br i1 %gt, label %then8, label %endif7

then:                                             ; preds = %entry
  %res4 = load %String, %String* %res, align 8
  ret %String %res4

endif7:                                           ; preds = %then8, %endif
  %i = alloca i32, align 4
  %start16 = load i32, i32* %start, align 4
  store i32 %start16, i32* %i, align 4
  br label %while.cond

then8:                                            ; preds = %endif
  %ref13 = load %String*, %String** %self, align 8
  %count14 = getelementptr %String, %String* %ref13, i32 0, i32 1
  %count15 = load i32, i32* %count14, align 4
  store i32 %count15, i32* %stop, align 4
  br label %endif7

while.cond:                                       ; preds = %while.body, %endif7
  %i17 = load i32, i32* %i, align 4
  %stop18 = load i32, i32* %stop, align 4
  %lt19 = icmp slt i32 %i17, %stop18
  br i1 %lt19, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref20 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref20, i32 0, i32 0
  %value21 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value21, 0
  %i22 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %i22
  %idx = load i8, i8* %arr.at, align 1
  call void @String.push(%String* %res, i8 %idx)
  %cur = load i32, i32* %i, align 4
  %add23 = add i32 %cur, 1
  store i32 %add23, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %res24 = load %String, %String* %res, align 8
  ret %String %res24
}

define %String @String.upper(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %endif, %entry
  %i1 = load i32, i32* %i, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %lt = icmp slt i32 %i1, %count2
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %c = alloca i8, align 1
  %ref3 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref3, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value4, 0
  %i5 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %i5
  %idx = load i8, i8* %arr.at, align 1
  store i8 %idx, i8* %c, align 1
  %c6 = load i8, i8* %c, align 1
  %ge = icmp sge i8 %c6, 97
  %c7 = load i8, i8* %c, align 1
  %le = icmp sle i8 %c7, 122
  %and = and i1 %ge, %le
  br i1 %and, label %then, label %next

while.end:                                        ; preds = %while.cond
  %res11 = load %String, %String* %res, align 8
  ret %String %res11

endif:                                            ; preds = %next, %then
  %cur = load i32, i32* %i, align 4
  %add = add i32 %cur, 1
  store i32 %add, i32* %i, align 4
  br label %while.cond

then:                                             ; preds = %while.body
  %c8 = load i8, i8* %c, align 1
  %cast = sext i8 %c8 to i32
  %sub = sub i32 %cast, 32
  %cast9 = trunc i32 %sub to i8
  call void @String.push(%String* %res, i8 %cast9)
  br label %endif

next:                                             ; preds = %while.body
  %c10 = load i8, i8* %c, align 1
  call void @String.push(%String* %res, i8 %c10)
  br label %endif
}

define %String @String.lower(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %endif, %entry
  %i1 = load i32, i32* %i, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %lt = icmp slt i32 %i1, %count2
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %c = alloca i8, align 1
  %ref3 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref3, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value4, 0
  %i5 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %i5
  %idx = load i8, i8* %arr.at, align 1
  store i8 %idx, i8* %c, align 1
  %c6 = load i8, i8* %c, align 1
  %ge = icmp sge i8 %c6, 65
  %c7 = load i8, i8* %c, align 1
  %le = icmp sle i8 %c7, 90
  %and = and i1 %ge, %le
  br i1 %and, label %then, label %next

while.end:                                        ; preds = %while.cond
  %res12 = load %String, %String* %res, align 8
  ret %String %res12

endif:                                            ; preds = %next, %then
  %cur = load i32, i32* %i, align 4
  %add11 = add i32 %cur, 1
  store i32 %add11, i32* %i, align 4
  br label %while.cond

then:                                             ; preds = %while.body
  %c8 = load i8, i8* %c, align 1
  %cast = sext i8 %c8 to i32
  %add = add i32 %cast, 32
  %cast9 = trunc i32 %add to i8
  call void @String.push(%String* %res, i8 %cast9)
  br label %endif

next:                                             ; preds = %while.body
  %c10 = load i8, i8* %c, align 1
  call void @String.push(%String* %res, i8 %c10)
  br label %endif
}

define %String @String.trim(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %start = alloca i32, align 4
  store i32 0, i32* %start, align 4
  br label %while.cond

while.cond:                                       ; preds = %endif, %entry
  %start1 = load i32, i32* %start, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %lt = icmp slt i32 %start1, %count2
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %c = alloca i8, align 1
  %ref3 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref3, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value4, 0
  %start5 = load i32, i32* %start, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %start5
  %idx = load i8, i8* %arr.at, align 1
  store i8 %idx, i8* %c, align 1
  %c6 = load i8, i8* %c, align 1
  %eq = icmp eq i8 %c6, 32
  %c7 = load i8, i8* %c, align 1
  %eq8 = icmp eq i8 %c7, 9
  %or = or i1 %eq, %eq8
  %c9 = load i8, i8* %c, align 1
  %eq10 = icmp eq i8 %c9, 10
  %or11 = or i1 %or, %eq10
  %c12 = load i8, i8* %c, align 1
  %eq13 = icmp eq i8 %c12, 13
  %or14 = or i1 %or11, %eq13
  br i1 %or14, label %then, label %next

while.end:                                        ; preds = %next, %while.cond
  %stop = alloca i32, align 4
  %ref15 = load %String*, %String** %self, align 8
  %count16 = getelementptr %String, %String* %ref15, i32 0, i32 1
  %count17 = load i32, i32* %count16, align 4
  store i32 %count17, i32* %stop, align 4
  br label %while.cond18

endif:                                            ; preds = %then
  br label %while.cond

then:                                             ; preds = %while.body
  %cur = load i32, i32* %start, align 4
  %add = add i32 %cur, 1
  store i32 %add, i32* %start, align 4
  br label %endif

next:                                             ; preds = %while.body
  br label %while.end

while.cond18:                                     ; preds = %endif31, %while.end
  %stop21 = load i32, i32* %stop, align 4
  %start22 = load i32, i32* %start, align 4
  %gt = icmp sgt i32 %stop21, %start22
  br i1 %gt, label %while.body19, label %while.end20

while.body19:                                     ; preds = %while.cond18
  %c23 = alloca i8, align 1
  %ref24 = load %String*, %String** %self, align 8
  %value25 = getelementptr %String, %String* %ref24, i32 0, i32 0
  %value26 = load { i8*, i64 }, { i8*, i64 }* %value25, align 8
  %arr.data27 = extractvalue { i8*, i64 } %value26, 0
  %stop28 = load i32, i32* %stop, align 4
  %sub = sub i32 %stop28, 1
  %arr.at29 = getelementptr i8, i8* %arr.data27, i32 %sub
  %idx30 = load i8, i8* %arr.at29, align 1
  store i8 %idx30, i8* %c23, align 1
  %c34 = load i8, i8* %c23, align 1
  %eq35 = icmp eq i8 %c34, 32
  %c36 = load i8, i8* %c23, align 1
  %eq37 = icmp eq i8 %c36, 9
  %or38 = or i1 %eq35, %eq37
  %c39 = load i8, i8* %c23, align 1
  %eq40 = icmp eq i8 %c39, 10
  %or41 = or i1 %or38, %eq40
  %c42 = load i8, i8* %c23, align 1
  %eq43 = icmp eq i8 %c42, 13
  %or44 = or i1 %or41, %eq43
  br i1 %or44, label %then32, label %next33

while.end20:                                      ; preds = %next33, %while.cond18
  %ref47 = load %String*, %String** %self, align 8
  %start48 = load i32, i32* %start, align 4
  %stop49 = load i32, i32* %stop, align 4
  %start50 = load i32, i32* %start, align 4
  %sub51 = sub i32 %stop49, %start50
  %call = call %String @String.substr(%String* %ref47, i32 %start48, i32 %sub51)
  ret %String %call

endif31:                                          ; preds = %then32
  br label %while.cond18

then32:                                           ; preds = %while.body19
  %cur45 = load i32, i32* %stop, align 4
  %sub46 = sub i32 %cur45, 1
  store i32 %sub46, i32* %stop, align 4
  br label %endif31

next33:                                           ; preds = %while.body19
  br label %while.end20
}

define %String @String.replace(%String* %0, { i8*, i64 } %1, { i8*, i64 } %2) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %old = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %old, align 8
  %fresh = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %2, { i8*, i64 }* %fresh, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %old1 = load { i8*, i64 }, { i8*, i64 }* %old, align 8
  %opt.ptr = extractvalue { i8*, i64 } %old1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %n = alloca i32, align 4
  %old4 = load { i8*, i64 }, { i8*, i64 }* %old, align 8
  %arr.data = extractvalue { i8*, i64 } %old4, 0
  %call5 = call i64 @strlen(i8* %arr.data)
  %cast = trunc i64 %call5 to i32
  store i32 %cast, i32* %n, align 4
  %n8 = load i32, i32* %n, align 4
  %eq = icmp eq i32 %n8, 0
  br i1 %eq, label %then7, label %endif6

then:                                             ; preds = %entry
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value2 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  call void @String.assign(%String* %res, { i8*, i64 } %value2)
  %res3 = load %String, %String* %res, align 8
  ret %String %res3

endif6:                                           ; preds = %endif
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then7:                                            ; preds = %endif
  %ref9 = load %String*, %String** %self, align 8
  %value10 = getelementptr %String, %String* %ref9, i32 0, i32 0
  %value11 = load { i8*, i64 }, { i8*, i64 }* %value10, align 8
  call void @String.assign(%String* %res, { i8*, i64 } %value11)
  %res12 = load %String, %String* %res, align 8
  ret %String %res12

while.cond:                                       ; preds = %endif37, %endif6
  %i13 = load i32, i32* %i, align 4
  %ref14 = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref14, i32 0, i32 1
  %count15 = load i32, i32* %count, align 4
  %lt = icmp slt i32 %i13, %count15
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %hit = alloca i1, align 1
  store i1 false, i1* %hit, align 1
  %i18 = load i32, i32* %i, align 4
  %n19 = load i32, i32* %n, align 4
  %add = add i32 %i18, %n19
  %ref20 = load %String*, %String** %self, align 8
  %count21 = getelementptr %String, %String* %ref20, i32 0, i32 1
  %count22 = load i32, i32* %count21, align 4
  %le = icmp sle i32 %add, %count22
  br i1 %le, label %then17, label %endif16

while.end:                                        ; preds = %while.cond
  %res50 = load %String, %String* %res, align 8
  ret %String %res50

endif16:                                          ; preds = %then17, %while.body
  %hit39 = load i1, i1* %hit, align 1
  br i1 %hit39, label %then38, label %next

then17:                                           ; preds = %while.body
  %ref23 = load %String*, %String** %self, align 8
  %value24 = getelementptr %String, %String* %ref23, i32 0, i32 0
  %value25 = load { i8*, i64 }, { i8*, i64 }* %value24, align 8
  %arr.data26 = extractvalue { i8*, i64 } %value25, 0
  %i27 = load i32, i32* %i, align 4
  %i28 = load i32, i32* %i, align 4
  %n29 = load i32, i32* %n, align 4
  %add30 = add i32 %i28, %n29
  %start = sext i32 %i27 to i64
  %end = sext i32 %add30 to i64
  %slice.data = getelementptr i8, i8* %arr.data26, i64 %start
  %slice.len = sub i64 %end, %start
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %slice.data, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 %slice.len, 1
  %arr.data31 = extractvalue { i8*, i64 } %arr.len, 0
  %old32 = load { i8*, i64 }, { i8*, i64 }* %old, align 8
  %arr.data33 = extractvalue { i8*, i64 } %old32, 0
  %n34 = load i32, i32* %n, align 4
  %call35 = call i32 @strncmp(i8* %arr.data31, i8* %arr.data33, i32 %n34)
  %eq36 = icmp eq i32 %call35, 0
  store i1 %eq36, i1* %hit, align 1
  br label %endif16

endif37:                                          ; preds = %next, %then38
  br label %while.cond

then38:                                           ; preds = %endif16
  %fresh40 = load { i8*, i64 }, { i8*, i64 }* %fresh, align 8
  call void @String.join(%String* %res, { i8*, i64 } %fresh40)
  %n41 = load i32, i32* %n, align 4
  %cur = load i32, i32* %i, align 4
  %add42 = add i32 %cur, %n41
  store i32 %add42, i32* %i, align 4
  br label %endif37

next:                                             ; preds = %endif16
  %ref43 = load %String*, %String** %self, align 8
  %value44 = getelementptr %String, %String* %ref43, i32 0, i32 0
  %value45 = load { i8*, i64 }, { i8*, i64 }* %value44, align 8
  %arr.data46 = extractvalue { i8*, i64 } %value45, 0
  %i47 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data46, i32 %i47
  %idx = load i8, i8* %arr.at, align 1
  call void @String.push(%String* %res, i8 %idx)
  %cur48 = load i32, i32* %i, align 4
  %add49 = add i32 %cur48, 1
  store i32 %add49, i32* %i, align 4
  br label %endif37
}

define %String @String.repeat(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %i1 = load i32, i32* %i, align 4
  %n2 = load i32, i32* %n, align 4
  %lt = icmp slt i32 %i1, %n2
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value3 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  call void @String.join(%String* %res, { i8*, i64 } %value3)
  %cur = load i32, i32* %i, align 4
  %add = add i32 %cur, 1
  store i32 %add, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %res4 = load %String, %String* %res, align 8
  ret %String %res4
}

define %String @String.reverse(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %res = alloca %String, align 8
  %call = call %String @String.create()
  store %String %call, %String* %res, align 8
  %i = alloca i32, align 4
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count1 = load i32, i32* %count, align 4
  %sub = sub i32 %count1, 1
  store i32 %sub, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %i2 = load i32, i32* %i, align 4
  %ge = icmp sge i32 %i2, 0
  br i1 %ge, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref3 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref3, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %arr.data = extractvalue { i8*, i64 } %value4, 0
  %i5 = load i32, i32* %i, align 4
  %arr.at = getelementptr i8, i8* %arr.data, i32 %i5
  %idx = load i8, i8* %arr.at, align 1
  call void @String.push(%String* %res, i8 %idx)
  %cur = load i32, i32* %i, align 4
  %sub6 = sub i32 %cur, 1
  store i32 %sub6, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %res7 = load %String, %String* %res, align 8
  ret %String %res7
}

define i32 @String.compare(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %other = alloca %String*, align 8
  store %String* %1, %String** %other, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value1, 0
  %isnull = icmp eq i8* %opt.ptr, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.5, i32 0, i32 0), i64 0 }, { i8*, i64 } %value1
  %arr.data = extractvalue { i8*, i64 } %fallback, 0
  %ref2 = load %String*, %String** %other, align 8
  %value3 = getelementptr %String, %String* %ref2, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value3, align 8
  %opt.ptr5 = extractvalue { i8*, i64 } %value4, 0
  %isnull6 = icmp eq i8* %opt.ptr5, null
  %fallback7 = select i1 %isnull6, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.6, i32 0, i32 0), i64 0 }, { i8*, i64 } %value4
  %arr.data8 = extractvalue { i8*, i64 } %fallback7, 0
  %call = call i32 @strcmp(i8* %arr.data, i8* %arr.data8)
  ret i32 %call
}

declare i32 @strcmp(i8*, i8*)

define i32 @String.to_int(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value1, 0
  %isnull = icmp eq i8* %opt.ptr, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.7, i32 0, i32 0), i64 0 }, { i8*, i64 } %value1
  %arr.data = extractvalue { i8*, i64 } %fallback, 0
  %call = call i32 @atoi(i8* %arr.data)
  ret i32 %call
}

declare i32 @atoi(i8*)

define void @"String.=.array"(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %ref = load %String*, %String** %self, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  call void @String.assign(%String* %ref, { i8*, i64 } %str1)
  ret void
}

define void @"String.=.ref.String"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %v = alloca %String*, align 8
  store %String* %1, %String** %v, align 8
  %ref = load %String*, %String** %self, align 8
  %ref1 = load %String*, %String** %v, align 8
  %value = getelementptr %String, %String* %ref1, i32 0, i32 0
  %value2 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  call void @String.assign(%String* %ref, { i8*, i64 } %value2)
  ret void
}

define %String @"String.+.ref.String"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %v = alloca %String*, align 8
  store %String* %1, %String** %v, align 8
  %res = alloca %String, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %call = call %String @String.from({ i8*, i64 } %value1)
  store %String %call, %String* %res, align 8
  %ref2 = load %String*, %String** %v, align 8
  %value3 = getelementptr %String, %String* %ref2, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value3, align 8
  call void @String.join(%String* %res, { i8*, i64 } %value4)
  %res5 = load %String, %String* %res, align 8
  ret %String %res5
}

define %String @"String.+.array"(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %res = alloca %String, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %call = call %String @String.from({ i8*, i64 } %value1)
  store %String %call, %String* %res, align 8
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  call void @String.join(%String* %res, { i8*, i64 } %str2)
  %res3 = load %String, %String* %res, align 8
  ret %String %res3
}

define void @"String.+=.ref.String"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %v = alloca %String*, align 8
  store %String* %1, %String** %v, align 8
  %ref = load %String*, %String** %self, align 8
  %ref1 = load %String*, %String** %v, align 8
  %value = getelementptr %String, %String* %ref1, i32 0, i32 0
  %value2 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  call void @String.join(%String* %ref, { i8*, i64 } %value2)
  ret void
}

define void @"String.+=.array"(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %ref = load %String*, %String** %self, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  call void @String.join(%String* %ref, { i8*, i64 } %str1)
  ret void
}

define i1 @"String.==.ref.String"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %v = alloca %String*, align 8
  store %String* %1, %String** %v, align 8
  %ref = load %String*, %String** %self, align 8
  %ref1 = load %String*, %String** %v, align 8
  %call = call i32 @String.compare(%String* %ref, %String* %ref1)
  %eq = icmp eq i32 %call, 0
  ret i1 %eq
}

define i1 @"String.!=.ref.String"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %v = alloca %String*, align 8
  store %String* %1, %String** %v, align 8
  %ref = load %String*, %String** %self, align 8
  %ref1 = load %String*, %String** %v, align 8
  %call = call i32 @String.compare(%String* %ref, %String* %ref1)
  %ne = icmp ne i32 %call, 0
  ret i1 %ne
}

define i1 @"String.==.array"(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr = extractvalue { i8*, i64 } %str1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %ref3 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref3, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr5 = extractvalue { i8*, i64 } %value4, 0
  %isnull = icmp eq i8* %opt.ptr5, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.8, i32 0, i32 0), i64 0 }, { i8*, i64 } %value4
  %arr.data = extractvalue { i8*, i64 } %fallback, 0
  %str6 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data7 = extractvalue { i8*, i64 } %str6, 0
  %call = call i32 @strcmp(i8* %arr.data, i8* %arr.data7)
  %eq8 = icmp eq i32 %call, 0
  ret i1 %eq8

then:                                             ; preds = %entry
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %eq = icmp eq i32 %count2, 0
  ret i1 %eq
}

define i1 @"String.!=.array"(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %str1 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr = extractvalue { i8*, i64 } %str1, 0
  %nullcmp = icmp eq i8* %opt.ptr, null
  br i1 %nullcmp, label %then, label %endif

endif:                                            ; preds = %entry
  %ref3 = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref3, i32 0, i32 0
  %value4 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr5 = extractvalue { i8*, i64 } %value4, 0
  %isnull = icmp eq i8* %opt.ptr5, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.9, i32 0, i32 0), i64 0 }, { i8*, i64 } %value4
  %arr.data = extractvalue { i8*, i64 } %fallback, 0
  %str6 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %arr.data7 = extractvalue { i8*, i64 } %str6, 0
  %call = call i32 @strcmp(i8* %arr.data, i8* %arr.data7)
  %ne8 = icmp ne i32 %call, 0
  ret i1 %ne8

then:                                             ; preds = %entry
  %ref = load %String*, %String** %self, align 8
  %count = getelementptr %String, %String* %ref, i32 0, i32 1
  %count2 = load i32, i32* %count, align 4
  %ne = icmp ne i32 %count2, 0
  ret i1 %ne
}

define i1 @"String.<.ref.String"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %v = alloca %String*, align 8
  store %String* %1, %String** %v, align 8
  %ref = load %String*, %String** %self, align 8
  %ref1 = load %String*, %String** %v, align 8
  %call = call i32 @String.compare(%String* %ref, %String* %ref1)
  %lt = icmp slt i32 %call, 0
  ret i1 %lt
}

define i1 @"String.>.ref.String"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %v = alloca %String*, align 8
  store %String* %1, %String** %v, align 8
  %ref = load %String*, %String** %self, align 8
  %ref1 = load %String*, %String** %v, align 8
  %call = call i32 @String.compare(%String* %ref, %String* %ref1)
  %gt = icmp sgt i32 %call, 0
  ret i1 %gt
}

define i1 @"String.<.array"(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value1, 0
  %isnull = icmp eq i8* %opt.ptr, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.10, i32 0, i32 0), i64 0 }, { i8*, i64 } %value1
  %arr.data = extractvalue { i8*, i64 } %fallback, 0
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr3 = extractvalue { i8*, i64 } %str2, 0
  %isnull4 = icmp eq i8* %opt.ptr3, null
  %fallback5 = select i1 %isnull4, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.11, i32 0, i32 0), i64 0 }, { i8*, i64 } %str2
  %arr.data6 = extractvalue { i8*, i64 } %fallback5, 0
  %call = call i32 @strcmp(i8* %arr.data, i8* %arr.data6)
  %lt = icmp slt i32 %call, 0
  ret i1 %lt
}

define i1 @"String.>.array"(%String* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %str = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %str, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value1, 0
  %isnull = icmp eq i8* %opt.ptr, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.12, i32 0, i32 0), i64 0 }, { i8*, i64 } %value1
  %arr.data = extractvalue { i8*, i64 } %fallback, 0
  %str2 = load { i8*, i64 }, { i8*, i64 }* %str, align 8
  %opt.ptr3 = extractvalue { i8*, i64 } %str2, 0
  %isnull4 = icmp eq i8* %opt.ptr3, null
  %fallback5 = select i1 %isnull4, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.13, i32 0, i32 0), i64 0 }, { i8*, i64 } %str2
  %arr.data6 = extractvalue { i8*, i64 } %fallback5, 0
  %call = call i32 @strcmp(i8* %arr.data, i8* %arr.data6)
  %gt = icmp sgt i32 %call, 0
  ret i1 %gt
}

define { i8*, i64 } @String.output(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %ref = load %String*, %String** %self, align 8
  %value = getelementptr %String, %String* %ref, i32 0, i32 0
  %value1 = load { i8*, i64 }, { i8*, i64 }* %value, align 8
  %opt.ptr = extractvalue { i8*, i64 } %value1, 0
  %isnull = icmp eq i8* %opt.ptr, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.14, i32 0, i32 0), i64 0 }, { i8*, i64 } %value1
  ret { i8*, i64 } %fallback
}

define %Hero @Hero.create() {
entry:
  %h = alloca %Hero, align 8
  store %Hero zeroinitializer, %Hero* %h, align 8
  %name = getelementptr %Hero, %Hero* %h, i32 0, i32 0
  %heap = call i8* @calloc(i64 32, i64 1)
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %heap, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 32, 1
  store { i8*, i64 } %arr.len, { i8*, i64 }* %name, align 8
  %title = getelementptr %Hero, %Hero* %h, i32 0, i32 1
  %heap1 = call i8* @calloc(i64 32, i64 1)
  %arr.ptr2 = insertvalue { i8*, i64 } undef, i8* %heap1, 0
  %arr.len3 = insertvalue { i8*, i64 } %arr.ptr2, i64 32, 1
  store { i8*, i64 } %arr.len3, { i8*, i64 }* %title, align 8
  %h4 = load %Hero, %Hero* %h, align 8
  ret %Hero %h4
}

define void @Hero.setup(%Hero* %0, { i8*, i64 } %1, { i8*, i64 } %2) {
entry:
  %self = alloca %Hero*, align 8
  store %Hero* %0, %Hero** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %title = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %2, { i8*, i64 }* %title, align 8
  %ref = load %Hero*, %Hero** %self, align 8
  %name1 = getelementptr %Hero, %Hero* %ref, i32 0, i32 0
  %name2 = load { i8*, i64 }, { i8*, i64 }* %name1, align 8
  %arr.data = extractvalue { i8*, i64 } %name2, 0
  %name3 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data4 = extractvalue { i8*, i64 } %name3, 0
  %call = call i8* @strcpy(i8* %arr.data, i8* %arr.data4)
  %ref5 = load %Hero*, %Hero** %self, align 8
  %title6 = getelementptr %Hero, %Hero* %ref5, i32 0, i32 1
  %title7 = load { i8*, i64 }, { i8*, i64 }* %title6, align 8
  %arr.data8 = extractvalue { i8*, i64 } %title7, 0
  %title9 = load { i8*, i64 }, { i8*, i64 }* %title, align 8
  %arr.data10 = extractvalue { i8*, i64 } %title9, 0
  %call11 = call i8* @strcpy(i8* %arr.data8, i8* %arr.data10)
  ret void
}

declare i8* @strcpy(i8*, i8*)

define void @Hero.announce(%Hero* %0) {
entry:
  %self = alloca %Hero*, align 8
  store %Hero* %0, %Hero** %self, align 8
  %ref = load %Hero*, %Hero** %self, align 8
  %title = getelementptr %Hero, %Hero* %ref, i32 0, i32 1
  %title1 = load { i8*, i64 }, { i8*, i64 }* %title, align 8
  %str.len = extractvalue { i8*, i64 } %title1, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %title1, 0
  %ref2 = load %Hero*, %Hero** %self, align 8
  %name = getelementptr %Hero, %Hero* %ref2, i32 0, i32 0
  %name3 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %str.len4 = extractvalue { i8*, i64 } %name3, 1
  %len325 = trunc i64 %str.len4 to i32
  %str.data6 = extractvalue { i8*, i64 } %name3, 0
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt, i32 0, i32 0), i32 %len32, i8* %str.data, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.15, i32 0, i32 0), i32 %len325, i8* %str.data6, i32 9, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.16, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)

define void @"Hero.=.ref.Hero"(%Hero* %0, %Hero* %1) {
entry:
  %self = alloca %Hero*, align 8
  store %Hero* %0, %Hero** %self, align 8
  %other = alloca %Hero*, align 8
  store %Hero* %1, %Hero** %other, align 8
  %ref = load %Hero*, %Hero** %self, align 8
  %name = getelementptr %Hero, %Hero* %ref, i32 0, i32 0
  %arr = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %arr, 0
  call void @free(i8* %arr.data)
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %name, align 8
  %ref1 = load %Hero*, %Hero** %self, align 8
  %title = getelementptr %Hero, %Hero* %ref1, i32 0, i32 1
  %arr2 = load { i8*, i64 }, { i8*, i64 }* %title, align 8
  %arr.data3 = extractvalue { i8*, i64 } %arr2, 0
  call void @free(i8* %arr.data3)
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %title, align 8
  %ref4 = load %Hero*, %Hero** %self, align 8
  %name5 = getelementptr %Hero, %Hero* %ref4, i32 0, i32 0
  %heap = call i8* @calloc(i64 32, i64 1)
  %arr.ptr = insertvalue { i8*, i64 } undef, i8* %heap, 0
  %arr.len = insertvalue { i8*, i64 } %arr.ptr, i64 32, 1
  store { i8*, i64 } %arr.len, { i8*, i64 }* %name5, align 8
  %ref6 = load %Hero*, %Hero** %self, align 8
  %title7 = getelementptr %Hero, %Hero* %ref6, i32 0, i32 1
  %heap8 = call i8* @calloc(i64 32, i64 1)
  %arr.ptr9 = insertvalue { i8*, i64 } undef, i8* %heap8, 0
  %arr.len10 = insertvalue { i8*, i64 } %arr.ptr9, i64 32, 1
  store { i8*, i64 } %arr.len10, { i8*, i64 }* %title7, align 8
  %ref11 = load %Hero*, %Hero** %self, align 8
  %name12 = getelementptr %Hero, %Hero* %ref11, i32 0, i32 0
  %name13 = load { i8*, i64 }, { i8*, i64 }* %name12, align 8
  %arr.data14 = extractvalue { i8*, i64 } %name13, 0
  %ref15 = load %Hero*, %Hero** %other, align 8
  %name16 = getelementptr %Hero, %Hero* %ref15, i32 0, i32 0
  %name17 = load { i8*, i64 }, { i8*, i64 }* %name16, align 8
  %arr.data18 = extractvalue { i8*, i64 } %name17, 0
  %call = call i8* @strcpy(i8* %arr.data14, i8* %arr.data18)
  %ref19 = load %Hero*, %Hero** %self, align 8
  %title20 = getelementptr %Hero, %Hero* %ref19, i32 0, i32 1
  %title21 = load { i8*, i64 }, { i8*, i64 }* %title20, align 8
  %arr.data22 = extractvalue { i8*, i64 } %title21, 0
  %ref23 = load %Hero*, %Hero** %other, align 8
  %title24 = getelementptr %Hero, %Hero* %ref23, i32 0, i32 1
  %title25 = load { i8*, i64 }, { i8*, i64 }* %title24, align 8
  %arr.data26 = extractvalue { i8*, i64 } %title25, 0
  %call27 = call i8* @strcpy(i8* %arr.data22, i8* %arr.data26)
  ret void
}

define void @Hero.drop(%Hero* %0) {
entry:
  %self = alloca %Hero*, align 8
  store %Hero* %0, %Hero** %self, align 8
  %ref = load %Hero*, %Hero** %self, align 8
  %name = getelementptr %Hero, %Hero* %ref, i32 0, i32 0
  %arr = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %arr, 0
  call void @free(i8* %arr.data)
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %name, align 8
  %ref1 = load %Hero*, %Hero** %self, align 8
  %title = getelementptr %Hero, %Hero* %ref1, i32 0, i32 1
  %arr2 = load { i8*, i64 }, { i8*, i64 }* %title, align 8
  %arr.data3 = extractvalue { i8*, i64 } %arr2, 0
  call void @free(i8* %arr.data3)
  store { i8*, i64 } zeroinitializer, { i8*, i64 }* %title, align 8
  ret void
}

define i32 @main(i32 %0, i8** %1) {
entry:
  store i32 1, i32* @EPERM, align 4
  store i32 2, i32* @ENOENT, align 4
  store i32 3, i32* @ESRCH, align 4
  store i32 4, i32* @EINTR, align 4
  store i32 5, i32* @EIO, align 4
  store i32 6, i32* @ENXIO, align 4
  store i32 7, i32* @E2BIG, align 4
  store i32 8, i32* @ENOEXEC, align 4
  store i32 9, i32* @EBADF, align 4
  store i32 10, i32* @ECHILD, align 4
  store i32 13, i32* @EACCES, align 4
  store i32 14, i32* @EFAULT, align 4
  store i32 15, i32* @ENOTBLK, align 4
  store i32 16, i32* @EBUSY, align 4
  store i32 17, i32* @EEXIST, align 4
  store i32 18, i32* @EXDEV, align 4
  store i32 19, i32* @ENODEV, align 4
  store i32 20, i32* @ENOTDIR, align 4
  store i32 21, i32* @EISDIR, align 4
  store i32 22, i32* @EINVAL, align 4
  store i32 23, i32* @ENFILE, align 4
  store i32 24, i32* @EMFILE, align 4
  store i32 25, i32* @ENOTTY, align 4
  store i32 26, i32* @ETXTBSY, align 4
  store i32 27, i32* @EFBIG, align 4
  store i32 28, i32* @ENOSPC, align 4
  store i32 29, i32* @ESPIPE, align 4
  store i32 30, i32* @EROFS, align 4
  store i32 31, i32* @EMLINK, align 4
  store i32 32, i32* @EPIPE, align 4
  store i32 33, i32* @EDOM, align 4
  store i32 34, i32* @ERANGE, align 4
  store i32 0, i32* @O_RDONLY, align 4
  store i32 1, i32* @O_WRONLY, align 4
  store i32 2, i32* @O_RDWR, align 4
  store i32 1, i32* @FD_CLOEXEC, align 4
  store i32 0, i32* @IPPROTO_IP, align 4
  store i32 6, i32* @IPPROTO_TCP, align 4
  store i32 17, i32* @IPPROTO_UDP, align 4
  store i32 41, i32* @IPPROTO_IPV6, align 4
  store i32 0, i32* @SHUT_RD, align 4
  store i32 1, i32* @SHUT_WR, align 4
  store i32 2, i32* @SHUT_RDWR, align 4
  store i32 1, i32* @POLLIN, align 4
  store i32 2, i32* @POLLPRI, align 4
  store i32 4, i32* @POLLOUT, align 4
  store i32 8, i32* @POLLERR, align 4
  store i32 16, i32* @POLLHUP, align 4
  store i32 32, i32* @POLLNVAL, align 4
  store i32 0, i32* @INADDR_ANY, align 4
  store i32 2130706433, i32* @INADDR_LOOPBACK, align 4
  store i32 -1, i32* @INADDR_BROADCAST, align 4
  store i32 1, i32* @SIGHUP, align 4
  store i32 2, i32* @SIGINT, align 4
  store i32 3, i32* @SIGQUIT, align 4
  store i32 4, i32* @SIGILL, align 4
  store i32 5, i32* @SIGTRAP, align 4
  store i32 6, i32* @SIGABRT, align 4
  store i32 8, i32* @SIGFPE, align 4
  store i32 9, i32* @SIGKILL, align 4
  store i32 11, i32* @SIGSEGV, align 4
  store i32 13, i32* @SIGPIPE, align 4
  store i32 14, i32* @SIGALRM, align 4
  store i32 15, i32* @SIGTERM, align 4
  store i32 61440, i32* @S_IFMT, align 4
  store i32 49152, i32* @S_IFSOCK, align 4
  store i32 40960, i32* @S_IFLNK, align 4
  store i32 32768, i32* @S_IFREG, align 4
  store i32 24576, i32* @S_IFBLK, align 4
  store i32 16384, i32* @S_IFDIR, align 4
  store i32 8192, i32* @S_IFCHR, align 4
  store i32 4096, i32* @S_IFIFO, align 4
  store i32 2048, i32* @S_ISUID, align 4
  store i32 1024, i32* @S_ISGID, align 4
  store i32 512, i32* @S_ISVTX, align 4
  store i32 0, i32* @F_OK, align 4
  store i32 1, i32* @X_OK, align 4
  store i32 2, i32* @W_OK, align 4
  store i32 4, i32* @R_OK, align 4
  store i32 0, i32* @SEEK_SET, align 4
  store i32 1, i32* @SEEK_CUR, align 4
  store i32 2, i32* @SEEK_END, align 4
  store i32 0, i32* @STDIN_FILENO, align 4
  store i32 1, i32* @STDOUT_FILENO, align 4
  store i32 2, i32* @STDERR_FILENO, align 4
  %hero = alloca %Hero, align 8
  %call = call %Hero @Hero.create()
  store %Hero %call, %Hero* %hero, align 8
  call void @Hero.setup(%Hero* %hero, { i8*, i64 } { i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.17, i32 0, i32 0), i64 6 }, { i8*, i64 } { i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.18, i32 0, i32 0), i64 8 })
  call void @Hero.announce(%Hero* %hero)
  %other = alloca %Hero, align 8
  %call1 = call %Hero @Hero.create()
  store %Hero %call1, %Hero* %other, align 8
  call void @Hero.setup(%Hero* %other, { i8*, i64 } { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.19, i32 0, i32 0), i64 5 }, { i8*, i64 } { i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.20, i32 0, i32 0), i64 8 })
  call void @"Hero.=.ref.Hero"(%Hero* %hero, %Hero* %other)
  call void @Hero.announce(%Hero* %hero)
  call void @Hero.setup(%Hero* %other, { i8*, i64 } { i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.21, i32 0, i32 0), i64 4 }, { i8*, i64 } { i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.22, i32 0, i32 0), i64 9 })
  call void @Hero.announce(%Hero* %hero)
  call void @Hero.announce(%Hero* %other)
  call void @Hero.drop(%Hero* %other)
  call void @Hero.drop(%Hero* %hero)
  ret i32 0
}
```
