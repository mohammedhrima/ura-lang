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
use "@/header"

fn main() i32:
    s String = String::create("Hello World")

    u String = s.upper()
    output(u, "\n")

    l String = s.lower()
    output(l, "\n")

    sub String = s.substr(6, 5)
    output(sub, "\n")

    sub2 String = s.substr(0, 5)
    output(sub2, "\n")

    sub3 String = s.substr(0, 100)
    output(sub3, "\n")

    sub4 String = s.substr(-1, 5)
    output(sub4.size(), "\n")

    t1 String = String::create("   hello   ")
    t2 String = t1.trim()
    output(t2, "\n")

    t3 String = String::create("hello")
    t4 String = t3.trim()
    output(t4, "\n")

    t5 String = String::create("   ")
    t6 String = t5.trim()
    output(t6.size(), "\n")

    r1 String = String::create("foo bar foo baz")
    r2 String = r1.replace("foo", "qux")
    output(r2, "\n")

    r3 String = r1.replace("xyz", "qux")
    output(r3, "\n")

    r4 String = r1.replace("", "qux")
    output(r4, "\n")

    rep String = String::create("ab")
    rep2 String = rep.repeat(3)
    output(rep2, "\n")

    rep3 String = rep.repeat(0)
    output(rep3.size(), "\n")

    rev String = String::create("abcde")
    rev2 String = rev.reverse()
    output(rev2, "\n")

    rev3 String = String::create("a")
    rev4 String = rev3.reverse()
    output(rev4, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

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

define i32 @String.size(%String* %0) {
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
  br label %if6

if6:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if6.then, %if6
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

define i32 @main() {
entry:
  %s = alloca %String, align 8
  %u = alloca %String, align 8
  %l = alloca %String, align 8
  %sub = alloca %String, align 8
  %sub2 = alloca %String, align 8
  %sub3 = alloca %String, align 8
  %sub4 = alloca %String, align 8
  %t1 = alloca %String, align 8
  %t2 = alloca %String, align 8
  %t3 = alloca %String, align 8
  %t4 = alloca %String, align 8
  %t5 = alloca %String, align 8
  %t6 = alloca %String, align 8
  %r1 = alloca %String, align 8
  %r2 = alloca %String, align 8
  %r3 = alloca %String, align 8
  %r4 = alloca %String, align 8
  %rep = alloca %String, align 8
  %rep2 = alloca %String, align 8
  %rep3 = alloca %String, align 8
  %rev = alloca %String, align 8
  %rev2 = alloca %String, align 8
  %rev3 = alloca %String, align 8
  %rev4 = alloca %String, align 8
  %call.String.create = call %String @String.create(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0))
  store %String %call.String.create, %String* %s, align 8
  %call.String.upper = call %String @String.upper(%String* %s)
  store %String %call.String.upper, %String* %u, align 8
  %call.String.chars = call i8* @String.chars(%String* %u)
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i8* %call.String.chars)
  %call.String.lower = call %String @String.lower(%String* %s)
  store %String %call.String.lower, %String* %l, align 8
  %call.String.chars1 = call i8* @String.chars(%String* %l)
  %call.dprintf2 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i8* %call.String.chars1)
  %call.String.substr = call %String @String.substr(%String* %s, i32 6, i32 5)
  store %String %call.String.substr, %String* %sub, align 8
  %call.String.chars3 = call i8* @String.chars(%String* %sub)
  %call.dprintf4 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.3, i32 0, i32 0), i8* %call.String.chars3)
  %call.String.substr5 = call %String @String.substr(%String* %s, i32 0, i32 5)
  store %String %call.String.substr5, %String* %sub2, align 8
  %call.String.chars6 = call i8* @String.chars(%String* %sub2)
  %call.dprintf7 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i8* %call.String.chars6)
  %call.String.substr8 = call %String @String.substr(%String* %s, i32 0, i32 100)
  store %String %call.String.substr8, %String* %sub3, align 8
  %call.String.chars9 = call i8* @String.chars(%String* %sub3)
  %call.dprintf10 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.5, i32 0, i32 0), i8* %call.String.chars9)
  %call.String.substr11 = call %String @String.substr(%String* %s, i32 -1, i32 5)
  store %String %call.String.substr11, %String* %sub4, align 8
  %call.String.size = call i32 @String.size(%String* %sub4)
  %call.dprintf12 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.6, i32 0, i32 0), i32 %call.String.size)
  %call.String.create13 = call %String @String.create(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.7, i32 0, i32 0))
  store %String %call.String.create13, %String* %t1, align 8
  %call.String.trim = call %String @String.trim(%String* %t1)
  store %String %call.String.trim, %String* %t2, align 8
  %call.String.chars14 = call i8* @String.chars(%String* %t2)
  %call.dprintf15 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.8, i32 0, i32 0), i8* %call.String.chars14)
  %call.String.create16 = call %String @String.create(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.9, i32 0, i32 0))
  store %String %call.String.create16, %String* %t3, align 8
  %call.String.trim17 = call %String @String.trim(%String* %t3)
  store %String %call.String.trim17, %String* %t4, align 8
  %call.String.chars18 = call i8* @String.chars(%String* %t4)
  %call.dprintf19 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.10, i32 0, i32 0), i8* %call.String.chars18)
  %call.String.create20 = call %String @String.create(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.11, i32 0, i32 0))
  store %String %call.String.create20, %String* %t5, align 8
  %call.String.trim21 = call %String @String.trim(%String* %t5)
  store %String %call.String.trim21, %String* %t6, align 8
  %call.String.size22 = call i32 @String.size(%String* %t6)
  %call.dprintf23 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.12, i32 0, i32 0), i32 %call.String.size22)
  %call.String.create24 = call %String @String.create(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.13, i32 0, i32 0))
  store %String %call.String.create24, %String* %r1, align 8
  %call.String.replace = call %String @String.replace(%String* %r1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.14, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.15, i32 0, i32 0))
  store %String %call.String.replace, %String* %r2, align 8
  %call.String.chars25 = call i8* @String.chars(%String* %r2)
  %call.dprintf26 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.16, i32 0, i32 0), i8* %call.String.chars25)
  %call.String.replace27 = call %String @String.replace(%String* %r1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.17, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.18, i32 0, i32 0))
  store %String %call.String.replace27, %String* %r3, align 8
  %call.String.chars28 = call i8* @String.chars(%String* %r3)
  %call.dprintf29 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.19, i32 0, i32 0), i8* %call.String.chars28)
  %call.String.replace30 = call %String @String.replace(%String* %r1, i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.20, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.21, i32 0, i32 0))
  store %String %call.String.replace30, %String* %r4, align 8
  %call.String.chars31 = call i8* @String.chars(%String* %r4)
  %call.dprintf32 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.22, i32 0, i32 0), i8* %call.String.chars31)
  %call.String.create33 = call %String @String.create(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.23, i32 0, i32 0))
  store %String %call.String.create33, %String* %rep, align 8
  %call.String.repeat = call %String @String.repeat(%String* %rep, i32 3)
  store %String %call.String.repeat, %String* %rep2, align 8
  %call.String.chars34 = call i8* @String.chars(%String* %rep2)
  %call.dprintf35 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.24, i32 0, i32 0), i8* %call.String.chars34)
  %call.String.repeat36 = call %String @String.repeat(%String* %rep, i32 0)
  store %String %call.String.repeat36, %String* %rep3, align 8
  %call.String.size37 = call i32 @String.size(%String* %rep3)
  %call.dprintf38 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.25, i32 0, i32 0), i32 %call.String.size37)
  %call.String.create39 = call %String @String.create(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.26, i32 0, i32 0))
  store %String %call.String.create39, %String* %rev, align 8
  %call.String.reverse = call %String @String.reverse(%String* %rev)
  store %String %call.String.reverse, %String* %rev2, align 8
  %call.String.chars40 = call i8* @String.chars(%String* %rev2)
  %call.dprintf41 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.27, i32 0, i32 0), i8* %call.String.chars40)
  %call.String.create42 = call %String @String.create(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.28, i32 0, i32 0))
  store %String %call.String.create42, %String* %rev3, align 8
  %call.String.reverse43 = call %String @String.reverse(%String* %rev3)
  store %String %call.String.reverse43, %String* %rev4, align 8
  %call.String.chars44 = call i8* @String.chars(%String* %rev4)
  %call.dprintf45 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.29, i32 0, i32 0), i8* %call.String.chars44)
  call void @String.drop(%String* %rev4)
  call void @String.drop(%String* %rev3)
  call void @String.drop(%String* %rev2)
  call void @String.drop(%String* %rev)
  call void @String.drop(%String* %rep3)
  call void @String.drop(%String* %rep2)
  call void @String.drop(%String* %rep)
  call void @String.drop(%String* %r4)
  call void @String.drop(%String* %r3)
  call void @String.drop(%String* %r2)
  call void @String.drop(%String* %r1)
  call void @String.drop(%String* %t6)
  call void @String.drop(%String* %t5)
  call void @String.drop(%String* %t4)
  call void @String.drop(%String* %t3)
  call void @String.drop(%String* %t2)
  call void @String.drop(%String* %t1)
  call void @String.drop(%String* %sub4)
  call void @String.drop(%String* %sub3)
  call void @String.drop(%String* %sub2)
  call void @String.drop(%String* %sub)
  call void @String.drop(%String* %l)
  call void @String.drop(%String* %u)
  call void @String.drop(%String* %s)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
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
use "@/header"

