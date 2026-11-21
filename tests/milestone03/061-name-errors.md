# milestone03

## index

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
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Node = type { i32, %Node* }

define %Node @create(i32 %0) {
entry:
  %value = alloca i32, align 4
  %res = alloca %Node, align 8
  store i32 %0, i32* %value, align 4
  store %Node zeroinitializer, %Node* %res, align 8
  %res.value = getelementptr inbounds %Node, %Node* %res, i32 0, i32 0
  %"$value" = load i32, i32* %value, align 4
  store i32 %"$value", i32* %res.value, align 4
  %res.next = getelementptr inbounds %Node, %Node* %res, i32 0, i32 1
  store %Node* null, %Node** %res.next, align 8
  %"$res" = load %Node, %Node* %res, align 8
  ret %Node %"$res"
}

define i32 @main() {
entry:
  %a = alloca %Node, align 8
  %b = alloca %Node, align 8
  %p = alloca %Node*, align 8
  %sum = alloca i32, align 4
  %call.create = call %Node @create(i32 1)
  store %Node %call.create, %Node* %a, align 8
  %call.create1 = call %Node @create(i32 2)
  store %Node %call.create1, %Node* %b, align 8
  %a.next = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  store %Node* %b, %Node** %a.next, align 8
  store %Node* %a, %Node** %p, align 8
  store i32 0, i32* %sum, align 4
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"*p" = load %Node*, %Node** %p, align 8
  %NQ = icmp ne %Node* %"*p", null
  br i1 %NQ, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"*p2" = load %Node*, %Node** %p, align 8
  %"*p.value" = getelementptr inbounds %Node, %Node* %"*p2", i32 0, i32 0
  %"$*p.value" = load i32, i32* %"*p.value", align 4
  %"$sum" = load i32, i32* %sum, align 4
  %ADD = add i32 %"$sum", %"$*p.value"
  store i32 %ADD, i32* %sum, align 4
  %"*p3" = load %Node*, %Node** %p, align 8
  %"*p.next" = getelementptr inbounds %Node, %Node* %"*p3", i32 0, i32 1
  %"$*p.next" = load %Node*, %Node** %"*p.next", align 8
  store %Node* %"$*p.next", %Node** %p, align 8
  br label %while0

endloop:                                          ; preds = %while0
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
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Node = type { i32, %Node* }

define i32 @main() {
entry:
  %c = alloca %Node, align 8
  %b = alloca %Node, align 8
  %a = alloca %Node, align 8
  %sum = alloca i32, align 4
  %p = alloca %Node*, align 8
  store %Node zeroinitializer, %Node* %c, align 8
  %c.value = getelementptr inbounds %Node, %Node* %c, i32 0, i32 0
  store i32 3, i32* %c.value, align 4
  %c.next = getelementptr inbounds %Node, %Node* %c, i32 0, i32 1
  store %Node* null, %Node** %c.next, align 8
  store %Node zeroinitializer, %Node* %b, align 8
  %b.value = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  store i32 2, i32* %b.value, align 4
  %b.next = getelementptr inbounds %Node, %Node* %b, i32 0, i32 1
  store %Node* %c, %Node** %b.next, align 8
  store %Node zeroinitializer, %Node* %a, align 8
  %a.value = getelementptr inbounds %Node, %Node* %a, i32 0, i32 0
  store i32 1, i32* %a.value, align 4
  %a.next = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  store %Node* %b, %Node** %a.next, align 8
  store i32 0, i32* %sum, align 4
  store %Node* %a, %Node** %p, align 8
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"*p" = load %Node*, %Node** %p, align 8
  %NQ = icmp ne %Node* %"*p", null
  br i1 %NQ, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"*p1" = load %Node*, %Node** %p, align 8
  %"*p.value" = getelementptr inbounds %Node, %Node* %"*p1", i32 0, i32 0
  %"$*p.value" = load i32, i32* %"*p.value", align 4
  %"$sum" = load i32, i32* %sum, align 4
  %ADD = add i32 %"$sum", %"$*p.value"
  store i32 %ADD, i32* %sum, align 4
  %"*p2" = load %Node*, %Node** %p, align 8
  %"*p.next" = getelementptr inbounds %Node, %Node* %"*p2", i32 0, i32 1
  %"$*p.next" = load %Node*, %Node** %"*p.next", align 8
  store %Node* %"$*p.next", %Node** %p, align 8
  br label %while0

endloop:                                          ; preds = %while0
  %a.next3 = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  %"*a.next" = load %Node*, %Node** %a.next3, align 8
  %"*a.next.next" = getelementptr inbounds %Node, %Node* %"*a.next", i32 0, i32 1
  %"**a.next.next" = load %Node*, %Node** %"*a.next.next", align 8
  %"**a.next.next.value" = getelementptr inbounds %Node, %Node* %"**a.next.next", i32 0, i32 0
  %"$**a.next.next.value" = load i32, i32* %"**a.next.next.value", align 4
  %MUL = mul i32 %"$**a.next.next.value", 10
  %"$sum4" = load i32, i32* %sum, align 4
  %ADD5 = add i32 %"$sum4", %MUL
  ret i32 %ADD5
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
    for i in Range::create(0, 2):
        struct Tick:
            n i32

        t Tick
    output("ok\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Range = type { i32, i32, i32 }
%Tick = type { i32 }

@str = private unnamed_addr constant [4 x i8] c"ok\0A\00", align 1

define %Range @Range.create(i32 %0, i32 %1) {
entry:
  %start = alloca i32, align 4
  %stop = alloca i32, align 4
  %r = alloca %Range, align 8
  store i32 %0, i32* %start, align 4
  store i32 %1, i32* %stop, align 4
  store %Range zeroinitializer, %Range* %r, align 4
  %r.start = getelementptr inbounds %Range, %Range* %r, i32 0, i32 0
  %"$start" = load i32, i32* %start, align 4
  store i32 %"$start", i32* %r.start, align 4
  %r.stop = getelementptr inbounds %Range, %Range* %r, i32 0, i32 1
  %"$stop" = load i32, i32* %stop, align 4
  store i32 %"$stop", i32* %r.stop, align 4
  %r.step = getelementptr inbounds %Range, %Range* %r, i32 0, i32 2
  store i32 1, i32* %r.step, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"$stop1" = load i32, i32* %stop, align 4
  %"$start2" = load i32, i32* %start, align 4
  %LT = icmp slt i32 %"$stop1", %"$start2"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %r.step3 = getelementptr inbounds %Range, %Range* %r, i32 0, i32 2
  store i32 -1, i32* %r.step3, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  %"$r" = load %Range, %Range* %r, align 4
  ret %Range %"$r"
}

define i1 @Range.in(%Range* %0, i32* %1, i32* %2) {
entry:
  %self = alloca %Range*, align 8
  %i = alloca i32*, align 8
  %x = alloca i32*, align 8
  %done = alloca i1, align 1
  store %Range* %0, %Range** %self, align 8
  store i32* %1, i32** %i, align 8
  store i32* %2, i32** %x, align 8
  %"*x" = load i32*, i32** %x, align 8
  %"*self" = load %Range*, %Range** %self, align 8
  %"*self.start" = getelementptr inbounds %Range, %Range* %"*self", i32 0, i32 0
  %"*i" = load i32*, i32** %i, align 8
  %"*self1" = load %Range*, %Range** %self, align 8
  %"*self.step" = getelementptr inbounds %Range, %Range* %"*self1", i32 0, i32 2
  %"$*i" = load i32, i32* %"*i", align 4
  %"$*self.step" = load i32, i32* %"*self.step", align 4
  %MUL = mul i32 %"$*i", %"$*self.step"
  %"$*self.start" = load i32, i32* %"*self.start", align 4
  %ADD = add i32 %"$*self.start", %MUL
  store i32 %ADD, i32* %"*x", align 4
  %"*self2" = load %Range*, %Range** %self, align 8
  %"*self.step3" = getelementptr inbounds %Range, %Range* %"*self2", i32 0, i32 2
  %"$*self.step4" = load i32, i32* %"*self.step3", align 4
  %GT = icmp sgt i32 %"$*self.step4", 0
  %"*x5" = load i32*, i32** %x, align 8
  %"*self6" = load %Range*, %Range** %self, align 8
  %"*self.stop" = getelementptr inbounds %Range, %Range* %"*self6", i32 0, i32 1
  %"$*x" = load i32, i32* %"*x5", align 4
  %"$*self.stop" = load i32, i32* %"*self.stop", align 4
  %GE = icmp sge i32 %"$*x", %"$*self.stop"
  %AND = and i1 %GT, %GE
  %"*self7" = load %Range*, %Range** %self, align 8
  %"*self.step8" = getelementptr inbounds %Range, %Range* %"*self7", i32 0, i32 2
  %"$*self.step9" = load i32, i32* %"*self.step8", align 4
  %LT = icmp slt i32 %"$*self.step9", 0
  %"*x10" = load i32*, i32** %x, align 8
  %"*self11" = load %Range*, %Range** %self, align 8
  %"*self.stop12" = getelementptr inbounds %Range, %Range* %"*self11", i32 0, i32 1
  %"$*x13" = load i32, i32* %"*x10", align 4
  %"$*self.stop14" = load i32, i32* %"*self.stop12", align 4
  %LE = icmp sle i32 %"$*x13", %"$*self.stop14"
  %AND15 = and i1 %LT, %LE
  %OR = or i1 %AND, %AND15
  store i1 %OR, i1* %done, align 1
  br label %if1

if1:                                              ; preds = %entry
  %"$done" = load i1, i1* %done, align 1
  br i1 %"$done", label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  ret i1 false

endif:                                            ; preds = %if1
  %"*i16" = load i32*, i32** %i, align 8
  %"$*i17" = load i32, i32* %"*i16", align 4
  %ADD18 = add i32 %"$*i17", 1
  store i32 %ADD18, i32* %"*i16", align 4
  ret i1 true
}

define i32 @main() {
entry:
  %tmp = alloca %Range, align 8
  %i = alloca i32, align 4
  %i1 = alloca i32, align 4
  %t = alloca %Tick, align 8
  %call.Range.create = call %Range @Range.create(i32 0, i32 2)
  store %Range %call.Range.create, %Range* %tmp, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %i1, align 4
  br label %while2

while2:                                           ; preds = %while2.then, %entry
  %call.Range.in = call i1 @Range.in(%Range* %tmp, i32* %i, i32* %i1)
  br i1 %call.Range.in, label %while2.then, label %endloop

while2.then:                                      ; preds = %while2
  store %Tick zeroinitializer, %Tick* %t, align 4
  br label %while2

endloop:                                          ; preds = %while2
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

