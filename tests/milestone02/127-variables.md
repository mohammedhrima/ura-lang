# milestone02

## index

- 127 — variable: an array type carries no size
- 128 — array: Array<T>: append, insert, pop, pop(index), first/last, resize, reverse
- 129 — use: a package that doesn't exist
- 130 — use: an error after a use points at the user's file and line
- 131 — operator: bitwise & | ^ << >> on scalars, compound forms, & stays a ref before a type, >> closes nested templates

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
    output(a[0], " ", a[1], " ", a[2], " ", a[3], " ", a[4], " size ", a.size(), "\n")
    x i32 = a.pop()
    y i32 = a.pop(0)
    z i32 = a.pop(1)
    a.last() = 9
    output(x, " ", y, " ", z, " ", a.first(), " ", a.last(), " ", a.len(), " ", a.is_empty(), "\n")
    a.resize(4)
    a.reverse()
    output(a[0], " ", a[1], " ", a[2], " ", a[3], " cap ", a.capacity(), "\n")
    a.clear()
    output(a.size(), " ", a.is_empty(), "\n")
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

%"Array<i8>" = type { i8*, i32, i32 }
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
  br label %if8

if8:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if8.then, label %endif

if8.then:                                         ; preds = %if8
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if8.then, %if8
  br label %if9

if9:                                              ; preds = %endif
  %"*self6" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if9.then, label %endif1

if9.then:                                         ; preds = %if9
  ret void

endif1:                                           ; preds = %if9
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
  br label %if10

if10:                                             ; preds = %endif1
  %"*self13" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap14" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if10.then, label %endif2

if10.then:                                        ; preds = %if10
  %"*self16" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data17" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i32*, i32** %"*self.data17", align 8
  call void @"free<i32>"(i32* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if10.then, %if10
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
  br label %if11

if11:                                             ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %AND = and i1 %EQ, %GT
  br i1 %AND, label %if11.then, label %endif

if11.then:                                        ; preds = %if11
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self2", i32 %"$*self.count5")
  br label %endif

endif:                                            ; preds = %if11.then, %if11
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
  br label %while12

while12:                                          ; preds = %while12.then, %entry
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$size2" = load i32, i32* %size, align 4
  %GT = icmp sgt i32 %"$*self.count", %"$size2"
  br i1 %GT, label %while12.then, label %endloop

while12.then:                                     ; preds = %while12
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
  br label %while12

endloop:                                          ; preds = %while12
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
  br label %if13

if13:                                             ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if13.then, label %endif

if13.then:                                        ; preds = %if13
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if13.then, %if13
  br label %while14

while14:                                          ; preds = %while14.then, %endif
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while14.then, label %endloop

while14.then:                                     ; preds = %while14
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
  br label %while14

endloop:                                          ; preds = %while14
  ret void
}

define void @"Array<i32>.release"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  call void @"Array<i32>.clear"(%"Array<i32>"* %"*self")
  br label %if15

if15:                                             ; preds = %entry
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if15.then, label %endif

