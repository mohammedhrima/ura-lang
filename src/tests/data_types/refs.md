# refs

## index

- 001 — write through a ref
- 002 — read through a ref, then write
- 003 — compound assignment through a ref
- 004 — swap with ref parameters
- 005 — a ref parameter changes the caller
- 006 — p1 = p2 copies the value
- 007 — p1 = &p2 rebinds
- 008 — pass a ref parameter on to another function
- 009 — a ref to a bool
- 010 — a ref in a loop
- 011 — take the address of a variable (from tests-old/ref.md #001)

---

## 001 — write through a ref

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

## 002 — read through a ref, then write

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

## 003 — compound assignment through a ref

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

## 004 — swap with ref parameters

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

## 005 — a ref parameter changes the caller

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

## 006 — p1 = p2 copies the value

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

## 007 — p1 = &p2 rebinds

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

## 008 — pass a ref parameter on to another function

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

## 009 — a ref to a bool

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

## 010 — a ref in a loop

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

## 011 — take the address of a variable (from tests-old/ref.md #001)

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

