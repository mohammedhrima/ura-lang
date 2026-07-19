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
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

fn help() : int
└─ return
   └─ int 7

fn main() : int
└─ output : void
   ├─ chars "help() = "
   ├─ call help : int
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
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

fn deep() : int
└─ return
   └─ int 3

fn side() : int
└─ return
   └─ int 4

fn total() : int
└─ return
   └─ + : int
      ├─ call deep : int
      └─ call side : int

fn main() : int
└─ output : void
   ├─ chars "total() = "
   ├─ call total : int
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
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

fn base() : int
└─ return
   └─ int 5

fn one() : int
└─ return
   └─ + : int
      ├─ call base : int
      └─ int 1

fn two() : int
└─ return
   └─ + : int
      ├─ call base : int
      └─ int 2

fn main() : int
└─ output : void
   ├─ call one : int
   ├─ chars " "
   ├─ call two : int
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
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

fn pong() : int
└─ return
   └─ int 2

fn ping() : int
└─ return
   └─ int 1

fn main() : int
└─ output : void
   ├─ + : int
   │  ├─ call ping : int
   │  └─ call pong : int
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
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

proto fn malloc(size : int) : chars

proto fn calloc(len : long, size : long) : chars

proto fn realloc(ptr : chars, newSize : int) : chars

proto fn free(ptr : chars) : void

proto fn aligned_alloc(alignment : int, size : int) : chars

proto fn isalnum(c : int) : int

proto fn isalpha(c : int) : int

proto fn isascii(c : int) : int

proto fn isblank(c : int) : int

proto fn iscntrl(c : int) : int

proto fn isdigit(c : int) : int

proto fn isgraph(c : int) : int

proto fn islower(c : int) : int

proto fn isprint(c : int) : int

proto fn ispunct(c : int) : int

proto fn isspace(c : int) : int

proto fn isupper(c : int) : int

proto fn isxdigit(c : int) : int

proto fn tolower(c : int) : int

proto fn toupper(c : int) : int

proto fn fopen(path : chars, mode : chars) : chars

proto fn freopen(path : chars, mode : chars, file : chars) : chars

proto fn fclose(file : chars) : int

proto fn fflush(file : chars) : int

proto fn fgetc(file : chars) : int

proto fn fputc(c : int, file : chars) : int

proto fn getc(file : chars) : int

proto fn putc(c : int, file : chars) : int

proto fn getchar() : int

proto fn putchar(c : char) : int

proto fn ungetc(c : int, file : chars) : int

proto fn fgets(buffer : chars, size : int, file : chars) : chars

proto fn fputs(str : chars, file : chars) : int

proto fn puts(str : chars) : int

proto fn gets(buffer : chars) : chars

proto fn fread(ptr : chars, size : int, nmemb : int, file : chars) : int

proto fn fwrite(ptr : chars, size : int, nmemb : int, file : chars) : int

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn read(fd : int, ptr : chars, len : long) : long

proto fn fseek(file : chars, offset : long, whence : int) : int

proto fn ftell(file : chars) : long

proto fn rewind(file : chars) : void

proto fn fgetpos(file : chars, pos : chars) : int

proto fn fsetpos(file : chars, pos : chars) : int

proto fn feof(file : chars) : int

proto fn ferror(file : chars) : int

proto fn clearerr(file : chars) : void

proto fn perror(str : chars) : void

proto fn setvbuf(file : chars, buffer : chars, mode : int, size : int) : int

proto fn setbuf(file : chars, buffer : chars) : void

proto fn remove(path : chars) : int

proto fn rename(oldpath : chars, newpath : chars) : int

proto fn tmpfile() : chars

proto fn tmpnam(str : chars) : chars

proto fn fileno(file : chars) : int

proto fn fdopen(fd : int, mode : chars) : chars

proto fn printf(format : chars, ...) : int

proto fn fprintf(file : chars, format : chars, ...) : int

proto fn sprintf(buffer : chars, format : chars, ...) : int

proto fn snprintf(buffer : chars, size : int, format : chars, ...) : int

proto fn dprintf(fd : int, format : chars, ...) : int

proto fn vprintf(format : chars, args : chars) : int

proto fn vfprintf(file : chars, format : chars, args : chars) : int

proto fn vsprintf(buffer : chars, format : chars, args : chars) : int

proto fn vsnprintf(buffer : chars, size : int, format : chars, args : chars) : int

proto fn vdprintf(fd : int, format : chars, args : chars) : int

proto fn scanf(format : chars, ...) : int

proto fn fscanf(file : chars, format : chars, ...) : int

proto fn sscanf(buffer : chars, format : chars, ...) : int

proto fn vscanf(format : chars, args : chars) : int

proto fn vfscanf(file : chars, format : chars, args : chars) : int

proto fn vsscanf(buffer : chars, format : chars, args : chars) : int

fn main() : int
├─ output : void
│  ├─ chars "isdigit "
│  ├─ call isdigit : int
│  │  └─ cast : int
│  │     └─ char '7'
│  └─ chars "\n"
├─ output : void
│  ├─ chars "isalpha "
│  ├─ call isalpha : int
│  │  └─ cast : int
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
