# milestone09

## index

- 028 — struct: pub inside struct must be followed by fn
- 029 — struct: instance method called via :: instead of .
- 030 — struct: pub fn called via . instead of ::
- 031 — struct: pub may only be used inside a struct
- 032 — operator: [] returns &i32, so b[0] reads and writes through
- 033 — operator: drop runs at the end of blocks, on continue, and not on a returned local
- 034 — operator: copying a struct with drop needs operator =
- 035 — operator: passing a struct with drop by value
- 036 — struct: from takes attributes and methods of a template, a struct and a grandparent; overrides win
- 037 — struct: from a struct that doesn't exist
- 038 — struct: String from Array<char> in "@/string", temporaries are dropped
- 039 — struct: a plain fn drop() is a normal method, only operator drop runs by itself

---

## 028 — struct: pub inside struct must be followed by fn

```ura
// error: Expected 'fn' after 'pub'
use "@/header"

struct Counter:
    n i32

    pub n i32

fn main() i32:
    io::out("hi\n")
    return 0
```

---

## 029 — struct: instance method called via :: instead of .

```ura
// error: 'Counter::bump' needs a receiver; call it on a value, or declare it 'pub fn' to make it static
use "@/header"

struct Counter:
    n i32

    fn bump():
        self.n += 1

fn main() i32:
    c Counter
    c.n = 0
    Counter::bump()
    return 0
```

### stderr

```
error: test.ura:13:14 'Counter' has no pub fn 'bump'
   |
13 |     Counter::bump()
   |              ^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 030 — struct: pub fn called via . instead of ::

```ura
// error: 'm.make' is a 'pub fn', so it is static and takes no receiver; call it as 'Maker::create()'
use "@/header"

struct Maker:
    n i32

    pub fn create() Maker:
        m Maker
        m.n = 1
        return m

fn main() i32:
    m Maker
    m.n = 0
    m.make()
    return 0
```

### stderr

```
error: test.ura:15:7 'Maker' has no method 'make'
   |
15 |     m.make()
   |       ^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 031 — struct: pub may only be used inside a struct

```ura
// error: 'pub' marks a static method; it may only be used inside a struct
use "@/header"

pub fn helper():
    return

fn main() i32:
    helper()
    return 0
```

### stderr

```
error: test.ura:4:5 'pub' may only be used inside a struct
  |
4 | pub fn helper():
  |     ^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 032 — operator: [] returns &i32, so b[0] reads and writes through

```ura
proto malloc(size i32) i32[]

struct Bag:
    data i32[]

    operator [](index i32) &i32:
        return &self.data[index]

fn main() i32:
    b Bag
    b.data = malloc(40)
    b[0] = 5
    b[1] = b[0] + 2
    b[1] += 1
    io::out($"{b[0]} {b[1]}\n")
    return b[1]
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }
%Bag = type { i32* }

@str = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  ret void

endif1:                                           ; preds = %if1
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if2

if2:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %while4

while4:                                           ; preds = %while4.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.count5" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.count9" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while4

endloop:                                          ; preds = %while4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
  ret void
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
  ret void
}

declare i32* @malloc(i32)

define i32* @"Bag.[]"(%Bag* %0, i32 %1) {
entry:
  %self = alloca %Bag*, align 8
  %index = alloca i32, align 4
  store %Bag* %0, %Bag** %self, align 8
  store i32 %1, i32* %index, align 4
  %"*self" = load %Bag*, %Bag** %self, align 8
  %"*self.data" = getelementptr inbounds %Bag, %Bag* %"*self", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$index"
  ret i32* %"*self.data[]"
}

define i32 @main() {
entry:
  %b = alloca %Bag, align 8
  %ref = alloca i32*, align 8
  %ref2 = alloca i32*, align 8
  %ref5 = alloca i32*, align 8
  %ref8 = alloca i32*, align 8
  %ref13 = alloca i32*, align 8
  %ref16 = alloca i32*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %ref27 = alloca i32*, align 8
  store %Bag zeroinitializer, %Bag* %b, align 8
  %b.data = getelementptr inbounds %Bag, %Bag* %b, i32 0, i32 0
  %call.malloc = call i32* @malloc(i32 40)
  store i32* %call.malloc, i32** %b.data, align 8
  %"call.Bag.[]" = call i32* @"Bag.[]"(%Bag* %b, i32 0)
  store i32* %"call.Bag.[]", i32** %ref, align 8
  %"*ref" = load i32*, i32** %ref, align 8
  store i32 5, i32* %"*ref", align 4
  %"call.Bag.[]1" = call i32* @"Bag.[]"(%Bag* %b, i32 1)
  store i32* %"call.Bag.[]1", i32** %ref2, align 8
  %"*ref3" = load i32*, i32** %ref2, align 8
  %"call.Bag.[]4" = call i32* @"Bag.[]"(%Bag* %b, i32 0)
  store i32* %"call.Bag.[]4", i32** %ref5, align 8
  %"*ref6" = load i32*, i32** %ref5, align 8
  %"$*ref" = load i32, i32* %"*ref6", align 4
  %ADD = add i32 %"$*ref", 2
  store i32 %ADD, i32* %"*ref3", align 4
  %"call.Bag.[]7" = call i32* @"Bag.[]"(%Bag* %b, i32 1)
  store i32* %"call.Bag.[]7", i32** %ref8, align 8
  %"*ref9" = load i32*, i32** %ref8, align 8
  %"$*ref10" = load i32, i32* %"*ref9", align 4
  %ADD11 = add i32 %"$*ref10", 1
  store i32 %ADD11, i32* %"*ref9", align 4
  %"call.Bag.[]12" = call i32* @"Bag.[]"(%Bag* %b, i32 0)
  store i32* %"call.Bag.[]12", i32** %ref13, align 8
  %"*ref14" = load i32*, i32** %ref13, align 8
  %"call.Bag.[]15" = call i32* @"Bag.[]"(%Bag* %b, i32 1)
  store i32* %"call.Bag.[]15", i32** %ref16, align 8
  %"*ref17" = load i32*, i32** %ref16, align 8
  %"$*ref18" = load i32, i32* %"*ref14", align 4
  %"$*ref19" = load i32, i32* %"*ref17", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %"$*ref18", i32 %"$*ref19")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD20 = add i32 %"$format.size", 1
  %AS = sext i32 %ADD20 to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf21 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %"$*ref18", i32 %"$*ref19")
  %"$format.size22" = load i32, i32* %format.size, align 4
  %ADD23 = add i32 %"$format.size22", 1
  %"$format.text24" = load i8*, i8** %format.text, align 8
  %"$format.size25" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text24", 0
  %1 = insertvalue %String %0, i32 %"$format.size25", 1
  %2 = insertvalue %String %1, i32 %ADD23, 2
  call void @"io::out"(%String %2)
  %"call.Bag.[]26" = call i32* @"Bag.[]"(%Bag* %b, i32 1)
  store i32* %"call.Bag.[]26", i32** %ref27, align 8
  %"*ref28" = load i32*, i32** %ref27, align 8
  %"$*ref29" = load i32, i32* %"*ref28", align 4
  ret i32 %"$*ref29"
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

---

## 033 — operator: drop runs at the end of blocks, on continue, and not on a returned local

