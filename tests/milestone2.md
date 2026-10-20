# milestone2

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
- 012 — ref: write through a ref
- 013 — ref: read through a ref, then write
- 014 — ref: compound assignment through a ref
- 015 — ref: swap with ref parameters
- 016 — ref: a ref parameter changes the caller
- 017 — ref: p1 = p2 copies the value
- 018 — ref: p1 = &p2 rebinds
- 019 — ref: pass a ref parameter on to another function
- 020 — ref: a ref to a bool
- 021 — ref: a ref in a loop
- 022 — ref: take the address of a variable
- 023 — char: escaped chars
- 024 — proto: a string in a variable
- 025 — proto: a string passed to a function
- 026 — proto: proto variadic function
- 027 — proto: chars type
- 028 — parser error: unclosed [ in an array literal
- 029 — program: fibonacci with a memo array
- 030 — program: upper-case a string in place
- 031 — program: not and ! on bools, in a loop
- 032 — program: count trues with as, then a cast that wraps
- 033 — program: caesar cipher: char arithmetic with as
- 034 — program: print the alphabet with a char counter
- 035 — program: isNullable on array elements and chars
- 036 — program: transpose a 3x3 matrix stored flat
- 037 — program: a ref to an array element
- 038 — program: an array literal made of calls
- 039 — program: elif chain on one line each, with not / and / or
- 040 — program: reverse a string, palindromes, a while on one line
- 041 — program: sieve of eratosthenes on a bool array
- 042 — program: atoi: parse numbers from strings
- 043 — program: itoa: write a number's digits into a buffer
- 044 — program: print a number in hex with as
- 045 — program: a string builder on a global buffer
- 046 — program: filter with as bool, not and isNullable

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
  %array = alloca [3 x i32], align 4
  %e = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32 4, i32* %e, align 4
  %e1 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 1
  store i32 5, i32* %e1, align 4
  %e2 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 2
  store i32 6, i32* %e2, align 4
  %array3 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32* %array3, i32** %a, align 8
  %load.a = load i32*, i32** %a, align 8
  %"load.a[0]" = getelementptr inbounds i32, i32* %load.a, i32 0
  %"load.load.a[0]" = load i32, i32* %"load.a[0]", align 4
  %load.a4 = load i32*, i32** %a, align 8
  %"load.a[2]" = getelementptr inbounds i32, i32* %load.a4, i32 2
  %"load.load.a[2]" = load i32, i32* %"load.a[2]", align 4
  %ADD = add i32 %"load.load.a[0]", %"load.load.a[2]"
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
  %array = alloca [3 x i32], align 4
  %e = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32 1, i32* %e, align 4
  %e1 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 1
  store i32 2, i32* %e1, align 4
  %e2 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 2
  store i32 3, i32* %e2, align 4
  %array3 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32* %array3, i32** %a, align 8
  %load.a = load i32*, i32** %a, align 8
  %"load.a[1]" = getelementptr inbounds i32, i32* %load.a, i32 1
  store i32 9, i32* %"load.a[1]", align 4
  %load.a4 = load i32*, i32** %a, align 8
  %"load.a[1]5" = getelementptr inbounds i32, i32* %load.a4, i32 1
  %"load.load.a[1]" = load i32, i32* %"load.a[1]5", align 4
  ret i32 %"load.load.a[1]"
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
  %array = alloca [3 x i32], align 4
  %i = alloca i32, align 4
  %e = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32 7, i32* %e, align 4
  %e1 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 1
  store i32 8, i32* %e1, align 4
  %e2 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 2
  store i32 9, i32* %e2, align 4
  %array3 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32* %array3, i32** %a, align 8
  store i32 2, i32* %i, align 4
  %load.a = load i32*, i32** %a, align 8
  %load.i = load i32, i32* %i, align 4
  %"load.a[load.i]" = getelementptr inbounds i32, i32* %load.a, i32 %load.i
  %"load.load.a[load.i]" = load i32, i32* %"load.a[load.i]", align 4
  ret i32 %"load.load.a[load.i]"
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
  %array = alloca [3 x i32], align 4
  %i = alloca i32, align 4
  %e = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32 7, i32* %e, align 4
  %e1 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 1
  store i32 8, i32* %e1, align 4
  %e2 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 2
  store i32 9, i32* %e2, align 4
  %array3 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32* %array3, i32** %a, align 8
  store i32 2, i32* %i, align 4
  %load.a = load i32*, i32** %a, align 8
  %load.i = load i32, i32* %i, align 4
  %SUB = sub i32 %load.i, 1
  %"load.a[load.i - 1]" = getelementptr inbounds i32, i32* %load.a, i32 %SUB
  %"load.load.a[load.i - 1]" = load i32, i32* %"load.a[load.i - 1]", align 4
  ret i32 %"load.load.a[load.i - 1]"
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
  %array = alloca [3 x i32], align 4
  store i32 10, i32* %x, align 4
  store i32 4, i32* %y, align 4
  %load.x = load i32, i32* %x, align 4
  %load.y = load i32, i32* %y, align 4
  %e = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32 %load.x, i32* %e, align 4
  %e1 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 1
  store i32 2, i32* %e1, align 4
  %e2 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 2
  store i32 %load.y, i32* %e2, align 4
  %array3 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32* %array3, i32** %a, align 8
  %load.a = load i32*, i32** %a, align 8
  %"load.a[0]" = getelementptr inbounds i32, i32* %load.a, i32 0
  %"load.load.a[0]" = load i32, i32* %"load.a[0]", align 4
  %load.a4 = load i32*, i32** %a, align 8
  %"load.a[1]" = getelementptr inbounds i32, i32* %load.a4, i32 1
  %"load.load.a[1]" = load i32, i32* %"load.a[1]", align 4
  %ADD = add i32 %"load.load.a[0]", %"load.load.a[1]"
  %load.a5 = load i32*, i32** %a, align 8
  %"load.a[2]" = getelementptr inbounds i32, i32* %load.a5, i32 2
  %"load.load.a[2]" = load i32, i32* %"load.a[2]", align 4
  %ADD6 = add i32 %ADD, %"load.load.a[2]"
  ret i32 %ADD6
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
  %array = alloca [3 x i32], align 4
  %e = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32 1, i32* %e, align 4
  %e1 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 1
  store i32 2, i32* %e1, align 4
  %e2 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 2
  store i32 3, i32* %e2, align 4
  %array3 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32* %array3, i32** %a, align 8
  %load.a = load i32*, i32** %a, align 8
  %"load.a[0]" = getelementptr inbounds i32, i32* %load.a, i32 0
  %old = load i32, i32* %"load.a[0]", align 4
  %ADD_ASSIGN = add i32 %old, 4
  store i32 %ADD_ASSIGN, i32* %"load.a[0]", align 4
  %load.a4 = load i32*, i32** %a, align 8
  %"load.a[2]" = getelementptr inbounds i32, i32* %load.a4, i32 2
  %old5 = load i32, i32* %"load.a[2]", align 4
  %MUL_ASSIGN = mul i32 %old5, 3
  store i32 %MUL_ASSIGN, i32* %"load.a[2]", align 4
  %load.a6 = load i32*, i32** %a, align 8
  %"load.a[0]7" = getelementptr inbounds i32, i32* %load.a6, i32 0
  %"load.load.a[0]" = load i32, i32* %"load.a[0]7", align 4
  %load.a8 = load i32*, i32** %a, align 8
  %"load.a[1]" = getelementptr inbounds i32, i32* %load.a8, i32 1
  %"load.load.a[1]" = load i32, i32* %"load.a[1]", align 4
  %ADD = add i32 %"load.load.a[0]", %"load.load.a[1]"
  %load.a9 = load i32*, i32** %a, align 8
  %"load.a[2]10" = getelementptr inbounds i32, i32* %load.a9, i32 2
  %"load.load.a[2]" = load i32, i32* %"load.a[2]10", align 4
  %ADD11 = add i32 %ADD, %"load.load.a[2]"
  %ADD12 = add i32 %ADD11, 2
  ret i32 %ADD12
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

@str = private constant [3 x i8] c"%d\00"
@str.1 = private constant [2 x i8] c" \00"
@str.2 = private constant [2 x i8] c"\0A\00"

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %array = alloca [5 x i32], align 4
  %sum = alloca i32, align 4
  %i = alloca i32, align 4
  %s = alloca [3 x i8], align 1
  %s10 = alloca [2 x i8], align 1
  %s18 = alloca [2 x i8], align 1
  %e = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 0
  store i32 3, i32* %e, align 4
  %e1 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 1
  store i32 1, i32* %e1, align 4
  %e2 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 2
  store i32 4, i32* %e2, align 4
  %e3 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 3
  store i32 1, i32* %e3, align 4
  %e4 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 4
  store i32 5, i32* %e4, align 4
  %array5 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 0
  store i32* %array5, i32** %a, align 8
  store i32 0, i32* %sum, align 4
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 5
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %dst = getelementptr inbounds [3 x i8], [3 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i64 3, i1 false)
  %load.a = load i32*, i32** %a, align 8
  %load.i6 = load i32, i32* %i, align 4
  %"load.a[load.i]" = getelementptr inbounds i32, i32* %load.a, i32 %load.i6
  %"load.load.a[load.i]" = load i32, i32* %"load.a[load.i]", align 4
  %"printf(str,load.load.a[load.i])" = call i32 (i8*, ...) @printf(i8* %dst, i32 %"load.load.a[load.i]")
  %load.i8 = load i32, i32* %i, align 4
  %LT9 = icmp slt i32 %load.i8, 4
  br i1 %LT9, label %then7, label %endif

endwhile:                                         ; preds = %cond
  %dst19 = getelementptr inbounds [2 x i8], [2 x i8]* %s18, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst19, i8* align 1 getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0), i64 2, i1 false)
  %"printf(str)20" = call i32 (i8*, ...) @printf(i8* %dst19)
  %load.sum = load i32, i32* %sum, align 4
  ret i32 %load.sum

then7:                                            ; preds = %then
  %dst11 = getelementptr inbounds [2 x i8], [2 x i8]* %s10, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst11, i8* align 1 getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i64 2, i1 false)
  %"printf(str)" = call i32 (i8*, ...) @printf(i8* %dst11)
  br label %endif

endif:                                            ; preds = %then7, %then
  %load.a12 = load i32*, i32** %a, align 8
  %load.i13 = load i32, i32* %i, align 4
  %"load.a[load.i]14" = getelementptr inbounds i32, i32* %load.a12, i32 %load.i13
  %"load.load.a[load.i]15" = load i32, i32* %"load.a[load.i]14", align 4
  %old = load i32, i32* %sum, align 4
  %ADD_ASSIGN = add i32 %old, %"load.load.a[load.i]15"
  store i32 %ADD_ASSIGN, i32* %sum, align 4
  %old16 = load i32, i32* %i, align 4
  %ADD_ASSIGN17 = add i32 %old16, 1
  store i32 %ADD_ASSIGN17, i32* %i, align 4
  br label %cond
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
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
  br label %cond

cond:                                             ; preds = %then, %entry
  %load.i = load i32, i32* %i, align 4
  %load.n = load i32, i32* %n, align 4
  %LT = icmp slt i32 %load.i, %load.n
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.a = load i32*, i32** %a, align 8
  %load.i1 = load i32, i32* %i, align 4
  %"load.a[load.i]" = getelementptr inbounds i32, i32* %load.a, i32 %load.i1
  %"load.load.a[load.i]" = load i32, i32* %"load.a[load.i]", align 4
  %old = load i32, i32* %total, align 4
  %ADD_ASSIGN = add i32 %old, %"load.load.a[load.i]"
  store i32 %ADD_ASSIGN, i32* %total, align 4
  %old2 = load i32, i32* %i, align 4
  %ADD_ASSIGN3 = add i32 %old2, 1
  store i32 %ADD_ASSIGN3, i32* %i, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  %load.total = load i32, i32* %total, align 4
  ret i32 %load.total
}

define i32 @main() {
entry:
  %values = alloca i32*, align 8
  %array = alloca [3 x i32], align 4
  %e = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32 1, i32* %e, align 4
  %e1 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 1
  store i32 2, i32* %e1, align 4
  %e2 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 2
  store i32 3, i32* %e2, align 4
  %array3 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32* %array3, i32** %values, align 8
  %load.values = load i32*, i32** %values, align 8
  %"sum(load.values,3)" = call i32 @sum(i32* %load.values, i32 3)
  ret i32 %"sum(load.values,3)"
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
  %load.a = load i32*, i32** %a, align 8
  %"load.a[0]" = getelementptr inbounds i32, i32* %load.a, i32 0
  %load.v = load i32, i32* %v, align 4
  store i32 %load.v, i32* %"load.a[0]", align 4
  ret void
}

