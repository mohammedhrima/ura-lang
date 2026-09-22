# functions

## index

- 001 — calling function before declaration
- 002 — math op on functions
- 003 — nested calling
- 004 — factorial
- 005 — fn returns ref
- 006 — swap
- 007 — proto function
- 008 — proto variadic function
- 009 — overloaded function
- 010 — bare return in a void function
- 011 — multi line string

---

## 001 — calling function before declaration

```ura
fn main() i32:
    return later()

fn later() i32:
    return 9
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %later = call i32 @later()
  ret i32 %later
}

define i32 @later() {
entry:
  ret i32 9
}
```

---

## 002 — math op on functions

```ura
fn square(x i32) i32:
    return x * x

fn main() i32:
    return square(3) + square(4)
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @square(i32 %0) {
entry:
  %x = alloca i32, align 4
  store i32 %0, i32* %x, align 4
  %x1 = load i32, i32* %x, align 4
  %x2 = load i32, i32* %x, align 4
  %MUL = mul i32 %x1, %x2
  ret i32 %MUL
}

define i32 @main() {
entry:
  %square = call i32 @square(i32 3)
  %square1 = call i32 @square(i32 4)
  %ADD = add i32 %square, %square1
  ret i32 %ADD
}
```

---

## 003 — nested calling

```ura
fn inc(x i32) i32:
    return x + 1

fn main() i32:
    return inc(inc(inc(0)))
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @inc(i32 %0) {
entry:
  %x = alloca i32, align 4
  store i32 %0, i32* %x, align 4
  %x1 = load i32, i32* %x, align 4
  %ADD = add i32 %x1, 1
  ret i32 %ADD
}

define i32 @main() {
entry:
  %inc = call i32 @inc(i32 0)
  %inc1 = call i32 @inc(i32 %inc)
  %inc2 = call i32 @inc(i32 %inc1)
  ret i32 %inc2
}
```

---

## 004 — factorial

```ura
fn fact(n i32) i32:
    if n <= 1:
        return 1
    return n * fact(n - 1)

fn main() i32:
    return fact(5)
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @fact(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %n1 = load i32, i32* %n, align 4
  %LE = icmp sle i32 %n1, 1
  br i1 %LE, label %then, label %endif

then:                                             ; preds = %entry
  ret i32 1

endif:                                            ; preds = %entry
  %n2 = load i32, i32* %n, align 4
  %n3 = load i32, i32* %n, align 4
  %SUB = sub i32 %n3, 1
  %fact = call i32 @fact(i32 %SUB)
  %MUL = mul i32 %n2, %fact
  ret i32 %MUL
}

define i32 @main() {
entry:
  %fact = call i32 @fact(i32 5)
  ret i32 %fact
}
```

---

## 005 — fn returns ref

```ura
fn same(p &i32) &i32:
    return &p

fn main() i32:
    a i32 = 6
    q &i32 = same(&a)
    return q
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32* @same(i32* %0) {
entry:
  %p = alloca i32*, align 8
  store i32* %0, i32** %p, align 8
  %p1 = load i32*, i32** %p, align 8
  ret i32* %p1
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %q = alloca i32*, align 8
  store i32 6, i32* %a, align 4
  %same = call i32* @same(i32* %a)
  store i32* %same, i32** %q, align 8
  %q1 = load i32*, i32** %q, align 8
  %q.dref = load i32, i32* %q1, align 4
  ret i32 %q.dref
}
```

---

## 006 — swap

```ura
fn swap(x &i32, y &i32):
    t i32 = x
    x = y
    y = t

fn main() i32:
    a i32 = 1
    b i32 = 2
    swap(&a, &b)
    return a * 10 + b
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define void @swap(i32* %0, i32* %1) {
entry:
  %x = alloca i32*, align 8
  %y = alloca i32*, align 8
  %t = alloca i32, align 4
  store i32* %0, i32** %x, align 8
  store i32* %1, i32** %y, align 8
  %x1 = load i32*, i32** %x, align 8
  %x.dref = load i32, i32* %x1, align 4
  store i32 %x.dref, i32* %t, align 4
  %y2 = load i32*, i32** %y, align 8
  %y.dref = load i32, i32* %y2, align 4
  %x3 = load i32*, i32** %x, align 8
  store i32 %y.dref, i32* %x3, align 4
  %t4 = load i32, i32* %t, align 4
  %y5 = load i32*, i32** %y, align 8
  store i32 %t4, i32* %y5, align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 1, i32* %a, align 4
  store i32 2, i32* %b, align 4
  call void @swap(i32* %a, i32* %b)
  %a1 = load i32, i32* %a, align 4
  %MUL = mul i32 %a1, 10
  %b2 = load i32, i32* %b, align 4
  %ADD = add i32 %MUL, %b2
  ret i32 %ADD
}
```

---

## 007 — proto function

```ura
proto exit(code i32)

fn main() i32:
   exit(10)
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

declare void @exit(i32)

define i32 @main() {
entry:
  call void @exit(i32 10)
  ret i32 0
}
```

---

## 008 — proto variadic function

