# milestone3

## index

- 001 — global scope: no main: the file is the program
- 002 — struct: declare: attributes start at zero
- 003 — struct: write an attribute, read it back
- 004 — struct: attributes don't overlap
- 005 — struct: compound assignment on an attribute
- 006 — struct: two variables of one struct are separate
- 007 — struct: attributes of different types
- 008 — struct: nested struct: r.b.x
- 009 — struct: attributes in a loop
- 010 — struct: array attribute
- 011 — struct: print attributes
- 012 — struct: a function returns a struct
- 013 — struct: a ref variable to a struct
- 014 — struct: write through a ref parameter
- 015 — struct: read through a ref parameter
- 016 — method: a method reads self
- 017 — method: a method writes self
- 018 — method: a method reads and writes self
- 019 — method: a method with arguments
- 020 — method: a method calls another method
- 021 — method: a method returns a bool
- 022 — method: a method in a loop
- 023 — method: two structs, one method name
- 024 — method: a method on a nested attribute
- 025 — method: overloaded method
- 026 — method: a method on a ref receiver
- 027 — method: a method through a ref changes the caller
- 028 — crazy: a method called three times through a ref
- 029 — crazy: methods of a nested struct, called through self
- 030 — crazy: bubble sort: a struct with an array, methods calling methods
- 031 — crazy: bank account: methods, a ref parameter, an array of bills
- 032 — crazy: an array of structs is a copy
- 033 — crazy: vectors: struct parameters, struct returns, chained calls
- 034 — crazy: linked list: ref attributes, isNullable, rebinding a ref in a loop
- 035 — crazy: a recursive method that counts its calls
- 036 — crazy: a method takes a ref to its own struct type
- 037 — crazy: stack: push squares, pop until empty
- 038 — crazy: ring-buffer queue: full, wrap around, drain
- 039 — crazy: binary tree: recursion through ref attributes
- 040 — crazy: a linked list over array elements
- 041 — crazy: search a linked list, -1 when missing
- 042 — crazy: assigning a struct copies it
- 043 — crazy: a struct parameter is a copy
- 044 — crazy: compound assignment through a ref attribute
- 045 — crazy: a method on an array element
- 046 — crazy: builder: a method returns a ref to self, chained
- 047 — crazy: a struct holding an array of structs
- 048 — crazy: tree walk with a ref out-parameter
- 049 — crazy: a function returns a ref into a struct, the caller binds it
- 050 — crazy: fibonacci as a pair: recursion returning a struct, .a on a call
- 051 — crazy: method calls nested as arguments
- 052 — crazy: print a linked list backwards with recursion
- 053 — crazy: sort structs in an array by an attribute
- 054 — crazy: ping pong: two structs that name each other, before either is declared
- 055 — crazy: while True with break and return inside a method
- 056 — crazy: script: a global stack evaluates postfix 3 4 + 2 *
- 057 — crazy: structs used before declared, a struct local to a function
- 058 — crazy: a linked list built with chained methods that return refs
- 059 — crazy: binary search tree: recursive methods through ref attributes
- 060 — crazy: 2x2 matrix product with methods and ref parameters
- 061 — name error: a struct declared twice
- 062 — name error: unknown attribute
- 063 — name error: unknown method
- 064 — name error: self outside a method
- 065 — structs: missing name
- 066 — structs: missing colon
- 067 — structs: expression inside a struct
- 068 — structs: attribute without a type
- 069 — structs: attribute declared twice
- 070 — structs: no attributes
- 071 — structs: contains itself by value
- 072 — structs: unknown attribute
- 073 — structs: unknown method
- 074 — structs: method with wrong arguments
- 075 — structs: bare attribute inside a method
- 076 — structs: bare method call inside a method
- 077 — structs: method called on the type
- 078 — structs: self outside a method
- 079 — expressions: add a struct and a number
- 080 — expressions: compare structs
- 081 — types: struct into another struct
- 082 — struct: init a struct
- 083 — struct: modify struct attribute
- 084 — struct: method reads and writes self
- 085 — struct: overloaded method
- 086 — struct: linked list
- 087 — program: linked list walked through ref attributes
- 088 — method: a method reads self
- 089 — method: a method mutates through self, and the caller sees it
- 090 — method: a method with a return value
- 091 — method: a method calling a sibling method declared below it
- 092 — method: two structs may declare the same method name
- 093 — method: a method on an array element
- 094 — method: a method on a nested struct field
- 095 — method: calling a method the struct does not have
- 096 — method: calling a method on a non-struct
- 097 — method: wrong argument count to a method
- 098 — method: chaining a field access onto a call
- 099 — struct: declare a struct and a local: named type, alloca, zero-init
- 100 — struct: a field whose type is another struct
- 101 — struct: forward reference: a field type declared later in the file
- 102 — struct: a struct declared inside a function body
- 103 — struct: a struct declared inside an if block
- 104 — struct: a struct declared inside a loop body
- 105 — struct: an array-of-struct field is a fat pointer to the named type
- 106 — struct: a struct nested inside another struct's body
- 107 — struct: self reference through an array stays finite
- 108 — struct: an unused struct emits no type: LLVM drops unreferenced named types
- 109 — struct: the same struct name in three nested scopes: innermost wins
- 110 — struct: read and write a field
- 111 — struct: nested field access through a struct field
- 112 — struct: a stack array of structs: Room[N]
- 113 — struct: a 2D array of structs: Room[N][M]
- 114 — struct: '.len' on arrays of structs, including 2D
- 115 — struct: 'stack' and 'heap' are ordinary identifiers
- 116 — struct: a 'ref?' field may refer to its own struct
- 117 — struct: bind, read and write through a 'ref?' field
- 118 — struct: struct assignment copies by value
- 119 — struct: a struct as a by-value parameter
- 120 — struct: a 'ref' struct parameter mutates the caller's struct
- 121 — struct: returning a struct: the create() constructor convention
- 122 — struct: a field declared twice
- 123 — struct: a field whose type does not exist
- 124 — struct: a struct that contains itself by value
- 125 — struct: two structs that contain each other by value
- 126 — struct: a variable of a type that was never declared
- 127 — struct: reading a field the struct does not have
- 128 — struct: reading a field from a scalar
- 129 — struct: '.len' on a scalar
- 130 — struct: a struct declared in a block is not visible outside it
- 131 — struct: a struct declared twice in the same scope
- 132 — struct: a method and a field chain called on a ref? field
- 133 — struct: struct redefinition
- 134 — struct: accessing an undefined field
- 135 — struct: calling undefined method on struct
- 136 — struct: a struct whose first field is a struct is not circular
- 137 — struct: a struct as a parameter and as a return value
- 138 — struct: assigning a struct copies it
- 139 — struct: a struct declared inside a function
- 140 — struct: a nested struct as a parameter and a copy
- 141 — struct: a struct value {x: 10, y: 11}: missing attributes are zero, nested, returned
- 142 — struct: a struct value with an unknown attribute
- 143 — struct: a struct value given to an i32

---

## 001 — global scope: no main: the file is the program

```ura
// expect: 40   (23 at 4 * 10, 4 at 0, 99 missing: 0)
struct Finder:
    data i32[]
    size i32

    fn index_of(v i32) i32:
        i i32 = 0
        while True:
            if i == self.size: break
            if self.data[i] == v: return i
            i += 1
        return -1

f Finder
f.data = [4, 8, 15, 16, 23, 42]
f.size = 6
return f.index_of(23) * 10 + f.index_of(4) + (f.index_of(99) + 1) * 100
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Finder = type { i32*, i32 }

@f = global %Finder zeroinitializer

define i32 @Finder.index_of(%Finder* %0, i32 %1) {
entry:
  %self = alloca %Finder*, align 8
  %v = alloca i32, align 4
  %i = alloca i32, align 4
  store %Finder* %0, %Finder** %self, align 8
  store i32 %1, i32* %v, align 4
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %endif1, %entry
  br i1 true, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  br label %if1

if1:                                              ; preds = %while0.then
  %"*self" = load %Finder*, %Finder** %self, align 8
  %"*self.size" = getelementptr inbounds %Finder, %Finder* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.size" = load i32, i32* %"*self.size", align 4
  %EQ = icmp eq i32 %"$i", %"$*self.size"
  br i1 %EQ, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  br label %endloop

endif:                                            ; preds = %if1
  br label %if2

if2:                                              ; preds = %endif
  %"*self2" = load %Finder*, %Finder** %self, align 8
  %"*self.data" = getelementptr inbounds %Finder, %Finder* %"*self2", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$i3" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$i3"
  %"$*self.data[]" = load i32, i32* %"*self.data[]", align 4
  %"$v" = load i32, i32* %v, align 4
  %EQ4 = icmp eq i32 %"$*self.data[]", %"$v"
  br i1 %EQ4, label %if2.then, label %endif1

if2.then:                                         ; preds = %if2
  %"$i5" = load i32, i32* %i, align 4
  ret i32 %"$i5"

endif1:                                           ; preds = %if2
  %"$i6" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i6", 1
  store i32 %ADD, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %if1.then, %while0
  ret i32 -1
}

define i32 @main() {
entry:
  %arr = alloca [6 x i32], align 4
  store [6 x i32] [i32 4, i32 8, i32 15, i32 16, i32 23, i32 42], [6 x i32]* %arr, align 4
  %0 = getelementptr inbounds [6 x i32], [6 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** getelementptr inbounds (%Finder, %Finder* @f, i32 0, i32 0), align 8
  store i32 6, i32* getelementptr inbounds (%Finder, %Finder* @f, i32 0, i32 1), align 4
  %call.Finder.index_of = call i32 @Finder.index_of(%Finder* @f, i32 23)
  %MUL = mul i32 %call.Finder.index_of, 10
  %call.Finder.index_of1 = call i32 @Finder.index_of(%Finder* @f, i32 4)
  %ADD = add i32 %MUL, %call.Finder.index_of1
  %call.Finder.index_of2 = call i32 @Finder.index_of(%Finder* @f, i32 99)
  %ADD3 = add i32 %call.Finder.index_of2, 1
  %MUL4 = mul i32 %ADD3, 100
  %ADD5 = add i32 %ADD, %MUL4
  ret i32 %ADD5
}
```

---

## 002 — struct: declare: attributes start at zero

```ura
// expect: 0
struct User:
    a i32
    b i32

fn main() i32:
    u User
    return u.a + u.b
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32, i32 }

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %u.b = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  %"$u.a" = load i32, i32* %u.a, align 4
  %"$u.b" = load i32, i32* %u.b, align 4
  %ADD = add i32 %"$u.a", %"$u.b"
  ret i32 %ADD
}
```

---

## 003 — struct: write an attribute, read it back

```ura
// expect: 7
struct User:
    a i32
    b i32

fn main() i32:
    u User
    u.b = 7
    return u.b
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32, i32 }

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.b = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  store i32 7, i32* %u.b, align 4
  %u.b1 = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  %"$u.b" = load i32, i32* %u.b1, align 4
  ret i32 %"$u.b"
}
```

---

## 004 — struct: attributes don't overlap

```ura
// expect: 34
struct User:
    a i32
    b i32

fn main() i32:
    u User
    u.a = 3
    u.b = 4
    return u.a * 10 + u.b
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32, i32 }

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 3, i32* %u.a, align 4
  %u.b = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  store i32 4, i32* %u.b, align 4
  %u.a1 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a" = load i32, i32* %u.a1, align 4
  %MUL = mul i32 %"$u.a", 10
  %u.b2 = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  %"$u.b" = load i32, i32* %u.b2, align 4
  %ADD = add i32 %MUL, %"$u.b"
  ret i32 %ADD
}
```

---

## 005 — struct: compound assignment on an attribute

```ura
// expect: 21   ((5 + 2) * 3)
struct User:
    a i32

fn main() i32:
    u User
    u.a = 5
    u.a += 2
    u.a *= 3
    return u.a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32 }

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 5, i32* %u.a, align 4
  %u.a1 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a" = load i32, i32* %u.a1, align 4
  %ADD = add i32 %"$u.a", 2
  store i32 %ADD, i32* %u.a1, align 4
  %u.a2 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a3" = load i32, i32* %u.a2, align 4
  %MUL = mul i32 %"$u.a3", 3
  store i32 %MUL, i32* %u.a2, align 4
  %u.a4 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a5" = load i32, i32* %u.a4, align 4
  ret i32 %"$u.a5"
}
```

---

## 006 — struct: two variables of one struct are separate

```ura
// expect: 12
struct User:
    a i32

fn main() i32:
    u User
    v User
    u.a = 1
    v.a = 2
    return u.a * 10 + v.a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32 }

define i32 @main() {
entry:
  %u = alloca %User, align 8
  %v = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  store %User zeroinitializer, %User* %v, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 1, i32* %u.a, align 4
  %v.a = getelementptr inbounds %User, %User* %v, i32 0, i32 0
  store i32 2, i32* %v.a, align 4
  %u.a1 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a" = load i32, i32* %u.a1, align 4
  %MUL = mul i32 %"$u.a", 10
  %v.a2 = getelementptr inbounds %User, %User* %v, i32 0, i32 0
  %"$v.a" = load i32, i32* %v.a2, align 4
  %ADD = add i32 %MUL, %"$v.a"
  ret i32 %ADD
}
```

---

## 007 — struct: attributes of different types

```ura
// expect: 4
struct Item:
    id i32
    used bool
    tag char

fn main() i32:
    it Item
    it.id = 4
    it.used = True
    it.tag = 'x'
    if it.used and it.tag == 'x':
        return it.id
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Item = type { i32, i1, i8 }

define i32 @main() {
entry:
  %it = alloca %Item, align 8
  store %Item zeroinitializer, %Item* %it, align 4
  %it.id = getelementptr inbounds %Item, %Item* %it, i32 0, i32 0
  store i32 4, i32* %it.id, align 4
  %it.used = getelementptr inbounds %Item, %Item* %it, i32 0, i32 1
  store i1 true, i1* %it.used, align 1
  %it.tag = getelementptr inbounds %Item, %Item* %it, i32 0, i32 2
  store i8 120, i8* %it.tag, align 1
  br label %if0

if0:                                              ; preds = %entry
  %it.used1 = getelementptr inbounds %Item, %Item* %it, i32 0, i32 1
  %it.tag2 = getelementptr inbounds %Item, %Item* %it, i32 0, i32 2
  %"$it.tag" = load i8, i8* %it.tag2, align 1
  %EQ = icmp eq i8 %"$it.tag", 120
  %"$it.used" = load i1, i1* %it.used1, align 1
  %AND = and i1 %"$it.used", %EQ
  br i1 %AND, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %it.id3 = getelementptr inbounds %Item, %Item* %it, i32 0, i32 0
  %"$it.id" = load i32, i32* %it.id3, align 4
  ret i32 %"$it.id"

endif:                                            ; preds = %if0
  ret i32 0
}
```

---

## 008 — struct: nested struct: r.b.x

```ura
// expect: 6   (5 - 1 + 2)
struct Point:
    x i32
    y i32

struct Rect:
    a Point
    b Point

fn main() i32:
    r Rect
    r.a.x = 1
    r.b.x = 5
    r.b.y = 2
    return r.b.x - r.a.x + r.b.y
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Rect = type { %Point, %Point }
%Point = type { i32, i32 }

define i32 @main() {
entry:
  %r = alloca %Rect, align 8
  store %Rect zeroinitializer, %Rect* %r, align 4
  %r.a = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 0
  %r.a.x = getelementptr inbounds %Point, %Point* %r.a, i32 0, i32 0
  store i32 1, i32* %r.a.x, align 4
  %r.b = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %r.b.x = getelementptr inbounds %Point, %Point* %r.b, i32 0, i32 0
  store i32 5, i32* %r.b.x, align 4
  %r.b1 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %r.b.y = getelementptr inbounds %Point, %Point* %r.b1, i32 0, i32 1
  store i32 2, i32* %r.b.y, align 4
  %r.b2 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %r.b.x3 = getelementptr inbounds %Point, %Point* %r.b2, i32 0, i32 0
  %r.a4 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 0
  %r.a.x5 = getelementptr inbounds %Point, %Point* %r.a4, i32 0, i32 0
  %"$r.b.x" = load i32, i32* %r.b.x3, align 4
  %"$r.a.x" = load i32, i32* %r.a.x5, align 4
  %SUB = sub i32 %"$r.b.x", %"$r.a.x"
  %r.b6 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %r.b.y7 = getelementptr inbounds %Point, %Point* %r.b6, i32 0, i32 1
  %"$r.b.y" = load i32, i32* %r.b.y7, align 4
  %ADD = add i32 %SUB, %"$r.b.y"
  ret i32 %ADD
}
```

---

## 009 — struct: attributes in a loop

```ura
// expect: 10   (1 + 2 + 3 + 4)
struct Counter:
    n i32
    total i32

fn main() i32:
    c Counter
    while c.n < 4:
        c.n += 1
        c.total += c.n
    return c.total
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Counter = type { i32, i32 }

define i32 @main() {
entry:
  %c = alloca %Counter, align 8
  store %Counter zeroinitializer, %Counter* %c, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %c.n = getelementptr inbounds %Counter, %Counter* %c, i32 0, i32 0
  %"$c.n" = load i32, i32* %c.n, align 4
  %LT = icmp slt i32 %"$c.n", 4
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %c.n1 = getelementptr inbounds %Counter, %Counter* %c, i32 0, i32 0
  %"$c.n2" = load i32, i32* %c.n1, align 4
  %ADD = add i32 %"$c.n2", 1
  store i32 %ADD, i32* %c.n1, align 4
  %c.total = getelementptr inbounds %Counter, %Counter* %c, i32 0, i32 1
  %c.n3 = getelementptr inbounds %Counter, %Counter* %c, i32 0, i32 0
  %"$c.n4" = load i32, i32* %c.n3, align 4
  %"$c.total" = load i32, i32* %c.total, align 4
  %ADD5 = add i32 %"$c.total", %"$c.n4"
  store i32 %ADD5, i32* %c.total, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %c.total6 = getelementptr inbounds %Counter, %Counter* %c, i32 0, i32 1
  %"$c.total7" = load i32, i32* %c.total6, align 4
  ret i32 %"$c.total7"
}
```

---

## 010 — struct: array attribute

```ura
// expect: 12
struct Row:
    cells i32[]

fn main() i32:
    r Row
    r.cells = [1, 2, 3]
    r.cells[0] = 9
    return r.cells[0] + r.cells[2]
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Row = type { i32* }

define i32 @main() {
entry:
  %r = alloca %Row, align 8
  %arr = alloca [3 x i32], align 4
  store %Row zeroinitializer, %Row* %r, align 8
  %r.cells = getelementptr inbounds %Row, %Row* %r, i32 0, i32 0
  store [3 x i32] [i32 1, i32 2, i32 3], [3 x i32]* %arr, align 4
  %0 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %r.cells, align 8
  %r.cells1 = getelementptr inbounds %Row, %Row* %r, i32 0, i32 0
  %"$r.cells" = load i32*, i32** %r.cells1, align 8
  %"r.cells[]" = getelementptr inbounds i32, i32* %"$r.cells", i32 0
  store i32 9, i32* %"r.cells[]", align 4
  %r.cells2 = getelementptr inbounds %Row, %Row* %r, i32 0, i32 0
  %"$r.cells3" = load i32*, i32** %r.cells2, align 8
  %"r.cells[]4" = getelementptr inbounds i32, i32* %"$r.cells3", i32 0
  %r.cells5 = getelementptr inbounds %Row, %Row* %r, i32 0, i32 0
  %"$r.cells6" = load i32*, i32** %r.cells5, align 8
  %"r.cells[]7" = getelementptr inbounds i32, i32* %"$r.cells6", i32 2
  %"$r.cells[]" = load i32, i32* %"r.cells[]4", align 4
  %"$r.cells[]8" = load i32, i32* %"r.cells[]7", align 4
  %ADD = add i32 %"$r.cells[]", %"$r.cells[]8"
  ret i32 %ADD
}
```

---

## 011 — struct: print attributes

```ura
// stdout: ura 42
// expect: 0
proto printf(fmt i8[], ...) i32

struct User:
    id i32
    name i8[]

fn main() i32:
    u User
    u.id = 42
    u.name = "ura"
    printf("%s %d\n", u.name, u.id)
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32, i8* }

@str = private unnamed_addr constant [4 x i8] c"ura\00", align 1
@str.1 = private unnamed_addr constant [7 x i8] c"%s %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 8
  %u.id = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 42, i32* %u.id, align 4
  %u.name = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  store i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i8** %u.name, align 8
  %u.name1 = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  %u.id2 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.name" = load i8*, i8** %u.name1, align 8
  %"$u.id" = load i32, i32* %u.id2, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0), i8* %"$u.name", i32 %"$u.id")
  ret i32 0
}
```

---

## 012 — struct: a function returns a struct

```ura
// expect: 5
struct User:
    a i32

fn make(a i32) User:
    u User
    u.a = a
    return u

fn main() i32:
    u User = make(5)
    return u.a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32 }

define %User @make(i32 %0) {
entry:
  %a = alloca i32, align 4
  %u = alloca %User, align 8
  store i32 %0, i32* %a, align 4
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$a" = load i32, i32* %a, align 4
  store i32 %"$a", i32* %u.a, align 4
  %"$u" = load %User, %User* %u, align 4
  ret %User %"$u"
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  %call.make = call %User @make(i32 5)
  store %User %call.make, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a" = load i32, i32* %u.a, align 4
  ret i32 %"$u.a"
}
```

---

## 013 — struct: a ref variable to a struct

```ura
// expect: 8
struct User:
    a i32

fn main() i32:
    u User
    p &User = &u
    p.a = 8
    return u.a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32 }

define i32 @main() {
entry:
  %u = alloca %User, align 8
  %p = alloca %User*, align 8
  store %User zeroinitializer, %User* %u, align 4
  store %User* %u, %User** %p, align 8
  %"*p" = load %User*, %User** %p, align 8
  %"*p.a" = getelementptr inbounds %User, %User* %"*p", i32 0, i32 0
  store i32 8, i32* %"*p.a", align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a" = load i32, i32* %u.a, align 4
  ret i32 %"$u.a"
}
```

---

## 014 — struct: write through a ref parameter

```ura
// expect: 3
struct User:
    a i32

fn set(p &User):
    p.a = 3

fn main() i32:
    u User
    set(&u)
    return u.a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32 }

define void @set(%User* %0) {
entry:
  %p = alloca %User*, align 8
  store %User* %0, %User** %p, align 8
  %"*p" = load %User*, %User** %p, align 8
  %"*p.a" = getelementptr inbounds %User, %User* %"*p", i32 0, i32 0
  store i32 3, i32* %"*p.a", align 4
  ret void
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  call void @set(%User* %u)
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a" = load i32, i32* %u.a, align 4
  ret i32 %"$u.a"
}
```

---

## 015 — struct: read through a ref parameter

```ura
// expect: 7
struct User:
    a i32
    b i32

fn total(p &User) i32:
    return p.a + p.b

fn main() i32:
    u User
    u.a = 2
    u.b = 5
    return total(&u)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32, i32 }

define i32 @total(%User* %0) {
entry:
  %p = alloca %User*, align 8
  store %User* %0, %User** %p, align 8
  %"*p" = load %User*, %User** %p, align 8
  %"*p.a" = getelementptr inbounds %User, %User* %"*p", i32 0, i32 0
  %"*p1" = load %User*, %User** %p, align 8
  %"*p.b" = getelementptr inbounds %User, %User* %"*p1", i32 0, i32 1
  %"$*p.a" = load i32, i32* %"*p.a", align 4
  %"$*p.b" = load i32, i32* %"*p.b", align 4
  %ADD = add i32 %"$*p.a", %"$*p.b"
  ret i32 %ADD
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 2, i32* %u.a, align 4
  %u.b = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  store i32 5, i32* %u.b, align 4
  %call.total = call i32 @total(%User* %u)
  ret i32 %call.total
}
```

---

## 016 — method: a method reads self

```ura
// expect: 4
struct User:
    a i32

    fn get() i32:
        return self.a

fn main() i32:
    u User
    u.a = 4
    return u.get()
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32 }

define i32 @User.get(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %"*self" = load %User*, %User** %self, align 8
  %"*self.a" = getelementptr inbounds %User, %User* %"*self", i32 0, i32 0
  %"$*self.a" = load i32, i32* %"*self.a", align 4
  ret i32 %"$*self.a"
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 4, i32* %u.a, align 4
  %call.User.get = call i32 @User.get(%User* %u)
  ret i32 %call.User.get
}
```

---

## 017 — method: a method writes self

```ura
// expect: 9
struct User:
    a i32

    fn set(n i32):
        self.a = n

fn main() i32:
    u User
    u.set(9)
    return u.a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32 }

define void @User.set(%User* %0, i32 %1) {
entry:
  %self = alloca %User*, align 8
  %n = alloca i32, align 4
  store %User* %0, %User** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %User*, %User** %self, align 8
  %"*self.a" = getelementptr inbounds %User, %User* %"*self", i32 0, i32 0
  %"$n" = load i32, i32* %n, align 4
  store i32 %"$n", i32* %"*self.a", align 4
  ret void
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  call void @User.set(%User* %u, i32 9)
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a" = load i32, i32* %u.a, align 4
  ret i32 %"$u.a"
}
```

---

## 018 — method: a method reads and writes self

```ura
// expect: 7
struct User:
    a i32

    fn bump() i32:
        self.a += 1
        return self.a

fn main() i32:
    u User
    u.a = 5
    u.bump()
    return u.bump()
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32 }

define i32 @User.bump(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %"*self" = load %User*, %User** %self, align 8
  %"*self.a" = getelementptr inbounds %User, %User* %"*self", i32 0, i32 0
  %"$*self.a" = load i32, i32* %"*self.a", align 4
  %ADD = add i32 %"$*self.a", 1
  store i32 %ADD, i32* %"*self.a", align 4
  %"*self1" = load %User*, %User** %self, align 8
  %"*self.a2" = getelementptr inbounds %User, %User* %"*self1", i32 0, i32 0
  %"$*self.a3" = load i32, i32* %"*self.a2", align 4
  ret i32 %"$*self.a3"
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 5, i32* %u.a, align 4
  %call.User.bump = call i32 @User.bump(%User* %u)
  %call.User.bump1 = call i32 @User.bump(%User* %u)
  ret i32 %call.User.bump1
}
```

---

## 019 — method: a method with arguments

```ura
// expect: 8
struct User:
    a i32

    fn add(n i32) i32:
        return self.a + n

fn main() i32:
    u User
    u.a = 5
    return u.add(3)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32 }

define i32 @User.add(%User* %0, i32 %1) {
entry:
  %self = alloca %User*, align 8
  %n = alloca i32, align 4
  store %User* %0, %User** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %User*, %User** %self, align 8
  %"*self.a" = getelementptr inbounds %User, %User* %"*self", i32 0, i32 0
  %"$*self.a" = load i32, i32* %"*self.a", align 4
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$*self.a", %"$n"
  ret i32 %ADD
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 5, i32* %u.a, align 4
  %call.User.add = call i32 @User.add(%User* %u, i32 3)
  ret i32 %call.User.add
}
```

---

## 020 — method: a method calls another method

```ura
// expect: 12
struct User:
    a i32

    fn get() i32:
        return self.a

    fn twice() i32:
        return self.get() * 2

fn main() i32:
    u User
    u.a = 6
    return u.twice()
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32 }

define i32 @User.get(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %"*self" = load %User*, %User** %self, align 8
  %"*self.a" = getelementptr inbounds %User, %User* %"*self", i32 0, i32 0
  %"$*self.a" = load i32, i32* %"*self.a", align 4
  ret i32 %"$*self.a"
}

define i32 @User.twice(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %"*self" = load %User*, %User** %self, align 8
  %call.User.get = call i32 @User.get(%User* %"*self")
  %MUL = mul i32 %call.User.get, 2
  ret i32 %MUL
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 6, i32* %u.a, align 4
  %call.User.twice = call i32 @User.twice(%User* %u)
  ret i32 %call.User.twice
}
```

---

## 021 — method: a method returns a bool

```ura
// expect: 1
struct User:
    age i32

    fn adult() bool:
        return self.age >= 18

fn main() i32:
    u User
    u.age = 20
    if u.adult():
        return 1
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32 }

define i1 @User.adult(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %"*self" = load %User*, %User** %self, align 8
  %"*self.age" = getelementptr inbounds %User, %User* %"*self", i32 0, i32 0
  %"$*self.age" = load i32, i32* %"*self.age", align 4
  %GE = icmp sge i32 %"$*self.age", 18
  ret i1 %GE
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.age = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 20, i32* %u.age, align 4
  br label %if0

if0:                                              ; preds = %entry
  %call.User.adult = call i1 @User.adult(%User* %u)
  br i1 %call.User.adult, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  ret i32 0
}
```

---

## 022 — method: a method in a loop

```ura
// stdout: 3
// expect: 3
proto printf(fmt i8[], ...) i32

struct Counter:
    n i32

    fn tick():
        self.n += 1

fn main() i32:
    c Counter
    i i32 = 0
    while i < 3:
        c.tick()
        i += 1
    printf("%d\n", c.n)
    return c.n
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Counter = type { i32 }

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @Counter.tick(%Counter* %0) {
entry:
  %self = alloca %Counter*, align 8
  store %Counter* %0, %Counter** %self, align 8
  %"*self" = load %Counter*, %Counter** %self, align 8
  %"*self.n" = getelementptr inbounds %Counter, %Counter* %"*self", i32 0, i32 0
  %"$*self.n" = load i32, i32* %"*self.n", align 4
  %ADD = add i32 %"$*self.n", 1
  store i32 %ADD, i32* %"*self.n", align 4
  ret void
}

define i32 @main() {
entry:
  %c = alloca %Counter, align 8
  %i = alloca i32, align 4
  store %Counter zeroinitializer, %Counter* %c, align 4
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  call void @Counter.tick(%Counter* %c)
  %"$i1" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i1", 1
  store i32 %ADD, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %c.n = getelementptr inbounds %Counter, %Counter* %c, i32 0, i32 0
  %"$c.n" = load i32, i32* %c.n, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$c.n")
  %c.n2 = getelementptr inbounds %Counter, %Counter* %c, i32 0, i32 0
  %"$c.n3" = load i32, i32* %c.n2, align 4
  ret i32 %"$c.n3"
}
```

---

## 023 — method: two structs, one method name

```ura
// expect: 21   (1 + 2 * 10)
struct A:
    v i32

    fn get() i32:
        return self.v

struct B:
    v i32

    fn get() i32:
        return self.v * 10

fn main() i32:
    a A
    b B
    a.v = 1
    b.v = 2
    return a.get() + b.get()
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%A = type { i32 }
%B = type { i32 }

define i32 @A.get(%A* %0) {
entry:
  %self = alloca %A*, align 8
  store %A* %0, %A** %self, align 8
  %"*self" = load %A*, %A** %self, align 8
  %"*self.v" = getelementptr inbounds %A, %A* %"*self", i32 0, i32 0
  %"$*self.v" = load i32, i32* %"*self.v", align 4
  ret i32 %"$*self.v"
}

define i32 @B.get(%B* %0) {
entry:
  %self = alloca %B*, align 8
  store %B* %0, %B** %self, align 8
  %"*self" = load %B*, %B** %self, align 8
  %"*self.v" = getelementptr inbounds %B, %B* %"*self", i32 0, i32 0
  %"$*self.v" = load i32, i32* %"*self.v", align 4
  %MUL = mul i32 %"$*self.v", 10
  ret i32 %MUL
}

define i32 @main() {
entry:
  %a = alloca %A, align 8
  %b = alloca %B, align 8
  store %A zeroinitializer, %A* %a, align 4
  store %B zeroinitializer, %B* %b, align 4
  %a.v = getelementptr inbounds %A, %A* %a, i32 0, i32 0
  store i32 1, i32* %a.v, align 4
  %b.v = getelementptr inbounds %B, %B* %b, i32 0, i32 0
  store i32 2, i32* %b.v, align 4
  %call.A.get = call i32 @A.get(%A* %a)
  %call.B.get = call i32 @B.get(%B* %b)
  %ADD = add i32 %call.A.get, %call.B.get
  ret i32 %ADD
}
```

---

## 024 — method: a method on a nested attribute

