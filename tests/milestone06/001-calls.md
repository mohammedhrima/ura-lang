# milestone06

## index

- 001 — calls: output a struct
- 002 — function: multi line string
- 003 — builtin: output picks the spec from each argument
- 004 — builtin: output with literals and escapes
- 005 — errput: errput writes to stderr, output writes to stdout
- 006 — errput: errput auto-formats every scalar type, like output
- 007 — errput: errput expands a struct with the default dump
- 008 — errput: errput honours operator output()
- 009 — errput: errput prints arrays and nested arrays
- 010 — errput: errput() with no arguments still emits the colour pair
- 011 — errput: errput and output keep separate struct printers
- 012 — output: basic output: strings, ints, floats, bools
- 013 — output: output with expressions: arithmetic inline
- 014 — output: output all supported types in one block
- 015 — output: output() auto-formats each argument by its type
- 016 — string: String constructors and accessors
- 017 — string: assign, push, pop, join and clear
- 018 — string: find, contains, starts_with and ends_with

---

## 001 — calls: output a struct

```ura
struct User:
    a i32

fn main() i32:
    u User
    output(u)
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32 }

@str = private unnamed_addr constant [6 x i8] c"User{\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"a: %d\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"}\00", align 1
@str.3 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0))
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a" = load i32, i32* %u.a, align 4
  %call.dprintf1 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i32 %"$u.a")
  %call.dprintf2 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %call.dprintf3 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.3, i32 0, i32 0))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 002 — function: multi line string

```ura
fn foo(ptr i32):

fn main() i32:
   a i32 = 10
   foo(a)
   output(
        "hello "
        "world",
        12, "\n")
```

---

## 003 — builtin: output picks the spec from each argument

```ura
fn main() i32:
   n i32 = 42
   c i8 = 'z'
   s i8[] = "hi"
   flag bool = n > 1
   output(n, " ", c, " ", s, " ", flag, "\n")
   return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.2 = private unnamed_addr constant [13 x i8] c"%d %c %s %s\0A\00", align 1

define i32 @main() {
entry:
  %n = alloca i32, align 4
  %c = alloca i8, align 1
  %s = alloca i8*, align 8
  %arr = alloca [3 x i8], align 1
  %flag = alloca i1, align 1
  %bool.text = alloca i8*, align 8
  store i32 42, i32* %n, align 4
  store i8 122, i8* %c, align 1
  store [3 x i8] c"hi\00", [3 x i8]* %arr, align 1
  %0 = getelementptr inbounds [3 x i8], [3 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %s, align 8
  %"$n" = load i32, i32* %n, align 4
  %GT = icmp sgt i32 %"$n", 1
  store i1 %GT, i1* %flag, align 1
  %"$n1" = load i32, i32* %n, align 4
  %"$c" = load i8, i8* %c, align 1
  %AS = sext i8 %"$c" to i32
  %"$s" = load i8*, i8** %s, align 8
  %"$flag" = load i1, i1* %flag, align 1
  store i8* null, i8** %bool.text, align 8
  br label %if0

if0:                                              ; preds = %entry
  br i1 %"$flag", label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else1:                                            ; preds = %if0
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.2, i32 0, i32 0), i32 %"$n1", i32 %AS, i8* %"$s", i8* %"$bool.text")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 004 — builtin: output with literals and escapes

```ura
fn main() i32:
   n i32 = 3
   output("n=", n, "\ttab\n")
   output("no newline")
   output("\n")
   return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [10 x i8] c"n=%d\09tab\0A\00", align 1
@str.1 = private unnamed_addr constant [11 x i8] c"no newline\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

define i32 @main() {
entry:
  %n = alloca i32, align 4
  store i32 3, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %"$n")
  %call.dprintf1 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.1, i32 0, i32 0))
  %call.dprintf2 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 005 — errput: errput writes to stderr, output writes to stdout

```ura
// stdout: on stdout
// stderr: [0;31mon stderr
// expect: 1

fn main() i32:
    output("on stdout\n")
    errput("on stderr\n")
    return 0
```

---

## 006 — errput: errput auto-formats every scalar type, like output

```ura
// stderr: [0;31mhp=42 big=9000000000 small=-7
// expect: 1

fn main() i32:
    hp    i32  = 42
    big   i64  = 9000000000
    small i8   = -7
    alive bool = True
    grade char = 'A'
    speed f32  = 1.5
    exact f64  = 2.5
    errput("hp=", hp, " big=", big, " small=", small, "\n")
    errput("alive=", alive, " grade=", grade, "\n")
    errput("speed=", speed, " exact=", exact, "\n")
    return 0
```

---

## 007 — errput: errput expands a struct with the default dump

```ura
// stderr: [0;31mhero Hero{name: Aldric, stats: Stats{atk: 18, def: 7}}
// expect: 1

struct Stats:
    atk i32
    def i32

struct Hero:
    name  char[]
    stats Stats

fn main() i32:
    h Hero
    h.name = "Aldric"
    h.stats.atk = 18
    h.stats.def = 7
    errput("hero ", h, "\n")
    return 0
```

---

## 008 — errput: errput honours operator output()

```ura
// stderr: [0;31mtag custom printer
// expect: 1

struct Tag:
    name char[]

    operator output() char[]:
        ret self.name

fn main() i32:
    t Tag
    t.name = "custom printer"
    errput("tag ", t, "\n")
    return 0
```

---

## 009 — errput: errput prints arrays and nested arrays

```ura
// stderr: [0;31mnums [1, 2, 3]
// expect: 1

fn main() i32:
    nums i32[]   = [1, 2, 3]
    grid i32[][] = [[1, 2], [3, 4]]
    word char[]  = "runes"
    errput("nums ", nums, "\n")
    errput("grid ", grid, "\n")
    errput("word ", word, "\n")
    return 0
```

---

## 010 — errput: errput() with no arguments still emits the colour pair

```ura
// stderr: [0;31m[0m[0;31mafter the empty call
// expect: 1

fn main() i32:
    errput()
    errput("after the empty call\n")
    return 0
```

---

## 011 — errput: errput and output keep separate struct printers

```ura
// stdout: out Point{x: 3, y: 4}
// stderr: [0;31merr Point{x: 3, y: 4}
// expect: 1

struct Point:
    x i32
    y i32

fn main() i32:
    p Point
    p.x = 3
    p.y = 4
    errput("err ", p, "\n")
    output("out ", p, "\n")
    errput("err again ", p, "\n")
    return 0
```

---

## 012 — output: basic output: strings, ints, floats, bools

```ura
// stdout: === Aldric enters the dungeon ===
// stdout: Floor: 1
// stdout: Alive: True
// stdout: Speed: 1.500000
// stdout: Ready to fight!
// expect: 0

