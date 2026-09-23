# imports

## index

- 001 — a relative `use` pulls in a sibling file
- 002 — `use` resolves against the importing file, including a subdirectory
- 003 — a diamond loads the shared file once, and says nothing
- 004 — an import cycle warns and still compiles
- 005 — `use` of a file that does not exist
- 006 — `use "@/..."` resolves against the standard library

## 001 — a relative `use` pulls in a sibling file

```ura
// ---- helper.ura
fn help() i32:
    return 7

// ---- 001.ura - a relative `use` pulls in a sibling file
use "helper"

main():
    output("help() = ", help(), "\n")
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

fn help() : i32
└─ return
   └─ int 7

fn main() : i32
└─ output : void
   ├─ char[] "help() = "
   ├─ call help : i32
   └─ char[] "\n"
```

```out
help() = 7
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [10 x i8] c"help() = \00", align 1
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

define i32 @help() {
entry:
  ret i32 7
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %call = call i32 @help()
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 9, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %call, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 002 — `use` resolves against the importing file, including a subdirectory

```ura
// ---- sub/deep.ura
fn deep() i32:
    return 3

// ---- side.ura
fn side() i32:
    return 4

// ---- mid.ura
use "sub/deep"
use "side"

fn total() i32:
    return deep() + side()

// ---- 002.ura - `use` resolves against the importing file
use "mid"

main():
    output("total() = ", total(), "\n")
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

fn deep() : i32
└─ return
   └─ int 3

fn side() : i32
└─ return
   └─ int 4

fn total() : i32
└─ return
   └─ + : i32
      ├─ call deep : i32
      └─ call side : i32

fn main() : i32
└─ output : void
   ├─ char[] "total() = "
   ├─ call total : i32
   └─ char[] "\n"
```

```out
total() = 7
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [11 x i8] c"total() = \00", align 1
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

define i32 @deep() {
entry:
  ret i32 3
}

define i32 @side() {
entry:
  ret i32 4
}

define i32 @total() {
entry:
  %call = call i32 @deep()
  %call1 = call i32 @side()
  %add = add i32 %call, %call1
  ret i32 %add
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %call = call i32 @total()
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 10, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str, i32 0, i32 0), i32 %call, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 003 — a diamond loads the shared file once, and says nothing

```ura
// ---- dbase.ura
fn base() i32:
    return 5

// ---- d1.ura
use "dbase"

fn one() i32:
    return base() + 1

// ---- d2.ura
use "dbase"

fn two() i32:
    return base() + 2

// ---- 003.ura - a diamond loads the shared file once
use "d1"
use "d2"

main():
    output(one(), " ", two(), "\n")
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

fn base() : i32
└─ return
   └─ int 5

fn one() : i32
└─ return
   └─ + : i32
      ├─ call base : i32
      └─ int 1

fn two() : i32
└─ return
   └─ + : i32
      ├─ call base : i32
      └─ int 2

fn main() : i32
└─ output : void
   ├─ call one : i32
   ├─ char[] " "
   ├─ call two : i32
   └─ char[] "\n"
```

```out
6 7
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [13 x i8] c"%d%.*s%d%.*s\00", align 1

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

define i32 @base() {
entry:
  ret i32 5
}

define i32 @one() {
entry:
  %call = call i32 @base()
  %add = add i32 %call, 1
  ret i32 %add
}

define i32 @two() {
entry:
  %call = call i32 @base()
  %add = add i32 %call, 2
  ret i32 %add
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %call = call i32 @one()
  %call1 = call i32 @two()
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i32 %call, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %call1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 004 — an import cycle warns and still compiles

```ura
// ---- c1.ura
use "c2"

fn ping() i32:
    return 1

// ---- c2.ura
use "c1"

fn pong() i32:
    return 2

// ---- 004.ura - an import cycle warns and still compiles
use "c1"

main():
    output(ping() + pong(), "\n")
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

fn pong() : i32
└─ return
   └─ int 2

fn ping() : i32
└─ return
   └─ int 1

fn main() : i32
└─ output : void
   ├─ + : i32
   │  ├─ call ping : i32
   │  └─ call pong : i32
   └─ char[] "\n"
```

```out
3
```

```err
warning: import cycle: 'c1.ura' imports itself through 'c2.ura'
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

define i32 @pong() {
entry:
  ret i32 2
}

