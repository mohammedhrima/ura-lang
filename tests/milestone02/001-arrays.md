# milestone02

## index

- 001 — array: literal and indexing
- 002 — array: write through an index
- 003 — array: index from a variable
- 004 — array: index from an expression
- 005 — array: literal made of variables
- 006 — array: compound assignment on an element
- 007 — array: loop over an array
- 008 — array: an array passed to a function
- 009 — array: a function writes into the caller's array
- 010 — array: a string is a writable i8[]
- 011 — array: an array of bools
- 012 — proto: a string in a variable
- 013 — proto: a string passed to a function
- 014 — proto: proto variadic function
- 015 — proto: chars type
- 016 — ref: write through a ref
- 017 — ref: read through a ref, then write
- 018 — ref: compound assignment through a ref
- 019 — ref: swap with ref parameters
- 020 — ref: a ref parameter changes the caller
- 021 — ref: p1 = p2 copies the value
- 022 — ref: p1 = &p2 rebinds
- 023 — ref: pass a ref parameter on to another function
- 024 — ref: a ref to a bool
- 025 — ref: a ref in a loop
- 026 — ref: take the address of a variable
- 027 — crazy: fibonacci with a memo array
- 028 — crazy: upper-case a string in place
- 029 — crazy: not and ! on bools, in a loop
- 030 — crazy: count trues with as, then a cast that wraps
- 031 — crazy: caesar cipher: char arithmetic with as
- 032 — crazy: print the alphabet with a char counter
- 033 — crazy: isNullable on array elements and chars
- 034 — crazy: transpose a 3x3 matrix stored flat
- 035 — crazy: a ref to an array element
- 036 — crazy: an array literal made of calls
- 037 — crazy: elif chain on one line each, with not / and / or
- 038 — crazy: reverse a string, palindromes, a while on one line
- 039 — crazy: sieve of eratosthenes on a bool array
- 040 — crazy: atoi: parse numbers from strings
- 041 — crazy: itoa: write a number's digits into a buffer
- 042 — crazy: print a number in hex with as
- 043 — crazy: a string builder on a global buffer
- 044 — crazy: filter with as bool, not and isNullable

---

## 001 — array: literal and indexing

```ura
// expect: 10
fn main() i32:
    a i32[] = [4, 5, 6]
    return a[0] + a[2]
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %arr = alloca [3 x i32], align 4
  store [3 x i32] [i32 4, i32 5, i32 6], [3 x i32]* %arr, align 4
  %0 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %a, align 8
  %"$a" = load i32*, i32** %a, align 8
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 0
  %"$a1" = load i32*, i32** %a, align 8
  %"a[]2" = getelementptr inbounds i32, i32* %"$a1", i32 2
  %"$a[]" = load i32, i32* %"a[]", align 4
  %"$a[]3" = load i32, i32* %"a[]2", align 4
  %ADD = add i32 %"$a[]", %"$a[]3"
  ret i32 %ADD
}
```

---

## 002 — array: write through an index

```ura
// expect: 9
fn main() i32:
    a i32[] = [1, 2, 3]
    a[1] = 9
    return a[1]
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %arr = alloca [3 x i32], align 4
  store [3 x i32] [i32 1, i32 2, i32 3], [3 x i32]* %arr, align 4
  %0 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %a, align 8
  %"$a" = load i32*, i32** %a, align 8
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 1
  store i32 9, i32* %"a[]", align 4
  %"$a1" = load i32*, i32** %a, align 8
  %"a[]2" = getelementptr inbounds i32, i32* %"$a1", i32 1
  %"$a[]" = load i32, i32* %"a[]2", align 4
  ret i32 %"$a[]"
}
```

---

## 003 — array: index from a variable

```ura
// expect: 9
fn main() i32:
    a i32[] = [7, 8, 9]
    i i32 = 2
    return a[i]
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %arr = alloca [3 x i32], align 4
  %i = alloca i32, align 4
  store [3 x i32] [i32 7, i32 8, i32 9], [3 x i32]* %arr, align 4
  %0 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %a, align 8
  store i32 2, i32* %i, align 4
  %"$a" = load i32*, i32** %a, align 8
  %"$i" = load i32, i32* %i, align 4
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 %"$i"
  %"$a[]" = load i32, i32* %"a[]", align 4
  ret i32 %"$a[]"
}
```

---

## 004 — array: index from an expression

```ura
// expect: 8
fn main() i32:
    a i32[] = [7, 8, 9]
    i i32 = 2
    return a[i - 1]
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %arr = alloca [3 x i32], align 4
  %i = alloca i32, align 4
  store [3 x i32] [i32 7, i32 8, i32 9], [3 x i32]* %arr, align 4
  %0 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %a, align 8
  store i32 2, i32* %i, align 4
  %"$i" = load i32, i32* %i, align 4
  %SUB = sub i32 %"$i", 1
  %"$a" = load i32*, i32** %a, align 8
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 %SUB
  %"$a[]" = load i32, i32* %"a[]", align 4
  ret i32 %"$a[]"
}
```

---

## 005 — array: literal made of variables

```ura
// expect: 16
fn main() i32:
    x i32 = 10
    y i32 = 4
    a i32[] = [x, 2, y]
    return a[0] + a[1] + a[2]
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  %a = alloca i32*, align 8
  %arr = alloca [3 x i32], align 4
  store i32 10, i32* %x, align 4
  store i32 4, i32* %y, align 4
  %"$x" = load i32, i32* %x, align 4
  %"$y" = load i32, i32* %y, align 4
  %0 = insertvalue [3 x i32] undef, i32 %"$x", 0
  %1 = insertvalue [3 x i32] %0, i32 2, 1
  %2 = insertvalue [3 x i32] %1, i32 %"$y", 2
  store [3 x i32] %2, [3 x i32]* %arr, align 4
  %3 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  store i32* %3, i32** %a, align 8
  %"$a" = load i32*, i32** %a, align 8
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 0
  %"$a1" = load i32*, i32** %a, align 8
  %"a[]2" = getelementptr inbounds i32, i32* %"$a1", i32 1
  %"$a[]" = load i32, i32* %"a[]", align 4
  %"$a[]3" = load i32, i32* %"a[]2", align 4
  %ADD = add i32 %"$a[]", %"$a[]3"
  %"$a4" = load i32*, i32** %a, align 8
  %"a[]5" = getelementptr inbounds i32, i32* %"$a4", i32 2
  %"$a[]6" = load i32, i32* %"a[]5", align 4
  %ADD7 = add i32 %ADD, %"$a[]6"
  ret i32 %ADD7
}
```

---

## 006 — array: compound assignment on an element

```ura
// expect: 18   (5 + 2 + 9 + 2)
fn main() i32:
    a i32[] = [1, 2, 3]
    a[0] += 4
    a[2] *= 3
    return a[0] + a[1] + a[2] + 2
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %arr = alloca [3 x i32], align 4
  store [3 x i32] [i32 1, i32 2, i32 3], [3 x i32]* %arr, align 4
  %0 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %a, align 8
  %"$a" = load i32*, i32** %a, align 8
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 0
  %"$a[]" = load i32, i32* %"a[]", align 4
  %ADD = add i32 %"$a[]", 4
  store i32 %ADD, i32* %"a[]", align 4
  %"$a1" = load i32*, i32** %a, align 8
  %"a[]2" = getelementptr inbounds i32, i32* %"$a1", i32 2
  %"$a[]3" = load i32, i32* %"a[]2", align 4
  %MUL = mul i32 %"$a[]3", 3
  store i32 %MUL, i32* %"a[]2", align 4
  %"$a4" = load i32*, i32** %a, align 8
  %"a[]5" = getelementptr inbounds i32, i32* %"$a4", i32 0
  %"$a6" = load i32*, i32** %a, align 8
  %"a[]7" = getelementptr inbounds i32, i32* %"$a6", i32 1
  %"$a[]8" = load i32, i32* %"a[]5", align 4
  %"$a[]9" = load i32, i32* %"a[]7", align 4
  %ADD10 = add i32 %"$a[]8", %"$a[]9"
  %"$a11" = load i32*, i32** %a, align 8
  %"a[]12" = getelementptr inbounds i32, i32* %"$a11", i32 2
  %"$a[]13" = load i32, i32* %"a[]12", align 4
  %ADD14 = add i32 %ADD10, %"$a[]13"
  %ADD15 = add i32 %ADD14, 2
  ret i32 %ADD15
}
```

---

## 007 — array: loop over an array

```ura
// stdout: 3 1 4 1 5
// expect: 14
proto printf(fmt i8[], ...) i32

fn main() i32:
    a i32[] = [3, 1, 4, 1, 5]
    sum i32 = 0
    i i32 = 0
    while i < 5:
        printf("%d", a[i])
        if i < 4:
            printf(" ")
        sum += a[i]
        i += 1
    printf("\n")
    return sum
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %arr = alloca [5 x i32], align 4
  %sum = alloca i32, align 4
  %i = alloca i32, align 4
  store [5 x i32] [i32 3, i32 1, i32 4, i32 1, i32 5], [5 x i32]* %arr, align 4
  %0 = getelementptr inbounds [5 x i32], [5 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %a, align 8
  store i32 0, i32* %sum, align 4
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %endif, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 5
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$a" = load i32*, i32** %a, align 8
  %"$i1" = load i32, i32* %i, align 4
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 %"$i1"
  %"$a[]" = load i32, i32* %"a[]", align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i32 %"$a[]")
  br label %if1

if1:                                              ; preds = %while0.then
  %"$i2" = load i32, i32* %i, align 4
  %LT3 = icmp slt i32 %"$i2", 4
  br i1 %LT3, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  %call.printf4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  br label %endif

endif:                                            ; preds = %if1.then, %if1
  %"$a5" = load i32*, i32** %a, align 8
  %"$i6" = load i32, i32* %i, align 4
  %"a[]7" = getelementptr inbounds i32, i32* %"$a5", i32 %"$i6"
  %"$a[]8" = load i32, i32* %"a[]7", align 4
  %"$sum" = load i32, i32* %sum, align 4
  %ADD = add i32 %"$sum", %"$a[]8"
  store i32 %ADD, i32* %sum, align 4
  %"$i9" = load i32, i32* %i, align 4
  %ADD10 = add i32 %"$i9", 1
  store i32 %ADD10, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %call.printf11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %"$sum12" = load i32, i32* %sum, align 4
  ret i32 %"$sum12"
}
```

---

## 008 — array: an array passed to a function

```ura
// expect: 6
fn sum(a i32[], n i32) i32:
    total i32 = 0
    i i32 = 0
    while i < n:
        total += a[i]
        i += 1
    return total

fn main() i32:
    values i32[] = [1, 2, 3]
    return sum(values, 3)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @sum(i32* %0, i32 %1) {
entry:
  %a = alloca i32*, align 8
  %n = alloca i32, align 4
  %total = alloca i32, align 4
  %i = alloca i32, align 4
  store i32* %0, i32** %a, align 8
  store i32 %1, i32* %n, align 4
  store i32 0, i32* %total, align 4
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %"$n" = load i32, i32* %n, align 4
  %LT = icmp slt i32 %"$i", %"$n"
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$a" = load i32*, i32** %a, align 8
  %"$i1" = load i32, i32* %i, align 4
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 %"$i1"
  %"$a[]" = load i32, i32* %"a[]", align 4
  %"$total" = load i32, i32* %total, align 4
  %ADD = add i32 %"$total", %"$a[]"
  store i32 %ADD, i32* %total, align 4
  %"$i2" = load i32, i32* %i, align 4
  %ADD3 = add i32 %"$i2", 1
  store i32 %ADD3, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$total4" = load i32, i32* %total, align 4
  ret i32 %"$total4"
}

define i32 @main() {
entry:
  %values = alloca i32*, align 8
  %arr = alloca [3 x i32], align 4
  store [3 x i32] [i32 1, i32 2, i32 3], [3 x i32]* %arr, align 4
  %0 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %values, align 8
  %"$values" = load i32*, i32** %values, align 8
  %call.sum = call i32 @sum(i32* %"$values", i32 3)
  ret i32 %call.sum
}
```

