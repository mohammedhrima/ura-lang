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
// error: 'typeof' takes one argument, got 0
fn main() i32:
   x i32 = 1
   return typeof()
```

### stderr

```
error: test.ura:4:11 'typeof' takes one argument, got 0
  |
4 |    return typeof()
  |           ^^^^^^
help: use typeof(x)
error: test.ura:4:11 expected 'i32', found 'i8[]'
  |
4 |    return typeof()
  |           ^^^^^^
error: aborting due to 2 errors
```

### status

```
1
```

---

## 002 — builtins: sizeof with two arguments

```ura
// error: 'sizeof' takes one argument, got 2
fn main() i32:
   x i32 = 1
   return sizeof(x, x)
```

### stderr

```
error: test.ura:4:11 'sizeof' takes one argument, got 2
  |
4 |    return sizeof(x, x)
  |           ^^^^^^
help: use sizeof(x)
error: aborting due to 1 error
```

### status

```
1
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
@str = private unnamed_addr constant [10 x i8] c"%s %s %s\0A\00", align 1

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %s = alloca i8*, align 8
  %arr = alloca [4 x i8], align 1
  %u = alloca %User, align 8
  store i32 1, i32* %x, align 4
  store [4 x i8] c"abc\00", [4 x i8]* %arr, align 1
  %0 = getelementptr inbounds [4 x i8], [4 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %s, align 8
  store %User zeroinitializer, %User* %u, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @TYPEOF, i32 0, i32 0), i8* getelementptr inbounds ([5 x i8], [5 x i8]* @TYPEOF.1, i32 0, i32 0), i8* getelementptr inbounds ([5 x i8], [5 x i8]* @TYPEOF.2, i32 0, i32 0))
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32 }

@TYPEOF = private unnamed_addr constant [7 x i8] c"User[]\00", align 1
@TYPEOF.1 = private unnamed_addr constant [4 x i8] c"i32\00", align 1
@str = private unnamed_addr constant [7 x i8] c"%s %s\0A\00", align 1

define i32 @main() {
entry:
  %u = alloca %User, align 8
  %p = alloca %User*, align 8
  store %User zeroinitializer, %User* %u, align 4
  store %User* %u, %User** %p, align 8
  %"*p" = load %User*, %User** %p, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @TYPEOF, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @TYPEOF.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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
@str = private unnamed_addr constant [30 x i8] c"typeof x = %s, sizeof x = %d\0A\00", align 1
@TYPEOF.1 = private unnamed_addr constant [4 x i8] c"f32\00", align 1
@str.2 = private unnamed_addr constant [30 x i8] c"typeof f = %s, sizeof f = %d\0A\00", align 1
@str.3 = private unnamed_addr constant [17 x i8] c"sizeof i64 = %d\0A\00", align 1

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %f = alloca float, align 4
  store i32 5, i32* %x, align 4
  store float 1.500000e+00, float* %f, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([30 x i8], [30 x i8]* @str, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @TYPEOF, i32 0, i32 0), i32 ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i32))
  %call.dprintf1 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([30 x i8], [30 x i8]* @str.2, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @TYPEOF.1, i32 0, i32 0), i32 ptrtoint (float* getelementptr (float, float* null, i32 1) to i32))
  %call.dprintf2 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.3, i32 0, i32 0), i32 ptrtoint (i64* getelementptr (i64, i64* null, i32 1) to i32))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 007 — builtin: typeof / sizeof on an array

```ura
// stdout: typeof a = i32[], sizeof a = 8
// expect: 0

fn main() i32:
    a i32[] = [1, 2, 3]
    output("typeof a = ", typeof(a), ", sizeof a = ", sizeof(a), "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@TYPEOF = private unnamed_addr constant [6 x i8] c"i32[]\00", align 1
@str = private unnamed_addr constant [30 x i8] c"typeof a = %s, sizeof a = %d\0A\00", align 1

define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %arr = alloca [3 x i32], align 4
  store [3 x i32] [i32 1, i32 2, i32 3], [3 x i32]* %arr, align 4
  %0 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %a, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([30 x i8], [30 x i8]* @str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @TYPEOF, i32 0, i32 0), i32 ptrtoint (i1** getelementptr (i1*, i1** null, i32 1) to i32))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### stderr

```
error: test.ura:4:19 expected '(' after 'sizeof'
  |
4 |     output(sizeof x)
  |                   ^
help: use output(sizeof(x) x)
error: aborting due to 1 error
```

### status

```
1
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

### stderr

```
error: test.ura:4:19 expected '(' after 'typeof'
  |
4 |     output(typeof x)
  |                   ^
help: use output(typeof(x) x)
error: aborting due to 1 error
```

### status

```
1
```

---

## 010 — builtin: typeof and sizeof on chars, bool, and a literal