define i32 @ping() {
entry:
  ret i32 1
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %call = call i32 @ping()
  %call1 = call i32 @pong()
  %add = add i32 %call, %call1
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %add, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 005 — `use` of a file that does not exist

```ura
// 005.ura - `use` of a file that does not exist

use "nope"

main():
    output("unreachable\n")
```

```tree
```

```out
```

```err
error: Cannot find file 'nope.ura'
```

```ll
```

## 006 — `use "@/..."` resolves against the standard library

```ura
// imports/006.ura - "@/" resolves against the standard library

use "@/memory"
use "@/ctype"
use "@/io"

main():
    output("isdigit ", isdigit('7' as i32), "\n")
    output("isalpha ", isalpha('7' as i32), "\n")
    output("stdlib parsed\n")
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

proto fn malloc(size : i32) : pointer

proto fn calloc(len : i64, size : i64) : pointer

proto fn realloc(ptr : pointer, newSize : i32) : pointer

proto fn free(ptr : pointer) : void

proto fn aligned_alloc(alignment : i32, size : i32) : pointer

proto fn isalnum(c : i32) : i32

proto fn isalpha(c : i32) : i32

proto fn isascii(c : i32) : i32

proto fn isblank(c : i32) : i32

proto fn iscntrl(c : i32) : i32

proto fn isdigit(c : i32) : i32

proto fn isgraph(c : i32) : i32

proto fn islower(c : i32) : i32

proto fn isprint(c : i32) : i32

proto fn ispunct(c : i32) : i32

proto fn isspace(c : i32) : i32

proto fn isupper(c : i32) : i32

proto fn isxdigit(c : i32) : i32

proto fn tolower(c : i32) : i32

proto fn toupper(c : i32) : i32

proto fn fopen(path : pointer, mode : pointer) : pointer

proto fn freopen(path : pointer, mode : pointer, file : pointer) : pointer

proto fn fclose(file : pointer) : i32

proto fn fflush(file : pointer) : i32

proto fn fgetc(file : pointer) : i32

proto fn fputc(c : i32, file : pointer) : i32

proto fn getc(file : pointer) : i32

proto fn putc(c : i32, file : pointer) : i32

proto fn getchar() : i32

proto fn putchar(c : char) : i32

proto fn ungetc(c : i32, file : pointer) : i32

proto fn fgets(buffer : pointer, size : i32, file : pointer) : pointer

proto fn fputs(str : pointer, file : pointer) : i32

proto fn puts(str : pointer) : i32

proto fn gets(buffer : pointer) : pointer

proto fn fread(ptr : pointer, size : i32, nmemb : i32, file : pointer) : i32

proto fn fwrite(ptr : pointer, size : i32, nmemb : i32, file : pointer) : i32

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn read(fd : i32, ptr : pointer, len : i64) : i64

proto fn fseek(file : pointer, offset : i64, whence : i32) : i32

proto fn ftell(file : pointer) : i64

proto fn rewind(file : pointer) : void

proto fn fgetpos(file : pointer, pos : pointer) : i32

proto fn fsetpos(file : pointer, pos : pointer) : i32

proto fn feof(file : pointer) : i32

proto fn ferror(file : pointer) : i32

proto fn clearerr(file : pointer) : void

proto fn perror(str : pointer) : void

proto fn setvbuf(file : pointer, buffer : pointer, mode : i32, size : i32) : i32

proto fn setbuf(file : pointer, buffer : pointer) : void

proto fn remove(path : pointer) : i32

proto fn rename(oldpath : pointer, newpath : pointer) : i32

proto fn tmpfile() : pointer

proto fn tmpnam(str : pointer) : pointer

proto fn fileno(file : pointer) : i32

proto fn fdopen(fd : i32, mode : pointer) : pointer

proto fn printf(format : pointer, ...) : i32

proto fn fprintf(file : pointer, format : pointer, ...) : i32

proto fn sprintf(buffer : pointer, format : pointer, ...) : i32

proto fn snprintf(buffer : pointer, size : i32, format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn vprintf(format : pointer, args : pointer) : i32

proto fn vfprintf(file : pointer, format : pointer, args : pointer) : i32

proto fn vsprintf(buffer : pointer, format : pointer, args : pointer) : i32

proto fn vsnprintf(buffer : pointer, size : i32, format : pointer, args : pointer) : i32

proto fn vdprintf(fd : i32, format : pointer, args : pointer) : i32

proto fn scanf(format : pointer, ...) : i32

proto fn fscanf(file : pointer, format : pointer, ...) : i32

proto fn sscanf(buffer : pointer, format : pointer, ...) : i32

proto fn vscanf(format : pointer, args : pointer) : i32

proto fn vfscanf(file : pointer, format : pointer, args : pointer) : i32

proto fn vsscanf(buffer : pointer, format : pointer, args : pointer) : i32

fn main() : i32
├─ output : void
│  ├─ char[] "isdigit "
│  ├─ call isdigit : i32
│  │  └─ cast : i32
│  │     └─ char '7'
│  └─ char[] "\n"
├─ output : void
│  ├─ char[] "isalpha "
│  ├─ call isalpha : i32
│  │  └─ cast : i32
│  │     └─ char '7'
│  └─ char[] "\n"
└─ output : void
   └─ char[] "stdlib parsed\n"
```

```out
isdigit 1
isalpha 0
stdlib parsed
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [9 x i8] c"isdigit \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.2 = private unnamed_addr constant [9 x i8] c"isalpha \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.5 = private unnamed_addr constant [15 x i8] c"stdlib parsed\0A\00", align 1
@fmt.6 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

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
  %call = call i32 @isdigit(i32 55)
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %call, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %call1 = call i32 @isalpha(i32 55)
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.4, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.2, i32 0, i32 0), i32 %call1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.6, i32 0, i32 0), i32 14, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.5, i32 0, i32 0))
  ret i32 0
}

declare i32 @isdigit(i32)

declare i32 @printf(i8*, ...)

declare i32 @isalpha(i32)
```
