# milestone7

## index

- 001 — enum: auto values start at 0 and count up
- 002 — enum: variants on one line, comma-separated
- 003 — enum: an explicit value resets the running counter
- 004 — enum: an enum-typed variable compared to a variant
- 005 — enum: match over an enum inside a function
- 006 — enum: an enum used as a struct field type
- 007 — enum: a variant used before the enum is declared
- 008 — enum: an enum must declare at least one variant
- 009 — enum: a variant name cannot be declared twice
- 010 — enum: an explicit variant value must be an integer
- 011 — match: a single-value case matches
- 012 — match: a case with several values
- 013 — match: default runs when nothing matches
- 014 — match: break leaves the match early
- 015 — match: break exits the match, not the enclosing loop
- 016 — match: continue applies to the loop, skipping the rest of the iteration
- 017 — match: a case needs at least one value
- 018 — match: case value type must match the subject
- 019 — match: a case with no enclosing match
- 020 — match: a default with no enclosing match
- 021 — match: break needs a loop or match
- 022 — match: a String subject compares through operator ==, break and continue drop it once

---

## 001 — enum: auto values start at 0 and count up

```ura
// stdout: 0 1 2
// expect: 0
enum Color:
    RED,
    GREEN,
    BLUE,

fn main() i32:
    output(Color::RED, " ", Color::GREEN, " ", Color::BLUE, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %arr = alloca [2 x i8], align 1
  %arr1 = alloca [2 x i8], align 1
  %arr2 = alloca [2 x i8], align 1
  %arr3 = alloca [10 x i8], align 1
  store [2 x i8] c" \00", [2 x i8]* %arr, align 1
  %0 = getelementptr inbounds [2 x i8], [2 x i8]* %arr, i32 0, i32 0
  store [2 x i8] c" \00", [2 x i8]* %arr1, align 1
  %1 = getelementptr inbounds [2 x i8], [2 x i8]* %arr1, i32 0, i32 0
  store [2 x i8] c"\0A\00", [2 x i8]* %arr2, align 1
  %2 = getelementptr inbounds [2 x i8], [2 x i8]* %arr2, i32 0, i32 0
  store [10 x i8] c"%d %d %d\0A\00", [10 x i8]* %arr3, align 1
  %3 = getelementptr inbounds [10 x i8], [10 x i8]* %arr3, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %3, i32 0, i32 1, i32 2)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 002 — enum: variants on one line, comma-separated

```ura
// stdout: 012
// expect: 0
enum Color: RED, GREEN, BLUE

fn main() i32:
    output(Color::RED, Color::GREEN, Color::BLUE, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %arr = alloca [2 x i8], align 1
  %arr1 = alloca [8 x i8], align 1
  store [2 x i8] c"\0A\00", [2 x i8]* %arr, align 1
  %0 = getelementptr inbounds [2 x i8], [2 x i8]* %arr, i32 0, i32 0
  store [8 x i8] c"%d%d%d\0A\00", [8 x i8]* %arr1, align 1
  %1 = getelementptr inbounds [8 x i8], [8 x i8]* %arr1, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %1, i32 0, i32 1, i32 2)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 003 — enum: an explicit value resets the running counter

```ura
// stdout: 1234
// expect: 0
enum Dir:
    NORTH = 1,
    EAST,
    SOUTH,
    WEST,

fn main() i32:
    output(Dir::NORTH, Dir::EAST, Dir::SOUTH, Dir::WEST, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %arr = alloca [2 x i8], align 1
  %arr1 = alloca [10 x i8], align 1
  store [2 x i8] c"\0A\00", [2 x i8]* %arr, align 1
  %0 = getelementptr inbounds [2 x i8], [2 x i8]* %arr, i32 0, i32 0
  store [10 x i8] c"%d%d%d%d\0A\00", [10 x i8]* %arr1, align 1
  %1 = getelementptr inbounds [10 x i8], [10 x i8]* %arr1, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %1, i32 1, i32 2, i32 3, i32 4)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 004 — enum: an enum-typed variable compared to a variant

```ura
// stdout: green
// expect: 0
enum Color: RED, GREEN, BLUE

fn main() i32:
    c Color = Color::GREEN
    if c == Color::GREEN:
        output("green\n")
    else:
        output("other\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %c = alloca i32, align 4
  %arr = alloca [7 x i8], align 1
  %arr1 = alloca [7 x i8], align 1
  %arr2 = alloca [7 x i8], align 1
  %arr3 = alloca [7 x i8], align 1
  store i32 1, i32* %c, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$c" = load i32, i32* %c, align 4
  %EQ = icmp eq i32 %"$c", 1
  br i1 %EQ, label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  store [7 x i8] c"green\0A\00", [7 x i8]* %arr, align 1
  %0 = getelementptr inbounds [7 x i8], [7 x i8]* %arr, i32 0, i32 0
  store [7 x i8] c"green\0A\00", [7 x i8]* %arr1, align 1
  %1 = getelementptr inbounds [7 x i8], [7 x i8]* %arr1, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %1)
  br label %endif