```ura
// expect: 7
struct Point:
    x i32

    fn get() i32:
        return self.x

struct Rect:
    a Point
    b Point

fn main() i32:
    r Rect
    r.b.x = 7
    return r.b.get()
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Point = type { i32 }
%Rect = type { %Point, %Point }

define i32 @Point.get(%Point* %0) {
entry:
  %self = alloca %Point*, align 8
  store %Point* %0, %Point** %self, align 8
  %"*self" = load %Point*, %Point** %self, align 8
  %"*self.x" = getelementptr inbounds %Point, %Point* %"*self", i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  ret i32 %"$*self.x"
}

define i32 @main() {
entry:
  %r = alloca %Rect, align 8
  store %Rect zeroinitializer, %Rect* %r, align 4
  %r.b = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %r.b.x = getelementptr inbounds %Point, %Point* %r.b, i32 0, i32 0
  store i32 7, i32* %r.b.x, align 4
  %r.b1 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %call.Point.get = call i32 @Point.get(%Point* %r.b1)
  ret i32 %call.Point.get
}
```

---

## 025 — method: overloaded method

```ura
// expect: 6   (1 + 5)
struct User:
    a i32

    fn set(n i32):
        self.a = n

    fn set():
        self.a = 1

fn main() i32:
    u User
    u.set()
    first i32 = u.a
    u.set(5)
    return first + u.a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32 }

define void @User.set(%User* %0, i32 %1) {
entry:
  %self = alloca %User*, align 8
  %n = alloca i32, align 4
  store %User* %0, %User** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %User*, %User** %self, align 8
  %"*self.a" = getelementptr inbounds %User, %User* %"*self", i32 0, i32 0
  %"$n" = load i32, i32* %n, align 4
  store i32 %"$n", i32* %"*self.a", align 4
  ret void
}

define void @User.set.1(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %"*self" = load %User*, %User** %self, align 8
  %"*self.a" = getelementptr inbounds %User, %User* %"*self", i32 0, i32 0
  store i32 1, i32* %"*self.a", align 4
  ret void
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  %first = alloca i32, align 4
  store %User zeroinitializer, %User* %u, align 4
  call void @User.set.1(%User* %u)
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a" = load i32, i32* %u.a, align 4
  store i32 %"$u.a", i32* %first, align 4
  call void @User.set(%User* %u, i32 5)
  %u.a1 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$first" = load i32, i32* %first, align 4
  %"$u.a2" = load i32, i32* %u.a1, align 4
  %ADD = add i32 %"$first", %"$u.a2"
  ret i32 %ADD
}
```

---

## 026 — method: a method on a ref receiver

```ura
// expect: 4
struct User:
    a i32

    fn get() i32:
        return self.a

fn via(p &User) i32:
    return p.get()

fn main() i32:
    u User
    u.a = 4
    return via(&u)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32 }

define i32 @User.get(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %"*self" = load %User*, %User** %self, align 8
  %"*self.a" = getelementptr inbounds %User, %User* %"*self", i32 0, i32 0
  %"$*self.a" = load i32, i32* %"*self.a", align 4
  ret i32 %"$*self.a"
}

define i32 @via(%User* %0) {
entry:
  %p = alloca %User*, align 8
  store %User* %0, %User** %p, align 8
  %"*p" = load %User*, %User** %p, align 8
  %call.User.get = call i32 @User.get(%User* %"*p")
  ret i32 %call.User.get
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 4, i32* %u.a, align 4
  %call.via = call i32 @via(%User* %u)
  ret i32 %call.via
}
```

---

## 027 — method: a method through a ref changes the caller

```ura
// expect: 11
struct User:
    a i32

    fn set(n i32):
        self.a = n

fn reset(p &User):
    p.set(11)

fn main() i32:
    u User
    u.a = 3
    reset(&u)
    return u.a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32 }

define void @User.set(%User* %0, i32 %1) {
entry:
  %self = alloca %User*, align 8
  %n = alloca i32, align 4
  store %User* %0, %User** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %User*, %User** %self, align 8
  %"*self.a" = getelementptr inbounds %User, %User* %"*self", i32 0, i32 0
  %"$n" = load i32, i32* %n, align 4
  store i32 %"$n", i32* %"*self.a", align 4
  ret void
}

define void @reset(%User* %0) {
entry:
  %p = alloca %User*, align 8
  store %User* %0, %User** %p, align 8
  %"*p" = load %User*, %User** %p, align 8
  call void @User.set(%User* %"*p", i32 11)
  ret void
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  store i32 3, i32* %u.a, align 4
  call void @reset(%User* %u)
  %u.a1 = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a" = load i32, i32* %u.a1, align 4
  ret i32 %"$u.a"
}
```

---

## 028 — crazy: a method called three times through a ref

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
target triple = "x86_64-unknown-linux-gnu"

%Pair = type { i32, i32 }

define void @Pair.swap(%Pair* %0) {
entry:
  %self = alloca %Pair*, align 8
  %t = alloca i32, align 4
  store %Pair* %0, %Pair** %self, align 8
  %"*self" = load %Pair*, %Pair** %self, align 8
  %"*self.a" = getelementptr inbounds %Pair, %Pair* %"*self", i32 0, i32 0
  %"$*self.a" = load i32, i32* %"*self.a", align 4
  store i32 %"$*self.a", i32* %t, align 4
  %"*self1" = load %Pair*, %Pair** %self, align 8
  %"*self.a2" = getelementptr inbounds %Pair, %Pair* %"*self1", i32 0, i32 0
  %"*self3" = load %Pair*, %Pair** %self, align 8
  %"*self.b" = getelementptr inbounds %Pair, %Pair* %"*self3", i32 0, i32 1
  %"$*self.b" = load i32, i32* %"*self.b", align 4
  store i32 %"$*self.b", i32* %"*self.a2", align 4
  %"*self4" = load %Pair*, %Pair** %self, align 8
  %"*self.b5" = getelementptr inbounds %Pair, %Pair* %"*self4", i32 0, i32 1
  %"$t" = load i32, i32* %t, align 4
  store i32 %"$t", i32* %"*self.b5", align 4
  ret void
}

define void @thrice(%Pair* %0) {
entry:
  %p = alloca %Pair*, align 8
  store %Pair* %0, %Pair** %p, align 8
  %"*p" = load %Pair*, %Pair** %p, align 8
  call void @Pair.swap(%Pair* %"*p")
  %"*p1" = load %Pair*, %Pair** %p, align 8
  call void @Pair.swap(%Pair* %"*p1")
  %"*p2" = load %Pair*, %Pair** %p, align 8
  call void @Pair.swap(%Pair* %"*p2")
  ret void
}

define i32 @main() {
entry:
  %x = alloca %Pair, align 8
  store %Pair zeroinitializer, %Pair* %x, align 4
  %x.a = getelementptr inbounds %Pair, %Pair* %x, i32 0, i32 0
  store i32 1, i32* %x.a, align 4
  %x.b = getelementptr inbounds %Pair, %Pair* %x, i32 0, i32 1
  store i32 2, i32* %x.b, align 4
  call void @thrice(%Pair* %x)
  %x.a1 = getelementptr inbounds %Pair, %Pair* %x, i32 0, i32 0
  %"$x.a" = load i32, i32* %x.a1, align 4
  %MUL = mul i32 %"$x.a", 10
  %x.b2 = getelementptr inbounds %Pair, %Pair* %x, i32 0, i32 1
  %"$x.b" = load i32, i32* %x.b2, align 4
  %ADD = add i32 %MUL, %"$x.b"
  ret i32 %ADD
}
```

---

## 029 — crazy: methods of a nested struct, called through self

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
target triple = "x86_64-unknown-linux-gnu"

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
  %"*self" = load %Point*, %Point** %self, align 8
  %"*self.x" = getelementptr inbounds %Point, %Point* %"*self", i32 0, i32 0
  %"$dx" = load i32, i32* %dx, align 4
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %ADD = add i32 %"$*self.x", %"$dx"
  store i32 %ADD, i32* %"*self.x", align 4
  %"*self1" = load %Point*, %Point** %self, align 8
  %"*self.y" = getelementptr inbounds %Point, %Point* %"*self1", i32 0, i32 1
  %"$dy" = load i32, i32* %dy, align 4
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  %ADD2 = add i32 %"$*self.y", %"$dy"
  store i32 %ADD2, i32* %"*self.y", align 4
  ret void
}

define void @Rect.shift(%Rect* %0, i32 %1) {
entry:
  %self = alloca %Rect*, align 8
  %d = alloca i32, align 4
  store %Rect* %0, %Rect** %self, align 8
  store i32 %1, i32* %d, align 4
  %"*self" = load %Rect*, %Rect** %self, align 8
  %"*self.a" = getelementptr inbounds %Rect, %Rect* %"*self", i32 0, i32 0
  %"$d" = load i32, i32* %d, align 4
  %"$d1" = load i32, i32* %d, align 4
  call void @Point.move(%Point* %"*self.a", i32 %"$d", i32 %"$d1")
  %"*self2" = load %Rect*, %Rect** %self, align 8
  %"*self.b" = getelementptr inbounds %Rect, %Rect* %"*self2", i32 0, i32 1
  %"$d3" = load i32, i32* %d, align 4
  %"$d4" = load i32, i32* %d, align 4
  call void @Point.move(%Point* %"*self.b", i32 %"$d3", i32 %"$d4")
  ret void
}

define i32 @Rect.area(%Rect* %0) {
entry:
  %self = alloca %Rect*, align 8
  store %Rect* %0, %Rect** %self, align 8
  %"*self" = load %Rect*, %Rect** %self, align 8
  %"*self.b" = getelementptr inbounds %Rect, %Rect* %"*self", i32 0, i32 1
  %"*self.b.x" = getelementptr inbounds %Point, %Point* %"*self.b", i32 0, i32 0
  %"*self1" = load %Rect*, %Rect** %self, align 8
  %"*self.a" = getelementptr inbounds %Rect, %Rect* %"*self1", i32 0, i32 0
  %"*self.a.x" = getelementptr inbounds %Point, %Point* %"*self.a", i32 0, i32 0
  %"$*self.b.x" = load i32, i32* %"*self.b.x", align 4
  %"$*self.a.x" = load i32, i32* %"*self.a.x", align 4
  %SUB = sub i32 %"$*self.b.x", %"$*self.a.x"
  %"*self2" = load %Rect*, %Rect** %self, align 8
  %"*self.b3" = getelementptr inbounds %Rect, %Rect* %"*self2", i32 0, i32 1
  %"*self.b.y" = getelementptr inbounds %Point, %Point* %"*self.b3", i32 0, i32 1
  %"*self4" = load %Rect*, %Rect** %self, align 8
  %"*self.a5" = getelementptr inbounds %Rect, %Rect* %"*self4", i32 0, i32 0
  %"*self.a.y" = getelementptr inbounds %Point, %Point* %"*self.a5", i32 0, i32 1
  %"$*self.b.y" = load i32, i32* %"*self.b.y", align 4
  %"$*self.a.y" = load i32, i32* %"*self.a.y", align 4
  %SUB6 = sub i32 %"$*self.b.y", %"$*self.a.y"
  %MUL = mul i32 %SUB, %SUB6
  ret i32 %MUL
}

define i32 @main() {
entry:
  %r = alloca %Rect, align 8
  store %Rect zeroinitializer, %Rect* %r, align 4
  %r.b = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  call void @Point.move(%Point* %r.b, i32 4, i32 5)
  call void @Rect.shift(%Rect* %r, i32 10)
  %call.Rect.area = call i32 @Rect.area(%Rect* %r)
  %r.a = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 0
  %r.a.x = getelementptr inbounds %Point, %Point* %r.a, i32 0, i32 0
  %"$r.a.x" = load i32, i32* %r.a.x, align 4
  %ADD = add i32 %call.Rect.area, %"$r.a.x"
  ret i32 %ADD
}
```

---

## 030 — crazy: bubble sort: a struct with an array, methods calling methods

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
target triple = "x86_64-unknown-linux-gnu"

%List = type { i32*, i32 }

@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.1 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

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
  %"*self" = load %List*, %List** %self, align 8
  %"*self.items" = getelementptr inbounds %List, %List* %"*self", i32 0, i32 0
  %"$*self.items" = load i32*, i32** %"*self.items", align 8
  %"$i" = load i32, i32* %i, align 4
  %"*self.items[]" = getelementptr inbounds i32, i32* %"$*self.items", i32 %"$i"
  %"$*self.items[]" = load i32, i32* %"*self.items[]", align 4
  store i32 %"$*self.items[]", i32* %t, align 4
  %"*self1" = load %List*, %List** %self, align 8
  %"*self.items2" = getelementptr inbounds %List, %List* %"*self1", i32 0, i32 0
  %"$*self.items3" = load i32*, i32** %"*self.items2", align 8
  %"$i4" = load i32, i32* %i, align 4
  %"*self.items[]5" = getelementptr inbounds i32, i32* %"$*self.items3", i32 %"$i4"
  %"*self6" = load %List*, %List** %self, align 8
  %"*self.items7" = getelementptr inbounds %List, %List* %"*self6", i32 0, i32 0
  %"$*self.items8" = load i32*, i32** %"*self.items7", align 8
  %"$j" = load i32, i32* %j, align 4
  %"*self.items[]9" = getelementptr inbounds i32, i32* %"$*self.items8", i32 %"$j"
  %"$*self.items[]10" = load i32, i32* %"*self.items[]9", align 4
  store i32 %"$*self.items[]10", i32* %"*self.items[]5", align 4
  %"*self11" = load %List*, %List** %self, align 8
  %"*self.items12" = getelementptr inbounds %List, %List* %"*self11", i32 0, i32 0
  %"$*self.items13" = load i32*, i32** %"*self.items12", align 8
  %"$j14" = load i32, i32* %j, align 4
  %"*self.items[]15" = getelementptr inbounds i32, i32* %"$*self.items13", i32 %"$j14"
  %"$t" = load i32, i32* %t, align 4
  store i32 %"$t", i32* %"*self.items[]15", align 4
  ret void
}

define void @List.sort(%List* %0) {
entry:
  %self = alloca %List*, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  store %List* %0, %List** %self, align 8
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %endloop, %entry
  %"*self" = load %List*, %List** %self, align 8
  %"*self.size" = getelementptr inbounds %List, %List* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.size" = load i32, i32* %"*self.size", align 4
  %LT = icmp slt i32 %"$i", %"$*self.size"
  br i1 %LT, label %while0.then, label %endloop1

while0.then:                                      ; preds = %while0
  store i32 0, i32* %j, align 4
  br label %while1

while1:                                           ; preds = %endif, %while0.then
  %"*self2" = load %List*, %List** %self, align 8
  %"*self.size3" = getelementptr inbounds %List, %List* %"*self2", i32 0, i32 1
  %"$*self.size4" = load i32, i32* %"*self.size3", align 4
  %SUB = sub i32 %"$*self.size4", 1
  %"$i5" = load i32, i32* %i, align 4
  %SUB6 = sub i32 %SUB, %"$i5"
  %"$j" = load i32, i32* %j, align 4
  %LT7 = icmp slt i32 %"$j", %SUB6
  br i1 %LT7, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  br label %if2

if2:                                              ; preds = %while1.then
  %"*self8" = load %List*, %List** %self, align 8
  %"*self.items" = getelementptr inbounds %List, %List* %"*self8", i32 0, i32 0
  %"$*self.items" = load i32*, i32** %"*self.items", align 8
  %"$j9" = load i32, i32* %j, align 4
  %"*self.items[]" = getelementptr inbounds i32, i32* %"$*self.items", i32 %"$j9"
  %"*self10" = load %List*, %List** %self, align 8
  %"*self.items11" = getelementptr inbounds %List, %List* %"*self10", i32 0, i32 0
  %"$j12" = load i32, i32* %j, align 4
  %ADD = add i32 %"$j12", 1
  %"$*self.items13" = load i32*, i32** %"*self.items11", align 8
  %"*self.items[]14" = getelementptr inbounds i32, i32* %"$*self.items13", i32 %ADD
  %"$*self.items[]" = load i32, i32* %"*self.items[]", align 4
  %"$*self.items[]15" = load i32, i32* %"*self.items[]14", align 4
  %GT = icmp sgt i32 %"$*self.items[]", %"$*self.items[]15"
  br i1 %GT, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*self16" = load %List*, %List** %self, align 8
  %"$j17" = load i32, i32* %j, align 4
  %ADD18 = add i32 %"$j17", 1
  %"$j19" = load i32, i32* %j, align 4
  call void @List.swap(%List* %"*self16", i32 %"$j19", i32 %ADD18)
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  %"$j20" = load i32, i32* %j, align 4
  %ADD21 = add i32 %"$j20", 1
  store i32 %ADD21, i32* %j, align 4
  br label %while1

endloop:                                          ; preds = %while1
  %"$i22" = load i32, i32* %i, align 4
  %ADD23 = add i32 %"$i22", 1
  store i32 %ADD23, i32* %i, align 4
  br label %while0

endloop1:                                         ; preds = %while0
  ret void
}

define void @List.show(%List* %0) {
entry:
  %self = alloca %List*, align 8
  %i = alloca i32, align 4
  store %List* %0, %List** %self, align 8
  store i32 0, i32* %i, align 4
  br label %while3

while3:                                           ; preds = %endif, %entry
  %"*self" = load %List*, %List** %self, align 8
  %"*self.size" = getelementptr inbounds %List, %List* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.size" = load i32, i32* %"*self.size", align 4
  %LT = icmp slt i32 %"$i", %"$*self.size"
  br i1 %LT, label %while3.then, label %endloop

while3.then:                                      ; preds = %while3
  br label %if4

if4:                                              ; preds = %while3.then
  %"$i1" = load i32, i32* %i, align 4
  %GT = icmp sgt i32 %"$i1", 0
  br i1 %GT, label %if4.then, label %endif

if4.then:                                         ; preds = %if4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  br label %endif

endif:                                            ; preds = %if4.then, %if4
  %"*self2" = load %List*, %List** %self, align 8
  %"*self.items" = getelementptr inbounds %List, %List* %"*self2", i32 0, i32 0
  %"$*self.items" = load i32*, i32** %"*self.items", align 8
  %"$i3" = load i32, i32* %i, align 4
  %"*self.items[]" = getelementptr inbounds i32, i32* %"$*self.items", i32 %"$i3"
  %"$*self.items[]" = load i32, i32* %"*self.items[]", align 4
  %call.printf4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0), i32 %"$*self.items[]")
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while3

endloop:                                          ; preds = %while3
  %call.printf6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  ret void
}

