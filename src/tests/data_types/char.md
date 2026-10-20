# char

## index

- 001 — escaped chars

---

## 001 — escaped chars

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

