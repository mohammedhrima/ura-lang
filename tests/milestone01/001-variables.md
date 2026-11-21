# milestone01

## index

- 001 — variables: declare with a value
- 002 — variables: reassign
- 003 — variables: use another variable
- 004 — variables: compound assignment
- 005 — variables: declare variable
- 006 — array: declarations without a value start at zero
- 007 — bool: a bool variable
- 008 — bool: False skips the branch
- 009 — bool: a comparison stored in a bool, reassigned
- 010 — bool: bool parameters and return values
- 011 — bool: or between bool variables
- 012 — bool: a bool as a while condition
- 013 — bool: a bool default value is False
- 014 — bool: boolean type
- 015 — char: print chars
- 016 — char: compare chars
- 017 — char: order of chars
- 018 — char: escaped chars
- 019 — char: reassign a char
- 020 — char: char parameter and return value
- 021 — char: a char decides a loop
- 022 — char: default value of a char is '\0'
- 023 — char: declaring char/i8 variable
- 024 — if: if / elif / else chain
- 025 — if: every comparison
- 026 — if: and / or
- 027 — if: body on the same line
- 028 — if: nested ifs
- 029 — if: no branch taken: code after the if runs
- 030 — if: recursion
- 031 — if: a variable declared in a block
- 032 — if: basic if
- 033 — if: basic if/elif/else chain
- 034 — if: ifs with comparision operators
- 035 — if: nested ifs
- 036 — if: logic operators and/or
- 037 — while: count to 10
- 038 — while: break
- 039 — while: continue
- 040 — while: nested loops
- 041 — while: break leaves only the inner loop
- 042 — while: continue in the inner loop
- 043 — while: return from inside a loop
- 044 — while: loop that never runs
- 045 — while: loop in a function, called from main
- 046 — while: basic while loop
- 047 — while: break
- 048 — while: continue
- 049 — while: nested whiles
- 050 — while: get crazy
- 051 — function: parameters and calls
- 052 — function: call before the definition
- 053 — function: nested calls
- 054 — function: a parameter is a copy
- 055 — function: call result in a variable
- 056 — function: void function and bare return
- 057 — function: function inside a function
- 058 — function: calling function before declaration
- 059 — function: math op on functions
- 060 — function: bare return in a void function
- 061 — proto: printf with a number
- 062 — proto: several extra arguments
- 063 — proto: exit from C
- 064 — proto: escapes
- 065 — proto: printf returns how many chars it wrote
- 066 — proto: printf in a loop
- 067 — proto: proto function
- 068 — proto: escaped characters
- 069 — global scope: a global variable changed by a function
- 070 — crazy: mutual recursion, used before it's declared
- 071 — crazy: gcd, printf with many arguments
- 072 — crazy: collatz: a parameter changed in a loop
- 073 — crazy: the 20th prime: continue, break, a bool function
- 074 — crazy: return from two nested loops
- 075 — crazy: operator precedence soup
- 076 — crazy: negative numbers: signed / and %
- 077 — crazy: ackermann: deep recursion, a call as an argument
- 078 — crazy: one name declared in sibling blocks
- 079 — crazy: script: a global counts recursive calls
- 080 — crazy: script: nested loops with continue and break at file level

---

## 001 — variables: declare with a value

```ura
// expect: 10
fn main() i32:
    a i32 = 10
    return a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 10, i32* %a, align 4
  %"$a" = load i32, i32* %a, align 4
  ret i32 %"$a"
}
```

---

## 002 — variables: reassign

```ura
// expect: 20
fn main() i32:
    a i32 = 10
    a = 20
    return a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 10, i32* %a, align 4
  store i32 20, i32* %a, align 4
  %"$a" = load i32, i32* %a, align 4
  ret i32 %"$a"
}
```

---

## 003 — variables: use another variable

```ura
// expect: 30
fn main() i32:
    a i32 = 10
    b i32 = a * 2
    a = a + b
    return a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 10, i32* %a, align 4
  %"$a" = load i32, i32* %a, align 4
  %MUL = mul i32 %"$a", 2
  store i32 %MUL, i32* %b, align 4
  %"$a1" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %ADD = add i32 %"$a1", %"$b"
  store i32 %ADD, i32* %a, align 4
  %"$a2" = load i32, i32* %a, align 4
  ret i32 %"$a2"
}
```

---

## 004 — variables: compound assignment

```ura
// expect: 21
fn main() i32:
    a i32 = 10
    b i32 = a * 2
    a = a + b
    a += 5
    a -= 3
    a *= 2
    a /= 4
    a %= 5
    return a + b
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 10, i32* %a, align 4
  %"$a" = load i32, i32* %a, align 4
  %MUL = mul i32 %"$a", 2
  store i32 %MUL, i32* %b, align 4
  %"$a1" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %ADD = add i32 %"$a1", %"$b"
  store i32 %ADD, i32* %a, align 4
  %"$a2" = load i32, i32* %a, align 4
  %ADD3 = add i32 %"$a2", 5
  store i32 %ADD3, i32* %a, align 4
  %"$a4" = load i32, i32* %a, align 4
  %SUB = sub i32 %"$a4", 3
  store i32 %SUB, i32* %a, align 4
  %"$a5" = load i32, i32* %a, align 4
  %MUL6 = mul i32 %"$a5", 2
  store i32 %MUL6, i32* %a, align 4
  %"$a7" = load i32, i32* %a, align 4
  %DIV = sdiv i32 %"$a7", 4
  store i32 %DIV, i32* %a, align 4
  %"$a8" = load i32, i32* %a, align 4
  %MOD = srem i32 %"$a8", 5
  store i32 %MOD, i32* %a, align 4
  %"$a9" = load i32, i32* %a, align 4
  %"$b10" = load i32, i32* %b, align 4
  %ADD11 = add i32 %"$a9", %"$b10"
  ret i32 %ADD11
}
```

---

## 005 — variables: declare variable

```ura
fn main() i32:
    a i32 = 10
    a = 20
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 10, i32* %a, align 4
  store i32 20, i32* %a, align 4
  ret i32 0
}
```

---

## 006 — array: declarations without a value start at zero

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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %b = alloca i1, align 1
  %c = alloca i8, align 1
  store i32 0, i32* %x, align 4
  store i1 false, i1* %b, align 1
  store i8 0, i8* %c, align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$c" = load i8, i8* %c, align 1
  %NQ = icmp ne i8 %"$c", 0
  %"$b" = load i1, i1* %b, align 1
  %OR = or i1 %"$b", %NQ
  br i1 %OR, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  %"$x" = load i32, i32* %x, align 4
  ret i32 %"$x"
}
```

---

## 007 — bool: a bool variable

```ura
// expect: 1
fn main() i32:
    flag bool = True
    if flag:
        return 1
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %flag = alloca i1, align 1
  store i1 true, i1* %flag, align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$flag" = load i1, i1* %flag, align 1
  br i1 %"$flag", label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  ret i32 0
}
```

---

## 008 — bool: False skips the branch

```ura
// expect: 0
fn main() i32:
    flag bool = False
    if flag:
        return 1
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %flag = alloca i1, align 1
  store i1 false, i1* %flag, align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$flag" = load i1, i1* %flag, align 1
  br i1 %"$flag", label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  ret i32 0
}
```

---

## 009 — bool: a comparison stored in a bool, reassigned

```ura
// expect: 2
fn main() i32:
    a i32 = 5
    big bool = a > 3
    n i32 = 0
    if big:
        n += 1
    big = a > 9
    if big:
        n += 10
    big = False
    if big:
        n += 100
    if big == False:
        n += 1
    return n
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %big = alloca i1, align 1
  %n = alloca i32, align 4
  store i32 5, i32* %a, align 4
  %"$a" = load i32, i32* %a, align 4
  %GT = icmp sgt i32 %"$a", 3
  store i1 %GT, i1* %big, align 1
  store i32 0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$big" = load i1, i1* %big, align 1
  br i1 %"$big", label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 1
  store i32 %ADD, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  %"$a4" = load i32, i32* %a, align 4
  %GT5 = icmp sgt i32 %"$a4", 9
  store i1 %GT5, i1* %big, align 1
  br label %if1

if1:                                              ; preds = %endif
  %"$big6" = load i1, i1* %big, align 1
  br i1 %"$big6", label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  %"$n7" = load i32, i32* %n, align 4
  %ADD8 = add i32 %"$n7", 10
  store i32 %ADD8, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %if1.then, %if1
  store i1 false, i1* %big, align 1
  br label %if2

if2:                                              ; preds = %endif1
  %"$big9" = load i1, i1* %big, align 1
  br i1 %"$big9", label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"$n10" = load i32, i32* %n, align 4
  %ADD11 = add i32 %"$n10", 100
  store i32 %ADD11, i32* %n, align 4
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  br label %if3

if3:                                              ; preds = %endif2
  %"$big12" = load i1, i1* %big, align 1
  %EQ = icmp eq i1 %"$big12", false
  br i1 %EQ, label %if3.then, label %endif3

if3.then:                                         ; preds = %if3
  %"$n13" = load i32, i32* %n, align 4
  %ADD14 = add i32 %"$n13", 1
  store i32 %ADD14, i32* %n, align 4
  br label %endif3

endif3:                                           ; preds = %if3.then, %if3
  %"$n15" = load i32, i32* %n, align 4
  ret i32 %"$n15"
}
```

---

## 010 — bool: bool parameters and return values

```ura
// expect: 3
fn is_even(n i32) bool:
    return n % 2 == 0

fn both(a bool, b bool) bool:
    return a and b

fn main() i32:
    n i32 = 0
    if is_even(4):
        n += 1
    if both(is_even(2), is_even(6)):
        n += 2
    if both(is_even(2), is_even(7)):
        n += 10
    return n
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i1 @is_even(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %MOD = srem i32 %"$n", 2
  %EQ = icmp eq i32 %MOD, 0
  ret i1 %EQ
}

define i1 @both(i1 %0, i1 %1) {
entry:
  %a = alloca i1, align 1
  %b = alloca i1, align 1
  store i1 %0, i1* %a, align 1
  store i1 %1, i1* %b, align 1
  %"$a" = load i1, i1* %a, align 1
  %"$b" = load i1, i1* %b, align 1
  %AND = and i1 %"$a", %"$b"
  ret i1 %AND
}