fn main() i32:
    output("=== Aldric enters the dungeon ===\n")
    output("Floor: ", 1, "\n")
    output("Alive: ", True, "\n")
    output("Speed: ", 1.5, "\n")
    output("Ready to fight!\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [35 x i8] c"=== Aldric enters the dungeon ===\0A\00", align 1
@str.1 = private unnamed_addr constant [11 x i8] c"Floor: %d\0A\00", align 1
@str.2 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.4 = private unnamed_addr constant [11 x i8] c"Alive: %s\0A\00", align 1
@str.5 = private unnamed_addr constant [11 x i8] c"Speed: %f\0A\00", align 1
@str.6 = private unnamed_addr constant [17 x i8] c"Ready to fight!\0A\00", align 1

define i32 @main() {
entry:
  %bool.text = alloca i8*, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([35 x i8], [35 x i8]* @str, i32 0, i32 0))
  %call.dprintf1 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.1, i32 0, i32 0), i32 1)
  store i8* null, i8** %bool.text, align 8
  br label %if0

if0:                                              ; preds = %entry
  br i1 true, label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else1:                                            ; preds = %if0
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf2 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.4, i32 0, i32 0), i8* %"$bool.text")
  %call.dprintf3 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.5, i32 0, i32 0), double 1.500000e+00)
  %call.dprintf4 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.6, i32 0, i32 0))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 013 — output: output with expressions: arithmetic inline

```ura
// stdout: Damage dealt: 17
// stdout: XP gained:    150
// stdout: Gold split:   30
// stdout: Bonus:        37
// expect: 0

fn main() i32:
    output("Damage dealt: ", 25 - 8, "\n")
    output("XP gained:    ", 3 * 50, "\n")
    output("Gold split:   ", 120 / 4, "\n")
    output("Bonus:        ", 7 + 3 * 10, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [18 x i8] c"Damage dealt: %d\0A\00", align 1
@str.1 = private unnamed_addr constant [18 x i8] c"XP gained:    %d\0A\00", align 1
@str.2 = private unnamed_addr constant [18 x i8] c"Gold split:   %d\0A\00", align 1
@str.3 = private unnamed_addr constant [18 x i8] c"Bonus:        %d\0A\00", align 1

define i32 @main() {
entry:
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str, i32 0, i32 0), i32 17)
  %call.dprintf1 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str.1, i32 0, i32 0), i32 150)
  %call.dprintf2 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str.2, i32 0, i32 0), i32 30)
  %call.dprintf3 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str.3, i32 0, i32 0), i32 37)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 014 — output: output all supported types in one block

```ura
// stdout: Name:  Aldric
// stdout: HP:    100
// stdout: Grade: S
// stdout: Alive: True
// stdout: Speed: 1.750000
// expect: 0

fn main() i32:
    name  char[] = "Aldric"
    hp    i32   = 100
    grade char  = 'S'
    alive bool  = True
    speed f32 = 1.75

    output("Name:  ", name,  "\n")
    output("HP:    ", hp,    "\n")
    output("Grade: ", grade, "\n")
    output("Alive: ", alive, "\n")
    output("Speed: ", speed, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [11 x i8] c"Name:  %s\0A\00", align 1
@str.1 = private unnamed_addr constant [11 x i8] c"HP:    %d\0A\00", align 1
@str.2 = private unnamed_addr constant [11 x i8] c"Grade: %c\0A\00", align 1
@str.3 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.4 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.5 = private unnamed_addr constant [11 x i8] c"Alive: %s\0A\00", align 1
@str.6 = private unnamed_addr constant [11 x i8] c"Speed: %f\0A\00", align 1

define i32 @main() {
entry:
  %name = alloca i8*, align 8
  %arr = alloca [7 x i8], align 1
  %hp = alloca i32, align 4
  %grade = alloca i8, align 1
  %alive = alloca i1, align 1
  %speed = alloca float, align 4
  %bool.text = alloca i8*, align 8
  store [7 x i8] c"Aldric\00", [7 x i8]* %arr, align 1
  %0 = getelementptr inbounds [7 x i8], [7 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %name, align 8
  store i32 100, i32* %hp, align 4
  store i8 83, i8* %grade, align 1
  store i1 true, i1* %alive, align 1
  store float 1.750000e+00, float* %speed, align 4
  %"$name" = load i8*, i8** %name, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str, i32 0, i32 0), i8* %"$name")
  %"$hp" = load i32, i32* %hp, align 4
  %call.dprintf1 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.1, i32 0, i32 0), i32 %"$hp")
  %"$grade" = load i8, i8* %grade, align 1
  %AS = sext i8 %"$grade" to i32
  %call.dprintf2 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.2, i32 0, i32 0), i32 %AS)
  %"$alive" = load i1, i1* %alive, align 1
  store i8* null, i8** %bool.text, align 8
  br label %if0

if0:                                              ; preds = %entry
  br i1 %"$alive", label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.3, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else1:                                            ; preds = %if0
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.4, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf3 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.5, i32 0, i32 0), i8* %"$bool.text")
  %"$speed" = load float, float* %speed, align 4
  %AS4 = fpext float %"$speed" to double
  %call.dprintf5 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.6, i32 0, i32 0), double %AS4)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 015 — output: output() auto-formats each argument by its type

```ura
// stdout: hello ura 42 True
// expect: 0

fn main() i32:
    name char[] = "ura"
    n i32 = 42
    ok bool = True
    output("hello ", name, " ", n, " ", ok, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.2 = private unnamed_addr constant [16 x i8] c"hello %s %d %s\0A\00", align 1

define i32 @main() {
entry:
  %name = alloca i8*, align 8
  %arr = alloca [4 x i8], align 1
  %n = alloca i32, align 4
  %ok = alloca i1, align 1
  %bool.text = alloca i8*, align 8
  store [4 x i8] c"ura\00", [4 x i8]* %arr, align 1
  %0 = getelementptr inbounds [4 x i8], [4 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %name, align 8
  store i32 42, i32* %n, align 4
  store i1 true, i1* %ok, align 1
  %"$name" = load i8*, i8** %name, align 8
  %"$n" = load i32, i32* %n, align 4
  %"$ok" = load i1, i1* %ok, align 1
  store i8* null, i8** %bool.text, align 8
  br label %if0

if0:                                              ; preds = %entry
  br i1 %"$ok", label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else1:                                            ; preds = %if0
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.2, i32 0, i32 0), i8* %"$name", i32 %"$n", i8* %"$bool.text")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 016 — string: String constructors and accessors

```ura
// stdout: 0 True
// stdout: hello 5 False
// stdout: 0 True
// stdout: 42
// stdout: -123
// stdout: 0
// stdout: h o 0 0
// stdout: hello
// expect: 0
use "@/header"

fn main() i32:
    s1 String = String::create("")
    output(s1.size(), " ", s1.is_empty(), "\n")

    s2 String = String::create("hello")
    output(s2, " ", s2.size(), " ", s2.is_empty(), "\n")

    s3 String = String::create("")
    output(s3.size(), " ", s3.is_empty(), "\n")

    s4 String = String::create(42)
    output(s4, "\n")

    s5 String = String::create(-123)
    output(s5, "\n")

    s6 String = String::create(0)
    output(s6, "\n")

    output(s2.at(0), " ", s2.at(4), " ",
           s2.at(-1) as i32, " ", s2.at(99) as i32, "\n")
    output(s2.chars(), "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.2 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.4 = private unnamed_addr constant [7 x i8] c"%d %s\0A\00", align 1
@str.5 = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@str.6 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.7 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.8 = private unnamed_addr constant [10 x i8] c"%s %d %s\0A\00", align 1
@str.9 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.10 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.11 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.12 = private unnamed_addr constant [7 x i8] c"%d %s\0A\00", align 1
@str.13 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.14 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.15 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.16 = private unnamed_addr constant [13 x i8] c"%c %c %d %d\0A\00", align 1
@str.17 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  ret void

endif1:                                           ; preds = %if1
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if2

if2:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %while4

while4:                                           ; preds = %while4.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.count5" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.count9" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while4

endloop:                                          ; preds = %while4
  ret void
}