```ura
// stdout: i8[] bool
// stdout: 16 8 1
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

%String = type { i8*, i32, i32 }

@TYPEOF = private unnamed_addr constant [5 x i8] c"i8[]\00", align 1
@TYPEOF.1 = private unnamed_addr constant [5 x i8] c"bool\00", align 1
@str = private unnamed_addr constant [7 x i8] c"%s %s\0A\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@str.3 = private unnamed_addr constant [10 x i8] c"%d %d %d\0A\00", align 1

define i32 @main() {
entry:
  %s = alloca i8*, align 8
  %arr = alloca [6 x i8], align 1
  %b = alloca i1, align 1
  store [6 x i8] c"hello\00", [6 x i8]* %arr, align 1
  %0 = getelementptr inbounds [6 x i8], [6 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %s, align 8
  store i1 true, i1* %b, align 1
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i8* getelementptr inbounds ([5 x i8], [5 x i8]* @TYPEOF, i32 0, i32 0), i8* getelementptr inbounds ([5 x i8], [5 x i8]* @TYPEOF.1, i32 0, i32 0))
  %call.dprintf1 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.3, i32 0, i32 0), i32 ptrtoint (%String* getelementptr (%String, %String* null, i32 1) to i32), i32 ptrtoint (i1** getelementptr (i1*, i1** null, i32 1) to i32), i32 ptrtoint (i1* getelementptr (i1, i1* null, i32 1) to i32))
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [13 x i8] c"%d %d %d %d\0A\00", align 1
@str.1 = private unnamed_addr constant [13 x i8] c"%d %d %d %d\0A\00", align 1
@str.2 = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1

define i32 @main() {
entry:
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str, i32 0, i32 0), i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32), i32 ptrtoint (i16* getelementptr (i16, i16* null, i32 1) to i32), i32 ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i32), i32 ptrtoint (i64* getelementptr (i64, i64* null, i32 1) to i32))
  %call.dprintf1 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.1, i32 0, i32 0), i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32), i32 ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i32), i32 ptrtoint (float* getelementptr (float, float* null, i32 1) to i32), i32 ptrtoint (double* getelementptr (double, double* null, i32 1) to i32))
  %call.dprintf2 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.2, i32 0, i32 0), i32 ptrtoint (i1* getelementptr (i1, i1* null, i32 1) to i32), i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @main() {
entry:
  %p = alloca %Point, align 8
  store %Point zeroinitializer, %Point* %p, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 ptrtoint (%Point* getelementptr (%Point, %Point* null, i32 1) to i32))
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
@str = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

define i32 @main() {
entry:
  %p = alloca %Point, align 8
  store %Point zeroinitializer, %Point* %p, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @TYPEOF, i32 0, i32 0))
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [6 x i8] c"%lld\0A\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"%lld\0A\00", align 1

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %big = alloca i64, align 8
  %small = alloca i16, align 2
  %c = alloca i32, align 4
  store i32 300, i32* %a, align 4
  %"$a" = load i32, i32* %a, align 4
  %AS = sext i32 %"$a" to i64
  store i64 %AS, i64* %big, align 8
  %"$a1" = load i32, i32* %a, align 4
  %AS2 = trunc i32 %"$a1" to i16
  store i16 %AS2, i16* %small, align 2
  %"$big" = load i64, i64* %big, align 8
  %AS3 = trunc i64 %"$big" to i32
  store i32 %AS3, i32* %c, align 4
  %"$big4" = load i64, i64* %big, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i64 %"$big4")
  %"$small" = load i16, i16* %small, align 2
  %AS5 = sext i16 %"$small" to i32
  %call.dprintf6 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 %AS5)
  %"$c" = load i32, i32* %c, align 4
  %call.dprintf7 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 %"$c")
  %call.dprintf8 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i64 5)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [13 x i8] c"i32->f32 %f\0A\00", align 1
@str.1 = private unnamed_addr constant [16 x i8] c"f32->i32 %d %d\0A\00", align 1
@str.2 = private unnamed_addr constant [13 x i8] c"i64->f32 %f\0A\00", align 1
@str.3 = private unnamed_addr constant [13 x i8] c"i16->f32 %f\0A\00", align 1
@str.4 = private unnamed_addr constant [27 x i8] c"char->f32 %f char->i32 %d\0A\00", align 1
@str.5 = private unnamed_addr constant [27 x i8] c"bool->f32 %f bool->i32 %d\0A\00", align 1
@str.6 = private unnamed_addr constant [15 x i8] c"f32->i64 %lld\0A\00", align 1
@str.7 = private unnamed_addr constant [15 x i8] c"round trip %f\0A\00", align 1

define i32 @main() {
entry:
  %i = alloca i32, align 4
  %f = alloca float, align 4
  %l = alloca i64, align 8
  %s = alloca i16, align 2
  %c = alloca i8, align 1
  %b = alloca i1, align 1
  store i32 300, i32* %i, align 4
  %"$i" = load i32, i32* %i, align 4
  %AS = sitofp i32 %"$i" to float
  store float %AS, float* %f, align 4
  %"$f" = load float, float* %f, align 4
  %AS1 = fpext float %"$f" to double
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str, i32 0, i32 0), double %AS1)
  %call.dprintf2 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.1, i32 0, i32 0), i32 2, i32 -2)
  store i64 7, i64* %l, align 8
  %"$l" = load i64, i64* %l, align 8
  %AS3 = sitofp i64 %"$l" to float
  %AS4 = fpext float %AS3 to double
  %call.dprintf5 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.2, i32 0, i32 0), double %AS4)
  store i16 5, i16* %s, align 2
  %"$s" = load i16, i16* %s, align 2
  %AS6 = sitofp i16 %"$s" to float
  %AS7 = fpext float %AS6 to double
  %call.dprintf8 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.3, i32 0, i32 0), double %AS7)
  store i8 65, i8* %c, align 1
  %"$c" = load i8, i8* %c, align 1
  %AS9 = sitofp i8 %"$c" to float
  %"$c10" = load i8, i8* %c, align 1
  %AS11 = sext i8 %"$c10" to i32
  %AS12 = fpext float %AS9 to double
  %call.dprintf13 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([27 x i8], [27 x i8]* @str.4, i32 0, i32 0), double %AS12, i32 %AS11)
  store i1 true, i1* %b, align 1
  %"$b" = load i1, i1* %b, align 1
  %AS14 = uitofp i1 %"$b" to float
  %"$b15" = load i1, i1* %b, align 1
  %AS16 = zext i1 %"$b15" to i32
  %AS17 = fpext float %AS14 to double
  %call.dprintf18 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([27 x i8], [27 x i8]* @str.5, i32 0, i32 0), double %AS17, i32 %AS16)
  %call.dprintf19 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.6, i32 0, i32 0), i64 9)
  %call.dprintf20 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.7, i32 0, i32 0), double 4.000000e+00)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [10 x i8] c"%c %d %c\0A\00", align 1
@str.1 = private unnamed_addr constant [16 x i8] c"'d' - 'a' = %d\0A\00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"orc\00", align 1
@str.3 = private unnamed_addr constant [7 x i8] c"spider\00", align 1
@str.4 = private unnamed_addr constant [7 x i8] c"dragon\00", align 1
@str.5 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

define i32 @main() {
entry:
  %grade = alloca i8, align 1
  %code = alloca i32, align 4
  %back = alloca i8, align 1
  %diff = alloca i32, align 4
  %roll = alloca i32, align 4
  %names = alloca i8**, align 8
  %arr = alloca [3 x i8*], align 8
  store i8 65, i8* %grade, align 1
  %"$grade" = load i8, i8* %grade, align 1
  %AS = sext i8 %"$grade" to i32
  store i32 %AS, i32* %code, align 4
  %"$code" = load i32, i32* %code, align 4
  %ADD = add i32 %"$code", 1
  %AS1 = trunc i32 %ADD to i8
  store i8 %AS1, i8* %back, align 1
  %"$grade2" = load i8, i8* %grade, align 1
  %AS3 = sext i8 %"$grade2" to i32
  %"$code4" = load i32, i32* %code, align 4
  %"$back" = load i8, i8* %back, align 1
  %AS5 = sext i8 %"$back" to i32
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %AS3, i32 %"$code4", i32 %AS5)
  store i32 3, i32* %diff, align 4
  %"$diff" = load i32, i32* %diff, align 4
  %call.dprintf6 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.1, i32 0, i32 0), i32 %"$diff")
  store i32 7, i32* %roll, align 4
  store [3 x i8*] [i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.3, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.4, i32 0, i32 0)], [3 x i8*]* %arr, align 8
  %0 = getelementptr inbounds [3 x i8*], [3 x i8*]* %arr, i32 0, i32 0
  store i8** %0, i8*** %names, align 8
  %"$roll" = load i32, i32* %roll, align 4
  %MOD = srem i32 %"$roll", 3
  %"$names" = load i8**, i8*** %names, align 8
  %"names[]" = getelementptr inbounds i8*, i8** %"$names", i32 %MOD
  %"$names[]" = load i8*, i8** %"names[]", align 8
  %call.dprintf7 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.5, i32 0, i32 0), i8* %"$names[]")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### stderr

```
error: test.ura:9:15 can't cast 'Point' to 'i32'
  |