if15.then:                                        ; preds = %if15
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  call void @"free<i32>"(i32* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if15.then, %if15
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
  br label %if16

if16:                                             ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if16.then, label %endif

if16.then:                                        ; preds = %if16
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if16.then, %if16
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
  br label %if17

if17:                                             ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if17.then, label %endif

if17.then:                                        ; preds = %if17
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self3" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<i32>.reserve"(%"Array<i32>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if17.then, %if17
  %"*self6" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count7" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self6", i32 0, i32 1
  %"$*self.count8" = load i32, i32* %"*self.count7", align 4
  store i32 %"$*self.count8", i32* %i, align 4
  br label %while18

while18:                                          ; preds = %while18.then, %endif
  %"$i" = load i32, i32* %i, align 4
  %"$index" = load i32, i32* %index, align 4
  %GT = icmp sgt i32 %"$i", %"$index"
  br i1 %GT, label %while18.then, label %endloop

while18.then:                                     ; preds = %while18
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
  br label %while18

endloop:                                          ; preds = %while18
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
  br label %if19

if19:                                             ; preds = %entry
  %"$index" = load i32, i32* %index, align 4
  %LT = icmp slt i32 %"$index", 0
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 1
  %"$index1" = load i32, i32* %index, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GE = icmp sge i32 %"$index1", %"$*self.count"
  %OR = or i1 %LT, %GE
  br i1 %OR, label %if19.then, label %endif

if19.then:                                        ; preds = %if19
  %"$value" = load i32, i32* %value, align 4
  ret i32 %"$value"

endif:                                            ; preds = %if19
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
  br label %while20

while20:                                          ; preds = %while20.then, %endif
  %"*self6" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count7" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self6", i32 0, i32 1
  %"$*self.count8" = load i32, i32* %"*self.count7", align 4
  %SUB = sub i32 %"$*self.count8", 1
  %"$i" = load i32, i32* %i, align 4
  %LT9 = icmp slt i32 %"$i", %SUB
  br i1 %LT9, label %while20.then, label %endloop

while20.then:                                     ; preds = %while20
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
  br label %while20

endloop:                                          ; preds = %while20
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
  br label %while21

while21:                                          ; preds = %while21.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %"$j" = load i32, i32* %j, align 4
  %LT = icmp slt i32 %"$i", %"$j"
  br i1 %LT, label %while21.then, label %endloop

while21.then:                                     ; preds = %while21
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
  br label %while21

endloop:                                          ; preds = %while21
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
  br label %if22

if22:                                             ; preds = %entry
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if22.then, label %endif

if22.then:                                        ; preds = %if22
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  call void @"free<i32>"(i32* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if22.then, %if22
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
  %ref23 = alloca i32*, align 8
  %ref25 = alloca i32*, align 8
  %ref28 = alloca i32*, align 8
  %bool.text = alloca i8*, align 8
  %ref34 = alloca i32*, align 8
  %ref37 = alloca i32*, align 8
  %ref40 = alloca i32*, align 8
  %ref43 = alloca i32*, align 8
  %bool.text52 = alloca i8*, align 8
  %c = alloca %"Array<i8>", align 8
  %ref55 = alloca i8*, align 8
  %ref58 = alloca i8*, align 8
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
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([24 x i8], [24 x i8]* @str, i32 0, i32 0), i32 %"$*ref", i32 %"$*ref17", i32 %"$*ref18", i32 %"$*ref19", i32 %"$*ref20", i32 %"call.Array<i32>.size")
  %"call.Array<i32>.pop" = call i32 @"Array<i32>.pop.1"(%"Array<i32>"* %a)
  store i32 %"call.Array<i32>.pop", i32* %x, align 4
  %"call.Array<i32>.pop21" = call i32 @"Array<i32>.pop"(%"Array<i32>"* %a, i32 0)
  store i32 %"call.Array<i32>.pop21", i32* %y, align 4
  %"call.Array<i32>.pop22" = call i32 @"Array<i32>.pop"(%"Array<i32>"* %a, i32 1)
  store i32 %"call.Array<i32>.pop22", i32* %z, align 4
  %"call.Array<i32>.last" = call i32* @"Array<i32>.last"(%"Array<i32>"* %a)
  store i32* %"call.Array<i32>.last", i32** %ref23, align 8
  %"*ref24" = load i32*, i32** %ref23, align 8
  store i32 9, i32* %"*ref24", align 4
  %"call.Array<i32>.first" = call i32* @"Array<i32>.first"(%"Array<i32>"* %a)
  store i32* %"call.Array<i32>.first", i32** %ref25, align 8
  %"*ref26" = load i32*, i32** %ref25, align 8
  %"call.Array<i32>.last27" = call i32* @"Array<i32>.last"(%"Array<i32>"* %a)
  store i32* %"call.Array<i32>.last27", i32** %ref28, align 8
  %"*ref29" = load i32*, i32** %ref28, align 8
  %"call.Array<i32>.len" = call i32 @"Array<i32>.len"(%"Array<i32>"* %a)
  %"call.Array<i32>.is_empty" = call i1 @"Array<i32>.is_empty"(%"Array<i32>"* %a)
  %"$x" = load i32, i32* %x, align 4
  %"$y" = load i32, i32* %y, align 4
  %"$z" = load i32, i32* %z, align 4
  %"$*ref30" = load i32, i32* %"*ref26", align 4
  %"$*ref31" = load i32, i32* %"*ref29", align 4
  store i8* null, i8** %bool.text, align 8
  br label %if23

if23:                                             ; preds = %entry
  br i1 %"call.Array<i32>.is_empty", label %if23.then, label %else24

if23.then:                                        ; preds = %if23
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else24:                                           ; preds = %if23
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else24, %if23.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf32 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @str.4, i32 0, i32 0), i32 %"$x", i32 %"$y", i32 %"$z", i32 %"$*ref30", i32 %"$*ref31", i32 %"call.Array<i32>.len", i8* %"$bool.text")
  call void @"Array<i32>.resize"(%"Array<i32>"* %a, i32 4)
  call void @"Array<i32>.reverse"(%"Array<i32>"* %a)
  %"call.Array<i32>.[]33" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %a, i32 0)
  store i32* %"call.Array<i32>.[]33", i32** %ref34, align 8
  %"*ref35" = load i32*, i32** %ref34, align 8
  %"call.Array<i32>.[]36" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %a, i32 1)
  store i32* %"call.Array<i32>.[]36", i32** %ref37, align 8
  %"*ref38" = load i32*, i32** %ref37, align 8
  %"call.Array<i32>.[]39" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %a, i32 2)
  store i32* %"call.Array<i32>.[]39", i32** %ref40, align 8
  %"*ref41" = load i32*, i32** %ref40, align 8
  %"call.Array<i32>.[]42" = call i32* @"Array<i32>.[]"(%"Array<i32>"* %a, i32 3)
  store i32* %"call.Array<i32>.[]42", i32** %ref43, align 8
  %"*ref44" = load i32*, i32** %ref43, align 8
  %"call.Array<i32>.capacity" = call i32 @"Array<i32>.capacity"(%"Array<i32>"* %a)
  %"$*ref45" = load i32, i32* %"*ref35", align 4
  %"$*ref46" = load i32, i32* %"*ref38", align 4
  %"$*ref47" = load i32, i32* %"*ref41", align 4
  %"$*ref48" = load i32, i32* %"*ref44", align 4
  %call.dprintf49 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([20 x i8], [20 x i8]* @str.5, i32 0, i32 0), i32 %"$*ref45", i32 %"$*ref46", i32 %"$*ref47", i32 %"$*ref48", i32 %"call.Array<i32>.capacity")
  call void @"Array<i32>.clear"(%"Array<i32>"* %a)
  %"call.Array<i32>.size50" = call i32 @"Array<i32>.size"(%"Array<i32>"* %a)
  %"call.Array<i32>.is_empty51" = call i1 @"Array<i32>.is_empty"(%"Array<i32>"* %a)
  store i8* null, i8** %bool.text52, align 8
  br label %if25

