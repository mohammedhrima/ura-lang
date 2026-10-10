# builtins

## index

- 001 — typeof on a value, a string and a struct
- 002 — typeof on a ref and on a type name
- 003 — sizeof folds to a constant
- 004 — output picks the spec from each argument
- 005 — output with literals and escapes

---

## 001 — typeof on a value, a string and a struct

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
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32 }

@const = private unnamed_addr constant [4 x i8] c"abc\00"
@const.1 = private unnamed_addr constant [13 x i8] c"%s%s%s%s%s%s\00"
@const.2 = private unnamed_addr constant [4 x i8] c"i32\00"
@const.3 = private unnamed_addr constant [2 x i8] c" \00"
@const.4 = private unnamed_addr constant [5 x i8] c"i8[]\00"
@const.5 = private unnamed_addr constant [2 x i8] c" \00"
@const.6 = private unnamed_addr constant [12 x i8] c"struct.User\00"
@const.7 = private unnamed_addr constant [2 x i8] c"\0A\00"

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %s = alloca i8*, align 8
  %array = alloca [4 x i8], align 1
  %u = alloca %User, align 8
  %array1 = alloca [13 x i8], align 1
  %array2 = alloca [4 x i8], align 1
  %array3 = alloca [2 x i8], align 1
  %array4 = alloca [5 x i8], align 1
  %array5 = alloca [2 x i8], align 1
  %array6 = alloca [12 x i8], align 1
  %array7 = alloca [2 x i8], align 1
  store i32 1, i32* %x, align 4
  %0 = bitcast [4 x i8]* %array to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %0, i8* align 1 getelementptr inbounds ([4 x i8], [4 x i8]* @const, i32 0, i32 0), i64 4, i1 false)
  %1 = getelementptr inbounds [4 x i8], [4 x i8]* %array, i32 0, i32 0
  store i8* %1, i8** %s, align 8
  %2 = bitcast [13 x i8]* %array1 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %2, i8* align 1 getelementptr inbounds ([13 x i8], [13 x i8]* @const.1, i32 0, i32 0), i64 13, i1 false)
  %3 = getelementptr inbounds [13 x i8], [13 x i8]* %array1, i32 0, i32 0
  %4 = bitcast [4 x i8]* %array2 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %4, i8* align 1 getelementptr inbounds ([4 x i8], [4 x i8]* @const.2, i32 0, i32 0), i64 4, i1 false)
  %5 = getelementptr inbounds [4 x i8], [4 x i8]* %array2, i32 0, i32 0
  %6 = bitcast [2 x i8]* %array3 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %6, i8* align 1 getelementptr inbounds ([2 x i8], [2 x i8]* @const.3, i32 0, i32 0), i64 2, i1 false)
  %7 = getelementptr inbounds [2 x i8], [2 x i8]* %array3, i32 0, i32 0
  %8 = bitcast [5 x i8]* %array4 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %8, i8* align 1 getelementptr inbounds ([5 x i8], [5 x i8]* @const.4, i32 0, i32 0), i64 5, i1 false)
  %9 = getelementptr inbounds [5 x i8], [5 x i8]* %array4, i32 0, i32 0
  %10 = bitcast [2 x i8]* %array5 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %10, i8* align 1 getelementptr inbounds ([2 x i8], [2 x i8]* @const.5, i32 0, i32 0), i64 2, i1 false)
  %11 = getelementptr inbounds [2 x i8], [2 x i8]* %array5, i32 0, i32 0
  %12 = bitcast [12 x i8]* %array6 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %12, i8* align 1 getelementptr inbounds ([12 x i8], [12 x i8]* @const.6, i32 0, i32 0), i64 12, i1 false)
  %13 = getelementptr inbounds [12 x i8], [12 x i8]* %array6, i32 0, i32 0
  %14 = bitcast [2 x i8]* %array7 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %14, i8* align 1 getelementptr inbounds ([2 x i8], [2 x i8]* @const.7, i32 0, i32 0), i64 2, i1 false)
  %15 = getelementptr inbounds [2 x i8], [2 x i8]* %array7, i32 0, i32 0
  call void (i8*, ...) @printf(i8* %3, i8* %5, i8* %7, i8* %9, i8* %11, i8* %13, i8* %15)
  ret i32 0
}

declare void @printf(i8*, ...)

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 002 — typeof on a ref and on a type name

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
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%User = type { i32 }

@const = private unnamed_addr constant [9 x i8] c"%s%s%s%s\00"
@const.1 = private unnamed_addr constant [13 x i8] c"&struct.User\00"
@const.2 = private unnamed_addr constant [2 x i8] c" \00"
@const.3 = private unnamed_addr constant [4 x i8] c"i32\00"
@const.4 = private unnamed_addr constant [2 x i8] c"\0A\00"