define i32 @main() {
entry:
  %n = alloca i32, align 4
  store i32 0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %call.is_even = call i1 @is_even(i32 4)
  br i1 %call.is_even, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 1
  store i32 %ADD, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %call.is_even3 = call i1 @is_even(i32 2)
  %call.is_even4 = call i1 @is_even(i32 6)
  %call.both = call i1 @both(i1 %call.is_even3, i1 %call.is_even4)
  br i1 %call.both, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  %"$n5" = load i32, i32* %n, align 4
  %ADD6 = add i32 %"$n5", 2
  store i32 %ADD6, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %if1.then, %if1
  br label %if2

if2:                                              ; preds = %endif1
  %call.is_even7 = call i1 @is_even(i32 2)
  %call.is_even8 = call i1 @is_even(i32 7)
  %call.both9 = call i1 @both(i1 %call.is_even7, i1 %call.is_even8)
  br i1 %call.both9, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"$n10" = load i32, i32* %n, align 4
  %ADD11 = add i32 %"$n10", 10
  store i32 %ADD11, i32* %n, align 4
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"$n12" = load i32, i32* %n, align 4
  ret i32 %"$n12"
}
```

---

## 011 — bool: or between bool variables

```ura
// expect: 5
fn main() i32:
    a bool = False
    b bool = True
    n i32 = 0
    if a or b:
        n += 5
    if a and b:
        n += 50
    return n
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i1, align 1
  %b = alloca i1, align 1
  %n = alloca i32, align 4
  store i1 false, i1* %a, align 1
  store i1 true, i1* %b, align 1
  store i32 0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$a" = load i1, i1* %a, align 1
  %"$b" = load i1, i1* %b, align 1
  %OR = or i1 %"$a", %"$b"
  br i1 %OR, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 5
  store i32 %ADD, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"$a2" = load i1, i1* %a, align 1
  %"$b3" = load i1, i1* %b, align 1
  %AND = and i1 %"$a2", %"$b3"
  br i1 %AND, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  %"$n4" = load i32, i32* %n, align 4
  %ADD5 = add i32 %"$n4", 50
  store i32 %ADD5, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %if1.then, %if1
  %"$n6" = load i32, i32* %n, align 4
  ret i32 %"$n6"
}
```

---

## 012 — bool: a bool as a while condition

```ura
// expect: 4
fn main() i32:
    i i32 = 0
    going bool = True
    while going:
        i += 1
        going = i < 4
    return i
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %i = alloca i32, align 4
  %going = alloca i1, align 1
  store i32 0, i32* %i, align 4
  store i1 true, i1* %going, align 1
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$going" = load i1, i1* %going, align 1
  br i1 %"$going", label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$i" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i", 1
  store i32 %ADD, i32* %i, align 4
  %"$i1" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i1", 4
  store i1 %LT, i1* %going, align 1
  br label %while0

endloop:                                          ; preds = %while0
  %"$i2" = load i32, i32* %i, align 4
  ret i32 %"$i2"
}
```

---

## 013 — bool: a bool default value is False

```ura
// expect: 7
fn main() i32:
    flag bool
    if flag:
        return 1
    return 7
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %flag = alloca i1, align 1
  store i1 false, i1* %flag, align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$flag" = load i1, i1* %flag, align 1
  br i1 %"$flag", label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  ret i32 7
}
```

---

## 014 — bool: boolean type

```ura
fn main() i32:
    flag bool = True
    if flag:
        return 1
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %flag = alloca i1, align 1
  store i1 true, i1* %flag, align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$flag" = load i1, i1* %flag, align 1
  br i1 %"$flag", label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  ret i32 0
}
```

---

## 015 — char: print chars

```ura
// stdout: a b
// expect: 0
proto printf(fmt i8[], ...) i32

fn main() i32:
    c1 char = 'a'
    c2 i8 = 'b'
    printf("%c %c\n", c1, c2)
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [7 x i8] c"%c %c\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %c1 = alloca i8, align 1
  %c2 = alloca i8, align 1
  store i8 97, i8* %c1, align 1
  store i8 98, i8* %c2, align 1
  %"$c1" = load i8, i8* %c1, align 1
  %"$c2" = load i8, i8* %c2, align 1
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i8 %"$c1", i8 %"$c2")
  ret i32 0
}
```

---

## 016 — char: compare chars

```ura
// expect: 2
fn main() i32:
    c char = 'x'
    n i32 = 0
    if c == 'x':
        n += 1
    if c != 'y':
        n += 1
    if c == 'z':
        n += 10
    return n
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %c = alloca i8, align 1
  %n = alloca i32, align 4
  store i8 120, i8* %c, align 1
  store i32 0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$c" = load i8, i8* %c, align 1
  %EQ = icmp eq i8 %"$c", 120
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 1
  store i32 %ADD, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"$c3" = load i8, i8* %c, align 1
  %NQ = icmp ne i8 %"$c3", 121
  br i1 %NQ, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  %"$n4" = load i32, i32* %n, align 4
  %ADD5 = add i32 %"$n4", 1
  store i32 %ADD5, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %if1.then, %if1
  br label %if2

if2:                                              ; preds = %endif1
  %"$c6" = load i8, i8* %c, align 1
  %EQ7 = icmp eq i8 %"$c6", 122
  br i1 %EQ7, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"$n8" = load i32, i32* %n, align 4
  %ADD9 = add i32 %"$n8", 10
  store i32 %ADD9, i32* %n, align 4
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"$n10" = load i32, i32* %n, align 4
  ret i32 %"$n10"
}
```

---

## 017 — char: order of chars

```ura
// expect: 3
fn main() i32:
    c char = 'm'
    n i32 = 0
    if c > 'a':
        n += 1
    if c < 'z':
        n += 1
    if c >= 'm' and c <= 'm':
        n += 1
    return n
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %c = alloca i8, align 1
  %n = alloca i32, align 4
  store i8 109, i8* %c, align 1
  store i32 0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$c" = load i8, i8* %c, align 1
  %GT = icmp sgt i8 %"$c", 97
  br i1 %GT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 1
  store i32 %ADD, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"$c3" = load i8, i8* %c, align 1
  %LT = icmp slt i8 %"$c3", 122
  br i1 %LT, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  %"$n4" = load i32, i32* %n, align 4
  %ADD5 = add i32 %"$n4", 1
  store i32 %ADD5, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %if1.then, %if1
  br label %if2

if2:                                              ; preds = %endif1
  %"$c6" = load i8, i8* %c, align 1
  %GE = icmp sge i8 %"$c6", 109
  %"$c7" = load i8, i8* %c, align 1
  %LE = icmp sle i8 %"$c7", 109
  %AND = and i1 %GE, %LE
  br i1 %AND, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"$n8" = load i32, i32* %n, align 4
  %ADD9 = add i32 %"$n8", 1
  store i32 %ADD9, i32* %n, align 4
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"$n10" = load i32, i32* %n, align 4
  ret i32 %"$n10"
}
```

---

## 018 — char: escaped chars

```ura
// stdout: [newline] [tab] ['] [\]
// expect: 0
proto printf(fmt i8[], ...) i32

fn main() i32:
    nl char = '\n'
    tab char = '\t'
    printf("[%c]\n", nl)
    printf("[%c]\n", tab)
    printf("[%c] [%c]\n", '\'', '\\')
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [6 x i8] c"[%c]\0A\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"[%c]\0A\00", align 1
@str.2 = private unnamed_addr constant [11 x i8] c"[%c] [%c]\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %nl = alloca i8, align 1
  %tab = alloca i8, align 1
  store i8 10, i8* %nl, align 1
  store i8 9, i8* %tab, align 1
  %"$nl" = load i8, i8* %nl, align 1
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i8 %"$nl")
  %"$tab" = load i8, i8* %tab, align 1
  %call.printf1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i8 %"$tab")
  %call.printf2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.2, i32 0, i32 0), i8 39, i8 92)
  ret i32 0
}
```

---

## 019 — char: reassign a char

```ura
// stdout: q
// expect: 0
proto printf(fmt i8[], ...) i32

fn main() i32:
    c char = 'p'
    c = 'q'
    printf("%c\n", c)
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"%c\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %c = alloca i8, align 1
  store i8 112, i8* %c, align 1
  store i8 113, i8* %c, align 1
  %"$c" = load i8, i8* %c, align 1
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i8 %"$c")
  ret i32 0
}
```

---

## 020 — char: char parameter and return value

```ura
// stdout: Y N
// expect: 0
proto printf(fmt i8[], ...) i32

fn answer(yes bool) char:
    if yes:
        return 'Y'
    return 'N'

fn show(c char):
    printf("%c", c)

fn main() i32:
    show(answer(True))
    printf(" ")
    show(answer(False))
    printf("\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [3 x i8] c"%c\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

declare i32 @printf(i8*, ...)

define i8 @answer(i1 %0) {
entry:
  %yes = alloca i1, align 1
  store i1 %0, i1* %yes, align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$yes" = load i1, i1* %yes, align 1
  br i1 %"$yes", label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i8 89

endif:                                            ; preds = %if0
  ret i8 78
}

define void @show(i8 %0) {
entry:
  %c = alloca i8, align 1
  store i8 %0, i8* %c, align 1
  %"$c" = load i8, i8* %c, align 1
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i8 %"$c")
  ret void
}

define i32 @main() {
entry:
  %call.answer = call i8 @answer(i1 true)
  call void @show(i8 %call.answer)
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %call.answer1 = call i8 @answer(i1 false)
  call void @show(i8 %call.answer1)
  %call.printf2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  ret i32 0
}
```

---

## 021 — char: a char decides a loop

```ura
// expect: 4
fn main() i32:
    c char = 'a'
    n i32 = 0
    while c != 'e':
        n += 1
        if n == 1:
            c = 'b'
        elif n == 2:
            c = 'c'
        elif n == 3:
            c = 'd'
        else:
            c = 'e'
    return n
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %c = alloca i8, align 1
  %n = alloca i32, align 4
  store i8 97, i8* %c, align 1
  store i32 0, i32* %n, align 4
  br label %while0

while0:                                           ; preds = %endif, %entry
  %"$c" = load i8, i8* %c, align 1
  %NQ = icmp ne i8 %"$c", 101
  br i1 %NQ, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 1
  store i32 %ADD, i32* %n, align 4
  br label %if1

if1:                                              ; preds = %while0.then
  %"$n1" = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %"$n1", 1
  br i1 %EQ, label %if1.then, label %elif2

if1.then:                                         ; preds = %if1
  store i8 98, i8* %c, align 1
  br label %endif

elif2:                                            ; preds = %if1
  %"$n2" = load i32, i32* %n, align 4
  %EQ3 = icmp eq i32 %"$n2", 2
  br i1 %EQ3, label %elif2.then, label %elif3

elif2.then:                                       ; preds = %elif2
  store i8 99, i8* %c, align 1
  br label %endif

elif3:                                            ; preds = %elif2
  %"$n4" = load i32, i32* %n, align 4
  %EQ5 = icmp eq i32 %"$n4", 3
  br i1 %EQ5, label %elif3.then, label %else4

elif3.then:                                       ; preds = %elif3
  store i8 100, i8* %c, align 1
  br label %endif

