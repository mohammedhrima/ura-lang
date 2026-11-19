# milestone2

## index

- 001 — array: literal and indexing
- 002 — array: write through an index
- 003 — array: index from a variable
- 004 — array: index from an expression
- 005 — array: literal made of variables
- 006 — array: compound assignment on an element
- 007 — array: loop over an array
- 008 — array: an array passed to a function
- 009 — array: a function writes into the caller's array
- 010 — array: a string is a writable i8[]
- 011 — array: an array of bools
- 012 — proto: a string in a variable
- 013 — proto: a string passed to a function
- 014 — proto: proto variadic function
- 015 — proto: chars type
- 016 — ref: write through a ref
- 017 — ref: read through a ref, then write
- 018 — ref: compound assignment through a ref
- 019 — ref: swap with ref parameters
- 020 — ref: a ref parameter changes the caller
- 021 — ref: p1 = p2 copies the value
- 022 — ref: p1 = &p2 rebinds
- 023 — ref: pass a ref parameter on to another function
- 024 — ref: a ref to a bool
- 025 — ref: a ref in a loop
- 026 — ref: take the address of a variable
- 027 — crazy: fibonacci with a memo array
- 028 — crazy: upper-case a string in place
- 029 — crazy: not and ! on bools, in a loop
- 030 — crazy: count trues with as, then a cast that wraps
- 031 — crazy: caesar cipher: char arithmetic with as
- 032 — crazy: print the alphabet with a char counter
- 033 — crazy: isNullable on array elements and chars
- 034 — crazy: transpose a 3x3 matrix stored flat
- 035 — crazy: a ref to an array element
- 036 — crazy: an array literal made of calls
- 037 — crazy: elif chain on one line each, with not / and / or
- 038 — crazy: reverse a string, palindromes, a while on one line
- 039 — crazy: sieve of eratosthenes on a bool array
- 040 — crazy: atoi: parse numbers from strings
- 041 — crazy: itoa: write a number's digits into a buffer
- 042 — crazy: print a number in hex with as
- 043 — crazy: a string builder on a global buffer
- 044 — crazy: filter with as bool, not and isNullable
- 045 — parser error: unclosed [ in an array literal
- 046 — refs: ref to a ref
- 047 — refs: missing type after &
- 048 — refs: initialized with a value
- 049 — refs: address of a literal
- 050 — refs: address of an expression
- 051 — refs: address of a call
- 052 — refs: address of an address
- 053 — refs: ref to a different type
- 054 — null: assigned to a value
- 055 — null: compared with a value
- 056 — null: address of null
- 057 — expressions: compound assign on chars
- 058 — expressions: negate a string
- 059 — calls: missing & on a ref argument
- 060 — arrays: unclosed type
- 061 — arrays: size in the type
- 062 — arrays: index a non-array
- 063 — arrays: index with a string
- 064 — arrays: mixed element types
- 065 — arrays: array into a scalar
- 066 — arrays: unclosed literal
- 067 — arrays: missing comma between elements
- 068 — array: string as i8[] is writable
- 069 — array: array passed to a function
- 070 — ref: write through a ref
- 071 — ref: ref as parameter
- 072 — ref: read through a ref
- 073 — ref: math through a ref
- 074 — ref: copy the pointed value between refs
- 075 — ref: rebind a ref
- 076 — function: fn returns ref
- 077 — function: swap
- 078 — function: overloaded function
- 079 — program: null compared to null
- 080 — statement: if with a ref
- 081 — array: 1D i32 array: literal, index read, indexed write
- 082 — array: variable index and element arithmetic
- 083 — array: arrays of bool, char, f32
- 084 — array: sized constructor i32[N] with zero-init
- 085 — array: VLA: runtime-sized stack array
- 086 — array: indexing a non-array
- 087 — array: non-integer index
- 088 — array: array literal with mixed element types
- 089 — array: array size is not an integer
- 090 — array: missing closing bracket on index
- 091 — array: empty array literal has no element type
- 092 — array: unknown member
- 093 — function: single ref param: buff hero hp
- 094 — function: swap two weapon damage values via refs
- 095 — function: refs used in expression, return result
- 096 — function: compound ref assign through in a loop
- 097 — function: ref-to-value: ref passing a where value is expected
- 098 — function: ref parameter mutates the caller (counter)
- 099 — function: a void function with a ref parameter
- 100 — ref: basic ref: bind to hp, ref write through
- 101 — ref: aliasing: two refs to same hp, both see mutations
- 102 — ref: compound ref assignment through in a loop
- 103 — ref: separate refs to different vars: hp ref ref and mp
- 104 — ref: multiply and subtract through refs: f64 shield, pay mana
- 105 — ref: chain of compound mutations ref through a
- 106 — ref: reference in conditional
- 107 — ref: a ref reads correctly through every operator + as a by-value arg
- 108 — ref: ref? nullable: bound at declaration, guarded reads pass, write-through works
- 109 — ref: nested fn shares an enclosing var through a ref parameter (the sanctioned path)
- 110 — ref: a ref reads correctly through the remaining math / comparison / bitwise / cast / logical ops
- 111 — ref: a reference must be bound when declared
- 112 — ref: binding a reference to the wrong type
- 113 — ref: a ref parameter given a value
- 114 — ref: taking a reference to a literal
- 115 — ref: a by-value parameter given a reference
- 116 — ref: a reference to a name that does not exist
- 117 — ref: a nested function may not capture an enclosing local
- 118 — ref: linked list: build with ref, walk it with cur = ref cur.next
- 119 — ref: a ref parameter rebinds a struct's ref field in place (add-front)
- 120 — ref: ref? compared to null with == and !=, bound and unbound
- 121 — ref: chained rebind removes the head: lst.head = ref lst.head.next
- 122 — ref: = ref rebinds the pointer, = value writes through the referent
- 123 — ref: ref a == ref a is identity true, ref a == ref b is false
- 124 — ref: a value function cannot return a reference
- 125 — ref: a ref declaration cannot be bound to a plain value
- 126 — ref: a non-nullable value cannot be compared to null
- 127 — variable: an array type carries no size
- 128 — array: Array<T> from use "@/array": push, insert, pop, remove, front/back, resize, reverse
- 129 — use: a package that doesn't exist
- 130 — use: an error after a use points at the user's file and line
- 131 — operator: bitwise & | ^ << >> on scalars, compound forms, & stays a ref before a type, >> closes nested templates

---

## 001 — array: literal and indexing

```ura
// expect: 10
fn main() i32:
    a i32[] = [4, 5, 6]
    return a[0] + a[2]
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %array = alloca [3 x i32], align 4
  %e = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32 4, i32* %e, align 4
  %e1 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 1
  store i32 5, i32* %e1, align 4
  %e2 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 2
  store i32 6, i32* %e2, align 4
  %array3 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32* %array3, i32** %a, align 8
  %load.a = load i32*, i32** %a, align 8
  %"load.a[0]" = getelementptr inbounds i32, i32* %load.a, i32 0
  %"load.load.a[0]" = load i32, i32* %"load.a[0]", align 4
  %load.a4 = load i32*, i32** %a, align 8
  %"load.a[2]" = getelementptr inbounds i32, i32* %load.a4, i32 2
  %"load.load.a[2]" = load i32, i32* %"load.a[2]", align 4
  %ADD = add i32 %"load.load.a[0]", %"load.load.a[2]"
  ret i32 %ADD
}
```

---

## 002 — array: write through an index

```ura
// expect: 9
fn main() i32:
    a i32[] = [1, 2, 3]
    a[1] = 9
    return a[1]
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %array = alloca [3 x i32], align 4
  %e = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32 1, i32* %e, align 4
  %e1 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 1
  store i32 2, i32* %e1, align 4
  %e2 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 2
  store i32 3, i32* %e2, align 4
  %array3 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32* %array3, i32** %a, align 8
  %load.a = load i32*, i32** %a, align 8
  %"load.a[1]" = getelementptr inbounds i32, i32* %load.a, i32 1
  store i32 9, i32* %"load.a[1]", align 4
  %load.a4 = load i32*, i32** %a, align 8
  %"load.a[1]5" = getelementptr inbounds i32, i32* %load.a4, i32 1
  %"load.load.a[1]" = load i32, i32* %"load.a[1]5", align 4
  ret i32 %"load.load.a[1]"
}
```

---

## 003 — array: index from a variable

```ura
// expect: 9
fn main() i32:
    a i32[] = [7, 8, 9]
    i i32 = 2
    return a[i]
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %array = alloca [3 x i32], align 4
  %i = alloca i32, align 4
  %e = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32 7, i32* %e, align 4
  %e1 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 1
  store i32 8, i32* %e1, align 4
  %e2 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 2
  store i32 9, i32* %e2, align 4
  %array3 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32* %array3, i32** %a, align 8
  store i32 2, i32* %i, align 4
  %load.a = load i32*, i32** %a, align 8
  %load.i = load i32, i32* %i, align 4
  %"load.a[load.i]" = getelementptr inbounds i32, i32* %load.a, i32 %load.i
  %"load.load.a[load.i]" = load i32, i32* %"load.a[load.i]", align 4
  ret i32 %"load.load.a[load.i]"
}
```

---

## 004 — array: index from an expression

```ura
// expect: 8
fn main() i32:
    a i32[] = [7, 8, 9]
    i i32 = 2
    return a[i - 1]
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %array = alloca [3 x i32], align 4
  %i = alloca i32, align 4
  %e = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32 7, i32* %e, align 4
  %e1 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 1
  store i32 8, i32* %e1, align 4
  %e2 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 2
  store i32 9, i32* %e2, align 4
  %array3 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32* %array3, i32** %a, align 8
  store i32 2, i32* %i, align 4
  %load.a = load i32*, i32** %a, align 8
  %load.i = load i32, i32* %i, align 4
  %SUB = sub i32 %load.i, 1
  %"load.a[load.i - 1]" = getelementptr inbounds i32, i32* %load.a, i32 %SUB
  %"load.load.a[load.i - 1]" = load i32, i32* %"load.a[load.i - 1]", align 4
  ret i32 %"load.load.a[load.i - 1]"
}
```

---

## 005 — array: literal made of variables

```ura
// expect: 16
fn main() i32:
    x i32 = 10
    y i32 = 4
    a i32[] = [x, 2, y]
    return a[0] + a[1] + a[2]
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
  %y = alloca i32, align 4
  %a = alloca i32*, align 8
  %array = alloca [3 x i32], align 4
  store i32 10, i32* %x, align 4
  store i32 4, i32* %y, align 4
  %load.x = load i32, i32* %x, align 4
  %load.y = load i32, i32* %y, align 4
  %e = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32 %load.x, i32* %e, align 4
  %e1 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 1
  store i32 2, i32* %e1, align 4
  %e2 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 2
  store i32 %load.y, i32* %e2, align 4
  %array3 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32* %array3, i32** %a, align 8
  %load.a = load i32*, i32** %a, align 8
  %"load.a[0]" = getelementptr inbounds i32, i32* %load.a, i32 0
  %"load.load.a[0]" = load i32, i32* %"load.a[0]", align 4
  %load.a4 = load i32*, i32** %a, align 8
  %"load.a[1]" = getelementptr inbounds i32, i32* %load.a4, i32 1
  %"load.load.a[1]" = load i32, i32* %"load.a[1]", align 4
  %ADD = add i32 %"load.load.a[0]", %"load.load.a[1]"
  %load.a5 = load i32*, i32** %a, align 8
  %"load.a[2]" = getelementptr inbounds i32, i32* %load.a5, i32 2
  %"load.load.a[2]" = load i32, i32* %"load.a[2]", align 4
  %ADD6 = add i32 %ADD, %"load.load.a[2]"
  ret i32 %ADD6
}
```

---

## 006 — array: compound assignment on an element

```ura
// expect: 18   (5 + 2 + 9 + 2)
fn main() i32:
    a i32[] = [1, 2, 3]
    a[0] += 4
    a[2] *= 3
    return a[0] + a[1] + a[2] + 2
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %array = alloca [3 x i32], align 4
  %e = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32 1, i32* %e, align 4
  %e1 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 1
  store i32 2, i32* %e1, align 4
  %e2 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 2
  store i32 3, i32* %e2, align 4
  %array3 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32* %array3, i32** %a, align 8
  %load.a = load i32*, i32** %a, align 8
  %"load.a[0]" = getelementptr inbounds i32, i32* %load.a, i32 0
  %old = load i32, i32* %"load.a[0]", align 4
  %ADD_ASSIGN = add i32 %old, 4
  store i32 %ADD_ASSIGN, i32* %"load.a[0]", align 4
  %load.a4 = load i32*, i32** %a, align 8
  %"load.a[2]" = getelementptr inbounds i32, i32* %load.a4, i32 2
  %old5 = load i32, i32* %"load.a[2]", align 4
  %MUL_ASSIGN = mul i32 %old5, 3
  store i32 %MUL_ASSIGN, i32* %"load.a[2]", align 4
  %load.a6 = load i32*, i32** %a, align 8
  %"load.a[0]7" = getelementptr inbounds i32, i32* %load.a6, i32 0
  %"load.load.a[0]" = load i32, i32* %"load.a[0]7", align 4
  %load.a8 = load i32*, i32** %a, align 8
  %"load.a[1]" = getelementptr inbounds i32, i32* %load.a8, i32 1
  %"load.load.a[1]" = load i32, i32* %"load.a[1]", align 4
  %ADD = add i32 %"load.load.a[0]", %"load.load.a[1]"
  %load.a9 = load i32*, i32** %a, align 8
  %"load.a[2]10" = getelementptr inbounds i32, i32* %load.a9, i32 2
  %"load.load.a[2]" = load i32, i32* %"load.a[2]10", align 4
  %ADD11 = add i32 %ADD, %"load.load.a[2]"
  %ADD12 = add i32 %ADD11, 2
  ret i32 %ADD12
}
```

---

## 007 — array: loop over an array

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
  %a = alloca i32*, align 8
  %array = alloca [5 x i32], align 4
  %sum = alloca i32, align 4
  %i = alloca i32, align 4
  %s = alloca [3 x i8], align 1
  %s10 = alloca [2 x i8], align 1
  %s18 = alloca [2 x i8], align 1
  %e = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 0
  store i32 3, i32* %e, align 4
  %e1 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 1
  store i32 1, i32* %e1, align 4
  %e2 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 2
  store i32 4, i32* %e2, align 4
  %e3 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 3
  store i32 1, i32* %e3, align 4
  %e4 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 4
  store i32 5, i32* %e4, align 4
  %array5 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 0
  store i32* %array5, i32** %a, align 8
  store i32 0, i32* %sum, align 4
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 5
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %dst = getelementptr inbounds [3 x i8], [3 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i64 3, i1 false)
  %load.a = load i32*, i32** %a, align 8
  %load.i6 = load i32, i32* %i, align 4
  %"load.a[load.i]" = getelementptr inbounds i32, i32* %load.a, i32 %load.i6
  %"load.load.a[load.i]" = load i32, i32* %"load.a[load.i]", align 4
  %"printf(str,load.load.a[load.i])" = call i32 (i8*, ...) @printf(i8* %dst, i32 %"load.load.a[load.i]")
  %load.i8 = load i32, i32* %i, align 4
  %LT9 = icmp slt i32 %load.i8, 4
  br i1 %LT9, label %then7, label %endif

endwhile:                                         ; preds = %cond
  %dst19 = getelementptr inbounds [2 x i8], [2 x i8]* %s18, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst19, i8* align 1 getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0), i64 2, i1 false)
  %"printf(str)20" = call i32 (i8*, ...) @printf(i8* %dst19)
  %load.sum = load i32, i32* %sum, align 4
  ret i32 %load.sum

then7:                                            ; preds = %then
  %dst11 = getelementptr inbounds [2 x i8], [2 x i8]* %s10, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst11, i8* align 1 getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i64 2, i1 false)
  %"printf(str)" = call i32 (i8*, ...) @printf(i8* %dst11)
  br label %endif

endif:                                            ; preds = %then7, %then
  %load.a12 = load i32*, i32** %a, align 8
  %load.i13 = load i32, i32* %i, align 4
  %"load.a[load.i]14" = getelementptr inbounds i32, i32* %load.a12, i32 %load.i13
  %"load.load.a[load.i]15" = load i32, i32* %"load.a[load.i]14", align 4
  %old = load i32, i32* %sum, align 4
  %ADD_ASSIGN = add i32 %old, %"load.load.a[load.i]15"
  store i32 %ADD_ASSIGN, i32* %sum, align 4
  %old16 = load i32, i32* %i, align 4
  %ADD_ASSIGN17 = add i32 %old16, 1
  store i32 %ADD_ASSIGN17, i32* %i, align 4
  br label %cond
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 008 — array: an array passed to a function

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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @sum(i32* %0, i32 %1) {
entry:
  %a = alloca i32*, align 8
  %n = alloca i32, align 4
  %total = alloca i32, align 4
  %i = alloca i32, align 4
  store i32* %0, i32** %a, align 8
  store i32 %1, i32* %n, align 4
  store i32 0, i32* %total, align 4
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %then, %entry
  %load.i = load i32, i32* %i, align 4
  %load.n = load i32, i32* %n, align 4
  %LT = icmp slt i32 %load.i, %load.n
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.a = load i32*, i32** %a, align 8
  %load.i1 = load i32, i32* %i, align 4
  %"load.a[load.i]" = getelementptr inbounds i32, i32* %load.a, i32 %load.i1
  %"load.load.a[load.i]" = load i32, i32* %"load.a[load.i]", align 4
  %old = load i32, i32* %total, align 4
  %ADD_ASSIGN = add i32 %old, %"load.load.a[load.i]"
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
  %values = alloca i32*, align 8
  %array = alloca [3 x i32], align 4
  %e = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32 1, i32* %e, align 4
  %e1 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 1
  store i32 2, i32* %e1, align 4
  %e2 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 2
  store i32 3, i32* %e2, align 4
  %array3 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32* %array3, i32** %values, align 8
  %load.values = load i32*, i32** %values, align 8
  %"sum(load.values,3)" = call i32 @sum(i32* %load.values, i32 3)
  ret i32 %"sum(load.values,3)"
}
```

---

## 009 — array: a function writes into the caller's array

```ura
// expect: 7
fn set_first(a i32[], v i32):
    a[0] = v

fn main() i32:
    values i32[] = [1, 2, 3]
    set_first(values, 7)
    return values[0]
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define void @set_first(i32* %0, i32 %1) {
entry:
  %a = alloca i32*, align 8
  %v = alloca i32, align 4
  store i32* %0, i32** %a, align 8
  store i32 %1, i32* %v, align 4
  %load.a = load i32*, i32** %a, align 8
  %"load.a[0]" = getelementptr inbounds i32, i32* %load.a, i32 0
  %load.v = load i32, i32* %v, align 4
  store i32 %load.v, i32* %"load.a[0]", align 4
  ret void
}

define i32 @main() {
entry:
  %values = alloca i32*, align 8
  %array = alloca [3 x i32], align 4
  %e = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32 1, i32* %e, align 4
  %e1 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 1
  store i32 2, i32* %e1, align 4
  %e2 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 2
  store i32 3, i32* %e2, align 4
  %array3 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32* %array3, i32** %values, align 8
  %load.values = load i32*, i32** %values, align 8
  call void @set_first(i32* %load.values, i32 7)
  %load.values4 = load i32*, i32** %values, align 8
  %"load.values[0]" = getelementptr inbounds i32, i32* %load.values4, i32 0
  %"load.load.values[0]" = load i32, i32* %"load.values[0]", align 4
  ret i32 %"load.load.values[0]"
}
```

---

## 010 — array: a string is a writable i8[]

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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %s = alloca i8*, align 8
  store i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i8** %s, align 8
  %"$s" = load i8*, i8** %s, align 8
  %"s[]" = getelementptr inbounds i8, i8* %"$s", i32 0
  store i8 120, i8* %"s[]", align 1
  %"$s1" = load i8*, i8** %s, align 8
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i8* %"$s1")
  br label %if0

if0:                                              ; preds = %entry
  %"$s2" = load i8*, i8** %s, align 8
  %"s[]3" = getelementptr inbounds i8, i8* %"$s2", i32 1
  %"$s[]" = load i8, i8* %"s[]3", align 1
  %EQ = icmp eq i8 %"$s[]", 98
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  ret i32 0
}
```

---

## 011 — array: an array of bools

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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %flags = alloca i1*, align 8
  %array = alloca [3 x i1], align 1
  %n = alloca i32, align 4
  %i = alloca i32, align 4
  %e = getelementptr inbounds [3 x i1], [3 x i1]* %array, i32 0, i32 0
  store i1 true, i1* %e, align 1
  %e1 = getelementptr inbounds [3 x i1], [3 x i1]* %array, i32 0, i32 1
  store i1 false, i1* %e1, align 1
  %e2 = getelementptr inbounds [3 x i1], [3 x i1]* %array, i32 0, i32 2
  store i1 true, i1* %e2, align 1
  %array3 = getelementptr inbounds [3 x i1], [3 x i1]* %array, i32 0, i32 0
  store i1* %array3, i1** %flags, align 8
  store i32 0, i32* %n, align 4
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 3
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.flags = load i1*, i1** %flags, align 8
  %load.i5 = load i32, i32* %i, align 4
  %"load.flags[load.i]" = getelementptr inbounds i1, i1* %load.flags, i32 %load.i5
  %"load.load.flags[load.i]" = load i1, i1* %"load.flags[load.i]", align 1
  br i1 %"load.load.flags[load.i]", label %then4, label %endif