```ura
struct Noisy:
    id i32

    operator drop():
        io::out($"drop {self.id}\n")

fn make(id i32) Noisy:
    n Noisy
    n.id = id
    return n

fn main() i32:
    a Noisy = make(1)
    i i32 = 0
    while i < 3:
        b Noisy
        b.id = 10 + i
        i += 1
        if i == 2:
            continue
        io::out($"loop {i}\n")
    if a.id == 1:
        c Noisy
        c.id = 5
    x Array<i32>
    x.append(4)
    y Array<i32> = x
    y.append(5)
    io::out($"{x.size()} {y.size()} {y[1]}\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }
%Noisy = type { i32 }
%"Array<i32>" = type { i32*, i32, i32 }

@str = private unnamed_addr constant [9 x i8] c"drop %d\0A\00", align 1
@str.1 = private unnamed_addr constant [9 x i8] c"loop %d\0A\00", align 1
@str.2 = private unnamed_addr constant [10 x i8] c"%d %d %d\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  ret void

endif1:                                           ; preds = %if1
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if2

if2:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %while4

while4:                                           ; preds = %while4.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.count5" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.count9" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while4

endloop:                                          ; preds = %while4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
  ret void
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
  ret void
}

define void @Noisy.drop(%Noisy* %0) {
entry:
  %self = alloca %Noisy*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Noisy* %0, %Noisy** %self, align 8
  %"*self" = load %Noisy*, %Noisy** %self, align 8
  %"*self.id" = getelementptr inbounds %Noisy, %Noisy* %"*self", i32 0, i32 0
  %"$*self.id" = load i32, i32* %"*self.id", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %"$*self.id")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %"$*self.id")
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %1 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %2 = insertvalue %String %1, i32 %"$format.size5", 1
  %3 = insertvalue %String %2, i32 %ADD3, 2
  call void @"io::out"(%String %3)
  ret void
}

define %Noisy @make(i32 %0) {
entry:
  %id = alloca i32, align 4
  %n = alloca %Noisy, align 8
  store i32 %0, i32* %id, align 4
  store %Noisy zeroinitializer, %Noisy* %n, align 4
  %n.id = getelementptr inbounds %Noisy, %Noisy* %n, i32 0, i32 0
  %"$id" = load i32, i32* %id, align 4
  store i32 %"$id", i32* %n.id, align 4
  %"$n" = load %Noisy, %Noisy* %n, align 4
  ret %Noisy %"$n"
}

define i32* @"new<i32>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to i32*
  ret i32* %AS1
}

define void @"copy<i32>"(i32* %0, i32* %1, i32 %2) {
entry:
  %dst = alloca i32*, align 8
  %src = alloca i32*, align 8
  %count = alloca i32, align 4
  store i32* %0, i32** %dst, align 8
  store i32* %1, i32** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i32*, i32** %dst, align 8
  %AS = bitcast i32* %"$dst" to i8*
  %"$src" = load i32*, i32** %src, align 8
  %AS1 = bitcast i32* %"$src" to i8*
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i32)
  %AS2 = sext i32 %MUL to i64
  %MUL3 = mul i64 %AS2, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %AS, i8* %AS1, i64 %MUL3)
  ret void
}

define void @"free<i32>"(i32* %0) {
entry:
  %data = alloca i32*, align 8
  store i32* %0, i32** %data, align 8
  %"$data" = load i32*, i32** %data, align 8
  %AS = bitcast i32* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define void @"Array<i32>.reserve"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i32*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if6

if6:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  br label %if7

if7:                                              ; preds = %endif
  %"*self6" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if7.then, label %endif1

if7.then:                                         ; preds = %if7
  ret void

endif1:                                           ; preds = %if7
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i32>" = call i32* @"new<i32>"(i32 %"$cap8")
  store i32* %"call.new<i32>", i32** %bigger, align 8
  %"*self9" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self9", i32 0, i32 0
  %"*self10" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self10", i32 0, i32 1
  %"$bigger" = load i32*, i32** %bigger, align 8
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i32>"(i32* %"$bigger", i32* %"$*self.data", i32 %"$*self.count12")
  br label %if8

if8:                                              ; preds = %endif1
  %"*self13" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap14" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if8.then, label %endif2

if8.then:                                         ; preds = %if8
  %"*self16" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data17" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i32*, i32** %"*self.data17", align 8
  call void @"free<i32>"(i32* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if8.then, %if8
  %"*self19" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data20" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self19", i32 0, i32 0
  %"$bigger21" = load i32*, i32** %bigger, align 8
  store i32* %"$bigger21", i32** %"*self.data20", align 8
  %"*self22" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap23" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @"Array<i32>.own"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  br label %if9

if9:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %AND = and i1 %EQ, %GT
  br i1 %AND, label %if9.then, label %endif

if9.then:                                         ; preds = %if9
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self2", i32 %"$*self.count5")
  br label %endif

endif:                                            ; preds = %if9.then, %if9
  ret void
}

define void @"Array<i32>.clear"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  br label %if10

if10:                                             ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if10.then, label %endif

if10.then:                                        ; preds = %if10
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if10.then, %if10
  br label %while11

while11:                                          ; preds = %while11.then, %endif
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while11.then, label %endloop

while11.then:                                     ; preds = %while11
  %"*self4" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$*self.count10"
  store i32 0, i32* %"*self.data[]", align 4
  br label %while11

endloop:                                          ; preds = %while11
  ret void
}

define void @"Array<i32>.append"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %value = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %value, align 4
  br label %if12

if12:                                             ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if12.then, label %endif

if12.then:                                        ; preds = %if12
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if12.then, %if12
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

define i32 @"Array<i32>.size"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i32* @"Array<i32>.[]"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %index = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %index, align 4
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  call void @"Array<i32>.own"(%"Array<i32>"* %"*self")
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$index"
  ret i32* %"*self.data[]"
}

define void @"Array<i32>.="(%"Array<i32>"* %0, %"Array<i32>"* %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %other = alloca %"Array<i32>"*, align 8
  %i = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store %"Array<i32>"* %1, %"Array<i32>"** %other, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  call void @"Array<i32>.clear"(%"Array<i32>"* %"*self")
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*other" = load %"Array<i32>"*, %"Array<i32>"** %other, align 8
  %"*other.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*other", i32 0, i32 1
  %"$*other.count" = load i32, i32* %"*other.count", align 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self1", i32 %"$*other.count")
  store i32 0, i32* %i, align 4
  br label %while13

while13:                                          ; preds = %while13.then, %entry
  %"*other2" = load %"Array<i32>"*, %"Array<i32>"** %other, align 8
  %"*other.count3" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*other2", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*other.count4" = load i32, i32* %"*other.count3", align 4
  %LT = icmp slt i32 %"$i", %"$*other.count4"
  br i1 %LT, label %while13.then, label %endloop

while13.then:                                     ; preds = %while13
  %"*self5" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self5", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$i6" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$i6"
  %"*other7" = load %"Array<i32>"*, %"Array<i32>"** %other, align 8
  %"*other.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*other7", i32 0, i32 0
  %"$*other.data" = load i32*, i32** %"*other.data", align 8
  %"$i8" = load i32, i32* %i, align 4
  %"*other.data[]" = getelementptr inbounds i32, i32* %"$*other.data", i32 %"$i8"
  %"$*other.data[]" = load i32, i32* %"*other.data[]", align 4
  store i32 %"$*other.data[]", i32* %"*self.data[]", align 4
  %"$i9" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i9", 1
  store i32 %ADD, i32* %i, align 4
  br label %while13

endloop:                                          ; preds = %while13
  %"*self10" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self10", i32 0, i32 1
  %"*other11" = load %"Array<i32>"*, %"Array<i32>"** %other, align 8
  %"*other.count12" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*other11", i32 0, i32 1
  %"$*other.count13" = load i32, i32* %"*other.count12", align 4
  store i32 %"$*other.count13", i32* %"*self.count", align 4
  ret void
}

define void @"Array<i32>.drop"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  call void @"Array<i32>.clear"(%"Array<i32>"* %"*self")
  br label %if14

if14:                                             ; preds = %entry
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if14.then, label %endif

if14.then:                                        ; preds = %if14
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  call void @"free<i32>"(i32* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if14.then, %if14
  ret void
}

define i32 @main() {
entry:
  %a = alloca %Noisy, align 8
  %i = alloca i32, align 4
  %b = alloca %Noisy, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %c = alloca %Noisy, align 8
  %x = alloca %"Array<i32>", align 8
  %y = alloca %"Array<i32>", align 8
  %copy = alloca %"Array<i32>", align 8
  %ref = alloca i32*, align 8
  %format.size15 = alloca i32, align 4
  %format.text20 = alloca i8*, align 8
  %call.make = call %Noisy @make(i32 1)
  store %Noisy %call.make, %Noisy* %a, align 4
  store i32 0, i32* %i, align 4
  br label %while15

while15:                                          ; preds = %endif, %if16.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while15.then, label %endloop

while15.then:                                     ; preds = %while15
  store %Noisy zeroinitializer, %Noisy* %b, align 4
  %b.id = getelementptr inbounds %Noisy, %Noisy* %b, i32 0, i32 0
  %"$i2" = load i32, i32* %i, align 4
  %ADD = add i32 10, %"$i2"
  store i32 %ADD, i32* %b.id, align 4
  %"$i3" = load i32, i32* %i, align 4
  %ADD4 = add i32 %"$i3", 1
  store i32 %ADD4, i32* %i, align 4
  br label %if16

if16:                                             ; preds = %while15.then
  %"$i5" = load i32, i32* %i, align 4
  %EQ = icmp eq i32 %"$i5", 2
  br i1 %EQ, label %if16.then, label %endif

if16.then:                                        ; preds = %if16
  call void @Noisy.drop(%Noisy* %b)
  br label %while15

endif:                                            ; preds = %if16
  %"$i6" = load i32, i32* %i, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0), i32 %"$i6")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD7 = add i32 %"$format.size", 1
  %AS = sext i32 %ADD7 to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf8 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0), i32 %"$i6")
  %"$format.size9" = load i32, i32* %format.size, align 4
  %ADD10 = add i32 %"$format.size9", 1
  %"$format.text11" = load i8*, i8** %format.text, align 8
  %"$format.size12" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text11", 0
  %1 = insertvalue %String %0, i32 %"$format.size12", 1
  %2 = insertvalue %String %1, i32 %ADD10, 2
  call void @"io::out"(%String %2)
  call void @Noisy.drop(%Noisy* %b)
  br label %while15

endloop:                                          ; preds = %while15
  br label %if17

if17:                                             ; preds = %endloop
  %a.id = getelementptr inbounds %Noisy, %Noisy* %a, i32 0, i32 0
  %"$a.id" = load i32, i32* %a.id, align 4
  %EQ13 = icmp eq i32 %"$a.id", 1
  br i1 %EQ13, label %if17.then, label %endif1

if17.then:                                        ; preds = %if17
  store %Noisy zeroinitializer, %Noisy* %c, align 4
  %c.id = getelementptr inbounds %Noisy, %Noisy* %c, i32 0, i32 0
  store i32 5, i32* %c.id, align 4
  call void @Noisy.drop(%Noisy* %c)
  br label %endif1

endif1:                                           ; preds = %if17.then, %if17
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %x, align 8
  call void @"Array<i32>.append"(%"Array<i32>"* %x, i32 4)
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %copy, align 8
  call void @"Array<i32>.="(%"Array<i32>"* %copy, %"Array<i32>"* %x)
  %"$copy" = load %"Array<i32>", %"Array<i32>"* %copy, align 8
  store %"Array<i32>" %"$copy", %"Array<i32>"* %y, align 8
  call void @"Array<i32>.append"(%"Array<i32>"* %y, i32 5)
  %"call.Array<i32>.size" = call i32 @"Array<i32>.size"(%"Array<i32>"* %x)
  %"call.Array<i32>.size14" = call i32 @"Array<i32>.size"(%"Array<i32>"* %y)
  %"call.Array<i32>.[]" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %y, i32 1)
  store i32* %"call.Array<i32>.[]", i32** %ref, align 8
  %"*ref" = load i32*, i32** %ref, align 8
  %"$*ref" = load i32, i32* %"*ref", align 4
  %call.snprintf16 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.2, i32 0, i32 0), i32 %"call.Array<i32>.size", i32 %"call.Array<i32>.size14", i32 %"$*ref")
  store i32 %call.snprintf16, i32* %format.size15, align 4
  %"$format.size17" = load i32, i32* %format.size15, align 4
  %ADD18 = add i32 %"$format.size17", 1
  %AS19 = sext i32 %ADD18 to i64
  %call.calloc21 = call i8* @calloc(i64 %AS19, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc21, i8** %format.text20, align 8
  %"$format.text22" = load i8*, i8** %format.text20, align 8
  %call.snprintf23 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text22", i64 %AS19, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.2, i32 0, i32 0), i32 %"call.Array<i32>.size", i32 %"call.Array<i32>.size14", i32 %"$*ref")
  %"$format.size24" = load i32, i32* %format.size15, align 4
  %ADD25 = add i32 %"$format.size24", 1
  %"$format.text26" = load i8*, i8** %format.text20, align 8
  %"$format.size27" = load i32, i32* %format.size15, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text26", 0
  %4 = insertvalue %String %3, i32 %"$format.size27", 1
  %5 = insertvalue %String %4, i32 %ADD25, 2
  call void @"io::out"(%String %5)
  call void @"Array<i32>.drop"(%"Array<i32>"* %y)
  call void @"Array<i32>.drop"(%"Array<i32>"* %x)
  call void @Noisy.drop(%Noisy* %a)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

---

## 034 — operator: copying a struct with drop needs operator =

```ura
struct Buffer:
    size i32

    operator drop():
        io::out("drop\n")

fn main() i32:
    a Buffer
    b Buffer = a
    return 0
```

### stderr

```
error: test.ura:9:16 'Buffer' can't be copied: it needs a drop and has no operator =
  |
9 |     b Buffer = a
  |                ^
help: define operator =(other &Buffer), or take it as &Buffer
error: aborting due to 1 error
```

### status

```
1
```

---

## 035 — operator: passing a struct with drop by value

```ura
struct Buffer:
    size i32

    operator drop():
        io::out("drop\n")

fn take(b Buffer) i32:
    return b.size

fn main() i32:
    a Buffer
    return take(a)
```

### stderr

```
error: test.ura:12:17 'Buffer' can't be copied: it needs a drop and has no operator =
   |
12 |     return take(a)
   |                 ^
help: define operator =(other &Buffer), or take it as &Buffer
error: aborting due to 1 error
```

### status

```
1
```

---

## 036 — struct: from takes attributes and methods of a template, a struct and a grandparent; overrides win

```ura
struct Named:
    label i8[]

    fn name() i8[]:
        return self.label

struct Text from Array<char> and Named:
    words i32

    fn push_back(c char):
        if self.count == self.cap:
            self.reserve(self.cap * 2 + 4)
        self.data[self.count] = c
        self.count += 1
        if c == ' ':
            self.words += 1

    fn shout() Text:
        res Text
        i i32 = 0
        while i < self.size():
            c char = self[i]
            if c >= 'a' and c <= 'z':
                c = (c as i32 - 32) as char
            res.append(c)
            i += 1
        return res

struct Title from Text:
    fn first() char:
        return self[0]

fn main() i32:
    t Text
    t.label = "greeting"
    t.append('h')
    t.append('i')
    t.append(' ')
    t.append('u')
    loud Text = t.shout()
    title Title
    title.append('z')
    io::out($"{t.size()} {t.words} {loud[0]}{loud[1]}{loud[3]} {t.name()} {title.first()} {typeof(loud)}\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }
%Text = type { i8*, i32, i32, i8*, i32 }
%Title = type { i8*, i32, i32, i8*, i32 }

@str = private unnamed_addr constant [9 x i8] c"greeting\00", align 1
@TYPEOF = private unnamed_addr constant [5 x i8] c"Text\00", align 1
@str.1 = private unnamed_addr constant [23 x i8] c"%d %d %c%c%c %s %c %s\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  ret void

endif1:                                           ; preds = %if1
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if2

if2:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %while4

while4:                                           ; preds = %while4.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.count5" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.count9" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while4

endloop:                                          ; preds = %while4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
  ret void
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
  ret void
}