define void @String.append(%String* %0, i8 %1) {
entry:
  %self = alloca %String*, align 8
  %value = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store i8 %1, i8* %value, align 1
  br label %if5

if5:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @String.reserve(%String* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if5.then, %if5
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.count8" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count9"
  %"$value" = load i8, i8* %value, align 1
  store i8 %"$value", i8* %"*self.data[]", align 1
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  ret void
}

define i8 @String.at(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %index = alloca i32, align 4
  %value = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %index, align 4
  store i8 0, i8* %value, align 1
  br label %if6

if6:                                              ; preds = %entry
  %"$index" = load i32, i32* %index, align 4
  %GE = icmp sge i32 %"$index", 0
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$index1" = load i32, i32* %index, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$index1", %"$*self.count"
  %AND = and i1 %GE, %LT
  br i1 %AND, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$index3" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$index3"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  store i8 %"$*self.data[]", i8* %value, align 1
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  %"$value" = load i8, i8* %value, align 1
  ret i8 %"$value"
}

define i32 @String.size(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i1 @String.is_empty(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %EQ = icmp eq i32 %"$*self.count", 0
  ret i1 %EQ
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if7

if7:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if7.then, label %endif

if7.then:                                         ; preds = %if7
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if7.then, %if7
  ret void
}

define %String @String.create(i8* %0) {
entry:
  %text = alloca i8*, align 8
  %res = alloca %String, align 8
  store i8* %0, i8** %text, align 8
  store %String zeroinitializer, %String* %res, align 8
  %"$text" = load i8*, i8** %text, align 8
  call void @String.join(%String* %res, i8* %"$text")
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define %String @String.create.1(i32 %0) {
entry:
  %number = alloca i32, align 4
  %res = alloca %String, align 8
  %n = alloca i32, align 4
  store i32 %0, i32* %number, align 4
  store %String zeroinitializer, %String* %res, align 8
  %"$number" = load i32, i32* %number, align 4
  store i32 %"$number", i32* %n, align 4
  br label %if8

if8:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %LT = icmp slt i32 %"$n", 0
  br i1 %LT, label %if8.then, label %endif

if8.then:                                         ; preds = %if8
  %"$n3" = load i32, i32* %n, align 4
  %SUB = sub i32 0, %"$n3"
  store i32 %SUB, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %if8.then, %if8
  br label %while9

while9:                                           ; preds = %endif1, %endif
  br i1 true, label %while9.then, label %endloop

while9.then:                                      ; preds = %while9
  %"$n4" = load i32, i32* %n, align 4
  %MOD = srem i32 %"$n4", 10
  %ADD = add i32 %MOD, 48
  %AS = trunc i32 %ADD to i8
  call void @String.append(%String* %res, i8 %AS)
  %"$n5" = load i32, i32* %n, align 4
  %DIV = sdiv i32 %"$n5", 10
  store i32 %DIV, i32* %n, align 4
  br label %if10

if10:                                             ; preds = %while9.then
  %"$n6" = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %"$n6", 0
  br i1 %EQ, label %if10.then, label %endif1

if10.then:                                        ; preds = %if10
  br label %endloop

endif1:                                           ; preds = %if10
  br label %while9

endloop:                                          ; preds = %if10.then, %while9
  br label %if11

if11:                                             ; preds = %endloop
  %"$number7" = load i32, i32* %number, align 4
  %LT8 = icmp slt i32 %"$number7", 0
  br i1 %LT8, label %if11.then, label %endif2

if11.then:                                        ; preds = %if11
  call void @String.append(%String* %res, i8 45)
  br label %endif2

endif2:                                           ; preds = %if11.then, %if11
  %call.String.reverse = call %String @String.reverse(%String* %res)
  call void @String.drop(%String* %res)
  ret %String %call.String.reverse
}

define void @String.join(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  store i32 0, i32* %i, align 4
  br label %while12

while12:                                          ; preds = %while12.then, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$i" = load i32, i32* %i, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$i"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while12.then, label %endloop

while12.then:                                     ; preds = %while12
  %"*self" = load %String*, %String** %self, align 8
  %"$text1" = load i8*, i8** %text, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"text[]3" = getelementptr inbounds i8, i8* %"$text1", i32 %"$i2"
  %"$text[]4" = load i8, i8* %"text[]3", align 1
  call void @String.append(%String* %"*self", i8 %"$text[]4")
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while12

endloop:                                          ; preds = %while12
  ret void
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

define %String @String.reverse(%String* %0) {
entry:
  %self = alloca %String*, align 8
  %res = alloca %String, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store %String zeroinitializer, %String* %res, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  store i32 %SUB, i32* %i, align 4
  br label %while13

while13:                                          ; preds = %while13.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %GE = icmp sge i32 %"$i", 0
  br i1 %GE, label %while13.then, label %endloop

while13.then:                                     ; preds = %while13
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i2" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i2"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  call void @String.append(%String* %res, i8 %"$*self.data[]")
  %"$i3" = load i32, i32* %i, align 4
  %SUB4 = sub i32 %"$i3", 1
  store i32 %SUB4, i32* %i, align 4
  br label %while13

endloop:                                          ; preds = %while13
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define i32 @main() {
entry:
  %s1 = alloca %String, align 8
  %bool.text = alloca i8*, align 8
  %s2 = alloca %String, align 8
  %bool.text6 = alloca i8*, align 8
  %s3 = alloca %String, align 8
  %bool.text12 = alloca i8*, align 8
  %s4 = alloca %String, align 8
  %s5 = alloca %String, align 8
  %s6 = alloca %String, align 8
  %call.String.create = call %String @String.create(i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str, i32 0, i32 0))
  store %String %call.String.create, %String* %s1, align 8
  %call.String.size = call i32 @String.size(%String* %s1)
  %call.String.is_empty = call i1 @String.is_empty(%String* %s1)
  store i8* null, i8** %bool.text, align 8
  br label %if14

if14:                                             ; preds = %entry
  br i1 %call.String.is_empty, label %if14.then, label %else15

if14.then:                                        ; preds = %if14
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else15:                                           ; preds = %if14
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else15, %if14.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.4, i32 0, i32 0), i32 %call.String.size, i8* %"$bool.text")
  %call.String.create3 = call %String @String.create(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.5, i32 0, i32 0))
  store %String %call.String.create3, %String* %s2, align 8
  %call.String.chars = call i8* @String.chars(%String* %s2)
  %call.String.size4 = call i32 @String.size(%String* %s2)
  %call.String.is_empty5 = call i1 @String.is_empty(%String* %s2)
  store i8* null, i8** %bool.text6, align 8
  br label %if16

if16:                                             ; preds = %endif
  br i1 %call.String.is_empty5, label %if16.then, label %else17

if16.then:                                        ; preds = %if16
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.6, i32 0, i32 0), i8** %bool.text6, align 8
  br label %endif1

else17:                                           ; preds = %if16
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.7, i32 0, i32 0), i8** %bool.text6, align 8
  br label %endif1