define i32 @main() {
entry:
  %values = alloca i32*, align 8
  %array = alloca [3 x i32], align 4
  %e = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32 1, i32* %e, align 4
  %e1 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 1
  store i32 2, i32* %e1, align 4
  %e2 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 2
  store i32 3, i32* %e2, align 4
  %array3 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32* %array3, i32** %values, align 8
  %load.values = load i32*, i32** %values, align 8
  call void @set_first(i32* %load.values, i32 7)
  %load.values4 = load i32*, i32** %values, align 8
  %"load.values[0]" = getelementptr inbounds i32, i32* %load.values4, i32 0
  %"load.load.values[0]" = load i32, i32* %"load.values[0]", align 4
  ret i32 %"load.load.values[0]"
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

@str = private constant [4 x i8] c"abc\00"
@str.1 = private constant [4 x i8] c"%s\0A\00"

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %s = alloca i8*, align 8
  %s1 = alloca [4 x i8], align 1
  %s2 = alloca [4 x i8], align 1
  %dst = getelementptr inbounds [4 x i8], [4 x i8]* %s1, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i64 4, i1 false)
  store i8* %dst, i8** %s, align 8
  %load.s = load i8*, i8** %s, align 8
  %"load.s[0]" = getelementptr inbounds i8, i8* %load.s, i32 0
  store i8 120, i8* %"load.s[0]", align 1
  %dst3 = getelementptr inbounds [4 x i8], [4 x i8]* %s2, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst3, i8* align 1 getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i64 4, i1 false)
  %load.s4 = load i8*, i8** %s, align 8
  %"printf(str,load.s)" = call i32 (i8*, ...) @printf(i8* %dst3, i8* %load.s4)
  %load.s5 = load i8*, i8** %s, align 8
  %"load.s[1]" = getelementptr inbounds i8, i8* %load.s5, i32 1
  %"load.load.s[1]" = load i8, i8* %"load.s[1]", align 1
  %EQ = icmp eq i8 %"load.load.s[1]", 98
  br i1 %EQ, label %then, label %endif

then:                                             ; preds = %entry
  ret i32 1

endif:                                            ; preds = %entry
  ret i32 0
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
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
  %array = alloca [3 x i1], align 1
  %n = alloca i32, align 4
  %i = alloca i32, align 4
  %e = getelementptr inbounds [3 x i1], [3 x i1]* %array, i32 0, i32 0
  store i1 true, i1* %e, align 1
  %e1 = getelementptr inbounds [3 x i1], [3 x i1]* %array, i32 0, i32 1
  store i1 false, i1* %e1, align 1
  %e2 = getelementptr inbounds [3 x i1], [3 x i1]* %array, i32 0, i32 2
  store i1 true, i1* %e2, align 1
  %array3 = getelementptr inbounds [3 x i1], [3 x i1]* %array, i32 0, i32 0
  store i1* %array3, i1** %flags, align 8
  store i32 0, i32* %n, align 4
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 3
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.flags = load i1*, i1** %flags, align 8
  %load.i5 = load i32, i32* %i, align 4
  %"load.flags[load.i]" = getelementptr inbounds i1, i1* %load.flags, i32 %load.i5
  %"load.load.flags[load.i]" = load i1, i1* %"load.flags[load.i]", align 1
  br i1 %"load.load.flags[load.i]", label %then4, label %endif

endwhile:                                         ; preds = %cond
  %load.n = load i32, i32* %n, align 4
  ret i32 %load.n

then4:                                            ; preds = %then
  %old = load i32, i32* %n, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %then4, %then
  %old6 = load i32, i32* %i, align 4
  %ADD_ASSIGN7 = add i32 %old6, 1
  store i32 %ADD_ASSIGN7, i32* %i, align 4
  br label %cond
}
```

---

## 012 — ref: write through a ref

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

@str = private constant [8 x i8] c"a = %d\0A\00"

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %p = alloca i32*, align 8
  %s = alloca [8 x i8], align 1
  store i32 10, i32* %a, align 4
  store i32* %a, i32** %p, align 8
  %dref.p = load i32*, i32** %p, align 8
  store i32 12, i32* %dref.p, align 4
  %dst = getelementptr inbounds [8 x i8], [8 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([8 x i8], [8 x i8]* @str, i32 0, i32 0), i64 8, i1 false)
  %load.a = load i32, i32* %a, align 4
  %"printf(str,load.a)" = call i32 (i8*, ...) @printf(i8* %dst, i32 %load.a)
  %load.a1 = load i32, i32* %a, align 4
  ret i32 %load.a1
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 013 — ref: read through a ref, then write

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

@str = private constant [9 x i8] c"p = %d, \00"
@str.1 = private constant [8 x i8] c"a = %d\0A\00"

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %p = alloca i32*, align 8
  %s = alloca [9 x i8], align 1
  %s4 = alloca [8 x i8], align 1
  store i32 10, i32* %a, align 4
  store i32* %a, i32** %p, align 8
  %dst = getelementptr inbounds [9 x i8], [9 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i64 9, i1 false)
  %dref.p = load i32*, i32** %p, align 8
  %load.dref.p = load i32, i32* %dref.p, align 4
  %"printf(str,load.dref.p)" = call i32 (i8*, ...) @printf(i8* %dst, i32 %load.dref.p)
  %dref.p1 = load i32*, i32** %p, align 8
  %dref.p2 = load i32*, i32** %p, align 8
  %load.dref.p3 = load i32, i32* %dref.p2, align 4
  %ADD = add i32 %load.dref.p3, 5
  store i32 %ADD, i32* %dref.p1, align 4
  %dst5 = getelementptr inbounds [8 x i8], [8 x i8]* %s4, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst5, i8* align 1 getelementptr inbounds ([8 x i8], [8 x i8]* @str.1, i32 0, i32 0), i64 8, i1 false)
  %load.a = load i32, i32* %a, align 4
  %"printf(str,load.a)" = call i32 (i8*, ...) @printf(i8* %dst5, i32 %load.a)
  %load.a6 = load i32, i32* %a, align 4
  ret i32 %load.a6
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 014 — ref: compound assignment through a ref

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
  %dref.p = load i32*, i32** %p, align 8
  %old = load i32, i32* %dref.p, align 4
  %ADD_ASSIGN = add i32 %old, 2
  store i32 %ADD_ASSIGN, i32* %dref.p, align 4
  %dref.p1 = load i32*, i32** %p, align 8
  %old2 = load i32, i32* %dref.p1, align 4
  %MUL_ASSIGN = mul i32 %old2, 2
  store i32 %MUL_ASSIGN, i32* %dref.p1, align 4
  %load.a = load i32, i32* %a, align 4
  ret i32 %load.a
}
```

---

## 015 — ref: swap with ref parameters

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

@str = private constant [16 x i8] c"a = %d, b = %d\0A\00"

declare i32 @printf(i8*, ...)

define void @swap(i32* %0, i32* %1) {
entry:
  %x = alloca i32*, align 8
  %y = alloca i32*, align 8
  %t = alloca i32, align 4
  store i32* %0, i32** %x, align 8
  store i32* %1, i32** %y, align 8
  %dref.x = load i32*, i32** %x, align 8
  %load.dref.x = load i32, i32* %dref.x, align 4
  store i32 %load.dref.x, i32* %t, align 4
  %dref.x1 = load i32*, i32** %x, align 8
  %dref.y = load i32*, i32** %y, align 8
  %load.dref.y = load i32, i32* %dref.y, align 4
  store i32 %load.dref.y, i32* %dref.x1, align 4
  %dref.y2 = load i32*, i32** %y, align 8
  %load.t = load i32, i32* %t, align 4
  store i32 %load.t, i32* %dref.y2, align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %s = alloca [16 x i8], align 1
  store i32 1, i32* %a, align 4
  store i32 2, i32* %b, align 4
  call void @swap(i32* %a, i32* %b)
  %dst = getelementptr inbounds [16 x i8], [16 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([16 x i8], [16 x i8]* @str, i32 0, i32 0), i64 16, i1 false)
  %load.a = load i32, i32* %a, align 4
  %load.b = load i32, i32* %b, align 4
  %"printf(str,load.a,load.b)" = call i32 (i8*, ...) @printf(i8* %dst, i32 %load.a, i32 %load.b)
  %load.a1 = load i32, i32* %a, align 4
  %MUL = mul i32 %load.a1, 10
  %load.b2 = load i32, i32* %b, align 4
  %ADD = add i32 %MUL, %load.b2
  ret i32 %ADD
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 016 — ref: a ref parameter changes the caller

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
  %dref.c = load i32*, i32** %c, align 8
  %old = load i32, i32* %dref.c, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %dref.c, align 4
  ret void
}

define i32 @main() {
entry:
  %n = alloca i32, align 4
  store i32 0, i32* %n, align 4
  call void @inc(i32* %n)
  call void @inc(i32* %n)
  call void @inc(i32* %n)
  %load.n = load i32, i32* %n, align 4
  ret i32 %load.n
}
```

---

## 017 — ref: p1 = p2 copies the value

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

@str = private constant [16 x i8] c"a = %d, b = %d\0A\00"

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %p1 = alloca i32*, align 8
  %p2 = alloca i32*, align 8
  %s = alloca [16 x i8], align 1
  store i32 1, i32* %a, align 4
  store i32 2, i32* %b, align 4
  store i32* %a, i32** %p1, align 8
  store i32* %b, i32** %p2, align 8
  %dref.p1 = load i32*, i32** %p1, align 8
  %dref.p2 = load i32*, i32** %p2, align 8
  %load.dref.p2 = load i32, i32* %dref.p2, align 4
  store i32 %load.dref.p2, i32* %dref.p1, align 4
  %dst = getelementptr inbounds [16 x i8], [16 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([16 x i8], [16 x i8]* @str, i32 0, i32 0), i64 16, i1 false)
  %load.a = load i32, i32* %a, align 4
  %load.b = load i32, i32* %b, align 4
  %"printf(str,load.a,load.b)" = call i32 (i8*, ...) @printf(i8* %dst, i32 %load.a, i32 %load.b)
  %load.a1 = load i32, i32* %a, align 4
  ret i32 %load.a1
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 018 — ref: p1 = &p2 rebinds

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

@str = private constant [16 x i8] c"a = %d, b = %d\0A\00"

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %p1 = alloca i32*, align 8
  %p2 = alloca i32*, align 8
  %s = alloca [16 x i8], align 1
  store i32 1, i32* %a, align 4
  store i32 2, i32* %b, align 4
  store i32* %a, i32** %p1, align 8
  store i32* %b, i32** %p2, align 8
  %dref.p2 = load i32*, i32** %p2, align 8
  store i32* %dref.p2, i32** %p1, align 8
  %dref.p1 = load i32*, i32** %p1, align 8
  store i32 9, i32* %dref.p1, align 4
  %dst = getelementptr inbounds [16 x i8], [16 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([16 x i8], [16 x i8]* @str, i32 0, i32 0), i64 16, i1 false)
  %load.a = load i32, i32* %a, align 4
  %load.b = load i32, i32* %b, align 4
  %"printf(str,load.a,load.b)" = call i32 (i8*, ...) @printf(i8* %dst, i32 %load.a, i32 %load.b)
  %load.b1 = load i32, i32* %b, align 4
  ret i32 %load.b1
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 019 — ref: pass a ref parameter on to another function

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
  %dref.p = load i32*, i32** %p, align 8
  %old = load i32, i32* %dref.p, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %dref.p, align 4
  ret void
}

define void @add_two(i32* %0) {
entry:
  %p = alloca i32*, align 8
  store i32* %0, i32** %p, align 8
  %dref.p = load i32*, i32** %p, align 8
  call void @add_one(i32* %dref.p)
  %dref.p1 = load i32*, i32** %p, align 8
  call void @add_one(i32* %dref.p1)
  ret void
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 0, i32* %a, align 4
  call void @add_two(i32* %a)
  %load.a = load i32, i32* %a, align 4
  ret i32 %load.a
}
```

---

## 020 — ref: a ref to a bool

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
  %dref.p = load i1*, i1** %p, align 8
  store i1 true, i1* %dref.p, align 1
  %load.b = load i1, i1* %b, align 1
  br i1 %load.b, label %then, label %endif

then:                                             ; preds = %entry
  ret i32 1