endwhile:                                         ; preds = %cond
  %load.n = load i32, i32* %n, align 4
  ret i32 %load.n

then4:                                            ; preds = %then
  %old = load i32, i32* %n, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %then4, %then
  %old6 = load i32, i32* %i, align 4
  %ADD_ASSIGN7 = add i32 %old6, 1
  store i32 %ADD_ASSIGN7, i32* %i, align 4
  br label %cond
}
```

---

## 012 — proto: a string in a variable

```ura
// stdout: <abcdef>
// expect: 0
proto printf(fmt i8[], ...) i32

fn main() i32:
    str i8[] = "abcdef"
    printf("<%s>\n", str)
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [7 x i8] c"abcdef\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"<%s>\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %str = alloca i8*, align 8
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i8** %str, align 8
  %"$str" = load i8*, i8** %str, align 8
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i8* %"$str")
  ret i32 0
}
```

---

## 013 — proto: a string passed to a function

```ura
// stdout: hi ura
// expect: 0
proto printf(fmt i8[], ...) i32

fn greet(name i8[]):
    printf("hi %s\n", name)

fn main() i32:
    greet("ura")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [7 x i8] c"hi %s\0A\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"ura\00", align 1

declare i32 @printf(i8*, ...)

define void @greet(i8* %0) {
entry:
  %name = alloca i8*, align 8
  store i8* %0, i8** %name, align 8
  %"$name" = load i8*, i8** %name, align 8
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i8* %"$name")
  ret void
}

define i32 @main() {
entry:
  call void @greet(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}
```

---

## 014 — proto: proto variadic function

```ura
proto printf(fmt i8[], ...) i32

fn main() i32:
    str i8[] = "abcdef"
    printf("<%s>", str)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [7 x i8] c"abcdef\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"<%s>\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %str = alloca i8*, align 8
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i8** %str, align 8
  %"$str" = load i8*, i8** %str, align 8
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0), i8* %"$str")
  ret i32 0
}
```

---

## 015 — proto: chars type

```ura
fn main() i32:
    str i8[] = "abcdef"
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [7 x i8] c"abcdef\00", align 1

define i32 @main() {
entry:
  %str = alloca i8*, align 8
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i8** %str, align 8
  ret i32 0
}
```

---

## 016 — ref: write through a ref

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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [8 x i8] c"a = %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %p = alloca i32*, align 8
  store i32 10, i32* %a, align 4
  store i32* %a, i32** %p, align 8
  %"*p" = load i32*, i32** %p, align 8
  store i32 12, i32* %"*p", align 4
  %"$a" = load i32, i32* %a, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str, i32 0, i32 0), i32 %"$a")
  %"$a1" = load i32, i32* %a, align 4
  ret i32 %"$a1"
}
```

---

## 017 — ref: read through a ref, then write

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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [9 x i8] c"p = %d, \00", align 1
@str.1 = private unnamed_addr constant [8 x i8] c"a = %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %p = alloca i32*, align 8
  store i32 10, i32* %a, align 4
  store i32* %a, i32** %p, align 8
  %"*p" = load i32*, i32** %p, align 8
  %"$*p" = load i32, i32* %"*p", align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %"$*p")
  %"*p1" = load i32*, i32** %p, align 8
  %"*p2" = load i32*, i32** %p, align 8
  %"$*p3" = load i32, i32* %"*p2", align 4
  %ADD = add i32 %"$*p3", 5
  store i32 %ADD, i32* %"*p1", align 4
  %"$a" = load i32, i32* %a, align 4
  %call.printf4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.1, i32 0, i32 0), i32 %"$a")
  %"$a5" = load i32, i32* %a, align 4
  ret i32 %"$a5"
}
```

---

## 018 — ref: compound assignment through a ref

```ura
// expect: 24
fn main() i32:
    a i32 = 10
    p &i32 = &a
    p += 2
    p *= 2
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
  %p = alloca i32*, align 8
  store i32 10, i32* %a, align 4
  store i32* %a, i32** %p, align 8
  %"*p" = load i32*, i32** %p, align 8
  %"$*p" = load i32, i32* %"*p", align 4
  %ADD = add i32 %"$*p", 2
  store i32 %ADD, i32* %"*p", align 4
  %"*p1" = load i32*, i32** %p, align 8
  %"$*p2" = load i32, i32* %"*p1", align 4
  %MUL = mul i32 %"$*p2", 2
  store i32 %MUL, i32* %"*p1", align 4
  %"$a" = load i32, i32* %a, align 4
  ret i32 %"$a"
}
```

---

## 019 — ref: swap with ref parameters

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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [16 x i8] c"a = %d, b = %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @swap(i32* %0, i32* %1) {
entry:
  %x = alloca i32*, align 8
  %y = alloca i32*, align 8
  %t = alloca i32, align 4
  store i32* %0, i32** %x, align 8
  store i32* %1, i32** %y, align 8
  %"*x" = load i32*, i32** %x, align 8
  %"$*x" = load i32, i32* %"*x", align 4
  store i32 %"$*x", i32* %t, align 4
  %"*x1" = load i32*, i32** %x, align 8
  %"*y" = load i32*, i32** %y, align 8
  %"$*y" = load i32, i32* %"*y", align 4
  store i32 %"$*y", i32* %"*x1", align 4
  %"*y2" = load i32*, i32** %y, align 8
  %"$t" = load i32, i32* %t, align 4
  store i32 %"$t", i32* %"*y2", align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 1, i32* %a, align 4
  store i32 2, i32* %b, align 4
  call void @swap(i32* %a, i32* %b)
  %"$a" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str, i32 0, i32 0), i32 %"$a", i32 %"$b")
  %"$a1" = load i32, i32* %a, align 4
  %MUL = mul i32 %"$a1", 10
  %"$b2" = load i32, i32* %b, align 4
  %ADD = add i32 %MUL, %"$b2"
  ret i32 %ADD
}
```

---

## 020 — ref: a ref parameter changes the caller

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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define void @inc(i32* %0) {
entry:
  %c = alloca i32*, align 8
  store i32* %0, i32** %c, align 8
  %"*c" = load i32*, i32** %c, align 8
  %"$*c" = load i32, i32* %"*c", align 4
  %ADD = add i32 %"$*c", 1
  store i32 %ADD, i32* %"*c", align 4
  ret void
}

define i32 @main() {
entry:
  %n = alloca i32, align 4
  store i32 0, i32* %n, align 4
  call void @inc(i32* %n)
  call void @inc(i32* %n)
  call void @inc(i32* %n)
  %"$n" = load i32, i32* %n, align 4
  ret i32 %"$n"
}
```

---

## 021 — ref: p1 = p2 copies the value

```ura
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [16 x i8] c"a = %d, b = %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %p1 = alloca i32*, align 8
  %p2 = alloca i32*, align 8
  store i32 1, i32* %a, align 4
  store i32 2, i32* %b, align 4
  store i32* %a, i32** %p1, align 8
  store i32* %b, i32** %p2, align 8
  %"*p1" = load i32*, i32** %p1, align 8
  %"*p2" = load i32*, i32** %p2, align 8
  %"$*p2" = load i32, i32* %"*p2", align 4
  store i32 %"$*p2", i32* %"*p1", align 4
  %"$a" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str, i32 0, i32 0), i32 %"$a", i32 %"$b")
  %"$a1" = load i32, i32* %a, align 4
  ret i32 %"$a1"
}
```

---

## 022 — ref: p1 = &p2 rebinds

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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [16 x i8] c"a = %d, b = %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %p1 = alloca i32*, align 8
  %p2 = alloca i32*, align 8
  store i32 1, i32* %a, align 4
  store i32 2, i32* %b, align 4
  store i32* %a, i32** %p1, align 8
  store i32* %b, i32** %p2, align 8
  %"*p2" = load i32*, i32** %p2, align 8
  store i32* %"*p2", i32** %p1, align 8
  %"*p1" = load i32*, i32** %p1, align 8
  store i32 9, i32* %"*p1", align 4
  %"$a" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str, i32 0, i32 0), i32 %"$a", i32 %"$b")
  %"$b1" = load i32, i32* %b, align 4
  ret i32 %"$b1"
}
```

---

## 023 — ref: pass a ref parameter on to another function

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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define void @add_one(i32* %0) {
entry:
  %p = alloca i32*, align 8
  store i32* %0, i32** %p, align 8
  %"*p" = load i32*, i32** %p, align 8
  %"$*p" = load i32, i32* %"*p", align 4
  %ADD = add i32 %"$*p", 1
  store i32 %ADD, i32* %"*p", align 4
  ret void
}

define void @add_two(i32* %0) {
entry:
  %p = alloca i32*, align 8
  store i32* %0, i32** %p, align 8
  %"*p" = load i32*, i32** %p, align 8
  call void @add_one(i32* %"*p")
  %"*p1" = load i32*, i32** %p, align 8
  call void @add_one(i32* %"*p1")
  ret void
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 0, i32* %a, align 4
  call void @add_two(i32* %a)
  %"$a" = load i32, i32* %a, align 4
  ret i32 %"$a"
}
```

---

## 024 — ref: a ref to a bool

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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %b = alloca i1, align 1
  %p = alloca i1*, align 8
  store i1 false, i1* %b, align 1
  store i1* %b, i1** %p, align 8
  %"*p" = load i1*, i1** %p, align 8
  store i1 true, i1* %"*p", align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$b" = load i1, i1* %b, align 1
  br i1 %"$b", label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  ret i32 0
}
```

---

## 025 — ref: a ref in a loop

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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %total = alloca i32, align 4
  %p = alloca i32*, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %total, align 4
  store i32* %total, i32** %p, align 8
  store i32 1, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LE = icmp sle i32 %"$i", 5
  br i1 %LE, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$i1" = load i32, i32* %i, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$i1")
  %"*p" = load i32*, i32** %p, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"$*p" = load i32, i32* %"*p", align 4
  %ADD = add i32 %"$*p", %"$i2"
  store i32 %ADD, i32* %"*p", align 4
  %"$i3" = load i32, i32* %i, align 4
  %ADD4 = add i32 %"$i3", 1
  store i32 %ADD4, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %call.printf5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %"$total" = load i32, i32* %total, align 4
  ret i32 %"$total"
}
```

---

## 026 — ref: take the address of a variable

```ura
fn main() i32:
    a i32 = 10
    p &i32 = &a
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
  %p = alloca i32*, align 8
  store i32 10, i32* %a, align 4
  store i32* %a, i32** %p, align 8
  ret i32 0
}
```

---

## 027 — crazy: fibonacci with a memo array

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

## 028 — crazy: upper-case a string in place

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
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [11 x i8] c"hello, ura\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @length(i8* %0) {
entry:
  %s = alloca i8*, align 8
  %n = alloca i32, align 4
  store i8* %0, i8** %s, align 8
  store i32 0, i32* %n, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$s" = load i8*, i8** %s, align 8
  %"$n" = load i32, i32* %n, align 4
  %"s[]" = getelementptr inbounds i8, i8* %"$s", i32 %"$n"
  %"$s[]" = load i8, i8* %"s[]", align 1
  %NQ = icmp ne i8 %"$s[]", 0
  br i1 %NQ, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$n1" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n1", 1
  store i32 %ADD, i32* %n, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$n2" = load i32, i32* %n, align 4
  ret i32 %"$n2"
}

define void @upper(i8* %0) {
entry:
  %s = alloca i8*, align 8
  %i = alloca i32, align 4
  store i8* %0, i8** %s, align 8
  store i32 0, i32* %i, align 4
  br label %while1

while1:                                           ; preds = %endif, %entry
  %"$s" = load i8*, i8** %s, align 8
  %"$i" = load i32, i32* %i, align 4
  %"s[]" = getelementptr inbounds i8, i8* %"$s", i32 %"$i"
  %"$s[]" = load i8, i8* %"s[]", align 1
  %NQ = icmp ne i8 %"$s[]", 0
  br i1 %NQ, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  br label %if2

if2:                                              ; preds = %while1.then
  %"$s1" = load i8*, i8** %s, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"s[]3" = getelementptr inbounds i8, i8* %"$s1", i32 %"$i2"
  %"$s[]4" = load i8, i8* %"s[]3", align 1
  %GE = icmp sge i8 %"$s[]4", 97
  %"$s5" = load i8*, i8** %s, align 8
  %"$i6" = load i32, i32* %i, align 4
  %"s[]7" = getelementptr inbounds i8, i8* %"$s5", i32 %"$i6"
  %"$s[]8" = load i8, i8* %"s[]7", align 1
  %LE = icmp sle i8 %"$s[]8", 122
  %AND = and i1 %GE, %LE
  br i1 %AND, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"$s9" = load i8*, i8** %s, align 8
  %"$i10" = load i32, i32* %i, align 4
  %"s[]11" = getelementptr inbounds i8, i8* %"$s9", i32 %"$i10"
  %"$s[]12" = load i8, i8* %"s[]11", align 1
  %SUB = sub i8 %"$s[]12", 32
  store i8 %SUB, i8* %"s[]11", align 1
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  %"$i13" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i13", 1
  store i32 %ADD, i32* %i, align 4
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define i32 @main() {
entry:
  %s = alloca i8*, align 8
  store i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str, i32 0, i32 0), i8** %s, align 8
  %"$s" = load i8*, i8** %s, align 8
  call void @upper(i8* %"$s")
  %"$s1" = load i8*, i8** %s, align 8
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i8* %"$s1")
  %"$s2" = load i8*, i8** %s, align 8
  %call.length = call i32 @length(i8* %"$s2")
  ret i32 %call.length
}
```

---

## 029 — crazy: not and ! on bools, in a loop

```ura
// expect: 12
fn main() i32:
    a bool = True
    b bool = !a
    n i32 = 0
    i i32 = 0
    while i < 5:
        a = not a
        if a: n += 1
        i += 1
    if not b and !(n == 3): n += 10
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
  %i = alloca i32, align 4
  store i1 true, i1* %a, align 1
  %load.a = load i1, i1* %a, align 1
  %NOT = xor i1 %load.a, true
  store i1 %NOT, i1* %b, align 1
  store i32 0, i32* %n, align 4
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 5
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.a1 = load i1, i1* %a, align 1
  %NOT2 = xor i1 %load.a1, true
  store i1 %NOT2, i1* %a, align 1
  %load.a4 = load i1, i1* %a, align 1
  br i1 %load.a4, label %then3, label %endif

endwhile:                                         ; preds = %cond
  %load.b = load i1, i1* %b, align 1
  %NOT9 = xor i1 %load.b, true
  %load.n = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %load.n, 3
  %NOT10 = xor i1 %EQ, true
  %AND = and i1 %NOT9, %NOT10
  br i1 %AND, label %then8, label %endif7

then3:                                            ; preds = %then
  %old = load i32, i32* %n, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %then3, %then
  %old5 = load i32, i32* %i, align 4
  %ADD_ASSIGN6 = add i32 %old5, 1
  store i32 %ADD_ASSIGN6, i32* %i, align 4
  br label %cond

then8:                                            ; preds = %endwhile
  %old11 = load i32, i32* %n, align 4
  %ADD_ASSIGN12 = add i32 %old11, 10
  store i32 %ADD_ASSIGN12, i32* %n, align 4
  br label %endif7

endif7:                                           ; preds = %then8, %endwhile
  %load.n13 = load i32, i32* %n, align 4
  ret i32 %load.n13
}
```

---

## 030 — crazy: count trues with as, then a cast that wraps

```ura
// expect: 6   (3 * 10, plus 1000 as i8 = -24)
fn main() i32:
    flags bool[] = [True, False, True, True]
    n i32 = 0
    i i32 = 0
    while i < 4:
        n += flags[i] as i32
        i += 1
    big i32 = 1000
    return n * 10 + (big as i8) as i32
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  %flags = alloca i1*, align 8
  %array = alloca [4 x i1], align 1
  %n = alloca i32, align 4
  %i = alloca i32, align 4
  %big = alloca i32, align 4
  %e = getelementptr inbounds [4 x i1], [4 x i1]* %array, i32 0, i32 0
  store i1 true, i1* %e, align 1
  %e1 = getelementptr inbounds [4 x i1], [4 x i1]* %array, i32 0, i32 1
  store i1 false, i1* %e1, align 1
  %e2 = getelementptr inbounds [4 x i1], [4 x i1]* %array, i32 0, i32 2
  store i1 true, i1* %e2, align 1
  %e3 = getelementptr inbounds [4 x i1], [4 x i1]* %array, i32 0, i32 3
  store i1 true, i1* %e3, align 1
  %array4 = getelementptr inbounds [4 x i1], [4 x i1]* %array, i32 0, i32 0
  store i1* %array4, i1** %flags, align 8
  store i32 0, i32* %n, align 4
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %then, %entry
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 4
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.flags = load i1*, i1** %flags, align 8
  %load.i5 = load i32, i32* %i, align 4
  %"load.flags[load.i]" = getelementptr inbounds i1, i1* %load.flags, i32 %load.i5
  %"load.load.flags[load.i]" = load i1, i1* %"load.flags[load.i]", align 1
  %"(i32)load.load.flags[load.i]" = zext i1 %"load.load.flags[load.i]" to i32
  %old = load i32, i32* %n, align 4
  %ADD_ASSIGN = add i32 %old, %"(i32)load.load.flags[load.i]"
  store i32 %ADD_ASSIGN, i32* %n, align 4
  %old6 = load i32, i32* %i, align 4
  %ADD_ASSIGN7 = add i32 %old6, 1
  store i32 %ADD_ASSIGN7, i32* %i, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  store i32 1000, i32* %big, align 4
  %load.n = load i32, i32* %n, align 4
  %MUL = mul i32 %load.n, 10
  %load.big = load i32, i32* %big, align 4
  %"(i8)load.big" = trunc i32 %load.big to i8
  %"(i32)(i8)load.big" = sext i8 %"(i8)load.big" to i32
  %ADD = add i32 %MUL, %"(i32)(i8)load.big"
  ret i32 %ADD
}
```

---

## 031 — crazy: caesar cipher: char arithmetic with as

```ura
// stdout: khoor, zruog | hello, world
// expect: 104
proto printf(fmt i8[], ...) i32

fn shift(c char, k i32) char:
    if c < 'a' or c > 'z': return c
    n i32 = (c as i32) - ('a' as i32)
    n = (n + k) % 26
    return (n + ('a' as i32)) as char

fn encode(s i8[], k i32):
    i i32 = 0
    while s[i] != '\0':
        s[i] = shift(s[i], k)
        i += 1

fn main() i32:
    s i8[] = "hello, world"
    encode(s, 3)
    printf("%s | ", s)
    encode(s, 23)
    printf("%s\n", s)
    return s[0] as i32
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private constant [13 x i8] c"hello, world\00"
@str.1 = private constant [6 x i8] c"%s | \00"
@str.2 = private constant [4 x i8] c"%s\0A\00"

declare i32 @printf(i8*, ...)

define i8 @shift(i8 %0, i32 %1) {
entry:
  %c = alloca i8, align 1
  %k = alloca i32, align 4
  %n = alloca i32, align 4
  store i8 %0, i8* %c, align 1
  store i32 %1, i32* %k, align 4
  %load.c = load i8, i8* %c, align 1
  %LT = icmp slt i8 %load.c, 97
  %load.c1 = load i8, i8* %c, align 1
  %GT = icmp sgt i8 %load.c1, 122
  %OR = or i1 %LT, %GT
  br i1 %OR, label %then, label %endif

then:                                             ; preds = %entry
  %load.c2 = load i8, i8* %c, align 1
  ret i8 %load.c2

endif:                                            ; preds = %entry
  %load.c3 = load i8, i8* %c, align 1
  %"(i32)load.c" = sext i8 %load.c3 to i32
  %SUB = sub i32 %"(i32)load.c", 97
  store i32 %SUB, i32* %n, align 4
  %load.n = load i32, i32* %n, align 4
  %load.k = load i32, i32* %k, align 4
  %ADD = add i32 %load.n, %load.k
  %MOD = srem i32 %ADD, 26
  store i32 %MOD, i32* %n, align 4
  %load.n4 = load i32, i32* %n, align 4
  %ADD5 = add i32 %load.n4, 97
  %"(i8)load.n + (i32)'a'" = trunc i32 %ADD5 to i8
  ret i8 %"(i8)load.n + (i32)'a'"
}

define void @encode(i8* %0, i32 %1) {
entry:
  %s = alloca i8*, align 8
  %k = alloca i32, align 4
  %i = alloca i32, align 4
  store i8* %0, i8** %s, align 8
  store i32 %1, i32* %k, align 4
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %then, %entry
  %load.s = load i8*, i8** %s, align 8
  %load.i = load i32, i32* %i, align 4
  %"load.s[load.i]" = getelementptr inbounds i8, i8* %load.s, i32 %load.i
  %"load.load.s[load.i]" = load i8, i8* %"load.s[load.i]", align 1
  %NQ = icmp ne i8 %"load.load.s[load.i]", 0
  br i1 %NQ, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.s1 = load i8*, i8** %s, align 8
  %load.i2 = load i32, i32* %i, align 4
  %"load.s[load.i]3" = getelementptr inbounds i8, i8* %load.s1, i32 %load.i2
  %load.s4 = load i8*, i8** %s, align 8
  %load.i5 = load i32, i32* %i, align 4
  %"load.s[load.i]6" = getelementptr inbounds i8, i8* %load.s4, i32 %load.i5
  %"load.load.s[load.i]7" = load i8, i8* %"load.s[load.i]6", align 1
  %load.k = load i32, i32* %k, align 4
  %"shift(load.load.s[load.i],load.k)" = call i8 @shift(i8 %"load.load.s[load.i]7", i32 %load.k)
  store i8 %"shift(load.load.s[load.i],load.k)", i8* %"load.s[load.i]3", align 1
  %old = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  ret void
}

define i32 @main() {
entry:
  %s = alloca i8*, align 8
  %s1 = alloca [13 x i8], align 1
  %s2 = alloca [6 x i8], align 1
  %s6 = alloca [4 x i8], align 1
  %dst = getelementptr inbounds [13 x i8], [13 x i8]* %s1, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([13 x i8], [13 x i8]* @str, i32 0, i32 0), i64 13, i1 false)
  store i8* %dst, i8** %s, align 8
  %load.s = load i8*, i8** %s, align 8
  call void @encode(i8* %load.s, i32 3)
  %dst3 = getelementptr inbounds [6 x i8], [6 x i8]* %s2, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst3, i8* align 1 getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i64 6, i1 false)
  %load.s4 = load i8*, i8** %s, align 8
  %"printf(str,load.s)" = call i32 (i8*, ...) @printf(i8* %dst3, i8* %load.s4)
  %load.s5 = load i8*, i8** %s, align 8
  call void @encode(i8* %load.s5, i32 23)
  %dst7 = getelementptr inbounds [4 x i8], [4 x i8]* %s6, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst7, i8* align 1 getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i64 4, i1 false)
  %load.s8 = load i8*, i8** %s, align 8
  %"printf(str,load.s)9" = call i32 (i8*, ...) @printf(i8* %dst7, i8* %load.s8)
  %load.s10 = load i8*, i8** %s, align 8
  %"load.s[0]" = getelementptr inbounds i8, i8* %load.s10, i32 0
  %"load.load.s[0]" = load i8, i8* %"load.s[0]", align 1
  %"(i32)load.load.s[0]" = sext i8 %"load.load.s[0]" to i32
  ret i32 %"(i32)load.load.s[0]"
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 032 — crazy: print the alphabet with a char counter

```ura
// stdout: abcde
// expect: 102
proto printf(fmt i8[], ...) i32