9 |     n i32 = p as i32
  |               ^^
error: aborting due to 1 error
```

### status

```
1
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

### stderr

```
error: test.ura:8:17 can't cast 'i32' to 'Point'
  |
8 |     p Point = 5 as Point
  |                 ^^
error: aborting due to 1 error
```

### status

```
1
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@str.3 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

define float @half() {
entry:
  ret float 5.000000e-01
}

define i32 @main() {
entry:
  %x = alloca float, align 4
  %y = alloca float, align 4
  store float 0x40091EB860000000, float* %x, align 4
  %"$x" = load float, float* %x, align 4
  %AS = fpext float %"$x" to double
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), double %AS)
  %call.dprintf1 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), double 2.710000e+00)
  %call.half = call float @half()
  %AS2 = fpext float %call.half to double
  %call.dprintf3 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), double %AS2)
  store float 0.000000e+00, float* %y, align 4
  %"$y" = load float, float* %y, align 4
  %AS4 = fpext float %"$y" to double
  %call.dprintf5 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.3, i32 0, i32 0), double %AS4)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@str.3 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@str.4 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.5 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.6 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.7 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.8 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.9 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.10 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@str.11 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

define i32 @main() {
entry:
  %a = alloca float, align 4
  %b = alloca float, align 4
  %bool.text = alloca i8*, align 8
  %bool.text19 = alloca i8*, align 8
  store float 3.000000e+00, float* %a, align 4
  store float 2.000000e+00, float* %b, align 4
  %"$a" = load float, float* %a, align 4
  %"$b" = load float, float* %b, align 4
  %ADD = fadd float %"$a", %"$b"
  %AS = fpext float %ADD to double
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), double %AS)
  %"$a2" = load float, float* %a, align 4
  %"$b3" = load float, float* %b, align 4
  %SUB = fsub float %"$a2", %"$b3"
  %AS4 = fpext float %SUB to double
  %call.dprintf5 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), double %AS4)
  %"$a6" = load float, float* %a, align 4
  %"$b7" = load float, float* %b, align 4
  %MUL = fmul float %"$a6", %"$b7"
  %AS8 = fpext float %MUL to double
  %call.dprintf9 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), double %AS8)
  %"$a10" = load float, float* %a, align 4
  %"$b11" = load float, float* %b, align 4
  %DIV = fdiv float %"$a10", %"$b11"
  %AS12 = fpext float %DIV to double
  %call.dprintf13 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.3, i32 0, i32 0), double %AS12)
  %"$a14" = load float, float* %a, align 4
  %"$b15" = load float, float* %b, align 4
  %GT = fcmp ogt float %"$a14", %"$b15"
  store i8* null, i8** %bool.text, align 8
  br label %if0

if0:                                              ; preds = %entry
  br i1 %GT, label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.4, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else1:                                            ; preds = %if0
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.5, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf16 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.6, i32 0, i32 0), i8* %"$bool.text")
  %"$a17" = load float, float* %a, align 4
  %"$b18" = load float, float* %b, align 4
  %EQ = fcmp oeq float %"$a17", %"$b18"
  store i8* null, i8** %bool.text19, align 8
  br label %if2

if2:                                              ; preds = %endif
  br i1 %EQ, label %if2.then, label %else3

if2.then:                                         ; preds = %if2
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.7, i32 0, i32 0), i8** %bool.text19, align 8
  br label %endif1

else3:                                            ; preds = %if2
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.8, i32 0, i32 0), i8** %bool.text19, align 8
  br label %endif1

endif1:                                           ; preds = %else3, %if2.then
  %"$bool.text20" = load i8*, i8** %bool.text19, align 8
  %call.dprintf21 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.9, i32 0, i32 0), i8* %"$bool.text20")
  %"$a22" = load float, float* %a, align 4
  %ADD23 = fadd float %"$a22", 1.500000e+00
  store float %ADD23, float* %a, align 4
  %"$a24" = load float, float* %a, align 4
  %AS25 = fpext float %"$a24" to double
  %call.dprintf26 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.10, i32 0, i32 0), double %AS25)
  %"$a27" = load float, float* %a, align 4
  %MUL28 = fmul float %"$a27", 2.000000e+00
  store float %MUL28, float* %a, align 4
  %"$a29" = load float, float* %a, align 4
  %AS30 = fpext float %"$a29" to double
  %call.dprintf31 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.11, i32 0, i32 0), double %AS30)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [7 x i8] c"%f %f\0A\00", align 1
@str.1 = private unnamed_addr constant [13 x i8] c"%f %f %f %f\0A\00", align 1
@str.2 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.4 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.5 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.6 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.7 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.8 = private unnamed_addr constant [10 x i8] c"%s %s %s\0A\00", align 1
@str.9 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1
@str.10 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

define i32 @main() {
entry:
  %d = alloca double, align 8
  %e = alloca double, align 8
  %bool.text = alloca i8*, align 8
  %bool.text18 = alloca i8*, align 8
  %bool.text19 = alloca i8*, align 8
  %c = alloca double, align 8
  %z = alloca double, align 8
  store double 0x400921FB54442D18, double* %d, align 8
  store double 2.000000e+00, double* %e, align 8
  %"$d" = load double, double* %d, align 8
  %"$e" = load double, double* %e, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), double %"$d", double %"$e")
  %"$d3" = load double, double* %d, align 8
  %"$e4" = load double, double* %e, align 8
  %ADD = fadd double %"$d3", %"$e4"
  %"$d5" = load double, double* %d, align 8
  %"$e6" = load double, double* %e, align 8
  %SUB = fsub double %"$d5", %"$e6"
  %"$d7" = load double, double* %d, align 8
  %"$e8" = load double, double* %e, align 8
  %MUL = fmul double %"$d7", %"$e8"
  %"$d9" = load double, double* %d, align 8
  %"$e10" = load double, double* %e, align 8
  %DIV = fdiv double %"$d9", %"$e10"
  %call.dprintf11 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.1, i32 0, i32 0), double %ADD, double %SUB, double %MUL, double %DIV)
  %"$d12" = load double, double* %d, align 8
  %"$e13" = load double, double* %e, align 8
  %GT = fcmp ogt double %"$d12", %"$e13"
  %"$d14" = load double, double* %d, align 8
  %"$d15" = load double, double* %d, align 8
  %EQ = fcmp oeq double %"$d14", %"$d15"
  %"$d16" = load double, double* %d, align 8
  %"$e17" = load double, double* %e, align 8
  %NQ = fcmp une double %"$d16", %"$e17"
  store i8* null, i8** %bool.text, align 8
  br label %if0

if0:                                              ; preds = %entry
  br i1 %GT, label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else1:                                            ; preds = %if0
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  store i8* null, i8** %bool.text18, align 8
  br label %if2

if2:                                              ; preds = %endif
  br i1 %EQ, label %if2.then, label %else3

if2.then:                                         ; preds = %if2
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.4, i32 0, i32 0), i8** %bool.text18, align 8
  br label %endif1

else3:                                            ; preds = %if2
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.5, i32 0, i32 0), i8** %bool.text18, align 8
  br label %endif1

endif1:                                           ; preds = %else3, %if2.then
  store i8* null, i8** %bool.text19, align 8
  br label %if4

if4:                                              ; preds = %endif1
  br i1 %NQ, label %if4.then, label %else5

if4.then:                                         ; preds = %if4
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.6, i32 0, i32 0), i8** %bool.text19, align 8
  br label %endif2

else5:                                            ; preds = %if4
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.7, i32 0, i32 0), i8** %bool.text19, align 8
  br label %endif2

endif2:                                           ; preds = %else5, %if4.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %"$bool.text20" = load i8*, i8** %bool.text18, align 8
  %"$bool.text21" = load i8*, i8** %bool.text19, align 8
  %call.dprintf22 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.8, i32 0, i32 0), i8* %"$bool.text", i8* %"$bool.text20", i8* %"$bool.text21")
  store double 1.000000e+01, double* %c, align 8
  %"$c" = load double, double* %c, align 8
  %ADD23 = fadd double %"$c", 2.500000e+00
  store double %ADD23, double* %c, align 8
  %"$c24" = load double, double* %c, align 8
  %SUB25 = fsub double %"$c24", 5.000000e-01
  store double %SUB25, double* %c, align 8
  %"$c26" = load double, double* %c, align 8
  %MUL27 = fmul double %"$c26", 2.000000e+00
  store double %MUL27, double* %c, align 8
  %"$c28" = load double, double* %c, align 8
  %DIV29 = fdiv double %"$c28", 4.000000e+00
  store double %DIV29, double* %c, align 8
  %"$c30" = load double, double* %c, align 8
  %call.dprintf31 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.9, i32 0, i32 0), double %"$c30")
  store double 0.000000e+00, double* %z, align 8
  %"$z" = load double, double* %z, align 8
  %call.dprintf32 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.10, i32 0, i32 0), double %"$z")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.3 = private unnamed_addr constant [7 x i8] c"%f %f\0A\00", align 1
@str.4 = private unnamed_addr constant [4 x i8] c"%f\0A\00", align 1

define double @half() {
entry:
  ret double 5.000000e-01
}

define double @scale(double %0) {
entry:
  %x = alloca double, align 8
  store double %0, double* %x, align 8
  %"$x" = load double, double* %x, align 8
  %MUL = fmul double %"$x", 2.000000e+00
  ret double %MUL
}

define i32 @main() {
entry:
  %d = alloca double, align 8
  %f = alloca float, align 4
  %bool.text = alloca i8*, align 8
  store double 0x400921FB54442D18, double* %d, align 8
  store float 0x400921FB60000000, float* %f, align 4
  %"$f" = load float, float* %f, align 4
  %AS = fpext float %"$f" to double
  %"$d" = load double, double* %d, align 8
  %EQ = fcmp oeq double %AS, %"$d"
  store i8* null, i8** %bool.text, align 8
  br label %if0

if0:                                              ; preds = %entry
  br i1 %EQ, label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else1:                                            ; preds = %if0
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i8* %"$bool.text")
  %call.scale = call double @scale(double -2.500000e+00)
  %call.scale1 = call double @scale(double 1.500000e+00)
  %call.dprintf2 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.3, i32 0, i32 0), double %call.scale, double %call.scale1)
  %call.half = call double @half()
  %call.dprintf3 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), double %call.half)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [22 x i8] c"d->f32 %f  f->f64 %f\0A\00", align 1
@str.1 = private unnamed_addr constant [24 x i8] c"d->i32 %d  d->i64 %lld\0A\00", align 1
@str.2 = private unnamed_addr constant [23 x i8] c"d->i16 %d  d->char %c\0A\00", align 1
@str.3 = private unnamed_addr constant [23 x i8] c"i32->d %f  bool->d %f\0A\00", align 1
@str.4 = private unnamed_addr constant [15 x i8] c"round trip %f\0A\00", align 1

define i32 @main() {
entry:
  %d = alloca double, align 8
  %f = alloca float, align 4
  store double 3.750000e+00, double* %d, align 8
  store float 1.500000e+00, float* %f, align 4
  %"$d" = load double, double* %d, align 8
  %AS = fptrunc double %"$d" to float
  %"$f" = load float, float* %f, align 4
  %AS1 = fpext float %"$f" to double
  %AS2 = fpext float %AS to double
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @str, i32 0, i32 0), double %AS2, double %AS1)
  %"$d3" = load double, double* %d, align 8
  %AS4 = fptosi double %"$d3" to i32
  %"$d5" = load double, double* %d, align 8
  %AS6 = fptosi double %"$d5" to i64
  %call.dprintf7 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([24 x i8], [24 x i8]* @str.1, i32 0, i32 0), i32 %AS4, i64 %AS6)
  %"$d8" = load double, double* %d, align 8
  %AS9 = fptosi double %"$d8" to i16
  %"$d10" = load double, double* %d, align 8
  %AS11 = fptosi double %"$d10" to i8
  %AS12 = sext i16 %AS9 to i32
  %AS13 = sext i8 %AS11 to i32
  %call.dprintf14 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.2, i32 0, i32 0), i32 %AS12, i32 %AS13)
  %call.dprintf15 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.3, i32 0, i32 0), double 7.000000e+00, double 1.000000e+00)
  %"$d16" = load double, double* %d, align 8
  %AS17 = fptosi double %"$d16" to i32
  %AS18 = sitofp i32 %AS17 to double
  %call.dprintf19 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.4, i32 0, i32 0), double %AS18)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### stderr

```
error: test.ura:4:12 expected 'i32', found 'f64'
  |
