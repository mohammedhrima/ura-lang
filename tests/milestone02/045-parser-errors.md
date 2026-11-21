# milestone02

## index

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
- 111 — ref: a ref declared without a value starts as null
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
error: test.ura:2:13 'i32' can't be null
  |
2 |     x i32 = null
  |             ^^^^
help: null fits a ref (&T) or an array (T[])
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
error: test.ura:3:14 'a' is never null, so this comparison is always the same
  |
3 |     b bool = a == null
  |              ^
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
error: test.ura:2:14 can't apply '-' to 'String'
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
error: test.ura:6:7 expected '&i32', found 'i32'
  |
6 |     f(a)
  |       ^
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
help: use x i32[]
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
error: test.ura:2:11 arrays have no size in the type
  |
2 |     x i32[3]
  |           ^
help: use 'i32[]'
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

define i32 @main() {
entry:
  %s = alloca i8*, align 8
  %arr = alloca [4 x i8], align 1
  %c = alloca i8, align 1
  store [4 x i8] c"abc\00", [4 x i8]* %arr, align 1
  %0 = getelementptr inbounds [4 x i8], [4 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** %s, align 8
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
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @sum(i32* %0) {
entry:
  %a = alloca i32*, align 8
  %total = alloca i32, align 4
  %i = alloca i32, align 4
  store i32* %0, i32** %a, align 8
  store i32 0, i32* %total, align 4
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$a" = load i32*, i32** %a, align 8
  %"$i1" = load i32, i32* %i, align 4
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 %"$i1"
  %"$a[]" = load i32, i32* %"a[]", align 4
  %"$total" = load i32, i32* %total, align 4
  %ADD = add i32 %"$total", %"$a[]"
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
  %values = alloca i32*, align 8
  %arr = alloca [3 x i32], align 4
  store [3 x i32] [i32 1, i32 2, i32 3], [3 x i32]* %arr, align 4
  %0 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %values, align 8
  %"$values" = load i32*, i32** %values, align 8
  %call.sum = call i32 @sum(i32* %"$values")
  ret i32 %call.sum
}
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
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32* @same(i32* %0) {
entry:
  %p = alloca i32*, align 8
  store i32* %0, i32** %p, align 8
  %"*p" = load i32*, i32** %p, align 8
  ret i32* %"*p"
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %q = alloca i32*, align 8
  %ref = alloca i32*, align 8
  store i32 6, i32* %a, align 4
  %call.same = call i32* @same(i32* %a)
  store i32* %call.same, i32** %ref, align 8
  %"*ref" = load i32*, i32** %ref, align 8
  store i32* %"*ref", i32** %q, align 8
  %"*q" = load i32*, i32** %q, align 8
  %"$*q" = load i32, i32* %"*q", align 4
  ret i32 %"$*q"
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
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [8 x i8] c"int %d\0A\00", align 1
@str.2 = private unnamed_addr constant [8 x i8] c"str %s\0A\00", align 1
@str.3 = private unnamed_addr constant [3 x i8] c"hi\00", align 1

declare void @printf(i8*, ...)

define void @show(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  call void (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str, i32 0, i32 0), i32 %"$n")
  ret void
}

define void @show.1(i8* %0) {
entry:
  %s = alloca i8*, align 8
  store i8* %0, i8** %s, align 8
  %"$s" = load i8*, i8** %s, align 8
  call void (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.2, i32 0, i32 0), i8* %"$s")
  ret void
}

define i32 @main() {
entry:
  call void @show(i32 42)
  call void @show.1(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.3, i32 0, i32 0))
  ret i32 0
}
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
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  br label %if0

if0:                                              ; preds = %entry
  br i1 true, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 7

endif:                                            ; preds = %if0
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [25 x i8] c"a[0]=%d a[1]=%d a[2]=%d\0A\00", align 1
@str.1 = private unnamed_addr constant [19 x i8] c"after a[1]=99: %d\0A\00", align 1

define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %arr = alloca [3 x i32], align 4
  store [3 x i32] [i32 10, i32 20, i32 30], [3 x i32]* %arr, align 4
  %0 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %a, align 8
  %"$a" = load i32*, i32** %a, align 8
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 0
  %"$a1" = load i32*, i32** %a, align 8
  %"a[]2" = getelementptr inbounds i32, i32* %"$a1", i32 1
  %"$a3" = load i32*, i32** %a, align 8
  %"a[]4" = getelementptr inbounds i32, i32* %"$a3", i32 2
  %"$a[]" = load i32, i32* %"a[]", align 4
  %"$a[]5" = load i32, i32* %"a[]2", align 4
  %"$a[]6" = load i32, i32* %"a[]4", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([25 x i8], [25 x i8]* @str, i32 0, i32 0), i32 %"$a[]", i32 %"$a[]5", i32 %"$a[]6")
  %"$a7" = load i32*, i32** %a, align 8
  %"a[]8" = getelementptr inbounds i32, i32* %"$a7", i32 1
  store i32 99, i32* %"a[]8", align 4
  %"$a9" = load i32*, i32** %a, align 8
  %"a[]10" = getelementptr inbounds i32, i32* %"$a9", i32 1
  %"$a[]11" = load i32, i32* %"a[]10", align 4
  %call.dprintf12 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @str.1, i32 0, i32 0), i32 %"$a[]11")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [19 x i8] c"a[i]=%d a[i-1]=%d\0A\00", align 1
@str.1 = private unnamed_addr constant [14 x i8] c"a[0]+a[3]=%d\0A\00", align 1