endif:                                            ; preds = %entry
  ret i32 0
}
```

---

## 021 — ref: a ref in a loop

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

@str = private constant [4 x i8] c"%d \00"
@str.1 = private constant [2 x i8] c"\0A\00"

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %total = alloca i32, align 4
  %p = alloca i32*, align 8
  %i = alloca i32, align 4
  %s = alloca [4 x i8], align 1
  %s5 = alloca [2 x i8], align 1
  store i32 0, i32* %total, align 4
  store i32* %total, i32** %p, align 8
  store i32 1, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %then, %entry
  %load.i = load i32, i32* %i, align 4
  %LE = icmp sle i32 %load.i, 5
  br i1 %LE, label %then, label %endwhile

then:                                             ; preds = %cond
  %dst = getelementptr inbounds [4 x i8], [4 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i64 4, i1 false)
  %load.i1 = load i32, i32* %i, align 4
  %"printf(str,load.i)" = call i32 (i8*, ...) @printf(i8* %dst, i32 %load.i1)
  %dref.p = load i32*, i32** %p, align 8
  %load.i2 = load i32, i32* %i, align 4
  %old = load i32, i32* %dref.p, align 4
  %ADD_ASSIGN = add i32 %old, %load.i2
  store i32 %ADD_ASSIGN, i32* %dref.p, align 4
  %old3 = load i32, i32* %i, align 4
  %ADD_ASSIGN4 = add i32 %old3, 1
  store i32 %ADD_ASSIGN4, i32* %i, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  %dst6 = getelementptr inbounds [2 x i8], [2 x i8]* %s5, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst6, i8* align 1 getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i64 2, i1 false)
  %"printf(str)" = call i32 (i8*, ...) @printf(i8* %dst6)
  %load.total = load i32, i32* %total, align 4
  ret i32 %load.total
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 022 — ref: take the address of a variable

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

## 023 — char: escaped chars

```ura
// stdout: [newline] [tab] ['] [\]
// expect: 0
proto printf(fmt i8[], ...) i32

fn main() i32:
    nl char = '\n'
    tab char = '\t'
    printf("[%c]\n", nl)
    printf("[%c]\n", tab)
    printf("[%c] [%c]\n", '\'', '\\')
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private constant [6 x i8] c"[%c]\0A\00"
@str.1 = private constant [6 x i8] c"[%c]\0A\00"
@str.2 = private constant [11 x i8] c"[%c] [%c]\0A\00"

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %nl = alloca i8, align 1
  %tab = alloca i8, align 1
  %s = alloca [6 x i8], align 1
  %s1 = alloca [6 x i8], align 1
  %s3 = alloca [11 x i8], align 1
  store i8 10, i8* %nl, align 1
  store i8 9, i8* %tab, align 1
  %dst = getelementptr inbounds [6 x i8], [6 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i64 6, i1 false)
  %load.nl = load i8, i8* %nl, align 1
  %"printf(str,load.nl)" = call i32 (i8*, ...) @printf(i8* %dst, i8 %load.nl)
  %dst2 = getelementptr inbounds [6 x i8], [6 x i8]* %s1, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst2, i8* align 1 getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i64 6, i1 false)
  %load.tab = load i8, i8* %tab, align 1
  %"printf(str,load.tab)" = call i32 (i8*, ...) @printf(i8* %dst2, i8 %load.tab)
  %dst4 = getelementptr inbounds [11 x i8], [11 x i8]* %s3, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst4, i8* align 1 getelementptr inbounds ([11 x i8], [11 x i8]* @str.2, i32 0, i32 0), i64 11, i1 false)
  %"printf(str,''','\\')" = call i32 (i8*, ...) @printf(i8* %dst4, i8 39, i8 92)
  ret i32 0
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 024 — proto: a string in a variable

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

@str = private constant [7 x i8] c"abcdef\00"
@str.1 = private constant [6 x i8] c"<%s>\0A\00"

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %str = alloca i8*, align 8
  %s = alloca [7 x i8], align 1
  %s1 = alloca [6 x i8], align 1
  %dst = getelementptr inbounds [7 x i8], [7 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i64 7, i1 false)
  store i8* %dst, i8** %str, align 8
  %dst2 = getelementptr inbounds [6 x i8], [6 x i8]* %s1, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst2, i8* align 1 getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i64 6, i1 false)
  %load.str = load i8*, i8** %str, align 8
  %"printf(str,load.str)" = call i32 (i8*, ...) @printf(i8* %dst2, i8* %load.str)
  ret i32 0
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 025 — proto: a string passed to a function

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

@str = private constant [7 x i8] c"hi %s\0A\00"
@str.1 = private constant [4 x i8] c"ura\00"

declare i32 @printf(i8*, ...)

define void @greet(i8* %0) {
entry:
  %name = alloca i8*, align 8
  %s = alloca [7 x i8], align 1
  store i8* %0, i8** %name, align 8
  %dst = getelementptr inbounds [7 x i8], [7 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i64 7, i1 false)
  %load.name = load i8*, i8** %name, align 8
  %"printf(str,load.name)" = call i32 (i8*, ...) @printf(i8* %dst, i8* %load.name)
  ret void
}

define i32 @main() {
entry:
  %s = alloca [4 x i8], align 1
  %dst = getelementptr inbounds [4 x i8], [4 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i64 4, i1 false)
  call void @greet(i8* %dst)
  ret i32 0
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 026 — proto: proto variadic function

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

@str = private constant [7 x i8] c"abcdef\00"
@str.1 = private constant [5 x i8] c"<%s>\00"

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %str = alloca i8*, align 8
  %s = alloca [7 x i8], align 1
  %s1 = alloca [5 x i8], align 1
  %dst = getelementptr inbounds [7 x i8], [7 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i64 7, i1 false)
  store i8* %dst, i8** %str, align 8
  %dst2 = getelementptr inbounds [5 x i8], [5 x i8]* %s1, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst2, i8* align 1 getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0), i64 5, i1 false)
  %load.str = load i8*, i8** %str, align 8
  %"printf(str,load.str)" = call i32 (i8*, ...) @printf(i8* %dst2, i8* %load.str)
  ret i32 0
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 027 — proto: chars type

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

@str = private constant [7 x i8] c"abcdef\00"