define void @Text.reserve(%Text* %0, i32 %1) {
entry:
  %self = alloca %Text*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %Text* %0, %Text** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if6

if6:                                              ; preds = %entry
  %"*self" = load %Text*, %Text** %self, align 8
  %"*self.count" = getelementptr inbounds %Text, %Text* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*self3" = load %Text*, %Text** %self, align 8
  %"*self.count4" = getelementptr inbounds %Text, %Text* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  br label %if7

if7:                                              ; preds = %endif
  %"*self6" = load %Text*, %Text** %self, align 8
  %"*self.cap" = getelementptr inbounds %Text, %Text* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if7.then, label %endif1

if7.then:                                         ; preds = %if7
  ret void

endif1:                                           ; preds = %if7
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %Text*, %Text** %self, align 8
  %"*self.data" = getelementptr inbounds %Text, %Text* %"*self9", i32 0, i32 0
  %"*self10" = load %Text*, %Text** %self, align 8
  %"*self.count11" = getelementptr inbounds %Text, %Text* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if8

if8:                                              ; preds = %endif1
  %"*self13" = load %Text*, %Text** %self, align 8
  %"*self.cap14" = getelementptr inbounds %Text, %Text* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if8.then, label %endif2

if8.then:                                         ; preds = %if8
  %"*self16" = load %Text*, %Text** %self, align 8
  %"*self.data17" = getelementptr inbounds %Text, %Text* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if8.then, %if8
  %"*self19" = load %Text*, %Text** %self, align 8
  %"*self.data20" = getelementptr inbounds %Text, %Text* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %Text*, %Text** %self, align 8
  %"*self.cap23" = getelementptr inbounds %Text, %Text* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @Text.own(%Text* %0) {
entry:
  %self = alloca %Text*, align 8
  store %Text* %0, %Text** %self, align 8
  br label %if9

if9:                                              ; preds = %entry
  %"*self" = load %Text*, %Text** %self, align 8
  %"*self.cap" = getelementptr inbounds %Text, %Text* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  %"*self1" = load %Text*, %Text** %self, align 8
  %"*self.count" = getelementptr inbounds %Text, %Text* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %AND = and i1 %EQ, %GT
  br i1 %AND, label %if9.then, label %endif

if9.then:                                         ; preds = %if9
  %"*self2" = load %Text*, %Text** %self, align 8
  %"*self3" = load %Text*, %Text** %self, align 8
  %"*self.count4" = getelementptr inbounds %Text, %Text* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  call void @Text.reserve(%Text* %"*self2", i32 %"$*self.count5")
  br label %endif

endif:                                            ; preds = %if9.then, %if9
  ret void
}

define void @Text.clear(%Text* %0) {
entry:
  %self = alloca %Text*, align 8
  store %Text* %0, %Text** %self, align 8
  br label %if10

if10:                                             ; preds = %entry
  %"*self" = load %Text*, %Text** %self, align 8
  %"*self.cap" = getelementptr inbounds %Text, %Text* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if10.then, label %endif

if10.then:                                        ; preds = %if10
  %"*self1" = load %Text*, %Text** %self, align 8
  %"*self.count" = getelementptr inbounds %Text, %Text* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if10.then, %if10
  br label %while11

while11:                                          ; preds = %while11.then, %endif
  %"*self2" = load %Text*, %Text** %self, align 8
  %"*self.count3" = getelementptr inbounds %Text, %Text* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while11.then, label %endloop

while11.then:                                     ; preds = %while11
  %"*self4" = load %Text*, %Text** %self, align 8
  %"*self.count5" = getelementptr inbounds %Text, %Text* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %Text*, %Text** %self, align 8
  %"*self.data" = getelementptr inbounds %Text, %Text* %"*self7", i32 0, i32 0
  %"*self8" = load %Text*, %Text** %self, align 8
  %"*self.count9" = getelementptr inbounds %Text, %Text* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while11

endloop:                                          ; preds = %while11
  ret void
}

define void @Text.append(%Text* %0, i8 %1) {
entry:
  %self = alloca %Text*, align 8
  %value = alloca i8, align 1
  store %Text* %0, %Text** %self, align 8
  store i8 %1, i8* %value, align 1
  br label %if12

if12:                                             ; preds = %entry
  %"*self" = load %Text*, %Text** %self, align 8
  %"*self.count" = getelementptr inbounds %Text, %Text* %"*self", i32 0, i32 1
  %"*self1" = load %Text*, %Text** %self, align 8
  %"*self.cap" = getelementptr inbounds %Text, %Text* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if12.then, label %endif

if12.then:                                        ; preds = %if12
  %"*self2" = load %Text*, %Text** %self, align 8
  %"*self3" = load %Text*, %Text** %self, align 8
  %"*self.count4" = getelementptr inbounds %Text, %Text* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @Text.reserve(%Text* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if12.then, %if12
  %"*self6" = load %Text*, %Text** %self, align 8
  %"*self.data" = getelementptr inbounds %Text, %Text* %"*self6", i32 0, i32 0
  %"*self7" = load %Text*, %Text** %self, align 8
  %"*self.count8" = getelementptr inbounds %Text, %Text* %"*self7", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count9"
  %"$value" = load i8, i8* %value, align 1
  store i8 %"$value", i8* %"*self.data[]", align 1
  %"*self10" = load %Text*, %Text** %self, align 8
  %"*self.count11" = getelementptr inbounds %Text, %Text* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  ret void
}

define i32 @Text.size(%Text* %0) {
entry:
  %self = alloca %Text*, align 8
  store %Text* %0, %Text** %self, align 8
  %"*self" = load %Text*, %Text** %self, align 8
  %"*self.count" = getelementptr inbounds %Text, %Text* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i8* @"Text.[]"(%Text* %0, i32 %1) {
entry:
  %self = alloca %Text*, align 8
  %index = alloca i32, align 4
  store %Text* %0, %Text** %self, align 8
  store i32 %1, i32* %index, align 4
  %"*self" = load %Text*, %Text** %self, align 8
  call void @Text.own(%Text* %"*self")
  %"*self1" = load %Text*, %Text** %self, align 8
  %"*self.data" = getelementptr inbounds %Text, %Text* %"*self1", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$index"
  ret i8* %"*self.data[]"
}

define void @Text.drop(%Text* %0) {
entry:
  %self = alloca %Text*, align 8
  store %Text* %0, %Text** %self, align 8
  %"*self" = load %Text*, %Text** %self, align 8
  call void @Text.clear(%Text* %"*self")
  br label %if13

if13:                                             ; preds = %entry
  %"*self1" = load %Text*, %Text** %self, align 8
  %"*self.cap" = getelementptr inbounds %Text, %Text* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if13.then, label %endif

if13.then:                                        ; preds = %if13
  %"*self2" = load %Text*, %Text** %self, align 8
  %"*self.data" = getelementptr inbounds %Text, %Text* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if13.then, %if13
  ret void
}

define i8* @Text.name(%Text* %0) {
entry:
  %self = alloca %Text*, align 8
  store %Text* %0, %Text** %self, align 8
  %"*self" = load %Text*, %Text** %self, align 8
  %"*self.label" = getelementptr inbounds %Text, %Text* %"*self", i32 0, i32 3
  %"$*self.label" = load i8*, i8** %"*self.label", align 8
  ret i8* %"$*self.label"
}

define %Text @Text.shout(%Text* %0) {
entry:
  %self = alloca %Text*, align 8
  %res = alloca %Text, align 8
  %i = alloca i32, align 4
  %c = alloca i8, align 1
  %ref = alloca i8*, align 8
  store %Text* %0, %Text** %self, align 8
  store %Text zeroinitializer, %Text* %res, align 8
  store i32 0, i32* %i, align 4
  br label %while14

while14:                                          ; preds = %endif, %entry
  %"*self" = load %Text*, %Text** %self, align 8
  %call.Text.size = call i32 @Text.size(%Text* %"*self")
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", %call.Text.size
  br i1 %LT, label %while14.then, label %endloop

while14.then:                                     ; preds = %while14
  %"*self1" = load %Text*, %Text** %self, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"call.Text.[]" = call i8* @"Text.[]"(%Text* %"*self1", i32 %"$i2")
  store i8* %"call.Text.[]", i8** %ref, align 8
  %"*ref" = load i8*, i8** %ref, align 8
  %"$*ref" = load i8, i8* %"*ref", align 1
  store i8 %"$*ref", i8* %c, align 1
  br label %if15

if15:                                             ; preds = %while14.then
  %"$c" = load i8, i8* %c, align 1
  %GE = icmp sge i8 %"$c", 97
  %"$c3" = load i8, i8* %c, align 1
  %LE = icmp sle i8 %"$c3", 122
  %AND = and i1 %GE, %LE
  br i1 %AND, label %if15.then, label %endif

if15.then:                                        ; preds = %if15
  %"$c4" = load i8, i8* %c, align 1
  %AS = sext i8 %"$c4" to i32
  %SUB = sub i32 %AS, 32
  %AS5 = trunc i32 %SUB to i8
  store i8 %AS5, i8* %c, align 1
  br label %endif

endif:                                            ; preds = %if15.then, %if15
  %"$c6" = load i8, i8* %c, align 1
  call void @Text.append(%Text* %res, i8 %"$c6")
  %"$i7" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i7", 1
  store i32 %ADD, i32* %i, align 4
  br label %while14

endloop:                                          ; preds = %while14
  %"$res" = load %Text, %Text* %res, align 8
  ret %Text %"$res"
}

define void @Title.reserve(%Title* %0, i32 %1) {
entry:
  %self = alloca %Title*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %Title* %0, %Title** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if16

if16:                                             ; preds = %entry
  %"*self" = load %Title*, %Title** %self, align 8
  %"*self.count" = getelementptr inbounds %Title, %Title* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if16.then, label %endif

if16.then:                                        ; preds = %if16
  %"*self3" = load %Title*, %Title** %self, align 8
  %"*self.count4" = getelementptr inbounds %Title, %Title* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if16.then, %if16
  br label %if17

if17:                                             ; preds = %endif
  %"*self6" = load %Title*, %Title** %self, align 8
  %"*self.cap" = getelementptr inbounds %Title, %Title* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if17.then, label %endif1

if17.then:                                        ; preds = %if17
  ret void

endif1:                                           ; preds = %if17
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %Title*, %Title** %self, align 8
  %"*self.data" = getelementptr inbounds %Title, %Title* %"*self9", i32 0, i32 0
  %"*self10" = load %Title*, %Title** %self, align 8
  %"*self.count11" = getelementptr inbounds %Title, %Title* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if18

if18:                                             ; preds = %endif1
  %"*self13" = load %Title*, %Title** %self, align 8
  %"*self.cap14" = getelementptr inbounds %Title, %Title* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if18.then, label %endif2

if18.then:                                        ; preds = %if18
  %"*self16" = load %Title*, %Title** %self, align 8
  %"*self.data17" = getelementptr inbounds %Title, %Title* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if18.then, %if18
  %"*self19" = load %Title*, %Title** %self, align 8
  %"*self.data20" = getelementptr inbounds %Title, %Title* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %Title*, %Title** %self, align 8
  %"*self.cap23" = getelementptr inbounds %Title, %Title* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @Title.own(%Title* %0) {
entry:
  %self = alloca %Title*, align 8
  store %Title* %0, %Title** %self, align 8
  br label %if19

if19:                                             ; preds = %entry
  %"*self" = load %Title*, %Title** %self, align 8
  %"*self.cap" = getelementptr inbounds %Title, %Title* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  %"*self1" = load %Title*, %Title** %self, align 8
  %"*self.count" = getelementptr inbounds %Title, %Title* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %AND = and i1 %EQ, %GT
  br i1 %AND, label %if19.then, label %endif

if19.then:                                        ; preds = %if19
  %"*self2" = load %Title*, %Title** %self, align 8
  %"*self3" = load %Title*, %Title** %self, align 8
  %"*self.count4" = getelementptr inbounds %Title, %Title* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  call void @Title.reserve(%Title* %"*self2", i32 %"$*self.count5")
  br label %endif

endif:                                            ; preds = %if19.then, %if19
  ret void
}

define void @Title.clear(%Title* %0) {
entry:
  %self = alloca %Title*, align 8
  store %Title* %0, %Title** %self, align 8
  br label %if20

if20:                                             ; preds = %entry
  %"*self" = load %Title*, %Title** %self, align 8
  %"*self.cap" = getelementptr inbounds %Title, %Title* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if20.then, label %endif

if20.then:                                        ; preds = %if20
  %"*self1" = load %Title*, %Title** %self, align 8
  %"*self.count" = getelementptr inbounds %Title, %Title* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if20.then, %if20
  br label %while21

while21:                                          ; preds = %while21.then, %endif
  %"*self2" = load %Title*, %Title** %self, align 8
  %"*self.count3" = getelementptr inbounds %Title, %Title* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while21.then, label %endloop

while21.then:                                     ; preds = %while21
  %"*self4" = load %Title*, %Title** %self, align 8
  %"*self.count5" = getelementptr inbounds %Title, %Title* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %Title*, %Title** %self, align 8
  %"*self.data" = getelementptr inbounds %Title, %Title* %"*self7", i32 0, i32 0
  %"*self8" = load %Title*, %Title** %self, align 8
  %"*self.count9" = getelementptr inbounds %Title, %Title* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while21

endloop:                                          ; preds = %while21
  ret void
}

define void @Title.append(%Title* %0, i8 %1) {
entry:
  %self = alloca %Title*, align 8
  %value = alloca i8, align 1
  store %Title* %0, %Title** %self, align 8
  store i8 %1, i8* %value, align 1
  br label %if22

if22:                                             ; preds = %entry
  %"*self" = load %Title*, %Title** %self, align 8
  %"*self.count" = getelementptr inbounds %Title, %Title* %"*self", i32 0, i32 1
  %"*self1" = load %Title*, %Title** %self, align 8
  %"*self.cap" = getelementptr inbounds %Title, %Title* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if22.then, label %endif

if22.then:                                        ; preds = %if22
  %"*self2" = load %Title*, %Title** %self, align 8
  %"*self3" = load %Title*, %Title** %self, align 8
  %"*self.count4" = getelementptr inbounds %Title, %Title* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @Title.reserve(%Title* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if22.then, %if22
  %"*self6" = load %Title*, %Title** %self, align 8
  %"*self.data" = getelementptr inbounds %Title, %Title* %"*self6", i32 0, i32 0
  %"*self7" = load %Title*, %Title** %self, align 8
  %"*self.count8" = getelementptr inbounds %Title, %Title* %"*self7", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count9"
  %"$value" = load i8, i8* %value, align 1
  store i8 %"$value", i8* %"*self.data[]", align 1
  %"*self10" = load %Title*, %Title** %self, align 8
  %"*self.count11" = getelementptr inbounds %Title, %Title* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  ret void
}

define i8* @"Title.[]"(%Title* %0, i32 %1) {
entry:
  %self = alloca %Title*, align 8
  %index = alloca i32, align 4
  store %Title* %0, %Title** %self, align 8
  store i32 %1, i32* %index, align 4
  %"*self" = load %Title*, %Title** %self, align 8
  call void @Title.own(%Title* %"*self")
  %"*self1" = load %Title*, %Title** %self, align 8
  %"*self.data" = getelementptr inbounds %Title, %Title* %"*self1", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$index"
  ret i8* %"*self.data[]"
}

define void @Title.drop(%Title* %0) {
entry:
  %self = alloca %Title*, align 8
  store %Title* %0, %Title** %self, align 8
  %"*self" = load %Title*, %Title** %self, align 8
  call void @Title.clear(%Title* %"*self")
  br label %if23

if23:                                             ; preds = %entry
  %"*self1" = load %Title*, %Title** %self, align 8
  %"*self.cap" = getelementptr inbounds %Title, %Title* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if23.then, label %endif

if23.then:                                        ; preds = %if23
  %"*self2" = load %Title*, %Title** %self, align 8
  %"*self.data" = getelementptr inbounds %Title, %Title* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if23.then, %if23
  ret void
}

define i8 @Title.first(%Title* %0) {
entry:
  %self = alloca %Title*, align 8
  %ref = alloca i8*, align 8
  store %Title* %0, %Title** %self, align 8
  %"*self" = load %Title*, %Title** %self, align 8
  %"call.Title.[]" = call i8* @"Title.[]"(%Title* %"*self", i32 0)
  store i8* %"call.Title.[]", i8** %ref, align 8
  %"*ref" = load i8*, i8** %ref, align 8
  %"$*ref" = load i8, i8* %"*ref", align 1
  ret i8 %"$*ref"
}

define i32 @main() {
entry:
  %t = alloca %Text, align 8
  %loud = alloca %Text, align 8
  %title = alloca %Title, align 8
  %ref = alloca i8*, align 8
  %ref2 = alloca i8*, align 8
  %ref5 = alloca i8*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Text zeroinitializer, %Text* %t, align 8
  %t.label = getelementptr inbounds %Text, %Text* %t, i32 0, i32 3
  store i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i8** %t.label, align 8
  call void @Text.append(%Text* %t, i8 104)
  call void @Text.append(%Text* %t, i8 105)
  call void @Text.append(%Text* %t, i8 32)
  call void @Text.append(%Text* %t, i8 117)
  %call.Text.shout = call %Text @Text.shout(%Text* %t)
  store %Text %call.Text.shout, %Text* %loud, align 8
  store %Title zeroinitializer, %Title* %title, align 8
  call void @Title.append(%Title* %title, i8 122)
  %call.Text.size = call i32 @Text.size(%Text* %t)
  %t.words = getelementptr inbounds %Text, %Text* %t, i32 0, i32 4
  %"call.Text.[]" = call i8* @"Text.[]"(%Text* %loud, i32 0)
  store i8* %"call.Text.[]", i8** %ref, align 8
  %"*ref" = load i8*, i8** %ref, align 8
  %"call.Text.[]1" = call i8* @"Text.[]"(%Text* %loud, i32 1)
  store i8* %"call.Text.[]1", i8** %ref2, align 8
  %"*ref3" = load i8*, i8** %ref2, align 8
  %"call.Text.[]4" = call i8* @"Text.[]"(%Text* %loud, i32 3)
  store i8* %"call.Text.[]4", i8** %ref5, align 8
  %"*ref6" = load i8*, i8** %ref5, align 8
  %call.Text.name = call i8* @Text.name(%Text* %t)
  %call.Title.first = call i8 @Title.first(%Title* %title)
  %"$t.words" = load i32, i32* %t.words, align 4
  %"$*ref" = load i8, i8* %"*ref", align 1
  %AS = sext i8 %"$*ref" to i32
  %"$*ref7" = load i8, i8* %"*ref3", align 1
  %AS8 = sext i8 %"$*ref7" to i32
  %"$*ref9" = load i8, i8* %"*ref6", align 1
  %AS10 = sext i8 %"$*ref9" to i32
  %AS11 = sext i8 %call.Title.first to i32
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.1, i32 0, i32 0), i32 %call.Text.size, i32 %"$t.words", i32 %AS, i32 %AS8, i32 %AS10, i8* %call.Text.name, i32 %AS11, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @TYPEOF, i32 0, i32 0))
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS12 = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS12, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf13 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS12, i8* getelementptr inbounds ([23 x i8], [23 x i8]* @str.1, i32 0, i32 0), i32 %call.Text.size, i32 %"$t.words", i32 %AS, i32 %AS8, i32 %AS10, i8* %call.Text.name, i32 %AS11, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @TYPEOF, i32 0, i32 0))
  %"$format.size14" = load i32, i32* %format.size, align 4
  %ADD15 = add i32 %"$format.size14", 1
  %"$format.text16" = load i8*, i8** %format.text, align 8
  %"$format.size17" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text16", 0
  %1 = insertvalue %String %0, i32 %"$format.size17", 1
  %2 = insertvalue %String %1, i32 %ADD15, 2
  call void @"io::out"(%String %2)
  call void @Title.drop(%Title* %title)
  call void @Text.drop(%Text* %loud)
  call void @Text.drop(%Text* %t)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

---

## 037 — struct: from a struct that doesn't exist

```ura
struct Text from Nothing:
    size i32

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:1:18 no struct named 'Nothing' to take from
  |
1 | struct Text from Nothing:
  |                  ^^^^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 038 — struct: String from Array<char> in "@/string", temporaries are dropped

```ura
use "@/string"

fn main() i32:
    s String = "  Hello World  "
    t String = s.trim()
    io::out($"[{t.chars()}] {t.size()}\n")
    io::out($"{t.upper().chars()} {t.lower().chars()} {t.reverse().chars()}\n")
    io::out($"{t.find("World")} {t.contains("lo W")} {t.starts_with("Hell")} {t.ends_with("ld")} {t.find("xyz")}\n")
    r String = t.replace("World", "ura")
    io::out($"{r.chars()} {r.substr(1, 3).chars()} {r.repeat(2).chars()}\n")
    a String = String::create("abc")
    b String = "abd"
    c String = a + "-" + b
    c += "!"
    io::out($"{c.chars()} {a < b} {a == "abc"} {a != b} {c.size()}\n")
    n String = "42"
    io::out($"{n.to_int() + 1} {typeof(n)}\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [16 x i8] c"  Hello World  \00", align 1
@str.3 = private unnamed_addr constant [9 x i8] c"[%s] %d\0A\00", align 1
@str.4 = private unnamed_addr constant [10 x i8] c"%s %s %s\0A\00", align 1
@str.5 = private unnamed_addr constant [6 x i8] c"World\00", align 1
@str.6 = private unnamed_addr constant [5 x i8] c"lo W\00", align 1
@str.7 = private unnamed_addr constant [5 x i8] c"Hell\00", align 1
@str.8 = private unnamed_addr constant [3 x i8] c"ld\00", align 1
@str.9 = private unnamed_addr constant [4 x i8] c"xyz\00", align 1
@str.10 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.11 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.12 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.13 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.14 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.15 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.16 = private unnamed_addr constant [16 x i8] c"%d %s %s %s %d\0A\00", align 1
@str.17 = private unnamed_addr constant [6 x i8] c"World\00", align 1
@str.18 = private unnamed_addr constant [4 x i8] c"ura\00", align 1
@str.19 = private unnamed_addr constant [10 x i8] c"%s %s %s\0A\00", align 1
@str.20 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@str.21 = private unnamed_addr constant [4 x i8] c"abd\00", align 1
@str.22 = private unnamed_addr constant [2 x i8] c"-\00", align 1
@str.23 = private unnamed_addr constant [2 x i8] c"!\00", align 1
@str.24 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@str.25 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.26 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.27 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.28 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.29 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.30 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.31 = private unnamed_addr constant [16 x i8] c"%s %s %s %s %d\0A\00", align 1
@str.32 = private unnamed_addr constant [3 x i8] c"42\00", align 1
@TYPEOF = private unnamed_addr constant [7 x i8] c"String\00", align 1
@str.33 = private unnamed_addr constant [7 x i8] c"%d %s\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  ret void

endif1:                                           ; preds = %if1
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if2

if2:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %while4

while4:                                           ; preds = %while4.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.count5" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.count9" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while4

endloop:                                          ; preds = %while4
  ret void
}

define void @String.append(%String* %0, i8 %1) {
entry:
  %self = alloca %String*, align 8
  %value = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store i8 %1, i8* %value, align 1
  br label %if5

if5:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @String.reserve(%String* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if5.then, %if5
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.count8" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count9"
  %"$value" = load i8, i8* %value, align 1
  store i8 %"$value", i8* %"*self.data[]", align 1
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define i32 @String.size(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @"String.="(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  %other = alloca %String*, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store %String* %1, %String** %other, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  %"*self1" = load %String*, %String** %self, align 8
  %"*other" = load %String*, %String** %other, align 8
  %"*other.count" = getelementptr inbounds %String, %String* %"*other", i32 0, i32 1
  %"$*other.count" = load i32, i32* %"*other.count", align 4
  call void @String.reserve(%String* %"*self1", i32 %"$*other.count")
  store i32 0, i32* %i, align 4
  br label %while6

while6:                                           ; preds = %while6.then, %entry
  %"*other2" = load %String*, %String** %other, align 8
  %"*other.count3" = getelementptr inbounds %String, %String* %"*other2", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*other.count4" = load i32, i32* %"*other.count3", align 4
  %LT = icmp slt i32 %"$i", %"$*other.count4"
  br i1 %LT, label %while6.then, label %endloop

while6.then:                                      ; preds = %while6
  %"*self5" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self5", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i6" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i6"
  %"*other7" = load %String*, %String** %other, align 8
  %"*other.data" = getelementptr inbounds %String, %String* %"*other7", i32 0, i32 0
  %"$*other.data" = load i8*, i8** %"*other.data", align 8
  %"$i8" = load i32, i32* %i, align 4
  %"*other.data[]" = getelementptr inbounds i8, i8* %"$*other.data", i32 %"$i8"
  %"$*other.data[]" = load i8, i8* %"*other.data[]", align 1
  store i8 %"$*other.data[]", i8* %"*self.data[]", align 1
  %"$i9" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i9", 1
  store i32 %ADD, i32* %i, align 4
  br label %while6

endloop:                                          ; preds = %while6
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"*other11" = load %String*, %String** %other, align 8
  %"*other.count12" = getelementptr inbounds %String, %String* %"*other11", i32 0, i32 1
  %"$*other.count13" = load i32, i32* %"*other.count12", align 4
  store i32 %"$*other.count13", i32* %"*self.count", align 4
  ret void
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if7

if7:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if7.then, label %endif

if7.then:                                         ; preds = %if7
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if7.then, %if7
  ret void
}

define %String @String.create(i8* %0) {
entry:
  %text = alloca i8*, align 8
  %res = alloca %String, align 8
  store i8* %0, i8** %text, align 8
  store %String zeroinitializer, %String* %res, align 8
  %"$text" = load i8*, i8** %text, align 8
  call void @String.join(%String* %res, i8* %"$text")
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define i32 @String.length_of(i8* %0) {
entry:
  %text = alloca i8*, align 8
  %n = alloca i32, align 4
  store i8* %0, i8** %text, align 8
  store i32 0, i32* %n, align 4
  br label %while8

while8:                                           ; preds = %while8.then, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$n" = load i32, i32* %n, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$n"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while8.then, label %endloop

while8.then:                                      ; preds = %while8
  %"$n1" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n1", 1
  store i32 %ADD, i32* %n, align 4
  br label %while8

endloop:                                          ; preds = %while8
  %"$n2" = load i32, i32* %n, align 4
  ret i32 %"$n2"
}

define void @String.join(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  store i32 0, i32* %i, align 4
  br label %while9

while9:                                           ; preds = %while9.then, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$i" = load i32, i32* %i, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$i"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while9.then, label %endloop

while9.then:                                      ; preds = %while9
  %"*self" = load %String*, %String** %self, align 8
  %"$text1" = load i8*, i8** %text, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"text[]3" = getelementptr inbounds i8, i8* %"$text1", i32 %"$i2"
  %"$text[]4" = load i8, i8* %"text[]3", align 1
  call void @String.append(%String* %"*self", i8 %"$text[]4")
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while9

endloop:                                          ; preds = %while9
  ret void
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

define i32 @String.to_int(%String* %0) {
entry:
  %self = alloca %String*, align 8
  %i = alloca i32, align 4
  %sign = alloca i32, align 4
  %n = alloca i32, align 4
  %c = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store i32 0, i32* %i, align 4
  store i32 1, i32* %sign, align 4
  br label %if10

if10:                                             ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 0
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  %EQ = icmp eq i8 %"$*self.data[]", 45
  %AND = and i1 %GT, %EQ
  br i1 %AND, label %if10.then, label %endif

if10.then:                                        ; preds = %if10
  store i32 -1, i32* %sign, align 4
  store i32 1, i32* %i, align 4
  br label %endif

endif:                                            ; preds = %if10.then, %if10
  store i32 0, i32* %n, align 4
  br label %while11

while11:                                          ; preds = %endif1, %endif
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count5"
  br i1 %LT, label %while11.then, label %endloop

while11.then:                                     ; preds = %while11
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  %"$i9" = load i32, i32* %i, align 4
  %"*self.data[]10" = getelementptr inbounds i8, i8* %"$*self.data8", i32 %"$i9"
  %"$*self.data[]11" = load i8, i8* %"*self.data[]10", align 1
  store i8 %"$*self.data[]11", i8* %c, align 1
  br label %if12

if12:                                             ; preds = %while11.then
  %"$c" = load i8, i8* %c, align 1
  %LT12 = icmp slt i8 %"$c", 48
  %"$c13" = load i8, i8* %c, align 1
  %GT14 = icmp sgt i8 %"$c13", 57
  %OR = or i1 %LT12, %GT14
  br i1 %OR, label %if12.then, label %endif1

if12.then:                                        ; preds = %if12
  br label %endloop

endif1:                                           ; preds = %if12
  %"$n" = load i32, i32* %n, align 4
  %MUL = mul i32 %"$n", 10
  %"$c15" = load i8, i8* %c, align 1
  %AS = sext i8 %"$c15" to i32
  %SUB = sub i32 %AS, 48
  %ADD = add i32 %MUL, %SUB
  store i32 %ADD, i32* %n, align 4
  %"$i16" = load i32, i32* %i, align 4
  %ADD17 = add i32 %"$i16", 1
  store i32 %ADD17, i32* %i, align 4
  br label %while11

endloop:                                          ; preds = %if12.then, %while11
  %"$n18" = load i32, i32* %n, align 4
  %"$sign" = load i32, i32* %sign, align 4
  %MUL19 = mul i32 %"$n18", %"$sign"
  ret i32 %MUL19
}

define i1 @String.matches_at(%String* %0, i32 %1, i8* %2) {
entry:
  %self = alloca %String*, align 8
  %index = alloca i32, align 4
  %text = alloca i8*, align 8
  %j = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %index, align 4
  store i8* %2, i8** %text, align 8
  store i32 0, i32* %j, align 4
  br label %while13

while13:                                          ; preds = %endif1, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$j" = load i32, i32* %j, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$j"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while13.then, label %endloop

while13.then:                                     ; preds = %while13
  br label %if14

if14:                                             ; preds = %while13.then
  %"$index" = load i32, i32* %index, align 4
  %"$j2" = load i32, i32* %j, align 4
  %ADD = add i32 %"$index", %"$j2"
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GE = icmp sge i32 %ADD, %"$*self.count"
  br i1 %GE, label %if14.then, label %endif

if14.then:                                        ; preds = %if14
  ret i1 false

endif:                                            ; preds = %if14
  br label %if15

if15:                                             ; preds = %endif
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 0
  %"$index4" = load i32, i32* %index, align 4
  %"$j5" = load i32, i32* %j, align 4
  %ADD6 = add i32 %"$index4", %"$j5"
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %ADD6
  %"$text7" = load i8*, i8** %text, align 8
  %"$j8" = load i32, i32* %j, align 4
  %"text[]9" = getelementptr inbounds i8, i8* %"$text7", i32 %"$j8"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  %"$text[]10" = load i8, i8* %"text[]9", align 1
  %NQ11 = icmp ne i8 %"$*self.data[]", %"$text[]10"
  br i1 %NQ11, label %if15.then, label %endif1

if15.then:                                        ; preds = %if15
  ret i1 false

endif1:                                           ; preds = %if15
  %"$j12" = load i32, i32* %j, align 4
  %ADD13 = add i32 %"$j12", 1
  store i32 %ADD13, i32* %j, align 4
  br label %while13

endloop:                                          ; preds = %while13
  ret i1 true
}

define i32 @String.find(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  store i32 0, i32* %i, align 4
  br label %while16

while16:                                          ; preds = %endif, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count"
  br i1 %LT, label %while16.then, label %endloop

while16.then:                                     ; preds = %while16
  br label %if17

if17:                                             ; preds = %while16.then
  %"*self1" = load %String*, %String** %self, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"$text" = load i8*, i8** %text, align 8
  %call.String.matches_at = call i1 @String.matches_at(%String* %"*self1", i32 %"$i2", i8* %"$text")
  br i1 %call.String.matches_at, label %if17.then, label %endif

if17.then:                                        ; preds = %if17
  %"$i3" = load i32, i32* %i, align 4
  ret i32 %"$i3"

endif:                                            ; preds = %if17
  %"$i4" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i4", 1
  store i32 %ADD, i32* %i, align 4
  br label %while16

endloop:                                          ; preds = %while16
  ret i32 -1
}

define i1 @String.contains(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"$text" = load i8*, i8** %text, align 8
  %call.String.find = call i32 @String.find(%String* %"*self", i8* %"$text")
  %NQ = icmp ne i32 %call.String.find, -1
  ret i1 %NQ
}

define i1 @String.starts_with(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"$text" = load i8*, i8** %text, align 8
  %call.String.matches_at = call i1 @String.matches_at(%String* %"*self", i32 0, i8* %"$text")
  ret i1 %call.String.matches_at
}

define i1 @String.ends_with(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  %n = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  %"$text" = load i8*, i8** %text, align 8
  %call.String.length_of = call i32 @String.length_of(i8* %"$text")
  store i32 %call.String.length_of, i32* %n, align 4
  br label %if18

if18:                                             ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$n" = load i32, i32* %n, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$n", %"$*self.count"
  br i1 %GT, label %if18.then, label %endif

if18.then:                                        ; preds = %if18
  ret i1 false

endif:                                            ; preds = %if18
  %"*self1" = load %String*, %String** %self, align 8
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count4" = load i32, i32* %"*self.count3", align 4
  %"$n5" = load i32, i32* %n, align 4
  %SUB = sub i32 %"$*self.count4", %"$n5"
  %"$text6" = load i8*, i8** %text, align 8
  %call.String.matches_at = call i1 @String.matches_at(%String* %"*self1", i32 %SUB, i8* %"$text6")
  ret i1 %call.String.matches_at
}

define %String @String.substr(%String* %0, i32 %1, i32 %2) {
entry:
  %self = alloca %String*, align 8
  %start = alloca i32, align 4
  %length = alloca i32, align 4
  %res = alloca %String, align 8
  %stop = alloca i32, align 4
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %start, align 4
  store i32 %2, i32* %length, align 4
  store %String zeroinitializer, %String* %res, align 8
  br label %if19

if19:                                             ; preds = %entry
  %"$start" = load i32, i32* %start, align 4
  %LT = icmp slt i32 %"$start", 0
  br i1 %LT, label %if19.then, label %endif

if19.then:                                        ; preds = %if19
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"

endif:                                            ; preds = %if19
  %"$start2" = load i32, i32* %start, align 4
  %"$length" = load i32, i32* %length, align 4
  %ADD = add i32 %"$start2", %"$length"
  store i32 %ADD, i32* %stop, align 4
  br label %if20

if20:                                             ; preds = %endif
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$stop" = load i32, i32* %stop, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$stop", %"$*self.count"
  br i1 %GT, label %if20.then, label %endif1

if20.then:                                        ; preds = %if20
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %stop, align 4
  br label %endif1

endif1:                                           ; preds = %if20.then, %if20
  %"$start6" = load i32, i32* %start, align 4
  store i32 %"$start6", i32* %i, align 4
  br label %while21

while21:                                          ; preds = %while21.then, %endif1
  %"$i" = load i32, i32* %i, align 4
  %"$stop7" = load i32, i32* %stop, align 4
  %LT8 = icmp slt i32 %"$i", %"$stop7"
  br i1 %LT8, label %while21.then, label %endloop

while21.then:                                     ; preds = %while21
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i10" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i10"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  call void @String.append(%String* %res, i8 %"$*self.data[]")
  %"$i11" = load i32, i32* %i, align 4
  %ADD12 = add i32 %"$i11", 1
  store i32 %ADD12, i32* %i, align 4
  br label %while21

endloop:                                          ; preds = %while21
  %"$res13" = load %String, %String* %res, align 8
  ret %String %"$res13"
}

define %String @String.upper(%String* %0) {
entry:
  %self = alloca %String*, align 8
  %res = alloca %String, align 8
  %i = alloca i32, align 4
  %c = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store %String zeroinitializer, %String* %res, align 8
  store i32 0, i32* %i, align 4
  br label %while22

while22:                                          ; preds = %endif, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count"
  br i1 %LT, label %while22.then, label %endloop

while22.then:                                     ; preds = %while22
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i2" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i2"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  store i8 %"$*self.data[]", i8* %c, align 1
  br label %if23

if23:                                             ; preds = %while22.then
  %"$c" = load i8, i8* %c, align 1
  %GE = icmp sge i8 %"$c", 97
  %"$c3" = load i8, i8* %c, align 1
  %LE = icmp sle i8 %"$c3", 122
  %AND = and i1 %GE, %LE
  br i1 %AND, label %if23.then, label %endif

if23.then:                                        ; preds = %if23
  %"$c4" = load i8, i8* %c, align 1
  %AS = sext i8 %"$c4" to i32
  %SUB = sub i32 %AS, 32
  %AS5 = trunc i32 %SUB to i8
  store i8 %AS5, i8* %c, align 1
  br label %endif

endif:                                            ; preds = %if23.then, %if23
  %"$c6" = load i8, i8* %c, align 1
  call void @String.append(%String* %res, i8 %"$c6")
  %"$i7" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i7", 1
  store i32 %ADD, i32* %i, align 4
  br label %while22

endloop:                                          ; preds = %while22
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define %String @String.lower(%String* %0) {
entry:
  %self = alloca %String*, align 8
  %res = alloca %String, align 8
  %i = alloca i32, align 4
  %c = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store %String zeroinitializer, %String* %res, align 8
  store i32 0, i32* %i, align 4
  br label %while24

while24:                                          ; preds = %endif, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count"
  br i1 %LT, label %while24.then, label %endloop

while24.then:                                     ; preds = %while24
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i2" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i2"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  store i8 %"$*self.data[]", i8* %c, align 1
  br label %if25

if25:                                             ; preds = %while24.then
  %"$c" = load i8, i8* %c, align 1
  %GE = icmp sge i8 %"$c", 65
  %"$c3" = load i8, i8* %c, align 1
  %LE = icmp sle i8 %"$c3", 90
  %AND = and i1 %GE, %LE
  br i1 %AND, label %if25.then, label %endif

if25.then:                                        ; preds = %if25
  %"$c4" = load i8, i8* %c, align 1
  %AS = sext i8 %"$c4" to i32
  %ADD = add i32 %AS, 32
  %AS5 = trunc i32 %ADD to i8
  store i8 %AS5, i8* %c, align 1
  br label %endif

endif:                                            ; preds = %if25.then, %if25
  %"$c6" = load i8, i8* %c, align 1
  call void @String.append(%String* %res, i8 %"$c6")
  %"$i7" = load i32, i32* %i, align 4
  %ADD8 = add i32 %"$i7", 1
  store i32 %ADD8, i32* %i, align 4
  br label %while24

endloop:                                          ; preds = %while24
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define i1 @String.is_space(%String* %0, i8 %1) {
entry:
  %self = alloca %String*, align 8
  %c = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store i8 %1, i8* %c, align 1
  %"$c" = load i8, i8* %c, align 1
  %EQ = icmp eq i8 %"$c", 32
  %"$c1" = load i8, i8* %c, align 1
  %EQ2 = icmp eq i8 %"$c1", 9
  %OR = or i1 %EQ, %EQ2
  %"$c3" = load i8, i8* %c, align 1
  %EQ4 = icmp eq i8 %"$c3", 10
  %OR5 = or i1 %OR, %EQ4
  %"$c6" = load i8, i8* %c, align 1
  %EQ7 = icmp eq i8 %"$c6", 13
  %OR8 = or i1 %OR5, %EQ7
  ret i1 %OR8
}

define %String @String.trim(%String* %0) {
entry:
  %self = alloca %String*, align 8
  %start = alloca i32, align 4
  %stop = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i32 0, i32* %start, align 4
  br label %while26

while26:                                          ; preds = %endif, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$start" = load i32, i32* %start, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$start", %"$*self.count"
  br i1 %LT, label %while26.then, label %endloop

while26.then:                                     ; preds = %while26
  br label %if27

if27:                                             ; preds = %while26.then
  %"*self3" = load %String*, %String** %self, align 8
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$start5" = load i32, i32* %start, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$start5"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  %call.String.is_space = call i1 @String.is_space(%String* %"*self3", i8 %"$*self.data[]")
  %NOT = xor i1 %call.String.is_space, true
  br i1 %NOT, label %if27.then, label %endif

if27.then:                                        ; preds = %if27
  br label %endloop

endif:                                            ; preds = %if27
  %"$start6" = load i32, i32* %start, align 4
  %ADD = add i32 %"$start6", 1
  store i32 %ADD, i32* %start, align 4
  br label %while26

endloop:                                          ; preds = %if27.then, %while26
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.count8" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 1
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  store i32 %"$*self.count9", i32* %stop, align 4
  br label %while28

while28:                                          ; preds = %endif1, %endloop
  %"$stop" = load i32, i32* %stop, align 4
  %"$start10" = load i32, i32* %start, align 4
  %GT = icmp sgt i32 %"$stop", %"$start10"
  br i1 %GT, label %while28.then, label %endloop2

while28.then:                                     ; preds = %while28
  br label %if29

if29:                                             ; preds = %while28.then
  %"*self11" = load %String*, %String** %self, align 8
  %"*self12" = load %String*, %String** %self, align 8
  %"*self.data13" = getelementptr inbounds %String, %String* %"*self12", i32 0, i32 0
  %"$stop14" = load i32, i32* %stop, align 4
  %SUB = sub i32 %"$stop14", 1
  %"$*self.data15" = load i8*, i8** %"*self.data13", align 8
  %"*self.data[]16" = getelementptr inbounds i8, i8* %"$*self.data15", i32 %SUB
  %"$*self.data[]17" = load i8, i8* %"*self.data[]16", align 1
  %call.String.is_space18 = call i1 @String.is_space(%String* %"*self11", i8 %"$*self.data[]17")
  %NOT19 = xor i1 %call.String.is_space18, true
  br i1 %NOT19, label %if29.then, label %endif1

if29.then:                                        ; preds = %if29
  br label %endloop2

endif1:                                           ; preds = %if29
  %"$stop20" = load i32, i32* %stop, align 4
  %SUB21 = sub i32 %"$stop20", 1
  store i32 %SUB21, i32* %stop, align 4
  br label %while28

endloop2:                                         ; preds = %if29.then, %while28
  %"*self22" = load %String*, %String** %self, align 8
  %"$stop23" = load i32, i32* %stop, align 4
  %"$start24" = load i32, i32* %start, align 4
  %SUB25 = sub i32 %"$stop23", %"$start24"
  %"$start26" = load i32, i32* %start, align 4
  %call.String.substr = call %String @String.substr(%String* %"*self22", i32 %"$start26", i32 %SUB25)
  ret %String %call.String.substr
}

define %String @String.replace(%String* %0, i8* %1, i8* %2) {
entry:
  %self = alloca %String*, align 8
  %old = alloca i8*, align 8
  %fresh = alloca i8*, align 8
  %res = alloca %String, align 8
  %n = alloca i32, align 4
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %old, align 8
  store i8* %2, i8** %fresh, align 8
  store %String zeroinitializer, %String* %res, align 8
  %"$old" = load i8*, i8** %old, align 8
  %call.String.length_of = call i32 @String.length_of(i8* %"$old")
  store i32 %call.String.length_of, i32* %n, align 4
  store i32 0, i32* %i, align 4
  br label %while30

while30:                                          ; preds = %endif, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count"
  br i1 %LT, label %while30.then, label %endloop

while30.then:                                     ; preds = %while30
  br label %if31

if31:                                             ; preds = %while30.then
  %"$n" = load i32, i32* %n, align 4
  %GT = icmp sgt i32 %"$n", 0
  %"*self1" = load %String*, %String** %self, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"$old3" = load i8*, i8** %old, align 8
  %call.String.matches_at = call i1 @String.matches_at(%String* %"*self1", i32 %"$i2", i8* %"$old3")
  %AND = and i1 %GT, %call.String.matches_at
  br i1 %AND, label %if31.then, label %else32

if31.then:                                        ; preds = %if31
  %"$fresh" = load i8*, i8** %fresh, align 8
  call void @String.join(%String* %res, i8* %"$fresh")
  %"$n4" = load i32, i32* %n, align 4
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", %"$n4"
  store i32 %ADD, i32* %i, align 4
  br label %endif

else32:                                           ; preds = %if31
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i7" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i7"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  call void @String.append(%String* %res, i8 %"$*self.data[]")
  %"$i8" = load i32, i32* %i, align 4
  %ADD9 = add i32 %"$i8", 1
  store i32 %ADD9, i32* %i, align 4
  br label %endif

endif:                                            ; preds = %else32, %if31.then
  br label %while30

endloop:                                          ; preds = %while30
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define %String @String.repeat(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %times = alloca i32, align 4
  %res = alloca %String, align 8
  %k = alloca i32, align 4
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %times, align 4
  store %String zeroinitializer, %String* %res, align 8
  store i32 0, i32* %k, align 4
  br label %while33

while33:                                          ; preds = %endloop, %entry
  %"$k" = load i32, i32* %k, align 4
  %"$times" = load i32, i32* %times, align 4
  %LT = icmp slt i32 %"$k", %"$times"
  br i1 %LT, label %while33.then, label %endloop1

while33.then:                                     ; preds = %while33
  store i32 0, i32* %i, align 4
  br label %while34

while34:                                          ; preds = %while34.then, %while33.then
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT2 = icmp slt i32 %"$i", %"$*self.count"
  br i1 %LT2, label %while34.then, label %endloop

while34.then:                                     ; preds = %while34
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i4" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i4"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  call void @String.append(%String* %res, i8 %"$*self.data[]")
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while34

endloop:                                          ; preds = %while34
  %"$k6" = load i32, i32* %k, align 4
  %ADD7 = add i32 %"$k6", 1
  store i32 %ADD7, i32* %k, align 4
  br label %while33

endloop1:                                         ; preds = %while33
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define %String @String.reverse(%String* %0) {
entry:
  %self = alloca %String*, align 8
  %res = alloca %String, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store %String zeroinitializer, %String* %res, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  store i32 %SUB, i32* %i, align 4
  br label %while35

while35:                                          ; preds = %while35.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %GE = icmp sge i32 %"$i", 0
  br i1 %GE, label %while35.then, label %endloop

while35.then:                                     ; preds = %while35
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i2" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i2"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  call void @String.append(%String* %res, i8 %"$*self.data[]")
  %"$i3" = load i32, i32* %i, align 4
  %SUB4 = sub i32 %"$i3", 1
  store i32 %SUB4, i32* %i, align 4
  br label %while35

endloop:                                          ; preds = %while35
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define i32 @String.compare(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  %other = alloca %String*, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store %String* %1, %String** %other, align 8
  store i32 0, i32* %i, align 4
  br label %while36

while36:                                          ; preds = %endif, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count"
  %"*other" = load %String*, %String** %other, align 8
  %"*other.count" = getelementptr inbounds %String, %String* %"*other", i32 0, i32 1
  %"$i1" = load i32, i32* %i, align 4
  %"$*other.count" = load i32, i32* %"*other.count", align 4
  %LT2 = icmp slt i32 %"$i1", %"$*other.count"
  %AND = and i1 %LT, %LT2
  br i1 %AND, label %while36.then, label %endloop

while36.then:                                     ; preds = %while36
  br label %if37

if37:                                             ; preds = %while36.then
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i4" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i4"
  %"*other5" = load %String*, %String** %other, align 8
  %"*other.data" = getelementptr inbounds %String, %String* %"*other5", i32 0, i32 0
  %"$*other.data" = load i8*, i8** %"*other.data", align 8
  %"$i6" = load i32, i32* %i, align 4
  %"*other.data[]" = getelementptr inbounds i8, i8* %"$*other.data", i32 %"$i6"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  %"$*other.data[]" = load i8, i8* %"*other.data[]", align 1
  %NQ = icmp ne i8 %"$*self.data[]", %"$*other.data[]"
  br i1 %NQ, label %if37.then, label %endif

if37.then:                                        ; preds = %if37
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data8" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"$*self.data9" = load i8*, i8** %"*self.data8", align 8
  %"$i10" = load i32, i32* %i, align 4
  %"*self.data[]11" = getelementptr inbounds i8, i8* %"$*self.data9", i32 %"$i10"
  %"$*self.data[]12" = load i8, i8* %"*self.data[]11", align 1
  %AS = sext i8 %"$*self.data[]12" to i32
  %"*other13" = load %String*, %String** %other, align 8
  %"*other.data14" = getelementptr inbounds %String, %String* %"*other13", i32 0, i32 0
  %"$*other.data15" = load i8*, i8** %"*other.data14", align 8
  %"$i16" = load i32, i32* %i, align 4
  %"*other.data[]17" = getelementptr inbounds i8, i8* %"$*other.data15", i32 %"$i16"
  %"$*other.data[]18" = load i8, i8* %"*other.data[]17", align 1
  %AS19 = sext i8 %"$*other.data[]18" to i32
  %SUB = sub i32 %AS, %AS19
  ret i32 %SUB

endif:                                            ; preds = %if37
  %"$i20" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i20", 1
  store i32 %ADD, i32* %i, align 4
  br label %while36

endloop:                                          ; preds = %while36
  %"*self21" = load %String*, %String** %self, align 8
  %"*self.count22" = getelementptr inbounds %String, %String* %"*self21", i32 0, i32 1
  %"*other23" = load %String*, %String** %other, align 8
  %"*other.count24" = getelementptr inbounds %String, %String* %"*other23", i32 0, i32 1
  %"$*self.count25" = load i32, i32* %"*self.count22", align 4
  %"$*other.count26" = load i32, i32* %"*other.count24", align 4
  %SUB27 = sub i32 %"$*self.count25", %"$*other.count26"
  ret i32 %SUB27
}

define i1 @"String.=="(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  %other = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  store %String* %1, %String** %other, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*other" = load %String*, %String** %other, align 8
  %call.String.compare = call i32 @String.compare(%String* %"*self", %String* %"*other")
  %EQ = icmp eq i32 %call.String.compare, 0
  ret i1 %EQ
}

define i1 @"String.!="(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  %other = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  store %String* %1, %String** %other, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*other" = load %String*, %String** %other, align 8
  %call.String.compare = call i32 @String.compare(%String* %"*self", %String* %"*other")
  %NQ = icmp ne i32 %call.String.compare, 0
  ret i1 %NQ
}

define i1 @"String.<"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  %other = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  store %String* %1, %String** %other, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*other" = load %String*, %String** %other, align 8
  %call.String.compare = call i32 @String.compare(%String* %"*self", %String* %"*other")
  %LT = icmp slt i32 %call.String.compare, 0
  ret i1 %LT
}

define void @"String.+="(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"$text" = load i8*, i8** %text, align 8
  call void @String.join(%String* %"*self", i8* %"$text")
  ret void
}

define void @"String.+=.1"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  %other = alloca %String*, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store %String* %1, %String** %other, align 8
  store i32 0, i32* %i, align 4
  br label %while38

while38:                                          ; preds = %while38.then, %entry
  %"*other" = load %String*, %String** %other, align 8
  %"*other.count" = getelementptr inbounds %String, %String* %"*other", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*other.count" = load i32, i32* %"*other.count", align 4
  %LT = icmp slt i32 %"$i", %"$*other.count"
  br i1 %LT, label %while38.then, label %endloop

while38.then:                                     ; preds = %while38
  %"*self" = load %String*, %String** %self, align 8
  %"*other1" = load %String*, %String** %other, align 8
  %"*other.data" = getelementptr inbounds %String, %String* %"*other1", i32 0, i32 0
  %"$*other.data" = load i8*, i8** %"*other.data", align 8
  %"$i2" = load i32, i32* %i, align 4
  %"*other.data[]" = getelementptr inbounds i8, i8* %"$*other.data", i32 %"$i2"
  %"$*other.data[]" = load i8, i8* %"*other.data[]", align 1
  call void @String.append(%String* %"*self", i8 %"$*other.data[]")
  %"$i3" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i3", 1
  store i32 %ADD, i32* %i, align 4
  br label %while38

endloop:                                          ; preds = %while38
  ret void
}

define %String @"String.+"(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  %res = alloca %String, align 8
  %copy = alloca %String, align 8
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  store %String zeroinitializer, %String* %copy, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @"String.="(%String* %copy, %String* %"*self")
  %"$copy" = load %String, %String* %copy, align 8
  store %String %"$copy", %String* %res, align 8
  %"$text" = load i8*, i8** %text, align 8
  call void @String.join(%String* %res, i8* %"$text")
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define %String @"String.+.2"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  %other = alloca %String*, align 8
  %res = alloca %String, align 8
  %copy = alloca %String, align 8
  store %String* %0, %String** %self, align 8
  store %String* %1, %String** %other, align 8
  store %String zeroinitializer, %String* %copy, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @"String.="(%String* %copy, %String* %"*self")
  %"$copy" = load %String, %String* %copy, align 8
  store %String %"$copy", %String* %res, align 8
  %"*other" = load %String*, %String** %other, align 8
  call void @"String.+=.1"(%String* %res, %String* %"*other")
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
  ret void
}

define i32 @main() {
entry:
  %s = alloca %String, align 8
  %t = alloca %String, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %tmp = alloca %String, align 8
  %tmp12 = alloca %String, align 8
  %tmp14 = alloca %String, align 8
  %format.size16 = alloca i32, align 4
  %format.text21 = alloca i8*, align 8
  %format.size30 = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %bool.text31 = alloca i8*, align 8
  %bool.text32 = alloca i8*, align 8
  %format.text39 = alloca i8*, align 8
  %r = alloca %String, align 8
  %tmp51 = alloca %String, align 8
  %tmp53 = alloca %String, align 8
  %format.size55 = alloca i32, align 4
  %format.text60 = alloca i8*, align 8
  %a = alloca %String, align 8
  %b = alloca %String, align 8
  %c = alloca %String, align 8
  %tmp68 = alloca %String, align 8
  %tmp71 = alloca %String, align 8
  %format.size73 = alloca i32, align 4
  %bool.text74 = alloca i8*, align 8
  %bool.text75 = alloca i8*, align 8
  %bool.text76 = alloca i8*, align 8
  %format.text84 = alloca i8*, align 8
  %n = alloca %String, align 8
  %format.size96 = alloca i32, align 4
  %format.text101 = alloca i8*, align 8
  store %String { i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str, i32 0, i32 0), i32 15, i32 0 }, %String* %s, align 8
  %call.String.trim = call %String @String.trim(%String* %s)
  store %String %call.String.trim, %String* %t, align 8
  %call.String.chars = call i8* @String.chars(%String* %t)
  %call.String.size = call i32 @String.size(%String* %t)
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.3, i32 0, i32 0), i8* %call.String.chars, i32 %call.String.size)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf6 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.3, i32 0, i32 0), i8* %call.String.chars, i32 %call.String.size)
  %"$format.size7" = load i32, i32* %format.size, align 4
  %ADD8 = add i32 %"$format.size7", 1
  %"$format.text9" = load i8*, i8** %format.text, align 8
  %"$format.size10" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text9", 0
  %1 = insertvalue %String %0, i32 %"$format.size10", 1
  %2 = insertvalue %String %1, i32 %ADD8, 2
  call void @"io::out"(%String %2)
  %call.String.upper = call %String @String.upper(%String* %t)
  store %String %call.String.upper, %String* %tmp, align 8
  %call.String.chars11 = call i8* @String.chars(%String* %tmp)
  %call.String.lower = call %String @String.lower(%String* %t)
  store %String %call.String.lower, %String* %tmp12, align 8
  %call.String.chars13 = call i8* @String.chars(%String* %tmp12)
  %call.String.reverse = call %String @String.reverse(%String* %t)
  store %String %call.String.reverse, %String* %tmp14, align 8
  %call.String.chars15 = call i8* @String.chars(%String* %tmp14)
  %call.snprintf17 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.4, i32 0, i32 0), i8* %call.String.chars11, i8* %call.String.chars13, i8* %call.String.chars15)
  store i32 %call.snprintf17, i32* %format.size16, align 4
  %"$format.size18" = load i32, i32* %format.size16, align 4
  %ADD19 = add i32 %"$format.size18", 1
  %AS20 = sext i32 %ADD19 to i64
  %call.calloc22 = call i8* @calloc(i64 %AS20, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc22, i8** %format.text21, align 8
  %"$format.text23" = load i8*, i8** %format.text21, align 8
  %call.snprintf24 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text23", i64 %AS20, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.4, i32 0, i32 0), i8* %call.String.chars11, i8* %call.String.chars13, i8* %call.String.chars15)
  %"$format.size25" = load i32, i32* %format.size16, align 4
  %ADD26 = add i32 %"$format.size25", 1
  %"$format.text27" = load i8*, i8** %format.text21, align 8
  %"$format.size28" = load i32, i32* %format.size16, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text27", 0
  %4 = insertvalue %String %3, i32 %"$format.size28", 1
  %5 = insertvalue %String %4, i32 %ADD26, 2
  call void @"io::out"(%String %5)
  call void @String.drop(%String* %tmp14)
  call void @String.drop(%String* %tmp12)
  call void @String.drop(%String* %tmp)
  %call.String.find = call i32 @String.find(%String* %t, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.5, i32 0, i32 0))
  %call.String.contains = call i1 @String.contains(%String* %t, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.6, i32 0, i32 0))
  %call.String.starts_with = call i1 @String.starts_with(%String* %t, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.7, i32 0, i32 0))
  %call.String.ends_with = call i1 @String.ends_with(%String* %t, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.8, i32 0, i32 0))
  %call.String.find29 = call i32 @String.find(%String* %t, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.9, i32 0, i32 0))
  store i32 0, i32* %format.size30, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if39

