# crazy

## index

- 001 — mutual recursion, used before it's declared
- 002 — gcd, printf with many arguments
- 003 — collatz: a parameter changed in a loop
- 004 — the 20th prime: continue, break, a bool function
- 005 — return from two nested loops
- 006 — fibonacci with a memo array
- 007 — upper-case a string in place
- 008 — a method called three times through a ref
- 009 — methods of a nested struct, called through self
- 010 — bubble sort: a struct with an array, methods calling methods
- 011 — bank account: methods, a ref parameter, an array of bills
- 012 — an array of structs is a copy
- 013 — vectors: struct parameters, struct returns, chained calls
- 014 — linked list: ref attributes, isNullable, rebinding a ref in a loop

---

## 001 — mutual recursion, used before it's declared

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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i1 @is_even(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %load.n = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %load.n, 0
  br i1 %EQ, label %then, label %endif

then:                                             ; preds = %entry
  ret i1 true

endif:                                            ; preds = %entry
  %load.n1 = load i32, i32* %n, align 4
  %SUB = sub i32 %load.n1, 1
  %"is_odd(load.n - 1)" = call i1 @is_odd(i32 %SUB)
  ret i1 %"is_odd(load.n - 1)"
}

define i1 @is_odd(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %load.n = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %load.n, 0
  br i1 %EQ, label %then, label %endif

then:                                             ; preds = %entry
  ret i1 false

endif:                                            ; preds = %entry
  %load.n1 = load i32, i32* %n, align 4
  %SUB = sub i32 %load.n1, 1
  %"is_even(load.n - 1)" = call i1 @is_even(i32 %SUB)
  ret i1 %"is_even(load.n - 1)"
}

define i32 @main() {
entry:
  %r = alloca i32, align 4
  store i32 0, i32* %r, align 4
  %"is_even(10)" = call i1 @is_even(i32 10)
  br i1 %"is_even(10)", label %then, label %endif

then:                                             ; preds = %entry
  %old = load i32, i32* %r, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %r, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %"is_odd(7)" = call i1 @is_odd(i32 7)
  br i1 %"is_odd(7)", label %then2, label %endif1

then2:                                            ; preds = %endif
  %old3 = load i32, i32* %r, align 4
  %ADD_ASSIGN4 = add i32 %old3, 2
  store i32 %ADD_ASSIGN4, i32* %r, align 4
  br label %endif1

endif1:                                           ; preds = %then2, %endif
  %"is_even(3)" = call i1 @is_even(i32 3)
  br i1 %"is_even(3)", label %then6, label %endif5

then6:                                            ; preds = %endif1
  %old7 = load i32, i32* %r, align 4
  %ADD_ASSIGN8 = add i32 %old7, 4
  store i32 %ADD_ASSIGN8, i32* %r, align 4
  br label %endif5

endif5:                                           ; preds = %then6, %endif1
  %load.r = load i32, i32* %r, align 4
  ret i32 %load.r
}
```

---

## 002 — gcd, printf with many arguments

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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private constant [10 x i8] c"%d %d %d\0A\00"

declare i32 @printf(i8*, ...)

define i32 @gcd(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  store i32 %1, i32* %b, align 4
  %load.b = load i32, i32* %b, align 4
  %EQ = icmp eq i32 %load.b, 0
  br i1 %EQ, label %then, label %endif

then:                                             ; preds = %entry
  %load.a = load i32, i32* %a, align 4
  ret i32 %load.a

endif:                                            ; preds = %entry
  %load.b1 = load i32, i32* %b, align 4
  %load.a2 = load i32, i32* %a, align 4
  %load.b3 = load i32, i32* %b, align 4
  %MOD = srem i32 %load.a2, %load.b3
  %"gcd(load.b,load.a % load.b)" = call i32 @gcd(i32 %load.b1, i32 %MOD)
  ret i32 %"gcd(load.b,load.a % load.b)"
}

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  %s = alloca [10 x i8], align 1
  %"gcd(84,36)" = call i32 @gcd(i32 84, i32 36)
  store i32 %"gcd(84,36)", i32* %x, align 4
  %"gcd(17,5)" = call i32 @gcd(i32 17, i32 5)
  store i32 %"gcd(17,5)", i32* %y, align 4
  %dst = getelementptr inbounds [10 x i8], [10 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i64 10, i1 false)
  %load.x = load i32, i32* %x, align 4
  %load.y = load i32, i32* %y, align 4
  %load.x1 = load i32, i32* %x, align 4
  %"gcd(load.x,4)" = call i32 @gcd(i32 %load.x1, i32 4)
  %"printf(str,load.x,load.y,gcd(load.x,4))" = call i32 (i8*, ...) @printf(i8* %dst, i32 %load.x, i32 %load.y, i32 %"gcd(load.x,4)")
  %load.x2 = load i32, i32* %x, align 4
  %load.y3 = load i32, i32* %y, align 4
  %ADD = add i32 %load.x2, %load.y3
  ret i32 %ADD
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 003 — collatz: a parameter changed in a loop

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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @steps(i32 %0) {
entry:
  %n = alloca i32, align 4
  %c = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  store i32 0, i32* %c, align 4
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.n = load i32, i32* %n, align 4
  %NQ = icmp ne i32 %load.n, 1
  br i1 %NQ, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.n2 = load i32, i32* %n, align 4
  %MOD = srem i32 %load.n2, 2
  %EQ = icmp eq i32 %MOD, 0
  br i1 %EQ, label %then1, label %next

endwhile:                                         ; preds = %cond
  %load.c = load i32, i32* %c, align 4
  ret i32 %load.c

then1:                                            ; preds = %then
  %old = load i32, i32* %n, align 4
  %DIV_ASSIGN = sdiv i32 %old, 2
  store i32 %DIV_ASSIGN, i32* %n, align 4
  br label %endif

next:                                             ; preds = %then
  %load.n3 = load i32, i32* %n, align 4
  %MUL = mul i32 %load.n3, 3
  %ADD = add i32 %MUL, 1
  store i32 %ADD, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %next, %then1
  %old4 = load i32, i32* %c, align 4
  %ADD_ASSIGN = add i32 %old4, 1
  store i32 %ADD_ASSIGN, i32* %c, align 4
  br label %cond
}

define i32 @main() {
entry:
  %"steps(27)" = call i32 @steps(i32 27)
  ret i32 %"steps(27)"
}
```

---

## 004 — the 20th prime: continue, break, a bool function

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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i1 @is_prime(i32 %0) {
entry:
  %n = alloca i32, align 4
  %d = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %load.n = load i32, i32* %n, align 4
  %LT = icmp slt i32 %load.n, 2
  br i1 %LT, label %then, label %endif

then:                                             ; preds = %entry
  ret i1 false

endif:                                            ; preds = %entry
  store i32 2, i32* %d, align 4
  br label %cond

cond:                                             ; preds = %endif4, %endif
  %load.d = load i32, i32* %d, align 4
  %load.d2 = load i32, i32* %d, align 4
  %MUL = mul i32 %load.d, %load.d2
  %load.n3 = load i32, i32* %n, align 4
  %LE = icmp sle i32 %MUL, %load.n3
  br i1 %LE, label %then1, label %endwhile

then1:                                            ; preds = %cond
  %load.n6 = load i32, i32* %n, align 4
  %load.d7 = load i32, i32* %d, align 4
  %MOD = srem i32 %load.n6, %load.d7
  %EQ = icmp eq i32 %MOD, 0
  br i1 %EQ, label %then5, label %endif4

endwhile:                                         ; preds = %cond
  ret i1 true

then5:                                            ; preds = %then1
  ret i1 false

endif4:                                           ; preds = %then1
  %old = load i32, i32* %d, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %d, align 4
  br label %cond
}