---

## 009 — array: a function writes into the caller's array

```ura
// expect: 7
fn set_first(a i32[], v i32):
    a[0] = v

fn main() i32:
    values i32[] = [1, 2, 3]
    set_first(values, 7)
    return values[0]
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define void @set_first(i32* %0, i32 %1) {
entry:
  %a = alloca i32*, align 8
  %v = alloca i32, align 4
  store i32* %0, i32** %a, align 8
  store i32 %1, i32* %v, align 4
  %"$a" = load i32*, i32** %a, align 8
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 0
  %"$v" = load i32, i32* %v, align 4
  store i32 %"$v", i32* %"a[]", align 4
  ret void
}

define i32 @main() {
entry:
  %values = alloca i32*, align 8
  %arr = alloca [3 x i32], align 4
  store [3 x i32] [i32 1, i32 2, i32 3], [3 x i32]* %arr, align 4
  %0 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %values, align 8
  %"$values" = load i32*, i32** %values, align 8
  call void @set_first(i32* %"$values", i32 7)
  %"$values1" = load i32*, i32** %values, align 8
  %"values[]" = getelementptr inbounds i32, i32* %"$values1", i32 0
  %"$values[]" = load i32, i32* %"values[]", align 4
  ret i32 %"$values[]"
}
```

---

## 010 — array: a string is a writable i8[]

```ura
// stdout: xbc
// expect: 1
proto printf(fmt i8[], ...) i32

fn main() i32:
    s i8[] = "abc"
    s[0] = 'x'
    printf("%s\n", s)
    if s[1] == 'b':
        return 1
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %s = alloca i8*, align 8
  %arr = alloca [4 x i8], align 1
  store [4 x i8] c"abc\00", [4 x i8]* %arr, align 1
  %0 = getelementptr inbounds [4 x i8], [4 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %s, align 8
  %"$s" = load i8*, i8** %s, align 8
  %"s[]" = getelementptr inbounds i8, i8* %"$s", i32 0
  store i8 120, i8* %"s[]", align 1
  %"$s1" = load i8*, i8** %s, align 8
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i8* %"$s1")
  br label %if0

if0:                                              ; preds = %entry
  %"$s2" = load i8*, i8** %s, align 8
  %"s[]3" = getelementptr inbounds i8, i8* %"$s2", i32 1
  %"$s[]" = load i8, i8* %"s[]3", align 1
  %EQ = icmp eq i8 %"$s[]", 98
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  ret i32 0
}
```

---

## 011 — array: an array of bools

```ura
// expect: 2
fn main() i32:
    flags bool[] = [True, False, True]
    n i32 = 0
    i i32 = 0
    while i < 3:
        if flags[i]:
            n += 1
        i += 1
    return n
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %flags = alloca i1*, align 8
  %arr = alloca [3 x i1], align 1
  %n = alloca i32, align 4
  %i = alloca i32, align 4
  store [3 x i1] [i1 true, i1 false, i1 true], [3 x i1]* %arr, align 1
  %0 = getelementptr inbounds [3 x i1], [3 x i1]* %arr, i32 0, i32 0
  store i1* %0, i1** %flags, align 8
  store i32 0, i32* %n, align 4
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %endif, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  br label %if1

if1:                                              ; preds = %while0.then
  %"$flags" = load i1*, i1** %flags, align 8
  %"$i1" = load i32, i32* %i, align 4
  %"flags[]" = getelementptr inbounds i1, i1* %"$flags", i32 %"$i1"
  %"$flags[]" = load i1, i1* %"flags[]", align 1
  br i1 %"$flags[]", label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 1
  store i32 %ADD, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %if1.then, %if1
  %"$i2" = load i32, i32* %i, align 4
  %ADD3 = add i32 %"$i2", 1
  store i32 %ADD3, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$n4" = load i32, i32* %n, align 4
  ret i32 %"$n4"
}
```

---

## 012 — proto: a string in a variable

```ura
// stdout: <abcdef>
// expect: 0
proto printf(fmt i8[], ...) i32

fn main() i32:
    str i8[] = "abcdef"
    printf("<%s>\n", str)
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private unnamed_addr constant [6 x i8] c"<%s>\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %str = alloca i8*, align 8
  %arr = alloca [7 x i8], align 1
  store [7 x i8] c"abcdef\00", [7 x i8]* %arr, align 1
  %0 = getelementptr inbounds [7 x i8], [7 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %str, align 8
  %"$str" = load i8*, i8** %str, align 8
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i8* %"$str")
  ret i32 0
}
```

---

## 013 — proto: a string passed to a function

```ura
// stdout: hi ura
// expect: 0
proto printf(fmt i8[], ...) i32

fn greet(name i8[]):
    printf("hi %s\n", name)

fn main() i32:
    greet("ura")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private unnamed_addr constant [7 x i8] c"hi %s\0A\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"ura\00", align 1

declare i32 @printf(i8*, ...)

define void @greet(i8* %0) {
entry:
  %name = alloca i8*, align 8
  store i8* %0, i8** %name, align 8
  %"$name" = load i8*, i8** %name, align 8
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i8* %"$name")
  ret void
}

define i32 @main() {
entry:
  call void @greet(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}
```

---

## 014 — proto: proto variadic function

```ura
proto printf(fmt i8[], ...) i32

fn main() i32:
    str i8[] = "abcdef"
    printf("<%s>", str)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private unnamed_addr constant [5 x i8] c"<%s>\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %str = alloca i8*, align 8
  %arr = alloca [7 x i8], align 1
  store [7 x i8] c"abcdef\00", [7 x i8]* %arr, align 1
  %0 = getelementptr inbounds [7 x i8], [7 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %str, align 8
  %"$str" = load i8*, i8** %str, align 8
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8* %"$str")
  ret i32 0
}
```

---

## 015 — proto: chars type

```ura
fn main() i32:
    str i8[] = "abcdef"
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %str = alloca i8*, align 8
  %arr = alloca [7 x i8], align 1
  store [7 x i8] c"abcdef\00", [7 x i8]* %arr, align 1
  %0 = getelementptr inbounds [7 x i8], [7 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %str, align 8
  ret i32 0
}
```

---

## 016 — ref: write through a ref

```ura
// stdout: a = 12
// expect: 12
proto printf(fmt i8[], ...) i32

fn main() i32:
    a i32 = 10
    p &i32 = &a
    p = 12
    printf("a = %d\n", a)
    return a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private unnamed_addr constant [8 x i8] c"a = %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %p = alloca i32*, align 8
  store i32 10, i32* %a, align 4
  store i32* %a, i32** %p, align 8
  %"*p" = load i32*, i32** %p, align 8
  store i32 12, i32* %"*p", align 4
  %"$a" = load i32, i32* %a, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str, i32 0, i32 0), i32 %"$a")
  %"$a1" = load i32, i32* %a, align 4
  ret i32 %"$a1"
}
```

---

## 017 — ref: read through a ref, then write

```ura
// stdout: p = 10, a = 15
// expect: 15
proto printf(fmt i8[], ...) i32

fn main() i32:
    a i32 = 10
    p &i32 = &a
    printf("p = %d, ", p)
    p = p + 5
    printf("a = %d\n", a)
    return a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private unnamed_addr constant [9 x i8] c"p = %d, \00", align 1
@str.1 = private unnamed_addr constant [8 x i8] c"a = %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %p = alloca i32*, align 8
  store i32 10, i32* %a, align 4
  store i32* %a, i32** %p, align 8
  %"*p" = load i32*, i32** %p, align 8
  %"$*p" = load i32, i32* %"*p", align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %"$*p")
  %"*p1" = load i32*, i32** %p, align 8
  %"*p2" = load i32*, i32** %p, align 8
  %"$*p3" = load i32, i32* %"*p2", align 4
  %ADD = add i32 %"$*p3", 5
  store i32 %ADD, i32* %"*p1", align 4
  %"$a" = load i32, i32* %a, align 4
  %call.printf4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.1, i32 0, i32 0), i32 %"$a")
  %"$a5" = load i32, i32* %a, align 4
  ret i32 %"$a5"
}
```

---

## 018 — ref: compound assignment through a ref

```ura
// expect: 24
fn main() i32:
    a i32 = 10
    p &i32 = &a
    p += 2
    p *= 2
    return a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %p = alloca i32*, align 8
  store i32 10, i32* %a, align 4
  store i32* %a, i32** %p, align 8
  %"*p" = load i32*, i32** %p, align 8
  %"$*p" = load i32, i32* %"*p", align 4
  %ADD = add i32 %"$*p", 2
  store i32 %ADD, i32* %"*p", align 4
  %"*p1" = load i32*, i32** %p, align 8
  %"$*p2" = load i32, i32* %"*p1", align 4
  %MUL = mul i32 %"$*p2", 2
  store i32 %MUL, i32* %"*p1", align 4
  %"$a" = load i32, i32* %a, align 4
  ret i32 %"$a"
}
```

---

## 019 — ref: swap with ref parameters

```ura
// stdout: a = 2, b = 1
// expect: 21
proto printf(fmt i8[], ...) i32

fn swap(x &i32, y &i32):
    t i32 = x
    x = y
    y = t

fn main() i32:
    a i32 = 1
    b i32 = 2
    swap(&a, &b)
    printf("a = %d, b = %d\n", a, b)
    return a * 10 + b
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private unnamed_addr constant [16 x i8] c"a = %d, b = %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @swap(i32* %0, i32* %1) {
entry:
  %x = alloca i32*, align 8
  %y = alloca i32*, align 8
  %t = alloca i32, align 4
  store i32* %0, i32** %x, align 8
  store i32* %1, i32** %y, align 8
  %"*x" = load i32*, i32** %x, align 8
  %"$*x" = load i32, i32* %"*x", align 4
  store i32 %"$*x", i32* %t, align 4
  %"*x1" = load i32*, i32** %x, align 8
  %"*y" = load i32*, i32** %y, align 8
  %"$*y" = load i32, i32* %"*y", align 4
  store i32 %"$*y", i32* %"*x1", align 4
  %"*y2" = load i32*, i32** %y, align 8
  %"$t" = load i32, i32* %t, align 4
  store i32 %"$t", i32* %"*y2", align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 1, i32* %a, align 4
  store i32 2, i32* %b, align 4
  call void @swap(i32* %a, i32* %b)
  %"$a" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str, i32 0, i32 0), i32 %"$a", i32 %"$b")
  %"$a1" = load i32, i32* %a, align 4
  %MUL = mul i32 %"$a1", 10
  %"$b2" = load i32, i32* %b, align 4
  %ADD = add i32 %MUL, %"$b2"
  ret i32 %ADD
}
```

---

## 020 — ref: a ref parameter changes the caller

```ura
// expect: 3
fn inc(c &i32):
    c += 1

fn main() i32:
    n i32 = 0
    inc(&n)
    inc(&n)
    inc(&n)
    return n
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define void @inc(i32* %0) {
entry:
  %c = alloca i32*, align 8
  store i32* %0, i32** %c, align 8
  %"*c" = load i32*, i32** %c, align 8
  %"$*c" = load i32, i32* %"*c", align 4
  %ADD = add i32 %"$*c", 1
  store i32 %ADD, i32* %"*c", align 4
  ret void
}

define i32 @main() {
entry:
  %n = alloca i32, align 4
  store i32 0, i32* %n, align 4
  call void @inc(i32* %n)
  call void @inc(i32* %n)
  call void @inc(i32* %n)
  %"$n" = load i32, i32* %n, align 4
  ret i32 %"$n"
}
```

---

## 021 — ref: p1 = p2 copies the value

