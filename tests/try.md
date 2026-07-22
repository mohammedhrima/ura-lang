# try

## index

- 001 — a throw is caught in the same function
- 002 — a throw propagates across calls to a caller's catch
- 003 — a rethrow inside a catch propagates outward
- 004 — a nested try: the inner catch handles it
- 005 — return inside a try returns from the function
- 006 — break inside a try exits the enclosing loop
- 007 — an uncaught throw prints its message and exits
- 008 — throwing a non-Error is a compile error
- 009 — catch without a matching try is a parse error
- 010 — code after a throw is unreachable and dropped

## 001 — a throw is caught in the same function

```ura
// try/001 — a throw is caught in the same function
use "@/error"

main():
    try:
        throw Error::make("boom")
    catch e:
        output("caught: ", e.message, "\n")
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

struct Error
├─ message : char[]
└─ fn Error.make(msg : array) : STRUCT_CALL
   ├─ e : STRUCT_CALL
   ├─ = : array
   │  ├─ .message : char[]
   │  │  └─ e : STRUCT_CALL
   │  └─ msg : char[]
   └─ return
      └─ e : STRUCT_CALL

fn main() : i32
└─ try
   ├─ throw
   │  └─ call make : STRUCT_CALL
   │     └─ char[] "boom"
   └─ catch
      ├─ binds e : STRUCT_CALL
      └─ output : void
         ├─ char[] "caught: "
         ├─ .message : char[]
         │  └─ e : STRUCT_CALL
         └─ char[] "\n"
```

```out
caught: boom
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Error = type { { i8*, i64 } }

@os = internal global %Os zeroinitializer
@ura.err_flag = internal global i1 false
@ura.err_value = internal global %Error zeroinitializer
@str = private unnamed_addr constant [5 x i8] c"boom\00", align 1
@str.1 = private unnamed_addr constant [9 x i8] c"caught: \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [13 x i8] c"%.*s%.*s%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  ret i8* null

throw.cont:                                       ; preds = %entry
  ret i8* %call
}

declare i8* @getenv(i8*)

define %Error @Error.make({ i8*, i64 } %0) {
entry:
  %msg = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %0, { i8*, i64 }* %msg, align 8
  %e = alloca %Error, align 8
  store %Error zeroinitializer, %Error* %e, align 8
  %message = getelementptr %Error, %Error* %e, i32 0, i32 0
  %msg1 = load { i8*, i64 }, { i8*, i64 }* %msg, align 8
  store { i8*, i64 } %msg1, { i8*, i64 }* %message, align 8
  %e2 = load %Error, %Error* %e, align 8
  ret %Error %e2
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %call = call %Error @Error.make({ i8*, i64 } { i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i64 4 })
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

catch:                                            ; preds = %throw.cont, %throw.unwind
  %e = alloca %Error, align 8
  %ev = load %Error, %Error* @ura.err_value, align 8
  store %Error %ev, %Error* %e, align 8
  store i1 false, i1* @ura.err_flag, align 1
  %message = getelementptr %Error, %Error* %e, i32 0, i32 0
  %message1 = load { i8*, i64 }, { i8*, i64 }* %message, align 8
  %str.len = extractvalue { i8*, i64 } %message1, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %message1, 0
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0), i32 %len32, i8* %str.data, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  br label %try.end

try.end:                                          ; preds = %catch
  ret i32 0

throw.unwind:                                     ; preds = %entry
  br label %catch

throw.cont:                                       ; preds = %entry
  store %Error %call, %Error* @ura.err_value, align 8
  store i1 true, i1* @ura.err_flag, align 1
  br label %catch
}

declare i32 @printf(i8*, ...)
```

## 002 — a throw propagates across calls to a caller's catch

