# milestone1

## index

- 001 — variables: declare with a value
- 002 — variables: reassign
- 003 — variables: use another variable
- 004 — variables: compound assignment
- 005 — variables: declarations without a value start at zero
- 006 — bool: a bool variable
- 007 — bool: False skips the branch
- 008 — bool: a comparison stored in a bool, reassigned
- 009 — bool: bool parameters and return values
- 010 — bool: or between bool variables
- 011 — bool: a bool as a while condition
- 012 — bool: a bool default value is False
- 013 — char: print chars
- 014 — char: compare chars
- 015 — char: order of chars
- 016 — char: reassign a char
- 017 — char: char parameter and return value
- 018 — char: a char decides a loop
- 019 — char: default value of a char is '\0'
- 020 — if: if / elif / else chain
- 021 — if: every comparison
- 022 — if: and / or
- 023 — if: body on the same line
- 024 — if: nested ifs
- 025 — if: no branch taken: code after the if runs
- 026 — if: recursion
- 027 — if: a variable declared in a block
- 028 — while: count to 10
- 029 — while: break
- 030 — while: continue
- 031 — while: nested loops
- 032 — while: break leaves only the inner loop
- 033 — while: continue in the inner loop
- 034 — while: return from inside a loop
- 035 — while: loop that never runs
- 036 — while: loop in a function, called from main
- 037 — functions: parameters and calls
- 038 — functions: call before the definition
- 039 — functions: nested calls
- 040 — functions: a parameter is a copy
- 041 — functions: call result in a variable
- 042 — functions: void function and bare return
- 043 — functions: function inside a function
- 044 — functions: math op on functions
- 045 — proto: printf with a number
- 046 — proto: several extra arguments
- 047 — proto: exit from C
- 048 — proto: escapes
- 049 — proto: printf returns how many chars it wrote
- 050 — proto: printf in a loop
- 051 — global scope: a global variable changed by a function
- 052 — global scope: script: a global counts recursive calls
- 053 — global scope: script: nested loops with continue and break at file level
- 054 — program: mutual recursion, used before it's declared
- 055 — program: gcd, printf with many arguments
- 056 — program: collatz: a parameter changed in a loop
- 057 — program: the 20th prime: continue, break, a bool function
- 058 — program: return from two nested loops
- 059 — program: operator precedence soup
- 060 — program: negative numbers: signed / and %
- 061 — program: ackermann: deep recursion, a call as an argument
- 062 — program: one name declared in sibling blocks
- 063 — tokenizer error: unknown character
- 064 — tokenizer error: unclosed string
- 065 — tokenizer error: unclosed character
- 066 — tokenizer error: character with two letters
- 067 — tokenizer error: unknown escape
- 068 — tokenizer error: unclosed comment
- 069 — tokenizer error: number too big for i32
- 070 — tokenizer error: indentation that is not a multiple of 4
- 071 — tokenizer error: two errors in one file: both are shown
- 072 — parser error: missing : after a function header
- 073 — parser error: missing ( after a function name
- 074 — parser error: missing ) after the parameters
- 075 — parser error: a function with no name
- 076 — parser error: a parameter with no type
- 077 — parser error: missing : after an if condition
- 078 — parser error: missing : after a while condition
- 079 — parser error: else with a condition
- 080 — parser error: elif without an if
- 081 — parser error: unclosed (
- 082 — parser error: nothing after =
- 083 — parser error: nothing after an operator
- 084 — parser error: unclosed ( in a call
- 085 — name error: undeclared variable
- 086 — name error: a typo in a variable name
- 087 — name error: a variable used before its declaration
- 088 — name error: a variable declared twice
- 089 — name error: unknown type
- 090 — name error: undeclared function
- 091 — name error: a typo in a function name
- 092 — name error: wrong number of arguments
- 093 — name error: a function declared twice
- 094 — name error: break outside a loop
- 095 — name error: continue outside a loop
- 096 — name error: calling a variable

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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 10, i32* %a, align 4
  %load.a = load i32, i32* %a, align 4
  ret i32 %load.a
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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 10, i32* %a, align 4
  store i32 20, i32* %a, align 4
  %load.a = load i32, i32* %a, align 4
  ret i32 %load.a
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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 10, i32* %a, align 4
  %load.a = load i32, i32* %a, align 4
  %MUL = mul i32 %load.a, 2
  store i32 %MUL, i32* %b, align 4
  %load.a1 = load i32, i32* %a, align 4
  %load.b = load i32, i32* %b, align 4
  %ADD = add i32 %load.a1, %load.b
  store i32 %ADD, i32* %a, align 4
  %load.a2 = load i32, i32* %a, align 4
  ret i32 %load.a2
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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 10, i32* %a, align 4
  %load.a = load i32, i32* %a, align 4
  %MUL = mul i32 %load.a, 2
  store i32 %MUL, i32* %b, align 4
  %load.a1 = load i32, i32* %a, align 4
  %load.b = load i32, i32* %b, align 4
  %ADD = add i32 %load.a1, %load.b
  store i32 %ADD, i32* %a, align 4
  %old = load i32, i32* %a, align 4
  %ADD_ASSIGN = add i32 %old, 5
  store i32 %ADD_ASSIGN, i32* %a, align 4
  %old2 = load i32, i32* %a, align 4
  %SUB_ASSIGN = sub i32 %old2, 3
  store i32 %SUB_ASSIGN, i32* %a, align 4
  %old3 = load i32, i32* %a, align 4
  %MUL_ASSIGN = mul i32 %old3, 2
  store i32 %MUL_ASSIGN, i32* %a, align 4
  %old4 = load i32, i32* %a, align 4
  %DIV_ASSIGN = sdiv i32 %old4, 4
  store i32 %DIV_ASSIGN, i32* %a, align 4
  %old5 = load i32, i32* %a, align 4
  %MOD_ASSIGN = srem i32 %old5, 5
  store i32 %MOD_ASSIGN, i32* %a, align 4
  %load.a6 = load i32, i32* %a, align 4
  %load.b7 = load i32, i32* %b, align 4
  %ADD8 = add i32 %load.a6, %load.b7
  ret i32 %ADD8
}
```

---

## 005 — variables: declarations without a value start at zero

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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %b = alloca i1, align 1
  %c = alloca i8, align 1
  store i32 0, i32* %x, align 4
  store i1 false, i1* %b, align 1
  store i8 0, i8* %c, align 1
  %load.b = load i1, i1* %b, align 1
  %load.c = load i8, i8* %c, align 1
  %NQ = icmp ne i8 %load.c, 0
  %OR = or i1 %load.b, %NQ
  br i1 %OR, label %then, label %endif

then:                                             ; preds = %entry
  ret i32 1

endif:                                            ; preds = %entry
  %load.x = load i32, i32* %x, align 4
  ret i32 %load.x
}
```

---

## 006 — bool: a bool variable

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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %flag = alloca i1, align 1
  store i1 true, i1* %flag, align 1
  %load.flag = load i1, i1* %flag, align 1
  br i1 %load.flag, label %then, label %endif

then:                                             ; preds = %entry
  ret i32 1

endif:                                            ; preds = %entry
  ret i32 0
}
```

---

## 007 — bool: False skips the branch

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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %flag = alloca i1, align 1
  store i1 false, i1* %flag, align 1
  %load.flag = load i1, i1* %flag, align 1
  br i1 %load.flag, label %then, label %endif

then:                                             ; preds = %entry
  ret i32 1

endif:                                            ; preds = %entry
  ret i32 0
}
```

---