endif1:                                           ; preds = %else17, %if16.then
  %"$bool.text7" = load i8*, i8** %bool.text6, align 8
  %call.dprintf8 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.8, i32 0, i32 0), i8* %call.String.chars, i32 %call.String.size4, i8* %"$bool.text7")
  %call.String.create9 = call %String @String.create(i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.9, i32 0, i32 0))
  store %String %call.String.create9, %String* %s3, align 8
  %call.String.size10 = call i32 @String.size(%String* %s3)
  %call.String.is_empty11 = call i1 @String.is_empty(%String* %s3)
  store i8* null, i8** %bool.text12, align 8
  br label %if18

if18:                                             ; preds = %endif1
  br i1 %call.String.is_empty11, label %if18.then, label %else19

if18.then:                                        ; preds = %if18
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.10, i32 0, i32 0), i8** %bool.text12, align 8
  br label %endif2

else19:                                           ; preds = %if18
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.11, i32 0, i32 0), i8** %bool.text12, align 8
  br label %endif2

endif2:                                           ; preds = %else19, %if18.then
  %"$bool.text13" = load i8*, i8** %bool.text12, align 8
  %call.dprintf14 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.12, i32 0, i32 0), i32 %call.String.size10, i8* %"$bool.text13")
  %call.String.create15 = call %String @String.create.1(i32 42)
  store %String %call.String.create15, %String* %s4, align 8
  %call.String.chars16 = call i8* @String.chars(%String* %s4)
  %call.dprintf17 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.13, i32 0, i32 0), i8* %call.String.chars16)
  %call.String.create18 = call %String @String.create.1(i32 -123)
  store %String %call.String.create18, %String* %s5, align 8
  %call.String.chars19 = call i8* @String.chars(%String* %s5)
  %call.dprintf20 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.14, i32 0, i32 0), i8* %call.String.chars19)
  %call.String.create21 = call %String @String.create.1(i32 0)
  store %String %call.String.create21, %String* %s6, align 8
  %call.String.chars22 = call i8* @String.chars(%String* %s6)
  %call.dprintf23 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.15, i32 0, i32 0), i8* %call.String.chars22)
  %call.String.at = call i8 @String.at(%String* %s2, i32 0)
  %call.String.at24 = call i8 @String.at(%String* %s2, i32 4)
  %call.String.at25 = call i8 @String.at(%String* %s2, i32 -1)
  %AS = sext i8 %call.String.at25 to i32
  %call.String.at26 = call i8 @String.at(%String* %s2, i32 99)
  %AS27 = sext i8 %call.String.at26 to i32
  %AS28 = sext i8 %call.String.at to i32
  %AS29 = sext i8 %call.String.at24 to i32
  %call.dprintf30 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.16, i32 0, i32 0), i32 %AS28, i32 %AS29, i32 %AS, i32 %AS27)
  %call.String.chars31 = call i8* @String.chars(%String* %s2)
  %call.dprintf32 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.17, i32 0, i32 0), i8* %call.String.chars31)
  call void @String.drop(%String* %s6)
  call void @String.drop(%String* %s5)
  call void @String.drop(%String* %s4)
  call void @String.drop(%String* %s3)
  call void @String.drop(%String* %s2)
  call void @String.drop(%String* %s1)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 017 — string: assign, push, pop, join and clear