else4:                                            ; preds = %elif3
  store i8 101, i8* %c, align 1
  br label %endif

endif:                                            ; preds = %else4, %elif3.then, %elif2.then, %if1.then
  br label %while0

endloop:                                          ; preds = %while0
  %"$n6" = load i32, i32* %n, align 4
  ret i32 %"$n6"
}
```

---

## 022 — char: default value of a char is '\0'

```ura
// expect: 1
fn main() i32:
    c char
    if c == '\0':
        return 1
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %c = alloca i8, align 1
  store i8 0, i8* %c, align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$c" = load i8, i8* %c, align 1
  %EQ = icmp eq i8 %"$c", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  ret i32 0
}
```

---

## 023 — char: declaring char/i8 variable

```ura
proto exit(code i32)

fn main() i32:
    c1 char = 'a'
    c2 i8 = 'b'
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %c1 = alloca i8, align 1
  %c2 = alloca i8, align 1
  store i8 97, i8* %c1, align 1
  store i8 98, i8* %c2, align 1
  ret i32 0
}
```

---

## 024 — if: if / elif / else chain

```ura
// expect: 123
fn grade(n i32) i32:
    if n < 10:
        return 1
    elif n < 20:
        return 2
    else:
        return 3

fn main() i32:
    return grade(5) * 100 + grade(15) * 10 + grade(25)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @grade(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %LT = icmp slt i32 %"$n", 10
  br i1 %LT, label %if0.then, label %elif1

if0.then:                                         ; preds = %if0
  ret i32 1

elif1:                                            ; preds = %if0
  %"$n1" = load i32, i32* %n, align 4
  %LT2 = icmp slt i32 %"$n1", 20
  br i1 %LT2, label %elif1.then, label %else2

elif1.then:                                       ; preds = %elif1
  ret i32 2

else2:                                            ; preds = %elif1
  ret i32 3

endif:                                            ; No predecessors!
  ret i32 0
}

define i32 @main() {
entry:
  %call.grade = call i32 @grade(i32 5)
  %MUL = mul i32 %call.grade, 100
  %call.grade1 = call i32 @grade(i32 15)
  %MUL2 = mul i32 %call.grade1, 10
  %ADD = add i32 %MUL, %MUL2
  %call.grade3 = call i32 @grade(i32 25)
  %ADD4 = add i32 %ADD, %call.grade3
  ret i32 %ADD4
}
```

---

## 025 — if: every comparison

```ura
// expect: 6
fn main() i32:
    a i32 = 5
    n i32 = 0
    if a == 5:
        n += 1
    if a != 4:
        n += 1
    if a < 6:
        n += 1
    if a <= 5:
        n += 1
    if a > 4:
        n += 1
    if a >= 5:
        n += 1
    if a > 5:
        n += 100
    return n
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %n = alloca i32, align 4
  store i32 5, i32* %a, align 4
  store i32 0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$a" = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %"$a", 5
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 1
  store i32 %ADD, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"$a7" = load i32, i32* %a, align 4
  %NQ = icmp ne i32 %"$a7", 4
  br i1 %NQ, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  %"$n8" = load i32, i32* %n, align 4
  %ADD9 = add i32 %"$n8", 1
  store i32 %ADD9, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %if1.then, %if1
  br label %if2

if2:                                              ; preds = %endif1
  %"$a10" = load i32, i32* %a, align 4
  %LT = icmp slt i32 %"$a10", 6
  br i1 %LT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"$n11" = load i32, i32* %n, align 4
  %ADD12 = add i32 %"$n11", 1
  store i32 %ADD12, i32* %n, align 4
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  br label %if3

if3:                                              ; preds = %endif2
  %"$a13" = load i32, i32* %a, align 4
  %LE = icmp sle i32 %"$a13", 5
  br i1 %LE, label %if3.then, label %endif3

if3.then:                                         ; preds = %if3
  %"$n14" = load i32, i32* %n, align 4
  %ADD15 = add i32 %"$n14", 1
  store i32 %ADD15, i32* %n, align 4
  br label %endif3

endif3:                                           ; preds = %if3.then, %if3
  br label %if4

if4:                                              ; preds = %endif3
  %"$a16" = load i32, i32* %a, align 4
  %GT = icmp sgt i32 %"$a16", 4
  br i1 %GT, label %if4.then, label %endif4

if4.then:                                         ; preds = %if4
  %"$n17" = load i32, i32* %n, align 4
  %ADD18 = add i32 %"$n17", 1
  store i32 %ADD18, i32* %n, align 4
  br label %endif4

endif4:                                           ; preds = %if4.then, %if4
  br label %if5

if5:                                              ; preds = %endif4
  %"$a19" = load i32, i32* %a, align 4
  %GE = icmp sge i32 %"$a19", 5
  br i1 %GE, label %if5.then, label %endif5

if5.then:                                         ; preds = %if5
  %"$n20" = load i32, i32* %n, align 4
  %ADD21 = add i32 %"$n20", 1
  store i32 %ADD21, i32* %n, align 4
  br label %endif5

endif5:                                           ; preds = %if5.then, %if5
  br label %if6

if6:                                              ; preds = %endif5
  %"$a22" = load i32, i32* %a, align 4
  %GT23 = icmp sgt i32 %"$a22", 5
  br i1 %GT23, label %if6.then, label %endif6

if6.then:                                         ; preds = %if6
  %"$n24" = load i32, i32* %n, align 4
  %ADD25 = add i32 %"$n24", 100
  store i32 %ADD25, i32* %n, align 4
  br label %endif6

endif6:                                           ; preds = %if6.then, %if6
  %"$n26" = load i32, i32* %n, align 4
  ret i32 %"$n26"
}
```

---

## 026 — if: and / or

```ura
// expect: 11
fn main() i32:
    a i32 = 5
    n i32 = 0
    if a > 1 and a < 10:
        n += 1
    if a < 1 or a == 5:
        n += 10
    if a < 1 and a == 5:
        n += 100
    return n
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %n = alloca i32, align 4
  store i32 5, i32* %a, align 4
  store i32 0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$a" = load i32, i32* %a, align 4
  %GT = icmp sgt i32 %"$a", 1
  %"$a3" = load i32, i32* %a, align 4
  %LT = icmp slt i32 %"$a3", 10
  %AND = and i1 %GT, %LT
  br i1 %AND, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 1
  store i32 %ADD, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"$a4" = load i32, i32* %a, align 4
  %LT5 = icmp slt i32 %"$a4", 1
  %"$a6" = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %"$a6", 5
  %OR = or i1 %LT5, %EQ
  br i1 %OR, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  %"$n7" = load i32, i32* %n, align 4
  %ADD8 = add i32 %"$n7", 10
  store i32 %ADD8, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %if1.then, %if1
  br label %if2

if2:                                              ; preds = %endif1
  %"$a9" = load i32, i32* %a, align 4
  %LT10 = icmp slt i32 %"$a9", 1
  %"$a11" = load i32, i32* %a, align 4
  %EQ12 = icmp eq i32 %"$a11", 5
  %AND13 = and i1 %LT10, %EQ12
  br i1 %AND13, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"$n14" = load i32, i32* %n, align 4
  %ADD15 = add i32 %"$n14", 100
  store i32 %ADD15, i32* %n, align 4
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"$n16" = load i32, i32* %n, align 4
  ret i32 %"$n16"
}
```

---

## 027 — if: body on the same line

```ura
// expect: 3
fn main() i32:
    a i32 = 5
    n i32 = 0
    if a > 3: n += 1
    if a > 9: n += 10
    if a == 5: n += 2
    return n
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %n = alloca i32, align 4
  store i32 5, i32* %a, align 4
  store i32 0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$a" = load i32, i32* %a, align 4
  %GT = icmp sgt i32 %"$a", 3
  br i1 %GT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 1
  store i32 %ADD, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"$a3" = load i32, i32* %a, align 4
  %GT4 = icmp sgt i32 %"$a3", 9
  br i1 %GT4, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  %"$n5" = load i32, i32* %n, align 4
  %ADD6 = add i32 %"$n5", 10
  store i32 %ADD6, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %if1.then, %if1
  br label %if2

if2:                                              ; preds = %endif1
  %"$a7" = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %"$a7", 5
  br i1 %EQ, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"$n8" = load i32, i32* %n, align 4
  %ADD9 = add i32 %"$n8", 2
  store i32 %ADD9, i32* %n, align 4
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"$n10" = load i32, i32* %n, align 4
  ret i32 %"$n10"
}
```

---

## 028 — if: nested ifs

```ura
// expect: 1
fn main() i32:
    a i32 = 5
    b i32 = 7
    if a == 5:
        if b == 7:
            return 1
        return 2
    return 3
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 5, i32* %a, align 4
  store i32 7, i32* %b, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$a" = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %"$a", 5
  br i1 %EQ, label %if0.then, label %endif1

if0.then:                                         ; preds = %if0
  br label %if1

if1:                                              ; preds = %if0.then
  %"$b" = load i32, i32* %b, align 4
  %EQ2 = icmp eq i32 %"$b", 7
  br i1 %EQ2, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  ret i32 1

endif:                                            ; preds = %if1
  ret i32 2

endif1:                                           ; preds = %if0
  ret i32 3
}
```

---

## 029 — if: no branch taken: code after the if runs

```ura
// expect: 9
fn main() i32:
    a i32 = 1
    if a == 2:
        a = 100
    elif a == 3:
        a = 200
    return a + 8
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 1, i32* %a, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$a" = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %"$a", 2
  br i1 %EQ, label %if0.then, label %elif1

if0.then:                                         ; preds = %if0
  store i32 100, i32* %a, align 4
  br label %endif

elif1:                                            ; preds = %if0
  %"$a1" = load i32, i32* %a, align 4
  %EQ2 = icmp eq i32 %"$a1", 3
  br i1 %EQ2, label %elif1.then, label %endif

elif1.then:                                       ; preds = %elif1
  store i32 200, i32* %a, align 4
  br label %endif

endif:                                            ; preds = %elif1.then, %elif1, %if0.then
  %"$a3" = load i32, i32* %a, align 4
  %ADD = add i32 %"$a3", 8
  ret i32 %ADD
}
```

---

## 030 — if: recursion

```ura
fn fact(n i32) i32:
    if n <= 1:
        return 1
    return n * fact(n - 1)

fn main() i32:
    return fact(5)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @fact(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %LE = icmp sle i32 %"$n", 1
  br i1 %LE, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  %"$n1" = load i32, i32* %n, align 4
  %SUB = sub i32 %"$n1", 1
  %call.fact = call i32 @fact(i32 %SUB)
  %"$n2" = load i32, i32* %n, align 4
  %MUL = mul i32 %"$n2", %call.fact
  ret i32 %MUL
}