fn main() i32:
    a String = String::create("hello")
    b String = String::create(" world")
    c String = a + b
    output(c, "\n")

    d String = a + " there"
    output(d, "\n")

    a += b
    output(a, "\n")

    e String = String::create("test")
    e += "ing"
    output(e, "\n")

    x String = String::create("abc")
    y String = String::create("abc")
    z String = String::create("xyz")

    output(x == y, "\n")
    output(x == z, "\n")
    output(x != z, "\n")
    output(x != y, "\n")
    output(x < z, "\n")
    output(z < x, "\n")
    output(z > x, "\n")
    output(x > z, "\n")

    output(x == "abc", "\n")
    output(x == "xyz", "\n")
    output(x != "xyz", "\n")
    output(x != "abc", "\n")

    f String = String::create("")
    g String = String::create("")
    f = y
    output(f, " ", f == y, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

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

define i32 @String.compare(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  %other = alloca %String*, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %self, align 8
  store %String* %1, %String** %other, align 8
  store i32 0, i32* %i, align 4
  br label %while9

while9:                                           ; preds = %endif, %entry
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
  br i1 %AND, label %while9.then, label %endloop

while9.then:                                      ; preds = %while9
  br label %if10

if10:                                             ; preds = %while9.then
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
  br i1 %NQ, label %if10.then, label %endif

if10.then:                                        ; preds = %if10
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

endif:                                            ; preds = %if10
  %"$i20" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i20", 1
  store i32 %ADD, i32* %i, align 4
  br label %while9

endloop:                                          ; preds = %while9
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

define i1 @"String.>"(%String* %0, %String* %1) {
entry:
  %self = alloca %String*, align 8
  %other = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  store %String* %1, %String** %other, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*other" = load %String*, %String** %other, align 8
  %call.String.compare = call i32 @String.compare(%String* %"*self", %String* %"*other")
  %GT = icmp sgt i32 %call.String.compare, 0
  ret i1 %GT
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
  br label %while11

while11:                                          ; preds = %while11.then, %entry
  %"*other" = load %String*, %String** %other, align 8
  %"*other.count" = getelementptr inbounds %String, %String* %"*other", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*other.count" = load i32, i32* %"*other.count", align 4
  %LT = icmp slt i32 %"$i", %"$*other.count"
  br i1 %LT, label %while11.then, label %endloop

while11.then:                                     ; preds = %while11
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
  br label %while11

endloop:                                          ; preds = %while11
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

define i32 @main() {
entry:
  %a = alloca %String, align 8
  %b = alloca %String, align 8
  %c = alloca %String, align 8
  %d = alloca %String, align 8
  %e = alloca %String, align 8
  %x = alloca %String, align 8
  %y = alloca %String, align 8
  %z = alloca %String, align 8
  %bool.text = alloca i8*, align 8
  %bool.text27 = alloca i8*, align 8
  %bool.text30 = alloca i8*, align 8
  %bool.text34 = alloca i8*, align 8
  %bool.text37 = alloca i8*, align 8
  %bool.text41 = alloca i8*, align 8
  %bool.text44 = alloca i8*, align 8
  %bool.text48 = alloca i8*, align 8
  %tmp = alloca %String, align 8
  %bool.text52 = alloca i8*, align 8
  %tmp55 = alloca %String, align 8
  %bool.text57 = alloca i8*, align 8
  %tmp60 = alloca %String, align 8
  %bool.text62 = alloca i8*, align 8
  %tmp65 = alloca %String, align 8
  %bool.text67 = alloca i8*, align 8
  %f = alloca %String, align 8
  %g = alloca %String, align 8
  %copy = alloca %String, align 8
  %bool.text74 = alloca i8*, align 8
  %call.String.create = call %String @String.create(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0))
  store %String %call.String.create, %String* %a, align 8
  %call.String.create13 = call %String @String.create(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.3, i32 0, i32 0))
  store %String %call.String.create13, %String* %b, align 8
  %"call.String.+" = call %String @"String.+.2"(%String* %a, %String* %b)
  store %String %"call.String.+", %String* %c, align 8
  %call.String.chars = call i8* @String.chars(%String* %c)
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i8* %call.String.chars)
  %"call.String.+14" = call %String @"String.+"(%String* %a, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.5, i32 0, i32 0))
  store %String %"call.String.+14", %String* %d, align 8
  %call.String.chars15 = call i8* @String.chars(%String* %d)
  %call.dprintf16 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.6, i32 0, i32 0), i8* %call.String.chars15)
  call void @"String.+=.1"(%String* %a, %String* %b)
  %call.String.chars17 = call i8* @String.chars(%String* %a)
  %call.dprintf18 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.7, i32 0, i32 0), i8* %call.String.chars17)
  %call.String.create19 = call %String @String.create(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.8, i32 0, i32 0))
  store %String %call.String.create19, %String* %e, align 8
  call void @"String.+="(%String* %e, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.9, i32 0, i32 0))
  %call.String.chars20 = call i8* @String.chars(%String* %e)
  %call.dprintf21 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.10, i32 0, i32 0), i8* %call.String.chars20)
  %call.String.create22 = call %String @String.create(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.11, i32 0, i32 0))
  store %String %call.String.create22, %String* %x, align 8
  %call.String.create23 = call %String @String.create(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.12, i32 0, i32 0))
  store %String %call.String.create23, %String* %y, align 8
  %call.String.create24 = call %String @String.create(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.13, i32 0, i32 0))
  store %String %call.String.create24, %String* %z, align 8
  %"call.String.==" = call i1 @"String.=="(%String* %x, %String* %y)
  store i8* null, i8** %bool.text, align 8
  br label %if12