```ura
// stdout: abc 3
// stdout: hello world this is a longer string
// stdout: 0 True
// stdout: XYZ 3
// stdout: Z XY 2
// stdout: 0 0
// stdout: hello world
// stdout: 0 True
// expect: 0
use "@/header"

fn main() i32:
    s String = String::create("")
    s = "abc"
    output(s, " ", s.size(), "\n")

    s = "hello world this is a longer string"
    output(s, "\n")

    s = ""
    output(s.size(), " ", s.is_empty(), "\n")

    s2 String = String::create("")
    s2.append('X')
    s2.append('Y')
    s2.append('Z')
    output(s2, " ", s2.size(), "\n")

    c char = s2.pop()
    output(c, " ", s2, " ", s2.size(), "\n")

    s2.pop()
    s2.pop()
    c2 char = s2.pop()
    output(c2 as i32, " ", s2.size(), "\n")

    s3 String = String::create("hello")
    s3.join(" world")
    output(s3, "\n")

    s3.clear()
    output(s3.size(), " ", s3.is_empty(), "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.2 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@str.3 = private unnamed_addr constant [7 x i8] c"%s %d\0A\00", align 1
@str.4 = private unnamed_addr constant [36 x i8] c"hello world this is a longer string\00", align 1
@str.5 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.6 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.7 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.8 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.9 = private unnamed_addr constant [7 x i8] c"%d %s\0A\00", align 1
@str.10 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.11 = private unnamed_addr constant [7 x i8] c"%s %d\0A\00", align 1
@str.12 = private unnamed_addr constant [10 x i8] c"%c %s %d\0A\00", align 1
@str.13 = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1
@str.14 = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@str.15 = private unnamed_addr constant [7 x i8] c" world\00", align 1
@str.16 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.17 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.18 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.19 = private unnamed_addr constant [7 x i8] c"%d %s\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  ret void

endif1:                                           ; preds = %if1
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if2

if2:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @String.own(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %AND = and i1 %EQ, %GT
  br i1 %AND, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self2" = load %String*, %String** %self, align 8
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  call void @String.reserve(%String* %"*self2", i32 %"$*self.count5")
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  ret void
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if4

if4:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if4.then, label %endif

if4.then:                                         ; preds = %if4
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if4.then, %if4
  br label %while5

while5:                                           ; preds = %while5.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while5.then, label %endloop

while5.then:                                      ; preds = %while5
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.count5" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.count9" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while5

endloop:                                          ; preds = %while5
  ret void
}

define void @String.append(%String* %0, i8 %1) {
entry:
  %self = alloca %String*, align 8
  %value = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store i8 %1, i8* %value, align 1
  br label %if6

if6:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*self2" = load %String*, %String** %self, align 8
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @String.reserve(%String* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.count8" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count9"
  %"$value" = load i8, i8* %value, align 1
  store i8 %"$value", i8* %"*self.data[]", align 1
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  ret void
}

define i8 @String.pop(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %index = alloca i32, align 4
  %value = alloca i8, align 1
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %index, align 4
  store i8 0, i8* %value, align 1
  br label %if7

if7:                                              ; preds = %entry
  %"$index" = load i32, i32* %index, align 4
  %LT = icmp slt i32 %"$index", 0
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$index1" = load i32, i32* %index, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GE = icmp sge i32 %"$index1", %"$*self.count"
  %OR = or i1 %LT, %GE
  br i1 %OR, label %if7.then, label %endif

if7.then:                                         ; preds = %if7
  %"$value" = load i8, i8* %value, align 1
  ret i8 %"$value"

endif:                                            ; preds = %if7
  %"*self2" = load %String*, %String** %self, align 8
  call void @String.own(%String* %"*self2")
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$index4" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$index4"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  store i8 %"$*self.data[]", i8* %value, align 1
  %"$index5" = load i32, i32* %index, align 4
  store i32 %"$index5", i32* %i, align 4
  br label %while8

while8:                                           ; preds = %while8.then, %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.count7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 1
  %"$*self.count8" = load i32, i32* %"*self.count7", align 4
  %SUB = sub i32 %"$*self.count8", 1
  %"$i" = load i32, i32* %i, align 4
  %LT9 = icmp slt i32 %"$i", %SUB
  br i1 %LT9, label %while8.then, label %endloop

while8.then:                                      ; preds = %while8
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.data11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 0
  %"$*self.data12" = load i8*, i8** %"*self.data11", align 8
  %"$i13" = load i32, i32* %i, align 4
  %"*self.data[]14" = getelementptr inbounds i8, i8* %"$*self.data12", i32 %"$i13"
  %"*self15" = load %String*, %String** %self, align 8
  %"*self.data16" = getelementptr inbounds %String, %String* %"*self15", i32 0, i32 0
  %"$i17" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i17", 1
  %"$*self.data18" = load i8*, i8** %"*self.data16", align 8
  %"*self.data[]19" = getelementptr inbounds i8, i8* %"$*self.data18", i32 %ADD
  %"$*self.data[]20" = load i8, i8* %"*self.data[]19", align 1
  store i8 %"$*self.data[]20", i8* %"*self.data[]14", align 1
  %"$i21" = load i32, i32* %i, align 4
  %ADD22 = add i32 %"$i21", 1
  store i32 %ADD22, i32* %i, align 4
  br label %while8

endloop:                                          ; preds = %while8
  %"*self23" = load %String*, %String** %self, align 8
  %"*self.count24" = getelementptr inbounds %String, %String* %"*self23", i32 0, i32 1
  %"$*self.count25" = load i32, i32* %"*self.count24", align 4
  %SUB26 = sub i32 %"$*self.count25", 1
  store i32 %SUB26, i32* %"*self.count24", align 4
  %"*self27" = load %String*, %String** %self, align 8
  %"*self.data28" = getelementptr inbounds %String, %String* %"*self27", i32 0, i32 0
  %"*self29" = load %String*, %String** %self, align 8
  %"*self.count30" = getelementptr inbounds %String, %String* %"*self29", i32 0, i32 1
  %"$*self.data31" = load i8*, i8** %"*self.data28", align 8
  %"$*self.count32" = load i32, i32* %"*self.count30", align 4
  %"*self.data[]33" = getelementptr inbounds i8, i8* %"$*self.data31", i32 %"$*self.count32"
  store i8 0, i8* %"*self.data[]33", align 1
  %"$value34" = load i8, i8* %value, align 1
  ret i8 %"$value34"
}

define i8 @String.pop.1(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  %call.String.pop = call i8 @String.pop(%String* %"*self", i32 %SUB)
  ret i8 %call.String.pop
}

define i32 @String.size(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i1 @String.is_empty(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %EQ = icmp eq i32 %"$*self.count", 0
  ret i1 %EQ
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if9

if9:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if9.then, label %endif

if9.then:                                         ; preds = %if9
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if9.then, %if9
  ret void
}

define %String @String.create(i8* %0) {
entry:
  %text = alloca i8*, align 8
  %res = alloca %String, align 8
  store i8* %0, i8** %text, align 8
  store %String zeroinitializer, %String* %res, align 8
  %"$text" = load i8*, i8** %text, align 8
  call void @String.join(%String* %res, i8* %"$text")
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define void @String.join(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  store i32 0, i32* %i, align 4
  br label %while10

while10:                                          ; preds = %while10.then, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$i" = load i32, i32* %i, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$i"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while10.then, label %endloop

while10.then:                                     ; preds = %while10
  %"*self" = load %String*, %String** %self, align 8
  %"$text1" = load i8*, i8** %text, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"text[]3" = getelementptr inbounds i8, i8* %"$text1", i32 %"$i2"
  %"$text[]4" = load i8, i8* %"text[]3", align 1
  call void @String.append(%String* %"*self", i8 %"$text[]4")
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while10

endloop:                                          ; preds = %while10
  ret void
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

define i32 @main() {
entry:
  %s = alloca %String, align 8
  %bool.text = alloca i8*, align 8
  %s2 = alloca %String, align 8
  %c = alloca i8, align 1
  %c2 = alloca i8, align 1
  %s3 = alloca %String, align 8
  %bool.text24 = alloca i8*, align 8
  %call.String.create = call %String @String.create(i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str, i32 0, i32 0))
  store %String %call.String.create, %String* %s, align 8
  call void @String.drop(%String* %s)
  store %String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 3, i32 0 }, %String* %s, align 8
  %call.String.chars = call i8* @String.chars(%String* %s)
  %call.String.size = call i32 @String.size(%String* %s)
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.3, i32 0, i32 0), i8* %call.String.chars, i32 %call.String.size)
  call void @String.drop(%String* %s)
  store %String { i8* getelementptr inbounds ([36 x i8], [36 x i8]* @str.4, i32 0, i32 0), i32 35, i32 0 }, %String* %s, align 8
  %call.String.chars2 = call i8* @String.chars(%String* %s)
  %call.dprintf3 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.5, i32 0, i32 0), i8* %call.String.chars2)
  call void @String.drop(%String* %s)
  store %String { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.6, i32 0, i32 0), i32 0, i32 0 }, %String* %s, align 8
  %call.String.size4 = call i32 @String.size(%String* %s)
  %call.String.is_empty = call i1 @String.is_empty(%String* %s)
  store i8* null, i8** %bool.text, align 8
  br label %if11

if11:                                             ; preds = %entry
  br i1 %call.String.is_empty, label %if11.then, label %else12

if11.then:                                        ; preds = %if11
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.7, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else12:                                           ; preds = %if11
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.8, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else12, %if11.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf5 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.9, i32 0, i32 0), i32 %call.String.size4, i8* %"$bool.text")
  %call.String.create6 = call %String @String.create(i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.10, i32 0, i32 0))
  store %String %call.String.create6, %String* %s2, align 8
  call void @String.append(%String* %s2, i8 88)
  call void @String.append(%String* %s2, i8 89)
  call void @String.append(%String* %s2, i8 90)
  %call.String.chars7 = call i8* @String.chars(%String* %s2)
  %call.String.size8 = call i32 @String.size(%String* %s2)
  %call.dprintf9 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.11, i32 0, i32 0), i8* %call.String.chars7, i32 %call.String.size8)
  %call.String.pop = call i8 @String.pop.1(%String* %s2)
  store i8 %call.String.pop, i8* %c, align 1
  %call.String.chars10 = call i8* @String.chars(%String* %s2)
  %call.String.size11 = call i32 @String.size(%String* %s2)
  %"$c" = load i8, i8* %c, align 1
  %AS = sext i8 %"$c" to i32
  %call.dprintf12 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.12, i32 0, i32 0), i32 %AS, i8* %call.String.chars10, i32 %call.String.size11)
  %call.String.pop13 = call i8 @String.pop.1(%String* %s2)
  %call.String.pop14 = call i8 @String.pop.1(%String* %s2)
  %call.String.pop15 = call i8 @String.pop.1(%String* %s2)
  store i8 %call.String.pop15, i8* %c2, align 1
  %"$c2" = load i8, i8* %c2, align 1
  %AS16 = sext i8 %"$c2" to i32
  %call.String.size17 = call i32 @String.size(%String* %s2)
  %call.dprintf18 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.13, i32 0, i32 0), i32 %AS16, i32 %call.String.size17)
  %call.String.create19 = call %String @String.create(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.14, i32 0, i32 0))
  store %String %call.String.create19, %String* %s3, align 8
  call void @String.join(%String* %s3, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.15, i32 0, i32 0))
  %call.String.chars20 = call i8* @String.chars(%String* %s3)
  %call.dprintf21 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.16, i32 0, i32 0), i8* %call.String.chars20)
  call void @String.clear(%String* %s3)
  %call.String.size22 = call i32 @String.size(%String* %s3)
  %call.String.is_empty23 = call i1 @String.is_empty(%String* %s3)
  store i8* null, i8** %bool.text24, align 8
  br label %if13

if13:                                             ; preds = %endif
  br i1 %call.String.is_empty23, label %if13.then, label %else14

if13.then:                                        ; preds = %if13
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.17, i32 0, i32 0), i8** %bool.text24, align 8
  br label %endif1

else14:                                           ; preds = %if13
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.18, i32 0, i32 0), i8** %bool.text24, align 8
  br label %endif1

endif1:                                           ; preds = %else14, %if13.then
  %"$bool.text25" = load i8*, i8** %bool.text24, align 8
  %call.dprintf26 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.19, i32 0, i32 0), i32 %call.String.size22, i8* %"$bool.text25")
  call void @String.drop(%String* %s3)
  call void @String.drop(%String* %s2)
  call void @String.drop(%String* %s)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 018 — string: find, contains, starts_with and ends_with

```ura
// stdout: 6
// stdout: 0
// stdout: -1
// stdout: 0
// stdout: 10
// stdout: True
// stdout: False
// stdout: True
// stdout: False
// stdout: True
// stdout: True
// stdout: False
// stdout: True
// stdout: -1
// stdout: False
// stdout: True
// stdout: True
// expect: 0
use "@/header"