## 008 — bool: a comparison stored in a bool, reassigned

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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %big = alloca i1, align 1
  %n = alloca i32, align 4
  store i32 5, i32* %a, align 4
  %load.a = load i32, i32* %a, align 4
  %GT = icmp sgt i32 %load.a, 3
  store i1 %GT, i1* %big, align 1
  store i32 0, i32* %n, align 4
  %load.big = load i1, i1* %big, align 1
  br i1 %load.big, label %then, label %endif

then:                                             ; preds = %entry
  %old = load i32, i32* %n, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %load.a1 = load i32, i32* %a, align 4
  %GT2 = icmp sgt i32 %load.a1, 9
  store i1 %GT2, i1* %big, align 1
  %load.big5 = load i1, i1* %big, align 1
  br i1 %load.big5, label %then4, label %endif3

then4:                                            ; preds = %endif
  %old6 = load i32, i32* %n, align 4
  %ADD_ASSIGN7 = add i32 %old6, 10
  store i32 %ADD_ASSIGN7, i32* %n, align 4
  br label %endif3

endif3:                                           ; preds = %then4, %endif
  store i1 false, i1* %big, align 1
  %load.big10 = load i1, i1* %big, align 1
  br i1 %load.big10, label %then9, label %endif8

then9:                                            ; preds = %endif3
  %old11 = load i32, i32* %n, align 4
  %ADD_ASSIGN12 = add i32 %old11, 100
  store i32 %ADD_ASSIGN12, i32* %n, align 4
  br label %endif8

endif8:                                           ; preds = %then9, %endif3
  %load.big15 = load i1, i1* %big, align 1
  %EQ = icmp eq i1 %load.big15, false
  br i1 %EQ, label %then14, label %endif13

then14:                                           ; preds = %endif8
  %old16 = load i32, i32* %n, align 4
  %ADD_ASSIGN17 = add i32 %old16, 1
  store i32 %ADD_ASSIGN17, i32* %n, align 4
  br label %endif13

endif13:                                          ; preds = %then14, %endif8
  %load.n = load i32, i32* %n, align 4
  ret i32 %load.n
}
```

---

## 009 — bool: bool parameters and return values

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
target triple = "x86_64-pc-linux-gnu"

define i1 @is_even(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %load.n = load i32, i32* %n, align 4
  %MOD = srem i32 %load.n, 2
  %EQ = icmp eq i32 %MOD, 0
  ret i1 %EQ
}

define i1 @both(i1 %0, i1 %1) {
entry:
  %a = alloca i1, align 1
  %b = alloca i1, align 1
  store i1 %0, i1* %a, align 1
  store i1 %1, i1* %b, align 1
  %load.a = load i1, i1* %a, align 1
  %load.b = load i1, i1* %b, align 1
  %AND = and i1 %load.a, %load.b
  ret i1 %AND
}

define i32 @main() {
entry:
  %n = alloca i32, align 4
  store i32 0, i32* %n, align 4
  %"is_even(4)" = call i1 @is_even(i32 4)
  br i1 %"is_even(4)", label %then, label %endif

then:                                             ; preds = %entry
  %old = load i32, i32* %n, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %"is_even(2)" = call i1 @is_even(i32 2)
  %"is_even(6)" = call i1 @is_even(i32 6)
  %"both(is_even(2),is_even(6))" = call i1 @both(i1 %"is_even(2)", i1 %"is_even(6)")
  br i1 %"both(is_even(2),is_even(6))", label %then2, label %endif1

then2:                                            ; preds = %endif
  %old3 = load i32, i32* %n, align 4
  %ADD_ASSIGN4 = add i32 %old3, 2
  store i32 %ADD_ASSIGN4, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %then2, %endif
  %"is_even(2)7" = call i1 @is_even(i32 2)
  %"is_even(7)" = call i1 @is_even(i32 7)
  %"both(is_even(2),is_even(7))" = call i1 @both(i1 %"is_even(2)7", i1 %"is_even(7)")
  br i1 %"both(is_even(2),is_even(7))", label %then6, label %endif5

then6:                                            ; preds = %endif1
  %old8 = load i32, i32* %n, align 4
  %ADD_ASSIGN9 = add i32 %old8, 10
  store i32 %ADD_ASSIGN9, i32* %n, align 4
  br label %endif5

endif5:                                           ; preds = %then6, %endif1
  %load.n = load i32, i32* %n, align 4
  ret i32 %load.n
}
```

---

## 010 — bool: or between bool variables

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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i1, align 1
  %b = alloca i1, align 1
  %n = alloca i32, align 4
  store i1 false, i1* %a, align 1
  store i1 true, i1* %b, align 1
  store i32 0, i32* %n, align 4
  %load.a = load i1, i1* %a, align 1
  %load.b = load i1, i1* %b, align 1
  %OR = or i1 %load.a, %load.b
  br i1 %OR, label %then, label %endif

then:                                             ; preds = %entry
  %old = load i32, i32* %n, align 4
  %ADD_ASSIGN = add i32 %old, 5
  store i32 %ADD_ASSIGN, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %load.a3 = load i1, i1* %a, align 1
  %load.b4 = load i1, i1* %b, align 1
  %AND = and i1 %load.a3, %load.b4
  br i1 %AND, label %then2, label %endif1

then2:                                            ; preds = %endif
  %old5 = load i32, i32* %n, align 4
  %ADD_ASSIGN6 = add i32 %old5, 50
  store i32 %ADD_ASSIGN6, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %then2, %endif
  %load.n = load i32, i32* %n, align 4
  ret i32 %load.n
}
```

---

## 011 — bool: a bool as a while condition

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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %i = alloca i32, align 4
  %going = alloca i1, align 1
  store i32 0, i32* %i, align 4
  store i1 true, i1* %going, align 1
  br label %cond

cond:                                             ; preds = %then, %entry
  %load.going = load i1, i1* %going, align 1
  br i1 %load.going, label %then, label %endwhile

then:                                             ; preds = %cond
  %old = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 4
  store i1 %LT, i1* %going, align 1
  br label %cond

endwhile:                                         ; preds = %cond
  %load.i1 = load i32, i32* %i, align 4
  ret i32 %load.i1
}
```

---

## 012 — bool: a bool default value is False

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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %flag = alloca i1, align 1
  store i1 false, i1* %flag, align 1
  %load.flag = load i1, i1* %flag, align 1
  br i1 %load.flag, label %then, label %endif

then:                                             ; preds = %entry
  ret i32 1

endif:                                            ; preds = %entry
  ret i32 7
}
```

---

## 013 — char: print chars

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
target triple = "x86_64-pc-linux-gnu"

@str = private constant [7 x i8] c"%c %c\0A\00"

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %c1 = alloca i8, align 1
  %c2 = alloca i8, align 1
  %s = alloca [7 x i8], align 1
  store i8 97, i8* %c1, align 1
  store i8 98, i8* %c2, align 1
  %dst = getelementptr inbounds [7 x i8], [7 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i64 7, i1 false)
  %load.c1 = load i8, i8* %c1, align 1
  %load.c2 = load i8, i8* %c2, align 1
  %"printf(str,load.c1,load.c2)" = call i32 (i8*, ...) @printf(i8* %dst, i8 %load.c1, i8 %load.c2)
  ret i32 0
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 014 — char: compare chars

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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %c = alloca i8, align 1
  %n = alloca i32, align 4
  store i8 120, i8* %c, align 1
  store i32 0, i32* %n, align 4
  %load.c = load i8, i8* %c, align 1
  %EQ = icmp eq i8 %load.c, 120
  br i1 %EQ, label %then, label %endif

then:                                             ; preds = %entry
  %old = load i32, i32* %n, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %load.c3 = load i8, i8* %c, align 1
  %NQ = icmp ne i8 %load.c3, 121
  br i1 %NQ, label %then2, label %endif1

then2:                                            ; preds = %endif
  %old4 = load i32, i32* %n, align 4
  %ADD_ASSIGN5 = add i32 %old4, 1
  store i32 %ADD_ASSIGN5, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %then2, %endif
  %load.c8 = load i8, i8* %c, align 1
  %EQ9 = icmp eq i8 %load.c8, 122
  br i1 %EQ9, label %then7, label %endif6

then7:                                            ; preds = %endif1
  %old10 = load i32, i32* %n, align 4
  %ADD_ASSIGN11 = add i32 %old10, 10
  store i32 %ADD_ASSIGN11, i32* %n, align 4
  br label %endif6

endif6:                                           ; preds = %then7, %endif1
  %load.n = load i32, i32* %n, align 4
  ret i32 %load.n
}
```

