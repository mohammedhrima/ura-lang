# parser

## index

- 001 — missing : after a function header
- 002 — missing ( after a function name
- 003 — missing ) after the parameters
- 004 — a function with no name
- 005 — a parameter with no type
- 006 — missing : after an if condition
- 007 — missing : after a while condition
- 008 — else with a condition
- 009 — elif without an if
- 010 — unclosed (
- 011 — unclosed [ in an array literal
- 012 — nothing after =
- 013 — nothing after an operator
- 014 — unclosed ( in a call

---

## 001 — missing : after a function header

```ura
// error: expected ':' after the function header
// help: use fn main() i32:
fn main() i32
    return 0
```

### stderr

```
error: test.ura:3:11 expected ':' after the function header
  |
3 | fn main() i32
  |           ^^^
help: use fn main() i32:
```

### status

```
1
```

---

## 002 — missing ( after a function name

```ura
// error: expected '(' after the function name
// help: use fn main() i32:
fn main i32:
    return 0
```

### stderr

```
error: test.ura:3:9 expected '(' after the function name
  |
3 | fn main i32:
  |         ^^^
help: use fn main() i32:
```

### status

```
1
```

---

## 003 — missing ) after the parameters

```ura
// error: expected ')' after the parameters
// help: use fn add(a i32, b i32) i32:
fn add(a i32, b i32 i32:
    return a + b

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:3:21 expected ')' after the parameters
  |
3 | fn add(a i32, b i32 i32:
  |                     ^^^
help: use fn add(a i32, b i32) i32:
```

### status

```
1
```

---

## 004 — a function with no name

```ura
// error: expected a name after 'fn'
// help: use fn name() i32:
fn () i32:
    return 0
```

### stderr

```
error: test.ura:3:4 expected a name after 'fn'
  |
3 | fn () i32:
  |    ^
help: use fn name() i32:
```

### status

```
1
```

---

## 005 — a parameter with no type

```ura
// error: parameter 'a' has no type
// help: use a i32
fn twice(a) i32:
    return a * 2

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:3:10 parameter 'a' has no type
  |
3 | fn twice(a) i32:
  |          ^
help: use fn twice(a i32) i32:
```

### status

```
1
```

---

## 006 — missing : after an if condition

```ura
// error: expected ':' after the if condition
// help: use if x > 1:
fn main() i32:
    x i32 = 2
    if x > 1
        return 1
    return 0
```

### stderr

```
error: test.ura:5:12 expected ':' after the if condition
  |
5 |     if x > 1
  |            ^
help: use if x > 1:
```

### status

```
1
```

---

## 007 — missing : after a while condition

```ura
// error: expected ':' after the while condition
// help: use while i < 3:
fn main() i32:
    i i32 = 0
    while i < 3
        i += 1
    return i
```

### stderr

```
error: test.ura:5:15 expected ':' after the while condition
  |
5 |     while i < 3
  |               ^
help: use while i < 3:
```

### status

```
1
```

---

## 008 — else with a condition

```ura
// error: else has no condition
// help: use elif x > 1:
fn main() i32:
    x i32 = 2
    if x == 0:
        return 0
    else x > 1:
        return 1
    return 2
```

### stderr

```
error: test.ura:7:5 else has no condition
  |
7 |     else x > 1:
  |     ^^^^
help: use elif x > 1:
```

### status

```
1
```

---

## 009 — elif without an if

```ura
// error: elif without an if before it
// help: use if x == 1:
fn main() i32:
    x i32 = 1
    elif x == 1:
        return 1
    return 0
```

### stderr

```
error: test.ura:5:5 elif without an if before it
  |
5 |     elif x == 1:
  |     ^^^^
help: use if x == 1:
```

### status

```
1
```

---

## 010 — unclosed (

```ura
// error: expected ')' to close this '('
// help: use x i32 = (1 + 2)
fn main() i32:
    x i32 = (1 + 2
    return x
```

### stderr

```
error: test.ura:4:18 expected ')' to close this '('
  |
4 |     x i32 = (1 + 2
  |                  ^
help: use x i32 = (1 + 2)
```

### status

```
1
```

---

## 011 — unclosed [ in an array literal

```ura
// error: expected ']' to close this '['
// help: use a i32[] = [1, 2]
fn main() i32:
    a i32[] = [1, 2
    return a[0]
```

### stderr

```
error: test.ura:4:19 expected ']' to close this '['
  |
4 |     a i32[] = [1, 2
  |                   ^
help: use a i32[] = [1, 2]
```

### status

```
1
```

---

## 012 — nothing after =

```ura
// error: expected a value after '='
fn main() i32:
    x i32 =
    return 0
```

### stderr

```
error: test.ura:3:11 expected a value after '='
  |
3 |     x i32 =
  |           ^
```

### status

```
1
```

---

## 013 — nothing after an operator

```ura
// error: expected a value after '+'
fn main() i32:
    x i32 = 1 +
    return x
```

### stderr

```
error: test.ura:3:15 expected a value after '+'
  |
3 |     x i32 = 1 +
  |               ^
```

### status

```
1
```

---

## 014 — unclosed ( in a call

```ura
// error: expected ')' to close the call to 'add'
// help: use return add(1, 2)
fn add(a i32, b i32) i32:
    return a + b

fn main() i32:
    return add(1, 2
```

### stderr

```
error: test.ura:7:19 expected ')' to close the call to 'add'
  |
7 |     return add(1, 2
  |                   ^
help: use return add(1, 2)
```

### status

```
1
```