fn main() i32:
    c char = 'a'
    while c <= 'e':
        printf("%c", c)
        c = ((c as i32) + 1) as char
    printf("\n")
    return c as i32
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [3 x i8] c"%c\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %c = alloca i8, align 1
  store i8 97, i8* %c, align 1
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$c" = load i8, i8* %c, align 1
  %LE = icmp sle i8 %"$c", 101
  br i1 %LE, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$c1" = load i8, i8* %c, align 1
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i8 %"$c1")
  %"$c2" = load i8, i8* %c, align 1
  %AS = sext i8 %"$c2" to i32
  %ADD = add i32 %AS, 1
  %AS3 = trunc i32 %ADD to i8
  store i8 %AS3, i8* %c, align 1
  br label %while0

endloop:                                          ; preds = %while0
  %call.printf4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %"$c5" = load i8, i8* %c, align 1
  %AS6 = sext i8 %"$c5" to i32
  ret i32 %AS6
}
```

---

## 033 — crazy: isNullable on array elements and chars

```ura
// expect: 33   (3 zeros * 10, plus 1, plus 2)
fn count_zero(a i32[], n i32) i32:
    c i32 = 0
    i i32 = 0
    while i < n:
        if isNullable(a[i]): c += 1
        i += 1
    return c

fn main() i32:
    a i32[] = [0, 5, 0, 0, 7]
    s i8[] = "ab"
    r i32 = count_zero(a, 5) * 10
    if isNullable(s[2]): r += 1
    if !isNullable(s[0]): r += 2
    return r
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private constant [3 x i8] c"ab\00"

define i32 @count_zero(i32* %0, i32 %1) {
entry:
  %a = alloca i32*, align 8
  %n = alloca i32, align 4
  %c = alloca i32, align 4
  %i = alloca i32, align 4
  store i32* %0, i32** %a, align 8
  store i32 %1, i32* %n, align 4
  store i32 0, i32* %c, align 4
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.i = load i32, i32* %i, align 4
  %load.n = load i32, i32* %n, align 4
  %LT = icmp slt i32 %load.i, %load.n
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.a = load i32*, i32** %a, align 8
  %load.i2 = load i32, i32* %i, align 4
  %"load.a[load.i]" = getelementptr inbounds i32, i32* %load.a, i32 %load.i2
  %"load.load.a[load.i]" = load i32, i32* %"load.a[load.i]", align 4
  %"isNullable(load.load.a[load.i])" = icmp eq i32 %"load.load.a[load.i]", 0
  br i1 %"isNullable(load.load.a[load.i])", label %then1, label %endif

endwhile:                                         ; preds = %cond
  %load.c = load i32, i32* %c, align 4
  ret i32 %load.c

then1:                                            ; preds = %then
  %old = load i32, i32* %c, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %c, align 4
  br label %endif

endif:                                            ; preds = %then1, %then
  %old3 = load i32, i32* %i, align 4
  %ADD_ASSIGN4 = add i32 %old3, 1
  store i32 %ADD_ASSIGN4, i32* %i, align 4
  br label %cond
}

define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %array = alloca [5 x i32], align 4
  %s = alloca i8*, align 8
  %s6 = alloca [3 x i8], align 1
  %r = alloca i32, align 4
  %e = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 0
  store i32 0, i32* %e, align 4
  %e1 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 1
  store i32 5, i32* %e1, align 4
  %e2 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 2
  store i32 0, i32* %e2, align 4
  %e3 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 3
  store i32 0, i32* %e3, align 4
  %e4 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 4
  store i32 7, i32* %e4, align 4
  %array5 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 0
  store i32* %array5, i32** %a, align 8
  %dst = getelementptr inbounds [3 x i8], [3 x i8]* %s6, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i64 3, i1 false)
  store i8* %dst, i8** %s, align 8
  %load.a = load i32*, i32** %a, align 8
  %"count_zero(load.a,5)" = call i32 @count_zero(i32* %load.a, i32 5)
  %MUL = mul i32 %"count_zero(load.a,5)", 10
  store i32 %MUL, i32* %r, align 4
  %load.s = load i8*, i8** %s, align 8
  %"load.s[2]" = getelementptr inbounds i8, i8* %load.s, i32 2
  %"load.load.s[2]" = load i8, i8* %"load.s[2]", align 1
  %"isNullable(load.load.s[2])" = icmp eq i8 %"load.load.s[2]", 0
  br i1 %"isNullable(load.load.s[2])", label %then, label %endif

then:                                             ; preds = %entry
  %old = load i32, i32* %r, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %r, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %load.s9 = load i8*, i8** %s, align 8
  %"load.s[0]" = getelementptr inbounds i8, i8* %load.s9, i32 0
  %"load.load.s[0]" = load i8, i8* %"load.s[0]", align 1
  %"isNullable(load.load.s[0])" = icmp eq i8 %"load.load.s[0]", 0
  %NOT = xor i1 %"isNullable(load.load.s[0])", true
  br i1 %NOT, label %then8, label %endif7

then8:                                            ; preds = %endif
  %old10 = load i32, i32* %r, align 4
  %ADD_ASSIGN11 = add i32 %old10, 2
  store i32 %ADD_ASSIGN11, i32* %r, align 4
  br label %endif7

endif7:                                           ; preds = %then8, %endif
  %load.r = load i32, i32* %r, align 4
  ret i32 %load.r
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 034 — crazy: transpose a 3x3 matrix stored flat

```ura
// expect: 154   (trace 15 * 10, plus t[1] = 4)
fn get(m i32[], w i32, r i32, c i32) i32:
    return m[r * w + c]

fn main() i32:
    m i32[] = [1, 2, 3, 4, 5, 6, 7, 8, 9]
    t i32[] = [0, 0, 0, 0, 0, 0, 0, 0, 0]
    r i32 = 0
    while r < 3:
        c i32 = 0
        while c < 3:
            t[c * 3 + r] = get(m, 3, r, c)
            c += 1
        r += 1
    trace i32 = 0
    i i32 = 0
    while i < 3:
        trace += t[i * 3 + i]
        i += 1
    return trace * 10 + t[1]
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @get(i32* %0, i32 %1, i32 %2, i32 %3) {
entry:
  %m = alloca i32*, align 8
  %w = alloca i32, align 4
  %r = alloca i32, align 4
  %c = alloca i32, align 4
  store i32* %0, i32** %m, align 8
  store i32 %1, i32* %w, align 4
  store i32 %2, i32* %r, align 4
  store i32 %3, i32* %c, align 4
  %load.m = load i32*, i32** %m, align 8
  %load.r = load i32, i32* %r, align 4
  %load.w = load i32, i32* %w, align 4
  %MUL = mul i32 %load.r, %load.w
  %load.c = load i32, i32* %c, align 4
  %ADD = add i32 %MUL, %load.c
  %"load.m[load.r * load.w + load.c]" = getelementptr inbounds i32, i32* %load.m, i32 %ADD
  %"load.load.m[load.r * load.w + load.c]" = load i32, i32* %"load.m[load.r * load.w + load.c]", align 4
  ret i32 %"load.load.m[load.r * load.w + load.c]"
}

define i32 @main() {
entry:
  %m = alloca i32*, align 8
  %array = alloca [9 x i32], align 4
  %t = alloca i32*, align 8
  %array10 = alloca [9 x i32], align 4
  %r = alloca i32, align 4
  %c = alloca i32, align 4
  %trace = alloca i32, align 4
  %i = alloca i32, align 4
  %e = getelementptr inbounds [9 x i32], [9 x i32]* %array, i32 0, i32 0
  store i32 1, i32* %e, align 4
  %e1 = getelementptr inbounds [9 x i32], [9 x i32]* %array, i32 0, i32 1
  store i32 2, i32* %e1, align 4
  %e2 = getelementptr inbounds [9 x i32], [9 x i32]* %array, i32 0, i32 2
  store i32 3, i32* %e2, align 4
  %e3 = getelementptr inbounds [9 x i32], [9 x i32]* %array, i32 0, i32 3
  store i32 4, i32* %e3, align 4
  %e4 = getelementptr inbounds [9 x i32], [9 x i32]* %array, i32 0, i32 4
  store i32 5, i32* %e4, align 4
  %e5 = getelementptr inbounds [9 x i32], [9 x i32]* %array, i32 0, i32 5
  store i32 6, i32* %e5, align 4
  %e6 = getelementptr inbounds [9 x i32], [9 x i32]* %array, i32 0, i32 6
  store i32 7, i32* %e6, align 4
  %e7 = getelementptr inbounds [9 x i32], [9 x i32]* %array, i32 0, i32 7
  store i32 8, i32* %e7, align 4
  %e8 = getelementptr inbounds [9 x i32], [9 x i32]* %array, i32 0, i32 8
  store i32 9, i32* %e8, align 4
  %array9 = getelementptr inbounds [9 x i32], [9 x i32]* %array, i32 0, i32 0
  store i32* %array9, i32** %m, align 8
  %e11 = getelementptr inbounds [9 x i32], [9 x i32]* %array10, i32 0, i32 0
  store i32 0, i32* %e11, align 4
  %e12 = getelementptr inbounds [9 x i32], [9 x i32]* %array10, i32 0, i32 1
  store i32 0, i32* %e12, align 4
  %e13 = getelementptr inbounds [9 x i32], [9 x i32]* %array10, i32 0, i32 2
  store i32 0, i32* %e13, align 4
  %e14 = getelementptr inbounds [9 x i32], [9 x i32]* %array10, i32 0, i32 3
  store i32 0, i32* %e14, align 4
  %e15 = getelementptr inbounds [9 x i32], [9 x i32]* %array10, i32 0, i32 4
  store i32 0, i32* %e15, align 4
  %e16 = getelementptr inbounds [9 x i32], [9 x i32]* %array10, i32 0, i32 5
  store i32 0, i32* %e16, align 4
  %e17 = getelementptr inbounds [9 x i32], [9 x i32]* %array10, i32 0, i32 6
  store i32 0, i32* %e17, align 4
  %e18 = getelementptr inbounds [9 x i32], [9 x i32]* %array10, i32 0, i32 7
  store i32 0, i32* %e18, align 4
  %e19 = getelementptr inbounds [9 x i32], [9 x i32]* %array10, i32 0, i32 8
  store i32 0, i32* %e19, align 4
  %array20 = getelementptr inbounds [9 x i32], [9 x i32]* %array10, i32 0, i32 0
  store i32* %array20, i32** %t, align 8
  store i32 0, i32* %r, align 4
  br label %cond

cond:                                             ; preds = %endwhile23, %entry
  %load.r = load i32, i32* %r, align 4
  %LT = icmp slt i32 %load.r, 3
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  store i32 0, i32* %c, align 4
  br label %cond21

endwhile:                                         ; preds = %cond
  store i32 0, i32* %trace, align 4
  store i32 0, i32* %i, align 4
  br label %cond31

cond21:                                           ; preds = %then22, %then
  %load.c = load i32, i32* %c, align 4
  %LT24 = icmp slt i32 %load.c, 3
  br i1 %LT24, label %then22, label %endwhile23

then22:                                           ; preds = %cond21
  %load.t = load i32*, i32** %t, align 8
  %load.c25 = load i32, i32* %c, align 4
  %MUL = mul i32 %load.c25, 3
  %load.r26 = load i32, i32* %r, align 4
  %ADD = add i32 %MUL, %load.r26
  %"load.t[load.c * 3 + load.r]" = getelementptr inbounds i32, i32* %load.t, i32 %ADD
  %load.m = load i32*, i32** %m, align 8
  %load.r27 = load i32, i32* %r, align 4
  %load.c28 = load i32, i32* %c, align 4
  %"get(load.m,3,load.r,load.c)" = call i32 @get(i32* %load.m, i32 3, i32 %load.r27, i32 %load.c28)
  store i32 %"get(load.m,3,load.r,load.c)", i32* %"load.t[load.c * 3 + load.r]", align 4
  %old = load i32, i32* %c, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %c, align 4
  br label %cond21

endwhile23:                                       ; preds = %cond21
  %old29 = load i32, i32* %r, align 4
  %ADD_ASSIGN30 = add i32 %old29, 1
  store i32 %ADD_ASSIGN30, i32* %r, align 4
  br label %cond

cond31:                                           ; preds = %then32, %endwhile
  %load.i = load i32, i32* %i, align 4
  %LT34 = icmp slt i32 %load.i, 3
  br i1 %LT34, label %then32, label %endwhile33

then32:                                           ; preds = %cond31
  %load.t35 = load i32*, i32** %t, align 8
  %load.i36 = load i32, i32* %i, align 4
  %MUL37 = mul i32 %load.i36, 3
  %load.i38 = load i32, i32* %i, align 4
  %ADD39 = add i32 %MUL37, %load.i38
  %"load.t[load.i * 3 + load.i]" = getelementptr inbounds i32, i32* %load.t35, i32 %ADD39
  %"load.load.t[load.i * 3 + load.i]" = load i32, i32* %"load.t[load.i * 3 + load.i]", align 4
  %old40 = load i32, i32* %trace, align 4
  %ADD_ASSIGN41 = add i32 %old40, %"load.load.t[load.i * 3 + load.i]"
  store i32 %ADD_ASSIGN41, i32* %trace, align 4
  %old42 = load i32, i32* %i, align 4
  %ADD_ASSIGN43 = add i32 %old42, 1
  store i32 %ADD_ASSIGN43, i32* %i, align 4
  br label %cond31

endwhile33:                                       ; preds = %cond31
  %load.trace = load i32, i32* %trace, align 4
  %MUL44 = mul i32 %load.trace, 10
  %load.t45 = load i32*, i32** %t, align 8
  %"load.t[1]" = getelementptr inbounds i32, i32* %load.t45, i32 1
  %"load.load.t[1]" = load i32, i32* %"load.t[1]", align 4
  %ADD46 = add i32 %MUL44, %"load.load.t[1]"
  ret i32 %ADD46
}
```

---

## 035 — crazy: a ref to an array element

```ura
// expect: 92   (10 + 22 + 60)
fn inc(p &i32):
    p += 1

fn main() i32:
    a i32[] = [10, 20, 30]
    inc(&a[1])
    inc(&a[1])
    p &i32 = &a[2]
    p *= 2
    return a[0] + a[1] + a[2]
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define void @inc(i32* %0) {
entry:
  %p = alloca i32*, align 8
  store i32* %0, i32** %p, align 8
  %dref.p = load i32*, i32** %p, align 8
  %old = load i32, i32* %dref.p, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %dref.p, align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %array = alloca [3 x i32], align 4
  %p = alloca i32*, align 8
  %e = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32 10, i32* %e, align 4
  %e1 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 1
  store i32 20, i32* %e1, align 4
  %e2 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 2
  store i32 30, i32* %e2, align 4
  %array3 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32* %array3, i32** %a, align 8
  %load.a = load i32*, i32** %a, align 8
  %"load.a[1]" = getelementptr inbounds i32, i32* %load.a, i32 1
  call void @inc(i32* %"load.a[1]")
  %load.a4 = load i32*, i32** %a, align 8
  %"load.a[1]5" = getelementptr inbounds i32, i32* %load.a4, i32 1
  call void @inc(i32* %"load.a[1]5")
  %load.a6 = load i32*, i32** %a, align 8
  %"load.a[2]" = getelementptr inbounds i32, i32* %load.a6, i32 2
  store i32* %"load.a[2]", i32** %p, align 8
  %dref.p = load i32*, i32** %p, align 8
  %old = load i32, i32* %dref.p, align 4
  %MUL_ASSIGN = mul i32 %old, 2
  store i32 %MUL_ASSIGN, i32* %dref.p, align 4
  %load.a7 = load i32*, i32** %a, align 8
  %"load.a[0]" = getelementptr inbounds i32, i32* %load.a7, i32 0
  %"load.load.a[0]" = load i32, i32* %"load.a[0]", align 4
  %load.a8 = load i32*, i32** %a, align 8
  %"load.a[1]9" = getelementptr inbounds i32, i32* %load.a8, i32 1
  %"load.load.a[1]" = load i32, i32* %"load.a[1]9", align 4
  %ADD = add i32 %"load.load.a[0]", %"load.load.a[1]"
  %load.a10 = load i32*, i32** %a, align 8
  %"load.a[2]11" = getelementptr inbounds i32, i32* %load.a10, i32 2
  %"load.load.a[2]" = load i32, i32* %"load.a[2]11", align 4
  %ADD12 = add i32 %ADD, %"load.load.a[2]"
  ret i32 %ADD12
}
```

---

## 036 — crazy: an array literal made of calls

```ura
// expect: 50   (1 + 4 + 9 + 20 + 16)
fn sq(x i32) i32:
    return x * x

fn main() i32:
    k i32 = 2
    a i32[] = [sq(1), sq(k), sq(k + 1), k * 10, sq(sq(k))]
    return a[0] + a[1] + a[2] + a[3] + a[4]
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @sq(i32 %0) {
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
  %k = alloca i32, align 4
  %a = alloca i32*, align 8
  %array = alloca [5 x i32], align 4
  store i32 2, i32* %k, align 4
  %"sq(1)" = call i32 @sq(i32 1)
  %load.k = load i32, i32* %k, align 4
  %"sq(load.k)" = call i32 @sq(i32 %load.k)
  %load.k1 = load i32, i32* %k, align 4
  %ADD = add i32 %load.k1, 1
  %"sq(load.k + 1)" = call i32 @sq(i32 %ADD)
  %load.k2 = load i32, i32* %k, align 4
  %MUL = mul i32 %load.k2, 10
  %load.k3 = load i32, i32* %k, align 4
  %"sq(load.k)4" = call i32 @sq(i32 %load.k3)
  %"sq(sq(load.k))" = call i32 @sq(i32 %"sq(load.k)4")
  %e = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 0
  store i32 %"sq(1)", i32* %e, align 4
  %e5 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 1
  store i32 %"sq(load.k)", i32* %e5, align 4
  %e6 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 2
  store i32 %"sq(load.k + 1)", i32* %e6, align 4
  %e7 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 3
  store i32 %MUL, i32* %e7, align 4
  %e8 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 4
  store i32 %"sq(sq(load.k))", i32* %e8, align 4
  %array9 = getelementptr inbounds [5 x i32], [5 x i32]* %array, i32 0, i32 0
  store i32* %array9, i32** %a, align 8
  %load.a = load i32*, i32** %a, align 8
  %"load.a[0]" = getelementptr inbounds i32, i32* %load.a, i32 0
  %"load.load.a[0]" = load i32, i32* %"load.a[0]", align 4
  %load.a10 = load i32*, i32** %a, align 8
  %"load.a[1]" = getelementptr inbounds i32, i32* %load.a10, i32 1
  %"load.load.a[1]" = load i32, i32* %"load.a[1]", align 4
  %ADD11 = add i32 %"load.load.a[0]", %"load.load.a[1]"
  %load.a12 = load i32*, i32** %a, align 8
  %"load.a[2]" = getelementptr inbounds i32, i32* %load.a12, i32 2
  %"load.load.a[2]" = load i32, i32* %"load.a[2]", align 4
  %ADD13 = add i32 %ADD11, %"load.load.a[2]"
  %load.a14 = load i32*, i32** %a, align 8
  %"load.a[3]" = getelementptr inbounds i32, i32* %load.a14, i32 3
  %"load.load.a[3]" = load i32, i32* %"load.a[3]", align 4
  %ADD15 = add i32 %ADD13, %"load.load.a[3]"
  %load.a16 = load i32*, i32** %a, align 8
  %"load.a[4]" = getelementptr inbounds i32, i32* %load.a16, i32 4
  %"load.load.a[4]" = load i32, i32* %"load.a[4]", align 4
  %ADD17 = add i32 %ADD15, %"load.load.a[4]"
  ret i32 %ADD17
}
```