else1:                                            ; preds = %if0
  store [7 x i8] c"other\0A\00", [7 x i8]* %arr2, align 1
  %2 = getelementptr inbounds [7 x i8], [7 x i8]* %arr2, i32 0, i32 0
  store [7 x i8] c"other\0A\00", [7 x i8]* %arr3, align 1
  %3 = getelementptr inbounds [7 x i8], [7 x i8]* %arr3, i32 0, i32 0
  %call.dprintf4 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %3)
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 005 — enum: match over an enum inside a function

```ura
// stdout: 1 -1
// expect: 0
enum Signal: LOW, HIGH, FLOAT

fn describe(s Signal) i32:
    match s:
        case Signal::LOW:
            return 0
        case Signal::HIGH:
            return 1
        default:
            return -1
    return -2

fn main() i32:
    output(describe(Signal::HIGH), " ", describe(Signal::FLOAT), "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @describe(i32 %0) {
entry:
  %s = alloca i32, align 4
  %"match@91" = alloca i32, align 4
  store i32 %0, i32* %s, align 4
  %"$s" = load i32, i32* %s, align 4
  store i32 %"$s", i32* %"match@91", align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$match@91" = load i32, i32* %"match@91", align 4
  %EQ = icmp eq i32 %"$match@91", 0
  br i1 %EQ, label %if0.then, label %elif1

if0.then:                                         ; preds = %if0
  ret i32 0

elif1:                                            ; preds = %if0
  %"$match@911" = load i32, i32* %"match@91", align 4
  %EQ2 = icmp eq i32 %"$match@911", 1
  br i1 %EQ2, label %elif1.then, label %else2

elif1.then:                                       ; preds = %elif1
  ret i32 1

else2:                                            ; preds = %elif1
  ret i32 -1

endif:                                            ; No predecessors!
  ret i32 -2
}

define i32 @main() {
entry:
  %arr = alloca [2 x i8], align 1
  %arr2 = alloca [2 x i8], align 1
  %arr3 = alloca [7 x i8], align 1
  %call.describe = call i32 @describe(i32 1)
  store [2 x i8] c" \00", [2 x i8]* %arr, align 1
  %0 = getelementptr inbounds [2 x i8], [2 x i8]* %arr, i32 0, i32 0
  %call.describe1 = call i32 @describe(i32 2)
  store [2 x i8] c"\0A\00", [2 x i8]* %arr2, align 1
  %1 = getelementptr inbounds [2 x i8], [2 x i8]* %arr2, i32 0, i32 0
  store [7 x i8] c"%d %d\0A\00", [7 x i8]* %arr3, align 1
  %2 = getelementptr inbounds [7 x i8], [7 x i8]* %arr3, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %2, i32 %call.describe, i32 %call.describe1)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 006 — enum: an enum used as a struct field type

```ura
// stdout: 1 30
// expect: 0
enum Kind: WARRIOR, MAGE, ROGUE

struct Hero:
    kind Kind
    hp i32

