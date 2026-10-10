# array

## index

- 001 — literal and indexing
- 002 — writing through an index
- 003 — index from a variable
- 004 — literal with variables in it
- 005 — string as i8[] is writable
- 006 — array passed to a function
- 007 — array attribute of a struct

---

## 001 — literal and indexing

```ura
fn main() i32:
   a i32[] = [4, 5, 6]
   return a[0] + a[2]
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@const = private unnamed_addr constant [3 x i32] [i32 4, i32 5, i32 6]

define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %array = alloca [3 x i32], align 4
  %0 = bitcast [3 x i32]* %array to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %0, i8* align 1 bitcast ([3 x i32]* @const to i8*), i64 12, i1 false)
  %1 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32* %1, i32** %a, align 8
  %a1 = load i32*, i32** %a, align 8
  %2 = getelementptr inbounds i32, i32* %a1, i32 0
  %3 = load i32, i32* %2, align 4
  %a2 = load i32*, i32** %a, align 8
  %4 = getelementptr inbounds i32, i32* %a2, i32 2
  %5 = load i32, i32* %4, align 4
  %ADD = add i32 %3, %5
  ret i32 %ADD
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 002 — writing through an index

```ura
fn main() i32:
   a i32[] = [1, 2, 3]
   a[1] = 9
   return a[1]
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@const = private unnamed_addr constant [3 x i32] [i32 1, i32 2, i32 3]

define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %array = alloca [3 x i32], align 4
  %0 = bitcast [3 x i32]* %array to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %0, i8* align 1 bitcast ([3 x i32]* @const to i8*), i64 12, i1 false)
  %1 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32* %1, i32** %a, align 8
  %a1 = load i32*, i32** %a, align 8
  %2 = getelementptr inbounds i32, i32* %a1, i32 1
  store i32 9, i32* %2, align 4
  %a2 = load i32*, i32** %a, align 8
  %3 = getelementptr inbounds i32, i32* %a2, i32 1
  %4 = load i32, i32* %3, align 4
  ret i32 %4
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 003 — index from a variable

```ura
fn main() i32:
   a i32[] = [7, 8, 9]
   i i32 = 2
   return a[i]
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@const = private unnamed_addr constant [3 x i32] [i32 7, i32 8, i32 9]

define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %array = alloca [3 x i32], align 4
  %i = alloca i32, align 4
  %0 = bitcast [3 x i32]* %array to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %0, i8* align 1 bitcast ([3 x i32]* @const to i8*), i64 12, i1 false)
  %1 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32* %1, i32** %a, align 8
  store i32 2, i32* %i, align 4
  %a1 = load i32*, i32** %a, align 8
  %i2 = load i32, i32* %i, align 4
  %2 = getelementptr inbounds i32, i32* %a1, i32 %i2
  %3 = load i32, i32* %2, align 4
  ret i32 %3
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 004 — literal with variables in it

```ura
fn main() i32:
   x i32 = 10
   y i32 = 4
   a i32[] = [x, 2, y]
   return a[0] + a[1] + a[2]
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@const = private unnamed_addr constant [3 x i32] [i32 0, i32 2, i32 0]

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  %a = alloca i32*, align 8
  %array = alloca [3 x i32], align 4
  store i32 10, i32* %x, align 4
  store i32 4, i32* %y, align 4
  %x1 = load i32, i32* %x, align 4
  %y2 = load i32, i32* %y, align 4
  %0 = bitcast [3 x i32]* %array to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %0, i8* align 1 bitcast ([3 x i32]* @const to i8*), i64 12, i1 false)
  %1 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  %2 = getelementptr inbounds i32, i32* %1, i32 0
  store i32 %x1, i32* %2, align 4
  %3 = getelementptr inbounds i32, i32* %1, i32 2
  store i32 %y2, i32* %3, align 4
  store i32* %1, i32** %a, align 8
  %a3 = load i32*, i32** %a, align 8
  %4 = getelementptr inbounds i32, i32* %a3, i32 0
  %5 = load i32, i32* %4, align 4
  %a4 = load i32*, i32** %a, align 8
  %6 = getelementptr inbounds i32, i32* %a4, i32 1
  %7 = load i32, i32* %6, align 4
  %ADD = add i32 %5, %7
  %a5 = load i32*, i32** %a, align 8
  %8 = getelementptr inbounds i32, i32* %a5, i32 2
  %9 = load i32, i32* %8, align 4
  %ADD6 = add i32 %ADD, %9
  ret i32 %ADD6
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 005 — string as i8[] is writable