define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %arr = alloca [4 x i32], align 4
  %i = alloca i32, align 4
  store [4 x i32] [i32 5, i32 10, i32 15, i32 20], [4 x i32]* %arr, align 4
  %0 = getelementptr inbounds [4 x i32], [4 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %a, align 8
  store i32 3, i32* %i, align 4
  %"$a" = load i32*, i32** %a, align 8
  %"$i" = load i32, i32* %i, align 4
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i32 %"$i"
  %"$i1" = load i32, i32* %i, align 4
  %SUB = sub i32 %"$i1", 1
  %"$a2" = load i32*, i32** %a, align 8
  %"a[]3" = getelementptr inbounds i32, i32* %"$a2", i32 %SUB
  %"$a[]" = load i32, i32* %"a[]", align 4
  %"$a[]4" = load i32, i32* %"a[]3", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @str, i32 0, i32 0), i32 %"$a[]", i32 %"$a[]4")
  %"$a5" = load i32*, i32** %a, align 8
  %"a[]6" = getelementptr inbounds i32, i32* %"$a5", i32 0
  %"$a7" = load i32*, i32** %a, align 8
  %"a[]8" = getelementptr inbounds i32, i32* %"$a7", i32 3
  %"$a[]9" = load i32, i32* %"a[]6", align 4
  %"$a[]10" = load i32, i32* %"a[]8", align 4
  %ADD = add i32 %"$a[]9", %"$a[]10"
  %call.dprintf11 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.1, i32 0, i32 0), i32 %ADD)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.2 = private unnamed_addr constant [25 x i8] c"b[0]=%s c[1]=%c f[2]=%f\0A\00", align 1

define i32 @main() {
entry:
  %b = alloca i1*, align 8
  %arr = alloca [3 x i1], align 1
  %c = alloca i8*, align 8
  %arr1 = alloca [3 x i8], align 1
  %f = alloca float*, align 8
  %arr2 = alloca [3 x float], align 4
  %bool.text = alloca i8*, align 8
  store [3 x i1] [i1 true, i1 false, i1 true], [3 x i1]* %arr, align 1
  %0 = getelementptr inbounds [3 x i1], [3 x i1]* %arr, i32 0, i32 0
  store i1* %0, i1** %b, align 8
  store [3 x i8] c"abc", [3 x i8]* %arr1, align 1
  %1 = getelementptr inbounds [3 x i8], [3 x i8]* %arr1, i32 0, i32 0
  store i8* %1, i8** %c, align 8
  store [3 x float] [float 1.500000e+00, float 2.500000e+00, float 3.000000e+00], [3 x float]* %arr2, align 4
  %2 = getelementptr inbounds [3 x float], [3 x float]* %arr2, i32 0, i32 0
  store float* %2, float** %f, align 8
  %"$b" = load i1*, i1** %b, align 8
  %"b[]" = getelementptr inbounds i1, i1* %"$b", i32 0
  %"$c" = load i8*, i8** %c, align 8
  %"c[]" = getelementptr inbounds i8, i8* %"$c", i32 1
  %"$f" = load float*, float** %f, align 8
  %"f[]" = getelementptr inbounds float, float* %"$f", i32 2
  %"$b[]" = load i1, i1* %"b[]", align 1
  store i8* null, i8** %bool.text, align 8
  br label %if0

if0:                                              ; preds = %entry
  br i1 %"$b[]", label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else1:                                            ; preds = %if0
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  %"$c[]" = load i8, i8* %"c[]", align 1
  %AS = sext i8 %"$c[]" to i32
  %"$f[]" = load float, float* %"f[]", align 4
  %AS3 = fpext float %"$f[]" to double
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([25 x i8], [25 x i8]* @str.2, i32 0, i32 0), i8* %"$bool.text", i32 %AS, double %AS3)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [6 x i8] c"<%d>\0A\00", align 1

define void @buff(i32* %0) {
entry:
  %hp = alloca i32*, align 8
  store i32* %0, i32** %hp, align 8
  %"*hp" = load i32*, i32** %hp, align 8
  %"*hp1" = load i32*, i32** %hp, align 8
  %"$*hp" = load i32, i32* %"*hp1", align 4
  %ADD = add i32 %"$*hp", 20
  store i32 %ADD, i32* %"*hp", align 4
  ret void
}

define i32 @main() {
entry:
  %hero_hp = alloca i32, align 4
  store i32 80, i32* %hero_hp, align 4
  call void @buff(i32* %hero_hp)
  %"$hero_hp" = load i32, i32* %hero_hp, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %"$hero_hp")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [9 x i8] c"<%d %d>\0A\00", align 1

define void @swap_weapons(i32* %0, i32* %1) {
entry:
  %a = alloca i32*, align 8
  %b = alloca i32*, align 8
  %temp = alloca i32, align 4
  store i32* %0, i32** %a, align 8
  store i32* %1, i32** %b, align 8
  %"*a" = load i32*, i32** %a, align 8
  %"$*a" = load i32, i32* %"*a", align 4
  store i32 %"$*a", i32* %temp, align 4
  %"*a1" = load i32*, i32** %a, align 8
  %"*b" = load i32*, i32** %b, align 8
  %"$*b" = load i32, i32* %"*b", align 4
  store i32 %"$*b", i32* %"*a1", align 4
  %"*b2" = load i32*, i32** %b, align 8
  %"$temp" = load i32, i32* %temp, align 4
  store i32 %"$temp", i32* %"*b2", align 4
  ret void
}

define i32 @main() {
entry:
  %sword = alloca i32, align 4
  %dagger = alloca i32, align 4
  store i32 30, i32* %sword, align 4
  store i32 15, i32* %dagger, align 4
  call void @swap_weapons(i32* %sword, i32* %dagger)
  %"$sword" = load i32, i32* %sword, align 4
  %"$dagger" = load i32, i32* %dagger, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %"$sword", i32 %"$dagger")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [6 x i8] c"<%d>\0A\00", align 1