```ura
proto printf(fmt i8[], ...) i32

fn main() i32:
    a i32 = 1
    b i32 = 2
    p1 &i32 = &a
    p2 &i32 = &b
    p1 = p2
    printf("a = %d, b = %d\n", a, b)
    return a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private unnamed_addr constant [16 x i8] c"a = %d, b = %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %p1 = alloca i32*, align 8
  %p2 = alloca i32*, align 8
  store i32 1, i32* %a, align 4
  store i32 2, i32* %b, align 4
  store i32* %a, i32** %p1, align 8
  store i32* %b, i32** %p2, align 8
  %"*p1" = load i32*, i32** %p1, align 8
  %"*p2" = load i32*, i32** %p2, align 8
  %"$*p2" = load i32, i32* %"*p2", align 4
  store i32 %"$*p2", i32* %"*p1", align 4
  %"$a" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str, i32 0, i32 0), i32 %"$a", i32 %"$b")
  %"$a1" = load i32, i32* %a, align 4
  ret i32 %"$a1"
}
```

---

## 022 — ref: p1 = &p2 rebinds

```ura
// stdout: a = 1, b = 9
// expect: 9
proto printf(fmt i8[], ...) i32

fn main() i32:
    a i32 = 1
    b i32 = 2
    p1 &i32 = &a
    p2 &i32 = &b
    p1 = &p2
    p1 = 9
    printf("a = %d, b = %d\n", a, b)
    return b
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private unnamed_addr constant [16 x i8] c"a = %d, b = %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %p1 = alloca i32*, align 8
  %p2 = alloca i32*, align 8
  store i32 1, i32* %a, align 4
  store i32 2, i32* %b, align 4
  store i32* %a, i32** %p1, align 8
  store i32* %b, i32** %p2, align 8
  %"*p2" = load i32*, i32** %p2, align 8
  store i32* %"*p2", i32** %p1, align 8
  %"*p1" = load i32*, i32** %p1, align 8
  store i32 9, i32* %"*p1", align 4
  %"$a" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str, i32 0, i32 0), i32 %"$a", i32 %"$b")
  %"$b1" = load i32, i32* %b, align 4
  ret i32 %"$b1"
}
```

---

## 023 — ref: pass a ref parameter on to another function

```ura
// expect: 2
fn add_one(p &i32):
    p += 1

fn add_two(p &i32):
    add_one(&p)
    add_one(&p)

fn main() i32:
    a i32 = 0
    add_two(&a)
    return a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define void @add_one(i32* %0) {
entry:
  %p = alloca i32*, align 8
  store i32* %0, i32** %p, align 8
  %"*p" = load i32*, i32** %p, align 8
  %"$*p" = load i32, i32* %"*p", align 4
  %ADD = add i32 %"$*p", 1
  store i32 %ADD, i32* %"*p", align 4
  ret void
}

define void @add_two(i32* %0) {
entry:
  %p = alloca i32*, align 8
  store i32* %0, i32** %p, align 8
  %"*p" = load i32*, i32** %p, align 8
  call void @add_one(i32* %"*p")
  %"*p1" = load i32*, i32** %p, align 8
  call void @add_one(i32* %"*p1")
  ret void
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 0, i32* %a, align 4
  call void @add_two(i32* %a)
  %"$a" = load i32, i32* %a, align 4
  ret i32 %"$a"
}
```

---

## 024 — ref: a ref to a bool

```ura
// expect: 1
fn main() i32:
    b bool = False
    p &bool = &b
    p = True
    if b:
        return 1
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %b = alloca i1, align 1
  %p = alloca i1*, align 8
  store i1 false, i1* %b, align 1
  store i1* %b, i1** %p, align 8
  %"*p" = load i1*, i1** %p, align 8
  store i1 true, i1* %"*p", align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$b" = load i1, i1* %b, align 1
  br i1 %"$b", label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  ret i32 0
}
```

---

## 025 — ref: a ref in a loop

```ura
// stdout: 1 2 3 4 5
// expect: 15
proto printf(fmt i8[], ...) i32

fn main() i32:
    total i32 = 0
    p &i32 = &total
    i i32 = 1
    while i <= 5:
        printf("%d ", i)
        p += i
        i += 1
    printf("\n")
    return total
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %total = alloca i32, align 4
  %p = alloca i32*, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %total, align 4
  store i32* %total, i32** %p, align 8
  store i32 1, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LE = icmp sle i32 %"$i", 5
  br i1 %LE, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$i1" = load i32, i32* %i, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$i1")
  %"*p" = load i32*, i32** %p, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"$*p" = load i32, i32* %"*p", align 4
  %ADD = add i32 %"$*p", %"$i2"
  store i32 %ADD, i32* %"*p", align 4
  %"$i3" = load i32, i32* %i, align 4
  %ADD4 = add i32 %"$i3", 1
  store i32 %ADD4, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %call.printf5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %"$total" = load i32, i32* %total, align 4
  ret i32 %"$total"
}
```

---

## 026 — ref: take the address of a variable

```ura
fn main() i32:
    a i32 = 10
    p &i32 = &a
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %p = alloca i32*, align 8
  store i32 10, i32* %a, align 4
  store i32* %a, i32** %p, align 8
  ret i32 0
}
```

---

## 027 — crazy: fibonacci with a memo array

```ura
// expect: 109   (fib(20) = 6765, 6765 % 256)
fn fib(n i32, memo i32[]) i32:
    if n < 2: return n
    if memo[n] != 0: return memo[n]
    memo[n] = fib(n - 1, memo) + fib(n - 2, memo)
    return memo[n]

fn main() i32:
    memo i32[] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    return fib(20, memo) % 256
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @fib(i32 %0, i32* %1) {
entry:
  %n = alloca i32, align 4
  %memo = alloca i32*, align 8
  store i32 %0, i32* %n, align 4
  store i32* %1, i32** %memo, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %LT = icmp slt i32 %"$n", 2
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$n2" = load i32, i32* %n, align 4
  ret i32 %"$n2"

endif:                                            ; preds = %if0
  br label %if1

if1:                                              ; preds = %endif
  %"$memo" = load i32*, i32** %memo, align 8
  %"$n3" = load i32, i32* %n, align 4
  %"memo[]" = getelementptr inbounds i32, i32* %"$memo", i32 %"$n3"
  %"$memo[]" = load i32, i32* %"memo[]", align 4
  %NQ = icmp ne i32 %"$memo[]", 0
  br i1 %NQ, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  %"$memo4" = load i32*, i32** %memo, align 8
  %"$n5" = load i32, i32* %n, align 4
  %"memo[]6" = getelementptr inbounds i32, i32* %"$memo4", i32 %"$n5"
  %"$memo[]7" = load i32, i32* %"memo[]6", align 4
  ret i32 %"$memo[]7"

endif1:                                           ; preds = %if1
  %"$memo8" = load i32*, i32** %memo, align 8
  %"$n9" = load i32, i32* %n, align 4
  %"memo[]10" = getelementptr inbounds i32, i32* %"$memo8", i32 %"$n9"
  %"$n11" = load i32, i32* %n, align 4
  %SUB = sub i32 %"$n11", 1
  %"$memo12" = load i32*, i32** %memo, align 8
  %call.fib = call i32 @fib(i32 %SUB, i32* %"$memo12")
  %"$n13" = load i32, i32* %n, align 4
  %SUB14 = sub i32 %"$n13", 2
  %"$memo15" = load i32*, i32** %memo, align 8
  %call.fib16 = call i32 @fib(i32 %SUB14, i32* %"$memo15")
  %ADD = add i32 %call.fib, %call.fib16
  store i32 %ADD, i32* %"memo[]10", align 4
  %"$memo17" = load i32*, i32** %memo, align 8
  %"$n18" = load i32, i32* %n, align 4
  %"memo[]19" = getelementptr inbounds i32, i32* %"$memo17", i32 %"$n18"
  %"$memo[]20" = load i32, i32* %"memo[]19", align 4
  ret i32 %"$memo[]20"
}

define i32 @main() {
entry:
  %memo = alloca i32*, align 8
  %arr = alloca [21 x i32], align 4
  store [21 x i32] zeroinitializer, [21 x i32]* %arr, align 4
  %0 = getelementptr inbounds [21 x i32], [21 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %memo, align 8
  %"$memo" = load i32*, i32** %memo, align 8
  %call.fib = call i32 @fib(i32 20, i32* %"$memo")
  %MOD = srem i32 %call.fib, 256
  ret i32 %MOD
}
```

---

## 028 — crazy: upper-case a string in place

```ura
// stdout: HELLO, URA
// expect: 10
proto printf(fmt i8[], ...) i32

fn length(s i8[]) i32:
    n i32 = 0
    while s[n] != '\0':
        n += 1
    return n

fn upper(s i8[]):
    i i32 = 0
    while s[i] != '\0':
        if s[i] >= 'a' and s[i] <= 'z':
            s[i] -= 32 as char
        i += 1

fn main() i32:
    s i8[] = "hello, ura"
    upper(s)
    printf("%s\n", s)
    return length(s)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @length(i8* %0) {
entry:
  %s = alloca i8*, align 8
  %n = alloca i32, align 4
  store i8* %0, i8** %s, align 8
  store i32 0, i32* %n, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$s" = load i8*, i8** %s, align 8
  %"$n" = load i32, i32* %n, align 4
  %"s[]" = getelementptr inbounds i8, i8* %"$s", i32 %"$n"
  %"$s[]" = load i8, i8* %"s[]", align 1
  %NQ = icmp ne i8 %"$s[]", 0
  br i1 %NQ, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$n1" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n1", 1
  store i32 %ADD, i32* %n, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$n2" = load i32, i32* %n, align 4
  ret i32 %"$n2"
}

define void @upper(i8* %0) {
entry:
  %s = alloca i8*, align 8
  %i = alloca i32, align 4
  store i8* %0, i8** %s, align 8
  store i32 0, i32* %i, align 4
  br label %while1

while1:                                           ; preds = %endif, %entry
  %"$s" = load i8*, i8** %s, align 8
  %"$i" = load i32, i32* %i, align 4
  %"s[]" = getelementptr inbounds i8, i8* %"$s", i32 %"$i"
  %"$s[]" = load i8, i8* %"s[]", align 1
  %NQ = icmp ne i8 %"$s[]", 0
  br i1 %NQ, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  br label %if2

if2:                                              ; preds = %while1.then
  %"$s1" = load i8*, i8** %s, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"s[]3" = getelementptr inbounds i8, i8* %"$s1", i32 %"$i2"
  %"$s[]4" = load i8, i8* %"s[]3", align 1
  %GE = icmp sge i8 %"$s[]4", 97
  %"$s5" = load i8*, i8** %s, align 8
  %"$i6" = load i32, i32* %i, align 4
  %"s[]7" = getelementptr inbounds i8, i8* %"$s5", i32 %"$i6"
  %"$s[]8" = load i8, i8* %"s[]7", align 1
  %LE = icmp sle i8 %"$s[]8", 122
  %AND = and i1 %GE, %LE
  br i1 %AND, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"$s9" = load i8*, i8** %s, align 8
  %"$i10" = load i32, i32* %i, align 4
  %"s[]11" = getelementptr inbounds i8, i8* %"$s9", i32 %"$i10"
  %"$s[]12" = load i8, i8* %"s[]11", align 1
  %SUB = sub i8 %"$s[]12", 32
  store i8 %SUB, i8* %"s[]11", align 1
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  %"$i13" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i13", 1
  store i32 %ADD, i32* %i, align 4
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define i32 @main() {
entry:
  %s = alloca i8*, align 8
  %arr = alloca [11 x i8], align 1
  store [11 x i8] c"hello, ura\00", [11 x i8]* %arr, align 1
  %0 = getelementptr inbounds [11 x i8], [11 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %s, align 8
  %"$s" = load i8*, i8** %s, align 8
  call void @upper(i8* %"$s")
  %"$s1" = load i8*, i8** %s, align 8
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i8* %"$s1")
  %"$s2" = load i8*, i8** %s, align 8
  %call.length = call i32 @length(i8* %"$s2")
  ret i32 %call.length
}
```