```ura
fn main() i32:
   s i8[] = "abc"
   s[0] = 'x'
   c i8 = s[1]
   if c == 'b':
      return 1
   return 0
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@const = private unnamed_addr constant [4 x i8] c"abc\00"

define i32 @main() {
entry:
  %s = alloca i8*, align 8
  %array = alloca [4 x i8], align 1
  %c = alloca i8, align 1
  %0 = bitcast [4 x i8]* %array to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %0, i8* align 1 getelementptr inbounds ([4 x i8], [4 x i8]* @const, i32 0, i32 0), i64 4, i1 false)
  %1 = getelementptr inbounds [4 x i8], [4 x i8]* %array, i32 0, i32 0
  store i8* %1, i8** %s, align 8
  %s1 = load i8*, i8** %s, align 8
  %2 = getelementptr inbounds i8, i8* %s1, i32 0
  store i8 120, i8* %2, align 1
  %s2 = load i8*, i8** %s, align 8
  %3 = getelementptr inbounds i8, i8* %s2, i32 1
  %4 = load i8, i8* %3, align 1
  store i8 %4, i8* %c, align 1
  %c3 = load i8, i8* %c, align 1
  %EQ = icmp eq i8 %c3, 98
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

## 006 — array passed to a function

```ura
fn sum(a i32[]) i32:
   total i32 = 0
   i i32 = 0
   while i < 3:
      total += a[i]
      i += 1
   return total

fn main() i32:
   values i32[] = [1, 2, 3]
   return sum(values)
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@const = private unnamed_addr constant [3 x i32] [i32 1, i32 2, i32 3]

define i32 @sum(i32* %0) {
entry:
  %a = alloca i32*, align 8
  %total = alloca i32, align 4
  %i = alloca i32, align 4
  store i32* %0, i32** %a, align 8
  store i32 0, i32* %total, align 4
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %then, %entry
  %i1 = load i32, i32* %i, align 4
  %LT = icmp slt i32 %i1, 3
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %total2 = load i32, i32* %total, align 4
  %a3 = load i32*, i32** %a, align 8
  %i4 = load i32, i32* %i, align 4
  %1 = getelementptr inbounds i32, i32* %a3, i32 %i4
  %2 = load i32, i32* %1, align 4
  %ADD = add i32 %total2, %2
  store i32 %ADD, i32* %total, align 4
  %i5 = load i32, i32* %i, align 4
  %ADD6 = add i32 %i5, 1
  store i32 %ADD6, i32* %i, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  %total7 = load i32, i32* %total, align 4
  ret i32 %total7
}

define i32 @main() {
entry:
  %values = alloca i32*, align 8
  %array = alloca [3 x i32], align 4
  %0 = bitcast [3 x i32]* %array to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %0, i8* align 1 bitcast ([3 x i32]* @const to i8*), i64 12, i1 false)
  %1 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32* %1, i32** %values, align 8
  %values1 = load i32*, i32** %values, align 8
  %sum = call i32 @sum(i32* %values1)
  ret i32 %sum
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 007 — array attribute of a struct

```ura
struct Row:
   cells i32[]

fn main() i32:
   r Row
   r.cells = [1, 2, 3]
   r.cells[0] = 9
   return r.cells[0] + r.cells[2]
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Row = type { i32* }

@const = private unnamed_addr constant [3 x i32] [i32 1, i32 2, i32 3]

define i32 @main() {
entry:
  %r = alloca %Row, align 8
  %array = alloca [3 x i32], align 4
  %0 = bitcast [3 x i32]* %array to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %0, i8* align 1 bitcast ([3 x i32]* @const to i8*), i64 12, i1 false)
  %1 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  %r.cells = getelementptr inbounds %Row, %Row* %r, i32 0, i32 0
  store i32* %1, i32** %r.cells, align 8
  %r.cells1 = getelementptr inbounds %Row, %Row* %r, i32 0, i32 0
  %r.cells2 = load i32*, i32** %r.cells1, align 8
  %2 = getelementptr inbounds i32, i32* %r.cells2, i32 0
  store i32 9, i32* %2, align 4
  %r.cells3 = getelementptr inbounds %Row, %Row* %r, i32 0, i32 0
  %r.cells4 = load i32*, i32** %r.cells3, align 8
  %3 = getelementptr inbounds i32, i32* %r.cells4, i32 0
  %4 = load i32, i32* %3, align 4
  %r.cells5 = getelementptr inbounds %Row, %Row* %r, i32 0, i32 0
  %r.cells6 = load i32*, i32** %r.cells5, align 8
  %5 = getelementptr inbounds i32, i32* %r.cells6, i32 2
  %6 = load i32, i32* %5, align 4
  %ADD = add i32 %4, %6
  ret i32 %ADD
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

