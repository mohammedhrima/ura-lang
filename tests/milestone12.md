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
@str = private unnamed_addr constant [5 x i8] c"boom\00", align 1
@str.1 = private unnamed_addr constant [12 x i8] c"caught: %s\0A\00", align 1
@str.2 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

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
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap", %"$*self.cap"
  br i1 %LE, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret void

endif:                                            ; preds = %if0
  %"$cap3" = load i32, i32* %cap, align 4
  %"call.alloc<i8>" = call i8* @"alloc<i8>"(i32 %"$cap3")
  br label %thrown1

thrown1:                                          ; preds = %endif
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown1.then, label %endthrown

thrown1.then:                                     ; preds = %thrown1
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown1
  store i8* %"call.alloc<i8>", i8** %bigger, align 8
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 0
  %"*self5" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self5", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count")
  br label %thrown2

thrown2:                                          ; preds = %endthrown
  %"$ucg.thrown6" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown6", label %thrown2.then, label %endthrown1

thrown2.then:                                     ; preds = %thrown2
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown2
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data8" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"$*self.data9" = load i8*, i8** %"*self.data8", align 8
  call void @"free<i8>"(i8* %"$*self.data9")
  br label %thrown3

thrown3:                                          ; preds = %endthrown1
  %"$ucg.thrown10" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown10", label %thrown3.then, label %endthrown2

thrown3.then:                                     ; preds = %thrown3
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown2:                                       ; preds = %thrown3
  %"*self11" = load %String*, %String** %self, align 8
  %"*self.data12" = getelementptr inbounds %String, %String* %"*self11", i32 0, i32 0
  %"$bigger13" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger13", i8** %"*self.data12", align 8
  %"*self14" = load %String*, %String** %self, align 8
  %"*self.cap15" = getelementptr inbounds %String, %String* %"*self14", i32 0, i32 2
  %"$cap16" = load i32, i32* %cap, align 4
  store i32 %"$cap16", i32* %"*self.cap15", align 4
  ret void
}

define void @String.push_back(%String* %0, i8 %1) {
entry:
  %self = alloca %String*, align 8
  %value = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store i8 %1, i8* %value, align 1
  br label %if4

if4:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.count", %"$*self.cap"
  br i1 %EQ, label %if4.then, label %endif

if4.then:                                         ; preds = %if4
  %"*self2" = load %String*, %String** %self, align 8
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.cap4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 2
  %"$*self.cap5" = load i32, i32* %"*self.cap4", align 4
  %MUL = mul i32 %"$*self.cap5", 2
  %ADD = add i32 %MUL, 4
  call void @String.reserve(%String* %"*self2", i32 %ADD)
  br label %thrown5

thrown5:                                          ; preds = %if4.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown5.then, label %endthrown

thrown5.then:                                     ; preds = %thrown5
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown5
  br label %endif

endif:                                            ; preds = %endthrown, %if4
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
  br label %while6

while6:                                           ; preds = %while6.then, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while6.then, label %endloop

while6.then:                                      ; preds = %while6
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count2" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count3" = load i32, i32* %"*self.count2", align 4
  %SUB = sub i32 %"$*self.count3", 1
  store i32 %SUB, i32* %"*self.count2", align 4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 0
  %"*self5" = load %String*, %String** %self, align 8
  %"*self.count6" = getelementptr inbounds %String, %String* %"*self5", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count7" = load i32, i32* %"*self.count6", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count7"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while6

endloop:                                          ; preds = %while6
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
  br label %thrown7

thrown7:                                          ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown7.then, label %endthrown

thrown7.then:                                     ; preds = %thrown7
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown7
  %"*self2" = load %String*, %String** %self, align 8
  %"*other" = load %String*, %String** %other, align 8
  %"*other.count" = getelementptr inbounds %String, %String* %"*other", i32 0, i32 1
  %"$*other.count" = load i32, i32* %"*other.count", align 4
  call void @String.reserve(%String* %"*self2", i32 %"$*other.count")
  br label %thrown8

thrown8:                                          ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown8.then, label %endthrown1

thrown8.then:                                     ; preds = %thrown8
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown8
  store i32 0, i32* %i, align 4
  br label %while9

while9:                                           ; preds = %while9.then, %endthrown1
  %"*other4" = load %String*, %String** %other, align 8
  %"*other.count5" = getelementptr inbounds %String, %String* %"*other4", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*other.count6" = load i32, i32* %"*other.count5", align 4
  %LT = icmp slt i32 %"$i", %"$*other.count6"
  br i1 %LT, label %while9.then, label %endloop

while9.then:                                      ; preds = %while9
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
  br label %while9

endloop:                                          ; preds = %while9
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
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %thrown11

thrown11:                                         ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown11.then, label %endthrown1

thrown11.then:                                    ; preds = %thrown11
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown11
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
  br label %thrown12

thrown12:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown12.then, label %endthrown

thrown12.then:                                    ; preds = %thrown12
  store i1 false, i1* @ucg.thrown, align 1
  call void @String.drop(%String* %res)
  store i1 true, i1* @ucg.thrown, align 1
  ret %String zeroinitializer

endthrown:                                        ; preds = %thrown12
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
  br label %while13

while13:                                          ; preds = %endthrown, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$i" = load i32, i32* %i, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$i"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while13.then, label %endloop

while13.then:                                     ; preds = %while13
  %"*self" = load %String*, %String** %self, align 8
  %"$text1" = load i8*, i8** %text, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"text[]3" = getelementptr inbounds i8, i8* %"$text1", i32 %"$i2"
  %"$text[]4" = load i8, i8* %"text[]3", align 1
  call void @String.push_back(%String* %"*self", i8 %"$text[]4")
  br label %thrown14

thrown14:                                         ; preds = %while13.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown14.then, label %endthrown

thrown14.then:                                    ; preds = %thrown14
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown14
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while13

endloop:                                          ; preds = %while13
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
  br label %thrown15

thrown15:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown15.then, label %endthrown

thrown15.then:                                    ; preds = %thrown15
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i8* null

endthrown:                                        ; preds = %thrown15
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
  br label %thrown16

thrown16:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown16.then, label %endthrown

thrown16.then:                                    ; preds = %thrown16
  store i1 false, i1* @ucg.thrown, align 1
  %e.message2 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message2)
  call void @String.drop(%String* %message)
  store i1 true, i1* @ucg.thrown, align 1
  ret %Error zeroinitializer

endthrown:                                        ; preds = %thrown16
  %"$copy" = load %String, %String* %copy, align 8
  call void @String.drop(%String* %e.message)
  br label %thrown17

thrown17:                                         ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown17.then, label %endthrown1

thrown17.then:                                    ; preds = %thrown17
  store i1 false, i1* @ucg.thrown, align 1
  %e.message4 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message4)
  call void @String.drop(%String* %message)
  store i1 true, i1* @ucg.thrown, align 1
  ret %Error zeroinitializer

endthrown1:                                       ; preds = %thrown17
  store %String %"$copy", %String* %e.message, align 8
  %"$e" = load %Error, %Error* %e, align 8
  call void @String.drop(%String* %message)
  ret %Error %"$e"
}

define i32 @main() {
entry:
  %error.at = alloca %Error*, align 8
  %e = alloca %Error, align 8
  %error.at7 = alloca %Error*, align 8
  %error.at15 = alloca %Error*, align 8
  br label %try18

try18:                                            ; preds = %entry
  br i1 true, label %try18.then, label %endtry2

try18.then:                                       ; preds = %try18
  br label %try19

try19:                                            ; preds = %try18.then
  br i1 true, label %try19.then, label %endtry

try19.then:                                       ; preds = %try19
  %call.String.create = call %String @String.create(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0))
  br label %thrown20

thrown20:                                         ; preds = %try19.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown20.then, label %endthrown

thrown20.then:                                    ; preds = %thrown20
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endthrown:                                        ; preds = %thrown20
  %call.Error.create = call %Error @Error.create(%String %call.String.create)
  br label %thrown21

thrown21:                                         ; preds = %endthrown
  %"$ucg.thrown5" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown5", label %thrown21.then, label %endthrown1

thrown21.then:                                    ; preds = %thrown21
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endthrown1:                                       ; preds = %thrown21
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  store %Error %call.Error.create, %Error* %"*error.at", align 8
  store i32 1, i32* @ucg.error_type, align 4
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endtry:                                           ; preds = %endthrown1, %thrown21.then, %thrown20.then, %try19
  br label %catch22

catch22:                                          ; preds = %endtry
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown6" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown6", %EQ
  br i1 %AND, label %catch22.then, label %endcatch

catch22.then:                                     ; preds = %catch22
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at7, align 8
  %"*error.at8" = load %Error*, %Error** %error.at7, align 8
  %"$*error.at" = load %Error, %Error* %"*error.at8", align 8
  store %Error %"$*error.at", %Error* %e, align 8
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  %"$e.message" = load %String, %String* %e.message, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.1, i32 0, i32 0), %String %"$e.message")
  %e.message9 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message9)
  br label %endcatch

endcatch:                                         ; preds = %catch22.then, %catch22
  br label %rethrow23

rethrow23:                                        ; preds = %endcatch
  %"$ucg.thrown10" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown10", label %rethrow23.then, label %endrethrow

rethrow23.then:                                   ; preds = %rethrow23
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry2

endrethrow:                                       ; preds = %rethrow23
  ret i32 0

endtry2:                                          ; preds = %rethrow23.then, %try18
  br label %catch24

catch24:                                          ; preds = %endtry2
  %"$ucg.error_type11" = load i32, i32* @ucg.error_type, align 4
  %EQ12 = icmp eq i32 %"$ucg.error_type11", 1
  %"$ucg.thrown13" = load i1, i1* @ucg.thrown, align 1
  %AND14 = and i1 %"$ucg.thrown13", %EQ12
  br i1 %AND14, label %catch24.then, label %endcatch4

catch24.then:                                     ; preds = %catch24
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at15, align 8
  %"*error.at16" = load %Error*, %Error** %error.at15, align 8
  %"*error.at.message" = getelementptr inbounds %Error, %Error* %"*error.at16", i32 0, i32 0
  %call.String.c_str = call i8* @String.c_str(%String* %"*error.at.message")
  br label %thrown25

thrown25:                                         ; preds = %catch24.then
  %"$ucg.thrown17" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown17", label %thrown25.then, label %endthrown3

thrown25.then:                                    ; preds = %thrown25
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown3:                                       ; preds = %thrown25
  %call.dprintf18 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.2, i32 0, i32 0), i8* %call.String.c_str)
  ret i32 1

endcatch4:                                        ; preds = %catch24
  br label %if26

if26:                                             ; preds = %endcatch4
  %"$ucg.thrown19" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown19", label %if26.then, label %endif