```ura
// try/002 — a throw propagates across calls to a caller's catch
use "@/error"

fn deep() void:
    throw Error::make("from deep")

fn middle() void:
    deep()

main():
    try:
        middle()
    catch e:
        output("caught: ", e.message, "\n")
    output("after\n")
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

struct Error
├─ message : char[]
└─ fn Error.make(msg : array) : STRUCT_CALL
   ├─ e : STRUCT_CALL
   ├─ = : array
   │  ├─ .message : char[]
   │  │  └─ e : STRUCT_CALL
   │  └─ msg : char[]
   └─ return
      └─ e : STRUCT_CALL

fn deep() : void
└─ throw
   └─ call make : STRUCT_CALL
      └─ char[] "from deep"

fn middle() : void
└─ call deep : void

fn main() : i32
├─ try
│  ├─ call middle : void
│  └─ catch
│     ├─ binds e : STRUCT_CALL
│     └─ output : void
│        ├─ char[] "caught: "
│        ├─ .message : char[]
│        │  └─ e : STRUCT_CALL
│        └─ char[] "\n"
└─ output : void
   └─ char[] "after\n"
```

```out
caught: from deep
after
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Error = type { { i8*, i64 } }

@os = internal global %Os zeroinitializer
@ura.err_flag = internal global i1 false
@ura.err_value = internal global %Error zeroinitializer
@str = private unnamed_addr constant [10 x i8] c"from deep\00", align 1
@str.1 = private unnamed_addr constant [9 x i8] c"caught: \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [13 x i8] c"%.*s%.*s%.*s\00", align 1
@str.3 = private unnamed_addr constant [7 x i8] c"after\0A\00", align 1
@fmt.4 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  ret i8* null

throw.cont:                                       ; preds = %entry
  ret i8* %call
}

declare i8* @getenv(i8*)

define %Error @Error.make({ i8*, i64 } %0) {
entry:
  %msg = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %0, { i8*, i64 }* %msg, align 8
  %e = alloca %Error, align 8
  store %Error zeroinitializer, %Error* %e, align 8
  %message = getelementptr %Error, %Error* %e, i32 0, i32 0
  %msg1 = load { i8*, i64 }, { i8*, i64 }* %msg, align 8
  store { i8*, i64 } %msg1, { i8*, i64 }* %message, align 8
  %e2 = load %Error, %Error* %e, align 8
  ret %Error %e2
}

define void @deep() {
entry:
  %call = call %Error @Error.make({ i8*, i64 } { i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i64 9 })
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  ret void

throw.cont:                                       ; preds = %entry
  store %Error %call, %Error* @ura.err_value, align 8
  store i1 true, i1* @ura.err_flag, align 1
  ret void
}

define void @middle() {
entry:
  call void @deep()
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  ret void

throw.cont:                                       ; preds = %entry
  ret void
}

define i32 @main(i32 %0, i8** %1) {
entry:
  call void @middle()
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

catch:                                            ; preds = %throw.unwind
  %e = alloca %Error, align 8
  %ev = load %Error, %Error* @ura.err_value, align 8
  store %Error %ev, %Error* %e, align 8
  store i1 false, i1* @ura.err_flag, align 1
  %message = getelementptr %Error, %Error* %e, i32 0, i32 0
  %message1 = load { i8*, i64 }, { i8*, i64 }* %message, align 8
  %str.len = extractvalue { i8*, i64 } %message1, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %message1, 0
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0), i32 %len32, i8* %str.data, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  br label %try.end

try.end:                                          ; preds = %catch, %throw.cont
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.4, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.3, i32 0, i32 0))
  ret i32 0

throw.unwind:                                     ; preds = %entry
  br label %catch

throw.cont:                                       ; preds = %entry
  br label %try.end
}

declare i32 @printf(i8*, ...)
```

## 003 — a rethrow inside a catch propagates outward