4 |     return 1.5
  |            ^^^
error: aborting due to 1 error
```

### status

```
1
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [11 x i8] c"%lld %lld\0A\00", align 1
@str.1 = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"%lld\0A\00", align 1

define i32 @main() {
entry:
  %i64_max = alloca i64, align 8
  %i64_min = alloca i64, align 8
  %i32_max = alloca i32, align 4
  %i32_min = alloca i32, align 4
  %exact = alloca i64, align 8
  store i64 9223372036854775807, i64* %i64_max, align 8
  store i64 -9223372036854775807, i64* %i64_min, align 8
  store i32 2147483647, i32* %i32_max, align 4
  store i32 -2147483647, i32* %i32_min, align 4
  store i64 9007199254740993, i64* %exact, align 8
  %"$i64_max" = load i64, i64* %i64_max, align 8
  %"$i64_min" = load i64, i64* %i64_min, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str, i32 0, i32 0), i64 %"$i64_max", i64 %"$i64_min")
  %"$i32_max" = load i32, i32* %i32_max, align 4
  %"$i32_min" = load i32, i32* %i32_min, align 4
  %call.dprintf1 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0), i32 %"$i32_max", i32 %"$i32_min")
  %"$exact" = load i64, i64* %exact, align 8
  %call.dprintf2 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0), i64 %"$exact")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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
    return n * 2

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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [6 x i8] c"%lld\0A\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"%lld\0A\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"%lld\0A\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"%lld\0A\00", align 1
@str.4 = private unnamed_addr constant [6 x i8] c"%lld\0A\00", align 1
@str.5 = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1