if26.then:                                        ; preds = %if26
  ret i32 1

endif:                                            ; preds = %if26
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
@str = private unnamed_addr constant [10 x i8] c"from deep\00", align 1
@str.1 = private unnamed_addr constant [12 x i8] c"caught: %s\0A\00", align 1
@str.2 = private unnamed_addr constant [7 x i8] c"after\0A\00", align 1
@str.3 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

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
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap", %"$*self.cap"
  br i1 %LE, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret void

endif:                                            ; preds = %if0
  %"$cap3" = load i32, i32* %cap, align 4
  %"call.alloc<i8>" = call i8* @"alloc<i8>"(i32 %"$cap3")
  br label %thrown1

thrown1:                                          ; preds = %endif
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown1.then, label %endthrown

thrown1.then:                                     ; preds = %thrown1
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown1
  store i8* %"call.alloc<i8>", i8** %bigger, align 8
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 0
  %"*self5" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self5", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count")
  br label %thrown2

thrown2:                                          ; preds = %endthrown
  %"$ucg.thrown6" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown6", label %thrown2.then, label %endthrown1

thrown2.then:                                     ; preds = %thrown2
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown2
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data8" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"$*self.data9" = load i8*, i8** %"*self.data8", align 8
  call void @"free<i8>"(i8* %"$*self.data9")
  br label %thrown3

thrown3:                                          ; preds = %endthrown1
  %"$ucg.thrown10" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown10", label %thrown3.then, label %endthrown2

thrown3.then:                                     ; preds = %thrown3
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown2:                                       ; preds = %thrown3
  %"*self11" = load %String*, %String** %self, align 8
  %"*self.data12" = getelementptr inbounds %String, %String* %"*self11", i32 0, i32 0
  %"$bigger13" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger13", i8** %"*self.data12", align 8
  %"*self14" = load %String*, %String** %self, align 8
  %"*self.cap15" = getelementptr inbounds %String, %String* %"*self14", i32 0, i32 2
  %"$cap16" = load i32, i32* %cap, align 4
  store i32 %"$cap16", i32* %"*self.cap15", align 4
  ret void
}

define void @String.push_back(%String* %0, i8 %1) {
entry:
  %self = alloca %String*, align 8
  %value = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store i8 %1, i8* %value, align 1
  br label %if4

if4:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.count", %"$*self.cap"
  br i1 %EQ, label %if4.then, label %endif

if4.then:                                         ; preds = %if4
  %"*self2" = load %String*, %String** %self, align 8
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.cap4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 2
  %"$*self.cap5" = load i32, i32* %"*self.cap4", align 4
  %MUL = mul i32 %"$*self.cap5", 2
  %ADD = add i32 %MUL, 4
  call void @String.reserve(%String* %"*self2", i32 %ADD)
  br label %thrown5

thrown5:                                          ; preds = %if4.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown5.then, label %endthrown

thrown5.then:                                     ; preds = %thrown5
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown5
  br label %endif

endif:                                            ; preds = %endthrown, %if4
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
  br label %while6

while6:                                           ; preds = %while6.then, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while6.then, label %endloop

while6.then:                                      ; preds = %while6
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count2" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count3" = load i32, i32* %"*self.count2", align 4
  %SUB = sub i32 %"$*self.count3", 1
  store i32 %SUB, i32* %"*self.count2", align 4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 0
  %"*self5" = load %String*, %String** %self, align 8
  %"*self.count6" = getelementptr inbounds %String, %String* %"*self5", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count7" = load i32, i32* %"*self.count6", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count7"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while6

endloop:                                          ; preds = %while6
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
  br label %thrown7

thrown7:                                          ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown7.then, label %endthrown

thrown7.then:                                     ; preds = %thrown7
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown7
  %"*self2" = load %String*, %String** %self, align 8
  %"*other" = load %String*, %String** %other, align 8
  %"*other.count" = getelementptr inbounds %String, %String* %"*other", i32 0, i32 1
  %"$*other.count" = load i32, i32* %"*other.count", align 4
  call void @String.reserve(%String* %"*self2", i32 %"$*other.count")
  br label %thrown8

thrown8:                                          ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown8.then, label %endthrown1

thrown8.then:                                     ; preds = %thrown8
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown8
  store i32 0, i32* %i, align 4
  br label %while9

while9:                                           ; preds = %while9.then, %endthrown1
  %"*other4" = load %String*, %String** %other, align 8
  %"*other.count5" = getelementptr inbounds %String, %String* %"*other4", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*other.count6" = load i32, i32* %"*other.count5", align 4
  %LT = icmp slt i32 %"$i", %"$*other.count6"
  br i1 %LT, label %while9.then, label %endloop

while9.then:                                      ; preds = %while9
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
  br label %while9

endloop:                                          ; preds = %while9
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
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %thrown11

thrown11:                                         ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown11.then, label %endthrown1

thrown11.then:                                    ; preds = %thrown11
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown11
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
  br label %thrown12

thrown12:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown12.then, label %endthrown

thrown12.then:                                    ; preds = %thrown12
  store i1 false, i1* @ucg.thrown, align 1
  call void @String.drop(%String* %res)
  store i1 true, i1* @ucg.thrown, align 1
  ret %String zeroinitializer

endthrown:                                        ; preds = %thrown12
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
  br label %while13

while13:                                          ; preds = %endthrown, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$i" = load i32, i32* %i, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$i"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while13.then, label %endloop

while13.then:                                     ; preds = %while13
  %"*self" = load %String*, %String** %self, align 8
  %"$text1" = load i8*, i8** %text, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"text[]3" = getelementptr inbounds i8, i8* %"$text1", i32 %"$i2"
  %"$text[]4" = load i8, i8* %"text[]3", align 1
  call void @String.push_back(%String* %"*self", i8 %"$text[]4")
  br label %thrown14

thrown14:                                         ; preds = %while13.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown14.then, label %endthrown

thrown14.then:                                    ; preds = %thrown14
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown14
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while13

endloop:                                          ; preds = %while13
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
  br label %thrown15

thrown15:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown15.then, label %endthrown

thrown15.then:                                    ; preds = %thrown15
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i8* null

endthrown:                                        ; preds = %thrown15
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
  br label %thrown16

thrown16:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown16.then, label %endthrown

thrown16.then:                                    ; preds = %thrown16
  store i1 false, i1* @ucg.thrown, align 1
  %e.message2 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message2)
  call void @String.drop(%String* %message)
  store i1 true, i1* @ucg.thrown, align 1
  ret %Error zeroinitializer

endthrown:                                        ; preds = %thrown16
  %"$copy" = load %String, %String* %copy, align 8
  call void @String.drop(%String* %e.message)
  br label %thrown17

thrown17:                                         ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown17.then, label %endthrown1

thrown17.then:                                    ; preds = %thrown17
  store i1 false, i1* @ucg.thrown, align 1
  %e.message4 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message4)
  call void @String.drop(%String* %message)
  store i1 true, i1* @ucg.thrown, align 1
  ret %Error zeroinitializer

endthrown1:                                       ; preds = %thrown17
  store %String %"$copy", %String* %e.message, align 8
  %"$e" = load %Error, %Error* %e, align 8
  call void @String.drop(%String* %message)
  ret %Error %"$e"
}

define void @deep() {
entry:
  %error.at = alloca %Error*, align 8
  %call.String.create = call %String @String.create(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0))
  br label %thrown18

thrown18:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown18.then, label %endthrown

thrown18.then:                                    ; preds = %thrown18
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown18
  %call.Error.create = call %Error @Error.create(%String %call.String.create)
  br label %thrown19

thrown19:                                         ; preds = %endthrown
  %"$ucg.thrown2" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown2", label %thrown19.then, label %endthrown1

thrown19.then:                                    ; preds = %thrown19
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown19
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
  br label %thrown20

thrown20:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown20.then, label %endthrown

thrown20.then:                                    ; preds = %thrown20
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown20
  ret void
}

define i32 @main() {
entry:
  %e = alloca %Error, align 8
  %error.at = alloca %Error*, align 8
  %error.at12 = alloca %Error*, align 8
  br label %try21

try21:                                            ; preds = %entry
  br i1 true, label %try21.then, label %endtry1

try21.then:                                       ; preds = %try21
  br label %try22

try22:                                            ; preds = %try21.then
  br i1 true, label %try22.then, label %endtry

try22.then:                                       ; preds = %try22
  call void @middle()
  br label %thrown23

thrown23:                                         ; preds = %try22.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown23.then, label %endthrown

thrown23.then:                                    ; preds = %thrown23
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endthrown:                                        ; preds = %thrown23
  br label %endtry

endtry:                                           ; preds = %endthrown, %thrown23.then, %try22
  br label %catch24

catch24:                                          ; preds = %endtry
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown4" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown4", %EQ
  br i1 %AND, label %catch24.then, label %endcatch

catch24.then:                                     ; preds = %catch24
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  %"$*error.at" = load %Error, %Error* %"*error.at", align 8
  store %Error %"$*error.at", %Error* %e, align 8
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  %"$e.message" = load %String, %String* %e.message, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.1, i32 0, i32 0), %String %"$e.message")
  %e.message5 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message5)
  br label %endcatch

endcatch:                                         ; preds = %catch24.then, %catch24
  br label %rethrow25

rethrow25:                                        ; preds = %endcatch
  %"$ucg.thrown6" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown6", label %rethrow25.then, label %endrethrow

rethrow25.then:                                   ; preds = %rethrow25
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry1

endrethrow:                                       ; preds = %rethrow25
  %call.dprintf7 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.2, i32 0, i32 0))
  ret i32 0

endtry1:                                          ; preds = %rethrow25.then, %try21
  br label %catch26

catch26:                                          ; preds = %endtry1
  %"$ucg.error_type8" = load i32, i32* @ucg.error_type, align 4
  %EQ9 = icmp eq i32 %"$ucg.error_type8", 1
  %"$ucg.thrown10" = load i1, i1* @ucg.thrown, align 1
  %AND11 = and i1 %"$ucg.thrown10", %EQ9
  br i1 %AND11, label %catch26.then, label %endcatch3

catch26.then:                                     ; preds = %catch26
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at12, align 8
  %"*error.at13" = load %Error*, %Error** %error.at12, align 8
  %"*error.at.message" = getelementptr inbounds %Error, %Error* %"*error.at13", i32 0, i32 0
  %call.String.c_str = call i8* @String.c_str(%String* %"*error.at.message")
  br label %thrown27

thrown27:                                         ; preds = %catch26.then
  %"$ucg.thrown14" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown14", label %thrown27.then, label %endthrown2