```ura
// try/003 — a rethrow inside a catch propagates outward
use "@/error"

fn middle() void:
    try:
        throw Error::make("first")
    catch e:
        output("mid: ", e.message, "\n")
        throw Error::make("second")

main():
    try:
        middle()
    catch e:
        output("main: ", e.message, "\n")
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

struct Error
├─ message : char[]
└─ fn Error.make(msg : array) : STRUCT_CALL
   ├─ e : STRUCT_CALL
   ├─ = : array
   │  ├─ .message : char[]
   │  │  └─ e : STRUCT_CALL
   │  └─ msg : char[]
   └─ return
      └─ e : STRUCT_CALL

fn middle() : void
└─ try
   ├─ throw
   │  └─ call make : STRUCT_CALL
   │     └─ char[] "first"
   └─ catch
      ├─ binds e : STRUCT_CALL
      ├─ output : void
      │  ├─ char[] "mid: "
      │  ├─ .message : char[]
      │  │  └─ e : STRUCT_CALL
      │  └─ char[] "\n"
      └─ throw
         └─ call make : STRUCT_CALL
            └─ char[] "second"

fn main() : i32
└─ try
   ├─ call middle : void
   └─ catch
      ├─ binds e : STRUCT_CALL
      └─ output : void
         ├─ char[] "main: "
         ├─ .message : char[]
         │  └─ e : STRUCT_CALL
         └─ char[] "\n"
```

```out
mid: first
main: second
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Error = type { { i8*, i64 } }

@os = internal global %Os zeroinitializer
@ura.err_flag = internal global i1 false
@ura.err_value = internal global %Error zeroinitializer
@str = private unnamed_addr constant [6 x i8] c"first\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"mid: \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [13 x i8] c"%.*s%.*s%.*s\00", align 1
@str.3 = private unnamed_addr constant [7 x i8] c"second\00", align 1
@str.4 = private unnamed_addr constant [7 x i8] c"main: \00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [13 x i8] c"%.*s%.*s%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  ret i8* null

throw.cont:                                       ; preds = %entry
  ret i8* %call
}

declare i8* @getenv(i8*)

define %Error @Error.make({ i8*, i64 } %0) {
entry:
  %msg = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %0, { i8*, i64 }* %msg, align 8
  %e = alloca %Error, align 8
  store %Error zeroinitializer, %Error* %e, align 8
  %message = getelementptr %Error, %Error* %e, i32 0, i32 0
  %msg1 = load { i8*, i64 }, { i8*, i64 }* %msg, align 8
  store { i8*, i64 } %msg1, { i8*, i64 }* %message, align 8
  %e2 = load %Error, %Error* %e, align 8
  ret %Error %e2
}

define void @middle() {
entry:
  %call = call %Error @Error.make({ i8*, i64 } { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i64 5 })
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

catch:                                            ; preds = %throw.cont, %throw.unwind
  %e = alloca %Error, align 8
  %ev = load %Error, %Error* @ura.err_value, align 8
  store %Error %ev, %Error* %e, align 8
  store i1 false, i1* @ura.err_flag, align 1
  %message = getelementptr %Error, %Error* %e, i32 0, i32 0
  %message1 = load { i8*, i64 }, { i8*, i64 }* %message, align 8
  %str.len = extractvalue { i8*, i64 } %message1, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %message1, 0
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i32 %len32, i8* %str.data, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %call2 = call %Error @Error.make({ i8*, i64 } { i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.3, i32 0, i32 0), i64 6 })
  %eflag5 = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag5, label %throw.unwind3, label %throw.cont4

try.end:                                          ; No predecessors!
  ret void

throw.unwind:                                     ; preds = %entry
  br label %catch

throw.cont:                                       ; preds = %entry
  store %Error %call, %Error* @ura.err_value, align 8
  store i1 true, i1* @ura.err_flag, align 1
  br label %catch

throw.unwind3:                                    ; preds = %catch
  ret void

throw.cont4:                                      ; preds = %catch
  store %Error %call2, %Error* @ura.err_value, align 8
  store i1 true, i1* @ura.err_flag, align 1
  ret void
}

declare i32 @printf(i8*, ...)

define i32 @main(i32 %0, i8** %1) {
entry:
  call void @middle()
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

catch:                                            ; preds = %throw.unwind
  %e = alloca %Error, align 8
  %ev = load %Error, %Error* @ura.err_value, align 8
  store %Error %ev, %Error* %e, align 8
  store i1 false, i1* @ura.err_flag, align 1
  %message = getelementptr %Error, %Error* %e, i32 0, i32 0
  %message1 = load { i8*, i64 }, { i8*, i64 }* %message, align 8
  %str.len = extractvalue { i8*, i64 } %message1, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %message1, 0
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt.6, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.4, i32 0, i32 0), i32 %len32, i8* %str.data, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  br label %try.end

try.end:                                          ; preds = %catch, %throw.cont
  ret i32 0

throw.unwind:                                     ; preds = %entry
  br label %catch

throw.cont:                                       ; preds = %entry
  br label %try.end
}
```

