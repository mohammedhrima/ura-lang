## get crazy

# mutual recursion, used before it's declared
```ura
// expect: 3
fn is_even(n i32) bool:
    if n == 0: return True
    return is_odd(n - 1)

fn is_odd(n i32) bool:
    if n == 0: return False
    return is_even(n - 1)

fn main() i32:
    r i32 = 0
    if is_even(10): r += 1
    if is_odd(7): r += 2
    if is_even(3): r += 4
    return r
```

# gcd, printf with many arguments
```ura
// stdout: 12 1 4
// expect: 13
proto printf(fmt i8[], ...) i32

fn gcd(a i32, b i32) i32:
    if b == 0: return a
    return gcd(b, a % b)

fn main() i32:
    x i32 = gcd(84, 36)
    y i32 = gcd(17, 5)
    printf("%d %d %d\n", x, y, gcd(x, 4))
    return x + y
```

# collatz: a parameter changed in a loop
```ura
// expect: 111
fn steps(n i32) i32:
    c i32 = 0
    while n != 1:
        if n % 2 == 0:
            n /= 2
        else:
            n = n * 3 + 1
        c += 1
    return c

fn main() i32:
    return steps(27)
```

# the 20th prime: continue, break, a bool function
```ura
// expect: 71
fn is_prime(n i32) bool:
    if n < 2: return False
    d i32 = 2
    while d * d <= n:
        if n % d == 0: return False
        d += 1
    return True

fn main() i32:
    count i32 = 0
    n i32 = 0
    while n < 100:
        n += 1
        if is_prime(n) == False: continue
        count += 1
        if count == 20: break
    return n
```

# return from two nested loops
```ura
// expect: 67
fn main() i32:
    i i32 = 1
    while i < 20:
        j i32 = i
        while j < 20:
            if i * j == 42 and i + j == 13:
                return i * 10 + j
            j += 1
        i += 1
    return 0
```

# fibonacci with a memo array
```ura
// expect: 109   (fib(20) = 6765, 6765 % 256)
fn fib(n i32, memo i32[]) i32:
    if n < 2: return n
    if memo[n] != 0: return memo[n]
    memo[n] = fib(n - 1, memo) + fib(n - 2, memo)
    return memo[n]

fn main() i32:
    memo i32[] = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    return fib(20, memo) % 256
```

# upper-case a string in place
```ura
// stdout: HELLO, URA
// expect: 10
proto printf(fmt i8[], ...) i32

fn length(s i8[]) i32:
    n i32 = 0
    while s[n] != '\0':
        n += 1
    return n

fn upper(s i8[]):
    i i32 = 0
    while s[i] != '\0':
        if s[i] >= 'a' and s[i] <= 'z':
            s[i] -= 32 as char
        i += 1

fn main() i32:
    s i8[] = "hello, ura"
    upper(s)
    printf("%s\n", s)
    return length(s)
```

# a method called three times through a ref
```ura
// expect: 21
struct Pair:
    a i32
    b i32

    fn swap():
        t i32 = self.a
        self.a = self.b
        self.b = t

fn thrice(p &Pair):
    p.swap()
    p.swap()
    p.swap()

fn main() i32:
    x Pair
    x.a = 1
    x.b = 2
    thrice(&x)
    return x.a * 10 + x.b
```

# methods of a nested struct, called through self
```ura
// expect: 30   (area 4 * 5, plus a.x 10)
struct Point:
    x i32
    y i32

    fn move(dx i32, dy i32):
        self.x += dx
        self.y += dy

struct Rect:
    a Point
    b Point

    fn shift(d i32):
        self.a.move(d, d)
        self.b.move(d, d)

    fn area() i32:
        return (self.b.x - self.a.x) * (self.b.y - self.a.y)

fn main() i32:
    r Rect
    r.b.move(4, 5)
    r.shift(10)
    return r.area() + r.a.x
```

# bubble sort: a struct with an array, methods calling methods
```ura
// stdout: 1 3 4 5 9
// expect: 19
proto printf(fmt i8[], ...) i32

struct List:
    items i32[]
    size i32

    fn swap(i i32, j i32):
        t i32 = self.items[i]
        self.items[i] = self.items[j]
        self.items[j] = t

    fn sort():
        i i32 = 0
        while i < self.size:
            j i32 = 0
            while j < self.size - 1 - i:
                if self.items[j] > self.items[j + 1]:
                    self.swap(j, j + 1)
                j += 1
            i += 1

    fn show():
        i i32 = 0
        while i < self.size:
            if i > 0: printf(" ")
            printf("%d", self.items[i])
            i += 1
        printf("\n")

fn main() i32:
    l List
    l.items = [5, 3, 9, 1, 4]
    l.size = 5
    l.sort()
    l.show()
    return l.items[0] * 10 + l.items[4]
```

# bank account: methods, a ref parameter, an array of bills
```ura
// stdout: balance 10, ops 4, failed 1
// expect: 10
proto printf(fmt i8[], ...) i32

struct Account:
    balance i32
    ops i32

    fn deposit(n i32):
        self.balance += n
        self.ops += 1

    fn withdraw(n i32) bool:
        if n > self.balance:
            return False
        self.balance -= n
        self.ops += 1
        return True

fn pay_all(acc &Account, bills i32[], count i32) i32:
    failed i32 = 0
    i i32 = 0
    while i < count:
        if acc.withdraw(bills[i]) == False:
            failed += 1
        i += 1
    return failed

fn main() i32:
    a Account
    a.deposit(100)
    bills i32[] = [30, 50, 40, 10]
    failed i32 = pay_all(&a, bills, 4)
    printf("balance %d, ops %d, failed %d\n", a.balance, a.ops, failed)
    return a.balance
```

# an array of structs is a copy
```ura
// expect: 10   (1 + 7 + 2: q itself stays 2)
struct Point:
    x i32
    y i32

fn main() i32:
    p Point
    q Point
    p.x = 1
    q.x = 2
    ps Point[] = [p, q]
    ps[1].x = 7
    return ps[0].x + ps[1].x + q.x
```

# vectors: struct parameters, struct returns, chained calls
```ura
// expect: 27   (c = (7, 10), c . a = 7 + 20)
struct Vec:
    x i32
    y i32

    fn add(o Vec) Vec:
        r Vec
        r.x = self.x + o.x
        r.y = self.y + o.y
        return r

    fn dot(o Vec) i32:
        return self.x * o.x + self.y * o.y

fn make(x i32, y i32) Vec:
    v Vec
    v.x = x
    v.y = y
    return v

fn main() i32:
    a Vec = make(1, 2)
    b Vec = make(3, 4)
    c Vec = a.add(b).add(b)
    return c.dot(a)
```

# linked list: ref attributes, isNullable, rebinding a ref in a loop
```ura
// expect: 36   (1 + 2 + 3, plus 3 * 10)
struct Node:
    value i32
    next &Node

fn main() i32:
    c Node
    c.value = 3 // c.next is never set: it starts null
    b Node
    b.value = 2
    b.next = &c
    a Node
    a.value = 1
    a.next = &b
    sum i32 = 0
    p &Node = &a
    while not isNullable(p):
        sum += p.value
        p = &p.next
    return sum + a.next.next.value * 10
```
