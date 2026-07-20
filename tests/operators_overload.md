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

%Vec = type { i32, i32 }

@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [13 x i8] c"%d%.*s%d%.*s\00", align 1

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

define i32 @main() {
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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i32 %x3, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %y4, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
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

%Vec = type { i32 }

@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [13 x i8] c"%d%.*s%d%.*s\00", align 1

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

define i32 @main() {
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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i32 %x3, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %x7, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
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

%Vec = type { i32 }

@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@true_str.1 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.2 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [13 x i8] c"%s%.*s%s%.*s\00", align 1

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

define i32 @main() {
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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i8* %bool_str, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i8* %bool_str6, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
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

@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1

define i32 @main() {
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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %idx, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
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

%Vec = type { i32, i32 }

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

define i32 @main() {
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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.5, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.3, i32 0, i32 0), i8* %bool_str, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  %b5 = load %Vec, %Vec* %b, align 4
  %op6 = call i1 @"Vec.==.Vec"(%Vec* %a, %Vec %b5)
  %bool_str7 = select i1 %op6, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.7, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.8, i32 0, i32 0)
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.10, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.6, i32 0, i32 0), i8* %bool_str7, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.9, i32 0, i32 0))
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

%Vec = type { i32, i32 }

@str = private unnamed_addr constant [8 x i8] c"decl   \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [17 x i8] c"%.*s%d%.*s%d%.*s\00", align 1
@str.3 = private unnamed_addr constant [8 x i8] c"assign \00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [17 x i8] c"%.*s%d%.*s%d%.*s\00", align 1

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

define i32 @main() {
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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt, i32 0, i32 0), i32 7, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str, i32 0, i32 0), i32 %x3, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i32 %tag4, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %c = alloca %Vec, align 8
  store %Vec zeroinitializer, %Vec* %c, align 4
  %a5 = load %Vec, %Vec* %a, align 4
  call void @"Vec.=.Vec"(%Vec* %c, %Vec %a5)
  %x6 = getelementptr %Vec, %Vec* %c, i32 0, i32 0
  %x7 = load i32, i32* %x6, align 4
  %tag8 = getelementptr %Vec, %Vec* %c, i32 0, i32 1
  %tag9 = load i32, i32* %tag8, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt.6, i32 0, i32 0), i32 7, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.3, i32 0, i32 0), i32 %x7, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0), i32 %tag9, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
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

%Vec = type { i32 }

@str = private unnamed_addr constant [10 x i8] c"after += \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.2 = private unnamed_addr constant [10 x i8] c"after -= \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1

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

define i32 @main() {
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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 9, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %x4, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  call void @"Vec.-=.i32"(%Vec* %a, i32 3)
  %x5 = getelementptr %Vec, %Vec* %a, i32 0, i32 0
  %x6 = load i32, i32* %x5, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.4, i32 0, i32 0), i32 9, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.2, i32 0, i32 0), i32 %x6, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
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

%Vec = type { i32 }

@str = private unnamed_addr constant [10 x i8] c"dropping \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"done\0A\00", align 1
@fmt.3 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

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

define i32 @main() {
entry:
  %v = alloca %Vec, align 8
  store %Vec zeroinitializer, %Vec* %v, align 4
  %x = getelementptr %Vec, %Vec* %v, i32 0, i32 0
  store i32 7, i32* %x, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.3, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0))
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

%Vec = type { i32 }

@str = private unnamed_addr constant [6 x i8] c"drop \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"done\0A\00", align 1
@fmt.3 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

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

define i32 @main() {
entry:
  %a = alloca %Vec, align 8
  store %Vec zeroinitializer, %Vec* %a, align 4
  %x = getelementptr %Vec, %Vec* %a, i32 0, i32 0
  store i32 5, i32* %x, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.3, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0))
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

%Tag = type { i32 }

@str = private unnamed_addr constant [6 x i8] c"drop \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.2 = private unnamed_addr constant [7 x i8] c"early \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.5 = private unnamed_addr constant [6 x i8] c"g.id \00", align 1
@str.6 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.7 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1

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

define i32 @main() {
entry:
  %r = alloca i32, align 4
  %call = call i32 @early()
  store i32 %call, i32* %r, align 4
  %r1 = load i32, i32* %r, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.4, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.2, i32 0, i32 0), i32 %r1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %g = alloca %Tag, align 8
  %call2 = call %Tag @giveback()
  store %Tag %call2, %Tag* %g, align 4
  %id = getelementptr %Tag, %Tag* %g, i32 0, i32 0
  %id3 = load i32, i32* %id, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.7, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.5, i32 0, i32 0), i32 %id3, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.6, i32 0, i32 0))
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

%Tag = type { i32 }

@str = private unnamed_addr constant [6 x i8] c"drop \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.2 = private unnamed_addr constant [13 x i8] c"after break\0A\00", align 1
@fmt.3 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.4 = private unnamed_addr constant [16 x i8] c"after continue\0A\00", align 1
@fmt.5 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

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