thrown27.then:                                    ; preds = %thrown27
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown2:                                       ; preds = %thrown27
  %call.dprintf15 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.3, i32 0, i32 0), i8* %call.String.c_str)
  ret i32 1

endcatch3:                                        ; preds = %catch26
  br label %if28

if28:                                             ; preds = %endcatch3
  %"$ucg.thrown16" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown16", label %if28.then, label %endif

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
@str = private unnamed_addr constant [6 x i8] c"first\00", align 1
@str.1 = private unnamed_addr constant [9 x i8] c"mid: %s\0A\00", align 1
@str.2 = private unnamed_addr constant [7 x i8] c"second\00", align 1
@str.3 = private unnamed_addr constant [10 x i8] c"main: %s\0A\00", align 1
@str.4 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

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
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap", %"$*self.cap"
  br i1 %LE, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret void

endif:                                            ; preds = %if0
  %"$cap3" = load i32, i32* %cap, align 4
  %"call.alloc<i8>" = call i8* @"alloc<i8>"(i32 %"$cap3")
  br label %thrown1

thrown1:                                          ; preds = %endif
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown1.then, label %endthrown

thrown1.then:                                     ; preds = %thrown1
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown1
  store i8* %"call.alloc<i8>", i8** %bigger, align 8
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 0
  %"*self5" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self5", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count")
  br label %thrown2

thrown2:                                          ; preds = %endthrown
  %"$ucg.thrown6" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown6", label %thrown2.then, label %endthrown1

thrown2.then:                                     ; preds = %thrown2
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown2
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data8" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"$*self.data9" = load i8*, i8** %"*self.data8", align 8
  call void @"free<i8>"(i8* %"$*self.data9")
  br label %thrown3

thrown3:                                          ; preds = %endthrown1
  %"$ucg.thrown10" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown10", label %thrown3.then, label %endthrown2

thrown3.then:                                     ; preds = %thrown3
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown2:                                       ; preds = %thrown3
  %"*self11" = load %String*, %String** %self, align 8
  %"*self.data12" = getelementptr inbounds %String, %String* %"*self11", i32 0, i32 0
  %"$bigger13" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger13", i8** %"*self.data12", align 8
  %"*self14" = load %String*, %String** %self, align 8
  %"*self.cap15" = getelementptr inbounds %String, %String* %"*self14", i32 0, i32 2
  %"$cap16" = load i32, i32* %cap, align 4
  store i32 %"$cap16", i32* %"*self.cap15", align 4
  ret void
}

define void @String.push_back(%String* %0, i8 %1) {
entry:
  %self = alloca %String*, align 8
  %value = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store i8 %1, i8* %value, align 1
  br label %if4

if4:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.count", %"$*self.cap"
  br i1 %EQ, label %if4.then, label %endif

if4.then:                                         ; preds = %if4
  %"*self2" = load %String*, %String** %self, align 8
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.cap4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 2
  %"$*self.cap5" = load i32, i32* %"*self.cap4", align 4
  %MUL = mul i32 %"$*self.cap5", 2
  %ADD = add i32 %MUL, 4
  call void @String.reserve(%String* %"*self2", i32 %ADD)
  br label %thrown5

thrown5:                                          ; preds = %if4.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown5.then, label %endthrown

thrown5.then:                                     ; preds = %thrown5
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown5
  br label %endif

endif:                                            ; preds = %endthrown, %if4
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
  br label %while6

while6:                                           ; preds = %while6.then, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while6.then, label %endloop

while6.then:                                      ; preds = %while6
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count2" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count3" = load i32, i32* %"*self.count2", align 4
  %SUB = sub i32 %"$*self.count3", 1
  store i32 %SUB, i32* %"*self.count2", align 4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 0
  %"*self5" = load %String*, %String** %self, align 8
  %"*self.count6" = getelementptr inbounds %String, %String* %"*self5", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count7" = load i32, i32* %"*self.count6", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count7"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while6

endloop:                                          ; preds = %while6
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
  br label %thrown7

thrown7:                                          ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown7.then, label %endthrown

thrown7.then:                                     ; preds = %thrown7
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown7
  %"*self2" = load %String*, %String** %self, align 8
  %"*other" = load %String*, %String** %other, align 8
  %"*other.count" = getelementptr inbounds %String, %String* %"*other", i32 0, i32 1
  %"$*other.count" = load i32, i32* %"*other.count", align 4
  call void @String.reserve(%String* %"*self2", i32 %"$*other.count")
  br label %thrown8

thrown8:                                          ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown8.then, label %endthrown1

thrown8.then:                                     ; preds = %thrown8
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown8
  store i32 0, i32* %i, align 4
  br label %while9

while9:                                           ; preds = %while9.then, %endthrown1
  %"*other4" = load %String*, %String** %other, align 8
  %"*other.count5" = getelementptr inbounds %String, %String* %"*other4", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*other.count6" = load i32, i32* %"*other.count5", align 4
  %LT = icmp slt i32 %"$i", %"$*other.count6"
  br i1 %LT, label %while9.then, label %endloop

while9.then:                                      ; preds = %while9
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
  br label %while9

endloop:                                          ; preds = %while9
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
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %thrown11

thrown11:                                         ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown11.then, label %endthrown1

thrown11.then:                                    ; preds = %thrown11
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown11
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
  br label %thrown12

thrown12:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown12.then, label %endthrown

thrown12.then:                                    ; preds = %thrown12
  store i1 false, i1* @ucg.thrown, align 1
  call void @String.drop(%String* %res)
  store i1 true, i1* @ucg.thrown, align 1
  ret %String zeroinitializer

endthrown:                                        ; preds = %thrown12
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
  br label %while13

while13:                                          ; preds = %endthrown, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$i" = load i32, i32* %i, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$i"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while13.then, label %endloop

while13.then:                                     ; preds = %while13
  %"*self" = load %String*, %String** %self, align 8
  %"$text1" = load i8*, i8** %text, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"text[]3" = getelementptr inbounds i8, i8* %"$text1", i32 %"$i2"
  %"$text[]4" = load i8, i8* %"text[]3", align 1
  call void @String.push_back(%String* %"*self", i8 %"$text[]4")
  br label %thrown14

thrown14:                                         ; preds = %while13.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown14.then, label %endthrown

thrown14.then:                                    ; preds = %thrown14
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown14
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while13

endloop:                                          ; preds = %while13
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
  br label %thrown15

thrown15:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown15.then, label %endthrown

thrown15.then:                                    ; preds = %thrown15
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i8* null

endthrown:                                        ; preds = %thrown15
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
  br label %thrown16

thrown16:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown16.then, label %endthrown

thrown16.then:                                    ; preds = %thrown16
  store i1 false, i1* @ucg.thrown, align 1
  %e.message2 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message2)
  call void @String.drop(%String* %message)
  store i1 true, i1* @ucg.thrown, align 1
  ret %Error zeroinitializer

endthrown:                                        ; preds = %thrown16
  %"$copy" = load %String, %String* %copy, align 8
  call void @String.drop(%String* %e.message)
  br label %thrown17

thrown17:                                         ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown17.then, label %endthrown1

thrown17.then:                                    ; preds = %thrown17
  store i1 false, i1* @ucg.thrown, align 1
  %e.message4 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message4)
  call void @String.drop(%String* %message)
  store i1 true, i1* @ucg.thrown, align 1
  ret %Error zeroinitializer

endthrown1:                                       ; preds = %thrown17
  store %String %"$copy", %String* %e.message, align 8
  %"$e" = load %Error, %Error* %e, align 8
  call void @String.drop(%String* %message)
  ret %Error %"$e"
}

define void @middle() {
entry:
  %error.at = alloca %Error*, align 8
  %e = alloca %Error, align 8
  %error.at6 = alloca %Error*, align 8
  %error.at14 = alloca %Error*, align 8
  br label %try18

try18:                                            ; preds = %entry
  br i1 true, label %try18.then, label %endtry

try18.then:                                       ; preds = %try18
  %call.String.create = call %String @String.create(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0))
  br label %thrown19

thrown19:                                         ; preds = %try18.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown19.then, label %endthrown

thrown19.then:                                    ; preds = %thrown19
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endthrown:                                        ; preds = %thrown19
  %call.Error.create = call %Error @Error.create(%String %call.String.create)
  br label %thrown20

thrown20:                                         ; preds = %endthrown
  %"$ucg.thrown4" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown4", label %thrown20.then, label %endthrown1

thrown20.then:                                    ; preds = %thrown20
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endthrown1:                                       ; preds = %thrown20
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  store %Error %call.Error.create, %Error* %"*error.at", align 8
  store i32 1, i32* @ucg.error_type, align 4
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endtry:                                           ; preds = %endthrown1, %thrown20.then, %thrown19.then, %try18
  br label %catch21

catch21:                                          ; preds = %endtry
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown5" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown5", %EQ
  br i1 %AND, label %catch21.then, label %endcatch

catch21.then:                                     ; preds = %catch21
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at6, align 8
  %"*error.at7" = load %Error*, %Error** %error.at6, align 8
  %"$*error.at" = load %Error, %Error* %"*error.at7", align 8
  store %Error %"$*error.at", %Error* %e, align 8
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  %"$e.message" = load %String, %String* %e.message, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0), %String %"$e.message")
  %call.String.create8 = call %String @String.create(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.2, i32 0, i32 0))
  br label %thrown22

thrown22:                                         ; preds = %catch21.then
  %"$ucg.thrown9" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown9", label %thrown22.then, label %endthrown2

thrown22.then:                                    ; preds = %thrown22
  store i1 false, i1* @ucg.thrown, align 1
  %e.message10 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message10)
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown2:                                       ; preds = %thrown22
  %call.Error.create11 = call %Error @Error.create(%String %call.String.create8)
  br label %thrown23

thrown23:                                         ; preds = %endthrown2
  %"$ucg.thrown12" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown12", label %thrown23.then, label %endthrown3

thrown23.then:                                    ; preds = %thrown23
  store i1 false, i1* @ucg.thrown, align 1
  %e.message13 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message13)
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown3:                                       ; preds = %thrown23
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at14, align 8
  %"*error.at15" = load %Error*, %Error** %error.at14, align 8
  store %Error %call.Error.create11, %Error* %"*error.at15", align 8
  store i32 1, i32* @ucg.error_type, align 4
  store i1 false, i1* @ucg.thrown, align 1
  %e.message16 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message16)
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endcatch:                                         ; preds = %catch21
  br label %rethrow24

rethrow24:                                        ; preds = %endcatch
  %"$ucg.thrown17" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown17", label %rethrow24.then, label %endrethrow