define i32 @main() {
entry:
  %l = alloca %List, align 8
  %arr = alloca [5 x i32], align 4
  store %List zeroinitializer, %List* %l, align 8
  %l.items = getelementptr inbounds %List, %List* %l, i32 0, i32 0
  store [5 x i32] [i32 5, i32 3, i32 9, i32 1, i32 4], [5 x i32]* %arr, align 4
  %0 = getelementptr inbounds [5 x i32], [5 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %l.items, align 8
  %l.size = getelementptr inbounds %List, %List* %l, i32 0, i32 1
  store i32 5, i32* %l.size, align 4
  call void @List.sort(%List* %l)
  call void @List.show(%List* %l)
  %l.items1 = getelementptr inbounds %List, %List* %l, i32 0, i32 0
  %"$l.items" = load i32*, i32** %l.items1, align 8
  %"l.items[]" = getelementptr inbounds i32, i32* %"$l.items", i32 0
  %"$l.items[]" = load i32, i32* %"l.items[]", align 4
  %MUL = mul i32 %"$l.items[]", 10
  %l.items2 = getelementptr inbounds %List, %List* %l, i32 0, i32 0
  %"$l.items3" = load i32*, i32** %l.items2, align 8
  %"l.items[]4" = getelementptr inbounds i32, i32* %"$l.items3", i32 4
  %"$l.items[]5" = load i32, i32* %"l.items[]4", align 4
  %ADD = add i32 %MUL, %"$l.items[]5"
  ret i32 %ADD
}
```

---

## 031 — crazy: bank account: methods, a ref parameter, an array of bills

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
target triple = "x86_64-unknown-linux-gnu"

%Account = type { i32, i32 }

@str = private unnamed_addr constant [31 x i8] c"balance %d, ops %d, failed %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @Account.deposit(%Account* %0, i32 %1) {
entry:
  %self = alloca %Account*, align 8
  %n = alloca i32, align 4
  store %Account* %0, %Account** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %Account*, %Account** %self, align 8
  %"*self.balance" = getelementptr inbounds %Account, %Account* %"*self", i32 0, i32 0
  %"$n" = load i32, i32* %n, align 4
  %"$*self.balance" = load i32, i32* %"*self.balance", align 4
  %ADD = add i32 %"$*self.balance", %"$n"
  store i32 %ADD, i32* %"*self.balance", align 4
  %"*self1" = load %Account*, %Account** %self, align 8
  %"*self.ops" = getelementptr inbounds %Account, %Account* %"*self1", i32 0, i32 1
  %"$*self.ops" = load i32, i32* %"*self.ops", align 4
  %ADD2 = add i32 %"$*self.ops", 1
  store i32 %ADD2, i32* %"*self.ops", align 4
  ret void
}

define i1 @Account.withdraw(%Account* %0, i32 %1) {
entry:
  %self = alloca %Account*, align 8
  %n = alloca i32, align 4
  store %Account* %0, %Account** %self, align 8
  store i32 %1, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %Account*, %Account** %self, align 8
  %"*self.balance" = getelementptr inbounds %Account, %Account* %"*self", i32 0, i32 0
  %"$n" = load i32, i32* %n, align 4
  %"$*self.balance" = load i32, i32* %"*self.balance", align 4
  %GT = icmp sgt i32 %"$n", %"$*self.balance"
  br i1 %GT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i1 false

endif:                                            ; preds = %if0
  %"*self1" = load %Account*, %Account** %self, align 8
  %"*self.balance2" = getelementptr inbounds %Account, %Account* %"*self1", i32 0, i32 0
  %"$n3" = load i32, i32* %n, align 4
  %"$*self.balance4" = load i32, i32* %"*self.balance2", align 4
  %SUB = sub i32 %"$*self.balance4", %"$n3"
  store i32 %SUB, i32* %"*self.balance2", align 4
  %"*self5" = load %Account*, %Account** %self, align 8
  %"*self.ops" = getelementptr inbounds %Account, %Account* %"*self5", i32 0, i32 1
  %"$*self.ops" = load i32, i32* %"*self.ops", align 4
  %ADD = add i32 %"$*self.ops", 1
  store i32 %ADD, i32* %"*self.ops", align 4
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
  br label %while1

while1:                                           ; preds = %endif, %entry
  %"$i" = load i32, i32* %i, align 4
  %"$count" = load i32, i32* %count, align 4
  %LT = icmp slt i32 %"$i", %"$count"
  br i1 %LT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  br label %if2

if2:                                              ; preds = %while1.then
  %"*acc" = load %Account*, %Account** %acc, align 8
  %"$bills" = load i32*, i32** %bills, align 8
  %"$i1" = load i32, i32* %i, align 4
  %"bills[]" = getelementptr inbounds i32, i32* %"$bills", i32 %"$i1"
  %"$bills[]" = load i32, i32* %"bills[]", align 4
  %call.Account.withdraw = call i1 @Account.withdraw(%Account* %"*acc", i32 %"$bills[]")
  %EQ = icmp eq i1 %call.Account.withdraw, false
  br i1 %EQ, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"$failed" = load i32, i32* %failed, align 4
  %ADD = add i32 %"$failed", 1
  store i32 %ADD, i32* %failed, align 4
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  %"$i2" = load i32, i32* %i, align 4
  %ADD3 = add i32 %"$i2", 1
  store i32 %ADD3, i32* %i, align 4
  br label %while1

endloop:                                          ; preds = %while1
  %"$failed4" = load i32, i32* %failed, align 4
  ret i32 %"$failed4"
}

define i32 @main() {
entry:
  %a = alloca %Account, align 8
  %bills = alloca i32*, align 8
  %arr = alloca [4 x i32], align 4
  %failed = alloca i32, align 4
  store %Account zeroinitializer, %Account* %a, align 4
  call void @Account.deposit(%Account* %a, i32 100)
  store [4 x i32] [i32 30, i32 50, i32 40, i32 10], [4 x i32]* %arr, align 4
  %0 = getelementptr inbounds [4 x i32], [4 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %bills, align 8
  %"$bills" = load i32*, i32** %bills, align 8
  %call.pay_all = call i32 @pay_all(%Account* %a, i32* %"$bills", i32 4)
  store i32 %call.pay_all, i32* %failed, align 4
  %a.balance = getelementptr inbounds %Account, %Account* %a, i32 0, i32 0
  %a.ops = getelementptr inbounds %Account, %Account* %a, i32 0, i32 1
  %"$a.balance" = load i32, i32* %a.balance, align 4
  %"$a.ops" = load i32, i32* %a.ops, align 4
  %"$failed" = load i32, i32* %failed, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([31 x i8], [31 x i8]* @str, i32 0, i32 0), i32 %"$a.balance", i32 %"$a.ops", i32 %"$failed")
  %a.balance1 = getelementptr inbounds %Account, %Account* %a, i32 0, i32 0
  %"$a.balance2" = load i32, i32* %a.balance1, align 4
  ret i32 %"$a.balance2"
}
```

---

## 032 — crazy: an array of structs is a copy

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
target triple = "x86_64-unknown-linux-gnu"

%Point = type { i32, i32 }

define i32 @main() {
entry:
  %p = alloca %Point, align 8
  %q = alloca %Point, align 8
  %ps = alloca %Point*, align 8
  %arr = alloca [2 x %Point], align 8
  store %Point zeroinitializer, %Point* %p, align 4
  store %Point zeroinitializer, %Point* %q, align 4
  %p.x = getelementptr inbounds %Point, %Point* %p, i32 0, i32 0
  store i32 1, i32* %p.x, align 4
  %q.x = getelementptr inbounds %Point, %Point* %q, i32 0, i32 0
  store i32 2, i32* %q.x, align 4
  %"$p" = load %Point, %Point* %p, align 4
  %"$q" = load %Point, %Point* %q, align 4
  %0 = insertvalue [2 x %Point] undef, %Point %"$p", 0
  %1 = insertvalue [2 x %Point] %0, %Point %"$q", 1
  store [2 x %Point] %1, [2 x %Point]* %arr, align 4
  %2 = getelementptr inbounds [2 x %Point], [2 x %Point]* %arr, i32 0, i32 0
  store %Point* %2, %Point** %ps, align 8
  %"$ps" = load %Point*, %Point** %ps, align 8
  %"ps[]" = getelementptr inbounds %Point, %Point* %"$ps", i32 1
  %"ps[].x" = getelementptr inbounds %Point, %Point* %"ps[]", i32 0, i32 0
  store i32 7, i32* %"ps[].x", align 4
  %"$ps1" = load %Point*, %Point** %ps, align 8
  %"ps[]2" = getelementptr inbounds %Point, %Point* %"$ps1", i32 0
  %"ps[].x3" = getelementptr inbounds %Point, %Point* %"ps[]2", i32 0, i32 0
  %"$ps4" = load %Point*, %Point** %ps, align 8
  %"ps[]5" = getelementptr inbounds %Point, %Point* %"$ps4", i32 1
  %"ps[].x6" = getelementptr inbounds %Point, %Point* %"ps[]5", i32 0, i32 0
  %"$ps[].x" = load i32, i32* %"ps[].x3", align 4
  %"$ps[].x7" = load i32, i32* %"ps[].x6", align 4
  %ADD = add i32 %"$ps[].x", %"$ps[].x7"
  %q.x8 = getelementptr inbounds %Point, %Point* %q, i32 0, i32 0
  %"$q.x" = load i32, i32* %q.x8, align 4
  %ADD9 = add i32 %ADD, %"$q.x"
  ret i32 %ADD9
}
```

---

## 033 — crazy: vectors: struct parameters, struct returns, chained calls

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
target triple = "x86_64-unknown-linux-gnu"

%Vec = type { i32, i32 }

define %Vec @Vec.add(%Vec* %0, %Vec %1) {
entry:
  %self = alloca %Vec*, align 8
  %o = alloca %Vec, align 8
  %r = alloca %Vec, align 8
  store %Vec* %0, %Vec** %self, align 8
  store %Vec %1, %Vec* %o, align 4
  store %Vec zeroinitializer, %Vec* %r, align 4
  %r.x = getelementptr inbounds %Vec, %Vec* %r, i32 0, i32 0
  %"*self" = load %Vec*, %Vec** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec, %Vec* %"*self", i32 0, i32 0
  %o.x = getelementptr inbounds %Vec, %Vec* %o, i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$o.x" = load i32, i32* %o.x, align 4
  %ADD = add i32 %"$*self.x", %"$o.x"
  store i32 %ADD, i32* %r.x, align 4
  %r.y = getelementptr inbounds %Vec, %Vec* %r, i32 0, i32 1
  %"*self1" = load %Vec*, %Vec** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec, %Vec* %"*self1", i32 0, i32 1
  %o.y = getelementptr inbounds %Vec, %Vec* %o, i32 0, i32 1
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  %"$o.y" = load i32, i32* %o.y, align 4
  %ADD2 = add i32 %"$*self.y", %"$o.y"
  store i32 %ADD2, i32* %r.y, align 4
  %"$r" = load %Vec, %Vec* %r, align 4
  ret %Vec %"$r"
}

define i32 @Vec.dot(%Vec* %0, %Vec %1) {
entry:
  %self = alloca %Vec*, align 8
  %o = alloca %Vec, align 8
  store %Vec* %0, %Vec** %self, align 8
  store %Vec %1, %Vec* %o, align 4
  %"*self" = load %Vec*, %Vec** %self, align 8
  %"*self.x" = getelementptr inbounds %Vec, %Vec* %"*self", i32 0, i32 0
  %o.x = getelementptr inbounds %Vec, %Vec* %o, i32 0, i32 0
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %"$o.x" = load i32, i32* %o.x, align 4
  %MUL = mul i32 %"$*self.x", %"$o.x"
  %"*self1" = load %Vec*, %Vec** %self, align 8
  %"*self.y" = getelementptr inbounds %Vec, %Vec* %"*self1", i32 0, i32 1
  %o.y = getelementptr inbounds %Vec, %Vec* %o, i32 0, i32 1
  %"$*self.y" = load i32, i32* %"*self.y", align 4
  %"$o.y" = load i32, i32* %o.y, align 4
  %MUL2 = mul i32 %"$*self.y", %"$o.y"
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
  %v.x = getelementptr inbounds %Vec, %Vec* %v, i32 0, i32 0
  %"$x" = load i32, i32* %x, align 4
  store i32 %"$x", i32* %v.x, align 4
  %v.y = getelementptr inbounds %Vec, %Vec* %v, i32 0, i32 1
  %"$y" = load i32, i32* %y, align 4
  store i32 %"$y", i32* %v.y, align 4
  %"$v" = load %Vec, %Vec* %v, align 4
  ret %Vec %"$v"
}

define i32 @main() {
entry:
  %a = alloca %Vec, align 8
  %b = alloca %Vec, align 8
  %c = alloca %Vec, align 8
  %tmp = alloca %Vec, align 8
  %call.make = call %Vec @make(i32 1, i32 2)
  store %Vec %call.make, %Vec* %a, align 4
  %call.make1 = call %Vec @make(i32 3, i32 4)
  store %Vec %call.make1, %Vec* %b, align 4
  %"$b" = load %Vec, %Vec* %b, align 4
  %call.Vec.add = call %Vec @Vec.add(%Vec* %a, %Vec %"$b")
  store %Vec %call.Vec.add, %Vec* %tmp, align 4
  %"$b2" = load %Vec, %Vec* %b, align 4
  %call.Vec.add3 = call %Vec @Vec.add(%Vec* %tmp, %Vec %"$b2")
  store %Vec %call.Vec.add3, %Vec* %c, align 4
  %"$a" = load %Vec, %Vec* %a, align 4
  %call.Vec.dot = call i32 @Vec.dot(%Vec* %c, %Vec %"$a")
  ret i32 %call.Vec.dot
}
```

---

## 034 — crazy: linked list: ref attributes, isNullable, rebinding a ref in a loop

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

---

## 035 — crazy: a recursive method that counts its calls

```ura
// expect: 125   (5! = 120, plus 5 calls)
struct Math:
    calls i32

    fn fact(n i32) i32:
        self.calls += 1
        if n <= 1: return 1
        return n * self.fact(n - 1)

fn main() i32:
    m Math
    r i32 = m.fact(5)
    return r + m.calls
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Math = type { i32 }

define i32 @Math.fact(%Math* %0, i32 %1) {
entry:
  %self = alloca %Math*, align 8
  %n = alloca i32, align 4
  store %Math* %0, %Math** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %Math*, %Math** %self, align 8
  %"*self.calls" = getelementptr inbounds %Math, %Math* %"*self", i32 0, i32 0
  %"$*self.calls" = load i32, i32* %"*self.calls", align 4
  %ADD = add i32 %"$*self.calls", 1
  store i32 %ADD, i32* %"*self.calls", align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %LE = icmp sle i32 %"$n", 1
  br i1 %LE, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 1

endif:                                            ; preds = %if0
  %"*self1" = load %Math*, %Math** %self, align 8
  %"$n2" = load i32, i32* %n, align 4
  %SUB = sub i32 %"$n2", 1
  %call.Math.fact = call i32 @Math.fact(%Math* %"*self1", i32 %SUB)
  %"$n3" = load i32, i32* %n, align 4
  %MUL = mul i32 %"$n3", %call.Math.fact
  ret i32 %MUL
}

define i32 @main() {
entry:
  %m = alloca %Math, align 8
  %r = alloca i32, align 4
  store %Math zeroinitializer, %Math* %m, align 4
  %call.Math.fact = call i32 @Math.fact(%Math* %m, i32 5)
  store i32 %call.Math.fact, i32* %r, align 4
  %m.calls = getelementptr inbounds %Math, %Math* %m, i32 0, i32 0
  %"$r" = load i32, i32* %r, align 4
  %"$m.calls" = load i32, i32* %m.calls, align 4
  %ADD = add i32 %"$r", %"$m.calls"
  ret i32 %ADD
}
```

---

## 036 — crazy: a method takes a ref to its own struct type

```ura
// expect: 100   (a = 10, b = 0)
struct Counter:
    n i32

    fn absorb(other &Counter):
        self.n += other.n
        other.n = 0

fn main() i32:
    a Counter
    b Counter
    a.n = 4
    b.n = 6
    a.absorb(&b)
    a.absorb(&b)
    return a.n * 10 + b.n
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Counter = type { i32 }

define void @Counter.absorb(%Counter* %0, %Counter* %1) {
entry:
  %self = alloca %Counter*, align 8
  %other = alloca %Counter*, align 8
  store %Counter* %0, %Counter** %self, align 8
  store %Counter* %1, %Counter** %other, align 8
  %"*self" = load %Counter*, %Counter** %self, align 8
  %"*self.n" = getelementptr inbounds %Counter, %Counter* %"*self", i32 0, i32 0
  %"*other" = load %Counter*, %Counter** %other, align 8
  %"*other.n" = getelementptr inbounds %Counter, %Counter* %"*other", i32 0, i32 0
  %"$*other.n" = load i32, i32* %"*other.n", align 4
  %"$*self.n" = load i32, i32* %"*self.n", align 4
  %ADD = add i32 %"$*self.n", %"$*other.n"
  store i32 %ADD, i32* %"*self.n", align 4
  %"*other1" = load %Counter*, %Counter** %other, align 8
  %"*other.n2" = getelementptr inbounds %Counter, %Counter* %"*other1", i32 0, i32 0
  store i32 0, i32* %"*other.n2", align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Counter, align 8
  %b = alloca %Counter, align 8
  store %Counter zeroinitializer, %Counter* %a, align 4
  store %Counter zeroinitializer, %Counter* %b, align 4
  %a.n = getelementptr inbounds %Counter, %Counter* %a, i32 0, i32 0
  store i32 4, i32* %a.n, align 4
  %b.n = getelementptr inbounds %Counter, %Counter* %b, i32 0, i32 0
  store i32 6, i32* %b.n, align 4
  call void @Counter.absorb(%Counter* %a, %Counter* %b)
  call void @Counter.absorb(%Counter* %a, %Counter* %b)
  %a.n1 = getelementptr inbounds %Counter, %Counter* %a, i32 0, i32 0
  %"$a.n" = load i32, i32* %a.n1, align 4
  %MUL = mul i32 %"$a.n", 10
  %b.n2 = getelementptr inbounds %Counter, %Counter* %b, i32 0, i32 0
  %"$b.n" = load i32, i32* %b.n2, align 4
  %ADD = add i32 %MUL, %"$b.n"
  ret i32 %ADD
}
```

---

## 037 — crazy: stack: push squares, pop until empty

```ura
// expect: 173   (pops 25 16 9 4 1, last digits folded: ((((5*2+6)*2+9)*2+4)*2+1)
struct Stack:
    items i32[]
    top i32

    fn push(v i32):
        self.items[self.top] = v
        self.top += 1

    fn pop() i32:
        self.top -= 1
        return self.items[self.top]

    fn empty() bool:
        return self.top == 0

fn main() i32:
    s Stack
    s.items = [0, 0, 0, 0, 0, 0, 0, 0]
    i i32 = 1
    while i <= 5:
        s.push(i * i)
        i += 1
    total i32 = 0
    while not s.empty():
        total = total * 2 + s.pop() % 10
    return total
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Stack = type { i32*, i32 }

define void @Stack.push(%Stack* %0, i32 %1) {
entry:
  %self = alloca %Stack*, align 8
  %v = alloca i32, align 4
  store %Stack* %0, %Stack** %self, align 8
  store i32 %1, i32* %v, align 4
  %"*self" = load %Stack*, %Stack** %self, align 8
  %"*self.items" = getelementptr inbounds %Stack, %Stack* %"*self", i32 0, i32 0
  %"*self1" = load %Stack*, %Stack** %self, align 8
  %"*self.top" = getelementptr inbounds %Stack, %Stack* %"*self1", i32 0, i32 1
  %"$*self.items" = load i32*, i32** %"*self.items", align 8
  %"$*self.top" = load i32, i32* %"*self.top", align 4
  %"*self.items[]" = getelementptr inbounds i32, i32* %"$*self.items", i32 %"$*self.top"
  %"$v" = load i32, i32* %v, align 4
  store i32 %"$v", i32* %"*self.items[]", align 4
  %"*self2" = load %Stack*, %Stack** %self, align 8
  %"*self.top3" = getelementptr inbounds %Stack, %Stack* %"*self2", i32 0, i32 1
  %"$*self.top4" = load i32, i32* %"*self.top3", align 4
  %ADD = add i32 %"$*self.top4", 1
  store i32 %ADD, i32* %"*self.top3", align 4
  ret void
}

define i32 @Stack.pop(%Stack* %0) {
entry:
  %self = alloca %Stack*, align 8
  store %Stack* %0, %Stack** %self, align 8
  %"*self" = load %Stack*, %Stack** %self, align 8
  %"*self.top" = getelementptr inbounds %Stack, %Stack* %"*self", i32 0, i32 1
  %"$*self.top" = load i32, i32* %"*self.top", align 4
  %SUB = sub i32 %"$*self.top", 1
  store i32 %SUB, i32* %"*self.top", align 4
  %"*self1" = load %Stack*, %Stack** %self, align 8
  %"*self.items" = getelementptr inbounds %Stack, %Stack* %"*self1", i32 0, i32 0
  %"*self2" = load %Stack*, %Stack** %self, align 8
  %"*self.top3" = getelementptr inbounds %Stack, %Stack* %"*self2", i32 0, i32 1
  %"$*self.items" = load i32*, i32** %"*self.items", align 8
  %"$*self.top4" = load i32, i32* %"*self.top3", align 4
  %"*self.items[]" = getelementptr inbounds i32, i32* %"$*self.items", i32 %"$*self.top4"
  %"$*self.items[]" = load i32, i32* %"*self.items[]", align 4
  ret i32 %"$*self.items[]"
}

define i1 @Stack.empty(%Stack* %0) {
entry:
  %self = alloca %Stack*, align 8
  store %Stack* %0, %Stack** %self, align 8
  %"*self" = load %Stack*, %Stack** %self, align 8
  %"*self.top" = getelementptr inbounds %Stack, %Stack* %"*self", i32 0, i32 1
  %"$*self.top" = load i32, i32* %"*self.top", align 4
  %EQ = icmp eq i32 %"$*self.top", 0
  ret i1 %EQ
}

define i32 @main() {
entry:
  %s = alloca %Stack, align 8
  %arr = alloca [8 x i32], align 4
  %i = alloca i32, align 4
  %total = alloca i32, align 4
  store %Stack zeroinitializer, %Stack* %s, align 8
  %s.items = getelementptr inbounds %Stack, %Stack* %s, i32 0, i32 0
  store [8 x i32] zeroinitializer, [8 x i32]* %arr, align 4
  %0 = getelementptr inbounds [8 x i32], [8 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %s.items, align 8
  store i32 1, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LE = icmp sle i32 %"$i", 5
  br i1 %LE, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$i2" = load i32, i32* %i, align 4
  %"$i3" = load i32, i32* %i, align 4
  %MUL = mul i32 %"$i2", %"$i3"
  call void @Stack.push(%Stack* %s, i32 %MUL)
  %"$i4" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i4", 1
  store i32 %ADD, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  store i32 0, i32* %total, align 4
  br label %while1

while1:                                           ; preds = %while1.then, %endloop
  %call.Stack.empty = call i1 @Stack.empty(%Stack* %s)
  %NOT = xor i1 %call.Stack.empty, true
  br i1 %NOT, label %while1.then, label %endloop1

while1.then:                                      ; preds = %while1
  %"$total" = load i32, i32* %total, align 4
  %MUL5 = mul i32 %"$total", 2
  %call.Stack.pop = call i32 @Stack.pop(%Stack* %s)
  %MOD = srem i32 %call.Stack.pop, 10
  %ADD6 = add i32 %MUL5, %MOD
  store i32 %ADD6, i32* %total, align 4
  br label %while1

endloop1:                                         ; preds = %while1
  %"$total7" = load i32, i32* %total, align 4
  ret i32 %"$total7"
}
```

---

## 038 — crazy: ring-buffer queue: full, wrap around, drain

```ura
// expect: 242   (first 1 * 100, sum 2 + 3 + 9 = 14 * 10, 2 dropped)
struct Queue:
    items i32[]
    head i32
    tail i32
    size i32
    cap i32

    fn put(v i32) bool:
        if self.size == self.cap: return False
        self.items[self.tail] = v
        self.tail = (self.tail + 1) % self.cap
        self.size += 1
        return True

    fn take() i32:
        v i32 = self.items[self.head]
        self.head = (self.head + 1) % self.cap
        self.size -= 1
        return v

fn main() i32:
    q Queue
    q.items = [0, 0, 0]
    q.cap = 3
    dropped i32 = 0
    i i32 = 1
    while i <= 5:
        if !q.put(i): dropped += 1
        i += 1
    first i32 = q.take()
    q.put(9)
    sum i32 = 0
    while q.size > 0:
        sum += q.take()
    return first * 100 + sum * 10 + dropped
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Queue = type { i32*, i32, i32, i32, i32 }

define i1 @Queue.put(%Queue* %0, i32 %1) {
entry:
  %self = alloca %Queue*, align 8
  %v = alloca i32, align 4
  store %Queue* %0, %Queue** %self, align 8
  store i32 %1, i32* %v, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %Queue*, %Queue** %self, align 8
  %"*self.size" = getelementptr inbounds %Queue, %Queue* %"*self", i32 0, i32 3
  %"*self1" = load %Queue*, %Queue** %self, align 8
  %"*self.cap" = getelementptr inbounds %Queue, %Queue* %"*self1", i32 0, i32 4
  %"$*self.size" = load i32, i32* %"*self.size", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.size", %"$*self.cap"
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i1 false

endif:                                            ; preds = %if0
  %"*self2" = load %Queue*, %Queue** %self, align 8
  %"*self.items" = getelementptr inbounds %Queue, %Queue* %"*self2", i32 0, i32 0
  %"*self3" = load %Queue*, %Queue** %self, align 8
  %"*self.tail" = getelementptr inbounds %Queue, %Queue* %"*self3", i32 0, i32 2
  %"$*self.items" = load i32*, i32** %"*self.items", align 8
  %"$*self.tail" = load i32, i32* %"*self.tail", align 4
  %"*self.items[]" = getelementptr inbounds i32, i32* %"$*self.items", i32 %"$*self.tail"
  %"$v" = load i32, i32* %v, align 4
  store i32 %"$v", i32* %"*self.items[]", align 4
  %"*self4" = load %Queue*, %Queue** %self, align 8
  %"*self.tail5" = getelementptr inbounds %Queue, %Queue* %"*self4", i32 0, i32 2
  %"*self6" = load %Queue*, %Queue** %self, align 8
  %"*self.tail7" = getelementptr inbounds %Queue, %Queue* %"*self6", i32 0, i32 2
  %"$*self.tail8" = load i32, i32* %"*self.tail7", align 4
  %ADD = add i32 %"$*self.tail8", 1
  %"*self9" = load %Queue*, %Queue** %self, align 8
  %"*self.cap10" = getelementptr inbounds %Queue, %Queue* %"*self9", i32 0, i32 4
  %"$*self.cap11" = load i32, i32* %"*self.cap10", align 4
  %MOD = srem i32 %ADD, %"$*self.cap11"
  store i32 %MOD, i32* %"*self.tail5", align 4
  %"*self12" = load %Queue*, %Queue** %self, align 8
  %"*self.size13" = getelementptr inbounds %Queue, %Queue* %"*self12", i32 0, i32 3
  %"$*self.size14" = load i32, i32* %"*self.size13", align 4
  %ADD15 = add i32 %"$*self.size14", 1
  store i32 %ADD15, i32* %"*self.size13", align 4
  ret i1 true
}

define i32 @Queue.take(%Queue* %0) {
entry:
  %self = alloca %Queue*, align 8
  %v = alloca i32, align 4
  store %Queue* %0, %Queue** %self, align 8
  %"*self" = load %Queue*, %Queue** %self, align 8
  %"*self.items" = getelementptr inbounds %Queue, %Queue* %"*self", i32 0, i32 0
  %"*self1" = load %Queue*, %Queue** %self, align 8
  %"*self.head" = getelementptr inbounds %Queue, %Queue* %"*self1", i32 0, i32 1
  %"$*self.items" = load i32*, i32** %"*self.items", align 8
  %"$*self.head" = load i32, i32* %"*self.head", align 4
  %"*self.items[]" = getelementptr inbounds i32, i32* %"$*self.items", i32 %"$*self.head"
  %"$*self.items[]" = load i32, i32* %"*self.items[]", align 4
  store i32 %"$*self.items[]", i32* %v, align 4
  %"*self2" = load %Queue*, %Queue** %self, align 8
  %"*self.head3" = getelementptr inbounds %Queue, %Queue* %"*self2", i32 0, i32 1
  %"*self4" = load %Queue*, %Queue** %self, align 8
  %"*self.head5" = getelementptr inbounds %Queue, %Queue* %"*self4", i32 0, i32 1
  %"$*self.head6" = load i32, i32* %"*self.head5", align 4
  %ADD = add i32 %"$*self.head6", 1
  %"*self7" = load %Queue*, %Queue** %self, align 8
  %"*self.cap" = getelementptr inbounds %Queue, %Queue* %"*self7", i32 0, i32 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %MOD = srem i32 %ADD, %"$*self.cap"
  store i32 %MOD, i32* %"*self.head3", align 4
  %"*self8" = load %Queue*, %Queue** %self, align 8
  %"*self.size" = getelementptr inbounds %Queue, %Queue* %"*self8", i32 0, i32 3
  %"$*self.size" = load i32, i32* %"*self.size", align 4
  %SUB = sub i32 %"$*self.size", 1
  store i32 %SUB, i32* %"*self.size", align 4
  %"$v" = load i32, i32* %v, align 4
  ret i32 %"$v"
}

define i32 @main() {
entry:
  %q = alloca %Queue, align 8
  %arr = alloca [3 x i32], align 4
  %dropped = alloca i32, align 4
  %i = alloca i32, align 4
  %first = alloca i32, align 4
  %sum = alloca i32, align 4
  store %Queue zeroinitializer, %Queue* %q, align 8
  %q.items = getelementptr inbounds %Queue, %Queue* %q, i32 0, i32 0
  store [3 x i32] zeroinitializer, [3 x i32]* %arr, align 4
  %0 = getelementptr inbounds [3 x i32], [3 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %q.items, align 8
  %q.cap = getelementptr inbounds %Queue, %Queue* %q, i32 0, i32 4
  store i32 3, i32* %q.cap, align 4
  store i32 0, i32* %dropped, align 4
  store i32 1, i32* %i, align 4
  br label %while1

while1:                                           ; preds = %endif, %entry
  %"$i" = load i32, i32* %i, align 4
  %LE = icmp sle i32 %"$i", 5
  br i1 %LE, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  br label %if2

if2:                                              ; preds = %while1.then
  %"$i2" = load i32, i32* %i, align 4
  %call.Queue.put = call i1 @Queue.put(%Queue* %q, i32 %"$i2")
  %NOT = xor i1 %call.Queue.put, true
  br i1 %NOT, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"$dropped" = load i32, i32* %dropped, align 4
  %ADD = add i32 %"$dropped", 1
  store i32 %ADD, i32* %dropped, align 4
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  %"$i3" = load i32, i32* %i, align 4
  %ADD4 = add i32 %"$i3", 1
  store i32 %ADD4, i32* %i, align 4
  br label %while1

endloop:                                          ; preds = %while1
  %call.Queue.take = call i32 @Queue.take(%Queue* %q)
  store i32 %call.Queue.take, i32* %first, align 4
  %call.Queue.put5 = call i1 @Queue.put(%Queue* %q, i32 9)
  store i32 0, i32* %sum, align 4
  br label %while3

while3:                                           ; preds = %while3.then, %endloop
  %q.size = getelementptr inbounds %Queue, %Queue* %q, i32 0, i32 3
  %"$q.size" = load i32, i32* %q.size, align 4
  %GT = icmp sgt i32 %"$q.size", 0
  br i1 %GT, label %while3.then, label %endloop1

while3.then:                                      ; preds = %while3
  %call.Queue.take6 = call i32 @Queue.take(%Queue* %q)
  %"$sum" = load i32, i32* %sum, align 4
  %ADD7 = add i32 %"$sum", %call.Queue.take6
  store i32 %ADD7, i32* %sum, align 4
  br label %while3

endloop1:                                         ; preds = %while3
  %"$first" = load i32, i32* %first, align 4
  %MUL = mul i32 %"$first", 100
  %"$sum8" = load i32, i32* %sum, align 4
  %MUL9 = mul i32 %"$sum8", 10
  %ADD10 = add i32 %MUL, %MUL9
  %"$dropped11" = load i32, i32* %dropped, align 4
  %ADD12 = add i32 %ADD10, %"$dropped11"
  ret i32 %ADD12
}
```

---

## 039 — crazy: binary tree: recursion through ref attributes

```ura
// expect: 103   (sum 10 * 10, depth 3)
struct Tree:
    value i32
    left &Tree
    right &Tree

fn sum(t &Tree) i32:
    if isNullable(t): return 0
    return t.value + sum(&t.left) + sum(&t.right)

fn depth(t &Tree) i32:
    if isNullable(t): return 0
    l i32 = depth(&t.left)
    r i32 = depth(&t.right)
    if l > r: return l + 1
    return r + 1

fn main() i32:
    a Tree
    b Tree
    c Tree
    d Tree
    a.value = 1
    b.value = 2
    c.value = 3
    d.value = 4
    a.left = &b
    a.right = &c
    b.left = &d
    return sum(&a) * 10 + depth(&a)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Tree = type { i32, %Tree*, %Tree* }

define i32 @sum(%Tree* %0) {
entry:
  %t = alloca %Tree*, align 8
  store %Tree* %0, %Tree** %t, align 8
  %dref.t = load %Tree*, %Tree** %t, align 8
  %"isNullable(dref.t)" = icmp eq %Tree* %dref.t, null
  br i1 %"isNullable(dref.t)", label %then, label %endif

then:                                             ; preds = %entry
  ret i32 0

endif:                                            ; preds = %entry
  %dref.t1 = load %Tree*, %Tree** %t, align 8
  %load.dref.t.value = getelementptr inbounds %Tree, %Tree* %dref.t1, i32 0, i32 0
  %load.load.dref.t.value = load i32, i32* %load.dref.t.value, align 4
  %dref.t2 = load %Tree*, %Tree** %t, align 8
  %load.dref.t.left = getelementptr inbounds %Tree, %Tree* %dref.t2, i32 0, i32 1
  %dref.load.dref.t.left = load %Tree*, %Tree** %load.dref.t.left, align 8
  %"sum(&dref.load.dref.t.left)" = call i32 @sum(%Tree* %dref.load.dref.t.left)
  %ADD = add i32 %load.load.dref.t.value, %"sum(&dref.load.dref.t.left)"
  %dref.t3 = load %Tree*, %Tree** %t, align 8
  %load.dref.t.right = getelementptr inbounds %Tree, %Tree* %dref.t3, i32 0, i32 2
  %dref.load.dref.t.right = load %Tree*, %Tree** %load.dref.t.right, align 8
  %"sum(&dref.load.dref.t.right)" = call i32 @sum(%Tree* %dref.load.dref.t.right)
  %ADD4 = add i32 %ADD, %"sum(&dref.load.dref.t.right)"
  ret i32 %ADD4
}

define i32 @depth(%Tree* %0) {
entry:
  %t = alloca %Tree*, align 8
  %l = alloca i32, align 4
  %r = alloca i32, align 4
  store %Tree* %0, %Tree** %t, align 8
  %dref.t = load %Tree*, %Tree** %t, align 8
  %"isNullable(dref.t)" = icmp eq %Tree* %dref.t, null
  br i1 %"isNullable(dref.t)", label %then, label %endif

then:                                             ; preds = %entry
  ret i32 0

endif:                                            ; preds = %entry
  %dref.t1 = load %Tree*, %Tree** %t, align 8
  %load.dref.t.left = getelementptr inbounds %Tree, %Tree* %dref.t1, i32 0, i32 1
  %dref.load.dref.t.left = load %Tree*, %Tree** %load.dref.t.left, align 8
  %"depth(&dref.load.dref.t.left)" = call i32 @depth(%Tree* %dref.load.dref.t.left)
  store i32 %"depth(&dref.load.dref.t.left)", i32* %l, align 4
  %dref.t2 = load %Tree*, %Tree** %t, align 8
  %load.dref.t.right = getelementptr inbounds %Tree, %Tree* %dref.t2, i32 0, i32 2
  %dref.load.dref.t.right = load %Tree*, %Tree** %load.dref.t.right, align 8
  %"depth(&dref.load.dref.t.right)" = call i32 @depth(%Tree* %dref.load.dref.t.right)
  store i32 %"depth(&dref.load.dref.t.right)", i32* %r, align 4
  %load.l = load i32, i32* %l, align 4
  %load.r = load i32, i32* %r, align 4
  %GT = icmp sgt i32 %load.l, %load.r
  br i1 %GT, label %then4, label %endif3

then4:                                            ; preds = %endif
  %load.l5 = load i32, i32* %l, align 4
  %ADD = add i32 %load.l5, 1
  ret i32 %ADD

endif3:                                           ; preds = %endif
  %load.r6 = load i32, i32* %r, align 4
  %ADD7 = add i32 %load.r6, 1
  ret i32 %ADD7
}

define i32 @main() {
entry:
  %a = alloca %Tree, align 8
  %b = alloca %Tree, align 8
  %c = alloca %Tree, align 8
  %d = alloca %Tree, align 8
  store %Tree zeroinitializer, %Tree* %a, align 8
  store %Tree zeroinitializer, %Tree* %b, align 8
  store %Tree zeroinitializer, %Tree* %c, align 8
  store %Tree zeroinitializer, %Tree* %d, align 8
  %load.a.value = getelementptr inbounds %Tree, %Tree* %a, i32 0, i32 0
  store i32 1, i32* %load.a.value, align 4
  %load.b.value = getelementptr inbounds %Tree, %Tree* %b, i32 0, i32 0
  store i32 2, i32* %load.b.value, align 4
  %load.c.value = getelementptr inbounds %Tree, %Tree* %c, i32 0, i32 0
  store i32 3, i32* %load.c.value, align 4
  %load.d.value = getelementptr inbounds %Tree, %Tree* %d, i32 0, i32 0
  store i32 4, i32* %load.d.value, align 4
  %load.a.left = getelementptr inbounds %Tree, %Tree* %a, i32 0, i32 1
  store %Tree* %b, %Tree** %load.a.left, align 8
  %load.a.right = getelementptr inbounds %Tree, %Tree* %a, i32 0, i32 2
  store %Tree* %c, %Tree** %load.a.right, align 8
  %load.b.left = getelementptr inbounds %Tree, %Tree* %b, i32 0, i32 1
  store %Tree* %d, %Tree** %load.b.left, align 8
  %"sum(&a)" = call i32 @sum(%Tree* %a)
  %MUL = mul i32 %"sum(&a)", 10
  %"depth(&a)" = call i32 @depth(%Tree* %a)
  %ADD = add i32 %MUL, %"depth(&a)"
  ret i32 %ADD
}
```

---

## 040 — crazy: a linked list over array elements

```ura
// expect: 33   (length 3, plus the third value 30)
struct Node:
    value i32
    next &Node

fn length(n &Node) i32:
    if isNullable(n): return 0
    return 1 + length(&n.next)

fn main() i32:
    a Node
    b Node
    c Node
    nodes Node[] = [a, b, c]
    i i32 = 0
    while i < 3:
        nodes[i].value = (i + 1) * 10
        if i < 2: nodes[i].next = &nodes[i + 1]
        i += 1
    return length(&nodes[0]) + nodes[0].next.next.value
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Node = type { i32, %Node* }

define i32 @length(%Node* %0) {
entry:
  %n = alloca %Node*, align 8
  store %Node* %0, %Node** %n, align 8
  %dref.n = load %Node*, %Node** %n, align 8
  %"isNullable(dref.n)" = icmp eq %Node* %dref.n, null
  br i1 %"isNullable(dref.n)", label %then, label %endif

then:                                             ; preds = %entry
  ret i32 0

endif:                                            ; preds = %entry
  %dref.n1 = load %Node*, %Node** %n, align 8
  %load.dref.n.next = getelementptr inbounds %Node, %Node* %dref.n1, i32 0, i32 1
  %dref.load.dref.n.next = load %Node*, %Node** %load.dref.n.next, align 8
  %"length(&dref.load.dref.n.next)" = call i32 @length(%Node* %dref.load.dref.n.next)
  %ADD = add i32 1, %"length(&dref.load.dref.n.next)"
  ret i32 %ADD
}

define i32 @main() {
entry:
  %a = alloca %Node, align 8
  %b = alloca %Node, align 8
  %c = alloca %Node, align 8
  %nodes = alloca %Node*, align 8
  %array = alloca [3 x %Node], align 8
  %i = alloca i32, align 4
  store %Node zeroinitializer, %Node* %a, align 8
  store %Node zeroinitializer, %Node* %b, align 8
  store %Node zeroinitializer, %Node* %c, align 8
  %load.a = load %Node, %Node* %a, align 8
  %load.b = load %Node, %Node* %b, align 8
  %load.c = load %Node, %Node* %c, align 8
  %e = getelementptr inbounds [3 x %Node], [3 x %Node]* %array, i32 0, i32 0
  store %Node %load.a, %Node* %e, align 8
  %e1 = getelementptr inbounds [3 x %Node], [3 x %Node]* %array, i32 0, i32 1
  store %Node %load.b, %Node* %e1, align 8
  %e2 = getelementptr inbounds [3 x %Node], [3 x %Node]* %array, i32 0, i32 2
  store %Node %load.c, %Node* %e2, align 8
  %array3 = getelementptr inbounds [3 x %Node], [3 x %Node]* %array, i32 0, i32 0
  store %Node* %array3, %Node** %nodes, align 8
  store i32 0, i32* %i, align 4
  br label %cond

cond:                                             ; preds = %endif, %entry
  %load.i = load i32, i32* %i, align 4
  %LT = icmp slt i32 %load.i, 3
  br i1 %LT, label %then, label %endwhile

then:                                             ; preds = %cond
  %load.nodes = load %Node*, %Node** %nodes, align 8
  %load.i4 = load i32, i32* %i, align 4
  %"load.nodes[load.i]" = getelementptr inbounds %Node, %Node* %load.nodes, i32 %load.i4
  %"load.load.nodes[load.i].value" = getelementptr inbounds %Node, %Node* %"load.nodes[load.i]", i32 0, i32 0
  %load.i5 = load i32, i32* %i, align 4
  %ADD = add i32 %load.i5, 1
  %MUL = mul i32 %ADD, 10
  store i32 %MUL, i32* %"load.load.nodes[load.i].value", align 4
  %load.i7 = load i32, i32* %i, align 4
  %LT8 = icmp slt i32 %load.i7, 2
  br i1 %LT8, label %then6, label %endif

endwhile:                                         ; preds = %cond
  %load.nodes15 = load %Node*, %Node** %nodes, align 8
  %"load.nodes[0]" = getelementptr inbounds %Node, %Node* %load.nodes15, i32 0
  %"length(&load.nodes[0])" = call i32 @length(%Node* %"load.nodes[0]")
  %load.nodes16 = load %Node*, %Node** %nodes, align 8
  %"load.nodes[0]17" = getelementptr inbounds %Node, %Node* %load.nodes16, i32 0
  %"load.load.nodes[0].next" = getelementptr inbounds %Node, %Node* %"load.nodes[0]17", i32 0, i32 1
  %"dref.load.load.nodes[0].next" = load %Node*, %Node** %"load.load.nodes[0].next", align 8
  %"load.dref.load.load.nodes[0].next.next" = getelementptr inbounds %Node, %Node* %"dref.load.load.nodes[0].next", i32 0, i32 1
  %"dref.load.dref.load.load.nodes[0].next.next" = load %Node*, %Node** %"load.dref.load.load.nodes[0].next.next", align 8
  %"load.dref.load.dref.load.load.nodes[0].next.next.value" = getelementptr inbounds %Node, %Node* %"dref.load.dref.load.load.nodes[0].next.next", i32 0, i32 0
  %"load.load.dref.load.dref.load.load.nodes[0].next.next.value" = load i32, i32* %"load.dref.load.dref.load.load.nodes[0].next.next.value", align 4
  %ADD18 = add i32 %"length(&load.nodes[0])", %"load.load.dref.load.dref.load.load.nodes[0].next.next.value"
  ret i32 %ADD18

then6:                                            ; preds = %then
  %load.nodes9 = load %Node*, %Node** %nodes, align 8
  %load.i10 = load i32, i32* %i, align 4
  %"load.nodes[load.i]11" = getelementptr inbounds %Node, %Node* %load.nodes9, i32 %load.i10
  %"load.load.nodes[load.i].next" = getelementptr inbounds %Node, %Node* %"load.nodes[load.i]11", i32 0, i32 1
  %load.nodes12 = load %Node*, %Node** %nodes, align 8
  %load.i13 = load i32, i32* %i, align 4
  %ADD14 = add i32 %load.i13, 1
  %"load.nodes[load.i + 1]" = getelementptr inbounds %Node, %Node* %load.nodes12, i32 %ADD14
  store %Node* %"load.nodes[load.i + 1]", %Node** %"load.load.nodes[load.i].next", align 8
  br label %endif

endif:                                            ; preds = %then6, %then
  %old = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  br label %cond
}
```

---

## 041 — crazy: search a linked list, -1 when missing

```ura
// expect: 121   (13 at 2 * 10, 8 at 1, 99 missing: 100)
struct Node:
    value i32
    next &Node

fn find(head &Node, v i32) i32:
    i i32 = 0
    p &Node = &head
    while !isNullable(p):
        if p.value == v: return i
        p = &p.next
        i += 1
    return -1

fn main() i32:
    a Node
    b Node
    c Node
    a.value = 5
    b.value = 8
    c.value = 13
    a.next = &b
    b.next = &c
    r i32 = find(&a, 13) * 10 + find(&a, 8)
    if find(&a, 99) == -1: r += 100
    return r
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Node = type { i32, %Node* }

define i32 @find(%Node* %0, i32 %1) {
entry:
  %head = alloca %Node*, align 8
  %v = alloca i32, align 4
  %i = alloca i32, align 4
  %p = alloca %Node*, align 8
  store %Node* %0, %Node** %head, align 8
  store i32 %1, i32* %v, align 4
  store i32 0, i32* %i, align 4
  %dref.head = load %Node*, %Node** %head, align 8
  store %Node* %dref.head, %Node** %p, align 8
  br label %cond

cond:                                             ; preds = %endif, %entry
  %dref.p = load %Node*, %Node** %p, align 8
  %"isNullable(dref.p)" = icmp eq %Node* %dref.p, null
  %NOT = xor i1 %"isNullable(dref.p)", true
  br i1 %NOT, label %then, label %endwhile

then:                                             ; preds = %cond
  %dref.p2 = load %Node*, %Node** %p, align 8
  %load.dref.p.value = getelementptr inbounds %Node, %Node* %dref.p2, i32 0, i32 0
  %load.load.dref.p.value = load i32, i32* %load.dref.p.value, align 4
  %load.v = load i32, i32* %v, align 4
  %EQ = icmp eq i32 %load.load.dref.p.value, %load.v
  br i1 %EQ, label %then1, label %endif

endwhile:                                         ; preds = %cond
  ret i32 -1

then1:                                            ; preds = %then
  %load.i = load i32, i32* %i, align 4
  ret i32 %load.i

endif:                                            ; preds = %then
  %dref.p3 = load %Node*, %Node** %p, align 8
  %load.dref.p.next = getelementptr inbounds %Node, %Node* %dref.p3, i32 0, i32 1
  %dref.load.dref.p.next = load %Node*, %Node** %load.dref.p.next, align 8
  store %Node* %dref.load.dref.p.next, %Node** %p, align 8
  %old = load i32, i32* %i, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %i, align 4
  br label %cond
}

define i32 @main() {
entry:
  %a = alloca %Node, align 8
  %b = alloca %Node, align 8
  %c = alloca %Node, align 8
  %r = alloca i32, align 4
  store %Node zeroinitializer, %Node* %a, align 8
  store %Node zeroinitializer, %Node* %b, align 8
  store %Node zeroinitializer, %Node* %c, align 8
  %load.a.value = getelementptr inbounds %Node, %Node* %a, i32 0, i32 0
  store i32 5, i32* %load.a.value, align 4
  %load.b.value = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  store i32 8, i32* %load.b.value, align 4
  %load.c.value = getelementptr inbounds %Node, %Node* %c, i32 0, i32 0
  store i32 13, i32* %load.c.value, align 4
  %load.a.next = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  store %Node* %b, %Node** %load.a.next, align 8
  %load.b.next = getelementptr inbounds %Node, %Node* %b, i32 0, i32 1
  store %Node* %c, %Node** %load.b.next, align 8
  %"find(&a,13)" = call i32 @find(%Node* %a, i32 13)
  %MUL = mul i32 %"find(&a,13)", 10
  %"find(&a,8)" = call i32 @find(%Node* %a, i32 8)
  %ADD = add i32 %MUL, %"find(&a,8)"
  store i32 %ADD, i32* %r, align 4
  %"find(&a,99)" = call i32 @find(%Node* %a, i32 99)
  %EQ = icmp eq i32 %"find(&a,99)", -1
  br i1 %EQ, label %then, label %endif

then:                                             ; preds = %entry
  %old = load i32, i32* %r, align 4
  %ADD_ASSIGN = add i32 %old, 100
  store i32 %ADD_ASSIGN, i32* %r, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %load.r = load i32, i32* %r, align 4
  ret i32 %load.r
}
```

---

## 042 — crazy: assigning a struct copies it

```ura
// expect: 63   (1 + 20 + 10 + 2 + 10 + 20)
struct Point:
    x i32
    y i32

fn main() i32:
    a Point
    a.x = 1
    a.y = 2
    b Point = a
    b.x = 10
    c Point
    c = b
    c.y = 20
    return a.x + a.y * 10 + b.x + b.y + c.x + c.y
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Point = type { i32, i32 }

define i32 @main() {
entry:
  %a = alloca %Point, align 8
  %b = alloca %Point, align 8
  %c = alloca %Point, align 8
  store %Point zeroinitializer, %Point* %a, align 4
  %a.x = getelementptr inbounds %Point, %Point* %a, i32 0, i32 0
  store i32 1, i32* %a.x, align 4
  %a.y = getelementptr inbounds %Point, %Point* %a, i32 0, i32 1
  store i32 2, i32* %a.y, align 4
  %"$a" = load %Point, %Point* %a, align 4
  store %Point %"$a", %Point* %b, align 4
  %b.x = getelementptr inbounds %Point, %Point* %b, i32 0, i32 0
  store i32 10, i32* %b.x, align 4
  %"$b" = load %Point, %Point* %b, align 4
  store %Point %"$b", %Point* %c, align 4
  %c.y = getelementptr inbounds %Point, %Point* %c, i32 0, i32 1
  store i32 20, i32* %c.y, align 4
  %a.x1 = getelementptr inbounds %Point, %Point* %a, i32 0, i32 0
  %a.y2 = getelementptr inbounds %Point, %Point* %a, i32 0, i32 1
  %"$a.y" = load i32, i32* %a.y2, align 4
  %MUL = mul i32 %"$a.y", 10
  %"$a.x" = load i32, i32* %a.x1, align 4
  %ADD = add i32 %"$a.x", %MUL
  %b.x3 = getelementptr inbounds %Point, %Point* %b, i32 0, i32 0
  %"$b.x" = load i32, i32* %b.x3, align 4
  %ADD4 = add i32 %ADD, %"$b.x"
  %b.y = getelementptr inbounds %Point, %Point* %b, i32 0, i32 1
  %"$b.y" = load i32, i32* %b.y, align 4
  %ADD5 = add i32 %ADD4, %"$b.y"
  %c.x = getelementptr inbounds %Point, %Point* %c, i32 0, i32 0
  %"$c.x" = load i32, i32* %c.x, align 4
  %ADD6 = add i32 %ADD5, %"$c.x"
  %c.y7 = getelementptr inbounds %Point, %Point* %c, i32 0, i32 1
  %"$c.y" = load i32, i32* %c.y7, align 4
  %ADD8 = add i32 %ADD6, %"$c.y"
  ret i32 %ADD8
}
```

---

## 043 — crazy: a struct parameter is a copy

```ura
// expect: 61
struct Point:
    x i32

fn bump(p Point) i32:
    p.x += 5
    return p.x

fn main() i32:
    a Point
    a.x = 1
    r i32 = bump(a)
    return r * 10 + a.x
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Point = type { i32 }

define i32 @bump(%Point %0) {
entry:
  %p = alloca %Point, align 8
  store %Point %0, %Point* %p, align 4
  %p.x = getelementptr inbounds %Point, %Point* %p, i32 0, i32 0
  %"$p.x" = load i32, i32* %p.x, align 4
  %ADD = add i32 %"$p.x", 5
  store i32 %ADD, i32* %p.x, align 4
  %p.x1 = getelementptr inbounds %Point, %Point* %p, i32 0, i32 0
  %"$p.x2" = load i32, i32* %p.x1, align 4
  ret i32 %"$p.x2"
}

define i32 @main() {
entry:
  %a = alloca %Point, align 8
  %r = alloca i32, align 4
  store %Point zeroinitializer, %Point* %a, align 4
  %a.x = getelementptr inbounds %Point, %Point* %a, i32 0, i32 0
  store i32 1, i32* %a.x, align 4
  %"$a" = load %Point, %Point* %a, align 4
  %call.bump = call i32 @bump(%Point %"$a")
  store i32 %call.bump, i32* %r, align 4
  %"$r" = load i32, i32* %r, align 4
  %MUL = mul i32 %"$r", 10
  %a.x1 = getelementptr inbounds %Point, %Point* %a, i32 0, i32 0
  %"$a.x" = load i32, i32* %a.x1, align 4
  %ADD = add i32 %MUL, %"$a.x"
  ret i32 %ADD
}
```

---

## 044 — crazy: compound assignment through a ref attribute

```ura
// expect: 14   ((0 + 5) * 3 - 1)
struct Node:
    value i32
    next &Node

fn main() i32:
    a Node
    b Node
    a.next = &b
    a.next.value += 5
    a.next.value *= 3
    b.value -= 1
    return b.value
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Node = type { i32, %Node* }

define i32 @main() {
entry:
  %a = alloca %Node, align 8
  %b = alloca %Node, align 8
  store %Node zeroinitializer, %Node* %a, align 8
  store %Node zeroinitializer, %Node* %b, align 8
  %a.next = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  store %Node* %b, %Node** %a.next, align 8
  %a.next1 = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  %"*a.next" = load %Node*, %Node** %a.next1, align 8
  %"*a.next.value" = getelementptr inbounds %Node, %Node* %"*a.next", i32 0, i32 0
  %"$*a.next.value" = load i32, i32* %"*a.next.value", align 4
  %ADD = add i32 %"$*a.next.value", 5
  store i32 %ADD, i32* %"*a.next.value", align 4
  %a.next2 = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  %"*a.next3" = load %Node*, %Node** %a.next2, align 8
  %"*a.next.value4" = getelementptr inbounds %Node, %Node* %"*a.next3", i32 0, i32 0
  %"$*a.next.value5" = load i32, i32* %"*a.next.value4", align 4
  %MUL = mul i32 %"$*a.next.value5", 3
  store i32 %MUL, i32* %"*a.next.value4", align 4
  %b.value = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  %"$b.value" = load i32, i32* %b.value, align 4
  %SUB = sub i32 %"$b.value", 1
  store i32 %SUB, i32* %b.value, align 4
  %b.value6 = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  %"$b.value7" = load i32, i32* %b.value6, align 4
  ret i32 %"$b.value7"
}
```

---

## 045 — crazy: a method on an array element

```ura
// expect: 45   (4 * 10 + 5)
struct Counter:
    n i32

    fn add(k i32):
        self.n += k

fn main() i32:
    a Counter
    b Counter
    cs Counter[] = [a, b]
    i i32 = 0
    while i < 2:
        cs[i].add(i + 3)
        cs[i].add(1)
        i += 1
    return cs[0].n * 10 + cs[1].n
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Counter = type { i32 }

define void @Counter.add(%Counter* %0, i32 %1) {
entry:
  %self = alloca %Counter*, align 8
  %k = alloca i32, align 4
  store %Counter* %0, %Counter** %self, align 8
  store i32 %1, i32* %k, align 4
  %"*self" = load %Counter*, %Counter** %self, align 8
  %"*self.n" = getelementptr inbounds %Counter, %Counter* %"*self", i32 0, i32 0
  %"$k" = load i32, i32* %k, align 4
  %"$*self.n" = load i32, i32* %"*self.n", align 4
  %ADD = add i32 %"$*self.n", %"$k"
  store i32 %ADD, i32* %"*self.n", align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Counter, align 8
  %b = alloca %Counter, align 8
  %cs = alloca %Counter*, align 8
  %arr = alloca [2 x %Counter], align 8
  %i = alloca i32, align 4
  store %Counter zeroinitializer, %Counter* %a, align 4
  store %Counter zeroinitializer, %Counter* %b, align 4
  %"$a" = load %Counter, %Counter* %a, align 4
  %"$b" = load %Counter, %Counter* %b, align 4
  %0 = insertvalue [2 x %Counter] undef, %Counter %"$a", 0
  %1 = insertvalue [2 x %Counter] %0, %Counter %"$b", 1
  store [2 x %Counter] %1, [2 x %Counter]* %arr, align 4
  %2 = getelementptr inbounds [2 x %Counter], [2 x %Counter]* %arr, i32 0, i32 0
  store %Counter* %2, %Counter** %cs, align 8
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 2
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$cs" = load %Counter*, %Counter** %cs, align 8
  %"$i1" = load i32, i32* %i, align 4
  %"cs[]" = getelementptr inbounds %Counter, %Counter* %"$cs", i32 %"$i1"
  %"$i2" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i2", 3
  call void @Counter.add(%Counter* %"cs[]", i32 %ADD)
  %"$cs3" = load %Counter*, %Counter** %cs, align 8
  %"$i4" = load i32, i32* %i, align 4
  %"cs[]5" = getelementptr inbounds %Counter, %Counter* %"$cs3", i32 %"$i4"
  call void @Counter.add(%Counter* %"cs[]5", i32 1)
  %"$i6" = load i32, i32* %i, align 4
  %ADD7 = add i32 %"$i6", 1
  store i32 %ADD7, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$cs8" = load %Counter*, %Counter** %cs, align 8
  %"cs[]9" = getelementptr inbounds %Counter, %Counter* %"$cs8", i32 0
  %"cs[].n" = getelementptr inbounds %Counter, %Counter* %"cs[]9", i32 0, i32 0
  %"$cs[].n" = load i32, i32* %"cs[].n", align 4
  %MUL = mul i32 %"$cs[].n", 10
  %"$cs10" = load %Counter*, %Counter** %cs, align 8
  %"cs[]11" = getelementptr inbounds %Counter, %Counter* %"$cs10", i32 1
  %"cs[].n12" = getelementptr inbounds %Counter, %Counter* %"cs[]11", i32 0, i32 0
  %"$cs[].n13" = load i32, i32* %"cs[].n12", align 4
  %ADD14 = add i32 %MUL, %"$cs[].n13"
  ret i32 %ADD14
}
```

---

## 046 — crazy: builder: a method returns a ref to self, chained

```ura
// expect: 6
struct Builder:
    total i32

    fn add(v i32) &Builder:
        self.total += v
        return &self

fn main() i32:
    b Builder
    b.add(1).add(2).add(3)
    return b.total
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Builder = type { i32 }

define %Builder* @Builder.add(%Builder* %0, i32 %1) {
entry:
  %self = alloca %Builder*, align 8
  %v = alloca i32, align 4
  store %Builder* %0, %Builder** %self, align 8
  store i32 %1, i32* %v, align 4
  %"*self" = load %Builder*, %Builder** %self, align 8
  %"*self.total" = getelementptr inbounds %Builder, %Builder* %"*self", i32 0, i32 0
  %"$v" = load i32, i32* %v, align 4
  %"$*self.total" = load i32, i32* %"*self.total", align 4
  %ADD = add i32 %"$*self.total", %"$v"
  store i32 %ADD, i32* %"*self.total", align 4
  %"*self1" = load %Builder*, %Builder** %self, align 8
  ret %Builder* %"*self1"
}

define i32 @main() {
entry:
  %b = alloca %Builder, align 8
  %ref = alloca %Builder*, align 8
  %ref2 = alloca %Builder*, align 8
  %ref5 = alloca %Builder*, align 8
  store %Builder zeroinitializer, %Builder* %b, align 4
  %call.Builder.add = call %Builder* @Builder.add(%Builder* %b, i32 1)
  store %Builder* %call.Builder.add, %Builder** %ref, align 8
  %"*ref" = load %Builder*, %Builder** %ref, align 8
  %call.Builder.add1 = call %Builder* @Builder.add(%Builder* %"*ref", i32 2)
  store %Builder* %call.Builder.add1, %Builder** %ref2, align 8
  %"*ref3" = load %Builder*, %Builder** %ref2, align 8
  %call.Builder.add4 = call %Builder* @Builder.add(%Builder* %"*ref3", i32 3)
  store %Builder* %call.Builder.add4, %Builder** %ref5, align 8
  %"*ref6" = load %Builder*, %Builder** %ref5, align 8
  %b.total = getelementptr inbounds %Builder, %Builder* %b, i32 0, i32 0
  %"$b.total" = load i32, i32* %b.total, align 4
  ret i32 %"$b.total"
}
```

---

## 047 — crazy: a struct holding an array of structs

```ura
// expect: 12   (9 - (-3))
struct Point:
    x i32
    y i32

struct Shape:
    points Point[]
    count i32

    fn width() i32:
        lo i32 = self.points[0].x
        hi i32 = lo
        i i32 = 1
        while i < self.count:
            x i32 = self.points[i].x
            if x < lo: lo = x
            if x > hi: hi = x
            i += 1
        return hi - lo

fn main() i32:
    a Point
    b Point
    c Point
    a.x = 4
    b.x = -3
    c.x = 9
    s Shape
    s.points = [a, b, c]
    s.count = 3
    return s.width()
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Shape = type { %Point*, i32 }
%Point = type { i32, i32 }

define i32 @Shape.width(%Shape* %0) {
entry:
  %self = alloca %Shape*, align 8
  %lo = alloca i32, align 4
  %hi = alloca i32, align 4
  %i = alloca i32, align 4
  %x = alloca i32, align 4
  store %Shape* %0, %Shape** %self, align 8
  %"*self" = load %Shape*, %Shape** %self, align 8
  %"*self.points" = getelementptr inbounds %Shape, %Shape* %"*self", i32 0, i32 0
  %"$*self.points" = load %Point*, %Point** %"*self.points", align 8
  %"*self.points[]" = getelementptr inbounds %Point, %Point* %"$*self.points", i32 0
  %"*self.points[].x" = getelementptr inbounds %Point, %Point* %"*self.points[]", i32 0, i32 0
  %"$*self.points[].x" = load i32, i32* %"*self.points[].x", align 4
  store i32 %"$*self.points[].x", i32* %lo, align 4
  %"$lo" = load i32, i32* %lo, align 4
  store i32 %"$lo", i32* %hi, align 4
  store i32 1, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %endif1, %entry
  %"*self2" = load %Shape*, %Shape** %self, align 8
  %"*self.count" = getelementptr inbounds %Shape, %Shape* %"*self2", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count"
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"*self3" = load %Shape*, %Shape** %self, align 8
  %"*self.points4" = getelementptr inbounds %Shape, %Shape* %"*self3", i32 0, i32 0
  %"$*self.points5" = load %Point*, %Point** %"*self.points4", align 8
  %"$i6" = load i32, i32* %i, align 4
  %"*self.points[]7" = getelementptr inbounds %Point, %Point* %"$*self.points5", i32 %"$i6"
  %"*self.points[].x8" = getelementptr inbounds %Point, %Point* %"*self.points[]7", i32 0, i32 0
  %"$*self.points[].x9" = load i32, i32* %"*self.points[].x8", align 4
  store i32 %"$*self.points[].x9", i32* %x, align 4
  br label %if1

if1:                                              ; preds = %while0.then
  %"$x" = load i32, i32* %x, align 4
  %"$lo10" = load i32, i32* %lo, align 4
  %LT11 = icmp slt i32 %"$x", %"$lo10"
  br i1 %LT11, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  %"$x12" = load i32, i32* %x, align 4
  store i32 %"$x12", i32* %lo, align 4
  br label %endif

endif:                                            ; preds = %if1.then, %if1
  br label %if2

if2:                                              ; preds = %endif
  %"$x13" = load i32, i32* %x, align 4
  %"$hi" = load i32, i32* %hi, align 4
  %GT = icmp sgt i32 %"$x13", %"$hi"
  br i1 %GT, label %if2.then, label %endif1

if2.then:                                         ; preds = %if2
  %"$x14" = load i32, i32* %x, align 4
  store i32 %"$x14", i32* %hi, align 4
  br label %endif1

endif1:                                           ; preds = %if2.then, %if2
  %"$i15" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i15", 1
  store i32 %ADD, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %"$hi16" = load i32, i32* %hi, align 4
  %"$lo17" = load i32, i32* %lo, align 4
  %SUB = sub i32 %"$hi16", %"$lo17"
  ret i32 %SUB
}

define i32 @main() {
entry:
  %a = alloca %Point, align 8
  %b = alloca %Point, align 8
  %c = alloca %Point, align 8
  %s = alloca %Shape, align 8
  %arr = alloca [3 x %Point], align 8
  store %Point zeroinitializer, %Point* %a, align 4
  store %Point zeroinitializer, %Point* %b, align 4
  store %Point zeroinitializer, %Point* %c, align 4
  %a.x = getelementptr inbounds %Point, %Point* %a, i32 0, i32 0
  store i32 4, i32* %a.x, align 4
  %b.x = getelementptr inbounds %Point, %Point* %b, i32 0, i32 0
  store i32 -3, i32* %b.x, align 4
  %c.x = getelementptr inbounds %Point, %Point* %c, i32 0, i32 0
  store i32 9, i32* %c.x, align 4
  store %Shape zeroinitializer, %Shape* %s, align 8
  %s.points = getelementptr inbounds %Shape, %Shape* %s, i32 0, i32 0
  %"$a" = load %Point, %Point* %a, align 4
  %"$b" = load %Point, %Point* %b, align 4
  %"$c" = load %Point, %Point* %c, align 4
  %0 = insertvalue [3 x %Point] undef, %Point %"$a", 0
  %1 = insertvalue [3 x %Point] %0, %Point %"$b", 1
  %2 = insertvalue [3 x %Point] %1, %Point %"$c", 2
  store [3 x %Point] %2, [3 x %Point]* %arr, align 4
  %3 = getelementptr inbounds [3 x %Point], [3 x %Point]* %arr, i32 0, i32 0
  store %Point* %3, %Point** %s.points, align 8
  %s.count = getelementptr inbounds %Shape, %Shape* %s, i32 0, i32 1
  store i32 3, i32* %s.count, align 4
  %call.Shape.width = call i32 @Shape.width(%Shape* %s)
  ret i32 %call.Shape.width
}
```

---

## 048 — crazy: tree walk with a ref out-parameter

```ura
// expect: 183   (sum 18 * 10, 3 nodes)
struct Tree:
    value i32
    left &Tree
    right &Tree

fn walk(t &Tree, count &i32) i32:
    if isNullable(t): return 0
    count += 1
    return t.value + walk(&t.left, &count) + walk(&t.right, &count)

fn main() i32:
    a Tree
    b Tree
    c Tree
    a.value = 5
    b.value = 6
    c.value = 7
    a.left = &b
    b.right = &c
    n i32 = 0
    s i32 = walk(&a, &n)
    return s * 10 + n
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Tree = type { i32, %Tree*, %Tree* }

define i32 @walk(%Tree* %0, i32* %1) {
entry:
  %t = alloca %Tree*, align 8
  %count = alloca i32*, align 8
  store %Tree* %0, %Tree** %t, align 8
  store i32* %1, i32** %count, align 8
  %dref.t = load %Tree*, %Tree** %t, align 8
  %"isNullable(dref.t)" = icmp eq %Tree* %dref.t, null
  br i1 %"isNullable(dref.t)", label %then, label %endif

then:                                             ; preds = %entry
  ret i32 0

endif:                                            ; preds = %entry
  %dref.count = load i32*, i32** %count, align 8
  %old = load i32, i32* %dref.count, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %dref.count, align 4
  %dref.t1 = load %Tree*, %Tree** %t, align 8
  %load.dref.t.value = getelementptr inbounds %Tree, %Tree* %dref.t1, i32 0, i32 0
  %load.load.dref.t.value = load i32, i32* %load.dref.t.value, align 4
  %dref.t2 = load %Tree*, %Tree** %t, align 8
  %load.dref.t.left = getelementptr inbounds %Tree, %Tree* %dref.t2, i32 0, i32 1
  %dref.load.dref.t.left = load %Tree*, %Tree** %load.dref.t.left, align 8
  %dref.count3 = load i32*, i32** %count, align 8
  %"walk(&dref.load.dref.t.left,&dref.count)" = call i32 @walk(%Tree* %dref.load.dref.t.left, i32* %dref.count3)
  %ADD = add i32 %load.load.dref.t.value, %"walk(&dref.load.dref.t.left,&dref.count)"
  %dref.t4 = load %Tree*, %Tree** %t, align 8
  %load.dref.t.right = getelementptr inbounds %Tree, %Tree* %dref.t4, i32 0, i32 2
  %dref.load.dref.t.right = load %Tree*, %Tree** %load.dref.t.right, align 8
  %dref.count5 = load i32*, i32** %count, align 8
  %"walk(&dref.load.dref.t.right,&dref.count)" = call i32 @walk(%Tree* %dref.load.dref.t.right, i32* %dref.count5)
  %ADD6 = add i32 %ADD, %"walk(&dref.load.dref.t.right,&dref.count)"
  ret i32 %ADD6
}

define i32 @main() {
entry:
  %a = alloca %Tree, align 8
  %b = alloca %Tree, align 8
  %c = alloca %Tree, align 8
  %n = alloca i32, align 4
  %s = alloca i32, align 4
  store %Tree zeroinitializer, %Tree* %a, align 8
  store %Tree zeroinitializer, %Tree* %b, align 8
  store %Tree zeroinitializer, %Tree* %c, align 8
  %load.a.value = getelementptr inbounds %Tree, %Tree* %a, i32 0, i32 0
  store i32 5, i32* %load.a.value, align 4
  %load.b.value = getelementptr inbounds %Tree, %Tree* %b, i32 0, i32 0
  store i32 6, i32* %load.b.value, align 4
  %load.c.value = getelementptr inbounds %Tree, %Tree* %c, i32 0, i32 0
  store i32 7, i32* %load.c.value, align 4
  %load.a.left = getelementptr inbounds %Tree, %Tree* %a, i32 0, i32 1
  store %Tree* %b, %Tree** %load.a.left, align 8
  %load.b.right = getelementptr inbounds %Tree, %Tree* %b, i32 0, i32 2
  store %Tree* %c, %Tree** %load.b.right, align 8
  store i32 0, i32* %n, align 4
  %"walk(&a,&n)" = call i32 @walk(%Tree* %a, i32* %n)
  store i32 %"walk(&a,&n)", i32* %s, align 4
  %load.s = load i32, i32* %s, align 4
  %MUL = mul i32 %load.s, 10
  %load.n = load i32, i32* %n, align 4
  %ADD = add i32 %MUL, %load.n
  ret i32 %ADD
}
```

---

## 049 — crazy: a function returns a ref into a struct, the caller binds it

```ura
// expect: 103   (r binds to x.b: 3 + 100)
struct Pair:
    a i32
    b i32

fn bigger(p &Pair) &i32:
    if p.a > p.b: return &p.a
    return &p.b

fn main() i32:
    x Pair
    x.a = 3
    x.b = 7
    r &i32 = bigger(&x)
    r = 100
    return x.a + x.b
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Pair = type { i32, i32 }

define i32* @bigger(%Pair* %0) {
entry:
  %p = alloca %Pair*, align 8
  store %Pair* %0, %Pair** %p, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*p" = load %Pair*, %Pair** %p, align 8
  %"*p.a" = getelementptr inbounds %Pair, %Pair* %"*p", i32 0, i32 0
  %"*p1" = load %Pair*, %Pair** %p, align 8
  %"*p.b" = getelementptr inbounds %Pair, %Pair* %"*p1", i32 0, i32 1
  %"$*p.a" = load i32, i32* %"*p.a", align 4
  %"$*p.b" = load i32, i32* %"*p.b", align 4
  %GT = icmp sgt i32 %"$*p.a", %"$*p.b"
  br i1 %GT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*p2" = load %Pair*, %Pair** %p, align 8
  %"*p.a3" = getelementptr inbounds %Pair, %Pair* %"*p2", i32 0, i32 0
  ret i32* %"*p.a3"

endif:                                            ; preds = %if0
  %"*p4" = load %Pair*, %Pair** %p, align 8
  %"*p.b5" = getelementptr inbounds %Pair, %Pair* %"*p4", i32 0, i32 1
  ret i32* %"*p.b5"
}

define i32 @main() {
entry:
  %x = alloca %Pair, align 8
  %r = alloca i32*, align 8
  %ref = alloca i32*, align 8
  store %Pair zeroinitializer, %Pair* %x, align 4
  %x.a = getelementptr inbounds %Pair, %Pair* %x, i32 0, i32 0
  store i32 3, i32* %x.a, align 4
  %x.b = getelementptr inbounds %Pair, %Pair* %x, i32 0, i32 1
  store i32 7, i32* %x.b, align 4
  %call.bigger = call i32* @bigger(%Pair* %x)
  store i32* %call.bigger, i32** %ref, align 8
  %"*ref" = load i32*, i32** %ref, align 8
  store i32* %"*ref", i32** %r, align 8
  %"*r" = load i32*, i32** %r, align 8
  store i32 100, i32* %"*r", align 4
  %x.a1 = getelementptr inbounds %Pair, %Pair* %x, i32 0, i32 0
  %x.b2 = getelementptr inbounds %Pair, %Pair* %x, i32 0, i32 1
  %"$x.a" = load i32, i32* %x.a1, align 4
  %"$x.b" = load i32, i32* %x.b2, align 4
  %ADD = add i32 %"$x.a", %"$x.b"
  ret i32 %ADD
}
```

---

## 050 — crazy: fibonacci as a pair: recursion returning a struct, .a on a call

```ura
// expect: 55
struct Pair:
    a i32
    b i32

fn fib(n i32) Pair:
    r Pair
    if n == 0:
        r.a = 0
        r.b = 1
        return r
    p Pair = fib(n - 1)
    r.a = p.b
    r.b = p.a + p.b
    return r

fn main() i32:
    return fib(10).a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Pair = type { i32, i32 }

define %Pair @fib(i32 %0) {
entry:
  %n = alloca i32, align 4
  %r = alloca %Pair, align 8
  %p = alloca %Pair, align 8
  store i32 %0, i32* %n, align 4
  store %Pair zeroinitializer, %Pair* %r, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %"$n", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %r.a = getelementptr inbounds %Pair, %Pair* %r, i32 0, i32 0
  store i32 0, i32* %r.a, align 4
  %r.b = getelementptr inbounds %Pair, %Pair* %r, i32 0, i32 1
  store i32 1, i32* %r.b, align 4
  %"$r" = load %Pair, %Pair* %r, align 4
  ret %Pair %"$r"

endif:                                            ; preds = %if0
  %"$n1" = load i32, i32* %n, align 4
  %SUB = sub i32 %"$n1", 1
  %call.fib = call %Pair @fib(i32 %SUB)
  store %Pair %call.fib, %Pair* %p, align 4
  %r.a2 = getelementptr inbounds %Pair, %Pair* %r, i32 0, i32 0
  %p.b = getelementptr inbounds %Pair, %Pair* %p, i32 0, i32 1
  %"$p.b" = load i32, i32* %p.b, align 4
  store i32 %"$p.b", i32* %r.a2, align 4
  %r.b3 = getelementptr inbounds %Pair, %Pair* %r, i32 0, i32 1
  %p.a = getelementptr inbounds %Pair, %Pair* %p, i32 0, i32 0
  %p.b4 = getelementptr inbounds %Pair, %Pair* %p, i32 0, i32 1
  %"$p.a" = load i32, i32* %p.a, align 4
  %"$p.b5" = load i32, i32* %p.b4, align 4
  %ADD = add i32 %"$p.a", %"$p.b5"
  store i32 %ADD, i32* %r.b3, align 4
  %"$r6" = load %Pair, %Pair* %r, align 4
  ret %Pair %"$r6"
}

define i32 @main() {
entry:
  %tmp = alloca %Pair, align 8
  %call.fib = call %Pair @fib(i32 10)
  store %Pair %call.fib, %Pair* %tmp, align 4
  %tmp.a = getelementptr inbounds %Pair, %Pair* %tmp, i32 0, i32 0
  %"$tmp.a" = load i32, i32* %tmp.a, align 4
  ret i32 %"$tmp.a"
}
```

---

## 051 — crazy: method calls nested as arguments

```ura
// expect: 160   (a 15 * 10, b 10)
struct Acc:
    v i32

    fn get() i32:
        return self.v

    fn add(k i32) i32:
        self.v += k
        return self.v

fn main() i32:
    a Acc
    b Acc
    b.v = 5
    a.add(b.add(a.add(b.get())))
    return a.v * 10 + b.v
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Acc = type { i32 }

define i32 @Acc.get(%Acc* %0) {
entry:
  %self = alloca %Acc*, align 8
  store %Acc* %0, %Acc** %self, align 8
  %"*self" = load %Acc*, %Acc** %self, align 8
  %"*self.v" = getelementptr inbounds %Acc, %Acc* %"*self", i32 0, i32 0
  %"$*self.v" = load i32, i32* %"*self.v", align 4
  ret i32 %"$*self.v"
}

define i32 @Acc.add(%Acc* %0, i32 %1) {
entry:
  %self = alloca %Acc*, align 8
  %k = alloca i32, align 4
  store %Acc* %0, %Acc** %self, align 8
  store i32 %1, i32* %k, align 4
  %"*self" = load %Acc*, %Acc** %self, align 8
  %"*self.v" = getelementptr inbounds %Acc, %Acc* %"*self", i32 0, i32 0
  %"$k" = load i32, i32* %k, align 4
  %"$*self.v" = load i32, i32* %"*self.v", align 4
  %ADD = add i32 %"$*self.v", %"$k"
  store i32 %ADD, i32* %"*self.v", align 4
  %"*self1" = load %Acc*, %Acc** %self, align 8
  %"*self.v2" = getelementptr inbounds %Acc, %Acc* %"*self1", i32 0, i32 0
  %"$*self.v3" = load i32, i32* %"*self.v2", align 4
  ret i32 %"$*self.v3"
}

define i32 @main() {
entry:
  %a = alloca %Acc, align 8
  %b = alloca %Acc, align 8
  store %Acc zeroinitializer, %Acc* %a, align 4
  store %Acc zeroinitializer, %Acc* %b, align 4
  %b.v = getelementptr inbounds %Acc, %Acc* %b, i32 0, i32 0
  store i32 5, i32* %b.v, align 4
  %call.Acc.get = call i32 @Acc.get(%Acc* %b)
  %call.Acc.add = call i32 @Acc.add(%Acc* %a, i32 %call.Acc.get)
  %call.Acc.add1 = call i32 @Acc.add(%Acc* %b, i32 %call.Acc.add)
  %call.Acc.add2 = call i32 @Acc.add(%Acc* %a, i32 %call.Acc.add1)
  %a.v = getelementptr inbounds %Acc, %Acc* %a, i32 0, i32 0
  %"$a.v" = load i32, i32* %a.v, align 4
  %MUL = mul i32 %"$a.v", 10
  %b.v3 = getelementptr inbounds %Acc, %Acc* %b, i32 0, i32 0
  %"$b.v" = load i32, i32* %b.v3, align 4
  %ADD = add i32 %MUL, %"$b.v"
  ret i32 %ADD
}
```

---

## 052 — crazy: print a linked list backwards with recursion

```ura
// stdout: 321
// expect: 6
proto printf(fmt i8[], ...) i32

struct Node:
    value i32
    next &Node

fn print_rev(n &Node):
    if isNullable(n): return
    print_rev(&n.next)
    printf("%d", n.value)

fn main() i32:
    a Node
    b Node
    c Node
    a.value = 1
    b.value = 2
    c.value = 3
    a.next = &b
    b.next = &c
    print_rev(&a)
    printf("\n")
    return a.value + b.value + c.value
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Node = type { i32, %Node* }

@str = private constant [3 x i8] c"%d\00"
@str.1 = private constant [2 x i8] c"\0A\00"

declare i32 @printf(i8*, ...)

define void @print_rev(%Node* %0) {
entry:
  %n = alloca %Node*, align 8
  %s = alloca [3 x i8], align 1
  store %Node* %0, %Node** %n, align 8
  %dref.n = load %Node*, %Node** %n, align 8
  %"isNullable(dref.n)" = icmp eq %Node* %dref.n, null
  br i1 %"isNullable(dref.n)", label %then, label %endif

then:                                             ; preds = %entry
  ret void

endif:                                            ; preds = %entry
  %dref.n1 = load %Node*, %Node** %n, align 8
  %load.dref.n.next = getelementptr inbounds %Node, %Node* %dref.n1, i32 0, i32 1
  %dref.load.dref.n.next = load %Node*, %Node** %load.dref.n.next, align 8
  call void @print_rev(%Node* %dref.load.dref.n.next)
  %dst = getelementptr inbounds [3 x i8], [3 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i64 3, i1 false)
  %dref.n2 = load %Node*, %Node** %n, align 8
  %load.dref.n.value = getelementptr inbounds %Node, %Node* %dref.n2, i32 0, i32 0
  %load.load.dref.n.value = load i32, i32* %load.dref.n.value, align 4
  %"printf(str,load.load.dref.n.value)" = call i32 (i8*, ...) @printf(i8* %dst, i32 %load.load.dref.n.value)
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Node, align 8
  %b = alloca %Node, align 8
  %c = alloca %Node, align 8
  %s = alloca [2 x i8], align 1
  store %Node zeroinitializer, %Node* %a, align 8
  store %Node zeroinitializer, %Node* %b, align 8
  store %Node zeroinitializer, %Node* %c, align 8
  %load.a.value = getelementptr inbounds %Node, %Node* %a, i32 0, i32 0
  store i32 1, i32* %load.a.value, align 4
  %load.b.value = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  store i32 2, i32* %load.b.value, align 4
  %load.c.value = getelementptr inbounds %Node, %Node* %c, i32 0, i32 0
  store i32 3, i32* %load.c.value, align 4
  %load.a.next = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  store %Node* %b, %Node** %load.a.next, align 8
  %load.b.next = getelementptr inbounds %Node, %Node* %b, i32 0, i32 1
  store %Node* %c, %Node** %load.b.next, align 8
  call void @print_rev(%Node* %a)
  %dst = getelementptr inbounds [2 x i8], [2 x i8]* %s, i32 0, i32 0
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 1 %dst, i8* align 1 getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i64 2, i1 false)
  %"printf(str)" = call i32 (i8*, ...) @printf(i8* %dst)
  %load.a.value1 = getelementptr inbounds %Node, %Node* %a, i32 0, i32 0
  %load.load.a.value = load i32, i32* %load.a.value1, align 4
  %load.b.value2 = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  %load.load.b.value = load i32, i32* %load.b.value2, align 4
  %ADD = add i32 %load.load.a.value, %load.load.b.value
  %load.c.value3 = getelementptr inbounds %Node, %Node* %c, i32 0, i32 0
  %load.load.c.value = load i32, i32* %load.c.value3, align 4
  %ADD4 = add i32 %ADD, %load.load.c.value
  ret i32 %ADD4
}

; Function Attrs: argmemonly nofree nosync nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #0

attributes #0 = { argmemonly nofree nosync nounwind willreturn }
```

---

## 053 — crazy: sort structs in an array by an attribute

```ura
// stdout: abc
// expect: 123
proto printf(fmt i8[], ...) i32

struct Item:
    key i32
    tag char

fn main() i32:
    a Item
    b Item
    c Item
    a.key = 3
    a.tag = 'c'
    b.key = 1
    b.tag = 'a'
    c.key = 2
    c.tag = 'b'
    items Item[] = [a, b, c]
    i i32 = 0
    while i < 3:
        j i32 = i + 1
        while j < 3:
            if items[j].key < items[i].key:
                t Item = items[i]
                items[i] = items[j]
                items[j] = t
            j += 1
        i += 1
    printf("%c%c%c\n", items[0].tag, items[1].tag, items[2].tag)
    return items[0].key * 100 + items[1].key * 10 + items[2].key
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Item = type { i32, i8 }

@str = private unnamed_addr constant [8 x i8] c"%c%c%c\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %a = alloca %Item, align 8
  %b = alloca %Item, align 8
  %c = alloca %Item, align 8
  %items = alloca %Item*, align 8
  %arr = alloca [3 x %Item], align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %t = alloca %Item, align 8
  store %Item zeroinitializer, %Item* %a, align 4
  store %Item zeroinitializer, %Item* %b, align 4
  store %Item zeroinitializer, %Item* %c, align 4
  %a.key = getelementptr inbounds %Item, %Item* %a, i32 0, i32 0
  store i32 3, i32* %a.key, align 4
  %a.tag = getelementptr inbounds %Item, %Item* %a, i32 0, i32 1
  store i8 99, i8* %a.tag, align 1
  %b.key = getelementptr inbounds %Item, %Item* %b, i32 0, i32 0
  store i32 1, i32* %b.key, align 4
  %b.tag = getelementptr inbounds %Item, %Item* %b, i32 0, i32 1
  store i8 97, i8* %b.tag, align 1
  %c.key = getelementptr inbounds %Item, %Item* %c, i32 0, i32 0
  store i32 2, i32* %c.key, align 4
  %c.tag = getelementptr inbounds %Item, %Item* %c, i32 0, i32 1
  store i8 98, i8* %c.tag, align 1
  %"$a" = load %Item, %Item* %a, align 4
  %"$b" = load %Item, %Item* %b, align 4
  %"$c" = load %Item, %Item* %c, align 4
  %0 = insertvalue [3 x %Item] undef, %Item %"$a", 0
  %1 = insertvalue [3 x %Item] %0, %Item %"$b", 1
  %2 = insertvalue [3 x %Item] %1, %Item %"$c", 2
  store [3 x %Item] %2, [3 x %Item]* %arr, align 4
  %3 = getelementptr inbounds [3 x %Item], [3 x %Item]* %arr, i32 0, i32 0
  store %Item* %3, %Item** %items, align 8
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %endloop, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while0.then, label %endloop1

while0.then:                                      ; preds = %while0
  %"$i2" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i2", 1
  store i32 %ADD, i32* %j, align 4
  br label %while1

while1:                                           ; preds = %endif, %while0.then
  %"$j" = load i32, i32* %j, align 4
  %LT3 = icmp slt i32 %"$j", 3
  br i1 %LT3, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  br label %if2

if2:                                              ; preds = %while1.then
  %"$items" = load %Item*, %Item** %items, align 8
  %"$j4" = load i32, i32* %j, align 4
  %"items[]" = getelementptr inbounds %Item, %Item* %"$items", i32 %"$j4"
  %"items[].key" = getelementptr inbounds %Item, %Item* %"items[]", i32 0, i32 0
  %"$items5" = load %Item*, %Item** %items, align 8
  %"$i6" = load i32, i32* %i, align 4
  %"items[]7" = getelementptr inbounds %Item, %Item* %"$items5", i32 %"$i6"
  %"items[].key8" = getelementptr inbounds %Item, %Item* %"items[]7", i32 0, i32 0
  %"$items[].key" = load i32, i32* %"items[].key", align 4
  %"$items[].key9" = load i32, i32* %"items[].key8", align 4
  %LT10 = icmp slt i32 %"$items[].key", %"$items[].key9"
  br i1 %LT10, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"$items11" = load %Item*, %Item** %items, align 8
  %"$i12" = load i32, i32* %i, align 4
  %"items[]13" = getelementptr inbounds %Item, %Item* %"$items11", i32 %"$i12"
  %"$items[]" = load %Item, %Item* %"items[]13", align 4
  store %Item %"$items[]", %Item* %t, align 4
  %"$items14" = load %Item*, %Item** %items, align 8
  %"$i15" = load i32, i32* %i, align 4
  %"items[]16" = getelementptr inbounds %Item, %Item* %"$items14", i32 %"$i15"
  %"$items17" = load %Item*, %Item** %items, align 8
  %"$j18" = load i32, i32* %j, align 4
  %"items[]19" = getelementptr inbounds %Item, %Item* %"$items17", i32 %"$j18"
  %"$items[]20" = load %Item, %Item* %"items[]19", align 4
  store %Item %"$items[]20", %Item* %"items[]16", align 4
  %"$items21" = load %Item*, %Item** %items, align 8
  %"$j22" = load i32, i32* %j, align 4
  %"items[]23" = getelementptr inbounds %Item, %Item* %"$items21", i32 %"$j22"
  %"$t" = load %Item, %Item* %t, align 4
  store %Item %"$t", %Item* %"items[]23", align 4
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  %"$j24" = load i32, i32* %j, align 4
  %ADD25 = add i32 %"$j24", 1
  store i32 %ADD25, i32* %j, align 4
  br label %while1

endloop:                                          ; preds = %while1
  %"$i26" = load i32, i32* %i, align 4
  %ADD27 = add i32 %"$i26", 1
  store i32 %ADD27, i32* %i, align 4
  br label %while0

endloop1:                                         ; preds = %while0
  %"$items28" = load %Item*, %Item** %items, align 8
  %"items[]29" = getelementptr inbounds %Item, %Item* %"$items28", i32 0
  %"items[].tag" = getelementptr inbounds %Item, %Item* %"items[]29", i32 0, i32 1
  %"$items30" = load %Item*, %Item** %items, align 8
  %"items[]31" = getelementptr inbounds %Item, %Item* %"$items30", i32 1
  %"items[].tag32" = getelementptr inbounds %Item, %Item* %"items[]31", i32 0, i32 1
  %"$items33" = load %Item*, %Item** %items, align 8
  %"items[]34" = getelementptr inbounds %Item, %Item* %"$items33", i32 2
  %"items[].tag35" = getelementptr inbounds %Item, %Item* %"items[]34", i32 0, i32 1
  %"$items[].tag" = load i8, i8* %"items[].tag", align 1
  %"$items[].tag36" = load i8, i8* %"items[].tag32", align 1
  %"$items[].tag37" = load i8, i8* %"items[].tag35", align 1
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str, i32 0, i32 0), i8 %"$items[].tag", i8 %"$items[].tag36", i8 %"$items[].tag37")
  %"$items38" = load %Item*, %Item** %items, align 8
  %"items[]39" = getelementptr inbounds %Item, %Item* %"$items38", i32 0
  %"items[].key40" = getelementptr inbounds %Item, %Item* %"items[]39", i32 0, i32 0
  %"$items[].key41" = load i32, i32* %"items[].key40", align 4
  %MUL = mul i32 %"$items[].key41", 100
  %"$items42" = load %Item*, %Item** %items, align 8
  %"items[]43" = getelementptr inbounds %Item, %Item* %"$items42", i32 1
  %"items[].key44" = getelementptr inbounds %Item, %Item* %"items[]43", i32 0, i32 0
  %"$items[].key45" = load i32, i32* %"items[].key44", align 4
  %MUL46 = mul i32 %"$items[].key45", 10
  %ADD47 = add i32 %MUL, %MUL46
  %"$items48" = load %Item*, %Item** %items, align 8
  %"items[]49" = getelementptr inbounds %Item, %Item* %"$items48", i32 2
  %"items[].key50" = getelementptr inbounds %Item, %Item* %"items[]49", i32 0, i32 0
  %"$items[].key51" = load i32, i32* %"items[].key50", align 4
  %ADD52 = add i32 %ADD47, %"$items[].key51"
  ret i32 %ADD52
}
```

---

## 054 — crazy: ping pong: two structs that name each other, before either is declared

```ura
// expect: 33
struct Ping:
    hits i32

    fn hit(o &Pong, n i32):
        self.hits += 1
        if n > 0: o.hit(&self, n - 1)

struct Pong:
    hits i32

    fn hit(o &Ping, n i32):
        self.hits += 1
        if n > 0: o.hit(&self, n - 1)

fn main() i32:
    a Ping
    b Pong
    a.hit(&b, 5)
    return a.hits * 10 + b.hits
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Ping = type { i32 }
%Pong = type { i32 }

define void @Ping.hit(%Ping* %0, %Pong* %1, i32 %2) {
entry:
  %self = alloca %Ping*, align 8
  %o = alloca %Pong*, align 8
  %n = alloca i32, align 4
  store %Ping* %0, %Ping** %self, align 8
  store %Pong* %1, %Pong** %o, align 8
  store i32 %2, i32* %n, align 4
  %"*self" = load %Ping*, %Ping** %self, align 8
  %"*self.hits" = getelementptr inbounds %Ping, %Ping* %"*self", i32 0, i32 0
  %"$*self.hits" = load i32, i32* %"*self.hits", align 4
  %ADD = add i32 %"$*self.hits", 1
  store i32 %ADD, i32* %"*self.hits", align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %GT = icmp sgt i32 %"$n", 0
  br i1 %GT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*o" = load %Pong*, %Pong** %o, align 8
  %"*self1" = load %Ping*, %Ping** %self, align 8
  %"$n2" = load i32, i32* %n, align 4
  %SUB = sub i32 %"$n2", 1
  call void @Pong.hit(%Pong* %"*o", %Ping* %"*self1", i32 %SUB)
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  ret void
}

define void @Pong.hit(%Pong* %0, %Ping* %1, i32 %2) {
entry:
  %self = alloca %Pong*, align 8
  %o = alloca %Ping*, align 8
  %n = alloca i32, align 4
  store %Pong* %0, %Pong** %self, align 8
  store %Ping* %1, %Ping** %o, align 8
  store i32 %2, i32* %n, align 4
  %"*self" = load %Pong*, %Pong** %self, align 8
  %"*self.hits" = getelementptr inbounds %Pong, %Pong* %"*self", i32 0, i32 0
  %"$*self.hits" = load i32, i32* %"*self.hits", align 4
  %ADD = add i32 %"$*self.hits", 1
  store i32 %ADD, i32* %"*self.hits", align 4
  br label %if1

if1:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %GT = icmp sgt i32 %"$n", 0
  br i1 %GT, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  %"*o" = load %Ping*, %Ping** %o, align 8
  %"*self1" = load %Pong*, %Pong** %self, align 8
  %"$n2" = load i32, i32* %n, align 4
  %SUB = sub i32 %"$n2", 1
  call void @Ping.hit(%Ping* %"*o", %Pong* %"*self1", i32 %SUB)
  br label %endif

endif:                                            ; preds = %if1.then, %if1
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Ping, align 8
  %b = alloca %Pong, align 8
  store %Ping zeroinitializer, %Ping* %a, align 4
  store %Pong zeroinitializer, %Pong* %b, align 4
  call void @Ping.hit(%Ping* %a, %Pong* %b, i32 5)
  %a.hits = getelementptr inbounds %Ping, %Ping* %a, i32 0, i32 0
  %"$a.hits" = load i32, i32* %a.hits, align 4
  %MUL = mul i32 %"$a.hits", 10
  %b.hits = getelementptr inbounds %Pong, %Pong* %b, i32 0, i32 0
  %"$b.hits" = load i32, i32* %b.hits, align 4
  %ADD = add i32 %MUL, %"$b.hits"
  ret i32 %ADD
}
```

---

## 055 — crazy: while True with break and return inside a method

```ura
// expect: 40   (23 at 4 * 10, 4 at 0, 99 missing: 0)
struct Finder:
    data i32[]
    size i32

    fn index_of(v i32) i32:
        i i32 = 0
        while True:
            if i == self.size: break
            if self.data[i] == v: return i
            i += 1
        return -1

fn main() i32:
    f Finder
    f.data = [4, 8, 15, 16, 23, 42]
    f.size = 6
    return f.index_of(23) * 10 + f.index_of(4) + (f.index_of(99) + 1) * 100
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Finder = type { i32*, i32 }

define i32 @Finder.index_of(%Finder* %0, i32 %1) {
entry:
  %self = alloca %Finder*, align 8
  %v = alloca i32, align 4
  %i = alloca i32, align 4
  store %Finder* %0, %Finder** %self, align 8
  store i32 %1, i32* %v, align 4
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %endif1, %entry
  br i1 true, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  br label %if1

if1:                                              ; preds = %while0.then
  %"*self" = load %Finder*, %Finder** %self, align 8
  %"*self.size" = getelementptr inbounds %Finder, %Finder* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.size" = load i32, i32* %"*self.size", align 4
  %EQ = icmp eq i32 %"$i", %"$*self.size"
  br i1 %EQ, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  br label %endloop

endif:                                            ; preds = %if1
  br label %if2

if2:                                              ; preds = %endif
  %"*self2" = load %Finder*, %Finder** %self, align 8
  %"*self.data" = getelementptr inbounds %Finder, %Finder* %"*self2", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$i3" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$i3"
  %"$*self.data[]" = load i32, i32* %"*self.data[]", align 4
  %"$v" = load i32, i32* %v, align 4
  %EQ4 = icmp eq i32 %"$*self.data[]", %"$v"
  br i1 %EQ4, label %if2.then, label %endif1

if2.then:                                         ; preds = %if2
  %"$i5" = load i32, i32* %i, align 4
  ret i32 %"$i5"

endif1:                                           ; preds = %if2
  %"$i6" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i6", 1
  store i32 %ADD, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %if1.then, %while0
  ret i32 -1
}

define i32 @main() {
entry:
  %f = alloca %Finder, align 8
  %arr = alloca [6 x i32], align 4
  store %Finder zeroinitializer, %Finder* %f, align 8
  %f.data = getelementptr inbounds %Finder, %Finder* %f, i32 0, i32 0
  store [6 x i32] [i32 4, i32 8, i32 15, i32 16, i32 23, i32 42], [6 x i32]* %arr, align 4
  %0 = getelementptr inbounds [6 x i32], [6 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %f.data, align 8
  %f.size = getelementptr inbounds %Finder, %Finder* %f, i32 0, i32 1
  store i32 6, i32* %f.size, align 4
  %call.Finder.index_of = call i32 @Finder.index_of(%Finder* %f, i32 23)
  %MUL = mul i32 %call.Finder.index_of, 10
  %call.Finder.index_of1 = call i32 @Finder.index_of(%Finder* %f, i32 4)
  %ADD = add i32 %MUL, %call.Finder.index_of1
  %call.Finder.index_of2 = call i32 @Finder.index_of(%Finder* %f, i32 99)
  %ADD3 = add i32 %call.Finder.index_of2, 1
  %MUL4 = mul i32 %ADD3, 100
  %ADD5 = add i32 %ADD, %MUL4
  ret i32 %ADD5
}
```

---

## 056 — crazy: script: a global stack evaluates postfix 3 4 + 2 *

```ura
// expect: 14
struct Stack:
    items i32[]
    top i32

stack Stack
stack.items = [0, 0, 0, 0, 0, 0, 0, 0]

fn push(v i32):
    stack.items[stack.top] = v
    stack.top += 1

fn pop() i32:
    stack.top -= 1
    return stack.items[stack.top]

push(3)
push(4)
push(pop() + pop())
push(2)
push(pop() * pop())
return pop()
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Stack = type { i32*, i32 }

@stack = global %Stack zeroinitializer

define void @push(i32 %0) {
entry:
  %v = alloca i32, align 4
  store i32 %0, i32* %v, align 4
  %"$stack.items" = load i32*, i32** getelementptr inbounds (%Stack, %Stack* @stack, i32 0, i32 0), align 8
  %"$stack.top" = load i32, i32* getelementptr inbounds (%Stack, %Stack* @stack, i32 0, i32 1), align 4
  %"stack.items[]" = getelementptr inbounds i32, i32* %"$stack.items", i32 %"$stack.top"
  %"$v" = load i32, i32* %v, align 4
  store i32 %"$v", i32* %"stack.items[]", align 4
  %"$stack.top1" = load i32, i32* getelementptr inbounds (%Stack, %Stack* @stack, i32 0, i32 1), align 4
  %ADD = add i32 %"$stack.top1", 1
  store i32 %ADD, i32* getelementptr inbounds (%Stack, %Stack* @stack, i32 0, i32 1), align 4
  ret void
}

define i32 @pop() {
entry:
  %"$stack.top" = load i32, i32* getelementptr inbounds (%Stack, %Stack* @stack, i32 0, i32 1), align 4
  %SUB = sub i32 %"$stack.top", 1
  store i32 %SUB, i32* getelementptr inbounds (%Stack, %Stack* @stack, i32 0, i32 1), align 4
  %"$stack.items" = load i32*, i32** getelementptr inbounds (%Stack, %Stack* @stack, i32 0, i32 0), align 8
  %"$stack.top1" = load i32, i32* getelementptr inbounds (%Stack, %Stack* @stack, i32 0, i32 1), align 4
  %"stack.items[]" = getelementptr inbounds i32, i32* %"$stack.items", i32 %"$stack.top1"
  %"$stack.items[]" = load i32, i32* %"stack.items[]", align 4
  ret i32 %"$stack.items[]"
}

define i32 @main() {
entry:
  %arr = alloca [8 x i32], align 4
  store [8 x i32] zeroinitializer, [8 x i32]* %arr, align 4
  %0 = getelementptr inbounds [8 x i32], [8 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** getelementptr inbounds (%Stack, %Stack* @stack, i32 0, i32 0), align 8
  call void @push(i32 3)
  call void @push(i32 4)
  %call.pop = call i32 @pop()
  %call.pop1 = call i32 @pop()
  %ADD = add i32 %call.pop, %call.pop1
  call void @push(i32 %ADD)
  call void @push(i32 2)
  %call.pop2 = call i32 @pop()
  %call.pop3 = call i32 @pop()
  %MUL = mul i32 %call.pop2, %call.pop3
  call void @push(i32 %MUL)
  %call.pop4 = call i32 @pop()
  ret i32 %call.pop4
}
```

---

## 057 — crazy: structs used before declared, a struct local to a function

```ura
// expect: 203   (area 5 * 4 = 20 * 10, plus 3)
origin Point
origin.x = 2

fn area(r &Rect) i32:
    return (r.b.x - r.a.x) * (r.b.y - r.a.y)

fn make() i32:
    struct Local:
        v i32
    l Local
    l.v = 3
    return l.v

struct Rect:
    a Point
    b Point

struct Point:
    x i32
    y i32

r Rect
r.a = origin
r.b.x = 7
r.b.y = 4
return area(&r) * 10 + make()
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Point = type { i32, i32 }
%Rect = type { %Point, %Point }
%Local = type { i32 }

@origin = global %Point zeroinitializer
@r = global %Rect zeroinitializer

define i32 @area(%Rect* %0) {
entry:
  %r = alloca %Rect*, align 8
  store %Rect* %0, %Rect** %r, align 8
  %"*r" = load %Rect*, %Rect** %r, align 8
  %"*r.b" = getelementptr inbounds %Rect, %Rect* %"*r", i32 0, i32 1
  %"*r.b.x" = getelementptr inbounds %Point, %Point* %"*r.b", i32 0, i32 0
  %"*r1" = load %Rect*, %Rect** %r, align 8
  %"*r.a" = getelementptr inbounds %Rect, %Rect* %"*r1", i32 0, i32 0
  %"*r.a.x" = getelementptr inbounds %Point, %Point* %"*r.a", i32 0, i32 0
  %"$*r.b.x" = load i32, i32* %"*r.b.x", align 4
  %"$*r.a.x" = load i32, i32* %"*r.a.x", align 4
  %SUB = sub i32 %"$*r.b.x", %"$*r.a.x"
  %"*r2" = load %Rect*, %Rect** %r, align 8
  %"*r.b3" = getelementptr inbounds %Rect, %Rect* %"*r2", i32 0, i32 1
  %"*r.b.y" = getelementptr inbounds %Point, %Point* %"*r.b3", i32 0, i32 1
  %"*r4" = load %Rect*, %Rect** %r, align 8
  %"*r.a5" = getelementptr inbounds %Rect, %Rect* %"*r4", i32 0, i32 0
  %"*r.a.y" = getelementptr inbounds %Point, %Point* %"*r.a5", i32 0, i32 1
  %"$*r.b.y" = load i32, i32* %"*r.b.y", align 4
  %"$*r.a.y" = load i32, i32* %"*r.a.y", align 4
  %SUB6 = sub i32 %"$*r.b.y", %"$*r.a.y"
  %MUL = mul i32 %SUB, %SUB6
  ret i32 %MUL
}

define i32 @make() {
entry:
  %l = alloca %Local, align 8
  store %Local zeroinitializer, %Local* %l, align 4
  %l.v = getelementptr inbounds %Local, %Local* %l, i32 0, i32 0
  store i32 3, i32* %l.v, align 4
  %l.v1 = getelementptr inbounds %Local, %Local* %l, i32 0, i32 0
  %"$l.v" = load i32, i32* %l.v1, align 4
  ret i32 %"$l.v"
}

define i32 @main() {
entry:
  store i32 2, i32* getelementptr inbounds (%Point, %Point* @origin, i32 0, i32 0), align 4
  %"$origin" = load %Point, %Point* @origin, align 4
  store %Point %"$origin", %Point* getelementptr inbounds (%Rect, %Rect* @r, i32 0, i32 0), align 4
  store i32 7, i32* getelementptr inbounds (%Rect, %Rect* @r, i32 0, i32 1, i32 0), align 4
  store i32 4, i32* getelementptr inbounds (%Rect, %Rect* @r, i32 0, i32 1, i32 1), align 4
  %call.area = call i32 @area(%Rect* @r)
  %MUL = mul i32 %call.area, 10
  %call.make = call i32 @make()
  %ADD = add i32 %MUL, %call.make
  ret i32 %ADD
}
```

---

## 058 — crazy: a linked list built with chained methods that return refs

```ura
// expect: 44   (length 4 * 10, last value 4)
struct Node:
    value i32
    next &Node

    fn attach(n &Node) &Node:
        self.next = &n
        return &n

    fn length() i32:
        n i32 = 1
        p &Node = &self
        while not isNullable(p.next):
            n += 1
            p = &p.next
        return n

a Node
b Node
c Node
d Node
d.value = 4
a.attach(&b).attach(&c).attach(&d)
return a.length() * 10 + a.next.next.next.value
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Node = type { i32, %Node* }

@a = global %Node zeroinitializer
@b = global %Node zeroinitializer
@c = global %Node zeroinitializer
@d = global %Node zeroinitializer

define %Node* @Node.link(%Node* %0, %Node* %1) {
entry:
  %self = alloca %Node*, align 8
  %n = alloca %Node*, align 8
  store %Node* %0, %Node** %self, align 8
  store %Node* %1, %Node** %n, align 8
  %dref.self = load %Node*, %Node** %self, align 8
  %load.dref.self.next = getelementptr inbounds %Node, %Node* %dref.self, i32 0, i32 1
  %dref.n = load %Node*, %Node** %n, align 8
  store %Node* %dref.n, %Node** %load.dref.self.next, align 8
  %dref.n1 = load %Node*, %Node** %n, align 8
  ret %Node* %dref.n1
}

define i32 @Node.length(%Node* %0) {
entry:
  %self = alloca %Node*, align 8
  %n = alloca i32, align 4
  %p = alloca %Node*, align 8
  store %Node* %0, %Node** %self, align 8
  store i32 1, i32* %n, align 4
  %dref.self = load %Node*, %Node** %self, align 8
  store %Node* %dref.self, %Node** %p, align 8
  br label %cond

cond:                                             ; preds = %then, %entry
  %dref.p = load %Node*, %Node** %p, align 8
  %load.dref.p.next = getelementptr inbounds %Node, %Node* %dref.p, i32 0, i32 1
  %dref.load.dref.p.next = load %Node*, %Node** %load.dref.p.next, align 8
  %"isNullable(dref.load.dref.p.next)" = icmp eq %Node* %dref.load.dref.p.next, null
  %NOT = xor i1 %"isNullable(dref.load.dref.p.next)", true
  br i1 %NOT, label %then, label %endwhile

then:                                             ; preds = %cond
  %old = load i32, i32* %n, align 4
  %ADD_ASSIGN = add i32 %old, 1
  store i32 %ADD_ASSIGN, i32* %n, align 4
  %dref.p1 = load %Node*, %Node** %p, align 8
  %load.dref.p.next2 = getelementptr inbounds %Node, %Node* %dref.p1, i32 0, i32 1
  %dref.load.dref.p.next3 = load %Node*, %Node** %load.dref.p.next2, align 8
  store %Node* %dref.load.dref.p.next3, %Node** %p, align 8
  br label %cond

endwhile:                                         ; preds = %cond
  %load.n = load i32, i32* %n, align 4
  ret i32 %load.n
}

define i32 @main() {
entry:
  store %Node zeroinitializer, %Node* @a, align 8
  store %Node zeroinitializer, %Node* @b, align 8
  store %Node zeroinitializer, %Node* @c, align 8
  store %Node zeroinitializer, %Node* @d, align 8
  store i32 4, i32* getelementptr inbounds (%Node, %Node* @d, i32 0, i32 0), align 4
  %"link(&load.a,&b)" = call %Node* @Node.link(%Node* @a, %Node* @b)
  %"link(link(&load.a,&b),&c)" = call %Node* @Node.link(%Node* %"link(&load.a,&b)", %Node* @c)
  %"link(link(link(&load.a,&b),&c),&d)" = call %Node* @Node.link(%Node* %"link(link(&load.a,&b),&c)", %Node* @d)
  %"length(&load.a)" = call i32 @Node.length(%Node* @a)
  %MUL = mul i32 %"length(&load.a)", 10
  %dref.load.a.next = load %Node*, %Node** getelementptr inbounds (%Node, %Node* @a, i32 0, i32 1), align 8
  %load.dref.load.a.next.next = getelementptr inbounds %Node, %Node* %dref.load.a.next, i32 0, i32 1
  %dref.load.dref.load.a.next.next = load %Node*, %Node** %load.dref.load.a.next.next, align 8
  %load.dref.load.dref.load.a.next.next.next = getelementptr inbounds %Node, %Node* %dref.load.dref.load.a.next.next, i32 0, i32 1
  %dref.load.dref.load.dref.load.a.next.next.next = load %Node*, %Node** %load.dref.load.dref.load.a.next.next.next, align 8
  %load.dref.load.dref.load.dref.load.a.next.next.next.value = getelementptr inbounds %Node, %Node* %dref.load.dref.load.dref.load.a.next.next.next, i32 0, i32 0
  %load.load.dref.load.dref.load.dref.load.a.next.next.next.value = load i32, i32* %load.dref.load.dref.load.dref.load.a.next.next.next.value, align 4
  %ADD = add i32 %MUL, %load.load.dref.load.dref.load.dref.load.a.next.next.next.value
  ret i32 %ADD
}
```

---

## 059 — crazy: binary search tree: recursive methods through ref attributes

```ura
// expect: 213   (sum 210, depth 3)
struct Tree:
    value i32
    left &Tree
    right &Tree

    fn insert(t &Tree):
        if t.value < self.value:
            if isNullable(self.left): self.left = &t
            else: self.left.insert(&t)
        else:
            if isNullable(self.right): self.right = &t
            else: self.right.insert(&t)

    fn sum() i32:
        s i32 = self.value
        if not isNullable(self.left): s += self.left.sum()
        if not isNullable(self.right): s += self.right.sum()
        return s

    fn depth() i32:
        l i32 = 0
        r i32 = 0
        if not isNullable(self.left): l = self.left.depth()
        if not isNullable(self.right): r = self.right.depth()
        if l > r: return l + 1
        return r + 1

a Tree
b Tree
c Tree
d Tree
e Tree
a.value = 50
b.value = 30
c.value = 70
d.value = 20
e.value = 40
a.insert(&b)
a.insert(&c)
a.insert(&d)
a.insert(&e)
return a.sum() + a.depth()
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Tree = type { i32, %Tree*, %Tree* }

@a = global %Tree zeroinitializer
@b = global %Tree zeroinitializer
@c = global %Tree zeroinitializer
@d = global %Tree zeroinitializer
@e = global %Tree zeroinitializer

define void @Tree.insert(%Tree* %0, %Tree* %1) {
entry:
  %self = alloca %Tree*, align 8
  %t = alloca %Tree*, align 8
  store %Tree* %0, %Tree** %self, align 8
  store %Tree* %1, %Tree** %t, align 8
  %dref.t = load %Tree*, %Tree** %t, align 8
  %load.dref.t.value = getelementptr inbounds %Tree, %Tree* %dref.t, i32 0, i32 0
  %load.load.dref.t.value = load i32, i32* %load.dref.t.value, align 4
  %dref.self = load %Tree*, %Tree** %self, align 8
  %load.dref.self.value = getelementptr inbounds %Tree, %Tree* %dref.self, i32 0, i32 0
  %load.load.dref.self.value = load i32, i32* %load.dref.self.value, align 4
  %LT = icmp slt i32 %load.load.dref.t.value, %load.load.dref.self.value
  br i1 %LT, label %then, label %next

then:                                             ; preds = %entry
  %dref.self4 = load %Tree*, %Tree** %self, align 8
  %load.dref.self.left = getelementptr inbounds %Tree, %Tree* %dref.self4, i32 0, i32 1
  %dref.load.dref.self.left = load %Tree*, %Tree** %load.dref.self.left, align 8
  %"isNullable(dref.load.dref.self.left)" = icmp eq %Tree* %dref.load.dref.self.left, null
  br i1 %"isNullable(dref.load.dref.self.left)", label %then2, label %next3

next:                                             ; preds = %entry
  %dref.self15 = load %Tree*, %Tree** %self, align 8
  %load.dref.self.right = getelementptr inbounds %Tree, %Tree* %dref.self15, i32 0, i32 2
  %dref.load.dref.self.right = load %Tree*, %Tree** %load.dref.self.right, align 8
  %"isNullable(dref.load.dref.self.right)" = icmp eq %Tree* %dref.load.dref.self.right, null
  br i1 %"isNullable(dref.load.dref.self.right)", label %then13, label %next14

then2:                                            ; preds = %then
  %dref.self5 = load %Tree*, %Tree** %self, align 8
  %load.dref.self.left6 = getelementptr inbounds %Tree, %Tree* %dref.self5, i32 0, i32 1
  %dref.t7 = load %Tree*, %Tree** %t, align 8
  store %Tree* %dref.t7, %Tree** %load.dref.self.left6, align 8
  br label %endif1

next3:                                            ; preds = %then
  %dref.self8 = load %Tree*, %Tree** %self, align 8
  %load.dref.self.left9 = getelementptr inbounds %Tree, %Tree* %dref.self8, i32 0, i32 1
  %dref.load.dref.self.left10 = load %Tree*, %Tree** %load.dref.self.left9, align 8
  %dref.t11 = load %Tree*, %Tree** %t, align 8
  call void @Tree.insert(%Tree* %dref.load.dref.self.left10, %Tree* %dref.t11)
  br label %endif1

endif1:                                           ; preds = %next3, %then2
  br label %endif

then13:                                           ; preds = %next
  %dref.self16 = load %Tree*, %Tree** %self, align 8
  %load.dref.self.right17 = getelementptr inbounds %Tree, %Tree* %dref.self16, i32 0, i32 2
  %dref.t18 = load %Tree*, %Tree** %t, align 8
  store %Tree* %dref.t18, %Tree** %load.dref.self.right17, align 8
  br label %endif12

next14:                                           ; preds = %next
  %dref.self19 = load %Tree*, %Tree** %self, align 8
  %load.dref.self.right20 = getelementptr inbounds %Tree, %Tree* %dref.self19, i32 0, i32 2
  %dref.load.dref.self.right21 = load %Tree*, %Tree** %load.dref.self.right20, align 8
  %dref.t22 = load %Tree*, %Tree** %t, align 8
  call void @Tree.insert(%Tree* %dref.load.dref.self.right21, %Tree* %dref.t22)
  br label %endif12

endif12:                                          ; preds = %next14, %then13
  br label %endif

endif:                                            ; preds = %endif12, %endif1
  ret void
}

define i32 @Tree.sum(%Tree* %0) {
entry:
  %self = alloca %Tree*, align 8
  %s = alloca i32, align 4
  store %Tree* %0, %Tree** %self, align 8
  %dref.self = load %Tree*, %Tree** %self, align 8
  %load.dref.self.value = getelementptr inbounds %Tree, %Tree* %dref.self, i32 0, i32 0
  %load.load.dref.self.value = load i32, i32* %load.dref.self.value, align 4
  store i32 %load.load.dref.self.value, i32* %s, align 4
  %dref.self1 = load %Tree*, %Tree** %self, align 8
  %load.dref.self.left = getelementptr inbounds %Tree, %Tree* %dref.self1, i32 0, i32 1
  %dref.load.dref.self.left = load %Tree*, %Tree** %load.dref.self.left, align 8
  %"isNullable(dref.load.dref.self.left)" = icmp eq %Tree* %dref.load.dref.self.left, null
  %NOT = xor i1 %"isNullable(dref.load.dref.self.left)", true
  br i1 %NOT, label %then, label %endif

then:                                             ; preds = %entry
  %dref.self2 = load %Tree*, %Tree** %self, align 8
  %load.dref.self.left3 = getelementptr inbounds %Tree, %Tree* %dref.self2, i32 0, i32 1
  %dref.load.dref.self.left4 = load %Tree*, %Tree** %load.dref.self.left3, align 8
  %"sum(&load.dref.load.dref.self.left)" = call i32 @Tree.sum(%Tree* %dref.load.dref.self.left4)
  %old = load i32, i32* %s, align 4
  %ADD_ASSIGN = add i32 %old, %"sum(&load.dref.load.dref.self.left)"
  store i32 %ADD_ASSIGN, i32* %s, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %dref.self7 = load %Tree*, %Tree** %self, align 8
  %load.dref.self.right = getelementptr inbounds %Tree, %Tree* %dref.self7, i32 0, i32 2
  %dref.load.dref.self.right = load %Tree*, %Tree** %load.dref.self.right, align 8
  %"isNullable(dref.load.dref.self.right)" = icmp eq %Tree* %dref.load.dref.self.right, null
  %NOT8 = xor i1 %"isNullable(dref.load.dref.self.right)", true
  br i1 %NOT8, label %then6, label %endif5

then6:                                            ; preds = %endif
  %dref.self9 = load %Tree*, %Tree** %self, align 8
  %load.dref.self.right10 = getelementptr inbounds %Tree, %Tree* %dref.self9, i32 0, i32 2
  %dref.load.dref.self.right11 = load %Tree*, %Tree** %load.dref.self.right10, align 8
  %"sum(&load.dref.load.dref.self.right)" = call i32 @Tree.sum(%Tree* %dref.load.dref.self.right11)
  %old12 = load i32, i32* %s, align 4
  %ADD_ASSIGN13 = add i32 %old12, %"sum(&load.dref.load.dref.self.right)"
  store i32 %ADD_ASSIGN13, i32* %s, align 4
  br label %endif5

endif5:                                           ; preds = %then6, %endif
  %load.s = load i32, i32* %s, align 4
  ret i32 %load.s
}

define i32 @Tree.depth(%Tree* %0) {
entry:
  %self = alloca %Tree*, align 8
  %l = alloca i32, align 4
  %r = alloca i32, align 4
  store %Tree* %0, %Tree** %self, align 8
  store i32 0, i32* %l, align 4
  store i32 0, i32* %r, align 4
  %dref.self = load %Tree*, %Tree** %self, align 8
  %load.dref.self.left = getelementptr inbounds %Tree, %Tree* %dref.self, i32 0, i32 1
  %dref.load.dref.self.left = load %Tree*, %Tree** %load.dref.self.left, align 8
  %"isNullable(dref.load.dref.self.left)" = icmp eq %Tree* %dref.load.dref.self.left, null
  %NOT = xor i1 %"isNullable(dref.load.dref.self.left)", true
  br i1 %NOT, label %then, label %endif

then:                                             ; preds = %entry
  %dref.self1 = load %Tree*, %Tree** %self, align 8
  %load.dref.self.left2 = getelementptr inbounds %Tree, %Tree* %dref.self1, i32 0, i32 1
  %dref.load.dref.self.left3 = load %Tree*, %Tree** %load.dref.self.left2, align 8
  %"depth(&load.dref.load.dref.self.left)" = call i32 @Tree.depth(%Tree* %dref.load.dref.self.left3)
  store i32 %"depth(&load.dref.load.dref.self.left)", i32* %l, align 4
  br label %endif

endif:                                            ; preds = %then, %entry
  %dref.self6 = load %Tree*, %Tree** %self, align 8
  %load.dref.self.right = getelementptr inbounds %Tree, %Tree* %dref.self6, i32 0, i32 2
  %dref.load.dref.self.right = load %Tree*, %Tree** %load.dref.self.right, align 8
  %"isNullable(dref.load.dref.self.right)" = icmp eq %Tree* %dref.load.dref.self.right, null
  %NOT7 = xor i1 %"isNullable(dref.load.dref.self.right)", true
  br i1 %NOT7, label %then5, label %endif4

then5:                                            ; preds = %endif
  %dref.self8 = load %Tree*, %Tree** %self, align 8
  %load.dref.self.right9 = getelementptr inbounds %Tree, %Tree* %dref.self8, i32 0, i32 2
  %dref.load.dref.self.right10 = load %Tree*, %Tree** %load.dref.self.right9, align 8
  %"depth(&load.dref.load.dref.self.right)" = call i32 @Tree.depth(%Tree* %dref.load.dref.self.right10)
  store i32 %"depth(&load.dref.load.dref.self.right)", i32* %r, align 4
  br label %endif4

endif4:                                           ; preds = %then5, %endif
  %load.l = load i32, i32* %l, align 4
  %load.r = load i32, i32* %r, align 4
  %GT = icmp sgt i32 %load.l, %load.r
  br i1 %GT, label %then12, label %endif11

then12:                                           ; preds = %endif4
  %load.l13 = load i32, i32* %l, align 4
  %ADD = add i32 %load.l13, 1
  ret i32 %ADD

endif11:                                          ; preds = %endif4
  %load.r14 = load i32, i32* %r, align 4
  %ADD15 = add i32 %load.r14, 1
  ret i32 %ADD15
}

define i32 @main() {
entry:
  store %Tree zeroinitializer, %Tree* @a, align 8
  store %Tree zeroinitializer, %Tree* @b, align 8
  store %Tree zeroinitializer, %Tree* @c, align 8
  store %Tree zeroinitializer, %Tree* @d, align 8
  store %Tree zeroinitializer, %Tree* @e, align 8
  store i32 50, i32* getelementptr inbounds (%Tree, %Tree* @a, i32 0, i32 0), align 4
  store i32 30, i32* getelementptr inbounds (%Tree, %Tree* @b, i32 0, i32 0), align 4
  store i32 70, i32* getelementptr inbounds (%Tree, %Tree* @c, i32 0, i32 0), align 4
  store i32 20, i32* getelementptr inbounds (%Tree, %Tree* @d, i32 0, i32 0), align 4
  store i32 40, i32* getelementptr inbounds (%Tree, %Tree* @e, i32 0, i32 0), align 4
  call void @Tree.insert(%Tree* @a, %Tree* @b)
  call void @Tree.insert(%Tree* @a, %Tree* @c)
  call void @Tree.insert(%Tree* @a, %Tree* @d)
  call void @Tree.insert(%Tree* @a, %Tree* @e)
  %"sum(&load.a)" = call i32 @Tree.sum(%Tree* @a)
  %"depth(&load.a)" = call i32 @Tree.depth(%Tree* @a)
  %ADD = add i32 %"sum(&load.a)", %"depth(&load.a)"
  ret i32 %ADD
}
```

---

## 060 — crazy: 2x2 matrix product with methods and ref parameters

```ura
// expect: 134   (19 + 22 + 43 + 50)
struct Mat:
    v i32[]

    fn at(r i32, c i32) i32:
        return self.v[r * 2 + c]

    fn mul(o &Mat, out &Mat):
        r i32 = 0
        while r < 2:
            c i32 = 0
            while c < 2:
                s i32 = 0
                k i32 = 0
                while k < 2:
                    s += self.at(r, k) * o.at(k, c)
                    k += 1
                out.v[r * 2 + c] = s
                c += 1
            r += 1

a Mat
b Mat
c Mat
a.v = [1, 2, 3, 4]
b.v = [5, 6, 7, 8]
c.v = [0, 0, 0, 0]
a.mul(&b, &c)
return c.v[0] + c.v[1] + c.v[2] + c.v[3]
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Mat = type { i32* }

@a = global %Mat zeroinitializer
@b = global %Mat zeroinitializer
@c = global %Mat zeroinitializer

define i32 @Mat.at(%Mat* %0, i32 %1, i32 %2) {
entry:
  %self = alloca %Mat*, align 8
  %r = alloca i32, align 4
  %c = alloca i32, align 4
  store %Mat* %0, %Mat** %self, align 8
  store i32 %1, i32* %r, align 4
  store i32 %2, i32* %c, align 4
  %"*self" = load %Mat*, %Mat** %self, align 8
  %"*self.v" = getelementptr inbounds %Mat, %Mat* %"*self", i32 0, i32 0
  %"$r" = load i32, i32* %r, align 4
  %MUL = mul i32 %"$r", 2
  %"$c" = load i32, i32* %c, align 4
  %ADD = add i32 %MUL, %"$c"
  %"$*self.v" = load i32*, i32** %"*self.v", align 8
  %"*self.v[]" = getelementptr inbounds i32, i32* %"$*self.v", i32 %ADD
  %"$*self.v[]" = load i32, i32* %"*self.v[]", align 4
  ret i32 %"$*self.v[]"
}

define void @Mat.mul(%Mat* %0, %Mat* %1, %Mat* %2) {
entry:
  %self = alloca %Mat*, align 8
  %o = alloca %Mat*, align 8
  %out = alloca %Mat*, align 8
  %r = alloca i32, align 4
  %c = alloca i32, align 4
  %s = alloca i32, align 4
  %k = alloca i32, align 4
  store %Mat* %0, %Mat** %self, align 8
  store %Mat* %1, %Mat** %o, align 8
  store %Mat* %2, %Mat** %out, align 8
  store i32 0, i32* %r, align 4
  br label %while0

while0:                                           ; preds = %endloop1, %entry
  %"$r" = load i32, i32* %r, align 4
  %LT = icmp slt i32 %"$r", 2
  br i1 %LT, label %while0.then, label %endloop2

while0.then:                                      ; preds = %while0
  store i32 0, i32* %c, align 4
  br label %while1

while1:                                           ; preds = %endloop, %while0.then
  %"$c" = load i32, i32* %c, align 4
  %LT3 = icmp slt i32 %"$c", 2
  br i1 %LT3, label %while1.then, label %endloop1

while1.then:                                      ; preds = %while1
  store i32 0, i32* %s, align 4
  store i32 0, i32* %k, align 4
  br label %while2

while2:                                           ; preds = %while2.then, %while1.then
  %"$k" = load i32, i32* %k, align 4
  %LT4 = icmp slt i32 %"$k", 2
  br i1 %LT4, label %while2.then, label %endloop

while2.then:                                      ; preds = %while2
  %"*self" = load %Mat*, %Mat** %self, align 8
  %"$r5" = load i32, i32* %r, align 4
  %"$k6" = load i32, i32* %k, align 4
  %call.Mat.at = call i32 @Mat.at(%Mat* %"*self", i32 %"$r5", i32 %"$k6")
  %"*o" = load %Mat*, %Mat** %o, align 8
  %"$k7" = load i32, i32* %k, align 4
  %"$c8" = load i32, i32* %c, align 4
  %call.Mat.at9 = call i32 @Mat.at(%Mat* %"*o", i32 %"$k7", i32 %"$c8")
  %MUL = mul i32 %call.Mat.at, %call.Mat.at9
  %"$s" = load i32, i32* %s, align 4
  %ADD = add i32 %"$s", %MUL
  store i32 %ADD, i32* %s, align 4
  %"$k10" = load i32, i32* %k, align 4
  %ADD11 = add i32 %"$k10", 1
  store i32 %ADD11, i32* %k, align 4
  br label %while2

endloop:                                          ; preds = %while2
  %"*out" = load %Mat*, %Mat** %out, align 8
  %"*out.v" = getelementptr inbounds %Mat, %Mat* %"*out", i32 0, i32 0
  %"$r12" = load i32, i32* %r, align 4
  %MUL13 = mul i32 %"$r12", 2
  %"$c14" = load i32, i32* %c, align 4
  %ADD15 = add i32 %MUL13, %"$c14"
  %"$*out.v" = load i32*, i32** %"*out.v", align 8
  %"*out.v[]" = getelementptr inbounds i32, i32* %"$*out.v", i32 %ADD15
  %"$s16" = load i32, i32* %s, align 4
  store i32 %"$s16", i32* %"*out.v[]", align 4
  %"$c17" = load i32, i32* %c, align 4
  %ADD18 = add i32 %"$c17", 1
  store i32 %ADD18, i32* %c, align 4
  br label %while1

endloop1:                                         ; preds = %while1
  %"$r19" = load i32, i32* %r, align 4
  %ADD20 = add i32 %"$r19", 1
  store i32 %ADD20, i32* %r, align 4
  br label %while0

endloop2:                                         ; preds = %while0
  ret void
}

define i32 @main() {
entry:
  %arr = alloca [4 x i32], align 4
  %arr1 = alloca [4 x i32], align 4
  %arr2 = alloca [4 x i32], align 4
  store [4 x i32] [i32 1, i32 2, i32 3, i32 4], [4 x i32]* %arr, align 4
  %0 = getelementptr inbounds [4 x i32], [4 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** getelementptr inbounds (%Mat, %Mat* @a, i32 0, i32 0), align 8
  store [4 x i32] [i32 5, i32 6, i32 7, i32 8], [4 x i32]* %arr1, align 4
  %1 = getelementptr inbounds [4 x i32], [4 x i32]* %arr1, i32 0, i32 0
  store i32* %1, i32** getelementptr inbounds (%Mat, %Mat* @b, i32 0, i32 0), align 8
  store [4 x i32] zeroinitializer, [4 x i32]* %arr2, align 4
  %2 = getelementptr inbounds [4 x i32], [4 x i32]* %arr2, i32 0, i32 0
  store i32* %2, i32** getelementptr inbounds (%Mat, %Mat* @c, i32 0, i32 0), align 8
  call void @Mat.mul(%Mat* @a, %Mat* @b, %Mat* @c)
  %"$c.v" = load i32*, i32** getelementptr inbounds (%Mat, %Mat* @c, i32 0, i32 0), align 8
  %"c.v[]" = getelementptr inbounds i32, i32* %"$c.v", i32 0
  %"$c.v3" = load i32*, i32** getelementptr inbounds (%Mat, %Mat* @c, i32 0, i32 0), align 8
  %"c.v[]4" = getelementptr inbounds i32, i32* %"$c.v3", i32 1
  %"$c.v[]" = load i32, i32* %"c.v[]", align 4
  %"$c.v[]5" = load i32, i32* %"c.v[]4", align 4
  %ADD = add i32 %"$c.v[]", %"$c.v[]5"
  %"$c.v6" = load i32*, i32** getelementptr inbounds (%Mat, %Mat* @c, i32 0, i32 0), align 8
  %"c.v[]7" = getelementptr inbounds i32, i32* %"$c.v6", i32 2
  %"$c.v[]8" = load i32, i32* %"c.v[]7", align 4
  %ADD9 = add i32 %ADD, %"$c.v[]8"
  %"$c.v10" = load i32*, i32** getelementptr inbounds (%Mat, %Mat* @c, i32 0, i32 0), align 8
  %"c.v[]11" = getelementptr inbounds i32, i32* %"$c.v10", i32 3
  %"$c.v[]12" = load i32, i32* %"c.v[]11", align 4
  %ADD13 = add i32 %ADD9, %"$c.v[]12"
  ret i32 %ADD13
}
```

---

## 061 — name error: a struct declared twice

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
error: aborting due to 1 error
```

### status

```
1
```

---

## 062 — name error: unknown attribute

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
error: test.ura:9:14 'User' has no attribute 'age'
  |
9 |     return u.age
  |              ^^^
help: User has: name, id
error: aborting due to 1 error
```

### status

```
1
```

---

## 063 — name error: unknown method

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
error: test.ura:11:14 'User' has no method 'run'
   |
11 |     return u.run()
   |              ^^^
help: User has: get
error: aborting due to 1 error
```

### status

```
1
```

---

## 064 — name error: self outside a method

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
error: aborting due to 1 error
```

### status

```
1
```

---

## 065 — structs: missing name

```ura
struct :
    a i32

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:1:8 expected a struct name after 'struct'
  |
1 | struct :
  |        ^
help: use struct Name :
error: aborting due to 1 error
```

### status

```
1
```

---

## 066 — structs: missing colon

```ura
struct User
    a i32

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:1:8 expected ':' after the struct name
  |
1 | struct User
  |        ^^^^
help: use struct User:
error: aborting due to 1 error
```

### status

```
1
```

---

## 067 — structs: expression inside a struct

```ura
struct User:
    a i32
    1 + 2

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:3:7 a struct holds only attributes and methods
  |
3 |     1 + 2
  |       ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 068 — structs: attribute without a type

```ura
struct User:
    a

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:2:5 attribute 'a' needs a type
  |
2 |     a
  |     ^
help: like: a i32
error: aborting due to 1 error
```

### status

```
1
```

---

## 069 — structs: attribute declared twice

```ura
struct User:
    a i32
    a i32

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:3:5 'a' is already declared, on line 2
  |
3 |     a i32
  |     ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 070 — structs: no attributes

```ura
struct E:

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:1:8 struct 'E' has no attributes
  |
1 | struct E:
  |        ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 071 — structs: contains itself by value

```ura
struct N:
    next N

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:2:10 struct 'N' can't contain itself
  |
2 |     next N
  |          ^
help: use a ref: '&N'
error: aborting due to 1 error
```

### status

```
1
```

---

## 072 — structs: unknown attribute

```ura
struct User:
    a i32

fn main() i32:
    u User
    return u.zz
```

### stderr

```
error: test.ura:6:14 'User' has no attribute 'zz'
  |
6 |     return u.zz
  |              ^^
help: User has: a
error: aborting due to 1 error
```

### status

```
1
```

---

## 073 — structs: unknown method

```ura
struct User:
    a i32

fn main() i32:
    u User
    return u.zz()
```

### stderr

```
error: test.ura:6:14 'User' has no method 'zz'
  |
6 |     return u.zz()
  |              ^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 074 — structs: method with wrong arguments

```ura
struct User:
    a i32

    fn add(n i32) i32:
        return self.a + n

fn main() i32:
    u User
    return u.add(1, 2)
```

### stderr

```
error: test.ura:9:14 'User' has no method 'add' that takes (i32, i32)
  |
9 |     return u.add(1, 2)
  |              ^^^
help: 'add' takes (i32)
error: aborting due to 1 error
```

### status

```
1
```

---

## 075 — structs: bare attribute inside a method

```ura
struct User:
    value i32

    fn get() i32:
        return value

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:5:16 undeclared variable 'value'
  |
5 |         return value
  |                ^^^^^
help: did you mean 'self.value'?
error: aborting due to 1 error
```

### status

```
1
```

---

## 076 — structs: bare method call inside a method

```ura
struct User:
    value i32

    fn get() i32:
        return self.value

    fn twice() i32:
        return get() * 2

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:8:16 no function named 'get'
  |
8 |         return get() * 2
  |                ^^^
help: did you mean 'self.get()'?
error: aborting due to 1 error
```

### status

```
1
```

---

## 077 — structs: method called on the type

```ura
struct User:
    a i32

    fn get() i32:
        return self.a

fn main() i32:
    return User.get()
```

### stderr

```
error: test.ura:8:12 'User' is a type, not a value
  |
8 |     return User.get()
  |            ^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 078 — structs: self outside a method

```ura
fn main() i32:
    return self.a
```

### stderr

```
error: test.ura:2:12 'self' only exists inside a method
  |
2 |     return self.a
  |            ^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 079 — expressions: add a struct and a number

```ura
struct User:
    a i32

fn main() i32:
    u User
    return u + 1
```

### stderr

```
error: test.ura:6:14 'User' has no operator '+' that takes 'i32'
  |
6 |     return u + 1
  |              ^
error: test.ura:6:14 expected 'i32', found 'User'
  |
6 |     return u + 1
  |              ^
error: aborting due to 2 errors
```

### status

```
1
```

---

## 080 — expressions: compare structs

```ura
struct User:
    a i32

fn main() i32:
    u User
    v User
    if u == v:
        return 1
    return 0
```

### stderr

```
error: test.ura:7:10 'User' has no operator '==' that takes 'User'
  |
7 |     if u == v:
  |          ^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 081 — types: struct into another struct

```ura
struct User:
    a i32

struct Addr:
    b i32

fn main() i32:
    v Addr
    u User = v
    return 0
```

### stderr

```
error: test.ura:9:14 expected 'User', found 'Addr'
  |
9 |     u User = v
  |              ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 082 — struct: init a struct

```ura
struct User:
   a i32
   b i32

fn main() i32:
   c i32
   u User
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32, i32 }

define i32 @main() {
entry:
  %c = alloca i32, align 4
  %u = alloca %User, align 8
  store i32 0, i32* %c, align 4
  store %User zeroinitializer, %User* %u, align 4
  ret i32 0
}
```

---

## 083 — struct: modify struct attribute

```ura
struct User:
   a i32
   b i32

fn main() i32:
   c i32
   u User
   u.b = 10
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32, i32 }

define i32 @main() {
entry:
  %c = alloca i32, align 4
  %u = alloca %User, align 8
  store i32 0, i32* %c, align 4
  store %User zeroinitializer, %User* %u, align 4
  %u.b = getelementptr inbounds %User, %User* %u, i32 0, i32 1
  store i32 10, i32* %u.b, align 4
  ret i32 0
}
```

---

## 084 — struct: method reads and writes self

```ura
struct User:
   a i32

   fn method() i32:
      self.a = 10
      return self.a

fn main() i32:
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
  ret i32 0
}
```

---

## 085 — struct: overloaded method

```ura
struct User:
   a i32

   fn set(n i32):
      self.a = n

   fn set():
      self.a = 1

fn main() i32:
   u User
   u.set()
   return u.a
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%User = type { i32 }

define void @User.set(%User* %0) {
entry:
  %self = alloca %User*, align 8
  store %User* %0, %User** %self, align 8
  %"*self" = load %User*, %User** %self, align 8
  %"*self.a" = getelementptr inbounds %User, %User* %"*self", i32 0, i32 0
  store i32 1, i32* %"*self.a", align 4
  ret void
}

define i32 @main() {
entry:
  %u = alloca %User, align 8
  store %User zeroinitializer, %User* %u, align 4
  call void @User.set(%User* %u)
  %u.a = getelementptr inbounds %User, %User* %u, i32 0, i32 0
  %"$u.a" = load i32, i32* %u.a, align 4
  ret i32 %"$u.a"
}
```

---

## 086 — struct: linked list

```ura
struct Node:
   value i32
   next &Node

fn create(value i32) Node:
   res Node
   res.value = value
   res.next = null
   return res

fn main() i32:
   a Node = create(1)
   b Node = create(2)

   a.next = &b

   p &Node = &a
   sum i32 = 0
   while &p != null:
      sum += p.value
      p = &p.next

   // output("sum: ", sum, "\n")
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Node = type { i32, %Node* }

define %Node @create(i32 %0) {
entry:
  %value = alloca i32, align 4
  %res = alloca %Node, align 8
  store i32 %0, i32* %value, align 4
  %value1 = load i32, i32* %value, align 4
  %res.value = getelementptr inbounds %Node, %Node* %res, i32 0, i32 0
  store i32 %value1, i32* %res.value, align 4
  %res.next = getelementptr inbounds %Node, %Node* %res, i32 0, i32 1
  store %Node* null, %Node** %res.next, align 8
  %res2 = load %Node, %Node* %res, align 8
  ret %Node %res2
}

define i32 @main() {
entry:
  %a = alloca %Node, align 8
  %b = alloca %Node, align 8
  %p = alloca %Node*, align 8
  %sum = alloca i32, align 4
  %create = call %Node @create(i32 1)
  store %Node %create, %Node* %a, align 8
  %create1 = call %Node @create(i32 2)
  store %Node %create1, %Node* %b, align 8
  %a.next = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  store %Node* %b, %Node** %a.next, align 8
  store %Node* %a, %Node** %p, align 8
  store i32 0, i32* %sum, align 4
  br label %cond

cond:                                             ; preds = %then, %entry
  %p2 = load %Node*, %Node** %p, align 8
  %NQ = icmp ne %Node* %p2, null
  br i1 %NQ, label %then, label %endwhile

then:                                             ; preds = %cond
  %sum3 = load i32, i32* %sum, align 4
  %p4 = load %Node*, %Node** %p, align 8
  %p.value = getelementptr inbounds %Node, %Node* %p4, i32 0, i32 0
  %p.value5 = load i32, i32* %p.value, align 4
  %ADD = add i32 %sum3, %p.value5
  store i32 %ADD, i32* %sum, align 4
  %p6 = load %Node*, %Node** %p, align 8
  %p.next = getelementptr inbounds %Node, %Node* %p6, i32 0, i32 1
  %p.next7 = load %Node*, %Node** %p.next, align 8
  store %Node* %p.next7, %Node** %p, align 8
  br label %cond

endwhile:                                         ; preds = %cond
  ret i32 0
}
```

---

## 087 — program: linked list walked through ref attributes

```ura
struct Node:
    value i32
    next &Node

fn main() i32:
    c Node
    c.value = 3
    c.next = null
    b Node
    b.value = 2
    b.next = &c
    a Node
    a.value = 1
    a.next = &b
    sum i32 = 0
    p &Node = &a
    while &p != null:
        sum += p.value
        p = &p.next
    return sum + a.next.next.value * 10
```

### llvm ir

```llvm
; ModuleID = 'ura-module'
source_filename = "ura-module"
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
  %c.value = getelementptr inbounds %Node, %Node* %c, i32 0, i32 0
  store i32 3, i32* %c.value, align 4
  %c.next = getelementptr inbounds %Node, %Node* %c, i32 0, i32 1
  store %Node* null, %Node** %c.next, align 8
  %b.value = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  store i32 2, i32* %b.value, align 4
  %b.next = getelementptr inbounds %Node, %Node* %b, i32 0, i32 1
  store %Node* %c, %Node** %b.next, align 8
  %a.value = getelementptr inbounds %Node, %Node* %a, i32 0, i32 0
  store i32 1, i32* %a.value, align 4
  %a.next = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  store %Node* %b, %Node** %a.next, align 8
  store i32 0, i32* %sum, align 4
  store %Node* %a, %Node** %p, align 8
  br label %cond

cond:                                             ; preds = %then, %entry
  %p1 = load %Node*, %Node** %p, align 8
  %NQ = icmp ne %Node* %p1, null
  br i1 %NQ, label %then, label %endwhile

then:                                             ; preds = %cond
  %sum2 = load i32, i32* %sum, align 4
  %p3 = load %Node*, %Node** %p, align 8
  %p.value = getelementptr inbounds %Node, %Node* %p3, i32 0, i32 0
  %p.value4 = load i32, i32* %p.value, align 4
  %ADD = add i32 %sum2, %p.value4
  store i32 %ADD, i32* %sum, align 4
  %p5 = load %Node*, %Node** %p, align 8
  %p.next = getelementptr inbounds %Node, %Node* %p5, i32 0, i32 1
  %p.next6 = load %Node*, %Node** %p.next, align 8
  store %Node* %p.next6, %Node** %p, align 8
  br label %cond

endwhile:                                         ; preds = %cond
  %sum7 = load i32, i32* %sum, align 4
  %a.next8 = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  %a.next9 = load %Node*, %Node** %a.next8, align 8
  %a.next.next = getelementptr inbounds %Node, %Node* %a.next9, i32 0, i32 1
  %a.next.next10 = load %Node*, %Node** %a.next.next, align 8
  %a.next.next.value = getelementptr inbounds %Node, %Node* %a.next.next10, i32 0, i32 0
  %a.next.next.value11 = load i32, i32* %a.next.next.value, align 4
  %MUL = mul i32 %a.next.next.value11, 10
  %ADD12 = add i32 %sum7, %MUL
  ret i32 %ADD12
}
```

---

## 088 — method: a method reads self

```ura
// stdout: room hall on floor 2
// expect: 0

struct Room:
    name  char[]
    floor i32

    fn describe():
        output("room ", self.name, " on floor ", self.floor, "\n")

fn main() i32:
    r Room
    r.name  = "hall"
    r.floor = 2
    r.describe()
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Room = type { i8*, i32 }

@str = private unnamed_addr constant [21 x i8] c"room %s on floor %d\0A\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"hall\00", align 1

define void @Room.describe(%Room* %0) {
entry:
  %self = alloca %Room*, align 8
  store %Room* %0, %Room** %self, align 8
  %"*self" = load %Room*, %Room** %self, align 8
  %"*self.name" = getelementptr inbounds %Room, %Room* %"*self", i32 0, i32 0
  %"*self1" = load %Room*, %Room** %self, align 8
  %"*self.floor" = getelementptr inbounds %Room, %Room* %"*self1", i32 0, i32 1
  %"$*self.name" = load i8*, i8** %"*self.name", align 8
  %"$*self.floor" = load i32, i32* %"*self.floor", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([21 x i8], [21 x i8]* @str, i32 0, i32 0), i8* %"$*self.name", i32 %"$*self.floor")
  ret void
}

define i32 @main() {
entry:
  %r = alloca %Room, align 8
  store %Room zeroinitializer, %Room* %r, align 8
  %r.name = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0), i8** %r.name, align 8
  %r.floor = getelementptr inbounds %Room, %Room* %r, i32 0, i32 1
  store i32 2, i32* %r.floor, align 4
  call void @Room.describe(%Room* %r)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 089 — method: a method mutates through self, and the caller sees it

```ura
// stdout: floor = 5
// expect: 0

struct Room:
    floor i32

    fn climb(n i32):
        self.floor = self.floor + n

fn main() i32:
    r Room
    r.floor = 2
    r.climb(3)
    output("floor = ", r.floor, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Room = type { i32 }

@str = private unnamed_addr constant [12 x i8] c"floor = %d\0A\00", align 1

define void @Room.climb(%Room* %0, i32 %1) {
entry:
  %self = alloca %Room*, align 8
  %n = alloca i32, align 4
  store %Room* %0, %Room** %self, align 8
  store i32 %1, i32* %n, align 4
  %"*self" = load %Room*, %Room** %self, align 8
  %"*self.floor" = getelementptr inbounds %Room, %Room* %"*self", i32 0, i32 0
  %"*self1" = load %Room*, %Room** %self, align 8
  %"*self.floor2" = getelementptr inbounds %Room, %Room* %"*self1", i32 0, i32 0
  %"$*self.floor" = load i32, i32* %"*self.floor2", align 4
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$*self.floor", %"$n"
  store i32 %ADD, i32* %"*self.floor", align 4
  ret void
}

define i32 @main() {
entry:
  %r = alloca %Room, align 8
  store %Room zeroinitializer, %Room* %r, align 4
  %r.floor = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  store i32 2, i32* %r.floor, align 4
  call void @Room.climb(%Room* %r, i32 3)
  %r.floor1 = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  %"$r.floor" = load i32, i32* %r.floor1, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i32 %"$r.floor")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 090 — method: a method with a return value

```ura
// stdout: depth = 40
// expect: 0

struct Room:
    floor i32

    fn depth() i32:
        return self.floor * 10

fn main() i32:
    r Room
    r.floor = 4
    output("depth = ", r.depth(), "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Room = type { i32 }

@str = private unnamed_addr constant [12 x i8] c"depth = %d\0A\00", align 1

define i32 @Room.depth(%Room* %0) {
entry:
  %self = alloca %Room*, align 8
  store %Room* %0, %Room** %self, align 8
  %"*self" = load %Room*, %Room** %self, align 8
  %"*self.floor" = getelementptr inbounds %Room, %Room* %"*self", i32 0, i32 0
  %"$*self.floor" = load i32, i32* %"*self.floor", align 4
  %MUL = mul i32 %"$*self.floor", 10
  ret i32 %MUL
}

define i32 @main() {
entry:
  %r = alloca %Room, align 8
  store %Room zeroinitializer, %Room* %r, align 4
  %r.floor = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  store i32 4, i32* %r.floor, align 4
  %call.Room.depth = call i32 @Room.depth(%Room* %r)
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i32 %call.Room.depth)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 091 — method: a method calling a sibling method declared below it

```ura
// stdout: floor 10
// expect: 0

struct Room:
    floor i32

    fn show():
        output("floor ", self.twice(), "\n")

    fn twice() i32:
        return self.floor * 2

fn main() i32:
    r Room
    r.floor = 5
    r.show()
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Room = type { i32 }

@str = private unnamed_addr constant [10 x i8] c"floor %d\0A\00", align 1

define void @Room.show(%Room* %0) {
entry:
  %self = alloca %Room*, align 8
  store %Room* %0, %Room** %self, align 8
  %"*self" = load %Room*, %Room** %self, align 8
  %call.Room.twice = call i32 @Room.twice(%Room* %"*self")
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %call.Room.twice)
  ret void
}

define i32 @Room.twice(%Room* %0) {
entry:
  %self = alloca %Room*, align 8
  store %Room* %0, %Room** %self, align 8
  %"*self" = load %Room*, %Room** %self, align 8
  %"*self.floor" = getelementptr inbounds %Room, %Room* %"*self", i32 0, i32 0
  %"$*self.floor" = load i32, i32* %"*self.floor", align 4
  %MUL = mul i32 %"$*self.floor", 2
  ret i32 %MUL
}

define i32 @main() {
entry:
  %r = alloca %Room, align 8
  store %Room zeroinitializer, %Room* %r, align 4
  %r.floor = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  store i32 5, i32* %r.floor, align 4
  call void @Room.show(%Room* %r)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 092 — method: two structs may declare the same method name

```ura
// stdout: room on floor 1
// stdout: tower of height 9
// expect: 0

struct Room:
    floor i32

    fn describe():
        output("room on floor ", self.floor, "\n")

struct Tower:
    height i32

    fn describe():
        output("tower of height ", self.height, "\n")

fn main() i32:
    r Room
    r.floor = 1
    t Tower
    t.height = 9
    r.describe()
    t.describe()
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Room = type { i32 }
%Tower = type { i32 }

@str = private unnamed_addr constant [18 x i8] c"room on floor %d\0A\00", align 1
@str.1 = private unnamed_addr constant [20 x i8] c"tower of height %d\0A\00", align 1

define void @Room.describe(%Room* %0) {
entry:
  %self = alloca %Room*, align 8
  store %Room* %0, %Room** %self, align 8
  %"*self" = load %Room*, %Room** %self, align 8
  %"*self.floor" = getelementptr inbounds %Room, %Room* %"*self", i32 0, i32 0
  %"$*self.floor" = load i32, i32* %"*self.floor", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str, i32 0, i32 0), i32 %"$*self.floor")
  ret void
}

define void @Tower.describe(%Tower* %0) {
entry:
  %self = alloca %Tower*, align 8
  store %Tower* %0, %Tower** %self, align 8
  %"*self" = load %Tower*, %Tower** %self, align 8
  %"*self.height" = getelementptr inbounds %Tower, %Tower* %"*self", i32 0, i32 0
  %"$*self.height" = load i32, i32* %"*self.height", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @str.1, i32 0, i32 0), i32 %"$*self.height")
  ret void
}

define i32 @main() {
entry:
  %r = alloca %Room, align 8
  %t = alloca %Tower, align 8
  store %Room zeroinitializer, %Room* %r, align 4
  %r.floor = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  store i32 1, i32* %r.floor, align 4
  store %Tower zeroinitializer, %Tower* %t, align 4
  %t.height = getelementptr inbounds %Tower, %Tower* %t, i32 0, i32 0
  store i32 9, i32* %t.height, align 4
  call void @Room.describe(%Room* %r)
  call void @Tower.describe(%Tower* %t)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 093 — method: a method on an array element

```ura
// stdout: 8 1
// expect: 0

struct Room:
    floor i32

    fn bump():
        self.floor = self.floor + 1

fn main() i32:
    rooms Room[] = Room[2]
    rooms[0].floor = 7
    rooms[0].bump()
    rooms[1].bump()
    output(rooms[0].floor, " ", rooms[1].floor, "\n")
    return 0
```

---

## 094 — method: a method on a nested struct field

```ura
// stdout: entry depth 3
// expect: 0

struct Room:
    floor i32

    fn depth() i32:
        return self.floor

struct Dungeon:
    entry Room

fn main() i32:
    d Dungeon
    d.entry.floor = 3
    output("entry depth ", d.entry.depth(), "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Room = type { i32 }
%Dungeon = type { %Room }

@str = private unnamed_addr constant [16 x i8] c"entry depth %d\0A\00", align 1

define i32 @Room.depth(%Room* %0) {
entry:
  %self = alloca %Room*, align 8
  store %Room* %0, %Room** %self, align 8
  %"*self" = load %Room*, %Room** %self, align 8
  %"*self.floor" = getelementptr inbounds %Room, %Room* %"*self", i32 0, i32 0
  %"$*self.floor" = load i32, i32* %"*self.floor", align 4
  ret i32 %"$*self.floor"
}

define i32 @main() {
entry:
  %d = alloca %Dungeon, align 8
  store %Dungeon zeroinitializer, %Dungeon* %d, align 4
  %d.entry = getelementptr inbounds %Dungeon, %Dungeon* %d, i32 0, i32 0
  %d.entry.floor = getelementptr inbounds %Room, %Room* %d.entry, i32 0, i32 0
  store i32 3, i32* %d.entry.floor, align 4
  %d.entry1 = getelementptr inbounds %Dungeon, %Dungeon* %d, i32 0, i32 0
  %call.Room.depth = call i32 @Room.depth(%Room* %d.entry1)
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str, i32 0, i32 0), i32 %call.Room.depth)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 095 — method: calling a method the struct does not have

```ura
// error: Struct Room has no method 'nope'; check the spelling or declare it in the struct

struct Room:
    floor i32

fn main() i32:
    r Room
    r.nope()
    return 0
```

### stderr

```
error: test.ura:8:7 'Room' has no method 'nope'
  |
8 |     r.nope()
  |       ^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 096 — method: calling a method on a non-struct

```ura
// error: Cannot call '.nope()' on i32; only a struct has methods

fn main() i32:
    x i32 = 3
    x.nope()
    return 0
```

### stderr

```
error: test.ura:5:5 'x' is 'i32', not a struct
  |
5 |     x.nope()
  |     ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 097 — method: wrong argument count to a method

```ura
// error: Wrong number of arguments to 'climb'

struct Room:
    floor i32

    fn climb(n i32):
        self.floor = self.floor + n

fn main() i32:
    r Room
    r.climb()
    return 0
```

### stderr

```
error: test.ura:11:7 'Room' has no method 'climb' that takes ()
   |
11 |     r.climb()
   |       ^^^^^
help: 'climb' takes (i32)
error: aborting due to 1 error
```

### status

```
1
```

---

## 098 — method: chaining a field access onto a call

```ura
// stdout: static 7
// stdout: plain  3
// expect: 0

struct Room:
    floor i32

    pub fn create(f i32) Room:
        r Room
        r.floor = f
        return r

fn make(f i32) Room:
    r Room
    r.floor = f
    return r

fn main() i32:
    output("static ", Room::create(7).floor, "\n")
    output("plain  ", make(3).floor, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Room = type { i32 }

@str = private unnamed_addr constant [11 x i8] c"static %d\0A\00", align 1
@str.1 = private unnamed_addr constant [11 x i8] c"plain  %d\0A\00", align 1

define %Room @Room.create(i32 %0) {
entry:
  %f = alloca i32, align 4
  %r = alloca %Room, align 8
  store i32 %0, i32* %f, align 4
  store %Room zeroinitializer, %Room* %r, align 4
  %r.floor = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  %"$f" = load i32, i32* %f, align 4
  store i32 %"$f", i32* %r.floor, align 4
  %"$r" = load %Room, %Room* %r, align 4
  ret %Room %"$r"
}

define %Room @make(i32 %0) {
entry:
  %f = alloca i32, align 4
  %r = alloca %Room, align 8
  store i32 %0, i32* %f, align 4
  store %Room zeroinitializer, %Room* %r, align 4
  %r.floor = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  %"$f" = load i32, i32* %f, align 4
  store i32 %"$f", i32* %r.floor, align 4
  %"$r" = load %Room, %Room* %r, align 4
  ret %Room %"$r"
}

define i32 @main() {
entry:
  %tmp = alloca %Room, align 8
  %tmp1 = alloca %Room, align 8
  %call.Room.create = call %Room @Room.create(i32 7)
  store %Room %call.Room.create, %Room* %tmp, align 4
  %tmp.floor = getelementptr inbounds %Room, %Room* %tmp, i32 0, i32 0
  %"$tmp.floor" = load i32, i32* %tmp.floor, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str, i32 0, i32 0), i32 %"$tmp.floor")
  %call.make = call %Room @make(i32 3)
  store %Room %call.make, %Room* %tmp1, align 4
  %tmp.floor2 = getelementptr inbounds %Room, %Room* %tmp1, i32 0, i32 0
  %"$tmp.floor3" = load i32, i32* %tmp.floor2, align 4
  %call.dprintf4 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.1, i32 0, i32 0), i32 %"$tmp.floor3")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 099 — struct: declare a struct and a local: named type, alloca, zero-init

```ura
// stdout: ok
// expect: 0

struct Player:
    name char[]
    hp   i32
    mp   i32

fn main() i32:
    p Player
    output("ok\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Player = type { i8*, i32, i32 }

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1

define i32 @main() {
entry:
  %p = alloca %Player, align 8
  store %Player zeroinitializer, %Player* %p, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 100 — struct: a field whose type is another struct

```ura
// stdout: ok
// expect: 0

struct Room:
    name  char[]
    floor i32

struct Dungeon:
    name  char[]
    entry Room

fn main() i32:
    d Dungeon
    output("ok\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Dungeon = type { i8*, %Room }
%Room = type { i8*, i32 }

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1

define i32 @main() {
entry:
  %d = alloca %Dungeon, align 8
  store %Dungeon zeroinitializer, %Dungeon* %d, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 101 — struct: forward reference: a field type declared later in the file

```ura
// stdout: ok
// expect: 0

struct Dungeon:
    name  char[]
    entry Room

struct Room:
    name  char[]
    floor i32

fn main() i32:
    d Dungeon
    output("ok\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Dungeon = type { i8*, %Room }
%Room = type { i8*, i32 }

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1

define i32 @main() {
entry:
  %d = alloca %Dungeon, align 8
  store %Dungeon zeroinitializer, %Dungeon* %d, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 102 — struct: a struct declared inside a function body

```ura
// stdout: ok
// expect: 0

fn main() i32:
    struct Local:
        x i32
        y i32

    l Local
    output("ok\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Local = type { i32, i32 }

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1

define i32 @main() {
entry:
  %l = alloca %Local, align 8
  store %Local zeroinitializer, %Local* %l, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 103 — struct: a struct declared inside an if block

```ura
// stdout: ok
// expect: 0

fn main() i32:
    n i32 = 1
    if n == 1:
        struct Inner:
            x i32

        i Inner
        output("ok\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Inner = type { i32 }

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1

define i32 @main() {
entry:
  %n = alloca i32, align 4
  %i = alloca %Inner, align 8
  store i32 1, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %"$n", 1
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  store %Inner zeroinitializer, %Inner* %i, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 104 — struct: a struct declared inside a loop body

```ura
// stdout: ok
// expect: 0

fn main() i32:
    i i32 = 0
    while i < 2:
        struct Tick:
            n i32

        t Tick
        i += 1
    output("ok\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Tick = type { i32 }

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1

define i32 @main() {
entry:
  %i = alloca i32, align 4
  %t = alloca %Tick, align 8
  store i32 0, i32* %i, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 2
  br i1 %LT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  store %Tick zeroinitializer, %Tick* %t, align 4
  %"$i1" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i1", 1
  store i32 %ADD, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 105 — struct: an array-of-struct field is a fat pointer to the named type

```ura
// stdout: ok
// expect: 0

struct Room:
    floor i32

struct Tree:
    label char[]
    kids  Room[]

fn main() i32:
    t Tree
    output("ok\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Tree = type { i8*, %Room* }
%Room = type { i32 }

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1

define i32 @main() {
entry:
  %t = alloca %Tree, align 8
  store %Tree zeroinitializer, %Tree* %t, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 106 — struct: a struct nested inside another struct's body

```ura
// stdout: ok
// expect: 0

struct Outer:
    name char[]
    struct Config:
        debug bool

    hp i32

fn main() i32:
    o Outer
    output("ok\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Outer = type { i8*, i32 }

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1

define i32 @main() {
entry:
  %o = alloca %Outer, align 8
  store %Outer zeroinitializer, %Outer* %o, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 107 — struct: self reference through an array stays finite

```ura
// stdout: ok
// expect: 0

struct Tree:
    label char[]
    kids  Tree[]

fn main() i32:
    t Tree
    output("ok\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Tree = type { i8*, %Tree* }

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1

define i32 @main() {
entry:
  %t = alloca %Tree, align 8
  store %Tree zeroinitializer, %Tree* %t, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 108 — struct: an unused struct emits no type: LLVM drops unreferenced named types

```ura
// stdout: ok
// expect: 0

struct Unused:
    x i32
    y i32

fn main() i32:
    output("ok\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1

define i32 @main() {
entry:
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 109 — struct: the same struct name in three nested scopes: innermost wins

```ura
// stdout: ok
// expect: 0

struct Local:
    a char
    b char

fn main() i32:
    l0 Local

    struct Local:
        x i32
        y i32

    n i32 = 1
    if n == 1:
        struct Local:
            z char[]

        l3 Local

    l Local
    output("ok\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Local.0 = type { i32, i32 }
%Local.1 = type { i8* }

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1

define i32 @main() {
entry:
  %l0 = alloca %Local.0, align 8
  %n = alloca i32, align 4
  %l3 = alloca %Local.1, align 8
  %l = alloca %Local.0, align 8
  store %Local.0 zeroinitializer, %Local.0* %l0, align 4
  store i32 1, i32* %n, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %"$n", 1
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  store %Local.1 zeroinitializer, %Local.1* %l3, align 8
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  store %Local.0 zeroinitializer, %Local.0* %l, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 110 — struct: read and write a field

```ura
// stdout: armory on floor 3
// stdout: floor now 13
// expect: 0

struct Room:
    name  char[]
    floor i32

fn main() i32:
    p Room
    p.name  = "armory"
    p.floor = 3
    output(p.name, " on floor ", p.floor, "\n")
    p.floor = p.floor + 10
    output("floor now ", p.floor, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Room = type { i8*, i32 }

@str = private unnamed_addr constant [7 x i8] c"armory\00", align 1
@str.1 = private unnamed_addr constant [16 x i8] c"%s on floor %d\0A\00", align 1
@str.2 = private unnamed_addr constant [14 x i8] c"floor now %d\0A\00", align 1

define i32 @main() {
entry:
  %p = alloca %Room, align 8
  store %Room zeroinitializer, %Room* %p, align 8
  %p.name = getelementptr inbounds %Room, %Room* %p, i32 0, i32 0
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i8** %p.name, align 8
  %p.floor = getelementptr inbounds %Room, %Room* %p, i32 0, i32 1
  store i32 3, i32* %p.floor, align 4
  %p.name1 = getelementptr inbounds %Room, %Room* %p, i32 0, i32 0
  %p.floor2 = getelementptr inbounds %Room, %Room* %p, i32 0, i32 1
  %"$p.name" = load i8*, i8** %p.name1, align 8
  %"$p.floor" = load i32, i32* %p.floor2, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.1, i32 0, i32 0), i8* %"$p.name", i32 %"$p.floor")
  %p.floor3 = getelementptr inbounds %Room, %Room* %p, i32 0, i32 1
  %p.floor4 = getelementptr inbounds %Room, %Room* %p, i32 0, i32 1
  %"$p.floor5" = load i32, i32* %p.floor4, align 4
  %ADD = add i32 %"$p.floor5", 10
  store i32 %ADD, i32* %p.floor3, align 4
  %p.floor6 = getelementptr inbounds %Room, %Room* %p, i32 0, i32 1
  %"$p.floor7" = load i32, i32* %p.floor6, align 4
  %call.dprintf8 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.2, i32 0, i32 0), i32 %"$p.floor7")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 111 — struct: nested field access through a struct field

```ura
// stdout: keep entry gate floor 1
// expect: 0

struct Room:
    name  char[]
    floor i32

struct Dungeon:
    name  char[]
    entry Room

fn main() i32:
    d Dungeon
    d.name = "keep"
    d.entry.name  = "gate"
    d.entry.floor = 1
    output(d.name, " entry ", d.entry.name, " floor ", d.entry.floor, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Dungeon = type { i8*, %Room }
%Room = type { i8*, i32 }

@str = private unnamed_addr constant [5 x i8] c"keep\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"gate\00", align 1
@str.2 = private unnamed_addr constant [22 x i8] c"%s entry %s floor %d\0A\00", align 1

define i32 @main() {
entry:
  %d = alloca %Dungeon, align 8
  store %Dungeon zeroinitializer, %Dungeon* %d, align 8
  %d.name = getelementptr inbounds %Dungeon, %Dungeon* %d, i32 0, i32 0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %d.name, align 8
  %d.entry = getelementptr inbounds %Dungeon, %Dungeon* %d, i32 0, i32 1
  %d.entry.name = getelementptr inbounds %Room, %Room* %d.entry, i32 0, i32 0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0), i8** %d.entry.name, align 8
  %d.entry1 = getelementptr inbounds %Dungeon, %Dungeon* %d, i32 0, i32 1
  %d.entry.floor = getelementptr inbounds %Room, %Room* %d.entry1, i32 0, i32 1
  store i32 1, i32* %d.entry.floor, align 4
  %d.name2 = getelementptr inbounds %Dungeon, %Dungeon* %d, i32 0, i32 0
  %d.entry3 = getelementptr inbounds %Dungeon, %Dungeon* %d, i32 0, i32 1
  %d.entry.name4 = getelementptr inbounds %Room, %Room* %d.entry3, i32 0, i32 0
  %d.entry5 = getelementptr inbounds %Dungeon, %Dungeon* %d, i32 0, i32 1
  %d.entry.floor6 = getelementptr inbounds %Room, %Room* %d.entry5, i32 0, i32 1
  %"$d.name" = load i8*, i8** %d.name2, align 8
  %"$d.entry.name" = load i8*, i8** %d.entry.name4, align 8
  %"$d.entry.floor" = load i32, i32* %d.entry.floor6, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @str.2, i32 0, i32 0), i8* %"$d.name", i8* %"$d.entry.name", i32 %"$d.entry.floor")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 112 — struct: a stack array of structs: Room[N]

```ura
// stdout: lobby@1 cellar@0
// expect: 0

struct Room:
    name  char[]
    floor i32

fn main() i32:
    s Room[] = Room[2]
    s[0].name  = "lobby"
    s[0].floor = 1
    s[1].name  = "cellar"
    output(s[0].name, "@", s[0].floor, " ", s[1].name, "@", s[1].floor, "\n")
    return 0
```

---

## 113 — struct: a 2D array of structs: Room[N][M]

```ura
// stdout: grid-a@3 grid-b@4
// expect: 0

struct Room:
    name  char[]
    floor i32

fn main() i32:
    g Room[][] = Room[2][1]
    g[0][0].name  = "grid-a"
    g[0][0].floor = 3
    g[1][0].name  = "grid-b"
    g[1][0].floor = 4
    output(g[0][0].name, "@", g[0][0].floor, " ", g[1][0].name, "@", g[1][0].floor, "\n")
    return 0
```

---

## 114 — struct: '.len' on arrays of structs, including 2D

```ura
// stdout: 2 4
// expect: 0

struct Room:
    floor i32

fn main() i32:
    s Room[]   = Room[2]
    g Room[][] = Room[4][1]
    output(s.len, " ", g.len, "\n")
    return 0
```

---

## 115 — struct: 'stack' and 'heap' are ordinary identifiers

```ura
// stdout: 5 7
// expect: 0

fn main() i32:
    stack i32 = 5
    heap  i32 = 7
    output(stack, " ", heap, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1

define i32 @main() {
entry:
  %stack = alloca i32, align 4
  %heap = alloca i32, align 4
  store i32 5, i32* %stack, align 4
  store i32 7, i32* %heap, align 4
  %"$stack" = load i32, i32* %stack, align 4
  %"$heap" = load i32, i32* %heap, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %"$stack", i32 %"$heap")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 116 — struct: a 'ref?' field may refer to its own struct

```ura
// stdout: 1
// expect: 0

struct Node:
    value i32
    next &Node

fn main() i32:
    a Node
    a.value = 1
    output(a.value, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Node = type { i32, %Node* }

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @main() {
entry:
  %a = alloca %Node, align 8
  store %Node zeroinitializer, %Node* %a, align 8
  %a.value = getelementptr inbounds %Node, %Node* %a, i32 0, i32 0
  store i32 1, i32* %a.value, align 4
  %a.value1 = getelementptr inbounds %Node, %Node* %a, i32 0, i32 0
  %"$a.value" = load i32, i32* %a.value1, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$a.value")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 117 — struct: bind, read and write through a 'ref?' field

```ura
// stdout: 20 3 20
// expect: 0

struct Node:
    value i32
    next &Node

fn main() i32:
    a Node
    b Node
    c Node
    a.value = 1
    b.value = 2
    c.value = 3
    a.next = &b
    b.next = &c
    a.next.value = 20
    output(a.next.value, " ", a.next.next.value, " ", b.value, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Node = type { i32, %Node* }

@str = private unnamed_addr constant [10 x i8] c"%d %d %d\0A\00", align 1

define i32 @main() {
entry:
  %a = alloca %Node, align 8
  %b = alloca %Node, align 8
  %c = alloca %Node, align 8
  store %Node zeroinitializer, %Node* %a, align 8
  store %Node zeroinitializer, %Node* %b, align 8
  store %Node zeroinitializer, %Node* %c, align 8
  %a.value = getelementptr inbounds %Node, %Node* %a, i32 0, i32 0
  store i32 1, i32* %a.value, align 4
  %b.value = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  store i32 2, i32* %b.value, align 4
  %c.value = getelementptr inbounds %Node, %Node* %c, i32 0, i32 0
  store i32 3, i32* %c.value, align 4
  %a.next = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  store %Node* %b, %Node** %a.next, align 8
  %b.next = getelementptr inbounds %Node, %Node* %b, i32 0, i32 1
  store %Node* %c, %Node** %b.next, align 8
  %a.next1 = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  %"*a.next" = load %Node*, %Node** %a.next1, align 8
  %"*a.next.value" = getelementptr inbounds %Node, %Node* %"*a.next", i32 0, i32 0
  store i32 20, i32* %"*a.next.value", align 4
  %a.next2 = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  %"*a.next3" = load %Node*, %Node** %a.next2, align 8
  %"*a.next.value4" = getelementptr inbounds %Node, %Node* %"*a.next3", i32 0, i32 0
  %a.next5 = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  %"*a.next6" = load %Node*, %Node** %a.next5, align 8
  %"*a.next.next" = getelementptr inbounds %Node, %Node* %"*a.next6", i32 0, i32 1
  %"**a.next.next" = load %Node*, %Node** %"*a.next.next", align 8
  %"**a.next.next.value" = getelementptr inbounds %Node, %Node* %"**a.next.next", i32 0, i32 0
  %b.value7 = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  %"$*a.next.value" = load i32, i32* %"*a.next.value4", align 4
  %"$**a.next.next.value" = load i32, i32* %"**a.next.next.value", align 4
  %"$b.value" = load i32, i32* %b.value7, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %"$*a.next.value", i32 %"$**a.next.next.value", i32 %"$b.value")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 118 — struct: struct assignment copies by value

```ura
// stdout: hall 9 / hall 2
// expect: 0

struct Room:
    name  char[]
    floor i32

fn main() i32:
    a Room
    a.name  = "hall"
    a.floor = 2
    b Room
    b = a
    b.floor = 9
    output(b.name, " ", b.floor, " / ", a.name, " ", a.floor, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Room = type { i8*, i32 }

@str = private unnamed_addr constant [5 x i8] c"hall\00", align 1
@str.1 = private unnamed_addr constant [15 x i8] c"%s %d / %s %d\0A\00", align 1

define i32 @main() {
entry:
  %a = alloca %Room, align 8
  %b = alloca %Room, align 8
  store %Room zeroinitializer, %Room* %a, align 8
  %a.name = getelementptr inbounds %Room, %Room* %a, i32 0, i32 0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %a.name, align 8
  %a.floor = getelementptr inbounds %Room, %Room* %a, i32 0, i32 1
  store i32 2, i32* %a.floor, align 4
  %"$a" = load %Room, %Room* %a, align 8
  store %Room %"$a", %Room* %b, align 8
  %b.floor = getelementptr inbounds %Room, %Room* %b, i32 0, i32 1
  store i32 9, i32* %b.floor, align 4
  %b.name = getelementptr inbounds %Room, %Room* %b, i32 0, i32 0
  %b.floor1 = getelementptr inbounds %Room, %Room* %b, i32 0, i32 1
  %a.name2 = getelementptr inbounds %Room, %Room* %a, i32 0, i32 0
  %a.floor3 = getelementptr inbounds %Room, %Room* %a, i32 0, i32 1
  %"$b.name" = load i8*, i8** %b.name, align 8
  %"$b.floor" = load i32, i32* %b.floor1, align 4
  %"$a.name" = load i8*, i8** %a.name2, align 8
  %"$a.floor" = load i32, i32* %a.floor3, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.1, i32 0, i32 0), i8* %"$b.name", i32 %"$b.floor", i8* %"$a.name", i32 %"$a.floor")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 119 — struct: a struct as a by-value parameter

```ura
// stdout: room hall@2
// expect: 0

struct Room:
    name  char[]
    floor i32

fn show(r Room):
    output("room ", r.name, "@", r.floor, "\n")

fn main() i32:
    a Room
    a.name  = "hall"
    a.floor = 2
    show(a)
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Room = type { i8*, i32 }

@str = private unnamed_addr constant [12 x i8] c"room %s@%d\0A\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"hall\00", align 1

define void @show(%Room %0) {
entry:
  %r = alloca %Room, align 8
  store %Room %0, %Room* %r, align 8
  %r.name = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  %r.floor = getelementptr inbounds %Room, %Room* %r, i32 0, i32 1
  %"$r.name" = load i8*, i8** %r.name, align 8
  %"$r.floor" = load i32, i32* %r.floor, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i8* %"$r.name", i32 %"$r.floor")
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Room, align 8
  store %Room zeroinitializer, %Room* %a, align 8
  %a.name = getelementptr inbounds %Room, %Room* %a, i32 0, i32 0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0), i8** %a.name, align 8
  %a.floor = getelementptr inbounds %Room, %Room* %a, i32 0, i32 1
  store i32 2, i32* %a.floor, align 4
  %"$a" = load %Room, %Room* %a, align 8
  call void @show(%Room %"$a")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 120 — struct: a 'ref' struct parameter mutates the caller's struct

```ura
// stdout: after 3
// expect: 0

struct Room:
    name  char[]
    floor i32

fn bump(r &Room):
    r.floor = r.floor + 1

fn main() i32:
    a Room
    a.floor = 2
    bump(&a)
    output("after ", a.floor, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Room = type { i8*, i32 }

@str = private unnamed_addr constant [10 x i8] c"after %d\0A\00", align 1

define void @bump(%Room* %0) {
entry:
  %r = alloca %Room*, align 8
  store %Room* %0, %Room** %r, align 8
  %"*r" = load %Room*, %Room** %r, align 8
  %"*r.floor" = getelementptr inbounds %Room, %Room* %"*r", i32 0, i32 1
  %"*r1" = load %Room*, %Room** %r, align 8
  %"*r.floor2" = getelementptr inbounds %Room, %Room* %"*r1", i32 0, i32 1
  %"$*r.floor" = load i32, i32* %"*r.floor2", align 4
  %ADD = add i32 %"$*r.floor", 1
  store i32 %ADD, i32* %"*r.floor", align 4
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Room, align 8
  store %Room zeroinitializer, %Room* %a, align 8
  %a.floor = getelementptr inbounds %Room, %Room* %a, i32 0, i32 1
  store i32 2, i32* %a.floor, align 4
  call void @bump(%Room* %a)
  %a.floor1 = getelementptr inbounds %Room, %Room* %a, i32 0, i32 1
  %"$a.floor" = load i32, i32* %a.floor1, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %"$a.floor")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 121 — struct: returning a struct: the create() constructor convention

```ura
// stdout: made 4
// expect: 0

struct Room:
    name  char[]
    floor i32

fn create(f i32) Room:
    r Room
    r.name  = "made"
    r.floor = f
    return r

fn describe(r Room) char[]:
    return r.name

fn main() i32:
    a Room = create(4)
    output(describe(a), " ", a.floor, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Room = type { i8*, i32 }

@str = private unnamed_addr constant [5 x i8] c"made\00", align 1
@str.1 = private unnamed_addr constant [7 x i8] c"%s %d\0A\00", align 1

define %Room @create(i32 %0) {
entry:
  %f = alloca i32, align 4
  %r = alloca %Room, align 8
  store i32 %0, i32* %f, align 4
  store %Room zeroinitializer, %Room* %r, align 8
  %r.name = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %r.name, align 8
  %r.floor = getelementptr inbounds %Room, %Room* %r, i32 0, i32 1
  %"$f" = load i32, i32* %f, align 4
  store i32 %"$f", i32* %r.floor, align 4
  %"$r" = load %Room, %Room* %r, align 8
  ret %Room %"$r"
}

define i8* @describe(%Room %0) {
entry:
  %r = alloca %Room, align 8
  store %Room %0, %Room* %r, align 8
  %r.name = getelementptr inbounds %Room, %Room* %r, i32 0, i32 0
  %"$r.name" = load i8*, i8** %r.name, align 8
  ret i8* %"$r.name"
}

define i32 @main() {
entry:
  %a = alloca %Room, align 8
  %call.create = call %Room @create(i32 4)
  store %Room %call.create, %Room* %a, align 8
  %"$a" = load %Room, %Room* %a, align 8
  %call.describe = call i8* @describe(%Room %"$a")
  %a.floor = getelementptr inbounds %Room, %Room* %a, i32 0, i32 1
  %"$a.floor" = load i32, i32* %a.floor, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0), i8* %call.describe, i32 %"$a.floor")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 122 — struct: a field declared twice

```ura
// error: Field 'hp' is already declared in this struct; rename it or remove the duplicate

struct Player:
    name char[]
    hp   i32
    hp   i32

fn main() i32:
    p Player
    return 0
```

### stderr

```
error: test.ura:6:5 'hp' is already declared, on line 5
  |
6 |     hp   i32
  |     ^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 123 — struct: a field whose type does not exist

```ura
// error: Unknown type 'Room'

struct Dungeon:
    entry Room

fn main() i32:
    d Dungeon
    return 0
```

### stderr

```
error: test.ura:4:11 unknown type 'Room'
  |
4 |     entry Room
  |           ^^^^
help: declare it: struct Room:
error: aborting due to 1 error
```

### status

```
1
```

---

## 124 — struct: a struct that contains itself by value

```ura
// error: Struct Node cannot contain itself by value; use an array or a 'ref?' so the size stays finite

struct Node:
    value i32
    next  Node

fn main() i32:
    n Node
    return 0
```

### stderr

```
error: test.ura:5:11 struct 'Node' can't contain itself
  |
5 |     next  Node
  |           ^^^^
help: use a ref: '&Node'
error: aborting due to 1 error
```

### status

```
1
```

---

## 125 — struct: two structs that contain each other by value

```ura
// error: Struct A cannot contain itself by value; use an array or a 'ref?' so the size stays finite

struct A:
    b B

struct B:
    a A

fn main() i32:
    x A
    return 0
```

### stderr

```
error: test.ura:4:7 struct 'A' can't contain itself
  |
4 |     b B
  |       ^
help: use a ref: '&B'
error: aborting due to 1 error
```

### status

```
1
```

---

## 126 — struct: a variable of a type that was never declared

```ura
// error: Unknown type 'Player'

fn main() i32:
    p Player
    return 0
```

### stderr

```
error: test.ura:4:7 unknown type 'Player'
  |
4 |     p Player
  |       ^^^^^^
help: declare it: struct Player:
error: aborting due to 1 error
```

### status

```
1
```

---

## 127 — struct: reading a field the struct does not have

```ura
// error: Struct Player has no field 'nope'; check the spelling or declare it in the struct

struct Player:
    name char[]
    hp   i32

fn main() i32:
    p Player
    output(p.nope)
    return 0
```

### stderr

```
error: test.ura:9:14 'Player' has no attribute 'nope'
  |
9 |     output(p.nope)
  |              ^^^^
help: Player has: name, hp
error: aborting due to 1 error
```

### status

```
1
```

---

## 128 — struct: reading a field from a scalar

```ura
// error: Cannot read '.foo' from i32; only a struct has fields

fn main() i32:
    x i32 = 3
    output(x.foo)
    return 0
```

### stderr

```
error: test.ura:5:12 'x' is 'i32', not a struct
  |
5 |     output(x.foo)
  |            ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 129 — struct: '.len' on a scalar

```ura
// error: '.len' is only valid on an array, not i32

fn main() i32:
    x i32 = 3
    output(x.len)
    return 0
```

### stderr

```
error: test.ura:5:12 'x' is 'i32', not a struct
  |
5 |     output(x.len)
  |            ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 130 — struct: a struct declared in a block is not visible outside it

```ura
// error: Unknown type 'Inner'

fn main() i32:
    n i32 = 1
    if n == 1:
        struct Inner:
            x i32

    i Inner
    return 0
```

### stderr

```
error: test.ura:9:7 unknown type 'Inner'
  |
9 |     i Inner
  |       ^^^^^
help: declare it: struct Inner:
error: aborting due to 1 error
```

### status

```
1
```

---

## 131 — struct: a struct declared twice in the same scope

```ura
// error: Redeclaration of struct 'Player'

struct Player:
    hp i32

struct Player:
    mp i32

fn main() i32:
    p Player
    return 0
```

### stderr

```
error: test.ura:6:8 struct 'Player' is already declared, on line 3
  |
6 | struct Player:
  |        ^^^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 132 — struct: a method and a field chain called on a ref? field

```ura
// stdout: worth 5
// stdout: worth 7
// stdout: worth 9
// stdout: chained field 9
// expect: 0

struct Item:
    worth i32
    next &Item

    pub fn create(w i32) Item:
        it Item
        it.worth = w
        return it

    fn show():
        output("worth ", self.worth, "\n")

fn main() i32:
    head Item = Item::create(5)
    mid  Item = Item::create(7)
    tail Item = Item::create(9)
    head.next = &mid
    mid.next  = &tail
    head.show()
    head.next.show()
    head.next.next.show()
    output("chained field ", head.next.next.worth, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Item = type { i32, %Item* }

@str = private unnamed_addr constant [10 x i8] c"worth %d\0A\00", align 1
@str.1 = private unnamed_addr constant [18 x i8] c"chained field %d\0A\00", align 1

define %Item @Item.create(i32 %0) {
entry:
  %w = alloca i32, align 4
  %it = alloca %Item, align 8
  store i32 %0, i32* %w, align 4
  store %Item zeroinitializer, %Item* %it, align 8
  %it.worth = getelementptr inbounds %Item, %Item* %it, i32 0, i32 0
  %"$w" = load i32, i32* %w, align 4
  store i32 %"$w", i32* %it.worth, align 4
  %"$it" = load %Item, %Item* %it, align 8
  ret %Item %"$it"
}

define void @Item.show(%Item* %0) {
entry:
  %self = alloca %Item*, align 8
  store %Item* %0, %Item** %self, align 8
  %"*self" = load %Item*, %Item** %self, align 8
  %"*self.worth" = getelementptr inbounds %Item, %Item* %"*self", i32 0, i32 0
  %"$*self.worth" = load i32, i32* %"*self.worth", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %"$*self.worth")
  ret void
}

define i32 @main() {
entry:
  %head = alloca %Item, align 8
  %mid = alloca %Item, align 8
  %tail = alloca %Item, align 8
  %call.Item.create = call %Item @Item.create(i32 5)
  store %Item %call.Item.create, %Item* %head, align 8
  %call.Item.create1 = call %Item @Item.create(i32 7)
  store %Item %call.Item.create1, %Item* %mid, align 8
  %call.Item.create2 = call %Item @Item.create(i32 9)
  store %Item %call.Item.create2, %Item* %tail, align 8
  %head.next = getelementptr inbounds %Item, %Item* %head, i32 0, i32 1
  store %Item* %mid, %Item** %head.next, align 8
  %mid.next = getelementptr inbounds %Item, %Item* %mid, i32 0, i32 1
  store %Item* %tail, %Item** %mid.next, align 8
  call void @Item.show(%Item* %head)
  %head.next3 = getelementptr inbounds %Item, %Item* %head, i32 0, i32 1
  %"*head.next" = load %Item*, %Item** %head.next3, align 8
  call void @Item.show(%Item* %"*head.next")
  %head.next4 = getelementptr inbounds %Item, %Item* %head, i32 0, i32 1
  %"*head.next5" = load %Item*, %Item** %head.next4, align 8
  %"*head.next.next" = getelementptr inbounds %Item, %Item* %"*head.next5", i32 0, i32 1
  %"**head.next.next" = load %Item*, %Item** %"*head.next.next", align 8
  call void @Item.show(%Item* %"**head.next.next")
  %head.next6 = getelementptr inbounds %Item, %Item* %head, i32 0, i32 1
  %"*head.next7" = load %Item*, %Item** %head.next6, align 8
  %"*head.next.next8" = getelementptr inbounds %Item, %Item* %"*head.next7", i32 0, i32 1
  %"**head.next.next9" = load %Item*, %Item** %"*head.next.next8", align 8
  %"**head.next.next.worth" = getelementptr inbounds %Item, %Item* %"**head.next.next9", i32 0, i32 0
  %"$**head.next.next.worth" = load i32, i32* %"**head.next.next.worth", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str.1, i32 0, i32 0), i32 %"$**head.next.next.worth")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 133 — struct: struct redefinition

```ura
// error: Redeclaration of struct 'Point'
use "@/header"

struct Point:
    x i32
    y i32

struct Point:
    a i32
    b i32

fn main() i32:
    output("hi\n")
    return 0
```

### stderr

```
error: test.ura:8:8 struct 'Point' is already declared, on line 4
  |
8 | struct Point:
  |        ^^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 134 — struct: accessing an undefined field

```ura
// error: Struct Point has no field 'z'; check the spelling or declare it in the struct
use "@/header"

struct Point:
    x i32
    y i32

fn main() i32:
    p Point
    p.x = 1
    p.z = 5
    return 0
```

### stderr

```
error: test.ura:11:7 'Point' has no attribute 'z'
   |
11 |     p.z = 5
   |       ^
help: Point has: x, y
error: aborting due to 1 error
```

### status

```
1
```

---

## 135 — struct: calling undefined method on struct

```ura
// error: Struct Point has no method 'normalize'; check the spelling or declare it in the struct
use "@/header"

struct Point:
    x i32
    y i32

fn main() i32:
    p Point
    p.x = 1
    p.normalize()
    return 0
```

### stderr

```
error: test.ura:11:7 'Point' has no method 'normalize'
   |
11 |     p.normalize()
   |       ^^^^^^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 136 — struct: a struct whose first field is a struct is not circular

```ura
// stdout: first: First{tag: Inner{name: one}, n: 1}
// stdout: last:  Last{n: 2, tag: Inner{name: two}}
// expect: 0

struct Inner:
    name char[]

struct First:
    tag Inner
    n   i32

struct Last:
    n   i32
    tag Inner

fn main() i32:
    a First
    a.tag.name = "one"
    a.n = 1
    output("first: ", a, "\n")

    b Last
    b.n = 2
    b.tag.name = "two"
    output("last:  ", b, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%First = type { %Inner, i32 }
%Inner = type { i8* }
%Last = type { i32, %Inner }

@str = private unnamed_addr constant [4 x i8] c"one\00", align 1
@str.1 = private unnamed_addr constant [8 x i8] c"first: \00", align 1
@str.2 = private unnamed_addr constant [7 x i8] c"First{\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"tag: \00", align 1
@str.4 = private unnamed_addr constant [7 x i8] c"Inner{\00", align 1
@str.5 = private unnamed_addr constant [9 x i8] c"name: %s\00", align 1
@str.6 = private unnamed_addr constant [2 x i8] c"}\00", align 1
@str.7 = private unnamed_addr constant [8 x i8] c", n: %d\00", align 1
@str.8 = private unnamed_addr constant [2 x i8] c"}\00", align 1
@str.9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@str.10 = private unnamed_addr constant [4 x i8] c"two\00", align 1
@str.11 = private unnamed_addr constant [8 x i8] c"last:  \00", align 1
@str.12 = private unnamed_addr constant [6 x i8] c"Last{\00", align 1
@str.13 = private unnamed_addr constant [6 x i8] c"n: %d\00", align 1
@str.14 = private unnamed_addr constant [8 x i8] c", tag: \00", align 1
@str.15 = private unnamed_addr constant [7 x i8] c"Inner{\00", align 1
@str.16 = private unnamed_addr constant [9 x i8] c"name: %s\00", align 1
@str.17 = private unnamed_addr constant [2 x i8] c"}\00", align 1
@str.18 = private unnamed_addr constant [2 x i8] c"}\00", align 1
@str.19 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

define i32 @main() {
entry:
  %a = alloca %First, align 8
  %b = alloca %Last, align 8
  store %First zeroinitializer, %First* %a, align 8
  %a.tag = getelementptr inbounds %First, %First* %a, i32 0, i32 0
  %a.tag.name = getelementptr inbounds %Inner, %Inner* %a.tag, i32 0, i32 0
  store i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i8** %a.tag.name, align 8
  %a.n = getelementptr inbounds %First, %First* %a, i32 0, i32 1
  store i32 1, i32* %a.n, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.1, i32 0, i32 0))
  %call.dprintf1 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.2, i32 0, i32 0))
  %a.tag2 = getelementptr inbounds %First, %First* %a, i32 0, i32 0
  %call.dprintf3 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0))
  %call.dprintf4 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.4, i32 0, i32 0))
  %a.tag.name5 = getelementptr inbounds %Inner, %Inner* %a.tag2, i32 0, i32 0
  %"$a.tag.name" = load i8*, i8** %a.tag.name5, align 8
  %call.dprintf6 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.5, i32 0, i32 0), i8* %"$a.tag.name")
  %call.dprintf7 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.6, i32 0, i32 0))
  %a.n8 = getelementptr inbounds %First, %First* %a, i32 0, i32 1
  %"$a.n" = load i32, i32* %a.n8, align 4
  %call.dprintf9 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.7, i32 0, i32 0), i32 %"$a.n")
  %call.dprintf10 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.8, i32 0, i32 0))
  %call.dprintf11 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.9, i32 0, i32 0))
  store %Last zeroinitializer, %Last* %b, align 8
  %b.n = getelementptr inbounds %Last, %Last* %b, i32 0, i32 0
  store i32 2, i32* %b.n, align 4
  %b.tag = getelementptr inbounds %Last, %Last* %b, i32 0, i32 1
  %b.tag.name = getelementptr inbounds %Inner, %Inner* %b.tag, i32 0, i32 0
  store i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.10, i32 0, i32 0), i8** %b.tag.name, align 8
  %call.dprintf12 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.11, i32 0, i32 0))
  %call.dprintf13 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.12, i32 0, i32 0))
  %b.n14 = getelementptr inbounds %Last, %Last* %b, i32 0, i32 0
  %"$b.n" = load i32, i32* %b.n14, align 4
  %call.dprintf15 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.13, i32 0, i32 0), i32 %"$b.n")
  %b.tag16 = getelementptr inbounds %Last, %Last* %b, i32 0, i32 1
  %call.dprintf17 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.14, i32 0, i32 0))
  %call.dprintf18 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.15, i32 0, i32 0))
  %b.tag.name19 = getelementptr inbounds %Inner, %Inner* %b.tag16, i32 0, i32 0
  %"$b.tag.name" = load i8*, i8** %b.tag.name19, align 8
  %call.dprintf20 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.16, i32 0, i32 0), i8* %"$b.tag.name")
  %call.dprintf21 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.17, i32 0, i32 0))
  %call.dprintf22 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.18, i32 0, i32 0))
  %call.dprintf23 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.19, i32 0, i32 0))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 137 — struct: a struct as a parameter and as a return value

```ura
// expect: 21   (3 + 4 + 14)
struct Point:
    x i32
    y i32

fn sum(p Point) i32:
    return p.x + p.y

fn scale(p Point, k i32) Point:
    p.x *= k
    p.y *= k
    return p

fn main() i32:
    a Point
    a.x = 3
    a.y = 4
    b Point = scale(a, 2)
    return sum(a) + sum(b)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Point = type { i32, i32 }

define i32 @sum(%Point %0) {
entry:
  %p = alloca %Point, align 8
  store %Point %0, %Point* %p, align 4
  %p.x = getelementptr inbounds %Point, %Point* %p, i32 0, i32 0
  %p.y = getelementptr inbounds %Point, %Point* %p, i32 0, i32 1
  %"$p.x" = load i32, i32* %p.x, align 4
  %"$p.y" = load i32, i32* %p.y, align 4
  %ADD = add i32 %"$p.x", %"$p.y"
  ret i32 %ADD
}

define %Point @scale(%Point %0, i32 %1) {
entry:
  %p = alloca %Point, align 8
  %k = alloca i32, align 4
  store %Point %0, %Point* %p, align 4
  store i32 %1, i32* %k, align 4
  %p.x = getelementptr inbounds %Point, %Point* %p, i32 0, i32 0
  %"$k" = load i32, i32* %k, align 4
  %"$p.x" = load i32, i32* %p.x, align 4
  %MUL = mul i32 %"$p.x", %"$k"
  store i32 %MUL, i32* %p.x, align 4
  %p.y = getelementptr inbounds %Point, %Point* %p, i32 0, i32 1
  %"$k1" = load i32, i32* %k, align 4
  %"$p.y" = load i32, i32* %p.y, align 4
  %MUL2 = mul i32 %"$p.y", %"$k1"
  store i32 %MUL2, i32* %p.y, align 4
  %"$p" = load %Point, %Point* %p, align 4
  ret %Point %"$p"
}

define i32 @main() {
entry:
  %a = alloca %Point, align 8
  %b = alloca %Point, align 8
  store %Point zeroinitializer, %Point* %a, align 4
  %a.x = getelementptr inbounds %Point, %Point* %a, i32 0, i32 0
  store i32 3, i32* %a.x, align 4
  %a.y = getelementptr inbounds %Point, %Point* %a, i32 0, i32 1
  store i32 4, i32* %a.y, align 4
  %"$a" = load %Point, %Point* %a, align 4
  %call.scale = call %Point @scale(%Point %"$a", i32 2)
  store %Point %call.scale, %Point* %b, align 4
  %"$a1" = load %Point, %Point* %a, align 4
  %call.sum = call i32 @sum(%Point %"$a1")
  %"$b" = load %Point, %Point* %b, align 4
  %call.sum2 = call i32 @sum(%Point %"$b")
  %ADD = add i32 %call.sum, %call.sum2
  ret i32 %ADD
}
```

---

## 138 — struct: assigning a struct copies it

```ura
// expect: 15   (a stays 1 + 2, b is 5 + 7)
struct Point:
    x i32
    y i32

fn main() i32:
    a Point
    a.x = 1
    a.y = 2
    b Point
    b = a
    b.x = 5
    b.y += 5
    return a.x + a.y + b.x + b.y
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Point = type { i32, i32 }

define i32 @main() {
entry:
  %a = alloca %Point, align 8
  %b = alloca %Point, align 8
  store %Point zeroinitializer, %Point* %a, align 4
  %a.x = getelementptr inbounds %Point, %Point* %a, i32 0, i32 0
  store i32 1, i32* %a.x, align 4
  %a.y = getelementptr inbounds %Point, %Point* %a, i32 0, i32 1
  store i32 2, i32* %a.y, align 4
  %"$a" = load %Point, %Point* %a, align 4
  store %Point %"$a", %Point* %b, align 4
  %b.x = getelementptr inbounds %Point, %Point* %b, i32 0, i32 0
  store i32 5, i32* %b.x, align 4
  %b.y = getelementptr inbounds %Point, %Point* %b, i32 0, i32 1
  %"$b.y" = load i32, i32* %b.y, align 4
  %ADD = add i32 %"$b.y", 5
  store i32 %ADD, i32* %b.y, align 4
  %a.x1 = getelementptr inbounds %Point, %Point* %a, i32 0, i32 0
  %a.y2 = getelementptr inbounds %Point, %Point* %a, i32 0, i32 1
  %"$a.x" = load i32, i32* %a.x1, align 4
  %"$a.y" = load i32, i32* %a.y2, align 4
  %ADD3 = add i32 %"$a.x", %"$a.y"
  %b.x4 = getelementptr inbounds %Point, %Point* %b, i32 0, i32 0
  %"$b.x" = load i32, i32* %b.x4, align 4
  %ADD5 = add i32 %ADD3, %"$b.x"
  %b.y6 = getelementptr inbounds %Point, %Point* %b, i32 0, i32 1
  %"$b.y7" = load i32, i32* %b.y6, align 4
  %ADD8 = add i32 %ADD5, %"$b.y7"
  ret i32 %ADD8
}
```

---

## 139 — struct: a struct declared inside a function

```ura
// expect: 9
fn main() i32:
    struct Pair:
        a i32
        b i32
    p Pair
    p.a = 4
    p.b = 5
    return p.a + p.b
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Pair = type { i32, i32 }

define i32 @main() {
entry:
  %p = alloca %Pair, align 8
  store %Pair zeroinitializer, %Pair* %p, align 4
  %p.a = getelementptr inbounds %Pair, %Pair* %p, i32 0, i32 0
  store i32 4, i32* %p.a, align 4
  %p.b = getelementptr inbounds %Pair, %Pair* %p, i32 0, i32 1
  store i32 5, i32* %p.b, align 4
  %p.a1 = getelementptr inbounds %Pair, %Pair* %p, i32 0, i32 0
  %p.b2 = getelementptr inbounds %Pair, %Pair* %p, i32 0, i32 1
  %"$p.a" = load i32, i32* %p.a1, align 4
  %"$p.b" = load i32, i32* %p.b2, align 4
  %ADD = add i32 %"$p.a", %"$p.b"
  ret i32 %ADD
}
```

---

## 140 — struct: a nested struct as a parameter and a copy

```ura
// expect: 10   (width 4 + height 6)
struct Point:
    x i32
    y i32

struct Rect:
    min Point
    max Point

fn size(r Rect) i32:
    return (r.max.x - r.min.x) + (r.max.y - r.min.y)

fn main() i32:
    r Rect
    r.min.x = 1
    r.min.y = 2
    r.max.x = 5
    r.max.y = 8
    c Rect = r
    return size(c)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Rect = type { %Point, %Point }
%Point = type { i32, i32 }

define i32 @size(%Rect %0) {
entry:
  %r = alloca %Rect, align 8
  store %Rect %0, %Rect* %r, align 4
  %r.max = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %r.max.x = getelementptr inbounds %Point, %Point* %r.max, i32 0, i32 0
  %r.min = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 0
  %r.min.x = getelementptr inbounds %Point, %Point* %r.min, i32 0, i32 0
  %"$r.max.x" = load i32, i32* %r.max.x, align 4
  %"$r.min.x" = load i32, i32* %r.min.x, align 4
  %SUB = sub i32 %"$r.max.x", %"$r.min.x"
  %r.max1 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %r.max.y = getelementptr inbounds %Point, %Point* %r.max1, i32 0, i32 1
  %r.min2 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 0
  %r.min.y = getelementptr inbounds %Point, %Point* %r.min2, i32 0, i32 1
  %"$r.max.y" = load i32, i32* %r.max.y, align 4
  %"$r.min.y" = load i32, i32* %r.min.y, align 4
  %SUB3 = sub i32 %"$r.max.y", %"$r.min.y"
  %ADD = add i32 %SUB, %SUB3
  ret i32 %ADD
}

define i32 @main() {
entry:
  %r = alloca %Rect, align 8
  %c = alloca %Rect, align 8
  store %Rect zeroinitializer, %Rect* %r, align 4
  %r.min = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 0
  %r.min.x = getelementptr inbounds %Point, %Point* %r.min, i32 0, i32 0
  store i32 1, i32* %r.min.x, align 4
  %r.min1 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 0
  %r.min.y = getelementptr inbounds %Point, %Point* %r.min1, i32 0, i32 1
  store i32 2, i32* %r.min.y, align 4
  %r.max = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %r.max.x = getelementptr inbounds %Point, %Point* %r.max, i32 0, i32 0
  store i32 5, i32* %r.max.x, align 4
  %r.max2 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %r.max.y = getelementptr inbounds %Point, %Point* %r.max2, i32 0, i32 1
  store i32 8, i32* %r.max.y, align 4
  %"$r" = load %Rect, %Rect* %r, align 4
  store %Rect %"$r", %Rect* %c, align 4
  %"$c" = load %Rect, %Rect* %c, align 4
  %call.size = call i32 @size(%Rect %"$c")
  ret i32 %call.size
}
```

---

## 141 — struct: a struct value {x: 10, y: 11}: missing attributes are zero, nested, returned

```ura
// expect: 39   (10 + 11 + 3 + 0 + 1 + 2 + 5 + 7)
struct Point:
    x i32
    y i32

struct Rect:
    min Point
    max Point

fn origin() Point:
    return {x: 0, y: 0}

fn main() i32:
    p Point = {x: 10, y: 11}
    q Point = {x: 3}
    r Rect = {
        min: {x: 1, y: 2},
        max: {x: 5, y: 7},
    }
    o Point = origin()
    return p.x + p.y + q.x + q.y + r.min.x + r.min.y + r.max.x + r.max.y + o.x
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Point = type { i32, i32 }
%Rect = type { %Point, %Point }

define %Point @origin() {
entry:
  ret %Point zeroinitializer
}

define i32 @main() {
entry:
  %p = alloca %Point, align 8
  %q = alloca %Point, align 8
  %r = alloca %Rect, align 8
  %o = alloca %Point, align 8
  store %Point { i32 10, i32 11 }, %Point* %p, align 4
  store %Point { i32 3, i32 0 }, %Point* %q, align 4
  store %Rect { %Point { i32 1, i32 2 }, %Point { i32 5, i32 7 } }, %Rect* %r, align 4
  %call.origin = call %Point @origin()
  store %Point %call.origin, %Point* %o, align 4
  %p.x = getelementptr inbounds %Point, %Point* %p, i32 0, i32 0
  %p.y = getelementptr inbounds %Point, %Point* %p, i32 0, i32 1
  %"$p.x" = load i32, i32* %p.x, align 4
  %"$p.y" = load i32, i32* %p.y, align 4
  %ADD = add i32 %"$p.x", %"$p.y"
  %q.x = getelementptr inbounds %Point, %Point* %q, i32 0, i32 0
  %"$q.x" = load i32, i32* %q.x, align 4
  %ADD1 = add i32 %ADD, %"$q.x"
  %q.y = getelementptr inbounds %Point, %Point* %q, i32 0, i32 1
  %"$q.y" = load i32, i32* %q.y, align 4
  %ADD2 = add i32 %ADD1, %"$q.y"
  %r.min = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 0
  %r.min.x = getelementptr inbounds %Point, %Point* %r.min, i32 0, i32 0
  %"$r.min.x" = load i32, i32* %r.min.x, align 4
  %ADD3 = add i32 %ADD2, %"$r.min.x"
  %r.min4 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 0
  %r.min.y = getelementptr inbounds %Point, %Point* %r.min4, i32 0, i32 1
  %"$r.min.y" = load i32, i32* %r.min.y, align 4
  %ADD5 = add i32 %ADD3, %"$r.min.y"
  %r.max = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %r.max.x = getelementptr inbounds %Point, %Point* %r.max, i32 0, i32 0
  %"$r.max.x" = load i32, i32* %r.max.x, align 4
  %ADD6 = add i32 %ADD5, %"$r.max.x"
  %r.max7 = getelementptr inbounds %Rect, %Rect* %r, i32 0, i32 1
  %r.max.y = getelementptr inbounds %Point, %Point* %r.max7, i32 0, i32 1
  %"$r.max.y" = load i32, i32* %r.max.y, align 4
  %ADD8 = add i32 %ADD6, %"$r.max.y"
  %o.x = getelementptr inbounds %Point, %Point* %o, i32 0, i32 0
  %"$o.x" = load i32, i32* %o.x, align 4
  %ADD9 = add i32 %ADD8, %"$o.x"
  ret i32 %ADD9
}
```

---

## 142 — struct: a struct value with an unknown attribute

```ura
// error: 'Point' has no attribute 'z'
struct Point:
    x i32
    y i32

fn main() i32:
    p Point = {x: 1, z: 2}
    return p.x
```

### stderr

```
error: test.ura:7:22 'Point' has no attribute 'z'
  |
7 |     p Point = {x: 1, z: 2}
  |                      ^
error: aborting due to 1 error
```

### status

```
1
```

---

## 143 — struct: a struct value given to an i32

```ura
// error: expected 'i32', found a struct value
fn main() i32:
    n i32 = {x: 1}
    return n
```

### stderr

```
error: test.ura:3:13 expected 'i32', found a struct value
  |
3 |     n i32 = {x: 1}
  |             ^
error: aborting due to 1 error
```

### status

```
1
```