define i32 @main() {
entry:
  %str = alloca i8*, align 8
  %s = alloca [7 x i8], align 1
  %dst = getelementptr inbounds [7 x i8], [7 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i64 7, i1 false)
  store i8* %dst, i8** %str, align 8
  ret i32 0
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 028 — parser error: unclosed [ in an array literal

```ura
// error: expected ']' to close this '['
// help: use a i32[] = [1, 2]
fn main() i32:
    a i32[] = [1, 2
    return a[0]
```

### stderr

```
error: test.ura:4:19 expected ']' to close this '['
  |
4 |     a i32[] = [1, 2
  |                   ^
help: use a i32[] = [1, 2]
```

### status

```
1
```

---

## 029 — program: fibonacci with a memo array

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
  %load.n = load i32, i32* %n, align 4
  %LT = icmp slt i32 %load.n, 2
  br i1 %LT, label %then, label %endif

then:                                             ; preds = %entry
  %load.n1 = load i32, i32* %n, align 4
  ret i32 %load.n1

endif:                                            ; preds = %entry
  %load.memo = load i32*, i32** %memo, align 8
  %load.n4 = load i32, i32* %n, align 4
  %"load.memo[load.n]" = getelementptr inbounds i32, i32* %load.memo, i32 %load.n4
  %"load.load.memo[load.n]" = load i32, i32* %"load.memo[load.n]", align 4
  %NQ = icmp ne i32 %"load.load.memo[load.n]", 0
  br i1 %NQ, label %then3, label %endif2

then3:                                            ; preds = %endif
  %load.memo5 = load i32*, i32** %memo, align 8
  %load.n6 = load i32, i32* %n, align 4
  %"load.memo[load.n]7" = getelementptr inbounds i32, i32* %load.memo5, i32 %load.n6
  %"load.load.memo[load.n]8" = load i32, i32* %"load.memo[load.n]7", align 4
  ret i32 %"load.load.memo[load.n]8"

endif2:                                           ; preds = %endif
  %load.memo9 = load i32*, i32** %memo, align 8
  %load.n10 = load i32, i32* %n, align 4
  %"load.memo[load.n]11" = getelementptr inbounds i32, i32* %load.memo9, i32 %load.n10
  %load.n12 = load i32, i32* %n, align 4
  %SUB = sub i32 %load.n12, 1
  %load.memo13 = load i32*, i32** %memo, align 8
  %"fib(load.n - 1,load.memo)" = call i32 @fib(i32 %SUB, i32* %load.memo13)
  %load.n14 = load i32, i32* %n, align 4
  %SUB15 = sub i32 %load.n14, 2
  %load.memo16 = load i32*, i32** %memo, align 8
  %"fib(load.n - 2,load.memo)" = call i32 @fib(i32 %SUB15, i32* %load.memo16)
  %ADD = add i32 %"fib(load.n - 1,load.memo)", %"fib(load.n - 2,load.memo)"
  store i32 %ADD, i32* %"load.memo[load.n]11", align 4
  %load.memo17 = load i32*, i32** %memo, align 8
  %load.n18 = load i32, i32* %n, align 4
  %"load.memo[load.n]19" = getelementptr inbounds i32, i32* %load.memo17, i32 %load.n18
  %"load.load.memo[load.n]20" = load i32, i32* %"load.memo[load.n]19", align 4
  ret i32 %"load.load.memo[load.n]20"
}

define i32 @main() {
entry:
  %memo = alloca i32*, align 8
  %array = alloca [21 x i32], align 4
  %e = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 0
  store i32 0, i32* %e, align 4
  %e1 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 1
  store i32 0, i32* %e1, align 4
  %e2 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 2
  store i32 0, i32* %e2, align 4
  %e3 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 3
  store i32 0, i32* %e3, align 4
  %e4 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 4
  store i32 0, i32* %e4, align 4
  %e5 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 5
  store i32 0, i32* %e5, align 4
  %e6 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 6
  store i32 0, i32* %e6, align 4
  %e7 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 7
  store i32 0, i32* %e7, align 4
  %e8 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 8
  store i32 0, i32* %e8, align 4
  %e9 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 9
  store i32 0, i32* %e9, align 4
  %e10 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 10
  store i32 0, i32* %e10, align 4
  %e11 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 11
  store i32 0, i32* %e11, align 4
  %e12 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 12
  store i32 0, i32* %e12, align 4
  %e13 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 13
  store i32 0, i32* %e13, align 4
  %e14 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 14
  store i32 0, i32* %e14, align 4
  %e15 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 15
  store i32 0, i32* %e15, align 4
  %e16 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 16
  store i32 0, i32* %e16, align 4
  %e17 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 17
  store i32 0, i32* %e17, align 4
  %e18 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 18
  store i32 0, i32* %e18, align 4
  %e19 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 19
  store i32 0, i32* %e19, align 4
  %e20 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 20
  store i32 0, i32* %e20, align 4
  %array21 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 0
  store i32* %array21, i32** %memo, align 8
  %load.memo = load i32*, i32** %memo, align 8
  %"fib(20,load.memo)" = call i32 @fib(i32 20, i32* %load.memo)
  %MOD = srem i32 %"fib(20,load.memo)", 256
  ret i32 %MOD
}
```

---

## 030 — program: upper-case a string in place

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

@str = private constant [11 x i8] c"hello, ura\00"
@str.1 = private constant [4 x i8] c"%s\0A\00"

declare i32 @printf(i8*, ...)

define i32 @length(i8* %0) {
entry:
  %s = alloca i8*, align 8
  %n = alloca i32, align 4
  store i8* %0, i8** %s, align 8
  store i32 0, i32* %n, align 4
  br label %cond

cond:                                             ; preds = %then, %entry
  %load.s = load i8*, i8** %s, align 8
  %load.n = load i32, i32* %n, align 4
  %"load.s[load.n]" = getelementptr inbounds i8, i8* %load.s, i32 %load.n
  %"load.load.s[load.n]" = load i8, i8* %"load.s[load.n]", align 1
  %NQ = icmp ne i8 %"load.load.s[load.n]", 0
  br i1 %NQ, label %then, label %endwhile

then:                                             ; preds = %cond
  %old = load i32, i32* %n, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %n, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  %load.n1 = load i32, i32* %n, align 4
  ret i32 %load.n1
}

define void @upper(i8* %0) {
entry:
  %s = alloca i8*, align 8
  %i = alloca i32, align 4
  store i8* %0, i8** %s, align 8
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.s = load i8*, i8** %s, align 8
  %load.i = load i32, i32* %i, align 4
  %"load.s[load.i]" = getelementptr inbounds i8, i8* %load.s, i32 %load.i
  %"load.load.s[load.i]" = load i8, i8* %"load.s[load.i]", align 1
  %NQ = icmp ne i8 %"load.load.s[load.i]", 0
  br i1 %NQ, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.s2 = load i8*, i8** %s, align 8
  %load.i3 = load i32, i32* %i, align 4
  %"load.s[load.i]4" = getelementptr inbounds i8, i8* %load.s2, i32 %load.i3
  %"load.load.s[load.i]5" = load i8, i8* %"load.s[load.i]4", align 1
  %GE = icmp sge i8 %"load.load.s[load.i]5", 97
  %load.s6 = load i8*, i8** %s, align 8
  %load.i7 = load i32, i32* %i, align 4
  %"load.s[load.i]8" = getelementptr inbounds i8, i8* %load.s6, i32 %load.i7
  %"load.load.s[load.i]9" = load i8, i8* %"load.s[load.i]8", align 1
  %LE = icmp sle i8 %"load.load.s[load.i]9", 122
  %AND = and i1 %GE, %LE
  br i1 %AND, label %then1, label %endif

endwhile:                                         ; preds = %cond
  ret void

then1:                                            ; preds = %then
  %load.s10 = load i8*, i8** %s, align 8
  %load.i11 = load i32, i32* %i, align 4
  %"load.s[load.i]12" = getelementptr inbounds i8, i8* %load.s10, i32 %load.i11
  %old = load i8, i8* %"load.s[load.i]12", align 1
  %SUB_ASSIGN = sub i8 %old, 32
  store i8 %SUB_ASSIGN, i8* %"load.s[load.i]12", align 1
  br label %endif

endif:                                            ; preds = %then1, %then
  %old13 = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old13, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  br label %cond
}

define i32 @main() {
entry:
  %s = alloca i8*, align 8
  %s1 = alloca [11 x i8], align 1
  %s2 = alloca [4 x i8], align 1
  %dst = getelementptr inbounds [11 x i8], [11 x i8]* %s1, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([11 x i8], [11 x i8]* @str, i32 0, i32 0), i64 11, i1 false)
  store i8* %dst, i8** %s, align 8
  %load.s = load i8*, i8** %s, align 8
  call void @upper(i8* %load.s)
  %dst3 = getelementptr inbounds [4 x i8], [4 x i8]* %s2, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst3, i8* align 1 getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i64 4, i1 false)
  %load.s4 = load i8*, i8** %s, align 8
  %"printf(str,load.s)" = call i32 (i8*, ...) @printf(i8* %dst3, i8* %load.s4)
  %load.s5 = load i8*, i8** %s, align 8
  %"length(load.s)" = call i32 @length(i8* %load.s5)
  ret i32 %"length(load.s)"
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 031 — program: not and ! on bools, in a loop

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
  %load.a = load i1, i1* %a, align 1
  %NOT = xor i1 %load.a, true
  store i1 %NOT, i1* %b, align 1
  store i32 0, i32* %n, align 4
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 5
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.a1 = load i1, i1* %a, align 1
  %NOT2 = xor i1 %load.a1, true
  store i1 %NOT2, i1* %a, align 1
  %load.a4 = load i1, i1* %a, align 1
  br i1 %load.a4, label %then3, label %endif

endwhile:                                         ; preds = %cond
  %load.b = load i1, i1* %b, align 1
  %NOT9 = xor i1 %load.b, true
  %load.n = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %load.n, 3
  %NOT10 = xor i1 %EQ, true
  %AND = and i1 %NOT9, %NOT10
  br i1 %AND, label %then8, label %endif7

then3:                                            ; preds = %then
  %old = load i32, i32* %n, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %then3, %then
  %old5 = load i32, i32* %i, align 4
  %ADD_ASSIGN6 = add i32 %old5, 1
  store i32 %ADD_ASSIGN6, i32* %i, align 4
  br label %cond

then8:                                            ; preds = %endwhile
  %old11 = load i32, i32* %n, align 4
  %ADD_ASSIGN12 = add i32 %old11, 10
  store i32 %ADD_ASSIGN12, i32* %n, align 4
  br label %endif7

endif7:                                           ; preds = %then8, %endwhile
  %load.n13 = load i32, i32* %n, align 4
  ret i32 %load.n13
}
```

---

## 032 — program: count trues with as, then a cast that wraps

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
  %array = alloca [4 x i1], align 1
  %n = alloca i32, align 4
  %i = alloca i32, align 4
  %big = alloca i32, align 4
  %e = getelementptr inbounds [4 x i1], [4 x i1]* %array, i32 0, i32 0
  store i1 true, i1* %e, align 1
  %e1 = getelementptr inbounds [4 x i1], [4 x i1]* %array, i32 0, i32 1
  store i1 false, i1* %e1, align 1
  %e2 = getelementptr inbounds [4 x i1], [4 x i1]* %array, i32 0, i32 2
  store i1 true, i1* %e2, align 1
  %e3 = getelementptr inbounds [4 x i1], [4 x i1]* %array, i32 0, i32 3
  store i1 true, i1* %e3, align 1
  %array4 = getelementptr inbounds [4 x i1], [4 x i1]* %array, i32 0, i32 0
  store i1* %array4, i1** %flags, align 8
  store i32 0, i32* %n, align 4
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %then, %entry
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 4
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.flags = load i1*, i1** %flags, align 8
  %load.i5 = load i32, i32* %i, align 4
  %"load.flags[load.i]" = getelementptr inbounds i1, i1* %load.flags, i32 %load.i5
  %"load.load.flags[load.i]" = load i1, i1* %"load.flags[load.i]", align 1
  %"(i32)load.load.flags[load.i]" = zext i1 %"load.load.flags[load.i]" to i32
  %old = load i32, i32* %n, align 4
  %ADD_ASSIGN = add i32 %old, %"(i32)load.load.flags[load.i]"
  store i32 %ADD_ASSIGN, i32* %n, align 4
  %old6 = load i32, i32* %i, align 4
  %ADD_ASSIGN7 = add i32 %old6, 1
  store i32 %ADD_ASSIGN7, i32* %i, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  store i32 1000, i32* %big, align 4
  %load.n = load i32, i32* %n, align 4
  %MUL = mul i32 %load.n, 10
  %load.big = load i32, i32* %big, align 4
  %"(i8)load.big" = trunc i32 %load.big to i8
  %"(i32)(i8)load.big" = sext i8 %"(i8)load.big" to i32
  %ADD = add i32 %MUL, %"(i32)(i8)load.big"
  ret i32 %ADD
}
```

---

## 033 — program: caesar cipher: char arithmetic with as

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

@str = private constant [13 x i8] c"hello, world\00"
@str.1 = private constant [6 x i8] c"%s | \00"
@str.2 = private constant [4 x i8] c"%s\0A\00"

declare i32 @printf(i8*, ...)

define i8 @shift(i8 %0, i32 %1) {
entry:
  %c = alloca i8, align 1
  %k = alloca i32, align 4
  %n = alloca i32, align 4
  store i8 %0, i8* %c, align 1
  store i32 %1, i32* %k, align 4
  %load.c = load i8, i8* %c, align 1
  %LT = icmp slt i8 %load.c, 97
  %load.c1 = load i8, i8* %c, align 1
  %GT = icmp sgt i8 %load.c1, 122
  %OR = or i1 %LT, %GT
  br i1 %OR, label %then, label %endif

then:                                             ; preds = %entry
  %load.c2 = load i8, i8* %c, align 1
  ret i8 %load.c2

endif:                                            ; preds = %entry
  %load.c3 = load i8, i8* %c, align 1
  %"(i32)load.c" = sext i8 %load.c3 to i32
  %SUB = sub i32 %"(i32)load.c", 97
  store i32 %SUB, i32* %n, align 4
  %load.n = load i32, i32* %n, align 4
  %load.k = load i32, i32* %k, align 4
  %ADD = add i32 %load.n, %load.k
  %MOD = srem i32 %ADD, 26
  store i32 %MOD, i32* %n, align 4
  %load.n4 = load i32, i32* %n, align 4
  %ADD5 = add i32 %load.n4, 97
  %"(i8)load.n + (i32)'a'" = trunc i32 %ADD5 to i8
  ret i8 %"(i8)load.n + (i32)'a'"
}

define void @encode(i8* %0, i32 %1) {
entry:
  %s = alloca i8*, align 8
  %k = alloca i32, align 4
  %i = alloca i32, align 4
  store i8* %0, i8** %s, align 8
  store i32 %1, i32* %k, align 4
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %then, %entry
  %load.s = load i8*, i8** %s, align 8
  %load.i = load i32, i32* %i, align 4
  %"load.s[load.i]" = getelementptr inbounds i8, i8* %load.s, i32 %load.i
  %"load.load.s[load.i]" = load i8, i8* %"load.s[load.i]", align 1
  %NQ = icmp ne i8 %"load.load.s[load.i]", 0
  br i1 %NQ, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.s1 = load i8*, i8** %s, align 8
  %load.i2 = load i32, i32* %i, align 4
  %"load.s[load.i]3" = getelementptr inbounds i8, i8* %load.s1, i32 %load.i2
  %load.s4 = load i8*, i8** %s, align 8
  %load.i5 = load i32, i32* %i, align 4
  %"load.s[load.i]6" = getelementptr inbounds i8, i8* %load.s4, i32 %load.i5
  %"load.load.s[load.i]7" = load i8, i8* %"load.s[load.i]6", align 1
  %load.k = load i32, i32* %k, align 4
  %"shift(load.load.s[load.i],load.k)" = call i8 @shift(i8 %"load.load.s[load.i]7", i32 %load.k)
  store i8 %"shift(load.load.s[load.i],load.k)", i8* %"load.s[load.i]3", align 1
  %old = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  ret void
}

define i32 @main() {
entry:
  %s = alloca i8*, align 8
  %s1 = alloca [13 x i8], align 1
  %s2 = alloca [6 x i8], align 1
  %s6 = alloca [4 x i8], align 1
  %dst = getelementptr inbounds [13 x i8], [13 x i8]* %s1, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([13 x i8], [13 x i8]* @str, i32 0, i32 0), i64 13, i1 false)
  store i8* %dst, i8** %s, align 8
  %load.s = load i8*, i8** %s, align 8
  call void @encode(i8* %load.s, i32 3)
  %dst3 = getelementptr inbounds [6 x i8], [6 x i8]* %s2, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst3, i8* align 1 getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i64 6, i1 false)
  %load.s4 = load i8*, i8** %s, align 8
  %"printf(str,load.s)" = call i32 (i8*, ...) @printf(i8* %dst3, i8* %load.s4)
  %load.s5 = load i8*, i8** %s, align 8
  call void @encode(i8* %load.s5, i32 23)
  %dst7 = getelementptr inbounds [4 x i8], [4 x i8]* %s6, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst7, i8* align 1 getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i64 4, i1 false)
  %load.s8 = load i8*, i8** %s, align 8
  %"printf(str,load.s)9" = call i32 (i8*, ...) @printf(i8* %dst7, i8* %load.s8)
  %load.s10 = load i8*, i8** %s, align 8
  %"load.s[0]" = getelementptr inbounds i8, i8* %load.s10, i32 0
  %"load.load.s[0]" = load i8, i8* %"load.s[0]", align 1
  %"(i32)load.load.s[0]" = sext i8 %"load.load.s[0]" to i32
  ret i32 %"(i32)load.load.s[0]"
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 034 — program: print the alphabet with a char counter

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

@str = private constant [3 x i8] c"%c\00"
@str.1 = private constant [2 x i8] c"\0A\00"

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %c = alloca i8, align 1
  %s = alloca [3 x i8], align 1
  %s3 = alloca [2 x i8], align 1
  store i8 97, i8* %c, align 1
  br label %cond

cond:                                             ; preds = %then, %entry
  %load.c = load i8, i8* %c, align 1
  %LE = icmp sle i8 %load.c, 101
  br i1 %LE, label %then, label %endwhile

then:                                             ; preds = %cond
  %dst = getelementptr inbounds [3 x i8], [3 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i64 3, i1 false)
  %load.c1 = load i8, i8* %c, align 1
  %"printf(str,load.c)" = call i32 (i8*, ...) @printf(i8* %dst, i8 %load.c1)
  %load.c2 = load i8, i8* %c, align 1
  %"(i32)load.c" = sext i8 %load.c2 to i32
  %ADD = add i32 %"(i32)load.c", 1
  %"(i8)(i32)load.c + 1" = trunc i32 %ADD to i8
  store i8 %"(i8)(i32)load.c + 1", i8* %c, align 1
  br label %cond

endwhile:                                         ; preds = %cond
  %dst4 = getelementptr inbounds [2 x i8], [2 x i8]* %s3, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst4, i8* align 1 getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i64 2, i1 false)
  %"printf(str)" = call i32 (i8*, ...) @printf(i8* %dst4)
  %load.c5 = load i8, i8* %c, align 1
  %"(i32)load.c6" = sext i8 %load.c5 to i32
  ret i32 %"(i32)load.c6"
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 035 — program: isNullable on array elements and chars

```ura
// expect: 33   (3 zeros * 10, plus 1, plus 2)
fn count_zero(a i32[], n i32) i32:
    c i32 = 0
    i i32 = 0
    while i < n:
        if isNullable(a[i]): c += 1
        i += 1
    return c

