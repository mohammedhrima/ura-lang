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
- 019 — every operator on one struct, and `=` from an int overload
- 020 — drop on fall-through: an if block, each iteration, reverse order
- 021 — a struct drops the structs it owns, at any depth, but not what it borrows
- 022 — a temporary is destroyed at the end of its statement

## 001 — operator + on two structs

```ura
// operators_overload/001.ura - operator + on two structs

struct Vec:
    x int
    y int

    pub fn create(a int, b int) Vec:
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
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

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
   ├─ chars " "
   ├─ .y : i32
   │  └─ c : STRUCT_CALL
   └─ chars "\n"
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
@fmt = private unnamed_addr constant [9 x i8] c"%d%s%d%s\00", align 1

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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt, i32 0, i32 0), i32 %x3, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %y4, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 002 — two overloads of + with different right-hand types

```ura
// operators_overload/002.ura - two overloads of + with different RHS types

struct Vec:
    x int

    pub fn create(a int) Vec:
        v Vec
        v.x = a
        return v

    operator +(other Vec) Vec:
        return Vec::create(self.x + other.x)

    operator +(n int) Vec:
        return Vec::create(self.x + n)

main():
    a Vec = Vec::create(1)
    b Vec = Vec::create(10)
    output((a + b).x, " ", (a + 5).x, "\n")
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

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
   ├─ chars " "
   ├─ .x : i32
   │  └─ + : STRUCT_CALL
   │     ├─ a : STRUCT_CALL
   │     └─ int 5
   └─ chars "\n"
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
@fmt = private unnamed_addr constant [9 x i8] c"%d%s%d%s\00", align 1

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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt, i32 0, i32 0), i32 %x3, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %x7, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 003 — operator == returning bool

```ura
// operators_overload/003.ura - operator == returning bool

struct Vec:
    x int

    pub fn create(a int) Vec:
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
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

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
   ├─ chars " "
   ├─ == : bool
   │  ├─ a : STRUCT_CALL
   │  └─ c : STRUCT_CALL
   └─ chars "\n"
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
@fmt = private unnamed_addr constant [9 x i8] c"%s%s%s%s\00", align 1

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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt, i32 0, i32 0), i8* %bool_str, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i8* %bool_str6, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 004 — no overload for that operator

```ura
// operators_overload/004.ura - no overload for that operator

struct Vec:
    x int

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
    x int

struct Point:
    x int

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
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn main() : i32
└─ output : void
   ├─ index : i32
   │  ├─ array : i32[]
   │  │  ├─ int 10
   │  │  ├─ int 20
   │  │  └─ int 30
   │  └─ int 1
   └─ chars "\n"
```

```out
20
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1

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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 %idx, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 007 — operators and a method on the same struct

```ura
// operators_overload/007.ura - operators and a method on the same struct

struct Vec:
    x int
    y int

    pub fn create(a int, b int) Vec:
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
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

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
      ├─ chars "("
      ├─ .x : i32
      │  └─ self : STRUCT_CALL
      ├─ chars ", "
      ├─ .y : i32
      │  └─ self : STRUCT_CALL
      └─ chars ")\n"

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
│  ├─ chars "a == a  "
│  ├─ == : bool
│  │  ├─ a : STRUCT_CALL
│  │  └─ a : STRUCT_CALL
│  └─ chars "\n"
├─ output : void
│  ├─ chars "a == b  "
│  ├─ == : bool
│  │  ├─ a : STRUCT_CALL
│  │  └─ b : STRUCT_CALL
│  └─ chars "\n"
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
@fmt = private unnamed_addr constant [11 x i8] c"%s%d%s%d%s\00", align 1
@str.3 = private unnamed_addr constant [9 x i8] c"a == a  \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.5 = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1
@str.6 = private unnamed_addr constant [9 x i8] c"a == b  \00", align 1
@true_str.7 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.8 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.10 = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1

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
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %x1, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0), i32 %y3, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.2, i32 0, i32 0))
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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.5, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.3, i32 0, i32 0), i8* %bool_str, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  %b5 = load %Vec, %Vec* %b, align 4
  %op6 = call i1 @"Vec.==.Vec"(%Vec* %a, %Vec %b5)
  %bool_str7 = select i1 %op6, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.7, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.8, i32 0, i32 0)
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.10, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.6, i32 0, i32 0), i8* %bool_str7, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.9, i32 0, i32 0))
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
    x int

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
    n int = 1
    s chars = "hi"
    output(n + s, "\n")
```

```tree
```

```out
```