if39:                                             ; preds = %entry
  br i1 %call.String.contains, label %if39.then, label %else40

if39.then:                                        ; preds = %if39
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.10, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else40:                                           ; preds = %if39
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.11, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else40, %if39.then
  store i8* null, i8** %bool.text31, align 8
  br label %if41

if41:                                             ; preds = %endif
  br i1 %call.String.starts_with, label %if41.then, label %else42

if41.then:                                        ; preds = %if41
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.12, i32 0, i32 0), i8** %bool.text31, align 8
  br label %endif1

else42:                                           ; preds = %if41
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.13, i32 0, i32 0), i8** %bool.text31, align 8
  br label %endif1

endif1:                                           ; preds = %else42, %if41.then
  store i8* null, i8** %bool.text32, align 8
  br label %if43

if43:                                             ; preds = %endif1
  br i1 %call.String.ends_with, label %if43.then, label %else44

if43.then:                                        ; preds = %if43
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.14, i32 0, i32 0), i8** %bool.text32, align 8
  br label %endif2

else44:                                           ; preds = %if43
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.15, i32 0, i32 0), i8** %bool.text32, align 8
  br label %endif2

endif2:                                           ; preds = %else44, %if43.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %"$bool.text33" = load i8*, i8** %bool.text31, align 8
  %"$bool.text34" = load i8*, i8** %bool.text32, align 8
  %call.snprintf35 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.16, i32 0, i32 0), i32 %call.String.find, i8* %"$bool.text", i8* %"$bool.text33", i8* %"$bool.text34", i32 %call.String.find29)
  store i32 %call.snprintf35, i32* %format.size30, align 4
  %"$format.size36" = load i32, i32* %format.size30, align 4
  %ADD37 = add i32 %"$format.size36", 1
  %AS38 = sext i32 %ADD37 to i64
  %call.calloc40 = call i8* @calloc(i64 %AS38, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc40, i8** %format.text39, align 8
  %"$format.text41" = load i8*, i8** %format.text39, align 8
  %"$bool.text42" = load i8*, i8** %bool.text, align 8
  %"$bool.text43" = load i8*, i8** %bool.text31, align 8
  %"$bool.text44" = load i8*, i8** %bool.text32, align 8
  %call.snprintf45 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text41", i64 %AS38, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.16, i32 0, i32 0), i32 %call.String.find, i8* %"$bool.text42", i8* %"$bool.text43", i8* %"$bool.text44", i32 %call.String.find29)
  %"$format.size46" = load i32, i32* %format.size30, align 4
  %ADD47 = add i32 %"$format.size46", 1
  %"$format.text48" = load i8*, i8** %format.text39, align 8
  %"$format.size49" = load i32, i32* %format.size30, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text48", 0
  %7 = insertvalue %String %6, i32 %"$format.size49", 1
  %8 = insertvalue %String %7, i32 %ADD47, 2
  call void @"io::out"(%String %8)
  %call.String.replace = call %String @String.replace(%String* %t, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.17, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.18, i32 0, i32 0))
  store %String %call.String.replace, %String* %r, align 8
  %call.String.chars50 = call i8* @String.chars(%String* %r)
  %call.String.substr = call %String @String.substr(%String* %r, i32 1, i32 3)
  store %String %call.String.substr, %String* %tmp51, align 8
  %call.String.chars52 = call i8* @String.chars(%String* %tmp51)
  %call.String.repeat = call %String @String.repeat(%String* %r, i32 2)
  store %String %call.String.repeat, %String* %tmp53, align 8
  %call.String.chars54 = call i8* @String.chars(%String* %tmp53)
  %call.snprintf56 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.19, i32 0, i32 0), i8* %call.String.chars50, i8* %call.String.chars52, i8* %call.String.chars54)
  store i32 %call.snprintf56, i32* %format.size55, align 4
  %"$format.size57" = load i32, i32* %format.size55, align 4
  %ADD58 = add i32 %"$format.size57", 1
  %AS59 = sext i32 %ADD58 to i64
  %call.calloc61 = call i8* @calloc(i64 %AS59, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc61, i8** %format.text60, align 8
  %"$format.text62" = load i8*, i8** %format.text60, align 8
  %call.snprintf63 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text62", i64 %AS59, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.19, i32 0, i32 0), i8* %call.String.chars50, i8* %call.String.chars52, i8* %call.String.chars54)
  %"$format.size64" = load i32, i32* %format.size55, align 4
  %ADD65 = add i32 %"$format.size64", 1
  %"$format.text66" = load i8*, i8** %format.text60, align 8
  %"$format.size67" = load i32, i32* %format.size55, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text66", 0
  %10 = insertvalue %String %9, i32 %"$format.size67", 1
  %11 = insertvalue %String %10, i32 %ADD65, 2
  call void @"io::out"(%String %11)
  call void @String.drop(%String* %tmp53)
  call void @String.drop(%String* %tmp51)
  %call.String.create = call %String @String.create(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.20, i32 0, i32 0))
  store %String %call.String.create, %String* %a, align 8
  store %String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.21, i32 0, i32 0), i32 3, i32 0 }, %String* %b, align 8
  %"call.String.+" = call %String @"String.+"(%String* %a, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.22, i32 0, i32 0))
  store %String %"call.String.+", %String* %tmp68, align 8
  %"call.String.+69" = call %String @"String.+.2"(%String* %tmp68, %String* %b)
  store %String %"call.String.+69", %String* %c, align 8
  call void @String.drop(%String* %tmp68)
  call void @"String.+="(%String* %c, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.23, i32 0, i32 0))
  %call.String.chars70 = call i8* @String.chars(%String* %c)
  %"call.String.<" = call i1 @"String.<"(%String* %a, %String* %b)
  store %String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.24, i32 0, i32 0), i32 3, i32 0 }, %String* %tmp71, align 8
  %"call.String.==" = call i1 @"String.=="(%String* %a, %String* %tmp71)
  %"call.String.!=" = call i1 @"String.!="(%String* %a, %String* %b)
  %call.String.size72 = call i32 @String.size(%String* %c)
  store i32 0, i32* %format.size73, align 4
  store i8* null, i8** %bool.text74, align 8
  br label %if45