## 004 — a nested try: the inner catch handles it

```ura
// try/004 — a nested try: the inner catch handles it
use "@/error"

main():
    try:
        try:
            throw Error::make("inner")
        catch e:
            output("inner: ", e.message, "\n")
    catch e:
        output("outer: ", e.message, "\n")
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

struct Error
├─ message : char[]
└─ fn Error.make(msg : array) : STRUCT_CALL
   ├─ e : STRUCT_CALL
   ├─ = : array
   │  ├─ .message : char[]
   │  │  └─ e : STRUCT_CALL
   │  └─ msg : char[]
   └─ return
      └─ e : STRUCT_CALL

fn main() : i32
└─ try
   ├─ try
   │  ├─ throw
   │  │  └─ call make : STRUCT_CALL
   │  │     └─ char[] "inner"
   │  └─ catch
   │     ├─ binds e : STRUCT_CALL
   │     └─ output : void
   │        ├─ char[] "inner: "
   │        ├─ .message : char[]
   │        │  └─ e : STRUCT_CALL
   │        └─ char[] "\n"
   └─ catch
      ├─ binds e : STRUCT_CALL
      └─ output : void
         ├─ char[] "outer: "
         ├─ .message : char[]
         │  └─ e : STRUCT_CALL
         └─ char[] "\n"
```

