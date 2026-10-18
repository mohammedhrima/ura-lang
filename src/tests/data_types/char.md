# char

## index

- 001 — print chars
- 002 — compare chars
- 003 — order of chars
- 004 — escaped chars
- 005 — reassign a char
- 006 — char parameter and return value
- 007 — a char decides a loop
- 008 — default value of a char is '\0'
- 009 — declaring char/i8 variable (from tests-old/data_types.md #003)

---

## 001 — print chars

```ura
// stdout: a b
// expect: 0
proto printf(fmt i8[], ...) i32

fn main() i32:
    c1 char = 'a'
    c2 i8 = 'b'
    printf("%c %c\n", c1, c2)
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private constant [7 x i8] c"%c %c\0A\00"

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %c1 = alloca i8, align 1
  %c2 = alloca i8, align 1
  %s = alloca [7 x i8], align 1
  store i8 97, i8* %c1, align 1
  store i8 98, i8* %c2, align 1
  %dst = getelementptr inbounds [7 x i8], [7 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i64 7, i1 false)
  %load.c1 = load i8, i8* %c1, align 1
  %load.c2 = load i8, i8* %c2, align 1
  %"printf(str,load.c1,load.c2)" = call i32 (i8*, ...) @printf(i8* %dst, i8 %load.c1, i8 %load.c2)
  ret i32 0
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 002 — compare chars

```ura
// expect: 2
fn main() i32:
    c char = 'x'
    n i32 = 0
    if c == 'x':
        n += 1
    if c != 'y':
        n += 1
    if c == 'z':
        n += 10
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
  %c = alloca i8, align 1
  %n = alloca i32, align 4
  store i8 120, i8* %c, align 1
  store i32 0, i32* %n, align 4
  %load.c = load i8, i8* %c, align 1
  %EQ = icmp eq i8 %load.c, 120
  br i1 %EQ, label %then, label %endif

then:                                             ; preds = %entry
  %old = load i32, i32* %n, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %load.c3 = load i8, i8* %c, align 1
  %NQ = icmp ne i8 %load.c3, 121
  br i1 %NQ, label %then2, label %endif1

then2:                                            ; preds = %endif
  %old4 = load i32, i32* %n, align 4
  %ADD_ASSIGN5 = add i32 %old4, 1
  store i32 %ADD_ASSIGN5, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %then2, %endif
  %load.c8 = load i8, i8* %c, align 1
  %EQ9 = icmp eq i8 %load.c8, 122
  br i1 %EQ9, label %then7, label %endif6

then7:                                            ; preds = %endif1
  %old10 = load i32, i32* %n, align 4
  %ADD_ASSIGN11 = add i32 %old10, 10
  store i32 %ADD_ASSIGN11, i32* %n, align 4
  br label %endif6

endif6:                                           ; preds = %then7, %endif1
  %load.n = load i32, i32* %n, align 4
  ret i32 %load.n
}
```

---

## 003 — order of chars

```ura
// expect: 3
fn main() i32:
    c char = 'm'
    n i32 = 0
    if c > 'a':
        n += 1
    if c < 'z':
        n += 1
    if c >= 'm' and c <= 'm':
        n += 1
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
  %c = alloca i8, align 1
  %n = alloca i32, align 4
  store i8 109, i8* %c, align 1
  store i32 0, i32* %n, align 4
  %load.c = load i8, i8* %c, align 1
  %GT = icmp sgt i8 %load.c, 97
  br i1 %GT, label %then, label %endif

then:                                             ; preds = %entry
  %old = load i32, i32* %n, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %load.c3 = load i8, i8* %c, align 1
  %LT = icmp slt i8 %load.c3, 122
  br i1 %LT, label %then2, label %endif1

then2:                                            ; preds = %endif
  %old4 = load i32, i32* %n, align 4
  %ADD_ASSIGN5 = add i32 %old4, 1
  store i32 %ADD_ASSIGN5, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %then2, %endif
  %load.c8 = load i8, i8* %c, align 1
  %GE = icmp sge i8 %load.c8, 109
  %load.c9 = load i8, i8* %c, align 1
  %LE = icmp sle i8 %load.c9, 109
  %AND = and i1 %GE, %LE
  br i1 %AND, label %then7, label %endif6

then7:                                            ; preds = %endif1
  %old10 = load i32, i32* %n, align 4
  %ADD_ASSIGN11 = add i32 %old10, 1
  store i32 %ADD_ASSIGN11, i32* %n, align 4
  br label %endif6

endif6:                                           ; preds = %then7, %endif1
  %load.n = load i32, i32* %n, align 4
  ret i32 %load.n
}
```

---

## 004 — escaped chars

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

## 005 — reassign a char

```ura
// stdout: q
// expect: 0
proto printf(fmt i8[], ...) i32

fn main() i32:
    c char = 'p'
    c = 'q'
    printf("%c\n", c)
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private constant [4 x i8] c"%c\0A\00"

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %c = alloca i8, align 1
  %s = alloca [4 x i8], align 1
  store i8 112, i8* %c, align 1
  store i8 113, i8* %c, align 1
  %dst = getelementptr inbounds [4 x i8], [4 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i64 4, i1 false)
  %load.c = load i8, i8* %c, align 1
  %"printf(str,load.c)" = call i32 (i8*, ...) @printf(i8* %dst, i8 %load.c)
  ret i32 0
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 006 — char parameter and return value