define i32 @main() {
entry:
  %count = alloca i32, align 4
  %n = alloca i32, align 4
  store i32 0, i32* %count, align 4
  store i32 0, i32* %n, align 4
  br label %cond

cond:                                             ; preds = %endif5, %then1, %entry
  %load.n = load i32, i32* %n, align 4
  %LT = icmp slt i32 %load.n, 100
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %old = load i32, i32* %n, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %n, align 4
  %load.n2 = load i32, i32* %n, align 4
  %"is_prime(load.n)" = call i1 @is_prime(i32 %load.n2)
  %EQ = icmp eq i1 %"is_prime(load.n)", false
  br i1 %EQ, label %then1, label %endif

endwhile:                                         ; preds = %then6, %cond
  %load.n8 = load i32, i32* %n, align 4
  ret i32 %load.n8

then1:                                            ; preds = %then
  br label %cond

endif:                                            ; preds = %then
  %old3 = load i32, i32* %count, align 4
  %ADD_ASSIGN4 = add i32 %old3, 1
  store i32 %ADD_ASSIGN4, i32* %count, align 4
  %load.count = load i32, i32* %count, align 4
  %EQ7 = icmp eq i32 %load.count, 20
  br i1 %EQ7, label %then6, label %endif5

then6:                                            ; preds = %endif
  br label %endwhile

endif5:                                           ; preds = %endif
  br label %cond
}
```

---

## 005 — return from two nested loops

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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  store i32 1, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %endwhile4, %entry
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 20
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.i1 = load i32, i32* %i, align 4
  store i32 %load.i1, i32* %j, align 4
  br label %cond2

endwhile:                                         ; preds = %cond
  ret i32 0

cond2:                                            ; preds = %endif, %then
  %load.j = load i32, i32* %j, align 4
  %LT5 = icmp slt i32 %load.j, 20
  br i1 %LT5, label %then3, label %endwhile4

then3:                                            ; preds = %cond2
  %load.i7 = load i32, i32* %i, align 4
  %load.j8 = load i32, i32* %j, align 4
  %MUL = mul i32 %load.i7, %load.j8
  %EQ = icmp eq i32 %MUL, 42
  %load.i9 = load i32, i32* %i, align 4
  %load.j10 = load i32, i32* %j, align 4
  %ADD = add i32 %load.i9, %load.j10
  %EQ11 = icmp eq i32 %ADD, 13
  %AND = and i1 %EQ, %EQ11
  br i1 %AND, label %then6, label %endif

endwhile4:                                        ; preds = %cond2
  %old16 = load i32, i32* %i, align 4
  %ADD_ASSIGN17 = add i32 %old16, 1
  store i32 %ADD_ASSIGN17, i32* %i, align 4
  br label %cond

then6:                                            ; preds = %then3
  %load.i12 = load i32, i32* %i, align 4
  %MUL13 = mul i32 %load.i12, 10
  %load.j14 = load i32, i32* %j, align 4
  %ADD15 = add i32 %MUL13, %load.j14
  ret i32 %ADD15

endif:                                            ; preds = %then3
  %old = load i32, i32* %j, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %j, align 4
  br label %cond2
}
```

---

## 006 — fibonacci with a memo array

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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @fib(i32 %0, i32* %1) {
entry:
  %n = alloca i32, align 4
  %memo = alloca i32*, align 8
  store i32 %0, i32* %n, align 4
  store i32* %1, i32** %memo, align 8
  %load.n = load i32, i32* %n, align 4
  %LT = icmp slt i32 %load.n, 2
  br i1 %LT, label %then, label %endif

then:                                             ; preds = %entry
  %load.n1 = load i32, i32* %n, align 4
  ret i32 %load.n1

endif:                                            ; preds = %entry
  %load.memo = load i32*, i32** %memo, align 8
  %load.n4 = load i32, i32* %n, align 4
  %"load.memo[load.n]" = getelementptr inbounds i32, i32* %load.memo, i32 %load.n4
  %"load.load.memo[load.n]" = load i32, i32* %"load.memo[load.n]", align 4
  %NQ = icmp ne i32 %"load.load.memo[load.n]", 0
  br i1 %NQ, label %then3, label %endif2

then3:                                            ; preds = %endif
  %load.memo5 = load i32*, i32** %memo, align 8
  %load.n6 = load i32, i32* %n, align 4
  %"load.memo[load.n]7" = getelementptr inbounds i32, i32* %load.memo5, i32 %load.n6
  %"load.load.memo[load.n]8" = load i32, i32* %"load.memo[load.n]7", align 4
  ret i32 %"load.load.memo[load.n]8"

endif2:                                           ; preds = %endif
  %load.memo9 = load i32*, i32** %memo, align 8
  %load.n10 = load i32, i32* %n, align 4
  %"load.memo[load.n]11" = getelementptr inbounds i32, i32* %load.memo9, i32 %load.n10
  %load.n12 = load i32, i32* %n, align 4
  %SUB = sub i32 %load.n12, 1
  %load.memo13 = load i32*, i32** %memo, align 8
  %"fib(load.n - 1,load.memo)" = call i32 @fib(i32 %SUB, i32* %load.memo13)
  %load.n14 = load i32, i32* %n, align 4
  %SUB15 = sub i32 %load.n14, 2
  %load.memo16 = load i32*, i32** %memo, align 8
  %"fib(load.n - 2,load.memo)" = call i32 @fib(i32 %SUB15, i32* %load.memo16)
  %ADD = add i32 %"fib(load.n - 1,load.memo)", %"fib(load.n - 2,load.memo)"
  store i32 %ADD, i32* %"load.memo[load.n]11", align 4
  %load.memo17 = load i32*, i32** %memo, align 8
  %load.n18 = load i32, i32* %n, align 4
  %"load.memo[load.n]19" = getelementptr inbounds i32, i32* %load.memo17, i32 %load.n18
  %"load.load.memo[load.n]20" = load i32, i32* %"load.memo[load.n]19", align 4
  ret i32 %"load.load.memo[load.n]20"
}

