# milestone12

## index

- 001 — nullable: comparing a never-null value to null is rejected
- 002 — nullable: null needs a target type
- 003 — try: a throw is caught in the same function
- 004 — try: a throw propagates across calls to a caller's catch
- 005 — try: a rethrow inside a catch propagates outward
- 006 — try: a nested try: the inner catch handles it
- 007 — try: return inside a try returns from the function
- 008 — try: break inside a try exits the enclosing loop
- 009 — try: an uncaught throw prints its message and exits
- 010 — try: any type can be thrown; a catch takes the type it names
- 011 — try: catch without a matching try is a parse error
- 012 — try: code after a throw is unreachable and dropped

---

## 001 — nullable: comparing a never-null value to null is rejected

```ura
// error: 's1' is never null, so this comparison is always the same

fn main() i32:
    s1 char[] = "hello"
    if s1 != null:
        output("never\n")
    return 0
```

---

## 002 — nullable: null needs a target type

```ura
// error: 'x' can never be null; declare it optional to assign null to it

fn main() i32:
    x char[] = null
    return 0
```

---

## 003 — try: a throw is caught in the same function

```ura
// stdout: caught: boom
// expect: 0

fn main() i32:
    try:
        throw Error::create(String::create("boom"))
    catch e Error:
        output("caught: ", e.message, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%String = type { i8*, i32, i32 }
%Error = type { %String }

@ucg.thrown = global i1 false
@ucg.error = global [2 x i64] zeroinitializer
@ucg.error_type = global i32 0
@str = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"boom\00", align 1
@str.2 = private unnamed_addr constant [12 x i8] c"caught: %s\0A\00", align 1

define i8* @"alloc<i8>"(i32 %0) {
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
  %"*self5" = load %String*, %String** %self, align 8
  %"*self.count6" = getelementptr inbounds %String, %String* %"*self5", i32 0, i32 1
  %"$*self.count7" = load i32, i32* %"*self.count6", align 4
  store i32 %"$*self.count7", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 2
  %"$cap9" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap9", %"$*self.cap"
  br i1 %LE, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  ret void

endif1:                                           ; preds = %if1
  %"$cap10" = load i32, i32* %cap, align 4
  %"call.alloc<i8>" = call i8* @"alloc<i8>"(i32 %"$cap10")
  br label %thrown2

thrown2:                                          ; preds = %endif1
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown2.then, label %endthrown

thrown2.then:                                     ; preds = %thrown2
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown2
  store i8* %"call.alloc<i8>", i8** %bigger, align 8
  %"*self11" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self11", i32 0, i32 0
  %"*self12" = load %String*, %String** %self, align 8
  %"*self.count13" = getelementptr inbounds %String, %String* %"*self12", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count14" = load i32, i32* %"*self.count13", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count14")
  br label %thrown3

thrown3:                                          ; preds = %endthrown
  %"$ucg.thrown15" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown15", label %thrown3.then, label %endthrown2

thrown3.then:                                     ; preds = %thrown3
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown2:                                       ; preds = %thrown3
  br label %if4

if4:                                              ; preds = %endthrown2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.cap17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 2
  %"$*self.cap18" = load i32, i32* %"*self.cap17", align 4
  %GT = icmp sgt i32 %"$*self.cap18", 0
  br i1 %GT, label %if4.then, label %endif4

if4.then:                                         ; preds = %if4
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$*self.data21" = load i8*, i8** %"*self.data20", align 8
  call void @"free<i8>"(i8* %"$*self.data21")
  br label %thrown5

thrown5:                                          ; preds = %if4.then
  %"$ucg.thrown22" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown22", label %thrown5.then, label %endthrown3

thrown5.then:                                     ; preds = %thrown5
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown3:                                       ; preds = %thrown5
  br label %endif4

endif4:                                           ; preds = %endthrown3, %if4
  %"*self23" = load %String*, %String** %self, align 8
  %"*self.data24" = getelementptr inbounds %String, %String* %"*self23", i32 0, i32 0
  %"$bigger25" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger25", i8** %"*self.data24", align 8
  %"*self26" = load %String*, %String** %self, align 8
  %"*self.cap27" = getelementptr inbounds %String, %String* %"*self26", i32 0, i32 2
  %"$cap28" = load i32, i32* %cap, align 4
  store i32 %"$cap28", i32* %"*self.cap27", align 4
  ret void
}

define void @String.push_back(%String* %0, i8 %1) {
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
  br label %thrown7

thrown7:                                          ; preds = %if6.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown7.then, label %endthrown

thrown7.then:                                     ; preds = %thrown7
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown7
  br label %endif

endif:                                            ; preds = %endthrown, %if6
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

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if8

if8:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if8.then, label %endif

if8.then:                                         ; preds = %if8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if8.then, %if8
  br label %while9

while9:                                           ; preds = %while9.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while9.then, label %endloop

while9.then:                                      ; preds = %while9
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
  br label %while9

endloop:                                          ; preds = %while9
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
  br label %thrown10

thrown10:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown10.then, label %endthrown

thrown10.then:                                    ; preds = %thrown10
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown10
  %"*self2" = load %String*, %String** %self, align 8
  %"*other" = load %String*, %String** %other, align 8
  %"*other.count" = getelementptr inbounds %String, %String* %"*other", i32 0, i32 1
  %"$*other.count" = load i32, i32* %"*other.count", align 4
  call void @String.reserve(%String* %"*self2", i32 %"$*other.count")
  br label %thrown11

thrown11:                                         ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown11.then, label %endthrown1

thrown11.then:                                    ; preds = %thrown11
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown11
  store i32 0, i32* %i, align 4
  br label %while12

while12:                                          ; preds = %while12.then, %endthrown1
  %"*other4" = load %String*, %String** %other, align 8
  %"*other.count5" = getelementptr inbounds %String, %String* %"*other4", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*other.count6" = load i32, i32* %"*other.count5", align 4
  %LT = icmp slt i32 %"$i", %"$*other.count6"
  br i1 %LT, label %while12.then, label %endloop

while12.then:                                     ; preds = %while12
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i8" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i8"
  %"*other9" = load %String*, %String** %other, align 8
  %"*other.data" = getelementptr inbounds %String, %String* %"*other9", i32 0, i32 0
  %"$*other.data" = load i8*, i8** %"*other.data", align 8
  %"$i10" = load i32, i32* %i, align 4
  %"*other.data[]" = getelementptr inbounds i8, i8* %"$*other.data", i32 %"$i10"
  %"$*other.data[]" = load i8, i8* %"*other.data[]", align 1
  store i8 %"$*other.data[]", i8* %"*self.data[]", align 1
  %"$i11" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i11", 1
  store i32 %ADD, i32* %i, align 4
  br label %while12

endloop:                                          ; preds = %while12
  %"*self12" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self12", i32 0, i32 1
  %"*other13" = load %String*, %String** %other, align 8
  %"*other.count14" = getelementptr inbounds %String, %String* %"*other13", i32 0, i32 1
  %"$*other.count15" = load i32, i32* %"*other.count14", align 4
  store i32 %"$*other.count15", i32* %"*self.count", align 4
  ret void
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %thrown13

thrown13:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown13.then, label %endthrown

thrown13.then:                                    ; preds = %thrown13
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown13
  br label %if14

if14:                                             ; preds = %endthrown
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if14.then, label %endif

if14.then:                                        ; preds = %if14
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %thrown15

thrown15:                                         ; preds = %if14.then
  %"$ucg.thrown4" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown4", label %thrown15.then, label %endthrown1

thrown15.then:                                    ; preds = %thrown15
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown15
  br label %endif

endif:                                            ; preds = %endthrown1, %if14
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
  br label %thrown16

thrown16:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown16.then, label %endthrown

thrown16.then:                                    ; preds = %thrown16
  store i1 false, i1* @ucg.thrown, align 1
  call void @String.drop(%String* %res)
  store i1 true, i1* @ucg.thrown, align 1
  ret %String zeroinitializer

endthrown:                                        ; preds = %thrown16
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
  br label %while17

while17:                                          ; preds = %endthrown, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$i" = load i32, i32* %i, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$i"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while17.then, label %endloop

while17.then:                                     ; preds = %while17
  %"*self" = load %String*, %String** %self, align 8
  %"$text1" = load i8*, i8** %text, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"text[]3" = getelementptr inbounds i8, i8* %"$text1", i32 %"$i2"
  %"$text[]4" = load i8, i8* %"text[]3", align 1
  call void @String.push_back(%String* %"*self", i8 %"$text[]4")
  br label %thrown18

thrown18:                                         ; preds = %while17.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown18.then, label %endthrown

thrown18.then:                                    ; preds = %thrown18
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown18
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while17

endloop:                                          ; preds = %while17
  ret void
}

define i8* @String.c_str(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  br label %thrown19

thrown19:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown19.then, label %endthrown

thrown19.then:                                    ; preds = %thrown19
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i8* null

endthrown:                                        ; preds = %thrown19
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

define %Error @Error.create(%String %0) {
entry:
  %message = alloca %String, align 8
  %e = alloca %Error, align 8
  %copy = alloca %String, align 8
  store %String %0, %String* %message, align 8
  store %Error zeroinitializer, %Error* %e, align 8
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  store %String zeroinitializer, %String* %copy, align 8
  call void @"String.="(%String* %copy, %String* %message)
  br label %thrown20

thrown20:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown20.then, label %endthrown

thrown20.then:                                    ; preds = %thrown20
  store i1 false, i1* @ucg.thrown, align 1
  %e.message2 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message2)
  call void @String.drop(%String* %message)
  store i1 true, i1* @ucg.thrown, align 1
  ret %Error zeroinitializer

endthrown:                                        ; preds = %thrown20
  %"$copy" = load %String, %String* %copy, align 8
  call void @String.drop(%String* %e.message)
  br label %thrown21

thrown21:                                         ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown21.then, label %endthrown1

thrown21.then:                                    ; preds = %thrown21
  store i1 false, i1* @ucg.thrown, align 1
  %e.message4 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message4)
  call void @String.drop(%String* %message)
  store i1 true, i1* @ucg.thrown, align 1
  ret %Error zeroinitializer

endthrown1:                                       ; preds = %thrown21
  store %String %"$copy", %String* %e.message, align 8
  %"$e" = load %Error, %Error* %e, align 8
  call void @String.drop(%String* %message)
  ret %Error %"$e"
}

define i32 @uncaught(%Error* %0) {
entry:
  %error = alloca %Error*, align 8
  store %Error* %0, %Error** %error, align 8
  %"*error" = load %Error*, %Error** %error, align 8
  %"*error.message" = getelementptr inbounds %Error, %Error* %"*error", i32 0, i32 0
  %call.String.c_str = call i8* @String.c_str(%String* %"*error.message")
  br label %thrown22

thrown22:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown22.then, label %endthrown

thrown22.then:                                    ; preds = %thrown22
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown:                                        ; preds = %thrown22
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i8* %call.String.c_str)
  ret i32 1
}

define i32 @main() {
entry:
  %error.at = alloca %Error*, align 8
  %e = alloca %Error, align 8
  %error.at7 = alloca %Error*, align 8
  %error.at15 = alloca %Error*, align 8
  br label %try23

try23:                                            ; preds = %entry
  br i1 true, label %try23.then, label %endtry2

try23.then:                                       ; preds = %try23
  br label %try24

try24:                                            ; preds = %try23.then
  br i1 true, label %try24.then, label %endtry

try24.then:                                       ; preds = %try24
  %call.String.create = call %String @String.create(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0))
  br label %thrown25

thrown25:                                         ; preds = %try24.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown25.then, label %endthrown

thrown25.then:                                    ; preds = %thrown25
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endthrown:                                        ; preds = %thrown25
  %call.Error.create = call %Error @Error.create(%String %call.String.create)
  br label %thrown26

thrown26:                                         ; preds = %endthrown
  %"$ucg.thrown5" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown5", label %thrown26.then, label %endthrown1

thrown26.then:                                    ; preds = %thrown26
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endthrown1:                                       ; preds = %thrown26
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  store %Error %call.Error.create, %Error* %"*error.at", align 8
  store i32 1, i32* @ucg.error_type, align 4
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endtry:                                           ; preds = %endthrown1, %thrown26.then, %thrown25.then, %try24
  br label %catch27

catch27:                                          ; preds = %endtry
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown6" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown6", %EQ
  br i1 %AND, label %catch27.then, label %endcatch

catch27.then:                                     ; preds = %catch27
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at7, align 8
  %"*error.at8" = load %Error*, %Error** %error.at7, align 8
  %"$*error.at" = load %Error, %Error* %"*error.at8", align 8
  store %Error %"$*error.at", %Error* %e, align 8
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  %"$e.message" = load %String, %String* %e.message, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.2, i32 0, i32 0), %String %"$e.message")
  %e.message9 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message9)
  br label %endcatch

endcatch:                                         ; preds = %catch27.then, %catch27
  br label %rethrow28

rethrow28:                                        ; preds = %endcatch
  %"$ucg.thrown10" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown10", label %rethrow28.then, label %endrethrow

rethrow28.then:                                   ; preds = %rethrow28
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry2

endrethrow:                                       ; preds = %rethrow28
  ret i32 0

endtry2:                                          ; preds = %rethrow28.then, %try23
  br label %catch29

catch29:                                          ; preds = %endtry2
  %"$ucg.error_type11" = load i32, i32* @ucg.error_type, align 4
  %EQ12 = icmp eq i32 %"$ucg.error_type11", 1
  %"$ucg.thrown13" = load i1, i1* @ucg.thrown, align 1
  %AND14 = and i1 %"$ucg.thrown13", %EQ12
  br i1 %AND14, label %catch29.then, label %endcatch4

catch29.then:                                     ; preds = %catch29
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at15, align 8
  %"*error.at16" = load %Error*, %Error** %error.at15, align 8
  %call.uncaught = call i32 @uncaught(%Error* %"*error.at16")
  br label %thrown30

thrown30:                                         ; preds = %catch29.then
  %"$ucg.thrown17" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown17", label %thrown30.then, label %endthrown3

thrown30.then:                                    ; preds = %thrown30
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown3:                                       ; preds = %thrown30
  ret i32 %call.uncaught

endcatch4:                                        ; preds = %catch29
  br label %if31

if31:                                             ; preds = %endcatch4
  %"$ucg.thrown18" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown18", label %if31.then, label %endif

if31.then:                                        ; preds = %if31
  ret i32 1

endif:                                            ; preds = %if31
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 004 — try: a throw propagates across calls to a caller's catch

```ura
// stdout: caught: from deep
// stdout: after
// expect: 0