rethrow24.then:                                   ; preds = %rethrow24
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endrethrow:                                       ; preds = %rethrow24
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
  call void @middle()
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
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.3, i32 0, i32 0), %String %"$e.message")
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
  %"*error.at.message" = getelementptr inbounds %Error, %Error* %"*error.at12", i32 0, i32 0
  %call.String.c_str = call i8* @String.c_str(%String* %"*error.at.message")
  br label %thrown31

thrown31:                                         ; preds = %catch30.then
  %"$ucg.thrown13" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown13", label %thrown31.then, label %endthrown2

thrown31.then:                                    ; preds = %thrown31
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown2:                                       ; preds = %thrown31
  %call.dprintf14 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.4, i32 0, i32 0), i8* %call.String.c_str)
  ret i32 1

endcatch3:                                        ; preds = %catch30
  br label %if32

if32:                                             ; preds = %endcatch3
  %"$ucg.thrown15" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown15", label %if32.then, label %endif

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
@str = private unnamed_addr constant [6 x i8] c"inner\00", align 1
@str.1 = private unnamed_addr constant [11 x i8] c"inner: %s\0A\00", align 1
@str.2 = private unnamed_addr constant [11 x i8] c"outer: %s\0A\00", align 1
@str.3 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

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
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap", %"$*self.cap"
  br i1 %LE, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret void

endif:                                            ; preds = %if0
  %"$cap3" = load i32, i32* %cap, align 4
  %"call.alloc<i8>" = call i8* @"alloc<i8>"(i32 %"$cap3")
  br label %thrown1

thrown1:                                          ; preds = %endif
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown1.then, label %endthrown

thrown1.then:                                     ; preds = %thrown1
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown1
  store i8* %"call.alloc<i8>", i8** %bigger, align 8
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 0
  %"*self5" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self5", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count")
  br label %thrown2

thrown2:                                          ; preds = %endthrown
  %"$ucg.thrown6" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown6", label %thrown2.then, label %endthrown1

thrown2.then:                                     ; preds = %thrown2
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown2
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data8" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"$*self.data9" = load i8*, i8** %"*self.data8", align 8
  call void @"free<i8>"(i8* %"$*self.data9")
  br label %thrown3

thrown3:                                          ; preds = %endthrown1
  %"$ucg.thrown10" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown10", label %thrown3.then, label %endthrown2

thrown3.then:                                     ; preds = %thrown3
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown2:                                       ; preds = %thrown3
  %"*self11" = load %String*, %String** %self, align 8
  %"*self.data12" = getelementptr inbounds %String, %String* %"*self11", i32 0, i32 0
  %"$bigger13" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger13", i8** %"*self.data12", align 8
  %"*self14" = load %String*, %String** %self, align 8
  %"*self.cap15" = getelementptr inbounds %String, %String* %"*self14", i32 0, i32 2
  %"$cap16" = load i32, i32* %cap, align 4
  store i32 %"$cap16", i32* %"*self.cap15", align 4
  ret void
}

define void @String.push_back(%String* %0, i8 %1) {
entry:
  %self = alloca %String*, align 8
  %value = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store i8 %1, i8* %value, align 1
  br label %if4

if4:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.count", %"$*self.cap"
  br i1 %EQ, label %if4.then, label %endif

if4.then:                                         ; preds = %if4
  %"*self2" = load %String*, %String** %self, align 8
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.cap4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 2
  %"$*self.cap5" = load i32, i32* %"*self.cap4", align 4
  %MUL = mul i32 %"$*self.cap5", 2
  %ADD = add i32 %MUL, 4
  call void @String.reserve(%String* %"*self2", i32 %ADD)
  br label %thrown5

thrown5:                                          ; preds = %if4.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown5.then, label %endthrown

thrown5.then:                                     ; preds = %thrown5
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown5
  br label %endif

endif:                                            ; preds = %endthrown, %if4
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
  br label %while6

while6:                                           ; preds = %while6.then, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while6.then, label %endloop

while6.then:                                      ; preds = %while6
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count2" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count3" = load i32, i32* %"*self.count2", align 4
  %SUB = sub i32 %"$*self.count3", 1
  store i32 %SUB, i32* %"*self.count2", align 4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 0
  %"*self5" = load %String*, %String** %self, align 8
  %"*self.count6" = getelementptr inbounds %String, %String* %"*self5", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count7" = load i32, i32* %"*self.count6", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count7"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while6

endloop:                                          ; preds = %while6
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
  br label %thrown7

thrown7:                                          ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown7.then, label %endthrown

thrown7.then:                                     ; preds = %thrown7
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown7
  %"*self2" = load %String*, %String** %self, align 8
  %"*other" = load %String*, %String** %other, align 8
  %"*other.count" = getelementptr inbounds %String, %String* %"*other", i32 0, i32 1
  %"$*other.count" = load i32, i32* %"*other.count", align 4
  call void @String.reserve(%String* %"*self2", i32 %"$*other.count")
  br label %thrown8

thrown8:                                          ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown8.then, label %endthrown1

thrown8.then:                                     ; preds = %thrown8
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown8
  store i32 0, i32* %i, align 4
  br label %while9

while9:                                           ; preds = %while9.then, %endthrown1
  %"*other4" = load %String*, %String** %other, align 8
  %"*other.count5" = getelementptr inbounds %String, %String* %"*other4", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*other.count6" = load i32, i32* %"*other.count5", align 4
  %LT = icmp slt i32 %"$i", %"$*other.count6"
  br i1 %LT, label %while9.then, label %endloop

while9.then:                                      ; preds = %while9
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
  br label %while9

endloop:                                          ; preds = %while9
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
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %thrown11

thrown11:                                         ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown11.then, label %endthrown1

thrown11.then:                                    ; preds = %thrown11
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown11
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
  br label %thrown12

thrown12:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown12.then, label %endthrown

thrown12.then:                                    ; preds = %thrown12
  store i1 false, i1* @ucg.thrown, align 1
  call void @String.drop(%String* %res)
  store i1 true, i1* @ucg.thrown, align 1
  ret %String zeroinitializer

endthrown:                                        ; preds = %thrown12
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
  br label %while13

while13:                                          ; preds = %endthrown, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$i" = load i32, i32* %i, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$i"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while13.then, label %endloop

while13.then:                                     ; preds = %while13
  %"*self" = load %String*, %String** %self, align 8
  %"$text1" = load i8*, i8** %text, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"text[]3" = getelementptr inbounds i8, i8* %"$text1", i32 %"$i2"
  %"$text[]4" = load i8, i8* %"text[]3", align 1
  call void @String.push_back(%String* %"*self", i8 %"$text[]4")
  br label %thrown14

thrown14:                                         ; preds = %while13.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown14.then, label %endthrown

thrown14.then:                                    ; preds = %thrown14
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown14
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while13

endloop:                                          ; preds = %while13
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
  br label %thrown15

thrown15:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown15.then, label %endthrown

thrown15.then:                                    ; preds = %thrown15
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i8* null

endthrown:                                        ; preds = %thrown15
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
  br label %thrown16

thrown16:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown16.then, label %endthrown

thrown16.then:                                    ; preds = %thrown16
  store i1 false, i1* @ucg.thrown, align 1
  %e.message2 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message2)
  call void @String.drop(%String* %message)
  store i1 true, i1* @ucg.thrown, align 1
  ret %Error zeroinitializer

endthrown:                                        ; preds = %thrown16
  %"$copy" = load %String, %String* %copy, align 8
  call void @String.drop(%String* %e.message)
  br label %thrown17

thrown17:                                         ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown17.then, label %endthrown1

thrown17.then:                                    ; preds = %thrown17
  store i1 false, i1* @ucg.thrown, align 1
  %e.message4 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message4)
  call void @String.drop(%String* %message)
  store i1 true, i1* @ucg.thrown, align 1
  ret %Error zeroinitializer

endthrown1:                                       ; preds = %thrown17
  store %String %"$copy", %String* %e.message, align 8
  %"$e" = load %Error, %Error* %e, align 8
  call void @String.drop(%String* %message)
  ret %Error %"$e"
}

define i32 @main() {
entry:
  %error.at = alloca %Error*, align 8
  %e = alloca %Error, align 8
  %error.at10 = alloca %Error*, align 8
  %e18 = alloca %Error, align 8
  %error.at19 = alloca %Error*, align 8
  %error.at31 = alloca %Error*, align 8
  br label %try18

try18:                                            ; preds = %entry
  br i1 true, label %try18.then, label %endtry5

try18.then:                                       ; preds = %try18
  br label %try19

try19:                                            ; preds = %try18.then
  br i1 true, label %try19.then, label %endtry2

try19.then:                                       ; preds = %try19
  br label %try20

try20:                                            ; preds = %try19.then
  br i1 true, label %try20.then, label %endtry

try20.then:                                       ; preds = %try20
  %call.String.create = call %String @String.create(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0))
  br label %thrown21

thrown21:                                         ; preds = %try20.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown21.then, label %endthrown

thrown21.then:                                    ; preds = %thrown21
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endthrown:                                        ; preds = %thrown21
  %call.Error.create = call %Error @Error.create(%String %call.String.create)
  br label %thrown22

thrown22:                                         ; preds = %endthrown
  %"$ucg.thrown8" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown8", label %thrown22.then, label %endthrown1

thrown22.then:                                    ; preds = %thrown22
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endthrown1:                                       ; preds = %thrown22
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  store %Error %call.Error.create, %Error* %"*error.at", align 8
  store i32 1, i32* @ucg.error_type, align 4
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endtry:                                           ; preds = %endthrown1, %thrown22.then, %thrown21.then, %try20
  br label %catch23

catch23:                                          ; preds = %endtry
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown9" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown9", %EQ
  br i1 %AND, label %catch23.then, label %endcatch

catch23.then:                                     ; preds = %catch23
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at10, align 8
  %"*error.at11" = load %Error*, %Error** %error.at10, align 8
  %"$*error.at" = load %Error, %Error* %"*error.at11", align 8
  store %Error %"$*error.at", %Error* %e, align 8
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  %"$e.message" = load %String, %String* %e.message, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.1, i32 0, i32 0), %String %"$e.message")
  %e.message12 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message12)
  br label %endcatch

endcatch:                                         ; preds = %catch23.then, %catch23
  br label %rethrow24

rethrow24:                                        ; preds = %endcatch
  %"$ucg.thrown13" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown13", label %rethrow24.then, label %endrethrow

rethrow24.then:                                   ; preds = %rethrow24
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry2

endrethrow:                                       ; preds = %rethrow24
  br label %endtry2

endtry2:                                          ; preds = %endrethrow, %rethrow24.then, %try19
  br label %catch25

catch25:                                          ; preds = %endtry2
  %"$ucg.error_type14" = load i32, i32* @ucg.error_type, align 4
  %EQ15 = icmp eq i32 %"$ucg.error_type14", 1
  %"$ucg.thrown16" = load i1, i1* @ucg.thrown, align 1
  %AND17 = and i1 %"$ucg.thrown16", %EQ15
  br i1 %AND17, label %catch25.then, label %endcatch3

