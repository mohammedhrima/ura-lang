# milestone06

## index

- 019 — string: transforms: upper, lower, substr, trim, replace, repeat, reverse
- 020 — string: operators: + += == != < > against String and against a literal
- 021 — string: to_int, compare and chained transformations

---

## 019 — string: transforms: upper, lower, substr, trim, replace, repeat, reverse

```ura
// stdout: HELLO WORLD
// stdout: hello world
// stdout: World
// stdout: Hello
// stdout: Hello World
// stdout: 0
// stdout: hello
// stdout: hello
// stdout: 0
// stdout: qux bar qux baz
// stdout: foo bar foo baz
// stdout: foo bar foo baz
// stdout: ababab
// stdout: 0
// stdout: edcba
// stdout: a
// expect: 0
use "@/core"
use "@/posix"

fn main() i32:
    s String = String::create("Hello World")

    u String = s.upper()
    io::out($"{u}\n")

    l String = s.lower()
    io::out($"{l}\n")

    sub String = s.substr(6, 5)
    io::out($"{sub}\n")

    sub2 String = s.substr(0, 5)
    io::out($"{sub2}\n")

    sub3 String = s.substr(0, 100)
    io::out($"{sub3}\n")

    sub4 String = s.substr(-1, 5)
    io::out($"{sub4.size()}\n")

    t1 String = String::create("   hello   ")
    t2 String = t1.trim()
    io::out($"{t2}\n")

    t3 String = String::create("hello")
    t4 String = t3.trim()
    io::out($"{t4}\n")

    t5 String = String::create("   ")
    t6 String = t5.trim()
    io::out($"{t6.size()}\n")

    r1 String = String::create("foo bar foo baz")
    r2 String = r1.replace("foo", "qux")
    io::out($"{r2}\n")

    r3 String = r1.replace("xyz", "qux")
    io::out($"{r3}\n")

    r4 String = r1.replace("", "qux")
    io::out($"{r4}\n")

    rep String = String::create("ab")
    rep2 String = rep.repeat(3)
    io::out($"{rep2}\n")

    rep3 String = rep.repeat(0)
    io::out($"{rep3.size()}\n")

    rev String = String::create("abcde")
    rev2 String = rev.reverse()
    io::out($"{rev2}\n")

    rev3 String = String::create("a")
    rev4 String = rev3.reverse()
    io::out($"{rev4}\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [12 x i8] c"Hello World\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.3 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.4 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.5 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.6 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.7 = private unnamed_addr constant [12 x i8] c"   hello   \00", align 1
@str.8 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.9 = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@str.10 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.11 = private unnamed_addr constant [4 x i8] c"   \00", align 1
@str.12 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.13 = private unnamed_addr constant [16 x i8] c"foo bar foo baz\00", align 1
@str.14 = private unnamed_addr constant [4 x i8] c"foo\00", align 1
@str.15 = private unnamed_addr constant [4 x i8] c"qux\00", align 1
@str.16 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.17 = private unnamed_addr constant [4 x i8] c"xyz\00", align 1
@str.18 = private unnamed_addr constant [4 x i8] c"qux\00", align 1
@str.19 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.20 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.21 = private unnamed_addr constant [4 x i8] c"qux\00", align 1
@str.22 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.23 = private unnamed_addr constant [3 x i8] c"ab\00", align 1
@str.24 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.25 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.26 = private unnamed_addr constant [6 x i8] c"abcde\00", align 1
@str.27 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.28 = private unnamed_addr constant [2 x i8] c"a\00", align 1
@str.29 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

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

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %while1

while1:                                           ; preds = %while1.then, %endif
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
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
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %value = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %value, align 8
  %"*value" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*value")
  br label %if2

if2:                                              ; preds = %entry
  %"*value1" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*value2" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i8*, i8** %"*value.data", align 8
  call void @"free<i8>"(i8* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %if4

if4:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if4.then, label %endif1

if4.then:                                         ; preds = %if4
  ret void

endif1:                                           ; preds = %if4
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
  br label %if5

if5:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if5.then, label %endif2

if5.then:                                         ; preds = %if5
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if5.then, %if5
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

define void @String.append(%String* %0, i8 %1) {
entry:
  %self = alloca %String*, align 8
  %value = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store i8 %1, i8* %value, align 1
  br label %if6

if6:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*self2" = load %String*, %String** %self, align 8
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @String.reserve(%String* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if6.then, %if6
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
  br label %while7

while7:                                           ; preds = %while7.then, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$n" = load i32, i32* %n, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$n"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while7.then, label %endloop

while7.then:                                      ; preds = %while7
  %"$n1" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n1", 1
  store i32 %ADD, i32* %n, align 4
  br label %while7

endloop:                                          ; preds = %while7
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
  br label %while8

while8:                                           ; preds = %while8.then, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$i" = load i32, i32* %i, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$i"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while8.then, label %endloop

while8.then:                                      ; preds = %while8
  %"*self" = load %String*, %String** %self, align 8
  %"$text1" = load i8*, i8** %text, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"text[]3" = getelementptr inbounds i8, i8* %"$text1", i32 %"$i2"
  %"$text[]4" = load i8, i8* %"text[]3", align 1
  call void @String.append(%String* %"*self", i8 %"$text[]4")
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while8

endloop:                                          ; preds = %while8
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
  br label %while9

while9:                                           ; preds = %endif1, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$j" = load i32, i32* %j, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$j"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while9.then, label %endloop

while9.then:                                      ; preds = %while9
  br label %if10

if10:                                             ; preds = %while9.then
  %"$index" = load i32, i32* %index, align 4
  %"$j2" = load i32, i32* %j, align 4
  %ADD = add i32 %"$index", %"$j2"
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GE = icmp sge i32 %ADD, %"$*self.count"
  br i1 %GE, label %if10.then, label %endif

if10.then:                                        ; preds = %if10
  ret i1 false

endif:                                            ; preds = %if10
  br label %if11

if11:                                             ; preds = %endif
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
  br i1 %NQ11, label %if11.then, label %endif1

if11.then:                                        ; preds = %if11
  ret i1 false

endif1:                                           ; preds = %if11
  %"$j12" = load i32, i32* %j, align 4
  %ADD13 = add i32 %"$j12", 1
  store i32 %ADD13, i32* %j, align 4
  br label %while9

endloop:                                          ; preds = %while9
  ret i1 true
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
  br label %if12

if12:                                             ; preds = %entry
  %"$start" = load i32, i32* %start, align 4
  %LT = icmp slt i32 %"$start", 0
  br i1 %LT, label %if12.then, label %endif

if12.then:                                        ; preds = %if12
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"

endif:                                            ; preds = %if12
  %"$start2" = load i32, i32* %start, align 4
  %"$length" = load i32, i32* %length, align 4
  %ADD = add i32 %"$start2", %"$length"
  store i32 %ADD, i32* %stop, align 4
  br label %if13

if13:                                             ; preds = %endif
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$stop" = load i32, i32* %stop, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$stop", %"$*self.count"
  br i1 %GT, label %if13.then, label %endif1

if13.then:                                        ; preds = %if13
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %stop, align 4
  br label %endif1

endif1:                                           ; preds = %if13.then, %if13
  %"$start6" = load i32, i32* %start, align 4
  store i32 %"$start6", i32* %i, align 4
  br label %while14

while14:                                          ; preds = %while14.then, %endif1
  %"$i" = load i32, i32* %i, align 4
  %"$stop7" = load i32, i32* %stop, align 4
  %LT8 = icmp slt i32 %"$i", %"$stop7"
  br i1 %LT8, label %while14.then, label %endloop

while14.then:                                     ; preds = %while14
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
  br label %while14

endloop:                                          ; preds = %while14
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
  br label %while15

while15:                                          ; preds = %endif, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count"
  br i1 %LT, label %while15.then, label %endloop

while15.then:                                     ; preds = %while15
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i2" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i2"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  store i8 %"$*self.data[]", i8* %c, align 1
  br label %if16

if16:                                             ; preds = %while15.then
  %"$c" = load i8, i8* %c, align 1
  %GE = icmp sge i8 %"$c", 97
  %"$c3" = load i8, i8* %c, align 1
  %LE = icmp sle i8 %"$c3", 122
  %AND = and i1 %GE, %LE
  br i1 %AND, label %if16.then, label %endif

if16.then:                                        ; preds = %if16
  %"$c4" = load i8, i8* %c, align 1
  %AS = sext i8 %"$c4" to i32
  %SUB = sub i32 %AS, 32
  %AS5 = trunc i32 %SUB to i8
  store i8 %AS5, i8* %c, align 1
  br label %endif

endif:                                            ; preds = %if16.then, %if16
  %"$c6" = load i8, i8* %c, align 1
  call void @String.append(%String* %res, i8 %"$c6")
  %"$i7" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i7", 1
  store i32 %ADD, i32* %i, align 4
  br label %while15

endloop:                                          ; preds = %while15
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
  br label %while17

while17:                                          ; preds = %endif, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count"
  br i1 %LT, label %while17.then, label %endloop

while17.then:                                     ; preds = %while17
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i2" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i2"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  store i8 %"$*self.data[]", i8* %c, align 1
  br label %if18

if18:                                             ; preds = %while17.then
  %"$c" = load i8, i8* %c, align 1
  %GE = icmp sge i8 %"$c", 65
  %"$c3" = load i8, i8* %c, align 1
  %LE = icmp sle i8 %"$c3", 90
  %AND = and i1 %GE, %LE
  br i1 %AND, label %if18.then, label %endif

if18.then:                                        ; preds = %if18
  %"$c4" = load i8, i8* %c, align 1
  %AS = sext i8 %"$c4" to i32
  %ADD = add i32 %AS, 32
  %AS5 = trunc i32 %ADD to i8
  store i8 %AS5, i8* %c, align 1
  br label %endif

endif:                                            ; preds = %if18.then, %if18
  %"$c6" = load i8, i8* %c, align 1
  call void @String.append(%String* %res, i8 %"$c6")
  %"$i7" = load i32, i32* %i, align 4
  %ADD8 = add i32 %"$i7", 1
  store i32 %ADD8, i32* %i, align 4
  br label %while17

endloop:                                          ; preds = %while17
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
  br label %while19

while19:                                          ; preds = %endif, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$start" = load i32, i32* %start, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$start", %"$*self.count"
  br i1 %LT, label %while19.then, label %endloop

while19.then:                                     ; preds = %while19
  br label %if20

if20:                                             ; preds = %while19.then
  %"*self3" = load %String*, %String** %self, align 8
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$start5" = load i32, i32* %start, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$start5"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  %call.String.is_space = call i1 @String.is_space(%String* %"*self3", i8 %"$*self.data[]")
  %NOT = xor i1 %call.String.is_space, true
  br i1 %NOT, label %if20.then, label %endif

if20.then:                                        ; preds = %if20
  br label %endloop

endif:                                            ; preds = %if20
  %"$start6" = load i32, i32* %start, align 4
  %ADD = add i32 %"$start6", 1
  store i32 %ADD, i32* %start, align 4
  br label %while19

endloop:                                          ; preds = %if20.then, %while19
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.count8" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 1
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  store i32 %"$*self.count9", i32* %stop, align 4
  br label %while21

while21:                                          ; preds = %endif1, %endloop
  %"$stop" = load i32, i32* %stop, align 4
  %"$start10" = load i32, i32* %start, align 4
  %GT = icmp sgt i32 %"$stop", %"$start10"
  br i1 %GT, label %while21.then, label %endloop2

while21.then:                                     ; preds = %while21
  br label %if22

if22:                                             ; preds = %while21.then
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
  br i1 %NOT19, label %if22.then, label %endif1

if22.then:                                        ; preds = %if22
  br label %endloop2

endif1:                                           ; preds = %if22
  %"$stop20" = load i32, i32* %stop, align 4
  %SUB21 = sub i32 %"$stop20", 1
  store i32 %SUB21, i32* %stop, align 4
  br label %while21

endloop2:                                         ; preds = %if22.then, %while21
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
  br label %while23

while23:                                          ; preds = %endif, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count"
  br i1 %LT, label %while23.then, label %endloop

while23.then:                                     ; preds = %while23
  br label %if24

if24:                                             ; preds = %while23.then
  %"$n" = load i32, i32* %n, align 4
  %GT = icmp sgt i32 %"$n", 0
  %"*self1" = load %String*, %String** %self, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"$old3" = load i8*, i8** %old, align 8
  %call.String.matches_at = call i1 @String.matches_at(%String* %"*self1", i32 %"$i2", i8* %"$old3")
  %AND = and i1 %GT, %call.String.matches_at
  br i1 %AND, label %if24.then, label %else25

if24.then:                                        ; preds = %if24
  %"$fresh" = load i8*, i8** %fresh, align 8
  call void @String.join(%String* %res, i8* %"$fresh")
  %"$n4" = load i32, i32* %n, align 4
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", %"$n4"
  store i32 %ADD, i32* %i, align 4
  br label %endif

else25:                                           ; preds = %if24
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

endif:                                            ; preds = %else25, %if24.then
  br label %while23

endloop:                                          ; preds = %while23
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
  br label %while26

while26:                                          ; preds = %endloop, %entry
  %"$k" = load i32, i32* %k, align 4
  %"$times" = load i32, i32* %times, align 4
  %LT = icmp slt i32 %"$k", %"$times"
  br i1 %LT, label %while26.then, label %endloop1

while26.then:                                     ; preds = %while26
  store i32 0, i32* %i, align 4
  br label %while27

while27:                                          ; preds = %while27.then, %while26.then
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT2 = icmp slt i32 %"$i", %"$*self.count"
  br i1 %LT2, label %while27.then, label %endloop

while27.then:                                     ; preds = %while27
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
  br label %while27

endloop:                                          ; preds = %while27
  %"$k6" = load i32, i32* %k, align 4
  %ADD7 = add i32 %"$k6", 1
  store i32 %ADD7, i32* %k, align 4
  br label %while26

endloop1:                                         ; preds = %while26
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
  br label %while28

while28:                                          ; preds = %while28.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %GE = icmp sge i32 %"$i", 0
  br i1 %GE, label %while28.then, label %endloop

while28.then:                                     ; preds = %while28
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
  br label %while28

endloop:                                          ; preds = %while28
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
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i32 @main() {
entry:
  %s = alloca %String, align 8
  %u = alloca %String, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %l = alloca %String, align 8
  %format.size7 = alloca i32, align 4
  %format.text12 = alloca i8*, align 8
  %sub = alloca %String, align 8
  %format.size21 = alloca i32, align 4
  %format.text26 = alloca i8*, align 8
  %sub2 = alloca %String, align 8
  %format.size36 = alloca i32, align 4
  %format.text41 = alloca i8*, align 8
  %sub3 = alloca %String, align 8
  %format.size51 = alloca i32, align 4
  %format.text56 = alloca i8*, align 8
  %sub4 = alloca %String, align 8
  %format.size65 = alloca i32, align 4
  %format.text70 = alloca i8*, align 8
  %t1 = alloca %String, align 8
  %t2 = alloca %String, align 8
  %format.size80 = alloca i32, align 4
  %format.text85 = alloca i8*, align 8
  %t3 = alloca %String, align 8
  %t4 = alloca %String, align 8
  %format.size96 = alloca i32, align 4
  %format.text101 = alloca i8*, align 8
  %t5 = alloca %String, align 8
  %t6 = alloca %String, align 8
  %format.size112 = alloca i32, align 4
  %format.text117 = alloca i8*, align 8
  %r1 = alloca %String, align 8
  %r2 = alloca %String, align 8
  %format.size127 = alloca i32, align 4
  %format.text132 = alloca i8*, align 8
  %r3 = alloca %String, align 8
  %format.size142 = alloca i32, align 4
  %format.text147 = alloca i8*, align 8
  %r4 = alloca %String, align 8
  %format.size157 = alloca i32, align 4
  %format.text162 = alloca i8*, align 8
  %rep = alloca %String, align 8
  %rep2 = alloca %String, align 8
  %format.size172 = alloca i32, align 4
  %format.text177 = alloca i8*, align 8
  %rep3 = alloca %String, align 8
  %format.size187 = alloca i32, align 4
  %format.text192 = alloca i8*, align 8
  %rev = alloca %String, align 8
  %rev2 = alloca %String, align 8
  %format.size202 = alloca i32, align 4
  %format.text207 = alloca i8*, align 8
  %rev3 = alloca %String, align 8
  %rev4 = alloca %String, align 8
  %format.size218 = alloca i32, align 4
  %format.text223 = alloca i8*, align 8
  %call.String.create = call %String @String.create(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0))
  store %String %call.String.create, %String* %s, align 8
  %call.String.upper = call %String @String.upper(%String* %s)
  store %String %call.String.upper, %String* %u, align 8
  %call.String.chars = call i8* @String.chars(%String* %u)
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i8* %call.String.chars)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i8* %call.String.chars)
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  call void @"io::out"(%String %2)
  %call.String.lower = call %String @String.lower(%String* %s)
  store %String %call.String.lower, %String* %l, align 8
  %call.String.chars6 = call i8* @String.chars(%String* %l)
  %call.snprintf8 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i8* %call.String.chars6)
  store i32 %call.snprintf8, i32* %format.size7, align 4
  %"$format.size9" = load i32, i32* %format.size7, align 4
  %ADD10 = add i32 %"$format.size9", 1
  %AS11 = sext i32 %ADD10 to i64
  %call.calloc13 = call i8* @calloc(i64 %AS11, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc13, i8** %format.text12, align 8
  %"$format.text14" = load i8*, i8** %format.text12, align 8
  %call.snprintf15 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text14", i64 %AS11, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i8* %call.String.chars6)
  %"$format.size16" = load i32, i32* %format.size7, align 4
  %ADD17 = add i32 %"$format.size16", 1
  %"$format.text18" = load i8*, i8** %format.text12, align 8
  %"$format.size19" = load i32, i32* %format.size7, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text18", 0
  %4 = insertvalue %String %3, i32 %"$format.size19", 1
  %5 = insertvalue %String %4, i32 %ADD17, 2
  call void @"io::out"(%String %5)
  %call.String.substr = call %String @String.substr(%String* %s, i32 6, i32 5)
  store %String %call.String.substr, %String* %sub, align 8
  %call.String.chars20 = call i8* @String.chars(%String* %sub)
  %call.snprintf22 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.3, i32 0, i32 0), i8* %call.String.chars20)
  store i32 %call.snprintf22, i32* %format.size21, align 4
  %"$format.size23" = load i32, i32* %format.size21, align 4
  %ADD24 = add i32 %"$format.size23", 1
  %AS25 = sext i32 %ADD24 to i64
  %call.calloc27 = call i8* @calloc(i64 %AS25, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc27, i8** %format.text26, align 8
  %"$format.text28" = load i8*, i8** %format.text26, align 8
  %call.snprintf29 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text28", i64 %AS25, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.3, i32 0, i32 0), i8* %call.String.chars20)
  %"$format.size30" = load i32, i32* %format.size21, align 4
  %ADD31 = add i32 %"$format.size30", 1
  %"$format.text32" = load i8*, i8** %format.text26, align 8
  %"$format.size33" = load i32, i32* %format.size21, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text32", 0
  %7 = insertvalue %String %6, i32 %"$format.size33", 1
  %8 = insertvalue %String %7, i32 %ADD31, 2
  call void @"io::out"(%String %8)
  %call.String.substr34 = call %String @String.substr(%String* %s, i32 0, i32 5)
  store %String %call.String.substr34, %String* %sub2, align 8
  %call.String.chars35 = call i8* @String.chars(%String* %sub2)
  %call.snprintf37 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i8* %call.String.chars35)
  store i32 %call.snprintf37, i32* %format.size36, align 4
  %"$format.size38" = load i32, i32* %format.size36, align 4
  %ADD39 = add i32 %"$format.size38", 1
  %AS40 = sext i32 %ADD39 to i64
  %call.calloc42 = call i8* @calloc(i64 %AS40, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc42, i8** %format.text41, align 8
  %"$format.text43" = load i8*, i8** %format.text41, align 8
  %call.snprintf44 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text43", i64 %AS40, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i8* %call.String.chars35)
  %"$format.size45" = load i32, i32* %format.size36, align 4
  %ADD46 = add i32 %"$format.size45", 1
  %"$format.text47" = load i8*, i8** %format.text41, align 8
  %"$format.size48" = load i32, i32* %format.size36, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text47", 0
  %10 = insertvalue %String %9, i32 %"$format.size48", 1
  %11 = insertvalue %String %10, i32 %ADD46, 2
  call void @"io::out"(%String %11)
  %call.String.substr49 = call %String @String.substr(%String* %s, i32 0, i32 100)
  store %String %call.String.substr49, %String* %sub3, align 8
  %call.String.chars50 = call i8* @String.chars(%String* %sub3)
  %call.snprintf52 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.5, i32 0, i32 0), i8* %call.String.chars50)
  store i32 %call.snprintf52, i32* %format.size51, align 4
  %"$format.size53" = load i32, i32* %format.size51, align 4
  %ADD54 = add i32 %"$format.size53", 1
  %AS55 = sext i32 %ADD54 to i64
  %call.calloc57 = call i8* @calloc(i64 %AS55, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc57, i8** %format.text56, align 8
  %"$format.text58" = load i8*, i8** %format.text56, align 8
  %call.snprintf59 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text58", i64 %AS55, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.5, i32 0, i32 0), i8* %call.String.chars50)
  %"$format.size60" = load i32, i32* %format.size51, align 4
  %ADD61 = add i32 %"$format.size60", 1
  %"$format.text62" = load i8*, i8** %format.text56, align 8
  %"$format.size63" = load i32, i32* %format.size51, align 4
  %12 = insertvalue %String zeroinitializer, i8* %"$format.text62", 0
  %13 = insertvalue %String %12, i32 %"$format.size63", 1
  %14 = insertvalue %String %13, i32 %ADD61, 2
  call void @"io::out"(%String %14)
  %call.String.substr64 = call %String @String.substr(%String* %s, i32 -1, i32 5)
  store %String %call.String.substr64, %String* %sub4, align 8
  %call.String.size = call i32 @String.size(%String* %sub4)
  %call.snprintf66 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.6, i32 0, i32 0), i32 %call.String.size)
  store i32 %call.snprintf66, i32* %format.size65, align 4
  %"$format.size67" = load i32, i32* %format.size65, align 4
  %ADD68 = add i32 %"$format.size67", 1
  %AS69 = sext i32 %ADD68 to i64
  %call.calloc71 = call i8* @calloc(i64 %AS69, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc71, i8** %format.text70, align 8
  %"$format.text72" = load i8*, i8** %format.text70, align 8
  %call.snprintf73 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text72", i64 %AS69, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.6, i32 0, i32 0), i32 %call.String.size)
  %"$format.size74" = load i32, i32* %format.size65, align 4
  %ADD75 = add i32 %"$format.size74", 1
  %"$format.text76" = load i8*, i8** %format.text70, align 8
  %"$format.size77" = load i32, i32* %format.size65, align 4
  %15 = insertvalue %String zeroinitializer, i8* %"$format.text76", 0
  %16 = insertvalue %String %15, i32 %"$format.size77", 1
  %17 = insertvalue %String %16, i32 %ADD75, 2
  call void @"io::out"(%String %17)
  %call.String.create78 = call %String @String.create(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.7, i32 0, i32 0))
  store %String %call.String.create78, %String* %t1, align 8
  %call.String.trim = call %String @String.trim(%String* %t1)
  store %String %call.String.trim, %String* %t2, align 8
  %call.String.chars79 = call i8* @String.chars(%String* %t2)
  %call.snprintf81 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.8, i32 0, i32 0), i8* %call.String.chars79)
  store i32 %call.snprintf81, i32* %format.size80, align 4
  %"$format.size82" = load i32, i32* %format.size80, align 4
  %ADD83 = add i32 %"$format.size82", 1
  %AS84 = sext i32 %ADD83 to i64
  %call.calloc86 = call i8* @calloc(i64 %AS84, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc86, i8** %format.text85, align 8
  %"$format.text87" = load i8*, i8** %format.text85, align 8
  %call.snprintf88 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text87", i64 %AS84, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.8, i32 0, i32 0), i8* %call.String.chars79)
  %"$format.size89" = load i32, i32* %format.size80, align 4
  %ADD90 = add i32 %"$format.size89", 1
  %"$format.text91" = load i8*, i8** %format.text85, align 8
  %"$format.size92" = load i32, i32* %format.size80, align 4
  %18 = insertvalue %String zeroinitializer, i8* %"$format.text91", 0
  %19 = insertvalue %String %18, i32 %"$format.size92", 1
  %20 = insertvalue %String %19, i32 %ADD90, 2
  call void @"io::out"(%String %20)
  %call.String.create93 = call %String @String.create(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.9, i32 0, i32 0))
  store %String %call.String.create93, %String* %t3, align 8
  %call.String.trim94 = call %String @String.trim(%String* %t3)
  store %String %call.String.trim94, %String* %t4, align 8
  %call.String.chars95 = call i8* @String.chars(%String* %t4)
  %call.snprintf97 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.10, i32 0, i32 0), i8* %call.String.chars95)
  store i32 %call.snprintf97, i32* %format.size96, align 4
  %"$format.size98" = load i32, i32* %format.size96, align 4
  %ADD99 = add i32 %"$format.size98", 1
  %AS100 = sext i32 %ADD99 to i64
  %call.calloc102 = call i8* @calloc(i64 %AS100, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc102, i8** %format.text101, align 8
  %"$format.text103" = load i8*, i8** %format.text101, align 8
  %call.snprintf104 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text103", i64 %AS100, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.10, i32 0, i32 0), i8* %call.String.chars95)
  %"$format.size105" = load i32, i32* %format.size96, align 4
  %ADD106 = add i32 %"$format.size105", 1
  %"$format.text107" = load i8*, i8** %format.text101, align 8
  %"$format.size108" = load i32, i32* %format.size96, align 4
  %21 = insertvalue %String zeroinitializer, i8* %"$format.text107", 0
  %22 = insertvalue %String %21, i32 %"$format.size108", 1
  %23 = insertvalue %String %22, i32 %ADD106, 2
  call void @"io::out"(%String %23)
  %call.String.create109 = call %String @String.create(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.11, i32 0, i32 0))
  store %String %call.String.create109, %String* %t5, align 8
  %call.String.trim110 = call %String @String.trim(%String* %t5)
  store %String %call.String.trim110, %String* %t6, align 8
  %call.String.size111 = call i32 @String.size(%String* %t6)
  %call.snprintf113 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.12, i32 0, i32 0), i32 %call.String.size111)
  store i32 %call.snprintf113, i32* %format.size112, align 4
  %"$format.size114" = load i32, i32* %format.size112, align 4
  %ADD115 = add i32 %"$format.size114", 1
  %AS116 = sext i32 %ADD115 to i64
  %call.calloc118 = call i8* @calloc(i64 %AS116, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc118, i8** %format.text117, align 8
  %"$format.text119" = load i8*, i8** %format.text117, align 8
  %call.snprintf120 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text119", i64 %AS116, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.12, i32 0, i32 0), i32 %call.String.size111)
  %"$format.size121" = load i32, i32* %format.size112, align 4
  %ADD122 = add i32 %"$format.size121", 1
  %"$format.text123" = load i8*, i8** %format.text117, align 8
  %"$format.size124" = load i32, i32* %format.size112, align 4
  %24 = insertvalue %String zeroinitializer, i8* %"$format.text123", 0
  %25 = insertvalue %String %24, i32 %"$format.size124", 1
  %26 = insertvalue %String %25, i32 %ADD122, 2
  call void @"io::out"(%String %26)
  %call.String.create125 = call %String @String.create(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.13, i32 0, i32 0))
  store %String %call.String.create125, %String* %r1, align 8
  %call.String.replace = call %String @String.replace(%String* %r1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.14, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.15, i32 0, i32 0))
  store %String %call.String.replace, %String* %r2, align 8
  %call.String.chars126 = call i8* @String.chars(%String* %r2)
  %call.snprintf128 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.16, i32 0, i32 0), i8* %call.String.chars126)
  store i32 %call.snprintf128, i32* %format.size127, align 4
  %"$format.size129" = load i32, i32* %format.size127, align 4
  %ADD130 = add i32 %"$format.size129", 1
  %AS131 = sext i32 %ADD130 to i64
  %call.calloc133 = call i8* @calloc(i64 %AS131, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc133, i8** %format.text132, align 8
  %"$format.text134" = load i8*, i8** %format.text132, align 8
  %call.snprintf135 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text134", i64 %AS131, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.16, i32 0, i32 0), i8* %call.String.chars126)
  %"$format.size136" = load i32, i32* %format.size127, align 4
  %ADD137 = add i32 %"$format.size136", 1
  %"$format.text138" = load i8*, i8** %format.text132, align 8
  %"$format.size139" = load i32, i32* %format.size127, align 4
  %27 = insertvalue %String zeroinitializer, i8* %"$format.text138", 0
  %28 = insertvalue %String %27, i32 %"$format.size139", 1
  %29 = insertvalue %String %28, i32 %ADD137, 2
  call void @"io::out"(%String %29)
  %call.String.replace140 = call %String @String.replace(%String* %r1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.17, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.18, i32 0, i32 0))
  store %String %call.String.replace140, %String* %r3, align 8
  %call.String.chars141 = call i8* @String.chars(%String* %r3)
  %call.snprintf143 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.19, i32 0, i32 0), i8* %call.String.chars141)
  store i32 %call.snprintf143, i32* %format.size142, align 4
  %"$format.size144" = load i32, i32* %format.size142, align 4
  %ADD145 = add i32 %"$format.size144", 1
  %AS146 = sext i32 %ADD145 to i64
  %call.calloc148 = call i8* @calloc(i64 %AS146, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc148, i8** %format.text147, align 8
  %"$format.text149" = load i8*, i8** %format.text147, align 8
  %call.snprintf150 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text149", i64 %AS146, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.19, i32 0, i32 0), i8* %call.String.chars141)
  %"$format.size151" = load i32, i32* %format.size142, align 4
  %ADD152 = add i32 %"$format.size151", 1
  %"$format.text153" = load i8*, i8** %format.text147, align 8
  %"$format.size154" = load i32, i32* %format.size142, align 4
  %30 = insertvalue %String zeroinitializer, i8* %"$format.text153", 0
  %31 = insertvalue %String %30, i32 %"$format.size154", 1
  %32 = insertvalue %String %31, i32 %ADD152, 2
  call void @"io::out"(%String %32)
  %call.String.replace155 = call %String @String.replace(%String* %r1, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.20, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.21, i32 0, i32 0))
  store %String %call.String.replace155, %String* %r4, align 8
  %call.String.chars156 = call i8* @String.chars(%String* %r4)
  %call.snprintf158 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.22, i32 0, i32 0), i8* %call.String.chars156)
  store i32 %call.snprintf158, i32* %format.size157, align 4
  %"$format.size159" = load i32, i32* %format.size157, align 4
  %ADD160 = add i32 %"$format.size159", 1
  %AS161 = sext i32 %ADD160 to i64
  %call.calloc163 = call i8* @calloc(i64 %AS161, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc163, i8** %format.text162, align 8
  %"$format.text164" = load i8*, i8** %format.text162, align 8
  %call.snprintf165 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text164", i64 %AS161, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.22, i32 0, i32 0), i8* %call.String.chars156)
  %"$format.size166" = load i32, i32* %format.size157, align 4
  %ADD167 = add i32 %"$format.size166", 1
  %"$format.text168" = load i8*, i8** %format.text162, align 8
  %"$format.size169" = load i32, i32* %format.size157, align 4
  %33 = insertvalue %String zeroinitializer, i8* %"$format.text168", 0
  %34 = insertvalue %String %33, i32 %"$format.size169", 1
  %35 = insertvalue %String %34, i32 %ADD167, 2
  call void @"io::out"(%String %35)
  %call.String.create170 = call %String @String.create(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.23, i32 0, i32 0))
  store %String %call.String.create170, %String* %rep, align 8
  %call.String.repeat = call %String @String.repeat(%String* %rep, i32 3)
  store %String %call.String.repeat, %String* %rep2, align 8
  %call.String.chars171 = call i8* @String.chars(%String* %rep2)
  %call.snprintf173 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.24, i32 0, i32 0), i8* %call.String.chars171)
  store i32 %call.snprintf173, i32* %format.size172, align 4
  %"$format.size174" = load i32, i32* %format.size172, align 4
  %ADD175 = add i32 %"$format.size174", 1
  %AS176 = sext i32 %ADD175 to i64
  %call.calloc178 = call i8* @calloc(i64 %AS176, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc178, i8** %format.text177, align 8
  %"$format.text179" = load i8*, i8** %format.text177, align 8
  %call.snprintf180 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text179", i64 %AS176, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.24, i32 0, i32 0), i8* %call.String.chars171)
  %"$format.size181" = load i32, i32* %format.size172, align 4
  %ADD182 = add i32 %"$format.size181", 1
  %"$format.text183" = load i8*, i8** %format.text177, align 8
  %"$format.size184" = load i32, i32* %format.size172, align 4
  %36 = insertvalue %String zeroinitializer, i8* %"$format.text183", 0
  %37 = insertvalue %String %36, i32 %"$format.size184", 1
  %38 = insertvalue %String %37, i32 %ADD182, 2
  call void @"io::out"(%String %38)
  %call.String.repeat185 = call %String @String.repeat(%String* %rep, i32 0)
  store %String %call.String.repeat185, %String* %rep3, align 8
  %call.String.size186 = call i32 @String.size(%String* %rep3)
  %call.snprintf188 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.25, i32 0, i32 0), i32 %call.String.size186)
  store i32 %call.snprintf188, i32* %format.size187, align 4
  %"$format.size189" = load i32, i32* %format.size187, align 4
  %ADD190 = add i32 %"$format.size189", 1
  %AS191 = sext i32 %ADD190 to i64
  %call.calloc193 = call i8* @calloc(i64 %AS191, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc193, i8** %format.text192, align 8
  %"$format.text194" = load i8*, i8** %format.text192, align 8
  %call.snprintf195 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text194", i64 %AS191, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.25, i32 0, i32 0), i32 %call.String.size186)
  %"$format.size196" = load i32, i32* %format.size187, align 4
  %ADD197 = add i32 %"$format.size196", 1
  %"$format.text198" = load i8*, i8** %format.text192, align 8
  %"$format.size199" = load i32, i32* %format.size187, align 4
  %39 = insertvalue %String zeroinitializer, i8* %"$format.text198", 0
  %40 = insertvalue %String %39, i32 %"$format.size199", 1
  %41 = insertvalue %String %40, i32 %ADD197, 2
  call void @"io::out"(%String %41)
  %call.String.create200 = call %String @String.create(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.26, i32 0, i32 0))
  store %String %call.String.create200, %String* %rev, align 8
  %call.String.reverse = call %String @String.reverse(%String* %rev)
  store %String %call.String.reverse, %String* %rev2, align 8
  %call.String.chars201 = call i8* @String.chars(%String* %rev2)
  %call.snprintf203 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.27, i32 0, i32 0), i8* %call.String.chars201)
  store i32 %call.snprintf203, i32* %format.size202, align 4
  %"$format.size204" = load i32, i32* %format.size202, align 4
  %ADD205 = add i32 %"$format.size204", 1
  %AS206 = sext i32 %ADD205 to i64
  %call.calloc208 = call i8* @calloc(i64 %AS206, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc208, i8** %format.text207, align 8
  %"$format.text209" = load i8*, i8** %format.text207, align 8
  %call.snprintf210 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text209", i64 %AS206, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.27, i32 0, i32 0), i8* %call.String.chars201)
  %"$format.size211" = load i32, i32* %format.size202, align 4
  %ADD212 = add i32 %"$format.size211", 1
  %"$format.text213" = load i8*, i8** %format.text207, align 8
  %"$format.size214" = load i32, i32* %format.size202, align 4
  %42 = insertvalue %String zeroinitializer, i8* %"$format.text213", 0
  %43 = insertvalue %String %42, i32 %"$format.size214", 1
  %44 = insertvalue %String %43, i32 %ADD212, 2
  call void @"io::out"(%String %44)
  %call.String.create215 = call %String @String.create(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.28, i32 0, i32 0))
  store %String %call.String.create215, %String* %rev3, align 8
  %call.String.reverse216 = call %String @String.reverse(%String* %rev3)
  store %String %call.String.reverse216, %String* %rev4, align 8
  %call.String.chars217 = call i8* @String.chars(%String* %rev4)
  %call.snprintf219 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.29, i32 0, i32 0), i8* %call.String.chars217)
  store i32 %call.snprintf219, i32* %format.size218, align 4
  %"$format.size220" = load i32, i32* %format.size218, align 4
  %ADD221 = add i32 %"$format.size220", 1
  %AS222 = sext i32 %ADD221 to i64
  %call.calloc224 = call i8* @calloc(i64 %AS222, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc224, i8** %format.text223, align 8
  %"$format.text225" = load i8*, i8** %format.text223, align 8
  %call.snprintf226 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text225", i64 %AS222, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.29, i32 0, i32 0), i8* %call.String.chars217)
  %"$format.size227" = load i32, i32* %format.size218, align 4
  %ADD228 = add i32 %"$format.size227", 1
  %"$format.text229" = load i8*, i8** %format.text223, align 8
  %"$format.size230" = load i32, i32* %format.size218, align 4
  %45 = insertvalue %String zeroinitializer, i8* %"$format.text229", 0
  %46 = insertvalue %String %45, i32 %"$format.size230", 1
  %47 = insertvalue %String %46, i32 %ADD228, 2
  call void @"io::out"(%String %47)
  %48 = bitcast %String* %rev4 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %48)
  %49 = bitcast %String* %rev3 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %49)
  %50 = bitcast %String* %rev2 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %50)
  %51 = bitcast %String* %rev to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %51)
  %52 = bitcast %String* %rep3 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %52)
  %53 = bitcast %String* %rep2 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %53)
  %54 = bitcast %String* %rep to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %54)
  %55 = bitcast %String* %r4 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %55)
  %56 = bitcast %String* %r3 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %56)
  %57 = bitcast %String* %r2 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %57)
  %58 = bitcast %String* %r1 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %58)
  %59 = bitcast %String* %t6 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %59)
  %60 = bitcast %String* %t5 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %60)
  %61 = bitcast %String* %t4 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %61)
  %62 = bitcast %String* %t3 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %62)
  %63 = bitcast %String* %t2 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %63)
  %64 = bitcast %String* %t1 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %64)
  %65 = bitcast %String* %sub4 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %65)
  %66 = bitcast %String* %sub3 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %66)
  %67 = bitcast %String* %sub2 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %67)
  %68 = bitcast %String* %sub to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %68)
  %69 = bitcast %String* %l to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %69)
  %70 = bitcast %String* %u to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %70)
  %71 = bitcast %String* %s to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %71)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

---

## 020 — string: operators: + += == != < > against String and against a literal

```ura
// stdout: hello world
// stdout: hello there
// stdout: hello world
// stdout: testing
// stdout: True
// stdout: False
// stdout: True
// stdout: False
// stdout: True
// stdout: False
// stdout: True
// stdout: False
// stdout: True
// stdout: False
// stdout: True
// stdout: False
// stdout: abc True
// expect: 0
use "@/core"
use "@/posix"