```out
inner: inner
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Error = type { { i8*, i64 } }

@os = internal global %Os zeroinitializer
@ura.err_flag = internal global i1 false
@ura.err_value = internal global %Error zeroinitializer
@str = private unnamed_addr constant [6 x i8] c"inner\00", align 1
@str.1 = private unnamed_addr constant [8 x i8] c"inner: \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [13 x i8] c"%.*s%.*s%.*s\00", align 1
@str.3 = private unnamed_addr constant [8 x i8] c"outer: \00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.5 = private unnamed_addr constant [13 x i8] c"%.*s%.*s%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  ret i8* null

throw.cont:                                       ; preds = %entry
  ret i8* %call
}

declare i8* @getenv(i8*)

define %Error @Error.make({ i8*, i64 } %0) {
entry:
  %msg = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %0, { i8*, i64 }* %msg, align 8
  %e = alloca %Error, align 8
  store %Error zeroinitializer, %Error* %e, align 8
  %message = getelementptr %Error, %Error* %e, i32 0, i32 0
  %msg1 = load { i8*, i64 }, { i8*, i64 }* %msg, align 8
  store { i8*, i64 } %msg1, { i8*, i64 }* %message, align 8
  %e2 = load %Error, %Error* %e, align 8
  ret %Error %e2
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %call = call %Error @Error.make({ i8*, i64 } { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i64 5 })
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

catch:                                            ; No predecessors!
  %e4 = alloca %Error, align 8
  %ev5 = load %Error, %Error* @ura.err_value, align 8
  store %Error %ev5, %Error* %e4, align 8
  store i1 false, i1* @ura.err_flag, align 1
  %message6 = getelementptr %Error, %Error* %e4, i32 0, i32 0
  %message7 = load { i8*, i64 }, { i8*, i64 }* %message6, align 8
  %str.len8 = extractvalue { i8*, i64 } %message7, 1
  %len329 = trunc i64 %str.len8 to i32
  %str.data10 = extractvalue { i8*, i64 } %message7, 0
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt.5, i32 0, i32 0), i32 7, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.3, i32 0, i32 0), i32 %len329, i8* %str.data10, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  br label %try.end

try.end:                                          ; preds = %catch, %try.end2
  ret i32 0

catch1:                                           ; preds = %throw.cont, %throw.unwind
  %e = alloca %Error, align 8
  %ev = load %Error, %Error* @ura.err_value, align 8
  store %Error %ev, %Error* %e, align 8
  store i1 false, i1* @ura.err_flag, align 1
  %message = getelementptr %Error, %Error* %e, i32 0, i32 0
  %message3 = load { i8*, i64 }, { i8*, i64 }* %message, align 8
  %str.len = extractvalue { i8*, i64 } %message3, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %message3, 0
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i32 7, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.1, i32 0, i32 0), i32 %len32, i8* %str.data, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  br label %try.end2

try.end2:                                         ; preds = %catch1
  br label %try.end

throw.unwind:                                     ; preds = %entry
  br label %catch1

throw.cont:                                       ; preds = %entry
  store %Error %call, %Error* @ura.err_value, align 8
  store i1 true, i1* @ura.err_flag, align 1
  br label %catch1
}

declare i32 @printf(i8*, ...)
```

## 005 — return inside a try returns from the function

```ura
// try/005 — return inside a try returns from the function
use "@/error"

fn pick() i32:
    try:
        return 5
    catch e:
        return 0

main():
    output("ret = ", pick(), "\n")
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

struct Error
├─ message : char[]
└─ fn Error.make(msg : array) : STRUCT_CALL
   ├─ e : STRUCT_CALL
   ├─ = : array
   │  ├─ .message : char[]
   │  │  └─ e : STRUCT_CALL
   │  └─ msg : char[]
   └─ return
      └─ e : STRUCT_CALL

fn pick() : i32
└─ try
   ├─ return
   │  └─ int 5
   └─ catch
      ├─ binds e : STRUCT_CALL
      └─ return
         └─ int 0

fn main() : i32
└─ output : void
   ├─ char[] "ret = "
   ├─ call pick : i32
   └─ char[] "\n"
```

```out
ret = 5
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Error = type { { i8*, i64 } }

@os = internal global %Os zeroinitializer
@ura.err_flag = internal global i1 false
@ura.err_value = internal global %Error zeroinitializer
@str = private unnamed_addr constant [7 x i8] c"ret = \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  ret i8* null

throw.cont:                                       ; preds = %entry
  ret i8* %call
}

declare i8* @getenv(i8*)

define %Error @Error.make({ i8*, i64 } %0) {
entry:
  %msg = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %0, { i8*, i64 }* %msg, align 8
  %e = alloca %Error, align 8
  store %Error zeroinitializer, %Error* %e, align 8
  %message = getelementptr %Error, %Error* %e, i32 0, i32 0
  %msg1 = load { i8*, i64 }, { i8*, i64 }* %msg, align 8
  store { i8*, i64 } %msg1, { i8*, i64 }* %message, align 8
  %e2 = load %Error, %Error* %e, align 8
  ret %Error %e2
}

define i32 @pick() {
entry:
  ret i32 5

catch:                                            ; No predecessors!
  %e = alloca %Error, align 8
  %ev = load %Error, %Error* @ura.err_value, align 8
  store %Error %ev, %Error* %e, align 8
  store i1 false, i1* @ura.err_flag, align 1
  ret i32 0

try.end:                                          ; No predecessors!
  ret i32 0
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %call = call i32 @pick()
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  %msg = load { i8*, i64 }, { i8*, i64 }* getelementptr inbounds (%Error, %Error* @ura.err_value, i32 0, i32 0), align 8
  %msg.data = extractvalue { i8*, i64 } %msg, 0
  %msg.len = extractvalue { i8*, i64 } %msg, 1
  %2 = call i64 @write(i32 2, i8* %msg.data, i64 %msg.len)
  call void @exit(i32 1)
  unreachable

throw.cont:                                       ; preds = %entry
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %call, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)

declare i32 @printf(i8*, ...)
```

