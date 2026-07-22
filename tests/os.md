# os

## index

- 001 — argc and argv from the global os

## 001 — argc and argv from the global os

```ura
// os/001.ura - argc and argv from the global os

main():
    output("argc: ", os.argc, "\n")
    output("argv: ", os.argv.len, "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

fn main() : i32
├─ output : void
│  ├─ char[] "argc: "
│  ├─ .argc : i32
│  │  └─ os : STRUCT_CALL
│  └─ char[] "\n"
└─ output : void
   ├─ char[] "argv: "
   ├─ .len : u64
   │  └─ .argv : char[][]
   │     └─ os : STRUCT_CALL
   └─ char[] "\n"
```

```out
argc: 1
argv: 1
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [7 x i8] c"argc: \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.2 = private unnamed_addr constant [7 x i8] c"argv: \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [13 x i8] c"%.*s%llu%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  store i32 %0, i32* getelementptr inbounds (%Os, %Os* @os, i32 0, i32 0), align 4
  %argc64 = sext i32 %0 to i64
  %argvbuf = call i8* @calloc(i64 %argc64, i64 16)
  %argv.buf = bitcast i8* %argvbuf to { i8*, i64 }*
  %oi = alloca i64, align 8
  store i64 0, i64* %oi, align 8
  br label %os.cond

os.cond:                                          ; preds = %os.body, %entry
  %i = load i64, i64* %oi, align 8
  %more = icmp slt i64 %i, %argc64
  br i1 %more, label %os.body, label %os.end

os.body:                                          ; preds = %os.cond
  %ap = getelementptr i8*, i8** %1, i64 %i
  %arg = load i8*, i8** %ap, align 8
  %alen = call i64 @strlen(i8* %arg)
  %a.ptr = insertvalue { i8*, i64 } undef, i8* %arg, 0
  %a.len = insertvalue { i8*, i64 } %a.ptr, i64 %alen, 1
  %slot = getelementptr { i8*, i64 }, { i8*, i64 }* %argv.buf, i64 %i
  store { i8*, i64 } %a.len, { i8*, i64 }* %slot, align 8
  %inc = add i64 %i, 1
  store i64 %inc, i64* %oi, align 8
  br label %os.cond

os.end:                                           ; preds = %os.cond
  %argv.ptr = insertvalue { { i8*, i64 }*, i64 } undef, { i8*, i64 }* %argv.buf, 0
  %argv.len = insertvalue { { i8*, i64 }*, i64 } %argv.ptr, i64 %argc64, 1
  store { { i8*, i64 }*, i64 } %argv.len, { { i8*, i64 }*, i64 }* getelementptr inbounds (%Os, %Os* @os, i32 0, i32 1), align 8
  %argc = load i32, i32* getelementptr inbounds (%Os, %Os* @os, i32 0, i32 0), align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %argc, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %argv = load { { i8*, i64 }*, i64 }, { { i8*, i64 }*, i64 }* getelementptr inbounds (%Os, %Os* @os, i32 0, i32 1), align 8
  %len = extractvalue { { i8*, i64 }*, i64 } %argv, 1
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt.4, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.2, i32 0, i32 0), i64 %len, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i64 @strlen(i8*)

declare i32 @printf(i8*, ...)
```