define i32 @main() {
entry:
  %memo = alloca i32*, align 8
  %array = alloca [21 x i32], align 4
  %e = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 0
  store i32 0, i32* %e, align 4
  %e1 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 1
  store i32 0, i32* %e1, align 4
  %e2 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 2
  store i32 0, i32* %e2, align 4
  %e3 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 3
  store i32 0, i32* %e3, align 4
  %e4 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 4
  store i32 0, i32* %e4, align 4
  %e5 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 5
  store i32 0, i32* %e5, align 4
  %e6 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 6
  store i32 0, i32* %e6, align 4
  %e7 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 7
  store i32 0, i32* %e7, align 4
  %e8 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 8
  store i32 0, i32* %e8, align 4
  %e9 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 9
  store i32 0, i32* %e9, align 4
  %e10 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 10
  store i32 0, i32* %e10, align 4
  %e11 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 11
  store i32 0, i32* %e11, align 4
  %e12 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 12
  store i32 0, i32* %e12, align 4
  %e13 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 13
  store i32 0, i32* %e13, align 4
  %e14 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 14
  store i32 0, i32* %e14, align 4
  %e15 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 15
  store i32 0, i32* %e15, align 4
  %e16 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 16
  store i32 0, i32* %e16, align 4
  %e17 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 17
  store i32 0, i32* %e17, align 4
  %e18 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 18
  store i32 0, i32* %e18, align 4
  %e19 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 19
  store i32 0, i32* %e19, align 4
  %e20 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 20
  store i32 0, i32* %e20, align 4
  %array21 = getelementptr inbounds [21 x i32], [21 x i32]* %array, i32 0, i32 0
  store i32* %array21, i32** %memo, align 8
  %load.memo = load i32*, i32** %memo, align 8
  %"fib(20,load.memo)" = call i32 @fib(i32 20, i32* %load.memo)
  %MOD = srem i32 %"fib(20,load.memo)", 256
  ret i32 %MOD
}
```

---

## 007 — upper-case a string in place

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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private constant [11 x i8] c"hello, ura\00"
@str.1 = private constant [4 x i8] c"%s\0A\00"

declare i32 @printf(i8*, ...)

define i32 @length(i8* %0) {
entry:
  %s = alloca i8*, align 8
  %n = alloca i32, align 4
  store i8* %0, i8** %s, align 8
  store i32 0, i32* %n, align 4
  br label %cond

cond:                                             ; preds = %then, %entry
  %load.s = load i8*, i8** %s, align 8
  %load.n = load i32, i32* %n, align 4
  %"load.s[load.n]" = getelementptr inbounds i8, i8* %load.s, i32 %load.n
  %"load.load.s[load.n]" = load i8, i8* %"load.s[load.n]", align 1
  %NQ = icmp ne i8 %"load.load.s[load.n]", 0
  br i1 %NQ, label %then, label %endwhile

then:                                             ; preds = %cond
  %old = load i32, i32* %n, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %n, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  %load.n1 = load i32, i32* %n, align 4
  ret i32 %load.n1
}

define void @upper(i8* %0) {
entry:
  %s = alloca i8*, align 8
  %i = alloca i32, align 4
  store i8* %0, i8** %s, align 8
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.s = load i8*, i8** %s, align 8
  %load.i = load i32, i32* %i, align 4
  %"load.s[load.i]" = getelementptr inbounds i8, i8* %load.s, i32 %load.i
  %"load.load.s[load.i]" = load i8, i8* %"load.s[load.i]", align 1
  %NQ = icmp ne i8 %"load.load.s[load.i]", 0
  br i1 %NQ, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.s2 = load i8*, i8** %s, align 8
  %load.i3 = load i32, i32* %i, align 4
  %"load.s[load.i]4" = getelementptr inbounds i8, i8* %load.s2, i32 %load.i3
  %"load.load.s[load.i]5" = load i8, i8* %"load.s[load.i]4", align 1
  %GE = icmp sge i8 %"load.load.s[load.i]5", 97
  %load.s6 = load i8*, i8** %s, align 8
  %load.i7 = load i32, i32* %i, align 4
  %"load.s[load.i]8" = getelementptr inbounds i8, i8* %load.s6, i32 %load.i7
  %"load.load.s[load.i]9" = load i8, i8* %"load.s[load.i]8", align 1
  %LE = icmp sle i8 %"load.load.s[load.i]9", 122
  %AND = and i1 %GE, %LE
  br i1 %AND, label %then1, label %endif

endwhile:                                         ; preds = %cond
  ret void

then1:                                            ; preds = %then
  %load.s10 = load i8*, i8** %s, align 8
  %load.i11 = load i32, i32* %i, align 4
  %"load.s[load.i]12" = getelementptr inbounds i8, i8* %load.s10, i32 %load.i11
  %old = load i8, i8* %"load.s[load.i]12", align 1
  %SUB_ASSIGN = sub i8 %old, 32
  store i8 %SUB_ASSIGN, i8* %"load.s[load.i]12", align 1
  br label %endif

endif:                                            ; preds = %then1, %then
  %old13 = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old13, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  br label %cond
}

define i32 @main() {
entry:
  %s = alloca i8*, align 8
  %s1 = alloca [11 x i8], align 1
  %s2 = alloca [4 x i8], align 1
  %dst = getelementptr inbounds [11 x i8], [11 x i8]* %s1, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([11 x i8], [11 x i8]* @str, i32 0, i32 0), i64 11, i1 false)
  store i8* %dst, i8** %s, align 8
  %load.s = load i8*, i8** %s, align 8
  call void @upper(i8* %load.s)
  %dst3 = getelementptr inbounds [4 x i8], [4 x i8]* %s2, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst3, i8* align 1 getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i64 4, i1 false)
  %load.s4 = load i8*, i8** %s, align 8
  %"printf(str,load.s)" = call i32 (i8*, ...) @printf(i8* %dst3, i8* %load.s4)
  %load.s5 = load i8*, i8** %s, align 8
  %"length(load.s)" = call i32 @length(i8* %load.s5)
  ret i32 %"length(load.s)"
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 008 — a method called three times through a ref

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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Pair = type { i32, i32 }

define void @Pair.swap(%Pair* %0) {
entry:
  %self = alloca %Pair*, align 8
  %t = alloca i32, align 4
  store %Pair* %0, %Pair** %self, align 8
  %dref.self = load %Pair*, %Pair** %self, align 8
  %load.dref.self.a = getelementptr inbounds %Pair, %Pair* %dref.self, i32 0, i32 0
  %load.load.dref.self.a = load i32, i32* %load.dref.self.a, align 4
  store i32 %load.load.dref.self.a, i32* %t, align 4
  %dref.self1 = load %Pair*, %Pair** %self, align 8
  %load.dref.self.a2 = getelementptr inbounds %Pair, %Pair* %dref.self1, i32 0, i32 0
  %dref.self3 = load %Pair*, %Pair** %self, align 8
  %load.dref.self.b = getelementptr inbounds %Pair, %Pair* %dref.self3, i32 0, i32 1
  %load.load.dref.self.b = load i32, i32* %load.dref.self.b, align 4
  store i32 %load.load.dref.self.b, i32* %load.dref.self.a2, align 4
  %dref.self4 = load %Pair*, %Pair** %self, align 8
  %load.dref.self.b5 = getelementptr inbounds %Pair, %Pair* %dref.self4, i32 0, i32 1
  %load.t = load i32, i32* %t, align 4
  store i32 %load.t, i32* %load.dref.self.b5, align 4
  ret void
}

define void @thrice(%Pair* %0) {
entry:
  %p = alloca %Pair*, align 8
  store %Pair* %0, %Pair** %p, align 8
  %dref.p = load %Pair*, %Pair** %p, align 8
  call void @Pair.swap(%Pair* %dref.p)
  %dref.p1 = load %Pair*, %Pair** %p, align 8
  call void @Pair.swap(%Pair* %dref.p1)
  %dref.p2 = load %Pair*, %Pair** %p, align 8
  call void @Pair.swap(%Pair* %dref.p2)
  ret void
}

define i32 @main() {
entry:
  %x = alloca %Pair, align 8
  store %Pair zeroinitializer, %Pair* %x, align 4
  %load.x.a = getelementptr inbounds %Pair, %Pair* %x, i32 0, i32 0
  store i32 1, i32* %load.x.a, align 4
  %load.x.b = getelementptr inbounds %Pair, %Pair* %x, i32 0, i32 1
  store i32 2, i32* %load.x.b, align 4
  call void @thrice(%Pair* %x)
  %load.x.a1 = getelementptr inbounds %Pair, %Pair* %x, i32 0, i32 0
  %load.load.x.a = load i32, i32* %load.x.a1, align 4
  %MUL = mul i32 %load.load.x.a, 10
  %load.x.b2 = getelementptr inbounds %Pair, %Pair* %x, i32 0, i32 1
  %load.load.x.b = load i32, i32* %load.x.b2, align 4
  %ADD = add i32 %MUL, %load.load.x.b
  ret i32 %ADD
}
```