---

## 015 — char: order of chars

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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %c = alloca i8, align 1
  %n = alloca i32, align 4
  store i8 109, i8* %c, align 1
  store i32 0, i32* %n, align 4
  %load.c = load i8, i8* %c, align 1
  %GT = icmp sgt i8 %load.c, 97
  br i1 %GT, label %then, label %endif

then:                                             ; preds = %entry
  %old = load i32, i32* %n, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %load.c3 = load i8, i8* %c, align 1
  %LT = icmp slt i8 %load.c3, 122
  br i1 %LT, label %then2, label %endif1

then2:                                            ; preds = %endif
  %old4 = load i32, i32* %n, align 4
  %ADD_ASSIGN5 = add i32 %old4, 1
  store i32 %ADD_ASSIGN5, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %then2, %endif
  %load.c8 = load i8, i8* %c, align 1
  %GE = icmp sge i8 %load.c8, 109
  %load.c9 = load i8, i8* %c, align 1
  %LE = icmp sle i8 %load.c9, 109
  %AND = and i1 %GE, %LE
  br i1 %AND, label %then7, label %endif6

then7:                                            ; preds = %endif1
  %old10 = load i32, i32* %n, align 4
  %ADD_ASSIGN11 = add i32 %old10, 1
  store i32 %ADD_ASSIGN11, i32* %n, align 4
  br label %endif6

endif6:                                           ; preds = %then7, %endif1
  %load.n = load i32, i32* %n, align 4
  ret i32 %load.n
}
```

---

## 016 — char: reassign a char

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
target triple = "x86_64-pc-linux-gnu"

@str = private constant [4 x i8] c"%c\0A\00"

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %c = alloca i8, align 1
  %s = alloca [4 x i8], align 1
  store i8 112, i8* %c, align 1
  store i8 113, i8* %c, align 1
  %dst = getelementptr inbounds [4 x i8], [4 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i64 4, i1 false)
  %load.c = load i8, i8* %c, align 1
  %"printf(str,load.c)" = call i32 (i8*, ...) @printf(i8* %dst, i8 %load.c)
  ret i32 0
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 017 — char: char parameter and return value

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
target triple = "x86_64-pc-linux-gnu"

@str = private constant [3 x i8] c"%c\00"
@str.1 = private constant [2 x i8] c" \00"
@str.2 = private constant [2 x i8] c"\0A\00"

declare i32 @printf(i8*, ...)

define i8 @answer(i1 %0) {
entry:
  %yes = alloca i1, align 1
  store i1 %0, i1* %yes, align 1
  %load.yes = load i1, i1* %yes, align 1
  br i1 %load.yes, label %then, label %endif

then:                                             ; preds = %entry
  ret i8 89

endif:                                            ; preds = %entry
  ret i8 78
}

define void @show(i8 %0) {
entry:
  %c = alloca i8, align 1
  %s = alloca [3 x i8], align 1
  store i8 %0, i8* %c, align 1
  %dst = getelementptr inbounds [3 x i8], [3 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i64 3, i1 false)
  %load.c = load i8, i8* %c, align 1
  %"printf(str,load.c)" = call i32 (i8*, ...) @printf(i8* %dst, i8 %load.c)
  ret void
}

define i32 @main() {
entry:
  %s = alloca [2 x i8], align 1
  %s1 = alloca [2 x i8], align 1
  %"answer(True)" = call i8 @answer(i1 true)
  call void @show(i8 %"answer(True)")
  %dst = getelementptr inbounds [2 x i8], [2 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i64 2, i1 false)
  %"printf(str)" = call i32 (i8*, ...) @printf(i8* %dst)
  %"answer(False)" = call i8 @answer(i1 false)
  call void @show(i8 %"answer(False)")
  %dst2 = getelementptr inbounds [2 x i8], [2 x i8]* %s1, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst2, i8* align 1 getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0), i64 2, i1 false)
  %"printf(str)3" = call i32 (i8*, ...) @printf(i8* %dst2)
  ret i32 0
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 018 — char: a char decides a loop

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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %c = alloca i8, align 1
  %n = alloca i32, align 4
  store i8 97, i8* %c, align 1
  store i32 0, i32* %n, align 4
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.c = load i8, i8* %c, align 1
  %NQ = icmp ne i8 %load.c, 101
  br i1 %NQ, label %then, label %endwhile

then:                                             ; preds = %cond
  %old = load i32, i32* %n, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %n, align 4
  %load.n = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %load.n, 1
  br i1 %EQ, label %then1, label %next

endwhile:                                         ; preds = %cond
  %load.n10 = load i32, i32* %n, align 4
  ret i32 %load.n10

then1:                                            ; preds = %then
  store i8 98, i8* %c, align 1
  br label %endif

next:                                             ; preds = %then
  %load.n4 = load i32, i32* %n, align 4
  %EQ5 = icmp eq i32 %load.n4, 2
  br i1 %EQ5, label %then2, label %next3

then2:                                            ; preds = %next
  store i8 99, i8* %c, align 1
  br label %endif

next3:                                            ; preds = %next
  %load.n8 = load i32, i32* %n, align 4
  %EQ9 = icmp eq i32 %load.n8, 3
  br i1 %EQ9, label %then6, label %next7

then6:                                            ; preds = %next3
  store i8 100, i8* %c, align 1
  br label %endif

next7:                                            ; preds = %next3
  store i8 101, i8* %c, align 1
  br label %endif

endif:                                            ; preds = %next7, %then6, %then2, %then1
  br label %cond
}
```

---

## 019 — char: default value of a char is '\0'

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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %c = alloca i8, align 1
  store i8 0, i8* %c, align 1
  %load.c = load i8, i8* %c, align 1
  %EQ = icmp eq i8 %load.c, 0
  br i1 %EQ, label %then, label %endif

then:                                             ; preds = %entry
  ret i32 1

endif:                                            ; preds = %entry
  ret i32 0
}
```

---

## 020 — if: if / elif / else chain

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
target triple = "x86_64-pc-linux-gnu"

define i32 @grade(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %load.n = load i32, i32* %n, align 4
  %LT = icmp slt i32 %load.n, 10
  br i1 %LT, label %then, label %next

then:                                             ; preds = %entry
  ret i32 1

next:                                             ; preds = %entry
  %load.n3 = load i32, i32* %n, align 4
  %LT4 = icmp slt i32 %load.n3, 20
  br i1 %LT4, label %then1, label %next2

then1:                                            ; preds = %next
  ret i32 2

next2:                                            ; preds = %next
  ret i32 3

endif:                                            ; No predecessors!
  ret i32 0
}

define i32 @main() {
entry:
  %"grade(5)" = call i32 @grade(i32 5)
  %MUL = mul i32 %"grade(5)", 100
  %"grade(15)" = call i32 @grade(i32 15)
  %MUL1 = mul i32 %"grade(15)", 10
  %ADD = add i32 %MUL, %MUL1
  %"grade(25)" = call i32 @grade(i32 25)
  %ADD2 = add i32 %ADD, %"grade(25)"
  ret i32 %ADD2
}
```