fn main() i32:
    h Hero
    h.kind = Kind::MAGE
    h.hp = 30
    output(h.kind, " ", h.hp, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Hero = type { i32, i32 }

define i32 @main() {
entry:
  %h = alloca %Hero, align 8
  %arr = alloca [2 x i8], align 1
  %arr3 = alloca [2 x i8], align 1
  %arr4 = alloca [7 x i8], align 1
  store %Hero zeroinitializer, %Hero* %h, align 4
  %h.kind = getelementptr inbounds %Hero, %Hero* %h, i32 0, i32 0
  store i32 1, i32* %h.kind, align 4
  %h.hp = getelementptr inbounds %Hero, %Hero* %h, i32 0, i32 1
  store i32 30, i32* %h.hp, align 4
  %h.kind1 = getelementptr inbounds %Hero, %Hero* %h, i32 0, i32 0
  store [2 x i8] c" \00", [2 x i8]* %arr, align 1
  %0 = getelementptr inbounds [2 x i8], [2 x i8]* %arr, i32 0, i32 0
  %h.hp2 = getelementptr inbounds %Hero, %Hero* %h, i32 0, i32 1
  store [2 x i8] c"\0A\00", [2 x i8]* %arr3, align 1
  %1 = getelementptr inbounds [2 x i8], [2 x i8]* %arr3, i32 0, i32 0
  %"$h.kind" = load i32, i32* %h.kind1, align 4
  %"$h.hp" = load i32, i32* %h.hp2, align 4
  store [7 x i8] c"%d %d\0A\00", [7 x i8]* %arr4, align 1
  %2 = getelementptr inbounds [7 x i8], [7 x i8]* %arr4, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %2, i32 %"$h.kind", i32 %"$h.hp")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 007 — enum: a variant used before the enum is declared

```ura
// stdout: 0
// expect: 0
fn first() Color:
    return Color::RED

enum Color: RED, GREEN, BLUE

fn main() i32:
    output(first(), "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @first() {
entry:
  ret i32 0
}

define i32 @main() {
entry:
  %arr = alloca [2 x i8], align 1
  %arr1 = alloca [4 x i8], align 1
  %call.first = call i32 @first()
  store [2 x i8] c"\0A\00", [2 x i8]* %arr, align 1
  %0 = getelementptr inbounds [2 x i8], [2 x i8]* %arr, i32 0, i32 0
  store [4 x i8] c"%d\0A\00", [4 x i8]* %arr1, align 1
  %1 = getelementptr inbounds [4 x i8], [4 x i8]* %arr1, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %1, i32 %call.first)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 008 — enum: an enum must declare at least one variant

```ura
// error: enum 'Nothing' has no variants
enum Nothing:

fn main() i32:
    output(0, "\n")
    return 0
```

### stderr

```
error: test.ura:2:1 enum 'Nothing' has no variants
  |
2 | enum Nothing:
  | ^^^^
help: add some: enum Nothing: A, B
error: aborting due to 1 error
```

### status

```
1
```

---

## 009 — enum: a variant name cannot be declared twice

```ura
// error: 'RED' is already declared
enum C:
    RED, GREEN, RED

fn main() i32:
    output(0, "\n")
    return 0
```

### stderr

```
error: test.ura:3:17 'RED' is already declared
  |
3 |     RED, GREEN, RED
  |                 ^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 010 — enum: an explicit variant value must be an integer

```ura
// error: the value of 'A' must be an integer
enum C:
    A = hello

fn main() i32:
    output(0, "\n")
    return 0
```

### stderr

```
error: test.ura:3:9 the value of 'A' must be an integer
  |
3 |     A = hello
  |         ^^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 011 — match: a single-value case matches

```ura
// stdout: one
// expect: 0
fn main() i32:
    a i32 = 1
    match a:
        case 1:
            output("one\n")
        default:
            output("other\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %"match@61" = alloca i32, align 4
  %arr = alloca [5 x i8], align 1
  %arr1 = alloca [5 x i8], align 1
  %arr2 = alloca [7 x i8], align 1
  %arr3 = alloca [7 x i8], align 1
  store i32 1, i32* %a, align 4
  %"$a" = load i32, i32* %a, align 4
  store i32 %"$a", i32* %"match@61", align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$match@61" = load i32, i32* %"match@61", align 4
  %EQ = icmp eq i32 %"$match@61", 1
  br i1 %EQ, label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  store [5 x i8] c"one\0A\00", [5 x i8]* %arr, align 1
  %0 = getelementptr inbounds [5 x i8], [5 x i8]* %arr, i32 0, i32 0
  store [5 x i8] c"one\0A\00", [5 x i8]* %arr1, align 1
  %1 = getelementptr inbounds [5 x i8], [5 x i8]* %arr1, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %1)
  br label %endif

else1:                                            ; preds = %if0
  store [7 x i8] c"other\0A\00", [7 x i8]* %arr2, align 1
  %2 = getelementptr inbounds [7 x i8], [7 x i8]* %arr2, i32 0, i32 0
  store [7 x i8] c"other\0A\00", [7 x i8]* %arr3, align 1
  %3 = getelementptr inbounds [7 x i8], [7 x i8]* %arr3, i32 0, i32 0
  %call.dprintf4 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %3)
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 012 — match: a case with several values