fn main() i32:
    s String = String::create("hello world")

    output(s.find("world"), "\n")
    output(s.find("hello"), "\n")
    output(s.find("xyz"), "\n")
    output(s.find(""), "\n")
    output(s.find("d"), "\n")

    output(s.contains("world"), "\n")
    output(s.contains("xyz"), "\n")

    output(s.starts_with("hello"), "\n")
    output(s.starts_with("world"), "\n")
    output(s.starts_with(""), "\n")

    output(s.ends_with("world"), "\n")
    output(s.ends_with("hello"), "\n")
    output(s.ends_with("d"), "\n")

    empty String = String::create("")
    output(empty.find("x"), "\n")
    output(empty.contains("x"), "\n")
    output(empty.starts_with(""), "\n")
    output(empty.ends_with(""), "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [12 x i8] c"hello world\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"world\00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@str.4 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.5 = private unnamed_addr constant [4 x i8] c"xyz\00", align 1
@str.6 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.7 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.8 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.9 = private unnamed_addr constant [2 x i8] c"d\00", align 1
@str.10 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.11 = private unnamed_addr constant [6 x i8] c"world\00", align 1
@str.12 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.13 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.14 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.15 = private unnamed_addr constant [4 x i8] c"xyz\00", align 1
@str.16 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.17 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.18 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.19 = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@str.20 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.21 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.22 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.23 = private unnamed_addr constant [6 x i8] c"world\00", align 1
@str.24 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.25 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.26 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.27 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.28 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.29 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.30 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.31 = private unnamed_addr constant [6 x i8] c"world\00", align 1
@str.32 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.33 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.34 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.35 = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@str.36 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.37 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.38 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.39 = private unnamed_addr constant [2 x i8] c"d\00", align 1
@str.40 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.41 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.42 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.43 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.44 = private unnamed_addr constant [2 x i8] c"x\00", align 1
@str.45 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.46 = private unnamed_addr constant [2 x i8] c"x\00", align 1
@str.47 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.48 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.49 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.50 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.51 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.52 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.53 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.54 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.55 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.56 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.57 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  ret void

endif1:                                           ; preds = %if1
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if2

if2:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %while4

while4:                                           ; preds = %while4.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.count5" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.count9" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while4

endloop:                                          ; preds = %while4
  ret void
}