fn deep():
    throw Error::create(String::create("from deep"))

fn middle():
    deep()

fn main() i32:
    try:
        middle()
    catch e Error:
        output("caught: ", e.message, "\n")
    output("after\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%String = type { i8*, i32, i32 }
%Error = type { %String }

@ucg.thrown = global i1 false
@ucg.error = global [2 x i64] zeroinitializer
@ucg.error_type = global i32 0
@str = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c"from deep\00", align 1
@str.2 = private unnamed_addr constant [12 x i8] c"caught: %s\0A\00", align 1
@str.3 = private unnamed_addr constant [7 x i8] c"after\0A\00", align 1

define i8* @"alloc<i8>"(i32 %0) {
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
  %"*self5" = load %String*, %String** %self, align 8
  %"*self.count6" = getelementptr inbounds %String, %String* %"*self5", i32 0, i32 1
  %"$*self.count7" = load i32, i32* %"*self.count6", align 4
  store i32 %"$*self.count7", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 2
  %"$cap9" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap9", %"$*self.cap"
  br i1 %LE, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  ret void

endif1:                                           ; preds = %if1
  %"$cap10" = load i32, i32* %cap, align 4
  %"call.alloc<i8>" = call i8* @"alloc<i8>"(i32 %"$cap10")
  br label %thrown2

thrown2:                                          ; preds = %endif1
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown2.then, label %endthrown

thrown2.then:                                     ; preds = %thrown2
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown2
  store i8* %"call.alloc<i8>", i8** %bigger, align 8
  %"*self11" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self11", i32 0, i32 0
  %"*self12" = load %String*, %String** %self, align 8
  %"*self.count13" = getelementptr inbounds %String, %String* %"*self12", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count14" = load i32, i32* %"*self.count13", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count14")
  br label %thrown3

thrown3:                                          ; preds = %endthrown
  %"$ucg.thrown15" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown15", label %thrown3.then, label %endthrown2

thrown3.then:                                     ; preds = %thrown3
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown2:                                       ; preds = %thrown3
  br label %if4

if4:                                              ; preds = %endthrown2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.cap17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 2
  %"$*self.cap18" = load i32, i32* %"*self.cap17", align 4
  %GT = icmp sgt i32 %"$*self.cap18", 0
  br i1 %GT, label %if4.then, label %endif4

if4.then:                                         ; preds = %if4
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$*self.data21" = load i8*, i8** %"*self.data20", align 8
  call void @"free<i8>"(i8* %"$*self.data21")
  br label %thrown5

thrown5:                                          ; preds = %if4.then
  %"$ucg.thrown22" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown22", label %thrown5.then, label %endthrown3

thrown5.then:                                     ; preds = %thrown5
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown3:                                       ; preds = %thrown5
  br label %endif4

endif4:                                           ; preds = %endthrown3, %if4
  %"*self23" = load %String*, %String** %self, align 8
  %"*self.data24" = getelementptr inbounds %String, %String* %"*self23", i32 0, i32 0
  %"$bigger25" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger25", i8** %"*self.data24", align 8
  %"*self26" = load %String*, %String** %self, align 8
  %"*self.cap27" = getelementptr inbounds %String, %String* %"*self26", i32 0, i32 2
  %"$cap28" = load i32, i32* %cap, align 4
  store i32 %"$cap28", i32* %"*self.cap27", align 4
  ret void
}

define void @String.push_back(%String* %0, i8 %1) {
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
  br label %thrown7

thrown7:                                          ; preds = %if6.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown7.then, label %endthrown

thrown7.then:                                     ; preds = %thrown7
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown7
  br label %endif

endif:                                            ; preds = %endthrown, %if6
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

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if8

if8:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if8.then, label %endif

if8.then:                                         ; preds = %if8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if8.then, %if8
  br label %while9

while9:                                           ; preds = %while9.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while9.then, label %endloop

while9.then:                                      ; preds = %while9
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
  br label %while9

endloop:                                          ; preds = %while9
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
  br label %thrown10

thrown10:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown10.then, label %endthrown

thrown10.then:                                    ; preds = %thrown10
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown10
  %"*self2" = load %String*, %String** %self, align 8
  %"*other" = load %String*, %String** %other, align 8
  %"*other.count" = getelementptr inbounds %String, %String* %"*other", i32 0, i32 1
  %"$*other.count" = load i32, i32* %"*other.count", align 4
  call void @String.reserve(%String* %"*self2", i32 %"$*other.count")
  br label %thrown11

thrown11:                                         ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown11.then, label %endthrown1

thrown11.then:                                    ; preds = %thrown11
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown11
  store i32 0, i32* %i, align 4
  br label %while12

while12:                                          ; preds = %while12.then, %endthrown1
  %"*other4" = load %String*, %String** %other, align 8
  %"*other.count5" = getelementptr inbounds %String, %String* %"*other4", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*other.count6" = load i32, i32* %"*other.count5", align 4
  %LT = icmp slt i32 %"$i", %"$*other.count6"
  br i1 %LT, label %while12.then, label %endloop

while12.then:                                     ; preds = %while12
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i8" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i8"
  %"*other9" = load %String*, %String** %other, align 8
  %"*other.data" = getelementptr inbounds %String, %String* %"*other9", i32 0, i32 0
  %"$*other.data" = load i8*, i8** %"*other.data", align 8
  %"$i10" = load i32, i32* %i, align 4
  %"*other.data[]" = getelementptr inbounds i8, i8* %"$*other.data", i32 %"$i10"
  %"$*other.data[]" = load i8, i8* %"*other.data[]", align 1
  store i8 %"$*other.data[]", i8* %"*self.data[]", align 1
  %"$i11" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i11", 1
  store i32 %ADD, i32* %i, align 4
  br label %while12

endloop:                                          ; preds = %while12
  %"*self12" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self12", i32 0, i32 1
  %"*other13" = load %String*, %String** %other, align 8
  %"*other.count14" = getelementptr inbounds %String, %String* %"*other13", i32 0, i32 1
  %"$*other.count15" = load i32, i32* %"*other.count14", align 4
  store i32 %"$*other.count15", i32* %"*self.count", align 4
  ret void
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %thrown13

thrown13:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown13.then, label %endthrown

thrown13.then:                                    ; preds = %thrown13
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown13
  br label %if14

if14:                                             ; preds = %endthrown
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if14.then, label %endif

if14.then:                                        ; preds = %if14
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %thrown15

thrown15:                                         ; preds = %if14.then
  %"$ucg.thrown4" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown4", label %thrown15.then, label %endthrown1

thrown15.then:                                    ; preds = %thrown15
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown15
  br label %endif

endif:                                            ; preds = %endthrown1, %if14
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
  br label %thrown16

thrown16:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown16.then, label %endthrown

thrown16.then:                                    ; preds = %thrown16
  store i1 false, i1* @ucg.thrown, align 1
  call void @String.drop(%String* %res)
  store i1 true, i1* @ucg.thrown, align 1
  ret %String zeroinitializer

endthrown:                                        ; preds = %thrown16
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
  br label %while17

while17:                                          ; preds = %endthrown, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$i" = load i32, i32* %i, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$i"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while17.then, label %endloop

while17.then:                                     ; preds = %while17
  %"*self" = load %String*, %String** %self, align 8
  %"$text1" = load i8*, i8** %text, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"text[]3" = getelementptr inbounds i8, i8* %"$text1", i32 %"$i2"
  %"$text[]4" = load i8, i8* %"text[]3", align 1
  call void @String.push_back(%String* %"*self", i8 %"$text[]4")
  br label %thrown18

thrown18:                                         ; preds = %while17.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown18.then, label %endthrown

thrown18.then:                                    ; preds = %thrown18
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown18
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while17

endloop:                                          ; preds = %while17
  ret void
}

define i8* @String.c_str(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  br label %thrown19

thrown19:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown19.then, label %endthrown

thrown19.then:                                    ; preds = %thrown19
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i8* null

endthrown:                                        ; preds = %thrown19
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

define %Error @Error.create(%String %0) {
entry:
  %message = alloca %String, align 8
  %e = alloca %Error, align 8
  %copy = alloca %String, align 8
  store %String %0, %String* %message, align 8
  store %Error zeroinitializer, %Error* %e, align 8
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  store %String zeroinitializer, %String* %copy, align 8
  call void @"String.="(%String* %copy, %String* %message)
  br label %thrown20

thrown20:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown20.then, label %endthrown

thrown20.then:                                    ; preds = %thrown20
  store i1 false, i1* @ucg.thrown, align 1
  %e.message2 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message2)
  call void @String.drop(%String* %message)
  store i1 true, i1* @ucg.thrown, align 1
  ret %Error zeroinitializer

endthrown:                                        ; preds = %thrown20
  %"$copy" = load %String, %String* %copy, align 8
  call void @String.drop(%String* %e.message)
  br label %thrown21

thrown21:                                         ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown21.then, label %endthrown1

thrown21.then:                                    ; preds = %thrown21
  store i1 false, i1* @ucg.thrown, align 1
  %e.message4 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message4)
  call void @String.drop(%String* %message)
  store i1 true, i1* @ucg.thrown, align 1
  ret %Error zeroinitializer

endthrown1:                                       ; preds = %thrown21
  store %String %"$copy", %String* %e.message, align 8
  %"$e" = load %Error, %Error* %e, align 8
  call void @String.drop(%String* %message)
  ret %Error %"$e"
}

define i32 @uncaught(%Error* %0) {
entry:
  %error = alloca %Error*, align 8
  store %Error* %0, %Error** %error, align 8
  %"*error" = load %Error*, %Error** %error, align 8
  %"*error.message" = getelementptr inbounds %Error, %Error* %"*error", i32 0, i32 0
  %call.String.c_str = call i8* @String.c_str(%String* %"*error.message")
  br label %thrown22

thrown22:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown22.then, label %endthrown

thrown22.then:                                    ; preds = %thrown22
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown:                                        ; preds = %thrown22
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i8* %call.String.c_str)
  ret i32 1
}

define void @deep() {
entry:
  %error.at = alloca %Error*, align 8
  %call.String.create = call %String @String.create(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0))
  br label %thrown23

thrown23:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown23.then, label %endthrown

thrown23.then:                                    ; preds = %thrown23
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown23
  %call.Error.create = call %Error @Error.create(%String %call.String.create)
  br label %thrown24

thrown24:                                         ; preds = %endthrown
  %"$ucg.thrown2" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown2", label %thrown24.then, label %endthrown1

thrown24.then:                                    ; preds = %thrown24
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown24
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  store %Error %call.Error.create, %Error* %"*error.at", align 8
  store i32 1, i32* @ucg.error_type, align 4
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void
}

define void @middle() {
entry:
  call void @deep()
  br label %thrown25

thrown25:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown25.then, label %endthrown

thrown25.then:                                    ; preds = %thrown25
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown25
  ret void
}

define i32 @main() {
entry:
  %e = alloca %Error, align 8
  %error.at = alloca %Error*, align 8
  %error.at12 = alloca %Error*, align 8
  br label %try26

try26:                                            ; preds = %entry
  br i1 true, label %try26.then, label %endtry1

try26.then:                                       ; preds = %try26
  br label %try27

try27:                                            ; preds = %try26.then
  br i1 true, label %try27.then, label %endtry

try27.then:                                       ; preds = %try27
  call void @middle()
  br label %thrown28

thrown28:                                         ; preds = %try27.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown28.then, label %endthrown

thrown28.then:                                    ; preds = %thrown28
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endthrown:                                        ; preds = %thrown28
  br label %endtry

endtry:                                           ; preds = %endthrown, %thrown28.then, %try27
  br label %catch29

catch29:                                          ; preds = %endtry
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown4" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown4", %EQ
  br i1 %AND, label %catch29.then, label %endcatch

catch29.then:                                     ; preds = %catch29
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  %"$*error.at" = load %Error, %Error* %"*error.at", align 8
  store %Error %"$*error.at", %Error* %e, align 8
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  %"$e.message" = load %String, %String* %e.message, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.2, i32 0, i32 0), %String %"$e.message")
  %e.message5 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message5)
  br label %endcatch

endcatch:                                         ; preds = %catch29.then, %catch29
  br label %rethrow30

rethrow30:                                        ; preds = %endcatch
  %"$ucg.thrown6" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown6", label %rethrow30.then, label %endrethrow

rethrow30.then:                                   ; preds = %rethrow30
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry1

endrethrow:                                       ; preds = %rethrow30
  %call.dprintf7 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.3, i32 0, i32 0))
  ret i32 0

endtry1:                                          ; preds = %rethrow30.then, %try26
  br label %catch31

catch31:                                          ; preds = %endtry1
  %"$ucg.error_type8" = load i32, i32* @ucg.error_type, align 4
  %EQ9 = icmp eq i32 %"$ucg.error_type8", 1
  %"$ucg.thrown10" = load i1, i1* @ucg.thrown, align 1
  %AND11 = and i1 %"$ucg.thrown10", %EQ9
  br i1 %AND11, label %catch31.then, label %endcatch3

catch31.then:                                     ; preds = %catch31
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at12, align 8
  %"*error.at13" = load %Error*, %Error** %error.at12, align 8
  %call.uncaught = call i32 @uncaught(%Error* %"*error.at13")
  br label %thrown32