define i64 @twice(i64 %0) {
entry:
  %n = alloca i64, align 8
  store i64 %0, i64* %n, align 8
  %"$n" = load i64, i64* %n, align 8
  %MUL = mul i64 %"$n", 2
  ret i64 %MUL
}

define i32 @main() {
entry:
  %with_target = alloca i64, align 8
  store i64 9000000000, i64* %with_target, align 8
  %"$with_target" = load i64, i64* %with_target, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i64 %"$with_target")
  %call.dprintf1 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i64 9000000000)
  %call.dprintf2 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0), i64 9000000001)
  %call.dprintf3 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i64 9000000001)
  %call.twice = call i64 @twice(i64 9000000000)
  %call.dprintf4 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.4, i32 0, i32 0), i64 %call.twice)
  %call.dprintf5 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.5, i32 0, i32 0), i32 42, i32 2147483647)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 027 — literal: a literal that does not fit its target type

```ura
// error: 300 does not fit in 'i8' (range -128 to 127); widen the type, or convert explicitly with 'as'

fn main() i32:
    small i8 = 300
    return 0
```

### stderr

```
error: test.ura:4:16 300 doesn't fit in i8
  |
4 |     small i8 = 300
  |                ^^^
help: i8 goes from -128 to 127
error: aborting due to 1 error
```