define i32 @main() {
entry:
  %u = alloca %User, align 8
  %p = alloca %User*, align 8
  %array = alloca [9 x i8], align 1
  %array1 = alloca [13 x i8], align 1
  %array2 = alloca [2 x i8], align 1
  %array3 = alloca [4 x i8], align 1
  %array4 = alloca [2 x i8], align 1
  store %User* %u, %User** %p, align 8
  %0 = bitcast [9 x i8]* %array to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %0, i8* align 1 getelementptr inbounds ([9 x i8], [9 x i8]* @const, i32 0, i32 0), i64 9, i1 false)
  %1 = getelementptr inbounds [9 x i8], [9 x i8]* %array, i32 0, i32 0
  %2 = bitcast [13 x i8]* %array1 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %2, i8* align 1 getelementptr inbounds ([13 x i8], [13 x i8]* @const.1, i32 0, i32 0), i64 13, i1 false)
  %3 = getelementptr inbounds [13 x i8], [13 x i8]* %array1, i32 0, i32 0
  %4 = bitcast [2 x i8]* %array2 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %4, i8* align 1 getelementptr inbounds ([2 x i8], [2 x i8]* @const.2, i32 0, i32 0), i64 2, i1 false)
  %5 = getelementptr inbounds [2 x i8], [2 x i8]* %array2, i32 0, i32 0
  %6 = bitcast [4 x i8]* %array3 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %6, i8* align 1 getelementptr inbounds ([4 x i8], [4 x i8]* @const.3, i32 0, i32 0), i64 4, i1 false)
  %7 = getelementptr inbounds [4 x i8], [4 x i8]* %array3, i32 0, i32 0
  %8 = bitcast [2 x i8]* %array4 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %8, i8* align 1 getelementptr inbounds ([2 x i8], [2 x i8]* @const.4, i32 0, i32 0), i64 2, i1 false)
  %9 = getelementptr inbounds [2 x i8], [2 x i8]* %array4, i32 0, i32 0
  call void (i8*, ...) @printf(i8* %1, i8* %3, i8* %5, i8* %7, i8* %9)
  ret i32 0
}

declare void @printf(i8*, ...)

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 003 — sizeof folds to a constant

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
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Pad = type { i8, i32 }

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %s = alloca i8*, align 8
  %p = alloca %Pad, align 8
  ret i32 20
}
```

---

## 004 — output picks the spec from each argument

```ura
fn main() i32:
   n i32 = 42
   c i8 = 'z'
   s i8[] = "hi"
   flag b1 = n > 1
   output(n, " ", c, " ", s, " ", flag, "\n")
   return 0
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@const = private unnamed_addr constant [3 x i8] c"hi\00"
@const.1 = private unnamed_addr constant [17 x i8] c"%d%s%c%s%s%s%d%s\00"
@const.2 = private unnamed_addr constant [2 x i8] c" \00"
@const.3 = private unnamed_addr constant [2 x i8] c" \00"
@const.4 = private unnamed_addr constant [2 x i8] c" \00"
@const.5 = private unnamed_addr constant [2 x i8] c"\0A\00"

define i32 @main() {
entry:
  %n = alloca i32, align 4
  %c = alloca i8, align 1
  %s = alloca i8*, align 8
  %array = alloca [3 x i8], align 1
  %flag = alloca i1, align 1
  %array2 = alloca [17 x i8], align 1
  %array4 = alloca [2 x i8], align 1
  %array6 = alloca [2 x i8], align 1
  %array8 = alloca [2 x i8], align 1
  %array10 = alloca [2 x i8], align 1
  store i32 42, i32* %n, align 4
  store i8 122, i8* %c, align 1
  %0 = bitcast [3 x i8]* %array to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %0, i8* align 1 getelementptr inbounds ([3 x i8], [3 x i8]* @const, i32 0, i32 0), i64 3, i1 false)
  %1 = getelementptr inbounds [3 x i8], [3 x i8]* %array, i32 0, i32 0
  store i8* %1, i8** %s, align 8
  %n1 = load i32, i32* %n, align 4
  %GT = icmp sgt i32 %n1, 1
  store i1 %GT, i1* %flag, align 1
  %2 = bitcast [17 x i8]* %array2 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %2, i8* align 1 getelementptr inbounds ([17 x i8], [17 x i8]* @const.1, i32 0, i32 0), i64 17, i1 false)
  %3 = getelementptr inbounds [17 x i8], [17 x i8]* %array2, i32 0, i32 0
  %n3 = load i32, i32* %n, align 4
  %4 = bitcast [2 x i8]* %array4 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %4, i8* align 1 getelementptr inbounds ([2 x i8], [2 x i8]* @const.2, i32 0, i32 0), i64 2, i1 false)
  %5 = getelementptr inbounds [2 x i8], [2 x i8]* %array4, i32 0, i32 0
  %c5 = load i8, i8* %c, align 1
  %6 = bitcast [2 x i8]* %array6 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %6, i8* align 1 getelementptr inbounds ([2 x i8], [2 x i8]* @const.3, i32 0, i32 0), i64 2, i1 false)
  %7 = getelementptr inbounds [2 x i8], [2 x i8]* %array6, i32 0, i32 0
  %s7 = load i8*, i8** %s, align 8
  %8 = bitcast [2 x i8]* %array8 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %8, i8* align 1 getelementptr inbounds ([2 x i8], [2 x i8]* @const.4, i32 0, i32 0), i64 2, i1 false)
  %9 = getelementptr inbounds [2 x i8], [2 x i8]* %array8, i32 0, i32 0
  %flag9 = load i1, i1* %flag, align 1
  %10 = bitcast [2 x i8]* %array10 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %10, i8* align 1 getelementptr inbounds ([2 x i8], [2 x i8]* @const.5, i32 0, i32 0), i64 2, i1 false)
  %11 = getelementptr inbounds [2 x i8], [2 x i8]* %array10, i32 0, i32 0
  call void (i8*, ...) @printf(i8* %3, i32 %n3, i8* %5, i8 %c5, i8* %7, i8* %s7, i8* %9, i1 %flag9, i8* %11)
  ret i32 0
}