fn main() i32:
    a String = String::create("hello")
    b String = String::create(" world")
    c String = a + b
    io::out($"{c}\n")

    d String = a + " there"
    io::out($"{d}\n")

    a += b
    io::out($"{a}\n")

    e String = String::create("test")
    e += "ing"
    io::out($"{e}\n")

    x String = String::create("abc")
    y String = String::create("abc")
    z String = String::create("xyz")

    io::out($"{x == y}\n")
    io::out($"{x == z}\n")
    io::out($"{x != z}\n")
    io::out($"{x != y}\n")
    io::out($"{x < z}\n")
    io::out($"{z < x}\n")
    io::out($"{z > x}\n")
    io::out($"{x > z}\n")

    io::out($"{x == "abc"}\n")
    io::out($"{x == "xyz"}\n")
    io::out($"{x != "xyz"}\n")
    io::out($"{x != "abc"}\n")

    f String = String::create("")
    g String = String::create("")
    f = y
    io::out($"{f} {f == y}\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [6 x i8] c"hello\00", align 1
@str.3 = private unnamed_addr constant [7 x i8] c" world\00", align 1
@str.4 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.5 = private unnamed_addr constant [7 x i8] c" there\00", align 1
@str.6 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.7 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.8 = private unnamed_addr constant [5 x i8] c"test\00", align 1
@str.9 = private unnamed_addr constant [4 x i8] c"ing\00", align 1
@str.10 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.11 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@str.12 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@str.13 = private unnamed_addr constant [4 x i8] c"xyz\00", align 1
@str.14 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.15 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.16 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.17 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.18 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.19 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.20 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.21 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.22 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.23 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.24 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.25 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.26 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.27 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.28 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.29 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.30 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.31 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.32 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.33 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.34 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.35 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.36 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.37 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.38 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@str.39 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.40 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.41 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.42 = private unnamed_addr constant [4 x i8] c"xyz\00", align 1
@str.43 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.44 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.45 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.46 = private unnamed_addr constant [4 x i8] c"xyz\00", align 1
@str.47 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.48 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.49 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.50 = private unnamed_addr constant [4 x i8] c"abc\00", align 1
@str.51 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.52 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.53 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.54 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.55 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.56 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.57 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.58 = private unnamed_addr constant [7 x i8] c"%s %s\0A\00", align 1

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

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %while4

while4:                                           ; preds = %while4.then, %endif
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
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
  br label %while4

endloop:                                          ; preds = %while4
  ret void
}

