# milestone03

## index

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
    while not isZero(p):
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
  %"$p" = load %Node*, %Node** %p, align 8
  %EQ = icmp eq %Node* %"$p", null
  %NOT = xor i1 %EQ, true
  br i1 %NOT, label %while0.then, label %endloop

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
    if isZero(t): return 0
    return t.value + sum(&t.left) + sum(&t.right)

fn depth(t &Tree) i32:
    if isZero(t): return 0
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
target triple = "x86_64-unknown-linux-gnu"

%Tree = type { i32, %Tree*, %Tree* }

define i32 @sum(%Tree* %0) {
entry:
  %t = alloca %Tree*, align 8
  store %Tree* %0, %Tree** %t, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*t" = load %Tree*, %Tree** %t, align 8
  %"$t" = load %Tree*, %Tree** %t, align 8
  %EQ = icmp eq %Tree* %"$t", null
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 0

endif:                                            ; preds = %if0
  %"*t1" = load %Tree*, %Tree** %t, align 8
  %"*t.value" = getelementptr inbounds %Tree, %Tree* %"*t1", i32 0, i32 0
  %"*t2" = load %Tree*, %Tree** %t, align 8
  %"*t.left" = getelementptr inbounds %Tree, %Tree* %"*t2", i32 0, i32 1
  %"$*t.left" = load %Tree*, %Tree** %"*t.left", align 8
  %call.sum = call i32 @sum(%Tree* %"$*t.left")
  %"$*t.value" = load i32, i32* %"*t.value", align 4
  %ADD = add i32 %"$*t.value", %call.sum
  %"*t3" = load %Tree*, %Tree** %t, align 8
  %"*t.right" = getelementptr inbounds %Tree, %Tree* %"*t3", i32 0, i32 2
  %"$*t.right" = load %Tree*, %Tree** %"*t.right", align 8
  %call.sum4 = call i32 @sum(%Tree* %"$*t.right")
  %ADD5 = add i32 %ADD, %call.sum4
  ret i32 %ADD5
}

