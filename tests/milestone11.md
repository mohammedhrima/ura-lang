# milestone11

## index

- 001 — builtins: typeof without an argument
- 002 — builtins: sizeof with two arguments
- 003 — builtin: typeof on a value, a string and a struct
- 004 — builtin: typeof on a ref and on a type name
- 005 — builtin: sizeof folds to a constant
- 006 — builtin: typeof / sizeof on scalars
- 007 — builtin: typeof / sizeof on an array
- 008 — builtin: sizeof without parentheses
- 009 — builtin: typeof without parentheses
- 010 — builtin: typeof and sizeof on chars, bool, and a literal
- 011 — builtin: sizeof of every scalar type
- 012 — builtin: sizeof of a struct honours ABI padding
- 013 — builtin: typeof of a struct gives its name
- 014 — cast: as casting + i64 / i16 via explicit cast
- 015 — cast: as converts between f32 and the integer types
- 016 — cast: char arithmetic and an expression cast back to char
- 017 — cast: a struct cannot be cast to or from a scalar
- 018 — cast: casting a scalar to a struct
- 019 — literal: f32 literals: declare, print, zero-init, in a function
- 020 — literal: f32 arithmetic + comparison, compound assignment
- 021 — literal: f64 declaration, arithmetic and compound assignment
- 022 — literal: a f32 literal takes the type of its target
- 023 — literal: casts between f64, f32 and the integer types
- 024 — literal: a return that does not match the function's type
- 025 — literal: integer literals at the edge of each type's range
- 026 — literal: an integer literal too wide for i32 keeps its width
- 027 — literal: a literal that does not fit its target type
- 028 — literal: a negative literal at the low edge still fits
- 029 — operator: bitwise & | ^ << >>
- 030 — operator: unary not / minus / bitwise-not
- 031 — operator: compound bitwise assignment
- 032 — operator: bitwise operator on f32 operands is rejected
- 033 — operator: divide by zero prints file:line and exits 1
- 034 — operator: f32 division by zero traps (not inf)
- 035 — literal: f32 and f64 math, casts and printf

---

## 001 — builtins: typeof without an argument

```ura
fn main() i32:
   x i32 = 1
   return typeof()
```

---

## 002 — builtins: sizeof with two arguments

```ura
fn main() i32:
   x i32 = 1
   return sizeof(x, x)
```

---

## 003 — builtin: typeof on a value, a string and a struct

```ura
struct User:
   count i32

fn main() i32:
   x i32 = 1
   s i8[] = "abc"
   u User
   output(typeof(x), " ", typeof(s), " ", typeof(u), "\n")
   return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32 }

@TYPEOF = private unnamed_addr constant [4 x i8] c"i32\00", align 1
@TYPEOF.1 = private unnamed_addr constant [5 x i8] c"i8[]\00", align 1
@TYPEOF.2 = private unnamed_addr constant [5 x i8] c"User\00", align 1

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %s = alloca i8*, align 8
  %arr = alloca [4 x i8], align 1
  %u = alloca %User, align 8
  %arr1 = alloca [2 x i8], align 1
  %arr2 = alloca [2 x i8], align 1
  %arr3 = alloca [2 x i8], align 1
  %arr4 = alloca [10 x i8], align 1
  store i32 1, i32* %x, align 4
  store [4 x i8] c"abc\00", [4 x i8]* %arr, align 1
  %0 = getelementptr inbounds [4 x i8], [4 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %s, align 8
  store %User zeroinitializer, %User* %u, align 4
  store [2 x i8] c" \00", [2 x i8]* %arr1, align 1
  %1 = getelementptr inbounds [2 x i8], [2 x i8]* %arr1, i32 0, i32 0
  store [2 x i8] c" \00", [2 x i8]* %arr2, align 1
  %2 = getelementptr inbounds [2 x i8], [2 x i8]* %arr2, i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr3, align 1
  %3 = getelementptr inbounds [2 x i8], [2 x i8]* %arr3, i32 0, i32 0
  store [10 x i8] c"%s %s %s\0A\00", [10 x i8]* %arr4, align 1
  %4 = getelementptr inbounds [10 x i8], [10 x i8]* %arr4, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %4, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @TYPEOF, i32 0, i32 0), i8* getelementptr inbounds ([5 x i8], [5 x i8]* @TYPEOF.1, i32 0, i32 0), i8* getelementptr inbounds ([5 x i8], [5 x i8]* @TYPEOF.2, i32 0, i32 0))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 004 — builtin: typeof on a ref and on a type name

```ura
struct User:
   count i32

