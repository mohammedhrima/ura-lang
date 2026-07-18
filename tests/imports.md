# imports

## index

- 001 — a relative `use` pulls in a sibling file
- 002 — `use` resolves against the importing file, including a subdirectory
- 003 — a diamond loads the shared file once, and says nothing
- 004 — an import cycle warns and still compiles
- 005 — `use` of a file that does not exist

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