define i32 @main() {
entry:
  %call.fact = call i32 @fact(i32 5)
  ret i32 %call.fact
}
```

---

## 031 — if: a variable declared in a block

```ura
// expect: 3
fn main() i32:
    a i32 = 1
    if a == 1:
        b i32 = 2
        a = a + b
    return a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 1, i32* %a, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$a" = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %"$a", 1
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  store i32 2, i32* %b, align 4
  %"$a1" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %ADD = add i32 %"$a1", %"$b"
  store i32 %ADD, i32* %a, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  %"$a2" = load i32, i32* %a, align 4
  ret i32 %"$a2"
}
```

---

## 032 — if: basic if

```ura
fn main() i32:
    a i32 = 0
    if a < 1:
        return 1
    return 2
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 0, i32* %a, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$a" = load i32, i32* %a, align 4
  %LT = icmp slt i32 %"$a", 1
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  ret i32 2
}
```

---

## 033 — if: basic if/elif/else chain

```ura
fn main() i32:
    a i32 = 0
    if a < 1:
        return 1
    elif a < 2:
        return 2
    else:
        return 3
    return a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 0, i32* %a, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$a" = load i32, i32* %a, align 4
  %LT = icmp slt i32 %"$a", 1
  br i1 %LT, label %if0.then, label %elif1

if0.then:                                         ; preds = %if0
  ret i32 1

elif1:                                            ; preds = %if0
  %"$a1" = load i32, i32* %a, align 4
  %LT2 = icmp slt i32 %"$a1", 2
  br i1 %LT2, label %elif1.then, label %else2

elif1.then:                                       ; preds = %elif1
  ret i32 2

else2:                                            ; preds = %elif1
  ret i32 3

endif:                                            ; No predecessors!
  %"$a3" = load i32, i32* %a, align 4
  ret i32 %"$a3"
}
```

---

## 034 — if: ifs with comparision operators

```ura
fn main() i32:
    a i32 = 5
    n i32 = 0
    if a > 3: n = n + 1
    if a > 5: n = n + 10
    if a < 6: n = n + 1
    if a < 5: n = n + 10
    if a >= 5: n = n + 1
    if a >= 6: n = n + 10
    if a <= 5: n = n + 1
    if a <= 4: n = n + 10
    if a == 5: n = n + 1
    if a == 4: n = n + 10
    if a != 4: n = n + 1
    if a != 5: n = n + 10
    return n
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %n = alloca i32, align 4
  store i32 5, i32* %a, align 4
  store i32 0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$a" = load i32, i32* %a, align 4
  %GT = icmp sgt i32 %"$a", 3
  br i1 %GT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 1
  store i32 %ADD, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"$a12" = load i32, i32* %a, align 4
  %GT13 = icmp sgt i32 %"$a12", 5
  br i1 %GT13, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  %"$n14" = load i32, i32* %n, align 4
  %ADD15 = add i32 %"$n14", 10
  store i32 %ADD15, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %if1.then, %if1
  br label %if2

if2:                                              ; preds = %endif1
  %"$a16" = load i32, i32* %a, align 4
  %LT = icmp slt i32 %"$a16", 6
  br i1 %LT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"$n17" = load i32, i32* %n, align 4
  %ADD18 = add i32 %"$n17", 1
  store i32 %ADD18, i32* %n, align 4
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  br label %if3

if3:                                              ; preds = %endif2
  %"$a19" = load i32, i32* %a, align 4
  %LT20 = icmp slt i32 %"$a19", 5
  br i1 %LT20, label %if3.then, label %endif3

if3.then:                                         ; preds = %if3
  %"$n21" = load i32, i32* %n, align 4
  %ADD22 = add i32 %"$n21", 10
  store i32 %ADD22, i32* %n, align 4
  br label %endif3

endif3:                                           ; preds = %if3.then, %if3
  br label %if4

if4:                                              ; preds = %endif3
  %"$a23" = load i32, i32* %a, align 4
  %GE = icmp sge i32 %"$a23", 5
  br i1 %GE, label %if4.then, label %endif4

if4.then:                                         ; preds = %if4
  %"$n24" = load i32, i32* %n, align 4
  %ADD25 = add i32 %"$n24", 1
  store i32 %ADD25, i32* %n, align 4
  br label %endif4

endif4:                                           ; preds = %if4.then, %if4
  br label %if5

if5:                                              ; preds = %endif4
  %"$a26" = load i32, i32* %a, align 4
  %GE27 = icmp sge i32 %"$a26", 6
  br i1 %GE27, label %if5.then, label %endif5

if5.then:                                         ; preds = %if5
  %"$n28" = load i32, i32* %n, align 4
  %ADD29 = add i32 %"$n28", 10
  store i32 %ADD29, i32* %n, align 4
  br label %endif5

endif5:                                           ; preds = %if5.then, %if5
  br label %if6

if6:                                              ; preds = %endif5
  %"$a30" = load i32, i32* %a, align 4
  %LE = icmp sle i32 %"$a30", 5
  br i1 %LE, label %if6.then, label %endif6

if6.then:                                         ; preds = %if6
  %"$n31" = load i32, i32* %n, align 4
  %ADD32 = add i32 %"$n31", 1
  store i32 %ADD32, i32* %n, align 4
  br label %endif6

endif6:                                           ; preds = %if6.then, %if6
  br label %if7

if7:                                              ; preds = %endif6
  %"$a33" = load i32, i32* %a, align 4
  %LE34 = icmp sle i32 %"$a33", 4
  br i1 %LE34, label %if7.then, label %endif7

if7.then:                                         ; preds = %if7
  %"$n35" = load i32, i32* %n, align 4
  %ADD36 = add i32 %"$n35", 10
  store i32 %ADD36, i32* %n, align 4
  br label %endif7

endif7:                                           ; preds = %if7.then, %if7
  br label %if8

if8:                                              ; preds = %endif7
  %"$a37" = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %"$a37", 5
  br i1 %EQ, label %if8.then, label %endif8

if8.then:                                         ; preds = %if8
  %"$n38" = load i32, i32* %n, align 4
  %ADD39 = add i32 %"$n38", 1
  store i32 %ADD39, i32* %n, align 4
  br label %endif8

endif8:                                           ; preds = %if8.then, %if8
  br label %if9

if9:                                              ; preds = %endif8
  %"$a40" = load i32, i32* %a, align 4
  %EQ41 = icmp eq i32 %"$a40", 4
  br i1 %EQ41, label %if9.then, label %endif9

if9.then:                                         ; preds = %if9
  %"$n42" = load i32, i32* %n, align 4
  %ADD43 = add i32 %"$n42", 10
  store i32 %ADD43, i32* %n, align 4
  br label %endif9

endif9:                                           ; preds = %if9.then, %if9
  br label %if10

if10:                                             ; preds = %endif9
  %"$a44" = load i32, i32* %a, align 4
  %NQ = icmp ne i32 %"$a44", 4
  br i1 %NQ, label %if10.then, label %endif10

if10.then:                                        ; preds = %if10
  %"$n45" = load i32, i32* %n, align 4
  %ADD46 = add i32 %"$n45", 1
  store i32 %ADD46, i32* %n, align 4
  br label %endif10

endif10:                                          ; preds = %if10.then, %if10
  br label %if11

if11:                                             ; preds = %endif10
  %"$a47" = load i32, i32* %a, align 4
  %NQ48 = icmp ne i32 %"$a47", 5
  br i1 %NQ48, label %if11.then, label %endif11

if11.then:                                        ; preds = %if11
  %"$n49" = load i32, i32* %n, align 4
  %ADD50 = add i32 %"$n49", 10
  store i32 %ADD50, i32* %n, align 4
  br label %endif11

endif11:                                          ; preds = %if11.then, %if11
  %"$n51" = load i32, i32* %n, align 4
  ret i32 %"$n51"
}
```

---

## 035 — if: nested ifs

```ura
fn main() i32:
    a i32 = 5
    b i32 = 7
    if a == 5:
        if b == 7: return 1
        return 2
    return 3
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 5, i32* %a, align 4
  store i32 7, i32* %b, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$a" = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %"$a", 5
  br i1 %EQ, label %if0.then, label %endif1

if0.then:                                         ; preds = %if0
  br label %if1

if1:                                              ; preds = %if0.then
  %"$b" = load i32, i32* %b, align 4
  %EQ2 = icmp eq i32 %"$b", 7
  br i1 %EQ2, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  ret i32 1

endif:                                            ; preds = %if1
  ret i32 2

endif1:                                           ; preds = %if0
  ret i32 3
}
```

---

## 036 — if: logic operators and/or

```ura
fn main() i32:
    a i32 = 5
    if a > 1 and a < 10:
        return 1
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 5, i32* %a, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$a" = load i32, i32* %a, align 4
  %GT = icmp sgt i32 %"$a", 1
  %"$a1" = load i32, i32* %a, align 4
  %LT = icmp slt i32 %"$a1", 10
  %AND = and i1 %GT, %LT
  br i1 %AND, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  ret i32 0
}
```

---

## 037 — while: count to 10

```ura
// expect: 10
fn main() i32:
    a i32 = 0
    while a < 10:
        a += 1
    return a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 0, i32* %a, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$a" = load i32, i32* %a, align 4
  %LT = icmp slt i32 %"$a", 10
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$a1" = load i32, i32* %a, align 4
  %ADD = add i32 %"$a1", 1
  store i32 %ADD, i32* %a, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$a2" = load i32, i32* %a, align 4
  ret i32 %"$a2"
}
```

---

## 038 — while: break

```ura
// expect: 3
fn main() i32:
    a i32 = 0
    while a < 10:
        if a == 3:
            break
        a += 1
    return a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 0, i32* %a, align 4
  br label %while0

while0:                                           ; preds = %endif, %entry
  %"$a" = load i32, i32* %a, align 4
  %LT = icmp slt i32 %"$a", 10
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  br label %if1

if1:                                              ; preds = %while0.then
  %"$a1" = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %"$a1", 3
  br i1 %EQ, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  br label %endloop

endif:                                            ; preds = %if1
  %"$a2" = load i32, i32* %a, align 4
  %ADD = add i32 %"$a2", 1
  store i32 %ADD, i32* %a, align 4
  br label %while0

endloop:                                          ; preds = %if1.then, %while0
  %"$a3" = load i32, i32* %a, align 4
  ret i32 %"$a3"
}
```

---

## 039 — while: continue

```ura
// expect: 25   (1 + 3 + 5 + 7 + 9)
fn main() i32:
    i i32 = 0
    sum i32 = 0
    while i < 10:
        i += 1
        if i % 2 == 0:
            continue
        sum += i
    return sum
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %i = alloca i32, align 4
  %sum = alloca i32, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %sum, align 4
  br label %while0