## 006 — break inside a try exits the enclosing loop

```ura
// try/006 — break inside a try exits the enclosing loop
use "@/error"

main():
    for i in 0..3:
        try:
            if i == 1:
                break
            output("i=", i, "\n")
        catch e:
            output("x\n")
    output("done\n")
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

struct Error
├─ message : char[]
└─ fn Error.make(msg : array) : STRUCT_CALL
   ├─ e : STRUCT_CALL
   ├─ = : array
   │  ├─ .message : char[]
   │  │  └─ e : STRUCT_CALL
   │  └─ msg : char[]
   └─ return
      └─ e : STRUCT_CALL

fn main() : i32
├─ for
│  ├─ i : i32
│  ├─ range : i32
│  │  ├─ int 0
│  │  └─ int 3
│  └─ try
│     ├─ if
│     │  ├─ condition == : bool
│     │  │  ├─ i : i32
│     │  │  └─ int 1
│     │  └─ break
│     ├─ output : void
│     │  ├─ char[] "i="
│     │  ├─ i : i32
│     │  └─ char[] "\n"
│     └─ catch
│        ├─ binds e : STRUCT_CALL
│        └─ output : void
│           └─ char[] "x\n"
└─ output : void
   └─ char[] "done\n"
```

```out
i=0
done
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Error = type { { i8*, i64 } }

@os = internal global %Os zeroinitializer
@ura.err_flag = internal global i1 false
@ura.err_value = internal global %Error zeroinitializer
@str = private unnamed_addr constant [3 x i8] c"i=\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.2 = private unnamed_addr constant [3 x i8] c"x\0A\00", align 1
@fmt.3 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.4 = private unnamed_addr constant [6 x i8] c"done\0A\00", align 1
@fmt.5 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  ret i8* null

throw.cont:                                       ; preds = %entry
  ret i8* %call
}

declare i8* @getenv(i8*)

define %Error @Error.make({ i8*, i64 } %0) {
entry:
  %msg = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %0, { i8*, i64 }* %msg, align 8
  %e = alloca %Error, align 8
  store %Error zeroinitializer, %Error* %e, align 8
  %message = getelementptr %Error, %Error* %e, i32 0, i32 0
  %msg1 = load { i8*, i64 }, { i8*, i64 }* %msg, align 8
  store { i8*, i64 } %msg1, { i8*, i64 }* %message, align 8
  %e2 = load %Error, %Error* %e, align 8
  ret %Error %e2
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i1 = load i32, i32* %i, align 4
  %lt = icmp slt i32 %i1, 3
  %gt = icmp sgt i32 %i1, 3
  %more = select i1 true, i1 %lt, i1 %gt
  br i1 %more, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %i2 = load i32, i32* %i, align 4
  %eq = icmp eq i32 %i2, 1
  br i1 %eq, label %then, label %endif

for.inc:                                          ; preds = %try.end
  %i4 = load i32, i32* %i, align 4
  %next = add i32 %i4, 1
  store i32 %next, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %then, %for.cond
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.5, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.4, i32 0, i32 0))
  ret i32 0

catch:                                            ; No predecessors!
  %e = alloca %Error, align 8
  %ev = load %Error, %Error* @ura.err_value, align 8
  store %Error %ev, %Error* %e, align 8
  store i1 false, i1* @ura.err_flag, align 1
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.3, i32 0, i32 0), i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.2, i32 0, i32 0))
  br label %try.end

try.end:                                          ; preds = %catch, %endif
  br label %for.inc

endif:                                            ; preds = %for.body
  %i3 = load i32, i32* %i, align 4
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i32 %i3, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  br label %try.end

then:                                             ; preds = %for.body
  br label %for.end
}

declare i32 @printf(i8*, ...)
```