---

## 037 — crazy: elif chain on one line each, with not / and / or

```ura
// expect: 31
fn grade(n i32) char:
    if n >= 90: return 'A'
    elif n >= 80 and not (n == 85): return 'B'
    elif n == 85 or n >= 70: return 'C'
    else: return 'F'

fn main() i32:
    r i32 = 0
    if grade(95) == 'A': r += 1
    if grade(82) == 'B': r += 2
    if grade(85) == 'C': r += 4
    if grade(72) == 'C': r += 8
    if grade(50) == 'F': r += 16
    return r
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i8 @grade(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %load.n = load i32, i32* %n, align 4
  %GE = icmp sge i32 %load.n, 90
  br i1 %GE, label %then, label %next

then:                                             ; preds = %entry
  ret i8 65

next:                                             ; preds = %entry
  %load.n3 = load i32, i32* %n, align 4
  %GE4 = icmp sge i32 %load.n3, 80
  %load.n5 = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %load.n5, 85
  %NOT = xor i1 %EQ, true
  %AND = and i1 %GE4, %NOT
  br i1 %AND, label %then1, label %next2

then1:                                            ; preds = %next
  ret i8 66

next2:                                            ; preds = %next
  %load.n8 = load i32, i32* %n, align 4
  %EQ9 = icmp eq i32 %load.n8, 85
  %load.n10 = load i32, i32* %n, align 4
  %GE11 = icmp sge i32 %load.n10, 70
  %OR = or i1 %EQ9, %GE11
  br i1 %OR, label %then6, label %next7

then6:                                            ; preds = %next2
  ret i8 67

next7:                                            ; preds = %next2
  ret i8 70

endif:                                            ; No predecessors!
  ret i8 0
}

define i32 @main() {
entry:
  %r = alloca i32, align 4
  store i32 0, i32* %r, align 4
  %"grade(95)" = call i8 @grade(i32 95)
  %EQ = icmp eq i8 %"grade(95)", 65
  br i1 %EQ, label %then, label %endif

then:                                             ; preds = %entry
  %old = load i32, i32* %r, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %r, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %"grade(82)" = call i8 @grade(i32 82)
  %EQ3 = icmp eq i8 %"grade(82)", 66
  br i1 %EQ3, label %then2, label %endif1

then2:                                            ; preds = %endif
  %old4 = load i32, i32* %r, align 4
  %ADD_ASSIGN5 = add i32 %old4, 2
  store i32 %ADD_ASSIGN5, i32* %r, align 4
  br label %endif1

endif1:                                           ; preds = %then2, %endif
  %"grade(85)" = call i8 @grade(i32 85)
  %EQ8 = icmp eq i8 %"grade(85)", 67
  br i1 %EQ8, label %then7, label %endif6

then7:                                            ; preds = %endif1
  %old9 = load i32, i32* %r, align 4
  %ADD_ASSIGN10 = add i32 %old9, 4
  store i32 %ADD_ASSIGN10, i32* %r, align 4
  br label %endif6

endif6:                                           ; preds = %then7, %endif1
  %"grade(72)" = call i8 @grade(i32 72)
  %EQ13 = icmp eq i8 %"grade(72)", 67
  br i1 %EQ13, label %then12, label %endif11

then12:                                           ; preds = %endif6
  %old14 = load i32, i32* %r, align 4
  %ADD_ASSIGN15 = add i32 %old14, 8
  store i32 %ADD_ASSIGN15, i32* %r, align 4
  br label %endif11

endif11:                                          ; preds = %then12, %endif6
  %"grade(50)" = call i8 @grade(i32 50)
  %EQ18 = icmp eq i8 %"grade(50)", 70
  br i1 %EQ18, label %then17, label %endif16

then17:                                           ; preds = %endif11
  %old19 = load i32, i32* %r, align 4
  %ADD_ASSIGN20 = add i32 %old19, 16
  store i32 %ADD_ASSIGN20, i32* %r, align 4
  br label %endif16

endif16:                                          ; preds = %then17, %endif11
  %load.r = load i32, i32* %r, align 4
  ret i32 %load.r
}
```

---

## 038 — crazy: reverse a string, palindromes, a while on one line

```ura
// stdout: gnal aru
// expect: 83   (1 + 2, plus length 8 * 10)
fn len(s i8[]) i32:
    n i32 = 0
    while s[n] != '\0': n += 1
    return n

fn is_palindrome(s i8[]) bool:
    i i32 = 0
    j i32 = len(s) - 1
    while i < j:
        if s[i] != s[j]: return False
        i += 1
        j -= 1
    return True

fn reverse(s i8[]):
    i i32 = 0
    j i32 = len(s) - 1
    while i < j:
        t char = s[i]
        s[i] = s[j]
        s[j] = t
        i += 1
        j -= 1

proto printf(fmt i8[], ...) i32

fn main() i32:
    a i8[] = "racecar"
    b i8[] = "ura lang"
    reverse(b)
    printf("%s\n", b)
    r i32 = 0
    if is_palindrome(a): r += 1
    if not is_palindrome(b): r += 2
    return r + len(b) * 10
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private constant [8 x i8] c"racecar\00"
@str.1 = private constant [9 x i8] c"ura lang\00"
@str.2 = private constant [4 x i8] c"%s\0A\00"

define i32 @len(i8* %0) {
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

define i1 @is_palindrome(i8* %0) {
entry:
  %s = alloca i8*, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  store i8* %0, i8** %s, align 8
  store i32 0, i32* %i, align 4
  %load.s = load i8*, i8** %s, align 8
  %"len(load.s)" = call i32 @len(i8* %load.s)
  %SUB = sub i32 %"len(load.s)", 1
  store i32 %SUB, i32* %j, align 4
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.i = load i32, i32* %i, align 4
  %load.j = load i32, i32* %j, align 4
  %LT = icmp slt i32 %load.i, %load.j
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.s2 = load i8*, i8** %s, align 8
  %load.i3 = load i32, i32* %i, align 4
  %"load.s[load.i]" = getelementptr inbounds i8, i8* %load.s2, i32 %load.i3
  %"load.load.s[load.i]" = load i8, i8* %"load.s[load.i]", align 1
  %load.s4 = load i8*, i8** %s, align 8
  %load.j5 = load i32, i32* %j, align 4
  %"load.s[load.j]" = getelementptr inbounds i8, i8* %load.s4, i32 %load.j5
  %"load.load.s[load.j]" = load i8, i8* %"load.s[load.j]", align 1
  %NQ = icmp ne i8 %"load.load.s[load.i]", %"load.load.s[load.j]"
  br i1 %NQ, label %then1, label %endif

endwhile:                                         ; preds = %cond
  ret i1 true

then1:                                            ; preds = %then
  ret i1 false

endif:                                            ; preds = %then
  %old = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  %old6 = load i32, i32* %j, align 4
  %SUB_ASSIGN = sub i32 %old6, 1
  store i32 %SUB_ASSIGN, i32* %j, align 4
  br label %cond
}

define void @reverse(i8* %0) {
entry:
  %s = alloca i8*, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %t = alloca i8, align 1
  store i8* %0, i8** %s, align 8
  store i32 0, i32* %i, align 4
  %load.s = load i8*, i8** %s, align 8
  %"len(load.s)" = call i32 @len(i8* %load.s)
  %SUB = sub i32 %"len(load.s)", 1
  store i32 %SUB, i32* %j, align 4
  br label %cond

cond:                                             ; preds = %then, %entry
  %load.i = load i32, i32* %i, align 4
  %load.j = load i32, i32* %j, align 4
  %LT = icmp slt i32 %load.i, %load.j
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.s1 = load i8*, i8** %s, align 8
  %load.i2 = load i32, i32* %i, align 4
  %"load.s[load.i]" = getelementptr inbounds i8, i8* %load.s1, i32 %load.i2
  %"load.load.s[load.i]" = load i8, i8* %"load.s[load.i]", align 1
  store i8 %"load.load.s[load.i]", i8* %t, align 1
  %load.s3 = load i8*, i8** %s, align 8
  %load.i4 = load i32, i32* %i, align 4
  %"load.s[load.i]5" = getelementptr inbounds i8, i8* %load.s3, i32 %load.i4
  %load.s6 = load i8*, i8** %s, align 8
  %load.j7 = load i32, i32* %j, align 4
  %"load.s[load.j]" = getelementptr inbounds i8, i8* %load.s6, i32 %load.j7
  %"load.load.s[load.j]" = load i8, i8* %"load.s[load.j]", align 1
  store i8 %"load.load.s[load.j]", i8* %"load.s[load.i]5", align 1
  %load.s8 = load i8*, i8** %s, align 8
  %load.j9 = load i32, i32* %j, align 4
  %"load.s[load.j]10" = getelementptr inbounds i8, i8* %load.s8, i32 %load.j9
  %load.t = load i8, i8* %t, align 1
  store i8 %load.t, i8* %"load.s[load.j]10", align 1
  %old = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  %old11 = load i32, i32* %j, align 4
  %SUB_ASSIGN = sub i32 %old11, 1
  store i32 %SUB_ASSIGN, i32* %j, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  ret void
}

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %a = alloca i8*, align 8
  %s = alloca [8 x i8], align 1
  %b = alloca i8*, align 8
  %s1 = alloca [9 x i8], align 1
  %s3 = alloca [4 x i8], align 1
  %r = alloca i32, align 4
  %dst = getelementptr inbounds [8 x i8], [8 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([8 x i8], [8 x i8]* @str, i32 0, i32 0), i64 8, i1 false)
  store i8* %dst, i8** %a, align 8
  %dst2 = getelementptr inbounds [9 x i8], [9 x i8]* %s1, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst2, i8* align 1 getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0), i64 9, i1 false)
  store i8* %dst2, i8** %b, align 8
  %load.b = load i8*, i8** %b, align 8
  call void @reverse(i8* %load.b)
  %dst4 = getelementptr inbounds [4 x i8], [4 x i8]* %s3, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst4, i8* align 1 getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i64 4, i1 false)
  %load.b5 = load i8*, i8** %b, align 8
  %"printf(str,load.b)" = call i32 (i8*, ...) @printf(i8* %dst4, i8* %load.b5)
  store i32 0, i32* %r, align 4
  %load.a = load i8*, i8** %a, align 8
  %"is_palindrome(load.a)" = call i1 @is_palindrome(i8* %load.a)
  br i1 %"is_palindrome(load.a)", label %then, label %endif

then:                                             ; preds = %entry
  %old = load i32, i32* %r, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %r, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %load.b8 = load i8*, i8** %b, align 8
  %"is_palindrome(load.b)" = call i1 @is_palindrome(i8* %load.b8)
  %NOT = xor i1 %"is_palindrome(load.b)", true
  br i1 %NOT, label %then7, label %endif6

then7:                                            ; preds = %endif
  %old9 = load i32, i32* %r, align 4
  %ADD_ASSIGN10 = add i32 %old9, 2
  store i32 %ADD_ASSIGN10, i32* %r, align 4
  br label %endif6

endif6:                                           ; preds = %then7, %endif
  %load.r = load i32, i32* %r, align 4
  %load.b11 = load i8*, i8** %b, align 8
  %"len(load.b)" = call i32 @len(i8* %load.b11)
  %MUL = mul i32 %"len(load.b)", 10
  %ADD = add i32 %load.r, %MUL
  ret i32 %ADD
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 039 — crazy: sieve of eratosthenes on a bool array

```ura
// expect: 77   (2 + 3 + 5 + 7 + 11 + 13 + 17 + 19)
fn main() i32:
    composite bool[] = [False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False, False]
    sum i32 = 0
    i i32 = 2
    while i < 20:
        if not composite[i]:
            sum += i
            j i32 = i * i
            while j < 20:
                composite[j] = True
                j += i
        i += 1
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
  %composite = alloca i1*, align 8
  %array = alloca [20 x i1], align 1
  %sum = alloca i32, align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %e = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 0
  store i1 false, i1* %e, align 1
  %e1 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 1
  store i1 false, i1* %e1, align 1
  %e2 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 2
  store i1 false, i1* %e2, align 1
  %e3 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 3
  store i1 false, i1* %e3, align 1
  %e4 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 4
  store i1 false, i1* %e4, align 1
  %e5 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 5
  store i1 false, i1* %e5, align 1
  %e6 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 6
  store i1 false, i1* %e6, align 1
  %e7 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 7
  store i1 false, i1* %e7, align 1
  %e8 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 8
  store i1 false, i1* %e8, align 1
  %e9 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 9
  store i1 false, i1* %e9, align 1
  %e10 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 10
  store i1 false, i1* %e10, align 1
  %e11 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 11
  store i1 false, i1* %e11, align 1
  %e12 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 12
  store i1 false, i1* %e12, align 1
  %e13 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 13
  store i1 false, i1* %e13, align 1
  %e14 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 14
  store i1 false, i1* %e14, align 1
  %e15 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 15
  store i1 false, i1* %e15, align 1
  %e16 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 16
  store i1 false, i1* %e16, align 1
  %e17 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 17
  store i1 false, i1* %e17, align 1
  %e18 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 18
  store i1 false, i1* %e18, align 1
  %e19 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 19
  store i1 false, i1* %e19, align 1
  %array20 = getelementptr inbounds [20 x i1], [20 x i1]* %array, i32 0, i32 0
  store i1* %array20, i1** %composite, align 8
  store i32 0, i32* %sum, align 4
  store i32 2, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 20
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.composite = load i1*, i1** %composite, align 8
  %load.i22 = load i32, i32* %i, align 4
  %"load.composite[load.i]" = getelementptr inbounds i1, i1* %load.composite, i32 %load.i22
  %"load.load.composite[load.i]" = load i1, i1* %"load.composite[load.i]", align 1
  %NOT = xor i1 %"load.load.composite[load.i]", true
  br i1 %NOT, label %then21, label %endif

endwhile:                                         ; preds = %cond
  %load.sum = load i32, i32* %sum, align 4
  ret i32 %load.sum

then21:                                           ; preds = %then
  %load.i23 = load i32, i32* %i, align 4
  %old = load i32, i32* %sum, align 4
  %ADD_ASSIGN = add i32 %old, %load.i23
  store i32 %ADD_ASSIGN, i32* %sum, align 4
  %load.i24 = load i32, i32* %i, align 4
  %load.i25 = load i32, i32* %i, align 4
  %MUL = mul i32 %load.i24, %load.i25
  store i32 %MUL, i32* %j, align 4
  br label %cond26

cond26:                                           ; preds = %then27, %then21
  %load.j = load i32, i32* %j, align 4
  %LT29 = icmp slt i32 %load.j, 20
  br i1 %LT29, label %then27, label %endwhile28

then27:                                           ; preds = %cond26
  %load.composite30 = load i1*, i1** %composite, align 8
  %load.j31 = load i32, i32* %j, align 4
  %"load.composite[load.j]" = getelementptr inbounds i1, i1* %load.composite30, i32 %load.j31
  store i1 true, i1* %"load.composite[load.j]", align 1
  %load.i32 = load i32, i32* %i, align 4
  %old33 = load i32, i32* %j, align 4
  %ADD_ASSIGN34 = add i32 %old33, %load.i32
  store i32 %ADD_ASSIGN34, i32* %j, align 4
  br label %cond26

endwhile28:                                       ; preds = %cond26
  br label %endif

endif:                                            ; preds = %endwhile28, %then
  %old35 = load i32, i32* %i, align 4
  %ADD_ASSIGN36 = add i32 %old35, 1
  store i32 %ADD_ASSIGN36, i32* %i, align 4
  br label %cond
}
```

---

## 040 — crazy: atoi: parse numbers from strings

```ura
// expect: 85   (123 - 45 + 7)
fn atoi(s i8[]) i32:
    n i32 = 0
    i i32 = 0
    neg bool = False
    if s[0] == '-':
        neg = True
        i = 1
    while s[i] >= '0' and s[i] <= '9':
        n = n * 10 + (s[i] as i32) - ('0' as i32)
        i += 1
    if neg: return -n
    return n

fn main() i32:
    return atoi("123") + atoi("-45") + atoi("7x")
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"123\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"-45\00", align 1
@str.2 = private unnamed_addr constant [3 x i8] c"7x\00", align 1

define i32 @atoi(i8* %0) {
entry:
  %s = alloca i8*, align 8
  %n = alloca i32, align 4
  %i = alloca i32, align 4
  %neg = alloca i1, align 1
  store i8* %0, i8** %s, align 8
  store i32 0, i32* %n, align 4
  store i32 0, i32* %i, align 4
  store i1 false, i1* %neg, align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$s" = load i8*, i8** %s, align 8
  %"s[]" = getelementptr inbounds i8, i8* %"$s", i32 0
  %"$s[]" = load i8, i8* %"s[]", align 1
  %EQ = icmp eq i8 %"$s[]", 45
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  store i1 true, i1* %neg, align 1
  store i32 1, i32* %i, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %while1

while1:                                           ; preds = %while1.then, %endif
  %"$s2" = load i8*, i8** %s, align 8
  %"$i" = load i32, i32* %i, align 4
  %"s[]3" = getelementptr inbounds i8, i8* %"$s2", i32 %"$i"
  %"$s[]4" = load i8, i8* %"s[]3", align 1
  %GE = icmp sge i8 %"$s[]4", 48
  %"$s5" = load i8*, i8** %s, align 8
  %"$i6" = load i32, i32* %i, align 4
  %"s[]7" = getelementptr inbounds i8, i8* %"$s5", i32 %"$i6"
  %"$s[]8" = load i8, i8* %"s[]7", align 1
  %LE = icmp sle i8 %"$s[]8", 57
  %AND = and i1 %GE, %LE
  br i1 %AND, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"$n" = load i32, i32* %n, align 4
  %MUL = mul i32 %"$n", 10
  %"$s9" = load i8*, i8** %s, align 8
  %"$i10" = load i32, i32* %i, align 4
  %"s[]11" = getelementptr inbounds i8, i8* %"$s9", i32 %"$i10"
  %"$s[]12" = load i8, i8* %"s[]11", align 1
  %AS = sext i8 %"$s[]12" to i32
  %ADD = add i32 %MUL, %AS
  %SUB = sub i32 %ADD, 48
  store i32 %SUB, i32* %n, align 4
  %"$i13" = load i32, i32* %i, align 4
  %ADD14 = add i32 %"$i13", 1
  store i32 %ADD14, i32* %i, align 4
  br label %while1

endloop:                                          ; preds = %while1
  br label %if2

if2:                                              ; preds = %endloop
  %"$neg" = load i1, i1* %neg, align 1
  br i1 %"$neg", label %if2.then, label %endif1

if2.then:                                         ; preds = %if2
  %"$n15" = load i32, i32* %n, align 4
  %NEG = sub i32 0, %"$n15"
  ret i32 %NEG

endif1:                                           ; preds = %if2
  %"$n16" = load i32, i32* %n, align 4
  ret i32 %"$n16"
}

define i32 @main() {
entry:
  %call.atoi = call i32 @atoi(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  %call.atoi1 = call i32 @atoi(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0))
  %ADD = add i32 %call.atoi, %call.atoi1
  %call.atoi2 = call i32 @atoi(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.2, i32 0, i32 0))
  %ADD3 = add i32 %ADD, %call.atoi2
  ret i32 %ADD3
}
```

---

## 041 — crazy: itoa: write a number's digits into a buffer

```ura
// stdout: 9071
// expect: 57   ('9')
proto printf(fmt i8[], ...) i32

fn itoa(n i32, out i8[]):
    digits i8[] = "0000000000"
    k i32 = 0
    if n == 0:
        out[0] = '0'
        out[1] = '\0'
        return
    while n > 0:
        digits[k] = ((n % 10) + ('0' as i32)) as char
        n /= 10
        k += 1
    i i32 = 0
    while i < k:
        out[i] = digits[k - 1 - i]
        i += 1
    out[k] = '\0'

fn main() i32:
    buf i8[] = "............"
    itoa(9071, buf)
    printf("%s\n", buf)
    return buf[0] as i32
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private constant [11 x i8] c"0000000000\00"
@str.1 = private constant [13 x i8] c"............\00"
@str.2 = private constant [4 x i8] c"%s\0A\00"

declare i32 @printf(i8*, ...)

