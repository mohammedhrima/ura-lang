## references (&)

# write through a ref
```ura
// stdout: a = 12
// expect: 12
proto printf(fmt i8[], ...) i32

fn main() i32:
    a i32 = 10
    p &i32 = &a
    p = 12
    printf("a = %d\n", a)
    return a
```

# read through a ref, then write
```ura
// stdout: p = 10, a = 15
// expect: 15
proto printf(fmt i8[], ...) i32

fn main() i32:
    a i32 = 10
    p &i32 = &a
    printf("p = %d, ", p)
    p = p + 5
    printf("a = %d\n", a)
    return a
```

# compound assignment through a ref
```ura
// expect: 24
fn main() i32:
    a i32 = 10
    p &i32 = &a
    p += 2
    p *= 2
    return a
```

# swap with ref parameters
```ura
// stdout: a = 2, b = 1
// expect: 21
proto printf(fmt i8[], ...) i32

fn swap(x &i32, y &i32):
    t i32 = x
    x = y
    y = t

fn main() i32:
    a i32 = 1
    b i32 = 2
    swap(&a, &b)
    printf("a = %d, b = %d\n", a, b)
    return a * 10 + b
```

# a ref parameter changes the caller
```ura
// expect: 3
fn inc(c &i32):
    c += 1

fn main() i32:
    n i32 = 0
    inc(&n)
    inc(&n)
    inc(&n)
    return n
```

# p1 = p2 copies the value
```ura
// stdout: a = 2, b = 2
// expect: 2
proto printf(fmt i8[], ...) i32

fn main() i32:
    a i32 = 1
    b i32 = 2
    p1 &i32 = &a
    p2 &i32 = &b
    p1 = p2
    printf("a = %d, b = %d\n", a, b)
    return a
```

# p1 = &p2 rebinds
```ura
// stdout: a = 1, b = 9
// expect: 9
proto printf(fmt i8[], ...) i32

fn main() i32:
    a i32 = 1
    b i32 = 2
    p1 &i32 = &a
    p2 &i32 = &b
    p1 = &p2
    p1 = 9
    printf("a = %d, b = %d\n", a, b)
    return b
```

# pass a ref parameter on to another function
```ura
// expect: 2
fn add_one(p &i32):
    p += 1

fn add_two(p &i32):
    add_one(&p)
    add_one(&p)

fn main() i32:
    a i32 = 0
    add_two(&a)
    return a
```

# a ref to a bool
```ura
// expect: 1
fn main() i32:
    b bool = False
    p &bool = &b
    p = True
    if b:
        return 1
    return 0
```

# a ref in a loop
```ura
// stdout: 1 2 3 4 5
// expect: 15
proto printf(fmt i8[], ...) i32

fn main() i32:
    total i32 = 0
    p &i32 = &total
    i i32 = 1
    while i <= 5:
        printf("%d ", i)
        p += i
        i += 1
    printf("\n")
    return total
```