```ura
// stdout: small
// expect: 0
fn main() i32:
    a i32 = 3
    match a:
        case 1, 2, 3:
            output("small\n")
        default:
            output("big\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %"match@63" = alloca i32, align 4
  %arr = alloca [7 x i8], align 1
  %arr6 = alloca [7 x i8], align 1
  %arr7 = alloca [5 x i8], align 1
  %arr8 = alloca [5 x i8], align 1
  store i32 3, i32* %a, align 4
  %"$a" = load i32, i32* %a, align 4
  store i32 %"$a", i32* %"match@63", align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$match@63" = load i32, i32* %"match@63", align 4
  %EQ = icmp eq i32 %"$match@63", 1
  %"$match@631" = load i32, i32* %"match@63", align 4
  %EQ2 = icmp eq i32 %"$match@631", 2
  %OR = or i1 %EQ, %EQ2
  %"$match@633" = load i32, i32* %"match@63", align 4
  %EQ4 = icmp eq i32 %"$match@633", 3
  %OR5 = or i1 %OR, %EQ4
  br i1 %OR5, label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  store [7 x i8] c"small\0A\00", [7 x i8]* %arr, align 1
  %0 = getelementptr inbounds [7 x i8], [7 x i8]* %arr, i32 0, i32 0
  store [7 x i8] c"small\0A\00", [7 x i8]* %arr6, align 1
  %1 = getelementptr inbounds [7 x i8], [7 x i8]* %arr6, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %1)
  br label %endif

else1:                                            ; preds = %if0
  store [5 x i8] c"big\0A\00", [5 x i8]* %arr7, align 1
  %2 = getelementptr inbounds [5 x i8], [5 x i8]* %arr7, i32 0, i32 0
  store [5 x i8] c"big\0A\00", [5 x i8]* %arr8, align 1
  %3 = getelementptr inbounds [5 x i8], [5 x i8]* %arr8, i32 0, i32 0
  %call.dprintf9 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %3)
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 013 — match: default runs when nothing matches

```ura
// stdout: fallback
// expect: 0
fn main() i32:
    a i32 = 99
    match a:
        case 1:
            output("one\n")
        default:
            output("fallback\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %"match@67" = alloca i32, align 4
  %arr = alloca [5 x i8], align 1
  %arr1 = alloca [5 x i8], align 1
  %arr2 = alloca [10 x i8], align 1
  %arr3 = alloca [10 x i8], align 1
  store i32 99, i32* %a, align 4
  %"$a" = load i32, i32* %a, align 4
  store i32 %"$a", i32* %"match@67", align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$match@67" = load i32, i32* %"match@67", align 4
  %EQ = icmp eq i32 %"$match@67", 1
  br i1 %EQ, label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  store [5 x i8] c"one\0A\00", [5 x i8]* %arr, align 1
  %0 = getelementptr inbounds [5 x i8], [5 x i8]* %arr, i32 0, i32 0
  store [5 x i8] c"one\0A\00", [5 x i8]* %arr1, align 1
  %1 = getelementptr inbounds [5 x i8], [5 x i8]* %arr1, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %1)
  br label %endif

else1:                                            ; preds = %if0
  store [10 x i8] c"fallback\0A\00", [10 x i8]* %arr2, align 1
  %2 = getelementptr inbounds [10 x i8], [10 x i8]* %arr2, i32 0, i32 0
  store [10 x i8] c"fallback\0A\00", [10 x i8]* %arr3, align 1
  %3 = getelementptr inbounds [10 x i8], [10 x i8]* %arr3, i32 0, i32 0
  %call.dprintf4 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %3)
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 014 — match: break leaves the match early

```ura
// stdout: before
// expect: 0
fn main() i32:
    a i32 = 1
    match a:
        case 1:
            output("before\n")
            break
            output("after\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %"match@64" = alloca i32, align 4
  %arr = alloca [8 x i8], align 1
  %arr1 = alloca [8 x i8], align 1
  store i32 1, i32* %a, align 4
  %"$a" = load i32, i32* %a, align 4
  store i32 %"$a", i32* %"match@64", align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$match@64" = load i32, i32* %"match@64", align 4
  %EQ = icmp eq i32 %"$match@64", 1
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  store [8 x i8] c"before\0A\00", [8 x i8]* %arr, align 1
  %0 = getelementptr inbounds [8 x i8], [8 x i8]* %arr, i32 0, i32 0
  store [8 x i8] c"before\0A\00", [8 x i8]* %arr1, align 1
  %1 = getelementptr inbounds [8 x i8], [8 x i8]* %arr1, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %1)
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 015 — match: break exits the match, not the enclosing loop

```ura
// stdout: 1
// stdout: 2
// stdout: 3
// expect: 0
fn main() i32:
    i i32 = 0
    while i < 3:
        i = i + 1
        match i:
            case 2:
                break
        output(i, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %i = alloca i32, align 4
  %"match@124" = alloca i32, align 4
  %arr = alloca [2 x i8], align 1
  %arr4 = alloca [4 x i8], align 1
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %endif, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$i1" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i1", 1
  store i32 %ADD, i32* %i, align 4
  %"$i2" = load i32, i32* %i, align 4
  store i32 %"$i2", i32* %"match@124", align 4
  br label %if1

if1:                                              ; preds = %while0.then
  %"$match@124" = load i32, i32* %"match@124", align 4
  %EQ = icmp eq i32 %"$match@124", 2
  br i1 %EQ, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  br label %endif

endif:                                            ; preds = %if1.then, %if1
  store [2 x i8] c"\0A\00", [2 x i8]* %arr, align 1
  %0 = getelementptr inbounds [2 x i8], [2 x i8]* %arr, i32 0, i32 0
  %"$i3" = load i32, i32* %i, align 4
  store [4 x i8] c"%d\0A\00", [4 x i8]* %arr4, align 1
  %1 = getelementptr inbounds [4 x i8], [4 x i8]* %arr4, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %1, i32 %"$i3")
  br label %while0