define void @itoa(i32 %0, i8* %1) {
entry:
  %n = alloca i32, align 4
  %out = alloca i8*, align 8
  %digits = alloca i8*, align 8
  %s = alloca [11 x i8], align 1
  %k = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  store i8* %1, i8** %out, align 8
  %dst = getelementptr inbounds [11 x i8], [11 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([11 x i8], [11 x i8]* @str, i32 0, i32 0), i64 11, i1 false)
  store i8* %dst, i8** %digits, align 8
  store i32 0, i32* %k, align 4
  %load.n = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %load.n, 0
  br i1 %EQ, label %then, label %endif

then:                                             ; preds = %entry
  %load.out = load i8*, i8** %out, align 8
  %"load.out[0]" = getelementptr inbounds i8, i8* %load.out, i32 0
  store i8 48, i8* %"load.out[0]", align 1
  %load.out1 = load i8*, i8** %out, align 8
  %"load.out[1]" = getelementptr inbounds i8, i8* %load.out1, i32 1
  store i8 0, i8* %"load.out[1]", align 1
  ret void

endif:                                            ; preds = %entry
  br label %cond

cond:                                             ; preds = %then2, %endif
  %load.n3 = load i32, i32* %n, align 4
  %GT = icmp sgt i32 %load.n3, 0
  br i1 %GT, label %then2, label %endwhile

then2:                                            ; preds = %cond
  %load.digits = load i8*, i8** %digits, align 8
  %load.k = load i32, i32* %k, align 4
  %"load.digits[load.k]" = getelementptr inbounds i8, i8* %load.digits, i32 %load.k
  %load.n4 = load i32, i32* %n, align 4
  %MOD = srem i32 %load.n4, 10
  %ADD = add i32 %MOD, 48
  %"(i8)load.n % 10 + (i32)'0'" = trunc i32 %ADD to i8
  store i8 %"(i8)load.n % 10 + (i32)'0'", i8* %"load.digits[load.k]", align 1
  %old = load i32, i32* %n, align 4
  %DIV_ASSIGN = sdiv i32 %old, 10
  store i32 %DIV_ASSIGN, i32* %n, align 4
  %old5 = load i32, i32* %k, align 4
  %ADD_ASSIGN = add i32 %old5, 1
  store i32 %ADD_ASSIGN, i32* %k, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  store i32 0, i32* %i, align 4
  br label %cond6

cond6:                                            ; preds = %then7, %endwhile
  %load.i = load i32, i32* %i, align 4
  %load.k9 = load i32, i32* %k, align 4
  %LT = icmp slt i32 %load.i, %load.k9
  br i1 %LT, label %then7, label %endwhile8

then7:                                            ; preds = %cond6
  %load.out10 = load i8*, i8** %out, align 8
  %load.i11 = load i32, i32* %i, align 4
  %"load.out[load.i]" = getelementptr inbounds i8, i8* %load.out10, i32 %load.i11
  %load.digits12 = load i8*, i8** %digits, align 8
  %load.k13 = load i32, i32* %k, align 4
  %SUB = sub i32 %load.k13, 1
  %load.i14 = load i32, i32* %i, align 4
  %SUB15 = sub i32 %SUB, %load.i14
  %"load.digits[load.k - 1 - load.i]" = getelementptr inbounds i8, i8* %load.digits12, i32 %SUB15
  %"load.load.digits[load.k - 1 - load.i]" = load i8, i8* %"load.digits[load.k - 1 - load.i]", align 1
  store i8 %"load.load.digits[load.k - 1 - load.i]", i8* %"load.out[load.i]", align 1
  %old16 = load i32, i32* %i, align 4
  %ADD_ASSIGN17 = add i32 %old16, 1
  store i32 %ADD_ASSIGN17, i32* %i, align 4
  br label %cond6

endwhile8:                                        ; preds = %cond6
  %load.out18 = load i8*, i8** %out, align 8
  %load.k19 = load i32, i32* %k, align 4
  %"load.out[load.k]" = getelementptr inbounds i8, i8* %load.out18, i32 %load.k19
  store i8 0, i8* %"load.out[load.k]", align 1
  ret void
}

define i32 @main() {
entry:
  %buf = alloca i8*, align 8
  %s = alloca [13 x i8], align 1
  %s1 = alloca [4 x i8], align 1
  %dst = getelementptr inbounds [13 x i8], [13 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([13 x i8], [13 x i8]* @str.1, i32 0, i32 0), i64 13, i1 false)
  store i8* %dst, i8** %buf, align 8
  %load.buf = load i8*, i8** %buf, align 8
  call void @itoa(i32 9071, i8* %load.buf)
  %dst2 = getelementptr inbounds [4 x i8], [4 x i8]* %s1, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst2, i8* align 1 getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i64 4, i1 false)
  %load.buf3 = load i8*, i8** %buf, align 8
  %"printf(str,load.buf)" = call i32 (i8*, ...) @printf(i8* %dst2, i8* %load.buf3)
  %load.buf4 = load i8*, i8** %buf, align 8
  %"load.buf[0]" = getelementptr inbounds i8, i8* %load.buf4, i32 0
  %"load.load.buf[0]" = load i8, i8* %"load.buf[0]", align 1
  %"(i32)load.load.buf[0]" = sext i8 %"load.load.buf[0]" to i32
  ret i32 %"(i32)load.load.buf[0]"
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 042 — crazy: print a number in hex with as

```ura
// stdout: 0000beef
// expect: 98   ('b')
proto printf(fmt i8[], ...) i32

fn hex_digit(n i32) char:
    if n < 10: return (('0' as i32) + n) as char
    return (('a' as i32) + n - 10) as char

fn to_hex(n i32, out i8[]):
    i i32 = 7
    while i >= 0:
        out[i] = hex_digit(n % 16)
        n /= 16
        i -= 1
    out[8] = '\0'

buf i8[] = "........."
to_hex(48879, buf)
printf("%s\n", buf)
return buf[4] as i32
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@buf = global i8* null
@str = private constant [10 x i8] c".........\00"
@str.1 = private constant [4 x i8] c"%s\0A\00"

declare i32 @printf(i8*, ...)

define i8 @hex_digit(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %load.n = load i32, i32* %n, align 4
  %LT = icmp slt i32 %load.n, 10
  br i1 %LT, label %then, label %endif

then:                                             ; preds = %entry
  %load.n1 = load i32, i32* %n, align 4
  %ADD = add i32 48, %load.n1
  %"(i8)(i32)'0' + load.n" = trunc i32 %ADD to i8
  ret i8 %"(i8)(i32)'0' + load.n"

endif:                                            ; preds = %entry
  %load.n2 = load i32, i32* %n, align 4
  %ADD3 = add i32 97, %load.n2
  %SUB = sub i32 %ADD3, 10
  %"(i8)(i32)'a' + load.n - 10" = trunc i32 %SUB to i8
  ret i8 %"(i8)(i32)'a' + load.n - 10"
}

define void @to_hex(i32 %0, i8* %1) {
entry:
  %n = alloca i32, align 4
  %out = alloca i8*, align 8
  %i = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  store i8* %1, i8** %out, align 8
  store i32 7, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %then, %entry
  %load.i = load i32, i32* %i, align 4
  %GE = icmp sge i32 %load.i, 0
  br i1 %GE, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.out = load i8*, i8** %out, align 8
  %load.i1 = load i32, i32* %i, align 4
  %"load.out[load.i]" = getelementptr inbounds i8, i8* %load.out, i32 %load.i1
  %load.n = load i32, i32* %n, align 4
  %MOD = srem i32 %load.n, 16
  %"hex_digit(load.n % 16)" = call i8 @hex_digit(i32 %MOD)
  store i8 %"hex_digit(load.n % 16)", i8* %"load.out[load.i]", align 1
  %old = load i32, i32* %n, align 4
  %DIV_ASSIGN = sdiv i32 %old, 16
  store i32 %DIV_ASSIGN, i32* %n, align 4
  %old2 = load i32, i32* %i, align 4
  %SUB_ASSIGN = sub i32 %old2, 1
  store i32 %SUB_ASSIGN, i32* %i, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  %load.out3 = load i8*, i8** %out, align 8
  %"load.out[8]" = getelementptr inbounds i8, i8* %load.out3, i32 8
  store i8 0, i8* %"load.out[8]", align 1
  ret void
}

define i32 @main() {
entry:
  %s = alloca [10 x i8], align 1
  %s1 = alloca [4 x i8], align 1
  %dst = getelementptr inbounds [10 x i8], [10 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i64 10, i1 false)
  store i8* %dst, i8** @buf, align 8
  %load.buf = load i8*, i8** @buf, align 8
  call void @to_hex(i32 48879, i8* %load.buf)
  %dst2 = getelementptr inbounds [4 x i8], [4 x i8]* %s1, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst2, i8* align 1 getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i64 4, i1 false)
  %load.buf3 = load i8*, i8** @buf, align 8
  %"printf(str,load.buf)" = call i32 (i8*, ...) @printf(i8* %dst2, i8* %load.buf3)
  %load.buf4 = load i8*, i8** @buf, align 8
  %"load.buf[4]" = getelementptr inbounds i8, i8* %load.buf4, i32 4
  %"load.load.buf[4]" = load i8, i8* %"load.buf[4]", align 1
  %"(i32)load.load.buf[4]" = sext i8 %"load.load.buf[4]" to i32
  ret i32 %"(i32)load.load.buf[4]"
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 043 — crazy: a string builder on a global buffer

```ura
// stdout: x=1234, y=7
// expect: 12
proto printf(fmt i8[], ...) i32

out i8[] = "                                "
len i32 = 0

fn put(c char):
    out[len] = c
    len += 1

fn put_num(n i32):
    if n >= 10: put_num(n / 10)
    put((('0' as i32) + n % 10) as char)

fn put_str(s i8[]):
    i i32 = 0
    while s[i] != '\0':
        put(s[i])
        i += 1

put_str("x=")
put_num(1234)
put_str(", y=")
put_num(7)
put('\0')
printf("%s\n", out)
return len
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@out = global i8* null
@len = global i32 0
@str = private unnamed_addr constant [33 x i8] c"                                \00", align 1
@str.1 = private unnamed_addr constant [3 x i8] c"x=\00", align 1
@str.2 = private unnamed_addr constant [5 x i8] c", y=\00", align 1
@str.3 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @put(i8 %0) {
entry:
  %c = alloca i8, align 1
  store i8 %0, i8* %c, align 1
  %"$out" = load i8*, i8** @out, align 8
  %"$len" = load i32, i32* @len, align 4
  %"out[]" = getelementptr inbounds i8, i8* %"$out", i32 %"$len"
  %"$c" = load i8, i8* %c, align 1
  store i8 %"$c", i8* %"out[]", align 1
  %"$len1" = load i32, i32* @len, align 4
  %ADD = add i32 %"$len1", 1
  store i32 %ADD, i32* @len, align 4
  ret void
}

define void @put_num(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %GE = icmp sge i32 %"$n", 10
  br i1 %GE, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"$n1" = load i32, i32* %n, align 4
  %DIV = sdiv i32 %"$n1", 10
  call void @put_num(i32 %DIV)
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  %"$n2" = load i32, i32* %n, align 4
  %MOD = srem i32 %"$n2", 10
  %ADD = add i32 48, %MOD
  %AS = trunc i32 %ADD to i8
  call void @put(i8 %AS)
  ret void
}

define void @put_str(i8* %0) {
entry:
  %s = alloca i8*, align 8
  %i = alloca i32, align 4
  store i8* %0, i8** %s, align 8
  store i32 0, i32* %i, align 4
  br label %while1

while1:                                           ; preds = %while1.then, %entry
  %"$s" = load i8*, i8** %s, align 8
  %"$i" = load i32, i32* %i, align 4
  %"s[]" = getelementptr inbounds i8, i8* %"$s", i32 %"$i"
  %"$s[]" = load i8, i8* %"s[]", align 1
  %NQ = icmp ne i8 %"$s[]", 0
  br i1 %NQ, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"$s1" = load i8*, i8** %s, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"s[]3" = getelementptr inbounds i8, i8* %"$s1", i32 %"$i2"
  %"$s[]4" = load i8, i8* %"s[]3", align 1
  call void @put(i8 %"$s[]4")
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define i32 @main() {
entry:
  store i8* getelementptr inbounds ([33 x i8], [33 x i8]* @str, i32 0, i32 0), i8** @out, align 8
  store i32 0, i32* @len, align 4
  call void @put_str(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0))
  call void @put_num(i32 1234)
  call void @put_str(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.2, i32 0, i32 0))
  call void @put_num(i32 7)
  call void @put(i8 0)
  %"$out" = load i8*, i8** @out, align 8
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.3, i32 0, i32 0), i8* %"$out")
  %"$len" = load i32, i32* @len, align 4
  ret i32 %"$len"
}
```

---

## 044 — crazy: filter with as bool, not and isNullable

```ura
// expect: 42   (4 odd, 2 even, zeros skipped)
fn count_if(a i32[], n i32, odd bool) i32:
    c i32 = 0
    i i32 = 0
    while i < n:
        is_odd bool = (a[i] % 2) as bool
        if is_odd == odd and not isNullable(a[i]): c += 1
        i += 1
    return c

a i32[] = [0, 1, 2, 3, 4, 5, -3, 0]
return count_if(a, 8, True) * 10 + count_if(a, 8, False)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@a = global i32* null

define i32 @count_if(i32* %0, i32 %1, i1 %2) {
entry:
  %a = alloca i32*, align 8
  %n = alloca i32, align 4
  %odd = alloca i1, align 1
  %c = alloca i32, align 4
  %i = alloca i32, align 4
  %is_odd = alloca i1, align 1
  store i32* %0, i32** %a, align 8
  store i32 %1, i32* %n, align 4
  store i1 %2, i1* %odd, align 1
  store i32 0, i32* %c, align 4
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.i = load i32, i32* %i, align 4
  %load.n = load i32, i32* %n, align 4
  %LT = icmp slt i32 %load.i, %load.n
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.a = load i32*, i32** %a, align 8
  %load.i1 = load i32, i32* %i, align 4
  %"load.a[load.i]" = getelementptr inbounds i32, i32* %load.a, i32 %load.i1
  %"load.load.a[load.i]" = load i32, i32* %"load.a[load.i]", align 4
  %MOD = srem i32 %"load.load.a[load.i]", 2
  %"(bool)load.load.a[load.i] % 2" = trunc i32 %MOD to i1
  store i1 %"(bool)load.load.a[load.i] % 2", i1* %is_odd, align 1
  %load.is_odd = load i1, i1* %is_odd, align 1
  %load.odd = load i1, i1* %odd, align 1
  %EQ = icmp eq i1 %load.is_odd, %load.odd
  %load.a3 = load i32*, i32** %a, align 8
  %load.i4 = load i32, i32* %i, align 4
  %"load.a[load.i]5" = getelementptr inbounds i32, i32* %load.a3, i32 %load.i4
  %"load.load.a[load.i]6" = load i32, i32* %"load.a[load.i]5", align 4
  %"isNullable(load.load.a[load.i])" = icmp eq i32 %"load.load.a[load.i]6", 0
  %NOT = xor i1 %"isNullable(load.load.a[load.i])", true
  %AND = and i1 %EQ, %NOT
  br i1 %AND, label %then2, label %endif

endwhile:                                         ; preds = %cond
  %load.c = load i32, i32* %c, align 4
  ret i32 %load.c

then2:                                            ; preds = %then
  %old = load i32, i32* %c, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %c, align 4
  br label %endif

endif:                                            ; preds = %then2, %then
  %old7 = load i32, i32* %i, align 4
  %ADD_ASSIGN8 = add i32 %old7, 1
  store i32 %ADD_ASSIGN8, i32* %i, align 4
  br label %cond
}

define i32 @main() {
entry:
  %array = alloca [8 x i32], align 4
  %e = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 0
  store i32 0, i32* %e, align 4
  %e1 = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 1
  store i32 1, i32* %e1, align 4
  %e2 = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 2
  store i32 2, i32* %e2, align 4
  %e3 = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 3
  store i32 3, i32* %e3, align 4
  %e4 = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 4
  store i32 4, i32* %e4, align 4
  %e5 = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 5
  store i32 5, i32* %e5, align 4
  %e6 = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 6
  store i32 -3, i32* %e6, align 4
  %e7 = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 7
  store i32 0, i32* %e7, align 4
  %array8 = getelementptr inbounds [8 x i32], [8 x i32]* %array, i32 0, i32 0
  store i32* %array8, i32** @a, align 8
  %load.a = load i32*, i32** @a, align 8
  %"count_if(load.a,8,True)" = call i32 @count_if(i32* %load.a, i32 8, i1 true)
  %MUL = mul i32 %"count_if(load.a,8,True)", 10
  %load.a9 = load i32*, i32** @a, align 8
  %"count_if(load.a,8,False)" = call i32 @count_if(i32* %load.a9, i32 8, i1 false)
  %ADD = add i32 %MUL, %"count_if(load.a,8,False)"
  ret i32 %ADD
}
```

---

## 045 — parser error: unclosed [ in an array literal

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

## 046 — refs: ref to a ref

```ura
fn main() i32:
    a i32 = 1
    p &&i32 = &a
    return 0
```

### stderr

```
error: test.ura:3:7 a ref can't point to a ref
  |
3 |     p &&i32 = &a
  |       ^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 047 — refs: missing type after &

```ura
fn main() i32:
    a i32 = 1
    p & = &a
    return 0
```

### stderr

```
error: test.ura:3:7 expected a type after '&'
  |
3 |     p & = &a
  |       ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 048 — refs: initialized with a value

```ura
fn main() i32:
    a i32 = 1
    p &i32 = a
    return 0
```

### stderr

```
error: test.ura:3:14 'p' is a ref, initialize it with an address
  |
3 |     p &i32 = a
  |              ^
help: write 'p &i32 = &a'
error: aborting due to 1 error
```

### status

```
1
```

---

## 049 — refs: address of a literal

```ura
fn main() i32:
    p &i32 = &5
    return 0
```

### stderr

```
error: test.ura:2:15 '&' expects a variable or a struct attribute
  |
2 |     p &i32 = &5
  |               ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 050 — refs: address of an expression

```ura
fn main() i32:
    a i32 = 1
    p &i32 = &(a + 1)
    return 0
```

### stderr

```
error: test.ura:3:18 '&' expects a variable or a struct attribute
  |
3 |     p &i32 = &(a + 1)
  |                  ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 051 — refs: address of a call

```ura
fn f() i32:
    return 1

fn main() i32:
    p &i32 = &f()
    return 0
```

### stderr

```
error: test.ura:5:15 '&' expects a variable or a struct attribute
  |
5 |     p &i32 = &f()
  |               ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 052 — refs: address of an address

```ura
fn main() i32:
    a i32 = 1
    p &i32 = &&a
    return 0
```

### stderr

```
error: test.ura:3:14 can't take the address of an address
  |
3 |     p &i32 = &&a
  |              ^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 053 — refs: ref to a different type

```ura
fn main() i32:
    c i8 = 'a'
    p &i32 = &c
    return 0
```

### stderr

```
error: test.ura:3:14 expected '&i32', found '&i8'
  |
3 |     p &i32 = &c
  |              ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 054 — null: assigned to a value

```ura
fn main() i32:
    x i32 = null
    return x
```

### stderr

```
error: test.ura:2:13 expected 'i32', found 'null'
  |
2 |     x i32 = null
  |             ^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 055 — null: compared with a value

```ura
fn main() i32:
    a i32 = 1
    b bool = a == null
    return 0
```

### stderr

```
error: test.ura:3:17 'null' needs a ref on the other side
  |
3 |     b bool = a == null
  |                 ^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 056 — null: address of null

```ura
fn main() i32:
    p &i32 = &null
    return 0
```

### stderr

```
error: test.ura:2:15 '&' expects a variable or a struct attribute
  |
2 |     p &i32 = &null
  |               ^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 057 — expressions: compound assign on chars

```ura
fn main() i32:
    s i8[] = "a"
    s += 1
    return 0
```

### stderr

```
error: test.ura:3:7 can't apply '+=' to 'i8[]' and 'i32'
  |
3 |     s += 1
  |       ^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 058 — expressions: negate a string

```ura
fn main() i32:
    s i8[] = -"a"
    return 0
```

### stderr

```
error: test.ura:2:14 can't apply '-' to 'i8[]'
  |
2 |     s i8[] = -"a"
  |              ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 059 — calls: missing & on a ref argument

```ura
fn f(p &i32):
    p = 1

fn main() i32:
    a i32 = 0
    f(a)
    return a
```

### stderr

```
error: test.ura:6:5 no overload of 'f' takes these arguments
  |
6 |     f(a)
  |     ^