if12:                                             ; preds = %entry
  br i1 %"call.String.==", label %if12.then, label %else13

if12.then:                                        ; preds = %if12
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.14, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else13:                                           ; preds = %if12
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.15, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else13, %if12.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.dprintf25 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.16, i32 0, i32 0), i8* %"$bool.text")
  %"call.String.==26" = call i1 @"String.=="(%String* %x, %String* %z)
  store i8* null, i8** %bool.text27, align 8
  br label %if14

if14:                                             ; preds = %endif
  br i1 %"call.String.==26", label %if14.then, label %else15

if14.then:                                        ; preds = %if14
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.17, i32 0, i32 0), i8** %bool.text27, align 8
  br label %endif1

else15:                                           ; preds = %if14
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.18, i32 0, i32 0), i8** %bool.text27, align 8
  br label %endif1

endif1:                                           ; preds = %else15, %if14.then
  %"$bool.text28" = load i8*, i8** %bool.text27, align 8
  %call.dprintf29 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.19, i32 0, i32 0), i8* %"$bool.text28")
  %"call.String.!=" = call i1 @"String.!="(%String* %x, %String* %z)
  store i8* null, i8** %bool.text30, align 8
  br label %if16

if16:                                             ; preds = %endif1
  br i1 %"call.String.!=", label %if16.then, label %else17