---

## 009 — methods of a nested struct, called through self

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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Point = type { i32, i32 }
%Rect = type { %Point, %Point }

define void @Point.move(%Point* %0, i32 %1, i32 %2) {
entry:
  %self = alloca %Point*, align 8
  %dx = alloca i32, align 4
  %dy = alloca i32, align 4
  store %Point* %0, %Point** %self, align 8
  store i32 %1, i32* %dx, align 4
  store i32 %2, i32* %dy, align 4
  %dref.self = load %Point*, %Point** %self, align 8
  %load.dref.self.x = getelementptr inbounds %Point, %Point* %dref.self, i32 0, i32 0
  %load.dx = load i32, i32* %dx, align 4
  %old = load i32, i32* %load.dref.self.x, align 4
  %ADD_ASSIGN = add i32 %old, %load.dx
  store i32 %ADD_ASSIGN, i32* %load.dref.self.x, align 4
  %dref.self1 = load %Point*, %Point** %self, align 8
  %load.dref.self.y = getelementptr inbounds %Point, %Point* %dref.self1, i32 0, i32 1
  %load.dy = load i32, i32* %dy, align 4
  %old2 = load i32, i32* %load.dref.self.y, align 4
  %ADD_ASSIGN3 = add i32 %old2, %load.dy
  store i32 %ADD_ASSIGN3, i32* %load.dref.self.y, align 4
  ret void
}

define void @Rect.shift(%Rect* %0, i32 %1) {
entry:
  %self = alloca %Rect*, align 8
  %d = alloca i32, align 4
  store %Rect* %0, %Rect** %self, align 8
  store i32 %1, i32* %d, align 4
  %dref.self = load %Rect*, %Rect** %self, align 8
  %load.dref.self.a = getelementptr inbounds %Rect, %Rect* %dref.self, i32 0, i32 0
  %load.d = load i32, i32* %d, align 4
  %load.d1 = load i32, i32* %d, align 4
  call void @Point.move(%Point* %load.dref.self.a, i32 %load.d, i32 %load.d1)
  %dref.self2 = load %Rect*, %Rect** %self, align 8
  %load.dref.self.b = getelementptr inbounds %Rect, %Rect* %dref.self2, i32 0, i32 1
  %load.d3 = load i32, i32* %d, align 4
  %load.d4 = load i32, i32* %d, align 4
  call void @Point.move(%Point* %load.dref.self.b, i32 %load.d3, i32 %load.d4)
  ret void
}

define i32 @Rect.area(%Rect* %0) {
entry:
  %self = alloca %Rect*, align 8
  store %Rect* %0, %Rect** %self, align 8
  %dref.self = load %Rect*, %Rect** %self, align 8
  %load.dref.self.b = getelementptr inbounds %Rect, %Rect* %dref.self, i32 0, i32 1
  %load.load.dref.self.b.x = getelementptr inbounds %Point, %Point* %load.dref.self.b, i32 0, i32 0
  %load.load.load.dref.self.b.x = load i32, i32* %load.load.dref.self.b.x, align 4
  %dref.self1 = load %Rect*, %Rect** %self, align 8
  %load.dref.self.a = getelementptr inbounds %Rect, %Rect* %dref.self1, i32 0, i32 0
  %load.load.dref.self.a.x = getelementptr inbounds %Point, %Point* %load.dref.self.a, i32 0, i32 0
  %load.load.load.dref.self.a.x = load i32, i32* %load.load.dref.self.a.x, align 4
  %SUB = sub i32 %load.load.load.dref.self.b.x, %load.load.load.dref.self.a.x
  %dref.self2 = load %Rect*, %Rect** %self, align 8
  %load.dref.self.b3 = getelementptr inbounds %Rect, %Rect* %dref.self2, i32 0, i32 1
  %load.load.dref.self.b.y = getelementptr inbounds %Point, %Point* %load.dref.self.b3, i32 0, i32 1
  %load.load.load.dref.self.b.y = load i32, i32* %load.load.dref.self.b.y, align 4
  %dref.self4 = load %Rect*, %Rect** %self, align 8
  %load.dref.self.a5 = getelementptr inbounds %Rect, %Rect* %dref.self4, i32 0, i32 0
  %load.load.dref.self.a.y = getelementptr inbounds %Point, %Point* %load.dref.self.a5, i32 0, i32 1
  %load.load.load.dref.self.a.y = load i32, i32* %load.load.dref.self.a.y, align 4
  %SUB6 = sub i32 %load.load.load.dref.self.b.y, %load.load.load.dref.self.a.y
  %MUL = mul i32 %SUB, %SUB6
  ret i32 %MUL
}