fn main() i32:
    a i32[] = [0, 5, 0, 0, 7]
    s i8[] = "ab"
    r i32 = count_zero(a, 5) * 10
    if isNullable(s[2]): r += 1
    if !isNullable(s[0]): r += 2
    return r
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private constant [3 x i8] c"ab\00"

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
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.i = load i32, i32* %i, align 4
  %load.n = load i32, i32* %n, align 4
  %LT = icmp slt i32 %load.i, %load.n
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.a = load i32*, i32** %a, align 8
  %load.i2 = load i32, i32* %i, align 4
  %"load.a[load.i]" = getelementptr inbounds i32, i32* %load.a, i32 %load.i2
  %"load.load.a[load.i]" = load i32, i32* %"load.a[load.i]", align 4
  %"isNullable(load.load.a[load.i])" = icmp eq i32 %"load.load.a[load.i]", 0
  br i1 %"isNullable(load.load.a[load.i])", label %then1, label %endif

endwhile:                                         ; preds = %cond
  %load.c = load i32, i32* %c, align 4
  ret i32 %load.c

then1:                                            ; preds = %then
  %old = load i32, i32* %c, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %c, align 4
  br label %endif

endif:                                            ; preds = %then1, %then
  %old3 = load i32, i32* %i, align 4
  %ADD_ASSIGN4 = add i32 %old3, 1
  store i32 %ADD_ASSIGN4, i32* %i, align 4
  br label %cond
}

define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %array = alloca [5 x i32], align 4
  %s = alloca i8*, align 8
  %s6 = alloca [3 x i8], align 1
  %r = alloca i32, align 4
  %e = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 0
  store i32 0, i32* %e, align 4
  %e1 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 1
  store i32 5, i32* %e1, align 4
  %e2 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 2
  store i32 0, i32* %e2, align 4
  %e3 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 3
  store i32 0, i32* %e3, align 4
  %e4 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 4
  store i32 7, i32* %e4, align 4
  %array5 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 0
  store i32* %array5, i32** %a, align 8
  %dst = getelementptr inbounds [3 x i8], [3 x i8]* %s6, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i64 3, i1 false)
  store i8* %dst, i8** %s, align 8
  %load.a = load i32*, i32** %a, align 8
  %"count_zero(load.a,5)" = call i32 @count_zero(i32* %load.a, i32 5)
  %MUL = mul i32 %"count_zero(load.a,5)", 10
  store i32 %MUL, i32* %r, align 4
  %load.s = load i8*, i8** %s, align 8
  %"load.s[2]" = getelementptr inbounds i8, i8* %load.s, i32 2
  %"load.load.s[2]" = load i8, i8* %"load.s[2]", align 1
  %"isNullable(load.load.s[2])" = icmp eq i8 %"load.load.s[2]", 0
  br i1 %"isNullable(load.load.s[2])", label %then, label %endif

then:                                             ; preds = %entry
  %old = load i32, i32* %r, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %r, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %load.s9 = load i8*, i8** %s, align 8
  %"load.s[0]" = getelementptr inbounds i8, i8* %load.s9, i32 0
  %"load.load.s[0]" = load i8, i8* %"load.s[0]", align 1
  %"isNullable(load.load.s[0])" = icmp eq i8 %"load.load.s[0]", 0
  %NOT = xor i1 %"isNullable(load.load.s[0])", true
  br i1 %NOT, label %then8, label %endif7

then8:                                            ; preds = %endif
  %old10 = load i32, i32* %r, align 4
  %ADD_ASSIGN11 = add i32 %old10, 2
  store i32 %ADD_ASSIGN11, i32* %r, align 4
  br label %endif7

endif7:                                           ; preds = %then8, %endif
  %load.r = load i32, i32* %r, align 4
  ret i32 %load.r
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 036 — program: transpose a 3x3 matrix stored flat

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
  %load.m = load i32*, i32** %m, align 8
  %load.r = load i32, i32* %r, align 4
  %load.w = load i32, i32* %w, align 4
  %MUL = mul i32 %load.r, %load.w
  %load.c = load i32, i32* %c, align 4
  %ADD = add i32 %MUL, %load.c
  %"load.m[load.r * load.w + load.c]" = getelementptr inbounds i32, i32* %load.m, i32 %ADD
  %"load.load.m[load.r * load.w + load.c]" = load i32, i32* %"load.m[load.r * load.w + load.c]", align 4
  ret i32 %"load.load.m[load.r * load.w + load.c]"
}

define i32 @main() {
entry:
  %m = alloca i32*, align 8
  %array = alloca [9 x i32], align 4
  %t = alloca i32*, align 8
  %array10 = alloca [9 x i32], align 4
  %r = alloca i32, align 4
  %c = alloca i32, align 4
  %trace = alloca i32, align 4
  %i = alloca i32, align 4
  %e = getelementptr inbounds [9 x i32], [9 x i32]* %array, i32 0, i32 0
  store i32 1, i32* %e, align 4
  %e1 = getelementptr inbounds [9 x i32], [9 x i32]* %array, i32 0, i32 1
  store i32 2, i32* %e1, align 4
  %e2 = getelementptr inbounds [9 x i32], [9 x i32]* %array, i32 0, i32 2
  store i32 3, i32* %e2, align 4
  %e3 = getelementptr inbounds [9 x i32], [9 x i32]* %array, i32 0, i32 3
  store i32 4, i32* %e3, align 4
  %e4 = getelementptr inbounds [9 x i32], [9 x i32]* %array, i32 0, i32 4
  store i32 5, i32* %e4, align 4
  %e5 = getelementptr inbounds [9 x i32], [9 x i32]* %array, i32 0, i32 5
  store i32 6, i32* %e5, align 4
  %e6 = getelementptr inbounds [9 x i32], [9 x i32]* %array, i32 0, i32 6
  store i32 7, i32* %e6, align 4
  %e7 = getelementptr inbounds [9 x i32], [9 x i32]* %array, i32 0, i32 7
  store i32 8, i32* %e7, align 4
  %e8 = getelementptr inbounds [9 x i32], [9 x i32]* %array, i32 0, i32 8
  store i32 9, i32* %e8, align 4
  %array9 = getelementptr inbounds [9 x i32], [9 x i32]* %array, i32 0, i32 0
  store i32* %array9, i32** %m, align 8
  %e11 = getelementptr inbounds [9 x i32], [9 x i32]* %array10, i32 0, i32 0
  store i32 0, i32* %e11, align 4
  %e12 = getelementptr inbounds [9 x i32], [9 x i32]* %array10, i32 0, i32 1
  store i32 0, i32* %e12, align 4
  %e13 = getelementptr inbounds [9 x i32], [9 x i32]* %array10, i32 0, i32 2
  store i32 0, i32* %e13, align 4
  %e14 = getelementptr inbounds [9 x i32], [9 x i32]* %array10, i32 0, i32 3
  store i32 0, i32* %e14, align 4
  %e15 = getelementptr inbounds [9 x i32], [9 x i32]* %array10, i32 0, i32 4
  store i32 0, i32* %e15, align 4
  %e16 = getelementptr inbounds [9 x i32], [9 x i32]* %array10, i32 0, i32 5
  store i32 0, i32* %e16, align 4
  %e17 = getelementptr inbounds [9 x i32], [9 x i32]* %array10, i32 0, i32 6
  store i32 0, i32* %e17, align 4
  %e18 = getelementptr inbounds [9 x i32], [9 x i32]* %array10, i32 0, i32 7
  store i32 0, i32* %e18, align 4
  %e19 = getelementptr inbounds [9 x i32], [9 x i32]* %array10, i32 0, i32 8
  store i32 0, i32* %e19, align 4
  %array20 = getelementptr inbounds [9 x i32], [9 x i32]* %array10, i32 0, i32 0
  store i32* %array20, i32** %t, align 8
  store i32 0, i32* %r, align 4
  br label %cond

cond:                                             ; preds = %endwhile23, %entry
  %load.r = load i32, i32* %r, align 4
  %LT = icmp slt i32 %load.r, 3
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  store i32 0, i32* %c, align 4
  br label %cond21

endwhile:                                         ; preds = %cond
  store i32 0, i32* %trace, align 4
  store i32 0, i32* %i, align 4
  br label %cond31

cond21:                                           ; preds = %then22, %then
  %load.c = load i32, i32* %c, align 4
  %LT24 = icmp slt i32 %load.c, 3
  br i1 %LT24, label %then22, label %endwhile23

then22:                                           ; preds = %cond21
  %load.t = load i32*, i32** %t, align 8
  %load.c25 = load i32, i32* %c, align 4
  %MUL = mul i32 %load.c25, 3
  %load.r26 = load i32, i32* %r, align 4
  %ADD = add i32 %MUL, %load.r26
  %"load.t[load.c * 3 + load.r]" = getelementptr inbounds i32, i32* %load.t, i32 %ADD
  %load.m = load i32*, i32** %m, align 8
  %load.r27 = load i32, i32* %r, align 4
  %load.c28 = load i32, i32* %c, align 4
  %"get(load.m,3,load.r,load.c)" = call i32 @get(i32* %load.m, i32 3, i32 %load.r27, i32 %load.c28)
  store i32 %"get(load.m,3,load.r,load.c)", i32* %"load.t[load.c * 3 + load.r]", align 4
  %old = load i32, i32* %c, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %c, align 4
  br label %cond21

endwhile23:                                       ; preds = %cond21
  %old29 = load i32, i32* %r, align 4
  %ADD_ASSIGN30 = add i32 %old29, 1
  store i32 %ADD_ASSIGN30, i32* %r, align 4
  br label %cond

cond31:                                           ; preds = %then32, %endwhile
  %load.i = load i32, i32* %i, align 4
  %LT34 = icmp slt i32 %load.i, 3
  br i1 %LT34, label %then32, label %endwhile33

then32:                                           ; preds = %cond31
  %load.t35 = load i32*, i32** %t, align 8
  %load.i36 = load i32, i32* %i, align 4
  %MUL37 = mul i32 %load.i36, 3
  %load.i38 = load i32, i32* %i, align 4
  %ADD39 = add i32 %MUL37, %load.i38
  %"load.t[load.i * 3 + load.i]" = getelementptr inbounds i32, i32* %load.t35, i32 %ADD39
  %"load.load.t[load.i * 3 + load.i]" = load i32, i32* %"load.t[load.i * 3 + load.i]", align 4
  %old40 = load i32, i32* %trace, align 4
  %ADD_ASSIGN41 = add i32 %old40, %"load.load.t[load.i * 3 + load.i]"
  store i32 %ADD_ASSIGN41, i32* %trace, align 4
  %old42 = load i32, i32* %i, align 4
  %ADD_ASSIGN43 = add i32 %old42, 1
  store i32 %ADD_ASSIGN43, i32* %i, align 4
  br label %cond31

endwhile33:                                       ; preds = %cond31
  %load.trace = load i32, i32* %trace, align 4
  %MUL44 = mul i32 %load.trace, 10
  %load.t45 = load i32*, i32** %t, align 8
  %"load.t[1]" = getelementptr inbounds i32, i32* %load.t45, i32 1
  %"load.load.t[1]" = load i32, i32* %"load.t[1]", align 4
  %ADD46 = add i32 %MUL44, %"load.load.t[1]"
  ret i32 %ADD46
}
```

---

## 037 — program: a ref to an array element

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
  %dref.p = load i32*, i32** %p, align 8
  %old = load i32, i32* %dref.p, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %dref.p, align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %array = alloca [3 x i32], align 4
  %p = alloca i32*, align 8
  %e = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32 10, i32* %e, align 4
  %e1 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 1
  store i32 20, i32* %e1, align 4
  %e2 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 2
  store i32 30, i32* %e2, align 4
  %array3 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32* %array3, i32** %a, align 8
  %load.a = load i32*, i32** %a, align 8
  %"load.a[1]" = getelementptr inbounds i32, i32* %load.a, i32 1
  call void @inc(i32* %"load.a[1]")
  %load.a4 = load i32*, i32** %a, align 8
  %"load.a[1]5" = getelementptr inbounds i32, i32* %load.a4, i32 1
  call void @inc(i32* %"load.a[1]5")
  %load.a6 = load i32*, i32** %a, align 8
  %"load.a[2]" = getelementptr inbounds i32, i32* %load.a6, i32 2
  store i32* %"load.a[2]", i32** %p, align 8
  %dref.p = load i32*, i32** %p, align 8
  %old = load i32, i32* %dref.p, align 4
  %MUL_ASSIGN = mul i32 %old, 2
  store i32 %MUL_ASSIGN, i32* %dref.p, align 4
  %load.a7 = load i32*, i32** %a, align 8
  %"load.a[0]" = getelementptr inbounds i32, i32* %load.a7, i32 0
  %"load.load.a[0]" = load i32, i32* %"load.a[0]", align 4
  %load.a8 = load i32*, i32** %a, align 8
  %"load.a[1]9" = getelementptr inbounds i32, i32* %load.a8, i32 1
  %"load.load.a[1]" = load i32, i32* %"load.a[1]9", align 4
  %ADD = add i32 %"load.load.a[0]", %"load.load.a[1]"
  %load.a10 = load i32*, i32** %a, align 8
  %"load.a[2]11" = getelementptr inbounds i32, i32* %load.a10, i32 2
  %"load.load.a[2]" = load i32, i32* %"load.a[2]11", align 4
  %ADD12 = add i32 %ADD, %"load.load.a[2]"
  ret i32 %ADD12
}
```