---

## 021 — if: every comparison

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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %n = alloca i32, align 4
  store i32 5, i32* %a, align 4
  store i32 0, i32* %n, align 4
  %load.a = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %load.a, 5
  br i1 %EQ, label %then, label %endif

then:                                             ; preds = %entry
  %old = load i32, i32* %n, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %load.a3 = load i32, i32* %a, align 4
  %NQ = icmp ne i32 %load.a3, 4
  br i1 %NQ, label %then2, label %endif1

then2:                                            ; preds = %endif
  %old4 = load i32, i32* %n, align 4
  %ADD_ASSIGN5 = add i32 %old4, 1
  store i32 %ADD_ASSIGN5, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %then2, %endif
  %load.a8 = load i32, i32* %a, align 4
  %LT = icmp slt i32 %load.a8, 6
  br i1 %LT, label %then7, label %endif6

then7:                                            ; preds = %endif1
  %old9 = load i32, i32* %n, align 4
  %ADD_ASSIGN10 = add i32 %old9, 1
  store i32 %ADD_ASSIGN10, i32* %n, align 4
  br label %endif6

endif6:                                           ; preds = %then7, %endif1
  %load.a13 = load i32, i32* %a, align 4
  %LE = icmp sle i32 %load.a13, 5
  br i1 %LE, label %then12, label %endif11

then12:                                           ; preds = %endif6
  %old14 = load i32, i32* %n, align 4
  %ADD_ASSIGN15 = add i32 %old14, 1
  store i32 %ADD_ASSIGN15, i32* %n, align 4
  br label %endif11

endif11:                                          ; preds = %then12, %endif6
  %load.a18 = load i32, i32* %a, align 4
  %GT = icmp sgt i32 %load.a18, 4
  br i1 %GT, label %then17, label %endif16

then17:                                           ; preds = %endif11
  %old19 = load i32, i32* %n, align 4
  %ADD_ASSIGN20 = add i32 %old19, 1
  store i32 %ADD_ASSIGN20, i32* %n, align 4
  br label %endif16

endif16:                                          ; preds = %then17, %endif11
  %load.a23 = load i32, i32* %a, align 4
  %GE = icmp sge i32 %load.a23, 5
  br i1 %GE, label %then22, label %endif21

then22:                                           ; preds = %endif16
  %old24 = load i32, i32* %n, align 4
  %ADD_ASSIGN25 = add i32 %old24, 1
  store i32 %ADD_ASSIGN25, i32* %n, align 4
  br label %endif21

endif21:                                          ; preds = %then22, %endif16
  %load.a28 = load i32, i32* %a, align 4
  %GT29 = icmp sgt i32 %load.a28, 5
  br i1 %GT29, label %then27, label %endif26

then27:                                           ; preds = %endif21
  %old30 = load i32, i32* %n, align 4
  %ADD_ASSIGN31 = add i32 %old30, 100
  store i32 %ADD_ASSIGN31, i32* %n, align 4
  br label %endif26

endif26:                                          ; preds = %then27, %endif21
  %load.n = load i32, i32* %n, align 4
  ret i32 %load.n
}
```

---

## 022 — if: and / or

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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %n = alloca i32, align 4
  store i32 5, i32* %a, align 4
  store i32 0, i32* %n, align 4
  %load.a = load i32, i32* %a, align 4
  %GT = icmp sgt i32 %load.a, 1
  %load.a1 = load i32, i32* %a, align 4
  %LT = icmp slt i32 %load.a1, 10
  %AND = and i1 %GT, %LT
  br i1 %AND, label %then, label %endif

then:                                             ; preds = %entry
  %old = load i32, i32* %n, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %load.a4 = load i32, i32* %a, align 4
  %LT5 = icmp slt i32 %load.a4, 1
  %load.a6 = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %load.a6, 5
  %OR = or i1 %LT5, %EQ
  br i1 %OR, label %then3, label %endif2

then3:                                            ; preds = %endif
  %old7 = load i32, i32* %n, align 4
  %ADD_ASSIGN8 = add i32 %old7, 10
  store i32 %ADD_ASSIGN8, i32* %n, align 4
  br label %endif2

endif2:                                           ; preds = %then3, %endif
  %load.a11 = load i32, i32* %a, align 4
  %LT12 = icmp slt i32 %load.a11, 1
  %load.a13 = load i32, i32* %a, align 4
  %EQ14 = icmp eq i32 %load.a13, 5
  %AND15 = and i1 %LT12, %EQ14
  br i1 %AND15, label %then10, label %endif9

then10:                                           ; preds = %endif2
  %old16 = load i32, i32* %n, align 4
  %ADD_ASSIGN17 = add i32 %old16, 100
  store i32 %ADD_ASSIGN17, i32* %n, align 4
  br label %endif9

endif9:                                           ; preds = %then10, %endif2
  %load.n = load i32, i32* %n, align 4
  ret i32 %load.n
}
```

---

## 023 — if: body on the same line

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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %n = alloca i32, align 4
  store i32 5, i32* %a, align 4
  store i32 0, i32* %n, align 4
  %load.a = load i32, i32* %a, align 4
  %GT = icmp sgt i32 %load.a, 3
  br i1 %GT, label %then, label %endif

then:                                             ; preds = %entry
  %old = load i32, i32* %n, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %load.a3 = load i32, i32* %a, align 4
  %GT4 = icmp sgt i32 %load.a3, 9
  br i1 %GT4, label %then2, label %endif1

then2:                                            ; preds = %endif
  %old5 = load i32, i32* %n, align 4
  %ADD_ASSIGN6 = add i32 %old5, 10
  store i32 %ADD_ASSIGN6, i32* %n, align 4
  br label %endif1

endif1:                                           ; preds = %then2, %endif
  %load.a9 = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %load.a9, 5
  br i1 %EQ, label %then8, label %endif7

then8:                                            ; preds = %endif1
  %old10 = load i32, i32* %n, align 4
  %ADD_ASSIGN11 = add i32 %old10, 2
  store i32 %ADD_ASSIGN11, i32* %n, align 4
  br label %endif7

endif7:                                           ; preds = %then8, %endif1
  %load.n = load i32, i32* %n, align 4
  ret i32 %load.n
}
```

---

## 024 — if: nested ifs

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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 5, i32* %a, align 4
  store i32 7, i32* %b, align 4
  %load.a = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %load.a, 5
  br i1 %EQ, label %then, label %endif

then:                                             ; preds = %entry
  %load.b = load i32, i32* %b, align 4
  %EQ3 = icmp eq i32 %load.b, 7
  br i1 %EQ3, label %then2, label %endif1

then2:                                            ; preds = %then
  ret i32 1

endif1:                                           ; preds = %then
  ret i32 2

endif:                                            ; preds = %entry
  ret i32 3
}
```

---

## 025 — if: no branch taken: code after the if runs

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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 1, i32* %a, align 4
  %load.a = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %load.a, 2
  br i1 %EQ, label %then, label %next

then:                                             ; preds = %entry
  store i32 100, i32* %a, align 4
  br label %endif

next:                                             ; preds = %entry
  %load.a2 = load i32, i32* %a, align 4
  %EQ3 = icmp eq i32 %load.a2, 3
  br i1 %EQ3, label %then1, label %endif