define i32 @main() {
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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.3, i32 0, i32 0), i32 12, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.2, i32 0, i32 0))
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
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.5, i32 0, i32 0), i32 15, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.4, i32 0, i32 0))
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

%Vec = type { i32, i32, i32 }

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

define i32 @main() {
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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.6, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.4, i32 0, i32 0), i8* %bool_str, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  %b6 = load %Vec, %Vec* %b, align 4
  %op7 = call i1 @"Vec.==.Vec"(%Vec* %a, %Vec %b6)
  %bool_str8 = select i1 %op7, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.8, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.9, i32 0, i32 0)
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.11, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.7, i32 0, i32 0), i8* %bool_str8, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.10, i32 0, i32 0))
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

%Tag = type { i32 }

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

define i32 @main() {
entry:
  br i1 true, label %then, label %endif

endif:                                            ; preds = %then, %entry
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.5, i32 0, i32 0), i32 9, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.4, i32 0, i32 0))
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then:                                             ; preds = %entry
  %x = alloca %Tag, align 8
  %call = call %Tag @Tag.create(i32 1)
  store %Tag %call, %Tag* %x, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.3, i32 0, i32 0), i32 7, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.2, i32 0, i32 0))
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
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.7, i32 0, i32 0), i32 11, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.6, i32 0, i32 0))
  %p = alloca %Tag, align 8
  %call5 = call %Tag @Tag.create(i32 7)
  store %Tag %call5, %Tag* %p, align 4
  %q = alloca %Tag, align 8
  %call6 = call %Tag @Tag.create(i32 8)
  store %Tag %call6, %Tag* %q, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.9, i32 0, i32 0), i32 12, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.8, i32 0, i32 0))
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

%Res = type { i32 }
%Owner = type { %L2, %Res, %Res* }
%L2 = type { %L3 }
%L3 = type { %Res }

@str = private unnamed_addr constant [6 x i8] c"drop \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.2 = private unnamed_addr constant [7 x i8] c"built\0A\00", align 1
@fmt.3 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

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

define i32 @main() {
entry:
  %lent = alloca %Res, align 8
  %call = call %Res @Res.create(i32 9)
  store %Res %call, %Res* %lent, align 4
  %o = alloca %Owner, align 8
  %call1 = call %Owner @Owner.create()
  store %Owner %call1, %Owner* %o, align 8
  %borrowed = getelementptr %Owner, %Owner* %o, i32 0, i32 2
  store %Res* %lent, %Res** %borrowed, align 8
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.3, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.2, i32 0, i32 0))
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

%Plain = type { i32 }
%R = type { i32 }

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

define i32 @main() {
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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.6, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.5, i32 0, i32 0))
  %b3 = load %R, %R* %b, align 4
  %op4 = call %R @"R.+.R"(%R* %a, %R %b3)
  %out.tmp5 = alloca %R, align 8
  store %R %op4, %R* %out.tmp5, align 4
  %id = getelementptr %R, %R* %out.tmp5, i32 0, i32 0
  %id6 = load i32, i32* %id, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.8, i32 0, i32 0), i32 %id6, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  call void @R.drop(%R* %out.tmp5)
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.10, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.9, i32 0, i32 0))
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
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.12, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.11, i32 0, i32 0))
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
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.14, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.13, i32 0, i32 0))
  %a17 = load %R, %R* %a, align 4
  %b18 = load %R, %R* %b, align 4
  %call19 = call i32 @via_return(%R %a17, %R %b18)
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.16, i32 0, i32 0), i32 %call19, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.15, i32 0, i32 0))
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.18, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.17, i32 0, i32 0))
  %call20 = call %R @R.create(i32 9)
  %out.tmp21 = alloca %R, align 8
  store %R %call20, %R* %out.tmp21, align 4
  %id22 = getelementptr %R, %R* %out.tmp21, i32 0, i32 0
  %id23 = load i32, i32* %id22, align 4
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.20, i32 0, i32 0), i32 %id23, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.19, i32 0, i32 0))
  call void @R.drop(%R* %out.tmp21)
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.22, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.21, i32 0, i32 0))
  %call24 = call %Plain @Plain.create(i32 7)
  %out.tmp25 = alloca %Plain, align 8
  store %Plain %call24, %Plain* %out.tmp25, align 4
  %x = getelementptr %Plain, %Plain* %out.tmp25, i32 0, i32 0
  %x26 = load i32, i32* %x, align 4
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.24, i32 0, i32 0), i32 %x26, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.23, i32 0, i32 0))
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.26, i32 0, i32 0), i32 4, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.25, i32 0, i32 0))
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

%Vec2 = type { i32, i32 }

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