```err
error: Cannot use '+' with i32 and chars
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
    x int
    tag int

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
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

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
│  ├─ chars "decl   "
│  ├─ .x : i32
│  │  └─ b : STRUCT_CALL
│  ├─ chars " "
│  ├─ .tag : i32
│  │  └─ b : STRUCT_CALL
│  └─ chars "\n"
├─ c : STRUCT_CALL
├─ = : void
│  ├─ c : STRUCT_CALL
│  └─ a : STRUCT_CALL
└─ output : void
   ├─ chars "assign "
   ├─ .x : i32
   │  └─ c : STRUCT_CALL
   ├─ chars " "
   ├─ .tag : i32
   │  └─ c : STRUCT_CALL
   └─ chars "\n"
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
@fmt = private unnamed_addr constant [11 x i8] c"%s%d%s%d%s\00", align 1
@str.3 = private unnamed_addr constant [8 x i8] c"assign \00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [11 x i8] c"%s%d%s%d%s\00", align 1

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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str, i32 0, i32 0), i32 %x3, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i32 %tag4, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %c = alloca %Vec, align 8
  store %Vec zeroinitializer, %Vec* %c, align 4
  %a5 = load %Vec, %Vec* %a, align 4
  call void @"Vec.=.Vec"(%Vec* %c, %Vec %a5)
  %x6 = getelementptr %Vec, %Vec* %c, i32 0, i32 0
  %x7 = load i32, i32* %x6, align 4
  %tag8 = getelementptr %Vec, %Vec* %c, i32 0, i32 1
  %tag9 = load i32, i32* %tag8, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.6, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.3, i32 0, i32 0), i32 %x7, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0), i32 %tag9, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 011 — compound assignment operators

```ura
// operators_overload/011.ura - compound assignment operators

struct Vec:
    x int

    operator +=(other Vec) void:
        self.x = self.x + other.x

    operator -=(n int) void:
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
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

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
│  ├─ chars "after += "
│  ├─ .x : i32
│  │  └─ a : STRUCT_CALL
│  └─ chars "\n"
├─ -= : void
│  ├─ a : STRUCT_CALL
│  └─ int 3
└─ output : void
   ├─ chars "after -= "
   ├─ .x : i32
   │  └─ a : STRUCT_CALL
   └─ chars "\n"
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
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.2 = private unnamed_addr constant [10 x i8] c"after -= \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %x4, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  call void @"Vec.-=.i32"(%Vec* %a, i32 3)
  %x5 = getelementptr %Vec, %Vec* %a, i32 0, i32 0
  %x6 = load i32, i32* %x5, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.4, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.2, i32 0, i32 0), i32 %x6, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 012 — a compound operator with no overload

```ura
// operators_overload/012.ura - a compound operator with no overload

struct Vec:
    x int

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
    x int

    operator drop:
        output("dropping ", self.x, "\n")

main():
    v Vec
    v.x = 7
    output("done\n")
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

struct Vec
├─ x : i32
└─ fn Vec.drop(self : STRUCT_CALL) : void
   └─ output : void
      ├─ chars "dropping "
      ├─ .x : i32
      │  └─ self : STRUCT_CALL
      └─ chars "\n"

fn main() : i32
├─ v : STRUCT_CALL
├─ = : i32
│  ├─ .x : i32
│  │  └─ v : STRUCT_CALL
│  └─ int 7
└─ output : void
   └─ chars "done\n"
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
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"done\0A\00", align 1
@fmt.3 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define void @Vec.drop(%Vec* %0) {
entry:
  %self = alloca %Vec*, align 8
  store %Vec* %0, %Vec** %self, align 8
  %ref = load %Vec*, %Vec** %self, align 8
  %x = getelementptr %Vec, %Vec* %ref, i32 0, i32 0
  %x1 = load i32, i32* %x, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %x1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %v = alloca %Vec, align 8
  store %Vec zeroinitializer, %Vec* %v, align 4
  %x = getelementptr %Vec, %Vec* %v, i32 0, i32 0
  store i32 7, i32* %x, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.3, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0))
  call void @Vec.drop(%Vec* %v)
  ret i32 0
}
```

## 014 — 'clean' does not destroy a struct, even one with `operator drop`

```ura
// operators_overload/014.ura - clean does not destroy a struct

struct Vec:
    x int

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
    x int

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
    x int

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
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

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
      ├─ chars "drop "
      ├─ .x : i32
      │  └─ self : STRUCT_CALL
      └─ chars "\n"

fn main() : i32
├─ a : STRUCT_CALL
├─ = : i32
│  ├─ .x : i32
│  │  └─ a : STRUCT_CALL
│  └─ int 5
└─ output : void
   └─ chars "done\n"
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
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"done\0A\00", align 1
@fmt.3 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

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
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %x1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %a = alloca %Vec, align 8
  store %Vec zeroinitializer, %Vec* %a, align 4
  %x = getelementptr %Vec, %Vec* %a, i32 0, i32 0
  store i32 5, i32* %x, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.3, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0))
  call void @Vec.drop(%Vec* %a)
  ret i32 0
}
```