if16.then:                                        ; preds = %if16
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.20, i32 0, i32 0), i8** %bool.text30, align 8
  br label %endif2

else17:                                           ; preds = %if16
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.21, i32 0, i32 0), i8** %bool.text30, align 8
  br label %endif2

endif2:                                           ; preds = %else17, %if16.then
  %"$bool.text31" = load i8*, i8** %bool.text30, align 8
  %call.dprintf32 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.22, i32 0, i32 0), i8* %"$bool.text31")
  %"call.String.!=33" = call i1 @"String.!="(%String* %x, %String* %y)
  store i8* null, i8** %bool.text34, align 8
  br label %if18

if18:                                             ; preds = %endif2
  br i1 %"call.String.!=33", label %if18.then, label %else19

if18.then:                                        ; preds = %if18
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.23, i32 0, i32 0), i8** %bool.text34, align 8
  br label %endif3

else19:                                           ; preds = %if18
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.24, i32 0, i32 0), i8** %bool.text34, align 8
  br label %endif3

endif3:                                           ; preds = %else19, %if18.then
  %"$bool.text35" = load i8*, i8** %bool.text34, align 8
  %call.dprintf36 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.25, i32 0, i32 0), i8* %"$bool.text35")
  %"call.String.<" = call i1 @"String.<"(%String* %x, %String* %z)
  store i8* null, i8** %bool.text37, align 8
  br label %if20

