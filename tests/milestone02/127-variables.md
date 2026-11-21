# milestone02

## index

- 127 — variable: an array type carries no size
- 128 — array: Array<T>: append, insert, pop, pop(index), first/last, resize, reverse
- 129 — use: a package that doesn't exist
- 130 — use: an error after a use points at the user's file and line
- 131 — operator: bitwise & | ^ << >> on scalars, compound forms, & stays a ref before a type, >> closes nested templates
- 132 — array: any integer type is an index, an unsigned one is zero-extended

---

## 127 — variable: an array type carries no size

```ura
// error: An array type carries no size; ex. 'arr int[] = int[3]' to make one

fn main() i32:
    xs i32[3]
    xs[0] = 7
    return 0
```

### stderr

```
error: test.ura:4:12 arrays have no size in the type
  |
4 |     xs i32[3]
  |            ^
help: use 'i32[]'
error: aborting due to 1 error
```

### status

```
1
```

---

## 128 — array: Array<T>: append, insert, pop, pop(index), first/last, resize, reverse

```ura
fn main() i32:
    a Array<i32>
    a.append(1)
    a.append(2)
    a.append(3)
    a.insert(0, 0)
    a.insert(2, 7)
    a[0] = 10
    io::out($"{a[0]} {a[1]} {a[2]} {a[3]} {a[4]} size {a.size()}\n")
    x i32 = a.pop()
    y i32 = a.pop(0)
    z i32 = a.pop(1)
    a.last() = 9
    io::out($"{x} {y} {z} {a.first()} {a.last()} {a.len()} {a.is_empty()}\n")
    a.resize(4)
    a.reverse()
    io::out($"{a[0]} {a[1]} {a[2]} {a[3]} cap {a.capacity()}\n")
    a.clear()
    io::out($"{a.size()} {a.is_empty()}\n")
    a.release()
    c Array<i8>
    c.append('h')
    c.append('i')
    io::out($"{c[0]}{c[1]} {typeof(c)}\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }
%"Array<i32>" = type { i32*, i32, i32 }

@str = private unnamed_addr constant [24 x i8] c"%d %d %d %d %d size %d\0A\00", align 1
@str.2 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.4 = private unnamed_addr constant [22 x i8] c"%d %d %d %d %d %d %s\0A\00", align 1
@str.5 = private unnamed_addr constant [20 x i8] c"%d %d %d %d cap %d\0A\00", align 1
@str.6 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.7 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.8 = private unnamed_addr constant [7 x i8] c"%d %s\0A\00", align 1
@TYPEOF = private unnamed_addr constant [10 x i8] c"Array<i8>\00", align 1
@str.9 = private unnamed_addr constant [9 x i8] c"%c%c %s\0A\00", align 1

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

define void @"Array<i8>.reserve"(%"Array<i8>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self3" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self6" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self6", i32 0, i32 2
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
  %"*self9" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self9", i32 0, i32 0
  %"*self10" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self10", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count12")
  br label %if2

if2:                                              ; preds = %endif1
  %"*self13" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap14" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if2.then, label %endif2

if2.then:                                         ; preds = %if2
  %"*self16" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data17" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if2.then, %if2
  %"*self19" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data20" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self19", i32 0, i32 0
  %"$bigger21" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger21", i8** %"*self.data20", align 8
  %"*self22" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap23" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @"Array<i8>.own"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %AND = and i1 %EQ, %GT
  br i1 %AND, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self3" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  call void @"Array<i8>.reserve"(%"Array<i8>"* %"*self2", i32 %"$*self.count5")
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  ret void
}

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if4

if4:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if4.then, label %endif

if4.then:                                         ; preds = %if4
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if4.then, %if4
  br label %while5

while5:                                           ; preds = %while5.then, %endif
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while5.then, label %endloop

while5.then:                                      ; preds = %while5
  %"*self4" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count10"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while5

endloop:                                          ; preds = %while5
  ret void
}

define void @"Array<i8>.append"(%"Array<i8>"* %0, i8 %1) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %value = alloca i8, align 1
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store i8 %1, i8* %value, align 1
  br label %if6

if6:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self3" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<i8>.reserve"(%"Array<i8>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if6.then, %if6
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

define i8* @"Array<i8>.[]"(%"Array<i8>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  %index = alloca i32, align 4
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  store i32 %1, i32* %index, align 4
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  call void @"Array<i8>.own"(%"Array<i8>"* %"*self")
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 0
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
  br label %if7

if7:                                              ; preds = %entry
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if7.then, label %endif

if7.then:                                         ; preds = %if7
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if7.then, %if7
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if8

if8:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if8.then, label %endif

if8.then:                                         ; preds = %if8
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if8.then, %if8
  br label %if9

if9:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if9.then, label %endif1

if9.then:                                         ; preds = %if9
  ret void

endif1:                                           ; preds = %if9
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
  br label %if10

if10:                                             ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if10.then, label %endif2

if10.then:                                        ; preds = %if10
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if10.then, %if10
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
  br label %if11

if11:                                             ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if11.then, label %endif

if11.then:                                        ; preds = %if11
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if11.then, %if11
  br label %while12

while12:                                          ; preds = %while12.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while12.then, label %endloop

while12.then:                                     ; preds = %while12
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
  br label %while12

endloop:                                          ; preds = %while12
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
  br label %if13

if13:                                             ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if13.then, label %endif

if13.then:                                        ; preds = %if13
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if13.then, %if13
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
  br label %if14

if14:                                             ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if14.then, label %endif

if14.then:                                        ; preds = %if14
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if14.then, %if14
  br label %if15

if15:                                             ; preds = %endif
  %"*self6" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if15.then, label %endif1

if15.then:                                        ; preds = %if15
  ret void

endif1:                                           ; preds = %if15
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
  br label %if16

if16:                                             ; preds = %endif1
  %"*self13" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap14" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if16.then, label %endif2

if16.then:                                        ; preds = %if16
  %"*self16" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data17" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i32*, i32** %"*self.data17", align 8
  call void @"free<i32>"(i32* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if16.then, %if16
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
  br label %if17

if17:                                             ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %AND = and i1 %EQ, %GT
  br i1 %AND, label %if17.then, label %endif

if17.then:                                        ; preds = %if17
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self2", i32 %"$*self.count5")
  br label %endif

endif:                                            ; preds = %if17.then, %if17
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
  br label %while18

while18:                                          ; preds = %while18.then, %entry
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$size2" = load i32, i32* %size, align 4
  %GT = icmp sgt i32 %"$*self.count", %"$size2"
  br i1 %GT, label %while18.then, label %endloop

while18.then:                                     ; preds = %while18
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
  br label %while18

endloop:                                          ; preds = %while18
  %"*self10" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self10", i32 0, i32 1
  %"$size12" = load i32, i32* %size, align 4
  store i32 %"$size12", i32* %"*self.count11", align 4
  ret void
}

define void @"Array<i32>.clear"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  br label %if19

if19:                                             ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if19.then, label %endif

if19.then:                                        ; preds = %if19
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if19.then, %if19
  br label %while20

while20:                                          ; preds = %while20.then, %endif
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while20.then, label %endloop

while20.then:                                     ; preds = %while20
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
  br label %while20

endloop:                                          ; preds = %while20
  ret void
}

define void @"Array<i32>.release"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  call void @"Array<i32>.clear"(%"Array<i32>"* %"*self")
  br label %if21

if21:                                             ; preds = %entry
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if21.then, label %endif

if21.then:                                        ; preds = %if21
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  call void @"free<i32>"(i32* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if21.then, %if21
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 0
  store i32* null, i32** %"*self.data4", align 8
  %"*self5" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap6" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self5", i32 0, i32 2
  store i32 0, i32* %"*self.cap6", align 4
  ret void
}

define void @"Array<i32>.append"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %value = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %value, align 4
  br label %if22

if22:                                             ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if22.then, label %endif

if22.then:                                        ; preds = %if22
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if22.then, %if22
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

define void @"Array<i32>.insert"(%"Array<i32>"* %0, i32 %1, i32 %2) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %index = alloca i32, align 4
  %value = alloca i32, align 4
  %i = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %index, align 4
  store i32 %2, i32* %value, align 4
  br label %if23

if23:                                             ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if23.then, label %endif

if23.then:                                        ; preds = %if23
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if23.then, %if23
  %"*self6" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count7" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self6", i32 0, i32 1
  %"$*self.count8" = load i32, i32* %"*self.count7", align 4
  store i32 %"$*self.count8", i32* %i, align 4
  br label %while24

while24:                                          ; preds = %while24.then, %endif
  %"$i" = load i32, i32* %i, align 4
  %"$index" = load i32, i32* %index, align 4
  %GT = icmp sgt i32 %"$i", %"$index"
  br i1 %GT, label %while24.then, label %endloop

while24.then:                                     ; preds = %while24
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
  br label %while24

endloop:                                          ; preds = %while24
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

define i32 @"Array<i32>.pop"(%"Array<i32>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %index = alloca i32, align 4
  %value = alloca i32, align 4
  %i = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  store i32 %1, i32* %index, align 4
  store i32 0, i32* %value, align 4
  br label %if25

if25:                                             ; preds = %entry
  %"$index" = load i32, i32* %index, align 4
  %LT = icmp slt i32 %"$index", 0
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$index1" = load i32, i32* %index, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GE = icmp sge i32 %"$index1", %"$*self.count"
  %OR = or i1 %LT, %GE
  br i1 %OR, label %if25.then, label %endif

if25.then:                                        ; preds = %if25
  %"$value" = load i32, i32* %value, align 4
  ret i32 %"$value"

endif:                                            ; preds = %if25
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  call void @"Array<i32>.own"(%"Array<i32>"* %"*self2")
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$index4" = load i32, i32* %index, align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$index4"
  %"$*self.data[]" = load i32, i32* %"*self.data[]", align 4
  store i32 %"$*self.data[]", i32* %value, align 4
  %"$index5" = load i32, i32* %index, align 4
  store i32 %"$index5", i32* %i, align 4
  br label %while26

while26:                                          ; preds = %while26.then, %endif
  %"*self6" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count7" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self6", i32 0, i32 1
  %"$*self.count8" = load i32, i32* %"*self.count7", align 4
  %SUB = sub i32 %"$*self.count8", 1
  %"$i" = load i32, i32* %i, align 4
  %LT9 = icmp slt i32 %"$i", %SUB
  br i1 %LT9, label %while26.then, label %endloop

while26.then:                                     ; preds = %while26
  %"*self10" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data11" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self10", i32 0, i32 0
  %"$*self.data12" = load i32*, i32** %"*self.data11", align 8
  %"$i13" = load i32, i32* %i, align 4
  %"*self.data[]14" = getelementptr inbounds i32, i32* %"$*self.data12", i32 %"$i13"
  %"*self15" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data16" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self15", i32 0, i32 0
  %"$i17" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i17", 1
  %"$*self.data18" = load i32*, i32** %"*self.data16", align 8
  %"*self.data[]19" = getelementptr inbounds i32, i32* %"$*self.data18", i32 %ADD
  %"$*self.data[]20" = load i32, i32* %"*self.data[]19", align 4
  store i32 %"$*self.data[]20", i32* %"*self.data[]14", align 4
  %"$i21" = load i32, i32* %i, align 4
  %ADD22 = add i32 %"$i21", 1
  store i32 %ADD22, i32* %i, align 4
  br label %while26

endloop:                                          ; preds = %while26
  %"*self23" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count24" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self23", i32 0, i32 1
  %"$*self.count25" = load i32, i32* %"*self.count24", align 4
  %SUB26 = sub i32 %"$*self.count25", 1
  store i32 %SUB26, i32* %"*self.count24", align 4
  %"*self27" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data28" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self27", i32 0, i32 0
  %"*self29" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count30" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self29", i32 0, i32 1
  %"$*self.data31" = load i32*, i32** %"*self.data28", align 8
  %"$*self.count32" = load i32, i32* %"*self.count30", align 4
  %"*self.data[]33" = getelementptr inbounds i32, i32* %"$*self.data31", i32 %"$*self.count32"
  store i32 0, i32* %"*self.data[]33", align 4
  %"$value34" = load i32, i32* %value, align 4
  ret i32 %"$value34"
}

define i32 @"Array<i32>.pop.1"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  %"call.Array<i32>.pop" = call i32 @"Array<i32>.pop"(%"Array<i32>"* %"*self", i32 %SUB)
  ret i32 %"call.Array<i32>.pop"
}

define void @"Array<i32>.reverse"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %tmp = alloca i32, align 4
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  call void @"Array<i32>.own"(%"Array<i32>"* %"*self")
  store i32 0, i32* %i, align 4
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %SUB = sub i32 %"$*self.count", 1
  store i32 %SUB, i32* %j, align 4
  br label %while27

while27:                                          ; preds = %while27.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %"$j" = load i32, i32* %j, align 4
  %LT = icmp slt i32 %"$i", %"$j"
  br i1 %LT, label %while27.then, label %endloop

while27.then:                                     ; preds = %while27
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"$i3" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 %"$i3"
  %"$*self.data[]" = load i32, i32* %"*self.data[]", align 4
  store i32 %"$*self.data[]", i32* %tmp, align 4
  %"*self4" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data5" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self4", i32 0, i32 0
  %"$*self.data6" = load i32*, i32** %"*self.data5", align 8
  %"$i7" = load i32, i32* %i, align 4
  %"*self.data[]8" = getelementptr inbounds i32, i32* %"$*self.data6", i32 %"$i7"
  %"*self9" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data10" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self9", i32 0, i32 0
  %"$*self.data11" = load i32*, i32** %"*self.data10", align 8
  %"$j12" = load i32, i32* %j, align 4
  %"*self.data[]13" = getelementptr inbounds i32, i32* %"$*self.data11", i32 %"$j12"
  %"$*self.data[]14" = load i32, i32* %"*self.data[]13", align 4
  store i32 %"$*self.data[]14", i32* %"*self.data[]8", align 4
  %"*self15" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data16" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self15", i32 0, i32 0
  %"$*self.data17" = load i32*, i32** %"*self.data16", align 8
  %"$j18" = load i32, i32* %j, align 4
  %"*self.data[]19" = getelementptr inbounds i32, i32* %"$*self.data17", i32 %"$j18"
  %"$tmp" = load i32, i32* %tmp, align 4
  store i32 %"$tmp", i32* %"*self.data[]19", align 4
  %"$i20" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i20", 1
  store i32 %ADD, i32* %i, align 4
  %"$j21" = load i32, i32* %j, align 4
  %SUB22 = sub i32 %"$j21", 1
  store i32 %SUB22, i32* %j, align 4
  br label %while27

endloop:                                          ; preds = %while27
  ret void
}

define i32* @"Array<i32>.first"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  %"*self.data[]" = getelementptr inbounds i32, i32* %"$*self.data", i32 0
  ret i32* %"*self.data[]"
}

define i32* @"Array<i32>.last"(%"Array<i32>"* %0) {
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

define i32 @"Array<i32>.len"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
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

define i1 @"Array<i32>.is_empty"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %EQ = icmp eq i32 %"$*self.count", 0
  ret i1 %EQ
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

define void @"Array<i32>.drop"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  call void @"Array<i32>.clear"(%"Array<i32>"* %"*self")
  br label %if28

if28:                                             ; preds = %entry
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if28.then, label %endif

if28.then:                                        ; preds = %if28
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  call void @"free<i32>"(i32* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if28.then, %if28
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
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  %z = alloca i32, align 4
  %ref28 = alloca i32*, align 8
  %ref30 = alloca i32*, align 8
  %ref33 = alloca i32*, align 8
  %format.size35 = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text42 = alloca i8*, align 8
  %ref52 = alloca i32*, align 8
  %ref55 = alloca i32*, align 8
  %ref58 = alloca i32*, align 8
  %ref61 = alloca i32*, align 8
  %format.size63 = alloca i32, align 4
  %format.text72 = alloca i8*, align 8
  %format.size82 = alloca i32, align 4
  %bool.text83 = alloca i8*, align 8
  %format.text89 = alloca i8*, align 8
  %c = alloca %"Array<i8>", align 8
  %ref98 = alloca i8*, align 8
  %ref101 = alloca i8*, align 8
  %format.size103 = alloca i32, align 4
  %format.text112 = alloca i8*, align 8
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %a, align 8
  call void @"Array<i32>.append"(%"Array<i32>"* %a, i32 1)
  call void @"Array<i32>.append"(%"Array<i32>"* %a, i32 2)
  call void @"Array<i32>.append"(%"Array<i32>"* %a, i32 3)
  call void @"Array<i32>.insert"(%"Array<i32>"* %a, i32 0, i32 0)
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
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([24 x i8], [24 x i8]* @str, i32 0, i32 0), i32 %"$*ref", i32 %"$*ref17", i32 %"$*ref18", i32 %"$*ref19", i32 %"$*ref20", i32 %"call.Array<i32>.size")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf21 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([24 x i8], [24 x i8]* @str, i32 0, i32 0), i32 %"$*ref", i32 %"$*ref17", i32 %"$*ref18", i32 %"$*ref19", i32 %"$*ref20", i32 %"call.Array<i32>.size")
  %"$format.size22" = load i32, i32* %format.size, align 4
  %ADD23 = add i32 %"$format.size22", 1
  %"$format.text24" = load i8*, i8** %format.text, align 8
  %"$format.size25" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text24", 0
  %1 = insertvalue %String %0, i32 %"$format.size25", 1
  %2 = insertvalue %String %1, i32 %ADD23, 2
  call void @"io::out"(%String %2)
  %"call.Array<i32>.pop" = call i32 @"Array<i32>.pop.1"(%"Array<i32>"* %a)
  store i32 %"call.Array<i32>.pop", i32* %x, align 4
  %"call.Array<i32>.pop26" = call i32 @"Array<i32>.pop"(%"Array<i32>"* %a, i32 0)
  store i32 %"call.Array<i32>.pop26", i32* %y, align 4
  %"call.Array<i32>.pop27" = call i32 @"Array<i32>.pop"(%"Array<i32>"* %a, i32 1)
  store i32 %"call.Array<i32>.pop27", i32* %z, align 4
  %"call.Array<i32>.last" = call i32* @"Array<i32>.last"(%"Array<i32>"* %a)
  store i32* %"call.Array<i32>.last", i32** %ref28, align 8
  %"*ref29" = load i32*, i32** %ref28, align 8
  store i32 9, i32* %"*ref29", align 4
  %"call.Array<i32>.first" = call i32* @"Array<i32>.first"(%"Array<i32>"* %a)
  store i32* %"call.Array<i32>.first", i32** %ref30, align 8
  %"*ref31" = load i32*, i32** %ref30, align 8
  %"call.Array<i32>.last32" = call i32* @"Array<i32>.last"(%"Array<i32>"* %a)
  store i32* %"call.Array<i32>.last32", i32** %ref33, align 8
  %"*ref34" = load i32*, i32** %ref33, align 8
  %"call.Array<i32>.len" = call i32 @"Array<i32>.len"(%"Array<i32>"* %a)
  %"call.Array<i32>.is_empty" = call i1 @"Array<i32>.is_empty"(%"Array<i32>"* %a)
  store i32 0, i32* %format.size35, align 4
  %"$x" = load i32, i32* %x, align 4
  %"$y" = load i32, i32* %y, align 4
  %"$z" = load i32, i32* %z, align 4
  %"$*ref36" = load i32, i32* %"*ref31", align 4
  %"$*ref37" = load i32, i32* %"*ref34", align 4
  store i8* null, i8** %bool.text, align 8
  br label %if29

if29:                                             ; preds = %entry
  br i1 %"call.Array<i32>.is_empty", label %if29.then, label %else30

if29.then:                                        ; preds = %if29
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else30:                                           ; preds = %if29
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else30, %if29.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf38 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @str.4, i32 0, i32 0), i32 %"$x", i32 %"$y", i32 %"$z", i32 %"$*ref36", i32 %"$*ref37", i32 %"call.Array<i32>.len", i8* %"$bool.text")
  store i32 %call.snprintf38, i32* %format.size35, align 4
  %"$format.size39" = load i32, i32* %format.size35, align 4
  %ADD40 = add i32 %"$format.size39", 1
  %AS41 = sext i32 %ADD40 to i64
  %call.calloc43 = call i8* @calloc(i64 %AS41, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc43, i8** %format.text42, align 8
  %"$format.text44" = load i8*, i8** %format.text42, align 8
  %"$bool.text45" = load i8*, i8** %bool.text, align 8
  %call.snprintf46 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text44", i64 %AS41, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @str.4, i32 0, i32 0), i32 %"$x", i32 %"$y", i32 %"$z", i32 %"$*ref36", i32 %"$*ref37", i32 %"call.Array<i32>.len", i8* %"$bool.text45")
  %"$format.size47" = load i32, i32* %format.size35, align 4
  %ADD48 = add i32 %"$format.size47", 1
  %"$format.text49" = load i8*, i8** %format.text42, align 8
  %"$format.size50" = load i32, i32* %format.size35, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text49", 0
  %4 = insertvalue %String %3, i32 %"$format.size50", 1
  %5 = insertvalue %String %4, i32 %ADD48, 2
  call void @"io::out"(%String %5)
  call void @"Array<i32>.resize"(%"Array<i32>"* %a, i32 4)
  call void @"Array<i32>.reverse"(%"Array<i32>"* %a)
  %"call.Array<i32>.[]51" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %a, i32 0)
  store i32* %"call.Array<i32>.[]51", i32** %ref52, align 8
  %"*ref53" = load i32*, i32** %ref52, align 8
  %"call.Array<i32>.[]54" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %a, i32 1)
  store i32* %"call.Array<i32>.[]54", i32** %ref55, align 8
  %"*ref56" = load i32*, i32** %ref55, align 8
  %"call.Array<i32>.[]57" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %a, i32 2)
  store i32* %"call.Array<i32>.[]57", i32** %ref58, align 8
  %"*ref59" = load i32*, i32** %ref58, align 8
  %"call.Array<i32>.[]60" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %a, i32 3)
  store i32* %"call.Array<i32>.[]60", i32** %ref61, align 8
  %"*ref62" = load i32*, i32** %ref61, align 8
  %"call.Array<i32>.capacity" = call i32 @"Array<i32>.capacity"(%"Array<i32>"* %a)
  %"$*ref64" = load i32, i32* %"*ref53", align 4
  %"$*ref65" = load i32, i32* %"*ref56", align 4
  %"$*ref66" = load i32, i32* %"*ref59", align 4
  %"$*ref67" = load i32, i32* %"*ref62", align 4
  %call.snprintf68 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @str.5, i32 0, i32 0), i32 %"$*ref64", i32 %"$*ref65", i32 %"$*ref66", i32 %"$*ref67", i32 %"call.Array<i32>.capacity")
  store i32 %call.snprintf68, i32* %format.size63, align 4
  %"$format.size69" = load i32, i32* %format.size63, align 4
  %ADD70 = add i32 %"$format.size69", 1
  %AS71 = sext i32 %ADD70 to i64
  %call.calloc73 = call i8* @calloc(i64 %AS71, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc73, i8** %format.text72, align 8
  %"$format.text74" = load i8*, i8** %format.text72, align 8
  %call.snprintf75 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text74", i64 %AS71, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @str.5, i32 0, i32 0), i32 %"$*ref64", i32 %"$*ref65", i32 %"$*ref66", i32 %"$*ref67", i32 %"call.Array<i32>.capacity")
  %"$format.size76" = load i32, i32* %format.size63, align 4
  %ADD77 = add i32 %"$format.size76", 1
  %"$format.text78" = load i8*, i8** %format.text72, align 8
  %"$format.size79" = load i32, i32* %format.size63, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text78", 0
  %7 = insertvalue %String %6, i32 %"$format.size79", 1
  %8 = insertvalue %String %7, i32 %ADD77, 2
  call void @"io::out"(%String %8)
  call void @"Array<i32>.clear"(%"Array<i32>"* %a)
  %"call.Array<i32>.size80" = call i32 @"Array<i32>.size"(%"Array<i32>"* %a)
  %"call.Array<i32>.is_empty81" = call i1 @"Array<i32>.is_empty"(%"Array<i32>"* %a)
  store i32 0, i32* %format.size82, align 4
  store i8* null, i8** %bool.text83, align 8
  br label %if31

if31:                                             ; preds = %endif
  br i1 %"call.Array<i32>.is_empty81", label %if31.then, label %else32

if31.then:                                        ; preds = %if31
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.6, i32 0, i32 0), i8** %bool.text83, align 8
  br label %endif1

else32:                                           ; preds = %if31
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.7, i32 0, i32 0), i8** %bool.text83, align 8
  br label %endif1

endif1:                                           ; preds = %else32, %if31.then
  %"$bool.text84" = load i8*, i8** %bool.text83, align 8
  %call.snprintf85 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.8, i32 0, i32 0), i32 %"call.Array<i32>.size80", i8* %"$bool.text84")
  store i32 %call.snprintf85, i32* %format.size82, align 4
  %"$format.size86" = load i32, i32* %format.size82, align 4
  %ADD87 = add i32 %"$format.size86", 1
  %AS88 = sext i32 %ADD87 to i64
  %call.calloc90 = call i8* @calloc(i64 %AS88, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc90, i8** %format.text89, align 8
  %"$format.text91" = load i8*, i8** %format.text89, align 8
  %"$bool.text92" = load i8*, i8** %bool.text83, align 8
  %call.snprintf93 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text91", i64 %AS88, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.8, i32 0, i32 0), i32 %"call.Array<i32>.size80", i8* %"$bool.text92")
  %"$format.size94" = load i32, i32* %format.size82, align 4
  %ADD95 = add i32 %"$format.size94", 1
  %"$format.text96" = load i8*, i8** %format.text89, align 8
  %"$format.size97" = load i32, i32* %format.size82, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text96", 0
  %10 = insertvalue %String %9, i32 %"$format.size97", 1
  %11 = insertvalue %String %10, i32 %ADD95, 2
  call void @"io::out"(%String %11)
  call void @"Array<i32>.release"(%"Array<i32>"* %a)
  store %"Array<i8>" zeroinitializer, %"Array<i8>"* %c, align 8
  call void @"Array<i8>.append"(%"Array<i8>"* %c, i8 104)
  call void @"Array<i8>.append"(%"Array<i8>"* %c, i8 105)
  %"call.Array<i8>.[]" = call i8* @"Array<i8>.[]"(%"Array<i8>"* %c, i32 0)
  store i8* %"call.Array<i8>.[]", i8** %ref98, align 8
  %"*ref99" = load i8*, i8** %ref98, align 8
  %"call.Array<i8>.[]100" = call i8* @"Array<i8>.[]"(%"Array<i8>"* %c, i32 1)
  store i8* %"call.Array<i8>.[]100", i8** %ref101, align 8
  %"*ref102" = load i8*, i8** %ref101, align 8
  %"$*ref104" = load i8, i8* %"*ref99", align 1
  %AS105 = sext i8 %"$*ref104" to i32
  %"$*ref106" = load i8, i8* %"*ref102", align 1
  %AS107 = sext i8 %"$*ref106" to i32
  %call.snprintf108 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.9, i32 0, i32 0), i32 %AS105, i32 %AS107, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @TYPEOF, i32 0, i32 0))
  store i32 %call.snprintf108, i32* %format.size103, align 4
  %"$format.size109" = load i32, i32* %format.size103, align 4
  %ADD110 = add i32 %"$format.size109", 1
  %AS111 = sext i32 %ADD110 to i64
  %call.calloc113 = call i8* @calloc(i64 %AS111, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc113, i8** %format.text112, align 8
  %"$format.text114" = load i8*, i8** %format.text112, align 8
  %call.snprintf115 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text114", i64 %AS111, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.9, i32 0, i32 0), i32 %AS105, i32 %AS107, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @TYPEOF, i32 0, i32 0))
  %"$format.size116" = load i32, i32* %format.size103, align 4
  %ADD117 = add i32 %"$format.size116", 1
  %"$format.text118" = load i8*, i8** %format.text112, align 8
  %"$format.size119" = load i32, i32* %format.size103, align 4
  %12 = insertvalue %String zeroinitializer, i8* %"$format.text118", 0
  %13 = insertvalue %String %12, i32 %"$format.size119", 1
  %14 = insertvalue %String %13, i32 %ADD117, 2
  call void @"io::out"(%String %14)
  call void @"Array<i8>.drop"(%"Array<i8>"* %c)
  call void @"Array<i32>.drop"(%"Array<i32>"* %a)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
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
help: add a dependency that has it: ura install <name> <version> <repo url>
error: aborting due to 1 error
```