fn main() i32:
   u User
   p &User = &u
   output(typeof(&p), " ", typeof(i32), "\n")
   return 0
```

---

## 005 — builtin: sizeof folds to a constant

```ura
struct Pad:
   flag i8
   count i32

fn main() i32:
   x i32
   s i8[]
   p Pad
   return sizeof(x) + sizeof(s) + sizeof(p)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Pad = type { i8, i32 }

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %s = alloca i8*, align 8
  %p = alloca %Pad, align 8
  store i32 0, i32* %x, align 4
  store i8* null, i8** %s, align 8
  store %Pad zeroinitializer, %Pad* %p, align 4
  ret i32 add (i32 add (i32 ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i32), i32 ptrtoint (i1** getelementptr (i1*, i1** null, i32 1) to i32)), i32 ptrtoint (%Pad* getelementptr (%Pad, %Pad* null, i32 1) to i32))
}
```

---

## 006 — builtin: typeof / sizeof on scalars

```ura
// stdout: typeof x = i32, sizeof x = 4
// stdout: typeof f = f32, sizeof f = 4
// stdout: sizeof i64 = 8
// expect: 0

fn main() i32:
    x i32   = 5
    f f32 = 1.5
    output("typeof x = ", typeof(x), ", sizeof x = ", sizeof(x), "\n")
    output("typeof f = ", typeof(f), ", sizeof f = ", sizeof(f), "\n")
    output("sizeof i64 = ", sizeof(1 as i64), "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@TYPEOF = private unnamed_addr constant [4 x i8] c"i32\00", align 1
@TYPEOF.1 = private unnamed_addr constant [4 x i8] c"f32\00", align 1

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %f = alloca float, align 4
  %arr = alloca [12 x i8], align 1
  %arr1 = alloca [14 x i8], align 1
  %arr2 = alloca [2 x i8], align 1
  %arr3 = alloca [30 x i8], align 1
  %arr4 = alloca [12 x i8], align 1
  %arr5 = alloca [14 x i8], align 1
  %arr6 = alloca [2 x i8], align 1
  %arr7 = alloca [30 x i8], align 1
  %arr9 = alloca [14 x i8], align 1
  %arr10 = alloca [2 x i8], align 1
  %arr11 = alloca [17 x i8], align 1
  store i32 5, i32* %x, align 4
  store float 1.500000e+00, float* %f, align 4
  store [12 x i8] c"typeof x = \00", [12 x i8]* %arr, align 1
  %0 = getelementptr inbounds [12 x i8], [12 x i8]* %arr, i32 0, i32 0
  store [14 x i8] c", sizeof x = \00", [14 x i8]* %arr1, align 1
  %1 = getelementptr inbounds [14 x i8], [14 x i8]* %arr1, i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr2, align 1
  %2 = getelementptr inbounds [2 x i8], [2 x i8]* %arr2, i32 0, i32 0
  store [30 x i8] c"typeof x = %s, sizeof x = %d\0A\00", [30 x i8]* %arr3, align 1
  %3 = getelementptr inbounds [30 x i8], [30 x i8]* %arr3, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %3, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @TYPEOF, i32 0, i32 0), i32 ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i32))
  store [12 x i8] c"typeof f = \00", [12 x i8]* %arr4, align 1
  %4 = getelementptr inbounds [12 x i8], [12 x i8]* %arr4, i32 0, i32 0
  store [14 x i8] c", sizeof f = \00", [14 x i8]* %arr5, align 1
  %5 = getelementptr inbounds [14 x i8], [14 x i8]* %arr5, i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr6, align 1
  %6 = getelementptr inbounds [2 x i8], [2 x i8]* %arr6, i32 0, i32 0
  store [30 x i8] c"typeof f = %s, sizeof f = %d\0A\00", [30 x i8]* %arr7, align 1
  %7 = getelementptr inbounds [30 x i8], [30 x i8]* %arr7, i32 0, i32 0
  %call.dprintf8 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %7, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @TYPEOF.1, i32 0, i32 0), i32 ptrtoint (float* getelementptr (float, float* null, i32 1) to i32))
  store [14 x i8] c"sizeof i64 = \00", [14 x i8]* %arr9, align 1
  %8 = getelementptr inbounds [14 x i8], [14 x i8]* %arr9, i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr10, align 1
  %9 = getelementptr inbounds [2 x i8], [2 x i8]* %arr10, i32 0, i32 0
  store [17 x i8] c"sizeof i64 = %d\0A\00", [17 x i8]* %arr11, align 1
  %10 = getelementptr inbounds [17 x i8], [17 x i8]* %arr11, i32 0, i32 0
  %call.dprintf12 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %10, i32 ptrtoint (i64* getelementptr (i64, i64* null, i32 1) to i32))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 007 — builtin: typeof / sizeof on an array