define i32 @main() {
entry:
  %r = alloca %Rect, align 8
  store %Rect zeroinitializer, %Rect* %r, align 4
  %load.r.b = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  call void @Point.move(%Point* %load.r.b, i32 4, i32 5)
  call void @Rect.shift(%Rect* %r, i32 10)
  %"area(&load.r)" = call i32 @Rect.area(%Rect* %r)
  %load.r.a = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 0
  %load.load.r.a.x = getelementptr inbounds %Point, %Point* %load.r.a, i32 0, i32 0
  %load.load.load.r.a.x = load i32, i32* %load.load.r.a.x, align 4
  %ADD = add i32 %"area(&load.r)", %load.load.load.r.a.x
  ret i32 %ADD
}
```

---

## 010 — bubble sort: a struct with an array, methods calling methods

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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%List = type { i32*, i32 }

@str = private constant [2 x i8] c" \00"
@str.1 = private constant [3 x i8] c"%d\00"
@str.2 = private constant [2 x i8] c"\0A\00"

declare i32 @printf(i8*, ...)

define void @List.swap(%List* %0, i32 %1, i32 %2) {
entry:
  %self = alloca %List*, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %t = alloca i32, align 4
  store %List* %0, %List** %self, align 8
  store i32 %1, i32* %i, align 4
  store i32 %2, i32* %j, align 4
  %dref.self = load %List*, %List** %self, align 8
  %load.dref.self.items = getelementptr inbounds %List, %List* %dref.self, i32 0, i32 0
  %load.load.dref.self.items = load i32*, i32** %load.dref.self.items, align 8
  %load.i = load i32, i32* %i, align 4
  %"load.load.dref.self.items[load.i]" = getelementptr inbounds i32, i32* %load.load.dref.self.items, i32 %load.i
  %"load.load.load.dref.self.items[load.i]" = load i32, i32* %"load.load.dref.self.items[load.i]", align 4
  store i32 %"load.load.load.dref.self.items[load.i]", i32* %t, align 4
  %dref.self1 = load %List*, %List** %self, align 8
  %load.dref.self.items2 = getelementptr inbounds %List, %List* %dref.self1, i32 0, i32 0
  %load.load.dref.self.items3 = load i32*, i32** %load.dref.self.items2, align 8
  %load.i4 = load i32, i32* %i, align 4
  %"load.load.dref.self.items[load.i]5" = getelementptr inbounds i32, i32* %load.load.dref.self.items3, i32 %load.i4
  %dref.self6 = load %List*, %List** %self, align 8
  %load.dref.self.items7 = getelementptr inbounds %List, %List* %dref.self6, i32 0, i32 0
  %load.load.dref.self.items8 = load i32*, i32** %load.dref.self.items7, align 8
  %load.j = load i32, i32* %j, align 4
  %"load.load.dref.self.items[load.j]" = getelementptr inbounds i32, i32* %load.load.dref.self.items8, i32 %load.j
  %"load.load.load.dref.self.items[load.j]" = load i32, i32* %"load.load.dref.self.items[load.j]", align 4
  store i32 %"load.load.load.dref.self.items[load.j]", i32* %"load.load.dref.self.items[load.i]5", align 4
  %dref.self9 = load %List*, %List** %self, align 8
  %load.dref.self.items10 = getelementptr inbounds %List, %List* %dref.self9, i32 0, i32 0
  %load.load.dref.self.items11 = load i32*, i32** %load.dref.self.items10, align 8
  %load.j12 = load i32, i32* %j, align 4
  %"load.load.dref.self.items[load.j]13" = getelementptr inbounds i32, i32* %load.load.dref.self.items11, i32 %load.j12
  %load.t = load i32, i32* %t, align 4
  store i32 %load.t, i32* %"load.load.dref.self.items[load.j]13", align 4
  ret void
}

define void @List.sort(%List* %0) {
entry:
  %self = alloca %List*, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  store %List* %0, %List** %self, align 8
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %endwhile3, %entry
  %load.i = load i32, i32* %i, align 4
  %dref.self = load %List*, %List** %self, align 8
  %load.dref.self.size = getelementptr inbounds %List, %List* %dref.self, i32 0, i32 1
  %load.load.dref.self.size = load i32, i32* %load.dref.self.size, align 4
  %LT = icmp slt i32 %load.i, %load.load.dref.self.size
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  store i32 0, i32* %j, align 4
  br label %cond1

endwhile:                                         ; preds = %cond
  ret void

cond1:                                            ; preds = %endif, %then
  %load.j = load i32, i32* %j, align 4
  %dref.self4 = load %List*, %List** %self, align 8
  %load.dref.self.size5 = getelementptr inbounds %List, %List* %dref.self4, i32 0, i32 1
  %load.load.dref.self.size6 = load i32, i32* %load.dref.self.size5, align 4
  %SUB = sub i32 %load.load.dref.self.size6, 1
  %load.i7 = load i32, i32* %i, align 4
  %SUB8 = sub i32 %SUB, %load.i7
  %LT9 = icmp slt i32 %load.j, %SUB8
  br i1 %LT9, label %then2, label %endwhile3

then2:                                            ; preds = %cond1
  %dref.self11 = load %List*, %List** %self, align 8
  %load.dref.self.items = getelementptr inbounds %List, %List* %dref.self11, i32 0, i32 0
  %load.load.dref.self.items = load i32*, i32** %load.dref.self.items, align 8
  %load.j12 = load i32, i32* %j, align 4
  %"load.load.dref.self.items[load.j]" = getelementptr inbounds i32, i32* %load.load.dref.self.items, i32 %load.j12
  %"load.load.load.dref.self.items[load.j]" = load i32, i32* %"load.load.dref.self.items[load.j]", align 4
  %dref.self13 = load %List*, %List** %self, align 8
  %load.dref.self.items14 = getelementptr inbounds %List, %List* %dref.self13, i32 0, i32 0
  %load.load.dref.self.items15 = load i32*, i32** %load.dref.self.items14, align 8
  %load.j16 = load i32, i32* %j, align 4
  %ADD = add i32 %load.j16, 1
  %"load.load.dref.self.items[load.j + 1]" = getelementptr inbounds i32, i32* %load.load.dref.self.items15, i32 %ADD
  %"load.load.load.dref.self.items[load.j + 1]" = load i32, i32* %"load.load.dref.self.items[load.j + 1]", align 4
  %GT = icmp sgt i32 %"load.load.load.dref.self.items[load.j]", %"load.load.load.dref.self.items[load.j + 1]"
  br i1 %GT, label %then10, label %endif

endwhile3:                                        ; preds = %cond1
  %old21 = load i32, i32* %i, align 4
  %ADD_ASSIGN22 = add i32 %old21, 1
  store i32 %ADD_ASSIGN22, i32* %i, align 4
  br label %cond

then10:                                           ; preds = %then2
  %dref.self17 = load %List*, %List** %self, align 8
  %load.j18 = load i32, i32* %j, align 4
  %load.j19 = load i32, i32* %j, align 4
  %ADD20 = add i32 %load.j19, 1
  call void @List.swap(%List* %dref.self17, i32 %load.j18, i32 %ADD20)
  br label %endif

endif:                                            ; preds = %then10, %then2
  %old = load i32, i32* %j, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %j, align 4
  br label %cond1
}

define void @List.show(%List* %0) {
entry:
  %self = alloca %List*, align 8
  %i = alloca i32, align 4
  %s = alloca [2 x i8], align 1
  %s3 = alloca [3 x i8], align 1
  %s7 = alloca [2 x i8], align 1
  store %List* %0, %List** %self, align 8
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.i = load i32, i32* %i, align 4
  %dref.self = load %List*, %List** %self, align 8
  %load.dref.self.size = getelementptr inbounds %List, %List* %dref.self, i32 0, i32 1
  %load.load.dref.self.size = load i32, i32* %load.dref.self.size, align 4
  %LT = icmp slt i32 %load.i, %load.load.dref.self.size
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.i2 = load i32, i32* %i, align 4
  %GT = icmp sgt i32 %load.i2, 0
  br i1 %GT, label %then1, label %endif

endwhile:                                         ; preds = %cond
  %dst8 = getelementptr inbounds [2 x i8], [2 x i8]* %s7, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst8, i8* align 1 getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0), i64 2, i1 false)
  %"printf(str)9" = call i32 (i8*, ...) @printf(i8* %dst8)
  ret void

then1:                                            ; preds = %then
  %dst = getelementptr inbounds [2 x i8], [2 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i64 2, i1 false)
  %"printf(str)" = call i32 (i8*, ...) @printf(i8* %dst)
  br label %endif

endif:                                            ; preds = %then1, %then
  %dst4 = getelementptr inbounds [3 x i8], [3 x i8]* %s3, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst4, i8* align 1 getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0), i64 3, i1 false)
  %dref.self5 = load %List*, %List** %self, align 8
  %load.dref.self.items = getelementptr inbounds %List, %List* %dref.self5, i32 0, i32 0
  %load.load.dref.self.items = load i32*, i32** %load.dref.self.items, align 8
  %load.i6 = load i32, i32* %i, align 4
  %"load.load.dref.self.items[load.i]" = getelementptr inbounds i32, i32* %load.load.dref.self.items, i32 %load.i6
  %"load.load.load.dref.self.items[load.i]" = load i32, i32* %"load.load.dref.self.items[load.i]", align 4
  %"printf(str,load.load.load.dref.self.items[load.i])" = call i32 (i8*, ...) @printf(i8* %dst4, i32 %"load.load.load.dref.self.items[load.i]")
  %old = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  br label %cond
}

define i32 @main() {
entry:
  %l = alloca %List, align 8
  %array = alloca [5 x i32], align 4
  store %List zeroinitializer, %List* %l, align 8
  %load.l.items = getelementptr inbounds %List, %List* %l, i32 0, i32 0
  %e = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 0
  store i32 5, i32* %e, align 4
  %e1 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 1
  store i32 3, i32* %e1, align 4
  %e2 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 2
  store i32 9, i32* %e2, align 4
  %e3 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 3
  store i32 1, i32* %e3, align 4
  %e4 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 4
  store i32 4, i32* %e4, align 4
  %array5 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 0
  store i32* %array5, i32** %load.l.items, align 8
  %load.l.size = getelementptr inbounds %List, %List* %l, i32 0, i32 1
  store i32 5, i32* %load.l.size, align 4
  call void @List.sort(%List* %l)
  call void @List.show(%List* %l)
  %load.l.items6 = getelementptr inbounds %List, %List* %l, i32 0, i32 0
  %load.load.l.items = load i32*, i32** %load.l.items6, align 8
  %"load.load.l.items[0]" = getelementptr inbounds i32, i32* %load.load.l.items, i32 0
  %"load.load.load.l.items[0]" = load i32, i32* %"load.load.l.items[0]", align 4
  %MUL = mul i32 %"load.load.load.l.items[0]", 10
  %load.l.items7 = getelementptr inbounds %List, %List* %l, i32 0, i32 0
  %load.load.l.items8 = load i32*, i32** %load.l.items7, align 8
  %"load.load.l.items[4]" = getelementptr inbounds i32, i32* %load.load.l.items8, i32 4
  %"load.load.load.l.items[4]" = load i32, i32* %"load.load.l.items[4]", align 4
  %ADD = add i32 %MUL, %"load.load.load.l.items[4]"
  ret i32 %ADD
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 011 — bank account: methods, a ref parameter, an array of bills

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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Account = type { i32, i32 }

@str = private constant [31 x i8] c"balance %d, ops %d, failed %d\0A\00"

declare i32 @printf(i8*, ...)

define void @Account.deposit(%Account* %0, i32 %1) {
entry:
  %self = alloca %Account*, align 8
  %n = alloca i32, align 4
  store %Account* %0, %Account** %self, align 8
  store i32 %1, i32* %n, align 4
  %dref.self = load %Account*, %Account** %self, align 8
  %load.dref.self.balance = getelementptr inbounds %Account, %Account* %dref.self, i32 0, i32 0
  %load.n = load i32, i32* %n, align 4
  %old = load i32, i32* %load.dref.self.balance, align 4
  %ADD_ASSIGN = add i32 %old, %load.n
  store i32 %ADD_ASSIGN, i32* %load.dref.self.balance, align 4
  %dref.self1 = load %Account*, %Account** %self, align 8
  %load.dref.self.ops = getelementptr inbounds %Account, %Account* %dref.self1, i32 0, i32 1
  %old2 = load i32, i32* %load.dref.self.ops, align 4
  %ADD_ASSIGN3 = add i32 %old2, 1
  store i32 %ADD_ASSIGN3, i32* %load.dref.self.ops, align 4
  ret void
}

define i1 @Account.withdraw(%Account* %0, i32 %1) {
entry:
  %self = alloca %Account*, align 8
  %n = alloca i32, align 4
  store %Account* %0, %Account** %self, align 8
  store i32 %1, i32* %n, align 4
  %load.n = load i32, i32* %n, align 4
  %dref.self = load %Account*, %Account** %self, align 8
  %load.dref.self.balance = getelementptr inbounds %Account, %Account* %dref.self, i32 0, i32 0
  %load.load.dref.self.balance = load i32, i32* %load.dref.self.balance, align 4
  %GT = icmp sgt i32 %load.n, %load.load.dref.self.balance
  br i1 %GT, label %then, label %endif

then:                                             ; preds = %entry
  ret i1 false

endif:                                            ; preds = %entry
  %dref.self1 = load %Account*, %Account** %self, align 8
  %load.dref.self.balance2 = getelementptr inbounds %Account, %Account* %dref.self1, i32 0, i32 0
  %load.n3 = load i32, i32* %n, align 4
  %old = load i32, i32* %load.dref.self.balance2, align 4
  %SUB_ASSIGN = sub i32 %old, %load.n3
  store i32 %SUB_ASSIGN, i32* %load.dref.self.balance2, align 4
  %dref.self4 = load %Account*, %Account** %self, align 8
  %load.dref.self.ops = getelementptr inbounds %Account, %Account* %dref.self4, i32 0, i32 1
  %old5 = load i32, i32* %load.dref.self.ops, align 4
  %ADD_ASSIGN = add i32 %old5, 1
  store i32 %ADD_ASSIGN, i32* %load.dref.self.ops, align 4
  ret i1 true
}

define i32 @pay_all(%Account* %0, i32* %1, i32 %2) {
entry:
  %acc = alloca %Account*, align 8
  %bills = alloca i32*, align 8
  %count = alloca i32, align 4
  %failed = alloca i32, align 4
  %i = alloca i32, align 4
  store %Account* %0, %Account** %acc, align 8
  store i32* %1, i32** %bills, align 8
  store i32 %2, i32* %count, align 4
  store i32 0, i32* %failed, align 4
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.i = load i32, i32* %i, align 4
  %load.count = load i32, i32* %count, align 4
  %LT = icmp slt i32 %load.i, %load.count
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %dref.acc = load %Account*, %Account** %acc, align 8
  %load.bills = load i32*, i32** %bills, align 8
  %load.i2 = load i32, i32* %i, align 4
  %"load.bills[load.i]" = getelementptr inbounds i32, i32* %load.bills, i32 %load.i2
  %"load.load.bills[load.i]" = load i32, i32* %"load.bills[load.i]", align 4
  %"withdraw(&load.dref.acc,load.load.bills[load.i])" = call i1 @Account.withdraw(%Account* %dref.acc, i32 %"load.load.bills[load.i]")
  %EQ = icmp eq i1 %"withdraw(&load.dref.acc,load.load.bills[load.i])", false
  br i1 %EQ, label %then1, label %endif

endwhile:                                         ; preds = %cond
  %load.failed = load i32, i32* %failed, align 4
  ret i32 %load.failed

then1:                                            ; preds = %then
  %old = load i32, i32* %failed, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %failed, align 4
  br label %endif

endif:                                            ; preds = %then1, %then
  %old3 = load i32, i32* %i, align 4
  %ADD_ASSIGN4 = add i32 %old3, 1
  store i32 %ADD_ASSIGN4, i32* %i, align 4
  br label %cond
}

define i32 @main() {
entry:
  %a = alloca %Account, align 8
  %bills = alloca i32*, align 8
  %array = alloca [4 x i32], align 4
  %failed = alloca i32, align 4
  %s = alloca [31 x i8], align 1
  store %Account zeroinitializer, %Account* %a, align 4
  call void @Account.deposit(%Account* %a, i32 100)
  %e = getelementptr inbounds [4 x i32], [4 x i32]* %array, i32 0, i32 0
  store i32 30, i32* %e, align 4
  %e1 = getelementptr inbounds [4 x i32], [4 x i32]* %array, i32 0, i32 1
  store i32 50, i32* %e1, align 4
  %e2 = getelementptr inbounds [4 x i32], [4 x i32]* %array, i32 0, i32 2
  store i32 40, i32* %e2, align 4
  %e3 = getelementptr inbounds [4 x i32], [4 x i32]* %array, i32 0, i32 3
  store i32 10, i32* %e3, align 4
  %array4 = getelementptr inbounds [4 x i32], [4 x i32]* %array, i32 0, i32 0
  store i32* %array4, i32** %bills, align 8
  %load.bills = load i32*, i32** %bills, align 8
  %"pay_all(&a,load.bills,4)" = call i32 @pay_all(%Account* %a, i32* %load.bills, i32 4)
  store i32 %"pay_all(&a,load.bills,4)", i32* %failed, align 4
  %dst = getelementptr inbounds [31 x i8], [31 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([31 x i8], [31 x i8]* @str, i32 0, i32 0), i64 31, i1 false)
  %load.a.balance = getelementptr inbounds %Account, %Account* %a, i32 0, i32 0
  %load.load.a.balance = load i32, i32* %load.a.balance, align 4
  %load.a.ops = getelementptr inbounds %Account, %Account* %a, i32 0, i32 1
  %load.load.a.ops = load i32, i32* %load.a.ops, align 4
  %load.failed = load i32, i32* %failed, align 4
  %"printf(str,load.load.a.balance,load.load.a.ops,load.failed)" = call i32 (i8*, ...) @printf(i8* %dst, i32 %load.load.a.balance, i32 %load.load.a.ops, i32 %load.failed)
  %load.a.balance5 = getelementptr inbounds %Account, %Account* %a, i32 0, i32 0
  %load.load.a.balance6 = load i32, i32* %load.a.balance5, align 4
  ret i32 %load.load.a.balance6
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 012 — an array of structs is a copy

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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Point = type { i32, i32 }

define i32 @main() {
entry:
  %p = alloca %Point, align 8
  %q = alloca %Point, align 8
  %ps = alloca %Point*, align 8
  %array = alloca [2 x %Point], align 8
  store %Point zeroinitializer, %Point* %p, align 4
  store %Point zeroinitializer, %Point* %q, align 4
  %load.p.x = getelementptr inbounds %Point, %Point* %p, i32 0, i32 0
  store i32 1, i32* %load.p.x, align 4
  %load.q.x = getelementptr inbounds %Point, %Point* %q, i32 0, i32 0
  store i32 2, i32* %load.q.x, align 4
  %load.p = load %Point, %Point* %p, align 4
  %load.q = load %Point, %Point* %q, align 4
  %e = getelementptr inbounds [2 x %Point], [2 x %Point]* %array, i32 0, i32 0
  store %Point %load.p, %Point* %e, align 4
  %e1 = getelementptr inbounds [2 x %Point], [2 x %Point]* %array, i32 0, i32 1
  store %Point %load.q, %Point* %e1, align 4
  %array2 = getelementptr inbounds [2 x %Point], [2 x %Point]* %array, i32 0, i32 0
  store %Point* %array2, %Point** %ps, align 8
  %load.ps = load %Point*, %Point** %ps, align 8
  %"load.ps[1]" = getelementptr inbounds %Point, %Point* %load.ps, i32 1
  %"load.load.ps[1].x" = getelementptr inbounds %Point, %Point* %"load.ps[1]", i32 0, i32 0
  store i32 7, i32* %"load.load.ps[1].x", align 4
  %load.ps3 = load %Point*, %Point** %ps, align 8
  %"load.ps[0]" = getelementptr inbounds %Point, %Point* %load.ps3, i32 0
  %"load.load.ps[0].x" = getelementptr inbounds %Point, %Point* %"load.ps[0]", i32 0, i32 0
  %"load.load.load.ps[0].x" = load i32, i32* %"load.load.ps[0].x", align 4
  %load.ps4 = load %Point*, %Point** %ps, align 8
  %"load.ps[1]5" = getelementptr inbounds %Point, %Point* %load.ps4, i32 1
  %"load.load.ps[1].x6" = getelementptr inbounds %Point, %Point* %"load.ps[1]5", i32 0, i32 0
  %"load.load.load.ps[1].x" = load i32, i32* %"load.load.ps[1].x6", align 4
  %ADD = add i32 %"load.load.load.ps[0].x", %"load.load.load.ps[1].x"
  %load.q.x7 = getelementptr inbounds %Point, %Point* %q, i32 0, i32 0
  %load.load.q.x = load i32, i32* %load.q.x7, align 4
  %ADD8 = add i32 %ADD, %load.load.q.x
  ret i32 %ADD8
}
```