thrown32:                                         ; preds = %catch31.then
  %"$ucg.thrown14" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown14", label %thrown32.then, label %endthrown2

thrown32.then:                                    ; preds = %thrown32
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown2:                                       ; preds = %thrown32
  ret i32 %call.uncaught

endcatch3:                                        ; preds = %catch31
  br label %if33

if33:                                             ; preds = %endcatch3
  %"$ucg.thrown15" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown15", label %if33.then, label %endif

if33.then:                                        ; preds = %if33
  ret i32 1

endif:                                            ; preds = %if33
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 005 — try: a rethrow inside a catch propagates outward

```ura
// stdout: mid: first
// stdout: main: second
// expect: 0

fn middle():
    try:
        throw Error::create(String::create("first"))
    catch e Error:
        output("mid: ", e.message, "\n")
        throw Error::create(String::create("second"))

fn main() i32:
    try:
        middle()
    catch e Error:
        output("main: ", e.message, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%String = type { i8*, i32, i32 }
%Error = type { %String }

@ucg.thrown = global i1 false
@ucg.error = global [2 x i64] zeroinitializer
@ucg.error_type = global i32 0
@str = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"first\00", align 1
@str.2 = private unnamed_addr constant [9 x i8] c"mid: %s\0A\00", align 1
@str.3 = private unnamed_addr constant [7 x i8] c"second\00", align 1
@str.4 = private unnamed_addr constant [10 x i8] c"main: %s\0A\00", align 1

define i8* @"alloc<i8>"(i32 %0) {
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
  %"*self5" = load %String*, %String** %self, align 8
  %"*self.count6" = getelementptr inbounds %String, %String* %"*self5", i32 0, i32 1
  %"$*self.count7" = load i32, i32* %"*self.count6", align 4
  store i32 %"$*self.count7", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 2
  %"$cap9" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap9", %"$*self.cap"
  br i1 %LE, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  ret void

endif1:                                           ; preds = %if1
  %"$cap10" = load i32, i32* %cap, align 4
  %"call.alloc<i8>" = call i8* @"alloc<i8>"(i32 %"$cap10")
  br label %thrown2

thrown2:                                          ; preds = %endif1
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown2.then, label %endthrown

thrown2.then:                                     ; preds = %thrown2
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown2
  store i8* %"call.alloc<i8>", i8** %bigger, align 8
  %"*self11" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self11", i32 0, i32 0
  %"*self12" = load %String*, %String** %self, align 8
  %"*self.count13" = getelementptr inbounds %String, %String* %"*self12", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count14" = load i32, i32* %"*self.count13", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count14")
  br label %thrown3

thrown3:                                          ; preds = %endthrown
  %"$ucg.thrown15" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown15", label %thrown3.then, label %endthrown2

thrown3.then:                                     ; preds = %thrown3
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown2:                                       ; preds = %thrown3
  br label %if4

if4:                                              ; preds = %endthrown2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.cap17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 2
  %"$*self.cap18" = load i32, i32* %"*self.cap17", align 4
  %GT = icmp sgt i32 %"$*self.cap18", 0
  br i1 %GT, label %if4.then, label %endif4

if4.then:                                         ; preds = %if4
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$*self.data21" = load i8*, i8** %"*self.data20", align 8
  call void @"free<i8>"(i8* %"$*self.data21")
  br label %thrown5

thrown5:                                          ; preds = %if4.then
  %"$ucg.thrown22" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown22", label %thrown5.then, label %endthrown3

thrown5.then:                                     ; preds = %thrown5
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown3:                                       ; preds = %thrown5
  br label %endif4

endif4:                                           ; preds = %endthrown3, %if4
  %"*self23" = load %String*, %String** %self, align 8
  %"*self.data24" = getelementptr inbounds %String, %String* %"*self23", i32 0, i32 0
  %"$bigger25" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger25", i8** %"*self.data24", align 8
  %"*self26" = load %String*, %String** %self, align 8
  %"*self.cap27" = getelementptr inbounds %String, %String* %"*self26", i32 0, i32 2
  %"$cap28" = load i32, i32* %cap, align 4
  store i32 %"$cap28", i32* %"*self.cap27", align 4
  ret void
}

define void @String.push_back(%String* %0, i8 %1) {
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
  br label %thrown7

thrown7:                                          ; preds = %if6.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown7.then, label %endthrown

thrown7.then:                                     ; preds = %thrown7
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown7
  br label %endif

endif:                                            ; preds = %endthrown, %if6
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

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if8

if8:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if8.then, label %endif

if8.then:                                         ; preds = %if8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if8.then, %if8
  br label %while9

while9:                                           ; preds = %while9.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while9.then, label %endloop

while9.then:                                      ; preds = %while9
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
  br label %while9

endloop:                                          ; preds = %while9
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
  br label %thrown10

thrown10:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown10.then, label %endthrown

thrown10.then:                                    ; preds = %thrown10
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown10
  %"*self2" = load %String*, %String** %self, align 8
  %"*other" = load %String*, %String** %other, align 8
  %"*other.count" = getelementptr inbounds %String, %String* %"*other", i32 0, i32 1
  %"$*other.count" = load i32, i32* %"*other.count", align 4
  call void @String.reserve(%String* %"*self2", i32 %"$*other.count")
  br label %thrown11

thrown11:                                         ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown11.then, label %endthrown1

thrown11.then:                                    ; preds = %thrown11
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown11
  store i32 0, i32* %i, align 4
  br label %while12

while12:                                          ; preds = %while12.then, %endthrown1
  %"*other4" = load %String*, %String** %other, align 8
  %"*other.count5" = getelementptr inbounds %String, %String* %"*other4", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*other.count6" = load i32, i32* %"*other.count5", align 4
  %LT = icmp slt i32 %"$i", %"$*other.count6"
  br i1 %LT, label %while12.then, label %endloop

while12.then:                                     ; preds = %while12
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i8" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i8"
  %"*other9" = load %String*, %String** %other, align 8
  %"*other.data" = getelementptr inbounds %String, %String* %"*other9", i32 0, i32 0
  %"$*other.data" = load i8*, i8** %"*other.data", align 8
  %"$i10" = load i32, i32* %i, align 4
  %"*other.data[]" = getelementptr inbounds i8, i8* %"$*other.data", i32 %"$i10"
  %"$*other.data[]" = load i8, i8* %"*other.data[]", align 1
  store i8 %"$*other.data[]", i8* %"*self.data[]", align 1
  %"$i11" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i11", 1
  store i32 %ADD, i32* %i, align 4
  br label %while12

endloop:                                          ; preds = %while12
  %"*self12" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self12", i32 0, i32 1
  %"*other13" = load %String*, %String** %other, align 8
  %"*other.count14" = getelementptr inbounds %String, %String* %"*other13", i32 0, i32 1
  %"$*other.count15" = load i32, i32* %"*other.count14", align 4
  store i32 %"$*other.count15", i32* %"*self.count", align 4
  ret void
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %thrown13

thrown13:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown13.then, label %endthrown

thrown13.then:                                    ; preds = %thrown13
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown13
  br label %if14

if14:                                             ; preds = %endthrown
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if14.then, label %endif

if14.then:                                        ; preds = %if14
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %thrown15

thrown15:                                         ; preds = %if14.then
  %"$ucg.thrown4" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown4", label %thrown15.then, label %endthrown1

thrown15.then:                                    ; preds = %thrown15
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown15
  br label %endif

endif:                                            ; preds = %endthrown1, %if14
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
  br label %thrown16

thrown16:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown16.then, label %endthrown

thrown16.then:                                    ; preds = %thrown16
  store i1 false, i1* @ucg.thrown, align 1
  call void @String.drop(%String* %res)
  store i1 true, i1* @ucg.thrown, align 1
  ret %String zeroinitializer

endthrown:                                        ; preds = %thrown16
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
  br label %while17

while17:                                          ; preds = %endthrown, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$i" = load i32, i32* %i, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$i"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while17.then, label %endloop

while17.then:                                     ; preds = %while17
  %"*self" = load %String*, %String** %self, align 8
  %"$text1" = load i8*, i8** %text, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"text[]3" = getelementptr inbounds i8, i8* %"$text1", i32 %"$i2"
  %"$text[]4" = load i8, i8* %"text[]3", align 1
  call void @String.push_back(%String* %"*self", i8 %"$text[]4")
  br label %thrown18

thrown18:                                         ; preds = %while17.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown18.then, label %endthrown

thrown18.then:                                    ; preds = %thrown18
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown18
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while17

endloop:                                          ; preds = %while17
  ret void
}

define i8* @String.c_str(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  br label %thrown19

thrown19:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown19.then, label %endthrown

thrown19.then:                                    ; preds = %thrown19
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i8* null

endthrown:                                        ; preds = %thrown19
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

define %Error @Error.create(%String %0) {
entry:
  %message = alloca %String, align 8
  %e = alloca %Error, align 8
  %copy = alloca %String, align 8
  store %String %0, %String* %message, align 8
  store %Error zeroinitializer, %Error* %e, align 8
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  store %String zeroinitializer, %String* %copy, align 8
  call void @"String.="(%String* %copy, %String* %message)
  br label %thrown20

thrown20:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown20.then, label %endthrown

thrown20.then:                                    ; preds = %thrown20
  store i1 false, i1* @ucg.thrown, align 1
  %e.message2 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message2)
  call void @String.drop(%String* %message)
  store i1 true, i1* @ucg.thrown, align 1
  ret %Error zeroinitializer

endthrown:                                        ; preds = %thrown20
  %"$copy" = load %String, %String* %copy, align 8
  call void @String.drop(%String* %e.message)
  br label %thrown21

thrown21:                                         ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown21.then, label %endthrown1

thrown21.then:                                    ; preds = %thrown21
  store i1 false, i1* @ucg.thrown, align 1
  %e.message4 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message4)
  call void @String.drop(%String* %message)
  store i1 true, i1* @ucg.thrown, align 1
  ret %Error zeroinitializer

endthrown1:                                       ; preds = %thrown21
  store %String %"$copy", %String* %e.message, align 8
  %"$e" = load %Error, %Error* %e, align 8
  call void @String.drop(%String* %message)
  ret %Error %"$e"
}

define i32 @uncaught(%Error* %0) {
entry:
  %error = alloca %Error*, align 8
  store %Error* %0, %Error** %error, align 8
  %"*error" = load %Error*, %Error** %error, align 8
  %"*error.message" = getelementptr inbounds %Error, %Error* %"*error", i32 0, i32 0
  %call.String.c_str = call i8* @String.c_str(%String* %"*error.message")
  br label %thrown22

thrown22:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown22.then, label %endthrown

thrown22.then:                                    ; preds = %thrown22
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown:                                        ; preds = %thrown22
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i8* %call.String.c_str)
  ret i32 1
}

define void @middle() {
entry:
  %error.at = alloca %Error*, align 8
  %e = alloca %Error, align 8
  %error.at6 = alloca %Error*, align 8
  %error.at14 = alloca %Error*, align 8
  br label %try23

try23:                                            ; preds = %entry
  br i1 true, label %try23.then, label %endtry

try23.then:                                       ; preds = %try23
  %call.String.create = call %String @String.create(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0))
  br label %thrown24

thrown24:                                         ; preds = %try23.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown24.then, label %endthrown

thrown24.then:                                    ; preds = %thrown24
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endthrown:                                        ; preds = %thrown24
  %call.Error.create = call %Error @Error.create(%String %call.String.create)
  br label %thrown25

thrown25:                                         ; preds = %endthrown
  %"$ucg.thrown4" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown4", label %thrown25.then, label %endthrown1

thrown25.then:                                    ; preds = %thrown25
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endthrown1:                                       ; preds = %thrown25
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  store %Error %call.Error.create, %Error* %"*error.at", align 8
  store i32 1, i32* @ucg.error_type, align 4
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endtry:                                           ; preds = %endthrown1, %thrown25.then, %thrown24.then, %try23
  br label %catch26

catch26:                                          ; preds = %endtry
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown5" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown5", %EQ
  br i1 %AND, label %catch26.then, label %endcatch

catch26.then:                                     ; preds = %catch26
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at6, align 8
  %"*error.at7" = load %Error*, %Error** %error.at6, align 8
  %"$*error.at" = load %Error, %Error* %"*error.at7", align 8
  store %Error %"$*error.at", %Error* %e, align 8
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  %"$e.message" = load %String, %String* %e.message, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.2, i32 0, i32 0), %String %"$e.message")
  %call.String.create8 = call %String @String.create(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.3, i32 0, i32 0))
  br label %thrown27

thrown27:                                         ; preds = %catch26.then
  %"$ucg.thrown9" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown9", label %thrown27.then, label %endthrown2

thrown27.then:                                    ; preds = %thrown27
  store i1 false, i1* @ucg.thrown, align 1
  %e.message10 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message10)
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown2:                                       ; preds = %thrown27
  %call.Error.create11 = call %Error @Error.create(%String %call.String.create8)
  br label %thrown28

thrown28:                                         ; preds = %endthrown2
  %"$ucg.thrown12" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown12", label %thrown28.then, label %endthrown3

thrown28.then:                                    ; preds = %thrown28
  store i1 false, i1* @ucg.thrown, align 1
  %e.message13 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message13)
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown3:                                       ; preds = %thrown28
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at14, align 8
  %"*error.at15" = load %Error*, %Error** %error.at14, align 8
  store %Error %call.Error.create11, %Error* %"*error.at15", align 8
  store i32 1, i32* @ucg.error_type, align 4
  store i1 false, i1* @ucg.thrown, align 1
  %e.message16 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message16)
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endcatch:                                         ; preds = %catch26
  br label %rethrow29

rethrow29:                                        ; preds = %endcatch
  %"$ucg.thrown17" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown17", label %rethrow29.then, label %endrethrow

rethrow29.then:                                   ; preds = %rethrow29
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endrethrow:                                       ; preds = %rethrow29
  ret void
}

define i32 @main() {
entry:
  %e = alloca %Error, align 8
  %error.at = alloca %Error*, align 8
  %error.at11 = alloca %Error*, align 8
  br label %try30

try30:                                            ; preds = %entry
  br i1 true, label %try30.then, label %endtry1

try30.then:                                       ; preds = %try30
  br label %try31

try31:                                            ; preds = %try30.then
  br i1 true, label %try31.then, label %endtry

try31.then:                                       ; preds = %try31
  call void @middle()
  br label %thrown32

thrown32:                                         ; preds = %try31.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown32.then, label %endthrown

thrown32.then:                                    ; preds = %thrown32
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endthrown:                                        ; preds = %thrown32
  br label %endtry

endtry:                                           ; preds = %endthrown, %thrown32.then, %try31
  br label %catch33

catch33:                                          ; preds = %endtry
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown4" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown4", %EQ
  br i1 %AND, label %catch33.then, label %endcatch

catch33.then:                                     ; preds = %catch33
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  %"$*error.at" = load %Error, %Error* %"*error.at", align 8
  store %Error %"$*error.at", %Error* %e, align 8
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  %"$e.message" = load %String, %String* %e.message, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.4, i32 0, i32 0), %String %"$e.message")
  %e.message5 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message5)
  br label %endcatch

endcatch:                                         ; preds = %catch33.then, %catch33
  br label %rethrow34

rethrow34:                                        ; preds = %endcatch
  %"$ucg.thrown6" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown6", label %rethrow34.then, label %endrethrow

rethrow34.then:                                   ; preds = %rethrow34
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry1

endrethrow:                                       ; preds = %rethrow34
  ret i32 0

endtry1:                                          ; preds = %rethrow34.then, %try30
  br label %catch35

catch35:                                          ; preds = %endtry1
  %"$ucg.error_type7" = load i32, i32* @ucg.error_type, align 4
  %EQ8 = icmp eq i32 %"$ucg.error_type7", 1
  %"$ucg.thrown9" = load i1, i1* @ucg.thrown, align 1
  %AND10 = and i1 %"$ucg.thrown9", %EQ8
  br i1 %AND10, label %catch35.then, label %endcatch3

catch35.then:                                     ; preds = %catch35
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at11, align 8
  %"*error.at12" = load %Error*, %Error** %error.at11, align 8
  %call.uncaught = call i32 @uncaught(%Error* %"*error.at12")
  br label %thrown36

thrown36:                                         ; preds = %catch35.then
  %"$ucg.thrown13" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown13", label %thrown36.then, label %endthrown2

thrown36.then:                                    ; preds = %thrown36
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown2:                                       ; preds = %thrown36
  ret i32 %call.uncaught

endcatch3:                                        ; preds = %catch35
  br label %if37

if37:                                             ; preds = %endcatch3
  %"$ucg.thrown14" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown14", label %if37.then, label %endif

if37.then:                                        ; preds = %if37
  ret i32 1

endif:                                            ; preds = %if37
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 006 — try: a nested try: the inner catch handles it

```ura
// stdout: inner: inner
// expect: 0

