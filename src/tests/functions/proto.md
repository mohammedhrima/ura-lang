# proto

## index

- 001 — printf with a number
- 002 — several extra arguments
- 003 — exit from C
- 004 — a string in a variable
- 005 — escapes
- 006 — printf returns how many chars it wrote
- 007 — a string passed to a function
- 008 — printf in a loop
- 009 — proto function (from tests-old/functions.md #007)
- 010 — proto variadic function (from tests-old/functions.md #008)
- 011 — chars type (from tests-old/data_types.md #004)
- 012 — escaped characters (from tests-old/data_types.md #006)

---

## 001 — printf with a number

```ura
// stdout: hello 42
// expect: 0
proto printf(fmt i8[], ...) i32

fn main() i32:
    printf("hello %d\n", 42)
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private constant [10 x i8] c"hello %d\0A\00"

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %s = alloca [10 x i8], align 1
  %dst = getelementptr inbounds [10 x i8], [10 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i64 10, i1 false)
  %"printf(str,42)" = call i32 (i8*, ...) @printf(i8* %dst, i32 42)
  ret i32 0
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 002 — several extra arguments

```ura
// stdout: 1 + 2 = 3
// expect: 0
proto printf(fmt i8[], ...) i32

fn main() i32:
    a i32 = 1
    b i32 = 2
    printf("%d + %d = %d\n", a, b, a + b)
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private constant [14 x i8] c"%d + %d = %d\0A\00"

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %s = alloca [14 x i8], align 1
  store i32 1, i32* %a, align 4
  store i32 2, i32* %b, align 4
  %dst = getelementptr inbounds [14 x i8], [14 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([14 x i8], [14 x i8]* @str, i32 0, i32 0), i64 14, i1 false)
  %load.a = load i32, i32* %a, align 4
  %load.b = load i32, i32* %b, align 4
  %load.a1 = load i32, i32* %a, align 4
  %load.b2 = load i32, i32* %b, align 4
  %ADD = add i32 %load.a1, %load.b2
  %"printf(str,load.a,load.b,load.a + load.b)" = call i32 (i8*, ...) @printf(i8* %dst, i32 %load.a, i32 %load.b, i32 %ADD)
  ret i32 0
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 003 — exit from C

```ura
// expect: 10
proto exit(code i32)

fn main() i32:
    exit(10)
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
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

## 004 — a string in a variable

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

## 005 — escapes

```ura
// stdout: a	b (a real tab), then: "quoted" \ done
// expect: 0
proto printf(fmt i8[], ...) i32

fn main() i32:
    printf("a\tb\n")
    printf("\"quoted\" \\ done\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private constant [5 x i8] c"a\09b\0A\00"
@str.1 = private constant [17 x i8] c"\22quoted\22 \\ done\0A\00"

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %s = alloca [5 x i8], align 1
  %s1 = alloca [17 x i8], align 1
  %dst = getelementptr inbounds [5 x i8], [5 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i64 5, i1 false)
  %"printf(str)" = call i32 (i8*, ...) @printf(i8* %dst)
  %dst2 = getelementptr inbounds [17 x i8], [17 x i8]* %s1, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst2, i8* align 1 getelementptr inbounds ([17 x i8], [17 x i8]* @str.1, i32 0, i32 0), i64 17, i1 false)
  %"printf(str)3" = call i32 (i8*, ...) @printf(i8* %dst2)
  ret i32 0
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 006 — printf returns how many chars it wrote

```ura
// stdout: abc
// expect: 3
proto printf(fmt i8[], ...) i32

fn main() i32:
    return printf("abc")
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private constant [4 x i8] c"abc\00"

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %s = alloca [4 x i8], align 1
  %dst = getelementptr inbounds [4 x i8], [4 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i64 4, i1 false)
  %"printf(str)" = call i32 (i8*, ...) @printf(i8* %dst)
  ret i32 %"printf(str)"
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 007 — a string passed to a function

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

## 008 — printf in a loop

```ura
// stdout: 0 1 2
// expect: 0
proto printf(fmt i8[], ...) i32

fn main() i32:
    i i32 = 0
    while i < 3:
        printf("%d", i)
        if i < 2:
            printf(" ")
        i += 1
    printf("\n")
    return 0
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
  %i = alloca i32, align 4
  %s = alloca [3 x i8], align 1
  %s5 = alloca [2 x i8], align 1
  %s7 = alloca [2 x i8], align 1
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 3
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %dst = getelementptr inbounds [3 x i8], [3 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i64 3, i1 false)
  %load.i1 = load i32, i32* %i, align 4
  %"printf(str,load.i)" = call i32 (i8*, ...) @printf(i8* %dst, i32 %load.i1)
  %load.i3 = load i32, i32* %i, align 4
  %LT4 = icmp slt i32 %load.i3, 2
  br i1 %LT4, label %then2, label %endif

endwhile:                                         ; preds = %cond
  %dst8 = getelementptr inbounds [2 x i8], [2 x i8]* %s7, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst8, i8* align 1 getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0), i64 2, i1 false)
  %"printf(str)9" = call i32 (i8*, ...) @printf(i8* %dst8)
  ret i32 0

then2:                                            ; preds = %then
  %dst6 = getelementptr inbounds [2 x i8], [2 x i8]* %s5, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst6, i8* align 1 getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i64 2, i1 false)
  %"printf(str)" = call i32 (i8*, ...) @printf(i8* %dst6)
  br label %endif

endif:                                            ; preds = %then2, %then
  %old = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  br label %cond
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 009 — proto function (from tests-old/functions.md #007)

```ura
proto exit(code i32)

fn main() i32:
   exit(10)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
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

## 010 — proto variadic function (from tests-old/functions.md #008)

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

## 011 — chars type (from tests-old/data_types.md #004)

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

## 012 — escaped characters (from tests-old/data_types.md #006)

```ura
proto printf(s i8[], ...)

fn main() i32:
   printf("tab\there\n")
   return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private constant [10 x i8] c"tab\09here\0A\00"

declare void @printf(i8*, ...)

define i32 @main() {
entry:
  %s = alloca [10 x i8], align 1
  %dst = getelementptr inbounds [10 x i8], [10 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i64 10, i1 false)
  call void (i8*, ...) @printf(i8* %dst)
  ret i32 0
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