---

## 013 — vectors: struct parameters, struct returns, chained calls

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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Vec = type { i32, i32 }

define %Vec @Vec.add(%Vec* %0, %Vec %1) {
entry:
  %self = alloca %Vec*, align 8
  %o = alloca %Vec, align 8
  %r = alloca %Vec, align 8
  store %Vec* %0, %Vec** %self, align 8
  store %Vec %1, %Vec* %o, align 4
  store %Vec zeroinitializer, %Vec* %r, align 4
  %load.r.x = getelementptr inbounds %Vec, %Vec* %r, i32 0, i32 0
  %dref.self = load %Vec*, %Vec** %self, align 8
  %load.dref.self.x = getelementptr inbounds %Vec, %Vec* %dref.self, i32 0, i32 0
  %load.load.dref.self.x = load i32, i32* %load.dref.self.x, align 4
  %load.o.x = getelementptr inbounds %Vec, %Vec* %o, i32 0, i32 0
  %load.load.o.x = load i32, i32* %load.o.x, align 4
  %ADD = add i32 %load.load.dref.self.x, %load.load.o.x
  store i32 %ADD, i32* %load.r.x, align 4
  %load.r.y = getelementptr inbounds %Vec, %Vec* %r, i32 0, i32 1
  %dref.self1 = load %Vec*, %Vec** %self, align 8
  %load.dref.self.y = getelementptr inbounds %Vec, %Vec* %dref.self1, i32 0, i32 1
  %load.load.dref.self.y = load i32, i32* %load.dref.self.y, align 4
  %load.o.y = getelementptr inbounds %Vec, %Vec* %o, i32 0, i32 1
  %load.load.o.y = load i32, i32* %load.o.y, align 4
  %ADD2 = add i32 %load.load.dref.self.y, %load.load.o.y
  store i32 %ADD2, i32* %load.r.y, align 4
  %load.r = load %Vec, %Vec* %r, align 4
  ret %Vec %load.r
}