fn main() i32:
    try:
        try:
            throw Error::create(String::create("inner"))
        catch e Error:
            output("inner: ", e.message, "\n")
    catch e Error:
        output("outer: ", e.message, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%String = type { i8*, i32, i32 }
%Error = type { %String }

@ucg.thrown = global i1 false
@ucg.error = global [2 x i64] zeroinitializer
@ucg.error_type = global i32 0
@str = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"inner\00", align 1
@str.2 = private unnamed_addr constant [11 x i8] c"inner: %s\0A\00", align 1
@str.3 = private unnamed_addr constant [11 x i8] c"outer: %s\0A\00", align 1

define i8* @"alloc<i8>"(i32 %0) {
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
  %"*self5" = load %String*, %String** %self, align 8
  %"*self.count6" = getelementptr inbounds %String, %String* %"*self5", i32 0, i32 1
  %"$*self.count7" = load i32, i32* %"*self.count6", align 4
  store i32 %"$*self.count7", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 2
  %"$cap9" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap9", %"$*self.cap"
  br i1 %LE, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  ret void

endif1:                                           ; preds = %if1
  %"$cap10" = load i32, i32* %cap, align 4
  %"call.alloc<i8>" = call i8* @"alloc<i8>"(i32 %"$cap10")
  br label %thrown2

thrown2:                                          ; preds = %endif1
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown2.then, label %endthrown

thrown2.then:                                     ; preds = %thrown2
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown2
  store i8* %"call.alloc<i8>", i8** %bigger, align 8
  %"*self11" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self11", i32 0, i32 0
  %"*self12" = load %String*, %String** %self, align 8
  %"*self.count13" = getelementptr inbounds %String, %String* %"*self12", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count14" = load i32, i32* %"*self.count13", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count14")
  br label %thrown3

thrown3:                                          ; preds = %endthrown
  %"$ucg.thrown15" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown15", label %thrown3.then, label %endthrown2

thrown3.then:                                     ; preds = %thrown3
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown2:                                       ; preds = %thrown3
  br label %if4

if4:                                              ; preds = %endthrown2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.cap17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 2
  %"$*self.cap18" = load i32, i32* %"*self.cap17", align 4
  %GT = icmp sgt i32 %"$*self.cap18", 0
  br i1 %GT, label %if4.then, label %endif4

if4.then:                                         ; preds = %if4
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$*self.data21" = load i8*, i8** %"*self.data20", align 8
  call void @"free<i8>"(i8* %"$*self.data21")
  br label %thrown5

thrown5:                                          ; preds = %if4.then
  %"$ucg.thrown22" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown22", label %thrown5.then, label %endthrown3

thrown5.then:                                     ; preds = %thrown5
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown3:                                       ; preds = %thrown5
  br label %endif4

endif4:                                           ; preds = %endthrown3, %if4
  %"*self23" = load %String*, %String** %self, align 8
  %"*self.data24" = getelementptr inbounds %String, %String* %"*self23", i32 0, i32 0
  %"$bigger25" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger25", i8** %"*self.data24", align 8
  %"*self26" = load %String*, %String** %self, align 8
  %"*self.cap27" = getelementptr inbounds %String, %String* %"*self26", i32 0, i32 2
  %"$cap28" = load i32, i32* %cap, align 4
  store i32 %"$cap28", i32* %"*self.cap27", align 4
  ret void
}

define void @String.push_back(%String* %0, i8 %1) {
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
  br label %thrown7

thrown7:                                          ; preds = %if6.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown7.then, label %endthrown

thrown7.then:                                     ; preds = %thrown7
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown7
  br label %endif

endif:                                            ; preds = %endthrown, %if6
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

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if8

if8:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if8.then, label %endif

if8.then:                                         ; preds = %if8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if8.then, %if8
  br label %while9

while9:                                           ; preds = %while9.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while9.then, label %endloop

while9.then:                                      ; preds = %while9
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
  br label %while9

endloop:                                          ; preds = %while9
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
  br label %thrown10

thrown10:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown10.then, label %endthrown

thrown10.then:                                    ; preds = %thrown10
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown10
  %"*self2" = load %String*, %String** %self, align 8
  %"*other" = load %String*, %String** %other, align 8
  %"*other.count" = getelementptr inbounds %String, %String* %"*other", i32 0, i32 1
  %"$*other.count" = load i32, i32* %"*other.count", align 4
  call void @String.reserve(%String* %"*self2", i32 %"$*other.count")
  br label %thrown11

thrown11:                                         ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown11.then, label %endthrown1

thrown11.then:                                    ; preds = %thrown11
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown11
  store i32 0, i32* %i, align 4
  br label %while12

while12:                                          ; preds = %while12.then, %endthrown1
  %"*other4" = load %String*, %String** %other, align 8
  %"*other.count5" = getelementptr inbounds %String, %String* %"*other4", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*other.count6" = load i32, i32* %"*other.count5", align 4
  %LT = icmp slt i32 %"$i", %"$*other.count6"
  br i1 %LT, label %while12.then, label %endloop

while12.then:                                     ; preds = %while12
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i8" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i8"
  %"*other9" = load %String*, %String** %other, align 8
  %"*other.data" = getelementptr inbounds %String, %String* %"*other9", i32 0, i32 0
  %"$*other.data" = load i8*, i8** %"*other.data", align 8
  %"$i10" = load i32, i32* %i, align 4
  %"*other.data[]" = getelementptr inbounds i8, i8* %"$*other.data", i32 %"$i10"
  %"$*other.data[]" = load i8, i8* %"*other.data[]", align 1
  store i8 %"$*other.data[]", i8* %"*self.data[]", align 1
  %"$i11" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i11", 1
  store i32 %ADD, i32* %i, align 4
  br label %while12

endloop:                                          ; preds = %while12
  %"*self12" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self12", i32 0, i32 1
  %"*other13" = load %String*, %String** %other, align 8
  %"*other.count14" = getelementptr inbounds %String, %String* %"*other13", i32 0, i32 1
  %"$*other.count15" = load i32, i32* %"*other.count14", align 4
  store i32 %"$*other.count15", i32* %"*self.count", align 4
  ret void
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %thrown13

thrown13:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown13.then, label %endthrown

thrown13.then:                                    ; preds = %thrown13
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown13
  br label %if14

if14:                                             ; preds = %endthrown
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if14.then, label %endif

if14.then:                                        ; preds = %if14
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %thrown15

thrown15:                                         ; preds = %if14.then
  %"$ucg.thrown4" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown4", label %thrown15.then, label %endthrown1

thrown15.then:                                    ; preds = %thrown15
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown15
  br label %endif

endif:                                            ; preds = %endthrown1, %if14
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
  br label %thrown16

thrown16:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown16.then, label %endthrown

thrown16.then:                                    ; preds = %thrown16
  store i1 false, i1* @ucg.thrown, align 1
  call void @String.drop(%String* %res)
  store i1 true, i1* @ucg.thrown, align 1
  ret %String zeroinitializer

endthrown:                                        ; preds = %thrown16
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
  br label %while17

while17:                                          ; preds = %endthrown, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$i" = load i32, i32* %i, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$i"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while17.then, label %endloop

while17.then:                                     ; preds = %while17
  %"*self" = load %String*, %String** %self, align 8
  %"$text1" = load i8*, i8** %text, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"text[]3" = getelementptr inbounds i8, i8* %"$text1", i32 %"$i2"
  %"$text[]4" = load i8, i8* %"text[]3", align 1
  call void @String.push_back(%String* %"*self", i8 %"$text[]4")
  br label %thrown18

thrown18:                                         ; preds = %while17.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown18.then, label %endthrown

thrown18.then:                                    ; preds = %thrown18
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown18
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while17

endloop:                                          ; preds = %while17
  ret void
}

define i8* @String.c_str(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  br label %thrown19

thrown19:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown19.then, label %endthrown

thrown19.then:                                    ; preds = %thrown19
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i8* null

endthrown:                                        ; preds = %thrown19
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

define %Error @Error.create(%String %0) {
entry:
  %message = alloca %String, align 8
  %e = alloca %Error, align 8
  %copy = alloca %String, align 8
  store %String %0, %String* %message, align 8
  store %Error zeroinitializer, %Error* %e, align 8
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  store %String zeroinitializer, %String* %copy, align 8
  call void @"String.="(%String* %copy, %String* %message)
  br label %thrown20

thrown20:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown20.then, label %endthrown

thrown20.then:                                    ; preds = %thrown20
  store i1 false, i1* @ucg.thrown, align 1
  %e.message2 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message2)
  call void @String.drop(%String* %message)
  store i1 true, i1* @ucg.thrown, align 1
  ret %Error zeroinitializer

endthrown:                                        ; preds = %thrown20
  %"$copy" = load %String, %String* %copy, align 8
  call void @String.drop(%String* %e.message)
  br label %thrown21

thrown21:                                         ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown21.then, label %endthrown1

thrown21.then:                                    ; preds = %thrown21
  store i1 false, i1* @ucg.thrown, align 1
  %e.message4 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message4)
  call void @String.drop(%String* %message)
  store i1 true, i1* @ucg.thrown, align 1
  ret %Error zeroinitializer

endthrown1:                                       ; preds = %thrown21
  store %String %"$copy", %String* %e.message, align 8
  %"$e" = load %Error, %Error* %e, align 8
  call void @String.drop(%String* %message)
  ret %Error %"$e"
}

define i32 @uncaught(%Error* %0) {
entry:
  %error = alloca %Error*, align 8
  store %Error* %0, %Error** %error, align 8
  %"*error" = load %Error*, %Error** %error, align 8
  %"*error.message" = getelementptr inbounds %Error, %Error* %"*error", i32 0, i32 0
  %call.String.c_str = call i8* @String.c_str(%String* %"*error.message")
  br label %thrown22

thrown22:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown22.then, label %endthrown

thrown22.then:                                    ; preds = %thrown22
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown:                                        ; preds = %thrown22
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i8* %call.String.c_str)
  ret i32 1
}

define i32 @main() {
entry:
  %error.at = alloca %Error*, align 8
  %e = alloca %Error, align 8
  %error.at10 = alloca %Error*, align 8
  %e18 = alloca %Error, align 8
  %error.at19 = alloca %Error*, align 8
  %error.at31 = alloca %Error*, align 8
  br label %try23

try23:                                            ; preds = %entry
  br i1 true, label %try23.then, label %endtry5

try23.then:                                       ; preds = %try23
  br label %try24

try24:                                            ; preds = %try23.then
  br i1 true, label %try24.then, label %endtry2

try24.then:                                       ; preds = %try24
  br label %try25

try25:                                            ; preds = %try24.then
  br i1 true, label %try25.then, label %endtry

try25.then:                                       ; preds = %try25
  %call.String.create = call %String @String.create(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0))
  br label %thrown26