## 017 — `return` unwinds every scope, and keeps the returned local

```ura
// operators_overload/017.ura - return unwinds scopes, keeps the returned local

struct Tag:
    id int

    pub fn create(n int) Tag:
        t Tag
        t.id = n
        return t

    operator =(other Tag) void:
        self.id = other.id

    operator drop:
        output("drop ", self.id, "\n")

fn early() int:
    a Tag = Tag::create(1)
    if 1 < 2:
        b Tag = Tag::create(10)
        return 7
    return 0

fn giveback() Tag:
    t Tag = Tag::create(99)
    return t

main():
    r int = early()
    output("early ", r, "\n")
    g Tag = giveback()
    output("g.id ", g.id, "\n")
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

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
      ├─ chars "drop "
      ├─ .id : i32
      │  └─ self : STRUCT_CALL
      └─ chars "\n"

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
│  ├─ chars "early "
│  ├─ r : i32
│  └─ chars "\n"
├─ = : STRUCT_CALL
│  ├─ g : STRUCT_CALL
│  └─ call giveback : STRUCT_CALL
└─ output : void
   ├─ chars "g.id "
   ├─ .id : i32
   │  └─ g : STRUCT_CALL
   └─ chars "\n"
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
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.2 = private unnamed_addr constant [7 x i8] c"early \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.5 = private unnamed_addr constant [6 x i8] c"g.id \00", align 1
@str.6 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.7 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

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
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %id1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.4, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.2, i32 0, i32 0), i32 %r1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %g = alloca %Tag, align 8
  %call2 = call %Tag @giveback()
  store %Tag %call2, %Tag* %g, align 4
  %id = getelementptr %Tag, %Tag* %g, i32 0, i32 0
  %id3 = load i32, i32* %id, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.7, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.5, i32 0, i32 0), i32 %id3, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.6, i32 0, i32 0))
  call void @Tag.drop(%Tag* %g)
  ret i32 0
}
```

## 018 — a destructor fires on `break` and on `continue`

```ura
// operators_overload/018.ura - destructor fires on break and continue

struct Tag:
    id int

    pub fn create(n int) Tag:
        t Tag
        t.id = n
        return t

    operator =(other Tag) void:
        self.id = other.id

    operator drop:
        output("drop ", self.id, "\n")

main():
    i int = 0
    while i < 3:
        b Tag = Tag::create(100 + i)
        if i == 1:
            break
        i = i + 1
    output("after break\n")

    k int = 0
    while k < 2:
        c Tag = Tag::create(200 + k)
        k = k + 1
        continue
    output("after continue\n")
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

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
      ├─ chars "drop "
      ├─ .id : i32
      │  └─ self : STRUCT_CALL
      └─ chars "\n"

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
│  └─ chars "after break\n"
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
   └─ chars "after continue\n"
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
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.2 = private unnamed_addr constant [13 x i8] c"after break\0A\00", align 1
@fmt.3 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.4 = private unnamed_addr constant [16 x i8] c"after continue\0A\00", align 1
@fmt.5 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

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
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %id1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.3, i32 0, i32 0), i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.2, i32 0, i32 0))
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
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.5, i32 0, i32 0), i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.4, i32 0, i32 0))
  ret i32 0
}
```

## 019 — every operator on one struct, and `=` from an int overload