```ura
// stdout: Y N
// expect: 0
proto printf(fmt i8[], ...) i32

fn answer(yes bool) char:
    if yes:
        return 'Y'
    return 'N'

fn show(c char):
    printf("%c", c)

fn main() i32:
    show(answer(True))
    printf(" ")
    show(answer(False))
    printf("\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private constant [3 x i8] c"%c\00"
@str.1 = private constant [2 x i8] c" \00"
@str.2 = private constant [2 x i8] c"\0A\00"

declare i32 @printf(i8*, ...)

define i8 @answer(i1 %0) {
entry:
  %yes = alloca i1, align 1
  store i1 %0, i1* %yes, align 1
  %load.yes = load i1, i1* %yes, align 1
  br i1 %load.yes, label %then, label %endif

then:                                             ; preds = %entry
  ret i8 89

endif:                                            ; preds = %entry
  ret i8 78
}

define void @show(i8 %0) {
entry:
  %c = alloca i8, align 1
  %s = alloca [3 x i8], align 1
  store i8 %0, i8* %c, align 1
  %dst = getelementptr inbounds [3 x i8], [3 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i64 3, i1 false)
  %load.c = load i8, i8* %c, align 1
  %"printf(str,load.c)" = call i32 (i8*, ...) @printf(i8* %dst, i8 %load.c)
  ret void
}

define i32 @main() {
entry:
  %s = alloca [2 x i8], align 1
  %s1 = alloca [2 x i8], align 1
  %"answer(True)" = call i8 @answer(i1 true)
  call void @show(i8 %"answer(True)")
  %dst = getelementptr inbounds [2 x i8], [2 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i64 2, i1 false)
  %"printf(str)" = call i32 (i8*, ...) @printf(i8* %dst)
  %"answer(False)" = call i8 @answer(i1 false)
  call void @show(i8 %"answer(False)")
  %dst2 = getelementptr inbounds [2 x i8], [2 x i8]* %s1, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst2, i8* align 1 getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0), i64 2, i1 false)
  %"printf(str)3" = call i32 (i8*, ...) @printf(i8* %dst2)
  ret i32 0
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 007 — a char decides a loop

```ura
// expect: 4
fn main() i32:
    c char = 'a'
    n i32 = 0
    while c != 'e':
        n += 1
        if n == 1:
            c = 'b'
        elif n == 2:
            c = 'c'
        elif n == 3:
            c = 'd'
        else:
            c = 'e'
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
  %c = alloca i8, align 1
  %n = alloca i32, align 4
  store i8 97, i8* %c, align 1
  store i32 0, i32* %n, align 4
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.c = load i8, i8* %c, align 1
  %NQ = icmp ne i8 %load.c, 101
  br i1 %NQ, label %then, label %endwhile

then:                                             ; preds = %cond
  %old = load i32, i32* %n, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %n, align 4
  %load.n = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %load.n, 1
  br i1 %EQ, label %then1, label %next

endwhile:                                         ; preds = %cond
  %load.n10 = load i32, i32* %n, align 4
  ret i32 %load.n10

then1:                                            ; preds = %then
  store i8 98, i8* %c, align 1
  br label %endif

next:                                             ; preds = %then
  %load.n4 = load i32, i32* %n, align 4
  %EQ5 = icmp eq i32 %load.n4, 2
  br i1 %EQ5, label %then2, label %next3

then2:                                            ; preds = %next
  store i8 99, i8* %c, align 1
  br label %endif

next3:                                            ; preds = %next
  %load.n8 = load i32, i32* %n, align 4
  %EQ9 = icmp eq i32 %load.n8, 3
  br i1 %EQ9, label %then6, label %next7

then6:                                            ; preds = %next3
  store i8 100, i8* %c, align 1
  br label %endif

next7:                                            ; preds = %next3
  store i8 101, i8* %c, align 1
  br label %endif

endif:                                            ; preds = %next7, %then6, %then2, %then1
  br label %cond
}
```

---

## 008 — default value of a char is '\0'

```ura
// expect: 1
fn main() i32:
    c char
    if c == '\0':
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
  %c = alloca i8, align 1
  store i8 0, i8* %c, align 1
  %load.c = load i8, i8* %c, align 1
  %EQ = icmp eq i8 %load.c, 0
  br i1 %EQ, label %then, label %endif

then:                                             ; preds = %entry
  ret i32 1

endif:                                            ; preds = %entry
  ret i32 0
}
```

---

## 009 — declaring char/i8 variable (from tests-old/data_types.md #003)

```ura
proto exit(code i32)

fn main() i32:
    c1 char = 'a'
    c2 i8 = 'b'
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
  %c1 = alloca i8, align 1
  %c2 = alloca i8, align 1
  store i8 97, i8* %c1, align 1
  store i8 98, i8* %c2, align 1
  ret i32 0
}
```