if20:                                             ; preds = %endif3
  br i1 %"call.String.<", label %if20.then, label %else21

if20.then:                                        ; preds = %if20
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.26, i32 0, i32 0), i8** %bool.text37, align 8
  br label %endif4

else21:                                           ; preds = %if20
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.27, i32 0, i32 0), i8** %bool.text37, align 8
  br label %endif4

endif4:                                           ; preds = %else21, %if20.then
  %"$bool.text38" = load i8*, i8** %bool.text37, align 8
  %call.dprintf39 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.28, i32 0, i32 0), i8* %"$bool.text38")
  %"call.String.<40" = call i1 @"String.<"(%String* %z, %String* %x)
  store i8* null, i8** %bool.text41, align 8
  br label %if22

if22:                                             ; preds = %endif4
  br i1 %"call.String.<40", label %if22.then, label %else23

if22.then:                                        ; preds = %if22
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.29, i32 0, i32 0), i8** %bool.text41, align 8
  br label %endif5

else23:                                           ; preds = %if22
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.30, i32 0, i32 0), i8** %bool.text41, align 8
  br label %endif5

endif5:                                           ; preds = %else23, %if22.then
  %"$bool.text42" = load i8*, i8** %bool.text41, align 8
  %call.dprintf43 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.31, i32 0, i32 0), i8* %"$bool.text42")
  %"call.String.>" = call i1 @"String.>"(%String* %z, %String* %x)
  store i8* null, i8** %bool.text44, align 8
  br label %if24

if24:                                             ; preds = %endif5
  br i1 %"call.String.>", label %if24.then, label %else25

if24.then:                                        ; preds = %if24
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.32, i32 0, i32 0), i8** %bool.text44, align 8
  br label %endif6

else25:                                           ; preds = %if24
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.33, i32 0, i32 0), i8** %bool.text44, align 8
  br label %endif6

endif6:                                           ; preds = %else25, %if24.then
  %"$bool.text45" = load i8*, i8** %bool.text44, align 8
  %call.dprintf46 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.34, i32 0, i32 0), i8* %"$bool.text45")
  %"call.String.>47" = call i1 @"String.>"(%String* %x, %String* %z)
  store i8* null, i8** %bool.text48, align 8
  br label %if26

if26:                                             ; preds = %endif6
  br i1 %"call.String.>47", label %if26.then, label %else27

if26.then:                                        ; preds = %if26
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.35, i32 0, i32 0), i8** %bool.text48, align 8
  br label %endif7

else27:                                           ; preds = %if26
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.36, i32 0, i32 0), i8** %bool.text48, align 8
  br label %endif7