---

## 029 — crazy: not and ! on bools, in a loop

```ura
// expect: 12
fn main() i32:
    a bool = True
    b bool = !a
    n i32 = 0
    i i32 = 0
    while i < 5:
        a = not a
        if a: n += 1
        i += 1
    if not b and !(n == 3): n += 10
    return n
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i1, align 1
  %b = alloca i1, align 1
  %n = alloca i32, align 4
  %i = alloca i32, align 4
  store i1 true, i1* %a, align 1
  %"$a" = load i1, i1* %a, align 1
  %NOT = xor i1 %"$a", true
  store i1 %NOT, i1* %b, align 1
  store i32 0, i32* %n, align 4
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %endif, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 5
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$a2" = load i1, i1* %a, align 1
  %NOT3 = xor i1 %"$a2", true
  store i1 %NOT3, i1* %a, align 1
  br label %if1

if1:                                              ; preds = %while0.then
  %"$a4" = load i1, i1* %a, align 1
  br i1 %"$a4", label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 1
  store i32 %ADD, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %if1.then, %if1
  %"$i5" = load i32, i32* %i, align 4
  %ADD6 = add i32 %"$i5", 1
  store i32 %ADD6, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  br label %if2

if2:                                              ; preds = %endloop
  %"$b" = load i1, i1* %b, align 1
  %NOT7 = xor i1 %"$b", true
  %"$n8" = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %"$n8", 3
  %NOT9 = xor i1 %EQ, true
  %AND = and i1 %NOT7, %NOT9
  br i1 %AND, label %if2.then, label %endif1

if2.then:                                         ; preds = %if2
  %"$n10" = load i32, i32* %n, align 4
  %ADD11 = add i32 %"$n10", 10
  store i32 %ADD11, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %if2.then, %if2
  %"$n12" = load i32, i32* %n, align 4
  ret i32 %"$n12"
}
```

---

## 030 — crazy: count trues with as, then a cast that wraps

```ura
// expect: 6   (3 * 10, plus 1000 as i8 = -24)
fn main() i32:
    flags bool[] = [True, False, True, True]
    n i32 = 0
    i i32 = 0
    while i < 4:
        n += flags[i] as i32
        i += 1
    big i32 = 1000
    return n * 10 + (big as i8) as i32
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %flags = alloca i1*, align 8
  %arr = alloca [4 x i1], align 1
  %n = alloca i32, align 4
  %i = alloca i32, align 4
  %big = alloca i32, align 4
  store [4 x i1] [i1 true, i1 false, i1 true, i1 true], [4 x i1]* %arr, align 1
  %0 = getelementptr inbounds [4 x i1], [4 x i1]* %arr, i32 0, i32 0
  store i1* %0, i1** %flags, align 8
  store i32 0, i32* %n, align 4
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 4
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$flags" = load i1*, i1** %flags, align 8
  %"$i1" = load i32, i32* %i, align 4
  %"flags[]" = getelementptr inbounds i1, i1* %"$flags", i32 %"$i1"
  %"$flags[]" = load i1, i1* %"flags[]", align 1
  %AS = zext i1 %"$flags[]" to i32
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", %AS
  store i32 %ADD, i32* %n, align 4
  %"$i2" = load i32, i32* %i, align 4
  %ADD3 = add i32 %"$i2", 1
  store i32 %ADD3, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  store i32 1000, i32* %big, align 4
  %"$n4" = load i32, i32* %n, align 4
  %MUL = mul i32 %"$n4", 10
  %"$big" = load i32, i32* %big, align 4
  %AS5 = trunc i32 %"$big" to i8
  %AS6 = sext i8 %AS5 to i32
  %ADD7 = add i32 %MUL, %AS6
  ret i32 %ADD7
}
```

---

## 031 — crazy: caesar cipher: char arithmetic with as

```ura
// stdout: khoor, zruog | hello, world
// expect: 104
proto printf(fmt i8[], ...) i32

fn shift(c char, k i32) char:
    if c < 'a' or c > 'z': return c
    n i32 = (c as i32) - ('a' as i32)
    n = (n + k) % 26
    return (n + ('a' as i32)) as char

fn encode(s i8[], k i32):
    i i32 = 0
    while s[i] != '\0':
        s[i] = shift(s[i], k)
        i += 1

fn main() i32:
    s i8[] = "hello, world"
    encode(s, 3)
    printf("%s | ", s)
    encode(s, 23)
    printf("%s\n", s)
    return s[0] as i32
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private unnamed_addr constant [6 x i8] c"%s | \00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

declare i32 @printf(i8*, ...)

define i8 @shift(i8 %0, i32 %1) {
entry:
  %c = alloca i8, align 1
  %k = alloca i32, align 4
  %n = alloca i32, align 4
  store i8 %0, i8* %c, align 1
  store i32 %1, i32* %k, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$c" = load i8, i8* %c, align 1
  %LT = icmp slt i8 %"$c", 97
  %"$c1" = load i8, i8* %c, align 1
  %GT = icmp sgt i8 %"$c1", 122
  %OR = or i1 %LT, %GT
  br i1 %OR, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$c2" = load i8, i8* %c, align 1
  ret i8 %"$c2"

endif:                                            ; preds = %if0
  %"$c3" = load i8, i8* %c, align 1
  %AS = sext i8 %"$c3" to i32
  %SUB = sub i32 %AS, 97
  store i32 %SUB, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %"$k" = load i32, i32* %k, align 4
  %ADD = add i32 %"$n", %"$k"
  %MOD = srem i32 %ADD, 26
  store i32 %MOD, i32* %n, align 4
  %"$n4" = load i32, i32* %n, align 4
  %ADD5 = add i32 %"$n4", 97
  %AS6 = trunc i32 %ADD5 to i8
  ret i8 %AS6
}

define void @encode(i8* %0, i32 %1) {
entry:
  %s = alloca i8*, align 8
  %k = alloca i32, align 4
  %i = alloca i32, align 4
  store i8* %0, i8** %s, align 8
  store i32 %1, i32* %k, align 4
  store i32 0, i32* %i, align 4
  br label %while1

while1:                                           ; preds = %while1.then, %entry
  %"$s" = load i8*, i8** %s, align 8
  %"$i" = load i32, i32* %i, align 4
  %"s[]" = getelementptr inbounds i8, i8* %"$s", i32 %"$i"
  %"$s[]" = load i8, i8* %"s[]", align 1
  %NQ = icmp ne i8 %"$s[]", 0
  br i1 %NQ, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"$s1" = load i8*, i8** %s, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"s[]3" = getelementptr inbounds i8, i8* %"$s1", i32 %"$i2"
  %"$s4" = load i8*, i8** %s, align 8
  %"$i5" = load i32, i32* %i, align 4
  %"s[]6" = getelementptr inbounds i8, i8* %"$s4", i32 %"$i5"
  %"$s[]7" = load i8, i8* %"s[]6", align 1
  %"$k" = load i32, i32* %k, align 4
  %call.shift = call i8 @shift(i8 %"$s[]7", i32 %"$k")
  store i8 %call.shift, i8* %"s[]3", align 1
  %"$i8" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i8", 1
  store i32 %ADD, i32* %i, align 4
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define i32 @main() {
entry:
  %s = alloca i8*, align 8
  %arr = alloca [13 x i8], align 1
  store [13 x i8] c"hello, world\00", [13 x i8]* %arr, align 1
  %0 = getelementptr inbounds [13 x i8], [13 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %s, align 8
  %"$s" = load i8*, i8** %s, align 8
  call void @encode(i8* %"$s", i32 3)
  %"$s1" = load i8*, i8** %s, align 8
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i8* %"$s1")
  %"$s2" = load i8*, i8** %s, align 8
  call void @encode(i8* %"$s2", i32 23)
  %"$s3" = load i8*, i8** %s, align 8
  %call.printf4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i8* %"$s3")
  %"$s5" = load i8*, i8** %s, align 8
  %"s[]" = getelementptr inbounds i8, i8* %"$s5", i32 0
  %"$s[]" = load i8, i8* %"s[]", align 1
  %AS = sext i8 %"$s[]" to i32
  ret i32 %AS
}
```

---

## 032 — crazy: print the alphabet with a char counter

```ura
// stdout: abcde
// expect: 102
proto printf(fmt i8[], ...) i32

fn main() i32:
    c char = 'a'
    while c <= 'e':
        printf("%c", c)
        c = ((c as i32) + 1) as char
    printf("\n")
    return c as i32
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private unnamed_addr constant [3 x i8] c"%c\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %c = alloca i8, align 1
  store i8 97, i8* %c, align 1
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$c" = load i8, i8* %c, align 1
  %LE = icmp sle i8 %"$c", 101
  br i1 %LE, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$c1" = load i8, i8* %c, align 1
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i8 %"$c1")
  %"$c2" = load i8, i8* %c, align 1
  %AS = sext i8 %"$c2" to i32
  %ADD = add i32 %AS, 1
  %AS3 = trunc i32 %ADD to i8
  store i8 %AS3, i8* %c, align 1
  br label %while0

endloop:                                          ; preds = %while0
  %call.printf4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %"$c5" = load i8, i8* %c, align 1
  %AS6 = sext i8 %"$c5" to i32
  ret i32 %AS6
}
```

---

## 033 — crazy: isNullable on array elements and chars

```ura
// expect: 33   (3 zeros * 10, plus 1, plus 2)
fn count_zero(a i32[], n i32) i32:
    c i32 = 0
    i i32 = 0
    while i < n:
        if isZero(a[i]): c += 1
        i += 1
    return c

fn main() i32:
    a i32[] = [0, 5, 0, 0, 7]
    s i8[] = "ab"
    r i32 = count_zero(a, 5) * 10
    if isZero(s[2]): r += 1
    if !isZero(s[0]): r += 2
    return r
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @count_zero(i32* %0, i32 %1) {
entry:
  %a = alloca i32*, align 8
  %n = alloca i32, align 4
  %c = alloca i32, align 4
  %i = alloca i32, align 4
  store i32* %0, i32** %a, align 8
  store i32 %1, i32* %n, align 4
  store i32 0, i32* %c, align 4
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %endif, %entry
  %"$i" = load i32, i32* %i, align 4
  %"$n" = load i32, i32* %n, align 4
  %LT = icmp slt i32 %"$i", %"$n"
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  br label %if1

if1:                                              ; preds = %while0.then
  %"$a" = load i32*, i32** %a, align 8
  %"$i1" = load i32, i32* %i, align 4
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 %"$i1"
  %"$a[]" = load i32, i32* %"a[]", align 4
  %EQ = icmp eq i32 %"$a[]", 0
  br i1 %EQ, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  %"$c" = load i32, i32* %c, align 4
  %ADD = add i32 %"$c", 1
  store i32 %ADD, i32* %c, align 4
  br label %endif

endif:                                            ; preds = %if1.then, %if1
  %"$i2" = load i32, i32* %i, align 4
  %ADD3 = add i32 %"$i2", 1
  store i32 %ADD3, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$c4" = load i32, i32* %c, align 4
  ret i32 %"$c4"
}

define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %arr = alloca [5 x i32], align 4
  %s = alloca i8*, align 8
  %arr2 = alloca [3 x i8], align 1
  %r = alloca i32, align 4
  store [5 x i32] [i32 0, i32 5, i32 0, i32 0, i32 7], [5 x i32]* %arr, align 4
  %0 = getelementptr inbounds [5 x i32], [5 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %a, align 8
  store [3 x i8] c"ab\00", [3 x i8]* %arr2, align 1
  %1 = getelementptr inbounds [3 x i8], [3 x i8]* %arr2, i32 0, i32 0
  store i8* %1, i8** %s, align 8
  %"$a" = load i32*, i32** %a, align 8
  %call.count_zero = call i32 @count_zero(i32* %"$a", i32 5)
  %MUL = mul i32 %call.count_zero, 10
  store i32 %MUL, i32* %r, align 4
  br label %if2

if2:                                              ; preds = %entry
  %"$s" = load i8*, i8** %s, align 8
  %"s[]" = getelementptr inbounds i8, i8* %"$s", i32 2
  %"$s[]" = load i8, i8* %"s[]", align 1
  %EQ = icmp eq i8 %"$s[]", 0
  br i1 %EQ, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"$r" = load i32, i32* %r, align 4
  %ADD = add i32 %"$r", 1
  store i32 %ADD, i32* %r, align 4
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  br label %if3

if3:                                              ; preds = %endif
  %"$s3" = load i8*, i8** %s, align 8
  %"s[]4" = getelementptr inbounds i8, i8* %"$s3", i32 0
  %"$s[]5" = load i8, i8* %"s[]4", align 1
  %EQ6 = icmp eq i8 %"$s[]5", 0
  %NOT = xor i1 %EQ6, true
  br i1 %NOT, label %if3.then, label %endif1

if3.then:                                         ; preds = %if3
  %"$r7" = load i32, i32* %r, align 4
  %ADD8 = add i32 %"$r7", 2
  store i32 %ADD8, i32* %r, align 4
  br label %endif1

endif1:                                           ; preds = %if3.then, %if3
  %"$r9" = load i32, i32* %r, align 4
  ret i32 %"$r9"
}
```