### status

```
1
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
    output(a as i32, " ", b, " ", c, " ", d, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [13 x i8] c"%d %d %u %u\0A\00", align 1

define i32 @main() {
entry:
  %a = alloca i8, align 1
  %b = alloca i16, align 2
  %c = alloca i8, align 1
  %d = alloca i16, align 2
  store i8 -128, i8* %a, align 1
  store i16 -32768, i16* %b, align 2
  store i8 -1, i8* %c, align 1
  store i16 -1, i16* %d, align 2
  %"$a" = load i8, i8* %a, align 1
  %AS = sext i8 %"$a" to i32
  %"$b" = load i16, i16* %b, align 2
  %AS1 = sext i16 %"$b" to i32
  %"$c" = load i8, i8* %c, align 1
  %AS2 = zext i8 %"$c" to i32
  %"$d" = load i16, i16* %d, align 2
  %AS3 = zext i16 %"$d" to i32
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str, i32 0, i32 0), i32 %AS, i32 %AS1, i32 %AS2, i32 %AS3)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.3 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.4 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 12, i32* %a, align 4
  store i32 10, i32* %b, align 4
  %"$a" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %BAND = and i32 %"$a", %"$b"
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %BAND)
  %"$a1" = load i32, i32* %a, align 4
  %"$b2" = load i32, i32* %b, align 4
  %BOR = or i32 %"$a1", %"$b2"
  %call.dprintf3 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 %BOR)
  %"$a4" = load i32, i32* %a, align 4
  %"$b5" = load i32, i32* %b, align 4
  %BXOR = xor i32 %"$a4", %"$b5"
  %call.dprintf6 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 %BXOR)
  %"$a7" = load i32, i32* %a, align 4
  %LSHIFT = shl i32 %"$a7", 2
  %call.dprintf8 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.3, i32 0, i32 0), i32 %LSHIFT)
  %"$a9" = load i32, i32* %a, align 4
  %RSHIFT = ashr i32 %"$a9", 1
  %call.dprintf10 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i32 %RSHIFT)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.3 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.4 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.5 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.6 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.7 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.8 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %t = alloca i1, align 1
  %bool.text = alloca i8*, align 8
  %bool.text7 = alloca i8*, align 8
  store i32 5, i32* %a, align 4
  store i1 true, i1* %t, align 1
  %"$a" = load i32, i32* %a, align 4
  %NEG = sub i32 0, %"$a"
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %NEG)
  %"$t" = load i1, i1* %t, align 1
  %NOT = xor i1 %"$t", true
  store i8* null, i8** %bool.text, align 8
  br label %if0

if0:                                              ; preds = %entry
  br i1 %NOT, label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else1:                                            ; preds = %if0
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf2 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.3, i32 0, i32 0), i8* %"$bool.text")
  %"$a3" = load i32, i32* %a, align 4
  %BXOR = xor i32 %"$a3", -1
  %call.dprintf4 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i32 %BXOR)
  %"$a5" = load i32, i32* %a, align 4
  %LT = icmp slt i32 %"$a5", 3
  %NOT6 = xor i1 %LT, true
  store i8* null, i8** %bool.text7, align 8
  br label %if2

if2:                                              ; preds = %endif
  br i1 %NOT6, label %if2.then, label %else3

if2.then:                                         ; preds = %if2
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.5, i32 0, i32 0), i8** %bool.text7, align 8
  br label %endif1

else3:                                            ; preds = %if2
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.6, i32 0, i32 0), i8** %bool.text7, align 8
  br label %endif1

endif1:                                           ; preds = %else3, %if2.then
  %"$bool.text8" = load i8*, i8** %bool.text7, align 8
  %call.dprintf9 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.7, i32 0, i32 0), i8* %"$bool.text8")
  %"$a10" = load i32, i32* %a, align 4
  %NEG11 = sub i32 0, %"$a10"
  %ADD = add i32 %NEG11, 10
  %call.dprintf12 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.8, i32 0, i32 0), i32 %ADD)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [23 x i8] c"and %d  or %d  xor %d\0A\00", align 1
@str.1 = private unnamed_addr constant [16 x i8] c"shl %d  shr %d\0A\00", align 1
@str.2 = private unnamed_addr constant [17 x i8] c"unsigned shr %u\0A\00", align 1
@str.3 = private unnamed_addr constant [15 x i8] c"signed shr %d\0A\00", align 1

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  %d = alloca i32, align 4
  %e = alloca i32, align 4
  %u = alloca i32, align 4
  %s = alloca i32, align 4
  store i32 12, i32* %a, align 4
  %"$a" = load i32, i32* %a, align 4
  %BAND = and i32 %"$a", 10
  store i32 %BAND, i32* %a, align 4
  store i32 12, i32* %b, align 4
  %"$b" = load i32, i32* %b, align 4
  %BOR = or i32 %"$b", 3
  store i32 %BOR, i32* %b, align 4
  store i32 12, i32* %c, align 4
  %"$c" = load i32, i32* %c, align 4
  %BXOR = xor i32 %"$c", 10
  store i32 %BXOR, i32* %c, align 4
  %"$a1" = load i32, i32* %a, align 4
  %"$b2" = load i32, i32* %b, align 4
  %"$c3" = load i32, i32* %c, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str, i32 0, i32 0), i32 %"$a1", i32 %"$b2", i32 %"$c3")
  store i32 3, i32* %d, align 4
  %"$d" = load i32, i32* %d, align 4
  %LSHIFT = shl i32 %"$d", 4
  store i32 %LSHIFT, i32* %d, align 4
  store i32 48, i32* %e, align 4
  %"$e" = load i32, i32* %e, align 4
  %RSHIFT = ashr i32 %"$e", 2
  store i32 %RSHIFT, i32* %e, align 4
  %"$d4" = load i32, i32* %d, align 4
  %"$e5" = load i32, i32* %e, align 4
  %call.dprintf6 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.1, i32 0, i32 0), i32 %"$d4", i32 %"$e5")
  store i32 -294967296, i32* %u, align 4
  %"$u" = load i32, i32* %u, align 4
  %RSHIFT7 = lshr i32 %"$u", 1
  store i32 %RSHIFT7, i32* %u, align 4
  %"$u8" = load i32, i32* %u, align 4
  %call.dprintf9 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.2, i32 0, i32 0), i32 %"$u8")
  store i32 -16, i32* %s, align 4
  %"$s" = load i32, i32* %s, align 4
  %RSHIFT10 = ashr i32 %"$s", 2
  store i32 %RSHIFT10, i32* %s, align 4
  %"$s11" = load i32, i32* %s, align 4
  %call.dprintf12 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.3, i32 0, i32 0), i32 %"$s11")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 032 — operator: bitwise operator on f32 operands is rejected

```ura
// error: Bitwise and shift operators require integer operands