while0:                                           ; preds = %endif, %if1.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 10
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$i1" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i1", 1
  store i32 %ADD, i32* %i, align 4
  br label %if1

if1:                                              ; preds = %while0.then
  %"$i2" = load i32, i32* %i, align 4
  %MOD = srem i32 %"$i2", 2
  %EQ = icmp eq i32 %MOD, 0
  br i1 %EQ, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  br label %while0

endif:                                            ; preds = %if1
  %"$i3" = load i32, i32* %i, align 4
  %"$sum" = load i32, i32* %sum, align 4
  %ADD4 = add i32 %"$sum", %"$i3"
  store i32 %ADD4, i32* %sum, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$sum5" = load i32, i32* %sum, align 4
  ret i32 %"$sum5"
}
```

---

## 040 — while: nested loops

```ura
// expect: 12   (3 x 4)
fn main() i32:
    i i32 = 0
    c i32 = 0
    while i < 3:
        j i32 = 0
        while j < 4:
            c += 1
            j += 1
        i += 1
    return c
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %i = alloca i32, align 4
  %c = alloca i32, align 4
  %j = alloca i32, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %c, align 4
  br label %while0

while0:                                           ; preds = %endloop, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while0.then, label %endloop1

while0.then:                                      ; preds = %while0
  store i32 0, i32* %j, align 4
  br label %while1

while1:                                           ; preds = %while1.then, %while0.then
  %"$j" = load i32, i32* %j, align 4
  %LT2 = icmp slt i32 %"$j", 4
  br i1 %LT2, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"$c" = load i32, i32* %c, align 4
  %ADD = add i32 %"$c", 1
  store i32 %ADD, i32* %c, align 4
  %"$j3" = load i32, i32* %j, align 4
  %ADD4 = add i32 %"$j3", 1
  store i32 %ADD4, i32* %j, align 4
  br label %while1

endloop:                                          ; preds = %while1
  %"$i5" = load i32, i32* %i, align 4
  %ADD6 = add i32 %"$i5", 1
  store i32 %ADD6, i32* %i, align 4
  br label %while0

endloop1:                                         ; preds = %while0
  %"$c7" = load i32, i32* %c, align 4
  ret i32 %"$c7"
}
```

---

## 041 — while: break leaves only the inner loop

```ura
// expect: 3
fn main() i32:
    i i32 = 0
    c i32 = 0
    while i < 3:
        while c < 100:
            c += 1
            break
        i += 1
    return c
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %i = alloca i32, align 4
  %c = alloca i32, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %c, align 4
  br label %while0

while0:                                           ; preds = %endloop, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while0.then, label %endloop1

while0.then:                                      ; preds = %while0
  br label %while1

while1:                                           ; preds = %while0.then
  %"$c" = load i32, i32* %c, align 4
  %LT2 = icmp slt i32 %"$c", 100
  br i1 %LT2, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"$c3" = load i32, i32* %c, align 4
  %ADD = add i32 %"$c3", 1
  store i32 %ADD, i32* %c, align 4
  br label %endloop

endloop:                                          ; preds = %while1.then, %while1
  %"$i4" = load i32, i32* %i, align 4
  %ADD5 = add i32 %"$i4", 1
  store i32 %ADD5, i32* %i, align 4
  br label %while0

endloop1:                                         ; preds = %while0
  %"$c6" = load i32, i32* %c, align 4
  ret i32 %"$c6"
}
```

---

## 042 — while: continue in the inner loop

```ura
// expect: 6   (j = 1 and j = 3, three times)
fn main() i32:
    i i32 = 0
    c i32 = 0
    while i < 3:
        j i32 = 0
        while j < 4:
            j += 1
            if j % 2 == 0:
                continue
            c += 1
        i += 1
    return c
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %i = alloca i32, align 4
  %c = alloca i32, align 4
  %j = alloca i32, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %c, align 4
  br label %while0

while0:                                           ; preds = %endloop, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while0.then, label %endloop1

while0.then:                                      ; preds = %while0
  store i32 0, i32* %j, align 4
  br label %while1

while1:                                           ; preds = %endif, %if2.then, %while0.then
  %"$j" = load i32, i32* %j, align 4
  %LT2 = icmp slt i32 %"$j", 4
  br i1 %LT2, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"$j3" = load i32, i32* %j, align 4
  %ADD = add i32 %"$j3", 1
  store i32 %ADD, i32* %j, align 4
  br label %if2

if2:                                              ; preds = %while1.then
  %"$j4" = load i32, i32* %j, align 4
  %MOD = srem i32 %"$j4", 2
  %EQ = icmp eq i32 %MOD, 0
  br i1 %EQ, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  br label %while1

endif:                                            ; preds = %if2
  %"$c" = load i32, i32* %c, align 4
  %ADD5 = add i32 %"$c", 1
  store i32 %ADD5, i32* %c, align 4
  br label %while1

endloop:                                          ; preds = %while1
  %"$i6" = load i32, i32* %i, align 4
  %ADD7 = add i32 %"$i6", 1
  store i32 %ADD7, i32* %i, align 4
  br label %while0

endloop1:                                         ; preds = %while0
  %"$c8" = load i32, i32* %c, align 4
  ret i32 %"$c8"
}
```

---

## 043 — while: return from inside a loop

```ura
// expect: 7
fn main() i32:
    i i32 = 0
    while i < 100:
        if i == 7:
            return i
        i += 1
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %endif, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 100
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  br label %if1

if1:                                              ; preds = %while0.then
  %"$i1" = load i32, i32* %i, align 4
  %EQ = icmp eq i32 %"$i1", 7
  br i1 %EQ, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  %"$i2" = load i32, i32* %i, align 4
  ret i32 %"$i2"

endif:                                            ; preds = %if1
  %"$i3" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i3", 1
  store i32 %ADD, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  ret i32 0
}
```

---

## 044 — while: loop that never runs

```ura
// expect: 5
fn main() i32:
    a i32 = 5
    while a < 0:
        a = 100
    return a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 5, i32* %a, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$a" = load i32, i32* %a, align 4
  %LT = icmp slt i32 %"$a", 0
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  store i32 100, i32* %a, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$a1" = load i32, i32* %a, align 4
  ret i32 %"$a1"
}
```

---

## 045 — while: loop in a function, called from main

```ura
// expect: 55   (1 + 2 + ... + 10)
fn sum_to(n i32) i32:
    total i32 = 0
    i i32 = 1
    while i <= n:
        total += i
        i += 1
    return total

fn main() i32:
    return sum_to(10)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @sum_to(i32 %0) {
entry:
  %n = alloca i32, align 4
  %total = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  store i32 0, i32* %total, align 4
  store i32 1, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %"$n" = load i32, i32* %n, align 4
  %LE = icmp sle i32 %"$i", %"$n"
  br i1 %LE, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$i1" = load i32, i32* %i, align 4
  %"$total" = load i32, i32* %total, align 4
  %ADD = add i32 %"$total", %"$i1"
  store i32 %ADD, i32* %total, align 4
  %"$i2" = load i32, i32* %i, align 4
  %ADD3 = add i32 %"$i2", 1
  store i32 %ADD3, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$total4" = load i32, i32* %total, align 4
  ret i32 %"$total4"
}

define i32 @main() {
entry:
  %call.sum_to = call i32 @sum_to(i32 10)
  ret i32 %call.sum_to
}
```

---

## 046 — while: basic while loop

```ura
fn main() i32:
    a i32 = 0
    while a < 10:
        a = a + 1
    return a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 0, i32* %a, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$a" = load i32, i32* %a, align 4
  %LT = icmp slt i32 %"$a", 10
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$a1" = load i32, i32* %a, align 4
  %ADD = add i32 %"$a1", 1
  store i32 %ADD, i32* %a, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$a2" = load i32, i32* %a, align 4
  ret i32 %"$a2"
}
```

---

## 047 — while: break

```ura
fn main() i32:
    a i32 = 0
    while a < 10:
        if a == 3:
            break
        a = a + 1
    return a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 0, i32* %a, align 4
  br label %while0

while0:                                           ; preds = %endif, %entry
  %"$a" = load i32, i32* %a, align 4
  %LT = icmp slt i32 %"$a", 10
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  br label %if1

if1:                                              ; preds = %while0.then
  %"$a1" = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %"$a1", 3
  br i1 %EQ, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  br label %endloop

endif:                                            ; preds = %if1
  %"$a2" = load i32, i32* %a, align 4
  %ADD = add i32 %"$a2", 1
  store i32 %ADD, i32* %a, align 4
  br label %while0

endloop:                                          ; preds = %if1.then, %while0
  %"$a3" = load i32, i32* %a, align 4
  ret i32 %"$a3"
}
```

---

## 048 — while: continue

```ura
fn main() i32:
    a i32 = 0
    while a < 10:
        if a == 3:
            a = a + 2
            continue
        a = a + 1
    return a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 0, i32* %a, align 4
  br label %while0

while0:                                           ; preds = %endif, %if1.then, %entry
  %"$a" = load i32, i32* %a, align 4
  %LT = icmp slt i32 %"$a", 10
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  br label %if1

if1:                                              ; preds = %while0.then
  %"$a1" = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %"$a1", 3
  br i1 %EQ, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  %"$a2" = load i32, i32* %a, align 4
  %ADD = add i32 %"$a2", 2
  store i32 %ADD, i32* %a, align 4
  br label %while0

endif:                                            ; preds = %if1
  %"$a3" = load i32, i32* %a, align 4
  %ADD4 = add i32 %"$a3", 1
  store i32 %ADD4, i32* %a, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$a5" = load i32, i32* %a, align 4
  ret i32 %"$a5"
}
```

---

## 049 — while: nested whiles

```ura
// break leaves only the innermost loop
// expect: 3
fn main() i32:
    i i32 = 0
    c i32 = 0
    while i < 3:
        while True: c = c + 1 break
        i = i + 1
    return c
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %i = alloca i32, align 4
  %c = alloca i32, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %c, align 4
  br label %while0

while0:                                           ; preds = %endloop, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while0.then, label %endloop1

while0.then:                                      ; preds = %while0
  br label %while1

while1:                                           ; preds = %while0.then
  br i1 true, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"$c" = load i32, i32* %c, align 4
  %ADD = add i32 %"$c", 1
  store i32 %ADD, i32* %c, align 4
  br label %endloop

endloop:                                          ; preds = %while1.then, %while1
  %"$i2" = load i32, i32* %i, align 4
  %ADD3 = add i32 %"$i2", 1
  store i32 %ADD3, i32* %i, align 4
  br label %while0

