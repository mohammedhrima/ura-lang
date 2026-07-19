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
fn help() int:
    return 7

// ---- 001.ura - a relative `use` pulls in a sibling file
use "helper"

main():
    output("help() = ", help(), "\n")
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn help() : i32
└─ return
   └─ int 7

fn main() : i32
└─ output : void
   ├─ chars "help() = "
   ├─ call help : i32
   └─ chars "\n"
```

```out
help() = 7
```

```err
```

```ll

@str = private unnamed_addr constant [10 x i8] c"help() = \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define i32 @help() {
entry:
  ret i32 7
}

define i32 @main() {
entry:
  %call = call i32 @help()
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %call, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 002 — `use` resolves against the importing file, including a subdirectory

```ura
// ---- sub/deep.ura
fn deep() int:
    return 3

// ---- side.ura
fn side() int:
    return 4

// ---- mid.ura
use "sub/deep"
use "side"

fn total() int:
    return deep() + side()

// ---- 002.ura - `use` resolves against the importing file
use "mid"

main():
    output("total() = ", total(), "\n")
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

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
   ├─ chars "total() = "
   ├─ call total : i32
   └─ chars "\n"
```

```out
total() = 7
```

```err
```

```ll

@str = private unnamed_addr constant [11 x i8] c"total() = \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

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

define i32 @main() {
entry:
  %call = call i32 @total()
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str, i32 0, i32 0), i32 %call, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 003 — a diamond loads the shared file once, and says nothing

```ura
// ---- dbase.ura
fn base() int:
    return 5

// ---- d1.ura
use "dbase"

fn one() int:
    return base() + 1

// ---- d2.ura
use "dbase"

fn two() int:
    return base() + 2

// ---- 003.ura - a diamond loads the shared file once
use "d1"
use "d2"

main():
    output(one(), " ", two(), "\n")
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

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
   ├─ chars " "
   ├─ call two : i32
   └─ chars "\n"
```

```out
6 7
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [9 x i8] c"%d%s%d%s\00", align 1

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

define i32 @main() {
entry:
  %call = call i32 @one()
  %call1 = call i32 @two()
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt, i32 0, i32 0), i32 %call, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %call1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 004 — an import cycle warns and still compiles

```ura
// ---- c1.ura
use "c2"

fn ping() int:
    return 1

// ---- c2.ura
use "c1"

fn pong() int:
    return 2

// ---- 004.ura - an import cycle warns and still compiles
use "c1"

main():
    output(ping() + pong(), "\n")
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

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
   └─ chars "\n"
```

```out
3
```

```err
warning: import cycle: 'c1.ura' imports itself through 'c2.ura'
```

```ll

@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1

define i32 @pong() {
entry:
  ret i32 2
}

define i32 @ping() {
entry:
  ret i32 1
}

define i32 @main() {
entry:
  %call = call i32 @ping()
  %call1 = call i32 @pong()
  %add = add i32 %call, %call1
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 %add, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
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
    output("isdigit ", isdigit('7' as int), "\n")
    output("isalpha ", isalpha('7' as int), "\n")
    output("stdlib parsed\n")
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

proto fn malloc(size : i32) : chars

proto fn calloc(len : i64, size : i64) : chars

proto fn realloc(ptr : chars, newSize : i32) : chars

proto fn free(ptr : chars) : void

proto fn aligned_alloc(alignment : i32, size : i32) : chars

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

proto fn fopen(path : chars, mode : chars) : chars

proto fn freopen(path : chars, mode : chars, file : chars) : chars

proto fn fclose(file : chars) : i32

proto fn fflush(file : chars) : i32

proto fn fgetc(file : chars) : i32

proto fn fputc(c : i32, file : chars) : i32

proto fn getc(file : chars) : i32

proto fn putc(c : i32, file : chars) : i32

proto fn getchar() : i32

proto fn putchar(c : char) : i32

proto fn ungetc(c : i32, file : chars) : i32

proto fn fgets(buffer : chars, size : i32, file : chars) : chars

proto fn fputs(str : chars, file : chars) : i32

proto fn puts(str : chars) : i32

proto fn gets(buffer : chars) : chars

proto fn fread(ptr : chars, size : i32, nmemb : i32, file : chars) : i32

proto fn fwrite(ptr : chars, size : i32, nmemb : i32, file : chars) : i32

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn read(fd : i32, ptr : chars, len : i64) : i64

proto fn fseek(file : chars, offset : i64, whence : i32) : i32

proto fn ftell(file : chars) : i64

proto fn rewind(file : chars) : void

proto fn fgetpos(file : chars, pos : chars) : i32

proto fn fsetpos(file : chars, pos : chars) : i32

proto fn feof(file : chars) : i32

proto fn ferror(file : chars) : i32

proto fn clearerr(file : chars) : void

proto fn perror(str : chars) : void

proto fn setvbuf(file : chars, buffer : chars, mode : i32, size : i32) : i32

proto fn setbuf(file : chars, buffer : chars) : void

proto fn remove(path : chars) : i32

proto fn rename(oldpath : chars, newpath : chars) : i32

proto fn tmpfile() : chars

proto fn tmpnam(str : chars) : chars

proto fn fileno(file : chars) : i32

proto fn fdopen(fd : i32, mode : chars) : chars

proto fn printf(format : chars, ...) : i32

proto fn fprintf(file : chars, format : chars, ...) : i32

proto fn sprintf(buffer : chars, format : chars, ...) : i32

proto fn snprintf(buffer : chars, size : i32, format : chars, ...) : i32

proto fn dprintf(fd : i32, format : chars, ...) : i32

proto fn vprintf(format : chars, args : chars) : i32

proto fn vfprintf(file : chars, format : chars, args : chars) : i32

proto fn vsprintf(buffer : chars, format : chars, args : chars) : i32

proto fn vsnprintf(buffer : chars, size : i32, format : chars, args : chars) : i32

proto fn vdprintf(fd : i32, format : chars, args : chars) : i32

proto fn scanf(format : chars, ...) : i32

proto fn fscanf(file : chars, format : chars, ...) : i32

proto fn sscanf(buffer : chars, format : chars, ...) : i32

proto fn vscanf(format : chars, args : chars) : i32

proto fn vfscanf(file : chars, format : chars, args : chars) : i32

proto fn vsscanf(buffer : chars, format : chars, args : chars) : i32

fn main() : i32
├─ output : void
│  ├─ chars "isdigit "
│  ├─ call isdigit : i32
│  │  └─ cast : i32
│  │     └─ char '7'
│  └─ chars "\n"
├─ output : void
│  ├─ chars "isalpha "
│  ├─ call isalpha : i32
│  │  └─ cast : i32
│  │     └─ char '7'
│  └─ chars "\n"
└─ output : void
   └─ chars "stdlib parsed\n"
```

```out
isdigit 1
isalpha 0
stdlib parsed
```

```err
```

```ll

@str = private unnamed_addr constant [9 x i8] c"isdigit \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.2 = private unnamed_addr constant [9 x i8] c"isalpha \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.5 = private unnamed_addr constant [15 x i8] c"stdlib parsed\0A\00", align 1
@fmt.6 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i32 @main() {
entry:
  %call = call i32 @isdigit(i32 55)
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %call, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %call1 = call i32 @isalpha(i32 55)
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.4, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.2, i32 0, i32 0), i32 %call1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.6, i32 0, i32 0), i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.5, i32 0, i32 0))
  ret i32 0
}

declare i32 @isdigit(i32)

declare i32 @printf(i8*, ...)

declare i32 @isalpha(i32)
```