```ura
// stdout: typeof a = array, sizeof a = 16
// expect: 0

fn main() i32:
    a i32[] = [1, 2, 3]
    output("typeof a = ", typeof(a), ", sizeof a = ", sizeof(a), "\n")
    return 0
```

---

## 008 — builtin: sizeof without parentheses

```ura
// error: Expected '(' after 'sizeof'
fn main() i32:
    x i32 = 5
    output(sizeof x)
    return 0
```

---

## 009 — builtin: typeof without parentheses

```ura
// error: Expected '(' after 'typeof'
fn main() i32:
    x i32 = 5
    output(typeof x)
    return 0
```

---

## 010 — builtin: typeof and sizeof on chars, bool, and a literal

```ura
// stdout: array bool
// stdout: 16 16 1
// expect: 0

fn main() i32:
    s char[] = "hello"
    b bool  = True
    output(typeof(s), " ", typeof(b), "\n")
    output(sizeof("hello"), " ", sizeof(s), " ", sizeof(b), "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@TYPEOF = private unnamed_addr constant [5 x i8] c"i8[]\00", align 1
@TYPEOF.1 = private unnamed_addr constant [5 x i8] c"bool\00", align 1

define i32 @main() {
entry:
  %s = alloca i8*, align 8
  %arr = alloca [6 x i8], align 1
  %b = alloca i1, align 1
  %arr1 = alloca [2 x i8], align 1
  %arr2 = alloca [2 x i8], align 1
  %arr3 = alloca [7 x i8], align 1
  %arr4 = alloca [6 x i8], align 1
  %arr5 = alloca [2 x i8], align 1
  %arr6 = alloca [2 x i8], align 1
  %arr7 = alloca [2 x i8], align 1
  %arr8 = alloca [10 x i8], align 1
  store [6 x i8] c"hello\00", [6 x i8]* %arr, align 1
  %0 = getelementptr inbounds [6 x i8], [6 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %s, align 8
  store i1 true, i1* %b, align 1
  store [2 x i8] c" \00", [2 x i8]* %arr1, align 1
  %1 = getelementptr inbounds [2 x i8], [2 x i8]* %arr1, i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr2, align 1
  %2 = getelementptr inbounds [2 x i8], [2 x i8]* %arr2, i32 0, i32 0
  store [7 x i8] c"%s %s\0A\00", [7 x i8]* %arr3, align 1
  %3 = getelementptr inbounds [7 x i8], [7 x i8]* %arr3, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %3, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @TYPEOF, i32 0, i32 0), i8* getelementptr inbounds ([5 x i8], [5 x i8]* @TYPEOF.1, i32 0, i32 0))
  store [6 x i8] c"hello\00", [6 x i8]* %arr4, align 1
  %4 = getelementptr inbounds [6 x i8], [6 x i8]* %arr4, i32 0, i32 0
  store [2 x i8] c" \00", [2 x i8]* %arr5, align 1
  %5 = getelementptr inbounds [2 x i8], [2 x i8]* %arr5, i32 0, i32 0
  store [2 x i8] c" \00", [2 x i8]* %arr6, align 1
  %6 = getelementptr inbounds [2 x i8], [2 x i8]* %arr6, i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr7, align 1
  %7 = getelementptr inbounds [2 x i8], [2 x i8]* %arr7, i32 0, i32 0
  store [10 x i8] c"%d %d %d\0A\00", [10 x i8]* %arr8, align 1
  %8 = getelementptr inbounds [10 x i8], [10 x i8]* %arr8, i32 0, i32 0
  %call.dprintf9 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %8, i32 ptrtoint (i1** getelementptr (i1*, i1** null, i32 1) to i32), i32 ptrtoint (i1** getelementptr (i1*, i1** null, i32 1) to i32), i32 ptrtoint (i1* getelementptr (i1, i1* null, i32 1) to i32))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 011 — builtin: sizeof of every scalar type

```ura
// stdout: 1 2 4 8
// stdout: 1 4 4 8
// stdout: 1 1
// expect: 0
fn main() i32:
    output(sizeof(i8), " ", sizeof(i16), " ", sizeof(i32), " ", sizeof(i64), "\n")
    output(sizeof(u8), " ", sizeof(u32), " ", sizeof(f32), " ", sizeof(f64), "\n")
    output(sizeof(bool), " ", sizeof(char), "\n")
    return 0