## 007 — an uncaught throw prints its message and exits

```ura
// try/007 — an uncaught throw prints its message and exits
use "@/error"

main():
    throw Error::make("uncaught\n")
    output("unreachable\n")
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

struct Error
├─ message : char[]
└─ fn Error.make(msg : array) : STRUCT_CALL
   ├─ e : STRUCT_CALL
   ├─ = : array
   │  ├─ .message : char[]
   │  │  └─ e : STRUCT_CALL
   │  └─ msg : char[]
   └─ return
      └─ e : STRUCT_CALL

fn main() : i32
├─ throw
│  └─ call make : STRUCT_CALL
│     └─ char[] "uncaught\n"
└─ output : void
   └─ char[] "unreachable\n"
```

```out
```

```err
uncaught
exit: 1
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Error = type { { i8*, i64 } }

@os = internal global %Os zeroinitializer
@ura.err_flag = internal global i1 false
@ura.err_value = internal global %Error zeroinitializer
@str = private unnamed_addr constant [10 x i8] c"uncaught\0A\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  ret i8* null

throw.cont:                                       ; preds = %entry
  ret i8* %call
}

declare i8* @getenv(i8*)

define %Error @Error.make({ i8*, i64 } %0) {
entry:
  %msg = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %0, { i8*, i64 }* %msg, align 8
  %e = alloca %Error, align 8
  store %Error zeroinitializer, %Error* %e, align 8
  %message = getelementptr %Error, %Error* %e, i32 0, i32 0
  %msg1 = load { i8*, i64 }, { i8*, i64 }* %msg, align 8
  store { i8*, i64 } %msg1, { i8*, i64 }* %message, align 8
  %e2 = load %Error, %Error* %e, align 8
  ret %Error %e2
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %call = call %Error @Error.make({ i8*, i64 } { i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i64 9 })
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  %msg = load { i8*, i64 }, { i8*, i64 }* getelementptr inbounds (%Error, %Error* @ura.err_value, i32 0, i32 0), align 8
  %msg.data = extractvalue { i8*, i64 } %msg, 0
  %msg.len = extractvalue { i8*, i64 } %msg, 1
  %2 = call i64 @write(i32 2, i8* %msg.data, i64 %msg.len)
  call void @exit(i32 1)
  unreachable

throw.cont:                                       ; preds = %entry
  store %Error %call, %Error* @ura.err_value, align 8
  store i1 true, i1* @ura.err_flag, align 1
  %msg1 = load { i8*, i64 }, { i8*, i64 }* getelementptr inbounds (%Error, %Error* @ura.err_value, i32 0, i32 0), align 8
  %msg.data2 = extractvalue { i8*, i64 } %msg1, 0
  %msg.len3 = extractvalue { i8*, i64 } %msg1, 1
  %3 = call i64 @write(i32 2, i8* %msg.data2, i64 %msg.len3)
  call void @exit(i32 1)
  unreachable
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)
```

## 008 — throwing a non-Error is a compile error

```ura
// try/008 — throwing a non-Error is a compile error
use "@/error"

main():
    throw 5
```

```tree
```

```out
```

```err
error: 'throw' expects an Error, got i32
  008.ura:5:5
  |
5 |     throw 5
  |     ^^^^^
```

```ll
```

## 009 — catch without a matching try is a parse error