if45:                                             ; preds = %endif2
  br i1 %"call.String.<", label %if45.then, label %else46

if45.then:                                        ; preds = %if45
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.25, i32 0, i32 0), i8** %bool.text74, align 8
  br label %endif3

else46:                                           ; preds = %if45
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.26, i32 0, i32 0), i8** %bool.text74, align 8
  br label %endif3

endif3:                                           ; preds = %else46, %if45.then
  store i8* null, i8** %bool.text75, align 8
  br label %if47

if47:                                             ; preds = %endif3
  br i1 %"call.String.==", label %if47.then, label %else48

if47.then:                                        ; preds = %if47
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.27, i32 0, i32 0), i8** %bool.text75, align 8
  br label %endif4

else48:                                           ; preds = %if47
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.28, i32 0, i32 0), i8** %bool.text75, align 8
  br label %endif4

endif4:                                           ; preds = %else48, %if47.then
  store i8* null, i8** %bool.text76, align 8
  br label %if49

if49:                                             ; preds = %endif4
  br i1 %"call.String.!=", label %if49.then, label %else50

if49.then:                                        ; preds = %if49
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.29, i32 0, i32 0), i8** %bool.text76, align 8
  br label %endif5

else50:                                           ; preds = %if49
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.30, i32 0, i32 0), i8** %bool.text76, align 8
  br label %endif5