```ura
// operators_overload/019.ura - every operator on one struct

struct Vec:
    x int
    y int
    tag int

    pub fn create(a int, b int) Vec:
        v Vec
        v.x = a
        v.y = b
        return v

    operator +(other Vec) Vec:
        return Vec::create(self.x + other.x, self.y + other.y)

    operator +(n int) Vec:
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

    operator -=(n int) void:
        self.x = self.x - n
        self.y = self.y - n

    fn show() void:
        output("(", self.x, ", ", self.y, ") tag ", self.tag, "\n")

main():
    a Vec = Vec::create(1, 2)
    b Vec = Vec::create(10, 20)

    c Vec = a + b
    c.show()

    // the int overload's result as a declaration initialiser
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
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

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
      ├─ chars "("
      ├─ .x : i32
      │  └─ self : STRUCT_CALL
      ├─ chars ", "
      ├─ .y : i32
      │  └─ self : STRUCT_CALL
      ├─ chars ") tag "
      ├─ .tag : i32
      │  └─ self : STRUCT_CALL
      └─ chars "\n"

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
│  ├─ chars "a == a  "
│  ├─ == : bool
│  │  ├─ a : STRUCT_CALL
│  │  └─ a : STRUCT_CALL
│  └─ chars "\n"
├─ output : void
│  ├─ chars "a == b  "
│  ├─ == : bool
│  │  ├─ a : STRUCT_CALL
│  │  └─ b : STRUCT_CALL
│  └─ chars "\n"
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
@fmt = private unnamed_addr constant [15 x i8] c"%s%d%s%d%s%d%s\00", align 1
@str.4 = private unnamed_addr constant [9 x i8] c"a == a  \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1
@str.7 = private unnamed_addr constant [9 x i8] c"a == b  \00", align 1
@true_str.8 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.9 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.10 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.11 = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1

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
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %x1, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0), i32 %y3, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.2, i32 0, i32 0), i32 %tag5, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.6, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.4, i32 0, i32 0), i8* %bool_str, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  %b6 = load %Vec, %Vec* %b, align 4
  %op7 = call i1 @"Vec.==.Vec"(%Vec* %a, %Vec %b6)
  %bool_str8 = select i1 %op7, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.8, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.9, i32 0, i32 0)
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.11, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.7, i32 0, i32 0), i8* %bool_str8, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.10, i32 0, i32 0))
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
    id int

    pub fn create(n int) Tag:
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

    i int = 0
    while i < 3:
        y Tag = Tag::create(10 + i)
        i += 1
    output("after loop\n")

    p Tag = Tag::create(7)
    q Tag = Tag::create(8)
    output("end of main\n")
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

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
      ├─ chars "drop "
      ├─ .id : i32
      │  └─ self : STRUCT_CALL
      └─ chars "\n"

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
│     └─ chars "inside\n"
├─ output : void
│  └─ chars "after if\n"
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
│  └─ chars "after loop\n"
├─ = : STRUCT_CALL
│  ├─ p : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     └─ int 7
├─ = : STRUCT_CALL
│  ├─ q : STRUCT_CALL
│  └─ call create : STRUCT_CALL
│     └─ int 8
└─ output : void
   └─ chars "end of main\n"
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
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.2 = private unnamed_addr constant [8 x i8] c"inside\0A\00", align 1
@fmt.3 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.4 = private unnamed_addr constant [10 x i8] c"after if\0A\00", align 1
@fmt.5 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.6 = private unnamed_addr constant [12 x i8] c"after loop\0A\00", align 1
@fmt.7 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.8 = private unnamed_addr constant [13 x i8] c"end of main\0A\00", align 1
@fmt.9 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

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
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %id1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  br i1 true, label %then, label %endif

endif:                                            ; preds = %then, %entry
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.5, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.4, i32 0, i32 0))
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

then:                                             ; preds = %entry
  %x = alloca %Tag, align 8
  %call = call %Tag @Tag.create(i32 1)
  store %Tag %call, %Tag* %x, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.3, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.2, i32 0, i32 0))
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
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.7, i32 0, i32 0), i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.6, i32 0, i32 0))
  %p = alloca %Tag, align 8
  %call5 = call %Tag @Tag.create(i32 7)
  store %Tag %call5, %Tag* %p, align 4
  %q = alloca %Tag, align 8
  %call6 = call %Tag @Tag.create(i32 8)
  store %Tag %call6, %Tag* %q, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.9, i32 0, i32 0), i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.8, i32 0, i32 0))
  call void @Tag.drop(%Tag* %q)
  call void @Tag.drop(%Tag* %p)
  ret i32 0
}
```

## 021 — a struct drops the structs it owns, at any depth, but not what it borrows