help: 'a' is 'i32', but 'p' wants '&i32'
help: pass its address: '&a'
error: aborting due to 1 error
```

### status

```
1
```

---

## 060 — arrays: unclosed type

```ura
fn main() i32:
    x i32[
    return 0
```

### stderr

```
error: test.ura:2:10 expected ']' after '['
  |
2 |     x i32[
  |          ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 061 — arrays: size in the type

```ura
fn main() i32:
    x i32[3]
    return 0
```

### stderr

```
error: test.ura:2:11 expected ']' after '['
  |
2 |     x i32[3]
  |           ^
help: arrays have no size in the type: 'i32[]'
error: aborting due to 1 error
```

### status

```
1
```

---

## 062 — arrays: index a non-array

```ura
fn main() i32:
    a i32 = 1
    return a[0]
```

### stderr

```
error: test.ura:3:12 'a' is 'i32', not an array
  |
3 |     return a[0]
  |            ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 063 — arrays: index with a string

```ura
fn main() i32:
    x i32[] = [1, 2]
    return x["a"]
```

### stderr

```
error: test.ura:3:14 expected 'i32', found 'i8[]'
  |
3 |     return x["a"]
  |              ^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 064 — arrays: mixed element types

```ura
fn main() i32:
    x i32[] = [1, "a"]
    return 0
```

### stderr

```
error: test.ura:2:19 expected 'i32', found 'i8[]'
  |
2 |     x i32[] = [1, "a"]
  |                   ^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 065 — arrays: array into a scalar

```ura
fn main() i32:
    a i32 = [1, 2]
    return a
```

### stderr

```
error: test.ura:2:13 expected 'i32', found 'i32[]'
  |
2 |     a i32 = [1, 2]
  |             ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 066 — arrays: unclosed literal

```ura
fn main() i32:
    x i32[] = [1, 2
    return 0
```

### stderr

```
error: test.ura:2:15 unclosed '['
  |
2 |     x i32[] = [1, 2
  |               ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 067 — arrays: missing comma between elements

```ura
fn main() i32:
    x i32[] = [1 2]
    return 0
```

### stderr

```
error: test.ura:2:18 expected ',' between elements
  |
2 |     x i32[] = [1 2]
  |                  ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 068 — array: string as i8[] is writable

```ura
fn main() i32:
   s i8[] = "abc"
   s[0] = 'x'
   c i8 = s[1]
   if c == 'b':
      return 1
   return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"abc\00", align 1

define i32 @main() {
entry:
  %s = alloca i8*, align 8
  %c = alloca i8, align 1
  store i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i8** %s, align 8
  %"$s" = load i8*, i8** %s, align 8
  %"s[]" = getelementptr inbounds i8, i8* %"$s", i32 0
  store i8 120, i8* %"s[]", align 1
  %"$s1" = load i8*, i8** %s, align 8
  %"s[]2" = getelementptr inbounds i8, i8* %"$s1", i32 1
  %"$s[]" = load i8, i8* %"s[]2", align 1
  store i8 %"$s[]", i8* %c, align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$c" = load i8, i8* %c, align 1
  %EQ = icmp eq i8 %"$c", 98
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  ret i32 0
}
```

---

## 069 — array: array passed to a function

```ura
fn sum(a i32[]) i32:
   total i32 = 0
   i i32 = 0
   while i < 3:
      total += a[i]
      i += 1
   return total

fn main() i32:
   values i32[] = [1, 2, 3]
   return sum(values)
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@const = private unnamed_addr constant [3 x i32] [i32 1, i32 2, i32 3]

define i32 @sum(i32* %0) {
entry:
  %a = alloca i32*, align 8
  %total = alloca i32, align 4
  %i = alloca i32, align 4
  store i32* %0, i32** %a, align 8
  store i32 0, i32* %total, align 4
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %then, %entry
  %i1 = load i32, i32* %i, align 4
  %LT = icmp slt i32 %i1, 3
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %total2 = load i32, i32* %total, align 4
  %a3 = load i32*, i32** %a, align 8
  %i4 = load i32, i32* %i, align 4
  %1 = getelementptr inbounds i32, i32* %a3, i32 %i4
  %2 = load i32, i32* %1, align 4
  %ADD = add i32 %total2, %2
  store i32 %ADD, i32* %total, align 4
  %i5 = load i32, i32* %i, align 4
  %ADD6 = add i32 %i5, 1
  store i32 %ADD6, i32* %i, align 4
  br label %cond

endwhile:                                         ; preds = %cond
  %total7 = load i32, i32* %total, align 4
  ret i32 %total7
}

define i32 @main() {
entry:
  %values = alloca i32*, align 8
  %array = alloca [3 x i32], align 4
  %0 = bitcast [3 x i32]* %array to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %0, i8* align 1 bitcast ([3 x i32]* @const to i8*), i64 12, i1 false)
  %1 = getelementptr inbounds [3 x i32], [3 x i32]* %array, i32 0, i32 0
  store i32* %1, i32** %values, align 8
  %values1 = load i32*, i32** %values, align 8
  %sum = call i32 @sum(i32* %values1)
  ret i32 %sum
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 070 — ref: write through a ref

```ura
fn main() i32:
    a i32 = 10
    p &i32 = &a
    p = 12
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
  %p = alloca i32*, align 8
  store i32 10, i32* %a, align 4
  store i32* %a, i32** %p, align 8
  %"*p" = load i32*, i32** %p, align 8
  store i32 12, i32* %"*p", align 4
  %"$a" = load i32, i32* %a, align 4
  ret i32 %"$a"
}
```

---

## 071 — ref: ref as parameter

```ura
fn foo(a &i32):
    a = 2

fn main() i32:
    a i32 = 10
    foo(&a)
    return a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define void @foo(i32* %0) {
entry:
  %a = alloca i32*, align 8
  store i32* %0, i32** %a, align 8
  %"*a" = load i32*, i32** %a, align 8
  store i32 2, i32* %"*a", align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 10, i32* %a, align 4
  call void @foo(i32* %a)
  %"$a" = load i32, i32* %a, align 4
  ret i32 %"$a"
}
```

---

## 072 — ref: read through a ref

```ura
fn main() i32:
    a i32 = 10
    p &i32 = &a
    b i32 = p
    return b
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
  %p = alloca i32*, align 8
  %b = alloca i32, align 4
  store i32 10, i32* %a, align 4
  store i32* %a, i32** %p, align 8
  %"*p" = load i32*, i32** %p, align 8
  %"$*p" = load i32, i32* %"*p", align 4
  store i32 %"$*p", i32* %b, align 4
  %"$b" = load i32, i32* %b, align 4
  ret i32 %"$b"
}
```

---

## 073 — ref: math through a ref

```ura
fn main() i32:
    a i32 = 10
    p &i32 = &a
    p = p + 5
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
  %p = alloca i32*, align 8
  store i32 10, i32* %a, align 4
  store i32* %a, i32** %p, align 8
  %"*p" = load i32*, i32** %p, align 8
  %"*p1" = load i32*, i32** %p, align 8
  %"$*p" = load i32, i32* %"*p1", align 4
  %ADD = add i32 %"$*p", 5
  store i32 %ADD, i32* %"*p", align 4
  %"$a" = load i32, i32* %a, align 4
  ret i32 %"$a"
}
```

---

## 074 — ref: copy the pointed value between refs

```ura
fn main() i32:
    a i32 = 1
    b i32 = 2
    p1 &i32 = &a
    p2 &i32 = &b
    p1 = p2
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
  %p1 = alloca i32*, align 8
  %p2 = alloca i32*, align 8
  store i32 1, i32* %a, align 4
  store i32 2, i32* %b, align 4
  store i32* %a, i32** %p1, align 8
  store i32* %b, i32** %p2, align 8
  %"*p1" = load i32*, i32** %p1, align 8
  %"*p2" = load i32*, i32** %p2, align 8
  %"$*p2" = load i32, i32* %"*p2", align 4
  store i32 %"$*p2", i32* %"*p1", align 4
  %"$a" = load i32, i32* %a, align 4
  ret i32 %"$a"
}
```

---

## 075 — ref: rebind a ref

```ura
fn main() i32:
    a i32 = 1
    b i32 = 2
    p1 &i32 = &a
    p2 &i32 = &b
    p1 = &p2
    p1 = 9
    return b
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
  %p1 = alloca i32*, align 8
  %p2 = alloca i32*, align 8
  store i32 1, i32* %a, align 4
  store i32 2, i32* %b, align 4
  store i32* %a, i32** %p1, align 8
  store i32* %b, i32** %p2, align 8
  %"*p2" = load i32*, i32** %p2, align 8
  store i32* %"*p2", i32** %p1, align 8
  %"*p1" = load i32*, i32** %p1, align 8
  store i32 9, i32* %"*p1", align 4
  %"$b" = load i32, i32* %b, align 4
  ret i32 %"$b"
}
```

---

## 076 — function: fn returns ref

```ura
fn same(p &i32) &i32:
    return &p

fn main() i32:
    a i32 = 6
    q &i32 = same(&a)
    return q
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32* @same(i32* %0) {
entry:
  %p = alloca i32*, align 8
  store i32* %0, i32** %p, align 8
  %p1 = load i32*, i32** %p, align 8
  ret i32* %p1
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %q = alloca i32*, align 8
  store i32 6, i32* %a, align 4
  %same = call i32* @same(i32* %a)
  store i32* %same, i32** %q, align 8
  %q1 = load i32*, i32** %q, align 8
  %q.dref = load i32, i32* %q1, align 4
  ret i32 %q.dref
}
```

---

## 077 — function: swap

```ura
fn swap(x &i32, y &i32):
    t i32 = x
    x = y
    y = t

fn main() i32:
    a i32 = 1
    b i32 = 2
    swap(&a, &b)
    return a * 10 + b
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define void @swap(i32* %0, i32* %1) {
entry:
  %x = alloca i32*, align 8
  %y = alloca i32*, align 8
  %t = alloca i32, align 4
  store i32* %0, i32** %x, align 8
  store i32* %1, i32** %y, align 8
  %"*x" = load i32*, i32** %x, align 8
  %"$*x" = load i32, i32* %"*x", align 4
  store i32 %"$*x", i32* %t, align 4
  %"*x1" = load i32*, i32** %x, align 8
  %"*y" = load i32*, i32** %y, align 8
  %"$*y" = load i32, i32* %"*y", align 4
  store i32 %"$*y", i32* %"*x1", align 4
  %"*y2" = load i32*, i32** %y, align 8
  %"$t" = load i32, i32* %t, align 4
  store i32 %"$t", i32* %"*y2", align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 1, i32* %a, align 4
  store i32 2, i32* %b, align 4
  call void @swap(i32* %a, i32* %b)
  %"$a" = load i32, i32* %a, align 4
  %MUL = mul i32 %"$a", 10
  %"$b" = load i32, i32* %b, align 4
  %ADD = add i32 %MUL, %"$b"
  ret i32 %ADD
}
```

---

## 078 — function: overloaded function

```ura
proto printf(s i8[], ...)

fn show(n i32):
   printf("int %d\n", n)

fn show(s i8[]):
   printf("str %s\n", s)

fn main() i32:
   show(42)
   show("hi")
   return 0
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@const = private unnamed_addr constant [8 x i8] c"int %d\0A\00"
@const.2 = private unnamed_addr constant [8 x i8] c"str %s\0A\00"
@const.3 = private unnamed_addr constant [3 x i8] c"hi\00"

declare void @printf(i8*, ...)

define void @show(i32 %0) {
entry:
  %n = alloca i32, align 4
  %array = alloca [8 x i8], align 1
  store i32 %0, i32* %n, align 4
  %1 = bitcast [8 x i8]* %array to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %1, i8* align 1 getelementptr inbounds ([8 x i8], [8 x i8]* @const, i32 0, i32 0), i64 8, i1 false)
  %2 = getelementptr inbounds [8 x i8], [8 x i8]* %array, i32 0, i32 0
  %n1 = load i32, i32* %n, align 4
  call void (i8*, ...) @printf(i8* %2, i32 %n1)
  ret void
}

define void @show.1(i8* %0) {
entry:
  %s = alloca i8*, align 8
  %array = alloca [8 x i8], align 1
  store i8* %0, i8** %s, align 8
  %1 = bitcast [8 x i8]* %array to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %1, i8* align 1 getelementptr inbounds ([8 x i8], [8 x i8]* @const.2, i32 0, i32 0), i64 8, i1 false)
  %2 = getelementptr inbounds [8 x i8], [8 x i8]* %array, i32 0, i32 0
  %s1 = load i8*, i8** %s, align 8
  call void (i8*, ...) @printf(i8* %2, i8* %s1)
  ret void
}

define i32 @main() {
entry:
  %array = alloca [3 x i8], align 1
  call void @show(i32 42)
  %0 = bitcast [3 x i8]* %array to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %0, i8* align 1 getelementptr inbounds ([3 x i8], [3 x i8]* @const.3, i32 0, i32 0), i64 3, i1 false)
  %1 = getelementptr inbounds [3 x i8], [3 x i8]* %array, i32 0, i32 0
  call void @show.1(i8* %1)
  ret i32 0
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 079 — program: null compared to null

```ura
fn main() i32:
    if null == null:
        return 7
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

define i32 @main() {
entry:
  br i1 true, label %then, label %endif

then:                                             ; preds = %entry
  ret i32 7

endif:                                            ; preds = %entry
  ret i32 0
}
```

---

## 080 — statement: if with a ref

```ura
fn main() i32:
    b bool = False
    p &bool = &b
    p = True
    if b:
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
  %b = alloca i1, align 1
  %p = alloca i1*, align 8
  store i1 false, i1* %b, align 1
  store i1* %b, i1** %p, align 8
  %"*p" = load i1*, i1** %p, align 8
  store i1 true, i1* %"*p", align 1
  br label %if0

if0:                                              ; preds = %entry
  %"$b" = load i1, i1* %b, align 1
  br i1 %"$b", label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  ret i32 0
}
```

---

## 081 — array: 1D i32 array: literal, index read, indexed write

```ura
// stdout: a[0]=10 a[1]=20 a[2]=30
// stdout: after a[1]=99: 99
// expect: 0

fn main() i32:
    a i32[] = [10, 20, 30]
    output("a[0]=", a[0], " a[1]=", a[1], " a[2]=", a[2], "\n")
    a[1] = 99
    output("after a[1]=99: ", a[1], "\n")
    return 0
```

---

## 082 — array: variable index and element arithmetic

```ura
// stdout: a[i]=20 a[i-1]=15
// stdout: a[0]+a[3]=25
// expect: 0

fn main() i32:
    a i32[] = [5, 10, 15, 20]
    i i32 = 3
    output("a[i]=", a[i], " a[i-1]=", a[i - 1], "\n")
    output("a[0]+a[3]=", a[0] + a[3], "\n")
    return 0
```

---

## 083 — array: arrays of bool, char, f32

```ura
// stdout: b[0]=True c[1]=b f[2]=3.000000
// expect: 0

fn main() i32:
    b bool[]  = [True, False, True]
    c char[]  = ['a', 'b', 'c']
    f f32[] = [1.5, 2.5, 3.0]
    output("b[0]=", b[0], " c[1]=", c[1], " f[2]=", f[2], "\n")
    return 0
```

---

## 084 — array: sized constructor i32[N] with zero-init

```ura
// stdout: zero-init: 0 0
// stdout: a[2]=42
// expect: 0

fn main() i32:
    a i32[] = i32[5]
    output("zero-init: ", a[0], " ", a[4], "\n")
    a[2] = 42
    output("a[2]=", a[2], "\n")
    return 0
```

---

## 085 — array: VLA: runtime-sized stack array

```ura
// stdout: a[0]=0 a[7]=100
// expect: 0

fn main() i32:
    n i32 = 4
    a i32[] = i32[n * 2]
    a[7] = 100
    output("a[0]=", a[0], " a[7]=", a[7], "\n")
    return 0
```

---

## 086 — array: indexing a non-array

```ura
// error: Cannot index 'i32', it is not an array
fn main() i32:
    a i32 = 5
    output(a[0])
    return 0
```

---

## 087 — array: non-integer index

```ura
// error: Array index must be an integer, got f32
fn main() i32:
    a i32[] = [1, 2, 3]
    output(a[1.5])
    return 0
```

---

## 088 — array: array literal with mixed element types

```ura
// error: Array elements must all be the same type
fn main() i32:
    a i32[] = [1, True, 3]
    return 0
```

---

## 089 — array: array size is not an integer

```ura
// error: Array size must be an integer
fn main() i32:
    a i32[] = i32[1.5]
    return 0
```

---

## 090 — array: missing closing bracket on index

```ura
// error: Expected ']' after array index
fn main() i32:
    a i32[] = [1, 2, 3]
    output(a[0)
    return 0
```

---

## 091 — array: empty array literal has no element type

```ura
// error: Empty array literal has no element type
fn main() i32:
    a i32[] = []
    return 0
```

---

## 092 — array: unknown member

```ura
// error: Unknown member '.size'
fn main() i32:
    a i32[] = [1, 2, 3]
    output(a.size)
    return 0
```

---

## 093 — function: single ref param: buff hero hp

```ura
// stdout: <100>
// expect: 0

fn buff(hp &i32):
    hp = hp + 20

fn main() i32:
    hero_hp i32 = 80
    buff(&hero_hp)
    output("<", hero_hp, ">\n")
    return 0
```

---

## 094 — function: swap two weapon damage values via refs

```ura
// stdout: <15 30>
// expect: 0

fn swap_weapons(a &i32, b &i32):
    temp i32 = a
    a = b
    b = temp

fn main() i32:
    sword  i32 = 30
    dagger i32 = 15
    swap_weapons(&sword, &dagger)
    output("<", sword, " ", dagger, ">\n")
    return 0
```

---

## 095 — function: refs used in expression, return result

```ura
// stdout: <50>
// expect: 0

fn add_xp(a &i32, b &i32) i32:
    result i32 = a + b
    return result

fn main() i32:
    kill_xp  i32 = 30
    bonus_xp i32 = 20
    z        i32
    z = add_xp(&kill_xp, &bonus_xp)
    output("<", z, ">\n")
    return 0
```

---

## 096 — function: compound ref assign through in a loop

```ura
// stdout: <5>
// expect: 0

fn tick(kills &i32):
    kills += 1

fn main() i32:
    kills i32 = 0
    i     i32 = 0
    while i < 5:
        tick(&kills)
        i = i + 1
    output("<", kills, ">\n")
    return 0
```

---

## 097 — function: ref-to-value: ref passing a where value is expected

```ura
// stdout: HP: 75
// expect: 0

fn show_hp(hp i32):
    output("HP: ", hp, "\n")

fn main() i32:
    hero_hp i32 = 75
    r &i32 = &hero_hp
    show_hp(r)
    return 0
```

---

## 098 — function: ref parameter mutates the caller (counter)

```ura
// stderr: exit: 2
// expect: 1

fn tick(n &i32) i32:
    n = n + 1
    return n
fn main() i32:
    n i32 = 0
    tick(&n)
    tick(&n)
    return n
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @tick(i32* %0) {
entry:
  %n = alloca i32*, align 8
  store i32* %0, i32** %n, align 8
  %"*n" = load i32*, i32** %n, align 8
  %"*n1" = load i32*, i32** %n, align 8
  %"$*n" = load i32, i32* %"*n1", align 4
  %ADD = add i32 %"$*n", 1
  store i32 %ADD, i32* %"*n", align 4
  %"*n2" = load i32*, i32** %n, align 8
  %"$*n3" = load i32, i32* %"*n2", align 4
  ret i32 %"$*n3"
}

define i32 @main() {
entry:
  %n = alloca i32, align 4
  store i32 0, i32* %n, align 4
  %call.tick = call i32 @tick(i32* %n)
  %call.tick1 = call i32 @tick(i32* %n)
  %"$n" = load i32, i32* %n, align 4
  ret i32 %"$n"
}
```

---

## 099 — function: a void function with a ref parameter

```ura
// stderr: exit: 6
// expect: 1

fn bump(n &i32):
    n = n + 1
fn main() i32:
    n i32 = 5
    bump(&n)
    return n
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define void @bump(i32* %0) {
entry:
  %n = alloca i32*, align 8
  store i32* %0, i32** %n, align 8
  %"*n" = load i32*, i32** %n, align 8
  %"*n1" = load i32*, i32** %n, align 8
  %"$*n" = load i32, i32* %"*n1", align 4
  %ADD = add i32 %"$*n", 1
  store i32 %ADD, i32* %"*n", align 4
  ret void
}

define i32 @main() {
entry:
  %n = alloca i32, align 4
  store i32 5, i32* %n, align 4
  call void @bump(i32* %n)
  %"$n" = load i32, i32* %n, align 4
  ret i32 %"$n"
}
```

---

## 100 — ref: basic ref: bind to hp, ref write through

```ura
// stdout: <0>
// expect: 0

fn main() i32:
    hp i32 = 100
    r &i32 = &hp
    r = 0
    output("<", hp, ">\n")
    return 0
```

---

## 101 — ref: aliasing: two refs to same hp, both see mutations

```ura
// stdout: <70>
// expect: 0

fn main() i32:
    hp i32 = 100
    a &i32 = &hp
    b &i32 = &hp
    a = 80
    b = b - 10
    output("<", hp, ">\n")
    return 0
```

---

## 102 — ref: compound ref assignment through in a loop

```ura
// stdout: <200>
// expect: 0

fn main() i32:
    xp      i32 = 0
    xp_gain i32 = 50
    r &i32 = &xp
    i       i32 = 0
    while i < 4:
        r += xp_gain
        i = i + 1
    output("<", xp, ">\n")
    return 0
```

---

## 103 — ref: separate refs to different vars: hp ref ref and mp

```ura
// stdout: HP after hit:   70
// stdout: MP after spell: 30
// expect: 0

fn main() i32:
    hp i32 = 100
    mp i32 = 50

    shield &i32 = &hp
    shield = shield - 30
    output("HP after hit:   ", hp, "\n")

    mana &i32 = &mp
    mana = mana - 20
    output("MP after spell: ", mp, "\n")
    return 0
```

---