catch25.then:                                     ; preds = %catch25
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at19, align 8
  %"*error.at20" = load %Error*, %Error** %error.at19, align 8
  %"$*error.at21" = load %Error, %Error* %"*error.at20", align 8
  store %Error %"$*error.at21", %Error* %e18, align 8
  %e.message22 = getelementptr inbounds %Error, %Error* %e18, i32 0, i32 0
  %"$e.message23" = load %String, %String* %e.message22, align 8
  %call.dprintf24 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.2, i32 0, i32 0), %String %"$e.message23")
  %e.message25 = getelementptr inbounds %Error, %Error* %e18, i32 0, i32 0
  call void @String.drop(%String* %e.message25)
  br label %endcatch3

endcatch3:                                        ; preds = %catch25.then, %catch25
  br label %rethrow26

rethrow26:                                        ; preds = %endcatch3
  %"$ucg.thrown26" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown26", label %rethrow26.then, label %endrethrow4

rethrow26.then:                                   ; preds = %rethrow26
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry5

endrethrow4:                                      ; preds = %rethrow26
  ret i32 0

endtry5:                                          ; preds = %rethrow26.then, %try18
  br label %catch27

catch27:                                          ; preds = %endtry5
  %"$ucg.error_type27" = load i32, i32* @ucg.error_type, align 4
  %EQ28 = icmp eq i32 %"$ucg.error_type27", 1
  %"$ucg.thrown29" = load i1, i1* @ucg.thrown, align 1
  %AND30 = and i1 %"$ucg.thrown29", %EQ28
  br i1 %AND30, label %catch27.then, label %endcatch7

catch27.then:                                     ; preds = %catch27
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at31, align 8
  %"*error.at32" = load %Error*, %Error** %error.at31, align 8
  %"*error.at.message" = getelementptr inbounds %Error, %Error* %"*error.at32", i32 0, i32 0
  %call.String.c_str = call i8* @String.c_str(%String* %"*error.at.message")
  br label %thrown28

thrown28:                                         ; preds = %catch27.then
  %"$ucg.thrown33" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown33", label %thrown28.then, label %endthrown6

thrown28.then:                                    ; preds = %thrown28
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown6:                                       ; preds = %thrown28
  %call.dprintf34 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.3, i32 0, i32 0), i8* %call.String.c_str)
  ret i32 1

endcatch7:                                        ; preds = %catch27
  br label %if29

if29:                                             ; preds = %endcatch7
  %"$ucg.thrown35" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown35", label %if29.then, label %endif

if29.then:                                        ; preds = %if29
  ret i32 1

endif:                                            ; preds = %if29
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
@str = private unnamed_addr constant [10 x i8] c"ret = %d\0A\00", align 1
@str.1 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

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
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap", %"$*self.cap"
  br i1 %LE, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret void

endif:                                            ; preds = %if0
  %"$cap3" = load i32, i32* %cap, align 4
  %"call.alloc<i8>" = call i8* @"alloc<i8>"(i32 %"$cap3")
  br label %thrown1

thrown1:                                          ; preds = %endif
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown1.then, label %endthrown

thrown1.then:                                     ; preds = %thrown1
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown1
  store i8* %"call.alloc<i8>", i8** %bigger, align 8
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 0
  %"*self5" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self5", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count")
  br label %thrown2

thrown2:                                          ; preds = %endthrown
  %"$ucg.thrown6" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown6", label %thrown2.then, label %endthrown1

thrown2.then:                                     ; preds = %thrown2
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown2
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data8" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"$*self.data9" = load i8*, i8** %"*self.data8", align 8
  call void @"free<i8>"(i8* %"$*self.data9")
  br label %thrown3

thrown3:                                          ; preds = %endthrown1
  %"$ucg.thrown10" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown10", label %thrown3.then, label %endthrown2

thrown3.then:                                     ; preds = %thrown3
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown2:                                       ; preds = %thrown3
  %"*self11" = load %String*, %String** %self, align 8
  %"*self.data12" = getelementptr inbounds %String, %String* %"*self11", i32 0, i32 0
  %"$bigger13" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger13", i8** %"*self.data12", align 8
  %"*self14" = load %String*, %String** %self, align 8
  %"*self.cap15" = getelementptr inbounds %String, %String* %"*self14", i32 0, i32 2
  %"$cap16" = load i32, i32* %cap, align 4
  store i32 %"$cap16", i32* %"*self.cap15", align 4
  ret void
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %while4

while4:                                           ; preds = %while4.then, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count2" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count3" = load i32, i32* %"*self.count2", align 4
  %SUB = sub i32 %"$*self.count3", 1
  store i32 %SUB, i32* %"*self.count2", align 4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 0
  %"*self5" = load %String*, %String** %self, align 8
  %"*self.count6" = getelementptr inbounds %String, %String* %"*self5", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count7" = load i32, i32* %"*self.count6", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count7"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while4

endloop:                                          ; preds = %while4
  ret void
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %thrown5

thrown5:                                          ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown5.then, label %endthrown

thrown5.then:                                     ; preds = %thrown5
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %thrown6

thrown6:                                          ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown6.then, label %endthrown1

thrown6.then:                                     ; preds = %thrown6
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown6
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
  br label %thrown7

thrown7:                                          ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown7.then, label %endthrown

thrown7.then:                                     ; preds = %thrown7
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i8* null

endthrown:                                        ; preds = %thrown7
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

define i32 @pick() {
entry:
  %e = alloca %Error, align 8
  %error.at = alloca %Error*, align 8
  br label %try8

try8:                                             ; preds = %entry
  br i1 true, label %try8.then, label %endtry

try8.then:                                        ; preds = %try8
  ret i32 5

endtry:                                           ; preds = %try8
  br label %catch9

catch9:                                           ; preds = %endtry
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown", %EQ
  br i1 %AND, label %catch9.then, label %endcatch

catch9.then:                                      ; preds = %catch9
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  %"$*error.at" = load %Error, %Error* %"*error.at", align 8
  store %Error %"$*error.at", %Error* %e, align 8
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message)
  ret i32 0

endcatch:                                         ; preds = %catch9
  br label %rethrow10

rethrow10:                                        ; preds = %endcatch
  %"$ucg.thrown1" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown1", label %rethrow10.then, label %endrethrow

rethrow10.then:                                   ; preds = %rethrow10
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endrethrow:                                       ; preds = %rethrow10
  ret i32 0
}

define i32 @main() {
entry:
  %error.at = alloca %Error*, align 8
  br label %try11

try11:                                            ; preds = %entry
  br i1 true, label %try11.then, label %endtry

try11.then:                                       ; preds = %try11
  %call.pick = call i32 @pick()
  br label %thrown12

thrown12:                                         ; preds = %try11.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown12.then, label %endthrown

thrown12.then:                                    ; preds = %thrown12
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endthrown:                                        ; preds = %thrown12
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %call.pick)
  ret i32 0

endtry:                                           ; preds = %thrown12.then, %try11
  br label %catch13

catch13:                                          ; preds = %endtry
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown2" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown2", %EQ
  br i1 %AND, label %catch13.then, label %endcatch

catch13.then:                                     ; preds = %catch13
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  %"*error.at.message" = getelementptr inbounds %Error, %Error* %"*error.at", i32 0, i32 0
  %call.String.c_str = call i8* @String.c_str(%String* %"*error.at.message")
  br label %thrown14

thrown14:                                         ; preds = %catch13.then
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown14.then, label %endthrown1

thrown14.then:                                    ; preds = %thrown14
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown1:                                       ; preds = %thrown14
  %call.dprintf4 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0), i8* %call.String.c_str)
  ret i32 1

endcatch:                                         ; preds = %catch13
  br label %if15

if15:                                             ; preds = %endcatch
  %"$ucg.thrown5" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown5", label %if15.then, label %endif

if15.then:                                        ; preds = %if15
  ret i32 1

endif:                                            ; preds = %if15
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
@str = private unnamed_addr constant [6 x i8] c"i=%d\0A\00", align 1
@str.1 = private unnamed_addr constant [3 x i8] c"x\0A\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"done\0A\00", align 1
@str.3 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

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
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap", %"$*self.cap"
  br i1 %LE, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret void

endif:                                            ; preds = %if0
  %"$cap3" = load i32, i32* %cap, align 4
  %"call.alloc<i8>" = call i8* @"alloc<i8>"(i32 %"$cap3")
  br label %thrown1

thrown1:                                          ; preds = %endif
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown1.then, label %endthrown

thrown1.then:                                     ; preds = %thrown1
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown1
  store i8* %"call.alloc<i8>", i8** %bigger, align 8
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 0
  %"*self5" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self5", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count")
  br label %thrown2

thrown2:                                          ; preds = %endthrown
  %"$ucg.thrown6" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown6", label %thrown2.then, label %endthrown1

thrown2.then:                                     ; preds = %thrown2
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown2
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data8" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"$*self.data9" = load i8*, i8** %"*self.data8", align 8
  call void @"free<i8>"(i8* %"$*self.data9")
  br label %thrown3

thrown3:                                          ; preds = %endthrown1
  %"$ucg.thrown10" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown10", label %thrown3.then, label %endthrown2

thrown3.then:                                     ; preds = %thrown3
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown2:                                       ; preds = %thrown3
  %"*self11" = load %String*, %String** %self, align 8
  %"*self.data12" = getelementptr inbounds %String, %String* %"*self11", i32 0, i32 0
  %"$bigger13" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger13", i8** %"*self.data12", align 8
  %"*self14" = load %String*, %String** %self, align 8
  %"*self.cap15" = getelementptr inbounds %String, %String* %"*self14", i32 0, i32 2
  %"$cap16" = load i32, i32* %cap, align 4
  store i32 %"$cap16", i32* %"*self.cap15", align 4
  ret void
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %while4

while4:                                           ; preds = %while4.then, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count2" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count3" = load i32, i32* %"*self.count2", align 4
  %SUB = sub i32 %"$*self.count3", 1
  store i32 %SUB, i32* %"*self.count2", align 4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 0
  %"*self5" = load %String*, %String** %self, align 8
  %"*self.count6" = getelementptr inbounds %String, %String* %"*self5", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count7" = load i32, i32* %"*self.count6", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count7"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while4

endloop:                                          ; preds = %while4
  ret void
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %thrown5

thrown5:                                          ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown5.then, label %endthrown

thrown5.then:                                     ; preds = %thrown5
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %thrown6

thrown6:                                          ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown6.then, label %endthrown1

thrown6.then:                                     ; preds = %thrown6
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown6
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
  br label %thrown7