```ura
// operators_overload/021.ura - owned struct fields drop recursively

struct Res:
    id int

    pub fn create(n int) Res:
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
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

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
      ├─ chars "drop "
      ├─ .id : i32
      │  └─ self : STRUCT_CALL
      └─ chars "\n"

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
   └─ chars "built\n"
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
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.2 = private unnamed_addr constant [7 x i8] c"built\0A\00", align 1
@fmt.3 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

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
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %id1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.3, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.2, i32 0, i32 0))
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
    x int

    pub fn create(n int) Plain:
        p Plain
        p.x = n
        return p

struct R:
    id int

    pub fn create(n int) R:
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

fn via_return(a R, b R) int:
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
    i int = 0
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
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

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
│     ├─ chars "show "
│     ├─ .id : i32
│     │  └─ self : STRUCT_CALL
│     └─ chars "\n"
└─ fn R.drop(self : STRUCT_CALL) : void
   └─ output : void
      ├─ chars "DROP "
      ├─ .id : i32
      │  └─ self : STRUCT_CALL
      └─ chars "\n"

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
│  └─ chars "--\n"
├─ output : void
│  ├─ .id : i32
│  │  └─ + : STRUCT_CALL
│  │     ├─ a : STRUCT_CALL
│  │     └─ b : STRUCT_CALL
│  └─ chars "\n"
├─ output : void
│  └─ chars "--\n"
├─ call show : void
│  └─ + : STRUCT_CALL
│     ├─ + : STRUCT_CALL
│     │  ├─ a : STRUCT_CALL
│     │  └─ b : STRUCT_CALL
│     └─ a : STRUCT_CALL
├─ output : void
│  └─ chars "--\n"
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
│  └─ chars "--\n"
├─ output : void
│  ├─ call via_return : i32
│  │  ├─ a : STRUCT_CALL
│  │  └─ b : STRUCT_CALL
│  └─ chars "\n"
├─ output : void
│  └─ chars "--\n"
├─ output : void
│  ├─ .id : i32
│  │  └─ call create : STRUCT_CALL
│  │     └─ int 9
│  └─ chars "\n"
├─ output : void
│  └─ chars "--\n"
├─ output : void
│  ├─ .x : i32
│  │  └─ call create : STRUCT_CALL
│  │     └─ int 7
│  └─ chars "\n"
└─ output : void
   └─ chars "end\n"
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
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"DROP \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.5 = private unnamed_addr constant [4 x i8] c"--\0A\00", align 1
@fmt.6 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.8 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.9 = private unnamed_addr constant [4 x i8] c"--\0A\00", align 1
@fmt.10 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.11 = private unnamed_addr constant [4 x i8] c"--\0A\00", align 1
@fmt.12 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.13 = private unnamed_addr constant [4 x i8] c"--\0A\00", align 1
@fmt.14 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.15 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.16 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.17 = private unnamed_addr constant [4 x i8] c"--\0A\00", align 1
@fmt.18 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.19 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.20 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.21 = private unnamed_addr constant [4 x i8] c"--\0A\00", align 1
@fmt.22 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.23 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.24 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.25 = private unnamed_addr constant [5 x i8] c"end\0A\00", align 1
@fmt.26 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

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
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %id1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
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
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.4, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0), i32 %id1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.6, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.5, i32 0, i32 0))
  %b3 = load %R, %R* %b, align 4
  %op4 = call %R @"R.+.R"(%R* %a, %R %b3)
  %out.tmp5 = alloca %R, align 8
  store %R %op4, %R* %out.tmp5, align 4
  %id = getelementptr %R, %R* %out.tmp5, i32 0, i32 0
  %id6 = load i32, i32* %id, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.8, i32 0, i32 0), i32 %id6, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  call void @R.drop(%R* %out.tmp5)
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.10, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.9, i32 0, i32 0))
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
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.12, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.11, i32 0, i32 0))
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
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.14, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.13, i32 0, i32 0))
  %a17 = load %R, %R* %a, align 4
  %b18 = load %R, %R* %b, align 4
  %call19 = call i32 @via_return(%R %a17, %R %b18)
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.16, i32 0, i32 0), i32 %call19, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.15, i32 0, i32 0))
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.18, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.17, i32 0, i32 0))
  %call20 = call %R @R.create(i32 9)
  %out.tmp21 = alloca %R, align 8
  store %R %call20, %R* %out.tmp21, align 4
  %id22 = getelementptr %R, %R* %out.tmp21, i32 0, i32 0
  %id23 = load i32, i32* %id22, align 4
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.20, i32 0, i32 0), i32 %id23, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.19, i32 0, i32 0))
  call void @R.drop(%R* %out.tmp21)
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.22, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.21, i32 0, i32 0))
  %call24 = call %Plain @Plain.create(i32 7)
  %out.tmp25 = alloca %Plain, align 8
  store %Plain %call24, %Plain* %out.tmp25, align 4
  %x = getelementptr %Plain, %Plain* %out.tmp25, i32 0, i32 0
  %x26 = load i32, i32* %x, align 4
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.24, i32 0, i32 0), i32 %x26, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.23, i32 0, i32 0))
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.26, i32 0, i32 0), i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.25, i32 0, i32 0))
  call void @R.drop(%R* %b)
  call void @R.drop(%R* %a)
  ret i32 0
}
```