define void @"Array<i8>.="(%"Array<i8>"* %0, %"Array<i8>"* %1) {
entry:
  %left = alloca %"Array<i8>"*, align 8
  %right = alloca %"Array<i8>"*, align 8
  %i = alloca i32, align 4
  store %"Array<i8>"* %0, %"Array<i8>"** %left, align 8
  store %"Array<i8>"* %1, %"Array<i8>"** %right, align 8
  %"*left" = load %"Array<i8>"*, %"Array<i8>"** %left, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*left")
  %"*left1" = load %"Array<i8>"*, %"Array<i8>"** %left, align 8
  %"*right" = load %"Array<i8>"*, %"Array<i8>"** %right, align 8
  %"*right.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*right", i32 0, i32 1
  %"$*right.count" = load i32, i32* %"*right.count", align 4
  call void @"Array<i8>.reserve"(%"Array<i8>"* %"*left1", i32 %"$*right.count")
  store i32 0, i32* %i, align 4
  br label %while5

while5:                                           ; preds = %while5.then, %entry
  %"*right2" = load %"Array<i8>"*, %"Array<i8>"** %right, align 8
  %"*right.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*right2", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*right.count4" = load i32, i32* %"*right.count3", align 4
  %LT = icmp slt i32 %"$i", %"$*right.count4"
  br i1 %LT, label %while5.then, label %endloop

while5.then:                                      ; preds = %while5
  %"*left5" = load %"Array<i8>"*, %"Array<i8>"** %left, align 8
  %"*left.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*left5", i32 0, i32 0
  %"$*left.data" = load i8*, i8** %"*left.data", align 8
  %"$i6" = load i32, i32* %i, align 4
  %"*left.data[]" = getelementptr inbounds i8, i8* %"$*left.data", i32 %"$i6"
  %"*right7" = load %"Array<i8>"*, %"Array<i8>"** %right, align 8
  %"*right.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*right7", i32 0, i32 0
  %"$*right.data" = load i8*, i8** %"*right.data", align 8
  %"$i8" = load i32, i32* %i, align 4
  %"*right.data[]" = getelementptr inbounds i8, i8* %"$*right.data", i32 %"$i8"
  %"$*right.data[]" = load i8, i8* %"*right.data[]", align 1
  store i8 %"$*right.data[]", i8* %"*left.data[]", align 1
  %"$i9" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i9", 1
  store i32 %ADD, i32* %i, align 4
  br label %while5

endloop:                                          ; preds = %while5
  %"*left10" = load %"Array<i8>"*, %"Array<i8>"** %left, align 8
  %"*left.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*left10", i32 0, i32 1
  %"*right11" = load %"Array<i8>"*, %"Array<i8>"** %right, align 8
  %"*right.count12" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*right11", i32 0, i32 1
  %"$*right.count13" = load i32, i32* %"*right.count12", align 4
  store i32 %"$*right.count13", i32* %"*left.count", align 4
  ret void
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %value = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %value, align 8
  %"*value" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*value")
  br label %if6

if6:                                              ; preds = %entry
  %"*value1" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*value2" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i8*, i8** %"*value.data", align 8
  call void @"free<i8>"(i8* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if7

if7:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if7.then, label %endif

if7.then:                                         ; preds = %if7
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if7.then, %if7
  br label %if8

if8:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if8.then, label %endif1

if8.then:                                         ; preds = %if8
  ret void

endif1:                                           ; preds = %if8
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
  br label %if9

if9:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if9.then, label %endif2

if9.then:                                         ; preds = %if9
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if9.then, %if9
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

define void @String.append(%String* %0, i8 %1) {
entry:
  %self = alloca %String*, align 8
  %value = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store i8 %1, i8* %value, align 1
  br label %if10

if10:                                             ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if10.then, label %endif

if10.then:                                        ; preds = %if10
  %"*self2" = load %String*, %String** %self, align 8
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @String.reserve(%String* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if10.then, %if10
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

define void @String.join(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  store i32 0, i32* %i, align 4
  br label %while11

while11:                                          ; preds = %while11.then, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$i" = load i32, i32* %i, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$i"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while11.then, label %endloop

while11.then:                                     ; preds = %while11
  %"*self" = load %String*, %String** %self, align 8
  %"$text1" = load i8*, i8** %text, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"text[]3" = getelementptr inbounds i8, i8* %"$text1", i32 %"$i2"
  %"$text[]4" = load i8, i8* %"text[]3", align 1
  call void @String.append(%String* %"*self", i8 %"$text[]4")
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while11

endloop:                                          ; preds = %while11
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

define i32 @String.compare(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  %other = alloca %String*, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store %String* %1, %String** %other, align 8
  store i32 0, i32* %i, align 4
  br label %while12

while12:                                          ; preds = %endif, %entry
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
  br i1 %AND, label %while12.then, label %endloop

while12.then:                                     ; preds = %while12
  br label %if13

if13:                                             ; preds = %while12.then
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
  br i1 %NQ, label %if13.then, label %endif

if13.then:                                        ; preds = %if13
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

endif:                                            ; preds = %if13
  %"$i20" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i20", 1
  store i32 %ADD, i32* %i, align 4
  br label %while12

endloop:                                          ; preds = %while12
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
  %left = alloca %String*, align 8
  %right = alloca %String*, align 8
  store %String* %0, %String** %left, align 8
  store %String* %1, %String** %right, align 8
  %"*left" = load %String*, %String** %left, align 8
  %"*right" = load %String*, %String** %right, align 8
  %call.String.compare = call i32 @String.compare(%String* %"*left", %String* %"*right")
  %EQ = icmp eq i32 %call.String.compare, 0
  ret i1 %EQ
}

define i1 @"String.!="(%String* %0, %String* %1) {
entry:
  %left = alloca %String*, align 8
  %right = alloca %String*, align 8
  store %String* %0, %String** %left, align 8
  store %String* %1, %String** %right, align 8
  %"*left" = load %String*, %String** %left, align 8
  %"*right" = load %String*, %String** %right, align 8
  %call.String.compare = call i32 @String.compare(%String* %"*left", %String* %"*right")
  %NQ = icmp ne i32 %call.String.compare, 0
  ret i1 %NQ
}

define i1 @"String.<"(%String* %0, %String* %1) {
entry:
  %left = alloca %String*, align 8
  %right = alloca %String*, align 8
  store %String* %0, %String** %left, align 8
  store %String* %1, %String** %right, align 8
  %"*left" = load %String*, %String** %left, align 8
  %"*right" = load %String*, %String** %right, align 8
  %call.String.compare = call i32 @String.compare(%String* %"*left", %String* %"*right")
  %LT = icmp slt i32 %call.String.compare, 0
  ret i1 %LT
}

define i1 @"String.>"(%String* %0, %String* %1) {
entry:
  %left = alloca %String*, align 8
  %right = alloca %String*, align 8
  store %String* %0, %String** %left, align 8
  store %String* %1, %String** %right, align 8
  %"*left" = load %String*, %String** %left, align 8
  %"*right" = load %String*, %String** %right, align 8
  %call.String.compare = call i32 @String.compare(%String* %"*left", %String* %"*right")
  %GT = icmp sgt i32 %call.String.compare, 0
  ret i1 %GT
}

define void @"String.+="(%String* %0, i8* %1) {
entry:
  %left = alloca %String*, align 8
  %right = alloca i8*, align 8
  store %String* %0, %String** %left, align 8
  store i8* %1, i8** %right, align 8
  %"*left" = load %String*, %String** %left, align 8
  %"$right" = load i8*, i8** %right, align 8
  call void @String.join(%String* %"*left", i8* %"$right")
  ret void
}

define void @"String.+=.1"(%String* %0, %String* %1) {
entry:
  %left = alloca %String*, align 8
  %right = alloca %String*, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %left, align 8
  store %String* %1, %String** %right, align 8
  store i32 0, i32* %i, align 4
  br label %while14

while14:                                          ; preds = %while14.then, %entry
  %"*right" = load %String*, %String** %right, align 8
  %"*right.count" = getelementptr inbounds %String, %String* %"*right", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*right.count" = load i32, i32* %"*right.count", align 4
  %LT = icmp slt i32 %"$i", %"$*right.count"
  br i1 %LT, label %while14.then, label %endloop

while14.then:                                     ; preds = %while14
  %"*left" = load %String*, %String** %left, align 8
  %"*right1" = load %String*, %String** %right, align 8
  %"*right.data" = getelementptr inbounds %String, %String* %"*right1", i32 0, i32 0
  %"$*right.data" = load i8*, i8** %"*right.data", align 8
  %"$i2" = load i32, i32* %i, align 4
  %"*right.data[]" = getelementptr inbounds i8, i8* %"$*right.data", i32 %"$i2"
  %"$*right.data[]" = load i8, i8* %"*right.data[]", align 1
  call void @String.append(%String* %"*left", i8 %"$*right.data[]")
  %"$i3" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i3", 1
  store i32 %ADD, i32* %i, align 4
  br label %while14

endloop:                                          ; preds = %while14
  ret void
}

define %String @"String.+"(%String* %0, i8* %1) {
entry:
  %left = alloca %String*, align 8
  %right = alloca i8*, align 8
  %res = alloca %String, align 8
  %copy = alloca %String, align 8
  store %String* %0, %String** %left, align 8
  store i8* %1, i8** %right, align 8
  store %String zeroinitializer, %String* %copy, align 8
  %"*left" = load %String*, %String** %left, align 8
  %2 = bitcast %String* %copy to %"Array<i8>"*
  %3 = bitcast %String* %"*left" to %"Array<i8>"*
  call void @"Array<i8>.="(%"Array<i8>"* %2, %"Array<i8>"* %3)
  %"$copy" = load %String, %String* %copy, align 8
  store %String %"$copy", %String* %res, align 8
  %"$right" = load i8*, i8** %right, align 8
  call void @String.join(%String* %res, i8* %"$right")
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define %String @"String.+.2"(%String* %0, %String* %1) {
entry:
  %left = alloca %String*, align 8
  %right = alloca %String*, align 8
  %res = alloca %String, align 8
  %copy = alloca %String, align 8
  store %String* %0, %String** %left, align 8
  store %String* %1, %String** %right, align 8
  store %String zeroinitializer, %String* %copy, align 8
  %"*left" = load %String*, %String** %left, align 8
  %2 = bitcast %String* %copy to %"Array<i8>"*
  %3 = bitcast %String* %"*left" to %"Array<i8>"*
  call void @"Array<i8>.="(%"Array<i8>"* %2, %"Array<i8>"* %3)
  %"$copy" = load %String, %String* %copy, align 8
  store %String %"$copy", %String* %res, align 8
  %"*right" = load %String*, %String** %right, align 8
  call void @"String.+=.1"(%String* %res, %String* %"*right")
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
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i32 @main() {
entry:
  %a = alloca %String, align 8
  %b = alloca %String, align 8
  %c = alloca %String, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %d = alloca %String, align 8
  %format.size21 = alloca i32, align 4
  %format.text26 = alloca i8*, align 8
  %format.size35 = alloca i32, align 4
  %format.text40 = alloca i8*, align 8
  %e = alloca %String, align 8
  %format.size50 = alloca i32, align 4
  %format.text55 = alloca i8*, align 8
  %x = alloca %String, align 8
  %y = alloca %String, align 8
  %z = alloca %String, align 8
  %format.size66 = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text71 = alloca i8*, align 8
  %format.size81 = alloca i32, align 4
  %bool.text82 = alloca i8*, align 8
  %format.text88 = alloca i8*, align 8
  %format.size97 = alloca i32, align 4
  %bool.text98 = alloca i8*, align 8
  %format.text104 = alloca i8*, align 8
  %format.size114 = alloca i32, align 4
  %bool.text115 = alloca i8*, align 8
  %format.text121 = alloca i8*, align 8
  %format.size130 = alloca i32, align 4
  %bool.text131 = alloca i8*, align 8
  %format.text137 = alloca i8*, align 8
  %format.size147 = alloca i32, align 4
  %bool.text148 = alloca i8*, align 8
  %format.text154 = alloca i8*, align 8
  %format.size163 = alloca i32, align 4
  %bool.text164 = alloca i8*, align 8
  %format.text170 = alloca i8*, align 8
  %format.size180 = alloca i32, align 4
  %bool.text181 = alloca i8*, align 8
  %format.text187 = alloca i8*, align 8
  %tmp = alloca %String, align 8
  %format.size197 = alloca i32, align 4
  %bool.text198 = alloca i8*, align 8
  %format.text204 = alloca i8*, align 8
  %tmp213 = alloca %String, align 8
  %format.size215 = alloca i32, align 4
  %bool.text216 = alloca i8*, align 8
  %format.text222 = alloca i8*, align 8
  %tmp231 = alloca %String, align 8
  %format.size233 = alloca i32, align 4
  %bool.text234 = alloca i8*, align 8
  %format.text240 = alloca i8*, align 8
  %tmp249 = alloca %String, align 8
  %format.size251 = alloca i32, align 4
  %bool.text252 = alloca i8*, align 8
  %format.text258 = alloca i8*, align 8
  %f = alloca %String, align 8
  %g = alloca %String, align 8
  %copy = alloca %String, align 8
  %format.size271 = alloca i32, align 4
  %bool.text272 = alloca i8*, align 8
  %format.text278 = alloca i8*, align 8
  %call.String.create = call %String @String.create(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0))
  store %String %call.String.create, %String* %a, align 8
  %call.String.create13 = call %String @String.create(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.3, i32 0, i32 0))
  store %String %call.String.create13, %String* %b, align 8
  %"call.String.+" = call %String @"String.+.2"(%String* %a, %String* %b)
  store %String %"call.String.+", %String* %c, align 8
  %call.String.chars = call i8* @String.chars(%String* %c)
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i8* %call.String.chars)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf14 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i8* %call.String.chars)
  %"$format.size15" = load i32, i32* %format.size, align 4
  %ADD16 = add i32 %"$format.size15", 1
  %"$format.text17" = load i8*, i8** %format.text, align 8
  %"$format.size18" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text17", 0
  %1 = insertvalue %String %0, i32 %"$format.size18", 1
  %2 = insertvalue %String %1, i32 %ADD16, 2
  call void @"io::out"(%String %2)
  %"call.String.+19" = call %String @"String.+"(%String* %a, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.5, i32 0, i32 0))
  store %String %"call.String.+19", %String* %d, align 8
  %call.String.chars20 = call i8* @String.chars(%String* %d)
  %call.snprintf22 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.6, i32 0, i32 0), i8* %call.String.chars20)
  store i32 %call.snprintf22, i32* %format.size21, align 4
  %"$format.size23" = load i32, i32* %format.size21, align 4
  %ADD24 = add i32 %"$format.size23", 1
  %AS25 = sext i32 %ADD24 to i64
  %call.calloc27 = call i8* @calloc(i64 %AS25, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc27, i8** %format.text26, align 8
  %"$format.text28" = load i8*, i8** %format.text26, align 8
  %call.snprintf29 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text28", i64 %AS25, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.6, i32 0, i32 0), i8* %call.String.chars20)
  %"$format.size30" = load i32, i32* %format.size21, align 4
  %ADD31 = add i32 %"$format.size30", 1
  %"$format.text32" = load i8*, i8** %format.text26, align 8
  %"$format.size33" = load i32, i32* %format.size21, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text32", 0
  %4 = insertvalue %String %3, i32 %"$format.size33", 1
  %5 = insertvalue %String %4, i32 %ADD31, 2
  call void @"io::out"(%String %5)
  call void @"String.+=.1"(%String* %a, %String* %b)
  %call.String.chars34 = call i8* @String.chars(%String* %a)
  %call.snprintf36 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.7, i32 0, i32 0), i8* %call.String.chars34)
  store i32 %call.snprintf36, i32* %format.size35, align 4
  %"$format.size37" = load i32, i32* %format.size35, align 4
  %ADD38 = add i32 %"$format.size37", 1
  %AS39 = sext i32 %ADD38 to i64
  %call.calloc41 = call i8* @calloc(i64 %AS39, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc41, i8** %format.text40, align 8
  %"$format.text42" = load i8*, i8** %format.text40, align 8
  %call.snprintf43 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text42", i64 %AS39, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.7, i32 0, i32 0), i8* %call.String.chars34)
  %"$format.size44" = load i32, i32* %format.size35, align 4
  %ADD45 = add i32 %"$format.size44", 1
  %"$format.text46" = load i8*, i8** %format.text40, align 8
  %"$format.size47" = load i32, i32* %format.size35, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text46", 0
  %7 = insertvalue %String %6, i32 %"$format.size47", 1
  %8 = insertvalue %String %7, i32 %ADD45, 2
  call void @"io::out"(%String %8)
  %call.String.create48 = call %String @String.create(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.8, i32 0, i32 0))
  store %String %call.String.create48, %String* %e, align 8
  call void @"String.+="(%String* %e, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.9, i32 0, i32 0))
  %call.String.chars49 = call i8* @String.chars(%String* %e)
  %call.snprintf51 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.10, i32 0, i32 0), i8* %call.String.chars49)
  store i32 %call.snprintf51, i32* %format.size50, align 4
  %"$format.size52" = load i32, i32* %format.size50, align 4
  %ADD53 = add i32 %"$format.size52", 1
  %AS54 = sext i32 %ADD53 to i64
  %call.calloc56 = call i8* @calloc(i64 %AS54, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc56, i8** %format.text55, align 8
  %"$format.text57" = load i8*, i8** %format.text55, align 8
  %call.snprintf58 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text57", i64 %AS54, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.10, i32 0, i32 0), i8* %call.String.chars49)
  %"$format.size59" = load i32, i32* %format.size50, align 4
  %ADD60 = add i32 %"$format.size59", 1
  %"$format.text61" = load i8*, i8** %format.text55, align 8
  %"$format.size62" = load i32, i32* %format.size50, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text61", 0
  %10 = insertvalue %String %9, i32 %"$format.size62", 1
  %11 = insertvalue %String %10, i32 %ADD60, 2
  call void @"io::out"(%String %11)
  %call.String.create63 = call %String @String.create(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.11, i32 0, i32 0))
  store %String %call.String.create63, %String* %x, align 8
  %call.String.create64 = call %String @String.create(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.12, i32 0, i32 0))
  store %String %call.String.create64, %String* %y, align 8
  %call.String.create65 = call %String @String.create(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.13, i32 0, i32 0))
  store %String %call.String.create65, %String* %z, align 8
  %"call.String.==" = call i1 @"String.=="(%String* %x, %String* %y)
  store i32 0, i32* %format.size66, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if15