endloop:                                          ; preds = %while0
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 016 — match: continue applies to the loop, skipping the rest of the iteration

```ura
// stdout: 1
// stdout: 3
// expect: 0
fn main() i32:
    i i32 = 0
    while i < 3:
        i = i + 1
        match i:
            case 2:
                continue
        output(i, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %i = alloca i32, align 4
  %"match@111" = alloca i32, align 4
  %arr = alloca [2 x i8], align 1
  %arr4 = alloca [4 x i8], align 1
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %endif, %if1.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$i1" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i1", 1
  store i32 %ADD, i32* %i, align 4
  %"$i2" = load i32, i32* %i, align 4
  store i32 %"$i2", i32* %"match@111", align 4
  br label %if1

if1:                                              ; preds = %while0.then
  %"$match@111" = load i32, i32* %"match@111", align 4
  %EQ = icmp eq i32 %"$match@111", 2
  br i1 %EQ, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  br label %while0

endif:                                            ; preds = %if1
  store [2 x i8] c"\0A\00", [2 x i8]* %arr, align 1
  %0 = getelementptr inbounds [2 x i8], [2 x i8]* %arr, i32 0, i32 0
  %"$i3" = load i32, i32* %i, align 4
  store [4 x i8] c"%d\0A\00", [4 x i8]* %arr4, align 1
  %1 = getelementptr inbounds [4 x i8], [4 x i8]* %arr4, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %1, i32 %"$i3")
  br label %while0

endloop:                                          ; preds = %while0
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 017 — match: a case needs at least one value

```ura
// error: expected a value after 'case'
// errors/match/001 — a case needs at least one value
fn main() i32:
    a i32 = 1
    match a:
        case:
            output("x\n")
    return 0
```

### stderr

```
error: test.ura:6:9 expected a value after 'case'
  |
6 |         case:
  |         ^^^^
help: like: case 1:
error: aborting due to 1 error
```

### status

```
1
```

---

## 018 — match: case value type must match the subject

```ura
// error: expected 'i32', found 'i8[]'
// errors/match/002 — case value type must match the subject
fn main() i32:
    a i32 = 1
    match a:
        case "x":
            output("x\n")
    return 0
```

### stderr

```
error: test.ura:6:14 expected 'i32', found 'i8[]'
  |
6 |         case "x":
  |              ^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 019 — match: a case with no enclosing match

```ura
// error: 'case' without a matching 'match'
// errors/match/003 — a case with no enclosing match
fn main() i32:
    case 1:
        output("x\n")
    return 0
```

### stderr

```
error: test.ura:4:5 'case' without a matching 'match'
  |
4 |     case 1:
  |     ^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 020 — match: a default with no enclosing match

```ura
// error: 'default' without a matching 'match'
// errors/match/004 — a default with no enclosing match
fn main() i32:
    default:
        output("x\n")
    return 0
```

### stderr

```
error: test.ura:4:5 'default' without a matching 'match'
  |
4 |     default:
  |     ^^^^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 021 — match: break needs a loop or match

```ura
// error: 'break' outside a loop or match
// errors/match/005 — break needs a loop or match
fn main() i32:
    break
    return 0
```

### stderr

```
error: test.ura:4:5 break outside a loop or match
  |
4 |     break
  |     ^^^^^
help: break only works inside a while or a match
error: aborting due to 1 error
```

### status

```
1
```

---

## 022 — match: a String subject compares through operator ==, break and continue drop it once

