# arrays

## index

- 001 — literal and indexing
- 002 — write through an index
- 003 — index from a variable
- 004 — index from an expression
- 005 — literal made of variables
- 006 — compound assignment on an element
- 007 — loop over an array
- 008 — an array passed to a function
- 009 — a function writes into the caller's array
- 010 — a string is a writable i8[]
- 011 — an array of bools

---

## 001 — literal and indexing

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

## 002 — write through an index

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

## 003 — index from a variable

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

## 004 — index from an expression

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

## 005 — literal made of variables

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

## 006 — compound assignment on an element

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

## 007 — loop over an array

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

## 008 — an array passed to a function

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

## 009 — a function writes into the caller's array

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

## 010 — a string is a writable i8[]

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

## 011 — an array of bools

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