endif7:                                           ; preds = %else27, %if26.then
  %"$bool.text49" = load i8*, i8** %bool.text48, align 8
  %call.dprintf50 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.37, i32 0, i32 0), i8* %"$bool.text49")
  store %String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.38, i32 0, i32 0), i32 3, i32 0 }, %String* %tmp, align 8
  %"call.String.==51" = call i1 @"String.=="(%String* %x, %String* %tmp)
  store i8* null, i8** %bool.text52, align 8
  br label %if28

if28:                                             ; preds = %endif7
  br i1 %"call.String.==51", label %if28.then, label %else29

if28.then:                                        ; preds = %if28
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.39, i32 0, i32 0), i8** %bool.text52, align 8
  br label %endif8

else29:                                           ; preds = %if28
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.40, i32 0, i32 0), i8** %bool.text52, align 8
  br label %endif8

endif8:                                           ; preds = %else29, %if28.then
  %"$bool.text53" = load i8*, i8** %bool.text52, align 8
  %call.dprintf54 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.41, i32 0, i32 0), i8* %"$bool.text53")
  call void @String.drop(%String* %tmp)
  store %String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.42, i32 0, i32 0), i32 3, i32 0 }, %String* %tmp55, align 8
  %"call.String.==56" = call i1 @"String.=="(%String* %x, %String* %tmp55)
  store i8* null, i8** %bool.text57, align 8
  br label %if30

if30:                                             ; preds = %endif8
  br i1 %"call.String.==56", label %if30.then, label %else31

if30.then:                                        ; preds = %if30
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.43, i32 0, i32 0), i8** %bool.text57, align 8
  br label %endif9

else31:                                           ; preds = %if30
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.44, i32 0, i32 0), i8** %bool.text57, align 8
  br label %endif9

endif9:                                           ; preds = %else31, %if30.then
  %"$bool.text58" = load i8*, i8** %bool.text57, align 8
  %call.dprintf59 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.45, i32 0, i32 0), i8* %"$bool.text58")
  call void @String.drop(%String* %tmp55)
  store %String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.46, i32 0, i32 0), i32 3, i32 0 }, %String* %tmp60, align 8
  %"call.String.!=61" = call i1 @"String.!="(%String* %x, %String* %tmp60)
  store i8* null, i8** %bool.text62, align 8
  br label %if32

if32:                                             ; preds = %endif9
  br i1 %"call.String.!=61", label %if32.then, label %else33

if32.then:                                        ; preds = %if32
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.47, i32 0, i32 0), i8** %bool.text62, align 8
  br label %endif10

else33:                                           ; preds = %if32
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.48, i32 0, i32 0), i8** %bool.text62, align 8
  br label %endif10

endif10:                                          ; preds = %else33, %if32.then
  %"$bool.text63" = load i8*, i8** %bool.text62, align 8
  %call.dprintf64 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.49, i32 0, i32 0), i8* %"$bool.text63")
  call void @String.drop(%String* %tmp60)
  store %String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.50, i32 0, i32 0), i32 3, i32 0 }, %String* %tmp65, align 8
  %"call.String.!=66" = call i1 @"String.!="(%String* %x, %String* %tmp65)
  store i8* null, i8** %bool.text67, align 8
  br label %if34

if34:                                             ; preds = %endif10
  br i1 %"call.String.!=66", label %if34.then, label %else35

if34.then:                                        ; preds = %if34
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.51, i32 0, i32 0), i8** %bool.text67, align 8
  br label %endif11

else35:                                           ; preds = %if34
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.52, i32 0, i32 0), i8** %bool.text67, align 8
  br label %endif11

endif11:                                          ; preds = %else35, %if34.then
  %"$bool.text68" = load i8*, i8** %bool.text67, align 8
  %call.dprintf69 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.53, i32 0, i32 0), i8* %"$bool.text68")
  call void @String.drop(%String* %tmp65)
  %call.String.create70 = call %String @String.create(i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.54, i32 0, i32 0))
  store %String %call.String.create70, %String* %f, align 8
  %call.String.create71 = call %String @String.create(i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.55, i32 0, i32 0))
  store %String %call.String.create71, %String* %g, align 8
  store %String zeroinitializer, %String* %copy, align 8
  call void @"String.="(%String* %copy, %String* %y)
  %"$copy" = load %String, %String* %copy, align 8
  call void @String.drop(%String* %f)
  store %String %"$copy", %String* %f, align 8
  %call.String.chars72 = call i8* @String.chars(%String* %f)
  %"call.String.==73" = call i1 @"String.=="(%String* %f, %String* %y)
  store i8* null, i8** %bool.text74, align 8
  br label %if36