if15:                                             ; preds = %entry
  br i1 %"call.String.==", label %if15.then, label %else16

if15.then:                                        ; preds = %if15
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.14, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else16:                                           ; preds = %if15
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.15, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else16, %if15.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf67 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.16, i32 0, i32 0), i8* %"$bool.text")
  store i32 %call.snprintf67, i32* %format.size66, align 4
  %"$format.size68" = load i32, i32* %format.size66, align 4
  %ADD69 = add i32 %"$format.size68", 1
  %AS70 = sext i32 %ADD69 to i64
  %call.calloc72 = call i8* @calloc(i64 %AS70, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc72, i8** %format.text71, align 8
  %"$format.text73" = load i8*, i8** %format.text71, align 8
  %"$bool.text74" = load i8*, i8** %bool.text, align 8
  %call.snprintf75 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text73", i64 %AS70, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.16, i32 0, i32 0), i8* %"$bool.text74")
  %"$format.size76" = load i32, i32* %format.size66, align 4
  %ADD77 = add i32 %"$format.size76", 1
  %"$format.text78" = load i8*, i8** %format.text71, align 8
  %"$format.size79" = load i32, i32* %format.size66, align 4
  %12 = insertvalue %String zeroinitializer, i8* %"$format.text78", 0
  %13 = insertvalue %String %12, i32 %"$format.size79", 1
  %14 = insertvalue %String %13, i32 %ADD77, 2
  call void @"io::out"(%String %14)
  %"call.String.==80" = call i1 @"String.=="(%String* %x, %String* %z)
  store i32 0, i32* %format.size81, align 4
  store i8* null, i8** %bool.text82, align 8
  br label %if17

if17:                                             ; preds = %endif
  br i1 %"call.String.==80", label %if17.then, label %else18

if17.then:                                        ; preds = %if17
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.17, i32 0, i32 0), i8** %bool.text82, align 8
  br label %endif1