endloop1:                                         ; preds = %while0
  %"$c4" = load i32, i32* %c, align 4
  ret i32 %"$c4"
}
```

---

## 050 — while: get crazy

```ura
fn main() i32:
    i i32 = 0
    c i32 = 0
    while i < 3:
        j i32 = 0
        while j < 4:
            c = c + 1
            j = j + 1
        i = i + 1
    return c
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %i = alloca i32, align 4
  %c = alloca i32, align 4
  %j = alloca i32, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %c, align 4
  br label %while0

while0:                                           ; preds = %endloop, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while0.then, label %endloop1

while0.then:                                      ; preds = %while0
  store i32 0, i32* %j, align 4
  br label %while1

while1:                                           ; preds = %while1.then, %while0.then
  %"$j" = load i32, i32* %j, align 4
  %LT2 = icmp slt i32 %"$j", 4
  br i1 %LT2, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"$c" = load i32, i32* %c, align 4
  %ADD = add i32 %"$c", 1
  store i32 %ADD, i32* %c, align 4
  %"$j3" = load i32, i32* %j, align 4
  %ADD4 = add i32 %"$j3", 1
  store i32 %ADD4, i32* %j, align 4
  br label %while1

endloop:                                          ; preds = %while1
  %"$i5" = load i32, i32* %i, align 4
  %ADD6 = add i32 %"$i5", 1
  store i32 %ADD6, i32* %i, align 4
  br label %while0

endloop1:                                         ; preds = %while0
  %"$c7" = load i32, i32* %c, align 4
  ret i32 %"$c7"
}
```

---

## 051 — function: parameters and calls

```ura
// expect: 25
fn square(x i32) i32:
    return x * x

fn add(a i32, b i32) i32:
    return a + b

fn main() i32:
    return add(square(3), square(4))
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @square(i32 %0) {
entry:
  %x = alloca i32, align 4
  store i32 %0, i32* %x, align 4
  %"$x" = load i32, i32* %x, align 4
  %"$x1" = load i32, i32* %x, align 4
  %MUL = mul i32 %"$x", %"$x1"
  ret i32 %MUL
}

define i32 @add(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  store i32 %1, i32* %b, align 4
  %"$a" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %ADD = add i32 %"$a", %"$b"
  ret i32 %ADD
}

define i32 @main() {
entry:
  %call.square = call i32 @square(i32 3)
  %call.square1 = call i32 @square(i32 4)
  %call.add = call i32 @add(i32 %call.square, i32 %call.square1)
  ret i32 %call.add
}
```

---

## 052 — function: call before the definition

```ura
// expect: 18
fn main() i32:
    return later(2)

fn later(n i32) i32:
    return n * 9
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %call.later = call i32 @later(i32 2)
  ret i32 %call.later
}

define i32 @later(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %MUL = mul i32 %"$n", 9
  ret i32 %MUL
}
```

---

## 053 — function: nested calls

```ura
// expect: 3
fn inc(x i32) i32:
    return x + 1

fn main() i32:
    return inc(inc(inc(0)))
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @inc(i32 %0) {
entry:
  %x = alloca i32, align 4
  store i32 %0, i32* %x, align 4
  %"$x" = load i32, i32* %x, align 4
  %ADD = add i32 %"$x", 1
  ret i32 %ADD
}

define i32 @main() {
entry:
  %call.inc = call i32 @inc(i32 0)
  %call.inc1 = call i32 @inc(i32 %call.inc)
  %call.inc2 = call i32 @inc(i32 %call.inc1)
  ret i32 %call.inc2
}
```

---

## 054 — function: a parameter is a copy

```ura
// expect: 56
fn bump(x i32) i32:
    x += 1
    return x

fn main() i32:
    a i32 = 5
    b i32 = bump(a)
    return a * 10 + b
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @bump(i32 %0) {
entry:
  %x = alloca i32, align 4
  store i32 %0, i32* %x, align 4
  %"$x" = load i32, i32* %x, align 4
  %ADD = add i32 %"$x", 1
  store i32 %ADD, i32* %x, align 4
  %"$x1" = load i32, i32* %x, align 4
  ret i32 %"$x1"
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 5, i32* %a, align 4
  %"$a" = load i32, i32* %a, align 4
  %call.bump = call i32 @bump(i32 %"$a")
  store i32 %call.bump, i32* %b, align 4
  %"$a1" = load i32, i32* %a, align 4
  %MUL = mul i32 %"$a1", 10
  %"$b" = load i32, i32* %b, align 4
  %ADD = add i32 %MUL, %"$b"
  ret i32 %ADD
}
```

---

## 055 — function: call result in a variable

```ura
// expect: 14
fn twice(n i32) i32:
    return n * 2

fn main() i32:
    a i32 = twice(3)
    b i32 = twice(a) + 2
    return b
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @twice(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %MUL = mul i32 %"$n", 2
  ret i32 %MUL
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %call.twice = call i32 @twice(i32 3)
  store i32 %call.twice, i32* %a, align 4
  %"$a" = load i32, i32* %a, align 4
  %call.twice1 = call i32 @twice(i32 %"$a")
  %ADD = add i32 %call.twice1, 2
  store i32 %ADD, i32* %b, align 4
  %"$b" = load i32, i32* %b, align 4
  ret i32 %"$b"
}
```

---

## 056 — function: void function and bare return

```ura
// expect: 4
fn nothing():
    return

fn main() i32:
    nothing()
    return 4
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define void @nothing() {
entry:
  ret void
}

define i32 @main() {
entry:
  call void @nothing()
  ret i32 4
}
```

---

## 057 — function: function inside a function

```ura
// expect: 7
fn main() i32:
    fn help() i32:
        return 7
    return help()
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %call.help = call i32 @help()
  ret i32 %call.help
}

define i32 @help() {
entry:
  ret i32 7
}
```

---

## 058 — function: calling function before declaration

```ura
fn main() i32:
    return later()

fn later() i32:
    return 9
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %call.later = call i32 @later()
  ret i32 %call.later
}

define i32 @later() {
entry:
  ret i32 9
}
```

---

## 059 — function: math op on functions

```ura
fn square(x i32) i32:
    return x * x

fn main() i32:
    return square(3) + square(4)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @square(i32 %0) {
entry:
  %x = alloca i32, align 4
  store i32 %0, i32* %x, align 4
  %"$x" = load i32, i32* %x, align 4
  %"$x1" = load i32, i32* %x, align 4
  %MUL = mul i32 %"$x", %"$x1"
  ret i32 %MUL
}

define i32 @main() {
entry:
  %call.square = call i32 @square(i32 3)
  %call.square1 = call i32 @square(i32 4)
  %ADD = add i32 %call.square, %call.square1
  ret i32 %ADD
}
```

---

## 060 — function: bare return in a void function

```ura
fn foo(a i32):
    return

fn foo(a bool):
    return

fn main() i32:
    foo(1)
    foo(True)
    return 7
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define void @foo(i32 %0) {
entry:
  %a = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  ret void
}

define void @foo.1(i1 %0) {
entry:
  %a = alloca i1, align 1
  store i1 %0, i1* %a, align 1
  ret void
}

define i32 @main() {
entry:
  call void @foo(i32 1)
  call void @foo.1(i1 true)
  ret i32 7
}
```

---

## 061 — proto: printf with a number

```ura
// stdout: hello 42
// expect: 0
proto printf(fmt i8[], ...) i32

fn main() i32:
    printf("hello %d\n", 42)
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [10 x i8] c"hello %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 42)
  ret i32 0
}
```

---

## 062 — proto: several extra arguments

```ura
// stdout: 1 + 2 = 3
// expect: 0
proto printf(fmt i8[], ...) i32

fn main() i32:
    a i32 = 1
    b i32 = 2
    printf("%d + %d = %d\n", a, b, a + b)
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [14 x i8] c"%d + %d = %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 1, i32* %a, align 4
  store i32 2, i32* %b, align 4
  %"$a" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %ADD = add i32 %"$a", %"$b"
  %"$a1" = load i32, i32* %a, align 4
  %"$b2" = load i32, i32* %b, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str, i32 0, i32 0), i32 %"$a1", i32 %"$b2", i32 %ADD)
  ret i32 0
}
```

---

## 063 — proto: exit from C

```ura
// expect: 10
proto exit(code i32)

fn main() i32:
    exit(10)
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

declare void @exit(i32)

define i32 @main() {
entry:
  call void @exit(i32 10)
  ret i32 0
}
```

---

## 064 — proto: escapes

```ura
// stdout: a	b (a real tab), then: "quoted" \ done
// expect: 0
proto printf(fmt i8[], ...) i32

fn main() i32:
    printf("a\tb\n")
    printf("\"quoted\" \\ done\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [5 x i8] c"a\09b\0A\00", align 1
@str.1 = private unnamed_addr constant [17 x i8] c"\22quoted\22 \\ done\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0))
  %call.printf1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}
```

---

## 065 — proto: printf returns how many chars it wrote

```ura
// stdout: abc
// expect: 3
proto printf(fmt i8[], ...) i32

fn main() i32:
    return printf("abc")
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"abc\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret i32 %call.printf
}
```

---

## 066 — proto: printf in a loop

```ura
// stdout: 0 1 2
// expect: 0
proto printf(fmt i8[], ...) i32

fn main() i32:
    i i32 = 0
    while i < 3:
        printf("%d", i)
        if i < 2:
            printf(" ")
        i += 1
    printf("\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %endif, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$i1" = load i32, i32* %i, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i32 %"$i1")
  br label %if1

if1:                                              ; preds = %while0.then
  %"$i2" = load i32, i32* %i, align 4
  %LT3 = icmp slt i32 %"$i2", 2
  br i1 %LT3, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  %call.printf4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  br label %endif

endif:                                            ; preds = %if1.then, %if1
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %call.printf6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  ret i32 0
}
```

---

## 067 — proto: proto function

```ura
proto exit(code i32)

fn main() i32:
    exit(10)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

declare void @exit(i32)

define i32 @main() {
entry:
  call void @exit(i32 10)
  ret i32 0
}
```

---

## 068 — proto: escaped characters

```ura
proto printf(s i8[], ...)

fn main() i32:
    printf("tab\there\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [10 x i8] c"tab\09here\0A\00", align 1

declare void @printf(i8*, ...)

define i32 @main() {
entry:
  call void (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0))
  ret i32 0
}
```

---

## 069 — global scope: a global variable changed by a function

```ura
// expect: 4
count i32 = 0

fn tick():
    count += 1

fn main() i32:
    i i32 = 0
    while i < 4:
        tick()
        i += 1
    return count
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@count = global i32 0

define void @tick() {
entry:
  %"$count" = load i32, i32* @count, align 4
  %ADD = add i32 %"$count", 1
  store i32 %ADD, i32* @count, align 4
  ret void
}