if36:                                             ; preds = %endif11
  br i1 %"call.String.==73", label %if36.then, label %else37

if36.then:                                        ; preds = %if36
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.56, i32 0, i32 0), i8** %bool.text74, align 8
  br label %endif12

else37:                                           ; preds = %if36
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.57, i32 0, i32 0), i8** %bool.text74, align 8
  br label %endif12

endif12:                                          ; preds = %else37, %if36.then
  %"$bool.text75" = load i8*, i8** %bool.text74, align 8
  %call.dprintf76 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.58, i32 0, i32 0), i8* %call.String.chars72, i8* %"$bool.text75")
  call void @String.drop(%String* %g)
  call void @String.drop(%String* %f)
  call void @String.drop(%String* %z)
  call void @String.drop(%String* %y)
  call void @String.drop(%String* %x)
  call void @String.drop(%String* %e)
  call void @String.drop(%String* %d)
  call void @String.drop(%String* %c)
  call void @String.drop(%String* %b)
  call void @String.drop(%String* %a)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
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
use "@/header"

fn main() i32:
    s1 String = String::create("12345")
    output(s1.to_int(), "\n")

    s2 String = String::create("-99")
    output(s2.to_int(), "\n")

    s3 String = String::create("0")
    output(s3.to_int(), "\n")

    a String = String::create("apple")
    b String = String::create("banana")
    output(a.compare(&b), "\n")
    output(b.compare(&a), "\n")
    output(a.compare(&a), "\n")

    s String = String::create("Hello World")
    r String = s.upper()
    r2 String = r.reverse()
    output(r2, "\n")

    big String = String::create("aaaa")
    i i32 = 0
    while i < 5:
        big.join("bbbb")
        i += 1
    output(big.size(), "\n")

    n1 String = String::create(1000000)
    output(n1, "\n")

    n2 String = String::create(-2000000)
    output(n2, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

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

define i32 @String.size(%String* %0) {
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
  br label %if6

if6:                                              ; preds = %entry
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if6.then, %if6
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
  call void @String.drop(%String* %res)
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
  br label %while17

while17:                                          ; preds = %while17.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %GE = icmp sge i32 %"$i", 0
  br i1 %GE, label %while17.then, label %endloop

while17.then:                                     ; preds = %while17
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
  br label %while17

endloop:                                          ; preds = %while17
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
  br label %while18

while18:                                          ; preds = %endif, %entry
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
  br i1 %AND, label %while18.then, label %endloop

while18.then:                                     ; preds = %while18
  br label %if19

if19:                                             ; preds = %while18.then
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
  br i1 %NQ, label %if19.then, label %endif

if19.then:                                        ; preds = %if19
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

endif:                                            ; preds = %if19
  %"$i20" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i20", 1
  store i32 %ADD, i32* %i, align 4
  br label %while18

endloop:                                          ; preds = %while18
  %"*self21" = load %String*, %String** %self, align 8
  %"*self.count22" = getelementptr inbounds %String, %String* %"*self21", i32 0, i32 1
  %"*other23" = load %String*, %String** %other, align 8
  %"*other.count24" = getelementptr inbounds %String, %String* %"*other23", i32 0, i32 1
  %"$*self.count25" = load i32, i32* %"*self.count22", align 4
  %"$*other.count26" = load i32, i32* %"*other.count24", align 4
  %SUB27 = sub i32 %"$*self.count25", %"$*other.count26"
  ret i32 %SUB27
}

define i32 @main() {
entry:
  %s1 = alloca %String, align 8
  %s2 = alloca %String, align 8
  %s3 = alloca %String, align 8
  %a = alloca %String, align 8
  %b = alloca %String, align 8
  %s = alloca %String, align 8
  %r = alloca %String, align 8
  %r2 = alloca %String, align 8
  %big = alloca %String, align 8
  %i = alloca i32, align 4
  %n1 = alloca %String, align 8
  %n2 = alloca %String, align 8
  %call.String.create = call %String @String.create(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0))
  store %String %call.String.create, %String* %s1, align 8
  %call.String.to_int = call i32 @String.to_int(%String* %s1)
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 %call.String.to_int)
  %call.String.create1 = call %String @String.create(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.3, i32 0, i32 0))
  store %String %call.String.create1, %String* %s2, align 8
  %call.String.to_int2 = call i32 @String.to_int(%String* %s2)
  %call.dprintf3 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.4, i32 0, i32 0), i32 %call.String.to_int2)
  %call.String.create4 = call %String @String.create(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  store %String %call.String.create4, %String* %s3, align 8
  %call.String.to_int5 = call i32 @String.to_int(%String* %s3)
  %call.dprintf6 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.6, i32 0, i32 0), i32 %call.String.to_int5)
  %call.String.create7 = call %String @String.create(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.7, i32 0, i32 0))
  store %String %call.String.create7, %String* %a, align 8
  %call.String.create8 = call %String @String.create(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.8, i32 0, i32 0))
  store %String %call.String.create8, %String* %b, align 8
  %call.String.compare = call i32 @String.compare(%String* %a, %String* %b)
  %call.dprintf9 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.9, i32 0, i32 0), i32 %call.String.compare)
  %call.String.compare10 = call i32 @String.compare(%String* %b, %String* %a)
  %call.dprintf11 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.10, i32 0, i32 0), i32 %call.String.compare10)
  %call.String.compare12 = call i32 @String.compare(%String* %a, %String* %a)
  %call.dprintf13 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.11, i32 0, i32 0), i32 %call.String.compare12)
  %call.String.create14 = call %String @String.create(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.12, i32 0, i32 0))
  store %String %call.String.create14, %String* %s, align 8
  %call.String.upper = call %String @String.upper(%String* %s)
  store %String %call.String.upper, %String* %r, align 8
  %call.String.reverse = call %String @String.reverse(%String* %r)
  store %String %call.String.reverse, %String* %r2, align 8
  %call.String.chars = call i8* @String.chars(%String* %r2)
  %call.dprintf15 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.13, i32 0, i32 0), i8* %call.String.chars)
  %call.String.create16 = call %String @String.create(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.14, i32 0, i32 0))
  store %String %call.String.create16, %String* %big, align 8
  store i32 0, i32* %i, align 4
  br label %while20