if25:                                             ; preds = %endif
  br i1 %"call.Array<i32>.is_empty51", label %if25.then, label %else26

if25.then:                                        ; preds = %if25
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.6, i32 0, i32 0), i8** %bool.text52, align 8
  br label %endif1

else26:                                           ; preds = %if25
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.7, i32 0, i32 0), i8** %bool.text52, align 8
  br label %endif1

endif1:                                           ; preds = %else26, %if25.then
  %"$bool.text53" = load i8*, i8** %bool.text52, align 8
  %call.dprintf54 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.8, i32 0, i32 0), i32 %"call.Array<i32>.size50", i8* %"$bool.text53")
  call void @"Array<i32>.release"(%"Array<i32>"* %a)
  store %"Array<i8>" zeroinitializer, %"Array<i8>"* %c, align 8
  call void @"Array<i8>.append"(%"Array<i8>"* %c, i8 104)
  call void @"Array<i8>.append"(%"Array<i8>"* %c, i8 105)
  %"call.Array<i8>.[]" = call i8* @"Array<i8>.[]"(%"Array<i8>"* %c, i32 0)
  store i8* %"call.Array<i8>.[]", i8** %ref55, align 8
  %"*ref56" = load i8*, i8** %ref55, align 8
  %"call.Array<i8>.[]57" = call i8* @"Array<i8>.[]"(%"Array<i8>"* %c, i32 1)
  store i8* %"call.Array<i8>.[]57", i8** %ref58, align 8
  %"*ref59" = load i8*, i8** %ref58, align 8
  %"$*ref60" = load i8, i8* %"*ref56", align 1
  %AS = sext i8 %"$*ref60" to i32
  %"$*ref61" = load i8, i8* %"*ref59", align 1
  %AS62 = sext i8 %"$*ref61" to i32
  %call.dprintf63 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.9, i32 0, i32 0), i32 %AS, i32 %AS62, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @TYPEOF, i32 0, i32 0))
  call void @"Array<i8>.drop"(%"Array<i8>"* %c)
  call void @"Array<i32>.drop"(%"Array<i32>"* %a)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

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
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %while1

while1:                                           ; preds = %while1.then, %endif
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
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
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define void @"Array<i32>.drop"(%"Array<i32>"* %0) {
entry:
  %self = alloca %"Array<i32>"*, align 8
  store %"Array<i32>"* %0, %"Array<i32>"** %self, align 8
  %"*self" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  call void @"Array<i32>.clear"(%"Array<i32>"* %"*self")
  br label %if2

if2:                                              ; preds = %entry
  %"*self1" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*self2" = load %"Array<i32>"*, %"Array<i32>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<i32>", %"Array<i32>"* %"*self2", i32 0, i32 0
  %"$*self.data" = load i32*, i32** %"*self.data", align 8
  call void @"free<i32>"(i32* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if2.then, %if2
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
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self1" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %while4

while4:                                           ; preds = %while4.then, %endif
  %"*self2" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
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
  br label %while4

endloop:                                          ; preds = %while4
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
  br label %if5

if5:                                              ; preds = %entry
  %"*self1" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if5.then, label %endif

if5.then:                                         ; preds = %if5
  %"*self2" = load %"Array<Array<i32>>"*, %"Array<Array<i32>>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Array<i32>>", %"Array<Array<i32>>"* %"*self2", i32 0, i32 0
  %"$*self.data" = load %"Array<i32>"*, %"Array<i32>"** %"*self.data", align 8
  call void @"free<Array<i32>>"(%"Array<i32>"* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if5.then, %if5
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
  br label %if6

if6:                                              ; preds = %entry
  br i1 %EQ, label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.2, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else7, %if6.then
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

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