define i32 @depth(%Tree* %0) {
entry:
  %t = alloca %Tree*, align 8
  %l = alloca i32, align 4
  %r = alloca i32, align 4
  store %Tree* %0, %Tree** %t, align 8
  br label %if1

if1:                                              ; preds = %entry
  %"*t" = load %Tree*, %Tree** %t, align 8
  %"$t" = load %Tree*, %Tree** %t, align 8
  %EQ = icmp eq %Tree* %"$t", null
  br i1 %EQ, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  ret i32 0

endif:                                            ; preds = %if1
  %"*t2" = load %Tree*, %Tree** %t, align 8
  %"*t.left" = getelementptr inbounds %Tree, %Tree* %"*t2", i32 0, i32 1
  %"$*t.left" = load %Tree*, %Tree** %"*t.left", align 8
  %call.depth = call i32 @depth(%Tree* %"$*t.left")
  store i32 %call.depth, i32* %l, align 4
  %"*t3" = load %Tree*, %Tree** %t, align 8
  %"*t.right" = getelementptr inbounds %Tree, %Tree* %"*t3", i32 0, i32 2
  %"$*t.right" = load %Tree*, %Tree** %"*t.right", align 8
  %call.depth4 = call i32 @depth(%Tree* %"$*t.right")
  store i32 %call.depth4, i32* %r, align 4
  br label %if2

if2:                                              ; preds = %endif
  %"$l" = load i32, i32* %l, align 4
  %"$r" = load i32, i32* %r, align 4
  %GT = icmp sgt i32 %"$l", %"$r"
  br i1 %GT, label %if2.then, label %endif1

if2.then:                                         ; preds = %if2
  %"$l5" = load i32, i32* %l, align 4
  %ADD = add i32 %"$l5", 1
  ret i32 %ADD

endif1:                                           ; preds = %if2
  %"$r6" = load i32, i32* %r, align 4
  %ADD7 = add i32 %"$r6", 1
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
  %a.value = getelementptr inbounds %Tree, %Tree* %a, i32 0, i32 0
  store i32 1, i32* %a.value, align 4
  %b.value = getelementptr inbounds %Tree, %Tree* %b, i32 0, i32 0
  store i32 2, i32* %b.value, align 4
  %c.value = getelementptr inbounds %Tree, %Tree* %c, i32 0, i32 0
  store i32 3, i32* %c.value, align 4
  %d.value = getelementptr inbounds %Tree, %Tree* %d, i32 0, i32 0
  store i32 4, i32* %d.value, align 4
  %a.left = getelementptr inbounds %Tree, %Tree* %a, i32 0, i32 1
  store %Tree* %b, %Tree** %a.left, align 8
  %a.right = getelementptr inbounds %Tree, %Tree* %a, i32 0, i32 2
  store %Tree* %c, %Tree** %a.right, align 8
  %b.left = getelementptr inbounds %Tree, %Tree* %b, i32 0, i32 1
  store %Tree* %d, %Tree** %b.left, align 8
  %call.sum = call i32 @sum(%Tree* %a)
  %MUL = mul i32 %call.sum, 10
  %call.depth = call i32 @depth(%Tree* %a)
  %ADD = add i32 %MUL, %call.depth
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
    if isZero(n): return 0
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
target triple = "x86_64-unknown-linux-gnu"

%Node = type { i32, %Node* }

define i32 @length(%Node* %0) {
entry:
  %n = alloca %Node*, align 8
  store %Node* %0, %Node** %n, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*n" = load %Node*, %Node** %n, align 8
  %"$n" = load %Node*, %Node** %n, align 8
  %EQ = icmp eq %Node* %"$n", null
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 0

endif:                                            ; preds = %if0
  %"*n1" = load %Node*, %Node** %n, align 8
  %"*n.next" = getelementptr inbounds %Node, %Node* %"*n1", i32 0, i32 1
  %"$*n.next" = load %Node*, %Node** %"*n.next", align 8
  %call.length = call i32 @length(%Node* %"$*n.next")
  %ADD = add i32 1, %call.length
  ret i32 %ADD
}

define i32 @main() {
entry:
  %a = alloca %Node, align 8
  %b = alloca %Node, align 8
  %c = alloca %Node, align 8
  %nodes = alloca %Node*, align 8
  %arr = alloca [3 x %Node], align 8
  %i = alloca i32, align 4
  store %Node zeroinitializer, %Node* %a, align 8
  store %Node zeroinitializer, %Node* %b, align 8
  store %Node zeroinitializer, %Node* %c, align 8
  %"$a" = load %Node, %Node* %a, align 8
  %"$b" = load %Node, %Node* %b, align 8
  %"$c" = load %Node, %Node* %c, align 8
  %0 = insertvalue [3 x %Node] undef, %Node %"$a", 0
  %1 = insertvalue [3 x %Node] %0, %Node %"$b", 1
  %2 = insertvalue [3 x %Node] %1, %Node %"$c", 2
  store [3 x %Node] %2, [3 x %Node]* %arr, align 8
  %3 = getelementptr inbounds [3 x %Node], [3 x %Node]* %arr, i32 0, i32 0
  store %Node* %3, %Node** %nodes, align 8
  store i32 0, i32* %i, align 4
  br label %while1

while1:                                           ; preds = %endif, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
  %"$nodes" = load %Node*, %Node** %nodes, align 8
  %"$i1" = load i32, i32* %i, align 4
  %"nodes[]" = getelementptr inbounds %Node, %Node* %"$nodes", i32 %"$i1"
  %"nodes[].value" = getelementptr inbounds %Node, %Node* %"nodes[]", i32 0, i32 0
  %"$i2" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i2", 1
  %MUL = mul i32 %ADD, 10
  store i32 %MUL, i32* %"nodes[].value", align 4
  br label %if2

if2:                                              ; preds = %while1.then
  %"$i3" = load i32, i32* %i, align 4
  %LT4 = icmp slt i32 %"$i3", 2
  br i1 %LT4, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"$nodes5" = load %Node*, %Node** %nodes, align 8
  %"$i6" = load i32, i32* %i, align 4
  %"nodes[]7" = getelementptr inbounds %Node, %Node* %"$nodes5", i32 %"$i6"
  %"nodes[].next" = getelementptr inbounds %Node, %Node* %"nodes[]7", i32 0, i32 1
  %"$i8" = load i32, i32* %i, align 4
  %ADD9 = add i32 %"$i8", 1
  %"$nodes10" = load %Node*, %Node** %nodes, align 8
  %"nodes[]11" = getelementptr inbounds %Node, %Node* %"$nodes10", i32 %ADD9
  store %Node* %"nodes[]11", %Node** %"nodes[].next", align 8
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  %"$i12" = load i32, i32* %i, align 4
  %ADD13 = add i32 %"$i12", 1
  store i32 %ADD13, i32* %i, align 4
  br label %while1

endloop:                                          ; preds = %while1
  %"$nodes14" = load %Node*, %Node** %nodes, align 8
  %"nodes[]15" = getelementptr inbounds %Node, %Node* %"$nodes14", i32 0
  %call.length = call i32 @length(%Node* %"nodes[]15")
  %"$nodes16" = load %Node*, %Node** %nodes, align 8
  %"nodes[]17" = getelementptr inbounds %Node, %Node* %"$nodes16", i32 0
  %"nodes[].next18" = getelementptr inbounds %Node, %Node* %"nodes[]17", i32 0, i32 1
  %"*nodes[].next" = load %Node*, %Node** %"nodes[].next18", align 8
  %"*nodes[].next.next" = getelementptr inbounds %Node, %Node* %"*nodes[].next", i32 0, i32 1
  %"**nodes[].next.next" = load %Node*, %Node** %"*nodes[].next.next", align 8
  %"**nodes[].next.next.value" = getelementptr inbounds %Node, %Node* %"**nodes[].next.next", i32 0, i32 0
  %"$**nodes[].next.next.value" = load i32, i32* %"**nodes[].next.next.value", align 4
  %ADD19 = add i32 %call.length, %"$**nodes[].next.next.value"
  ret i32 %ADD19
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
    while !isZero(p):
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
target triple = "x86_64-unknown-linux-gnu"

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
  %"*head" = load %Node*, %Node** %head, align 8
  store %Node* %"*head", %Node** %p, align 8
  br label %while0

while0:                                           ; preds = %endif, %entry
  %"*p" = load %Node*, %Node** %p, align 8
  %"$p" = load %Node*, %Node** %p, align 8
  %EQ = icmp eq %Node* %"$p", null
  %NOT = xor i1 %EQ, true
  br i1 %NOT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  br label %if1

if1:                                              ; preds = %while0.then
  %"*p1" = load %Node*, %Node** %p, align 8
  %"*p.value" = getelementptr inbounds %Node, %Node* %"*p1", i32 0, i32 0
  %"$*p.value" = load i32, i32* %"*p.value", align 4
  %"$v" = load i32, i32* %v, align 4
  %EQ2 = icmp eq i32 %"$*p.value", %"$v"
  br i1 %EQ2, label %if1.then, label %endif

if1.then:                                         ; preds = %if1
  %"$i" = load i32, i32* %i, align 4
  ret i32 %"$i"

endif:                                            ; preds = %if1
  %"*p3" = load %Node*, %Node** %p, align 8
  %"*p.next" = getelementptr inbounds %Node, %Node* %"*p3", i32 0, i32 1
  %"$*p.next" = load %Node*, %Node** %"*p.next", align 8
  store %Node* %"$*p.next", %Node** %p, align 8
  %"$i4" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i4", 1
  store i32 %ADD, i32* %i, align 4
  br label %while0

endloop:                                          ; preds = %while0
  ret i32 -1
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
  %a.value = getelementptr inbounds %Node, %Node* %a, i32 0, i32 0
  store i32 5, i32* %a.value, align 4
  %b.value = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  store i32 8, i32* %b.value, align 4
  %c.value = getelementptr inbounds %Node, %Node* %c, i32 0, i32 0
  store i32 13, i32* %c.value, align 4
  %a.next = getelementptr inbounds %Node, %Node* %a, i32 0, i32 1
  store %Node* %b, %Node** %a.next, align 8
  %b.next = getelementptr inbounds %Node, %Node* %b, i32 0, i32 1
  store %Node* %c, %Node** %b.next, align 8
  %call.find = call i32 @find(%Node* %a, i32 13)
  %MUL = mul i32 %call.find, 10
  %call.find1 = call i32 @find(%Node* %a, i32 8)
  %ADD = add i32 %MUL, %call.find1
  store i32 %ADD, i32* %r, align 4
  br label %if2

if2:                                              ; preds = %entry
  %call.find2 = call i32 @find(%Node* %a, i32 99)
  %EQ = icmp eq i32 %call.find2, -1
  br i1 %EQ, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"$r" = load i32, i32* %r, align 4
  %ADD3 = add i32 %"$r", 100
  store i32 %ADD3, i32* %r, align 4
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  %"$r4" = load i32, i32* %r, align 4
  ret i32 %"$r4"
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
    if isZero(t): return 0
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
target triple = "x86_64-unknown-linux-gnu"

%Tree = type { i32, %Tree*, %Tree* }

define i32 @walk(%Tree* %0, i32* %1) {
entry:
  %t = alloca %Tree*, align 8
  %count = alloca i32*, align 8
  store %Tree* %0, %Tree** %t, align 8
  store i32* %1, i32** %count, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*t" = load %Tree*, %Tree** %t, align 8
  %"$t" = load %Tree*, %Tree** %t, align 8
  %EQ = icmp eq %Tree* %"$t", null
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret i32 0

endif:                                            ; preds = %if0
  %"*count" = load i32*, i32** %count, align 8
  %"$*count" = load i32, i32* %"*count", align 4
  %ADD = add i32 %"$*count", 1
  store i32 %ADD, i32* %"*count", align 4
  %"*t1" = load %Tree*, %Tree** %t, align 8
  %"*t.value" = getelementptr inbounds %Tree, %Tree* %"*t1", i32 0, i32 0
  %"*t2" = load %Tree*, %Tree** %t, align 8
  %"*t.left" = getelementptr inbounds %Tree, %Tree* %"*t2", i32 0, i32 1
  %"*count3" = load i32*, i32** %count, align 8
  %"$*t.left" = load %Tree*, %Tree** %"*t.left", align 8
  %call.walk = call i32 @walk(%Tree* %"$*t.left", i32* %"*count3")
  %"$*t.value" = load i32, i32* %"*t.value", align 4
  %ADD4 = add i32 %"$*t.value", %call.walk
  %"*t5" = load %Tree*, %Tree** %t, align 8
  %"*t.right" = getelementptr inbounds %Tree, %Tree* %"*t5", i32 0, i32 2
  %"*count6" = load i32*, i32** %count, align 8
  %"$*t.right" = load %Tree*, %Tree** %"*t.right", align 8
  %call.walk7 = call i32 @walk(%Tree* %"$*t.right", i32* %"*count6")
  %ADD8 = add i32 %ADD4, %call.walk7
  ret i32 %ADD8
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
  %a.value = getelementptr inbounds %Tree, %Tree* %a, i32 0, i32 0
  store i32 5, i32* %a.value, align 4
  %b.value = getelementptr inbounds %Tree, %Tree* %b, i32 0, i32 0
  store i32 6, i32* %b.value, align 4
  %c.value = getelementptr inbounds %Tree, %Tree* %c, i32 0, i32 0
  store i32 7, i32* %c.value, align 4
  %a.left = getelementptr inbounds %Tree, %Tree* %a, i32 0, i32 1
  store %Tree* %b, %Tree** %a.left, align 8
  %b.right = getelementptr inbounds %Tree, %Tree* %b, i32 0, i32 2
  store %Tree* %c, %Tree** %b.right, align 8
  store i32 0, i32* %n, align 4
  %call.walk = call i32 @walk(%Tree* %a, i32* %n)
  store i32 %call.walk, i32* %s, align 4
  %"$s" = load i32, i32* %s, align 4
  %MUL = mul i32 %"$s", 10
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %MUL, %"$n"
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
    if isZero(n): return
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
target triple = "x86_64-unknown-linux-gnu"

%Node = type { i32, %Node* }

@str = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

declare i32 @printf(i8*, ...)

define void @print_rev(%Node* %0) {
entry:
  %n = alloca %Node*, align 8
  store %Node* %0, %Node** %n, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*n" = load %Node*, %Node** %n, align 8
  %"$n" = load %Node*, %Node** %n, align 8
  %EQ = icmp eq %Node* %"$n", null
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret void

endif:                                            ; preds = %if0
  %"*n1" = load %Node*, %Node** %n, align 8
  %"*n.next" = getelementptr inbounds %Node, %Node* %"*n1", i32 0, i32 1
  %"$*n.next" = load %Node*, %Node** %"*n.next", align 8
  call void @print_rev(%Node* %"$*n.next")
  %"*n2" = load %Node*, %Node** %n, align 8
  %"*n.value" = getelementptr inbounds %Node, %Node* %"*n2", i32 0, i32 0
  %"$*n.value" = load i32, i32* %"*n.value", align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i32 %"$*n.value")
  ret void
}

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
  call void @print_rev(%Node* %a)
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %a.value1 = getelementptr inbounds %Node, %Node* %a, i32 0, i32 0
  %b.value2 = getelementptr inbounds %Node, %Node* %b, i32 0, i32 0
  %"$a.value" = load i32, i32* %a.value1, align 4
  %"$b.value" = load i32, i32* %b.value2, align 4
  %ADD = add i32 %"$a.value", %"$b.value"
  %c.value3 = getelementptr inbounds %Node, %Node* %c, i32 0, i32 0
  %"$c.value" = load i32, i32* %c.value3, align 4
  %ADD4 = add i32 %ADD, %"$c.value"
  ret i32 %ADD4
}
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
        while not isZero(p.next):
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
target triple = "x86_64-unknown-linux-gnu"

%Node = type { i32, %Node* }

@a = global %Node zeroinitializer
@b = global %Node zeroinitializer
@c = global %Node zeroinitializer
@d = global %Node zeroinitializer

define %Node* @Node.attach(%Node* %0, %Node* %1) {
entry:
  %self = alloca %Node*, align 8
  %n = alloca %Node*, align 8
  store %Node* %0, %Node** %self, align 8
  store %Node* %1, %Node** %n, align 8
  %"*self" = load %Node*, %Node** %self, align 8
  %"*self.next" = getelementptr inbounds %Node, %Node* %"*self", i32 0, i32 1
  %"*n" = load %Node*, %Node** %n, align 8
  store %Node* %"*n", %Node** %"*self.next", align 8
  %"*n1" = load %Node*, %Node** %n, align 8
  ret %Node* %"*n1"
}

define i32 @Node.length(%Node* %0) {
entry:
  %self = alloca %Node*, align 8
  %n = alloca i32, align 4
  %p = alloca %Node*, align 8
  store %Node* %0, %Node** %self, align 8
  store i32 1, i32* %n, align 4
  %"*self" = load %Node*, %Node** %self, align 8
  store %Node* %"*self", %Node** %p, align 8
  br label %while0

while0:                                           ; preds = %while0.then, %entry
  %"*p" = load %Node*, %Node** %p, align 8
  %"*p.next" = getelementptr inbounds %Node, %Node* %"*p", i32 0, i32 1
  %"$*p.next" = load %Node*, %Node** %"*p.next", align 8
  %EQ = icmp eq %Node* %"$*p.next", null
  %NOT = xor i1 %EQ, true
  br i1 %NOT, label %while0.then, label %endloop

while0.then:                                      ; preds = %while0
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 1
  store i32 %ADD, i32* %n, align 4
  %"*p1" = load %Node*, %Node** %p, align 8
  %"*p.next2" = getelementptr inbounds %Node, %Node* %"*p1", i32 0, i32 1
  %"$*p.next3" = load %Node*, %Node** %"*p.next2", align 8
  store %Node* %"$*p.next3", %Node** %p, align 8
  br label %while0

endloop:                                          ; preds = %while0
  %"$n4" = load i32, i32* %n, align 4
  ret i32 %"$n4"
}

define i32 @main() {
entry:
  %ref = alloca %Node*, align 8
  %ref2 = alloca %Node*, align 8
  %ref5 = alloca %Node*, align 8
  store i32 4, i32* getelementptr inbounds (%Node, %Node* @d, i32 0, i32 0), align 4
  %call.Node.attach = call %Node* @Node.attach(%Node* @a, %Node* @b)
  store %Node* %call.Node.attach, %Node** %ref, align 8
  %"*ref" = load %Node*, %Node** %ref, align 8
  %call.Node.attach1 = call %Node* @Node.attach(%Node* %"*ref", %Node* @c)
  store %Node* %call.Node.attach1, %Node** %ref2, align 8
  %"*ref3" = load %Node*, %Node** %ref2, align 8
  %call.Node.attach4 = call %Node* @Node.attach(%Node* %"*ref3", %Node* @d)
  store %Node* %call.Node.attach4, %Node** %ref5, align 8
  %"*ref6" = load %Node*, %Node** %ref5, align 8
  %call.Node.length = call i32 @Node.length(%Node* @a)
  %MUL = mul i32 %call.Node.length, 10
  %"*a.next" = load %Node*, %Node** getelementptr inbounds (%Node, %Node* @a, i32 0, i32 1), align 8
  %"*a.next.next" = getelementptr inbounds %Node, %Node* %"*a.next", i32 0, i32 1
  %"**a.next.next" = load %Node*, %Node** %"*a.next.next", align 8
  %"**a.next.next.next" = getelementptr inbounds %Node, %Node* %"**a.next.next", i32 0, i32 1
  %"***a.next.next.next" = load %Node*, %Node** %"**a.next.next.next", align 8
  %"***a.next.next.next.value" = getelementptr inbounds %Node, %Node* %"***a.next.next.next", i32 0, i32 0
  %"$***a.next.next.next.value" = load i32, i32* %"***a.next.next.next.value", align 4
  %ADD = add i32 %MUL, %"$***a.next.next.next.value"
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
            if isZero(self.left): self.left = &t
            else: self.left.insert(&t)
        else:
            if isZero(self.right): self.right = &t
            else: self.right.insert(&t)

    fn sum() i32:
        s i32 = self.value
        if not isZero(self.left): s += self.left.sum()
        if not isZero(self.right): s += self.right.sum()
        return s

    fn depth() i32:
        l i32 = 0
        r i32 = 0
        if not isZero(self.left): l = self.left.depth()
        if not isZero(self.right): r = self.right.depth()
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
target triple = "x86_64-unknown-linux-gnu"

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
  br label %if0

if0:                                              ; preds = %entry
  %"*t" = load %Tree*, %Tree** %t, align 8
  %"*t.value" = getelementptr inbounds %Tree, %Tree* %"*t", i32 0, i32 0
  %"*self" = load %Tree*, %Tree** %self, align 8
  %"*self.value" = getelementptr inbounds %Tree, %Tree* %"*self", i32 0, i32 0
  %"$*t.value" = load i32, i32* %"*t.value", align 4
  %"$*self.value" = load i32, i32* %"*self.value", align 4
  %LT = icmp slt i32 %"$*t.value", %"$*self.value"
  br i1 %LT, label %if0.then, label %else3

if0.then:                                         ; preds = %if0
  br label %if1

if1:                                              ; preds = %if0.then
  %"*self3" = load %Tree*, %Tree** %self, align 8
  %"*self.left" = getelementptr inbounds %Tree, %Tree* %"*self3", i32 0, i32 1
  %"$*self.left" = load %Tree*, %Tree** %"*self.left", align 8
  %EQ = icmp eq %Tree* %"$*self.left", null
  br i1 %EQ, label %if1.then, label %else2

if1.then:                                         ; preds = %if1
  %"*self4" = load %Tree*, %Tree** %self, align 8
  %"*self.left5" = getelementptr inbounds %Tree, %Tree* %"*self4", i32 0, i32 1
  %"*t6" = load %Tree*, %Tree** %t, align 8
  store %Tree* %"*t6", %Tree** %"*self.left5", align 8
  br label %endif

else2:                                            ; preds = %if1
  %"*self7" = load %Tree*, %Tree** %self, align 8
  %"*self.left8" = getelementptr inbounds %Tree, %Tree* %"*self7", i32 0, i32 1
  %"**self.left" = load %Tree*, %Tree** %"*self.left8", align 8
  %"*t9" = load %Tree*, %Tree** %t, align 8
  call void @Tree.insert(%Tree* %"**self.left", %Tree* %"*t9")
  br label %endif

endif:                                            ; preds = %else2, %if1.then
  br label %endif2

else3:                                            ; preds = %if0
  br label %if4

if4:                                              ; preds = %else3
  %"*self10" = load %Tree*, %Tree** %self, align 8
  %"*self.right" = getelementptr inbounds %Tree, %Tree* %"*self10", i32 0, i32 2
  %"$*self.right" = load %Tree*, %Tree** %"*self.right", align 8
  %EQ11 = icmp eq %Tree* %"$*self.right", null
  br i1 %EQ11, label %if4.then, label %else5

if4.then:                                         ; preds = %if4
  %"*self12" = load %Tree*, %Tree** %self, align 8
  %"*self.right13" = getelementptr inbounds %Tree, %Tree* %"*self12", i32 0, i32 2
  %"*t14" = load %Tree*, %Tree** %t, align 8
  store %Tree* %"*t14", %Tree** %"*self.right13", align 8
  br label %endif1

else5:                                            ; preds = %if4
  %"*self15" = load %Tree*, %Tree** %self, align 8
  %"*self.right16" = getelementptr inbounds %Tree, %Tree* %"*self15", i32 0, i32 2
  %"**self.right" = load %Tree*, %Tree** %"*self.right16", align 8
  %"*t17" = load %Tree*, %Tree** %t, align 8
  call void @Tree.insert(%Tree* %"**self.right", %Tree* %"*t17")
  br label %endif1

endif1:                                           ; preds = %else5, %if4.then
  br label %endif2

endif2:                                           ; preds = %endif1, %endif
  ret void
}

define i32 @Tree.sum(%Tree* %0) {
entry:
  %self = alloca %Tree*, align 8
  %s = alloca i32, align 4
  store %Tree* %0, %Tree** %self, align 8
  %"*self" = load %Tree*, %Tree** %self, align 8
  %"*self.value" = getelementptr inbounds %Tree, %Tree* %"*self", i32 0, i32 0
  %"$*self.value" = load i32, i32* %"*self.value", align 4
  store i32 %"$*self.value", i32* %s, align 4
  br label %if6

if6:                                              ; preds = %entry
  %"*self2" = load %Tree*, %Tree** %self, align 8
  %"*self.left" = getelementptr inbounds %Tree, %Tree* %"*self2", i32 0, i32 1
  %"$*self.left" = load %Tree*, %Tree** %"*self.left", align 8
  %EQ = icmp eq %Tree* %"$*self.left", null
  %NOT = xor i1 %EQ, true
  br i1 %NOT, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*self3" = load %Tree*, %Tree** %self, align 8
  %"*self.left4" = getelementptr inbounds %Tree, %Tree* %"*self3", i32 0, i32 1
  %"**self.left" = load %Tree*, %Tree** %"*self.left4", align 8
  %call.Tree.sum = call i32 @Tree.sum(%Tree* %"**self.left")
  %"$s" = load i32, i32* %s, align 4
  %ADD = add i32 %"$s", %call.Tree.sum
  store i32 %ADD, i32* %s, align 4
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  br label %if7

if7:                                              ; preds = %endif
  %"*self5" = load %Tree*, %Tree** %self, align 8
  %"*self.right" = getelementptr inbounds %Tree, %Tree* %"*self5", i32 0, i32 2
  %"$*self.right" = load %Tree*, %Tree** %"*self.right", align 8
  %EQ6 = icmp eq %Tree* %"$*self.right", null
  %NOT7 = xor i1 %EQ6, true
  br i1 %NOT7, label %if7.then, label %endif1

if7.then:                                         ; preds = %if7
  %"*self8" = load %Tree*, %Tree** %self, align 8
  %"*self.right9" = getelementptr inbounds %Tree, %Tree* %"*self8", i32 0, i32 2
  %"**self.right" = load %Tree*, %Tree** %"*self.right9", align 8
  %call.Tree.sum10 = call i32 @Tree.sum(%Tree* %"**self.right")
  %"$s11" = load i32, i32* %s, align 4
  %ADD12 = add i32 %"$s11", %call.Tree.sum10
  store i32 %ADD12, i32* %s, align 4
  br label %endif1

endif1:                                           ; preds = %if7.then, %if7
  %"$s13" = load i32, i32* %s, align 4
  ret i32 %"$s13"
}

define i32 @Tree.depth(%Tree* %0) {
entry:
  %self = alloca %Tree*, align 8
  %l = alloca i32, align 4
  %r = alloca i32, align 4
  store %Tree* %0, %Tree** %self, align 8
  store i32 0, i32* %l, align 4
  store i32 0, i32* %r, align 4
  br label %if8

if8:                                              ; preds = %entry
  %"*self" = load %Tree*, %Tree** %self, align 8
  %"*self.left" = getelementptr inbounds %Tree, %Tree* %"*self", i32 0, i32 1
  %"$*self.left" = load %Tree*, %Tree** %"*self.left", align 8
  %EQ = icmp eq %Tree* %"$*self.left", null
  %NOT = xor i1 %EQ, true
  br i1 %NOT, label %if8.then, label %endif

if8.then:                                         ; preds = %if8
  %"*self3" = load %Tree*, %Tree** %self, align 8
  %"*self.left4" = getelementptr inbounds %Tree, %Tree* %"*self3", i32 0, i32 1
  %"**self.left" = load %Tree*, %Tree** %"*self.left4", align 8
  %call.Tree.depth = call i32 @Tree.depth(%Tree* %"**self.left")
  store i32 %call.Tree.depth, i32* %l, align 4
  br label %endif

endif:                                            ; preds = %if8.then, %if8
  br label %if9

if9:                                              ; preds = %endif
  %"*self5" = load %Tree*, %Tree** %self, align 8
  %"*self.right" = getelementptr inbounds %Tree, %Tree* %"*self5", i32 0, i32 2
  %"$*self.right" = load %Tree*, %Tree** %"*self.right", align 8
  %EQ6 = icmp eq %Tree* %"$*self.right", null
  %NOT7 = xor i1 %EQ6, true
  br i1 %NOT7, label %if9.then, label %endif1

if9.then:                                         ; preds = %if9
  %"*self8" = load %Tree*, %Tree** %self, align 8
  %"*self.right9" = getelementptr inbounds %Tree, %Tree* %"*self8", i32 0, i32 2
  %"**self.right" = load %Tree*, %Tree** %"*self.right9", align 8
  %call.Tree.depth10 = call i32 @Tree.depth(%Tree* %"**self.right")
  store i32 %call.Tree.depth10, i32* %r, align 4
  br label %endif1

endif1:                                           ; preds = %if9.then, %if9
  br label %if10

if10:                                             ; preds = %endif1
  %"$l" = load i32, i32* %l, align 4
  %"$r" = load i32, i32* %r, align 4
  %GT = icmp sgt i32 %"$l", %"$r"
  br i1 %GT, label %if10.then, label %endif2

if10.then:                                        ; preds = %if10
  %"$l11" = load i32, i32* %l, align 4
  %ADD = add i32 %"$l11", 1
  ret i32 %ADD

endif2:                                           ; preds = %if10
  %"$r12" = load i32, i32* %r, align 4
  %ADD13 = add i32 %"$r12", 1
  ret i32 %ADD13
}

define i32 @main() {
entry:
  store i32 50, i32* getelementptr inbounds (%Tree, %Tree* @a, i32 0, i32 0), align 4
  store i32 30, i32* getelementptr inbounds (%Tree, %Tree* @b, i32 0, i32 0), align 4
  store i32 70, i32* getelementptr inbounds (%Tree, %Tree* @c, i32 0, i32 0), align 4
  store i32 20, i32* getelementptr inbounds (%Tree, %Tree* @d, i32 0, i32 0), align 4
  store i32 40, i32* getelementptr inbounds (%Tree, %Tree* @e, i32 0, i32 0), align 4
  call void @Tree.insert(%Tree* @a, %Tree* @b)
  call void @Tree.insert(%Tree* @a, %Tree* @c)
  call void @Tree.insert(%Tree* @a, %Tree* @d)
  call void @Tree.insert(%Tree* @a, %Tree* @e)
  %call.Tree.sum = call i32 @Tree.sum(%Tree* @a)
  %call.Tree.depth = call i32 @Tree.depth(%Tree* @a)
  %ADD = add i32 %call.Tree.sum, %call.Tree.depth
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