---

## 034 — crazy: transpose a 3x3 matrix stored flat

```ura
// expect: 154   (trace 15 * 10, plus t[1] = 4)
fn get(m i32[], w i32, r i32, c i32) i32:
    return m[r * w + c]

fn main() i32:
    m i32[] = [1, 2, 3, 4, 5, 6, 7, 8, 9]
    t i32[] = [0, 0, 0, 0, 0, 0, 0, 0, 0]
    r i32 = 0
    while r < 3:
        c i32 = 0
        while c < 3:
            t[c * 3 + r] = get(m, 3, r, c)
            c += 1
        r += 1
    trace i32 = 0
    i i32 = 0
    while i < 3:
        trace += t[i * 3 + i]
        i += 1
    return trace * 10 + t[1]
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @get(i32* %0, i32 %1, i32 %2, i32 %3) {
entry:
  %m = alloca i32*, align 8
  %w = alloca i32, align 4
  %r = alloca i32, align 4
  %c = alloca i32, align 4
  store i32* %0, i32** %m, align 8
  store i32 %1, i32* %w, align 4
  store i32 %2, i32* %r, align 4
  store i32 %3, i32* %c, align 4
  %"$r" = load i32, i32* %r, align 4
  %"$w" = load i32, i32* %w, align 4
  %MUL = mul i32 %"$r", %"$w"
  %"$c" = load i32, i32* %c, align 4
  %ADD = add i32 %MUL, %"$c"
  %"$m" = load i32*, i32** %m, align 8
  %"m[]" = getelementptr inbounds i32, i32* %"$m", i32 %ADD
  %"$m[]" = load i32, i32* %"m[]", align 4
  ret i32 %"$m[]"
}

define i32 @main() {
entry:
  %m = alloca i32*, align 8
  %arr = alloca [9 x i32], align 4
  %t = alloca i32*, align 8
  %arr3 = alloca [9 x i32], align 4
  %r = alloca i32, align 4
  %c = alloca i32, align 4
  %trace = alloca i32, align 4
  %i = alloca i32, align 4
  store [9 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5, i32 6, i32 7, i32 8, i32 9], [9 x i32]* %arr, align 4
  %0 = getelementptr inbounds [9 x i32], [9 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %m, align 8
  store [9 x i32] zeroinitializer, [9 x i32]* %arr3, align 4
  %1 = getelementptr inbounds [9 x i32], [9 x i32]* %arr3, i32 0, i32 0
  store i32* %1, i32** %t, align 8
  store i32 0, i32* %r, align 4
  br label %while0

while0:                                           ; preds = %endloop, %entry
  %"$r" = load i32, i32* %r, align 4
  %LT = icmp slt i32 %"$r", 3
  br i1 %LT, label %while0.then, label %endloop1

while0.then:                                      ; preds = %while0
  store i32 0, i32* %c, align 4
  br label %while1

while1:                                           ; preds = %while1.then, %while0.then
  %"$c" = load i32, i32* %c, align 4
  %LT4 = icmp slt i32 %"$c", 3
  br i1 %LT4, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"$c5" = load i32, i32* %c, align 4
  %MUL = mul i32 %"$c5", 3
  %"$r6" = load i32, i32* %r, align 4
  %ADD = add i32 %MUL, %"$r6"
  %"$t" = load i32*, i32** %t, align 8
  %"t[]" = getelementptr inbounds i32, i32* %"$t", i32 %ADD
  %"$m" = load i32*, i32** %m, align 8
  %"$r7" = load i32, i32* %r, align 4
  %"$c8" = load i32, i32* %c, align 4
  %call.get = call i32 @get(i32* %"$m", i32 3, i32 %"$r7", i32 %"$c8")
  store i32 %call.get, i32* %"t[]", align 4
  %"$c9" = load i32, i32* %c, align 4
  %ADD10 = add i32 %"$c9", 1
  store i32 %ADD10, i32* %c, align 4
  br label %while1

endloop:                                          ; preds = %while1
  %"$r11" = load i32, i32* %r, align 4
  %ADD12 = add i32 %"$r11", 1
  store i32 %ADD12, i32* %r, align 4
  br label %while0

endloop1:                                         ; preds = %while0
  store i32 0, i32* %trace, align 4
  store i32 0, i32* %i, align 4
  br label %while2

while2:                                           ; preds = %while2.then, %endloop1
  %"$i" = load i32, i32* %i, align 4
  %LT13 = icmp slt i32 %"$i", 3
  br i1 %LT13, label %while2.then, label %endloop2

while2.then:                                      ; preds = %while2
  %"$i14" = load i32, i32* %i, align 4
  %MUL15 = mul i32 %"$i14", 3
  %"$i16" = load i32, i32* %i, align 4
  %ADD17 = add i32 %MUL15, %"$i16"
  %"$t18" = load i32*, i32** %t, align 8
  %"t[]19" = getelementptr inbounds i32, i32* %"$t18", i32 %ADD17
  %"$t[]" = load i32, i32* %"t[]19", align 4
  %"$trace" = load i32, i32* %trace, align 4
  %ADD20 = add i32 %"$trace", %"$t[]"
  store i32 %ADD20, i32* %trace, align 4
  %"$i21" = load i32, i32* %i, align 4
  %ADD22 = add i32 %"$i21", 1
  store i32 %ADD22, i32* %i, align 4
  br label %while2

endloop2:                                         ; preds = %while2
  %"$trace23" = load i32, i32* %trace, align 4
  %MUL24 = mul i32 %"$trace23", 10
  %"$t25" = load i32*, i32** %t, align 8
  %"t[]26" = getelementptr inbounds i32, i32* %"$t25", i32 1
  %"$t[]27" = load i32, i32* %"t[]26", align 4
  %ADD28 = add i32 %MUL24, %"$t[]27"
  ret i32 %ADD28
}
```

---

## 035 — crazy: a ref to an array element

```ura
// expect: 92   (10 + 22 + 60)
fn inc(p &i32):
    p += 1

fn main() i32:
    a i32[] = [10, 20, 30]
    inc(&a[1])
    inc(&a[1])
    p &i32 = &a[2]
    p *= 2
    return a[0] + a[1] + a[2]
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define void @inc(i32* %0) {
entry:
  %p = alloca i32*, align 8
  store i32* %0, i32** %p, align 8
  %"*p" = load i32*, i32** %p, align 8
  %"$*p" = load i32, i32* %"*p", align 4
  %ADD = add i32 %"$*p", 1
  store i32 %ADD, i32* %"*p", align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %arr = alloca [3 x i32], align 4
  %p = alloca i32*, align 8
  store [3 x i32] [i32 10, i32 20, i32 30], [3 x i32]* %arr, align 4
  %0 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %a, align 8
  %"$a" = load i32*, i32** %a, align 8
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 1
  call void @inc(i32* %"a[]")
  %"$a1" = load i32*, i32** %a, align 8
  %"a[]2" = getelementptr inbounds i32, i32* %"$a1", i32 1
  call void @inc(i32* %"a[]2")
  %"$a3" = load i32*, i32** %a, align 8
  %"a[]4" = getelementptr inbounds i32, i32* %"$a3", i32 2
  store i32* %"a[]4", i32** %p, align 8
  %"*p" = load i32*, i32** %p, align 8
  %"$*p" = load i32, i32* %"*p", align 4
  %MUL = mul i32 %"$*p", 2
  store i32 %MUL, i32* %"*p", align 4
  %"$a5" = load i32*, i32** %a, align 8
  %"a[]6" = getelementptr inbounds i32, i32* %"$a5", i32 0
  %"$a7" = load i32*, i32** %a, align 8
  %"a[]8" = getelementptr inbounds i32, i32* %"$a7", i32 1
  %"$a[]" = load i32, i32* %"a[]6", align 4
  %"$a[]9" = load i32, i32* %"a[]8", align 4
  %ADD = add i32 %"$a[]", %"$a[]9"
  %"$a10" = load i32*, i32** %a, align 8
  %"a[]11" = getelementptr inbounds i32, i32* %"$a10", i32 2
  %"$a[]12" = load i32, i32* %"a[]11", align 4
  %ADD13 = add i32 %ADD, %"$a[]12"
  ret i32 %ADD13
}
```

---

## 036 — crazy: an array literal made of calls

```ura
// expect: 50   (1 + 4 + 9 + 20 + 16)
fn sq(x i32) i32:
    return x * x

fn main() i32:
    k i32 = 2
    a i32[] = [sq(1), sq(k), sq(k + 1), k * 10, sq(sq(k))]
    return a[0] + a[1] + a[2] + a[3] + a[4]
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @sq(i32 %0) {
entry:
  %x = alloca i32, align 4
  store i32 %0, i32* %x, align 4
  %"$x" = load i32, i32* %x, align 4
  %"$x1" = load i32, i32* %x, align 4
  %MUL = mul i32 %"$x", %"$x1"
  ret i32 %MUL
}

define i32 @main() {
entry:
  %k = alloca i32, align 4
  %a = alloca i32*, align 8
  %arr = alloca [5 x i32], align 4
  store i32 2, i32* %k, align 4
  %call.sq = call i32 @sq(i32 1)
  %"$k" = load i32, i32* %k, align 4
  %call.sq1 = call i32 @sq(i32 %"$k")
  %"$k2" = load i32, i32* %k, align 4
  %ADD = add i32 %"$k2", 1
  %call.sq3 = call i32 @sq(i32 %ADD)
  %"$k4" = load i32, i32* %k, align 4
  %MUL = mul i32 %"$k4", 10
  %"$k5" = load i32, i32* %k, align 4
  %call.sq6 = call i32 @sq(i32 %"$k5")
  %call.sq7 = call i32 @sq(i32 %call.sq6)
  %0 = insertvalue [5 x i32] undef, i32 %call.sq, 0
  %1 = insertvalue [5 x i32] %0, i32 %call.sq1, 1
  %2 = insertvalue [5 x i32] %1, i32 %call.sq3, 2
  %3 = insertvalue [5 x i32] %2, i32 %MUL, 3
  %4 = insertvalue [5 x i32] %3, i32 %call.sq7, 4
  store [5 x i32] %4, [5 x i32]* %arr, align 4
  %5 = getelementptr inbounds [5 x i32], [5 x i32]* %arr, i32 0, i32 0
  store i32* %5, i32** %a, align 8
  %"$a" = load i32*, i32** %a, align 8
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 0
  %"$a8" = load i32*, i32** %a, align 8
  %"a[]9" = getelementptr inbounds i32, i32* %"$a8", i32 1
  %"$a[]" = load i32, i32* %"a[]", align 4
  %"$a[]10" = load i32, i32* %"a[]9", align 4
  %ADD11 = add i32 %"$a[]", %"$a[]10"
  %"$a12" = load i32*, i32** %a, align 8
  %"a[]13" = getelementptr inbounds i32, i32* %"$a12", i32 2
  %"$a[]14" = load i32, i32* %"a[]13", align 4
  %ADD15 = add i32 %ADD11, %"$a[]14"
  %"$a16" = load i32*, i32** %a, align 8
  %"a[]17" = getelementptr inbounds i32, i32* %"$a16", i32 3
  %"$a[]18" = load i32, i32* %"a[]17", align 4
  %ADD19 = add i32 %ADD15, %"$a[]18"
  %"$a20" = load i32*, i32** %a, align 8
  %"a[]21" = getelementptr inbounds i32, i32* %"$a20", i32 4
  %"$a[]22" = load i32, i32* %"a[]21", align 4
  %ADD23 = add i32 %ADD19, %"$a[]22"
  ret i32 %ADD23
}
```

