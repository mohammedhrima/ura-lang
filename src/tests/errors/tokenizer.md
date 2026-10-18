# tokenizer

## index

- 001 — unknown character
- 002 — unclosed string
- 003 — unclosed character
- 004 — character with two letters
- 005 — unknown escape
- 006 — unclosed comment
- 007 — number too big for i32
- 008 — indentation that is not a multiple of 4
- 009 — two errors in one file: both are shown

---

## 001 — unknown character

```ura
// error: unknown character '$'
// help: remove it
fn main() i32:
    x i32 = 5 $ 3
    return x
```

### stderr

```
error: test.ura:4:15 unknown character '$'
  |
4 |     x i32 = 5 $ 3
  |               ^
help: remove it
```

### status

```
1
```

---

## 002 — unclosed string

```ura
// error: unclosed string
// help: add " at the end
fn main() i32:
    s i8[] = "abc
    return 0
```

### stderr

```
error: test.ura:4:14 unclosed string
  |
4 |     s i8[] = "abc
  |              ^^^^
help: use s i8[] = "abc"
```

### status

```
1
```

---

## 003 — unclosed character

```ura
// error: unclosed character
// help: add ' after the character
fn main() i32:
    c char = 'a
    return 0
```

### stderr

```
error: test.ura:4:14 unclosed character
  |
4 |     c char = 'a
  |              ^^
help: use c char = 'a'
```

### status

```
1
```

---

## 004 — character with two letters

```ura
// error: a character holds one letter, not 2
// help: use "ab" for a string
fn main() i32:
    c char = 'ab'
    return 0
```

### stderr

```
error: test.ura:4:14 a character holds one letter, not 2
  |
4 |     c char = 'ab'
  |              ^^^^
help: use "ab" for a string
```

### status

```
1
```

---

## 005 — unknown escape

```ura
// error: unknown escape '\q'
// help: the escapes are \n \t \r \0 \\ \" \'
fn main() i32:
    s i8[] = "a\qb"
    return 0
```

### stderr

```
error: test.ura:4:16 unknown escape '\q'
  |
4 |     s i8[] = "a\qb"
  |                ^^
help: the escapes are \n \t \r \0 \\ \" \'
```

### status

```
1
```

---

## 006 — unclosed comment

```ura
// error: unclosed comment
// help: add */ to close it
fn main() i32:
    /* this never ends
    return 0
```

### stderr

```
error: test.ura:4:5 unclosed comment
  |
4 |     /* this never ends
  |     ^^
help: add */ to close it
```

### status

```
1
```

---

## 007 — number too big for i32

```ura
// error: 99999999999 doesn't fit in i32
// help: i32 goes from -2147483648 to 2147483647
fn main() i32:
    x i32 = 99999999999
    return 0
```

### stderr

```
error: test.ura:4:13 99999999999 doesn't fit in i32
  |
4 |     x i32 = 99999999999
  |             ^^^^^^^^^^^
help: i32 goes from -2147483648 to 2147483647
```

### status

```
1
```

---

## 008 — indentation that is not a multiple of 4

```ura
// error: indentation of 3 spaces
// help: indent with a multiple of 4 spaces
fn main() i32:
   return 0
```

### stderr

```
error: test.ura:4:1 indentation of 3 spaces
  |
4 |    return 0
  | ^^^
help: indent with a multiple of 4 spaces
```

### status

```
1
```

---

## 009 — two errors in one file: both are shown

```ura
// error: unknown character '$'
// error: unknown character '@'
fn main() i32:
    x i32 = 5 $ 3
    y i32 = 2 @ 1
    return 0
```

### stderr

```
error: test.ura:4:15 unknown character '$'
  |
4 |     x i32 = 5 $ 3
  |               ^
help: remove it
error: test.ura:5:15 unknown character '@'
  |
5 |     y i32 = 2 @ 1
  |               ^
help: remove it
```

### status

```
1
```

