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

---

## 017 — match: a case needs at least one value

```ura
// error: Expected an expression after 'case'
// errors/match/001 — a case needs at least one value
fn main() i32:
    a i32 = 1
    match a:
        case:
            output("x\n")
    return 0
```

---

## 018 — match: case value type must match the subject

```ura
// error: This case value is array but the subject is i32; they must be the same type
// errors/match/002 — case value type must match the subject
fn main() i32:
    a i32 = 1
    match a:
        case "x":
            output("x\n")
    return 0
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

---

## 021 — match: break needs a loop or match

```ura
// error: 'break' outside a loop or match
// errors/match/005 — break needs a loop or match
fn main() i32:
    break
    return 0
```