define i32 @add_xp(i32* %0, i32* %1) {
entry:
  %a = alloca i32*, align 8
  %b = alloca i32*, align 8
  %result = alloca i32, align 4
  store i32* %0, i32** %a, align 8
  store i32* %1, i32** %b, align 8
  %"*a" = load i32*, i32** %a, align 8
  %"*b" = load i32*, i32** %b, align 8
  %"$*a" = load i32, i32* %"*a", align 4
  %"$*b" = load i32, i32* %"*b", align 4
  %ADD = add i32 %"$*a", %"$*b"
  store i32 %ADD, i32* %result, align 4
  %"$result" = load i32, i32* %result, align 4
  ret i32 %"$result"
}

define i32 @main() {
entry:
  %kill_xp = alloca i32, align 4
  %bonus_xp = alloca i32, align 4
  %z = alloca i32, align 4
  store i32 30, i32* %kill_xp, align 4
  store i32 20, i32* %bonus_xp, align 4
  %call.add_xp = call i32 @add_xp(i32* %kill_xp, i32* %bonus_xp)
  store i32 %call.add_xp, i32* %z, align 4
  %"$z" = load i32, i32* %z, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %"$z")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [6 x i8] c"<%d>\0A\00", align 1

define void @tick(i32* %0) {
entry:
  %kills = alloca i32*, align 8
  store i32* %0, i32** %kills, align 8
  %"*kills" = load i32*, i32** %kills, align 8
  %"$*kills" = load i32, i32* %"*kills", align 4
  %ADD = add i32 %"$*kills", 1
  store i32 %ADD, i32* %"*kills", align 4
  ret void
}

define i32 @main() {
entry:
  %kills = alloca i32, align 4
  %i = alloca i32, align 4
  store i32 0, i32* %kills, align 4
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 5
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  call void @tick(i32* %kills)
  %"$i1" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i1", 1
  store i32 %ADD, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$kills" = load i32, i32* %kills, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %"$kills")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [8 x i8] c"HP: %d\0A\00", align 1

define void @show_hp(i32 %0) {
entry:
  %hp = alloca i32, align 4
  store i32 %0, i32* %hp, align 4
  %"$hp" = load i32, i32* %hp, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str, i32 0, i32 0), i32 %"$hp")
  ret void
}

define i32 @main() {
entry:
  %hero_hp = alloca i32, align 4
  %r = alloca i32*, align 8
  store i32 75, i32* %hero_hp, align 4
  store i32* %hero_hp, i32** %r, align 8
  %"*r" = load i32*, i32** %r, align 8
  %"$*r" = load i32, i32* %"*r", align 4
  call void @show_hp(i32 %"$*r")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [6 x i8] c"<%d>\0A\00", align 1

define i32 @main() {
entry:
  %hp = alloca i32, align 4
  %r = alloca i32*, align 8
  store i32 100, i32* %hp, align 4
  store i32* %hp, i32** %r, align 8
  %"*r" = load i32*, i32** %r, align 8
  store i32 0, i32* %"*r", align 4
  %"$hp" = load i32, i32* %hp, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %"$hp")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [6 x i8] c"<%d>\0A\00", align 1

define i32 @main() {
entry:
  %hp = alloca i32, align 4
  %a = alloca i32*, align 8
  %b = alloca i32*, align 8
  store i32 100, i32* %hp, align 4
  store i32* %hp, i32** %a, align 8
  store i32* %hp, i32** %b, align 8
  %"*a" = load i32*, i32** %a, align 8
  store i32 80, i32* %"*a", align 4
  %"*b" = load i32*, i32** %b, align 8
  %"*b1" = load i32*, i32** %b, align 8
  %"$*b" = load i32, i32* %"*b1", align 4
  %SUB = sub i32 %"$*b", 10
  store i32 %SUB, i32* %"*b", align 4
  %"$hp" = load i32, i32* %hp, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %"$hp")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [6 x i8] c"<%d>\0A\00", align 1

define i32 @main() {
entry:
  %xp = alloca i32, align 4
  %xp_gain = alloca i32, align 4
  %r = alloca i32*, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %xp, align 4
  store i32 50, i32* %xp_gain, align 4
  store i32* %xp, i32** %r, align 8
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 4
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"*r" = load i32*, i32** %r, align 8
  %"$xp_gain" = load i32, i32* %xp_gain, align 4
  %"$*r" = load i32, i32* %"*r", align 4
  %ADD = add i32 %"$*r", %"$xp_gain"
  store i32 %ADD, i32* %"*r", align 4
  %"$i1" = load i32, i32* %i, align 4
  %ADD2 = add i32 %"$i1", 1
  store i32 %ADD2, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$xp" = load i32, i32* %xp, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %"$xp")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [20 x i8] c"HP after hit:   %d\0A\00", align 1
@str.1 = private unnamed_addr constant [20 x i8] c"MP after spell: %d\0A\00", align 1

define i32 @main() {
entry:
  %hp = alloca i32, align 4
  %mp = alloca i32, align 4
  %shield = alloca i32*, align 8
  %mana = alloca i32*, align 8
  store i32 100, i32* %hp, align 4
  store i32 50, i32* %mp, align 4
  store i32* %hp, i32** %shield, align 8
  %"*shield" = load i32*, i32** %shield, align 8
  %"*shield1" = load i32*, i32** %shield, align 8
  %"$*shield" = load i32, i32* %"*shield1", align 4
  %SUB = sub i32 %"$*shield", 30
  store i32 %SUB, i32* %"*shield", align 4
  %"$hp" = load i32, i32* %hp, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @str, i32 0, i32 0), i32 %"$hp")
  store i32* %mp, i32** %mana, align 8
  %"*mana" = load i32*, i32** %mana, align 8
  %"*mana2" = load i32*, i32** %mana, align 8
  %"$*mana" = load i32, i32* %"*mana2", align 4
  %SUB3 = sub i32 %"$*mana", 20
  store i32 %SUB3, i32* %"*mana", align 4
  %"$mp" = load i32, i32* %mp, align 4
  %call.dprintf4 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @str.1, i32 0, i32 0), i32 %"$mp")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [9 x i8] c"<%d %d>\0A\00", align 1