---

## 037 — crazy: elif chain on one line each, with not / and / or

```ura
// expect: 31
fn grade(n i32) char:
    if n >= 90: return 'A'
    elif n >= 80 and not (n == 85): return 'B'
    elif n == 85 or n >= 70: return 'C'
    else: return 'F'

fn main() i32:
    r i32 = 0
    if grade(95) == 'A': r += 1
    if grade(82) == 'B': r += 2
    if grade(85) == 'C': r += 4
    if grade(72) == 'C': r += 8
    if grade(50) == 'F': r += 16
    return r
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i8 @grade(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %GE = icmp sge i32 %"$n", 90
  br i1 %GE, label %if0.then, label %elif1

if0.then:                                         ; preds = %if0
  ret i8 65

elif1:                                            ; preds = %if0
  %"$n1" = load i32, i32* %n, align 4
  %GE2 = icmp sge i32 %"$n1", 80
  %"$n3" = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %"$n3", 85
  %NOT = xor i1 %EQ, true
  %AND = and i1 %GE2, %NOT
  br i1 %AND, label %elif1.then, label %elif2

elif1.then:                                       ; preds = %elif1
  ret i8 66

elif2:                                            ; preds = %elif1
  %"$n4" = load i32, i32* %n, align 4
  %EQ5 = icmp eq i32 %"$n4", 85
  %"$n6" = load i32, i32* %n, align 4
  %GE7 = icmp sge i32 %"$n6", 70
  %OR = or i1 %EQ5, %GE7
  br i1 %OR, label %elif2.then, label %else3

elif2.then:                                       ; preds = %elif2
  ret i8 67

else3:                                            ; preds = %elif2
  ret i8 70

endif:                                            ; No predecessors!
  ret i8 0
}

define i32 @main() {
entry:
  %r = alloca i32, align 4
  store i32 0, i32* %r, align 4
  br label %if4

if4:                                              ; preds = %entry
  %call.grade = call i8 @grade(i32 95)
  %EQ = icmp eq i8 %call.grade, 65
  br i1 %EQ, label %if4.then, label %endif

if4.then:                                         ; preds = %if4
  %"$r" = load i32, i32* %r, align 4
  %ADD = add i32 %"$r", 1
  store i32 %ADD, i32* %r, align 4
  br label %endif

endif:                                            ; preds = %if4.then, %if4
  br label %if5

if5:                                              ; preds = %endif
  %call.grade5 = call i8 @grade(i32 82)
  %EQ6 = icmp eq i8 %call.grade5, 66
  br i1 %EQ6, label %if5.then, label %endif1

if5.then:                                         ; preds = %if5
  %"$r7" = load i32, i32* %r, align 4
  %ADD8 = add i32 %"$r7", 2
  store i32 %ADD8, i32* %r, align 4
  br label %endif1

endif1:                                           ; preds = %if5.then, %if5
  br label %if6

if6:                                              ; preds = %endif1
  %call.grade9 = call i8 @grade(i32 85)
  %EQ10 = icmp eq i8 %call.grade9, 67
  br i1 %EQ10, label %if6.then, label %endif2

if6.then:                                         ; preds = %if6
  %"$r11" = load i32, i32* %r, align 4
  %ADD12 = add i32 %"$r11", 4
  store i32 %ADD12, i32* %r, align 4
  br label %endif2

endif2:                                           ; preds = %if6.then, %if6
  br label %if7

if7:                                              ; preds = %endif2
  %call.grade13 = call i8 @grade(i32 72)
  %EQ14 = icmp eq i8 %call.grade13, 67
  br i1 %EQ14, label %if7.then, label %endif3

if7.then:                                         ; preds = %if7
  %"$r15" = load i32, i32* %r, align 4
  %ADD16 = add i32 %"$r15", 8
  store i32 %ADD16, i32* %r, align 4
  br label %endif3

endif3:                                           ; preds = %if7.then, %if7
  br label %if8

if8:                                              ; preds = %endif3
  %call.grade17 = call i8 @grade(i32 50)
  %EQ18 = icmp eq i8 %call.grade17, 70
  br i1 %EQ18, label %if8.then, label %endif4

if8.then:                                         ; preds = %if8
  %"$r19" = load i32, i32* %r, align 4
  %ADD20 = add i32 %"$r19", 16
  store i32 %ADD20, i32* %r, align 4
  br label %endif4

endif4:                                           ; preds = %if8.then, %if8
  %"$r21" = load i32, i32* %r, align 4
  ret i32 %"$r21"
}
```

---

## 038 — crazy: reverse a string, palindromes, a while on one line

```ura
// stdout: gnal aru
// expect: 83   (1 + 2, plus length 8 * 10)
fn len(s i8[]) i32:
    n i32 = 0
    while s[n] != '\0': n += 1
    return n

fn is_palindrome(s i8[]) bool:
    i i32 = 0
    j i32 = len(s) - 1
    while i < j:
        if s[i] != s[j]: return False
        i += 1
        j -= 1
    return True

fn reverse(s i8[]):
    i i32 = 0
    j i32 = len(s) - 1
    while i < j:
        t char = s[i]
        s[i] = s[j]
        s[j] = t
        i += 1
        j -= 1

proto printf(fmt i8[], ...) i32

fn main() i32:
    a i8[] = "racecar"
    b i8[] = "ura lang"
    reverse(b)
    printf("%s\n", b)
    r i32 = 0
    if is_palindrome(a): r += 1
    if not is_palindrome(b): r += 2
    return r + len(b) * 10
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

define i32 @len(i8* %0) {
entry:
  %s = alloca i8*, align 8
  %n = alloca i32, align 4
  store i8* %0, i8** %s, align 8
  store i32 0, i32* %n, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$s" = load i8*, i8** %s, align 8
  %"$n" = load i32, i32* %n, align 4
  %"s[]" = getelementptr inbounds i8, i8* %"$s", i32 %"$n"
  %"$s[]" = load i8, i8* %"s[]", align 1
  %NQ = icmp ne i8 %"$s[]", 0
  br i1 %NQ, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$n1" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n1", 1
  store i32 %ADD, i32* %n, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$n2" = load i32, i32* %n, align 4
  ret i32 %"$n2"
}

define i1 @is_palindrome(i8* %0) {
entry:
  %s = alloca i8*, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  store i8* %0, i8** %s, align 8
  store i32 0, i32* %i, align 4
  %"$s" = load i8*, i8** %s, align 8
  %call.len = call i32 @len(i8* %"$s")
  %SUB = sub i32 %call.len, 1
  store i32 %SUB, i32* %j, align 4
  br label %while1

while1:                                           ; preds = %endif, %entry
  %"$i" = load i32, i32* %i, align 4
  %"$j" = load i32, i32* %j, align 4
  %LT = icmp slt i32 %"$i", %"$j"
  br i1 %LT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  br label %if2

if2:                                              ; preds = %while1.then
  %"$s1" = load i8*, i8** %s, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"s[]" = getelementptr inbounds i8, i8* %"$s1", i32 %"$i2"
  %"$s3" = load i8*, i8** %s, align 8
  %"$j4" = load i32, i32* %j, align 4
  %"s[]5" = getelementptr inbounds i8, i8* %"$s3", i32 %"$j4"
  %"$s[]" = load i8, i8* %"s[]", align 1
  %"$s[]6" = load i8, i8* %"s[]5", align 1
  %NQ = icmp ne i8 %"$s[]", %"$s[]6"
  br i1 %NQ, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  ret i1 false

endif:                                            ; preds = %if2
  %"$i7" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i7", 1
  store i32 %ADD, i32* %i, align 4
  %"$j8" = load i32, i32* %j, align 4
  %SUB9 = sub i32 %"$j8", 1
  store i32 %SUB9, i32* %j, align 4
  br label %while1

endloop:                                          ; preds = %while1
  ret i1 true
}

define void @reverse(i8* %0) {
entry:
  %s = alloca i8*, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %t = alloca i8, align 1
  store i8* %0, i8** %s, align 8
  store i32 0, i32* %i, align 4
  %"$s" = load i8*, i8** %s, align 8
  %call.len = call i32 @len(i8* %"$s")
  %SUB = sub i32 %call.len, 1
  store i32 %SUB, i32* %j, align 4
  br label %while3

while3:                                           ; preds = %while3.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %"$j" = load i32, i32* %j, align 4
  %LT = icmp slt i32 %"$i", %"$j"
  br i1 %LT, label %while3.then, label %endloop

while3.then:                                      ; preds = %while3
  %"$s1" = load i8*, i8** %s, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"s[]" = getelementptr inbounds i8, i8* %"$s1", i32 %"$i2"
  %"$s[]" = load i8, i8* %"s[]", align 1
  store i8 %"$s[]", i8* %t, align 1
  %"$s3" = load i8*, i8** %s, align 8
  %"$i4" = load i32, i32* %i, align 4
  %"s[]5" = getelementptr inbounds i8, i8* %"$s3", i32 %"$i4"
  %"$s6" = load i8*, i8** %s, align 8
  %"$j7" = load i32, i32* %j, align 4
  %"s[]8" = getelementptr inbounds i8, i8* %"$s6", i32 %"$j7"
  %"$s[]9" = load i8, i8* %"s[]8", align 1
  store i8 %"$s[]9", i8* %"s[]5", align 1
  %"$s10" = load i8*, i8** %s, align 8
  %"$j11" = load i32, i32* %j, align 4
  %"s[]12" = getelementptr inbounds i8, i8* %"$s10", i32 %"$j11"
  %"$t" = load i8, i8* %t, align 1
  store i8 %"$t", i8* %"s[]12", align 1
  %"$i13" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i13", 1
  store i32 %ADD, i32* %i, align 4
  %"$j14" = load i32, i32* %j, align 4
  %SUB15 = sub i32 %"$j14", 1
  store i32 %SUB15, i32* %j, align 4
  br label %while3

endloop:                                          ; preds = %while3
  ret void
}

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %a = alloca i8*, align 8
  %arr = alloca [8 x i8], align 1
  %b = alloca i8*, align 8
  %arr2 = alloca [9 x i8], align 1
  %r = alloca i32, align 4
  store [8 x i8] c"racecar\00", [8 x i8]* %arr, align 1
  %0 = getelementptr inbounds [8 x i8], [8 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %a, align 8
  store [9 x i8] c"ura lang\00", [9 x i8]* %arr2, align 1
  %1 = getelementptr inbounds [9 x i8], [9 x i8]* %arr2, i32 0, i32 0
  store i8* %1, i8** %b, align 8
  %"$b" = load i8*, i8** %b, align 8
  call void @reverse(i8* %"$b")
  %"$b3" = load i8*, i8** %b, align 8
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i8* %"$b3")
  store i32 0, i32* %r, align 4
  br label %if4

if4:                                              ; preds = %entry
  %"$a" = load i8*, i8** %a, align 8
  %call.is_palindrome = call i1 @is_palindrome(i8* %"$a")
  br i1 %call.is_palindrome, label %if4.then, label %endif

if4.then:                                         ; preds = %if4
  %"$r" = load i32, i32* %r, align 4
  %ADD = add i32 %"$r", 1
  store i32 %ADD, i32* %r, align 4
  br label %endif

endif:                                            ; preds = %if4.then, %if4
  br label %if5

if5:                                              ; preds = %endif
  %"$b4" = load i8*, i8** %b, align 8
  %call.is_palindrome5 = call i1 @is_palindrome(i8* %"$b4")
  %NOT = xor i1 %call.is_palindrome5, true
  br i1 %NOT, label %if5.then, label %endif1

if5.then:                                         ; preds = %if5
  %"$r6" = load i32, i32* %r, align 4
  %ADD7 = add i32 %"$r6", 2
  store i32 %ADD7, i32* %r, align 4
  br label %endif1

endif1:                                           ; preds = %if5.then, %if5
  %"$b8" = load i8*, i8** %b, align 8
  %call.len = call i32 @len(i8* %"$b8")
  %MUL = mul i32 %call.len, 10
  %"$r9" = load i32, i32* %r, align 4
  %ADD10 = add i32 %"$r9", %MUL
  ret i32 %ADD10
}
```

