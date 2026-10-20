# milestone7

## index

- 001 — enum: auto values start at 0 and count up
- 002 — enum: variants on one line, comma-separated
- 003 — enum: an explicit value resets the running counter
- 004 — enum: an enum-typed variable compared to a variant
- 005 — enum: match over an enum inside a function
- 006 — enum: an enum used as a struct field type
- 007 — enum: a variant as an array index and in arithmetic
- 008 — enum: a variant used before the enum is declared
- 009 — enum: an enum must declare at least one variant
- 010 — enum: a variant name cannot be declared twice
- 011 — enum: an explicit variant value must be an integer
- 012 — match: a single-value case matches
- 013 — match: a case with several values
- 014 — match: default runs when nothing matches
- 015 — match: break leaves the match early
- 016 — match: break exits the match, not the enclosing loop
- 017 — match: continue applies to the loop, skipping the rest of the iteration
- 018 — match: a case needs at least one value
- 019 — match: case value type must match the subject
- 020 — match: a case with no enclosing match
- 021 — match: a default with no enclosing match
- 022 — match: break needs a loop or match

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
    output(RED, " ", GREEN, " ", BLUE, "\n")
    return 0
```

---

## 002 — enum: variants on one line, comma-separated

```ura
// stdout: 012
// expect: 0
enum Color: RED, GREEN, BLUE

fn main() i32:
    output(RED, GREEN, BLUE, "\n")
    return 0
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
    output(NORTH, EAST, SOUTH, WEST, "\n")
    return 0
```

---

## 004 — enum: an enum-typed variable compared to a variant

```ura
// stdout: green
// expect: 0
enum Color: RED, GREEN, BLUE

fn main() i32:
    c Color = GREEN
    if c == GREEN:
        output("green\n")
    else:
        output("other\n")
    return 0
```

---

## 005 — enum: match over an enum inside a function

```ura
// stdout: 1 -1
// expect: 0
enum Signal: LOW, HIGH, FLOAT

fn describe(s Signal) i32:
    match s:
        case LOW:
            return 0
        case HIGH:
            return 1
        default:
            return -1
    return -2

fn main() i32:
    output(describe(HIGH), " ", describe(FLOAT), "\n")
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
    h.kind = MAGE
    h.hp = 30
    output(h.kind, " ", h.hp, "\n")
    return 0
```

---

## 007 — enum: a variant as an array index and in arithmetic

```ura
// stdout: 14 2
// expect: 0
enum Slot: HEAD, BODY, LEGS

fn main() i32:
    gear i32[] = new i32[3]
    gear[HEAD] = 5
    gear[LEGS] = 9
    output(gear[HEAD] + gear[LEGS], " ", LEGS - HEAD, "\n")
    return 0
```

---

## 008 — enum: a variant used before the enum is declared

```ura
// stdout: 0
// expect: 0
fn first() Color:
    return RED

enum Color: RED, GREEN, BLUE

fn main() i32:
    output(first(), "\n")
    return 0
```

---

## 009 — enum: an enum must declare at least one variant

```ura
// error: Enum Nothing must declare at least one variant
enum Nothing:

fn main() i32:
    output(0, "\n")
    return 0
```

---

## 010 — enum: a variant name cannot be declared twice

```ura
// error: Redeclaration of variable 'RED'
enum C:
    RED, GREEN, RED

fn main() i32:
    output(0, "\n")
    return 0
```

---

## 011 — enum: an explicit variant value must be an integer

```ura
// error: Enum variant 'A' needs an integer value after '='
enum C:
    A = hello

fn main() i32:
    output(0, "\n")
    return 0
```

---

## 012 — match: a single-value case matches

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

## 013 — match: a case with several values

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

## 014 — match: default runs when nothing matches

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

## 015 — match: break leaves the match early

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

## 016 — match: break exits the match, not the enclosing loop

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

## 017 — match: continue applies to the loop, skipping the rest of the iteration

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

## 018 — match: a case needs at least one value

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

## 019 — match: case value type must match the subject

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

## 020 — match: a case with no enclosing match

```ura
// error: 'case' without a matching 'match'
// errors/match/003 — a case with no enclosing match
fn main() i32:
    case 1:
        output("x\n")
    return 0
```

---

## 021 — match: a default with no enclosing match

```ura
// error: 'default' without a matching 'match'
// errors/match/004 — a default with no enclosing match
fn main() i32:
    default:
        output("x\n")
    return 0
```

---

## 022 — match: break needs a loop or match

```ura
// error: 'break' outside a loop or match
// errors/match/005 — break needs a loop or match
fn main() i32:
    break
    return 0
```