## 104 — ref: multiply and subtract through refs: f64 shield, pay mana

```ura
// stdout: <20 55>
// expect: 0

fn main() i32:
    shield i32 = 10
    mp     i32 = 80

    s &i32 = &shield
    m &i32 = &mp
    s *= 2
    m -= 25

    output("<", shield, " ", mp, ">\n")
    return 0
```

---

## 105 — ref: chain of compound mutations ref through a

```ura
// stdout: <270>
// expect: 0

fn main() i32:
    score i32 = 0
    r &i32 = &score
    r += 100
    r += 50
    r *= 2
    r -= 30
    output("<", score, ">\n")
    return 0
```

---

## 106 — ref: reference in conditional

```ura
// stdout: healthy
// expect: 0

fn main() i32:
    hp i32 = 75
    r &i32 = &hp
    if r > 50:
        output("healthy\n")
    else:
        output("critical\n")
    return 0
```

---

## 107 — ref: a ref reads correctly through every operator + as a by-value arg

```ura
// stdout: 15
// stdout: 7
// stdout: 24
// stdout: True
// stdout: True
// stdout: 8
// stdout: 24
// stdout: -12
// stdout: -13
// stdout: 24
// expect: 0

fn twice(n i32) i32:
    return n * 2

fn main() i32:
    x i32 = 12
    r &i32 = &x
    output(r + 3, "\n")
    output(r - 5, "\n")
    output(r * 2, "\n")
    output(r > 10, "\n")
    output(r == 12, "\n")
    output(r & 10, "\n")
    output(r << 1, "\n")
    output(-r, "\n")
    output(~r, "\n")
    output(twice(r), "\n")
    return 0
```

---

## 108 — ref: ref? nullable: bound at declaration, guarded reads pass, write-through works

```ura
// stdout: 42
// stdout: 7
// expect: 0

fn main() i32:
    x i32 = 42
    m &i32 = &x
    output(m, "\n")
    m = 7
    output(x, "\n")
    return 0
```

---

## 109 — ref: nested fn shares an enclosing var through a ref parameter (the sanctioned path)

```ura
// stdout: 6
// expect: 0

fn main() i32:
    x i32 = 5
    fn bump(n &i32):
        n = n + 1
    bump(&x)
    output(x, "\n")
    return 0
```

---

## 110 — ref: a ref reads correctly through the remaining math / comparison / bitwise / cast / logical ops

```ura
// stdout: 3
// stdout: 2
// stdout: False
// stdout: True
// stdout: True
// stdout: False
// stdout: 23
// stdout: 16
// stdout: 5
// stdout: 20
// stdout: False
// stdout: True
// stdout: False
// expect: 0

fn main() i32:
    a i32 = 20
    r &i32 = &a
    output(r / 6, "\n")
    output(r % 6, "\n")
    output(r != 20, "\n")
    output(r < 25, "\n")
    output(r <= 20, "\n")
    output(r >= 21, "\n")
    output(r | 3, "\n")
    output(r ^ 4, "\n")
    output(r >> 2, "\n")
    output(r as i64, "\n")
    flag bool = True
    b &bool = &flag
    output(b and False, "\n")
    output(b or False, "\n")
    output(not b, "\n")
    return 0
```

---

## 111 — ref: a reference must be bound when declared

```ura
// error: A reference must be bound when declared (use 'ref?' for an optional reference)

fn main() i32:
    r &i32
    return 0
```

---

## 112 — ref: binding a reference to the wrong type

```ura
// error: Reference type mismatch: expected array, got i32

fn main() i32:
    x i32 = 5
    y &char[] = &x
    return 0
```

---

## 113 — ref: a ref parameter given a value

```ura
// error: Argument 1 to 'tick' must be passed by reference (ref x)

fn tick(n &i32) i32:
    n = n + 1
    return n

fn main() i32:
    return tick(5)
```

---

## 114 — ref: taking a reference to a literal

```ura
// error: Expected a variable after 'ref'

fn main() i32:
    r &i32 = &5
    return 0
```

---

## 115 — ref: a by-value parameter given a reference

```ura
// error: Argument 1 to 'twice' does not take a reference

fn twice(n i32) i32:
    return n * 2

fn main() i32:
    x i32 = 5
    return twice(&x)
```

---

## 116 — ref: a reference to a name that does not exist

```ura
// error: Undeclared variable 'ghost'

fn main() i32:
    r &i32 = &ghost
    return 0
```

---

## 117 — ref: a nested function may not capture an enclosing local

```ura
// error: Cannot use 'r' from an enclosing function - pass it as a parameter

fn main() i32:
    x i32 = 5
    r &i32 = &x
    fn inner() i32:
        return r
    return inner()
```

---

## 118 — ref: linked list: build with ref, walk it with cur = ref cur.next

```ura
// stdout: sum=60
// expect: 0
struct Node:
    value i32
    next &Node

fn main() i32:
    a Node
    a.value = 10
    b Node
    b.value = 20
    c Node
    c.value = 30
    a.next = &b
    b.next = &c
    cur &Node = &a
    sum i32 = 0
    while cur != null:
        sum += cur.value
        cur = &cur.next
    output("sum=", sum, "\n")
    return 0
```

---

## 119 — ref: a ref parameter rebinds a struct's ref field in place (add-front)

```ura
// stdout: 2 1
// expect: 0
struct Node:
    content i32
    next &Node

struct Lst:
    head &Node

fn push(lst &Lst, node &Node):
    node.next = &lst.head
    lst.head  = &node

fn main() i32:
    lst Lst
    a Node
    a.content = 1
    b Node
    b.content = 2
    push(&lst, &a)
    push(&lst, &b)
    cur &Node = &lst.head
    while cur != null:
        output(cur.content, " ")
        cur = &cur.next
    output("\n")
    return 0
```

---

## 120 — ref: ref? compared to null with == and !=, bound and unbound

```ura
// stdout: False True True
// expect: 0
struct N:
    v i32
    nx &N

fn main() i32:
    a N
    a.v = 1
    p &N = &a
    q &N = null
    output(p == null, " ", p != null, " ", q == null, "\n")
    return 0
```

---

## 121 — ref: chained rebind removes the head: lst.head = ref lst.head.next

```ura
// stdout: 2
// expect: 0
struct Node:
    v i32
    next &Node

struct Lst:
    head &Node

fn main() i32:
    a Node
    a.v = 1
    b Node
    b.v = 2
    a.next = &b
    lst Lst
    lst.head = &a
    lst.head = &lst.head.next
    output(lst.head.v, "\n")
    return 0
```

---

## 122 — ref: = ref rebinds the pointer, = value writes through the referent

```ura
// stdout: 99 99 88
// expect: 0
fn main() i32:
    x i32 = 10
    y i32 = 20
    cur &i32 = &x
    cur = 99
    output(x, " ")
    cur = &y
    cur = 88
    output(x, " ", y, "\n")
    return 0
```

---

## 123 — ref: ref a == ref a is identity true, ref a == ref b is false

```ura
// stdout: True False
// expect: 0
struct N:
    v i32

fn main() i32:
    a N
    b N
    output(&a == &a, " ", &a == &b, "\n")
    return 0
```

---

## 124 — ref: a value function cannot return a reference

```ura
// error: 'f' returns a value, not a reference; drop the `ref`
fn f() i32:
    x i32 = 5
    return &x

fn main() i32:
    output(f(), "\n")
    return 0
```

---

## 125 — ref: a ref declaration cannot be bound to a plain value

```ura
// error: A reference must be bound to a variable (ref x)
fn main() i32:
    m &i32 = 5
    output(m, "\n")
    return 0
```

---

## 126 — ref: a non-nullable value cannot be compared to null

```ura
// error: Only arrays and pointers can be null, not i32
fn main() i32:
    x i32 = 5
    output(x == null, "\n")
    return 0
```

---

## 127 — variable: an array type carries no size

```ura
// error: An array type carries no size; ex. 'arr int[] = int[3]' to make one

fn main() i32:
    xs i32[3]
    xs[0] = 7
    return 0
```


---

## 128 — array: Array<T> from use "@/array": push, insert, pop, remove, front/back, resize, reverse