```ura
// try/009 — catch without a matching try is a parse error
main():
    catch e:
        output("x\n")
```

```tree
```

```out
```

```err
error: 'catch' without a matching 'try'
  009.ura:3:5
  |
3 |     catch e:
  |     ^^^^^
```

```ll
```

## 010 — code after a throw is unreachable and dropped

```ura
// try/010 — code after a throw is unreachable and dropped
use "@/error"

fn boom() void:
    throw Error::make("x")
    output("dead\n")

main():
    try:
        boom()
    catch e:
        output("caught: ", e.message, "\n")
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

struct Error
├─ message : char[]
└─ fn Error.make(msg : array) : STRUCT_CALL
   ├─ e : STRUCT_CALL
   ├─ = : array
   │  ├─ .message : char[]
   │  │  └─ e : STRUCT_CALL
   │  └─ msg : char[]
   └─ return
      └─ e : STRUCT_CALL

fn boom() : void
├─ throw
│  └─ call make : STRUCT_CALL
│     └─ char[] "x"
└─ output : void
   └─ char[] "dead\n"

fn main() : i32
└─ try
   ├─ call boom : void
   └─ catch
      ├─ binds e : STRUCT_CALL
      └─ output : void
         ├─ char[] "caught: "
         ├─ .message : char[]
         │  └─ e : STRUCT_CALL
         └─ char[] "\n"
```

```out
caught: x
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Error = type { { i8*, i64 } }

@os = internal global %Os zeroinitializer
@ura.err_flag = internal global i1 false
@ura.err_value = internal global %Error zeroinitializer
@str = private unnamed_addr constant [2 x i8] c"x\00", align 1
@str.1 = private unnamed_addr constant [9 x i8] c"caught: \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [13 x i8] c"%.*s%.*s%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  ret i8* null

throw.cont:                                       ; preds = %entry
  ret i8* %call
}

declare i8* @getenv(i8*)

define %Error @Error.make({ i8*, i64 } %0) {
entry:
  %msg = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %0, { i8*, i64 }* %msg, align 8
  %e = alloca %Error, align 8
  store %Error zeroinitializer, %Error* %e, align 8
  %message = getelementptr %Error, %Error* %e, i32 0, i32 0
  %msg1 = load { i8*, i64 }, { i8*, i64 }* %msg, align 8
  store { i8*, i64 } %msg1, { i8*, i64 }* %message, align 8
  %e2 = load %Error, %Error* %e, align 8
  ret %Error %e2
}

define void @boom() {
entry:
  %call = call %Error @Error.make({ i8*, i64 } { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i64 1 })
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

throw.unwind:                                     ; preds = %entry
  ret void

throw.cont:                                       ; preds = %entry
  store %Error %call, %Error* @ura.err_value, align 8
  store i1 true, i1* @ura.err_flag, align 1
  ret void
}

define i32 @main(i32 %0, i8** %1) {
entry:
  call void @boom()
  %eflag = load i1, i1* @ura.err_flag, align 1
  br i1 %eflag, label %throw.unwind, label %throw.cont

catch:                                            ; preds = %throw.unwind
  %e = alloca %Error, align 8
  %ev = load %Error, %Error* @ura.err_value, align 8
  store %Error %ev, %Error* %e, align 8
  store i1 false, i1* @ura.err_flag, align 1
  %message = getelementptr %Error, %Error* %e, i32 0, i32 0
  %message1 = load { i8*, i64 }, { i8*, i64 }* %message, align 8
  %str.len = extractvalue { i8*, i64 } %message1, 1
  %len32 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %message1, 0
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0), i32 %len32, i8* %str.data, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  br label %try.end

try.end:                                          ; preds = %catch, %throw.cont
  ret i32 0

throw.unwind:                                     ; preds = %entry
  br label %catch

throw.cont:                                       ; preds = %entry
  br label %try.end
}

declare i32 @printf(i8*, ...)
```