endif5:                                           ; preds = %else50, %if49.then
  %"$bool.text77" = load i8*, i8** %bool.text74, align 8
  %"$bool.text78" = load i8*, i8** %bool.text75, align 8
  %"$bool.text79" = load i8*, i8** %bool.text76, align 8
  %call.snprintf80 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.31, i32 0, i32 0), i8* %call.String.chars70, i8* %"$bool.text77", i8* %"$bool.text78", i8* %"$bool.text79", i32 %call.String.size72)
  store i32 %call.snprintf80, i32* %format.size73, align 4
  %"$format.size81" = load i32, i32* %format.size73, align 4
  %ADD82 = add i32 %"$format.size81", 1
  %AS83 = sext i32 %ADD82 to i64
  %call.calloc85 = call i8* @calloc(i64 %AS83, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc85, i8** %format.text84, align 8
  %"$format.text86" = load i8*, i8** %format.text84, align 8
  %"$bool.text87" = load i8*, i8** %bool.text74, align 8
  %"$bool.text88" = load i8*, i8** %bool.text75, align 8
  %"$bool.text89" = load i8*, i8** %bool.text76, align 8
  %call.snprintf90 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text86", i64 %AS83, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.31, i32 0, i32 0), i8* %call.String.chars70, i8* %"$bool.text87", i8* %"$bool.text88", i8* %"$bool.text89", i32 %call.String.size72)
  %"$format.size91" = load i32, i32* %format.size73, align 4
  %ADD92 = add i32 %"$format.size91", 1
  %"$format.text93" = load i8*, i8** %format.text84, align 8
  %"$format.size94" = load i32, i32* %format.size73, align 4
  %12 = insertvalue %String zeroinitializer, i8* %"$format.text93", 0
  %13 = insertvalue %String %12, i32 %"$format.size94", 1
  %14 = insertvalue %String %13, i32 %ADD92, 2
  call void @"io::out"(%String %14)
  call void @String.drop(%String* %tmp71)
  store %String { i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.32, i32 0, i32 0), i32 2, i32 0 }, %String* %n, align 8
  %call.String.to_int = call i32 @String.to_int(%String* %n)
  %ADD95 = add i32 %call.String.to_int, 1
  %call.snprintf97 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.33, i32 0, i32 0), i32 %ADD95, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @TYPEOF, i32 0, i32 0))
  store i32 %call.snprintf97, i32* %format.size96, align 4
  %"$format.size98" = load i32, i32* %format.size96, align 4
  %ADD99 = add i32 %"$format.size98", 1
  %AS100 = sext i32 %ADD99 to i64
  %call.calloc102 = call i8* @calloc(i64 %AS100, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc102, i8** %format.text101, align 8
  %"$format.text103" = load i8*, i8** %format.text101, align 8
  %call.snprintf104 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text103", i64 %AS100, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.33, i32 0, i32 0), i32 %ADD95, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @TYPEOF, i32 0, i32 0))
  %"$format.size105" = load i32, i32* %format.size96, align 4
  %ADD106 = add i32 %"$format.size105", 1
  %"$format.text107" = load i8*, i8** %format.text101, align 8
  %"$format.size108" = load i32, i32* %format.size96, align 4
  %15 = insertvalue %String zeroinitializer, i8* %"$format.text107", 0
  %16 = insertvalue %String %15, i32 %"$format.size108", 1
  %17 = insertvalue %String %16, i32 %ADD106, 2
  call void @"io::out"(%String %17)
  call void @String.drop(%String* %n)
  call void @String.drop(%String* %c)
  call void @String.drop(%String* %b)
  call void @String.drop(%String* %a)
  call void @String.drop(%String* %r)
  call void @String.drop(%String* %t)
  call void @String.drop(%String* %s)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

---

## 039 — struct: a plain fn drop() is a normal method, only operator drop runs by itself

```ura
struct Plain:
    id i32

    fn drop():
        io::out($"plain drop {self.id}\n")

struct Owned:
    id i32

    operator drop():
        io::out($"owned drop {self.id}\n")

fn main() i32:
    p Plain
    p.id = 1
    o Owned
    o.id = 2
    p.drop()
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }
%Plain = type { i32 }
%Owned = type { i32 }

@str = private unnamed_addr constant [15 x i8] c"plain drop %d\0A\00", align 1
@str.1 = private unnamed_addr constant [15 x i8] c"owned drop %d\0A\00", align 1

define i8* @"new<i8>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  ret i8* %call.calloc
}

define void @"copy<i8>"(i8* %0, i8* %1, i32 %2) {
entry:
  %dst = alloca i8*, align 8
  %src = alloca i8*, align 8
  %count = alloca i32, align 4
  store i8* %0, i8** %dst, align 8
  store i8* %1, i8** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load i8*, i8** %dst, align 8
  %"$src" = load i8*, i8** %src, align 8
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %MUL1 = mul i64 %AS, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %"$dst", i8* %"$src", i64 %MUL1)
  ret void
}

define void @"free<i8>"(i8* %0) {
entry:
  %data = alloca i8*, align 8
  store i8* %0, i8** %data, align 8
  %"$data" = load i8*, i8** %data, align 8
  call void @free(i8* %"$data")
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  ret void

endif1:                                           ; preds = %if1
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<i8>" = call i8* @"new<i8>"(i32 %"$cap8")
  store i8* %"call.new<i8>", i8** %bigger, align 8
  %"*self9" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self9", i32 0, i32 0
  %"*self10" = load %String*, %String** %self, align 8
  %"*self.count11" = getelementptr inbounds %String, %String* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if2

if2:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %String*, %String** %self, align 8
  %"*self.cap23" = getelementptr inbounds %String, %String* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %while4

while4:                                           ; preds = %while4.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.count5" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.count9" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while4

endloop:                                          ; preds = %while4
  ret void
}

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
  ret void
}

define i8* @String.chars(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count5"
  store i8 0, i8* %"*self.data[]", align 1
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  ret i8* %"$*self.data8"
}

declare i64 @write(i32, i8*, i64)

define void @"io::out"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 1, i8* %call.String.chars, i64 %AS)
  call void @String.drop(%String* %text)
  ret void
}

define void @Plain.drop(%Plain* %0) {
entry:
  %self = alloca %Plain*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Plain* %0, %Plain** %self, align 8
  %"*self" = load %Plain*, %Plain** %self, align 8
  %"*self.id" = getelementptr inbounds %Plain, %Plain* %"*self", i32 0, i32 0
  %"$*self.id" = load i32, i32* %"*self.id", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str, i32 0, i32 0), i32 %"$*self.id")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str, i32 0, i32 0), i32 %"$*self.id")
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %1 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %2 = insertvalue %String %1, i32 %"$format.size5", 1
  %3 = insertvalue %String %2, i32 %ADD3, 2
  call void @"io::out"(%String %3)
  ret void
}

define void @Owned.drop(%Owned* %0) {
entry:
  %self = alloca %Owned*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Owned* %0, %Owned** %self, align 8
  %"*self" = load %Owned*, %Owned** %self, align 8
  %"*self.id" = getelementptr inbounds %Owned, %Owned* %"*self", i32 0, i32 0
  %"$*self.id" = load i32, i32* %"*self.id", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.1, i32 0, i32 0), i32 %"$*self.id")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.1, i32 0, i32 0), i32 %"$*self.id")
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %1 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %2 = insertvalue %String %1, i32 %"$format.size5", 1
  %3 = insertvalue %String %2, i32 %ADD3, 2
  call void @"io::out"(%String %3)
  ret void
}

define i32 @main() {
entry:
  %p = alloca %Plain, align 8
  %o = alloca %Owned, align 8
  store %Plain zeroinitializer, %Plain* %p, align 4
  %p.id = getelementptr inbounds %Plain, %Plain* %p, i32 0, i32 0
  store i32 1, i32* %p.id, align 4
  store %Owned zeroinitializer, %Owned* %o, align 4
  %o.id = getelementptr inbounds %Owned, %Owned* %o, i32 0, i32 0
  store i32 2, i32* %o.id, align 4
  call void @Plain.drop(%Plain* %p)
  call void @Owned.drop(%Owned* %o)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