fn main() i32:
    x f32 = 1.5 & 2.0
    return 0
```

### stderr

```
error: test.ura:4:17 can't apply '&' to 'f64'
  |
4 |     x f32 = 1.5 & 2.0
  |                 ^
help: bitwise operators work on integers
error: aborting due to 1 error
```

### status

```
1
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

@str = private unnamed_addr constant [13 x i8] c"%f %f %f %f\0A\00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c"%f %f %f\0A\00", align 1
@str.2 = private unnamed_addr constant [13 x i8] c"%f %d %f %f\0A\00", align 1

declare i32 @printf(i8*, ...)

define float @half() {
entry:
  ret float 5.000000e-01
}

define i32 @main() {
entry:
  %a = alloca float, align 4
  %b = alloca float, align 4
  %c = alloca double, align 8
  %d = alloca double, align 8
  store float 3.000000e+00, float* %a, align 4
  store float 2.000000e+00, float* %b, align 4
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
  %0 = fpext float %ADD to double
  %1 = fpext float %SUB to double
  %2 = fpext float %MUL to double
  %3 = fpext float %DIV to double
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str, i32 0, i32 0), double %0, double %1, double %2, double %3)
  %"$a7" = load float, float* %a, align 4
  %ADD8 = fadd float %"$a7", 1.500000e+00
  store float %ADD8, float* %a, align 4
  store double 4.500000e+00, double* %c, align 8
  %"$c" = load double, double* %c, align 8
  %MUL9 = fmul double %"$c", 2.000000e+00
  store double %MUL9, double* %c, align 8
  %"$c10" = load double, double* %c, align 8
  %NEG = fneg double %"$c10"
  %"$a11" = load float, float* %a, align 4
  %"$c12" = load double, double* %c, align 8
  %4 = fpext float %"$a11" to double
  %call.printf13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), double %4, double %"$c12", double %NEG)
  store double 3.750000e+00, double* %d, align 8
  %"$d" = load double, double* %d, align 8
  %AS = fptosi double %"$d" to i32
  %"$d14" = load double, double* %d, align 8
  %call.printf15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.2, i32 0, i32 0), double %"$d14", i32 %AS, double 7.000000e+00, double 1.000000e+00)
  br label %if0

if0:                                              ; preds = %entry
  %"$a16" = load float, float* %a, align 4
  %"$b17" = load float, float* %b, align 4
  %GT = fcmp ogt float %"$a16", %"$b17"
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