---

## 038 — program: an array literal made of calls

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
  %load.x = load i32, i32* %x, align 4
  %load.x1 = load i32, i32* %x, align 4
  %MUL = mul i32 %load.x, %load.x1
  ret i32 %MUL
}

define i32 @main() {
entry:
  %k = alloca i32, align 4
  %a = alloca i32*, align 8
  %array = alloca [5 x i32], align 4
  store i32 2, i32* %k, align 4
  %"sq(1)" = call i32 @sq(i32 1)
  %load.k = load i32, i32* %k, align 4
  %"sq(load.k)" = call i32 @sq(i32 %load.k)
  %load.k1 = load i32, i32* %k, align 4
  %ADD = add i32 %load.k1, 1
  %"sq(load.k + 1)" = call i32 @sq(i32 %ADD)
  %load.k2 = load i32, i32* %k, align 4
  %MUL = mul i32 %load.k2, 10
  %load.k3 = load i32, i32* %k, align 4
  %"sq(load.k)4" = call i32 @sq(i32 %load.k3)
  %"sq(sq(load.k))" = call i32 @sq(i32 %"sq(load.k)4")
  %e = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 0
  store i32 %"sq(1)", i32* %e, align 4
  %e5 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 1
  store i32 %"sq(load.k)", i32* %e5, align 4
  %e6 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 2
  store i32 %"sq(load.k + 1)", i32* %e6, align 4
  %e7 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 3
  store i32 %MUL, i32* %e7, align 4
  %e8 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 4
  store i32 %"sq(sq(load.k))", i32* %e8, align 4
  %array9 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 0
  store i32* %array9, i32** %a, align 8
  %load.a = load i32*, i32** %a, align 8
  %"load.a[0]" = getelementptr inbounds i32, i32* %load.a, i32 0
  %"load.load.a[0]" = load i32, i32* %"load.a[0]", align 4
  %load.a10 = load i32*, i32** %a, align 8
  %"load.a[1]" = getelementptr inbounds i32, i32* %load.a10, i32 1
  %"load.load.a[1]" = load i32, i32* %"load.a[1]", align 4
  %ADD11 = add i32 %"load.load.a[0]", %"load.load.a[1]"
  %load.a12 = load i32*, i32** %a, align 8
  %"load.a[2]" = getelementptr inbounds i32, i32* %load.a12, i32 2
  %"load.load.a[2]" = load i32, i32* %"load.a[2]", align 4
  %ADD13 = add i32 %ADD11, %"load.load.a[2]"
  %load.a14 = load i32*, i32** %a, align 8
  %"load.a[3]" = getelementptr inbounds i32, i32* %load.a14, i32 3
  %"load.load.a[3]" = load i32, i32* %"load.a[3]", align 4
  %ADD15 = add i32 %ADD13, %"load.load.a[3]"
  %load.a16 = load i32*, i32** %a, align 8
  %"load.a[4]" = getelementptr inbounds i32, i32* %load.a16, i32 4
  %"load.load.a[4]" = load i32, i32* %"load.a[4]", align 4
  %ADD17 = add i32 %ADD15, %"load.load.a[4]"
  ret i32 %ADD17
}
```

---

## 039 — program: elif chain on one line each, with not / and / or

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
  %load.n = load i32, i32* %n, align 4
  %GE = icmp sge i32 %load.n, 90
  br i1 %GE, label %then, label %next

then:                                             ; preds = %entry
  ret i8 65

next:                                             ; preds = %entry
  %load.n3 = load i32, i32* %n, align 4
  %GE4 = icmp sge i32 %load.n3, 80
  %load.n5 = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %load.n5, 85
  %NOT = xor i1 %EQ, true
  %AND = and i1 %GE4, %NOT
  br i1 %AND, label %then1, label %next2

then1:                                            ; preds = %next
  ret i8 66

next2:                                            ; preds = %next
  %load.n8 = load i32, i32* %n, align 4
  %EQ9 = icmp eq i32 %load.n8, 85
  %load.n10 = load i32, i32* %n, align 4
  %GE11 = icmp sge i32 %load.n10, 70
  %OR = or i1 %EQ9, %GE11
  br i1 %OR, label %then6, label %next7

then6:                                            ; preds = %next2
  ret i8 67

next7:                                            ; preds = %next2
  ret i8 70

endif:                                            ; No predecessors!
  ret i8 0
}

define i32 @main() {
entry:
  %r = alloca i32, align 4
  store i32 0, i32* %r, align 4
  %"grade(95)" = call i8 @grade(i32 95)
  %EQ = icmp eq i8 %"grade(95)", 65
  br i1 %EQ, label %then, label %endif

then:                                             ; preds = %entry
  %old = load i32, i32* %r, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %r, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %"grade(82)" = call i8 @grade(i32 82)
  %EQ3 = icmp eq i8 %"grade(82)", 66
  br i1 %EQ3, label %then2, label %endif1

then2:                                            ; preds = %endif
  %old4 = load i32, i32* %r, align 4
  %ADD_ASSIGN5 = add i32 %old4, 2
  store i32 %ADD_ASSIGN5, i32* %r, align 4
  br label %endif1

endif1:                                           ; preds = %then2, %endif
  %"grade(85)" = call i8 @grade(i32 85)
  %EQ8 = icmp eq i8 %"grade(85)", 67
  br i1 %EQ8, label %then7, label %endif6

then7:                                            ; preds = %endif1
  %old9 = load i32, i32* %r, align 4
  %ADD_ASSIGN10 = add i32 %old9, 4
  store i32 %ADD_ASSIGN10, i32* %r, align 4
  br label %endif6

endif6:                                           ; preds = %then7, %endif1
  %"grade(72)" = call i8 @grade(i32 72)
  %EQ13 = icmp eq i8 %"grade(72)", 67
  br i1 %EQ13, label %then12, label %endif11

then12:                                           ; preds = %endif6
  %old14 = load i32, i32* %r, align 4
  %ADD_ASSIGN15 = add i32 %old14, 8
  store i32 %ADD_ASSIGN15, i32* %r, align 4
  br label %endif11

endif11:                                          ; preds = %then12, %endif6
  %"grade(50)" = call i8 @grade(i32 50)
  %EQ18 = icmp eq i8 %"grade(50)", 70
  br i1 %EQ18, label %then17, label %endif16

then17:                                           ; preds = %endif11
  %old19 = load i32, i32* %r, align 4
  %ADD_ASSIGN20 = add i32 %old19, 16
  store i32 %ADD_ASSIGN20, i32* %r, align 4
  br label %endif16

endif16:                                          ; preds = %then17, %endif11
  %load.r = load i32, i32* %r, align 4
  ret i32 %load.r
}
```

---

## 040 — program: reverse a string, palindromes, a while on one line

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

@str = private constant [8 x i8] c"racecar\00"
@str.1 = private constant [9 x i8] c"ura lang\00"
@str.2 = private constant [4 x i8] c"%s\0A\00"

define i32 @len(i8* %0) {
entry:
  %s = alloca i8*, align 8
  %n = alloca i32, align 4
  store i8* %0, i8** %s, align 8
  store i32 0, i32* %n, align 4
  br label %cond

cond:                                             ; preds = %then, %entry
  %load.s = load i8*, i8** %s, align 8
  %load.n = load i32, i32* %n, align 4
  %"load.s[load.n]" = getelementptr inbounds i8, i8* %load.s, i32 %load.n
  %"load.load.s[load.n]" = load i8, i8* %"load.s[load.n]", align 1
  %NQ = icmp ne i8 %"load.load.s[load.n]", 0
  br i1 %NQ, label %then, label %endwhile

then:                                             ; preds = %cond
  %old = load i32, i32* %n, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %n, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  %load.n1 = load i32, i32* %n, align 4
  ret i32 %load.n1
}

define i1 @is_palindrome(i8* %0) {
entry:
  %s = alloca i8*, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  store i8* %0, i8** %s, align 8
  store i32 0, i32* %i, align 4
  %load.s = load i8*, i8** %s, align 8
  %"len(load.s)" = call i32 @len(i8* %load.s)
  %SUB = sub i32 %"len(load.s)", 1
  store i32 %SUB, i32* %j, align 4
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.i = load i32, i32* %i, align 4
  %load.j = load i32, i32* %j, align 4
  %LT = icmp slt i32 %load.i, %load.j
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.s2 = load i8*, i8** %s, align 8
  %load.i3 = load i32, i32* %i, align 4
  %"load.s[load.i]" = getelementptr inbounds i8, i8* %load.s2, i32 %load.i3
  %"load.load.s[load.i]" = load i8, i8* %"load.s[load.i]", align 1
  %load.s4 = load i8*, i8** %s, align 8
  %load.j5 = load i32, i32* %j, align 4
  %"load.s[load.j]" = getelementptr inbounds i8, i8* %load.s4, i32 %load.j5
  %"load.load.s[load.j]" = load i8, i8* %"load.s[load.j]", align 1
  %NQ = icmp ne i8 %"load.load.s[load.i]", %"load.load.s[load.j]"
  br i1 %NQ, label %then1, label %endif

endwhile:                                         ; preds = %cond
  ret i1 true

then1:                                            ; preds = %then
  ret i1 false

endif:                                            ; preds = %then
  %old = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  %old6 = load i32, i32* %j, align 4
  %SUB_ASSIGN = sub i32 %old6, 1
  store i32 %SUB_ASSIGN, i32* %j, align 4
  br label %cond
}

define void @reverse(i8* %0) {
entry:
  %s = alloca i8*, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %t = alloca i8, align 1
  store i8* %0, i8** %s, align 8
  store i32 0, i32* %i, align 4
  %load.s = load i8*, i8** %s, align 8
  %"len(load.s)" = call i32 @len(i8* %load.s)
  %SUB = sub i32 %"len(load.s)", 1
  store i32 %SUB, i32* %j, align 4
  br label %cond

cond:                                             ; preds = %then, %entry
  %load.i = load i32, i32* %i, align 4
  %load.j = load i32, i32* %j, align 4
  %LT = icmp slt i32 %load.i, %load.j
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.s1 = load i8*, i8** %s, align 8
  %load.i2 = load i32, i32* %i, align 4
  %"load.s[load.i]" = getelementptr inbounds i8, i8* %load.s1, i32 %load.i2
  %"load.load.s[load.i]" = load i8, i8* %"load.s[load.i]", align 1
  store i8 %"load.load.s[load.i]", i8* %t, align 1
  %load.s3 = load i8*, i8** %s, align 8
  %load.i4 = load i32, i32* %i, align 4
  %"load.s[load.i]5" = getelementptr inbounds i8, i8* %load.s3, i32 %load.i4
  %load.s6 = load i8*, i8** %s, align 8
  %load.j7 = load i32, i32* %j, align 4
  %"load.s[load.j]" = getelementptr inbounds i8, i8* %load.s6, i32 %load.j7
  %"load.load.s[load.j]" = load i8, i8* %"load.s[load.j]", align 1
  store i8 %"load.load.s[load.j]", i8* %"load.s[load.i]5", align 1
  %load.s8 = load i8*, i8** %s, align 8
  %load.j9 = load i32, i32* %j, align 4
  %"load.s[load.j]10" = getelementptr inbounds i8, i8* %load.s8, i32 %load.j9
  %load.t = load i8, i8* %t, align 1
  store i8 %load.t, i8* %"load.s[load.j]10", align 1
  %old = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  %old11 = load i32, i32* %j, align 4
  %SUB_ASSIGN = sub i32 %old11, 1
  store i32 %SUB_ASSIGN, i32* %j, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  ret void
}

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %a = alloca i8*, align 8
  %s = alloca [8 x i8], align 1
  %b = alloca i8*, align 8
  %s1 = alloca [9 x i8], align 1
  %s3 = alloca [4 x i8], align 1
  %r = alloca i32, align 4
  %dst = getelementptr inbounds [8 x i8], [8 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([8 x i8], [8 x i8]* @str, i32 0, i32 0), i64 8, i1 false)
  store i8* %dst, i8** %a, align 8
  %dst2 = getelementptr inbounds [9 x i8], [9 x i8]* %s1, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst2, i8* align 1 getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0), i64 9, i1 false)
  store i8* %dst2, i8** %b, align 8
  %load.b = load i8*, i8** %b, align 8
  call void @reverse(i8* %load.b)
  %dst4 = getelementptr inbounds [4 x i8], [4 x i8]* %s3, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst4, i8* align 1 getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i64 4, i1 false)
  %load.b5 = load i8*, i8** %b, align 8
  %"printf(str,load.b)" = call i32 (i8*, ...) @printf(i8* %dst4, i8* %load.b5)
  store i32 0, i32* %r, align 4
  %load.a = load i8*, i8** %a, align 8
  %"is_palindrome(load.a)" = call i1 @is_palindrome(i8* %load.a)
  br i1 %"is_palindrome(load.a)", label %then, label %endif