define i32 @Vec.dot(%Vec* %0, %Vec %1) {
entry:
  %self = alloca %Vec*, align 8
  %o = alloca %Vec, align 8
  store %Vec* %0, %Vec** %self, align 8
  store %Vec %1, %Vec* %o, align 4
  %dref.self = load %Vec*, %Vec** %self, align 8
  %load.dref.self.x = getelementptr inbounds %Vec, %Vec* %dref.self, i32 0, i32 0
  %load.load.dref.self.x = load i32, i32* %load.dref.self.x, align 4
  %load.o.x = getelementptr inbounds %Vec, %Vec* %o, i32 0, i32 0
  %load.load.o.x = load i32, i32* %load.o.x, align 4
  %MUL = mul i32 %load.load.dref.self.x, %load.load.o.x
  %dref.self1 = load %Vec*, %Vec** %self, align 8
  %load.dref.self.y = getelementptr inbounds %Vec, %Vec* %dref.self1, i32 0, i32 1
  %load.load.dref.self.y = load i32, i32* %load.dref.self.y, align 4
  %load.o.y = getelementptr inbounds %Vec, %Vec* %o, i32 0, i32 1
  %load.load.o.y = load i32, i32* %load.o.y, align 4
  %MUL2 = mul i32 %load.load.dref.self.y, %load.load.o.y
  %ADD = add i32 %MUL, %MUL2
  ret i32 %ADD
}