define void @String.append(%String* %0, i8 %1) {
entry:
  %self = alloca %String*, align 8
  %value = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store i8 %1, i8* %value, align 1
  br label %if5

if5:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @String.reserve(%String* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if5.then, %if5
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.count8" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count9"
  %"$value" = load i8, i8* %value, align 1
  store i8 %"$value", i8* %"*self.data[]", align 1
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  ret void
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if6

if6:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  ret void
}

define %String @String.create(i8* %0) {
entry:
  %text = alloca i8*, align 8
  %res = alloca %String, align 8
  store i8* %0, i8** %text, align 8
  store %String zeroinitializer, %String* %res, align 8
  %"$text" = load i8*, i8** %text, align 8
  call void @String.join(%String* %res, i8* %"$text")
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define i32 @String.length_of(i8* %0) {
entry:
  %text = alloca i8*, align 8
  %n = alloca i32, align 4
  store i8* %0, i8** %text, align 8
  store i32 0, i32* %n, align 4
  br label %while7

while7:                                           ; preds = %while7.then, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$n" = load i32, i32* %n, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$n"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while7.then, label %endloop

while7.then:                                      ; preds = %while7
  %"$n1" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n1", 1
  store i32 %ADD, i32* %n, align 4
  br label %while7

endloop:                                          ; preds = %while7
  %"$n2" = load i32, i32* %n, align 4
  ret i32 %"$n2"
}

define void @String.join(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  store i32 0, i32* %i, align 4
  br label %while8

while8:                                           ; preds = %while8.then, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$i" = load i32, i32* %i, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$i"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while8.then, label %endloop

while8.then:                                      ; preds = %while8
  %"*self" = load %String*, %String** %self, align 8
  %"$text1" = load i8*, i8** %text, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"text[]3" = getelementptr inbounds i8, i8* %"$text1", i32 %"$i2"
  %"$text[]4" = load i8, i8* %"text[]3", align 1
  call void @String.append(%String* %"*self", i8 %"$text[]4")
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while8

endloop:                                          ; preds = %while8
  ret void
}

define i1 @String.matches_at(%String* %0, i32 %1, i8* %2) {
entry:
  %self = alloca %String*, align 8
  %index = alloca i32, align 4
  %text = alloca i8*, align 8
  %j = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %index, align 4
  store i8* %2, i8** %text, align 8
  store i32 0, i32* %j, align 4
  br label %while9

while9:                                           ; preds = %endif1, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$j" = load i32, i32* %j, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$j"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while9.then, label %endloop

while9.then:                                      ; preds = %while9
  br label %if10

if10:                                             ; preds = %while9.then
  %"$index" = load i32, i32* %index, align 4
  %"$j2" = load i32, i32* %j, align 4
  %ADD = add i32 %"$index", %"$j2"
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GE = icmp sge i32 %ADD, %"$*self.count"
  br i1 %GE, label %if10.then, label %endif

if10.then:                                        ; preds = %if10
  ret i1 false

endif:                                            ; preds = %if10
  br label %if11

if11:                                             ; preds = %endif
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 0
  %"$index4" = load i32, i32* %index, align 4
  %"$j5" = load i32, i32* %j, align 4
  %ADD6 = add i32 %"$index4", %"$j5"
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %ADD6
  %"$text7" = load i8*, i8** %text, align 8
  %"$j8" = load i32, i32* %j, align 4
  %"text[]9" = getelementptr inbounds i8, i8* %"$text7", i32 %"$j8"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  %"$text[]10" = load i8, i8* %"text[]9", align 1
  %NQ11 = icmp ne i8 %"$*self.data[]", %"$text[]10"
  br i1 %NQ11, label %if11.then, label %endif1

if11.then:                                        ; preds = %if11
  ret i1 false

endif1:                                           ; preds = %if11
  %"$j12" = load i32, i32* %j, align 4
  %ADD13 = add i32 %"$j12", 1
  store i32 %ADD13, i32* %j, align 4
  br label %while9

endloop:                                          ; preds = %while9
  ret i1 true
}

define i32 @String.find(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  store i32 0, i32* %i, align 4
  br label %while12

while12:                                          ; preds = %endif, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count"
  br i1 %LT, label %while12.then, label %endloop

while12.then:                                     ; preds = %while12
  br label %if13

if13:                                             ; preds = %while12.then
  %"*self1" = load %String*, %String** %self, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"$text" = load i8*, i8** %text, align 8
  %call.String.matches_at = call i1 @String.matches_at(%String* %"*self1", i32 %"$i2", i8* %"$text")
  br i1 %call.String.matches_at, label %if13.then, label %endif

if13.then:                                        ; preds = %if13
  %"$i3" = load i32, i32* %i, align 4
  ret i32 %"$i3"

endif:                                            ; preds = %if13
  %"$i4" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i4", 1
  store i32 %ADD, i32* %i, align 4
  br label %while12

endloop:                                          ; preds = %while12
  ret i32 -1
}

define i1 @String.contains(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"$text" = load i8*, i8** %text, align 8
  %call.String.find = call i32 @String.find(%String* %"*self", i8* %"$text")
  %NQ = icmp ne i32 %call.String.find, -1
  ret i1 %NQ
}

define i1 @String.starts_with(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"$text" = load i8*, i8** %text, align 8
  %call.String.matches_at = call i1 @String.matches_at(%String* %"*self", i32 0, i8* %"$text")
  ret i1 %call.String.matches_at
}

define i1 @String.ends_with(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  %n = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  %"$text" = load i8*, i8** %text, align 8
  %call.String.length_of = call i32 @String.length_of(i8* %"$text")
  store i32 %call.String.length_of, i32* %n, align 4
  br label %if14

if14:                                             ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$n" = load i32, i32* %n, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$n", %"$*self.count"
  br i1 %GT, label %if14.then, label %endif

if14.then:                                        ; preds = %if14
  ret i1 false

endif:                                            ; preds = %if14
  %"*self1" = load %String*, %String** %self, align 8
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count4" = load i32, i32* %"*self.count3", align 4
  %"$n5" = load i32, i32* %n, align 4
  %SUB = sub i32 %"$*self.count4", %"$n5"
  %"$text6" = load i8*, i8** %text, align 8
  %call.String.matches_at = call i1 @String.matches_at(%String* %"*self1", i32 %SUB, i8* %"$text6")
  ret i1 %call.String.matches_at
}

define i32 @main() {
entry:
  %s = alloca %String, align 8
  %bool.text = alloca i8*, align 8
  %bool.text21 = alloca i8*, align 8
  %bool.text24 = alloca i8*, align 8
  %bool.text28 = alloca i8*, align 8
  %bool.text32 = alloca i8*, align 8
  %bool.text35 = alloca i8*, align 8
  %bool.text39 = alloca i8*, align 8
  %bool.text43 = alloca i8*, align 8
  %empty = alloca %String, align 8
  %bool.text50 = alloca i8*, align 8
  %bool.text54 = alloca i8*, align 8
  %bool.text58 = alloca i8*, align 8
  %call.String.create = call %String @String.create(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0))
  store %String %call.String.create, %String* %s, align 8
  %call.String.find = call i32 @String.find(%String* %s, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0))
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 %call.String.find)
  %call.String.find11 = call i32 @String.find(%String* %s, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0))
  %call.dprintf12 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i32 %call.String.find11)
  %call.String.find13 = call i32 @String.find(%String* %s, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.5, i32 0, i32 0))
  %call.dprintf14 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.6, i32 0, i32 0), i32 %call.String.find13)
  %call.String.find15 = call i32 @String.find(%String* %s, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.7, i32 0, i32 0))
  %call.dprintf16 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.8, i32 0, i32 0), i32 %call.String.find15)
  %call.String.find17 = call i32 @String.find(%String* %s, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.9, i32 0, i32 0))
  %call.dprintf18 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.10, i32 0, i32 0), i32 %call.String.find17)
  %call.String.contains = call i1 @String.contains(%String* %s, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.11, i32 0, i32 0))
  store i8* null, i8** %bool.text, align 8
  br label %if15

if15:                                             ; preds = %entry
  br i1 %call.String.contains, label %if15.then, label %else16

if15.then:                                        ; preds = %if15
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.12, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else16:                                           ; preds = %if15
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.13, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else16, %if15.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf19 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.14, i32 0, i32 0), i8* %"$bool.text")
  %call.String.contains20 = call i1 @String.contains(%String* %s, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.15, i32 0, i32 0))
  store i8* null, i8** %bool.text21, align 8
  br label %if17

if17:                                             ; preds = %endif
  br i1 %call.String.contains20, label %if17.then, label %else18

if17.then:                                        ; preds = %if17
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.16, i32 0, i32 0), i8** %bool.text21, align 8
  br label %endif1

else18:                                           ; preds = %if17
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.17, i32 0, i32 0), i8** %bool.text21, align 8
  br label %endif1

endif1:                                           ; preds = %else18, %if17.then
  %"$bool.text22" = load i8*, i8** %bool.text21, align 8
  %call.dprintf23 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.18, i32 0, i32 0), i8* %"$bool.text22")
  %call.String.starts_with = call i1 @String.starts_with(%String* %s, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.19, i32 0, i32 0))
  store i8* null, i8** %bool.text24, align 8
  br label %if19

if19:                                             ; preds = %endif1
  br i1 %call.String.starts_with, label %if19.then, label %else20