### status

```
1
```

---

## 130 — use: an error after a use points at the user's file and line

```ura
use "@/string"

fn main() i32:
    a Array<i32>
    a.append("text")
    return 0
```

### stderr

```
error: test.ura:5:7 'Array<i32>' has no method 'append' that takes (String)
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

fn main() i32:
    a i32 = 6
    b i32 = 3
    io::out($"{a & b} {a | b} {a ^ b} {1 << 4} {-16 >> 2}\n")
    u u32 = 4294967280
    io::out($"{u >> 28}\n")
    c i32 = 1
    c <<= 3
    c |= 1
    c &= 13
    c ^= 4
    c >>= 1
    io::out($"{c} {1 + 2 << 1} {a & b == 2}\n")
    r &i32 = &a
    r = 9
    grid Array<Array<i32>>
    io::out($"{a} {grid.size() + 1}\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }
%"Array<i32>" = type { i32*, i32, i32 }
%"Array<Array<i32>>" = type { %"Array<i32>"*, i32, i32 }

@str = private unnamed_addr constant [16 x i8] c"%d %d %d %d %d\0A\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"%u\0A\00", align 1
@str.2 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.4 = private unnamed_addr constant [10 x i8] c"%d %d %s\0A\00", align 1
@str.5 = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1

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

define void @"free<i32>"(i32* %0) {
entry:
  %data = alloca i32*, align 8
  store i32* %0, i32** %data, align 8
  %"$data" = load i32*, i32** %data, align 8
  %AS = bitcast i32* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define void @"Array<i32>.clear"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  br label %if6

if6:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  br label %while7

while7:                                           ; preds = %while7.then, %endif
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while7.then, label %endloop

while7.then:                                      ; preds = %while7
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
  br label %while7

endloop:                                          ; preds = %while7
  ret void
}

define void @"Array<i32>.drop"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  call void @"Array<i32>.clear"(%"Array<i32>"* %"*self")
  br label %if8

if8:                                              ; preds = %entry
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if8.then, label %endif

if8.then:                                         ; preds = %if8
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  call void @"free<i32>"(i32* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if8.then, %if8
  ret void
}

define void @"free<Array<i32>>"(%"Array<i32>"* %0) {
entry:
  %data = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %data, align 8
  %"$data" = load %"Array<i32>"*, %"Array<i32>"** %data, align 8
  %AS = bitcast %"Array<i32>"* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define void @"Array<Array<i32>>.clear"(%"Array<Array<i32>>"* %0) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  br label %if9

if9:                                              ; preds = %entry
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if9.then, label %endif

if9.then:                                         ; preds = %if9
  %"*self1" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if9.then, %if9
  br label %while10

while10:                                          ; preds = %while10.then, %endif
  %"*self2" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while10.then, label %endloop

while10.then:                                     ; preds = %while10
  %"*self4" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"$*self.data", i32 %"$*self.count10"
  call void @"Array<i32>.drop"(%"Array<i32>"* %"*self.data[]")
  store %"Array<i32>" zeroinitializer, %"Array<i32>"* %"*self.data[]", align 8
  br label %while10

endloop:                                          ; preds = %while10
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

define void @"Array<Array<i32>>.drop"(%"Array<Array<i32>>"* %0) {
entry:
  %self = alloca %"Array<Array<i32>>"*, align 8
  store %"Array<Array<i32>>"* %0, %"Array<Array<i32>>"** %self, align 8
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  call void @"Array<Array<i32>>.clear"(%"Array<Array<i32>>"* %"*self")
  br label %if11

if11:                                             ; preds = %entry
  %"*self1" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if11.then, label %endif

if11.then:                                        ; preds = %if11
  %"*self2" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self2", i32 0, i32 0
  %"$*self.data" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data", align 8
  call void @"free<Array<i32>>"(%"Array<i32>"* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if11.then, %if11
  ret void
}

define i32 @main() {
entry:
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %u = alloca i32, align 4
  %format.size10 = alloca i32, align 4
  %format.text15 = alloca i8*, align 8
  %c = alloca i32, align 4
  %format.size34 = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text40 = alloca i8*, align 8
  %r = alloca i32*, align 8
  %grid = alloca %"Array<Array<i32>>", align 8
  %format.size50 = alloca i32, align 4
  %format.text56 = alloca i8*, align 8
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
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str, i32 0, i32 0), i32 %BAND, i32 %BOR, i32 %BXOR, i32 16, i32 -4)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf5 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str, i32 0, i32 0), i32 %BAND, i32 %BOR, i32 %BXOR, i32 16, i32 -4)
  %"$format.size6" = load i32, i32* %format.size, align 4
  %ADD7 = add i32 %"$format.size6", 1
  %"$format.text8" = load i8*, i8** %format.text, align 8
  %"$format.size9" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text8", 0
  %1 = insertvalue %String %0, i32 %"$format.size9", 1
  %2 = insertvalue %String %1, i32 %ADD7, 2
  call void @"io::out"(%String %2)
  store i32 -16, i32* %u, align 4
  %"$u" = load i32, i32* %u, align 4
  %RSHIFT = lshr i32 %"$u", 28
  %call.snprintf11 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 %RSHIFT)
  store i32 %call.snprintf11, i32* %format.size10, align 4
  %"$format.size12" = load i32, i32* %format.size10, align 4
  %ADD13 = add i32 %"$format.size12", 1
  %AS14 = sext i32 %ADD13 to i64
  %call.calloc16 = call i8* @calloc(i64 %AS14, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc16, i8** %format.text15, align 8
  %"$format.text17" = load i8*, i8** %format.text15, align 8
  %call.snprintf18 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text17", i64 %AS14, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 %RSHIFT)
  %"$format.size19" = load i32, i32* %format.size10, align 4
  %ADD20 = add i32 %"$format.size19", 1
  %"$format.text21" = load i8*, i8** %format.text15, align 8
  %"$format.size22" = load i32, i32* %format.size10, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text21", 0
  %4 = insertvalue %String %3, i32 %"$format.size22", 1
  %5 = insertvalue %String %4, i32 %ADD20, 2
  call void @"io::out"(%String %5)
  store i32 1, i32* %c, align 4
  %"$c" = load i32, i32* %c, align 4
  %LSHIFT = shl i32 %"$c", 3
  store i32 %LSHIFT, i32* %c, align 4
  %"$c23" = load i32, i32* %c, align 4
  %BOR24 = or i32 %"$c23", 1
  store i32 %BOR24, i32* %c, align 4
  %"$c25" = load i32, i32* %c, align 4
  %BAND26 = and i32 %"$c25", 13
  store i32 %BAND26, i32* %c, align 4
  %"$c27" = load i32, i32* %c, align 4
  %BXOR28 = xor i32 %"$c27", 4
  store i32 %BXOR28, i32* %c, align 4
  %"$c29" = load i32, i32* %c, align 4
  %RSHIFT30 = ashr i32 %"$c29", 1
  store i32 %RSHIFT30, i32* %c, align 4
  %"$a31" = load i32, i32* %a, align 4
  %"$b32" = load i32, i32* %b, align 4
  %BAND33 = and i32 %"$a31", %"$b32"
  %EQ = icmp eq i32 %BAND33, 2
  store i32 0, i32* %format.size34, align 4
  %"$c35" = load i32, i32* %c, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if12

if12:                                             ; preds = %entry
  br i1 %EQ, label %if12.then, label %else13

if12.then:                                        ; preds = %if12
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else13:                                           ; preds = %if12
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else13, %if12.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf36 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.4, i32 0, i32 0), i32 %"$c35", i32 6, i8* %"$bool.text")
  store i32 %call.snprintf36, i32* %format.size34, align 4
  %"$format.size37" = load i32, i32* %format.size34, align 4
  %ADD38 = add i32 %"$format.size37", 1
  %AS39 = sext i32 %ADD38 to i64
  %call.calloc41 = call i8* @calloc(i64 %AS39, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc41, i8** %format.text40, align 8
  %"$format.text42" = load i8*, i8** %format.text40, align 8
  %"$bool.text43" = load i8*, i8** %bool.text, align 8
  %call.snprintf44 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text42", i64 %AS39, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.4, i32 0, i32 0), i32 %"$c35", i32 6, i8* %"$bool.text43")
  %"$format.size45" = load i32, i32* %format.size34, align 4
  %ADD46 = add i32 %"$format.size45", 1
  %"$format.text47" = load i8*, i8** %format.text40, align 8
  %"$format.size48" = load i32, i32* %format.size34, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text47", 0
  %7 = insertvalue %String %6, i32 %"$format.size48", 1
  %8 = insertvalue %String %7, i32 %ADD46, 2
  call void @"io::out"(%String %8)
  store i32* %a, i32** %r, align 8
  %"*r" = load i32*, i32** %r, align 8
  store i32 9, i32* %"*r", align 4
  store %"Array<Array<i32>>" zeroinitializer, %"Array<Array<i32>>"* %grid, align 8
  %"call.Array<Array<i32>>.size" = call i32 @"Array<Array<i32>>.size"(%"Array<Array<i32>>"* %grid)
  %ADD49 = add i32 %"call.Array<Array<i32>>.size", 1
  %"$a51" = load i32, i32* %a, align 4
  %call.snprintf52 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.5, i32 0, i32 0), i32 %"$a51", i32 %ADD49)
  store i32 %call.snprintf52, i32* %format.size50, align 4
  %"$format.size53" = load i32, i32* %format.size50, align 4
  %ADD54 = add i32 %"$format.size53", 1
  %AS55 = sext i32 %ADD54 to i64
  %call.calloc57 = call i8* @calloc(i64 %AS55, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc57, i8** %format.text56, align 8
  %"$format.text58" = load i8*, i8** %format.text56, align 8
  %call.snprintf59 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text58", i64 %AS55, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.5, i32 0, i32 0), i32 %"$a51", i32 %ADD49)
  %"$format.size60" = load i32, i32* %format.size50, align 4
  %ADD61 = add i32 %"$format.size60", 1
  %"$format.text62" = load i8*, i8** %format.text56, align 8
  %"$format.size63" = load i32, i32* %format.size50, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text62", 0
  %10 = insertvalue %String %9, i32 %"$format.size63", 1
  %11 = insertvalue %String %10, i32 %ADD61, 2
  call void @"io::out"(%String %11)
  call void @"Array<Array<i32>>.drop"(%"Array<Array<i32>>"* %grid)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

---

## 132 — array: any integer type is an index, an unsigned one is zero-extended

```ura
// stdout: 30 20 10 40
// expect: 0
fn main() i32:
    a i32[] = [10, 20, 30, 40]
    i u64 = 2
    j u8 = 1
    k i64 = 0
    big u8 = 3
    io::out($"{a[i]} {a[j]} {a[k]} {a[big]}\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [13 x i8] c"%d %d %d %d\0A\00", align 1

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

define i32 @main() {
entry:
  %a = alloca i32*, align 8
  %arr = alloca [4 x i32], align 4
  %i = alloca i64, align 8
  %j = alloca i8, align 1
  %k = alloca i64, align 8
  %big = alloca i8, align 1
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store [4 x i32] [i32 10, i32 20, i32 30, i32 40], [4 x i32]* %arr, align 4
  %0 = getelementptr inbounds [4 x i32], [4 x i32]* %arr, i32 0, i32 0
  store i32* %0, i32** %a, align 8
  store i64 2, i64* %i, align 8
  store i8 1, i8* %j, align 1
  store i64 0, i64* %k, align 8
  store i8 3, i8* %big, align 1
  %"$a" = load i32*, i32** %a, align 8
  %"$i" = load i64, i64* %i, align 8
  %"a[]" = getelementptr inbounds i32, i32* %"$a", i64 %"$i"
  %"$a1" = load i32*, i32** %a, align 8
  %"$j" = load i8, i8* %j, align 1
  %AS = zext i8 %"$j" to i64
  %"a[]2" = getelementptr inbounds i32, i32* %"$a1", i64 %AS
  %"$a3" = load i32*, i32** %a, align 8
  %"$k" = load i64, i64* %k, align 8
  %"a[]4" = getelementptr inbounds i32, i32* %"$a3", i64 %"$k"
  %"$a5" = load i32*, i32** %a, align 8
  %"$big" = load i8, i8* %big, align 1
  %AS6 = zext i8 %"$big" to i64
  %"a[]7" = getelementptr inbounds i32, i32* %"$a5", i64 %AS6
  %"$a[]" = load i32, i32* %"a[]", align 4
  %"$a[]8" = load i32, i32* %"a[]2", align 4
  %"$a[]9" = load i32, i32* %"a[]4", align 4
  %"$a[]10" = load i32, i32* %"a[]7", align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str, i32 0, i32 0), i32 %"$a[]", i32 %"$a[]8", i32 %"$a[]9", i32 %"$a[]10")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS11 = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS11, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf12 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS11, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str, i32 0, i32 0), i32 %"$a[]", i32 %"$a[]8", i32 %"$a[]9", i32 %"$a[]10")
  %"$format.size13" = load i32, i32* %format.size, align 4
  %ADD14 = add i32 %"$format.size13", 1
  %"$format.text15" = load i8*, i8** %format.text, align 8
  %"$format.size16" = load i32, i32* %format.size, align 4
  %1 = insertvalue %String zeroinitializer, i8* %"$format.text15", 0
  %2 = insertvalue %String %1, i32 %"$format.size16", 1
  %3 = insertvalue %String %2, i32 %ADD14, 2
  call void @"io::out"(%String %3)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