then1:                                            ; preds = %next
  store i32 200, i32* %a, align 4
  br label %endif

endif:                                            ; preds = %then1, %next, %then
  %load.a4 = load i32, i32* %a, align 4
  %ADD = add i32 %load.a4, 8
  ret i32 %ADD
}
```

---

## 026 — if: recursion

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
target triple = "x86_64-pc-linux-gnu"

define i32 @fact(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %load.n = load i32, i32* %n, align 4
  %LE = icmp sle i32 %load.n, 1
  br i1 %LE, label %then, label %endif

then:                                             ; preds = %entry
  ret i32 1

endif:                                            ; preds = %entry
  %load.n1 = load i32, i32* %n, align 4
  %load.n2 = load i32, i32* %n, align 4
  %SUB = sub i32 %load.n2, 1
  %"fact(load.n - 1)" = call i32 @fact(i32 %SUB)
  %MUL = mul i32 %load.n1, %"fact(load.n - 1)"
  ret i32 %MUL
}

define i32 @main() {
entry:
  %"fact(5)" = call i32 @fact(i32 5)
  ret i32 %"fact(5)"
}
```

---

## 027 — if: a variable declared in a block

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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 1, i32* %a, align 4
  %load.a = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %load.a, 1
  br i1 %EQ, label %then, label %endif

then:                                             ; preds = %entry
  store i32 2, i32* %b, align 4
  %load.a1 = load i32, i32* %a, align 4
  %load.b = load i32, i32* %b, align 4
  %ADD = add i32 %load.a1, %load.b
  store i32 %ADD, i32* %a, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %load.a2 = load i32, i32* %a, align 4
  ret i32 %load.a2
}
```

---

## 028 — while: count to 10

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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 0, i32* %a, align 4
  br label %cond

cond:                                             ; preds = %then, %entry
  %load.a = load i32, i32* %a, align 4
  %LT = icmp slt i32 %load.a, 10
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %old = load i32, i32* %a, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %a, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  %load.a1 = load i32, i32* %a, align 4
  ret i32 %load.a1
}
```

---

## 029 — while: break

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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 0, i32* %a, align 4
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.a = load i32, i32* %a, align 4
  %LT = icmp slt i32 %load.a, 10
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.a2 = load i32, i32* %a, align 4
  %EQ = icmp eq i32 %load.a2, 3
  br i1 %EQ, label %then1, label %endif

endwhile:                                         ; preds = %then1, %cond
  %load.a3 = load i32, i32* %a, align 4
  ret i32 %load.a3

then1:                                            ; preds = %then
  br label %endwhile

endif:                                            ; preds = %then
  %old = load i32, i32* %a, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %a, align 4
  br label %cond
}
```

---

## 030 — while: continue

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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %i = alloca i32, align 4
  %sum = alloca i32, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %sum, align 4
  br label %cond

cond:                                             ; preds = %endif, %then1, %entry
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 10
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %old = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  %load.i2 = load i32, i32* %i, align 4
  %MOD = srem i32 %load.i2, 2
  %EQ = icmp eq i32 %MOD, 0
  br i1 %EQ, label %then1, label %endif

endwhile:                                         ; preds = %cond
  %load.sum = load i32, i32* %sum, align 4
  ret i32 %load.sum

then1:                                            ; preds = %then
  br label %cond

endif:                                            ; preds = %then
  %load.i3 = load i32, i32* %i, align 4
  %old4 = load i32, i32* %sum, align 4
  %ADD_ASSIGN5 = add i32 %old4, %load.i3
  store i32 %ADD_ASSIGN5, i32* %sum, align 4
  br label %cond
}
```

---

## 031 — while: nested loops

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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %i = alloca i32, align 4
  %c = alloca i32, align 4
  %j = alloca i32, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %c, align 4
  br label %cond

cond:                                             ; preds = %endwhile3, %entry
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 3
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  store i32 0, i32* %j, align 4
  br label %cond1

endwhile:                                         ; preds = %cond
  %load.c = load i32, i32* %c, align 4
  ret i32 %load.c

cond1:                                            ; preds = %then2, %then
  %load.j = load i32, i32* %j, align 4
  %LT4 = icmp slt i32 %load.j, 4
  br i1 %LT4, label %then2, label %endwhile3

then2:                                            ; preds = %cond1
  %old = load i32, i32* %c, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %c, align 4
  %old5 = load i32, i32* %j, align 4
  %ADD_ASSIGN6 = add i32 %old5, 1
  store i32 %ADD_ASSIGN6, i32* %j, align 4
  br label %cond1

endwhile3:                                        ; preds = %cond1
  %old7 = load i32, i32* %i, align 4
  %ADD_ASSIGN8 = add i32 %old7, 1
  store i32 %ADD_ASSIGN8, i32* %i, align 4
  br label %cond
}
```

---

## 032 — while: break leaves only the inner loop

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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %i = alloca i32, align 4
  %c = alloca i32, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %c, align 4
  br label %cond

cond:                                             ; preds = %endwhile3, %entry
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 3
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  br label %cond1

endwhile:                                         ; preds = %cond
  %load.c7 = load i32, i32* %c, align 4
  ret i32 %load.c7

cond1:                                            ; preds = %then
  %load.c = load i32, i32* %c, align 4
  %LT4 = icmp slt i32 %load.c, 100
  br i1 %LT4, label %then2, label %endwhile3

then2:                                            ; preds = %cond1
  %old = load i32, i32* %c, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %c, align 4
  br label %endwhile3

endwhile3:                                        ; preds = %then2, %cond1
  %old5 = load i32, i32* %i, align 4
  %ADD_ASSIGN6 = add i32 %old5, 1
  store i32 %ADD_ASSIGN6, i32* %i, align 4
  br label %cond
}
```

---

## 033 — while: continue in the inner loop

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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %i = alloca i32, align 4
  %c = alloca i32, align 4
  %j = alloca i32, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %c, align 4
  br label %cond

cond:                                             ; preds = %endwhile3, %entry
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 3
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  store i32 0, i32* %j, align 4
  br label %cond1

endwhile:                                         ; preds = %cond
  %load.c = load i32, i32* %c, align 4
  ret i32 %load.c

cond1:                                            ; preds = %endif, %then5, %then
  %load.j = load i32, i32* %j, align 4
  %LT4 = icmp slt i32 %load.j, 4
  br i1 %LT4, label %then2, label %endwhile3

then2:                                            ; preds = %cond1
  %old = load i32, i32* %j, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %j, align 4
  %load.j6 = load i32, i32* %j, align 4
  %MOD = srem i32 %load.j6, 2
  %EQ = icmp eq i32 %MOD, 0
  br i1 %EQ, label %then5, label %endif

endwhile3:                                        ; preds = %cond1
  %old9 = load i32, i32* %i, align 4
  %ADD_ASSIGN10 = add i32 %old9, 1
  store i32 %ADD_ASSIGN10, i32* %i, align 4
  br label %cond

then5:                                            ; preds = %then2
  br label %cond1

endif:                                            ; preds = %then2
  %old7 = load i32, i32* %c, align 4
  %ADD_ASSIGN8 = add i32 %old7, 1
  store i32 %ADD_ASSIGN8, i32* %c, align 4
  br label %cond1
}
```

---

## 034 — while: return from inside a loop

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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 100
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.i2 = load i32, i32* %i, align 4
  %EQ = icmp eq i32 %load.i2, 7
  br i1 %EQ, label %then1, label %endif

endwhile:                                         ; preds = %cond
  ret i32 0

then1:                                            ; preds = %then
  %load.i3 = load i32, i32* %i, align 4
  ret i32 %load.i3

endif:                                            ; preds = %then
  %old = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  br label %cond
}
```