define i32 @main() {
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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt.20, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.15, i32 0, i32 0), i8* %bool_str, i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.16, i32 0, i32 0), i8* %bool_str16, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.19, i32 0, i32 0))
  %b17 = load %Vec2, %Vec2* %b, align 4
  %op18 = call i1 @"Vec2.<.Vec2"(%Vec2* %a, %Vec2 %b17)
  %bool_str19 = select i1 %op18, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.22, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.23, i32 0, i32 0)
  %b20 = load %Vec2, %Vec2* %b, align 4
  %op21 = call i1 @"Vec2.>.Vec2"(%Vec2* %a, %Vec2 %b20)
  %bool_str22 = select i1 %op21, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.25, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.26, i32 0, i32 0)
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt.28, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.21, i32 0, i32 0), i8* %bool_str19, i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.24, i32 0, i32 0), i8* %bool_str22, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.27, i32 0, i32 0))
  %b23 = load %Vec2, %Vec2* %b, align 4
  %op24 = call i1 @"Vec2.<=.Vec2"(%Vec2* %a, %Vec2 %b23)
  %bool_str25 = select i1 %op24, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.30, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.31, i32 0, i32 0)
  %b26 = load %Vec2, %Vec2* %b, align 4
  %op27 = call i1 @"Vec2.>=.Vec2"(%Vec2* %a, %Vec2 %b26)
  %bool_str28 = select i1 %op27, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.33, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.34, i32 0, i32 0)
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt.36, i32 0, i32 0), i32 3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.29, i32 0, i32 0), i8* %bool_str25, i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.32, i32 0, i32 0), i8* %bool_str28, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.35, i32 0, i32 0))
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
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.44, i32 0, i32 0), i32 12, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.43, i32 0, i32 0))
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
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.47, i32 0, i32 0), i32 17, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str.45, i32 0, i32 0), i32 %n39, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.46, i32 0, i32 0))
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

%Flags = type { i32 }
%Vec2 = type { i32, i32 }

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

define i32 @main() {
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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i32 4, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i32 %bits3, i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i32 %bits8)
  %b9 = load %Flags, %Flags* %b, align 4
  %op10 = call %Flags @"Flags.^.Flags"(%Flags* %a, %Flags %b9)
  %out.tmp11 = alloca %Flags, align 8
  store %Flags %op10, %Flags* %out.tmp11, align 4
  %bits12 = getelementptr %Flags, %Flags* %out.tmp11, i32 0, i32 0
  %bits13 = load i32, i32* %bits12, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.4, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.2, i32 0, i32 0), i32 %bits13, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
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
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt.8, i32 0, i32 0), i32 4, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.5, i32 0, i32 0), i32 %bits17, i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.6, i32 0, i32 0), i32 %bits21, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  %p = alloca %Vec2, align 8
  %call22 = call %Vec2 @Vec2.create(i32 2, i32 3)
  store %Vec2 %call22, %Vec2* %p, align 4
  %q = alloca %Vec2, align 8
  %call23 = call %Vec2 @Vec2.create(i32 4, i32 5)
  store %Vec2 %call23, %Vec2* %q, align 4
  %q24 = load %Vec2, %Vec2* %q, align 4
  %op25 = call i32 @"Vec2.*.Vec2"(%Vec2* %p, %Vec2 %q24)
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.11, i32 0, i32 0), i32 4, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.9, i32 0, i32 0), i32 %op25, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.10, i32 0, i32 0))
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

%Flags = type { i32 }

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

define i32 @main() {
entry:
  %f = alloca %Flags, align 8
  store %Flags zeroinitializer, %Flags* %f, align 4
  %bits = getelementptr %Flags, %Flags* %f, i32 0, i32 0
  store i32 12, i32* %bits, align 4
  call void @"Flags.&=.i32"(%Flags* %f, i32 10)
  %bits1 = getelementptr %Flags, %Flags* %f, i32 0, i32 0
  %bits2 = load i32, i32* %bits1, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %bits2, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  call void @"Flags.|=.i32"(%Flags* %f, i32 3)
  %bits3 = getelementptr %Flags, %Flags* %f, i32 0, i32 0
  %bits4 = load i32, i32* %bits3, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.2, i32 0, i32 0), i32 %bits4, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  call void @"Flags.^=.i32"(%Flags* %f, i32 5)
  %bits5 = getelementptr %Flags, %Flags* %f, i32 0, i32 0
  %bits6 = load i32, i32* %bits5, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.4, i32 0, i32 0), i32 %bits6, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  call void @"Flags.<<=.i32"(%Flags* %f, i32 2)
  %bits7 = getelementptr %Flags, %Flags* %f, i32 0, i32 0
  %bits8 = load i32, i32* %bits7, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.6, i32 0, i32 0), i32 %bits8, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  call void @"Flags.>>=.i32"(%Flags* %f, i32 1)
  %bits9 = getelementptr %Flags, %Flags* %f, i32 0, i32 0
  %bits10 = load i32, i32* %bits9, align 4
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.8, i32 0, i32 0), i32 %bits10, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```