define i32 @main() {
entry:
  %shield = alloca i32, align 4
  %mp = alloca i32, align 4
  %s = alloca i32*, align 8
  %m = alloca i32*, align 8
  store i32 10, i32* %shield, align 4
  store i32 80, i32* %mp, align 4
  store i32* %shield, i32** %s, align 8
  store i32* %mp, i32** %m, align 8
  %"*s" = load i32*, i32** %s, align 8
  %"$*s" = load i32, i32* %"*s", align 4
  %MUL = mul i32 %"$*s", 2
  store i32 %MUL, i32* %"*s", align 4
  %"*m" = load i32*, i32** %m, align 8
  %"$*m" = load i32, i32* %"*m", align 4
  %SUB = sub i32 %"$*m", 25
  store i32 %SUB, i32* %"*m", align 4
  %"$shield" = load i32, i32* %shield, align 4
  %"$mp" = load i32, i32* %mp, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %"$shield", i32 %"$mp")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [6 x i8] c"<%d>\0A\00", align 1

define i32 @main() {
entry:
  %score = alloca i32, align 4
  %r = alloca i32*, align 8
  store i32 0, i32* %score, align 4
  store i32* %score, i32** %r, align 8
  %"*r" = load i32*, i32** %r, align 8
  %"$*r" = load i32, i32* %"*r", align 4
  %ADD = add i32 %"$*r", 100
  store i32 %ADD, i32* %"*r", align 4
  %"*r1" = load i32*, i32** %r, align 8
  %"$*r2" = load i32, i32* %"*r1", align 4
  %ADD3 = add i32 %"$*r2", 50
  store i32 %ADD3, i32* %"*r1", align 4
  %"*r4" = load i32*, i32** %r, align 8
  %"$*r5" = load i32, i32* %"*r4", align 4
  %MUL = mul i32 %"$*r5", 2
  store i32 %MUL, i32* %"*r4", align 4
  %"*r6" = load i32*, i32** %r, align 8
  %"$*r7" = load i32, i32* %"*r6", align 4
  %SUB = sub i32 %"$*r7", 30
  store i32 %SUB, i32* %"*r6", align 4
  %"$score" = load i32, i32* %score, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %"$score")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [9 x i8] c"healthy\0A\00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c"critical\0A\00", align 1

define i32 @main() {
entry:
  %hp = alloca i32, align 4
  %r = alloca i32*, align 8
  store i32 75, i32* %hp, align 4
  store i32* %hp, i32** %r, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*r" = load i32*, i32** %r, align 8
  %"$*r" = load i32, i32* %"*r", align 4
  %GT = icmp sgt i32 %"$*r", 50
  br i1 %GT, label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0))
  br label %endif

else1:                                            ; preds = %if0
  %call.dprintf1 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0))
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.3 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.4 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.5 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.6 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.7 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.8 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.9 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.10 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.11 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.12 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.13 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

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
  %x = alloca i32, align 4
  %r = alloca i32*, align 8
  %bool.text = alloca i8*, align 8
  %bool.text13 = alloca i8*, align 8
  store i32 12, i32* %x, align 4
  store i32* %x, i32** %r, align 8
  %"*r" = load i32*, i32** %r, align 8
  %"$*r" = load i32, i32* %"*r", align 4
  %ADD = add i32 %"$*r", 3
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %ADD)
  %"*r2" = load i32*, i32** %r, align 8
  %"$*r3" = load i32, i32* %"*r2", align 4
  %SUB = sub i32 %"$*r3", 5
  %call.dprintf4 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 %SUB)
  %"*r5" = load i32*, i32** %r, align 8
  %"$*r6" = load i32, i32* %"*r5", align 4
  %MUL = mul i32 %"$*r6", 2
  %call.dprintf7 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 %MUL)
  %"*r8" = load i32*, i32** %r, align 8
  %"$*r9" = load i32, i32* %"*r8", align 4
  %GT = icmp sgt i32 %"$*r9", 10
  store i8* null, i8** %bool.text, align 8
  br label %if0

if0:                                              ; preds = %entry
  br i1 %GT, label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.3, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else1:                                            ; preds = %if0
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.4, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf10 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.5, i32 0, i32 0), i8* %"$bool.text")
  %"*r11" = load i32*, i32** %r, align 8
  %"$*r12" = load i32, i32* %"*r11", align 4
  %EQ = icmp eq i32 %"$*r12", 12
  store i8* null, i8** %bool.text13, align 8
  br label %if2

if2:                                              ; preds = %endif
  br i1 %EQ, label %if2.then, label %else3

if2.then:                                         ; preds = %if2
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.6, i32 0, i32 0), i8** %bool.text13, align 8
  br label %endif1

else3:                                            ; preds = %if2
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.7, i32 0, i32 0), i8** %bool.text13, align 8
  br label %endif1