---

## 035 — while: loop that never runs

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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 5, i32* %a, align 4
  br label %cond

cond:                                             ; preds = %then, %entry
  %load.a = load i32, i32* %a, align 4
  %LT = icmp slt i32 %load.a, 0
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  store i32 100, i32* %a, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  %load.a1 = load i32, i32* %a, align 4
  ret i32 %load.a1
}
```

---

## 036 — while: loop in a function, called from main

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
target triple = "x86_64-pc-linux-gnu"

define i32 @sum_to(i32 %0) {
entry:
  %n = alloca i32, align 4
  %total = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  store i32 0, i32* %total, align 4
  store i32 1, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %then, %entry
  %load.i = load i32, i32* %i, align 4
  %load.n = load i32, i32* %n, align 4
  %LE = icmp sle i32 %load.i, %load.n
  br i1 %LE, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.i1 = load i32, i32* %i, align 4
  %old = load i32, i32* %total, align 4
  %ADD_ASSIGN = add i32 %old, %load.i1
  store i32 %ADD_ASSIGN, i32* %total, align 4
  %old2 = load i32, i32* %i, align 4
  %ADD_ASSIGN3 = add i32 %old2, 1
  store i32 %ADD_ASSIGN3, i32* %i, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  %load.total = load i32, i32* %total, align 4
  ret i32 %load.total
}

define i32 @main() {
entry:
  %"sum_to(10)" = call i32 @sum_to(i32 10)
  ret i32 %"sum_to(10)"
}
```

---

## 037 — functions: parameters and calls

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
target triple = "x86_64-pc-linux-gnu"

define i32 @square(i32 %0) {
entry:
  %x = alloca i32, align 4
  store i32 %0, i32* %x, align 4
  %load.x = load i32, i32* %x, align 4
  %load.x1 = load i32, i32* %x, align 4
  %MUL = mul i32 %load.x, %load.x1
  ret i32 %MUL
}

define i32 @add(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  store i32 %1, i32* %b, align 4
  %load.a = load i32, i32* %a, align 4
  %load.b = load i32, i32* %b, align 4
  %ADD = add i32 %load.a, %load.b
  ret i32 %ADD
}

define i32 @main() {
entry:
  %"square(3)" = call i32 @square(i32 3)
  %"square(4)" = call i32 @square(i32 4)
  %"add(square(3),square(4))" = call i32 @add(i32 %"square(3)", i32 %"square(4)")
  ret i32 %"add(square(3),square(4))"
}
```

---

## 038 — functions: call before the definition

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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %"later(2)" = call i32 @later(i32 2)
  ret i32 %"later(2)"
}

define i32 @later(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %load.n = load i32, i32* %n, align 4
  %MUL = mul i32 %load.n, 9
  ret i32 %MUL
}
```

---

## 039 — functions: nested calls

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
target triple = "x86_64-pc-linux-gnu"

define i32 @inc(i32 %0) {
entry:
  %x = alloca i32, align 4
  store i32 %0, i32* %x, align 4
  %load.x = load i32, i32* %x, align 4
  %ADD = add i32 %load.x, 1
  ret i32 %ADD
}

define i32 @main() {
entry:
  %"inc(0)" = call i32 @inc(i32 0)
  %"inc(inc(0))" = call i32 @inc(i32 %"inc(0)")
  %"inc(inc(inc(0)))" = call i32 @inc(i32 %"inc(inc(0))")
  ret i32 %"inc(inc(inc(0)))"
}
```

---

## 040 — functions: a parameter is a copy

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
target triple = "x86_64-pc-linux-gnu"

define i32 @bump(i32 %0) {
entry:
  %x = alloca i32, align 4
  store i32 %0, i32* %x, align 4
  %old = load i32, i32* %x, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %x, align 4
  %load.x = load i32, i32* %x, align 4
  ret i32 %load.x
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 5, i32* %a, align 4
  %load.a = load i32, i32* %a, align 4
  %"bump(load.a)" = call i32 @bump(i32 %load.a)
  store i32 %"bump(load.a)", i32* %b, align 4
  %load.a1 = load i32, i32* %a, align 4
  %MUL = mul i32 %load.a1, 10
  %load.b = load i32, i32* %b, align 4
  %ADD = add i32 %MUL, %load.b
  ret i32 %ADD
}
```

---

## 041 — functions: call result in a variable

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
target triple = "x86_64-pc-linux-gnu"

define i32 @twice(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %load.n = load i32, i32* %n, align 4
  %MUL = mul i32 %load.n, 2
  ret i32 %MUL
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %"twice(3)" = call i32 @twice(i32 3)
  store i32 %"twice(3)", i32* %a, align 4
  %load.a = load i32, i32* %a, align 4
  %"twice(load.a)" = call i32 @twice(i32 %load.a)
  %ADD = add i32 %"twice(load.a)", 2
  store i32 %ADD, i32* %b, align 4
  %load.b = load i32, i32* %b, align 4
  ret i32 %load.b
}
```

---

## 042 — functions: void function and bare return

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
target triple = "x86_64-pc-linux-gnu"

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

## 043 — functions: function inside a function

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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %"help()" = call i32 @help()
  ret i32 %"help()"
}

define i32 @help() {
entry:
  ret i32 7
}
```

---

## 044 — functions: math op on functions

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
target triple = "x86_64-pc-linux-gnu"

define i32 @square(i32 %0) {
entry:
  %x = alloca i32, align 4
  store i32 %0, i32* %x, align 4
  %load.x = load i32, i32* %x, align 4
  %load.x1 = load i32, i32* %x, align 4
  %MUL = mul i32 %load.x, %load.x1
  ret i32 %MUL
}

define i32 @main() {
entry:
  %"square(3)" = call i32 @square(i32 3)
  %"square(4)" = call i32 @square(i32 4)
  %ADD = add i32 %"square(3)", %"square(4)"
  ret i32 %ADD
}
```

---

## 045 — proto: printf with a number

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
target triple = "x86_64-pc-linux-gnu"

@str = private constant [10 x i8] c"hello %d\0A\00"

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %s = alloca [10 x i8], align 1
  %dst = getelementptr inbounds [10 x i8], [10 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i64 10, i1 false)
  %"printf(str,42)" = call i32 (i8*, ...) @printf(i8* %dst, i32 42)
  ret i32 0
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 046 — proto: several extra arguments

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
target triple = "x86_64-pc-linux-gnu"

@str = private constant [14 x i8] c"%d + %d = %d\0A\00"

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %s = alloca [14 x i8], align 1
  store i32 1, i32* %a, align 4
  store i32 2, i32* %b, align 4
  %dst = getelementptr inbounds [14 x i8], [14 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([14 x i8], [14 x i8]* @str, i32 0, i32 0), i64 14, i1 false)
  %load.a = load i32, i32* %a, align 4
  %load.b = load i32, i32* %b, align 4
  %load.a1 = load i32, i32* %a, align 4
  %load.b2 = load i32, i32* %b, align 4
  %ADD = add i32 %load.a1, %load.b2
  %"printf(str,load.a,load.b,load.a + load.b)" = call i32 (i8*, ...) @printf(i8* %dst, i32 %load.a, i32 %load.b, i32 %ADD)
  ret i32 0
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 047 — proto: exit from C

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
target triple = "x86_64-pc-linux-gnu"

declare void @exit(i32)

define i32 @main() {
entry:
  call void @exit(i32 10)
  ret i32 0
}
```

---

## 048 — proto: escapes

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
target triple = "x86_64-pc-linux-gnu"