thrown7:                                          ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown7.then, label %endthrown

thrown7.then:                                     ; preds = %thrown7
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i8* null

endthrown:                                        ; preds = %thrown7
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

define i32 @main() {
entry:
  %i = alloca i32, align 4
  %e = alloca %Error, align 8
  %error.at = alloca %Error*, align 8
  %error.at15 = alloca %Error*, align 8
  br label %try8

try8:                                             ; preds = %entry
  br i1 true, label %try8.then, label %endtry1

try8.then:                                        ; preds = %try8
  store i32 0, i32* %i, align 4
  br label %while9

while9:                                           ; preds = %endrethrow, %try8.then
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", 3
  br i1 %LT, label %while9.then, label %endloop

while9.then:                                      ; preds = %while9
  br label %try10

try10:                                            ; preds = %while9.then
  br i1 true, label %try10.then, label %endtry

try10.then:                                       ; preds = %try10
  br label %if11

if11:                                             ; preds = %try10.then
  %"$i4" = load i32, i32* %i, align 4
  %EQ = icmp eq i32 %"$i4", 1
  br i1 %EQ, label %if11.then, label %endif

if11.then:                                        ; preds = %if11
  br label %endloop

endif:                                            ; preds = %if11
  %"$i5" = load i32, i32* %i, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %"$i5")
  br label %endtry

endtry:                                           ; preds = %endif, %try10
  br label %catch12

catch12:                                          ; preds = %endtry
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ6 = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown", %EQ6
  br i1 %AND, label %catch12.then, label %endcatch

catch12.then:                                     ; preds = %catch12
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  %"$*error.at" = load %Error, %Error* %"*error.at", align 8
  store %Error %"$*error.at", %Error* %e, align 8
  %call.dprintf7 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0))
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message)
  br label %endcatch

endcatch:                                         ; preds = %catch12.then, %catch12
  br label %rethrow13

rethrow13:                                        ; preds = %endcatch
  %"$ucg.thrown8" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown8", label %rethrow13.then, label %endrethrow

rethrow13.then:                                   ; preds = %rethrow13
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry1

endrethrow:                                       ; preds = %rethrow13
  %"$i9" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i9", 1
  store i32 %ADD, i32* %i, align 4
  br label %while9

endloop:                                          ; preds = %if11.then, %while9
  %call.dprintf10 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0))
  ret i32 0

endtry1:                                          ; preds = %rethrow13.then, %try8
  br label %catch14

catch14:                                          ; preds = %endtry1
  %"$ucg.error_type11" = load i32, i32* @ucg.error_type, align 4
  %EQ12 = icmp eq i32 %"$ucg.error_type11", 1
  %"$ucg.thrown13" = load i1, i1* @ucg.thrown, align 1
  %AND14 = and i1 %"$ucg.thrown13", %EQ12
  br i1 %AND14, label %catch14.then, label %endcatch2

catch14.then:                                     ; preds = %catch14
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at15, align 8
  %"*error.at16" = load %Error*, %Error** %error.at15, align 8
  %"*error.at.message" = getelementptr inbounds %Error, %Error* %"*error.at16", i32 0, i32 0
  %call.String.c_str = call i8* @String.c_str(%String* %"*error.at.message")
  br label %thrown15

thrown15:                                         ; preds = %catch14.then
  %"$ucg.thrown17" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown17", label %thrown15.then, label %endthrown

thrown15.then:                                    ; preds = %thrown15
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown:                                        ; preds = %thrown15
  %call.dprintf18 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.3, i32 0, i32 0), i8* %call.String.c_str)
  ret i32 1

endcatch2:                                        ; preds = %catch14
  br label %if16

if16:                                             ; preds = %endcatch2
  %"$ucg.thrown19" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown19", label %if16.then, label %endif3

if16.then:                                        ; preds = %if16
  ret i32 1

endif3:                                           ; preds = %if16
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
@str = private unnamed_addr constant [10 x i8] c"uncaught\0A\00", align 1
@str.1 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

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
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap", %"$*self.cap"
  br i1 %LE, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret void

endif:                                            ; preds = %if0
  %"$cap3" = load i32, i32* %cap, align 4
  %"call.alloc<i8>" = call i8* @"alloc<i8>"(i32 %"$cap3")
  br label %thrown1

thrown1:                                          ; preds = %endif
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown1.then, label %endthrown

thrown1.then:                                     ; preds = %thrown1
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown1
  store i8* %"call.alloc<i8>", i8** %bigger, align 8
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 0
  %"*self5" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self5", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count")
  br label %thrown2

thrown2:                                          ; preds = %endthrown
  %"$ucg.thrown6" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown6", label %thrown2.then, label %endthrown1

thrown2.then:                                     ; preds = %thrown2
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown2
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data8" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"$*self.data9" = load i8*, i8** %"*self.data8", align 8
  call void @"free<i8>"(i8* %"$*self.data9")
  br label %thrown3

thrown3:                                          ; preds = %endthrown1
  %"$ucg.thrown10" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown10", label %thrown3.then, label %endthrown2

thrown3.then:                                     ; preds = %thrown3
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown2:                                       ; preds = %thrown3
  %"*self11" = load %String*, %String** %self, align 8
  %"*self.data12" = getelementptr inbounds %String, %String* %"*self11", i32 0, i32 0
  %"$bigger13" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger13", i8** %"*self.data12", align 8
  %"*self14" = load %String*, %String** %self, align 8
  %"*self.cap15" = getelementptr inbounds %String, %String* %"*self14", i32 0, i32 2
  %"$cap16" = load i32, i32* %cap, align 4
  store i32 %"$cap16", i32* %"*self.cap15", align 4
  ret void
}

define void @String.push_back(%String* %0, i8 %1) {
entry:
  %self = alloca %String*, align 8
  %value = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store i8 %1, i8* %value, align 1
  br label %if4

if4:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.count", %"$*self.cap"
  br i1 %EQ, label %if4.then, label %endif

if4.then:                                         ; preds = %if4
  %"*self2" = load %String*, %String** %self, align 8
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.cap4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 2
  %"$*self.cap5" = load i32, i32* %"*self.cap4", align 4
  %MUL = mul i32 %"$*self.cap5", 2
  %ADD = add i32 %MUL, 4
  call void @String.reserve(%String* %"*self2", i32 %ADD)
  br label %thrown5

thrown5:                                          ; preds = %if4.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown5.then, label %endthrown

thrown5.then:                                     ; preds = %thrown5
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown5
  br label %endif

endif:                                            ; preds = %endthrown, %if4
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
  br label %while6

while6:                                           ; preds = %while6.then, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while6.then, label %endloop

while6.then:                                      ; preds = %while6
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count2" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count3" = load i32, i32* %"*self.count2", align 4
  %SUB = sub i32 %"$*self.count3", 1
  store i32 %SUB, i32* %"*self.count2", align 4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 0
  %"*self5" = load %String*, %String** %self, align 8
  %"*self.count6" = getelementptr inbounds %String, %String* %"*self5", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count7" = load i32, i32* %"*self.count6", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count7"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while6

endloop:                                          ; preds = %while6
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
  br label %thrown7

thrown7:                                          ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown7.then, label %endthrown

thrown7.then:                                     ; preds = %thrown7
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown7
  %"*self2" = load %String*, %String** %self, align 8
  %"*other" = load %String*, %String** %other, align 8
  %"*other.count" = getelementptr inbounds %String, %String* %"*other", i32 0, i32 1
  %"$*other.count" = load i32, i32* %"*other.count", align 4
  call void @String.reserve(%String* %"*self2", i32 %"$*other.count")
  br label %thrown8

thrown8:                                          ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown8.then, label %endthrown1

thrown8.then:                                     ; preds = %thrown8
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown8
  store i32 0, i32* %i, align 4
  br label %while9

while9:                                           ; preds = %while9.then, %endthrown1
  %"*other4" = load %String*, %String** %other, align 8
  %"*other.count5" = getelementptr inbounds %String, %String* %"*other4", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*other.count6" = load i32, i32* %"*other.count5", align 4
  %LT = icmp slt i32 %"$i", %"$*other.count6"
  br i1 %LT, label %while9.then, label %endloop

while9.then:                                      ; preds = %while9
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
  br label %while9

endloop:                                          ; preds = %while9
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
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %thrown11

thrown11:                                         ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown11.then, label %endthrown1

thrown11.then:                                    ; preds = %thrown11
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown11
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
  br label %thrown12

thrown12:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown12.then, label %endthrown

thrown12.then:                                    ; preds = %thrown12
  store i1 false, i1* @ucg.thrown, align 1
  call void @String.drop(%String* %res)
  store i1 true, i1* @ucg.thrown, align 1
  ret %String zeroinitializer

endthrown:                                        ; preds = %thrown12
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
  br label %while13

while13:                                          ; preds = %endthrown, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$i" = load i32, i32* %i, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$i"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while13.then, label %endloop

while13.then:                                     ; preds = %while13
  %"*self" = load %String*, %String** %self, align 8
  %"$text1" = load i8*, i8** %text, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"text[]3" = getelementptr inbounds i8, i8* %"$text1", i32 %"$i2"
  %"$text[]4" = load i8, i8* %"text[]3", align 1
  call void @String.push_back(%String* %"*self", i8 %"$text[]4")
  br label %thrown14

thrown14:                                         ; preds = %while13.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown14.then, label %endthrown

thrown14.then:                                    ; preds = %thrown14
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown14
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while13

endloop:                                          ; preds = %while13
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
  br label %thrown15

thrown15:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown15.then, label %endthrown

thrown15.then:                                    ; preds = %thrown15
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i8* null

endthrown:                                        ; preds = %thrown15
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
  br label %thrown16

thrown16:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown16.then, label %endthrown

thrown16.then:                                    ; preds = %thrown16
  store i1 false, i1* @ucg.thrown, align 1
  %e.message2 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message2)
  call void @String.drop(%String* %message)
  store i1 true, i1* @ucg.thrown, align 1
  ret %Error zeroinitializer

endthrown:                                        ; preds = %thrown16
  %"$copy" = load %String, %String* %copy, align 8
  call void @String.drop(%String* %e.message)
  br label %thrown17

thrown17:                                         ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown17.then, label %endthrown1

thrown17.then:                                    ; preds = %thrown17
  store i1 false, i1* @ucg.thrown, align 1
  %e.message4 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message4)
  call void @String.drop(%String* %message)
  store i1 true, i1* @ucg.thrown, align 1
  ret %Error zeroinitializer

endthrown1:                                       ; preds = %thrown17
  store %String %"$copy", %String* %e.message, align 8
  %"$e" = load %Error, %Error* %e, align 8
  call void @String.drop(%String* %message)
  ret %Error %"$e"
}