---

## 039 — crazy: sieve of eratosthenes on a bool array

```ura
// expect: 77   (2 + 3 + 5 + 7 + 11 + 13 + 17 + 19)
fn main() i32:
    composite bool[] = [False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False]
    sum i32 = 0
    i i32 = 2
    while i < 20:
        if not composite[i]:
            sum += i
            j i32 = i * i
            while j < 20:
                composite[j] = True
                j += i
        i += 1
    return sum
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %composite = alloca i1*, align 8
  %arr = alloca [20 x i1], align 1
  %sum = alloca i32, align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  store [20 x i1] zeroinitializer, [20 x i1]* %arr, align 1
  %0 = getelementptr inbounds [20 x i1], [20 x i1]* %arr, i32 0, i32 0
  store i1* %0, i1** %composite, align 8
  store i32 0, i32* %sum, align 4
  store i32 2, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %endif, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 20
  br i1 %LT, label %while0.then, label %endloop1

while0.then:                                      ; preds = %while0
  br label %if1

if1:                                              ; preds = %while0.then
  %"$composite" = load i1*, i1** %composite, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"composite[]" = getelementptr inbounds i1, i1* %"$composite", i32 %"$i2"
  %"$composite[]" = load i1, i1* %"composite[]", align 1
  %NOT = xor i1 %"$composite[]", true
  br i1 %NOT, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  %"$i3" = load i32, i32* %i, align 4
  %"$sum" = load i32, i32* %sum, align 4
  %ADD = add i32 %"$sum", %"$i3"
  store i32 %ADD, i32* %sum, align 4
  %"$i4" = load i32, i32* %i, align 4
  %"$i5" = load i32, i32* %i, align 4
  %MUL = mul i32 %"$i4", %"$i5"
  store i32 %MUL, i32* %j, align 4
  br label %while2

while2:                                           ; preds = %while2.then, %if1.then
  %"$j" = load i32, i32* %j, align 4
  %LT6 = icmp slt i32 %"$j", 20
  br i1 %LT6, label %while2.then, label %endloop

while2.then:                                      ; preds = %while2
  %"$composite7" = load i1*, i1** %composite, align 8
  %"$j8" = load i32, i32* %j, align 4
  %"composite[]9" = getelementptr inbounds i1, i1* %"$composite7", i32 %"$j8"
  store i1 true, i1* %"composite[]9", align 1
  %"$i10" = load i32, i32* %i, align 4
  %"$j11" = load i32, i32* %j, align 4
  %ADD12 = add i32 %"$j11", %"$i10"
  store i32 %ADD12, i32* %j, align 4
  br label %while2

endloop:                                          ; preds = %while2
  br label %endif

endif:                                            ; preds = %endloop, %if1
  %"$i13" = load i32, i32* %i, align 4
  %ADD14 = add i32 %"$i13", 1
  store i32 %ADD14, i32* %i, align 4
  br label %while0

endloop1:                                         ; preds = %while0
  %"$sum15" = load i32, i32* %sum, align 4
  ret i32 %"$sum15"
}
```

---

## 040 — crazy: atoi: parse numbers from strings

```ura
// expect: 85   (123 - 45 + 7)
fn atoi(s i8[]) i32:
    n i32 = 0
    i i32 = 0
    neg bool = False
    if s[0] == '-':
        neg = True
        i = 1
    while s[i] >= '0' and s[i] <= '9':
        n = n * 10 + (s[i] as i32) - ('0' as i32)
        i += 1
    if neg: return -n
    return n

fn main() i32:
    return atoi("123") + atoi("-45") + atoi("7x")
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"123\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"-45\00", align 1
@str.2 = private unnamed_addr constant [3 x i8] c"7x\00", align 1

define i32 @atoi(i8* %0) {
entry:
  %s = alloca i8*, align 8
  %n = alloca i32, align 4
  %i = alloca i32, align 4
  %neg = alloca i1, align 1
  store i8* %0, i8** %s, align 8
  store i32 0, i32* %n, align 4
  store i32 0, i32* %i, align 4
  store i1 false, i1* %neg, align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$s" = load i8*, i8** %s, align 8
  %"s[]" = getelementptr inbounds i8, i8* %"$s", i32 0
  %"$s[]" = load i8, i8* %"s[]", align 1
  %EQ = icmp eq i8 %"$s[]", 45
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  store i1 true, i1* %neg, align 1
  store i32 1, i32* %i, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %while1

while1:                                           ; preds = %while1.then, %endif
  %"$s2" = load i8*, i8** %s, align 8
  %"$i" = load i32, i32* %i, align 4
  %"s[]3" = getelementptr inbounds i8, i8* %"$s2", i32 %"$i"
  %"$s[]4" = load i8, i8* %"s[]3", align 1
  %GE = icmp sge i8 %"$s[]4", 48
  %"$s5" = load i8*, i8** %s, align 8
  %"$i6" = load i32, i32* %i, align 4
  %"s[]7" = getelementptr inbounds i8, i8* %"$s5", i32 %"$i6"
  %"$s[]8" = load i8, i8* %"s[]7", align 1
  %LE = icmp sle i8 %"$s[]8", 57
  %AND = and i1 %GE, %LE
  br i1 %AND, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"$n" = load i32, i32* %n, align 4
  %MUL = mul i32 %"$n", 10
  %"$s9" = load i8*, i8** %s, align 8
  %"$i10" = load i32, i32* %i, align 4
  %"s[]11" = getelementptr inbounds i8, i8* %"$s9", i32 %"$i10"
  %"$s[]12" = load i8, i8* %"s[]11", align 1
  %AS = sext i8 %"$s[]12" to i32
  %ADD = add i32 %MUL, %AS
  %SUB = sub i32 %ADD, 48
  store i32 %SUB, i32* %n, align 4
  %"$i13" = load i32, i32* %i, align 4
  %ADD14 = add i32 %"$i13", 1
  store i32 %ADD14, i32* %i, align 4
  br label %while1

endloop:                                          ; preds = %while1
  br label %if2

if2:                                              ; preds = %endloop
  %"$neg" = load i1, i1* %neg, align 1
  br i1 %"$neg", label %if2.then, label %endif1

if2.then:                                         ; preds = %if2
  %"$n15" = load i32, i32* %n, align 4
  %NEG = sub i32 0, %"$n15"
  ret i32 %NEG

endif1:                                           ; preds = %if2
  %"$n16" = load i32, i32* %n, align 4
  ret i32 %"$n16"
}

define i32 @main() {
entry:
  %call.atoi = call i32 @atoi(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  %call.atoi1 = call i32 @atoi(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0))
  %ADD = add i32 %call.atoi, %call.atoi1
  %call.atoi2 = call i32 @atoi(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.2, i32 0, i32 0))
  %ADD3 = add i32 %ADD, %call.atoi2
  ret i32 %ADD3
}
```

---

## 041 — crazy: itoa: write a number's digits into a buffer

```ura
// stdout: 9071
// expect: 57   ('9')
proto printf(fmt i8[], ...) i32

fn itoa(n i32, out i8[]):
    digits i8[] = "0000000000"
    k i32 = 0
    if n == 0:
        out[0] = '0'
        out[1] = '\0'
        return
    while n > 0:
        digits[k] = ((n % 10) + ('0' as i32)) as char
        n /= 10
        k += 1
    i i32 = 0
    while i < k:
        out[i] = digits[k - 1 - i]
        i += 1
    out[k] = '\0'

fn main() i32:
    buf i8[] = "............"
    itoa(9071, buf)
    printf("%s\n", buf)
    return buf[0] as i32
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @itoa(i32 %0, i8* %1) {
entry:
  %n = alloca i32, align 4
  %out = alloca i8*, align 8
  %digits = alloca i8*, align 8
  %arr = alloca [11 x i8], align 1
  %k = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  store i8* %1, i8** %out, align 8
  store [11 x i8] c"0000000000\00", [11 x i8]* %arr, align 1
  %2 = getelementptr inbounds [11 x i8], [11 x i8]* %arr, i32 0, i32 0
  store i8* %2, i8** %digits, align 8
  store i32 0, i32* %k, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %"$n", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$out" = load i8*, i8** %out, align 8
  %"out[]" = getelementptr inbounds i8, i8* %"$out", i32 0
  store i8 48, i8* %"out[]", align 1
  %"$out2" = load i8*, i8** %out, align 8
  %"out[]3" = getelementptr inbounds i8, i8* %"$out2", i32 1
  store i8 0, i8* %"out[]3", align 1
  ret void

endif:                                            ; preds = %if0
  br label %while1

while1:                                           ; preds = %while1.then, %endif
  %"$n4" = load i32, i32* %n, align 4
  %GT = icmp sgt i32 %"$n4", 0
  br i1 %GT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"$digits" = load i8*, i8** %digits, align 8
  %"$k" = load i32, i32* %k, align 4
  %"digits[]" = getelementptr inbounds i8, i8* %"$digits", i32 %"$k"
  %"$n5" = load i32, i32* %n, align 4
  %MOD = srem i32 %"$n5", 10
  %ADD = add i32 %MOD, 48
  %AS = trunc i32 %ADD to i8
  store i8 %AS, i8* %"digits[]", align 1
  %"$n6" = load i32, i32* %n, align 4
  %DIV = sdiv i32 %"$n6", 10
  store i32 %DIV, i32* %n, align 4
  %"$k7" = load i32, i32* %k, align 4
  %ADD8 = add i32 %"$k7", 1
  store i32 %ADD8, i32* %k, align 4
  br label %while1

endloop:                                          ; preds = %while1
  store i32 0, i32* %i, align 4
  br label %while2

while2:                                           ; preds = %while2.then, %endloop
  %"$i" = load i32, i32* %i, align 4
  %"$k9" = load i32, i32* %k, align 4
  %LT = icmp slt i32 %"$i", %"$k9"
  br i1 %LT, label %while2.then, label %endloop1

while2.then:                                      ; preds = %while2
  %"$out10" = load i8*, i8** %out, align 8
  %"$i11" = load i32, i32* %i, align 4
  %"out[]12" = getelementptr inbounds i8, i8* %"$out10", i32 %"$i11"
  %"$k13" = load i32, i32* %k, align 4
  %SUB = sub i32 %"$k13", 1
  %"$i14" = load i32, i32* %i, align 4
  %SUB15 = sub i32 %SUB, %"$i14"
  %"$digits16" = load i8*, i8** %digits, align 8
  %"digits[]17" = getelementptr inbounds i8, i8* %"$digits16", i32 %SUB15
  %"$digits[]" = load i8, i8* %"digits[]17", align 1
  store i8 %"$digits[]", i8* %"out[]12", align 1
  %"$i18" = load i32, i32* %i, align 4
  %ADD19 = add i32 %"$i18", 1
  store i32 %ADD19, i32* %i, align 4
  br label %while2

endloop1:                                         ; preds = %while2
  %"$out20" = load i8*, i8** %out, align 8
  %"$k21" = load i32, i32* %k, align 4
  %"out[]22" = getelementptr inbounds i8, i8* %"$out20", i32 %"$k21"
  store i8 0, i8* %"out[]22", align 1
  ret void
}

define i32 @main() {
entry:
  %buf = alloca i8*, align 8
  %arr = alloca [13 x i8], align 1
  store [13 x i8] c"............\00", [13 x i8]* %arr, align 1
  %0 = getelementptr inbounds [13 x i8], [13 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %buf, align 8
  %"$buf" = load i8*, i8** %buf, align 8
  call void @itoa(i32 9071, i8* %"$buf")
  %"$buf1" = load i8*, i8** %buf, align 8
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i8* %"$buf1")
  %"$buf2" = load i8*, i8** %buf, align 8
  %"buf[]" = getelementptr inbounds i8, i8* %"$buf2", i32 0
  %"$buf[]" = load i8, i8* %"buf[]", align 1
  %AS = sext i8 %"$buf[]" to i32
  ret i32 %AS
}
```