else18:                                           ; preds = %if17
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.18, i32 0, i32 0), i8** %bool.text82, align 8
  br label %endif1

endif1:                                           ; preds = %else18, %if17.then
  %"$bool.text83" = load i8*, i8** %bool.text82, align 8
  %call.snprintf84 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.19, i32 0, i32 0), i8* %"$bool.text83")
  store i32 %call.snprintf84, i32* %format.size81, align 4
  %"$format.size85" = load i32, i32* %format.size81, align 4
  %ADD86 = add i32 %"$format.size85", 1
  %AS87 = sext i32 %ADD86 to i64
  %call.calloc89 = call i8* @calloc(i64 %AS87, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc89, i8** %format.text88, align 8
  %"$format.text90" = load i8*, i8** %format.text88, align 8
  %"$bool.text91" = load i8*, i8** %bool.text82, align 8
  %call.snprintf92 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text90", i64 %AS87, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.19, i32 0, i32 0), i8* %"$bool.text91")
  %"$format.size93" = load i32, i32* %format.size81, align 4
  %ADD94 = add i32 %"$format.size93", 1
  %"$format.text95" = load i8*, i8** %format.text88, align 8
  %"$format.size96" = load i32, i32* %format.size81, align 4
  %15 = insertvalue %String zeroinitializer, i8* %"$format.text95", 0
  %16 = insertvalue %String %15, i32 %"$format.size96", 1
  %17 = insertvalue %String %16, i32 %ADD94, 2
  call void @"io::out"(%String %17)
  %"call.String.!=" = call i1 @"String.!="(%String* %x, %String* %z)
  store i32 0, i32* %format.size97, align 4
  store i8* null, i8** %bool.text98, align 8
  br label %if19

if19:                                             ; preds = %endif1
  br i1 %"call.String.!=", label %if19.then, label %else20

if19.then:                                        ; preds = %if19
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.20, i32 0, i32 0), i8** %bool.text98, align 8
  br label %endif2

else20:                                           ; preds = %if19
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.21, i32 0, i32 0), i8** %bool.text98, align 8
  br label %endif2

endif2:                                           ; preds = %else20, %if19.then
  %"$bool.text99" = load i8*, i8** %bool.text98, align 8
  %call.snprintf100 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.22, i32 0, i32 0), i8* %"$bool.text99")
  store i32 %call.snprintf100, i32* %format.size97, align 4
  %"$format.size101" = load i32, i32* %format.size97, align 4
  %ADD102 = add i32 %"$format.size101", 1
  %AS103 = sext i32 %ADD102 to i64
  %call.calloc105 = call i8* @calloc(i64 %AS103, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc105, i8** %format.text104, align 8
  %"$format.text106" = load i8*, i8** %format.text104, align 8
  %"$bool.text107" = load i8*, i8** %bool.text98, align 8
  %call.snprintf108 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text106", i64 %AS103, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.22, i32 0, i32 0), i8* %"$bool.text107")
  %"$format.size109" = load i32, i32* %format.size97, align 4
  %ADD110 = add i32 %"$format.size109", 1
  %"$format.text111" = load i8*, i8** %format.text104, align 8
  %"$format.size112" = load i32, i32* %format.size97, align 4
  %18 = insertvalue %String zeroinitializer, i8* %"$format.text111", 0
  %19 = insertvalue %String %18, i32 %"$format.size112", 1
  %20 = insertvalue %String %19, i32 %ADD110, 2
  call void @"io::out"(%String %20)
  %"call.String.!=113" = call i1 @"String.!="(%String* %x, %String* %y)
  store i32 0, i32* %format.size114, align 4
  store i8* null, i8** %bool.text115, align 8
  br label %if21

if21:                                             ; preds = %endif2
  br i1 %"call.String.!=113", label %if21.then, label %else22

if21.then:                                        ; preds = %if21
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.23, i32 0, i32 0), i8** %bool.text115, align 8
  br label %endif3

else22:                                           ; preds = %if21
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.24, i32 0, i32 0), i8** %bool.text115, align 8
  br label %endif3

endif3:                                           ; preds = %else22, %if21.then
  %"$bool.text116" = load i8*, i8** %bool.text115, align 8
  %call.snprintf117 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.25, i32 0, i32 0), i8* %"$bool.text116")
  store i32 %call.snprintf117, i32* %format.size114, align 4
  %"$format.size118" = load i32, i32* %format.size114, align 4
  %ADD119 = add i32 %"$format.size118", 1
  %AS120 = sext i32 %ADD119 to i64
  %call.calloc122 = call i8* @calloc(i64 %AS120, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc122, i8** %format.text121, align 8
  %"$format.text123" = load i8*, i8** %format.text121, align 8
  %"$bool.text124" = load i8*, i8** %bool.text115, align 8
  %call.snprintf125 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text123", i64 %AS120, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.25, i32 0, i32 0), i8* %"$bool.text124")
  %"$format.size126" = load i32, i32* %format.size114, align 4
  %ADD127 = add i32 %"$format.size126", 1
  %"$format.text128" = load i8*, i8** %format.text121, align 8
  %"$format.size129" = load i32, i32* %format.size114, align 4
  %21 = insertvalue %String zeroinitializer, i8* %"$format.text128", 0
  %22 = insertvalue %String %21, i32 %"$format.size129", 1
  %23 = insertvalue %String %22, i32 %ADD127, 2
  call void @"io::out"(%String %23)
  %"call.String.<" = call i1 @"String.<"(%String* %x, %String* %z)
  store i32 0, i32* %format.size130, align 4
  store i8* null, i8** %bool.text131, align 8
  br label %if23

if23:                                             ; preds = %endif3
  br i1 %"call.String.<", label %if23.then, label %else24

if23.then:                                        ; preds = %if23
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.26, i32 0, i32 0), i8** %bool.text131, align 8
  br label %endif4

else24:                                           ; preds = %if23
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.27, i32 0, i32 0), i8** %bool.text131, align 8
  br label %endif4

endif4:                                           ; preds = %else24, %if23.then
  %"$bool.text132" = load i8*, i8** %bool.text131, align 8
  %call.snprintf133 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.28, i32 0, i32 0), i8* %"$bool.text132")
  store i32 %call.snprintf133, i32* %format.size130, align 4
  %"$format.size134" = load i32, i32* %format.size130, align 4
  %ADD135 = add i32 %"$format.size134", 1
  %AS136 = sext i32 %ADD135 to i64
  %call.calloc138 = call i8* @calloc(i64 %AS136, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc138, i8** %format.text137, align 8
  %"$format.text139" = load i8*, i8** %format.text137, align 8
  %"$bool.text140" = load i8*, i8** %bool.text131, align 8
  %call.snprintf141 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text139", i64 %AS136, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.28, i32 0, i32 0), i8* %"$bool.text140")
  %"$format.size142" = load i32, i32* %format.size130, align 4
  %ADD143 = add i32 %"$format.size142", 1
  %"$format.text144" = load i8*, i8** %format.text137, align 8
  %"$format.size145" = load i32, i32* %format.size130, align 4
  %24 = insertvalue %String zeroinitializer, i8* %"$format.text144", 0
  %25 = insertvalue %String %24, i32 %"$format.size145", 1
  %26 = insertvalue %String %25, i32 %ADD143, 2
  call void @"io::out"(%String %26)
  %"call.String.<146" = call i1 @"String.<"(%String* %z, %String* %x)
  store i32 0, i32* %format.size147, align 4
  store i8* null, i8** %bool.text148, align 8
  br label %if25

if25:                                             ; preds = %endif4
  br i1 %"call.String.<146", label %if25.then, label %else26

if25.then:                                        ; preds = %if25
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.29, i32 0, i32 0), i8** %bool.text148, align 8
  br label %endif5

else26:                                           ; preds = %if25
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.30, i32 0, i32 0), i8** %bool.text148, align 8
  br label %endif5