define i32 @main() {
entry:
  %i = alloca i32, align 4
  store i32 0, i32* @count, align 4
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 4
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  call void @tick()
  %"$i1" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i1", 1
  store i32 %ADD, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$count" = load i32, i32* @count, align 4
  ret i32 %"$count"
}
```

---

## 070 — crazy: mutual recursion, used before it's declared

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
target triple = "x86_64-unknown-linux-gnu"

define i1 @is_even(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %"$n", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i1 true

endif:                                            ; preds = %if0
  %"$n1" = load i32, i32* %n, align 4
  %SUB = sub i32 %"$n1", 1
  %call.is_odd = call i1 @is_odd(i32 %SUB)
  ret i1 %call.is_odd
}

define i1 @is_odd(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  br label %if1

if1:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %"$n", 0
  br i1 %EQ, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  ret i1 false

endif:                                            ; preds = %if1
  %"$n1" = load i32, i32* %n, align 4
  %SUB = sub i32 %"$n1", 1
  %call.is_even = call i1 @is_even(i32 %SUB)
  ret i1 %call.is_even
}

define i32 @main() {
entry:
  %r = alloca i32, align 4
  store i32 0, i32* %r, align 4
  br label %if2

if2:                                              ; preds = %entry
  %call.is_even = call i1 @is_even(i32 10)
  br i1 %call.is_even, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"$r" = load i32, i32* %r, align 4
  %ADD = add i32 %"$r", 1
  store i32 %ADD, i32* %r, align 4
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  br label %if3

if3:                                              ; preds = %endif
  %call.is_odd = call i1 @is_odd(i32 7)
  br i1 %call.is_odd, label %if3.then, label %endif1

if3.then:                                         ; preds = %if3
  %"$r3" = load i32, i32* %r, align 4
  %ADD4 = add i32 %"$r3", 2
  store i32 %ADD4, i32* %r, align 4
  br label %endif1

endif1:                                           ; preds = %if3.then, %if3
  br label %if4

if4:                                              ; preds = %endif1
  %call.is_even5 = call i1 @is_even(i32 3)
  br i1 %call.is_even5, label %if4.then, label %endif2

if4.then:                                         ; preds = %if4
  %"$r6" = load i32, i32* %r, align 4
  %ADD7 = add i32 %"$r6", 4
  store i32 %ADD7, i32* %r, align 4
  br label %endif2

endif2:                                           ; preds = %if4.then, %if4
  %"$r8" = load i32, i32* %r, align 4
  ret i32 %"$r8"
}
```

---

## 071 — crazy: gcd, printf with many arguments

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
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [10 x i8] c"%d %d %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @gcd(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  store i32 %1, i32* %b, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$b" = load i32, i32* %b, align 4
  %EQ = icmp eq i32 %"$b", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$a" = load i32, i32* %a, align 4
  ret i32 %"$a"

endif:                                            ; preds = %if0
  %"$a1" = load i32, i32* %a, align 4
  %"$b2" = load i32, i32* %b, align 4
  %MOD = srem i32 %"$a1", %"$b2"
  %"$b3" = load i32, i32* %b, align 4
  %call.gcd = call i32 @gcd(i32 %"$b3", i32 %MOD)
  ret i32 %call.gcd
}

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  %call.gcd = call i32 @gcd(i32 84, i32 36)
  store i32 %call.gcd, i32* %x, align 4
  %call.gcd1 = call i32 @gcd(i32 17, i32 5)
  store i32 %call.gcd1, i32* %y, align 4
  %"$x" = load i32, i32* %x, align 4
  %call.gcd2 = call i32 @gcd(i32 %"$x", i32 4)
  %"$x3" = load i32, i32* %x, align 4
  %"$y" = load i32, i32* %y, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %"$x3", i32 %"$y", i32 %call.gcd2)
  %"$x4" = load i32, i32* %x, align 4
  %"$y5" = load i32, i32* %y, align 4
  %ADD = add i32 %"$x4", %"$y5"
  ret i32 %ADD
}
```

---

## 072 — crazy: collatz: a parameter changed in a loop

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
target triple = "x86_64-unknown-linux-gnu"

define i32 @steps(i32 %0) {
entry:
  %n = alloca i32, align 4
  %c = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  store i32 0, i32* %c, align 4
  br label %while0

while0:                                           ; preds = %endif, %entry
  %"$n" = load i32, i32* %n, align 4
  %NQ = icmp ne i32 %"$n", 1
  br i1 %NQ, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  br label %if1

if1:                                              ; preds = %while0.then
  %"$n1" = load i32, i32* %n, align 4
  %MOD = srem i32 %"$n1", 2
  %EQ = icmp eq i32 %MOD, 0
  br i1 %EQ, label %if1.then, label %else2

if1.then:                                         ; preds = %if1
  %"$n2" = load i32, i32* %n, align 4
  %DIV = sdiv i32 %"$n2", 2
  store i32 %DIV, i32* %n, align 4
  br label %endif

else2:                                            ; preds = %if1
  %"$n3" = load i32, i32* %n, align 4
  %MUL = mul i32 %"$n3", 3
  %ADD = add i32 %MUL, 1
  store i32 %ADD, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %else2, %if1.then
  %"$c" = load i32, i32* %c, align 4
  %ADD4 = add i32 %"$c", 1
  store i32 %ADD4, i32* %c, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$c5" = load i32, i32* %c, align 4
  ret i32 %"$c5"
}

define i32 @main() {
entry:
  %call.steps = call i32 @steps(i32 27)
  ret i32 %call.steps
}
```

---

## 073 — crazy: the 20th prime: continue, break, a bool function

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
target triple = "x86_64-unknown-linux-gnu"

define i1 @is_prime(i32 %0) {
entry:
  %n = alloca i32, align 4
  %d = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %LT = icmp slt i32 %"$n", 2
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i1 false

endif:                                            ; preds = %if0
  store i32 2, i32* %d, align 4
  br label %while1

while1:                                           ; preds = %endif1, %endif
  %"$d" = load i32, i32* %d, align 4
  %"$d2" = load i32, i32* %d, align 4
  %MUL = mul i32 %"$d", %"$d2"
  %"$n3" = load i32, i32* %n, align 4
  %LE = icmp sle i32 %MUL, %"$n3"
  br i1 %LE, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  br label %if2

if2:                                              ; preds = %while1.then
  %"$n4" = load i32, i32* %n, align 4
  %"$d5" = load i32, i32* %d, align 4
  %MOD = srem i32 %"$n4", %"$d5"
  %EQ = icmp eq i32 %MOD, 0
  br i1 %EQ, label %if2.then, label %endif1

if2.then:                                         ; preds = %if2
  ret i1 false

endif1:                                           ; preds = %if2
  %"$d6" = load i32, i32* %d, align 4
  %ADD = add i32 %"$d6", 1
  store i32 %ADD, i32* %d, align 4
  br label %while1

endloop:                                          ; preds = %while1
  ret i1 true
}

define i32 @main() {
entry:
  %count = alloca i32, align 4
  %n = alloca i32, align 4
  store i32 0, i32* %count, align 4
  store i32 0, i32* %n, align 4
  br label %while3

while3:                                           ; preds = %endif1, %if4.then, %entry
  %"$n" = load i32, i32* %n, align 4
  %LT = icmp slt i32 %"$n", 100
  br i1 %LT, label %while3.then, label %endloop

while3.then:                                      ; preds = %while3
  %"$n2" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n2", 1
  store i32 %ADD, i32* %n, align 4
  br label %if4

if4:                                              ; preds = %while3.then
  %"$n3" = load i32, i32* %n, align 4
  %call.is_prime = call i1 @is_prime(i32 %"$n3")
  %EQ = icmp eq i1 %call.is_prime, false
  br i1 %EQ, label %if4.then, label %endif

if4.then:                                         ; preds = %if4
  br label %while3

endif:                                            ; preds = %if4
  %"$count" = load i32, i32* %count, align 4
  %ADD4 = add i32 %"$count", 1
  store i32 %ADD4, i32* %count, align 4
  br label %if5

if5:                                              ; preds = %endif
  %"$count5" = load i32, i32* %count, align 4
  %EQ6 = icmp eq i32 %"$count5", 20
  br i1 %EQ6, label %if5.then, label %endif1

if5.then:                                         ; preds = %if5
  br label %endloop

endif1:                                           ; preds = %if5
  br label %while3

endloop:                                          ; preds = %if5.then, %while3
  %"$n7" = load i32, i32* %n, align 4
  ret i32 %"$n7"
}
```

---

## 074 — crazy: return from two nested loops

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
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  store i32 1, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %endloop, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 20
  br i1 %LT, label %while0.then, label %endloop1

while0.then:                                      ; preds = %while0
  %"$i2" = load i32, i32* %i, align 4
  store i32 %"$i2", i32* %j, align 4
  br label %while1

while1:                                           ; preds = %endif, %while0.then
  %"$j" = load i32, i32* %j, align 4
  %LT3 = icmp slt i32 %"$j", 20
  br i1 %LT3, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  br label %if2

if2:                                              ; preds = %while1.then
  %"$i4" = load i32, i32* %i, align 4
  %"$j5" = load i32, i32* %j, align 4
  %MUL = mul i32 %"$i4", %"$j5"
  %EQ = icmp eq i32 %MUL, 42
  %"$i6" = load i32, i32* %i, align 4
  %"$j7" = load i32, i32* %j, align 4
  %ADD = add i32 %"$i6", %"$j7"
  %EQ8 = icmp eq i32 %ADD, 13
  %AND = and i1 %EQ, %EQ8
  br i1 %AND, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"$i9" = load i32, i32* %i, align 4
  %MUL10 = mul i32 %"$i9", 10
  %"$j11" = load i32, i32* %j, align 4
  %ADD12 = add i32 %MUL10, %"$j11"
  ret i32 %ADD12

endif:                                            ; preds = %if2
  %"$j13" = load i32, i32* %j, align 4
  %ADD14 = add i32 %"$j13", 1
  store i32 %ADD14, i32* %j, align 4
  br label %while1

endloop:                                          ; preds = %while1
  %"$i15" = load i32, i32* %i, align 4
  %ADD16 = add i32 %"$i15", 1
  store i32 %ADD16, i32* %i, align 4
  br label %while0

endloop1:                                         ; preds = %while0
  ret i32 0
}
```

---

## 075 — crazy: operator precedence soup

