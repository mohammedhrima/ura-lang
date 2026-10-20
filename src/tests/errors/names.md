# names

## index

- 001 — a struct declared twice
- 002 — unknown attribute
- 003 — unknown method
- 004 — self outside a method

---

## 001 — a struct declared twice

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

## 002 — unknown attribute

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

## 003 — unknown method

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

## 004 — self outside a method

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