@str = private constant [5 x i8] c"a\09b\0A\00"
@str.1 = private constant [17 x i8] c"\22quoted\22 \\ done\0A\00"

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %s = alloca [5 x i8], align 1
  %s1 = alloca [17 x i8], align 1
  %dst = getelementptr inbounds [5 x i8], [5 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i64 5, i1 false)
  %"printf(str)" = call i32 (i8*, ...) @printf(i8* %dst)
  %dst2 = getelementptr inbounds [17 x i8], [17 x i8]* %s1, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst2, i8* align 1 getelementptr inbounds ([17 x i8], [17 x i8]* @str.1, i32 0, i32 0), i64 17, i1 false)
  %"printf(str)3" = call i32 (i8*, ...) @printf(i8* %dst2)
  ret i32 0
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 049 — proto: printf returns how many chars it wrote

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
target triple = "x86_64-pc-linux-gnu"

@str = private constant [4 x i8] c"abc\00"

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %s = alloca [4 x i8], align 1
  %dst = getelementptr inbounds [4 x i8], [4 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i64 4, i1 false)
  %"printf(str)" = call i32 (i8*, ...) @printf(i8* %dst)
  ret i32 %"printf(str)"
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 050 — proto: printf in a loop

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
target triple = "x86_64-pc-linux-gnu"

@str = private constant [3 x i8] c"%d\00"
@str.1 = private constant [2 x i8] c" \00"
@str.2 = private constant [2 x i8] c"\0A\00"

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %i = alloca i32, align 4
  %s = alloca [3 x i8], align 1
  %s5 = alloca [2 x i8], align 1
  %s7 = alloca [2 x i8], align 1
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 3
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %dst = getelementptr inbounds [3 x i8], [3 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i64 3, i1 false)
  %load.i1 = load i32, i32* %i, align 4
  %"printf(str,load.i)" = call i32 (i8*, ...) @printf(i8* %dst, i32 %load.i1)
  %load.i3 = load i32, i32* %i, align 4
  %LT4 = icmp slt i32 %load.i3, 2
  br i1 %LT4, label %then2, label %endif

endwhile:                                         ; preds = %cond
  %dst8 = getelementptr inbounds [2 x i8], [2 x i8]* %s7, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst8, i8* align 1 getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0), i64 2, i1 false)
  %"printf(str)9" = call i32 (i8*, ...) @printf(i8* %dst8)
  ret i32 0

then2:                                            ; preds = %then
  %dst6 = getelementptr inbounds [2 x i8], [2 x i8]* %s5, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst6, i8* align 1 getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i64 2, i1 false)
  %"printf(str)" = call i32 (i8*, ...) @printf(i8* %dst6)
  br label %endif

endif:                                            ; preds = %then2, %then
  %old = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  br label %cond
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 051 — global scope: a global variable changed by a function

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
target triple = "x86_64-pc-linux-gnu"

@count = global i32 0

define void @tick() {
entry:
  %old = load i32, i32* @count, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* @count, align 4
  ret void
}

define i32 @main() {
entry:
  %i = alloca i32, align 4
  store i32 0, i32* @count, align 4
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %then, %entry
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 4
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  call void @tick()
  %old = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  %load.count = load i32, i32* @count, align 4
  ret i32 %load.count
}
```

---

## 052 — global scope: script: a global counts recursive calls

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
target triple = "x86_64-pc-linux-gnu"

@calls = global i32 0
@r = global i32 0
@str = private constant [7 x i8] c"%d %d\0A\00"

declare i32 @printf(i8*, ...)

define i32 @fib(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %old = load i32, i32* @calls, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* @calls, align 4
  %load.n = load i32, i32* %n, align 4
  %LT = icmp slt i32 %load.n, 2
  br i1 %LT, label %then, label %endif

then:                                             ; preds = %entry
  %load.n1 = load i32, i32* %n, align 4
  ret i32 %load.n1

endif:                                            ; preds = %entry
  %load.n2 = load i32, i32* %n, align 4
  %SUB = sub i32 %load.n2, 1
  %"fib(load.n - 1)" = call i32 @fib(i32 %SUB)
  %load.n3 = load i32, i32* %n, align 4
  %SUB4 = sub i32 %load.n3, 2
  %"fib(load.n - 2)" = call i32 @fib(i32 %SUB4)
  %ADD = add i32 %"fib(load.n - 1)", %"fib(load.n - 2)"
  ret i32 %ADD
}

define i32 @main() {
entry:
  %s = alloca [7 x i8], align 1
  store i32 0, i32* @calls, align 4
  %"fib(10)" = call i32 @fib(i32 10)
  store i32 %"fib(10)", i32* @r, align 4
  %dst = getelementptr inbounds [7 x i8], [7 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i64 7, i1 false)
  %load.r = load i32, i32* @r, align 4
  %load.calls = load i32, i32* @calls, align 4
  %"printf(str,load.r,load.calls)" = call i32 (i8*, ...) @printf(i8* %dst, i32 %load.r, i32 %load.calls)
  %load.calls1 = load i32, i32* @calls, align 4
  ret i32 %load.calls1
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 053 — global scope: script: nested loops with continue and break at file level

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
target triple = "x86_64-pc-linux-gnu"

@found = global i32 0
@i = global i32 0

define i32 @main() {
entry:
  %j = alloca i32, align 4
  store i32 -1, i32* @found, align 4
  store i32 0, i32* @i, align 4
  br label %cond

cond:                                             ; preds = %endif17, %then1, %entry
  %load.i = load i32, i32* @i, align 4
  %LT = icmp slt i32 %load.i, 10
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %old = load i32, i32* @i, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* @i, align 4
  %load.i2 = load i32, i32* @i, align 4
  %MOD = srem i32 %load.i2, 2
  %EQ = icmp eq i32 %MOD, 0
  br i1 %EQ, label %then1, label %endif

endwhile:                                         ; preds = %then18, %cond
  %load.found19 = load i32, i32* @found, align 4
  ret i32 %load.found19

then1:                                            ; preds = %then
  br label %cond

endif:                                            ; preds = %then
  store i32 0, i32* %j, align 4
  br label %cond3

cond3:                                            ; preds = %endif7, %endif
  %load.j = load i32, i32* %j, align 4
  %LT6 = icmp slt i32 %load.j, 10
  br i1 %LT6, label %then4, label %endwhile5

then4:                                            ; preds = %cond3
  %load.i9 = load i32, i32* @i, align 4
  %load.j10 = load i32, i32* %j, align 4
  %MUL = mul i32 %load.i9, %load.j10
  %EQ11 = icmp eq i32 %MUL, 21
  br i1 %EQ11, label %then8, label %endif7

endwhile5:                                        ; preds = %then8, %cond3
  %load.found = load i32, i32* @found, align 4
  %NQ = icmp ne i32 %load.found, -1
  br i1 %NQ, label %then18, label %endif17

then8:                                            ; preds = %then4
  %load.i12 = load i32, i32* @i, align 4
  %MUL13 = mul i32 %load.i12, 10
  %load.j14 = load i32, i32* %j, align 4
  %ADD = add i32 %MUL13, %load.j14
  store i32 %ADD, i32* @found, align 4
  br label %endwhile5

endif7:                                           ; preds = %then4
  %old15 = load i32, i32* %j, align 4
  %ADD_ASSIGN16 = add i32 %old15, 1
  store i32 %ADD_ASSIGN16, i32* %j, align 4
  br label %cond3

then18:                                           ; preds = %endwhile5
  br label %endwhile

endif17:                                          ; preds = %endwhile5
  br label %cond
}
```

---

## 054 — program: mutual recursion, used before it's declared

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