thrown26:                                         ; preds = %try25.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown26.then, label %endthrown

thrown26.then:                                    ; preds = %thrown26
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endthrown:                                        ; preds = %thrown26
  %call.Error.create = call %Error @Error.create(%String %call.String.create)
  br label %thrown27

thrown27:                                         ; preds = %endthrown
  %"$ucg.thrown8" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown8", label %thrown27.then, label %endthrown1

thrown27.then:                                    ; preds = %thrown27
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endthrown1:                                       ; preds = %thrown27
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  store %Error %call.Error.create, %Error* %"*error.at", align 8
  store i32 1, i32* @ucg.error_type, align 4
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endtry:                                           ; preds = %endthrown1, %thrown27.then, %thrown26.then, %try25
  br label %catch28

catch28:                                          ; preds = %endtry
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown9" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown9", %EQ
  br i1 %AND, label %catch28.then, label %endcatch

catch28.then:                                     ; preds = %catch28
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at10, align 8
  %"*error.at11" = load %Error*, %Error** %error.at10, align 8
  %"$*error.at" = load %Error, %Error* %"*error.at11", align 8
  store %Error %"$*error.at", %Error* %e, align 8
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  %"$e.message" = load %String, %String* %e.message, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.2, i32 0, i32 0), %String %"$e.message")
  %e.message12 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message12)
  br label %endcatch

endcatch:                                         ; preds = %catch28.then, %catch28
  br label %rethrow29

rethrow29:                                        ; preds = %endcatch
  %"$ucg.thrown13" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown13", label %rethrow29.then, label %endrethrow

rethrow29.then:                                   ; preds = %rethrow29
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry2

endrethrow:                                       ; preds = %rethrow29
  br label %endtry2

endtry2:                                          ; preds = %endrethrow, %rethrow29.then, %try24
  br label %catch30

catch30:                                          ; preds = %endtry2
  %"$ucg.error_type14" = load i32, i32* @ucg.error_type, align 4
  %EQ15 = icmp eq i32 %"$ucg.error_type14", 1
  %"$ucg.thrown16" = load i1, i1* @ucg.thrown, align 1
  %AND17 = and i1 %"$ucg.thrown16", %EQ15
  br i1 %AND17, label %catch30.then, label %endcatch3

catch30.then:                                     ; preds = %catch30
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at19, align 8
  %"*error.at20" = load %Error*, %Error** %error.at19, align 8
  %"$*error.at21" = load %Error, %Error* %"*error.at20", align 8
  store %Error %"$*error.at21", %Error* %e18, align 8
  %e.message22 = getelementptr inbounds %Error, %Error* %e18, i32 0, i32 0
  %"$e.message23" = load %String, %String* %e.message22, align 8
  %call.dprintf24 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.3, i32 0, i32 0), %String %"$e.message23")
  %e.message25 = getelementptr inbounds %Error, %Error* %e18, i32 0, i32 0
  call void @String.drop(%String* %e.message25)
  br label %endcatch3

endcatch3:                                        ; preds = %catch30.then, %catch30
  br label %rethrow31

rethrow31:                                        ; preds = %endcatch3
  %"$ucg.thrown26" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown26", label %rethrow31.then, label %endrethrow4

rethrow31.then:                                   ; preds = %rethrow31
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry5

endrethrow4:                                      ; preds = %rethrow31
  ret i32 0

endtry5:                                          ; preds = %rethrow31.then, %try23
  br label %catch32

catch32:                                          ; preds = %endtry5
  %"$ucg.error_type27" = load i32, i32* @ucg.error_type, align 4
  %EQ28 = icmp eq i32 %"$ucg.error_type27", 1
  %"$ucg.thrown29" = load i1, i1* @ucg.thrown, align 1
  %AND30 = and i1 %"$ucg.thrown29", %EQ28
  br i1 %AND30, label %catch32.then, label %endcatch7

catch32.then:                                     ; preds = %catch32
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at31, align 8
  %"*error.at32" = load %Error*, %Error** %error.at31, align 8
  %call.uncaught = call i32 @uncaught(%Error* %"*error.at32")
  br label %thrown33

thrown33:                                         ; preds = %catch32.then
  %"$ucg.thrown33" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown33", label %thrown33.then, label %endthrown6

thrown33.then:                                    ; preds = %thrown33
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown6:                                       ; preds = %thrown33
  ret i32 %call.uncaught

endcatch7:                                        ; preds = %catch32
  br label %if34

if34:                                             ; preds = %endcatch7
  %"$ucg.thrown34" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown34", label %if34.then, label %endif

if34.then:                                        ; preds = %if34
  ret i32 1

endif:                                            ; preds = %if34
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 007 — try: return inside a try returns from the function

```ura
// stdout: ret = 5
// expect: 0

fn pick() i32:
    try:
        return 5
    catch e Error:
        return 0

fn main() i32:
    output("ret = ", pick(), "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%String = type { i8*, i32, i32 }
%Error = type { %String }

@ucg.thrown = global i1 false
@ucg.error_type = global i32 0
@ucg.error = global [2 x i64] zeroinitializer
@str = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c"ret = %d\0A\00", align 1

define i8* @"alloc<i8>"(i32 %0) {
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
  %"*self5" = load %String*, %String** %self, align 8
  %"*self.count6" = getelementptr inbounds %String, %String* %"*self5", i32 0, i32 1
  %"$*self.count7" = load i32, i32* %"*self.count6", align 4
  store i32 %"$*self.count7", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 2
  %"$cap9" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap9", %"$*self.cap"
  br i1 %LE, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  ret void

endif1:                                           ; preds = %if1
  %"$cap10" = load i32, i32* %cap, align 4
  %"call.alloc<i8>" = call i8* @"alloc<i8>"(i32 %"$cap10")
  br label %thrown2

thrown2:                                          ; preds = %endif1
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown2.then, label %endthrown

thrown2.then:                                     ; preds = %thrown2
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown2
  store i8* %"call.alloc<i8>", i8** %bigger, align 8
  %"*self11" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self11", i32 0, i32 0
  %"*self12" = load %String*, %String** %self, align 8
  %"*self.count13" = getelementptr inbounds %String, %String* %"*self12", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count14" = load i32, i32* %"*self.count13", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count14")
  br label %thrown3

thrown3:                                          ; preds = %endthrown
  %"$ucg.thrown15" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown15", label %thrown3.then, label %endthrown2

thrown3.then:                                     ; preds = %thrown3
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown2:                                       ; preds = %thrown3
  br label %if4

if4:                                              ; preds = %endthrown2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.cap17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 2
  %"$*self.cap18" = load i32, i32* %"*self.cap17", align 4
  %GT = icmp sgt i32 %"$*self.cap18", 0
  br i1 %GT, label %if4.then, label %endif4

if4.then:                                         ; preds = %if4
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$*self.data21" = load i8*, i8** %"*self.data20", align 8
  call void @"free<i8>"(i8* %"$*self.data21")
  br label %thrown5

thrown5:                                          ; preds = %if4.then
  %"$ucg.thrown22" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown22", label %thrown5.then, label %endthrown3

thrown5.then:                                     ; preds = %thrown5
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown3:                                       ; preds = %thrown5
  br label %endif4

endif4:                                           ; preds = %endthrown3, %if4
  %"*self23" = load %String*, %String** %self, align 8
  %"*self.data24" = getelementptr inbounds %String, %String* %"*self23", i32 0, i32 0
  %"$bigger25" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger25", i8** %"*self.data24", align 8
  %"*self26" = load %String*, %String** %self, align 8
  %"*self.cap27" = getelementptr inbounds %String, %String* %"*self26", i32 0, i32 2
  %"$cap28" = load i32, i32* %cap, align 4
  store i32 %"$cap28", i32* %"*self.cap27", align 4
  ret void
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if6

if6:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  br label %while7

while7:                                           ; preds = %while7.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while7.then, label %endloop

while7.then:                                      ; preds = %while7
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
  br label %while7

endloop:                                          ; preds = %while7
  ret void
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %thrown8

thrown8:                                          ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown8.then, label %endthrown

thrown8.then:                                     ; preds = %thrown8
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown8
  br label %if9

if9:                                              ; preds = %endthrown
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if9.then, label %endif

if9.then:                                         ; preds = %if9
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %thrown10

thrown10:                                         ; preds = %if9.then
  %"$ucg.thrown4" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown4", label %thrown10.then, label %endthrown1

thrown10.then:                                    ; preds = %thrown10
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown10
  br label %endif

endif:                                            ; preds = %endthrown1, %if9
  ret void
}

define i8* @String.c_str(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  br label %thrown11

thrown11:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown11.then, label %endthrown

thrown11.then:                                    ; preds = %thrown11
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i8* null

endthrown:                                        ; preds = %thrown11
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

define i32 @uncaught(%Error* %0) {
entry:
  %error = alloca %Error*, align 8
  store %Error* %0, %Error** %error, align 8
  %"*error" = load %Error*, %Error** %error, align 8
  %"*error.message" = getelementptr inbounds %Error, %Error* %"*error", i32 0, i32 0
  %call.String.c_str = call i8* @String.c_str(%String* %"*error.message")
  br label %thrown12

thrown12:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown12.then, label %endthrown

thrown12.then:                                    ; preds = %thrown12
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown:                                        ; preds = %thrown12
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i8* %call.String.c_str)
  ret i32 1
}

define i32 @pick() {
entry:
  %e = alloca %Error, align 8
  %error.at = alloca %Error*, align 8
  br label %try13

try13:                                            ; preds = %entry
  br i1 true, label %try13.then, label %endtry

try13.then:                                       ; preds = %try13
  ret i32 5

endtry:                                           ; preds = %try13
  br label %catch14

catch14:                                          ; preds = %endtry
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown", %EQ
  br i1 %AND, label %catch14.then, label %endcatch

catch14.then:                                     ; preds = %catch14
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  %"$*error.at" = load %Error, %Error* %"*error.at", align 8
  store %Error %"$*error.at", %Error* %e, align 8
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message)
  ret i32 0

endcatch:                                         ; preds = %catch14
  br label %rethrow15

rethrow15:                                        ; preds = %endcatch
  %"$ucg.thrown1" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown1", label %rethrow15.then, label %endrethrow

rethrow15.then:                                   ; preds = %rethrow15
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endrethrow:                                       ; preds = %rethrow15
  ret i32 0
}

define i32 @main() {
entry:
  %error.at = alloca %Error*, align 8
  br label %try16

try16:                                            ; preds = %entry
  br i1 true, label %try16.then, label %endtry

try16.then:                                       ; preds = %try16
  %call.pick = call i32 @pick()
  br label %thrown17

thrown17:                                         ; preds = %try16.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown17.then, label %endthrown

thrown17.then:                                    ; preds = %thrown17
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endthrown:                                        ; preds = %thrown17
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i32 %call.pick)
  ret i32 0

endtry:                                           ; preds = %thrown17.then, %try16
  br label %catch18

catch18:                                          ; preds = %endtry
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown2" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown2", %EQ
  br i1 %AND, label %catch18.then, label %endcatch

catch18.then:                                     ; preds = %catch18
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  %call.uncaught = call i32 @uncaught(%Error* %"*error.at")
  br label %thrown19

thrown19:                                         ; preds = %catch18.then
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown19.then, label %endthrown1

thrown19.then:                                    ; preds = %thrown19
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown1:                                       ; preds = %thrown19
  ret i32 %call.uncaught

endcatch:                                         ; preds = %catch18
  br label %if20

if20:                                             ; preds = %endcatch
  %"$ucg.thrown4" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown4", label %if20.then, label %endif

if20.then:                                        ; preds = %if20
  ret i32 1

endif:                                            ; preds = %if20
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 008 — try: break inside a try exits the enclosing loop

```ura
// stdout: i=0
// stdout: done
// expect: 0

fn main() i32:
    i i32 = 0
    while i < 3:
        try:
            if i == 1:
                break
            output("i=", i, "\n")
        catch e Error:
            output("x\n")
        i += 1
    output("done\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%String = type { i8*, i32, i32 }
%Error = type { %String }

@ucg.thrown = global i1 false
@ucg.error_type = global i32 0
@ucg.error = global [2 x i64] zeroinitializer
@str = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"i=%d\0A\00", align 1
@str.2 = private unnamed_addr constant [3 x i8] c"x\0A\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"done\0A\00", align 1

define i8* @"alloc<i8>"(i32 %0) {
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
  %"*self5" = load %String*, %String** %self, align 8
  %"*self.count6" = getelementptr inbounds %String, %String* %"*self5", i32 0, i32 1
  %"$*self.count7" = load i32, i32* %"*self.count6", align 4
  store i32 %"$*self.count7", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 2
  %"$cap9" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap9", %"$*self.cap"
  br i1 %LE, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  ret void

endif1:                                           ; preds = %if1
  %"$cap10" = load i32, i32* %cap, align 4
  %"call.alloc<i8>" = call i8* @"alloc<i8>"(i32 %"$cap10")
  br label %thrown2

thrown2:                                          ; preds = %endif1
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown2.then, label %endthrown

thrown2.then:                                     ; preds = %thrown2
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown2
  store i8* %"call.alloc<i8>", i8** %bigger, align 8
  %"*self11" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self11", i32 0, i32 0
  %"*self12" = load %String*, %String** %self, align 8
  %"*self.count13" = getelementptr inbounds %String, %String* %"*self12", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count14" = load i32, i32* %"*self.count13", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count14")
  br label %thrown3

thrown3:                                          ; preds = %endthrown
  %"$ucg.thrown15" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown15", label %thrown3.then, label %endthrown2

thrown3.then:                                     ; preds = %thrown3
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown2:                                       ; preds = %thrown3
  br label %if4

if4:                                              ; preds = %endthrown2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.cap17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 2
  %"$*self.cap18" = load i32, i32* %"*self.cap17", align 4
  %GT = icmp sgt i32 %"$*self.cap18", 0
  br i1 %GT, label %if4.then, label %endif4

if4.then:                                         ; preds = %if4
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$*self.data21" = load i8*, i8** %"*self.data20", align 8
  call void @"free<i8>"(i8* %"$*self.data21")
  br label %thrown5

thrown5:                                          ; preds = %if4.then
  %"$ucg.thrown22" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown22", label %thrown5.then, label %endthrown3

thrown5.then:                                     ; preds = %thrown5
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown3:                                       ; preds = %thrown5
  br label %endif4

endif4:                                           ; preds = %endthrown3, %if4
  %"*self23" = load %String*, %String** %self, align 8
  %"*self.data24" = getelementptr inbounds %String, %String* %"*self23", i32 0, i32 0
  %"$bigger25" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger25", i8** %"*self.data24", align 8
  %"*self26" = load %String*, %String** %self, align 8
  %"*self.cap27" = getelementptr inbounds %String, %String* %"*self26", i32 0, i32 2
  %"$cap28" = load i32, i32* %cap, align 4
  store i32 %"$cap28", i32* %"*self.cap27", align 4
  ret void
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if6

if6:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  br label %while7

while7:                                           ; preds = %while7.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while7.then, label %endloop

while7.then:                                      ; preds = %while7
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
  br label %while7

endloop:                                          ; preds = %while7
  ret void
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %thrown8

thrown8:                                          ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown8.then, label %endthrown

thrown8.then:                                     ; preds = %thrown8
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown8
  br label %if9

if9:                                              ; preds = %endthrown
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if9.then, label %endif

if9.then:                                         ; preds = %if9
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %thrown10

thrown10:                                         ; preds = %if9.then
  %"$ucg.thrown4" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown4", label %thrown10.then, label %endthrown1

thrown10.then:                                    ; preds = %thrown10
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown10
  br label %endif

endif:                                            ; preds = %endthrown1, %if9
  ret void
}

