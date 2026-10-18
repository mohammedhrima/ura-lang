## errors 3: names

# undeclared variable
```ura
// error: undeclared variable 'y'
// help: declare it first, like: y i32 = 0
fn main() i32:
    x i32 = 1
    return y
```

# a typo in a variable name
```ura
// error: undeclared variable 'cuont'
// help: did you mean 'count'?
fn main() i32:
    count i32 = 3
    return cuont
```

# a variable used before its declaration
```ura
// error: 'x' is used before its declaration, on line 5
// help: move the declaration above this line
fn main() i32:
    y i32 = x + 1
    x i32 = 1
    return y
```

# a variable declared twice
```ura
// error: 'x' is already declared, on line 4
// help: use x = 2
fn main() i32:
    x i32 = 1
    x i32 = 2
    return x
```

# unknown type
```ura
// error: unknown type 'Point'
// help: declare it: struct Point:
fn main() i32:
    p Point
    return 0
```

# undeclared function
```ura
// error: no function named 'foo'
fn main() i32:
    return foo()
```

# a typo in a function name
```ura
// error: no function named 'ad'
// help: did you mean 'add'?
fn add(a i32, b i32) i32:
    return a + b

fn main() i32:
    return ad(1, 2)
```

# wrong number of arguments
```ura
// error: 'add' takes 2 arguments, not 1
// help: fn add(a i32, b i32) i32
fn add(a i32, b i32) i32:
    return a + b

fn main() i32:
    return add(1)
```

# a function declared twice
```ura
// error: function 'f' is already declared, on line 2
fn f() i32:
    return 1

fn f() i32:
    return 2

fn main() i32:
    return f()
```

# a struct declared twice
```ura
// error: struct 'User' is already declared, on line 2
struct User:
    a i32

struct User:
    b i32

fn main() i32:
    return 0
```

# unknown attribute
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

# unknown method
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

# break outside a loop
```ura
// error: break outside a loop
// help: break only works inside a while
fn main() i32:
    break
    return 0
```

# continue outside a loop
```ura
// error: continue outside a loop
// help: continue only works inside a while
fn main() i32:
    if True:
        continue
    return 0
```

# self outside a method
```ura
// error: 'self' only exists inside a method
fn get() i32:
    return self.a

fn main() i32:
    return 0
```

# calling a variable
```ura
// error: 'x' is a variable, not a function
fn main() i32:
    x i32 = 1
    return x()
```