endif1:                                           ; preds = %else3, %if2.then
  %"$bool.text14" = load i8*, i8** %bool.text13, align 8
  %call.dprintf15 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.8, i32 0, i32 0), i8* %"$bool.text14")
  %"*r16" = load i32*, i32** %r, align 8
  %"$*r17" = load i32, i32* %"*r16", align 4
  %BAND = and i32 %"$*r17", 10
  %call.dprintf18 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.9, i32 0, i32 0), i32 %BAND)
  %"*r19" = load i32*, i32** %r, align 8
  %"$*r20" = load i32, i32* %"*r19", align 4
  %LSHIFT = shl i32 %"$*r20", 1
  %call.dprintf21 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.10, i32 0, i32 0), i32 %LSHIFT)
  %"*r22" = load i32*, i32** %r, align 8
  %"$*r23" = load i32, i32* %"*r22", align 4
  %NEG = sub i32 0, %"$*r23"
  %call.dprintf24 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.11, i32 0, i32 0), i32 %NEG)
  %"*r25" = load i32*, i32** %r, align 8
  %"$*r26" = load i32, i32* %"*r25", align 4
  %BXOR = xor i32 %"$*r26", -1
  %call.dprintf27 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.12, i32 0, i32 0), i32 %BXOR)
  %"*r28" = load i32*, i32** %r, align 8
  %"$*r29" = load i32, i32* %"*r28", align 4
  %call.twice = call i32 @twice(i32 %"$*r29")
  %call.dprintf30 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.13, i32 0, i32 0), i32 %call.twice)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %m = alloca i32*, align 8
  store i32 42, i32* %x, align 4
  store i32* %x, i32** %m, align 8
  %"*m" = load i32*, i32** %m, align 8
  %"$*m" = load i32, i32* %"*m", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$*m")
  %"*m1" = load i32*, i32** %m, align 8
  store i32 7, i32* %"*m1", align 4
  %"$x" = load i32, i32* %x, align 4
  %call.dprintf2 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 %"$x")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @main() {
entry:
  %x = alloca i32, align 4
  store i32 5, i32* %x, align 4
  call void @bump(i32* %x)
  %"$x" = load i32, i32* %x, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$x")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)

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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.2 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.4 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.5 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.6 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.7 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.8 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.9 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.10 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.11 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.12 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.13 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.14 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.15 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.16 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.17 = private unnamed_addr constant [6 x i8] c"%lld\0A\00", align 1
@str.18 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.19 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.20 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.21 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.22 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.23 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.24 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.25 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.26 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %r = alloca i32*, align 8
  %bool.text = alloca i8*, align 8
  %bool.text15 = alloca i8*, align 8
  %bool.text20 = alloca i8*, align 8
  %bool.text25 = alloca i8*, align 8
  %flag = alloca i1, align 1
  %b = alloca i1*, align 8
  %bool.text40 = alloca i8*, align 8
  %bool.text45 = alloca i8*, align 8
  %bool.text50 = alloca i8*, align 8
  store i32 20, i32* %a, align 4
  store i32* %a, i32** %r, align 8
  %"*r" = load i32*, i32** %r, align 8
  %"$*r" = load i32, i32* %"*r", align 4
  %DIV = sdiv i32 %"$*r", 6
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %DIV)
  %"*r7" = load i32*, i32** %r, align 8
  %"$*r8" = load i32, i32* %"*r7", align 4
  %MOD = srem i32 %"$*r8", 6
  %call.dprintf9 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 %MOD)
  %"*r10" = load i32*, i32** %r, align 8
  %"$*r11" = load i32, i32* %"*r10", align 4
  %NQ = icmp ne i32 %"$*r11", 20
  store i8* null, i8** %bool.text, align 8
  br label %if0

if0:                                              ; preds = %entry
  br i1 %NQ, label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else1:                                            ; preds = %if0
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf12 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i8* %"$bool.text")
  %"*r13" = load i32*, i32** %r, align 8
  %"$*r14" = load i32, i32* %"*r13", align 4
  %LT = icmp slt i32 %"$*r14", 25
  store i8* null, i8** %bool.text15, align 8
  br label %if2

if2:                                              ; preds = %endif
  br i1 %LT, label %if2.then, label %else3

if2.then:                                         ; preds = %if2
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.5, i32 0, i32 0), i8** %bool.text15, align 8
  br label %endif1

else3:                                            ; preds = %if2
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.6, i32 0, i32 0), i8** %bool.text15, align 8
  br label %endif1

endif1:                                           ; preds = %else3, %if2.then
  %"$bool.text16" = load i8*, i8** %bool.text15, align 8
  %call.dprintf17 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.7, i32 0, i32 0), i8* %"$bool.text16")
  %"*r18" = load i32*, i32** %r, align 8
  %"$*r19" = load i32, i32* %"*r18", align 4
  %LE = icmp sle i32 %"$*r19", 20
  store i8* null, i8** %bool.text20, align 8
  br label %if4

if4:                                              ; preds = %endif1
  br i1 %LE, label %if4.then, label %else5

if4.then:                                         ; preds = %if4
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.8, i32 0, i32 0), i8** %bool.text20, align 8
  br label %endif2

else5:                                            ; preds = %if4
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.9, i32 0, i32 0), i8** %bool.text20, align 8
  br label %endif2

endif2:                                           ; preds = %else5, %if4.then
  %"$bool.text21" = load i8*, i8** %bool.text20, align 8
  %call.dprintf22 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.10, i32 0, i32 0), i8* %"$bool.text21")
  %"*r23" = load i32*, i32** %r, align 8
  %"$*r24" = load i32, i32* %"*r23", align 4
  %GE = icmp sge i32 %"$*r24", 21
  store i8* null, i8** %bool.text25, align 8
  br label %if6

if6:                                              ; preds = %endif2
  br i1 %GE, label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.11, i32 0, i32 0), i8** %bool.text25, align 8
  br label %endif3

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.12, i32 0, i32 0), i8** %bool.text25, align 8
  br label %endif3

