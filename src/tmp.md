## arrays

# literal and indexing
```ura
// expect: 10
fn main() i32:
    a i32[] = [4, 5, 6]
    return a[0] + a[2]
```

# write through an index
```ura
// expect: 9
fn main() i32:
    a i32[] = [1, 2, 3]
    a[1] = 9
    return a[1]
```

# index from a variable
```ura
// expect: 9
fn main() i32:
    a i32[] = [7, 8, 9]
    i i32 = 2
    return a[i]
```

# index from an expression
```ura
// expect: 8
fn main() i32:
    a i32[] = [7, 8, 9]
    i i32 = 2
    return a[i - 1]
```

# literal made of variables
```ura
// expect: 16
fn main() i32:
    x i32 = 10
    y i32 = 4
    a i32[] = [x, 2, y]
    return a[0] + a[1] + a[2]
```

# compound assignment on an element
```ura
// expect: 18   (5 + 2 + 9 + 2)
fn main() i32:
    a i32[] = [1, 2, 3]
    a[0] += 4
    a[2] *= 3
    return a[0] + a[1] + a[2] + 2
```

# loop over an array
```ura
// stdout: 3 1 4 1 5
// expect: 14
proto printf(fmt i8[], ...) i32

fn main() i32:
    a i32[] = [3, 1, 4, 1, 5]
    sum i32 = 0
    i i32 = 0
    while i < 5:
        printf("%d", a[i])
        if i < 4:
            printf(" ")
        sum += a[i]
        i += 1
    printf("\n")
    return sum
```

# an array passed to a function
```ura
// expect: 6
fn sum(a i32[], n i32) i32:
    total i32 = 0
    i i32 = 0
    while i < n:
        total += a[i]
        i += 1
    return total

fn main() i32:
    values i32[] = [1, 2, 3]
    return sum(values, 3)
```

# a function writes into the caller's array
```ura
// expect: 7
fn set_first(a i32[], v i32):
    a[0] = v

fn main() i32:
    values i32[] = [1, 2, 3]
    set_first(values, 7)
    return values[0]
```

# a string is a writable i8[]
```ura
// stdout: xbc
// expect: 1
proto printf(fmt i8[], ...) i32

fn main() i32:
    s i8[] = "abc"
    s[0] = 'x'
    printf("%s\n", s)
    if s[1] == 'b':
        return 1
    return 0
```

# an array of bools
```ura
// expect: 2
fn main() i32:
    flags bool[] = [True, False, True]
    n i32 = 0
    i i32 = 0
    while i < 3:
        if flags[i]:
            n += 1
        i += 1
    return n
```

# declarations without a value start at zero
```ura
// expect: 0
fn main() i32:
    x i32
    b bool
    c char
    if b or c != '\0':
        return 1
    return x
```
