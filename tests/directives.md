# directives

## index

- 001 — @if/@elif/@else selects the matching platform branch
- 002 — @if unix matches on macOS and linux
- 003 — a non-matching branch is dropped before parsing
- 004 — a platform-gated top-level proto
- 005 — @if selects a value inside a function
- 006 — without @no-warn, operator drop alone warns
- 007 — @no-warn above operator drop silences that warning
- 008 — @no-warn covers only the declaration it precedes

## 001 — @if/@elif/@else selects the matching platform branch

```ura
main():
    @if macos:
        output("mac\n")
    @elif linux:
        output("linux\n")
    @else:
        output("other\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

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
└─ output : void
   └─ char[] "mac\n"
```

```out
mac
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [5 x i8] c"mac\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

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
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 4, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 002 — @if unix matches on macOS and linux

```ura
main():
    @if unix:
        output("posix\n")
    @else:
        output("windows\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

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
└─ output : void
   └─ char[] "posix\n"
```

```out
posix
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [7 x i8] c"posix\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

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
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 003 — a non-matching branch is dropped before parsing

```ura
main():
    @if linux:
        undefined_that_would_error()
    @else:
        output("kept\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

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
└─ output : void
   └─ char[] "kept\n"
```

```out
kept
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [6 x i8] c"kept\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

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
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 004 — a platform-gated top-level proto

```ura
@if unix:
    proto fn getpid() i32
@else:
    proto fn GetCurrentProcessId() u32

main():
    output(getpid() > 0, "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

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

proto fn getpid() : i32

fn main() : i32
└─ output : void
   ├─ > : bool
   │  ├─ call getpid : i32
   │  └─ int 0
   └─ char[] "\n"
```

```out
True
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1

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
  %call = call i32 @getpid()
  %gt = icmp sgt i32 %call, 0
  %bool_str = select i1 %gt, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* %bool_str, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @getpid()

declare i32 @printf(i8*, ...)
```

## 005 — @if selects a value inside a function

```ura
fn tag() i32:
    @if macos:
        return 1
    @elif linux:
        return 2
    @else:
        return 0

main():
    output(tag(), "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

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

fn tag() : i32
└─ return
   └─ int 1

fn main() : i32
└─ output : void
   ├─ call tag : i32
   └─ char[] "\n"
```

```out
1
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1

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

define i32 @tag() {
entry:
  ret i32 1
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %call = call i32 @tag()
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %call, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 006 — without @no-warn, operator drop alone warns

```ura
// directives/006.ura - without @no-warn, operator drop alone warns

struct Owned:
    tag i32

    operator drop:
        output("dropping ", self.tag, "\n")

main():
    o Owned
    o.tag = 1
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

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

struct Owned
├─ tag : i32
└─ fn Owned.drop(self : STRUCT_CALL) : void
   └─ output : void
      ├─ char[] "dropping "
      ├─ .tag : i32
      │  └─ self : STRUCT_CALL
      └─ char[] "\n"

fn main() : i32
├─ o : STRUCT_CALL
└─ = : i32
   ├─ .tag : i32
   │  └─ o : STRUCT_CALL
   └─ int 1
```

```out
dropping 1
```

```err
warning: Struct Owned has 'operator drop' but no 'operator ='; copying it makes two owners of the same memory, and both will be destroyed
  006.ura:3:1
  |
3 | struct Owned:
  | ^^^^^^
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Owned = type { i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [10 x i8] c"dropping \00", align 1
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
  ret i8* %call
}

declare i8* @getenv(i8*)

define void @Owned.drop(%Owned* %0) {
entry:
  %self = alloca %Owned*, align 8
  store %Owned* %0, %Owned** %self, align 8
  %ref = load %Owned*, %Owned** %self, align 8
  %tag = getelementptr %Owned, %Owned* %ref, i32 0, i32 0
  %tag1 = load i32, i32* %tag, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 9, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %tag1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)

define i32 @main(i32 %0, i8** %1) {
entry:
  %o = alloca %Owned, align 8
  store %Owned zeroinitializer, %Owned* %o, align 4
  %tag = getelementptr %Owned, %Owned* %o, i32 0, i32 0
  store i32 1, i32* %tag, align 4
  call void @Owned.drop(%Owned* %o)
  ret i32 0
}
```

## 007 — @no-warn above operator drop silences that warning

```ura
// directives/007.ura - @no-warn above operator drop silences that warning

struct Owned:
    tag i32

    @no-warn
    operator drop:
        output("dropping ", self.tag, "\n")

main():
    o Owned
    o.tag = 1
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

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

struct Owned
├─ tag : i32
└─ fn Owned.drop(self : STRUCT_CALL) : void
   └─ output : void
      ├─ char[] "dropping "
      ├─ .tag : i32
      │  └─ self : STRUCT_CALL
      └─ char[] "\n"

fn main() : i32
├─ o : STRUCT_CALL
└─ = : i32
   ├─ .tag : i32
   │  └─ o : STRUCT_CALL
   └─ int 1
```

```out
dropping 1
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Owned = type { i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [10 x i8] c"dropping \00", align 1
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
  ret i8* %call
}

declare i8* @getenv(i8*)

define void @Owned.drop(%Owned* %0) {
entry:
  %self = alloca %Owned*, align 8
  store %Owned* %0, %Owned** %self, align 8
  %ref = load %Owned*, %Owned** %self, align 8
  %tag = getelementptr %Owned, %Owned* %ref, i32 0, i32 0
  %tag1 = load i32, i32* %tag, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 9, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %tag1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)

define i32 @main(i32 %0, i8** %1) {
entry:
  %o = alloca %Owned, align 8
  store %Owned zeroinitializer, %Owned* %o, align 4
  %tag = getelementptr %Owned, %Owned* %o, i32 0, i32 0
  store i32 1, i32* %tag, align 4
  call void @Owned.drop(%Owned* %o)
  ret i32 0
}
```

## 008 — @no-warn covers only the declaration it precedes

```ura
// directives/008.ura - @no-warn covers only the declaration it precedes

struct Quiet:
    tag i32

    @no-warn
    operator drop:
        output("quiet ", self.tag, "\n")

struct Loud:
    tag i32

    operator drop:
        output("loud ", self.tag, "\n")

main():
    q Quiet
    l Loud
    q.tag = 1
    l.tag = 2
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

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

struct Quiet
├─ tag : i32
└─ fn Quiet.drop(self : STRUCT_CALL) : void
   └─ output : void
      ├─ char[] "quiet "
      ├─ .tag : i32
      │  └─ self : STRUCT_CALL
      └─ char[] "\n"

struct Loud
├─ tag : i32
└─ fn Loud.drop(self : STRUCT_CALL) : void
   └─ output : void
      ├─ char[] "loud "
      ├─ .tag : i32
      │  └─ self : STRUCT_CALL
      └─ char[] "\n"

fn main() : i32
├─ q : STRUCT_CALL
├─ l : STRUCT_CALL
├─ = : i32
│  ├─ .tag : i32
│  │  └─ q : STRUCT_CALL
│  └─ int 1
└─ = : i32
   ├─ .tag : i32
   │  └─ l : STRUCT_CALL
   └─ int 2
```

```out
loud 2
quiet 1
```

```err
warning: Struct Loud has 'operator drop' but no 'operator ='; copying it makes two owners of the same memory, and both will be destroyed
   008.ura:10:1
   |
10 | struct Loud:
   | ^^^^^^
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Quiet = type { i32 }
%Loud = type { i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [7 x i8] c"quiet \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"loud \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1

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

define void @Quiet.drop(%Quiet* %0) {
entry:
  %self = alloca %Quiet*, align 8
  store %Quiet* %0, %Quiet** %self, align 8
  %ref = load %Quiet*, %Quiet** %self, align 8
  %tag = getelementptr %Quiet, %Quiet* %ref, i32 0, i32 0
  %tag1 = load i32, i32* %tag, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %tag1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)

define void @Loud.drop(%Loud* %0) {
entry:
  %self = alloca %Loud*, align 8
  store %Loud* %0, %Loud** %self, align 8
  %ref = load %Loud*, %Loud** %self, align 8
  %tag = getelementptr %Loud, %Loud* %ref, i32 0, i32 0
  %tag1 = load i32, i32* %tag, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.4, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0), i32 %tag1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  ret void
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %q = alloca %Quiet, align 8
  store %Quiet zeroinitializer, %Quiet* %q, align 4
  %l = alloca %Loud, align 8
  store %Loud zeroinitializer, %Loud* %l, align 4
  %tag = getelementptr %Quiet, %Quiet* %q, i32 0, i32 0
  store i32 1, i32* %tag, align 4
  %tag1 = getelementptr %Loud, %Loud* %l, i32 0, i32 0
  store i32 2, i32* %tag1, align 4
  call void @Loud.drop(%Loud* %l)
  call void @Quiet.drop(%Quiet* %q)
  ret i32 0
}
```