declare void @printf(i8*, ...)

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 005 — output with literals and escapes

```ura
fn main() i32:
   n i32 = 3
   output("n=", n, "\ttab\n")
   output("no newline")
   output("\n")
   return 0
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@const = private unnamed_addr constant [7 x i8] c"%s%d%s\00"
@const.1 = private unnamed_addr constant [3 x i8] c"n=\00"
@const.2 = private unnamed_addr constant [6 x i8] c"\09tab\0A\00"
@const.3 = private unnamed_addr constant [3 x i8] c"%s\00"
@const.4 = private unnamed_addr constant [11 x i8] c"no newline\00"
@const.5 = private unnamed_addr constant [3 x i8] c"%s\00"
@const.6 = private unnamed_addr constant [2 x i8] c"\0A\00"

define i32 @main() {
entry:
  %n = alloca i32, align 4
  %array = alloca [7 x i8], align 1
  %array1 = alloca [3 x i8], align 1
  %array3 = alloca [6 x i8], align 1
  %array4 = alloca [3 x i8], align 1
  %array5 = alloca [11 x i8], align 1
  %array6 = alloca [3 x i8], align 1
  %array7 = alloca [2 x i8], align 1
  store i32 3, i32* %n, align 4
  %0 = bitcast [7 x i8]* %array to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %0, i8* align 1 getelementptr inbounds ([7 x i8], [7 x i8]* @const, i32 0, i32 0), i64 7, i1 false)
  %1 = getelementptr inbounds [7 x i8], [7 x i8]* %array, i32 0, i32 0
  %2 = bitcast [3 x i8]* %array1 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %2, i8* align 1 getelementptr inbounds ([3 x i8], [3 x i8]* @const.1, i32 0, i32 0), i64 3, i1 false)
  %3 = getelementptr inbounds [3 x i8], [3 x i8]* %array1, i32 0, i32 0
  %n2 = load i32, i32* %n, align 4
  %4 = bitcast [6 x i8]* %array3 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %4, i8* align 1 getelementptr inbounds ([6 x i8], [6 x i8]* @const.2, i32 0, i32 0), i64 6, i1 false)
  %5 = getelementptr inbounds [6 x i8], [6 x i8]* %array3, i32 0, i32 0
  call void (i8*, ...) @printf(i8* %1, i8* %3, i32 %n2, i8* %5)
  %6 = bitcast [3 x i8]* %array4 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %6, i8* align 1 getelementptr inbounds ([3 x i8], [3 x i8]* @const.3, i32 0, i32 0), i64 3, i1 false)
  %7 = getelementptr inbounds [3 x i8], [3 x i8]* %array4, i32 0, i32 0
  %8 = bitcast [11 x i8]* %array5 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %8, i8* align 1 getelementptr inbounds ([11 x i8], [11 x i8]* @const.4, i32 0, i32 0), i64 11, i1 false)
  %9 = getelementptr inbounds [11 x i8], [11 x i8]* %array5, i32 0, i32 0
  call void (i8*, ...) @printf(i8* %7, i8* %9)
  %10 = bitcast [3 x i8]* %array6 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %10, i8* align 1 getelementptr inbounds ([3 x i8], [3 x i8]* @const.5, i32 0, i32 0), i64 3, i1 false)
  %11 = getelementptr inbounds [3 x i8], [3 x i8]* %array6, i32 0, i32 0
  %12 = bitcast [2 x i8]* %array7 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %12, i8* align 1 getelementptr inbounds ([2 x i8], [2 x i8]* @const.6, i32 0, i32 0), i64 2, i1 false)
  %13 = getelementptr inbounds [2 x i8], [2 x i8]* %array7, i32 0, i32 0
  call void (i8*, ...) @printf(i8* %11, i8* %13)
  ret i32 0
}

declare void @printf(i8*, ...)

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