then:                                             ; preds = %entry
  %old = load i32, i32* %r, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %r, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %load.b8 = load i8*, i8** %b, align 8
  %"is_palindrome(load.b)" = call i1 @is_palindrome(i8* %load.b8)
  %NOT = xor i1 %"is_palindrome(load.b)", true
  br i1 %NOT, label %then7, label %endif6

then7:                                            ; preds = %endif
  %old9 = load i32, i32* %r, align 4
  %ADD_ASSIGN10 = add i32 %old9, 2
  store i32 %ADD_ASSIGN10, i32* %r, align 4
  br label %endif6

endif6:                                           ; preds = %then7, %endif
  %load.r = load i32, i32* %r, align 4
  %load.b11 = load i8*, i8** %b, align 8
  %"len(load.b)" = call i32 @len(i8* %load.b11)
  %MUL = mul i32 %"len(load.b)", 10
  %ADD = add i32 %load.r, %MUL
  ret i32 %ADD
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 041 — program: sieve of eratosthenes on a bool array

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
  %array = alloca [20 x i1], align 1
  %sum = alloca i32, align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %e = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 0
  store i1 false, i1* %e, align 1
  %e1 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 1
  store i1 false, i1* %e1, align 1
  %e2 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 2
  store i1 false, i1* %e2, align 1
  %e3 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 3
  store i1 false, i1* %e3, align 1
  %e4 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 4
  store i1 false, i1* %e4, align 1
  %e5 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 5
  store i1 false, i1* %e5, align 1
  %e6 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 6
  store i1 false, i1* %e6, align 1
  %e7 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 7
  store i1 false, i1* %e7, align 1
  %e8 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 8
  store i1 false, i1* %e8, align 1
  %e9 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 9
  store i1 false, i1* %e9, align 1
  %e10 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 10
  store i1 false, i1* %e10, align 1
  %e11 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 11
  store i1 false, i1* %e11, align 1
  %e12 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 12
  store i1 false, i1* %e12, align 1
  %e13 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 13
  store i1 false, i1* %e13, align 1
  %e14 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 14
  store i1 false, i1* %e14, align 1
  %e15 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 15
  store i1 false, i1* %e15, align 1
  %e16 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 16
  store i1 false, i1* %e16, align 1
  %e17 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 17
  store i1 false, i1* %e17, align 1
  %e18 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 18
  store i1 false, i1* %e18, align 1
  %e19 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 19
  store i1 false, i1* %e19, align 1
  %array20 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 0
  store i1* %array20, i1** %composite, align 8
  store i32 0, i32* %sum, align 4
  store i32 2, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 20
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.composite = load i1*, i1** %composite, align 8
  %load.i22 = load i32, i32* %i, align 4
  %"load.composite[load.i]" = getelementptr inbounds i1, i1* %load.composite, i32 %load.i22
  %"load.load.composite[load.i]" = load i1, i1* %"load.composite[load.i]", align 1
  %NOT = xor i1 %"load.load.composite[load.i]", true
  br i1 %NOT, label %then21, label %endif

endwhile:                                         ; preds = %cond
  %load.sum = load i32, i32* %sum, align 4
  ret i32 %load.sum

then21:                                           ; preds = %then
  %load.i23 = load i32, i32* %i, align 4
  %old = load i32, i32* %sum, align 4
  %ADD_ASSIGN = add i32 %old, %load.i23
  store i32 %ADD_ASSIGN, i32* %sum, align 4
  %load.i24 = load i32, i32* %i, align 4
  %load.i25 = load i32, i32* %i, align 4
  %MUL = mul i32 %load.i24, %load.i25
  store i32 %MUL, i32* %j, align 4
  br label %cond26

cond26:                                           ; preds = %then27, %then21
  %load.j = load i32, i32* %j, align 4
  %LT29 = icmp slt i32 %load.j, 20
  br i1 %LT29, label %then27, label %endwhile28

then27:                                           ; preds = %cond26
  %load.composite30 = load i1*, i1** %composite, align 8
  %load.j31 = load i32, i32* %j, align 4
  %"load.composite[load.j]" = getelementptr inbounds i1, i1* %load.composite30, i32 %load.j31
  store i1 true, i1* %"load.composite[load.j]", align 1
  %load.i32 = load i32, i32* %i, align 4
  %old33 = load i32, i32* %j, align 4
  %ADD_ASSIGN34 = add i32 %old33, %load.i32
  store i32 %ADD_ASSIGN34, i32* %j, align 4
  br label %cond26

endwhile28:                                       ; preds = %cond26
  br label %endif