endif5:                                           ; preds = %else26, %if25.then
  %"$bool.text149" = load i8*, i8** %bool.text148, align 8
  %call.snprintf150 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.31, i32 0, i32 0), i8* %"$bool.text149")
  store i32 %call.snprintf150, i32* %format.size147, align 4
  %"$format.size151" = load i32, i32* %format.size147, align 4
  %ADD152 = add i32 %"$format.size151", 1
  %AS153 = sext i32 %ADD152 to i64
  %call.calloc155 = call i8* @calloc(i64 %AS153, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc155, i8** %format.text154, align 8
  %"$format.text156" = load i8*, i8** %format.text154, align 8
  %"$bool.text157" = load i8*, i8** %bool.text148, align 8
  %call.snprintf158 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text156", i64 %AS153, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.31, i32 0, i32 0), i8* %"$bool.text157")
  %"$format.size159" = load i32, i32* %format.size147, align 4
  %ADD160 = add i32 %"$format.size159", 1
  %"$format.text161" = load i8*, i8** %format.text154, align 8
  %"$format.size162" = load i32, i32* %format.size147, align 4
  %27 = insertvalue %String zeroinitializer, i8* %"$format.text161", 0
  %28 = insertvalue %String %27, i32 %"$format.size162", 1
  %29 = insertvalue %String %28, i32 %ADD160, 2
  call void @"io::out"(%String %29)
  %"call.String.>" = call i1 @"String.>"(%String* %z, %String* %x)
  store i32 0, i32* %format.size163, align 4
  store i8* null, i8** %bool.text164, align 8
  br label %if27

if27:                                             ; preds = %endif5
  br i1 %"call.String.>", label %if27.then, label %else28

if27.then:                                        ; preds = %if27
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.32, i32 0, i32 0), i8** %bool.text164, align 8
  br label %endif6

else28:                                           ; preds = %if27
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.33, i32 0, i32 0), i8** %bool.text164, align 8
  br label %endif6

endif6:                                           ; preds = %else28, %if27.then
  %"$bool.text165" = load i8*, i8** %bool.text164, align 8
  %call.snprintf166 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.34, i32 0, i32 0), i8* %"$bool.text165")
  store i32 %call.snprintf166, i32* %format.size163, align 4
  %"$format.size167" = load i32, i32* %format.size163, align 4
  %ADD168 = add i32 %"$format.size167", 1
  %AS169 = sext i32 %ADD168 to i64
  %call.calloc171 = call i8* @calloc(i64 %AS169, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc171, i8** %format.text170, align 8
  %"$format.text172" = load i8*, i8** %format.text170, align 8
  %"$bool.text173" = load i8*, i8** %bool.text164, align 8
  %call.snprintf174 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text172", i64 %AS169, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.34, i32 0, i32 0), i8* %"$bool.text173")
  %"$format.size175" = load i32, i32* %format.size163, align 4
  %ADD176 = add i32 %"$format.size175", 1
  %"$format.text177" = load i8*, i8** %format.text170, align 8
  %"$format.size178" = load i32, i32* %format.size163, align 4
  %30 = insertvalue %String zeroinitializer, i8* %"$format.text177", 0
  %31 = insertvalue %String %30, i32 %"$format.size178", 1
  %32 = insertvalue %String %31, i32 %ADD176, 2
  call void @"io::out"(%String %32)
  %"call.String.>179" = call i1 @"String.>"(%String* %x, %String* %z)
  store i32 0, i32* %format.size180, align 4
  store i8* null, i8** %bool.text181, align 8
  br label %if29

if29:                                             ; preds = %endif6
  br i1 %"call.String.>179", label %if29.then, label %else30

if29.then:                                        ; preds = %if29
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.35, i32 0, i32 0), i8** %bool.text181, align 8
  br label %endif7

else30:                                           ; preds = %if29
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.36, i32 0, i32 0), i8** %bool.text181, align 8
  br label %endif7

endif7:                                           ; preds = %else30, %if29.then
  %"$bool.text182" = load i8*, i8** %bool.text181, align 8
  %call.snprintf183 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.37, i32 0, i32 0), i8* %"$bool.text182")
  store i32 %call.snprintf183, i32* %format.size180, align 4
  %"$format.size184" = load i32, i32* %format.size180, align 4
  %ADD185 = add i32 %"$format.size184", 1
  %AS186 = sext i32 %ADD185 to i64
  %call.calloc188 = call i8* @calloc(i64 %AS186, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc188, i8** %format.text187, align 8
  %"$format.text189" = load i8*, i8** %format.text187, align 8
  %"$bool.text190" = load i8*, i8** %bool.text181, align 8
  %call.snprintf191 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text189", i64 %AS186, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.37, i32 0, i32 0), i8* %"$bool.text190")
  %"$format.size192" = load i32, i32* %format.size180, align 4
  %ADD193 = add i32 %"$format.size192", 1
  %"$format.text194" = load i8*, i8** %format.text187, align 8
  %"$format.size195" = load i32, i32* %format.size180, align 4
  %33 = insertvalue %String zeroinitializer, i8* %"$format.text194", 0
  %34 = insertvalue %String %33, i32 %"$format.size195", 1
  %35 = insertvalue %String %34, i32 %ADD193, 2
  call void @"io::out"(%String %35)
  store %String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.38, i32 0, i32 0), i32 3, i32 0 }, %String* %tmp, align 8
  %"call.String.==196" = call i1 @"String.=="(%String* %x, %String* %tmp)
  store i32 0, i32* %format.size197, align 4
  store i8* null, i8** %bool.text198, align 8
  br label %if31

if31:                                             ; preds = %endif7
  br i1 %"call.String.==196", label %if31.then, label %else32

if31.then:                                        ; preds = %if31
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.39, i32 0, i32 0), i8** %bool.text198, align 8
  br label %endif8

else32:                                           ; preds = %if31
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.40, i32 0, i32 0), i8** %bool.text198, align 8
  br label %endif8

endif8:                                           ; preds = %else32, %if31.then
  %"$bool.text199" = load i8*, i8** %bool.text198, align 8
  %call.snprintf200 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.41, i32 0, i32 0), i8* %"$bool.text199")
  store i32 %call.snprintf200, i32* %format.size197, align 4
  %"$format.size201" = load i32, i32* %format.size197, align 4
  %ADD202 = add i32 %"$format.size201", 1
  %AS203 = sext i32 %ADD202 to i64
  %call.calloc205 = call i8* @calloc(i64 %AS203, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc205, i8** %format.text204, align 8
  %"$format.text206" = load i8*, i8** %format.text204, align 8
  %"$bool.text207" = load i8*, i8** %bool.text198, align 8
  %call.snprintf208 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text206", i64 %AS203, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.41, i32 0, i32 0), i8* %"$bool.text207")
  %"$format.size209" = load i32, i32* %format.size197, align 4
  %ADD210 = add i32 %"$format.size209", 1
  %"$format.text211" = load i8*, i8** %format.text204, align 8
  %"$format.size212" = load i32, i32* %format.size197, align 4
  %36 = insertvalue %String zeroinitializer, i8* %"$format.text211", 0
  %37 = insertvalue %String %36, i32 %"$format.size212", 1
  %38 = insertvalue %String %37, i32 %ADD210, 2
  call void @"io::out"(%String %38)
  %39 = bitcast %String* %tmp to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %39)
  store %String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.42, i32 0, i32 0), i32 3, i32 0 }, %String* %tmp213, align 8
  %"call.String.==214" = call i1 @"String.=="(%String* %x, %String* %tmp213)
  store i32 0, i32* %format.size215, align 4
  store i8* null, i8** %bool.text216, align 8
  br label %if33

if33:                                             ; preds = %endif8
  br i1 %"call.String.==214", label %if33.then, label %else34

if33.then:                                        ; preds = %if33
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.43, i32 0, i32 0), i8** %bool.text216, align 8
  br label %endif9

else34:                                           ; preds = %if33
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.44, i32 0, i32 0), i8** %bool.text216, align 8
  br label %endif9

endif9:                                           ; preds = %else34, %if33.then
  %"$bool.text217" = load i8*, i8** %bool.text216, align 8
  %call.snprintf218 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.45, i32 0, i32 0), i8* %"$bool.text217")
  store i32 %call.snprintf218, i32* %format.size215, align 4
  %"$format.size219" = load i32, i32* %format.size215, align 4
  %ADD220 = add i32 %"$format.size219", 1
  %AS221 = sext i32 %ADD220 to i64
  %call.calloc223 = call i8* @calloc(i64 %AS221, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc223, i8** %format.text222, align 8
  %"$format.text224" = load i8*, i8** %format.text222, align 8
  %"$bool.text225" = load i8*, i8** %bool.text216, align 8
  %call.snprintf226 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text224", i64 %AS221, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.45, i32 0, i32 0), i8* %"$bool.text225")
  %"$format.size227" = load i32, i32* %format.size215, align 4
  %ADD228 = add i32 %"$format.size227", 1
  %"$format.text229" = load i8*, i8** %format.text222, align 8
  %"$format.size230" = load i32, i32* %format.size215, align 4
  %40 = insertvalue %String zeroinitializer, i8* %"$format.text229", 0
  %41 = insertvalue %String %40, i32 %"$format.size230", 1
  %42 = insertvalue %String %41, i32 %ADD228, 2
  call void @"io::out"(%String %42)
  %43 = bitcast %String* %tmp213 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %43)
  store %String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.46, i32 0, i32 0), i32 3, i32 0 }, %String* %tmp231, align 8
  %"call.String.!=232" = call i1 @"String.!="(%String* %x, %String* %tmp231)
  store i32 0, i32* %format.size233, align 4
  store i8* null, i8** %bool.text234, align 8
  br label %if35

if35:                                             ; preds = %endif9
  br i1 %"call.String.!=232", label %if35.then, label %else36

if35.then:                                        ; preds = %if35
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.47, i32 0, i32 0), i8** %bool.text234, align 8
  br label %endif10

else36:                                           ; preds = %if35
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.48, i32 0, i32 0), i8** %bool.text234, align 8
  br label %endif10

endif10:                                          ; preds = %else36, %if35.then
  %"$bool.text235" = load i8*, i8** %bool.text234, align 8
  %call.snprintf236 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.49, i32 0, i32 0), i8* %"$bool.text235")
  store i32 %call.snprintf236, i32* %format.size233, align 4
  %"$format.size237" = load i32, i32* %format.size233, align 4
  %ADD238 = add i32 %"$format.size237", 1
  %AS239 = sext i32 %ADD238 to i64
  %call.calloc241 = call i8* @calloc(i64 %AS239, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc241, i8** %format.text240, align 8
  %"$format.text242" = load i8*, i8** %format.text240, align 8
  %"$bool.text243" = load i8*, i8** %bool.text234, align 8
  %call.snprintf244 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text242", i64 %AS239, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.49, i32 0, i32 0), i8* %"$bool.text243")
  %"$format.size245" = load i32, i32* %format.size233, align 4
  %ADD246 = add i32 %"$format.size245", 1
  %"$format.text247" = load i8*, i8** %format.text240, align 8
  %"$format.size248" = load i32, i32* %format.size233, align 4
  %44 = insertvalue %String zeroinitializer, i8* %"$format.text247", 0
  %45 = insertvalue %String %44, i32 %"$format.size248", 1
  %46 = insertvalue %String %45, i32 %ADD246, 2
  call void @"io::out"(%String %46)
  %47 = bitcast %String* %tmp231 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %47)
  store %String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.50, i32 0, i32 0), i32 3, i32 0 }, %String* %tmp249, align 8
  %"call.String.!=250" = call i1 @"String.!="(%String* %x, %String* %tmp249)
  store i32 0, i32* %format.size251, align 4
  store i8* null, i8** %bool.text252, align 8
  br label %if37

if37:                                             ; preds = %endif10
  br i1 %"call.String.!=250", label %if37.then, label %else38

if37.then:                                        ; preds = %if37
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.51, i32 0, i32 0), i8** %bool.text252, align 8
  br label %endif11

else38:                                           ; preds = %if37
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.52, i32 0, i32 0), i8** %bool.text252, align 8
  br label %endif11

endif11:                                          ; preds = %else38, %if37.then
  %"$bool.text253" = load i8*, i8** %bool.text252, align 8
  %call.snprintf254 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.53, i32 0, i32 0), i8* %"$bool.text253")
  store i32 %call.snprintf254, i32* %format.size251, align 4
  %"$format.size255" = load i32, i32* %format.size251, align 4
  %ADD256 = add i32 %"$format.size255", 1
  %AS257 = sext i32 %ADD256 to i64
  %call.calloc259 = call i8* @calloc(i64 %AS257, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc259, i8** %format.text258, align 8
  %"$format.text260" = load i8*, i8** %format.text258, align 8
  %"$bool.text261" = load i8*, i8** %bool.text252, align 8
  %call.snprintf262 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text260", i64 %AS257, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.53, i32 0, i32 0), i8* %"$bool.text261")
  %"$format.size263" = load i32, i32* %format.size251, align 4
  %ADD264 = add i32 %"$format.size263", 1
  %"$format.text265" = load i8*, i8** %format.text258, align 8
  %"$format.size266" = load i32, i32* %format.size251, align 4
  %48 = insertvalue %String zeroinitializer, i8* %"$format.text265", 0
  %49 = insertvalue %String %48, i32 %"$format.size266", 1
  %50 = insertvalue %String %49, i32 %ADD264, 2
  call void @"io::out"(%String %50)
  %51 = bitcast %String* %tmp249 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %51)
  %call.String.create267 = call %String @String.create(i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.54, i32 0, i32 0))
  store %String %call.String.create267, %String* %f, align 8
  %call.String.create268 = call %String @String.create(i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.55, i32 0, i32 0))
  store %String %call.String.create268, %String* %g, align 8
  store %String zeroinitializer, %String* %copy, align 8
  %52 = bitcast %String* %copy to %"Array<i8>"*
  %53 = bitcast %String* %y to %"Array<i8>"*
  call void @"Array<i8>.="(%"Array<i8>"* %52, %"Array<i8>"* %53)
  %"$copy" = load %String, %String* %copy, align 8
  %54 = bitcast %String* %f to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %54)
  store %String %"$copy", %String* %f, align 8
  %call.String.chars269 = call i8* @String.chars(%String* %f)
  %"call.String.==270" = call i1 @"String.=="(%String* %f, %String* %y)
  store i32 0, i32* %format.size271, align 4
  store i8* null, i8** %bool.text272, align 8
  br label %if39

if39:                                             ; preds = %endif11
  br i1 %"call.String.==270", label %if39.then, label %else40

if39.then:                                        ; preds = %if39
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.56, i32 0, i32 0), i8** %bool.text272, align 8
  br label %endif12

