# names

## index

- 001 — undeclared variable
- 002 — a typo in a variable name
- 003 — a variable used before its declaration
- 004 — a variable declared twice
- 005 — unknown type
- 006 — undeclared function
- 007 — a typo in a function name
- 008 — wrong number of arguments
- 009 — a function declared twice
- 010 — a struct declared twice
- 011 — unknown attribute
- 012 — unknown method
- 013 — break outside a loop
- 014 — continue outside a loop
- 015 — self outside a method
- 016 — calling a variable

---

## 001 — undeclared variable

```ura
// error: undeclared variable 'y'
// help: declare it first, like: y i32 = 0
fn main() i32:
    x i32 = 1
    return y
```

### stderr

```
error: test.ura:5:12 undeclared variable 'y'
  |
5 |     return y
  |            ^
help: declare it first, like: y i32 = 0
```

### status

```
1
```

---

## 002 — a typo in a variable name

```ura
// error: undeclared variable 'cuont'
// help: did you mean 'count'?
fn main() i32:
    count i32 = 3
    return cuont
```

### stderr

```
error: test.ura:5:12 undeclared variable 'cuont'
  |
5 |     return cuont
  |            ^^^^^
help: did you mean 'count'?
```

### status

```
1
```

---

## 003 — a variable used before its declaration

```ura
// error: 'x' is used before its declaration, on line 5
// help: move the declaration above this line
fn main() i32:
    y i32 = x + 1
    x i32 = 1
    return y
```

### stderr

```
error: test.ura:4:13 'x' is used before its declaration, on line 5
  |
4 |     y i32 = x + 1
  |             ^
help: move the declaration above this line
```

### status

```
1
```

---

## 004 — a variable declared twice

```ura
// error: 'x' is already declared, on line 4
// help: use x = 2
fn main() i32:
    x i32 = 1
    x i32 = 2
    return x
```

### stderr

```
error: test.ura:5:5 'x' is already declared, on line 4
  |
5 |     x i32 = 2
  |     ^
help: use x = 2
```

### status

```
1
```

---

## 005 — unknown type

```ura
// error: unknown type 'Point'
// help: declare it: struct Point:
fn main() i32:
    p Point
    return 0
```

### stderr

```
error: test.ura:4:7 unknown type 'Point'
  |
4 |     p Point
  |       ^^^^^
help: declare it: struct Point:
```

### status

```
1
```

---

## 006 — undeclared function

```ura
// error: no function named 'foo'
fn main() i32:
    return foo()
```

### stderr

```
error: test.ura:3:12 no function named 'foo'
  |
3 |     return foo()
  |            ^^^
```

### status

```
1
```

---

## 007 — a typo in a function name

```ura
// error: no function named 'ad'
// help: did you mean 'add'?
fn add(a i32, b i32) i32:
    return a + b

fn main() i32:
    return ad(1, 2)
```

### stderr

```
error: test.ura:7:12 no function named 'ad'
  |
7 |     return ad(1, 2)
  |            ^^
help: did you mean 'add'?
```

### status

```
1
```

---

## 008 — wrong number of arguments

```ura
// error: 'add' takes 2 arguments, not 1
// help: fn add(a i32, b i32) i32
fn add(a i32, b i32) i32:
    return a + b

fn main() i32:
    return add(1)
```

### stderr

```
error: test.ura:7:12 'add' takes 2 arguments, not 1
  |
7 |     return add(1)
  |            ^^^
help: fn add(a i32, b i32) i32
```

### status

```
1
```

---

## 009 — a function declared twice

```ura
// error: function 'f' is already declared, on line 2
fn f() i32:
    return 1

fn f() i32:
    return 2

fn main() i32:
    return f()
```

### stderr

```
error: test.ura:5:4 function 'f' is already declared, on line 2
  |
5 | fn f() i32:
  |    ^
```

### status

```
1
```

---

## 010 — a struct declared twice

```ura
// error: struct 'User' is already declared, on line 2
struct User:
    a i32

struct User:
    b i32

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:5:8 struct 'User' is already declared, on line 2
  |
5 | struct User:
  |        ^^^^
```

### status

```
1
```

---

## 011 — unknown attribute

```ura
// error: User has no attribute 'age'
// help: User has: name, id
struct User:
    name i8[]
    id i32

fn main() i32:
    u User
    return u.age
```

### stderr

```
error: test.ura:9:14 User has no attribute 'age'
  |
9 |     return u.age
  |              ^^^
help: User has: name, id
```

### status

```
1
```

---

## 012 — unknown method

```ura
// error: User has no method 'run'
// help: User has: get
struct User:
    a i32

    fn get() i32:
        return self.a

fn main() i32:
    u User
    return u.run()
```

### stderr

```
error: test.ura:11:14 User has no method 'run'
   |
11 |     return u.run()
   |              ^^^
help: User has: get
```

### status

```
1
```

---

## 013 — break outside a loop

```ura
// error: break outside a loop
// help: break only works inside a while
fn main() i32:
    break
    return 0
```

### stderr

```
error: test.ura:4:5 break outside a loop
  |
4 |     break
  |     ^^^^^
help: break only works inside a while
```

### status

```
1
```

---

## 014 — continue outside a loop

```ura
// error: continue outside a loop
// help: continue only works inside a while
fn main() i32:
    if True:
        continue
    return 0
```

### stderr

```
error: test.ura:5:9 continue outside a loop
  |
5 |         continue
  |         ^^^^^^^^
help: continue only works inside a while
```

### status

```
1
```

---

## 015 — self outside a method

```ura
// error: 'self' only exists inside a method
fn get() i32:
    return self.a

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:3:12 'self' only exists inside a method
  |
3 |     return self.a
  |            ^^^^
```

### status

```
1
```

---

## 016 — calling a variable

```ura
// error: 'x' is a variable, not a function
fn main() i32:
    x i32 = 1
    return x()
```

### stderr

```
error: test.ura:4:12 'x' is a variable, not a function
  |
4 |     return x()
  |            ^
```

### status

```
1
```