endif:                                            ; preds = %endwhile28, %then
  %old35 = load i32, i32* %i, align 4
  %ADD_ASSIGN36 = add i32 %old35, 1
  store i32 %ADD_ASSIGN36, i32* %i, align 4
  br label %cond
}
```

---

## 042 — program: atoi: parse numbers from strings

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

@str = private constant [4 x i8] c"123\00"
@str.1 = private constant [4 x i8] c"-45\00"
@str.2 = private constant [3 x i8] c"7x\00"

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
  %load.s = load i8*, i8** %s, align 8
  %"load.s[0]" = getelementptr inbounds i8, i8* %load.s, i32 0
  %"load.load.s[0]" = load i8, i8* %"load.s[0]", align 1
  %EQ = icmp eq i8 %"load.load.s[0]", 45
  br i1 %EQ, label %then, label %endif

then:                                             ; preds = %entry
  store i1 true, i1* %neg, align 1
  store i32 1, i32* %i, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  br label %cond

cond:                                             ; preds = %then1, %endif
  %load.s2 = load i8*, i8** %s, align 8
  %load.i = load i32, i32* %i, align 4
  %"load.s[load.i]" = getelementptr inbounds i8, i8* %load.s2, i32 %load.i
  %"load.load.s[load.i]" = load i8, i8* %"load.s[load.i]", align 1
  %GE = icmp sge i8 %"load.load.s[load.i]", 48
  %load.s3 = load i8*, i8** %s, align 8
  %load.i4 = load i32, i32* %i, align 4
  %"load.s[load.i]5" = getelementptr inbounds i8, i8* %load.s3, i32 %load.i4
  %"load.load.s[load.i]6" = load i8, i8* %"load.s[load.i]5", align 1
  %LE = icmp sle i8 %"load.load.s[load.i]6", 57
  %AND = and i1 %GE, %LE
  br i1 %AND, label %then1, label %endwhile

then1:                                            ; preds = %cond
  %load.n = load i32, i32* %n, align 4
  %MUL = mul i32 %load.n, 10
  %load.s7 = load i8*, i8** %s, align 8
  %load.i8 = load i32, i32* %i, align 4
  %"load.s[load.i]9" = getelementptr inbounds i8, i8* %load.s7, i32 %load.i8
  %"load.load.s[load.i]10" = load i8, i8* %"load.s[load.i]9", align 1
  %"(i32)load.load.s[load.i]" = sext i8 %"load.load.s[load.i]10" to i32
  %ADD = add i32 %MUL, %"(i32)load.load.s[load.i]"
  %SUB = sub i32 %ADD, 48
  store i32 %SUB, i32* %n, align 4
  %old = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  %load.neg = load i1, i1* %neg, align 1
  br i1 %load.neg, label %then12, label %endif11

then12:                                           ; preds = %endwhile
  %load.n13 = load i32, i32* %n, align 4
  %SUB14 = sub i32 0, %load.n13
  ret i32 %SUB14

endif11:                                          ; preds = %endwhile
  %load.n15 = load i32, i32* %n, align 4
  ret i32 %load.n15
}

define i32 @main() {
entry:
  %s = alloca [4 x i8], align 1
  %s1 = alloca [4 x i8], align 1
  %s4 = alloca [3 x i8], align 1
  %dst = getelementptr inbounds [4 x i8], [4 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i64 4, i1 false)
  %"atoi(str)" = call i32 @atoi(i8* %dst)
  %dst2 = getelementptr inbounds [4 x i8], [4 x i8]* %s1, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst2, i8* align 1 getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i64 4, i1 false)
  %"atoi(str)3" = call i32 @atoi(i8* %dst2)
  %ADD = add i32 %"atoi(str)", %"atoi(str)3"
  %dst5 = getelementptr inbounds [3 x i8], [3 x i8]* %s4, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst5, i8* align 1 getelementptr inbounds ([3 x i8], [3 x i8]* @str.2, i32 0, i32 0), i64 3, i1 false)
  %"atoi(str)6" = call i32 @atoi(i8* %dst5)
  %ADD7 = add i32 %ADD, %"atoi(str)6"
  ret i32 %ADD7
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 043 — program: itoa: write a number's digits into a buffer

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

@str = private constant [11 x i8] c"0000000000\00"
@str.1 = private constant [13 x i8] c"............\00"
@str.2 = private constant [4 x i8] c"%s\0A\00"

declare i32 @printf(i8*, ...)

define void @itoa(i32 %0, i8* %1) {
entry:
  %n = alloca i32, align 4
  %out = alloca i8*, align 8
  %digits = alloca i8*, align 8
  %s = alloca [11 x i8], align 1
  %k = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  store i8* %1, i8** %out, align 8
  %dst = getelementptr inbounds [11 x i8], [11 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([11 x i8], [11 x i8]* @str, i32 0, i32 0), i64 11, i1 false)
  store i8* %dst, i8** %digits, align 8
  store i32 0, i32* %k, align 4
  %load.n = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %load.n, 0
  br i1 %EQ, label %then, label %endif

then:                                             ; preds = %entry
  %load.out = load i8*, i8** %out, align 8
  %"load.out[0]" = getelementptr inbounds i8, i8* %load.out, i32 0
  store i8 48, i8* %"load.out[0]", align 1
  %load.out1 = load i8*, i8** %out, align 8
  %"load.out[1]" = getelementptr inbounds i8, i8* %load.out1, i32 1
  store i8 0, i8* %"load.out[1]", align 1
  ret void

endif:                                            ; preds = %entry
  br label %cond

cond:                                             ; preds = %then2, %endif
  %load.n3 = load i32, i32* %n, align 4
  %GT = icmp sgt i32 %load.n3, 0
  br i1 %GT, label %then2, label %endwhile

then2:                                            ; preds = %cond
  %load.digits = load i8*, i8** %digits, align 8
  %load.k = load i32, i32* %k, align 4
  %"load.digits[load.k]" = getelementptr inbounds i8, i8* %load.digits, i32 %load.k
  %load.n4 = load i32, i32* %n, align 4
  %MOD = srem i32 %load.n4, 10
  %ADD = add i32 %MOD, 48
  %"(i8)load.n % 10 + (i32)'0'" = trunc i32 %ADD to i8
  store i8 %"(i8)load.n % 10 + (i32)'0'", i8* %"load.digits[load.k]", align 1
  %old = load i32, i32* %n, align 4
  %DIV_ASSIGN = sdiv i32 %old, 10
  store i32 %DIV_ASSIGN, i32* %n, align 4
  %old5 = load i32, i32* %k, align 4
  %ADD_ASSIGN = add i32 %old5, 1
  store i32 %ADD_ASSIGN, i32* %k, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  store i32 0, i32* %i, align 4
  br label %cond6

cond6:                                            ; preds = %then7, %endwhile
  %load.i = load i32, i32* %i, align 4
  %load.k9 = load i32, i32* %k, align 4
  %LT = icmp slt i32 %load.i, %load.k9
  br i1 %LT, label %then7, label %endwhile8

then7:                                            ; preds = %cond6
  %load.out10 = load i8*, i8** %out, align 8
  %load.i11 = load i32, i32* %i, align 4
  %"load.out[load.i]" = getelementptr inbounds i8, i8* %load.out10, i32 %load.i11
  %load.digits12 = load i8*, i8** %digits, align 8
  %load.k13 = load i32, i32* %k, align 4
  %SUB = sub i32 %load.k13, 1
  %load.i14 = load i32, i32* %i, align 4
  %SUB15 = sub i32 %SUB, %load.i14
  %"load.digits[load.k - 1 - load.i]" = getelementptr inbounds i8, i8* %load.digits12, i32 %SUB15
  %"load.load.digits[load.k - 1 - load.i]" = load i8, i8* %"load.digits[load.k - 1 - load.i]", align 1
  store i8 %"load.load.digits[load.k - 1 - load.i]", i8* %"load.out[load.i]", align 1
  %old16 = load i32, i32* %i, align 4
  %ADD_ASSIGN17 = add i32 %old16, 1
  store i32 %ADD_ASSIGN17, i32* %i, align 4
  br label %cond6

endwhile8:                                        ; preds = %cond6
  %load.out18 = load i8*, i8** %out, align 8
  %load.k19 = load i32, i32* %k, align 4
  %"load.out[load.k]" = getelementptr inbounds i8, i8* %load.out18, i32 %load.k19
  store i8 0, i8* %"load.out[load.k]", align 1
  ret void
}

define i32 @main() {
entry:
  %buf = alloca i8*, align 8
  %s = alloca [13 x i8], align 1
  %s1 = alloca [4 x i8], align 1
  %dst = getelementptr inbounds [13 x i8], [13 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([13 x i8], [13 x i8]* @str.1, i32 0, i32 0), i64 13, i1 false)
  store i8* %dst, i8** %buf, align 8
  %load.buf = load i8*, i8** %buf, align 8
  call void @itoa(i32 9071, i8* %load.buf)
  %dst2 = getelementptr inbounds [4 x i8], [4 x i8]* %s1, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst2, i8* align 1 getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i64 4, i1 false)
  %load.buf3 = load i8*, i8** %buf, align 8
  %"printf(str,load.buf)" = call i32 (i8*, ...) @printf(i8* %dst2, i8* %load.buf3)
  %load.buf4 = load i8*, i8** %buf, align 8
  %"load.buf[0]" = getelementptr inbounds i8, i8* %load.buf4, i32 0
  %"load.load.buf[0]" = load i8, i8* %"load.buf[0]", align 1
  %"(i32)load.load.buf[0]" = sext i8 %"load.load.buf[0]" to i32
  ret i32 %"(i32)load.load.buf[0]"
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 044 — program: print a number in hex with as

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
@str = private constant [10 x i8] c".........\00"
@str.1 = private constant [4 x i8] c"%s\0A\00"

declare i32 @printf(i8*, ...)

define i8 @hex_digit(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %load.n = load i32, i32* %n, align 4
  %LT = icmp slt i32 %load.n, 10
  br i1 %LT, label %then, label %endif

then:                                             ; preds = %entry
  %load.n1 = load i32, i32* %n, align 4
  %ADD = add i32 48, %load.n1
  %"(i8)(i32)'0' + load.n" = trunc i32 %ADD to i8
  ret i8 %"(i8)(i32)'0' + load.n"

endif:                                            ; preds = %entry
  %load.n2 = load i32, i32* %n, align 4
  %ADD3 = add i32 97, %load.n2
  %SUB = sub i32 %ADD3, 10
  %"(i8)(i32)'a' + load.n - 10" = trunc i32 %SUB to i8
  ret i8 %"(i8)(i32)'a' + load.n - 10"
}

define void @to_hex(i32 %0, i8* %1) {
entry:
  %n = alloca i32, align 4
  %out = alloca i8*, align 8
  %i = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  store i8* %1, i8** %out, align 8
  store i32 7, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %then, %entry
  %load.i = load i32, i32* %i, align 4
  %GE = icmp sge i32 %load.i, 0
  br i1 %GE, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.out = load i8*, i8** %out, align 8
  %load.i1 = load i32, i32* %i, align 4
  %"load.out[load.i]" = getelementptr inbounds i8, i8* %load.out, i32 %load.i1
  %load.n = load i32, i32* %n, align 4
  %MOD = srem i32 %load.n, 16
  %"hex_digit(load.n % 16)" = call i8 @hex_digit(i32 %MOD)
  store i8 %"hex_digit(load.n % 16)", i8* %"load.out[load.i]", align 1
  %old = load i32, i32* %n, align 4
  %DIV_ASSIGN = sdiv i32 %old, 16
  store i32 %DIV_ASSIGN, i32* %n, align 4
  %old2 = load i32, i32* %i, align 4
  %SUB_ASSIGN = sub i32 %old2, 1
  store i32 %SUB_ASSIGN, i32* %i, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  %load.out3 = load i8*, i8** %out, align 8
  %"load.out[8]" = getelementptr inbounds i8, i8* %load.out3, i32 8
  store i8 0, i8* %"load.out[8]", align 1
  ret void
}

define i32 @main() {
entry:
  %s = alloca [10 x i8], align 1
  %s1 = alloca [4 x i8], align 1
  %dst = getelementptr inbounds [10 x i8], [10 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i64 10, i1 false)
  store i8* %dst, i8** @buf, align 8
  %load.buf = load i8*, i8** @buf, align 8
  call void @to_hex(i32 48879, i8* %load.buf)
  %dst2 = getelementptr inbounds [4 x i8], [4 x i8]* %s1, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst2, i8* align 1 getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i64 4, i1 false)
  %load.buf3 = load i8*, i8** @buf, align 8
  %"printf(str,load.buf)" = call i32 (i8*, ...) @printf(i8* %dst2, i8* %load.buf3)
  %load.buf4 = load i8*, i8** @buf, align 8
  %"load.buf[4]" = getelementptr inbounds i8, i8* %load.buf4, i32 4
  %"load.load.buf[4]" = load i8, i8* %"load.buf[4]", align 1
  %"(i32)load.load.buf[4]" = sext i8 %"load.load.buf[4]" to i32
  ret i32 %"(i32)load.load.buf[4]"
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 045 — program: a string builder on a global buffer

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
@str = private constant [33 x i8] c"                                \00"
@str.1 = private constant [3 x i8] c"x=\00"
@str.2 = private constant [5 x i8] c", y=\00"
@str.3 = private constant [4 x i8] c"%s\0A\00"

declare i32 @printf(i8*, ...)

define void @put(i8 %0) {
entry:
  %c = alloca i8, align 1
  store i8 %0, i8* %c, align 1
  %load.out = load i8*, i8** @out, align 8
  %load.len = load i32, i32* @len, align 4
  %"load.out[load.len]" = getelementptr inbounds i8, i8* %load.out, i32 %load.len
  %load.c = load i8, i8* %c, align 1
  store i8 %load.c, i8* %"load.out[load.len]", align 1
  %old = load i32, i32* @len, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* @len, align 4
  ret void
}

define void @put_num(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %load.n = load i32, i32* %n, align 4
  %GE = icmp sge i32 %load.n, 10
  br i1 %GE, label %then, label %endif

then:                                             ; preds = %entry
  %load.n1 = load i32, i32* %n, align 4
  %DIV = sdiv i32 %load.n1, 10
  call void @put_num(i32 %DIV)
  br label %endif

endif:                                            ; preds = %then, %entry
  %load.n2 = load i32, i32* %n, align 4
  %MOD = srem i32 %load.n2, 10
  %ADD = add i32 48, %MOD
  %"(i8)(i32)'0' + load.n % 10" = trunc i32 %ADD to i8
  call void @put(i8 %"(i8)(i32)'0' + load.n % 10")
  ret void
}

define void @put_str(i8* %0) {
entry:
  %s = alloca i8*, align 8
  %i = alloca i32, align 4
  store i8* %0, i8** %s, align 8
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %then, %entry
  %load.s = load i8*, i8** %s, align 8
  %load.i = load i32, i32* %i, align 4
  %"load.s[load.i]" = getelementptr inbounds i8, i8* %load.s, i32 %load.i
  %"load.load.s[load.i]" = load i8, i8* %"load.s[load.i]", align 1
  %NQ = icmp ne i8 %"load.load.s[load.i]", 0
  br i1 %NQ, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.s1 = load i8*, i8** %s, align 8
  %load.i2 = load i32, i32* %i, align 4
  %"load.s[load.i]3" = getelementptr inbounds i8, i8* %load.s1, i32 %load.i2
  %"load.load.s[load.i]4" = load i8, i8* %"load.s[load.i]3", align 1
  call void @put(i8 %"load.load.s[load.i]4")
  %old = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  ret void
}

define i32 @main() {
entry:
  %s = alloca [33 x i8], align 1
  %s1 = alloca [3 x i8], align 1
  %s3 = alloca [5 x i8], align 1
  %s5 = alloca [4 x i8], align 1
  %dst = getelementptr inbounds [33 x i8], [33 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([33 x i8], [33 x i8]* @str, i32 0, i32 0), i64 33, i1 false)
  store i8* %dst, i8** @out, align 8
  store i32 0, i32* @len, align 4
  %dst2 = getelementptr inbounds [3 x i8], [3 x i8]* %s1, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst2, i8* align 1 getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0), i64 3, i1 false)
  call void @put_str(i8* %dst2)
  call void @put_num(i32 1234)
  %dst4 = getelementptr inbounds [5 x i8], [5 x i8]* %s3, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst4, i8* align 1 getelementptr inbounds ([5 x i8], [5 x i8]* @str.2, i32 0, i32 0), i64 5, i1 false)
  call void @put_str(i8* %dst4)
  call void @put_num(i32 7)
  call void @put(i8 0)
  %dst6 = getelementptr inbounds [4 x i8], [4 x i8]* %s5, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst6, i8* align 1 getelementptr inbounds ([4 x i8], [4 x i8]* @str.3, i32 0, i32 0), i64 4, i1 false)
  %load.out = load i8*, i8** @out, align 8
  %"printf(str,load.out)" = call i32 (i8*, ...) @printf(i8* %dst6, i8* %load.out)
  %load.len = load i32, i32* @len, align 4
  ret i32 %load.len
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 046 — program: filter with as bool, not and isNullable

```ura
// expect: 42   (4 odd, 2 even, zeros skipped)
fn count_if(a i32[], n i32, odd bool) i32:
    c i32 = 0
    i i32 = 0
    while i < n:
        is_odd bool = (a[i] % 2) as bool
        if is_odd == odd and not isNullable(a[i]): c += 1
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
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.i = load i32, i32* %i, align 4
  %load.n = load i32, i32* %n, align 4
  %LT = icmp slt i32 %load.i, %load.n
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.a = load i32*, i32** %a, align 8
  %load.i1 = load i32, i32* %i, align 4
  %"load.a[load.i]" = getelementptr inbounds i32, i32* %load.a, i32 %load.i1
  %"load.load.a[load.i]" = load i32, i32* %"load.a[load.i]", align 4
  %MOD = srem i32 %"load.load.a[load.i]", 2
  %"(bool)load.load.a[load.i] % 2" = trunc i32 %MOD to i1
  store i1 %"(bool)load.load.a[load.i] % 2", i1* %is_odd, align 1
  %load.is_odd = load i1, i1* %is_odd, align 1
  %load.odd = load i1, i1* %odd, align 1
  %EQ = icmp eq i1 %load.is_odd, %load.odd
  %load.a3 = load i32*, i32** %a, align 8
  %load.i4 = load i32, i32* %i, align 4
  %"load.a[load.i]5" = getelementptr inbounds i32, i32* %load.a3, i32 %load.i4
  %"load.load.a[load.i]6" = load i32, i32* %"load.a[load.i]5", align 4
  %"isNullable(load.load.a[load.i])" = icmp eq i32 %"load.load.a[load.i]6", 0
  %NOT = xor i1 %"isNullable(load.load.a[load.i])", true
  %AND = and i1 %EQ, %NOT
  br i1 %AND, label %then2, label %endif

endwhile:                                         ; preds = %cond
  %load.c = load i32, i32* %c, align 4
  ret i32 %load.c

then2:                                            ; preds = %then
  %old = load i32, i32* %c, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %c, align 4
  br label %endif

endif:                                            ; preds = %then2, %then
  %old7 = load i32, i32* %i, align 4
  %ADD_ASSIGN8 = add i32 %old7, 1
  store i32 %ADD_ASSIGN8, i32* %i, align 4
  br label %cond
}

define i32 @main() {
entry:
  %array = alloca [8 x i32], align 4
  %e = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 0
  store i32 0, i32* %e, align 4
  %e1 = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 1
  store i32 1, i32* %e1, align 4
  %e2 = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 2
  store i32 2, i32* %e2, align 4
  %e3 = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 3
  store i32 3, i32* %e3, align 4
  %e4 = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 4
  store i32 4, i32* %e4, align 4
  %e5 = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 5
  store i32 5, i32* %e5, align 4
  %e6 = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 6
  store i32 -3, i32* %e6, align 4
  %e7 = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 7
  store i32 0, i32* %e7, align 4
  %array8 = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 0
  store i32* %array8, i32** @a, align 8
  %load.a = load i32*, i32** @a, align 8
  %"count_if(load.a,8,True)" = call i32 @count_if(i32* %load.a, i32 8, i1 true)
  %MUL = mul i32 %"count_if(load.a,8,True)", 10
  %load.a9 = load i32*, i32** @a, align 8
  %"count_if(load.a,8,False)" = call i32 @count_if(i32* %load.a9, i32 8, i1 false)
  %ADD = add i32 %MUL, %"count_if(load.a,8,False)"
  ret i32 %ADD
}
```