define i32 @main() {
entry:
  %error.at = alloca %Error*, align 8
  %error.at5 = alloca %Error*, align 8
  br label %try18

try18:                                            ; preds = %entry
  br i1 true, label %try18.then, label %endtry

try18.then:                                       ; preds = %try18
  %call.String.create = call %String @String.create(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0))
  br label %thrown19

thrown19:                                         ; preds = %try18.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown19.then, label %endthrown

thrown19.then:                                    ; preds = %thrown19
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endthrown:                                        ; preds = %thrown19
  %call.Error.create = call %Error @Error.create(%String %call.String.create)
  br label %thrown20

thrown20:                                         ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown20.then, label %endthrown1

thrown20.then:                                    ; preds = %thrown20
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endthrown1:                                       ; preds = %thrown20
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  store %Error %call.Error.create, %Error* %"*error.at", align 8
  store i32 1, i32* @ucg.error_type, align 4
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endtry:                                           ; preds = %endthrown1, %thrown20.then, %thrown19.then, %try18
  br label %catch21

catch21:                                          ; preds = %endtry
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown4" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown4", %EQ
  br i1 %AND, label %catch21.then, label %endcatch

catch21.then:                                     ; preds = %catch21
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at5, align 8
  %"*error.at6" = load %Error*, %Error** %error.at5, align 8
  %"*error.at.message" = getelementptr inbounds %Error, %Error* %"*error.at6", i32 0, i32 0
  %call.String.c_str = call i8* @String.c_str(%String* %"*error.at.message")
  br label %thrown22

thrown22:                                         ; preds = %catch21.then
  %"$ucg.thrown7" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown7", label %thrown22.then, label %endthrown2

thrown22.then:                                    ; preds = %thrown22
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown2:                                       ; preds = %thrown22
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0), i8* %call.String.c_str)
  ret i32 1

endcatch:                                         ; preds = %catch21
  br label %if23

if23:                                             ; preds = %endcatch
  %"$ucg.thrown8" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown8", label %if23.then, label %endif

if23.then:                                        ; preds = %if23
  ret i32 1

endif:                                            ; preds = %if23
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
@str = private unnamed_addr constant [10 x i8] c"error %s\0A\00", align 1
@str.1 = private unnamed_addr constant [9 x i8] c"code %d\0A\00", align 1
@str.2 = private unnamed_addr constant [7 x i8] c"after\0A\00", align 1
@str.3 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

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
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap", %"$*self.cap"
  br i1 %LE, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret void

endif:                                            ; preds = %if0
  %"$cap3" = load i32, i32* %cap, align 4
  %"call.alloc<i8>" = call i8* @"alloc<i8>"(i32 %"$cap3")
  br label %thrown1

thrown1:                                          ; preds = %endif
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown1.then, label %endthrown

thrown1.then:                                     ; preds = %thrown1
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown1
  store i8* %"call.alloc<i8>", i8** %bigger, align 8
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 0
  %"*self5" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self5", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count")
  br label %thrown2

thrown2:                                          ; preds = %endthrown
  %"$ucg.thrown6" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown6", label %thrown2.then, label %endthrown1

thrown2.then:                                     ; preds = %thrown2
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown2
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data8" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"$*self.data9" = load i8*, i8** %"*self.data8", align 8
  call void @"free<i8>"(i8* %"$*self.data9")
  br label %thrown3

thrown3:                                          ; preds = %endthrown1
  %"$ucg.thrown10" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown10", label %thrown3.then, label %endthrown2

thrown3.then:                                     ; preds = %thrown3
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown2:                                       ; preds = %thrown3
  %"*self11" = load %String*, %String** %self, align 8
  %"*self.data12" = getelementptr inbounds %String, %String* %"*self11", i32 0, i32 0
  %"$bigger13" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger13", i8** %"*self.data12", align 8
  %"*self14" = load %String*, %String** %self, align 8
  %"*self.cap15" = getelementptr inbounds %String, %String* %"*self14", i32 0, i32 2
  %"$cap16" = load i32, i32* %cap, align 4
  store i32 %"$cap16", i32* %"*self.cap15", align 4
  ret void
}

define void @String.clear(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  br label %while4

while4:                                           ; preds = %while4.then, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while4.then, label %endloop

while4.then:                                      ; preds = %while4
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count2" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count3" = load i32, i32* %"*self.count2", align 4
  %SUB = sub i32 %"$*self.count3", 1
  store i32 %SUB, i32* %"*self.count2", align 4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 0
  %"*self5" = load %String*, %String** %self, align 8
  %"*self.count6" = getelementptr inbounds %String, %String* %"*self5", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count7" = load i32, i32* %"*self.count6", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count7"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while4

endloop:                                          ; preds = %while4
  ret void
}

define void @String.drop(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  call void @String.clear(%String* %"*self")
  br label %thrown5

thrown5:                                          ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown5.then, label %endthrown

thrown5.then:                                     ; preds = %thrown5
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown5
  %"*self2" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %thrown6

thrown6:                                          ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown6.then, label %endthrown1

thrown6.then:                                     ; preds = %thrown6
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown6
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
  br label %thrown7

thrown7:                                          ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown7.then, label %endthrown

thrown7.then:                                     ; preds = %thrown7
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i8* null

endthrown:                                        ; preds = %thrown7
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
  br label %try8

try8:                                             ; preds = %entry
  br i1 true, label %try8.then, label %endtry1

try8.then:                                        ; preds = %try8
  br label %try9

try9:                                             ; preds = %try8.then
  br i1 true, label %try9.then, label %endtry

try9.then:                                        ; preds = %try9
  call void @fail()
  br label %thrown10

thrown10:                                         ; preds = %try9.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown10.then, label %endthrown

thrown10.then:                                    ; preds = %thrown10
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endthrown:                                        ; preds = %thrown10
  br label %endtry

endtry:                                           ; preds = %endthrown, %thrown10.then, %try9
  br label %catch11

catch11:                                          ; preds = %endtry
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 2
  %"$ucg.thrown4" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown4", %EQ
  br i1 %AND, label %catch11.then, label %catch12

catch11.then:                                     ; preds = %catch11
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  %"$*error.at" = load %Error, %Error* %"*error.at", align 8
  store %Error %"$*error.at", %Error* %e, align 8
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  %"$e.message" = load %String, %String* %e.message, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), %String %"$e.message")
  %e.message5 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message5)
  br label %endcatch

catch12:                                          ; preds = %catch11
  %"$ucg.error_type6" = load i32, i32* @ucg.error_type, align 4
  %EQ7 = icmp eq i32 %"$ucg.error_type6", 1
  %"$ucg.thrown8" = load i1, i1* @ucg.thrown, align 1
  %AND9 = and i1 %"$ucg.thrown8", %EQ7
  br i1 %AND9, label %catch12.then, label %endcatch

catch12.then:                                     ; preds = %catch12
  store i1 false, i1* @ucg.thrown, align 1
  store i32* bitcast ([2 x i64]* @ucg.error to i32*), i32** %error.at10, align 8
  %"*error.at11" = load i32*, i32** %error.at10, align 8
  %"$*error.at12" = load i32, i32* %"*error.at11", align 4
  store i32 %"$*error.at12", i32* %code, align 4
  %"$code" = load i32, i32* %code, align 4
  %call.dprintf13 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0), i32 %"$code")
  br label %endcatch

endcatch:                                         ; preds = %catch12.then, %catch12, %catch11.then
  br label %rethrow13

rethrow13:                                        ; preds = %endcatch
  %"$ucg.thrown14" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown14", label %rethrow13.then, label %endrethrow

rethrow13.then:                                   ; preds = %rethrow13
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry1

endrethrow:                                       ; preds = %rethrow13
  %call.dprintf15 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.2, i32 0, i32 0))
  ret i32 0

endtry1:                                          ; preds = %rethrow13.then, %try8
  br label %catch14

catch14:                                          ; preds = %endtry1
  %"$ucg.error_type16" = load i32, i32* @ucg.error_type, align 4
  %EQ17 = icmp eq i32 %"$ucg.error_type16", 2
  %"$ucg.thrown18" = load i1, i1* @ucg.thrown, align 1
  %AND19 = and i1 %"$ucg.thrown18", %EQ17
  br i1 %AND19, label %catch14.then, label %endcatch3

catch14.then:                                     ; preds = %catch14
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at20, align 8
  %"*error.at21" = load %Error*, %Error** %error.at20, align 8
  %"*error.at.message" = getelementptr inbounds %Error, %Error* %"*error.at21", i32 0, i32 0
  %call.String.c_str = call i8* @String.c_str(%String* %"*error.at.message")
  br label %thrown15

thrown15:                                         ; preds = %catch14.then
  %"$ucg.thrown22" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown22", label %thrown15.then, label %endthrown2

thrown15.then:                                    ; preds = %thrown15
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown2:                                       ; preds = %thrown15
  %call.dprintf23 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.3, i32 0, i32 0), i8* %call.String.c_str)
  ret i32 1

endcatch3:                                        ; preds = %catch14
  br label %if16

if16:                                             ; preds = %endcatch3
  %"$ucg.thrown24" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown24", label %if16.then, label %endif

if16.then:                                        ; preds = %if16
  ret i32 1

endif:                                            ; preds = %if16
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
@str = private unnamed_addr constant [2 x i8] c"x\00", align 1
@str.1 = private unnamed_addr constant [12 x i8] c"caught: %s\0A\00", align 1
@str.2 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

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
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 2
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap", %"$*self.cap"
  br i1 %LE, label %if0.then, label %endif

if0.then:                                         ; preds = %if0
  ret void

endif:                                            ; preds = %if0
  %"$cap3" = load i32, i32* %cap, align 4
  %"call.alloc<i8>" = call i8* @"alloc<i8>"(i32 %"$cap3")
  br label %thrown1

thrown1:                                          ; preds = %endif
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown1.then, label %endthrown

thrown1.then:                                     ; preds = %thrown1
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown1
  store i8* %"call.alloc<i8>", i8** %bigger, align 8
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 0
  %"*self5" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self5", i32 0, i32 1
  %"$bigger" = load i8*, i8** %bigger, align 8
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  call void @"copy<i8>"(i8* %"$bigger", i8* %"$*self.data", i32 %"$*self.count")
  br label %thrown2

thrown2:                                          ; preds = %endthrown
  %"$ucg.thrown6" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown6", label %thrown2.then, label %endthrown1

thrown2.then:                                     ; preds = %thrown2
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown2
  %"*self7" = load %String*, %String** %self, align 8
  %"*self.data8" = getelementptr inbounds %String, %String* %"*self7", i32 0, i32 0
  %"$*self.data9" = load i8*, i8** %"*self.data8", align 8
  call void @"free<i8>"(i8* %"$*self.data9")
  br label %thrown3