## 055 — program: gcd, printf with many arguments

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

## 056 — program: collatz: a parameter changed in a loop

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

## 057 — program: the 20th prime: continue, break, a bool function

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

## 058 — program: return from two nested loops

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

## 059 — program: operator precedence soup

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
target triple = "x86_64-pc-linux-gnu"

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
  %load.a = load i32, i32* %a, align 4
  %MUL = mul i32 %load.a, 40
  %load.b = load i32, i32* %b, align 4
  %MUL1 = mul i32 %load.b, 7
  %ADD = add i32 %MUL, %MUL1
  %load.c = load i32, i32* %c, align 4
  %MUL2 = mul i32 %load.c, 3
  %ADD3 = add i32 %ADD, %MUL2
  %load.d = load i32, i32* %d, align 4
  %ADD4 = add i32 %ADD3, %load.d
  ret i32 %ADD4
}
```

---

## 060 — program: negative numbers: signed / and %

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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %c = alloca i32, align 4
  %d = alloca i32, align 4
  store i32 -3, i32* %a, align 4
  store i32 -1, i32* %b, align 4
  %load.a = load i32, i32* %a, align 4
  %load.b = load i32, i32* %b, align 4
  %MUL = mul i32 %load.a, %load.b
  %SUB = sub i32 0, %MUL
  store i32 %SUB, i32* %c, align 4
  store i32 5, i32* %d, align 4
  %load.a1 = load i32, i32* %a, align 4
  %load.b2 = load i32, i32* %b, align 4
  %ADD = add i32 %load.a1, %load.b2
  %load.c = load i32, i32* %c, align 4
  %ADD3 = add i32 %ADD, %load.c
  %load.d = load i32, i32* %d, align 4
  %ADD4 = add i32 %ADD3, %load.d
  %ADD5 = add i32 %ADD4, 100
  ret i32 %ADD5
}
```

---

## 061 — program: ackermann: deep recursion, a call as an argument

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
target triple = "x86_64-pc-linux-gnu"

define i32 @ack(i32 %0, i32 %1) {
entry:
  %m = alloca i32, align 4
  %n = alloca i32, align 4
  store i32 %0, i32* %m, align 4
  store i32 %1, i32* %n, align 4
  %load.m = load i32, i32* %m, align 4
  %EQ = icmp eq i32 %load.m, 0
  br i1 %EQ, label %then, label %endif

then:                                             ; preds = %entry
  %load.n = load i32, i32* %n, align 4
  %ADD = add i32 %load.n, 1
  ret i32 %ADD

endif:                                            ; preds = %entry
  %load.n3 = load i32, i32* %n, align 4
  %EQ4 = icmp eq i32 %load.n3, 0
  br i1 %EQ4, label %then2, label %endif1

then2:                                            ; preds = %endif
  %load.m5 = load i32, i32* %m, align 4
  %SUB = sub i32 %load.m5, 1
  %"ack(load.m - 1,1)" = call i32 @ack(i32 %SUB, i32 1)
  ret i32 %"ack(load.m - 1,1)"

endif1:                                           ; preds = %endif
  %load.m6 = load i32, i32* %m, align 4
  %SUB7 = sub i32 %load.m6, 1
  %load.m8 = load i32, i32* %m, align 4
  %load.n9 = load i32, i32* %n, align 4
  %SUB10 = sub i32 %load.n9, 1
  %"ack(load.m,load.n - 1)" = call i32 @ack(i32 %load.m8, i32 %SUB10)
  %"ack(load.m - 1,ack(load.m,load.n - 1))" = call i32 @ack(i32 %SUB7, i32 %"ack(load.m,load.n - 1)")
  ret i32 %"ack(load.m - 1,ack(load.m,load.n - 1))"
}

define i32 @main() {
entry:
  %"ack(3,3)" = call i32 @ack(i32 3, i32 3)
  ret i32 %"ack(3,3)"
}
```

---

## 062 — program: one name declared in sibling blocks

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
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %r = alloca i32, align 4
  %y = alloca i32, align 4
  %y5 = alloca i32, align 4
  %i = alloca i32, align 4
  %z = alloca i32, align 4
  store i32 1, i32* %x, align 4
  store i32 0, i32* %r, align 4
  %load.x = load i32, i32* %x, align 4
  %EQ = icmp eq i32 %load.x, 1
  br i1 %EQ, label %then, label %endif

then:                                             ; preds = %entry
  store i32 10, i32* %y, align 4
  %load.y = load i32, i32* %y, align 4
  %old = load i32, i32* %r, align 4
  %ADD_ASSIGN = add i32 %old, %load.y
  store i32 %ADD_ASSIGN, i32* %r, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %load.x3 = load i32, i32* %x, align 4
  %EQ4 = icmp eq i32 %load.x3, 1
  br i1 %EQ4, label %then2, label %endif1

then2:                                            ; preds = %endif
  store i32 20, i32* %y5, align 4
  %load.y6 = load i32, i32* %y5, align 4
  %old7 = load i32, i32* %r, align 4
  %ADD_ASSIGN8 = add i32 %old7, %load.y6
  store i32 %ADD_ASSIGN8, i32* %r, align 4
  br label %endif1

endif1:                                           ; preds = %then2, %endif
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %then9, %endif1
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 2
  br i1 %LT, label %then9, label %endwhile

then9:                                            ; preds = %cond
  %load.i10 = load i32, i32* %i, align 4
  %MUL = mul i32 %load.i10, 100
  store i32 %MUL, i32* %z, align 4
  %load.z = load i32, i32* %z, align 4
  %old11 = load i32, i32* %r, align 4
  %ADD_ASSIGN12 = add i32 %old11, %load.z
  store i32 %ADD_ASSIGN12, i32* %r, align 4
  %old13 = load i32, i32* %i, align 4
  %ADD_ASSIGN14 = add i32 %old13, 1
  store i32 %ADD_ASSIGN14, i32* %i, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  %load.r = load i32, i32* %r, align 4
  ret i32 %load.r
}
```

---

## 063 — tokenizer error: unknown character

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

## 064 — tokenizer error: unclosed string

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

## 065 — tokenizer error: unclosed character

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

## 066 — tokenizer error: character with two letters

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

## 067 — tokenizer error: unknown escape

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

## 068 — tokenizer error: unclosed comment

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

## 069 — tokenizer error: number too big for i32

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

## 070 — tokenizer error: indentation that is not a multiple of 4

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

## 071 — tokenizer error: two errors in one file: both are shown

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

---

## 072 — parser error: missing : after a function header

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

## 073 — parser error: missing ( after a function name

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

## 074 — parser error: missing ) after the parameters

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

## 075 — parser error: a function with no name

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

## 076 — parser error: a parameter with no type

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

## 077 — parser error: missing : after an if condition

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

## 078 — parser error: missing : after a while condition

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

## 079 — parser error: else with a condition

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

## 080 — parser error: elif without an if

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

## 081 — parser error: unclosed (

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

## 082 — parser error: nothing after =

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

## 083 — parser error: nothing after an operator

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

## 084 — parser error: unclosed ( in a call

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

---

## 085 — name error: undeclared variable

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

## 086 — name error: a typo in a variable name

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

## 087 — name error: a variable used before its declaration

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

## 088 — name error: a variable declared twice

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

## 089 — name error: unknown type

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

## 090 — name error: undeclared function

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

## 091 — name error: a typo in a function name

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

## 092 — name error: wrong number of arguments

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

## 093 — name error: a function declared twice

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

## 094 — name error: break outside a loop

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

## 095 — name error: continue outside a loop

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

## 096 — name error: calling a variable

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

