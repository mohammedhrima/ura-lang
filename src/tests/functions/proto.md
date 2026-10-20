# proto

## index

- 001 — a string in a variable
- 002 — a string passed to a function
- 003 — proto variadic function (from tests-old/functions.md #008)
- 004 — chars type (from tests-old/data_types.md #004)

---

## 001 — a string in a variable

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

## 002 — a string passed to a function

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

## 003 — proto variadic function (from tests-old/functions.md #008)

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

## 004 — chars type (from tests-old/data_types.md #004)

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