define i8* @String.c_str(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  br label %thrown11

thrown11:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown11.then, label %endthrown

thrown11.then:                                    ; preds = %thrown11
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i8* null

endthrown:                                        ; preds = %thrown11
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

define i32 @uncaught(%Error* %0) {
entry:
  %error = alloca %Error*, align 8
  store %Error* %0, %Error** %error, align 8
  %"*error" = load %Error*, %Error** %error, align 8
  %"*error.message" = getelementptr inbounds %Error, %Error* %"*error", i32 0, i32 0
  %call.String.c_str = call i8* @String.c_str(%String* %"*error.message")
  br label %thrown12

thrown12:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown12.then, label %endthrown

thrown12.then:                                    ; preds = %thrown12
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown:                                        ; preds = %thrown12
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i8* %call.String.c_str)
  ret i32 1
}

define i32 @main() {
entry:
  %i = alloca i32, align 4
  %e = alloca %Error, align 8
  %error.at = alloca %Error*, align 8
  %error.at15 = alloca %Error*, align 8
  br label %try13

try13:                                            ; preds = %entry
  br i1 true, label %try13.then, label %endtry1

try13.then:                                       ; preds = %try13
  store i32 0, i32* %i, align 4
  br label %while14

while14:                                          ; preds = %endrethrow, %try13.then
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while14.then, label %endloop

while14.then:                                     ; preds = %while14
  br label %try15

try15:                                            ; preds = %while14.then
  br i1 true, label %try15.then, label %endtry

try15.then:                                       ; preds = %try15
  br label %if16

if16:                                             ; preds = %try15.then
  %"$i4" = load i32, i32* %i, align 4
  %EQ = icmp eq i32 %"$i4", 1
  br i1 %EQ, label %if16.then, label %endif

if16.then:                                        ; preds = %if16
  br label %endloop

endif:                                            ; preds = %if16
  %"$i5" = load i32, i32* %i, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i32 %"$i5")
  br label %endtry

endtry:                                           ; preds = %endif, %try15
  br label %catch17

catch17:                                          ; preds = %endtry
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ6 = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown", %EQ6
  br i1 %AND, label %catch17.then, label %endcatch

catch17.then:                                     ; preds = %catch17
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  %"$*error.at" = load %Error, %Error* %"*error.at", align 8
  store %Error %"$*error.at", %Error* %e, align 8
  %call.dprintf7 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.2, i32 0, i32 0))
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message)
  br label %endcatch

endcatch:                                         ; preds = %catch17.then, %catch17
  br label %rethrow18

rethrow18:                                        ; preds = %endcatch
  %"$ucg.thrown8" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown8", label %rethrow18.then, label %endrethrow

rethrow18.then:                                   ; preds = %rethrow18
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry1

endrethrow:                                       ; preds = %rethrow18
  %"$i9" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i9", 1
  store i32 %ADD, i32* %i, align 4
  br label %while14

endloop:                                          ; preds = %if16.then, %while14
  %call.dprintf10 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0))
  ret i32 0

endtry1:                                          ; preds = %rethrow18.then, %try13
  br label %catch19

catch19:                                          ; preds = %endtry1
  %"$ucg.error_type11" = load i32, i32* @ucg.error_type, align 4
  %EQ12 = icmp eq i32 %"$ucg.error_type11", 1
  %"$ucg.thrown13" = load i1, i1* @ucg.thrown, align 1
  %AND14 = and i1 %"$ucg.thrown13", %EQ12
  br i1 %AND14, label %catch19.then, label %endcatch2

catch19.then:                                     ; preds = %catch19
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at15, align 8
  %"*error.at16" = load %Error*, %Error** %error.at15, align 8
  %call.uncaught = call i32 @uncaught(%Error* %"*error.at16")
  br label %thrown20

thrown20:                                         ; preds = %catch19.then
  %"$ucg.thrown17" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown17", label %thrown20.then, label %endthrown

thrown20.then:                                    ; preds = %thrown20
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown:                                        ; preds = %thrown20
  ret i32 %call.uncaught

endcatch2:                                        ; preds = %catch19
  br label %if21

if21:                                             ; preds = %endcatch2
  %"$ucg.thrown18" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown18", label %if21.then, label %endif3

if21.then:                                        ; preds = %if21
  ret i32 1

endif3:                                           ; preds = %if21
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 009 — try: an uncaught throw prints its message and exits

```ura
// stderr: uncaught
// expect: 1

fn main() i32:
    throw Error::create(String::create("uncaught\n"))
    output("unreachable\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%String = type { i8*, i32, i32 }
%Error = type { %String }

@ucg.thrown = global i1 false
@ucg.error = global [2 x i64] zeroinitializer
@ucg.error_type = global i32 0
@str = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c"uncaught\0A\00", align 1

define i8* @"alloc<i8>"(i32 %0) {
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
  %"*self5" = load %String*, %String** %self, align 8
  %"*self.count6" = getelementptr inbounds %String, %String* %"*self5", i32 0, i32 1
  %"$*self.count7" = load i32, i32* %"*self.count6", align 4
  store i32 %"$*self.count7", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 2
  %"$cap9" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap9", %"$*self.cap"
  br i1 %LE, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  ret void

endif1:                                           ; preds = %if1
  %"$cap10" = load i32, i32* %cap, align 4
  %"call.alloc<i8>" = call i8* @"alloc<i8>"(i32 %"$cap10")
  br label %thrown2

thrown2:                                          ; preds = %endif1
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown2.then, label %endthrown

thrown2.then:                                     ; preds = %thrown2
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown2
  store i8* %"call.alloc<i8>", i8** %bigger, align 8
  %"*self11" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self11", i32 0, i32 0
  %"*self12" = load %String*, %String** %self, align 8
  %"*self.count13" = getelementptr inbounds %String, %String* %"*self12", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count14" = load i32, i32* %"*self.count13", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count14")
  br label %thrown3

thrown3:                                          ; preds = %endthrown
  %"$ucg.thrown15" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown15", label %thrown3.then, label %endthrown2

thrown3.then:                                     ; preds = %thrown3
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown2:                                       ; preds = %thrown3
  br label %if4

if4:                                              ; preds = %endthrown2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.cap17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 2
  %"$*self.cap18" = load i32, i32* %"*self.cap17", align 4
  %GT = icmp sgt i32 %"$*self.cap18", 0
  br i1 %GT, label %if4.then, label %endif4

if4.then:                                         ; preds = %if4
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$*self.data21" = load i8*, i8** %"*self.data20", align 8
  call void @"free<i8>"(i8* %"$*self.data21")
  br label %thrown5

thrown5:                                          ; preds = %if4.then
  %"$ucg.thrown22" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown22", label %thrown5.then, label %endthrown3

thrown5.then:                                     ; preds = %thrown5
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown3:                                       ; preds = %thrown5
  br label %endif4

endif4:                                           ; preds = %endthrown3, %if4
  %"*self23" = load %String*, %String** %self, align 8
  %"*self.data24" = getelementptr inbounds %String, %String* %"*self23", i32 0, i32 0
  %"$bigger25" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger25", i8** %"*self.data24", align 8
  %"*self26" = load %String*, %String** %self, align 8
  %"*self.cap27" = getelementptr inbounds %String, %String* %"*self26", i32 0, i32 2
  %"$cap28" = load i32, i32* %cap, align 4
  store i32 %"$cap28", i32* %"*self.cap27", align 4
  ret void
}

define void @String.push_back(%String* %0, i8 %1) {
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
  br label %thrown7

thrown7:                                          ; preds = %if6.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown7.then, label %endthrown

thrown7.then:                                     ; preds = %thrown7
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown7
  br label %endif

endif:                                            ; preds = %endthrown, %if6
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

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if8

if8:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if8.then, label %endif

if8.then:                                         ; preds = %if8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if8.then, %if8
  br label %while9

while9:                                           ; preds = %while9.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while9.then, label %endloop

while9.then:                                      ; preds = %while9
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
  br label %while9

endloop:                                          ; preds = %while9
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
  br label %thrown10

thrown10:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown10.then, label %endthrown

thrown10.then:                                    ; preds = %thrown10
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown10
  %"*self2" = load %String*, %String** %self, align 8
  %"*other" = load %String*, %String** %other, align 8
  %"*other.count" = getelementptr inbounds %String, %String* %"*other", i32 0, i32 1
  %"$*other.count" = load i32, i32* %"*other.count", align 4
  call void @String.reserve(%String* %"*self2", i32 %"$*other.count")
  br label %thrown11

thrown11:                                         ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown11.then, label %endthrown1

thrown11.then:                                    ; preds = %thrown11
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown11
  store i32 0, i32* %i, align 4
  br label %while12

while12:                                          ; preds = %while12.then, %endthrown1
  %"*other4" = load %String*, %String** %other, align 8
  %"*other.count5" = getelementptr inbounds %String, %String* %"*other4", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*other.count6" = load i32, i32* %"*other.count5", align 4
  %LT = icmp slt i32 %"$i", %"$*other.count6"
  br i1 %LT, label %while12.then, label %endloop

while12.then:                                     ; preds = %while12
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i8" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i8"
  %"*other9" = load %String*, %String** %other, align 8
  %"*other.data" = getelementptr inbounds %String, %String* %"*other9", i32 0, i32 0
  %"$*other.data" = load i8*, i8** %"*other.data", align 8
  %"$i10" = load i32, i32* %i, align 4
  %"*other.data[]" = getelementptr inbounds i8, i8* %"$*other.data", i32 %"$i10"
  %"$*other.data[]" = load i8, i8* %"*other.data[]", align 1
  store i8 %"$*other.data[]", i8* %"*self.data[]", align 1
  %"$i11" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i11", 1
  store i32 %ADD, i32* %i, align 4
  br label %while12

endloop:                                          ; preds = %while12
  %"*self12" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self12", i32 0, i32 1
  %"*other13" = load %String*, %String** %other, align 8
  %"*other.count14" = getelementptr inbounds %String, %String* %"*other13", i32 0, i32 1
  %"$*other.count15" = load i32, i32* %"*other.count14", align 4
  store i32 %"$*other.count15", i32* %"*self.count", align 4
  ret void
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %thrown13

thrown13:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown13.then, label %endthrown

thrown13.then:                                    ; preds = %thrown13
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown13
  br label %if14

if14:                                             ; preds = %endthrown
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if14.then, label %endif

if14.then:                                        ; preds = %if14
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %thrown15

thrown15:                                         ; preds = %if14.then
  %"$ucg.thrown4" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown4", label %thrown15.then, label %endthrown1

thrown15.then:                                    ; preds = %thrown15
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown15
  br label %endif

endif:                                            ; preds = %endthrown1, %if14
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
  br label %thrown16

thrown16:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown16.then, label %endthrown

thrown16.then:                                    ; preds = %thrown16
  store i1 false, i1* @ucg.thrown, align 1
  call void @String.drop(%String* %res)
  store i1 true, i1* @ucg.thrown, align 1
  ret %String zeroinitializer

endthrown:                                        ; preds = %thrown16
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
  br label %while17

while17:                                          ; preds = %endthrown, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$i" = load i32, i32* %i, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$i"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while17.then, label %endloop

while17.then:                                     ; preds = %while17
  %"*self" = load %String*, %String** %self, align 8
  %"$text1" = load i8*, i8** %text, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"text[]3" = getelementptr inbounds i8, i8* %"$text1", i32 %"$i2"
  %"$text[]4" = load i8, i8* %"text[]3", align 1
  call void @String.push_back(%String* %"*self", i8 %"$text[]4")
  br label %thrown18

thrown18:                                         ; preds = %while17.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown18.then, label %endthrown

thrown18.then:                                    ; preds = %thrown18
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown18
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while17

endloop:                                          ; preds = %while17
  ret void
}

define i8* @String.c_str(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  br label %thrown19

thrown19:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown19.then, label %endthrown

thrown19.then:                                    ; preds = %thrown19
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i8* null

endthrown:                                        ; preds = %thrown19
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

define %Error @Error.create(%String %0) {
entry:
  %message = alloca %String, align 8
  %e = alloca %Error, align 8
  %copy = alloca %String, align 8
  store %String %0, %String* %message, align 8
  store %Error zeroinitializer, %Error* %e, align 8
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  store %String zeroinitializer, %String* %copy, align 8
  call void @"String.="(%String* %copy, %String* %message)
  br label %thrown20

thrown20:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown20.then, label %endthrown

thrown20.then:                                    ; preds = %thrown20
  store i1 false, i1* @ucg.thrown, align 1
  %e.message2 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message2)
  call void @String.drop(%String* %message)
  store i1 true, i1* @ucg.thrown, align 1
  ret %Error zeroinitializer

endthrown:                                        ; preds = %thrown20
  %"$copy" = load %String, %String* %copy, align 8
  call void @String.drop(%String* %e.message)
  br label %thrown21

thrown21:                                         ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown21.then, label %endthrown1

thrown21.then:                                    ; preds = %thrown21
  store i1 false, i1* @ucg.thrown, align 1
  %e.message4 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message4)
  call void @String.drop(%String* %message)
  store i1 true, i1* @ucg.thrown, align 1
  ret %Error zeroinitializer

endthrown1:                                       ; preds = %thrown21
  store %String %"$copy", %String* %e.message, align 8
  %"$e" = load %Error, %Error* %e, align 8
  call void @String.drop(%String* %message)
  ret %Error %"$e"
}

define i32 @uncaught(%Error* %0) {
entry:
  %error = alloca %Error*, align 8
  store %Error* %0, %Error** %error, align 8
  %"*error" = load %Error*, %Error** %error, align 8
  %"*error.message" = getelementptr inbounds %Error, %Error* %"*error", i32 0, i32 0
  %call.String.c_str = call i8* @String.c_str(%String* %"*error.message")
  br label %thrown22

thrown22:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown22.then, label %endthrown

thrown22.then:                                    ; preds = %thrown22
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown:                                        ; preds = %thrown22
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i8* %call.String.c_str)
  ret i32 1
}

define i32 @main() {
entry:
  %error.at = alloca %Error*, align 8
  %error.at5 = alloca %Error*, align 8
  br label %try23

try23:                                            ; preds = %entry
  br i1 true, label %try23.then, label %endtry

try23.then:                                       ; preds = %try23
  %call.String.create = call %String @String.create(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0))
  br label %thrown24

thrown24:                                         ; preds = %try23.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown24.then, label %endthrown

thrown24.then:                                    ; preds = %thrown24
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endthrown:                                        ; preds = %thrown24
  %call.Error.create = call %Error @Error.create(%String %call.String.create)
  br label %thrown25

thrown25:                                         ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown25.then, label %endthrown1

thrown25.then:                                    ; preds = %thrown25
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endthrown1:                                       ; preds = %thrown25
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  store %Error %call.Error.create, %Error* %"*error.at", align 8
  store i32 1, i32* @ucg.error_type, align 4
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endtry:                                           ; preds = %endthrown1, %thrown25.then, %thrown24.then, %try23
  br label %catch26

catch26:                                          ; preds = %endtry
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown4" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown4", %EQ
  br i1 %AND, label %catch26.then, label %endcatch

catch26.then:                                     ; preds = %catch26
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at5, align 8
  %"*error.at6" = load %Error*, %Error** %error.at5, align 8
  %call.uncaught = call i32 @uncaught(%Error* %"*error.at6")
  br label %thrown27

thrown27:                                         ; preds = %catch26.then
  %"$ucg.thrown7" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown7", label %thrown27.then, label %endthrown2

thrown27.then:                                    ; preds = %thrown27
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown2:                                       ; preds = %thrown27
  ret i32 %call.uncaught

endcatch:                                         ; preds = %catch26
  br label %if28

if28:                                             ; preds = %endcatch
  %"$ucg.thrown8" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown8", label %if28.then, label %endif

if28.then:                                        ; preds = %if28
  ret i32 1

endif:                                            ; preds = %if28
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 010 — try: any type can be thrown; a catch takes the type it names

```ura
// stdout: code 42
// stdout: after
// expect: 0