while20:                                          ; preds = %while20.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 5
  br i1 %LT, label %while20.then, label %endloop

while20.then:                                     ; preds = %while20
  call void @String.join(%String* %big, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.15, i32 0, i32 0))
  %"$i17" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i17", 1
  store i32 %ADD, i32* %i, align 4
  br label %while20

endloop:                                          ; preds = %while20
  %call.String.size = call i32 @String.size(%String* %big)
  %call.dprintf18 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.16, i32 0, i32 0), i32 %call.String.size)
  %call.String.create19 = call %String @String.create.1(i32 1000000)
  store %String %call.String.create19, %String* %n1, align 8
  %call.String.chars20 = call i8* @String.chars(%String* %n1)
  %call.dprintf21 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.17, i32 0, i32 0), i8* %call.String.chars20)
  %call.String.create22 = call %String @String.create.1(i32 -2000000)
  store %String %call.String.create22, %String* %n2, align 8
  %call.String.chars23 = call i8* @String.chars(%String* %n2)
  %call.dprintf24 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.18, i32 0, i32 0), i8* %call.String.chars23)
  call void @String.drop(%String* %n2)
  call void @String.drop(%String* %n1)
  call void @String.drop(%String* %big)
  call void @String.drop(%String* %r2)
  call void @String.drop(%String* %r)
  call void @String.drop(%String* %s)
  call void @String.drop(%String* %b)
  call void @String.drop(%String* %a)
  call void @String.drop(%String* %s3)
  call void @String.drop(%String* %s2)
  call void @String.drop(%String* %s1)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