```ura
// stdout: 1120
// stdout: test
// expect: 0
use "@/string"

fn kind(s String) i32:
    match s:
        case "hello", "test":
            return 1
        case "stop":
            break
        default:
            return 0
    return 2

fn main() i32:
    name String = String::from("test")
    output(kind(String::from("hello")), kind(name), kind(String::from("stop")), kind(String::from("x")), "\n")
    i i32 = 0
    while i < 3:
        i += 1
        match String::from("abc"):
            case "abc":
                continue
        output("never\n")
    output(name, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%String = type { i8*, i32, i32 }

declare i64 @strlen(i8*)

declare i8* @memcpy(i8*, i8*, i32)

declare i8* @calloc(i64, i64)

declare void @free(i8*)

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
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap", %"$*self.cap"
  br i1 %LE, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret void

endif:                                            ; preds = %if0
  %"$cap1" = load i32, i32* %cap, align 4
  %AS = sext i32 %"$cap1" to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %bigger, align 8
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %MUL = mul i32 %"$*self.count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %call.memcpy = call i8* @memcpy(i8* %"$bigger", i8* %"$*self.data", i32 %MUL)
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.data5" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 0
  %"$*self.data6" = load i8*, i8** %"*self.data5", align 8
  call void @free(i8* %"$*self.data6")
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data8" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"$bigger9" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger9", i8** %"*self.data8", align 8
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.cap11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 2
  %"$cap12" = load i32, i32* %cap, align 4
  store i32 %"$cap12", i32* %"*self.cap11", align 4
  ret void
}

define void @String.push_back(%String* %0, i8 %1) {
entry:
  %self = alloca %String*, align 8
  %value = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store i8 %1, i8* %value, align 1
  br label %if1

if1:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.count", %"$*self.cap"
  br i1 %EQ, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  %"*self2" = load %String*, %String** %self, align 8
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.cap4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 2
  %"$*self.cap5" = load i32, i32* %"*self.cap4", align 4
  %MUL = mul i32 %"$*self.cap5", 2
  %ADD = add i32 %MUL, 4
  call void @String.reserve(%String* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if1.then, %if1
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

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %while2

while2:                                           ; preds = %while2.then, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while2.then, label %endloop

while2.then:                                      ; preds = %while2
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count2" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count3" = load i32, i32* %"*self.count2", align 4
  %SUB = sub i32 %"$*self.count3", 1
  store i32 %SUB, i32* %"*self.count2", align 4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 0
  %"*self5" = load %String*, %String** %self, align 8
  %"*self.count6" = getelementptr inbounds %String, %String* %"*self5", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count7" = load i32, i32* %"*self.count6", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count7"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while2

endloop:                                          ; preds = %while2
  ret void
}

define void @"String.="(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  %other = alloca %String*, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store %String* %1, %String** %other, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  %"*self1" = load %String*, %String** %self, align 8
  %"*other" = load %String*, %String** %other, align 8
  %"*other.count" = getelementptr inbounds %String, %String* %"*other", i32 0, i32 1
  %"$*other.count" = load i32, i32* %"*other.count", align 4
  call void @String.reserve(%String* %"*self1", i32 %"$*other.count")
  store i32 0, i32* %i, align 4
  br label %while3

while3:                                           ; preds = %while3.then, %entry
  %"*other2" = load %String*, %String** %other, align 8
  %"*other.count3" = getelementptr inbounds %String, %String* %"*other2", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*other.count4" = load i32, i32* %"*other.count3", align 4
  %LT = icmp slt i32 %"$i", %"$*other.count4"
  br i1 %LT, label %while3.then, label %endloop

while3.then:                                      ; preds = %while3
  %"*self5" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self5", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i6" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i6"
  %"*other7" = load %String*, %String** %other, align 8
  %"*other.data" = getelementptr inbounds %String, %String* %"*other7", i32 0, i32 0
  %"$*other.data" = load i8*, i8** %"*other.data", align 8
  %"$i8" = load i32, i32* %i, align 4
  %"*other.data[]" = getelementptr inbounds i8, i8* %"$*other.data", i32 %"$i8"
  %"$*other.data[]" = load i8, i8* %"*other.data[]", align 1
  store i8 %"$*other.data[]", i8* %"*self.data[]", align 1
  %"$i9" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i9", 1
  store i32 %ADD, i32* %i, align 4
  br label %while3

endloop:                                          ; preds = %while3
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"*other11" = load %String*, %String** %other, align 8
  %"*other.count12" = getelementptr inbounds %String, %String* %"*other11", i32 0, i32 1
  %"$*other.count13" = load i32, i32* %"*other.count12", align 4
  store i32 %"$*other.count13", i32* %"*self.count", align 4
  ret void
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @free(i8* %"$*self.data")
  ret void
}

define %String @String.from(i8* %0) {
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
  br label %while4

while4:                                           ; preds = %while4.then, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$i" = load i32, i32* %i, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$i"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
  %"*self" = load %String*, %String** %self, align 8
  %"$text1" = load i8*, i8** %text, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"text[]3" = getelementptr inbounds i8, i8* %"$text1", i32 %"$i2"
  %"$text[]4" = load i8, i8* %"text[]3", align 1
  call void @String.push_back(%String* %"*self", i8 %"$text[]4")
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while4

endloop:                                          ; preds = %while4
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
  br label %while5

while5:                                           ; preds = %endif1, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$j" = load i32, i32* %j, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$j"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while5.then, label %endloop

while5.then:                                      ; preds = %while5
  br label %if6

if6:                                              ; preds = %while5.then
  %"$index" = load i32, i32* %index, align 4
  %"$j2" = load i32, i32* %j, align 4
  %ADD = add i32 %"$index", %"$j2"
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GE = icmp sge i32 %ADD, %"$*self.count"
  br i1 %GE, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  ret i1 false

endif:                                            ; preds = %if6
  br label %if7

if7:                                              ; preds = %endif
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
  br i1 %NQ11, label %if7.then, label %endif1

if7.then:                                         ; preds = %if7
  ret i1 false

endif1:                                           ; preds = %if7
  %"$j12" = load i32, i32* %j, align 4
  %ADD13 = add i32 %"$j12", 1
  store i32 %ADD13, i32* %j, align 4
  br label %while5

endloop:                                          ; preds = %while5
  ret i1 true
}

define i1 @"String.=="(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$text" = load i8*, i8** %text, align 8
  %call.strlen = call i64 @strlen(i8* %"$text")
  %AS = trunc i64 %call.strlen to i32
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %EQ = icmp eq i32 %"$*self.count", %AS
  %"*self1" = load %String*, %String** %self, align 8
  %"$text2" = load i8*, i8** %text, align 8
  %call.String.matches_at = call i1 @String.matches_at(%String* %"*self1", i32 0, i8* %"$text2")
  %AND = and i1 %EQ, %call.String.matches_at
  ret i1 %AND
}

define i32 @kind(%String %0) {
entry:
  %s = alloca %String, align 8
  %"match@88" = alloca %String, align 8
  %copy = alloca %String, align 8
  %arr = alloca [6 x i8], align 1
  %arr1 = alloca [5 x i8], align 1
  %arr3 = alloca [5 x i8], align 1
  store %String %0, %String* %s, align 8
  store %String zeroinitializer, %String* %copy, align 8
  call void @"String.="(%String* %copy, %String* %s)
  %"$copy" = load %String, %String* %copy, align 8
  store %String %"$copy", %String* %"match@88", align 8
  br label %if8

if8:                                              ; preds = %entry
  store [6 x i8] c"hello\00", [6 x i8]* %arr, align 1
  %1 = getelementptr inbounds [6 x i8], [6 x i8]* %arr, i32 0, i32 0
  %"call.String.==" = call i1 @"String.=="(%String* %"match@88", i8* %1)
  store [5 x i8] c"test\00", [5 x i8]* %arr1, align 1
  %2 = getelementptr inbounds [5 x i8], [5 x i8]* %arr1, i32 0, i32 0
  %"call.String.==2" = call i1 @"String.=="(%String* %"match@88", i8* %2)
  %OR = or i1 %"call.String.==", %"call.String.==2"
  br i1 %OR, label %if8.then, label %elif9

if8.then:                                         ; preds = %if8
  call void @String.drop(%String* %"match@88")
  call void @String.drop(%String* %s)
  ret i32 1

elif9:                                            ; preds = %if8
  store [5 x i8] c"stop\00", [5 x i8]* %arr3, align 1
  %3 = getelementptr inbounds [5 x i8], [5 x i8]* %arr3, i32 0, i32 0
  %"call.String.==4" = call i1 @"String.=="(%String* %"match@88", i8* %3)
  br i1 %"call.String.==4", label %elif9.then, label %else10

elif9.then:                                       ; preds = %elif9
  br label %endif

else10:                                           ; preds = %elif9
  call void @String.drop(%String* %"match@88")
  call void @String.drop(%String* %s)
  ret i32 0

endif:                                            ; preds = %elif9.then
  call void @String.drop(%String* %"match@88")
  call void @String.drop(%String* %s)
  ret i32 2
}

define i32 @main() {
entry:
  %name = alloca %String, align 8
  %arr = alloca [5 x i8], align 1
  %arr1 = alloca [6 x i8], align 1
  %copy = alloca %String, align 8
  %arr4 = alloca [5 x i8], align 1
  %arr7 = alloca [2 x i8], align 1
  %arr10 = alloca [2 x i8], align 1
  %arr11 = alloca [10 x i8], align 1
  %i = alloca i32, align 4
  %"match@458" = alloca %String, align 8
  %arr13 = alloca [4 x i8], align 1
  %arr15 = alloca [4 x i8], align 1
  %arr16 = alloca [7 x i8], align 1
  %arr17 = alloca [7 x i8], align 1
  %arr19 = alloca [2 x i8], align 1
  %arr20 = alloca [4 x i8], align 1
  store [5 x i8] c"test\00", [5 x i8]* %arr, align 1
  %0 = getelementptr inbounds [5 x i8], [5 x i8]* %arr, i32 0, i32 0
  %call.String.from = call %String @String.from(i8* %0)
  store %String %call.String.from, %String* %name, align 8
  store [6 x i8] c"hello\00", [6 x i8]* %arr1, align 1
  %1 = getelementptr inbounds [6 x i8], [6 x i8]* %arr1, i32 0, i32 0
  %call.String.from2 = call %String @String.from(i8* %1)
  %call.kind = call i32 @kind(%String %call.String.from2)
  store %String zeroinitializer, %String* %copy, align 8
  call void @"String.="(%String* %copy, %String* %name)
  %"$copy" = load %String, %String* %copy, align 8
  %call.kind3 = call i32 @kind(%String %"$copy")
  store [5 x i8] c"stop\00", [5 x i8]* %arr4, align 1
  %2 = getelementptr inbounds [5 x i8], [5 x i8]* %arr4, i32 0, i32 0
  %call.String.from5 = call %String @String.from(i8* %2)
  %call.kind6 = call i32 @kind(%String %call.String.from5)
  store [2 x i8] c"x\00", [2 x i8]* %arr7, align 1
  %3 = getelementptr inbounds [2 x i8], [2 x i8]* %arr7, i32 0, i32 0
  %call.String.from8 = call %String @String.from(i8* %3)
  %call.kind9 = call i32 @kind(%String %call.String.from8)
  store [2 x i8] c"\0A\00", [2 x i8]* %arr10, align 1
  %4 = getelementptr inbounds [2 x i8], [2 x i8]* %arr10, i32 0, i32 0
  store [10 x i8] c"%d%d%d%d\0A\00", [10 x i8]* %arr11, align 1
  %5 = getelementptr inbounds [10 x i8], [10 x i8]* %arr11, i32 0, i32 0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %5, i32 %call.kind, i32 %call.kind3, i32 %call.kind6, i32 %call.kind9)
  store i32 0, i32* %i, align 4
  br label %while11

while11:                                          ; preds = %endif, %if12.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while11.then, label %endloop

while11.then:                                     ; preds = %while11
  %"$i12" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i12", 1
  store i32 %ADD, i32* %i, align 4
  store [4 x i8] c"abc\00", [4 x i8]* %arr13, align 1
  %6 = getelementptr inbounds [4 x i8], [4 x i8]* %arr13, i32 0, i32 0
  %call.String.from14 = call %String @String.from(i8* %6)
  store %String %call.String.from14, %String* %"match@458", align 8
  br label %if12

if12:                                             ; preds = %while11.then
  store [4 x i8] c"abc\00", [4 x i8]* %arr15, align 1
  %7 = getelementptr inbounds [4 x i8], [4 x i8]* %arr15, i32 0, i32 0
  %"call.String.==" = call i1 @"String.=="(%String* %"match@458", i8* %7)
  br i1 %"call.String.==", label %if12.then, label %endif

if12.then:                                        ; preds = %if12
  call void @String.drop(%String* %"match@458")
  br label %while11

endif:                                            ; preds = %if12
  store [7 x i8] c"never\0A\00", [7 x i8]* %arr16, align 1
  %8 = getelementptr inbounds [7 x i8], [7 x i8]* %arr16, i32 0, i32 0
  store [7 x i8] c"never\0A\00", [7 x i8]* %arr17, align 1
  %9 = getelementptr inbounds [7 x i8], [7 x i8]* %arr17, i32 0, i32 0
  %call.dprintf18 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %9)
  call void @String.drop(%String* %"match@458")
  br label %while11

endloop:                                          ; preds = %while11
  store [2 x i8] c"\0A\00", [2 x i8]* %arr19, align 1
  %10 = getelementptr inbounds [2 x i8], [2 x i8]* %arr19, i32 0, i32 0
  %"$name" = load %String, %String* %name, align 8
  store [4 x i8] c"%s\0A\00", [4 x i8]* %arr20, align 1
  %11 = getelementptr inbounds [4 x i8], [4 x i8]* %arr20, i32 0, i32 0
  %call.dprintf21 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* %11, %String %"$name")
  call void @String.drop(%String* %name)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