fn fail():
    throw 42

fn main() i32:
    try:
        fail()
    catch e Error:
        output("error ", e.message, "\n")
    catch code i32:
        output("code ", code, "\n")
    output("after\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%String = type { i8*, i32, i32 }
%Error = type { %String }

@ucg.thrown = global i1 false
@ucg.error = global [2 x i64] zeroinitializer
@ucg.error_type = global i32 0
@str = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c"error %s\0A\00", align 1
@str.2 = private unnamed_addr constant [9 x i8] c"code %d\0A\00", align 1
@str.3 = private unnamed_addr constant [7 x i8] c"after\0A\00", align 1

define i8* @"alloc<i8>"(i32 %0) {
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
  %"*self5" = load %String*, %String** %self, align 8
  %"*self.count6" = getelementptr inbounds %String, %String* %"*self5", i32 0, i32 1
  %"$*self.count7" = load i32, i32* %"*self.count6", align 4
  store i32 %"$*self.count7", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 2
  %"$cap9" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap9", %"$*self.cap"
  br i1 %LE, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  ret void

endif1:                                           ; preds = %if1
  %"$cap10" = load i32, i32* %cap, align 4
  %"call.alloc<i8>" = call i8* @"alloc<i8>"(i32 %"$cap10")
  br label %thrown2

thrown2:                                          ; preds = %endif1
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown2.then, label %endthrown

thrown2.then:                                     ; preds = %thrown2
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown2
  store i8* %"call.alloc<i8>", i8** %bigger, align 8
  %"*self11" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self11", i32 0, i32 0
  %"*self12" = load %String*, %String** %self, align 8
  %"*self.count13" = getelementptr inbounds %String, %String* %"*self12", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count14" = load i32, i32* %"*self.count13", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count14")
  br label %thrown3

thrown3:                                          ; preds = %endthrown
  %"$ucg.thrown15" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown15", label %thrown3.then, label %endthrown2

thrown3.then:                                     ; preds = %thrown3
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown2:                                       ; preds = %thrown3
  br label %if4

if4:                                              ; preds = %endthrown2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.cap17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 2
  %"$*self.cap18" = load i32, i32* %"*self.cap17", align 4
  %GT = icmp sgt i32 %"$*self.cap18", 0
  br i1 %GT, label %if4.then, label %endif4

if4.then:                                         ; preds = %if4
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$*self.data21" = load i8*, i8** %"*self.data20", align 8
  call void @"free<i8>"(i8* %"$*self.data21")
  br label %thrown5

thrown5:                                          ; preds = %if4.then
  %"$ucg.thrown22" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown22", label %thrown5.then, label %endthrown3

thrown5.then:                                     ; preds = %thrown5
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown3:                                       ; preds = %thrown5
  br label %endif4

endif4:                                           ; preds = %endthrown3, %if4
  %"*self23" = load %String*, %String** %self, align 8
  %"*self.data24" = getelementptr inbounds %String, %String* %"*self23", i32 0, i32 0
  %"$bigger25" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger25", i8** %"*self.data24", align 8
  %"*self26" = load %String*, %String** %self, align 8
  %"*self.cap27" = getelementptr inbounds %String, %String* %"*self26", i32 0, i32 2
  %"$cap28" = load i32, i32* %cap, align 4
  store i32 %"$cap28", i32* %"*self.cap27", align 4
  ret void
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if6

if6:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if6.then, %if6
  br label %while7

while7:                                           ; preds = %while7.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while7.then, label %endloop

while7.then:                                      ; preds = %while7
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
  br label %while7

endloop:                                          ; preds = %while7
  ret void
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %thrown8

thrown8:                                          ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown8.then, label %endthrown

thrown8.then:                                     ; preds = %thrown8
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown8
  br label %if9

if9:                                              ; preds = %endthrown
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if9.then, label %endif

if9.then:                                         ; preds = %if9
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %thrown10

thrown10:                                         ; preds = %if9.then
  %"$ucg.thrown4" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown4", label %thrown10.then, label %endthrown1

thrown10.then:                                    ; preds = %thrown10
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown10
  br label %endif

endif:                                            ; preds = %endthrown1, %if9
  ret void
}

define i8* @String.c_str(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  br label %thrown11

thrown11:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown11.then, label %endthrown

thrown11.then:                                    ; preds = %thrown11
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i8* null

endthrown:                                        ; preds = %thrown11
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

define i32 @uncaught(%Error* %0) {
entry:
  %error = alloca %Error*, align 8
  store %Error* %0, %Error** %error, align 8
  %"*error" = load %Error*, %Error** %error, align 8
  %"*error.message" = getelementptr inbounds %Error, %Error* %"*error", i32 0, i32 0
  %call.String.c_str = call i8* @String.c_str(%String* %"*error.message")
  br label %thrown12

thrown12:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown12.then, label %endthrown

thrown12.then:                                    ; preds = %thrown12
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown:                                        ; preds = %thrown12
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i8* %call.String.c_str)
  ret i32 1
}

define void @fail() {
entry:
  %error.at = alloca i32*, align 8
  store i32* bitcast ([2 x i64]* @ucg.error to i32*), i32** %error.at, align 8
  %"*error.at" = load i32*, i32** %error.at, align 8
  store i32 42, i32* %"*error.at", align 4
  store i32 1, i32* @ucg.error_type, align 4
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void
}

define i32 @main() {
entry:
  %e = alloca %Error, align 8
  %error.at = alloca %Error*, align 8
  %code = alloca i32, align 4
  %error.at10 = alloca i32*, align 8
  %error.at20 = alloca %Error*, align 8
  br label %try13

try13:                                            ; preds = %entry
  br i1 true, label %try13.then, label %endtry1

try13.then:                                       ; preds = %try13
  br label %try14

try14:                                            ; preds = %try13.then
  br i1 true, label %try14.then, label %endtry

try14.then:                                       ; preds = %try14
  call void @fail()
  br label %thrown15

thrown15:                                         ; preds = %try14.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown15.then, label %endthrown

thrown15.then:                                    ; preds = %thrown15
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endthrown:                                        ; preds = %thrown15
  br label %endtry

endtry:                                           ; preds = %endthrown, %thrown15.then, %try14
  br label %catch16

catch16:                                          ; preds = %endtry
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 2
  %"$ucg.thrown4" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown4", %EQ
  br i1 %AND, label %catch16.then, label %catch17

catch16.then:                                     ; preds = %catch16
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  %"$*error.at" = load %Error, %Error* %"*error.at", align 8
  store %Error %"$*error.at", %Error* %e, align 8
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  %"$e.message" = load %String, %String* %e.message, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), %String %"$e.message")
  %e.message5 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message5)
  br label %endcatch

catch17:                                          ; preds = %catch16
  %"$ucg.error_type6" = load i32, i32* @ucg.error_type, align 4
  %EQ7 = icmp eq i32 %"$ucg.error_type6", 1
  %"$ucg.thrown8" = load i1, i1* @ucg.thrown, align 1
  %AND9 = and i1 %"$ucg.thrown8", %EQ7
  br i1 %AND9, label %catch17.then, label %endcatch

catch17.then:                                     ; preds = %catch17
  store i1 false, i1* @ucg.thrown, align 1
  store i32* bitcast ([2 x i64]* @ucg.error to i32*), i32** %error.at10, align 8
  %"*error.at11" = load i32*, i32** %error.at10, align 8
  %"$*error.at12" = load i32, i32* %"*error.at11", align 4
  store i32 %"$*error.at12", i32* %code, align 4
  %"$code" = load i32, i32* %code, align 4
  %call.dprintf13 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.2, i32 0, i32 0), i32 %"$code")
  br label %endcatch

endcatch:                                         ; preds = %catch17.then, %catch17, %catch16.then
  br label %rethrow18

rethrow18:                                        ; preds = %endcatch
  %"$ucg.thrown14" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown14", label %rethrow18.then, label %endrethrow

rethrow18.then:                                   ; preds = %rethrow18
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry1

endrethrow:                                       ; preds = %rethrow18
  %call.dprintf15 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.3, i32 0, i32 0))
  ret i32 0

endtry1:                                          ; preds = %rethrow18.then, %try13
  br label %catch19

catch19:                                          ; preds = %endtry1
  %"$ucg.error_type16" = load i32, i32* @ucg.error_type, align 4
  %EQ17 = icmp eq i32 %"$ucg.error_type16", 2
  %"$ucg.thrown18" = load i1, i1* @ucg.thrown, align 1
  %AND19 = and i1 %"$ucg.thrown18", %EQ17
  br i1 %AND19, label %catch19.then, label %endcatch3

catch19.then:                                     ; preds = %catch19
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at20, align 8
  %"*error.at21" = load %Error*, %Error** %error.at20, align 8
  %call.uncaught = call i32 @uncaught(%Error* %"*error.at21")
  br label %thrown20

thrown20:                                         ; preds = %catch19.then
  %"$ucg.thrown22" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown22", label %thrown20.then, label %endthrown2

thrown20.then:                                    ; preds = %thrown20
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown2:                                       ; preds = %thrown20
  ret i32 %call.uncaught

endcatch3:                                        ; preds = %catch19
  br label %if21

if21:                                             ; preds = %endcatch3
  %"$ucg.thrown23" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown23", label %if21.then, label %endif

if21.then:                                        ; preds = %if21
  ret i32 1

endif:                                            ; preds = %if21
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 011 — try: catch without a matching try is a parse error

```ura
// error: 'catch' without a matching 'try'
fn main() i32:
    catch e Error:
        output("x\n")
    return 0
```

### stderr

```
error: test.ura:3:5 'catch' without a matching 'try'
  |
3 |     catch e Error:
  |     ^^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 012 — try: code after a throw is unreachable and dropped

```ura
// stdout: caught: x
// expect: 0

fn boom():
    throw Error::create(String::create("x"))
    output("dead\n")