```ura
use "@/array"

fn main() i32:
    a Array<i32>
    a.push_back(1)
    a.append(2)
    a.push_back(3)
    a.push_front(0)
    a.insert(2, 7)
    a[0] = 10
    output(a[0], " ", a[1], " ", a[2], " ", a[3], " ", a[4], " size ", a.size(), "\n")
    x i32 = a.pop_back()
    y i32 = a.pop_front()
    z i32 = a.remove(1)
    a.back() = 9
    output(x, " ", y, " ", z, " ", a.front(), " ", a.back(), " ", a.size(), " ", a.empty(), "\n")
    a.resize(4)
    a.reverse()
    output(a[0], " ", a[1], " ", a[2], " ", a[3], " cap ", a.capacity(), "\n")
    a.clear()
    output(a.size(), " ", a.empty(), "\n")
    a.release()
    c Array<i8>
    c.append('h')
    c.append('i')
    output(c[0], c[1], " ", typeof(c), "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%"Array<i32>" = type { i32*, i32, i32 }
%"Array<i8>" = type { i8*, i32, i32 }

@str = private unnamed_addr constant [24 x i8] c"%d %d %d %d %d size %d\0A\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.3 = private unnamed_addr constant [22 x i8] c"%d %d %d %d %d %d %s\0A\00", align 1
@str.4 = private unnamed_addr constant [20 x i8] c"%d %d %d %d cap %d\0A\00", align 1
@str.5 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.6 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.7 = private unnamed_addr constant [7 x i8] c"%d %s\0A\00", align 1
@TYPEOF = private unnamed_addr constant [10 x i8] c"Array<i8>\00", align 1
@str.8 = private unnamed_addr constant [9 x i8] c"%c%c %s\0A\00", align 1

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i32)

declare void @free(i8*)

define void @"Array<i32>.reserve"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i32*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 2
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap", %"$*self.cap"
  br i1 %LE, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret void

endif:                                            ; preds = %if0
  %"$cap1" = load i32, i32* %cap, align 4
  %AS = sext i32 %"$cap1" to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i32) to i64))
  %AS2 = bitcast i8* %call.calloc to i32*
  store i32* %AS2, i32** %bigger, align 8
  %"$bigger" = load i32*, i32** %bigger, align 8
  %AS3 = bitcast i32* %"$bigger" to i8*
  %"*self4" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self4", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %AS5 = bitcast i32* %"$*self.data" to i8*
  %"*self6" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self6", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %MUL = mul i32 %"$*self.count", ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i32)
  %call.memcpy = call i8* @memcpy(i8* %AS3, i8* %AS5, i32 %MUL)
  %"*self7" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data8" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self7", i32 0, i32 0
  %"$*self.data9" = load i32*, i32** %"*self.data8", align 8
  %AS10 = bitcast i32* %"$*self.data9" to i8*
  call void @free(i8* %AS10)
  %"*self11" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data12" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self11", i32 0, i32 0
  %"$bigger13" = load i32*, i32** %bigger, align 8
  store i32* %"$bigger13", i32** %"*self.data12", align 8
  %"*self14" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap15" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self14", i32 0, i32 2
  %"$cap16" = load i32, i32* %cap, align 4
  store i32 %"$cap16", i32* %"*self.cap15", align 4
  ret void
}

define void @"Array<i32>.resize"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %size = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %size, align 4
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"$size" = load i32, i32* %size, align 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self", i32 %"$size")
  br label %while1

while1:                                           ; preds = %while1.then, %entry
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$size2" = load i32, i32* %size, align 4
  %GT = icmp sgt i32 %"$*self.count", %"$size2"
  br i1 %GT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %SUB = sub i32 %"$*self.count5", 1
  store i32 %SUB, i32* %"*self.count4", align 4
  %"*self6" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self6", i32 0, i32 0
  %"*self7" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count8" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self7", i32 0, i32 1
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$*self.count9"
  store i32 0, i32* %"*self.data[]", align 4
  br label %while1

endloop:                                          ; preds = %while1
  %"*self10" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self10", i32 0, i32 1
  %"$size12" = load i32, i32* %size, align 4
  store i32 %"$size12", i32* %"*self.count11", align 4
  ret void
}

define void @"Array<i32>.push_back"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %value = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %value, align 4
  br label %if2

if2:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.count", %"$*self.cap"
  br i1 %EQ, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 2
  %"$*self.cap5" = load i32, i32* %"*self.cap4", align 4
  %MUL = mul i32 %"$*self.cap5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  %"*self6" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self6", i32 0, i32 0
  %"*self7" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count8" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self7", i32 0, i32 1
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$*self.count9"
  %"$value" = load i32, i32* %value, align 4
  store i32 %"$value", i32* %"*self.data[]", align 4
  %"*self10" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  ret void
}

define void @"Array<i32>.append"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %value = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %value, align 4
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"$value" = load i32, i32* %value, align 4
  call void @"Array<i32>.push_back"(%"Array<i32>"* %"*self", i32 %"$value")
  ret void
}

define void @"Array<i32>.insert"(%"Array<i32>"* %0, i32 %1, i32 %2) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %index = alloca i32, align 4
  %value = alloca i32, align 4
  %i = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %index, align 4
  store i32 %2, i32* %value, align 4
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.count", %"$*self.cap"
  br i1 %EQ, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 2
  %"$*self.cap5" = load i32, i32* %"*self.cap4", align 4
  %MUL = mul i32 %"$*self.cap5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  %"*self6" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count7" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self6", i32 0, i32 1
  %"$*self.count8" = load i32, i32* %"*self.count7", align 4
  store i32 %"$*self.count8", i32* %i, align 4
  br label %while4

while4:                                           ; preds = %while4.then, %endif
  %"$i" = load i32, i32* %i, align 4
  %"$index" = load i32, i32* %index, align 4
  %GT = icmp sgt i32 %"$i", %"$index"
  br i1 %GT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
  %"*self9" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self9", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$i10" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$i10"
  %"*self11" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data12" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self11", i32 0, i32 0
  %"$i13" = load i32, i32* %i, align 4
  %SUB = sub i32 %"$i13", 1
  %"$*self.data14" = load i32*, i32** %"*self.data12", align 8
  %"*self.data[]15" = getelementptr inbounds i32, i32* %"$*self.data14", i32 %SUB
  %"$*self.data[]" = load i32, i32* %"*self.data[]15", align 4
  store i32 %"$*self.data[]", i32* %"*self.data[]", align 4
  %"$i16" = load i32, i32* %i, align 4
  %SUB17 = sub i32 %"$i16", 1
  store i32 %SUB17, i32* %i, align 4
  br label %while4

endloop:                                          ; preds = %while4
  %"*self18" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data19" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self18", i32 0, i32 0
  %"$*self.data20" = load i32*, i32** %"*self.data19", align 8
  %"$index21" = load i32, i32* %index, align 4
  %"*self.data[]22" = getelementptr inbounds i32, i32* %"$*self.data20", i32 %"$index21"
  %"$value" = load i32, i32* %value, align 4
  store i32 %"$value", i32* %"*self.data[]22", align 4
  %"*self23" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count24" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self23", i32 0, i32 1
  %"$*self.count25" = load i32, i32* %"*self.count24", align 4
  %ADD26 = add i32 %"$*self.count25", 1
  store i32 %ADD26, i32* %"*self.count24", align 4
  ret void
}

define void @"Array<i32>.push_front"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %value = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %value, align 4
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"$value" = load i32, i32* %value, align 4
  call void @"Array<i32>.insert"(%"Array<i32>"* %"*self", i32 0, i32 %"$value")
  ret void
}

define i32 @"Array<i32>.remove"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %index = alloca i32, align 4
  %value = alloca i32, align 4
  %i = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %index, align 4
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$index"
  %"$*self.data[]" = load i32, i32* %"*self.data[]", align 4
  store i32 %"$*self.data[]", i32* %value, align 4
  %"$index1" = load i32, i32* %index, align 4
  store i32 %"$index1", i32* %i, align 4
  br label %while5

while5:                                           ; preds = %while5.then, %entry
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", %SUB
  br i1 %LT, label %while5.then, label %endloop

while5.then:                                      ; preds = %while5
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 0
  %"$*self.data5" = load i32*, i32** %"*self.data4", align 8
  %"$i6" = load i32, i32* %i, align 4
  %"*self.data[]7" = getelementptr inbounds i32, i32* %"$*self.data5", i32 %"$i6"
  %"*self8" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data9" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self8", i32 0, i32 0
  %"$i10" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i10", 1
  %"$*self.data11" = load i32*, i32** %"*self.data9", align 8
  %"*self.data[]12" = getelementptr inbounds i32, i32* %"$*self.data11", i32 %ADD
  %"$*self.data[]13" = load i32, i32* %"*self.data[]12", align 4
  store i32 %"$*self.data[]13", i32* %"*self.data[]7", align 4
  %"$i14" = load i32, i32* %i, align 4
  %ADD15 = add i32 %"$i14", 1
  store i32 %ADD15, i32* %i, align 4
  br label %while5

endloop:                                          ; preds = %while5
  %"*self16" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count17" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self16", i32 0, i32 1
  %"$*self.count18" = load i32, i32* %"*self.count17", align 4
  %SUB19 = sub i32 %"$*self.count18", 1
  store i32 %SUB19, i32* %"*self.count17", align 4
  %"*self20" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data21" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self20", i32 0, i32 0
  %"*self22" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count23" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self22", i32 0, i32 1
  %"$*self.data24" = load i32*, i32** %"*self.data21", align 8
  %"$*self.count25" = load i32, i32* %"*self.count23", align 4
  %"*self.data[]26" = getelementptr inbounds i32, i32* %"$*self.data24", i32 %"$*self.count25"
  store i32 0, i32* %"*self.data[]26", align 4
  %"$value" = load i32, i32* %value, align 4
  ret i32 %"$value"
}

define i32 @"Array<i32>.pop_back"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %value = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  store i32 %SUB, i32* %"*self.count", align 4
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 0
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 1
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$*self.count4" = load i32, i32* %"*self.count3", align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$*self.count4"
  %"$*self.data[]" = load i32, i32* %"*self.data[]", align 4
  store i32 %"$*self.data[]", i32* %value, align 4
  %"*self5" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data6" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self5", i32 0, i32 0
  %"*self7" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count8" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self7", i32 0, i32 1
  %"$*self.data9" = load i32*, i32** %"*self.data6", align 8
  %"$*self.count10" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]11" = getelementptr inbounds i32, i32* %"$*self.data9", i32 %"$*self.count10"
  store i32 0, i32* %"*self.data[]11", align 4
  %"$value" = load i32, i32* %value, align 4
  ret i32 %"$value"
}

define i32 @"Array<i32>.pop_front"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"call.Array<i32>.remove" = call i32 @"Array<i32>.remove"(%"Array<i32>"* %"*self", i32 0)
  ret i32 %"call.Array<i32>.remove"
}

define i32* @"Array<i32>.front"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 0
  ret i32* %"*self.data[]"
}

define i32* @"Array<i32>.back"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 0
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %SUB
  ret i32* %"*self.data[]"
}

define i32 @"Array<i32>.size"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i32 @"Array<i32>.capacity"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  ret i32 %"$*self.cap"
}

define i1 @"Array<i32>.empty"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %EQ = icmp eq i32 %"$*self.count", 0
  ret i1 %EQ
}

define void @"Array<i32>.clear"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  br label %while6

while6:                                           ; preds = %while6.then, %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while6.then, label %endloop

while6.then:                                      ; preds = %while6
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count2" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  %"$*self.count3" = load i32, i32* %"*self.count2", align 4
  %SUB = sub i32 %"$*self.count3", 1
  store i32 %SUB, i32* %"*self.count2", align 4
  %"*self4" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self4", i32 0, i32 0
  %"*self5" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count6" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self5", i32 0, i32 1
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$*self.count7" = load i32, i32* %"*self.count6", align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$*self.count7"
  store i32 0, i32* %"*self.data[]", align 4
  br label %while6

endloop:                                          ; preds = %while6
  ret void
}

define void @"Array<i32>.reverse"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %tmp = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 0, i32* %i, align 4
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  store i32 %SUB, i32* %j, align 4
  br label %while7

while7:                                           ; preds = %while7.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %"$j" = load i32, i32* %j, align 4
  %LT = icmp slt i32 %"$i", %"$j"
  br i1 %LT, label %while7.then, label %endloop

while7.then:                                      ; preds = %while7
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$i2" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$i2"
  %"$*self.data[]" = load i32, i32* %"*self.data[]", align 4
  store i32 %"$*self.data[]", i32* %tmp, align 4
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 0
  %"$*self.data5" = load i32*, i32** %"*self.data4", align 8
  %"$i6" = load i32, i32* %i, align 4
  %"*self.data[]7" = getelementptr inbounds i32, i32* %"$*self.data5", i32 %"$i6"
  %"*self8" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data9" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self8", i32 0, i32 0
  %"$*self.data10" = load i32*, i32** %"*self.data9", align 8
  %"$j11" = load i32, i32* %j, align 4
  %"*self.data[]12" = getelementptr inbounds i32, i32* %"$*self.data10", i32 %"$j11"
  %"$*self.data[]13" = load i32, i32* %"*self.data[]12", align 4
  store i32 %"$*self.data[]13", i32* %"*self.data[]7", align 4
  %"*self14" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data15" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self14", i32 0, i32 0
  %"$*self.data16" = load i32*, i32** %"*self.data15", align 8
  %"$j17" = load i32, i32* %j, align 4
  %"*self.data[]18" = getelementptr inbounds i32, i32* %"$*self.data16", i32 %"$j17"
  %"$tmp" = load i32, i32* %tmp, align 4
  store i32 %"$tmp", i32* %"*self.data[]18", align 4
  %"$i19" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i19", 1
  store i32 %ADD, i32* %i, align 4
  %"$j20" = load i32, i32* %j, align 4
  %SUB21 = sub i32 %"$j20", 1
  store i32 %SUB21, i32* %j, align 4
  br label %while7

endloop:                                          ; preds = %while7
  ret void
}

define void @"Array<i32>.release"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  call void @"Array<i32>.clear"(%"Array<i32>"* %"*self")
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %AS = bitcast i32* %"$*self.data" to i8*
  call void @free(i8* %AS)
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data3" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 0
  store i32* null, i32** %"*self.data3", align 8
  %"*self4" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self4", i32 0, i32 2
  store i32 0, i32* %"*self.cap", align 4
  ret void
}

define i32* @"Array<i32>.[]"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %index = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %index, align 4
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$index"
  ret i32* %"*self.data[]"
}

define void @"Array<i32>.drop"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  call void @"Array<i32>.clear"(%"Array<i32>"* %"*self")
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %AS = bitcast i32* %"$*self.data" to i8*
  call void @free(i8* %AS)
  ret void
}

define void @"Array<i8>.reserve"(%"Array<i8>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if8

if8:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap", %"$*self.cap"
  br i1 %LE, label %if8.then, label %endif

if8.then:                                         ; preds = %if8
  ret void

endif:                                            ; preds = %if8
  %"$cap1" = load i32, i32* %cap, align 4
  %AS = sext i32 %"$cap1" to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %bigger, align 8
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"*self3" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self3", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %MUL = mul i32 %"$*self.count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %call.memcpy = call i8* @memcpy(i8* %"$bigger", i8* %"$*self.data", i32 %MUL)
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data5" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 0
  %"$*self.data6" = load i8*, i8** %"*self.data5", align 8
  call void @free(i8* %"$*self.data6")
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data8" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 0
  %"$bigger9" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger9", i8** %"*self.data8", align 8
  %"*self10" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap11" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self10", i32 0, i32 2
  %"$cap12" = load i32, i32* %cap, align 4
  store i32 %"$cap12", i32* %"*self.cap11", align 4
  ret void
}

define void @"Array<i8>.push_back"(%"Array<i8>"* %0, i8 %1) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %value = alloca i8, align 1
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store i8 %1, i8* %value, align 1
  br label %if9

if9:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.count", %"$*self.cap"
  br i1 %EQ, label %if9.then, label %endif

if9.then:                                         ; preds = %if9
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self3" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap4" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self3", i32 0, i32 2
  %"$*self.cap5" = load i32, i32* %"*self.cap4", align 4
  %MUL = mul i32 %"$*self.cap5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<i8>.reserve"(%"Array<i8>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if9.then, %if9
  %"*self6" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self6", i32 0, i32 0
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count8" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count9"
  %"$value" = load i8, i8* %value, align 1
  store i8 %"$value", i8* %"*self.data[]", align 1
  %"*self10" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  ret void
}

define void @"Array<i8>.append"(%"Array<i8>"* %0, i8 %1) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %value = alloca i8, align 1
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store i8 %1, i8* %value, align 1
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"$value" = load i8, i8* %value, align 1
  call void @"Array<i8>.push_back"(%"Array<i8>"* %"*self", i8 %"$value")
  ret void
}

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %while10

while10:                                          ; preds = %while10.then, %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while10.then, label %endloop

while10.then:                                     ; preds = %while10
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count2" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  %"$*self.count3" = load i32, i32* %"*self.count2", align 4
  %SUB = sub i32 %"$*self.count3", 1
  store i32 %SUB, i32* %"*self.count2", align 4
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 0
  %"*self5" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count6" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self5", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count7" = load i32, i32* %"*self.count6", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count7"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while10

endloop:                                          ; preds = %while10
  ret void
}

define i8* @"Array<i8>.[]"(%"Array<i8>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %index = alloca i32, align 4
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store i32 %1, i32* %index, align 4
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$index"
  ret i8* %"*self.data[]"
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*self")
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @free(i8* %"$*self.data")
  ret void
}

define i32 @main() {
entry:
  %a = alloca %"Array<i32>", align 8
  %ref = alloca i32*, align 8
  %ref3 = alloca i32*, align 8
  %ref6 = alloca i32*, align 8
  %ref9 = alloca i32*, align 8
  %ref12 = alloca i32*, align 8
  %ref15 = alloca i32*, align 8
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  %z = alloca i32, align 4
  %ref21 = alloca i32*, align 8
  %ref23 = alloca i32*, align 8
  %ref26 = alloca i32*, align 8
  %bool.text = alloca i8*, align 8
  %ref33 = alloca i32*, align 8
  %ref36 = alloca i32*, align 8
  %ref39 = alloca i32*, align 8
  %ref42 = alloca i32*, align 8
  %bool.text51 = alloca i8*, align 8
  %c = alloca %"Array<i8>", align 8
  %ref54 = alloca i8*, align 8
  %ref57 = alloca i8*, align 8
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %a, align 8
  call void @"Array<i32>.push_back"(%"Array<i32>"* %a, i32 1)
  call void @"Array<i32>.append"(%"Array<i32>"* %a, i32 2)
  call void @"Array<i32>.push_back"(%"Array<i32>"* %a, i32 3)
  call void @"Array<i32>.push_front"(%"Array<i32>"* %a, i32 0)
  call void @"Array<i32>.insert"(%"Array<i32>"* %a, i32 2, i32 7)
  %"call.Array<i32>.[]" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %a, i32 0)
  store i32* %"call.Array<i32>.[]", i32** %ref, align 8
  %"*ref" = load i32*, i32** %ref, align 8
  store i32 10, i32* %"*ref", align 4
  %"call.Array<i32>.[]2" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %a, i32 0)
  store i32* %"call.Array<i32>.[]2", i32** %ref3, align 8
  %"*ref4" = load i32*, i32** %ref3, align 8
  %"call.Array<i32>.[]5" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %a, i32 1)
  store i32* %"call.Array<i32>.[]5", i32** %ref6, align 8
  %"*ref7" = load i32*, i32** %ref6, align 8
  %"call.Array<i32>.[]8" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %a, i32 2)
  store i32* %"call.Array<i32>.[]8", i32** %ref9, align 8
  %"*ref10" = load i32*, i32** %ref9, align 8
  %"call.Array<i32>.[]11" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %a, i32 3)
  store i32* %"call.Array<i32>.[]11", i32** %ref12, align 8
  %"*ref13" = load i32*, i32** %ref12, align 8
  %"call.Array<i32>.[]14" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %a, i32 4)
  store i32* %"call.Array<i32>.[]14", i32** %ref15, align 8
  %"*ref16" = load i32*, i32** %ref15, align 8
  %"call.Array<i32>.size" = call i32 @"Array<i32>.size"(%"Array<i32>"* %a)
  %"$*ref" = load i32, i32* %"*ref4", align 4
  %"$*ref17" = load i32, i32* %"*ref7", align 4
  %"$*ref18" = load i32, i32* %"*ref10", align 4
  %"$*ref19" = load i32, i32* %"*ref13", align 4
  %"$*ref20" = load i32, i32* %"*ref16", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([24 x i8], [24 x i8]* @str, i32 0, i32 0), i32 %"$*ref", i32 %"$*ref17", i32 %"$*ref18", i32 %"$*ref19", i32 %"$*ref20", i32 %"call.Array<i32>.size")
  %"call.Array<i32>.pop_back" = call i32 @"Array<i32>.pop_back"(%"Array<i32>"* %a)
  store i32 %"call.Array<i32>.pop_back", i32* %x, align 4
  %"call.Array<i32>.pop_front" = call i32 @"Array<i32>.pop_front"(%"Array<i32>"* %a)
  store i32 %"call.Array<i32>.pop_front", i32* %y, align 4
  %"call.Array<i32>.remove" = call i32 @"Array<i32>.remove"(%"Array<i32>"* %a, i32 1)
  store i32 %"call.Array<i32>.remove", i32* %z, align 4
  %"call.Array<i32>.back" = call i32* @"Array<i32>.back"(%"Array<i32>"* %a)
  store i32* %"call.Array<i32>.back", i32** %ref21, align 8
  %"*ref22" = load i32*, i32** %ref21, align 8
  store i32 9, i32* %"*ref22", align 4
  %"call.Array<i32>.front" = call i32* @"Array<i32>.front"(%"Array<i32>"* %a)
  store i32* %"call.Array<i32>.front", i32** %ref23, align 8
  %"*ref24" = load i32*, i32** %ref23, align 8
  %"call.Array<i32>.back25" = call i32* @"Array<i32>.back"(%"Array<i32>"* %a)
  store i32* %"call.Array<i32>.back25", i32** %ref26, align 8
  %"*ref27" = load i32*, i32** %ref26, align 8
  %"call.Array<i32>.size28" = call i32 @"Array<i32>.size"(%"Array<i32>"* %a)
  %"call.Array<i32>.empty" = call i1 @"Array<i32>.empty"(%"Array<i32>"* %a)
  %"$x" = load i32, i32* %x, align 4
  %"$y" = load i32, i32* %y, align 4
  %"$z" = load i32, i32* %z, align 4
  %"$*ref29" = load i32, i32* %"*ref24", align 4
  %"$*ref30" = load i32, i32* %"*ref27", align 4
  store i8* null, i8** %bool.text, align 8
  br label %if11

if11:                                             ; preds = %entry
  br i1 %"call.Array<i32>.empty", label %if11.then, label %else12

if11.then:                                        ; preds = %if11
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else12:                                           ; preds = %if11
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else12, %if11.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf31 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @str.3, i32 0, i32 0), i32 %"$x", i32 %"$y", i32 %"$z", i32 %"$*ref29", i32 %"$*ref30", i32 %"call.Array<i32>.size28", i8* %"$bool.text")
  call void @"Array<i32>.resize"(%"Array<i32>"* %a, i32 4)
  call void @"Array<i32>.reverse"(%"Array<i32>"* %a)
  %"call.Array<i32>.[]32" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %a, i32 0)
  store i32* %"call.Array<i32>.[]32", i32** %ref33, align 8
  %"*ref34" = load i32*, i32** %ref33, align 8
  %"call.Array<i32>.[]35" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %a, i32 1)
  store i32* %"call.Array<i32>.[]35", i32** %ref36, align 8
  %"*ref37" = load i32*, i32** %ref36, align 8
  %"call.Array<i32>.[]38" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %a, i32 2)
  store i32* %"call.Array<i32>.[]38", i32** %ref39, align 8
  %"*ref40" = load i32*, i32** %ref39, align 8
  %"call.Array<i32>.[]41" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %a, i32 3)
  store i32* %"call.Array<i32>.[]41", i32** %ref42, align 8
  %"*ref43" = load i32*, i32** %ref42, align 8
  %"call.Array<i32>.capacity" = call i32 @"Array<i32>.capacity"(%"Array<i32>"* %a)
  %"$*ref44" = load i32, i32* %"*ref34", align 4
  %"$*ref45" = load i32, i32* %"*ref37", align 4
  %"$*ref46" = load i32, i32* %"*ref40", align 4
  %"$*ref47" = load i32, i32* %"*ref43", align 4
  %call.dprintf48 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @str.4, i32 0, i32 0), i32 %"$*ref44", i32 %"$*ref45", i32 %"$*ref46", i32 %"$*ref47", i32 %"call.Array<i32>.capacity")
  call void @"Array<i32>.clear"(%"Array<i32>"* %a)
  %"call.Array<i32>.size49" = call i32 @"Array<i32>.size"(%"Array<i32>"* %a)
  %"call.Array<i32>.empty50" = call i1 @"Array<i32>.empty"(%"Array<i32>"* %a)
  store i8* null, i8** %bool.text51, align 8
  br label %if13

if13:                                             ; preds = %endif
  br i1 %"call.Array<i32>.empty50", label %if13.then, label %else14

if13.then:                                        ; preds = %if13
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.5, i32 0, i32 0), i8** %bool.text51, align 8
  br label %endif1

else14:                                           ; preds = %if13
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.6, i32 0, i32 0), i8** %bool.text51, align 8
  br label %endif1

endif1:                                           ; preds = %else14, %if13.then
  %"$bool.text52" = load i8*, i8** %bool.text51, align 8
  %call.dprintf53 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.7, i32 0, i32 0), i32 %"call.Array<i32>.size49", i8* %"$bool.text52")
  call void @"Array<i32>.release"(%"Array<i32>"* %a)
  store %"Array<i8>" zeroinitializer, %"Array<i8>"* %c, align 8
  call void @"Array<i8>.append"(%"Array<i8>"* %c, i8 104)
  call void @"Array<i8>.append"(%"Array<i8>"* %c, i8 105)
  %"call.Array<i8>.[]" = call i8* @"Array<i8>.[]"(%"Array<i8>"* %c, i32 0)
  store i8* %"call.Array<i8>.[]", i8** %ref54, align 8
  %"*ref55" = load i8*, i8** %ref54, align 8
  %"call.Array<i8>.[]56" = call i8* @"Array<i8>.[]"(%"Array<i8>"* %c, i32 1)
  store i8* %"call.Array<i8>.[]56", i8** %ref57, align 8
  %"*ref58" = load i8*, i8** %ref57, align 8
  %"$*ref59" = load i8, i8* %"*ref55", align 1
  %AS = sext i8 %"$*ref59" to i32
  %"$*ref60" = load i8, i8* %"*ref58", align 1
  %AS61 = sext i8 %"$*ref60" to i32
  %call.dprintf62 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.8, i32 0, i32 0), i32 %AS, i32 %AS61, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @TYPEOF, i32 0, i32 0))
  call void @"Array<i8>.drop"(%"Array<i8>"* %c)
  call void @"Array<i32>.drop"(%"Array<i32>"* %a)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 129 — use: a package that doesn't exist

```ura
use "@/nothing"

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:1:5 package '@/nothing' not found
  |
1 | use "@/nothing"
  |     ^^^^^^^^^^^
help: run: ura install nothing
error: aborting due to 1 error
```

### status

```
1
```

---

## 130 — use: an error after a use points at the user's file and line

```ura
use "@/array"

fn main() i32:
    a Array<i32>
    a.append("text")
    return 0
```

### stderr

```
error: test.ura:5:7 'Array<i32>' has no method 'append' that takes (i8[])
  |
5 |     a.append("text")
  |       ^^^^^^
help: 'append' takes (i32)
error: aborting due to 1 error
```

### status

```
1
```

---

## 131 — operator: bitwise & | ^ << >> on scalars, compound forms, & stays a ref before a type, >> closes nested templates

```ura
// stdout: 2 7 5 16 -4
// stdout: 15
// stdout: 6 6 True
// stdout: 9 1
// expect: 0
use "@/array"

fn main() i32:
    a i32 = 6
    b i32 = 3
    output(a & b, " ", a | b, " ", a ^ b, " ", 1 << 4, " ", -16 >> 2, "\n")
    u u32 = 4294967280
    output(u >> 28, "\n")
    c i32 = 1
    c <<= 3
    c |= 1
    c &= 13
    c ^= 4
    c >>= 1
    output(c, " ", 1 + 2 << 1, " ", a & b == 2, "\n")
    r &i32 = &a
    r = 9
    grid Array<Array<i32>>
    output(a, " ", grid.size() + 1, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%"Array<i32>" = type { i32*, i32, i32 }
%"Array<Array<i32>>" = type { %"Array<i32>"*, i32, i32 }

@str = private unnamed_addr constant [16 x i8] c"%d %d %d %d %d\0A\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"%u\0A\00", align 1
@str.2 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.4 = private unnamed_addr constant [10 x i8] c"%d %d %s\0A\00", align 1
@str.5 = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1

declare void @free(i8*)

define void @"Array<i32>.clear"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count2" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  %"$*self.count3" = load i32, i32* %"*self.count2", align 4
  %SUB = sub i32 %"$*self.count3", 1
  store i32 %SUB, i32* %"*self.count2", align 4
  %"*self4" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self4", i32 0, i32 0
  %"*self5" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count6" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self5", i32 0, i32 1
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$*self.count7" = load i32, i32* %"*self.count6", align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$*self.count7"
  store i32 0, i32* %"*self.data[]", align 4
  br label %while0

endloop:                                          ; preds = %while0
  ret void
}

define void @"Array<i32>.drop"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  call void @"Array<i32>.clear"(%"Array<i32>"* %"*self")
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %AS = bitcast i32* %"$*self.data" to i8*
  call void @free(i8* %AS)
  ret void
}

define i32 @"Array<Array<i32>>.size"(%"Array<Array<i32>>"* %0) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @"Array<Array<i32>>.clear"(%"Array<Array<i32>>"* %0) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  br label %while1

while1:                                           ; preds = %while1.then, %entry
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"*self1" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count2" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self1", i32 0, i32 1
  %"$*self.count3" = load i32, i32* %"*self.count2", align 4
  %SUB = sub i32 %"$*self.count3", 1
  store i32 %SUB, i32* %"*self.count2", align 4
  %"*self4" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self4", i32 0, i32 0
  %"*self5" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count6" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self5", i32 0, i32 1
  %"$*self.data" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data", align 8
  %"$*self.count7" = load i32, i32* %"*self.count6", align 4
  %"*self.data[]" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"$*self.data", i32 %"$*self.count7"
  call void @"Array<i32>.drop"(%"Array<i32>"* %"*self.data[]")
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %"*self.data[]", align 8
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define void @"Array<Array<i32>>.drop"(%"Array<Array<i32>>"* %0) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  call void @"Array<Array<i32>>.clear"(%"Array<Array<i32>>"* %"*self")
  %"*self1" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self1", i32 0, i32 0
  %"$*self.data" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data", align 8
  %AS = bitcast %"Array<i32>"* %"$*self.data" to i8*
  call void @free(i8* %AS)
  ret void
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %u = alloca i32, align 4
  %c = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %r = alloca i32*, align 8
  %grid = alloca %"Array<Array<i32>>", align 8
  store i32 6, i32* %a, align 4
  store i32 3, i32* %b, align 4
  %"$a" = load i32, i32* %a, align 4
  %"$b" = load i32, i32* %b, align 4
  %BAND = and i32 %"$a", %"$b"
  %"$a1" = load i32, i32* %a, align 4
  %"$b2" = load i32, i32* %b, align 4
  %BOR = or i32 %"$a1", %"$b2"
  %"$a3" = load i32, i32* %a, align 4
  %"$b4" = load i32, i32* %b, align 4
  %BXOR = xor i32 %"$a3", %"$b4"
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str, i32 0, i32 0), i32 %BAND, i32 %BOR, i32 %BXOR, i32 16, i32 -4)
  store i32 -16, i32* %u, align 4
  %"$u" = load i32, i32* %u, align 4
  %RSHIFT = lshr i32 %"$u", 28
  %call.dprintf5 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 %RSHIFT)
  store i32 1, i32* %c, align 4
  %"$c" = load i32, i32* %c, align 4
  %LSHIFT = shl i32 %"$c", 3
  store i32 %LSHIFT, i32* %c, align 4
  %"$c6" = load i32, i32* %c, align 4
  %BOR7 = or i32 %"$c6", 1
  store i32 %BOR7, i32* %c, align 4
  %"$c8" = load i32, i32* %c, align 4
  %BAND9 = and i32 %"$c8", 13
  store i32 %BAND9, i32* %c, align 4
  %"$c10" = load i32, i32* %c, align 4
  %BXOR11 = xor i32 %"$c10", 4
  store i32 %BXOR11, i32* %c, align 4
  %"$c12" = load i32, i32* %c, align 4
  %RSHIFT13 = ashr i32 %"$c12", 1
  store i32 %RSHIFT13, i32* %c, align 4
  %"$a14" = load i32, i32* %a, align 4
  %"$b15" = load i32, i32* %b, align 4
  %BAND16 = and i32 %"$a14", %"$b15"
  %EQ = icmp eq i32 %BAND16, 2
  %"$c17" = load i32, i32* %c, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if2

if2:                                              ; preds = %entry
  br i1 %EQ, label %if2.then, label %else3

if2.then:                                         ; preds = %if2
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else3:                                            ; preds = %if2
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else3, %if2.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf18 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.4, i32 0, i32 0), i32 %"$c17", i32 6, i8* %"$bool.text")
  store i32* %a, i32** %r, align 8
  %"*r" = load i32*, i32** %r, align 8
  store i32 9, i32* %"*r", align 4
  store %"Array<Array<i32>>" zeroinitializer, %"Array<Array<i32>>"* %grid, align 8
  %"call.Array<Array<i32>>.size" = call i32 @"Array<Array<i32>>.size"(%"Array<Array<i32>>"* %grid)
  %ADD = add i32 %"call.Array<Array<i32>>.size", 1
  %"$a19" = load i32, i32* %a, align 4
  %call.dprintf20 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.5, i32 0, i32 0), i32 %"$a19", i32 %ADD)
  call void @"Array<Array<i32>>.drop"(%"Array<Array<i32>>"* %grid)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