else40:                                           ; preds = %if39
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.57, i32 0, i32 0), i8** %bool.text272, align 8
  br label %endif12

endif12:                                          ; preds = %else40, %if39.then
  %"$bool.text273" = load i8*, i8** %bool.text272, align 8
  %call.snprintf274 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.58, i32 0, i32 0), i8* %call.String.chars269, i8* %"$bool.text273")
  store i32 %call.snprintf274, i32* %format.size271, align 4
  %"$format.size275" = load i32, i32* %format.size271, align 4
  %ADD276 = add i32 %"$format.size275", 1
  %AS277 = sext i32 %ADD276 to i64
  %call.calloc279 = call i8* @calloc(i64 %AS277, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc279, i8** %format.text278, align 8
  %"$format.text280" = load i8*, i8** %format.text278, align 8
  %"$bool.text281" = load i8*, i8** %bool.text272, align 8
  %call.snprintf282 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text280", i64 %AS277, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.58, i32 0, i32 0), i8* %call.String.chars269, i8* %"$bool.text281")
  %"$format.size283" = load i32, i32* %format.size271, align 4
  %ADD284 = add i32 %"$format.size283", 1
  %"$format.text285" = load i8*, i8** %format.text278, align 8
  %"$format.size286" = load i32, i32* %format.size271, align 4
  %55 = insertvalue %String zeroinitializer, i8* %"$format.text285", 0
  %56 = insertvalue %String %55, i32 %"$format.size286", 1
  %57 = insertvalue %String %56, i32 %ADD284, 2
  call void @"io::out"(%String %57)
  %58 = bitcast %String* %g to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %58)
  %59 = bitcast %String* %f to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %59)
  %60 = bitcast %String* %z to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %60)
  %61 = bitcast %String* %y to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %61)
  %62 = bitcast %String* %x to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %62)
  %63 = bitcast %String* %e to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %63)
  %64 = bitcast %String* %d to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %64)
  %65 = bitcast %String* %c to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %65)
  %66 = bitcast %String* %b to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %66)
  %67 = bitcast %String* %a to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %67)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

---

## 021 — string: to_int, compare and chained transformations