define %Vec @make(i32 %0, i32 %1) {
entry:
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  %v = alloca %Vec, align 8
  store i32 %0, i32* %x, align 4
  store i32 %1, i32* %y, align 4
  store %Vec zeroinitializer, %Vec* %v, align 4
  %load.v.x = getelementptr inbounds %Vec, %Vec* %v, i32 0, i32 0
  %load.x = load i32, i32* %x, align 4
  store i32 %load.x, i32* %load.v.x, align 4
  %load.v.y = getelementptr inbounds %Vec, %Vec* %v, i32 0, i32 1
  %load.y = load i32, i32* %y, align 4
  store i32 %load.y, i32* %load.v.y, align 4
  %load.v = load %Vec, %Vec* %v, align 4
  ret %Vec %load.v
}

define i32 @main() {
entry:
  %a = alloca %Vec, align 8
  %b = alloca %Vec, align 8
  %c = alloca %Vec, align 8
  %tmp = alloca %Vec, align 8
  %"make(1,2)" = call %Vec @make(i32 1, i32 2)
  store %Vec %"make(1,2)", %Vec* %a, align 4
  %"make(3,4)" = call %Vec @make(i32 3, i32 4)
  store %Vec %"make(3,4)", %Vec* %b, align 4
  %load.b = load %Vec, %Vec* %b, align 4
  %"add(&load.a,load.b)" = call %Vec @Vec.add(%Vec* %a, %Vec %load.b)
  store %Vec %"add(&load.a,load.b)", %Vec* %tmp, align 4
  %load.b1 = load %Vec, %Vec* %b, align 4
  %"add(&add(&load.a,load.b),load.b)" = call %Vec @Vec.add(%Vec* %tmp, %Vec %load.b1)
  store %Vec %"add(&add(&load.a,load.b),load.b)", %Vec* %c, align 4
  %load.a = load %Vec, %Vec* %a, align 4
  %"dot(&load.c,load.a)" = call i32 @Vec.dot(%Vec* %c, %Vec %load.a)
  ret i32 %"dot(&load.c,load.a)"
}
```

---

## 014 — linked list: ref attributes, isNullable, rebinding a ref in a loop

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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Node = type { i32, %Node* }

define i32 @main() {
entry:
  %c = alloca %Node, align 8
  %b = alloca %Node, align 8
  %a = alloca %Node, align 8
  %sum = alloca i32, align 4
  %p = alloca %Node*, align 8
  store %Node zeroinitializer, %Node* %c, align 8
  %load.c.value = getelementptr inbounds %Node, %Node* %c, i32 0, i32 0
  store i32 3, i32* %load.c.value, align 4
  store %Node zeroinitializer, %Node* %b, align 8
  %load.b.value = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  store i32 2, i32* %load.b.value, align 4
  %load.b.next = getelementptr inbounds %Node, %Node* %b, i32 0, i32 1
  store %Node* %c, %Node** %load.b.next, align 8
  store %Node zeroinitializer, %Node* %a, align 8
  %load.a.value = getelementptr inbounds %Node, %Node* %a, i32 0, i32 0
  store i32 1, i32* %load.a.value, align 4
  %load.a.next = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  store %Node* %b, %Node** %load.a.next, align 8
  store i32 0, i32* %sum, align 4
  store %Node* %a, %Node** %p, align 8
  br label %cond

cond:                                             ; preds = %then, %entry
  %dref.p = load %Node*, %Node** %p, align 8
  %"isNullable(dref.p)" = icmp eq %Node* %dref.p, null
  %NOT = xor i1 %"isNullable(dref.p)", true
  br i1 %NOT, label %then, label %endwhile

then:                                             ; preds = %cond
  %dref.p1 = load %Node*, %Node** %p, align 8
  %load.dref.p.value = getelementptr inbounds %Node, %Node* %dref.p1, i32 0, i32 0
  %load.load.dref.p.value = load i32, i32* %load.dref.p.value, align 4
  %old = load i32, i32* %sum, align 4
  %ADD_ASSIGN = add i32 %old, %load.load.dref.p.value
  store i32 %ADD_ASSIGN, i32* %sum, align 4
  %dref.p2 = load %Node*, %Node** %p, align 8
  %load.dref.p.next = getelementptr inbounds %Node, %Node* %dref.p2, i32 0, i32 1
  %dref.load.dref.p.next = load %Node*, %Node** %load.dref.p.next, align 8
  store %Node* %dref.load.dref.p.next, %Node** %p, align 8
  br label %cond

endwhile:                                         ; preds = %cond
  %load.sum = load i32, i32* %sum, align 4
  %load.a.next3 = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  %dref.load.a.next = load %Node*, %Node** %load.a.next3, align 8
  %load.dref.load.a.next.next = getelementptr inbounds %Node, %Node* %dref.load.a.next, i32 0, i32 1
  %dref.load.dref.load.a.next.next = load %Node*, %Node** %load.dref.load.a.next.next, align 8
  %load.dref.load.dref.load.a.next.next.value = getelementptr inbounds %Node, %Node* %dref.load.dref.load.a.next.next, i32 0, i32 0
  %load.load.dref.load.dref.load.a.next.next.value = load i32, i32* %load.dref.load.dref.load.a.next.next.value, align 4
  %MUL = mul i32 %load.load.dref.load.dref.load.a.next.next.value, 10
  %ADD = add i32 %load.sum, %MUL
  ret i32 %ADD
}
```