fn main() i32:
    try:
        boom()
    catch e Error:
        output("caught: ", e.message, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%String = type { i8*, i32, i32 }
%Error = type { %String }

@ucg.thrown = global i1 false
@ucg.error = global [2 x i64] zeroinitializer
@ucg.error_type = global i32 0
@str = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"x\00", align 1
@str.2 = private unnamed_addr constant [12 x i8] c"caught: %s\0A\00", align 1

define i8* @"alloc<i8>"(i32 %0) {
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
  %"*self5" = load %String*, %String** %self, align 8
  %"*self.count6" = getelementptr inbounds %String, %String* %"*self5", i32 0, i32 1
  %"$*self.count7" = load i32, i32* %"*self.count6", align 4
  store i32 %"$*self.count7", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if0.then, %if0
  br label %if1

if1:                                              ; preds = %endif
  %"*self8" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self8", i32 0, i32 2
  %"$cap9" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap9", %"$*self.cap"
  br i1 %LE, label %if1.then, label %endif1

if1.then:                                         ; preds = %if1
  ret void

endif1:                                           ; preds = %if1
  %"$cap10" = load i32, i32* %cap, align 4
  %"call.alloc<i8>" = call i8* @"alloc<i8>"(i32 %"$cap10")
  br label %thrown2

thrown2:                                          ; preds = %endif1
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown2.then, label %endthrown

thrown2.then:                                     ; preds = %thrown2
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown2
  store i8* %"call.alloc<i8>", i8** %bigger, align 8
  %"*self11" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self11", i32 0, i32 0
  %"*self12" = load %String*, %String** %self, align 8
  %"*self.count13" = getelementptr inbounds %String, %String* %"*self12", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count14" = load i32, i32* %"*self.count13", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count14")
  br label %thrown3

thrown3:                                          ; preds = %endthrown
  %"$ucg.thrown15" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown15", label %thrown3.then, label %endthrown2

thrown3.then:                                     ; preds = %thrown3
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown2:                                       ; preds = %thrown3
  br label %if4

if4:                                              ; preds = %endthrown2
  %"*self16" = load %String*, %String** %self, align 8
  %"*self.cap17" = getelementptr inbounds %String, %String* %"*self16", i32 0, i32 2
  %"$*self.cap18" = load i32, i32* %"*self.cap17", align 4
  %GT = icmp sgt i32 %"$*self.cap18", 0
  br i1 %GT, label %if4.then, label %endif4

if4.then:                                         ; preds = %if4
  %"*self19" = load %String*, %String** %self, align 8
  %"*self.data20" = getelementptr inbounds %String, %String* %"*self19", i32 0, i32 0
  %"$*self.data21" = load i8*, i8** %"*self.data20", align 8
  call void @"free<i8>"(i8* %"$*self.data21")
  br label %thrown5

thrown5:                                          ; preds = %if4.then
  %"$ucg.thrown22" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown22", label %thrown5.then, label %endthrown3

thrown5.then:                                     ; preds = %thrown5
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown3:                                       ; preds = %thrown5
  br label %endif4

endif4:                                           ; preds = %endthrown3, %if4
  %"*self23" = load %String*, %String** %self, align 8
  %"*self.data24" = getelementptr inbounds %String, %String* %"*self23", i32 0, i32 0
  %"$bigger25" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger25", i8** %"*self.data24", align 8
  %"*self26" = load %String*, %String** %self, align 8
  %"*self.cap27" = getelementptr inbounds %String, %String* %"*self26", i32 0, i32 2
  %"$cap28" = load i32, i32* %cap, align 4
  store i32 %"$cap28", i32* %"*self.cap27", align 4
  ret void
}

define void @String.push_back(%String* %0, i8 %1) {
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
  br label %thrown7

thrown7:                                          ; preds = %if6.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown7.then, label %endthrown

thrown7.then:                                     ; preds = %thrown7
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown7
  br label %endif

endif:                                            ; preds = %endthrown, %if6
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

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %if8

if8:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if8.then, label %endif

if8.then:                                         ; preds = %if8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if8.then, %if8
  br label %while9

while9:                                           ; preds = %while9.then, %endif
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.count3" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while9.then, label %endloop

while9.then:                                      ; preds = %while9
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
  br label %while9

endloop:                                          ; preds = %while9
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
  br label %thrown10

thrown10:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown10.then, label %endthrown

thrown10.then:                                    ; preds = %thrown10
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown10
  %"*self2" = load %String*, %String** %self, align 8
  %"*other" = load %String*, %String** %other, align 8
  %"*other.count" = getelementptr inbounds %String, %String* %"*other", i32 0, i32 1
  %"$*other.count" = load i32, i32* %"*other.count", align 4
  call void @String.reserve(%String* %"*self2", i32 %"$*other.count")
  br label %thrown11

thrown11:                                         ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown11.then, label %endthrown1

thrown11.then:                                    ; preds = %thrown11
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown11
  store i32 0, i32* %i, align 4
  br label %while12

while12:                                          ; preds = %while12.then, %endthrown1
  %"*other4" = load %String*, %String** %other, align 8
  %"*other.count5" = getelementptr inbounds %String, %String* %"*other4", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*other.count6" = load i32, i32* %"*other.count5", align 4
  %LT = icmp slt i32 %"$i", %"$*other.count6"
  br i1 %LT, label %while12.then, label %endloop

while12.then:                                     ; preds = %while12
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$i8" = load i32, i32* %i, align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$i8"
  %"*other9" = load %String*, %String** %other, align 8
  %"*other.data" = getelementptr inbounds %String, %String* %"*other9", i32 0, i32 0
  %"$*other.data" = load i8*, i8** %"*other.data", align 8
  %"$i10" = load i32, i32* %i, align 4
  %"*other.data[]" = getelementptr inbounds i8, i8* %"$*other.data", i32 %"$i10"
  %"$*other.data[]" = load i8, i8* %"*other.data[]", align 1
  store i8 %"$*other.data[]", i8* %"*self.data[]", align 1
  %"$i11" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i11", 1
  store i32 %ADD, i32* %i, align 4
  br label %while12

endloop:                                          ; preds = %while12
  %"*self12" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self12", i32 0, i32 1
  %"*other13" = load %String*, %String** %other, align 8
  %"*other.count14" = getelementptr inbounds %String, %String* %"*other13", i32 0, i32 1
  %"$*other.count15" = load i32, i32* %"*other.count14", align 4
  store i32 %"$*other.count15", i32* %"*self.count", align 4
  ret void
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %thrown13

thrown13:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown13.then, label %endthrown

thrown13.then:                                    ; preds = %thrown13
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown13
  br label %if14

if14:                                             ; preds = %endthrown
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if14.then, label %endif

if14.then:                                        ; preds = %if14
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %thrown15

thrown15:                                         ; preds = %if14.then
  %"$ucg.thrown4" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown4", label %thrown15.then, label %endthrown1

thrown15.then:                                    ; preds = %thrown15
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown15
  br label %endif

endif:                                            ; preds = %endthrown1, %if14
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
  br label %thrown16

thrown16:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown16.then, label %endthrown

thrown16.then:                                    ; preds = %thrown16
  store i1 false, i1* @ucg.thrown, align 1
  call void @String.drop(%String* %res)
  store i1 true, i1* @ucg.thrown, align 1
  ret %String zeroinitializer

endthrown:                                        ; preds = %thrown16
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
  br label %while17

while17:                                          ; preds = %endthrown, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$i" = load i32, i32* %i, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$i"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while17.then, label %endloop

while17.then:                                     ; preds = %while17
  %"*self" = load %String*, %String** %self, align 8
  %"$text1" = load i8*, i8** %text, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"text[]3" = getelementptr inbounds i8, i8* %"$text1", i32 %"$i2"
  %"$text[]4" = load i8, i8* %"text[]3", align 1
  call void @String.push_back(%String* %"*self", i8 %"$text[]4")
  br label %thrown18

thrown18:                                         ; preds = %while17.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown18.then, label %endthrown

thrown18.then:                                    ; preds = %thrown18
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown18
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while17

endloop:                                          ; preds = %while17
  ret void
}

define i8* @String.c_str(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %ADD = add i32 %"$*self.count", 1
  call void @String.reserve(%String* %"*self", i32 %ADD)
  br label %thrown19

thrown19:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown19.then, label %endthrown

thrown19.then:                                    ; preds = %thrown19
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i8* null

endthrown:                                        ; preds = %thrown19
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

define %Error @Error.create(%String %0) {
entry:
  %message = alloca %String, align 8
  %e = alloca %Error, align 8
  %copy = alloca %String, align 8
  store %String %0, %String* %message, align 8
  store %Error zeroinitializer, %Error* %e, align 8
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  store %String zeroinitializer, %String* %copy, align 8
  call void @"String.="(%String* %copy, %String* %message)
  br label %thrown20

thrown20:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown20.then, label %endthrown

thrown20.then:                                    ; preds = %thrown20
  store i1 false, i1* @ucg.thrown, align 1
  %e.message2 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message2)
  call void @String.drop(%String* %message)
  store i1 true, i1* @ucg.thrown, align 1
  ret %Error zeroinitializer

endthrown:                                        ; preds = %thrown20
  %"$copy" = load %String, %String* %copy, align 8
  call void @String.drop(%String* %e.message)
  br label %thrown21

thrown21:                                         ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown21.then, label %endthrown1

thrown21.then:                                    ; preds = %thrown21
  store i1 false, i1* @ucg.thrown, align 1
  %e.message4 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message4)
  call void @String.drop(%String* %message)
  store i1 true, i1* @ucg.thrown, align 1
  ret %Error zeroinitializer

endthrown1:                                       ; preds = %thrown21
  store %String %"$copy", %String* %e.message, align 8
  %"$e" = load %Error, %Error* %e, align 8
  call void @String.drop(%String* %message)
  ret %Error %"$e"
}

define i32 @uncaught(%Error* %0) {
entry:
  %error = alloca %Error*, align 8
  store %Error* %0, %Error** %error, align 8
  %"*error" = load %Error*, %Error** %error, align 8
  %"*error.message" = getelementptr inbounds %Error, %Error* %"*error", i32 0, i32 0
  %call.String.c_str = call i8* @String.c_str(%String* %"*error.message")
  br label %thrown22

thrown22:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown22.then, label %endthrown

thrown22.then:                                    ; preds = %thrown22
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown:                                        ; preds = %thrown22
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i8* %call.String.c_str)
  ret i32 1
}

define void @boom() {
entry:
  %error.at = alloca %Error*, align 8
  %call.String.create = call %String @String.create(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  br label %thrown23

thrown23:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown23.then, label %endthrown

thrown23.then:                                    ; preds = %thrown23
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown23
  %call.Error.create = call %Error @Error.create(%String %call.String.create)
  br label %thrown24

thrown24:                                         ; preds = %endthrown
  %"$ucg.thrown2" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown2", label %thrown24.then, label %endthrown1

thrown24.then:                                    ; preds = %thrown24
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown24
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  store %Error %call.Error.create, %Error* %"*error.at", align 8
  store i32 1, i32* @ucg.error_type, align 4
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void
}

define i32 @main() {
entry:
  %e = alloca %Error, align 8
  %error.at = alloca %Error*, align 8
  %error.at11 = alloca %Error*, align 8
  br label %try25

try25:                                            ; preds = %entry
  br i1 true, label %try25.then, label %endtry1

try25.then:                                       ; preds = %try25
  br label %try26

try26:                                            ; preds = %try25.then
  br i1 true, label %try26.then, label %endtry

try26.then:                                       ; preds = %try26
  call void @boom()
  br label %thrown27

thrown27:                                         ; preds = %try26.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown27.then, label %endthrown

thrown27.then:                                    ; preds = %thrown27
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endthrown:                                        ; preds = %thrown27
  br label %endtry

endtry:                                           ; preds = %endthrown, %thrown27.then, %try26
  br label %catch28

catch28:                                          ; preds = %endtry
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown4" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown4", %EQ
  br i1 %AND, label %catch28.then, label %endcatch

catch28.then:                                     ; preds = %catch28
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  %"$*error.at" = load %Error, %Error* %"*error.at", align 8
  store %Error %"$*error.at", %Error* %e, align 8
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  %"$e.message" = load %String, %String* %e.message, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.2, i32 0, i32 0), %String %"$e.message")
  %e.message5 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message5)
  br label %endcatch

endcatch:                                         ; preds = %catch28.then, %catch28
  br label %rethrow29

rethrow29:                                        ; preds = %endcatch
  %"$ucg.thrown6" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown6", label %rethrow29.then, label %endrethrow

rethrow29.then:                                   ; preds = %rethrow29
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry1

endrethrow:                                       ; preds = %rethrow29
  ret i32 0

endtry1:                                          ; preds = %rethrow29.then, %try25
  br label %catch30

catch30:                                          ; preds = %endtry1
  %"$ucg.error_type7" = load i32, i32* @ucg.error_type, align 4
  %EQ8 = icmp eq i32 %"$ucg.error_type7", 1
  %"$ucg.thrown9" = load i1, i1* @ucg.thrown, align 1
  %AND10 = and i1 %"$ucg.thrown9", %EQ8
  br i1 %AND10, label %catch30.then, label %endcatch3

catch30.then:                                     ; preds = %catch30
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at11, align 8
  %"*error.at12" = load %Error*, %Error** %error.at11, align 8
  %call.uncaught = call i32 @uncaught(%Error* %"*error.at12")
  br label %thrown31

thrown31:                                         ; preds = %catch30.then
  %"$ucg.thrown13" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown13", label %thrown31.then, label %endthrown2

thrown31.then:                                    ; preds = %thrown31
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown2:                                       ; preds = %thrown31
  ret i32 %call.uncaught

endcatch3:                                        ; preds = %catch30
  br label %if32

if32:                                             ; preds = %endcatch3
  %"$ucg.thrown14" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown14", label %if32.then, label %endif

if32.then:                                        ; preds = %if32
  ret i32 1

endif:                                            ; preds = %if32
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