endif3:                                           ; preds = %else7, %if6.then
  %"$bool.text26" = load i8*, i8** %bool.text25, align 8
  %call.dprintf27 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.13, i32 0, i32 0), i8* %"$bool.text26")
  %"*r28" = load i32*, i32** %r, align 8
  %"$*r29" = load i32, i32* %"*r28", align 4
  %BOR = or i32 %"$*r29", 3
  %call.dprintf30 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.14, i32 0, i32 0), i32 %BOR)
  %"*r31" = load i32*, i32** %r, align 8
  %"$*r32" = load i32, i32* %"*r31", align 4
  %BXOR = xor i32 %"$*r32", 4
  %call.dprintf33 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.15, i32 0, i32 0), i32 %BXOR)
  %"*r34" = load i32*, i32** %r, align 8
  %"$*r35" = load i32, i32* %"*r34", align 4
  %RSHIFT = ashr i32 %"$*r35", 2
  %call.dprintf36 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.16, i32 0, i32 0), i32 %RSHIFT)
  %"*r37" = load i32*, i32** %r, align 8
  %"$*r38" = load i32, i32* %"*r37", align 4
  %AS = sext i32 %"$*r38" to i64
  %call.dprintf39 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.17, i32 0, i32 0), i64 %AS)
  store i1 true, i1* %flag, align 1
  store i1* %flag, i1** %b, align 8
  %"*b" = load i1*, i1** %b, align 8
  %"$*b" = load i1, i1* %"*b", align 1
  %AND = and i1 %"$*b", false
  store i8* null, i8** %bool.text40, align 8
  br label %if8

if8:                                              ; preds = %endif3
  br i1 %AND, label %if8.then, label %else9

if8.then:                                         ; preds = %if8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.18, i32 0, i32 0), i8** %bool.text40, align 8
  br label %endif4

else9:                                            ; preds = %if8
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.19, i32 0, i32 0), i8** %bool.text40, align 8
  br label %endif4

endif4:                                           ; preds = %else9, %if8.then
  %"$bool.text41" = load i8*, i8** %bool.text40, align 8
  %call.dprintf42 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.20, i32 0, i32 0), i8* %"$bool.text41")
  %"*b43" = load i1*, i1** %b, align 8
  %"$*b44" = load i1, i1* %"*b43", align 1
  %OR = or i1 %"$*b44", false
  store i8* null, i8** %bool.text45, align 8
  br label %if10

if10:                                             ; preds = %endif4
  br i1 %OR, label %if10.then, label %else11

if10.then:                                        ; preds = %if10
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.21, i32 0, i32 0), i8** %bool.text45, align 8
  br label %endif5

else11:                                           ; preds = %if10
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.22, i32 0, i32 0), i8** %bool.text45, align 8
  br label %endif5

endif5:                                           ; preds = %else11, %if10.then
  %"$bool.text46" = load i8*, i8** %bool.text45, align 8
  %call.dprintf47 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.23, i32 0, i32 0), i8* %"$bool.text46")
  %"*b48" = load i1*, i1** %b, align 8
  %"$*b49" = load i1, i1* %"*b48", align 1
  %NOT = xor i1 %"$*b49", true
  store i8* null, i8** %bool.text50, align 8
  br label %if12

if12:                                             ; preds = %endif5
  br i1 %NOT, label %if12.then, label %else13

if12.then:                                        ; preds = %if12
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.24, i32 0, i32 0), i8** %bool.text50, align 8
  br label %endif6

else13:                                           ; preds = %if12
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.25, i32 0, i32 0), i8** %bool.text50, align 8
  br label %endif6

endif6:                                           ; preds = %else13, %if12.then
  %"$bool.text51" = load i8*, i8** %bool.text50, align 8
  %call.dprintf52 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.26, i32 0, i32 0), i8* %"$bool.text51")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 111 — ref: a ref declared without a value starts as null

```ura
// stdout: null
// stdout: bound 4
// stdout: null again
// expect: 0

fn main() i32:
    r &i32
    if r == null:
        output("null\n")
    n i32 = 4
    r = &n
    if r != null:
        output("bound ", r, "\n")
    r = null
    if isZero(r):
        output("null again\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [6 x i8] c"null\0A\00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c"bound %d\0A\00", align 1
@str.2 = private unnamed_addr constant [12 x i8] c"null again\0A\00", align 1

define i32 @main() {
entry:
  %r = alloca i32*, align 8
  %n = alloca i32, align 4
  store i32* null, i32** %r, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*r" = load i32*, i32** %r, align 8
  %"$r" = load i32*, i32** %r, align 8
  %EQ = icmp eq i32* %"$r", null
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0))
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  store i32 4, i32* %n, align 4
  store i32* %n, i32** %r, align 8
  br label %if1

if1:                                              ; preds = %endif
  %"*r3" = load i32*, i32** %r, align 8
  %"$r4" = load i32*, i32** %r, align 8
  %NQ = icmp ne i32* %"$r4", null
  br i1 %NQ, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  %"*r5" = load i32*, i32** %r, align 8
  %"$*r" = load i32, i32* %"*r5", align 4
  %call.dprintf6 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i32 %"$*r")
  br label %endif1

endif1:                                           ; preds = %if1.then, %if1
  store i32* null, i32** %r, align 8
  br label %if2

if2:                                              ; preds = %endif1
  %"*r7" = load i32*, i32** %r, align 8
  %"$r8" = load i32*, i32** %r, align 8
  %EQ9 = icmp eq i32* %"$r8", null
  br i1 %EQ9, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %call.dprintf10 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.2, i32 0, i32 0))
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### stderr

```
error: test.ura:4:15 '&' expects a variable or a struct attribute
  |
4 |     r &i32 = &5
  |               ^
error: aborting due to 1 error
```

### status

```
1
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

### stderr

```
error: test.ura:7:16 'r' belongs to an enclosing function
  |
7 |         return r
  |                ^
help: pass it as a parameter
error: aborting due to 1 error
```

### status