thrown3:                                          ; preds = %endthrown1
  %"$ucg.thrown10" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown10", label %thrown3.then, label %endthrown2

thrown3.then:                                     ; preds = %thrown3
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown2:                                       ; preds = %thrown3
  %"*self11" = load %String*, %String** %self, align 8
  %"*self.data12" = getelementptr inbounds %String, %String* %"*self11", i32 0, i32 0
  %"$bigger13" = load i8*, i8** %bigger, align 8
  store i8* %"$bigger13", i8** %"*self.data12", align 8
  %"*self14" = load %String*, %String** %self, align 8
  %"*self.cap15" = getelementptr inbounds %String, %String* %"*self14", i32 0, i32 2
  %"$cap16" = load i32, i32* %cap, align 4
  store i32 %"$cap16", i32* %"*self.cap15", align 4
  ret void
}

define void @String.push_back(%String* %0, i8 %1) {
entry:
  %self = alloca %String*, align 8
  %value = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store i8 %1, i8* %value, align 1
  br label %if4

if4:                                              ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.count", %"$*self.cap"
  br i1 %EQ, label %if4.then, label %endif

if4.then:                                         ; preds = %if4
  %"*self2" = load %String*, %String** %self, align 8
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.cap4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 2
  %"$*self.cap5" = load i32, i32* %"*self.cap4", align 4
  %MUL = mul i32 %"$*self.cap5", 2
  %ADD = add i32 %MUL, 4
  call void @String.reserve(%String* %"*self2", i32 %ADD)
  br label %thrown5

thrown5:                                          ; preds = %if4.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown5.then, label %endthrown

thrown5.then:                                     ; preds = %thrown5
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown5
  br label %endif

endif:                                            ; preds = %endthrown, %if4
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
  br label %while6

while6:                                           ; preds = %while6.then, %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while6.then, label %endloop

while6.then:                                      ; preds = %while6
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.count2" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 1
  %"$*self.count3" = load i32, i32* %"*self.count2", align 4
  %SUB = sub i32 %"$*self.count3", 1
  store i32 %SUB, i32* %"*self.count2", align 4
  %"*self4" = load %String*, %String** %self, align 8
  %"*self.data" = getelementptr inbounds %String, %String* %"*self4", i32 0, i32 0
  %"*self5" = load %String*, %String** %self, align 8
  %"*self.count6" = getelementptr inbounds %String, %String* %"*self5", i32 0, i32 1
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  %"$*self.count7" = load i32, i32* %"*self.count6", align 4
  %"*self.data[]" = getelementptr inbounds i8, i8* %"$*self.data", i32 %"$*self.count7"
  store i8 0, i8* %"*self.data[]", align 1
  br label %while6

endloop:                                          ; preds = %while6
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
  br label %thrown7

thrown7:                                          ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown7.then, label %endthrown

thrown7.then:                                     ; preds = %thrown7
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown7
  %"*self2" = load %String*, %String** %self, align 8
  %"*other" = load %String*, %String** %other, align 8
  %"*other.count" = getelementptr inbounds %String, %String* %"*other", i32 0, i32 1
  %"$*other.count" = load i32, i32* %"*other.count", align 4
  call void @String.reserve(%String* %"*self2", i32 %"$*other.count")
  br label %thrown8

thrown8:                                          ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown8.then, label %endthrown1

thrown8.then:                                     ; preds = %thrown8
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown8
  store i32 0, i32* %i, align 4
  br label %while9

while9:                                           ; preds = %while9.then, %endthrown1
  %"*other4" = load %String*, %String** %other, align 8
  %"*other.count5" = getelementptr inbounds %String, %String* %"*other4", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*other.count6" = load i32, i32* %"*other.count5", align 4
  %LT = icmp slt i32 %"$i", %"$*other.count6"
  br i1 %LT, label %while9.then, label %endloop

while9.then:                                      ; preds = %while9
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
  br label %while9

endloop:                                          ; preds = %while9
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
  %"*self.data" = getelementptr inbounds %String, %String* %"*self2", i32 0, i32 0
  %"$*self.data" = load i8*, i8** %"*self.data", align 8
  call void @"free<i8>"(i8* %"$*self.data")
  br label %thrown11

thrown11:                                         ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown11.then, label %endthrown1

thrown11.then:                                    ; preds = %thrown11
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown11
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
  br label %thrown12

thrown12:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown12.then, label %endthrown

thrown12.then:                                    ; preds = %thrown12
  store i1 false, i1* @ucg.thrown, align 1
  call void @String.drop(%String* %res)
  store i1 true, i1* @ucg.thrown, align 1
  ret %String zeroinitializer

endthrown:                                        ; preds = %thrown12
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
  br label %while13

while13:                                          ; preds = %endthrown, %entry
  %"$text" = load i8*, i8** %text, align 8
  %"$i" = load i32, i32* %i, align 4
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 %"$i"
  %"$text[]" = load i8, i8* %"text[]", align 1
  %NQ = icmp ne i8 %"$text[]", 0
  br i1 %NQ, label %while13.then, label %endloop

while13.then:                                     ; preds = %while13
  %"*self" = load %String*, %String** %self, align 8
  %"$text1" = load i8*, i8** %text, align 8
  %"$i2" = load i32, i32* %i, align 4
  %"text[]3" = getelementptr inbounds i8, i8* %"$text1", i32 %"$i2"
  %"$text[]4" = load i8, i8* %"text[]3", align 1
  call void @String.push_back(%String* %"*self", i8 %"$text[]4")
  br label %thrown14

thrown14:                                         ; preds = %while13.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown14.then, label %endthrown

thrown14.then:                                    ; preds = %thrown14
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown14
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while13

endloop:                                          ; preds = %while13
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
  br label %thrown15

thrown15:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown15.then, label %endthrown

thrown15.then:                                    ; preds = %thrown15
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i8* null

endthrown:                                        ; preds = %thrown15
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
  br label %thrown16

thrown16:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown16.then, label %endthrown

thrown16.then:                                    ; preds = %thrown16
  store i1 false, i1* @ucg.thrown, align 1
  %e.message2 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message2)
  call void @String.drop(%String* %message)
  store i1 true, i1* @ucg.thrown, align 1
  ret %Error zeroinitializer

endthrown:                                        ; preds = %thrown16
  %"$copy" = load %String, %String* %copy, align 8
  call void @String.drop(%String* %e.message)
  br label %thrown17

thrown17:                                         ; preds = %endthrown
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown3", label %thrown17.then, label %endthrown1

thrown17.then:                                    ; preds = %thrown17
  store i1 false, i1* @ucg.thrown, align 1
  %e.message4 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message4)
  call void @String.drop(%String* %message)
  store i1 true, i1* @ucg.thrown, align 1
  ret %Error zeroinitializer

endthrown1:                                       ; preds = %thrown17
  store %String %"$copy", %String* %e.message, align 8
  %"$e" = load %Error, %Error* %e, align 8
  call void @String.drop(%String* %message)
  ret %Error %"$e"
}

define void @boom() {
entry:
  %error.at = alloca %Error*, align 8
  %call.String.create = call %String @String.create(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  br label %thrown18

thrown18:                                         ; preds = %entry
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown18.then, label %endthrown

thrown18.then:                                    ; preds = %thrown18
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown18
  %call.Error.create = call %Error @Error.create(%String %call.String.create)
  br label %thrown19

thrown19:                                         ; preds = %endthrown
  %"$ucg.thrown2" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown2", label %thrown19.then, label %endthrown1

thrown19.then:                                    ; preds = %thrown19
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown1:                                       ; preds = %thrown19
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
  br label %try20

try20:                                            ; preds = %entry
  br i1 true, label %try20.then, label %endtry1

try20.then:                                       ; preds = %try20
  br label %try21

try21:                                            ; preds = %try20.then
  br i1 true, label %try21.then, label %endtry

try21.then:                                       ; preds = %try21
  call void @boom()
  br label %thrown22

thrown22:                                         ; preds = %try21.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown22.then, label %endthrown

thrown22.then:                                    ; preds = %thrown22
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endthrown:                                        ; preds = %thrown22
  br label %endtry

endtry:                                           ; preds = %endthrown, %thrown22.then, %try21
  br label %catch23

catch23:                                          ; preds = %endtry
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown4" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown4", %EQ
  br i1 %AND, label %catch23.then, label %endcatch

catch23.then:                                     ; preds = %catch23
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  %"$*error.at" = load %Error, %Error* %"*error.at", align 8
  store %Error %"$*error.at", %Error* %e, align 8
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  %"$e.message" = load %String, %String* %e.message, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.1, i32 0, i32 0), %String %"$e.message")
  %e.message5 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  call void @String.drop(%String* %e.message5)
  br label %endcatch

endcatch:                                         ; preds = %catch23.then, %catch23
  br label %rethrow24

rethrow24:                                        ; preds = %endcatch
  %"$ucg.thrown6" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown6", label %rethrow24.then, label %endrethrow

rethrow24.then:                                   ; preds = %rethrow24
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry1

endrethrow:                                       ; preds = %rethrow24
  ret i32 0

endtry1:                                          ; preds = %rethrow24.then, %try20
  br label %catch25

catch25:                                          ; preds = %endtry1
  %"$ucg.error_type7" = load i32, i32* @ucg.error_type, align 4
  %EQ8 = icmp eq i32 %"$ucg.error_type7", 1
  %"$ucg.thrown9" = load i1, i1* @ucg.thrown, align 1
  %AND10 = and i1 %"$ucg.thrown9", %EQ8
  br i1 %AND10, label %catch25.then, label %endcatch3

catch25.then:                                     ; preds = %catch25
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at11, align 8
  %"*error.at12" = load %Error*, %Error** %error.at11, align 8
  %"*error.at.message" = getelementptr inbounds %Error, %Error* %"*error.at12", i32 0, i32 0
  %call.String.c_str = call i8* @String.c_str(%String* %"*error.at.message")
  br label %thrown26

thrown26:                                         ; preds = %catch25.then
  %"$ucg.thrown13" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown13", label %thrown26.then, label %endthrown2

thrown26.then:                                    ; preds = %thrown26
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown2:                                       ; preds = %thrown26
  %call.dprintf14 = call i32 (i32, i8*, ...) @dprintf(i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.2, i32 0, i32 0), i8* %call.String.c_str)
  ret i32 1

endcatch3:                                        ; preds = %catch25
  br label %if27

if27:                                             ; preds = %endcatch3
  %"$ucg.thrown15" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown15", label %if27.then, label %endif

if27.then:                                        ; preds = %if27
  ret i32 1

endif:                                            ; preds = %if27
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