```

---

## 012 — builtin: sizeof of a struct honours ABI padding

```ura
// stdout: 16
// expect: 0
struct Point:
    x i32
    y i64

fn main() i32:
    p Point
    output(sizeof(p), "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Point = type { i32, i64 }

define i32 @main() {
entry:
  %p = alloca %Point, align 8
  %arr = alloca [2 x i8], align 1
  %arr1 = alloca [4 x i8], align 1
  store %Point zeroinitializer, %Point* %p, align 8
  store [2 x i8] c"\0A\00", [2 x i8]* %arr, align 1
  %0 = getelementptr inbounds [2 x i8], [2 x i8]* %arr, i32 0, i32 0
  store [4 x i8] c"%d\0A\00", [4 x i8]* %arr1, align 1
  %1 = getelementptr inbounds [4 x i8], [4 x i8]* %arr1, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %1, i32 ptrtoint (%Point* getelementptr (%Point, %Point* null, i32 1) to i32))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 013 — builtin: typeof of a struct gives its name

```ura
// stdout: Point
// expect: 0
struct Point:
    x i32
    y i64

fn main() i32:
    p Point
    output(typeof(p), "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Point = type { i32, i64 }

@TYPEOF = private unnamed_addr constant [6 x i8] c"Point\00", align 1

define i32 @main() {
entry:
  %p = alloca %Point, align 8
  %arr = alloca [2 x i8], align 1
  %arr1 = alloca [4 x i8], align 1
  store %Point zeroinitializer, %Point* %p, align 8
  store [2 x i8] c"\0A\00", [2 x i8]* %arr, align 1
  %0 = getelementptr inbounds [2 x i8], [2 x i8]* %arr, i32 0, i32 0
  store [4 x i8] c"%s\0A\00", [4 x i8]* %arr1, align 1
  %1 = getelementptr inbounds [4 x i8], [4 x i8]* %arr1, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @TYPEOF, i32 0, i32 0))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 014 — cast: as casting + i64 / i16 via explicit cast

```ura
// stdout: 300
// stdout: 300
// stdout: 300
// stdout: 5
// expect: 0

fn main() i32:
    a i32   = 300
    big i64  = a as i64
    small i16 = a as i16
    c i32   = big as i32
    output(big, "\n")
    output(small, "\n")
    output(c, "\n")
    output(5 as i64, "\n")
    return 0
```

---

## 015 — cast: as converts between f32 and the integer types

```ura
// stdout: i32->f32 300.000000
// stdout: f32->i32 2 -2
// stdout: i64->f32 7.000000
// stdout: i16->f32 5.000000
// stdout: char->f32 65.000000 char->i32 65
// stdout: bool->f32 1.000000 bool->i32 1
// stdout: f32->i64 9
// stdout: round trip 4.000000
// expect: 0

fn main() i32:
    i i32   = 300
    f f32 = i as f32
    output("i32->f32 ", f, "\n")
    output("f32->i32 ", 2.9 as i32, " ", (0.0 - 2.9) as i32, "\n")
    l i64  = 7 as i64
    output("i64->f32 ", l as f32, "\n")
    s i16 = 5 as i16
    output("i16->f32 ", s as f32, "\n")
    c char  = 'A'
    output("char->f32 ", c as f32, " char->i32 ", c as i32, "\n")
    b bool  = True
    output("bool->f32 ", b as f32, " bool->i32 ", b as i32, "\n")
    output("f32->i64 ", 9.75 as i64, "\n")
    output("round trip ", 4.9 as i32 as f32, "\n")
    return 0
```

---

## 016 — cast: char arithmetic and an expression cast back to char

```ura
// stdout: A 65 B
// stdout: 'd' - 'a' = 3
// stdout: spider
// expect: 0

fn main() i32:
    grade char = 'A'
    code  i32  = grade as i32
    // an expression cast back to char
    back  char = (code + 1) as char
    output(grade, " ", code, " ", back, "\n")

    // arithmetic on two char literals
    diff i32 = 'd' as i32 - 'a' as i32
    output("'d' - 'a' = ", diff, "\n")

    // an expression used as an array index
    roll i32 = 7
    names char[][] = ["orc", "spider", "dragon"]
    output(names[roll % 3], "\n")
    return 0
```

---

## 017 — cast: a struct cannot be cast to or from a scalar

```ura
// error: Cannot cast Point to i32

struct Point:
    x i32
    y i32

fn main() i32:
    p Point
    n i32 = p as i32
    output(n, "\n")
    return 0
```

---

## 018 — cast: casting a scalar to a struct

```ura
// error: Cannot cast i32 to Point

struct Point:
    x i32
    y i32

fn main() i32:
    p Point = 5 as Point
    output(p.x, "\n")
    return 0
```

---

## 019 — literal: f32 literals: declare, print, zero-init, in a function

```ura
// stdout: 3.140000
// stdout: 2.710000
// stdout: 0.500000
// stdout: 0.000000
// expect: 0

fn half() f32:
    return 0.5

fn main() i32:
    x f32 = 3.14
    output(x, "\n")
    output(2.71, "\n")
    output(half(), "\n")
    y f32
    output(y, "\n")
    return 0
```

---

## 020 — literal: f32 arithmetic + comparison, compound assignment

```ura
// stdout: 5.000000
// stdout: 1.000000
// stdout: 6.000000
// stdout: 1.500000
// stdout: True
// stdout: False
// stdout: 4.500000
// stdout: 9.000000
// expect: 0

fn main() i32:
    a f32 = 3.0
    b f32 = 2.0
    output(a + b, "\n")
    output(a - b, "\n")
    output(a * b, "\n")
    output(a / b, "\n")
    output(a > b, "\n")
    output(a == b, "\n")
    a += 1.5
    output(a, "\n")
    a *= 2.0
    output(a, "\n")
    return 0
```

---

## 021 — literal: f64 declaration, arithmetic and compound assignment

```ura
// stdout: 3.141593 2.000000
// stdout: 5.141593 1.141593 6.283185 1.570796
// stdout: True True True
// stdout: 6.000000
// stdout: 0.000000
// expect: 0

fn main() i32:
    d f64 = 3.141592653589793
    e f64 = 2.0
    output(d, " ", e, "\n")
    output(d + e, " ", d - e, " ", d * e, " ", d / e, "\n")
    output(d > e, " ", d == d, " ", d != e, "\n")

    c f64 = 10.0
    c += 2.5
    c -= 0.5
    c *= 2.0
    c /= 4.0
    output(c, "\n")

    // zero-init
    z f64
    output(z, "\n")
    return 0
```

---

## 022 — literal: a f32 literal takes the type of its target

```ura
// stdout: False
// stdout: -5.000000 3.000000
// stdout: 0.500000
// expect: 0

fn half() f64:
    return 0.5

fn scale(x f64) f64:
    return x * 2.0

fn main() i32:
    // same digits, two different types
    d f64 = 3.141592653589793
    f f32  = 3.141592653589793
    // the f32 rounding is lossy, so widening it back does NOT equal d
    output((f as f64) == d, "\n")

    // an expression built only from literals adopts too, and unary minus
    // desugars to 0 - x, so its synthesised zero must be a f32
    output(scale(-2.5), " ", scale(1.0 + 0.5), "\n")

    // inference at a return
    output(half(), "\n")
    return 0
```

---

## 023 — literal: casts between f64, f32 and the integer types

```ura
// stdout: d->f32 3.750000  f->f64 1.500000
// stdout: d->i32 3  d->i64 3
// stdout: d->i16 3  d->char 
// stdout: i32->d 7.000000  bool->d 1.000000
// stdout: round trip 3.000000
// expect: 0

fn main() i32:
    d f64 = 3.75
    f f32  = 1.5
    output("d->f32 ", d as f32, "  f->f64 ", f as f64, "\n")
    output("d->i32 ", d as i32, "  d->i64 ", d as i64, "\n")
    output("d->i16 ", d as i16, "  d->char ", d as char, "\n")
    output("i32->d ", 7 as f64, "  bool->d ", True as f64, "\n")
    output("round trip ", d as i32 as f64, "\n")
    return 0
```

---

## 024 — literal: a return that does not match the function's type

```ura
// error: 'f' returns i32, but this returns f32

fn f() i32:
    return 1.5

fn main() i32:
    output(f(), "\n")
    return 0
```

---

## 025 — literal: integer literals at the edge of each type's range

```ura
// stdout: 9223372036854775807 -9223372036854775807
// stdout: 2147483647 -2147483647
// stdout: 9007199254740993
// expect: 0

fn main() i32:
    i64_max i64 = 9223372036854775807
    i64_min i64 = -9223372036854775807
    i32_max i32 = 2147483647
    i32_min i32 = -2147483647
    exact   i64 = 9007199254740993

    output(i64_max, " ", i64_min, "\n")
    output(i32_max, " ", i32_min, "\n")
    output(exact, "\n")
    return 0
```

---

## 026 — literal: an integer literal too wide for i32 keeps its width

```ura
// stdout: 9000000000
// stdout: 9000000000
// stdout: 9000000001
// stdout: 9000000001
// stdout: 18000000000
// stdout: 42 2147483647
// expect: 0

fn twice(n i64) i64:
    ret n * 2

fn main() i32:
    with_target i64 = 9000000000
    output(with_target, "\n")
    output(9000000000, "\n")
    output(9000000000 + 1, "\n")
    output(1 + 9000000000, "\n")
    output(twice(9000000000), "\n")
    output(42, " ", 2147483647, "\n")
    return 0
```

---

## 027 — literal: a literal that does not fit its target type

```ura
// error: 300 does not fit in 'i8' (range -128 to 127); widen the type, or convert explicitly with 'as'

fn main() i32:
    small i8 = 300
    return 0
```

---

## 028 — literal: a negative literal at the low edge still fits

```ura
// stdout: -128 -32768 255 65535
// expect: 0

fn main() i32:
    a i8  = -128
    b i16 = -32768
    c u8  = 255
    d u16 = 65535
    output(a, " ", b, " ", c, " ", d, "\n")
    return 0
```

---

## 029 — operator: bitwise & | ^ << >>

```ura
// stdout: 8
// stdout: 14
// stdout: 6
// stdout: 48
// stdout: 6
// expect: 0

fn main() i32:
    a i32 = 12
    b i32 = 10
    output(a & b, "\n")
    output(a | b, "\n")
    output(a ^ b, "\n")
    output(a << 2, "\n")
    output(a >> 1, "\n")
    return 0
```

---

## 030 — operator: unary not / minus / bitwise-not

```ura
// stdout: -5
// stdout: False
// stdout: -6
// stdout: True
// stdout: 5
// expect: 0

fn main() i32:
    a i32 = 5
    t bool = True
    output(-a, "\n")
    output(not t, "\n")
    output(~a, "\n")
    output(not (a < 3), "\n")
    output(-a + 10, "\n")
    return 0
```

---

## 031 — operator: compound bitwise assignment

```ura
// stdout: and 8  or 15  xor 6
// stdout: shl 48  shr 12
// stdout: unsigned shr 2000000000
// stdout: signed shr -4
// expect: 0

fn main() i32:
    a i32 = 12
    a &= 10
    b i32 = 12
    b |= 3
    c i32 = 12
    c ^= 10
    output("and ", a, "  or ", b, "  xor ", c, "\n")

    d i32 = 3
    d <<= 4
    e i32 = 48
    e >>= 2
    output("shl ", d, "  shr ", e, "\n")

    // an unsigned right shift is logical, not arithmetic
    u u32 = 4000000000 as u32
    u >>= 1
    output("unsigned shr ", u, "\n")

    // a signed one keeps the sign
    s i32 = 0 - 16
    s >>= 2
    output("signed shr ", s, "\n")
    return 0
```

---

## 032 — operator: bitwise operator on f32 operands is rejected

```ura
// error: Bitwise and shift operators require integer operands

fn main() i32:
    x f32 = 1.5 & 2.0
    return 0
```

---

## 033 — operator: divide by zero prints file:line and exits 1

```ura
// stderr: runtime error: Division by zero
// expect: 1

fn main() i32:
    b i32 = 0
    a i32 = 10
    c i32 = a / b
    return 0
```

---

## 034 — operator: f32 division by zero traps (not inf)

```ura
// stderr: runtime error: Division by zero
// expect: 1

fn main() i32:
    b f32 = 0.0
    a f32 = 10.0
    c f32 = a / b
    return 0
```

---

## 035 — literal: f32 and f64 math, casts and printf

```ura
// stdout: 5.000000 1.000000 6.000000 1.500000
// stdout: 4.500000 9.000000 -9.000000
// stdout: 3.750000 3 7.000000 1.000000
// expect: 7
proto printf(fmt i8[], ...) i32

fn half() f32:
    return 0.5

fn main() i32:
    a f32 = 3.0
    b f32 = 2.0
    printf("%f %f %f %f\n", a + b, a - b, a * b, a / b)
    a += 1.5
    c f64 = 4.5
    c *= 2.0
    printf("%f %f %f\n", a, c, -c)
    d f64 = 3.75
    printf("%f %d %f %f\n", d, d as i32, 7 as f64, True as f64)
    if a > b and half() == 0.5:
        return 7
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

declare i32 @printf(i8*, ...)

define float @half() {
entry:
  ret float 5.000000e-01
}

define i32 @main() {
entry:
  %a = alloca float, align 4
  %b = alloca float, align 4
  %arr = alloca [13 x i8], align 1
  %c = alloca double, align 8
  %arr10 = alloca [10 x i8], align 1
  %d = alloca double, align 8
  %arr15 = alloca [13 x i8], align 1
  store float 3.000000e+00, float* %a, align 4
  store float 2.000000e+00, float* %b, align 4
  store [13 x i8] c"%f %f %f %f\0A\00", [13 x i8]* %arr, align 1
  %0 = getelementptr inbounds [13 x i8], [13 x i8]* %arr, i32 0, i32 0
  %"$a" = load float, float* %a, align 4
  %"$b" = load float, float* %b, align 4
  %ADD = fadd float %"$a", %"$b"
  %"$a1" = load float, float* %a, align 4
  %"$b2" = load float, float* %b, align 4
  %SUB = fsub float %"$a1", %"$b2"
  %"$a3" = load float, float* %a, align 4
  %"$b4" = load float, float* %b, align 4
  %MUL = fmul float %"$a3", %"$b4"
  %"$a5" = load float, float* %a, align 4
  %"$b6" = load float, float* %b, align 4
  %DIV = fdiv float %"$a5", %"$b6"
  %1 = fpext float %ADD to double
  %2 = fpext float %SUB to double
  %3 = fpext float %MUL to double
  %4 = fpext float %DIV to double
  %call.printf = call i32 (i8*, ...) @printf(i8* %0, double %1, double %2, double %3, double %4)
  %"$a7" = load float, float* %a, align 4
  %ADD8 = fadd float %"$a7", 1.500000e+00
  store float %ADD8, float* %a, align 4
  store double 4.500000e+00, double* %c, align 8
  %"$c" = load double, double* %c, align 8
  %MUL9 = fmul double %"$c", 2.000000e+00
  store double %MUL9, double* %c, align 8
  store [10 x i8] c"%f %f %f\0A\00", [10 x i8]* %arr10, align 1
  %5 = getelementptr inbounds [10 x i8], [10 x i8]* %arr10, i32 0, i32 0
  %"$c11" = load double, double* %c, align 8
  %NEG = fneg double %"$c11"
  %"$a12" = load float, float* %a, align 4
  %"$c13" = load double, double* %c, align 8
  %6 = fpext float %"$a12" to double
  %call.printf14 = call i32 (i8*, ...) @printf(i8* %5, double %6, double %"$c13", double %NEG)
  store double 3.750000e+00, double* %d, align 8
  store [13 x i8] c"%f %d %f %f\0A\00", [13 x i8]* %arr15, align 1
  %7 = getelementptr inbounds [13 x i8], [13 x i8]* %arr15, i32 0, i32 0
  %"$d" = load double, double* %d, align 8
  %AS = fptosi double %"$d" to i32
  %"$d16" = load double, double* %d, align 8
  %call.printf17 = call i32 (i8*, ...) @printf(i8* %7, double %"$d16", i32 %AS, double 7.000000e+00, double 1.000000e+00)
  br label %if0

if0:                                              ; preds = %entry
  %"$a18" = load float, float* %a, align 4
  %"$b19" = load float, float* %b, align 4
  %GT = fcmp ogt float %"$a18", %"$b19"
  %call.half = call float @half()
  %EQ = fcmp oeq float %call.half, 5.000000e-01
  %AND = and i1 %GT, %EQ
  br i1 %AND, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 7

endif:                                            ; preds = %if0
  ret i32 0
}
```