```ura
proto printf(fmt chars, ...) i32

fn main() i32:
   str chars = "abcdef"
   printf("<%s>", str)
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@const = private unnamed_addr constant [7 x i8] c"abcdef\00"
@const.1 = private unnamed_addr constant [5 x i8] c"<%s>\00"

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %str = alloca i8*, align 8
  %array = alloca [7 x i8], align 1
  %array1 = alloca [5 x i8], align 1
  %0 = bitcast [7 x i8]* %array to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %0, i8* align 1 getelementptr inbounds ([7 x i8], [7 x i8]* @const, i32 0, i32 0), i64 7, i1 false)
  %1 = getelementptr inbounds [7 x i8], [7 x i8]* %array, i32 0, i32 0
  store i8* %1, i8** %str, align 8
  %2 = bitcast [5 x i8]* %array1 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %2, i8* align 1 getelementptr inbounds ([5 x i8], [5 x i8]* @const.1, i32 0, i32 0), i64 5, i1 false)
  %3 = getelementptr inbounds [5 x i8], [5 x i8]* %array1, i32 0, i32 0
  %str2 = load i8*, i8** %str, align 8
  %printf = call i32 (i8*, ...) @printf(i8* %3, i8* %str2)
  ret i32 0
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 009 — overloaded function

```ura
proto printf(s chars, ...)

fn show(n i32):
   printf("int %d\n", n)

fn show(s chars):
   printf("str %s\n", s)

fn main() i32:
   show(42)
   show("hi")
   return 0
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@const = private unnamed_addr constant [8 x i8] c"int %d\0A\00"
@const.2 = private unnamed_addr constant [8 x i8] c"str %s\0A\00"
@const.3 = private unnamed_addr constant [3 x i8] c"hi\00"

declare void @printf(i8*, ...)

define void @show(i32 %0) {
entry:
  %n = alloca i32, align 4
  %array = alloca [8 x i8], align 1
  store i32 %0, i32* %n, align 4
  %1 = bitcast [8 x i8]* %array to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %1, i8* align 1 getelementptr inbounds ([8 x i8], [8 x i8]* @const, i32 0, i32 0), i64 8, i1 false)
  %2 = getelementptr inbounds [8 x i8], [8 x i8]* %array, i32 0, i32 0
  %n1 = load i32, i32* %n, align 4
  call void (i8*, ...) @printf(i8* %2, i32 %n1)
  ret void
}

define void @show.1(i8* %0) {
entry:
  %s = alloca i8*, align 8
  %array = alloca [8 x i8], align 1
  store i8* %0, i8** %s, align 8
  %1 = bitcast [8 x i8]* %array to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %1, i8* align 1 getelementptr inbounds ([8 x i8], [8 x i8]* @const.2, i32 0, i32 0), i64 8, i1 false)
  %2 = getelementptr inbounds [8 x i8], [8 x i8]* %array, i32 0, i32 0
  %s1 = load i8*, i8** %s, align 8
  call void (i8*, ...) @printf(i8* %2, i8* %s1)
  ret void
}

define i32 @main() {
entry:
  %array = alloca [3 x i8], align 1
  call void @show(i32 42)
  %0 = bitcast [3 x i8]* %array to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %0, i8* align 1 getelementptr inbounds ([3 x i8], [3 x i8]* @const.3, i32 0, i32 0), i64 3, i1 false)
  %1 = getelementptr inbounds [3 x i8], [3 x i8]* %array, i32 0, i32 0
  call void @show.1(i8* %1)
  ret i32 0
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 010 — bare return in a void function

```ura
fn foo(a i32):
   return

fn foo(a b1):
   return

fn main() i32:
   foo(1)
   foo(True)
   return 7
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define void @foo(i32 %0) {
entry:
  %a = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  ret void
}

define void @foo.1(i1 %0) {
entry:
  %a = alloca i1, align 1
  store i1 %0, i1* %a, align 1
  ret void
}

define i32 @main() {
entry:
  call void @foo(i32 1)
  call void @foo.1(i1 true)
  ret i32 7
}
```

---

## 011 — multi line string

```ura
fn foo(ptr i32):

fn main() i32:
   a i32 = 10
   foo(a)
   output(
        "hello "
        "world",
        12, "\n")
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@const = private unnamed_addr constant [7 x i8] c"%s%d%s\00"
@const.1 = private unnamed_addr constant [12 x i8] c"hello world\00"
@const.2 = private unnamed_addr constant [2 x i8] c"\0A\00"

define void @foo(i32 %0) {
entry:
  %ptr = alloca i32, align 4
  store i32 %0, i32* %ptr, align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %array = alloca [7 x i8], align 1
  %array2 = alloca [12 x i8], align 1
  %array3 = alloca [2 x i8], align 1
  store i32 10, i32* %a, align 4
  %a1 = load i32, i32* %a, align 4
  call void @foo(i32 %a1)
  %0 = bitcast [7 x i8]* %array to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %0, i8* align 1 getelementptr inbounds ([7 x i8], [7 x i8]* @const, i32 0, i32 0), i64 7, i1 false)
  %1 = getelementptr inbounds [7 x i8], [7 x i8]* %array, i32 0, i32 0
  %2 = bitcast [12 x i8]* %array2 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %2, i8* align 1 getelementptr inbounds ([12 x i8], [12 x i8]* @const.1, i32 0, i32 0), i64 12, i1 false)
  %3 = getelementptr inbounds [12 x i8], [12 x i8]* %array2, i32 0, i32 0
  %4 = bitcast [2 x i8]* %array3 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %4, i8* align 1 getelementptr inbounds ([2 x i8], [2 x i8]* @const.2, i32 0, i32 0), i64 2, i1 false)
  %5 = getelementptr inbounds [2 x i8], [2 x i8]* %array3, i32 0, i32 0
  call void (i8*, ...) @printf(i8* %1, i8* %3, i32 12, i8* %5)
  ret i32 0
}

declare void @printf(i8*, ...)

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