```ura
// expect: 217   (a 5, b 0, c 5, d 2: 5*40 + 0*7 + 5*3 + 2)
fn main() i32:
    a i32 = 1 + 2 * 3 - 4 / 2 % 3
    b i32 = (1 + 2) * (3 - 4 / 2) % 3
    c i32 = 10 - 3 - 2
    d i32 = 100 / 10 / 5
    return a * 40 + b * 7 + c * 3 + d
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  %d = alloca i32, align 4
  store i32 5, i32* %a, align 4
  store i32 0, i32* %b, align 4
  store i32 5, i32* %c, align 4
  store i32 2, i32* %d, align 4
  %"$a" = load i32, i32* %a, align 4
  %MUL = mul i32 %"$a", 40
  %"$b" = load i32, i32* %b, align 4
  %MUL1 = mul i32 %"$b", 7
  %ADD = add i32 %MUL, %MUL1
  %"$c" = load i32, i32* %c, align 4
  %MUL2 = mul i32 %"$c", 3
  %ADD3 = add i32 %ADD, %MUL2
  %"$d" = load i32, i32* %d, align 4
  %ADD4 = add i32 %ADD3, %"$d"
  ret i32 %ADD4
}
```

---

## 076 — crazy: negative numbers: signed / and %

```ura
// expect: 98   (-3 - 1 - 3 + 5, plus 100)
fn main() i32:
    a i32 = -7 / 2
    b i32 = -7 % 3
    c i32 = 0 - a * b
    d i32 = -(-5)
    return a + b + c + d + 100
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  %d = alloca i32, align 4
  store i32 -3, i32* %a, align 4
  store i32 -1, i32* %b, align 4
  %"$a" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %MUL = mul i32 %"$a", %"$b"
  %SUB = sub i32 0, %MUL
  store i32 %SUB, i32* %c, align 4
  store i32 5, i32* %d, align 4
  %"$a1" = load i32, i32* %a, align 4
  %"$b2" = load i32, i32* %b, align 4
  %ADD = add i32 %"$a1", %"$b2"
  %"$c" = load i32, i32* %c, align 4
  %ADD3 = add i32 %ADD, %"$c"
  %"$d" = load i32, i32* %d, align 4
  %ADD4 = add i32 %ADD3, %"$d"
  %ADD5 = add i32 %ADD4, 100
  ret i32 %ADD5
}
```

---

## 077 — crazy: ackermann: deep recursion, a call as an argument

```ura
// expect: 61
fn ack(m i32, n i32) i32:
    if m == 0: return n + 1
    if n == 0: return ack(m - 1, 1)
    return ack(m - 1, ack(m, n - 1))

fn main() i32:
    return ack(3, 3)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @ack(i32 %0, i32 %1) {
entry:
  %m = alloca i32, align 4
  %n = alloca i32, align 4
  store i32 %0, i32* %m, align 4
  store i32 %1, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$m" = load i32, i32* %m, align 4
  %EQ = icmp eq i32 %"$m", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 1
  ret i32 %ADD

endif:                                            ; preds = %if0
  br label %if1

if1:                                              ; preds = %endif
  %"$n2" = load i32, i32* %n, align 4
  %EQ3 = icmp eq i32 %"$n2", 0
  br i1 %EQ3, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  %"$m4" = load i32, i32* %m, align 4
  %SUB = sub i32 %"$m4", 1
  %call.ack = call i32 @ack(i32 %SUB, i32 1)
  ret i32 %call.ack

endif1:                                           ; preds = %if1
  %"$m5" = load i32, i32* %m, align 4
  %SUB6 = sub i32 %"$m5", 1
  %"$n7" = load i32, i32* %n, align 4
  %SUB8 = sub i32 %"$n7", 1
  %"$m9" = load i32, i32* %m, align 4
  %call.ack10 = call i32 @ack(i32 %"$m9", i32 %SUB8)
  %call.ack11 = call i32 @ack(i32 %SUB6, i32 %call.ack10)
  ret i32 %call.ack11
}

define i32 @main() {
entry:
  %call.ack = call i32 @ack(i32 3, i32 3)
  ret i32 %call.ack
}
```

---

## 078 — crazy: one name declared in sibling blocks

```ura
// expect: 130   (10 + 20 + 0 + 100)
fn main() i32:
    x i32 = 1
    r i32 = 0
    if x == 1:
        y i32 = 10
        r += y
    if x == 1:
        y i32 = 20
        r += y
    i i32 = 0
    while i < 2:
        z i32 = i * 100
        r += z
        i += 1
    return r
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %r = alloca i32, align 4
  %y = alloca i32, align 4
  %y4 = alloca i32, align 4
  %i = alloca i32, align 4
  %z = alloca i32, align 4
  store i32 1, i32* %x, align 4
  store i32 0, i32* %r, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$x" = load i32, i32* %x, align 4
  %EQ = icmp eq i32 %"$x", 1
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  store i32 10, i32* %y, align 4
  %"$y" = load i32, i32* %y, align 4
  %"$r" = load i32, i32* %r, align 4
  %ADD = add i32 %"$r", %"$y"
  store i32 %ADD, i32* %r, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"$x2" = load i32, i32* %x, align 4
  %EQ3 = icmp eq i32 %"$x2", 1
  br i1 %EQ3, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  store i32 20, i32* %y4, align 4
  %"$y5" = load i32, i32* %y4, align 4
  %"$r6" = load i32, i32* %r, align 4
  %ADD7 = add i32 %"$r6", %"$y5"
  store i32 %ADD7, i32* %r, align 4
  br label %endif1

endif1:                                           ; preds = %if1.then, %if1
  store i32 0, i32* %i, align 4
  br label %while2

while2:                                           ; preds = %while2.then, %endif1
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 2
  br i1 %LT, label %while2.then, label %endloop

while2.then:                                      ; preds = %while2
  %"$i8" = load i32, i32* %i, align 4
  %MUL = mul i32 %"$i8", 100
  store i32 %MUL, i32* %z, align 4
  %"$z" = load i32, i32* %z, align 4
  %"$r9" = load i32, i32* %r, align 4
  %ADD10 = add i32 %"$r9", %"$z"
  store i32 %ADD10, i32* %r, align 4
  %"$i11" = load i32, i32* %i, align 4
  %ADD12 = add i32 %"$i11", 1
  store i32 %ADD12, i32* %i, align 4
  br label %while2

endloop:                                          ; preds = %while2
  %"$r13" = load i32, i32* %r, align 4
  ret i32 %"$r13"
}
```

---

## 079 — crazy: script: a global counts recursive calls

```ura
// stdout: 55 177
// expect: 177
proto printf(fmt i8[], ...) i32

calls i32 = 0

fn fib(n i32) i32:
    calls += 1
    if n < 2: return n
    return fib(n - 1) + fib(n - 2)

r i32 = fib(10)
printf("%d %d\n", r, calls)
return calls
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@calls = global i32 0
@r = global i32 0
@str = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @fib(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %"$calls" = load i32, i32* @calls, align 4
  %ADD = add i32 %"$calls", 1
  store i32 %ADD, i32* @calls, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %LT = icmp slt i32 %"$n", 2
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$n1" = load i32, i32* %n, align 4
  ret i32 %"$n1"

endif:                                            ; preds = %if0
  %"$n2" = load i32, i32* %n, align 4
  %SUB = sub i32 %"$n2", 1
  %call.fib = call i32 @fib(i32 %SUB)
  %"$n3" = load i32, i32* %n, align 4
  %SUB4 = sub i32 %"$n3", 2
  %call.fib5 = call i32 @fib(i32 %SUB4)
  %ADD6 = add i32 %call.fib, %call.fib5
  ret i32 %ADD6
}

define i32 @main() {
entry:
  store i32 0, i32* @calls, align 4
  %call.fib = call i32 @fib(i32 10)
  store i32 %call.fib, i32* @r, align 4
  %"$r" = load i32, i32* @r, align 4
  %"$calls" = load i32, i32* @calls, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %"$r", i32 %"$calls")
  %"$calls1" = load i32, i32* @calls, align 4
  ret i32 %"$calls1"
}
```

---

## 080 — crazy: script: nested loops with continue and break at file level

```ura
// expect: 37
found i32 = -1
i i32 = 0
while i < 10:
    i += 1
    if i % 2 == 0: continue
    j i32 = 0
    while j < 10:
        if i * j == 21:
            found = i * 10 + j
            break
        j += 1
    if found != -1: break
return found
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@found = global i32 0
@i = global i32 0

define i32 @main() {
entry:
  %j = alloca i32, align 4
  store i32 -1, i32* @found, align 4
  store i32 0, i32* @i, align 4
  br label %while0

while0:                                           ; preds = %endif2, %if1.then, %entry
  %"$i" = load i32, i32* @i, align 4
  %LT = icmp slt i32 %"$i", 10
  br i1 %LT, label %while0.then, label %endloop3

while0.then:                                      ; preds = %while0
  %"$i4" = load i32, i32* @i, align 4
  %ADD = add i32 %"$i4", 1
  store i32 %ADD, i32* @i, align 4
  br label %if1

if1:                                              ; preds = %while0.then
  %"$i5" = load i32, i32* @i, align 4
  %MOD = srem i32 %"$i5", 2
  %EQ = icmp eq i32 %MOD, 0
  br i1 %EQ, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  br label %while0

endif:                                            ; preds = %if1
  store i32 0, i32* %j, align 4
  br label %while2

while2:                                           ; preds = %endif1, %endif
  %"$j" = load i32, i32* %j, align 4
  %LT6 = icmp slt i32 %"$j", 10
  br i1 %LT6, label %while2.then, label %endloop

while2.then:                                      ; preds = %while2
  br label %if3

if3:                                              ; preds = %while2.then
  %"$i7" = load i32, i32* @i, align 4
  %"$j8" = load i32, i32* %j, align 4
  %MUL = mul i32 %"$i7", %"$j8"
  %EQ9 = icmp eq i32 %MUL, 21
  br i1 %EQ9, label %if3.then, label %endif1

if3.then:                                         ; preds = %if3
  %"$i10" = load i32, i32* @i, align 4
  %MUL11 = mul i32 %"$i10", 10
  %"$j12" = load i32, i32* %j, align 4
  %ADD13 = add i32 %MUL11, %"$j12"
  store i32 %ADD13, i32* @found, align 4
  br label %endloop

endif1:                                           ; preds = %if3
  %"$j14" = load i32, i32* %j, align 4
  %ADD15 = add i32 %"$j14", 1
  store i32 %ADD15, i32* %j, align 4
  br label %while2

endloop:                                          ; preds = %if3.then, %while2
  br label %if4

if4:                                              ; preds = %endloop
  %"$found" = load i32, i32* @found, align 4
  %NQ = icmp ne i32 %"$found", -1
  br i1 %NQ, label %if4.then, label %endif2

if4.then:                                         ; preds = %if4
  br label %endloop3

endif2:                                           ; preds = %if4
  br label %while0

endloop3:                                         ; preds = %if4.then, %while0
  %"$found16" = load i32, i32* @found, align 4
  ret i32 %"$found16"
}
```