if19.then:                                        ; preds = %if19
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.20, i32 0, i32 0), i8** %bool.text24, align 8
  br label %endif2

else20:                                           ; preds = %if19
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.21, i32 0, i32 0), i8** %bool.text24, align 8
  br label %endif2

endif2:                                           ; preds = %else20, %if19.then
  %"$bool.text25" = load i8*, i8** %bool.text24, align 8
  %call.dprintf26 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.22, i32 0, i32 0), i8* %"$bool.text25")
  %call.String.starts_with27 = call i1 @String.starts_with(%String* %s, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.23, i32 0, i32 0))
  store i8* null, i8** %bool.text28, align 8
  br label %if21

if21:                                             ; preds = %endif2
  br i1 %call.String.starts_with27, label %if21.then, label %else22

if21.then:                                        ; preds = %if21
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.24, i32 0, i32 0), i8** %bool.text28, align 8
  br label %endif3

else22:                                           ; preds = %if21
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.25, i32 0, i32 0), i8** %bool.text28, align 8
  br label %endif3

endif3:                                           ; preds = %else22, %if21.then
  %"$bool.text29" = load i8*, i8** %bool.text28, align 8
  %call.dprintf30 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.26, i32 0, i32 0), i8* %"$bool.text29")
  %call.String.starts_with31 = call i1 @String.starts_with(%String* %s, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.27, i32 0, i32 0))
  store i8* null, i8** %bool.text32, align 8
  br label %if23

if23:                                             ; preds = %endif3
  br i1 %call.String.starts_with31, label %if23.then, label %else24

if23.then:                                        ; preds = %if23
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.28, i32 0, i32 0), i8** %bool.text32, align 8
  br label %endif4

else24:                                           ; preds = %if23
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.29, i32 0, i32 0), i8** %bool.text32, align 8
  br label %endif4

endif4:                                           ; preds = %else24, %if23.then
  %"$bool.text33" = load i8*, i8** %bool.text32, align 8
  %call.dprintf34 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.30, i32 0, i32 0), i8* %"$bool.text33")
  %call.String.ends_with = call i1 @String.ends_with(%String* %s, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.31, i32 0, i32 0))
  store i8* null, i8** %bool.text35, align 8
  br label %if25

if25:                                             ; preds = %endif4
  br i1 %call.String.ends_with, label %if25.then, label %else26

if25.then:                                        ; preds = %if25
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.32, i32 0, i32 0), i8** %bool.text35, align 8
  br label %endif5

else26:                                           ; preds = %if25
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.33, i32 0, i32 0), i8** %bool.text35, align 8
  br label %endif5

endif5:                                           ; preds = %else26, %if25.then
  %"$bool.text36" = load i8*, i8** %bool.text35, align 8
  %call.dprintf37 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.34, i32 0, i32 0), i8* %"$bool.text36")
  %call.String.ends_with38 = call i1 @String.ends_with(%String* %s, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.35, i32 0, i32 0))
  store i8* null, i8** %bool.text39, align 8
  br label %if27

if27:                                             ; preds = %endif5
  br i1 %call.String.ends_with38, label %if27.then, label %else28

if27.then:                                        ; preds = %if27
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.36, i32 0, i32 0), i8** %bool.text39, align 8
  br label %endif6

else28:                                           ; preds = %if27
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.37, i32 0, i32 0), i8** %bool.text39, align 8
  br label %endif6

endif6:                                           ; preds = %else28, %if27.then
  %"$bool.text40" = load i8*, i8** %bool.text39, align 8
  %call.dprintf41 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.38, i32 0, i32 0), i8* %"$bool.text40")
  %call.String.ends_with42 = call i1 @String.ends_with(%String* %s, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.39, i32 0, i32 0))
  store i8* null, i8** %bool.text43, align 8
  br label %if29

if29:                                             ; preds = %endif6
  br i1 %call.String.ends_with42, label %if29.then, label %else30

if29.then:                                        ; preds = %if29
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.40, i32 0, i32 0), i8** %bool.text43, align 8
  br label %endif7

else30:                                           ; preds = %if29
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.41, i32 0, i32 0), i8** %bool.text43, align 8
  br label %endif7

endif7:                                           ; preds = %else30, %if29.then
  %"$bool.text44" = load i8*, i8** %bool.text43, align 8
  %call.dprintf45 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.42, i32 0, i32 0), i8* %"$bool.text44")
  %call.String.create46 = call %String @String.create(i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.43, i32 0, i32 0))
  store %String %call.String.create46, %String* %empty, align 8
  %call.String.find47 = call i32 @String.find(%String* %empty, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.44, i32 0, i32 0))
  %call.dprintf48 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.45, i32 0, i32 0), i32 %call.String.find47)
  %call.String.contains49 = call i1 @String.contains(%String* %empty, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.46, i32 0, i32 0))
  store i8* null, i8** %bool.text50, align 8
  br label %if31

if31:                                             ; preds = %endif7
  br i1 %call.String.contains49, label %if31.then, label %else32

if31.then:                                        ; preds = %if31
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.47, i32 0, i32 0), i8** %bool.text50, align 8
  br label %endif8

else32:                                           ; preds = %if31
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.48, i32 0, i32 0), i8** %bool.text50, align 8
  br label %endif8

endif8:                                           ; preds = %else32, %if31.then
  %"$bool.text51" = load i8*, i8** %bool.text50, align 8
  %call.dprintf52 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.49, i32 0, i32 0), i8* %"$bool.text51")
  %call.String.starts_with53 = call i1 @String.starts_with(%String* %empty, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.50, i32 0, i32 0))
  store i8* null, i8** %bool.text54, align 8
  br label %if33

if33:                                             ; preds = %endif8
  br i1 %call.String.starts_with53, label %if33.then, label %else34

if33.then:                                        ; preds = %if33
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.51, i32 0, i32 0), i8** %bool.text54, align 8
  br label %endif9

else34:                                           ; preds = %if33
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.52, i32 0, i32 0), i8** %bool.text54, align 8
  br label %endif9

endif9:                                           ; preds = %else34, %if33.then
  %"$bool.text55" = load i8*, i8** %bool.text54, align 8
  %call.dprintf56 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.53, i32 0, i32 0), i8* %"$bool.text55")
  %call.String.ends_with57 = call i1 @String.ends_with(%String* %empty, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.54, i32 0, i32 0))
  store i8* null, i8** %bool.text58, align 8
  br label %if35

if35:                                             ; preds = %endif9
  br i1 %call.String.ends_with57, label %if35.then, label %else36

if35.then:                                        ; preds = %if35
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.55, i32 0, i32 0), i8** %bool.text58, align 8
  br label %endif10

else36:                                           ; preds = %if35
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.56, i32 0, i32 0), i8** %bool.text58, align 8
  br label %endif10

endif10:                                          ; preds = %else36, %if35.then
  %"$bool.text59" = load i8*, i8** %bool.text58, align 8
  %call.dprintf60 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.57, i32 0, i32 0), i8* %"$bool.text59")
  call void @String.drop(%String* %empty)
  call void @String.drop(%String* %s)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