```ura
// stdout: 12345
// stdout: -99
// stdout: 0
// stdout: -1
// stdout: 1
// stdout: 0
// stdout: DLROW OLLEH
// stdout: 24
// stdout: 1000000
// stdout: -2000000
// expect: 0
use "@/core"
use "@/posix"

fn main() i32:
    s1 String = String::create("12345")
    io::out($"{s1.to_int()}\n")

    s2 String = String::create("-99")
    io::out($"{s2.to_int()}\n")

    s3 String = String::create("0")
    io::out($"{s3.to_int()}\n")

    a String = String::create("apple")
    b String = String::create("banana")
    io::out($"{a.compare(&b)}\n")
    io::out($"{b.compare(&a)}\n")
    io::out($"{a.compare(&a)}\n")

    s String = String::create("Hello World")
    r String = s.upper()
    r2 String = r.reverse()
    io::out($"{r2}\n")

    big String = String::create("aaaa")
    i i32 = 0
    while i < 5:
        big.join("bbbb")
        i += 1
    io::out($"{big.size()}\n")

    n1 String = String::create(1000000)
    io::out($"{n1}\n")

    n2 String = String::create(-2000000)
    io::out($"{n2}\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%"Array<i8>" = type { i8*, i32, i32 }
%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [6 x i8] c"12345\00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.3 = private unnamed_addr constant [4 x i8] c"-99\00", align 1
@str.4 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"0\00", align 1
@str.6 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.7 = private unnamed_addr constant [6 x i8] c"apple\00", align 1
@str.8 = private unnamed_addr constant [7 x i8] c"banana\00", align 1
@str.9 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.10 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.11 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.12 = private unnamed_addr constant [12 x i8] c"Hello World\00", align 1
@str.13 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.14 = private unnamed_addr constant [5 x i8] c"aaaa\00", align 1
@str.15 = private unnamed_addr constant [5 x i8] c"bbbb\00", align 1
@str.16 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@str.17 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.18 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

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

define void @"Array<i8>.clear"(%"Array<i8>"* %0) {
entry:
  %self = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %self, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"*self" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  %"*self1" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %while1

while1:                                           ; preds = %while1.then, %endif
  %"*self2" = load %"Array<i8>"*, %"Array<i8>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while1.then, label %endloop

while1.then:                                      ; preds = %while1
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
  br label %while1

endloop:                                          ; preds = %while1
  ret void
}

define void @"Array<i8>.drop"(%"Array<i8>"* %0) {
entry:
  %value = alloca %"Array<i8>"*, align 8
  store %"Array<i8>"* %0, %"Array<i8>"** %value, align 8
  %"*value" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  call void @"Array<i8>.clear"(%"Array<i8>"* %"*value")
  br label %if2

if2:                                              ; preds = %entry
  %"*value1" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if2.then, label %endif

if2.then:                                         ; preds = %if2
  %"*value2" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i8*, i8** %"*value.data", align 8
  call void @"free<i8>"(i8* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if2.then, %if2
  ret void
}

define void @String.reserve(%String* %0, i32 %1) {
entry:
  %self = alloca %String*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca i8*, align 8
  store %String* %0, %String** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if3

if3:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if3.then, label %endif

if3.then:                                         ; preds = %if3
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if3.then, %if3
  br label %if4

if4:                                              ; preds = %endif
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if4.then, label %endif1

if4.then:                                         ; preds = %if4
  ret void

endif1:                                           ; preds = %if4
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
  br label %if5

if5:                                              ; preds = %endif1
  %"*self13" = load %String*, %String** %self, align 8
  %"*self.cap14" = getelementptr inbounds %String, %String* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if5.then, label %endif2

if5.then:                                         ; preds = %if5
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.data17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 0
  %"$*self.data18" = load i8*, i8** %"*self.data17", align 8
  call void @"free<i8>"(i8* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if5.then, %if5
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

define void @String.append(%String* %0, i8 %1) {
entry:
  %self = alloca %String*, align 8
  %value = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store i8 %1, i8* %value, align 1
  br label %if6

if6:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*self2" = load %String*, %String** %self, align 8
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @String.reserve(%String* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if6.then, %if6
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

define %String @String.create.1(i32 %0) {
entry:
  %number = alloca i32, align 4
  %res = alloca %String, align 8
  %n = alloca i32, align 4
  store i32 %0, i32* %number, align 4
  store %String zeroinitializer, %String* %res, align 8
  %"$number" = load i32, i32* %number, align 4
  store i32 %"$number", i32* %n, align 4
  br label %if7

if7:                                              ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %LT = icmp slt i32 %"$n", 0
  br i1 %LT, label %if7.then, label %endif

if7.then:                                         ; preds = %if7
  %"$n3" = load i32, i32* %n, align 4
  %SUB = sub i32 0, %"$n3"
  store i32 %SUB, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %if7.then, %if7
  br label %while8

while8:                                           ; preds = %endif1, %endif
  br i1 true, label %while8.then, label %endloop

while8.then:                                      ; preds = %while8
  %"$n4" = load i32, i32* %n, align 4
  %MOD = srem i32 %"$n4", 10
  %ADD = add i32 %MOD, 48
  %AS = trunc i32 %ADD to i8
  call void @String.append(%String* %res, i8 %AS)
  %"$n5" = load i32, i32* %n, align 4
  %DIV = sdiv i32 %"$n5", 10
  store i32 %DIV, i32* %n, align 4
  br label %if9

if9:                                              ; preds = %while8.then
  %"$n6" = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %"$n6", 0
  br i1 %EQ, label %if9.then, label %endif1

if9.then:                                         ; preds = %if9
  br label %endloop

endif1:                                           ; preds = %if9
  br label %while8

endloop:                                          ; preds = %if9.then, %while8
  br label %if10

if10:                                             ; preds = %endloop
  %"$number7" = load i32, i32* %number, align 4
  %LT8 = icmp slt i32 %"$number7", 0
  br i1 %LT8, label %if10.then, label %endif2

if10.then:                                        ; preds = %if10
  call void @String.append(%String* %res, i8 45)
  br label %endif2

endif2:                                           ; preds = %if10.then, %if10
  %call.String.reverse = call %String @String.reverse(%String* %res)
  %1 = bitcast %String* %res to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret %String %call.String.reverse
}

define void @String.join(%String* %0, i8* %1) {
entry:
  %self = alloca %String*, align 8
  %text = alloca i8*, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store i8* %1, i8** %text, align 8
  store i32 0, i32* %i, align 4
  br label %while11

while11:                                          ; preds = %while11.then, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$i" = load i32, i32* %i, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$i"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while11.then, label %endloop

while11.then:                                     ; preds = %while11
  %"*self" = load %String*, %String** %self, align 8
  %"$text1" = load i8*, i8** %text, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"text[]3" = getelementptr inbounds i8, i8* %"$text1", i32 %"$i2"
  %"$text[]4" = load i8, i8* %"text[]3", align 1
  call void @String.append(%String* %"*self", i8 %"$text[]4")
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while11

endloop:                                          ; preds = %while11
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
  br label %if12

if12:                                             ; preds = %entry
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
  br i1 %AND, label %if12.then, label %endif

if12.then:                                        ; preds = %if12
  store i32 -1, i32* %sign, align 4
  store i32 1, i32* %i, align 4
  br label %endif

endif:                                            ; preds = %if12.then, %if12
  store i32 0, i32* %n, align 4
  br label %while13

while13:                                          ; preds = %endif1, %endif
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count5"
  br i1 %LT, label %while13.then, label %endloop

while13.then:                                     ; preds = %while13
  %"*self6" = load %String*, %String** %self, align 8
  %"*self.data7" = getelementptr inbounds %String, %String* %"*self6", i32 0, i32 0
  %"$*self.data8" = load i8*, i8** %"*self.data7", align 8
  %"$i9" = load i32, i32* %i, align 4
  %"*self.data[]10" = getelementptr inbounds i8, i8* %"$*self.data8", i32 %"$i9"
  %"$*self.data[]11" = load i8, i8* %"*self.data[]10", align 1
  store i8 %"$*self.data[]11", i8* %c, align 1
  br label %if14

if14:                                             ; preds = %while13.then
  %"$c" = load i8, i8* %c, align 1
  %LT12 = icmp slt i8 %"$c", 48
  %"$c13" = load i8, i8* %c, align 1
  %GT14 = icmp sgt i8 %"$c13", 57
  %OR = or i1 %LT12, %GT14
  br i1 %OR, label %if14.then, label %endif1

if14.then:                                        ; preds = %if14
  br label %endloop

endif1:                                           ; preds = %if14
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
  br label %while13

endloop:                                          ; preds = %if14.then, %while13
  %"$n18" = load i32, i32* %n, align 4
  %"$sign" = load i32, i32* %sign, align 4
  %MUL19 = mul i32 %"$n18", %"$sign"
  ret i32 %MUL19
}

define i32 @String.compare(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  %other = alloca %String*, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store %String* %1, %String** %other, align 8
  store i32 0, i32* %i, align 4
  br label %while15

while15:                                          ; preds = %endif, %entry
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
  br i1 %AND, label %while15.then, label %endloop

while15.then:                                     ; preds = %while15
  br label %if16

if16:                                             ; preds = %while15.then
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
  br i1 %NQ, label %if16.then, label %endif

if16.then:                                        ; preds = %if16
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

endif:                                            ; preds = %if16
  %"$i20" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i20", 1
  store i32 %ADD, i32* %i, align 4
  br label %while15

endloop:                                          ; preds = %while15
  %"*self21" = load %String*, %String** %self, align 8
  %"*self.count22" = getelementptr inbounds %String, %String* %"*self21", i32 0, i32 1
  %"*other23" = load %String*, %String** %other, align 8
  %"*other.count24" = getelementptr inbounds %String, %String* %"*other23", i32 0, i32 1
  %"$*self.count25" = load i32, i32* %"*self.count22", align 4
  %"$*other.count26" = load i32, i32* %"*other.count24", align 4
  %SUB27 = sub i32 %"$*self.count25", %"$*other.count26"
  ret i32 %SUB27
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
  br label %while17

while17:                                          ; preds = %endif, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$i", %"$*self.count"
  br i1 %LT, label %while17.then, label %endloop

while17.then:                                     ; preds = %while17
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i2" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i2"
  %"$*self.data[]" = load i8, i8* %"*self.data[]", align 1
  store i8 %"$*self.data[]", i8* %c, align 1
  br label %if18

if18:                                             ; preds = %while17.then
  %"$c" = load i8, i8* %c, align 1
  %GE = icmp sge i8 %"$c", 97
  %"$c3" = load i8, i8* %c, align 1
  %LE = icmp sle i8 %"$c3", 122
  %AND = and i1 %GE, %LE
  br i1 %AND, label %if18.then, label %endif

if18.then:                                        ; preds = %if18
  %"$c4" = load i8, i8* %c, align 1
  %AS = sext i8 %"$c4" to i32
  %SUB = sub i32 %AS, 32
  %AS5 = trunc i32 %SUB to i8
  store i8 %AS5, i8* %c, align 1
  br label %endif

endif:                                            ; preds = %if18.then, %if18
  %"$c6" = load i8, i8* %c, align 1
  call void @String.append(%String* %res, i8 %"$c6")
  %"$i7" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i7", 1
  store i32 %ADD, i32* %i, align 4
  br label %while17

endloop:                                          ; preds = %while17
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
  br label %while19

while19:                                          ; preds = %while19.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %GE = icmp sge i32 %"$i", 0
  br i1 %GE, label %while19.then, label %endloop

while19.then:                                     ; preds = %while19
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
  br label %while19

endloop:                                          ; preds = %while19
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
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i32 @main() {
entry:
  %s1 = alloca %String, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %s2 = alloca %String, align 8
  %format.size8 = alloca i32, align 4
  %format.text13 = alloca i8*, align 8
  %s3 = alloca %String, align 8
  %format.size23 = alloca i32, align 4
  %format.text28 = alloca i8*, align 8
  %a = alloca %String, align 8
  %b = alloca %String, align 8
  %format.size38 = alloca i32, align 4
  %format.text43 = alloca i8*, align 8
  %format.size52 = alloca i32, align 4
  %format.text57 = alloca i8*, align 8
  %format.size66 = alloca i32, align 4
  %format.text71 = alloca i8*, align 8
  %s = alloca %String, align 8
  %r = alloca %String, align 8
  %r2 = alloca %String, align 8
  %format.size80 = alloca i32, align 4
  %format.text85 = alloca i8*, align 8
  %big = alloca %String, align 8
  %i = alloca i32, align 4
  %format.size96 = alloca i32, align 4
  %format.text101 = alloca i8*, align 8
  %n1 = alloca %String, align 8
  %format.size111 = alloca i32, align 4
  %format.text116 = alloca i8*, align 8
  %n2 = alloca %String, align 8
  %format.size126 = alloca i32, align 4
  %format.text131 = alloca i8*, align 8
  %call.String.create = call %String @String.create(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0))
  store %String %call.String.create, %String* %s1, align 8
  %call.String.to_int = call i32 @String.to_int(%String* %s1)
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 %call.String.to_int)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 %call.String.to_int)
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  call void @"io::out"(%String %2)
  %call.String.create6 = call %String @String.create(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.3, i32 0, i32 0))
  store %String %call.String.create6, %String* %s2, align 8
  %call.String.to_int7 = call i32 @String.to_int(%String* %s2)
  %call.snprintf9 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i32 %call.String.to_int7)
  store i32 %call.snprintf9, i32* %format.size8, align 4
  %"$format.size10" = load i32, i32* %format.size8, align 4
  %ADD11 = add i32 %"$format.size10", 1
  %AS12 = sext i32 %ADD11 to i64
  %call.calloc14 = call i8* @calloc(i64 %AS12, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc14, i8** %format.text13, align 8
  %"$format.text15" = load i8*, i8** %format.text13, align 8
  %call.snprintf16 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text15", i64 %AS12, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i32 %call.String.to_int7)
  %"$format.size17" = load i32, i32* %format.size8, align 4
  %ADD18 = add i32 %"$format.size17", 1
  %"$format.text19" = load i8*, i8** %format.text13, align 8
  %"$format.size20" = load i32, i32* %format.size8, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text19", 0
  %4 = insertvalue %String %3, i32 %"$format.size20", 1
  %5 = insertvalue %String %4, i32 %ADD18, 2
  call void @"io::out"(%String %5)
  %call.String.create21 = call %String @String.create(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  store %String %call.String.create21, %String* %s3, align 8
  %call.String.to_int22 = call i32 @String.to_int(%String* %s3)
  %call.snprintf24 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.6, i32 0, i32 0), i32 %call.String.to_int22)
  store i32 %call.snprintf24, i32* %format.size23, align 4
  %"$format.size25" = load i32, i32* %format.size23, align 4
  %ADD26 = add i32 %"$format.size25", 1
  %AS27 = sext i32 %ADD26 to i64
  %call.calloc29 = call i8* @calloc(i64 %AS27, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc29, i8** %format.text28, align 8
  %"$format.text30" = load i8*, i8** %format.text28, align 8
  %call.snprintf31 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text30", i64 %AS27, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.6, i32 0, i32 0), i32 %call.String.to_int22)
  %"$format.size32" = load i32, i32* %format.size23, align 4
  %ADD33 = add i32 %"$format.size32", 1
  %"$format.text34" = load i8*, i8** %format.text28, align 8
  %"$format.size35" = load i32, i32* %format.size23, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text34", 0
  %7 = insertvalue %String %6, i32 %"$format.size35", 1
  %8 = insertvalue %String %7, i32 %ADD33, 2
  call void @"io::out"(%String %8)
  %call.String.create36 = call %String @String.create(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.7, i32 0, i32 0))
  store %String %call.String.create36, %String* %a, align 8
  %call.String.create37 = call %String @String.create(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.8, i32 0, i32 0))
  store %String %call.String.create37, %String* %b, align 8
  %call.String.compare = call i32 @String.compare(%String* %a, %String* %b)
  %call.snprintf39 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.9, i32 0, i32 0), i32 %call.String.compare)
  store i32 %call.snprintf39, i32* %format.size38, align 4
  %"$format.size40" = load i32, i32* %format.size38, align 4
  %ADD41 = add i32 %"$format.size40", 1
  %AS42 = sext i32 %ADD41 to i64
  %call.calloc44 = call i8* @calloc(i64 %AS42, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc44, i8** %format.text43, align 8
  %"$format.text45" = load i8*, i8** %format.text43, align 8
  %call.snprintf46 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text45", i64 %AS42, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.9, i32 0, i32 0), i32 %call.String.compare)
  %"$format.size47" = load i32, i32* %format.size38, align 4
  %ADD48 = add i32 %"$format.size47", 1
  %"$format.text49" = load i8*, i8** %format.text43, align 8
  %"$format.size50" = load i32, i32* %format.size38, align 4
  %9 = insertvalue %String zeroinitializer, i8* %"$format.text49", 0
  %10 = insertvalue %String %9, i32 %"$format.size50", 1
  %11 = insertvalue %String %10, i32 %ADD48, 2
  call void @"io::out"(%String %11)
  %call.String.compare51 = call i32 @String.compare(%String* %b, %String* %a)
  %call.snprintf53 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.10, i32 0, i32 0), i32 %call.String.compare51)
  store i32 %call.snprintf53, i32* %format.size52, align 4
  %"$format.size54" = load i32, i32* %format.size52, align 4
  %ADD55 = add i32 %"$format.size54", 1
  %AS56 = sext i32 %ADD55 to i64
  %call.calloc58 = call i8* @calloc(i64 %AS56, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc58, i8** %format.text57, align 8
  %"$format.text59" = load i8*, i8** %format.text57, align 8
  %call.snprintf60 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text59", i64 %AS56, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.10, i32 0, i32 0), i32 %call.String.compare51)
  %"$format.size61" = load i32, i32* %format.size52, align 4
  %ADD62 = add i32 %"$format.size61", 1
  %"$format.text63" = load i8*, i8** %format.text57, align 8
  %"$format.size64" = load i32, i32* %format.size52, align 4
  %12 = insertvalue %String zeroinitializer, i8* %"$format.text63", 0
  %13 = insertvalue %String %12, i32 %"$format.size64", 1
  %14 = insertvalue %String %13, i32 %ADD62, 2
  call void @"io::out"(%String %14)
  %call.String.compare65 = call i32 @String.compare(%String* %a, %String* %a)
  %call.snprintf67 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.11, i32 0, i32 0), i32 %call.String.compare65)
  store i32 %call.snprintf67, i32* %format.size66, align 4
  %"$format.size68" = load i32, i32* %format.size66, align 4
  %ADD69 = add i32 %"$format.size68", 1
  %AS70 = sext i32 %ADD69 to i64
  %call.calloc72 = call i8* @calloc(i64 %AS70, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc72, i8** %format.text71, align 8
  %"$format.text73" = load i8*, i8** %format.text71, align 8
  %call.snprintf74 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text73", i64 %AS70, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.11, i32 0, i32 0), i32 %call.String.compare65)
  %"$format.size75" = load i32, i32* %format.size66, align 4
  %ADD76 = add i32 %"$format.size75", 1
  %"$format.text77" = load i8*, i8** %format.text71, align 8
  %"$format.size78" = load i32, i32* %format.size66, align 4
  %15 = insertvalue %String zeroinitializer, i8* %"$format.text77", 0
  %16 = insertvalue %String %15, i32 %"$format.size78", 1
  %17 = insertvalue %String %16, i32 %ADD76, 2
  call void @"io::out"(%String %17)
  %call.String.create79 = call %String @String.create(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.12, i32 0, i32 0))
  store %String %call.String.create79, %String* %s, align 8
  %call.String.upper = call %String @String.upper(%String* %s)
  store %String %call.String.upper, %String* %r, align 8
  %call.String.reverse = call %String @String.reverse(%String* %r)
  store %String %call.String.reverse, %String* %r2, align 8
  %call.String.chars = call i8* @String.chars(%String* %r2)
  %call.snprintf81 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.13, i32 0, i32 0), i8* %call.String.chars)
  store i32 %call.snprintf81, i32* %format.size80, align 4
  %"$format.size82" = load i32, i32* %format.size80, align 4
  %ADD83 = add i32 %"$format.size82", 1
  %AS84 = sext i32 %ADD83 to i64
  %call.calloc86 = call i8* @calloc(i64 %AS84, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc86, i8** %format.text85, align 8
  %"$format.text87" = load i8*, i8** %format.text85, align 8
  %call.snprintf88 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text87", i64 %AS84, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.13, i32 0, i32 0), i8* %call.String.chars)
  %"$format.size89" = load i32, i32* %format.size80, align 4
  %ADD90 = add i32 %"$format.size89", 1
  %"$format.text91" = load i8*, i8** %format.text85, align 8
  %"$format.size92" = load i32, i32* %format.size80, align 4
  %18 = insertvalue %String zeroinitializer, i8* %"$format.text91", 0
  %19 = insertvalue %String %18, i32 %"$format.size92", 1
  %20 = insertvalue %String %19, i32 %ADD90, 2
  call void @"io::out"(%String %20)
  %call.String.create93 = call %String @String.create(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.14, i32 0, i32 0))
  store %String %call.String.create93, %String* %big, align 8
  store i32 0, i32* %i, align 4
  br label %while20

while20:                                          ; preds = %while20.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 5
  br i1 %LT, label %while20.then, label %endloop

while20.then:                                     ; preds = %while20
  call void @String.join(%String* %big, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.15, i32 0, i32 0))
  %"$i94" = load i32, i32* %i, align 4
  %ADD95 = add i32 %"$i94", 1
  store i32 %ADD95, i32* %i, align 4
  br label %while20

endloop:                                          ; preds = %while20
  %call.String.size = call i32 @String.size(%String* %big)
  %call.snprintf97 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.16, i32 0, i32 0), i32 %call.String.size)
  store i32 %call.snprintf97, i32* %format.size96, align 4
  %"$format.size98" = load i32, i32* %format.size96, align 4
  %ADD99 = add i32 %"$format.size98", 1
  %AS100 = sext i32 %ADD99 to i64
  %call.calloc102 = call i8* @calloc(i64 %AS100, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc102, i8** %format.text101, align 8
  %"$format.text103" = load i8*, i8** %format.text101, align 8
  %call.snprintf104 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text103", i64 %AS100, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.16, i32 0, i32 0), i32 %call.String.size)
  %"$format.size105" = load i32, i32* %format.size96, align 4
  %ADD106 = add i32 %"$format.size105", 1
  %"$format.text107" = load i8*, i8** %format.text101, align 8
  %"$format.size108" = load i32, i32* %format.size96, align 4
  %21 = insertvalue %String zeroinitializer, i8* %"$format.text107", 0
  %22 = insertvalue %String %21, i32 %"$format.size108", 1
  %23 = insertvalue %String %22, i32 %ADD106, 2
  call void @"io::out"(%String %23)
  %call.String.create109 = call %String @String.create.1(i32 1000000)
  store %String %call.String.create109, %String* %n1, align 8
  %call.String.chars110 = call i8* @String.chars(%String* %n1)
  %call.snprintf112 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.17, i32 0, i32 0), i8* %call.String.chars110)
  store i32 %call.snprintf112, i32* %format.size111, align 4
  %"$format.size113" = load i32, i32* %format.size111, align 4
  %ADD114 = add i32 %"$format.size113", 1
  %AS115 = sext i32 %ADD114 to i64
  %call.calloc117 = call i8* @calloc(i64 %AS115, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc117, i8** %format.text116, align 8
  %"$format.text118" = load i8*, i8** %format.text116, align 8
  %call.snprintf119 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text118", i64 %AS115, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.17, i32 0, i32 0), i8* %call.String.chars110)
  %"$format.size120" = load i32, i32* %format.size111, align 4
  %ADD121 = add i32 %"$format.size120", 1
  %"$format.text122" = load i8*, i8** %format.text116, align 8
  %"$format.size123" = load i32, i32* %format.size111, align 4
  %24 = insertvalue %String zeroinitializer, i8* %"$format.text122", 0
  %25 = insertvalue %String %24, i32 %"$format.size123", 1
  %26 = insertvalue %String %25, i32 %ADD121, 2
  call void @"io::out"(%String %26)
  %call.String.create124 = call %String @String.create.1(i32 -2000000)
  store %String %call.String.create124, %String* %n2, align 8
  %call.String.chars125 = call i8* @String.chars(%String* %n2)
  %call.snprintf127 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.18, i32 0, i32 0), i8* %call.String.chars125)
  store i32 %call.snprintf127, i32* %format.size126, align 4
  %"$format.size128" = load i32, i32* %format.size126, align 4
  %ADD129 = add i32 %"$format.size128", 1
  %AS130 = sext i32 %ADD129 to i64
  %call.calloc132 = call i8* @calloc(i64 %AS130, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc132, i8** %format.text131, align 8
  %"$format.text133" = load i8*, i8** %format.text131, align 8
  %call.snprintf134 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text133", i64 %AS130, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.18, i32 0, i32 0), i8* %call.String.chars125)
  %"$format.size135" = load i32, i32* %format.size126, align 4
  %ADD136 = add i32 %"$format.size135", 1
  %"$format.text137" = load i8*, i8** %format.text131, align 8
  %"$format.size138" = load i32, i32* %format.size126, align 4
  %27 = insertvalue %String zeroinitializer, i8* %"$format.text137", 0
  %28 = insertvalue %String %27, i32 %"$format.size138", 1
  %29 = insertvalue %String %28, i32 %ADD136, 2
  call void @"io::out"(%String %29)
  %30 = bitcast %String* %n2 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %30)
  %31 = bitcast %String* %n1 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %31)
  %32 = bitcast %String* %big to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %32)
  %33 = bitcast %String* %r2 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %33)
  %34 = bitcast %String* %r to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %34)
  %35 = bitcast %String* %s to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %35)
  %36 = bitcast %String* %b to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %36)
  %37 = bitcast %String* %a to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %37)
  %38 = bitcast %String* %s3 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %38)
  %39 = bitcast %String* %s2 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %39)
  %40 = bitcast %String* %s1 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %40)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