```
1
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Node = type { i32, %Node* }

@str = private unnamed_addr constant [8 x i8] c"sum=%d\0A\00", align 1

define i32 @main() {
entry:
  %a = alloca %Node, align 8
  %b = alloca %Node, align 8
  %c = alloca %Node, align 8
  %cur = alloca %Node*, align 8
  %sum = alloca i32, align 4
  store %Node zeroinitializer, %Node* %a, align 8
  %a.value = getelementptr inbounds %Node, %Node* %a, i32 0, i32 0
  store i32 10, i32* %a.value, align 4
  store %Node zeroinitializer, %Node* %b, align 8
  %b.value = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  store i32 20, i32* %b.value, align 4
  store %Node zeroinitializer, %Node* %c, align 8
  %c.value = getelementptr inbounds %Node, %Node* %c, i32 0, i32 0
  store i32 30, i32* %c.value, align 4
  %a.next = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  store %Node* %b, %Node** %a.next, align 8
  %b.next = getelementptr inbounds %Node, %Node* %b, i32 0, i32 1
  store %Node* %c, %Node** %b.next, align 8
  store %Node* %a, %Node** %cur, align 8
  store i32 0, i32* %sum, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"*cur" = load %Node*, %Node** %cur, align 8
  %"$cur" = load %Node*, %Node** %cur, align 8
  %NQ = icmp ne %Node* %"$cur", null
  br i1 %NQ, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"*cur1" = load %Node*, %Node** %cur, align 8
  %"*cur.value" = getelementptr inbounds %Node, %Node* %"*cur1", i32 0, i32 0
  %"$*cur.value" = load i32, i32* %"*cur.value", align 4
  %"$sum" = load i32, i32* %sum, align 4
  %ADD = add i32 %"$sum", %"$*cur.value"
  store i32 %ADD, i32* %sum, align 4
  %"*cur2" = load %Node*, %Node** %cur, align 8
  %"*cur.next" = getelementptr inbounds %Node, %Node* %"*cur2", i32 0, i32 1
  %"$*cur.next" = load %Node*, %Node** %"*cur.next", align 8
  store %Node* %"$*cur.next", %Node** %cur, align 8
  br label %while0

endloop:                                          ; preds = %while0
  %"$sum3" = load i32, i32* %sum, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str, i32 0, i32 0), i32 %"$sum3")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Lst = type { %Node* }
%Node = type { i32, %Node* }

@str = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

define void @push(%Lst* %0, %Node* %1) {
entry:
  %lst = alloca %Lst*, align 8
  %node = alloca %Node*, align 8
  store %Lst* %0, %Lst** %lst, align 8
  store %Node* %1, %Node** %node, align 8
  %"*node" = load %Node*, %Node** %node, align 8
  %"*node.next" = getelementptr inbounds %Node, %Node* %"*node", i32 0, i32 1
  %"*lst" = load %Lst*, %Lst** %lst, align 8
  %"*lst.head" = getelementptr inbounds %Lst, %Lst* %"*lst", i32 0, i32 0
  %"$*lst.head" = load %Node*, %Node** %"*lst.head", align 8
  store %Node* %"$*lst.head", %Node** %"*node.next", align 8
  %"*lst1" = load %Lst*, %Lst** %lst, align 8
  %"*lst.head2" = getelementptr inbounds %Lst, %Lst* %"*lst1", i32 0, i32 0
  %"*node3" = load %Node*, %Node** %node, align 8
  store %Node* %"*node3", %Node** %"*lst.head2", align 8
  ret void
}

define i32 @main() {
entry:
  %lst = alloca %Lst, align 8
  %a = alloca %Node, align 8
  %b = alloca %Node, align 8
  %cur = alloca %Node*, align 8
  store %Lst zeroinitializer, %Lst* %lst, align 8
  store %Node zeroinitializer, %Node* %a, align 8
  %a.content = getelementptr inbounds %Node, %Node* %a, i32 0, i32 0
  store i32 1, i32* %a.content, align 4
  store %Node zeroinitializer, %Node* %b, align 8
  %b.content = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  store i32 2, i32* %b.content, align 4
  call void @push(%Lst* %lst, %Node* %a)
  call void @push(%Lst* %lst, %Node* %b)
  %lst.head = getelementptr inbounds %Lst, %Lst* %lst, i32 0, i32 0
  %"$lst.head" = load %Node*, %Node** %lst.head, align 8
  store %Node* %"$lst.head", %Node** %cur, align 8
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"*cur" = load %Node*, %Node** %cur, align 8
  %"$cur" = load %Node*, %Node** %cur, align 8
  %NQ = icmp ne %Node* %"$cur", null
  br i1 %NQ, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"*cur1" = load %Node*, %Node** %cur, align 8
  %"*cur.content" = getelementptr inbounds %Node, %Node* %"*cur1", i32 0, i32 0
  %"$*cur.content" = load i32, i32* %"*cur.content", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$*cur.content")
  %"*cur2" = load %Node*, %Node** %cur, align 8
  %"*cur.next" = getelementptr inbounds %Node, %Node* %"*cur2", i32 0, i32 1
  %"$*cur.next" = load %Node*, %Node** %"*cur.next", align 8
  store %Node* %"$*cur.next", %Node** %cur, align 8
  br label %while0

endloop:                                          ; preds = %while0
  %call.dprintf3 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%N = type { i32, %N* }

@str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.2 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.4 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.5 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.6 = private unnamed_addr constant [10 x i8] c"%s %s %s\0A\00", align 1

define i32 @main() {
entry:
  %a = alloca %N, align 8
  %p = alloca %N*, align 8
  %q = alloca %N*, align 8
  %bool.text = alloca i8*, align 8
  %bool.text6 = alloca i8*, align 8
  %bool.text7 = alloca i8*, align 8
  store %N zeroinitializer, %N* %a, align 8
  %a.v = getelementptr inbounds %N, %N* %a, i32 0, i32 0
  store i32 1, i32* %a.v, align 4
  store %N* %a, %N** %p, align 8
  store %N* null, %N** %q, align 8
  %"*p" = load %N*, %N** %p, align 8
  %"$p" = load %N*, %N** %p, align 8
  %EQ = icmp eq %N* %"$p", null
  %"*p3" = load %N*, %N** %p, align 8
  %"$p4" = load %N*, %N** %p, align 8
  %NQ = icmp ne %N* %"$p4", null
  %"*q" = load %N*, %N** %q, align 8
  %"$q" = load %N*, %N** %q, align 8
  %EQ5 = icmp eq %N* %"$q", null
  store i8* null, i8** %bool.text, align 8
  br label %if0

if0:                                              ; preds = %entry
  br i1 %EQ, label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else1:                                            ; preds = %if0
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  store i8* null, i8** %bool.text6, align 8
  br label %if2

if2:                                              ; preds = %endif
  br i1 %NQ, label %if2.then, label %else3

if2.then:                                         ; preds = %if2
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.2, i32 0, i32 0), i8** %bool.text6, align 8
  br label %endif1

else3:                                            ; preds = %if2
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i8** %bool.text6, align 8
  br label %endif1

endif1:                                           ; preds = %else3, %if2.then
  store i8* null, i8** %bool.text7, align 8
  br label %if4

if4:                                              ; preds = %endif1
  br i1 %EQ5, label %if4.then, label %else5

if4.then:                                         ; preds = %if4
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.4, i32 0, i32 0), i8** %bool.text7, align 8
  br label %endif2

else5:                                            ; preds = %if4
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.5, i32 0, i32 0), i8** %bool.text7, align 8
  br label %endif2

endif2:                                           ; preds = %else5, %if4.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %"$bool.text8" = load i8*, i8** %bool.text6, align 8
  %"$bool.text9" = load i8*, i8** %bool.text7, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.6, i32 0, i32 0), i8* %"$bool.text", i8* %"$bool.text8", i8* %"$bool.text9")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Node = type { i32, %Node* }
%Lst = type { %Node* }

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @main() {
entry:
  %a = alloca %Node, align 8
  %b = alloca %Node, align 8
  %lst = alloca %Lst, align 8
  store %Node zeroinitializer, %Node* %a, align 8
  %a.v = getelementptr inbounds %Node, %Node* %a, i32 0, i32 0
  store i32 1, i32* %a.v, align 4
  store %Node zeroinitializer, %Node* %b, align 8
  %b.v = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  store i32 2, i32* %b.v, align 4
  %a.next = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  store %Node* %b, %Node** %a.next, align 8
  store %Lst zeroinitializer, %Lst* %lst, align 8
  %lst.head = getelementptr inbounds %Lst, %Lst* %lst, i32 0, i32 0
  store %Node* %a, %Node** %lst.head, align 8
  %lst.head1 = getelementptr inbounds %Lst, %Lst* %lst, i32 0, i32 0
  %lst.head2 = getelementptr inbounds %Lst, %Lst* %lst, i32 0, i32 0
  %"*lst.head" = load %Node*, %Node** %lst.head2, align 8
  %"*lst.head.next" = getelementptr inbounds %Node, %Node* %"*lst.head", i32 0, i32 1
  %"$*lst.head.next" = load %Node*, %Node** %"*lst.head.next", align 8
  store %Node* %"$*lst.head.next", %Node** %lst.head1, align 8
  %lst.head3 = getelementptr inbounds %Lst, %Lst* %lst, i32 0, i32 0
  %"*lst.head4" = load %Node*, %Node** %lst.head3, align 8
  %"*lst.head.v" = getelementptr inbounds %Node, %Node* %"*lst.head4", i32 0, i32 0
  %"$*lst.head.v" = load i32, i32* %"*lst.head.v", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$*lst.head.v")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str.1 = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1

define i32 @main() {
entry:
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  %cur = alloca i32*, align 8
  store i32 10, i32* %x, align 4
  store i32 20, i32* %y, align 4
  store i32* %x, i32** %cur, align 8
  %"*cur" = load i32*, i32** %cur, align 8
  store i32 99, i32* %"*cur", align 4
  %"$x" = load i32, i32* %x, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$x")
  store i32* %y, i32** %cur, align 8
  %"*cur1" = load i32*, i32** %cur, align 8
  store i32 88, i32* %"*cur1", align 4
  %"$x2" = load i32, i32* %x, align 4
  %"$y" = load i32, i32* %y, align 4
  %call.dprintf3 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0), i32 %"$x2", i32 %"$y")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%N = type { i32 }

@str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.2 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.4 = private unnamed_addr constant [7 x i8] c"%s %s\0A\00", align 1

define i32 @main() {
entry:
  %a = alloca %N, align 8
  %b = alloca %N, align 8
  %bool.text = alloca i8*, align 8
  %bool.text3 = alloca i8*, align 8
  store %N zeroinitializer, %N* %a, align 4
  store %N zeroinitializer, %N* %b, align 4
  %EQ = icmp eq %N* %a, %a
  %EQ2 = icmp eq %N* %a, %b
  store i8* null, i8** %bool.text, align 8
  br label %if0

if0:                                              ; preds = %entry
  br i1 %EQ, label %if0.then, label %else1

if0.then:                                         ; preds = %if0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else1:                                            ; preds = %if0
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else1, %if0.then
  store i8* null, i8** %bool.text3, align 8
  br label %if2

if2:                                              ; preds = %endif
  br i1 %EQ2, label %if2.then, label %else3

if2.then:                                         ; preds = %if2
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.2, i32 0, i32 0), i8** %bool.text3, align 8
  br label %endif1

else3:                                            ; preds = %if2
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i8** %bool.text3, align 8
  br label %endif1

endif1:                                           ; preds = %else3, %if2.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %"$bool.text4" = load i8*, i8** %bool.text3, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.4, i32 0, i32 0), i8* %"$bool.text", i8* %"$bool.text4")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
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

### stderr

```
error: test.ura:3:14 'm' is a ref, initialize it with an address
  |
3 |     m &i32 = 5
  |              ^
error: aborting due to 1 error
```

### status

```
1
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

### stderr

```
error: test.ura:4:12 'x' is never null, so this comparison is always the same
  |
4 |     output(x == null, "\n")
  |            ^
error: aborting due to 1 error
```

### status

```
1
```