---

## 042 — crazy: print a number in hex with as

```ura
// stdout: 0000beef
// expect: 98   ('b')
proto printf(fmt i8[], ...) i32

fn hex_digit(n i32) char:
    if n < 10: return (('0' as i32) + n) as char
    return (('a' as i32) + n - 10) as char

fn to_hex(n i32, out i8[]):
    i i32 = 7
    while i >= 0:
        out[i] = hex_digit(n % 16)
        n /= 16
        i -= 1
    out[8] = '\0'

buf i8[] = "........."
to_hex(48879, buf)
printf("%s\n", buf)
return buf[4] as i32
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@buf = global i8* null
@str = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

declare i32 @printf(i8*, ...)

define i8 @hex_digit(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %LT = icmp slt i32 %"$n", 10
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$n1" = load i32, i32* %n, align 4
  %ADD = add i32 48, %"$n1"
  %AS = trunc i32 %ADD to i8
  ret i8 %AS

endif:                                            ; preds = %if0
  %"$n2" = load i32, i32* %n, align 4
  %ADD3 = add i32 97, %"$n2"
  %SUB = sub i32 %ADD3, 10
  %AS4 = trunc i32 %SUB to i8
  ret i8 %AS4
}

define void @to_hex(i32 %0, i8* %1) {
entry:
  %n = alloca i32, align 4
  %out = alloca i8*, align 8
  %i = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  store i8* %1, i8** %out, align 8
  store i32 7, i32* %i, align 4
  br label %while1

while1:                                           ; preds = %while1.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %GE = icmp sge i32 %"$i", 0
  br i1 %GE, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"$out" = load i8*, i8** %out, align 8
  %"$i1" = load i32, i32* %i, align 4
  %"out[]" = getelementptr inbounds i8, i8* %"$out", i32 %"$i1"
  %"$n" = load i32, i32* %n, align 4
  %MOD = srem i32 %"$n", 16
  %call.hex_digit = call i8 @hex_digit(i32 %MOD)
  store i8 %call.hex_digit, i8* %"out[]", align 1
  %"$n2" = load i32, i32* %n, align 4
  %DIV = sdiv i32 %"$n2", 16
  store i32 %DIV, i32* %n, align 4
  %"$i3" = load i32, i32* %i, align 4
  %SUB = sub i32 %"$i3", 1
  store i32 %SUB, i32* %i, align 4
  br label %while1

endloop:                                          ; preds = %while1
  %"$out4" = load i8*, i8** %out, align 8
  %"out[]5" = getelementptr inbounds i8, i8* %"$out4", i32 8
  store i8 0, i8* %"out[]5", align 1
  ret void
}

define i32 @main() {
entry:
  %arr = alloca [10 x i8], align 1
  store [10 x i8] c".........\00", [10 x i8]* %arr, align 1
  %0 = getelementptr inbounds [10 x i8], [10 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** @buf, align 8
  %"$buf" = load i8*, i8** @buf, align 8
  call void @to_hex(i32 48879, i8* %"$buf")
  %"$buf1" = load i8*, i8** @buf, align 8
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i8* %"$buf1")
  %"$buf2" = load i8*, i8** @buf, align 8
  %"buf[]" = getelementptr inbounds i8, i8* %"$buf2", i32 4
  %"$buf[]" = load i8, i8* %"buf[]", align 1
  %AS = sext i8 %"$buf[]" to i32
  ret i32 %AS
}
```

---

## 043 — crazy: a string builder on a global buffer

```ura
// stdout: x=1234, y=7
// expect: 12
proto printf(fmt i8[], ...) i32

out i8[] = "                                "
len i32 = 0

fn put(c char):
    out[len] = c
    len += 1

fn put_num(n i32):
    if n >= 10: put_num(n / 10)
    put((('0' as i32) + n % 10) as char)

fn put_str(s i8[]):
    i i32 = 0
    while s[i] != '\0':
        put(s[i])
        i += 1

put_str("x=")
put_num(1234)
put_str(", y=")
put_num(7)
put('\0')
printf("%s\n", out)
return len
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@out = global i8* null
@len = global i32 0
@str = private unnamed_addr constant [3 x i8] c"x=\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c", y=\00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @put(i8 %0) {
entry:
  %c = alloca i8, align 1
  store i8 %0, i8* %c, align 1
  %"$out" = load i8*, i8** @out, align 8
  %"$len" = load i32, i32* @len, align 4
  %"out[]" = getelementptr inbounds i8, i8* %"$out", i32 %"$len"
  %"$c" = load i8, i8* %c, align 1
  store i8 %"$c", i8* %"out[]", align 1
  %"$len1" = load i32, i32* @len, align 4
  %ADD = add i32 %"$len1", 1
  store i32 %ADD, i32* @len, align 4
  ret void
}

define void @put_num(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %GE = icmp sge i32 %"$n", 10
  br i1 %GE, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$n1" = load i32, i32* %n, align 4
  %DIV = sdiv i32 %"$n1", 10
  call void @put_num(i32 %DIV)
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  %"$n2" = load i32, i32* %n, align 4
  %MOD = srem i32 %"$n2", 10
  %ADD = add i32 48, %MOD
  %AS = trunc i32 %ADD to i8
  call void @put(i8 %AS)
  ret void
}

define void @put_str(i8* %0) {
entry:
  %s = alloca i8*, align 8
  %i = alloca i32, align 4
  store i8* %0, i8** %s, align 8
  store i32 0, i32* %i, align 4
  br label %while1

while1:                                           ; preds = %while1.then, %entry
  %"$s" = load i8*, i8** %s, align 8
  %"$i" = load i32, i32* %i, align 4
  %"s[]" = getelementptr inbounds i8, i8* %"$s", i32 %"$i"
  %"$s[]" = load i8, i8* %"s[]", align 1
  %NQ = icmp ne i8 %"$s[]", 0
  br i1 %NQ, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"$s1" = load i8*, i8** %s, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"s[]3" = getelementptr inbounds i8, i8* %"$s1", i32 %"$i2"
  %"$s[]4" = load i8, i8* %"s[]3", align 1
  call void @put(i8 %"$s[]4")
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define i32 @main() {
entry:
  %arr = alloca [33 x i8], align 1
  store [33 x i8] c"                                \00", [33 x i8]* %arr, align 1
  %0 = getelementptr inbounds [33 x i8], [33 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** @out, align 8
  store i32 0, i32* @len, align 4
  call void @put_str(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0))
  call void @put_num(i32 1234)
  call void @put_str(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0))
  call void @put_num(i32 7)
  call void @put(i8 0)
  %"$out" = load i8*, i8** @out, align 8
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i8* %"$out")
  %"$len" = load i32, i32* @len, align 4
  ret i32 %"$len"
}
```

---

## 044 — crazy: filter with as bool, not and isNullable

```ura
// expect: 42   (4 odd, 2 even, zeros skipped)
fn count_if(a i32[], n i32, odd bool) i32:
    c i32 = 0
    i i32 = 0
    while i < n:
        is_odd bool = (a[i] % 2) as bool
        if is_odd == odd and not isZero(a[i]): c += 1
        i += 1
    return c

a i32[] = [0, 1, 2, 3, 4, 5, -3, 0]
return count_if(a, 8, True) * 10 + count_if(a, 8, False)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@a = global i32* null

define i32 @count_if(i32* %0, i32 %1, i1 %2) {
entry:
  %a = alloca i32*, align 8
  %n = alloca i32, align 4
  %odd = alloca i1, align 1
  %c = alloca i32, align 4
  %i = alloca i32, align 4
  %is_odd = alloca i1, align 1
  store i32* %0, i32** %a, align 8
  store i32 %1, i32* %n, align 4
  store i1 %2, i1* %odd, align 1
  store i32 0, i32* %c, align 4
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %endif, %entry
  %"$i" = load i32, i32* %i, align 4
  %"$n" = load i32, i32* %n, align 4
  %LT = icmp slt i32 %"$i", %"$n"
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$a" = load i32*, i32** %a, align 8
  %"$i1" = load i32, i32* %i, align 4
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 %"$i1"
  %"$a[]" = load i32, i32* %"a[]", align 4
  %MOD = srem i32 %"$a[]", 2
  %AS = icmp ne i32 %MOD, 0
  store i1 %AS, i1* %is_odd, align 1
  br label %if1

if1:                                              ; preds = %while0.then
  %"$is_odd" = load i1, i1* %is_odd, align 1
  %"$odd" = load i1, i1* %odd, align 1
  %EQ = icmp eq i1 %"$is_odd", %"$odd"
  %"$a2" = load i32*, i32** %a, align 8
  %"$i3" = load i32, i32* %i, align 4
  %"a[]4" = getelementptr inbounds i32, i32* %"$a2", i32 %"$i3"
  %"$a[]5" = load i32, i32* %"a[]4", align 4
  %EQ6 = icmp eq i32 %"$a[]5", 0
  %NOT = xor i1 %EQ6, true
  %AND = and i1 %EQ, %NOT
  br i1 %AND, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  %"$c" = load i32, i32* %c, align 4
  %ADD = add i32 %"$c", 1
  store i32 %ADD, i32* %c, align 4
  br label %endif

endif:                                            ; preds = %if1.then, %if1
  %"$i7" = load i32, i32* %i, align 4
  %ADD8 = add i32 %"$i7", 1
  store i32 %ADD8, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$c9" = load i32, i32* %c, align 4
  ret i32 %"$c9"
}

define i32 @main() {
entry:
  %arr = alloca [8 x i32], align 4
  store [8 x i32] [i32 0, i32 1, i32 2, i32 3, i32 4, i32 5, i32 -3, i32 0], [8 x i32]* %arr, align 4
  %0 = getelementptr inbounds [8 x i32], [8 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** @a, align 8
  %"$a" = load i32*, i32** @a, align 8
  %call.count_if = call i32 @count_if(i32* %"$a", i32 8, i1 true)
  %MUL = mul i32 %call.count_if, 10
  %"$a1" = load i32*, i32** @a, align 8
  %call.count_if2 = call i32 @count_if(i32* %"$a1", i32 8, i1 false)
  %ADD = add i32 %MUL, %call.count_if2
  ret i32 %ADD
}
```

