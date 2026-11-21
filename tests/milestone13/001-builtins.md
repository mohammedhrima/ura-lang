# milestone13

## index

- 001 — builtin: default argument promotion in a user variadic call
- 002 — directive: @if/@elif/@else selects the matching platform branch
- 003 — directive: @if unix matches on macOS and linux
- 004 — directive: a non-matching branch is dropped before parsing
- 005 — directive: a platform-gated top-level proto
- 006 — directive: @if selects a value inside a function
- 010 — function: fn-typed value: assign top-level fn to a local, call indirectly
- 011 — function: fn-typed parameter: callback passed to higher-order fn
- 012 — function: store a function in a fn-typed variable, then call it
- 013 — function: pass a function as an argument (higher-order)
- 014 — function: nested non-capturing helper function
- 015 — function: a function-typed parameter given a value
- 016 — function: proto then definition, and definition then proto
- 018 — function: conflict between two of your own protos
- 019 — function: calling a null (zero-init) function value traps with file:line
- 020 — function: infinite recursion overflows the stack
- 025 — project: calc
- 026 — project: libft (library and its main)
- 027 — project: net server
- 028 — project: net client
- 029 — dungeon: floor04
- 030 — dungeon: floor09b
- 031 — dungeon: floor14a
- 032 — dungeon: boss
- 034 — libft: isalnum
- 035 — libft: isalpha
- 036 — libft: isdigit
- 037 — libft: lstnew — new nodes, add_back, add_front and size
- 038 — libft: lstlast — returns a reference to the last node
- 039 — libft: lstiter — apply a function to each node's content in place
- 040 — libft: lstdelone — unlink a node by index and clear the list
- 041 — link: an SDL2 window drawn from ura
- 042 — link: a library that is not installed
- 043 — macros: @if / @elif / @else on os and installed()
- 044 — macros: @error stops the build with a message and a help
- 045 — macros: @if on something it can't check

---

## 001 — builtin: default argument promotion in a user variadic call

```ura
// stdout: 3.50 A 7 1 42
// stdout: 123456789
// stdout: 2.50 Z 9
// stdout: 7.00 43
// stdout: 1 0
// stdout: none
// expect: 0
use "@/core"

proto printf(format i8[], ...) i32

fn main() i32:
    f f32 = 3.5
    c char  = 'A'
    s i16 = 7 as i16
    b bool  = True
    l i64  = 123456789 as i64
    i i32   = 42

    printf("%.2f %c %d %d %d\n", f, c, s, b, i)
    printf("%ld\n", l)
    printf("%.2f %c %d\n", 2.5, 'Z', 9)
    printf("%.2f %d\n", f + f, i + 1)
    printf("%d %d\n", 1 < 2, 3 == 4)
    printf("none\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [18 x i8] c"%.2f %c %d %d %d\0A\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"%ld\0A\00", align 1
@str.2 = private unnamed_addr constant [12 x i8] c"%.2f %c %d\0A\00", align 1
@str.3 = private unnamed_addr constant [9 x i8] c"%.2f %d\0A\00", align 1
@str.4 = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1
@str.5 = private unnamed_addr constant [6 x i8] c"none\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %f = alloca float, align 4
  %c = alloca i8, align 1
  %s = alloca i16, align 2
  %b = alloca i1, align 1
  %l = alloca i64, align 8
  %i = alloca i32, align 4
  store float 3.500000e+00, float* %f, align 4
  store i8 65, i8* %c, align 1
  store i16 7, i16* %s, align 2
  store i1 true, i1* %b, align 1
  store i64 123456789, i64* %l, align 8
  store i32 42, i32* %i, align 4
  %"$f" = load float, float* %f, align 4
  %"$c" = load i8, i8* %c, align 1
  %"$s" = load i16, i16* %s, align 2
  %"$b" = load i1, i1* %b, align 1
  %"$i" = load i32, i32* %i, align 4
  %0 = fpext float %"$f" to double
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str, i32 0, i32 0), double %0, i8 %"$c", i16 %"$s", i1 %"$b", i32 %"$i")
  %"$l" = load i64, i64* %l, align 8
  %call.printf1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0), i64 %"$l")
  %call.printf2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.2, i32 0, i32 0), double 2.500000e+00, i8 90, i32 9)
  %"$f3" = load float, float* %f, align 4
  %"$f4" = load float, float* %f, align 4
  %ADD = fadd float %"$f3", %"$f4"
  %"$i5" = load i32, i32* %i, align 4
  %ADD6 = add i32 %"$i5", 1
  %1 = fpext float %ADD to double
  %call.printf7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.3, i32 0, i32 0), double %1, i32 %ADD6)
  %call.printf8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.4, i32 0, i32 0), i1 true, i1 false)
  %call.printf9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.5, i32 0, i32 0))
  ret i32 0
}
```

---

## 002 — directive: @if/@elif/@else selects the matching platform branch

```ura
// stdout: unix
// expect: 0
use "@/core"
fn main() i32:
    @if os == "macos":
        io::out("unix\n")
    @elif os == "linux":
        io::out("unix\n")
    @else:
        io::out("other\n")
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

@str = private unnamed_addr constant [6 x i8] c"unix\0A\00", align 1

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

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
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
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i32 @main() {
entry:
  call void @"io::out"(%String { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 5, i32 0 })
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)
```

---

## 003 — directive: @if unix matches on macOS and linux

```ura
// stdout: posix
// expect: 0
use "@/core"
fn main() i32:
    @if os == "macos" or os == "linux":
        io::out("posix\n")
    @else:
        io::out("windows\n")
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

@str = private unnamed_addr constant [7 x i8] c"posix\0A\00", align 1

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

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
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
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i32 @main() {
entry:
  call void @"io::out"(%String { i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 6, i32 0 })
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)
```

---

## 004 — directive: a non-matching branch is dropped before parsing

```ura
// stdout: kept
// expect: 0
use "@/core"
fn main() i32:
    @if os == "plan9":
        undefined_that_would_error()
    @else:
        io::out("kept\n")
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

@str = private unnamed_addr constant [6 x i8] c"kept\0A\00", align 1

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

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
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
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i32 @main() {
entry:
  call void @"io::out"(%String { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 5, i32 0 })
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)
```

---

## 005 — directive: a platform-gated top-level proto

```ura
// stdout: True
// expect: 0
use "@/core"
@if os == "macos" or os == "linux":
    proto getpid() i32
@else:
    proto GetCurrentProcessId() u32

fn main() i32:
    io::out($"{getpid() > 0}\n")
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

@str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1

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

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
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
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

declare i32 @getpid()

define i32 @main() {
entry:
  %format.size = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text = alloca i8*, align 8
  %call.getpid = call i32 @getpid()
  %GT = icmp sgt i32 %call.getpid, 0
  store i32 0, i32* %format.size, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if6

if6:                                              ; preds = %entry
  br i1 %GT, label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else7:                                            ; preds = %if6
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i8* %"$bool.text")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %"$bool.text1" = load i8*, i8** %bool.text, align 8
  %call.snprintf2 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i8* %"$bool.text1")
  %"$format.size3" = load i32, i32* %format.size, align 4
  %ADD4 = add i32 %"$format.size3", 1
  %"$format.text5" = load i8*, i8** %format.text, align 8
  %"$format.size6" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text5", 0
  %1 = insertvalue %String %0, i32 %"$format.size6", 1
  %2 = insertvalue %String %1, i32 %ADD4, 2
  call void @"io::out"(%String %2)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

---

## 006 — directive: @if selects a value inside a function

```ura
// stdout: 1
// expect: 0
use "@/core"
fn tag() i32:
    @if os == "macos":
        return 1
    @elif os == "linux":
        return 1
    @else:
        return 0

fn main() i32:
    io::out($"{tag()}\n")
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

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

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

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
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
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i32 @tag() {
entry:
  ret i32 1
}

define i32 @main() {
entry:
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %call.tag = call i32 @tag()
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %call.tag)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %call.tag)
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  call void @"io::out"(%String %2)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

---

## 010 — function: fn-typed value: assign top-level fn to a local, call indirectly

```ura
// stdout: 5
// stdout: 6
// expect: 0
use "@/core"

fn add(a i32, b i32) i32:
    return a + b

fn sub(a i32, b i32) i32:
    return a - b

fn main() i32:
    op fn(i32, i32) i32 = add
    io::out($"{op(2, 3)}\n")

    op = sub
    io::out($"{op(10, 4)}\n")
    return 0
```

---

## 011 — function: fn-typed parameter: callback passed to higher-order fn

```ura
// stdout: 42
// stdout: 21
// expect: 0
use "@/core"

fn double_it(n i32) i32:
    return n * 2

fn triple_it(n i32) i32:
    return n * 3

fn apply(cb fn(i32) i32, x i32) i32:
    return cb(x)

fn main() i32:
    io::out($"{apply(double_it, 21)}\n")
    io::out($"{apply(triple_it, 7)}\n")
    return 0
```

---

## 012 — function: store a function in a fn-typed variable, then call it

```ura
// expect: 42
use "@/core"

fn twice(n i32) i32:
    return n + n
fn main() i32:
    f fn(i32) i32 = twice
    return f(21)
```

---

## 013 — function: pass a function as an argument (higher-order)

```ura
// expect: 42
use "@/core"

fn twice(n i32) i32:
    return n + n
fn apply(f fn(i32) i32, x i32) i32:
    return f(x)
fn main() i32:
    return apply(twice, 21)
```

---

## 014 — function: nested non-capturing helper function

```ura
// expect: 42
use "@/core"

fn main() i32:
    fn dbl(k i32) i32:
        return k + k
    return dbl(21)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @main() {
entry:
  %call.dbl = call i32 @dbl(i32 21)
  ret i32 %call.dbl
}

define i32 @dbl(i32 %0) {
entry:
  %k = alloca i32, align 4
  store i32 %0, i32* %k, align 4
  %"$k" = load i32, i32* %k, align 4
  %"$k1" = load i32, i32* %k, align 4
  %ADD = add i32 %"$k", %"$k1"
  ret i32 %ADD
}
```

---

## 015 — function: a function-typed parameter given a value

```ura
// error: Argument 1 type mismatch in call to 'apply'
use "@/core"

fn twice(n i32) i32:
    return n + n

fn apply(f fn(i32) i32, x i32) i32:
    return f(x)

fn main() i32:
    return apply(5, 21)
```

### stderr

```
error: test.ura:7:10 parameter 'f' has no type
  |
7 | fn apply(f fn(i32) i32, x i32) i32:
  |          ^
help: like: f i32
error: test.ura:7:12 expected ',' between parameters
  |
7 | fn apply(f fn(i32) i32, x i32) i32:
  |            ^^
error: test.ura:7:12 expected ')' after the parameters
  |
7 | fn apply(f fn(i32) i32, x i32) i32:
  |            ^^
help: use fn apply(f) fn(i32) i32, x i32) i32:
error: test.ura:7:12 expected ':' after the function header
  |
7 | fn apply(f fn(i32) i32, x i32) i32:
  |            ^^
help: use fn apply(f: fn(i32) i32, x i32) i32:
error: test.ura:7:15 expected ')' after the parameters
  |
7 | fn apply(f fn(i32) i32, x i32) i32:
  |               ^^^
help: use fn apply(f fn()i32) i32, x i32) i32:
error: test.ura:7:18 expected ':' after the function header
  |
7 | fn apply(f fn(i32) i32, x i32) i32:
  |                  ^
help: use fn apply(f fn(i32:) i32, x i32) i32:
error: test.ura:7:18 unexpected ')'
  |
7 | fn apply(f fn(i32) i32, x i32) i32:
  |                  ^
error: test.ura:7:23 unexpected ','
  |
7 | fn apply(f fn(i32) i32, x i32) i32:
  |                       ^
error: test.ura:7:30 unexpected ')'
  |
7 | fn apply(f fn(i32) i32, x i32) i32:
  |                              ^
error: test.ura:7:35 unexpected ':'
  |
7 | fn apply(f fn(i32) i32, x i32) i32:
  |                                   ^
error: aborting due to 10 errors
```

### status

```
1
```

---

## 016 — function: proto then definition, and definition then proto

```ura
// stdout: 42 42
// expect: 0
use "@/core"

proto tally(n i32) i32

fn tally(n i32) i32:
    return n + 1

fn double(n i32) i32:
    return n * 2

proto double(n i32) i32

fn main() i32:
    io::out($"{tally(41)} {double(21)}\n")
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

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
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
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i32 @tally(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %ADD = add i32 %"$n", 1
  ret i32 %ADD
}

define i32 @double(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %MUL = mul i32 %"$n", 2
  ret i32 %MUL
}

define i32 @main() {
entry:
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %call.tally = call i32 @tally(i32 41)
  %call.double = call i32 @double(i32 21)
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %call.tally, i32 %call.double)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %call.tally, i32 %call.double)
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  call void @"io::out"(%String %2)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

---

## 018 — function: conflict between two of your own protos

```ura
// error: Conflicting declaration of 'scan': expected 2 parameter(s), found 3
use "@/core"

proto scan(a i32, b i8[]) i32

proto scan(a i32, b i8[], c i32) i32

fn main() i32:
    io::out("unreachable\n")
    return 0
```

### stderr

```
error: test.ura:6:7 'scan' is already declared with other parameters, on line 4
  |
6 | proto scan(a i32, b i8[], c i32) i32
  |       ^^^^
help: declare it once, with ... for the optional arguments
error: aborting due to 1 error
```

### status

```
1
```

---

## 019 — function: calling a null (zero-init) function value traps with file:line

```ura
// stderr: runtime error: Call to a null function value
// expect: 1
use "@/core"

fn main() i32:
    f fn(i32) i32
    return f(21)
```

---

## 020 — function: infinite recursion overflows the stack

```ura
// expect: 139
use "@/core"

fn recurse(n i32) i32:
    return recurse(n)

fn main() i32:
    return recurse(1)
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

define i32 @recurse(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %call.recurse = call i32 @recurse(i32 %"$n")
  ret i32 %call.recurse
}

define i32 @main() {
entry:
  %call.recurse = call i32 @recurse(i32 1)
  ret i32 %call.recurse
}
```

---

## 025 — project: calc

```ura
// stdin: 2 * (3 + 4) - 10 / 5
// stdin: 8 / (3 - 3)
// stdout: ura-calc: type an expression, an empty line quits
// stdout: > calculate expr:  2 * (3 + 4) - 10 / 5
// stdout: ast:
// stdout: op: -
// stdout: ├─ op: *
// stdout: │  ├─ 2
// stdout: │  └─ op: +
// stdout: │     ├─ 3
// stdout: │     └─ 4
// stdout: └─ op: /
// stdout:    ├─ 10
// stdout:    └─ 5
// stdout: result: 12
// stdout: ========================================
// stdout: > calculate expr:  8 / (3 - 3)
// stdout: ast:
// stdout: op: /
// stdout: ├─ 8
// stdout: └─ op: -
// stdout:    ├─ 3
// stdout:    └─ 3
// stdout: error: division by zero
// stdout: ========================================
// stdout: >
// expect: 0
use "@/core"

enum Kind: END, NUMBER, ADD, SUB, MUL, DIV, LPAREN, RPAREN

struct Token:
    kind Kind
    value i32

struct Node:
    token Token
    left i32
    right i32

fn is_digit(c char) bool:
    return c >= '0' and c <= '9'

fn lex(src String) Array<Token>:
    toks Array<Token>
    i i32 = 0
    while i < src.len():
        c char = src[i]
        t Token
        if c == ' ':
            i += 1
            continue
        if is_digit(c):
            t.kind = Kind::NUMBER
            while i < src.len() and is_digit(src[i]):
                t.value = t.value * 10 + ((src[i] as i32) - 48)
                i += 1
            toks.append(t)
            continue
        match c:
            case '+': t.kind = Kind::ADD
            case '-': t.kind = Kind::SUB
            case '*': t.kind = Kind::MUL
            case '/': t.kind = Kind::DIV
            case '(': t.kind = Kind::LPAREN
            case ')': t.kind = Kind::RPAREN
        toks.append(t)
        i += 1
    return toks

fn sign(kind Kind) String:
    match kind:
        case Kind::ADD: return "+"
        case Kind::SUB: return "-"
        case Kind::MUL: return "*"
        case Kind::DIV: return "/"
    return "?"


struct Parser:
    toks Array<Token>
    pos i32
    nodes Array<Node>

    fn peek() Kind:
        if self.pos < self.toks.len():
            return self.toks[self.pos].kind
        return Kind::END

    fn add(token Token, left i32, right i32) i32:
        n Node
        n.token = token
        n.left = left
        n.right = right
        self.nodes.append(n)
        return self.nodes.len() - 1

    fn factor() i32:
        t Token = self.toks[self.pos]
        self.pos += 1
        if t.kind == Kind::LPAREN:
            inner i32 = self.expr()
            self.pos += 1
            return inner
        return self.add(t, -1, -1)

    fn term() i32:
        left i32 = self.factor()
        while self.peek() == Kind::MUL or self.peek() == Kind::DIV:
            t Token = self.toks[self.pos]
            self.pos += 1
            right i32 = self.factor()
            left = self.add(t, left, right)
        return left

    fn expr() i32:
        left i32 = self.term()
        while self.peek() == Kind::ADD or self.peek() == Kind::SUB:
            t Token = self.toks[self.pos]
            self.pos += 1
            right i32 = self.term()
            left = self.add(t, left, right)
        return left

    fn label(i i32) String:
        n Node = self.nodes[i]
        if n.token.kind == Kind::NUMBER:
            return String::create(n.token.value)
        return $"op: {sign(n.token.kind)}"

    fn show(i i32, pre String, last bool):
        branch String = "├─ "
        more String = "│  "
        if last:
            branch = "└─ "
            more = "   "
        io::out($"{pre}{branch}{self.label(i)}\n")
        n Node = self.nodes[i]
        kid String = pre + more
        if n.left >= 0:
            self.show(n.left, kid, n.right < 0)
        if n.right >= 0:
            self.show(n.right, kid, True)

    fn eval(i i32) i32:
        n Node = self.nodes[i]
        match n.token.kind:
            case Kind::NUMBER: return n.token.value
            case Kind::ADD: return self.eval(n.left) + self.eval(n.right)
            case Kind::SUB: return self.eval(n.left) - self.eval(n.right)
            case Kind::MUL: return self.eval(n.left) * self.eval(n.right)
            case Kind::DIV:
                d i32 = self.eval(n.right)
                if d == 0:
                    throw Error::create("division by zero")
                return self.eval(n.left) / d
        return 0


fn calc(text String):
    io::out($"calculate expr:  {text}\nast:\n")
    p Parser
    p.toks = lex(text)
    root i32 = p.expr()
    io::out($"{p.label(root)}\n")
    n Node = p.nodes[root]
    if n.left >= 0:
        p.show(n.left, "", n.right < 0)
    if n.right >= 0:
        p.show(n.right, "", True)
    io::out($"result: {p.eval(root)}\n")

fn main() i32:
    io::out("ura-calc: type an expression, an empty line quits\n")
    io::out("> ")
    line String = io::input()
    while not line.is_empty():
        try:
            calc(line)
        catch e Error:
            io::out($"error: {e.message}\n")
        io::out("========================================\n")
        io::out("> ")
        line = io::input()
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
%Error = type { %String }
%Token = type { i32, i32 }
%"Array<Token>" = type { %Token*, i32, i32 }
%Node = type { %Token, i32, i32 }
%"Array<Node>" = type { %Node*, i32, i32 }
%Parser = type { %"Array<Token>", i32, %"Array<Node>" }

@ucg.thrown = global i1 false
@ucg.error = global [2 x i64] zeroinitializer
@ucg.error_type = global i32 0
@str = private unnamed_addr constant [2 x i8] c"+\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"-\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"*\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"/\00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"?\00", align 1
@str.5 = private unnamed_addr constant [7 x i8] c"op: %s\00", align 1
@str.6 = private unnamed_addr constant [8 x i8] c"\E2\94\9C\E2\94\80 \00", align 1
@str.7 = private unnamed_addr constant [6 x i8] c"\E2\94\82  \00", align 1
@str.8 = private unnamed_addr constant [8 x i8] c"\E2\94\94\E2\94\80 \00", align 1
@str.9 = private unnamed_addr constant [4 x i8] c"   \00", align 1
@str.10 = private unnamed_addr constant [8 x i8] c"%s%s%s\0A\00", align 1
@str.11 = private unnamed_addr constant [17 x i8] c"division by zero\00", align 1
@str.12 = private unnamed_addr constant [42 x i8] c"========================================\0A\00", align 1
@str.13 = private unnamed_addr constant [26 x i8] c"calculate expr:  %s\0Aast:\0A\00", align 1
@str.14 = private unnamed_addr constant [4 x i8] c"%s\0A\00", align 1
@str.15 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.16 = private unnamed_addr constant [1 x i8] zeroinitializer, align 1
@str.17 = private unnamed_addr constant [12 x i8] c"result: %d\0A\00", align 1
@str.18 = private unnamed_addr constant [11 x i8] c"error: %s\0A\00", align 1
@str.19 = private unnamed_addr constant [42 x i8] c"========================================\0A\00", align 1

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

define i8* @"Array<i8>.[]"(%"Array<i8>"* %0, i32 %1) {
entry:
  %left = alloca %"Array<i8>"*, align 8
  %index = alloca i32, align 4
  store %"Array<i8>"* %0, %"Array<i8>"** %left, align 8
  store i32 %1, i32* %index, align 4
  %"*left" = load %"Array<i8>"*, %"Array<i8>"** %left, align 8
  call void @"Array<i8>.own"(%"Array<i8>"* %"*left")
  %"*left1" = load %"Array<i8>"*, %"Array<i8>"** %left, align 8
  %"*left.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*left1", i32 0, i32 0
  %"$*left.data" = load i8*, i8** %"*left.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*left.data[]" = getelementptr inbounds i8, i8* %"$*left.data", i32 %"$index"
  ret i8* %"*left.data[]"
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
  br label %while6

while6:                                           ; preds = %while6.then, %entry
  %"*right2" = load %"Array<i8>"*, %"Array<i8>"** %right, align 8
  %"*right.count3" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*right2", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*right.count4" = load i32, i32* %"*right.count3", align 4
  %LT = icmp slt i32 %"$i", %"$*right.count4"
  br i1 %LT, label %while6.then, label %endloop

while6.then:                                      ; preds = %while6
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
  br label %while6

endloop:                                          ; preds = %while6
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
  br label %if7

if7:                                              ; preds = %entry
  %"*value1" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if7.then, label %endif

if7.then:                                         ; preds = %if7
  %"*value2" = load %"Array<i8>"*, %"Array<i8>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<i8>", %"Array<i8>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load i8*, i8** %"*value.data", align 8
  call void @"free<i8>"(i8* %"$*value.data")
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

define void @String.append(%String* %0, i8 %1) {
entry:
  %self = alloca %String*, align 8
  %value = alloca i8, align 1
  store %String* %0, %String** %self, align 8
  store i8 %1, i8* %value, align 1
  br label %if11

if11:                                             ; preds = %entry
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"*self1" = load %String*, %String** %self, align 8
  %"*self.cap" = getelementptr inbounds %String, %String* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if11.then, label %endif

if11.then:                                        ; preds = %if11
  %"*self2" = load %String*, %String** %self, align 8
  %"*self3" = load %String*, %String** %self, align 8
  %"*self.count4" = getelementptr inbounds %String, %String* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @String.reserve(%String* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if11.then, %if11
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

define i1 @String.is_empty(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %EQ = icmp eq i32 %"$*self.count", 0
  ret i1 %EQ
}

define %String @String.create(i32 %0) {
entry:
  %number = alloca i32, align 4
  %res = alloca %String, align 8
  %n = alloca i32, align 4
  store i32 %0, i32* %number, align 4
  store %String zeroinitializer, %String* %res, align 8
  %"$number" = load i32, i32* %number, align 4
  store i32 %"$number", i32* %n, align 4
  br label %if12

if12:                                             ; preds = %entry
  %"$n" = load i32, i32* %n, align 4
  %LT = icmp slt i32 %"$n", 0
  br i1 %LT, label %if12.then, label %endif

if12.then:                                        ; preds = %if12
  %"$n3" = load i32, i32* %n, align 4
  %SUB = sub i32 0, %"$n3"
  store i32 %SUB, i32* %n, align 4
  br label %endif

endif:                                            ; preds = %if12.then, %if12
  br label %while13

while13:                                          ; preds = %endif1, %endif
  br i1 true, label %while13.then, label %endloop

while13.then:                                     ; preds = %while13
  %"$n4" = load i32, i32* %n, align 4
  %MOD = srem i32 %"$n4", 10
  %ADD = add i32 %MOD, 48
  %AS = trunc i32 %ADD to i8
  call void @String.append(%String* %res, i8 %AS)
  %"$n5" = load i32, i32* %n, align 4
  %DIV = sdiv i32 %"$n5", 10
  store i32 %DIV, i32* %n, align 4
  br label %if14

if14:                                             ; preds = %while13.then
  %"$n6" = load i32, i32* %n, align 4
  %EQ = icmp eq i32 %"$n6", 0
  br i1 %EQ, label %if14.then, label %endif1

if14.then:                                        ; preds = %if14
  br label %endloop

endif1:                                           ; preds = %if14
  br label %while13

endloop:                                          ; preds = %if14.then, %while13
  br label %if15

if15:                                             ; preds = %endloop
  %"$number7" = load i32, i32* %number, align 4
  %LT8 = icmp slt i32 %"$number7", 0
  br i1 %LT8, label %if15.then, label %endif2

if15.then:                                        ; preds = %if15
  call void @String.append(%String* %res, i8 45)
  br label %endif2

endif2:                                           ; preds = %if15.then, %if15
  %call.String.reverse = call %String @String.reverse(%String* %res)
  %1 = bitcast %String* %res to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret %String %call.String.reverse
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
  br label %while16

while16:                                          ; preds = %while16.then, %entry
  %"$i" = load i32, i32* %i, align 4
  %GE = icmp sge i32 %"$i", 0
  br i1 %GE, label %while16.then, label %endloop

while16.then:                                     ; preds = %while16
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
  br label %while16

endloop:                                          ; preds = %while16
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
}

define void @"String.+="(%String* %0, %String* %1) {
entry:
  %left = alloca %String*, align 8
  %right = alloca %String*, align 8
  %i = alloca i32, align 4
  store %String* %0, %String** %left, align 8
  store %String* %1, %String** %right, align 8
  store i32 0, i32* %i, align 4
  br label %while17

while17:                                          ; preds = %while17.then, %entry
  %"*right" = load %String*, %String** %right, align 8
  %"*right.count" = getelementptr inbounds %String, %String* %"*right", i32 0, i32 1
  %"$i" = load i32, i32* %i, align 4
  %"$*right.count" = load i32, i32* %"*right.count", align 4
  %LT = icmp slt i32 %"$i", %"$*right.count"
  br i1 %LT, label %while17.then, label %endloop

while17.then:                                     ; preds = %while17
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
  br label %while17

endloop:                                          ; preds = %while17
  ret void
}

define %String @"String.+"(%String* %0, %String* %1) {
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
  call void @"String.+="(%String* %res, %String* %"*right")
  %"$res" = load %String, %String* %res, align 8
  ret %String %"$res"
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
  %1 = bitcast %String* %copy to %"Array<i8>"*
  %2 = bitcast %String* %message to %"Array<i8>"*
  call void @"Array<i8>.="(%"Array<i8>"* %1, %"Array<i8>"* %2)
  %"$copy" = load %String, %String* %copy, align 8
  %3 = bitcast %String* %e.message to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %3)
  store %String %"$copy", %String* %e.message, align 8
  %"$e" = load %Error, %Error* %e, align 8
  %4 = bitcast %String* %message to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %4)
  ret %Error %"$e"
}

define i32 @uncaught(%Error* %0) {
entry:
  %error = alloca %Error*, align 8
  %copy = alloca %String, align 8
  store %Error* %0, %Error** %error, align 8
  store %String zeroinitializer, %String* %copy, align 8
  %"*error" = load %Error*, %Error** %error, align 8
  %"*error.message" = getelementptr inbounds %Error, %Error* %"*error", i32 0, i32 0
  %1 = bitcast %String* %copy to %"Array<i8>"*
  %2 = bitcast %String* %"*error.message" to %"Array<i8>"*
  call void @"Array<i8>.="(%"Array<i8>"* %1, %"Array<i8>"* %2)
  %"$copy" = load %String, %String* %copy, align 8
  call void @"io::err"(%String %"$copy")
  ret i32 1
}

declare i64 @write(i32, i8*, i64)

declare i64 @read(i32, i8*, i64)

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

define void @"io::err"(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.String.len = call i32 @String.len(%String* %text)
  %AS = sext i32 %call.String.len to i64
  %call.write = call i64 @write(i32 2, i8* %call.String.chars, i64 %AS)
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define %String @"io::input"() {
entry:
  %line = alloca %String, align 8
  %c = alloca i8*, align 8
  store %String zeroinitializer, %String* %line, align 8
  %"call.new<i8>" = call i8* @"new<i8>"(i32 1)
  store i8* %"call.new<i8>", i8** %c, align 8
  br label %while18

while18:                                          ; preds = %while18.then, %entry
  %"$c" = load i8*, i8** %c, align 8
  %call.read = call i64 @read(i32 0, i8* %"$c", i64 1)
  %EQ = icmp eq i64 %call.read, 1
  %"$c1" = load i8*, i8** %c, align 8
  %"c[]" = getelementptr inbounds i8, i8* %"$c1", i32 0
  %"$c[]" = load i8, i8* %"c[]", align 1
  %NQ = icmp ne i8 %"$c[]", 10
  %AND = and i1 %EQ, %NQ
  br i1 %AND, label %while18.then, label %endloop

while18.then:                                     ; preds = %while18
  %"$c2" = load i8*, i8** %c, align 8
  %"c[]3" = getelementptr inbounds i8, i8* %"$c2", i32 0
  %"$c[]4" = load i8, i8* %"c[]3", align 1
  call void @String.append(%String* %line, i8 %"$c[]4")
  br label %while18

endloop:                                          ; preds = %while18
  %"$c5" = load i8*, i8** %c, align 8
  call void @"free<i8>"(i8* %"$c5")
  %"$line" = load %String, %String* %line, align 8
  ret %String %"$line"
}

define i1 @is_digit(i8 %0) {
entry:
  %c = alloca i8, align 1
  store i8 %0, i8* %c, align 1
  %"$c" = load i8, i8* %c, align 1
  %GE = icmp sge i8 %"$c", 48
  %"$c1" = load i8, i8* %c, align 1
  %LE = icmp sle i8 %"$c1", 57
  %AND = and i1 %GE, %LE
  ret i1 %AND
}

define %Token* @"new<Token>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", trunc (i64 mul nuw (i64 ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i64), i64 2) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to %Token*
  ret %Token* %AS1
}

define void @"copy<Token>"(%Token* %0, %Token* %1, i32 %2) {
entry:
  %dst = alloca %Token*, align 8
  %src = alloca %Token*, align 8
  %count = alloca i32, align 4
  store %Token* %0, %Token** %dst, align 8
  store %Token* %1, %Token** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load %Token*, %Token** %dst, align 8
  %AS = bitcast %Token* %"$dst" to i8*
  %"$src" = load %Token*, %Token** %src, align 8
  %AS1 = bitcast %Token* %"$src" to i8*
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", trunc (i64 mul nuw (i64 ptrtoint (i32* getelementptr (i32, i32* null, i32 1) to i64), i64 2) to i32)
  %AS2 = sext i32 %MUL to i64
  %MUL3 = mul i64 %AS2, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %AS, i8* %AS1, i64 %MUL3)
  ret void
}

define void @"free<Token>"(%Token* %0) {
entry:
  %data = alloca %Token*, align 8
  store %Token* %0, %Token** %data, align 8
  %"$data" = load %Token*, %Token** %data, align 8
  %AS = bitcast %Token* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define void @"Array<Token>.reserve"(%"Array<Token>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<Token>"*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca %Token*, align 8
  store %"Array<Token>"* %0, %"Array<Token>"** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if19

if19:                                             ; preds = %entry
  %"*self" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if19.then, label %endif

if19.then:                                        ; preds = %if19
  %"*self3" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if19.then, %if19
  br label %if20

if20:                                             ; preds = %endif
  %"*self6" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if20.then, label %endif1

if20.then:                                        ; preds = %if20
  ret void

endif1:                                           ; preds = %if20
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<Token>" = call %Token* @"new<Token>"(i32 %"$cap8")
  store %Token* %"call.new<Token>", %Token** %bigger, align 8
  %"*self9" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self9", i32 0, i32 0
  %"*self10" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self10", i32 0, i32 1
  %"$bigger" = load %Token*, %Token** %bigger, align 8
  %"$*self.data" = load %Token*, %Token** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<Token>"(%Token* %"$bigger", %Token* %"$*self.data", i32 %"$*self.count12")
  br label %if21

if21:                                             ; preds = %endif1
  %"*self13" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.cap14" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if21.then, label %endif2

if21.then:                                        ; preds = %if21
  %"*self16" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.data17" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self16", i32 0, i32 0
  %"$*self.data18" = load %Token*, %Token** %"*self.data17", align 8
  call void @"free<Token>"(%Token* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if21.then, %if21
  %"*self19" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.data20" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self19", i32 0, i32 0
  %"$bigger21" = load %Token*, %Token** %bigger, align 8
  store %Token* %"$bigger21", %Token** %"*self.data20", align 8
  %"*self22" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.cap23" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @"Array<Token>.own"(%"Array<Token>"* %0) {
entry:
  %self = alloca %"Array<Token>"*, align 8
  store %"Array<Token>"* %0, %"Array<Token>"** %self, align 8
  br label %if22

if22:                                             ; preds = %entry
  %"*self" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  %"*self1" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %AND = and i1 %EQ, %GT
  br i1 %AND, label %if22.then, label %endif

if22.then:                                        ; preds = %if22
  %"*self2" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self3" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  call void @"Array<Token>.reserve"(%"Array<Token>"* %"*self2", i32 %"$*self.count5")
  br label %endif

endif:                                            ; preds = %if22.then, %if22
  ret void
}

define void @"Array<Token>.clear"(%"Array<Token>"* %0) {
entry:
  %self = alloca %"Array<Token>"*, align 8
  store %"Array<Token>"* %0, %"Array<Token>"** %self, align 8
  br label %if23

if23:                                             ; preds = %entry
  %"*self" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if23.then, label %endif

if23.then:                                        ; preds = %if23
  %"*self1" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if23.then, %if23
  br label %while24

while24:                                          ; preds = %while24.then, %endif
  %"*self2" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while24.then, label %endloop

while24.then:                                     ; preds = %while24
  %"*self4" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load %Token*, %Token** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds %Token, %Token* %"$*self.data", i32 %"$*self.count10"
  store %Token zeroinitializer, %Token* %"*self.data[]", align 4
  br label %while24

endloop:                                          ; preds = %while24
  ret void
}

define void @"Array<Token>.append"(%"Array<Token>"* %0, %Token %1) {
entry:
  %self = alloca %"Array<Token>"*, align 8
  %value = alloca %Token, align 8
  store %"Array<Token>"* %0, %"Array<Token>"** %self, align 8
  store %Token %1, %Token* %value, align 4
  br label %if25

if25:                                             ; preds = %entry
  %"*self" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if25.then, label %endif

if25.then:                                        ; preds = %if25
  %"*self2" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self3" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<Token>.reserve"(%"Array<Token>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if25.then, %if25
  %"*self6" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self6", i32 0, i32 0
  %"*self7" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.count8" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self7", i32 0, i32 1
  %"$*self.data" = load %Token*, %Token** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds %Token, %Token* %"$*self.data", i32 %"$*self.count9"
  %"$value" = load %Token, %Token* %value, align 4
  store %Token %"$value", %Token* %"*self.data[]", align 4
  %"*self10" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  ret void
}

define i32 @"Array<Token>.len"(%"Array<Token>"* %0) {
entry:
  %self = alloca %"Array<Token>"*, align 8
  store %"Array<Token>"* %0, %"Array<Token>"** %self, align 8
  %"*self" = load %"Array<Token>"*, %"Array<Token>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define %Token* @"Array<Token>.[]"(%"Array<Token>"* %0, i32 %1) {
entry:
  %left = alloca %"Array<Token>"*, align 8
  %index = alloca i32, align 4
  store %"Array<Token>"* %0, %"Array<Token>"** %left, align 8
  store i32 %1, i32* %index, align 4
  %"*left" = load %"Array<Token>"*, %"Array<Token>"** %left, align 8
  call void @"Array<Token>.own"(%"Array<Token>"* %"*left")
  %"*left1" = load %"Array<Token>"*, %"Array<Token>"** %left, align 8
  %"*left.data" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*left1", i32 0, i32 0
  %"$*left.data" = load %Token*, %Token** %"*left.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*left.data[]" = getelementptr inbounds %Token, %Token* %"$*left.data", i32 %"$index"
  ret %Token* %"*left.data[]"
}

define void @"Array<Token>.drop"(%"Array<Token>"* %0) {
entry:
  %value = alloca %"Array<Token>"*, align 8
  store %"Array<Token>"* %0, %"Array<Token>"** %value, align 8
  %"*value" = load %"Array<Token>"*, %"Array<Token>"** %value, align 8
  call void @"Array<Token>.clear"(%"Array<Token>"* %"*value")
  br label %if26

if26:                                             ; preds = %entry
  %"*value1" = load %"Array<Token>"*, %"Array<Token>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if26.then, label %endif

if26.then:                                        ; preds = %if26
  %"*value2" = load %"Array<Token>"*, %"Array<Token>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<Token>", %"Array<Token>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load %Token*, %Token** %"*value.data", align 8
  call void @"free<Token>"(%Token* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if26.then, %if26
  ret void
}

define %"Array<Token>" @lex(%String %0) {
entry:
  %src = alloca %String, align 8
  %toks = alloca %"Array<Token>", align 8
  %i = alloca i32, align 4
  %c = alloca i8, align 1
  %ref = alloca i8*, align 8
  %t = alloca %Token, align 8
  %ref12 = alloca i8*, align 8
  %ref19 = alloca i8*, align 8
  %"match@1413" = alloca i8, align 1
  store %String %0, %String* %src, align 8
  store %"Array<Token>" zeroinitializer, %"Array<Token>"* %toks, align 8
  store i32 0, i32* %i, align 4
  br label %while27

while27:                                          ; preds = %endif2, %endloop, %if28.then, %entry
  %call.String.len = call i32 @String.len(%String* %src)
  %"$i" = load i32, i32* %i, align 4
  %LT = icmp slt i32 %"$i", %call.String.len
  br i1 %LT, label %while27.then, label %endloop3

while27.then:                                     ; preds = %while27
  %"$i4" = load i32, i32* %i, align 4
  %1 = bitcast %String* %src to %"Array<i8>"*
  %"call.Array<i8>.[]" = call i8* @"Array<i8>.[]"(%"Array<i8>"* %1, i32 %"$i4")
  store i8* %"call.Array<i8>.[]", i8** %ref, align 8
  %"*ref" = load i8*, i8** %ref, align 8
  %"$*ref" = load i8, i8* %"*ref", align 1
  store i8 %"$*ref", i8* %c, align 1
  store %Token zeroinitializer, %Token* %t, align 4
  br label %if28

if28:                                             ; preds = %while27.then
  %"$c" = load i8, i8* %c, align 1
  %EQ = icmp eq i8 %"$c", 32
  br i1 %EQ, label %if28.then, label %endif

if28.then:                                        ; preds = %if28
  %"$i5" = load i32, i32* %i, align 4
  %ADD = add i32 %"$i5", 1
  store i32 %ADD, i32* %i, align 4
  br label %while27

endif:                                            ; preds = %if28
  br label %if29

if29:                                             ; preds = %endif
  %"$c6" = load i8, i8* %c, align 1
  %call.is_digit = call i1 @is_digit(i8 %"$c6")
  br i1 %call.is_digit, label %if29.then, label %endif1

if29.then:                                        ; preds = %if29
  %t.kind = getelementptr inbounds %Token, %Token* %t, i32 0, i32 0
  store i32 1, i32* %t.kind, align 4
  br label %while30

while30:                                          ; preds = %while30.then, %if29.then
  %call.String.len7 = call i32 @String.len(%String* %src)
  %"$i8" = load i32, i32* %i, align 4
  %LT9 = icmp slt i32 %"$i8", %call.String.len7
  %"$i10" = load i32, i32* %i, align 4
  %2 = bitcast %String* %src to %"Array<i8>"*
  %"call.Array<i8>.[]11" = call i8* @"Array<i8>.[]"(%"Array<i8>"* %2, i32 %"$i10")
  store i8* %"call.Array<i8>.[]11", i8** %ref12, align 8
  %"*ref13" = load i8*, i8** %ref12, align 8
  %"$*ref14" = load i8, i8* %"*ref13", align 1
  %call.is_digit15 = call i1 @is_digit(i8 %"$*ref14")
  %AND = and i1 %LT9, %call.is_digit15
  br i1 %AND, label %while30.then, label %endloop

while30.then:                                     ; preds = %while30
  %t.value = getelementptr inbounds %Token, %Token* %t, i32 0, i32 1
  %t.value16 = getelementptr inbounds %Token, %Token* %t, i32 0, i32 1
  %"$t.value" = load i32, i32* %t.value16, align 4
  %MUL = mul i32 %"$t.value", 10
  %"$i17" = load i32, i32* %i, align 4
  %3 = bitcast %String* %src to %"Array<i8>"*
  %"call.Array<i8>.[]18" = call i8* @"Array<i8>.[]"(%"Array<i8>"* %3, i32 %"$i17")
  store i8* %"call.Array<i8>.[]18", i8** %ref19, align 8
  %"*ref20" = load i8*, i8** %ref19, align 8
  %"$*ref21" = load i8, i8* %"*ref20", align 1
  %AS = sext i8 %"$*ref21" to i32
  %SUB = sub i32 %AS, 48
  %ADD22 = add i32 %MUL, %SUB
  store i32 %ADD22, i32* %t.value, align 4
  %"$i23" = load i32, i32* %i, align 4
  %ADD24 = add i32 %"$i23", 1
  store i32 %ADD24, i32* %i, align 4
  br label %while30

endloop:                                          ; preds = %while30
  %"$t" = load %Token, %Token* %t, align 4
  call void @"Array<Token>.append"(%"Array<Token>"* %toks, %Token %"$t")
  br label %while27

endif1:                                           ; preds = %if29
  %"$c25" = load i8, i8* %c, align 1
  store i8 %"$c25", i8* %"match@1413", align 1
  br label %if31

if31:                                             ; preds = %endif1
  %"$match@1413" = load i8, i8* %"match@1413", align 1
  %EQ26 = icmp eq i8 %"$match@1413", 43
  br i1 %EQ26, label %if31.then, label %elif32

if31.then:                                        ; preds = %if31
  %t.kind27 = getelementptr inbounds %Token, %Token* %t, i32 0, i32 0
  store i32 2, i32* %t.kind27, align 4
  br label %endif2

elif32:                                           ; preds = %if31
  %"$match@141328" = load i8, i8* %"match@1413", align 1
  %EQ29 = icmp eq i8 %"$match@141328", 45
  br i1 %EQ29, label %elif32.then, label %elif33

elif32.then:                                      ; preds = %elif32
  %t.kind30 = getelementptr inbounds %Token, %Token* %t, i32 0, i32 0
  store i32 3, i32* %t.kind30, align 4
  br label %endif2

elif33:                                           ; preds = %elif32
  %"$match@141331" = load i8, i8* %"match@1413", align 1
  %EQ32 = icmp eq i8 %"$match@141331", 42
  br i1 %EQ32, label %elif33.then, label %elif34

elif33.then:                                      ; preds = %elif33
  %t.kind33 = getelementptr inbounds %Token, %Token* %t, i32 0, i32 0
  store i32 4, i32* %t.kind33, align 4
  br label %endif2

elif34:                                           ; preds = %elif33
  %"$match@141334" = load i8, i8* %"match@1413", align 1
  %EQ35 = icmp eq i8 %"$match@141334", 47
  br i1 %EQ35, label %elif34.then, label %elif35

elif34.then:                                      ; preds = %elif34
  %t.kind36 = getelementptr inbounds %Token, %Token* %t, i32 0, i32 0
  store i32 5, i32* %t.kind36, align 4
  br label %endif2

elif35:                                           ; preds = %elif34
  %"$match@141337" = load i8, i8* %"match@1413", align 1
  %EQ38 = icmp eq i8 %"$match@141337", 40
  br i1 %EQ38, label %elif35.then, label %elif36

elif35.then:                                      ; preds = %elif35
  %t.kind39 = getelementptr inbounds %Token, %Token* %t, i32 0, i32 0
  store i32 6, i32* %t.kind39, align 4
  br label %endif2

elif36:                                           ; preds = %elif35
  %"$match@141340" = load i8, i8* %"match@1413", align 1
  %EQ41 = icmp eq i8 %"$match@141340", 41
  br i1 %EQ41, label %elif36.then, label %endif2

elif36.then:                                      ; preds = %elif36
  %t.kind42 = getelementptr inbounds %Token, %Token* %t, i32 0, i32 0
  store i32 7, i32* %t.kind42, align 4
  br label %endif2

endif2:                                           ; preds = %elif36.then, %elif36, %elif35.then, %elif34.then, %elif33.then, %elif32.then, %if31.then
  %"$t43" = load %Token, %Token* %t, align 4
  call void @"Array<Token>.append"(%"Array<Token>"* %toks, %Token %"$t43")
  %"$i44" = load i32, i32* %i, align 4
  %ADD45 = add i32 %"$i44", 1
  store i32 %ADD45, i32* %i, align 4
  br label %while27

endloop3:                                         ; preds = %while27
  %"$toks" = load %"Array<Token>", %"Array<Token>"* %toks, align 8
  %4 = bitcast %String* %src to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %4)
  ret %"Array<Token>" %"$toks"
}

define %String @sign(i32 %0) {
entry:
  %kind = alloca i32, align 4
  %"match@1760" = alloca i32, align 4
  store i32 %0, i32* %kind, align 4
  %"$kind" = load i32, i32* %kind, align 4
  store i32 %"$kind", i32* %"match@1760", align 4
  br label %if37

if37:                                             ; preds = %entry
  %"$match@1760" = load i32, i32* %"match@1760", align 4
  %EQ = icmp eq i32 %"$match@1760", 2
  br i1 %EQ, label %if37.then, label %elif38

if37.then:                                        ; preds = %if37
  ret %String { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 1, i32 0 }

elif38:                                           ; preds = %if37
  %"$match@17601" = load i32, i32* %"match@1760", align 4
  %EQ2 = icmp eq i32 %"$match@17601", 3
  br i1 %EQ2, label %elif38.then, label %elif39

elif38.then:                                      ; preds = %elif38
  ret %String { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i32 1, i32 0 }

elif39:                                           ; preds = %elif38
  %"$match@17603" = load i32, i32* %"match@1760", align 4
  %EQ4 = icmp eq i32 %"$match@17603", 4
  br i1 %EQ4, label %elif39.then, label %elif40

elif39.then:                                      ; preds = %elif39
  ret %String { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0), i32 1, i32 0 }

elif40:                                           ; preds = %elif39
  %"$match@17605" = load i32, i32* %"match@1760", align 4
  %EQ6 = icmp eq i32 %"$match@17605", 5
  br i1 %EQ6, label %elif40.then, label %endif

elif40.then:                                      ; preds = %elif40
  ret %String { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0), i32 1, i32 0 }

endif:                                            ; preds = %elif40
  ret %String { i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0), i32 1, i32 0 }
}

define %Node* @"new<Node>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (%Node* getelementptr (%Node, %Node* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to %Node*
  ret %Node* %AS1
}

define void @"copy<Node>"(%Node* %0, %Node* %1, i32 %2) {
entry:
  %dst = alloca %Node*, align 8
  %src = alloca %Node*, align 8
  %count = alloca i32, align 4
  store %Node* %0, %Node** %dst, align 8
  store %Node* %1, %Node** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load %Node*, %Node** %dst, align 8
  %AS = bitcast %Node* %"$dst" to i8*
  %"$src" = load %Node*, %Node** %src, align 8
  %AS1 = bitcast %Node* %"$src" to i8*
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (%Node* getelementptr (%Node, %Node* null, i32 1) to i32)
  %AS2 = sext i32 %MUL to i64
  %MUL3 = mul i64 %AS2, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %AS, i8* %AS1, i64 %MUL3)
  ret void
}

define void @"free<Node>"(%Node* %0) {
entry:
  %data = alloca %Node*, align 8
  store %Node* %0, %Node** %data, align 8
  %"$data" = load %Node*, %Node** %data, align 8
  %AS = bitcast %Node* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define void @"Array<Node>.reserve"(%"Array<Node>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<Node>"*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca %Node*, align 8
  store %"Array<Node>"* %0, %"Array<Node>"** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if41

if41:                                             ; preds = %entry
  %"*self" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if41.then, label %endif

if41.then:                                        ; preds = %if41
  %"*self3" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if41.then, %if41
  br label %if42

if42:                                             ; preds = %endif
  %"*self6" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if42.then, label %endif1

if42.then:                                        ; preds = %if42
  ret void

endif1:                                           ; preds = %if42
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<Node>" = call %Node* @"new<Node>"(i32 %"$cap8")
  store %Node* %"call.new<Node>", %Node** %bigger, align 8
  %"*self9" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self9", i32 0, i32 0
  %"*self10" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self10", i32 0, i32 1
  %"$bigger" = load %Node*, %Node** %bigger, align 8
  %"$*self.data" = load %Node*, %Node** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<Node>"(%Node* %"$bigger", %Node* %"$*self.data", i32 %"$*self.count12")
  br label %if43

if43:                                             ; preds = %endif1
  %"*self13" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.cap14" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if43.then, label %endif2

if43.then:                                        ; preds = %if43
  %"*self16" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.data17" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self16", i32 0, i32 0
  %"$*self.data18" = load %Node*, %Node** %"*self.data17", align 8
  call void @"free<Node>"(%Node* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if43.then, %if43
  %"*self19" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.data20" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self19", i32 0, i32 0
  %"$bigger21" = load %Node*, %Node** %bigger, align 8
  store %Node* %"$bigger21", %Node** %"*self.data20", align 8
  %"*self22" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.cap23" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @"Array<Node>.own"(%"Array<Node>"* %0) {
entry:
  %self = alloca %"Array<Node>"*, align 8
  store %"Array<Node>"* %0, %"Array<Node>"** %self, align 8
  br label %if44

if44:                                             ; preds = %entry
  %"*self" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  %"*self1" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self1", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  %AND = and i1 %EQ, %GT
  br i1 %AND, label %if44.then, label %endif

if44.then:                                        ; preds = %if44
  %"*self2" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self3" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  call void @"Array<Node>.reserve"(%"Array<Node>"* %"*self2", i32 %"$*self.count5")
  br label %endif

endif:                                            ; preds = %if44.then, %if44
  ret void
}

define void @"Array<Node>.clear"(%"Array<Node>"* %0) {
entry:
  %self = alloca %"Array<Node>"*, align 8
  store %"Array<Node>"* %0, %"Array<Node>"** %self, align 8
  br label %if45

if45:                                             ; preds = %entry
  %"*self" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if45.then, label %endif

if45.then:                                        ; preds = %if45
  %"*self1" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if45.then, %if45
  br label %while46

while46:                                          ; preds = %while46.then, %endif
  %"*self2" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while46.then, label %endloop

while46.then:                                     ; preds = %while46
  %"*self4" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load %Node*, %Node** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds %Node, %Node* %"$*self.data", i32 %"$*self.count10"
  store %Node zeroinitializer, %Node* %"*self.data[]", align 4
  br label %while46

endloop:                                          ; preds = %while46
  ret void
}

define void @"Array<Node>.append"(%"Array<Node>"* %0, %Node %1) {
entry:
  %self = alloca %"Array<Node>"*, align 8
  %value = alloca %Node, align 8
  store %"Array<Node>"* %0, %"Array<Node>"** %self, align 8
  store %Node %1, %Node* %value, align 4
  br label %if47

if47:                                             ; preds = %entry
  %"*self" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if47.then, label %endif

if47.then:                                        ; preds = %if47
  %"*self2" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self3" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<Node>.reserve"(%"Array<Node>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if47.then, %if47
  %"*self6" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self6", i32 0, i32 0
  %"*self7" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.count8" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self7", i32 0, i32 1
  %"$*self.data" = load %Node*, %Node** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds %Node, %Node* %"$*self.data", i32 %"$*self.count9"
  %"$value" = load %Node, %Node* %value, align 4
  store %Node %"$value", %Node* %"*self.data[]", align 4
  %"*self10" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  ret void
}

define i32 @"Array<Node>.len"(%"Array<Node>"* %0) {
entry:
  %self = alloca %"Array<Node>"*, align 8
  store %"Array<Node>"* %0, %"Array<Node>"** %self, align 8
  %"*self" = load %"Array<Node>"*, %"Array<Node>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define %Node* @"Array<Node>.[]"(%"Array<Node>"* %0, i32 %1) {
entry:
  %left = alloca %"Array<Node>"*, align 8
  %index = alloca i32, align 4
  store %"Array<Node>"* %0, %"Array<Node>"** %left, align 8
  store i32 %1, i32* %index, align 4
  %"*left" = load %"Array<Node>"*, %"Array<Node>"** %left, align 8
  call void @"Array<Node>.own"(%"Array<Node>"* %"*left")
  %"*left1" = load %"Array<Node>"*, %"Array<Node>"** %left, align 8
  %"*left.data" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*left1", i32 0, i32 0
  %"$*left.data" = load %Node*, %Node** %"*left.data", align 8
  %"$index" = load i32, i32* %index, align 4
  %"*left.data[]" = getelementptr inbounds %Node, %Node* %"$*left.data", i32 %"$index"
  ret %Node* %"*left.data[]"
}

define void @"Array<Node>.drop"(%"Array<Node>"* %0) {
entry:
  %value = alloca %"Array<Node>"*, align 8
  store %"Array<Node>"* %0, %"Array<Node>"** %value, align 8
  %"*value" = load %"Array<Node>"*, %"Array<Node>"** %value, align 8
  call void @"Array<Node>.clear"(%"Array<Node>"* %"*value")
  br label %if48

if48:                                             ; preds = %entry
  %"*value1" = load %"Array<Node>"*, %"Array<Node>"** %value, align 8
  %"*value.cap" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*value1", i32 0, i32 2
  %"$*value.cap" = load i32, i32* %"*value.cap", align 4
  %GT = icmp sgt i32 %"$*value.cap", 0
  br i1 %GT, label %if48.then, label %endif

if48.then:                                        ; preds = %if48
  %"*value2" = load %"Array<Node>"*, %"Array<Node>"** %value, align 8
  %"*value.data" = getelementptr inbounds %"Array<Node>", %"Array<Node>"* %"*value2", i32 0, i32 0
  %"$*value.data" = load %Node*, %Node** %"*value.data", align 8
  call void @"free<Node>"(%Node* %"$*value.data")
  br label %endif

endif:                                            ; preds = %if48.then, %if48
  ret void
}

define i32 @Parser.peek(%Parser* %0) {
entry:
  %self = alloca %Parser*, align 8
  %ref = alloca %Token*, align 8
  store %Parser* %0, %Parser** %self, align 8
  br label %if49

if49:                                             ; preds = %entry
  %"*self" = load %Parser*, %Parser** %self, align 8
  %"*self.pos" = getelementptr inbounds %Parser, %Parser* %"*self", i32 0, i32 1
  %"*self1" = load %Parser*, %Parser** %self, align 8
  %"*self.toks" = getelementptr inbounds %Parser, %Parser* %"*self1", i32 0, i32 0
  %"call.Array<Token>.len" = call i32 @"Array<Token>.len"(%"Array<Token>"* %"*self.toks")
  %"$*self.pos" = load i32, i32* %"*self.pos", align 4
  %LT = icmp slt i32 %"$*self.pos", %"call.Array<Token>.len"
  br i1 %LT, label %if49.then, label %endif

if49.then:                                        ; preds = %if49
  %"*self2" = load %Parser*, %Parser** %self, align 8
  %"*self.toks3" = getelementptr inbounds %Parser, %Parser* %"*self2", i32 0, i32 0
  %"*self4" = load %Parser*, %Parser** %self, align 8
  %"*self.pos5" = getelementptr inbounds %Parser, %Parser* %"*self4", i32 0, i32 1
  %"$*self.pos6" = load i32, i32* %"*self.pos5", align 4
  %"call.Array<Token>.[]" = call %Token* @"Array<Token>.[]"(%"Array<Token>"* %"*self.toks3", i32 %"$*self.pos6")
  store %Token* %"call.Array<Token>.[]", %Token** %ref, align 8
  %"*ref" = load %Token*, %Token** %ref, align 8
  %"*ref.kind" = getelementptr inbounds %Token, %Token* %"*ref", i32 0, i32 0
  %"$*ref.kind" = load i32, i32* %"*ref.kind", align 4
  ret i32 %"$*ref.kind"

endif:                                            ; preds = %if49
  ret i32 0
}

define i32 @Parser.add(%Parser* %0, %Token %1, i32 %2, i32 %3) {
entry:
  %self = alloca %Parser*, align 8
  %token = alloca %Token, align 8
  %left = alloca i32, align 4
  %right = alloca i32, align 4
  %n = alloca %Node, align 8
  store %Parser* %0, %Parser** %self, align 8
  store %Token %1, %Token* %token, align 4
  store i32 %2, i32* %left, align 4
  store i32 %3, i32* %right, align 4
  store %Node zeroinitializer, %Node* %n, align 4
  %n.token = getelementptr inbounds %Node, %Node* %n, i32 0, i32 0
  %"$token" = load %Token, %Token* %token, align 4
  store %Token %"$token", %Token* %n.token, align 4
  %n.left = getelementptr inbounds %Node, %Node* %n, i32 0, i32 1
  %"$left" = load i32, i32* %left, align 4
  store i32 %"$left", i32* %n.left, align 4
  %n.right = getelementptr inbounds %Node, %Node* %n, i32 0, i32 2
  %"$right" = load i32, i32* %right, align 4
  store i32 %"$right", i32* %n.right, align 4
  %"*self" = load %Parser*, %Parser** %self, align 8
  %"*self.nodes" = getelementptr inbounds %Parser, %Parser* %"*self", i32 0, i32 2
  %"$n" = load %Node, %Node* %n, align 4
  call void @"Array<Node>.append"(%"Array<Node>"* %"*self.nodes", %Node %"$n")
  %"*self1" = load %Parser*, %Parser** %self, align 8
  %"*self.nodes2" = getelementptr inbounds %Parser, %Parser* %"*self1", i32 0, i32 2
  %"call.Array<Node>.len" = call i32 @"Array<Node>.len"(%"Array<Node>"* %"*self.nodes2")
  %SUB = sub i32 %"call.Array<Node>.len", 1
  ret i32 %SUB
}

define i32 @Parser.factor(%Parser* %0) {
entry:
  %self = alloca %Parser*, align 8
  %t = alloca %Token, align 8
  %ref = alloca %Token*, align 8
  %inner = alloca i32, align 4
  store %Parser* %0, %Parser** %self, align 8
  %"*self" = load %Parser*, %Parser** %self, align 8
  %"*self.toks" = getelementptr inbounds %Parser, %Parser* %"*self", i32 0, i32 0
  %"*self1" = load %Parser*, %Parser** %self, align 8
  %"*self.pos" = getelementptr inbounds %Parser, %Parser* %"*self1", i32 0, i32 1
  %"$*self.pos" = load i32, i32* %"*self.pos", align 4
  %"call.Array<Token>.[]" = call %Token* @"Array<Token>.[]"(%"Array<Token>"* %"*self.toks", i32 %"$*self.pos")
  store %Token* %"call.Array<Token>.[]", %Token** %ref, align 8
  %"*ref" = load %Token*, %Token** %ref, align 8
  %"$*ref" = load %Token, %Token* %"*ref", align 4
  store %Token %"$*ref", %Token* %t, align 4
  %"*self2" = load %Parser*, %Parser** %self, align 8
  %"*self.pos3" = getelementptr inbounds %Parser, %Parser* %"*self2", i32 0, i32 1
  %"$*self.pos4" = load i32, i32* %"*self.pos3", align 4
  %ADD = add i32 %"$*self.pos4", 1
  store i32 %ADD, i32* %"*self.pos3", align 4
  br label %if50

if50:                                             ; preds = %entry
  %t.kind = getelementptr inbounds %Token, %Token* %t, i32 0, i32 0
  %"$t.kind" = load i32, i32* %t.kind, align 4
  %EQ = icmp eq i32 %"$t.kind", 6
  br i1 %EQ, label %if50.then, label %endif

if50.then:                                        ; preds = %if50
  %"*self5" = load %Parser*, %Parser** %self, align 8
  %call.Parser.expr = call i32 @Parser.expr(%Parser* %"*self5")
  store i32 %call.Parser.expr, i32* %inner, align 4
  %"*self6" = load %Parser*, %Parser** %self, align 8
  %"*self.pos7" = getelementptr inbounds %Parser, %Parser* %"*self6", i32 0, i32 1
  %"$*self.pos8" = load i32, i32* %"*self.pos7", align 4
  %ADD9 = add i32 %"$*self.pos8", 1
  store i32 %ADD9, i32* %"*self.pos7", align 4
  %"$inner" = load i32, i32* %inner, align 4
  ret i32 %"$inner"

endif:                                            ; preds = %if50
  %"*self10" = load %Parser*, %Parser** %self, align 8
  %"$t" = load %Token, %Token* %t, align 4
  %call.Parser.add = call i32 @Parser.add(%Parser* %"*self10", %Token %"$t", i32 -1, i32 -1)
  ret i32 %call.Parser.add
}

define i32 @Parser.term(%Parser* %0) {
entry:
  %self = alloca %Parser*, align 8
  %left = alloca i32, align 4
  %t = alloca %Token, align 8
  %ref = alloca %Token*, align 8
  %right = alloca i32, align 4
  store %Parser* %0, %Parser** %self, align 8
  %"*self" = load %Parser*, %Parser** %self, align 8
  %call.Parser.factor = call i32 @Parser.factor(%Parser* %"*self")
  store i32 %call.Parser.factor, i32* %left, align 4
  br label %while51

while51:                                          ; preds = %while51.then, %entry
  %"*self1" = load %Parser*, %Parser** %self, align 8
  %call.Parser.peek = call i32 @Parser.peek(%Parser* %"*self1")
  %EQ = icmp eq i32 %call.Parser.peek, 4
  %"*self2" = load %Parser*, %Parser** %self, align 8
  %call.Parser.peek3 = call i32 @Parser.peek(%Parser* %"*self2")
  %EQ4 = icmp eq i32 %call.Parser.peek3, 5
  %OR = or i1 %EQ, %EQ4
  br i1 %OR, label %while51.then, label %endloop

while51.then:                                     ; preds = %while51
  %"*self5" = load %Parser*, %Parser** %self, align 8
  %"*self.toks" = getelementptr inbounds %Parser, %Parser* %"*self5", i32 0, i32 0
  %"*self6" = load %Parser*, %Parser** %self, align 8
  %"*self.pos" = getelementptr inbounds %Parser, %Parser* %"*self6", i32 0, i32 1
  %"$*self.pos" = load i32, i32* %"*self.pos", align 4
  %"call.Array<Token>.[]" = call %Token* @"Array<Token>.[]"(%"Array<Token>"* %"*self.toks", i32 %"$*self.pos")
  store %Token* %"call.Array<Token>.[]", %Token** %ref, align 8
  %"*ref" = load %Token*, %Token** %ref, align 8
  %"$*ref" = load %Token, %Token* %"*ref", align 4
  store %Token %"$*ref", %Token* %t, align 4
  %"*self7" = load %Parser*, %Parser** %self, align 8
  %"*self.pos8" = getelementptr inbounds %Parser, %Parser* %"*self7", i32 0, i32 1
  %"$*self.pos9" = load i32, i32* %"*self.pos8", align 4
  %ADD = add i32 %"$*self.pos9", 1
  store i32 %ADD, i32* %"*self.pos8", align 4
  %"*self10" = load %Parser*, %Parser** %self, align 8
  %call.Parser.factor11 = call i32 @Parser.factor(%Parser* %"*self10")
  store i32 %call.Parser.factor11, i32* %right, align 4
  %"*self12" = load %Parser*, %Parser** %self, align 8
  %"$t" = load %Token, %Token* %t, align 4
  %"$left" = load i32, i32* %left, align 4
  %"$right" = load i32, i32* %right, align 4
  %call.Parser.add = call i32 @Parser.add(%Parser* %"*self12", %Token %"$t", i32 %"$left", i32 %"$right")
  store i32 %call.Parser.add, i32* %left, align 4
  br label %while51

endloop:                                          ; preds = %while51
  %"$left13" = load i32, i32* %left, align 4
  ret i32 %"$left13"
}

define i32 @Parser.expr(%Parser* %0) {
entry:
  %self = alloca %Parser*, align 8
  %left = alloca i32, align 4
  %t = alloca %Token, align 8
  %ref = alloca %Token*, align 8
  %right = alloca i32, align 4
  store %Parser* %0, %Parser** %self, align 8
  %"*self" = load %Parser*, %Parser** %self, align 8
  %call.Parser.term = call i32 @Parser.term(%Parser* %"*self")
  store i32 %call.Parser.term, i32* %left, align 4
  br label %while52

while52:                                          ; preds = %while52.then, %entry
  %"*self1" = load %Parser*, %Parser** %self, align 8
  %call.Parser.peek = call i32 @Parser.peek(%Parser* %"*self1")
  %EQ = icmp eq i32 %call.Parser.peek, 2
  %"*self2" = load %Parser*, %Parser** %self, align 8
  %call.Parser.peek3 = call i32 @Parser.peek(%Parser* %"*self2")
  %EQ4 = icmp eq i32 %call.Parser.peek3, 3
  %OR = or i1 %EQ, %EQ4
  br i1 %OR, label %while52.then, label %endloop

while52.then:                                     ; preds = %while52
  %"*self5" = load %Parser*, %Parser** %self, align 8
  %"*self.toks" = getelementptr inbounds %Parser, %Parser* %"*self5", i32 0, i32 0
  %"*self6" = load %Parser*, %Parser** %self, align 8
  %"*self.pos" = getelementptr inbounds %Parser, %Parser* %"*self6", i32 0, i32 1
  %"$*self.pos" = load i32, i32* %"*self.pos", align 4
  %"call.Array<Token>.[]" = call %Token* @"Array<Token>.[]"(%"Array<Token>"* %"*self.toks", i32 %"$*self.pos")
  store %Token* %"call.Array<Token>.[]", %Token** %ref, align 8
  %"*ref" = load %Token*, %Token** %ref, align 8
  %"$*ref" = load %Token, %Token* %"*ref", align 4
  store %Token %"$*ref", %Token* %t, align 4
  %"*self7" = load %Parser*, %Parser** %self, align 8
  %"*self.pos8" = getelementptr inbounds %Parser, %Parser* %"*self7", i32 0, i32 1
  %"$*self.pos9" = load i32, i32* %"*self.pos8", align 4
  %ADD = add i32 %"$*self.pos9", 1
  store i32 %ADD, i32* %"*self.pos8", align 4
  %"*self10" = load %Parser*, %Parser** %self, align 8
  %call.Parser.term11 = call i32 @Parser.term(%Parser* %"*self10")
  store i32 %call.Parser.term11, i32* %right, align 4
  %"*self12" = load %Parser*, %Parser** %self, align 8
  %"$t" = load %Token, %Token* %t, align 4
  %"$left" = load i32, i32* %left, align 4
  %"$right" = load i32, i32* %right, align 4
  %call.Parser.add = call i32 @Parser.add(%Parser* %"*self12", %Token %"$t", i32 %"$left", i32 %"$right")
  store i32 %call.Parser.add, i32* %left, align 4
  br label %while52

endloop:                                          ; preds = %while52
  %"$left13" = load i32, i32* %left, align 4
  ret i32 %"$left13"
}

define %String @Parser.label(%Parser* %0, i32 %1) {
entry:
  %self = alloca %Parser*, align 8
  %i = alloca i32, align 4
  %n = alloca %Node, align 8
  %ref = alloca %Node*, align 8
  %tmp = alloca %String, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store %Parser* %0, %Parser** %self, align 8
  store i32 %1, i32* %i, align 4
  %"*self" = load %Parser*, %Parser** %self, align 8
  %"*self.nodes" = getelementptr inbounds %Parser, %Parser* %"*self", i32 0, i32 2
  %"$i" = load i32, i32* %i, align 4
  %"call.Array<Node>.[]" = call %Node* @"Array<Node>.[]"(%"Array<Node>"* %"*self.nodes", i32 %"$i")
  store %Node* %"call.Array<Node>.[]", %Node** %ref, align 8
  %"*ref" = load %Node*, %Node** %ref, align 8
  %"$*ref" = load %Node, %Node* %"*ref", align 4
  store %Node %"$*ref", %Node* %n, align 4
  br label %if53

if53:                                             ; preds = %entry
  %n.token = getelementptr inbounds %Node, %Node* %n, i32 0, i32 0
  %n.token.kind = getelementptr inbounds %Token, %Token* %n.token, i32 0, i32 0
  %"$n.token.kind" = load i32, i32* %n.token.kind, align 4
  %EQ = icmp eq i32 %"$n.token.kind", 1
  br i1 %EQ, label %if53.then, label %endif

if53.then:                                        ; preds = %if53
  %n.token1 = getelementptr inbounds %Node, %Node* %n, i32 0, i32 0
  %n.token.value = getelementptr inbounds %Token, %Token* %n.token1, i32 0, i32 1
  %"$n.token.value" = load i32, i32* %n.token.value, align 4
  %call.String.create = call %String @String.create(i32 %"$n.token.value")
  ret %String %call.String.create

endif:                                            ; preds = %if53
  %n.token2 = getelementptr inbounds %Node, %Node* %n, i32 0, i32 0
  %n.token.kind3 = getelementptr inbounds %Token, %Token* %n.token2, i32 0, i32 0
  %"$n.token.kind4" = load i32, i32* %n.token.kind3, align 4
  %call.sign = call %String @sign(i32 %"$n.token.kind4")
  store %String %call.sign, %String* %tmp, align 8
  %call.String.chars = call i8* @String.chars(%String* %tmp)
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.5, i32 0, i32 0), i8* %call.String.chars)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf5 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.5, i32 0, i32 0), i8* %call.String.chars)
  %"$format.size6" = load i32, i32* %format.size, align 4
  %ADD7 = add i32 %"$format.size6", 1
  %"$format.text8" = load i8*, i8** %format.text, align 8
  %"$format.size9" = load i32, i32* %format.size, align 4
  %2 = insertvalue %String zeroinitializer, i8* %"$format.text8", 0
  %3 = insertvalue %String %2, i32 %"$format.size9", 1
  %4 = insertvalue %String %3, i32 %ADD7, 2
  %5 = bitcast %String* %tmp to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %5)
  ret %String %4
}

define void @Parser.show(%Parser* %0, i32 %1, %String %2, i1 %3) {
entry:
  %self = alloca %Parser*, align 8
  %i = alloca i32, align 4
  %pre = alloca %String, align 8
  %last = alloca i1, align 1
  %branch = alloca %String, align 8
  %more = alloca %String, align 8
  %tmp = alloca %String, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %n = alloca %Node, align 8
  %ref = alloca %Node*, align 8
  %kid = alloca %String, align 8
  %copy = alloca %String, align 8
  %copy20 = alloca %String, align 8
  store %Parser* %0, %Parser** %self, align 8
  store i32 %1, i32* %i, align 4
  store %String %2, %String* %pre, align 8
  store i1 %3, i1* %last, align 1
  store %String { i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.6, i32 0, i32 0), i32 7, i32 0 }, %String* %branch, align 8
  store %String { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.7, i32 0, i32 0), i32 5, i32 0 }, %String* %more, align 8
  br label %if54

if54:                                             ; preds = %entry
  %"$last" = load i1, i1* %last, align 1
  br i1 %"$last", label %if54.then, label %endif

if54.then:                                        ; preds = %if54
  %4 = bitcast %String* %branch to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %4)
  store %String { i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.8, i32 0, i32 0), i32 7, i32 0 }, %String* %branch, align 8
  %5 = bitcast %String* %more to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %5)
  store %String { i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.9, i32 0, i32 0), i32 3, i32 0 }, %String* %more, align 8
  br label %endif

endif:                                            ; preds = %if54.then, %if54
  %call.String.chars = call i8* @String.chars(%String* %pre)
  %call.String.chars3 = call i8* @String.chars(%String* %branch)
  %"*self" = load %Parser*, %Parser** %self, align 8
  %"$i" = load i32, i32* %i, align 4
  %call.Parser.label = call %String @Parser.label(%Parser* %"*self", i32 %"$i")
  store %String %call.Parser.label, %String* %tmp, align 8
  %call.String.chars4 = call i8* @String.chars(%String* %tmp)
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.10, i32 0, i32 0), i8* %call.String.chars, i8* %call.String.chars3, i8* %call.String.chars4)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf5 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.10, i32 0, i32 0), i8* %call.String.chars, i8* %call.String.chars3, i8* %call.String.chars4)
  %"$format.size6" = load i32, i32* %format.size, align 4
  %ADD7 = add i32 %"$format.size6", 1
  %"$format.text8" = load i8*, i8** %format.text, align 8
  %"$format.size9" = load i32, i32* %format.size, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text8", 0
  %7 = insertvalue %String %6, i32 %"$format.size9", 1
  %8 = insertvalue %String %7, i32 %ADD7, 2
  call void @"io::out"(%String %8)
  %9 = bitcast %String* %tmp to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %9)
  %"*self10" = load %Parser*, %Parser** %self, align 8
  %"*self.nodes" = getelementptr inbounds %Parser, %Parser* %"*self10", i32 0, i32 2
  %"$i11" = load i32, i32* %i, align 4
  %"call.Array<Node>.[]" = call %Node* @"Array<Node>.[]"(%"Array<Node>"* %"*self.nodes", i32 %"$i11")
  store %Node* %"call.Array<Node>.[]", %Node** %ref, align 8
  %"*ref" = load %Node*, %Node** %ref, align 8
  %"$*ref" = load %Node, %Node* %"*ref", align 4
  store %Node %"$*ref", %Node* %n, align 4
  %"call.String.+" = call %String @"String.+"(%String* %pre, %String* %more)
  store %String %"call.String.+", %String* %kid, align 8
  br label %if55

if55:                                             ; preds = %endif
  %n.left = getelementptr inbounds %Node, %Node* %n, i32 0, i32 1
  %"$n.left" = load i32, i32* %n.left, align 4
  %GE = icmp sge i32 %"$n.left", 0
  br i1 %GE, label %if55.then, label %endif1

if55.then:                                        ; preds = %if55
  %"*self12" = load %Parser*, %Parser** %self, align 8
  %n.left13 = getelementptr inbounds %Node, %Node* %n, i32 0, i32 1
  store %String zeroinitializer, %String* %copy, align 8
  %10 = bitcast %String* %copy to %"Array<i8>"*
  %11 = bitcast %String* %kid to %"Array<i8>"*
  call void @"Array<i8>.="(%"Array<i8>"* %10, %"Array<i8>"* %11)
  %n.right = getelementptr inbounds %Node, %Node* %n, i32 0, i32 2
  %"$n.right" = load i32, i32* %n.right, align 4
  %LT = icmp slt i32 %"$n.right", 0
  %"$n.left14" = load i32, i32* %n.left13, align 4
  %"$copy" = load %String, %String* %copy, align 8
  call void @Parser.show(%Parser* %"*self12", i32 %"$n.left14", %String %"$copy", i1 %LT)
  br label %endif1

endif1:                                           ; preds = %if55.then, %if55
  br label %if56

if56:                                             ; preds = %endif1
  %n.right15 = getelementptr inbounds %Node, %Node* %n, i32 0, i32 2
  %"$n.right16" = load i32, i32* %n.right15, align 4
  %GE17 = icmp sge i32 %"$n.right16", 0
  br i1 %GE17, label %if56.then, label %endif2

if56.then:                                        ; preds = %if56
  %"*self18" = load %Parser*, %Parser** %self, align 8
  %n.right19 = getelementptr inbounds %Node, %Node* %n, i32 0, i32 2
  store %String zeroinitializer, %String* %copy20, align 8
  %12 = bitcast %String* %copy20 to %"Array<i8>"*
  %13 = bitcast %String* %kid to %"Array<i8>"*
  call void @"Array<i8>.="(%"Array<i8>"* %12, %"Array<i8>"* %13)
  %"$n.right21" = load i32, i32* %n.right19, align 4
  %"$copy22" = load %String, %String* %copy20, align 8
  call void @Parser.show(%Parser* %"*self18", i32 %"$n.right21", %String %"$copy22", i1 true)
  br label %endif2

endif2:                                           ; preds = %if56.then, %if56
  %14 = bitcast %String* %kid to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %14)
  %15 = bitcast %String* %more to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %15)
  %16 = bitcast %String* %branch to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %16)
  %17 = bitcast %String* %pre to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %17)
  ret void
}

define i32 @Parser.eval(%Parser* %0, i32 %1) {
entry:
  %self = alloca %Parser*, align 8
  %i = alloca i32, align 4
  %n = alloca %Node, align 8
  %ref = alloca %Node*, align 8
  %"match@3837" = alloca i32, align 4
  %d = alloca i32, align 4
  %error.at = alloca %Error*, align 8
  store %Parser* %0, %Parser** %self, align 8
  store i32 %1, i32* %i, align 4
  %"*self" = load %Parser*, %Parser** %self, align 8
  %"*self.nodes" = getelementptr inbounds %Parser, %Parser* %"*self", i32 0, i32 2
  %"$i" = load i32, i32* %i, align 4
  %"call.Array<Node>.[]" = call %Node* @"Array<Node>.[]"(%"Array<Node>"* %"*self.nodes", i32 %"$i")
  store %Node* %"call.Array<Node>.[]", %Node** %ref, align 8
  %"*ref" = load %Node*, %Node** %ref, align 8
  %"$*ref" = load %Node, %Node* %"*ref", align 4
  store %Node %"$*ref", %Node* %n, align 4
  %n.token = getelementptr inbounds %Node, %Node* %n, i32 0, i32 0
  %n.token.kind = getelementptr inbounds %Token, %Token* %n.token, i32 0, i32 0
  %"$n.token.kind" = load i32, i32* %n.token.kind, align 4
  store i32 %"$n.token.kind", i32* %"match@3837", align 4
  br label %if57

if57:                                             ; preds = %entry
  %"$match@3837" = load i32, i32* %"match@3837", align 4
  %EQ = icmp eq i32 %"$match@3837", 1
  br i1 %EQ, label %if57.then, label %elif58

if57.then:                                        ; preds = %if57
  %n.token9 = getelementptr inbounds %Node, %Node* %n, i32 0, i32 0
  %n.token.value = getelementptr inbounds %Token, %Token* %n.token9, i32 0, i32 1
  %"$n.token.value" = load i32, i32* %n.token.value, align 4
  ret i32 %"$n.token.value"

elif58:                                           ; preds = %if57
  %"$match@383710" = load i32, i32* %"match@3837", align 4
  %EQ11 = icmp eq i32 %"$match@383710", 2
  br i1 %EQ11, label %elif58.then, label %elif61

elif58.then:                                      ; preds = %elif58
  %"*self12" = load %Parser*, %Parser** %self, align 8
  %n.left = getelementptr inbounds %Node, %Node* %n, i32 0, i32 1
  %"$n.left" = load i32, i32* %n.left, align 4
  %call.Parser.eval = call i32 @Parser.eval(%Parser* %"*self12", i32 %"$n.left")
  br label %thrown59

thrown59:                                         ; preds = %elif58.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown59.then, label %endthrown

thrown59.then:                                    ; preds = %thrown59
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown:                                        ; preds = %thrown59
  %"*self13" = load %Parser*, %Parser** %self, align 8
  %n.right = getelementptr inbounds %Node, %Node* %n, i32 0, i32 2
  %"$n.right" = load i32, i32* %n.right, align 4
  %call.Parser.eval14 = call i32 @Parser.eval(%Parser* %"*self13", i32 %"$n.right")
  br label %thrown60

thrown60:                                         ; preds = %endthrown
  %"$ucg.thrown15" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown15", label %thrown60.then, label %endthrown1

thrown60.then:                                    ; preds = %thrown60
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown1:                                       ; preds = %thrown60
  %ADD = add i32 %call.Parser.eval, %call.Parser.eval14
  ret i32 %ADD

elif61:                                           ; preds = %elif58
  %"$match@383716" = load i32, i32* %"match@3837", align 4
  %EQ17 = icmp eq i32 %"$match@383716", 3
  br i1 %EQ17, label %elif61.then, label %elif64

elif61.then:                                      ; preds = %elif61
  %"*self18" = load %Parser*, %Parser** %self, align 8
  %n.left19 = getelementptr inbounds %Node, %Node* %n, i32 0, i32 1
  %"$n.left20" = load i32, i32* %n.left19, align 4
  %call.Parser.eval21 = call i32 @Parser.eval(%Parser* %"*self18", i32 %"$n.left20")
  br label %thrown62

thrown62:                                         ; preds = %elif61.then
  %"$ucg.thrown22" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown22", label %thrown62.then, label %endthrown2

thrown62.then:                                    ; preds = %thrown62
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown2:                                       ; preds = %thrown62
  %"*self23" = load %Parser*, %Parser** %self, align 8
  %n.right24 = getelementptr inbounds %Node, %Node* %n, i32 0, i32 2
  %"$n.right25" = load i32, i32* %n.right24, align 4
  %call.Parser.eval26 = call i32 @Parser.eval(%Parser* %"*self23", i32 %"$n.right25")
  br label %thrown63

thrown63:                                         ; preds = %endthrown2
  %"$ucg.thrown27" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown27", label %thrown63.then, label %endthrown3

thrown63.then:                                    ; preds = %thrown63
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown3:                                       ; preds = %thrown63
  %SUB = sub i32 %call.Parser.eval21, %call.Parser.eval26
  ret i32 %SUB

elif64:                                           ; preds = %elif61
  %"$match@383728" = load i32, i32* %"match@3837", align 4
  %EQ29 = icmp eq i32 %"$match@383728", 4
  br i1 %EQ29, label %elif64.then, label %elif67

elif64.then:                                      ; preds = %elif64
  %"*self30" = load %Parser*, %Parser** %self, align 8
  %n.left31 = getelementptr inbounds %Node, %Node* %n, i32 0, i32 1
  %"$n.left32" = load i32, i32* %n.left31, align 4
  %call.Parser.eval33 = call i32 @Parser.eval(%Parser* %"*self30", i32 %"$n.left32")
  br label %thrown65

thrown65:                                         ; preds = %elif64.then
  %"$ucg.thrown34" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown34", label %thrown65.then, label %endthrown4

thrown65.then:                                    ; preds = %thrown65
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown4:                                       ; preds = %thrown65
  %"*self35" = load %Parser*, %Parser** %self, align 8
  %n.right36 = getelementptr inbounds %Node, %Node* %n, i32 0, i32 2
  %"$n.right37" = load i32, i32* %n.right36, align 4
  %call.Parser.eval38 = call i32 @Parser.eval(%Parser* %"*self35", i32 %"$n.right37")
  br label %thrown66

thrown66:                                         ; preds = %endthrown4
  %"$ucg.thrown39" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown39", label %thrown66.then, label %endthrown5

thrown66.then:                                    ; preds = %thrown66
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown5:                                       ; preds = %thrown66
  %MUL = mul i32 %call.Parser.eval33, %call.Parser.eval38
  ret i32 %MUL

elif67:                                           ; preds = %elif64
  %"$match@383740" = load i32, i32* %"match@3837", align 4
  %EQ41 = icmp eq i32 %"$match@383740", 5
  br i1 %EQ41, label %elif67.then, label %endif8

elif67.then:                                      ; preds = %elif67
  %"*self42" = load %Parser*, %Parser** %self, align 8
  %n.right43 = getelementptr inbounds %Node, %Node* %n, i32 0, i32 2
  %"$n.right44" = load i32, i32* %n.right43, align 4
  %call.Parser.eval45 = call i32 @Parser.eval(%Parser* %"*self42", i32 %"$n.right44")
  br label %thrown68

thrown68:                                         ; preds = %elif67.then
  %"$ucg.thrown46" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown46", label %thrown68.then, label %endthrown6

thrown68.then:                                    ; preds = %thrown68
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown6:                                       ; preds = %thrown68
  store i32 %call.Parser.eval45, i32* %d, align 4
  br label %if69

if69:                                             ; preds = %endthrown6
  %"$d" = load i32, i32* %d, align 4
  %EQ47 = icmp eq i32 %"$d", 0
  br i1 %EQ47, label %if69.then, label %endif

if69.then:                                        ; preds = %if69
  %call.Error.create = call %Error @Error.create(%String { i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.11, i32 0, i32 0), i32 16, i32 0 })
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  store %Error %call.Error.create, %Error* %"*error.at", align 8
  store i32 1, i32* @ucg.error_type, align 4
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endif:                                            ; preds = %if69
  %"*self48" = load %Parser*, %Parser** %self, align 8
  %n.left49 = getelementptr inbounds %Node, %Node* %n, i32 0, i32 1
  %"$n.left50" = load i32, i32* %n.left49, align 4
  %call.Parser.eval51 = call i32 @Parser.eval(%Parser* %"*self48", i32 %"$n.left50")
  br label %thrown70

thrown70:                                         ; preds = %endif
  %"$ucg.thrown52" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown52", label %thrown70.then, label %endthrown7

thrown70.then:                                    ; preds = %thrown70
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  ret i32 0

endthrown7:                                       ; preds = %thrown70
  %"$d53" = load i32, i32* %d, align 4
  %DIV = sdiv i32 %call.Parser.eval51, %"$d53"
  ret i32 %DIV

endif8:                                           ; preds = %elif67
  ret i32 0
}

define void @calc(%String %0) {
entry:
  %text = alloca %String, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %p = alloca %Parser, align 8
  %copy = alloca %String, align 8
  %root = alloca i32, align 4
  %tmp = alloca %String, align 8
  %format.size8 = alloca i32, align 4
  %format.text13 = alloca i8*, align 8
  %n = alloca %Node, align 8
  %ref = alloca %Node*, align 8
  %format.size32 = alloca i32, align 4
  %format.text37 = alloca i8*, align 8
  store %String %0, %String* %text, align 8
  call void @"io::out"(%String { i8* getelementptr inbounds ([42 x i8], [42 x i8]* @str.12, i32 0, i32 0), i32 41, i32 0 })
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([26 x i8], [26 x i8]* @str.13, i32 0, i32 0), i8* %call.String.chars)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf2 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([26 x i8], [26 x i8]* @str.13, i32 0, i32 0), i8* %call.String.chars)
  %"$format.size3" = load i32, i32* %format.size, align 4
  %ADD4 = add i32 %"$format.size3", 1
  %"$format.text5" = load i8*, i8** %format.text, align 8
  %"$format.size6" = load i32, i32* %format.size, align 4
  %1 = insertvalue %String zeroinitializer, i8* %"$format.text5", 0
  %2 = insertvalue %String %1, i32 %"$format.size6", 1
  %3 = insertvalue %String %2, i32 %ADD4, 2
  call void @"io::out"(%String %3)
  store %Parser zeroinitializer, %Parser* %p, align 8
  %p.toks = getelementptr inbounds %Parser, %Parser* %p, i32 0, i32 0
  store %String zeroinitializer, %String* %copy, align 8
  %4 = bitcast %String* %copy to %"Array<i8>"*
  %5 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.="(%"Array<i8>"* %4, %"Array<i8>"* %5)
  %"$copy" = load %String, %String* %copy, align 8
  %call.lex = call %"Array<Token>" @lex(%String %"$copy")
  call void @"Array<Token>.drop"(%"Array<Token>"* %p.toks)
  store %"Array<Token>" %call.lex, %"Array<Token>"* %p.toks, align 8
  %call.Parser.expr = call i32 @Parser.expr(%Parser* %p)
  store i32 %call.Parser.expr, i32* %root, align 4
  %"$root" = load i32, i32* %root, align 4
  %call.Parser.label = call %String @Parser.label(%Parser* %p, i32 %"$root")
  store %String %call.Parser.label, %String* %tmp, align 8
  %call.String.chars7 = call i8* @String.chars(%String* %tmp)
  %call.snprintf9 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.14, i32 0, i32 0), i8* %call.String.chars7)
  store i32 %call.snprintf9, i32* %format.size8, align 4
  %"$format.size10" = load i32, i32* %format.size8, align 4
  %ADD11 = add i32 %"$format.size10", 1
  %AS12 = sext i32 %ADD11 to i64
  %call.calloc14 = call i8* @calloc(i64 %AS12, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc14, i8** %format.text13, align 8
  %"$format.text15" = load i8*, i8** %format.text13, align 8
  %call.snprintf16 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text15", i64 %AS12, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.14, i32 0, i32 0), i8* %call.String.chars7)
  %"$format.size17" = load i32, i32* %format.size8, align 4
  %ADD18 = add i32 %"$format.size17", 1
  %"$format.text19" = load i8*, i8** %format.text13, align 8
  %"$format.size20" = load i32, i32* %format.size8, align 4
  %6 = insertvalue %String zeroinitializer, i8* %"$format.text19", 0
  %7 = insertvalue %String %6, i32 %"$format.size20", 1
  %8 = insertvalue %String %7, i32 %ADD18, 2
  call void @"io::out"(%String %8)
  %9 = bitcast %String* %tmp to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %9)
  %p.nodes = getelementptr inbounds %Parser, %Parser* %p, i32 0, i32 2
  %"$root21" = load i32, i32* %root, align 4
  %"call.Array<Node>.[]" = call %Node* @"Array<Node>.[]"(%"Array<Node>"* %p.nodes, i32 %"$root21")
  store %Node* %"call.Array<Node>.[]", %Node** %ref, align 8
  %"*ref" = load %Node*, %Node** %ref, align 8
  %"$*ref" = load %Node, %Node* %"*ref", align 4
  store %Node %"$*ref", %Node* %n, align 4
  br label %if71

if71:                                             ; preds = %entry
  %n.left = getelementptr inbounds %Node, %Node* %n, i32 0, i32 1
  %"$n.left" = load i32, i32* %n.left, align 4
  %GE = icmp sge i32 %"$n.left", 0
  br i1 %GE, label %if71.then, label %endif

if71.then:                                        ; preds = %if71
  %n.left22 = getelementptr inbounds %Node, %Node* %n, i32 0, i32 1
  %n.right = getelementptr inbounds %Node, %Node* %n, i32 0, i32 2
  %"$n.right" = load i32, i32* %n.right, align 4
  %LT = icmp slt i32 %"$n.right", 0
  %"$n.left23" = load i32, i32* %n.left22, align 4
  call void @Parser.show(%Parser* %p, i32 %"$n.left23", %String { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.15, i32 0, i32 0), i32 0, i32 0 }, i1 %LT)
  br label %endif

endif:                                            ; preds = %if71.then, %if71
  br label %if72

if72:                                             ; preds = %endif
  %n.right24 = getelementptr inbounds %Node, %Node* %n, i32 0, i32 2
  %"$n.right25" = load i32, i32* %n.right24, align 4
  %GE26 = icmp sge i32 %"$n.right25", 0
  br i1 %GE26, label %if72.then, label %endif1

if72.then:                                        ; preds = %if72
  %n.right27 = getelementptr inbounds %Node, %Node* %n, i32 0, i32 2
  %"$n.right28" = load i32, i32* %n.right27, align 4
  call void @Parser.show(%Parser* %p, i32 %"$n.right28", %String { i8* getelementptr inbounds ([1 x i8], [1 x i8]* @str.16, i32 0, i32 0), i32 0, i32 0 }, i1 true)
  br label %endif1

endif1:                                           ; preds = %if72.then, %if72
  %"$root29" = load i32, i32* %root, align 4
  %call.Parser.eval = call i32 @Parser.eval(%Parser* %p, i32 %"$root29")
  br label %thrown73

thrown73:                                         ; preds = %endif1
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown73.then, label %endthrown

thrown73.then:                                    ; preds = %thrown73
  store i1 false, i1* @ucg.thrown, align 1
  %p.nodes30 = getelementptr inbounds %Parser, %Parser* %p, i32 0, i32 2
  call void @"Array<Node>.drop"(%"Array<Node>"* %p.nodes30)
  %p.toks31 = getelementptr inbounds %Parser, %Parser* %p, i32 0, i32 0
  call void @"Array<Token>.drop"(%"Array<Token>"* %p.toks31)
  %10 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %10)
  store i1 true, i1* @ucg.thrown, align 1
  ret void

endthrown:                                        ; preds = %thrown73
  %call.snprintf33 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.17, i32 0, i32 0), i32 %call.Parser.eval)
  store i32 %call.snprintf33, i32* %format.size32, align 4
  %"$format.size34" = load i32, i32* %format.size32, align 4
  %ADD35 = add i32 %"$format.size34", 1
  %AS36 = sext i32 %ADD35 to i64
  %call.calloc38 = call i8* @calloc(i64 %AS36, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc38, i8** %format.text37, align 8
  %"$format.text39" = load i8*, i8** %format.text37, align 8
  %call.snprintf40 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text39", i64 %AS36, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.17, i32 0, i32 0), i32 %call.Parser.eval)
  %"$format.size41" = load i32, i32* %format.size32, align 4
  %ADD42 = add i32 %"$format.size41", 1
  %"$format.text43" = load i8*, i8** %format.text37, align 8
  %"$format.size44" = load i32, i32* %format.size32, align 4
  %11 = insertvalue %String zeroinitializer, i8* %"$format.text43", 0
  %12 = insertvalue %String %11, i32 %"$format.size44", 1
  %13 = insertvalue %String %12, i32 %ADD42, 2
  call void @"io::out"(%String %13)
  %p.nodes45 = getelementptr inbounds %Parser, %Parser* %p, i32 0, i32 2
  call void @"Array<Node>.drop"(%"Array<Node>"* %p.nodes45)
  %p.toks46 = getelementptr inbounds %Parser, %Parser* %p, i32 0, i32 0
  call void @"Array<Token>.drop"(%"Array<Token>"* %p.toks46)
  %14 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %14)
  ret void
}

define i32 @main() {
entry:
  %line = alloca %String, align 8
  %copy = alloca %String, align 8
  %e = alloca %Error, align 8
  %error.at = alloca %Error*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  %error.at16 = alloca %Error*, align 8
  br label %try74

try74:                                            ; preds = %entry
  br i1 true, label %try74.then, label %endtry1

try74.then:                                       ; preds = %try74
  %"call.io::input" = call %String @"io::input"()
  store %String %"call.io::input", %String* %line, align 8
  br label %while75

while75:                                          ; preds = %endrethrow, %try74.then
  %call.String.is_empty = call i1 @String.is_empty(%String* %line)
  %NOT = xor i1 %call.String.is_empty, true
  br i1 %NOT, label %while75.then, label %endloop

while75.then:                                     ; preds = %while75
  br label %try76

try76:                                            ; preds = %while75.then
  br i1 true, label %try76.then, label %endtry

try76.then:                                       ; preds = %try76
  store %String zeroinitializer, %String* %copy, align 8
  %0 = bitcast %String* %copy to %"Array<i8>"*
  %1 = bitcast %String* %line to %"Array<i8>"*
  call void @"Array<i8>.="(%"Array<i8>"* %0, %"Array<i8>"* %1)
  %"$copy" = load %String, %String* %copy, align 8
  call void @calc(%String %"$copy")
  br label %thrown77

thrown77:                                         ; preds = %try76.then
  %"$ucg.thrown" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown", label %thrown77.then, label %endthrown

thrown77.then:                                    ; preds = %thrown77
  store i1 false, i1* @ucg.thrown, align 1
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry

endthrown:                                        ; preds = %thrown77
  br label %endtry

endtry:                                           ; preds = %endthrown, %thrown77.then, %try76
  br label %catch78

catch78:                                          ; preds = %endtry
  %"$ucg.error_type" = load i32, i32* @ucg.error_type, align 4
  %EQ = icmp eq i32 %"$ucg.error_type", 1
  %"$ucg.thrown3" = load i1, i1* @ucg.thrown, align 1
  %AND = and i1 %"$ucg.thrown3", %EQ
  br i1 %AND, label %catch78.then, label %endcatch

catch78.then:                                     ; preds = %catch78
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at, align 8
  %"*error.at" = load %Error*, %Error** %error.at, align 8
  %"$*error.at" = load %Error, %Error* %"*error.at", align 8
  store %Error %"$*error.at", %Error* %e, align 8
  %e.message = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  %call.String.chars = call i8* @String.chars(%String* %e.message)
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.18, i32 0, i32 0), i8* %call.String.chars)
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf4 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.18, i32 0, i32 0), i8* %call.String.chars)
  %"$format.size5" = load i32, i32* %format.size, align 4
  %ADD6 = add i32 %"$format.size5", 1
  %"$format.text7" = load i8*, i8** %format.text, align 8
  %"$format.size8" = load i32, i32* %format.size, align 4
  %2 = insertvalue %String zeroinitializer, i8* %"$format.text7", 0
  %3 = insertvalue %String %2, i32 %"$format.size8", 1
  %4 = insertvalue %String %3, i32 %ADD6, 2
  call void @"io::out"(%String %4)
  %e.message9 = getelementptr inbounds %Error, %Error* %e, i32 0, i32 0
  %5 = bitcast %String* %e.message9 to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %5)
  br label %endcatch

endcatch:                                         ; preds = %catch78.then, %catch78
  br label %rethrow79

rethrow79:                                        ; preds = %endcatch
  %"$ucg.thrown10" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown10", label %rethrow79.then, label %endrethrow

rethrow79.then:                                   ; preds = %rethrow79
  store i1 false, i1* @ucg.thrown, align 1
  %6 = bitcast %String* %line to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %6)
  store i1 true, i1* @ucg.thrown, align 1
  br label %endtry1

endrethrow:                                       ; preds = %rethrow79
  %"call.io::input11" = call %String @"io::input"()
  %7 = bitcast %String* %line to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %7)
  store %String %"call.io::input11", %String* %line, align 8
  br label %while75

endloop:                                          ; preds = %while75
  call void @"io::out"(%String { i8* getelementptr inbounds ([42 x i8], [42 x i8]* @str.19, i32 0, i32 0), i32 41, i32 0 })
  %8 = bitcast %String* %line to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %8)
  ret i32 0

endtry1:                                          ; preds = %rethrow79.then, %try74
  br label %catch80

catch80:                                          ; preds = %endtry1
  %"$ucg.error_type12" = load i32, i32* @ucg.error_type, align 4
  %EQ13 = icmp eq i32 %"$ucg.error_type12", 1
  %"$ucg.thrown14" = load i1, i1* @ucg.thrown, align 1
  %AND15 = and i1 %"$ucg.thrown14", %EQ13
  br i1 %AND15, label %catch80.then, label %endcatch2

catch80.then:                                     ; preds = %catch80
  store i1 false, i1* @ucg.thrown, align 1
  store %Error* bitcast ([2 x i64]* @ucg.error to %Error*), %Error** %error.at16, align 8
  %"*error.at17" = load %Error*, %Error** %error.at16, align 8
  %call.uncaught = call i32 @uncaught(%Error* %"*error.at17")
  ret i32 %call.uncaught

endcatch2:                                        ; preds = %catch80
  br label %if81

if81:                                             ; preds = %endcatch2
  %"$ucg.thrown18" = load i1, i1* @ucg.thrown, align 1
  br i1 %"$ucg.thrown18", label %if81.then, label %endif

if81.then:                                        ; preds = %if81
  ret i32 1

endif:                                            ; preds = %if81
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

---

## 026 — project: libft (library and its main)

```ura
// expect: 0
// ---- libft.ura
use "@/core"
fn ft_isalpha(c char) bool:
    return (c >= 'a' and c <= 'z') or (c >= 'A' and c <= 'Z')

fn ft_isdigit(c char) bool:
    return c >= '0' and c <= '9'

fn ft_isalnum(c char) bool:
    return ft_isalpha(c) or ft_isdigit(c)

fn ft_isascii(c char) bool:
    return c >= 0 as char and c <= 127 as char

fn ft_isprint(c char) bool:
    return c >= 32 as char and c <= 126 as char

fn ft_toupper(c char) char:
    if c >= 'a' and c <= 'z':
        return (c as i32 - 32) as char
    return c

fn ft_tolower(c char) char:
    if c >= 'A' and c <= 'Z':
        return (c as i32 + 32) as char
    return c

fn ft_strlen(s char[]) i32:
    return s.len as i32

fn ft_strncmp(a char[], b char[], n i32) i32:
    i i32 = 0
    while i < n:
        ca i32 = 0
        cb i32 = 0
        if i < a.len as i32:
            ca = a[i] as i32
        if i < b.len as i32:
            cb = b[i] as i32
        if ca != cb:
            return ca - cb
        if ca == 0:
            return 0
        i += 1
    return 0

fn ft_memcmp(a char[], b char[], n i32) i32:
    i i32 = 0
    while i < n:
        if a[i] != b[i]:
            return a[i] as i32 - b[i] as i32
        i += 1
    return 0

fn ft_strchr(s char[], c char) i32:
    i i32 = 0
    while i < s.len as i32:
        if s[i] == c:
            return i
        i += 1
    if c == 0 as char:
        return s.len as i32
    return -1

fn ft_strrchr(s char[], c char) i32:
    i i32 = s.len as i32 - 1
    while i >= 0:
        if s[i] == c:
            return i
        i -= 1
    return -1

fn ft_memchr(s char[], c char, n i32) i32:
    i i32 = 0
    while i < n:
        if s[i] == c:
            return i
        i += 1
    return -1

fn ft_strnstr(hay char[], needle char[], n i32) i32:
    m i32 = needle.len as i32
    if m == 0:
        return 0
    i i32 = 0
    while i + m <= n and i + m <= hay.len as i32:
        if ft_strncmp(hay[i..i + m], needle, m) == 0:
            return i
        i += 1
    return -1

fn ft_atoi(s char[]) i32:
    i i32 = 0
    while i < s.len as i32 and (s[i] == ' ' or s[i] == '\t' or s[i] == '\n'):
        i += 1
    sign i32 = 1
    if i < s.len as i32 and (s[i] == '-' or s[i] == '+'):
        if s[i] == '-':
            sign = -1
        i += 1
    n i32 = 0
    while i < s.len as i32 and ft_isdigit(s[i]):
        n = n * 10 + (s[i] as i32 - 48)
        i += 1
    return n * sign

fn ft_memset(b char[], c char, n i32):
    i i32 = 0
    while i < n:
        b[i] = c
        i += 1

fn ft_bzero(b char[], n i32):
    ft_memset(b, 0 as char, n)

fn ft_memcpy(dst char[], src char[], n i32):
    i i32 = 0
    while i < n:
        dst[i] = src[i]
        i += 1

fn ft_memmove(dst char[], src char[], n i32):
    tmp char[] = new char[n]
    ft_memcpy(tmp, src, n)
    ft_memcpy(dst, tmp, n)
    clean tmp

fn ft_strlcpy(dst char[], src char[], size i32) i32:
    slen i32 = src.len as i32
    i i32 = 0
    while i + 1 < size and i < slen:
        dst[i] = src[i]
        i += 1
    if size > 0:
        dst[i] = 0 as char
    return slen

fn ft_strlcat(dst char[], src char[], size i32) i32:
    dlen i32 = 0
    while dlen < size and dlen < dst.len as i32 and dst[dlen] != 0 as char:
        dlen += 1
    slen i32 = src.len as i32
    if dlen == size:
        return size + slen
    i i32 = 0
    while dlen + i + 1 < size and i < slen:
        dst[dlen + i] = src[i]
        i += 1
    dst[dlen + i] = 0 as char
    return dlen + slen

fn ft_strdup(s char[]) char[]:
    n i32 = s.len as i32
    out char[] = new char[n]
    ft_memcpy(out, s, n)
    return out

fn ft_calloc(count i32, size i32) char[]:
    return new char[count * size]

fn ft_substr(s char[], start i32, len i32) char[]:
    slen i32 = s.len as i32
    if start >= slen:
        return new char[0]
    stop i32 = start + len
    if stop > slen:
        stop = slen
    n i32 = stop - start
    out char[] = new char[n]
    i i32 = 0
    while i < n:
        out[i] = s[start + i]
        i += 1
    return out

fn ft_strjoin(a char[], b char[]) char[]:
    la i32 = a.len as i32
    lb i32 = b.len as i32
    out char[] = new char[la + lb]
    ft_memcpy(out, a, la)
    i i32 = 0
    while i < lb:
        out[la + i] = b[i]
        i += 1
    return out

fn ft_in_set(c char, set char[]) bool:
    i i32 = 0
    while i < set.len as i32:
        if set[i] == c:
            return True
        i += 1
    return False

fn ft_strtrim(s char[], set char[]) char[]:
    slen i32 = s.len as i32
    start i32 = 0
    while start < slen and ft_in_set(s[start], set):
        start += 1
    stop i32 = slen
    while stop > start and ft_in_set(s[stop - 1], set):
        stop -= 1
    return ft_substr(s, start, stop - start)

fn ft_count_words(s char[], c char) i32:
    slen i32 = s.len as i32
    wc i32 = 0
    i i32 = 0
    while i < slen:
        if s[i] != c:
            wc += 1
            while i < slen and s[i] != c:
                i += 1
        else:
            i += 1
    return wc

fn ft_split(s char[], c char) char[][]:
    slen i32 = s.len as i32
    wc i32 = ft_count_words(s, c)
    if wc == 0:
        return new char[0][1]
    out char[][] = new char[wc][1]
    w i32 = 0
    i i32 = 0
    while i < slen:
        if s[i] != c:
            start i32 = i
            while i < slen and s[i] != c:
                i += 1
            out[w] = ft_substr(s, start, i - start)
            w += 1
        else:
            i += 1
    return out

fn ft_num_digits(n i32) i32:
    d i32 = 0
    tmp i32 = n
    while tmp != 0:
        tmp = tmp / 10
        d += 1
    return d

fn ft_itoa(n i32) char[]:
    if n == 0:
        z char[] = new char[1]
        z[0] = '0'
        return z
    neg bool = n < 0
    digits i32 = ft_num_digits(n)
    total i32 = digits
    if neg:
        total += 1
    out char[] = new char[total]
    num i32 = n
    idx i32 = total - 1
    i i32 = 0
    while i < digits:
        d i32 = num % 10
        if d < 0:
            d = -d
        out[idx] = (d + 48) as char
        num = num / 10
        idx -= 1
        i += 1
    if neg:
        out[0] = '-'
    return out

fn ft_strmapi(s char[], f fn(u32, char) char) char[]:
    n i32 = s.len as i32
    out char[] = new char[n]
    i i32 = 0
    while i < n:
        out[i] = f(i as u32, s[i])
        i += 1
    return out

fn ft_striteri(s char[], f fn(u32, char) char) void:
    i i32 = 0
    while i < s.len as i32:
        s[i] = f(i as u32, s[i])
        i += 1

fn ft_putchar_fd(c char, fd i32):
    b char[] = new char[1]
    b[0] = c
    write(fd, b, 1)
    clean b

fn ft_putstr_fd(s char[], fd i32):
    write(fd, s, s.len as i64)

fn ft_putendl_fd(s char[], fd i32):
    ft_putstr_fd(s, fd)
    ft_putchar_fd('\n', fd)

fn ft_putnbr_fd(n i32, fd i32):
    s char[] = ft_itoa(n)
    ft_putstr_fd(s, fd)
    clean s

struct Node:
    content char[]
    next &Node

struct Lst:
    head &Node

fn ft_lstnew(content char[]) &Node:
    n Node[] = new Node[1]
    n[0].content = content
    return &n[0]

fn ft_lstadd_front(lst &Lst, node &Node):
    node.next = &lst.head
    lst.head  = &node

fn ft_lstadd_back(lst &Lst, node &Node):
    if lst.head == null:
        lst.head = &node
        return
    cur &Node = &lst.head
    while cur.next != null:
        cur = &cur.next
    cur.next = &node

fn ft_lstsize(lst &Lst) i32:
    cur &Node = &lst.head
    count i32 = 0
    while cur != null:
        count += 1
        cur = &cur.next
    return count

fn ft_lstlast(lst &Lst) &Node:
    cur &Node = &lst.head
    if cur == null:
        return null
    while cur.next != null:
        cur = &cur.next
    return &cur

fn ft_lstiter(lst &Lst, f fn(char[]) char[]) void:
    cur &Node = &lst.head
    while cur != null:
        cur.content = f(cur.content)
        cur = &cur.next

fn ft_lstmap(lst &Lst, f fn(char[]) char[]) Lst:
    out Lst
    cur &Node = &lst.head
    while cur != null:
        ft_lstadd_back(&out, ft_lstnew(f(cur.content)))
        cur = &cur.next
    return out

fn ft_lstdelone(lst &Lst, idx i32):
    if lst.head == null:
        return
    if idx <= 0:
        lst.head = &lst.head.next
        return
    prev &Node = &lst.head
    i i32 = 1
    while i < idx and prev.next != null:
        prev = &prev.next
        i += 1
    if prev.next != null:
        prev.next = &prev.next.next

fn ft_lstclear(lst &Lst):
    lst.head = null
// ---- main.ura
use "libft"

fn upcase_i(i u32, c char) char:
    if c >= 'a' and c <= 'z':
        return (c as i32 - 32) as char
    return c

fn upcase(s char[]) char[]:
    n i32 = s.len as i32
    out char[] = new char[n]
    i i32 = 0
    while i < n:
        c char = s[i]
        if c >= 'a' and c <= 'z':
            out[i] = (c as i32 - 32) as char
        else:
            out[i] = c
        i += 1
    return out

fn lower(s char[]) char[]:
    n i32 = s.len as i32
    out char[] = new char[n]
    i i32 = 0
    while i < n:
        c char = s[i]
        if c >= 'A' and c <= 'Z':
            out[i] = (c as i32 + 32) as char
        else:
            out[i] = c
        i += 1
    return out

fn dump(lst &Lst):
    cur &Node = &lst.head
    while cur != null:
        io::out($"{cur.content} ")
        cur = &cur.next
    io::out("\n")

fn main() i32:
    io::out($"ft_isalpha('a')={ft_isalpha('a')} ('1')={ft_isalpha('1')}\n")
    io::out($"ft_isdigit('7')={ft_isdigit('7')} ('x')={ft_isdigit('x')}\n")
    io::out($"ft_isalnum('Z')={ft_isalnum('Z')} (' ')={ft_isalnum(' ')}\n")
    io::out($"ft_toupper('a')={ft_toupper('a')} ft_tolower('A')={ft_tolower('A')}\n")
    io::out($"ft_strlen(\"hello\")={ft_strlen("hello")}\n")
    io::out($"ft_strncmp(abc,abd,3)={ft_strncmp("abc", "abd", 3)}\n")
    io::out($"ft_strchr(hello,l)={ft_strchr("hello", 'l')}\n")
    io::out($"ft_strrchr(hello,l)={ft_strrchr("hello", 'l')}\n")
    io::out($"ft_strnstr(hello world,wor,11)={ft_strnstr("hello world", "wor", 11)}\n")
    io::out($"ft_atoi(\"  -42abc\")={ft_atoi("  -42abc")}\n")

    buf char[] = new char[8]
    ft_memset(buf, 'x', 4)
    buf[4] = 0 as char
    io::out($"ft_memset -> {buf}\n")
    n i32 = ft_strlcpy(buf, "hi", 8)
    io::out($"ft_strlcpy -> {buf} (ret {n})\n")
    clean buf

    io::out("--- part 2 ---\n")
    a char[] = ft_substr("hello world", 6, 5)
    io::out($"ft_substr -> {a}\n")
    clean a

    j char[] = ft_strjoin("foo", "bar")
    io::out($"ft_strjoin -> {j}\n")
    clean j

    t char[] = ft_strtrim("xxhelloxx", "x")
    io::out($"ft_strtrim -> {t}\n")
    clean t

    parts char[][] = ft_split("a,bb,ccc", ',')
    io::out($"ft_split -> {parts.len as i32} words: ")
    i i32 = 0
    while i < parts.len as i32:
        io::out($"{parts[i]} ")
        i += 1
    io::out("\n")

    p char[] = ft_itoa(-2147483648)
    io::out($"ft_itoa(INT_MIN) -> {p}\n")
    clean p
    q char[] = ft_itoa(0)
    io::out($"ft_itoa(0) -> {q}\n")
    clean q

    m char[] = ft_strmapi("hello", upcase_i)
    io::out($"ft_strmapi -> {m}\n")
    clean m

    it char[] = ft_strdup("world")
    ft_striteri(it, upcase_i)
    io::out($"ft_striteri -> {it}\n")
    clean it

    io::out("ft_putnbr_fd -> ")
    ft_putnbr_fd(-42, 1)
    io::out("\n")
    ft_putendl_fd("ft_putendl_fd works", 1)

    io::out("--- bonus: linked list ---\n")
    lst Lst
    ft_lstadd_back(&lst, ft_lstnew("a"))
    ft_lstadd_back(&lst, ft_lstnew("b"))
    ft_lstadd_back(&lst, ft_lstnew("c"))
    ft_lstadd_front(&lst, ft_lstnew("z"))
    io::out("list:   ")
    dump(&lst)
    io::out($"size:   {ft_lstsize(&lst)}\n")

    L &Node = ft_lstlast(&lst)
    io::out($"last:   {L.content}\n")

    ft_lstiter(&lst, upcase)
    io::out("iter:   ")
    dump(&lst)

    mapped Lst = ft_lstmap(&lst, lower)
    io::out("map:    ")
    dump(&mapped)

    ft_lstdelone(&lst, 1)
    io::out("del 1:  ")
    dump(&lst)

    ft_lstdelone(&lst, 0)
    io::out("del 0:  ")
    dump(&lst)

    ft_lstclear(&lst)
    io::out($"clear:  size {ft_lstsize(&lst)}\n")
    return 0
```

---

## 027 — project: net server

```ura
// a TCP chat server on one poll() loop: stdin, the listener and every client.
// A line typed here goes to every client, a line from a client is printed.
// It needs port 8080, so run it by hand: the test only compiles it.
use "@/core"
use "@/posix/net"
use "@/posix/unistd"

PORT i32 = 8080

fn address(port i32) Array<i8>:
    data Array<i8>
    @if os == "macos":
        data.append(16)
        data.append(2)
    @else:
        data.append(2)
        data.append(0)
    data.append((port >> 8) as i8)
    data.append((port & 255) as i8)
    for i in Range::create(0, 12):
        data.append(0)
    return data

fn listener(port i32) i32:
    fd i32 = socket(2, 1, 0)
    if fd < 0:
        return -1
    one Array<i32>
    one.append(1)
    @if os == "macos":
        setsockopt(fd, 65535, 4, one.data as i8[], 4)
    @else:
        setsockopt(fd, 1, 2, one.data as i8[], 4)
    addr Array<i8> = address(port)
    is_bound bool = bind(fd, addr.data, 16) == 0
    if not is_bound or listen(fd, 16) < 0:
        close(fd)
        return -1
    return fd

fn watch(fds &Array<Pollfd>, fd i32):
    p Pollfd
    p.fd = fd
    p.events = POLLIN as i16
    fds.append(p)

fn main() i32:
    server i32 = listener(PORT)
    if server < 0:
        io::err("[server] can't listen on port 8080\n")
        return 1
    io::out("[server] listening on port 8080\n")

    // [0] stdin, [1] the listener, [2..] the clients
    fds Array<Pollfd>
    watch(&fds,0)
    watch(&fds,server)
    buf Array<i8>
    buf.resize(1024)

    while True:
        poll(fds.data as i8[], fds.len(), -1)

        // a line typed here goes to every client
        if ((fds[0].revents as i32) & POLLIN) != 0:
            r i64 = read(0, buf.data, 1023)
            if r <= 0:
                break
            i i32 = 2
            while i < fds.len():
                write(fds[i].fd, buf.data, r)
                i += 1

        // a new client joins the poll set
        if ((fds[1].revents as i32) & POLLIN) != 0:
            client i32 = accept(server, null, null)
            if client >= 0:
                watch(&fds,client)
                io::out("[server] client connected\n")

        // a client's line is printed; a closed client leaves the set
        j i32 = 2
        while j < fds.len():
            if ((fds[j].revents as i32) & POLLIN) == 0:
                j += 1
                continue
            r i64 = read(fds[j].fd, buf.data, 1023)
            if r <= 0:
                close(fds[j].fd)
                fds[j] = fds[fds.len() - 1]
                fds.pop()
                io::out("[server] client disconnected\n")
                continue
            io::out("[client] ")
            write(1, buf.data, r)
            j += 1

    close(server)
    return 0
```

---

## 028 — project: net client

```ura
// a TCP chat client on one poll() loop: stdin and the server socket.
// A line typed here goes to the server, a line from the server is printed.
// It needs the net server on port 8080, so run it by hand: the test only
// compiles it.
use "@/core"
use "@/posix/net"
use "@/posix/unistd"

PORT i32 = 8080

fn address(port i32) Array<i8>:
    data Array<i8>
    @if os == "macos":
        data.append(16)
        data.append(2)
    @else:
        data.append(2)
        data.append(0)
    data.append((port >> 8) as i8)
    data.append((port & 255) as i8)
    data.append(127)
    data.append(0)
    data.append(0)
    data.append(1)
    for i in Range::create(0, 8):
        data.append(0)
    return data

fn watch(fds &Array<Pollfd>, fd i32):
    p Pollfd
    p.fd = fd
    p.events = POLLIN as i16
    fds.append(p)

fn main() i32:
    fd i32 = socket(2, 1, 0)
    addr Array<i8> = address(PORT)
    if fd < 0 or connect(fd, addr.data, 16) != 0:
        io::err("[client] can't reach 127.0.0.1:8080\n")
        return 1
    io::out("[client] connected to 127.0.0.1:8080\n")

    // [0] stdin, [1] the server
    fds Array<Pollfd>
    watch(&fds,0)
    watch(&fds,fd)
    buf Array<i8>
    buf.resize(1024)

    while True:
        poll(fds.data as i8[], fds.len(), -1)

        // a line typed here goes to the server
        if ((fds[0].revents as i32) & POLLIN) != 0:
            r i64 = read(0, buf.data, 1023)
            if r <= 0:
                break
            write(fd, buf.data, r)

        // a line from the server is printed; a closed server ends the loop
        if ((fds[1].revents as i32) & POLLIN) != 0:
            r i64 = read(fd, buf.data, 1023)
            if r <= 0:
                io::out("[client] the server closed the connection\n")
                break
            io::out("[server] ")
            write(1, buf.data, r)

    close(fd)
    return 0
```

---

## 029 — dungeon: floor04

```ura
// stdout: Orc takes 17 damage — dead: True
// stdout: 4 squared: 16
// expect: 0
// dungeon/floor04.ura
use "@/core"
fn clamp(val i32, lo i32, hi i32) i32:
    if val < lo: return lo
    if val > hi: return hi
    return val

fn damage(atk i32, def i32) i32:
    return clamp(atk - def, 0, 999)

fn is_dead(hp i32) bool:
    return hp <= 0

fn square(n i32) i32:      return n * n
fn is_digit(c char) bool:  return c >= '0' and c <= '9'

fn main() i32:
    d i32 = damage(25, 8)
    io::out($"Orc takes {d} damage — dead: {is_dead(d - 60)}\n")
    io::out($"4 squared: {square(4)}\n")
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

@str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.2 = private unnamed_addr constant [34 x i8] c"Orc takes %d damage \E2\80\94 dead: %s\0A\00", align 1
@str.3 = private unnamed_addr constant [15 x i8] c"4 squared: %d\0A\00", align 1

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

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
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
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i32 @clamp(i32 %0, i32 %1, i32 %2) {
entry:
  %val = alloca i32, align 4
  %lo = alloca i32, align 4
  %hi = alloca i32, align 4
  store i32 %0, i32* %val, align 4
  store i32 %1, i32* %lo, align 4
  store i32 %2, i32* %hi, align 4
  br label %if6

if6:                                              ; preds = %entry
  %"$val" = load i32, i32* %val, align 4
  %"$lo" = load i32, i32* %lo, align 4
  %LT = icmp slt i32 %"$val", %"$lo"
  br i1 %LT, label %if6.then, label %endif

if6.then:                                         ; preds = %if6
  %"$lo2" = load i32, i32* %lo, align 4
  ret i32 %"$lo2"

endif:                                            ; preds = %if6
  br label %if7

if7:                                              ; preds = %endif
  %"$val3" = load i32, i32* %val, align 4
  %"$hi" = load i32, i32* %hi, align 4
  %GT = icmp sgt i32 %"$val3", %"$hi"
  br i1 %GT, label %if7.then, label %endif1

if7.then:                                         ; preds = %if7
  %"$hi4" = load i32, i32* %hi, align 4
  ret i32 %"$hi4"

endif1:                                           ; preds = %if7
  %"$val5" = load i32, i32* %val, align 4
  ret i32 %"$val5"
}

define i32 @damage(i32 %0, i32 %1) {
entry:
  %atk = alloca i32, align 4
  %def = alloca i32, align 4
  store i32 %0, i32* %atk, align 4
  store i32 %1, i32* %def, align 4
  %"$atk" = load i32, i32* %atk, align 4
  %"$def" = load i32, i32* %def, align 4
  %SUB = sub i32 %"$atk", %"$def"
  %call.clamp = call i32 @clamp(i32 %SUB, i32 0, i32 999)
  ret i32 %call.clamp
}

define i1 @is_dead(i32 %0) {
entry:
  %hp = alloca i32, align 4
  store i32 %0, i32* %hp, align 4
  %"$hp" = load i32, i32* %hp, align 4
  %LE = icmp sle i32 %"$hp", 0
  ret i1 %LE
}

define i32 @square(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %"$n1" = load i32, i32* %n, align 4
  %MUL = mul i32 %"$n", %"$n1"
  ret i32 %MUL
}

define i32 @main() {
entry:
  %d = alloca i32, align 4
  %format.size = alloca i32, align 4
  %bool.text = alloca i8*, align 8
  %format.text = alloca i8*, align 8
  %format.size8 = alloca i32, align 4
  %format.text13 = alloca i8*, align 8
  %call.damage = call i32 @damage(i32 25, i32 8)
  store i32 %call.damage, i32* %d, align 4
  %"$d" = load i32, i32* %d, align 4
  %SUB = sub i32 %"$d", 60
  %call.is_dead = call i1 @is_dead(i32 %SUB)
  store i32 0, i32* %format.size, align 4
  %"$d1" = load i32, i32* %d, align 4
  store i8* null, i8** %bool.text, align 8
  br label %if8

if8:                                              ; preds = %entry
  br i1 %call.is_dead, label %if8.then, label %else9

if8.then:                                         ; preds = %if8
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

else9:                                            ; preds = %if8
  store i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0), i8** %bool.text, align 8
  br label %endif

endif:                                            ; preds = %else9, %if8.then
  %"$bool.text" = load i8*, i8** %bool.text, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([34 x i8], [34 x i8]* @str.2, i32 0, i32 0), i32 %"$d1", i8* %"$bool.text")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %"$bool.text2" = load i8*, i8** %bool.text, align 8
  %call.snprintf3 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([34 x i8], [34 x i8]* @str.2, i32 0, i32 0), i32 %"$d1", i8* %"$bool.text2")
  %"$format.size4" = load i32, i32* %format.size, align 4
  %ADD5 = add i32 %"$format.size4", 1
  %"$format.text6" = load i8*, i8** %format.text, align 8
  %"$format.size7" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text6", 0
  %1 = insertvalue %String %0, i32 %"$format.size7", 1
  %2 = insertvalue %String %1, i32 %ADD5, 2
  call void @"io::out"(%String %2)
  %call.square = call i32 @square(i32 4)
  %call.snprintf9 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.3, i32 0, i32 0), i32 %call.square)
  store i32 %call.snprintf9, i32* %format.size8, align 4
  %"$format.size10" = load i32, i32* %format.size8, align 4
  %ADD11 = add i32 %"$format.size10", 1
  %AS12 = sext i32 %ADD11 to i64
  %call.calloc14 = call i8* @calloc(i64 %AS12, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc14, i8** %format.text13, align 8
  %"$format.text15" = load i8*, i8** %format.text13, align 8
  %call.snprintf16 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text15", i64 %AS12, i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.3, i32 0, i32 0), i32 %call.square)
  %"$format.size17" = load i32, i32* %format.size8, align 4
  %ADD18 = add i32 %"$format.size17", 1
  %"$format.text19" = load i8*, i8** %format.text13, align 8
  %"$format.size20" = load i32, i32* %format.size8, align 4
  %3 = insertvalue %String zeroinitializer, i8* %"$format.text19", 0
  %4 = insertvalue %String %3, i32 %"$format.size20", 1
  %5 = insertvalue %String %4, i32 %ADD18, 2
  call void @"io::out"(%String %5)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

---

## 030 — dungeon: floor09b

```ura
// stdout: 100
// expect: 0
// dungeon/floor09b.ura
use "@/core"
fn main() i32:
    hp  i32     = 80
    r &i32 = &hp
    r   = 100
    io::out($"{hp}\n")
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

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

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

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
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
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i32 @main() {
entry:
  %hp = alloca i32, align 4
  %r = alloca i32*, align 8
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store i32 80, i32* %hp, align 4
  store i32* %hp, i32** %r, align 8
  %"*r" = load i32*, i32** %r, align 8
  store i32 100, i32* %"*r", align 4
  %"$hp" = load i32, i32* %hp, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$hp")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$hp")
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %1 = insertvalue %String %0, i32 %"$format.size5", 1
  %2 = insertvalue %String %1, i32 %ADD3, 2
  call void @"io::out"(%String %2)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

---

## 031 — dungeon: floor14a

```ura
// stdout: hi
// stdout: length: 7
// expect: 0
// dungeon/floor14a.ura
use "@/core"
proto strlen(s i8[]) i64
proto write(fd i32, ptr i8[], len i64) i64
proto printf(fmt i8[], ...) i32

fn main() i32:
    n i32 = strlen("dungeon") as i32
    write(1, "hi\n", 3)
    printf("length: %d\n", n)
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [8 x i8] c"dungeon\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"hi\0A\00", align 1
@str.2 = private unnamed_addr constant [12 x i8] c"length: %d\0A\00", align 1

declare i64 @write(i32, i8*, i64)

declare i64 @strlen(i8*)

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %n = alloca i32, align 4
  %call.strlen = call i64 @strlen(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str, i32 0, i32 0))
  %AS = trunc i64 %call.strlen to i32
  store i32 %AS, i32* %n, align 4
  %call.write = call i64 @write(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i64 3)
  %"$n" = load i32, i32* %n, align 4
  %call.printf = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.2, i32 0, i32 0), i32 %"$n")
  ret i32 0
}
```

---

## 032 — dungeon: boss

```ura
// stdout: == Dungeon ==
// stdout: Party:
// stdout:   Aldric atk=18
// stdout:   Borin atk=25
// stdout:   Cass atk=22
// stdout: Champion: Borin (alive=True)
// stdout: Foe hp=20 loot=none
// stdout: Foe hp=45 loot=none
// stdout: Foe hp=120 loot=Dragonfang
// stdout: Trap! Aldric hp=90
// stdout: sizeof=24 typeof=Hero
// expect: 0
use "@/core"

enum EnemyType: GOBLIN, ORC, DRAGON

struct Stats:
    hp  i32
    atk i32

struct Hero:
    name  String
    stats Stats

    pub fn create(name String, hp i32, atk i32) Hero:
        h Hero
        h.name      = name
        h.stats.hp  = hp
        h.stats.atk = atk
        return h

    fn alive() bool:
        return self.stats.hp > 0

operator =(left &Hero, right &Hero):
    left.name = right.name
    left.stats = right.stats

operator $(hero &Hero) String:
    return hero.name

fn enemy_hp(kind EnemyType) i32:
    match kind:
        case EnemyType::GOBLIN: return 20
        case EnemyType::ORC:    return 45
        case EnemyType::DRAGON: return 120
    return 0

fn loot_for(kind EnemyType) String:
    if kind == EnemyType::DRAGON:
        return "Dragonfang"
    return "none"

fn strongest(party &Array<Hero>) i32:
    best i32 = 0
    for i in Range::create(1, party.len()):
        if party[i].stats.atk > party[best].stats.atk:
            best = i
    return best

fn main() i32:
    io::out("== Dungeon ==\n")
    party Array<Hero>
    party.append(Hero::create("Aldric", 100, 18))
    party.append(Hero::create("Borin", 90, 25))
    party.append(Hero::create("Cass", 80, 22))

    io::out("Party:\n")
    for h in party:
        io::out($"  {h} atk={h.stats.atk}\n")

    champ Hero = party[strongest(&party)]
    io::out($"Champion: {champ} (alive={champ.alive()})\n")

    foes Array<EnemyType> = [EnemyType::GOBLIN, EnemyType::ORC, EnemyType::DRAGON]
    for kind in foes:
        io::out($"Foe hp={enemy_hp(kind)} loot={loot_for(kind)}\n")

    for i in Range::create(0, party.len()):
        party[i].stats.hp -= 10
    io::out($"Trap! {party[0]} hp={party[0].stats.hp}\n")
    io::out($"sizeof={sizeof(champ)} typeof={typeof(champ)}\n")
    return 0
```

---

## 034 — libft: isalnum

```ura
// stdout: is alnum
// expect: 0
// libft/isalnum.ura
use "@/core"
fn isalpha(c char) bool: return c >= 'a' and c <= 'z'
fn isdigit(c char) bool: return c >= '1' and c <= '9'
fn isalnum(c char) bool: return isalpha(c) or isdigit(c)

fn main() i32:
    if isalnum('1'):
        io::out("is alnum\n")
    else:
        io::out("is not alnum\n")
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

@str = private unnamed_addr constant [10 x i8] c"is alnum\0A\00", align 1
@str.1 = private unnamed_addr constant [14 x i8] c"is not alnum\0A\00", align 1

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

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
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
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i1 @isalpha(i8 %0) {
entry:
  %c = alloca i8, align 1
  store i8 %0, i8* %c, align 1
  %"$c" = load i8, i8* %c, align 1
  %GE = icmp sge i8 %"$c", 97
  %"$c1" = load i8, i8* %c, align 1
  %LE = icmp sle i8 %"$c1", 122
  %AND = and i1 %GE, %LE
  ret i1 %AND
}

define i1 @isdigit(i8 %0) {
entry:
  %c = alloca i8, align 1
  store i8 %0, i8* %c, align 1
  %"$c" = load i8, i8* %c, align 1
  %GE = icmp sge i8 %"$c", 49
  %"$c1" = load i8, i8* %c, align 1
  %LE = icmp sle i8 %"$c1", 57
  %AND = and i1 %GE, %LE
  ret i1 %AND
}

define i1 @isalnum(i8 %0) {
entry:
  %c = alloca i8, align 1
  store i8 %0, i8* %c, align 1
  %"$c" = load i8, i8* %c, align 1
  %call.isalpha = call i1 @isalpha(i8 %"$c")
  %"$c1" = load i8, i8* %c, align 1
  %call.isdigit = call i1 @isdigit(i8 %"$c1")
  %OR = or i1 %call.isalpha, %call.isdigit
  ret i1 %OR
}

define i32 @main() {
entry:
  br label %if6

if6:                                              ; preds = %entry
  %call.isalnum = call i1 @isalnum(i8 49)
  br i1 %call.isalnum, label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  call void @"io::out"(%String { i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 9, i32 0 })
  br label %endif

else7:                                            ; preds = %if6
  call void @"io::out"(%String { i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.1, i32 0, i32 0), i32 13, i32 0 })
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)
```

---

## 035 — libft: isalpha

```ura
// stdout: is not alpha
// expect: 0
// libft/isalpha.ura
use "@/core"
fn isalpha(c char) bool: return c >= 'a' and c <= 'z'

fn main() i32:
    if isalpha('1'):
        io::out("is alpha\n")
    else:
        io::out("is not alpha\n")
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

@str = private unnamed_addr constant [10 x i8] c"is alpha\0A\00", align 1
@str.1 = private unnamed_addr constant [14 x i8] c"is not alpha\0A\00", align 1

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

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
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
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i1 @isalpha(i8 %0) {
entry:
  %c = alloca i8, align 1
  store i8 %0, i8* %c, align 1
  %"$c" = load i8, i8* %c, align 1
  %GE = icmp sge i8 %"$c", 97
  %"$c1" = load i8, i8* %c, align 1
  %LE = icmp sle i8 %"$c1", 122
  %AND = and i1 %GE, %LE
  ret i1 %AND
}

define i32 @main() {
entry:
  br label %if6

if6:                                              ; preds = %entry
  %call.isalpha = call i1 @isalpha(i8 49)
  br i1 %call.isalpha, label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  call void @"io::out"(%String { i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 9, i32 0 })
  br label %endif

else7:                                            ; preds = %if6
  call void @"io::out"(%String { i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.1, i32 0, i32 0), i32 13, i32 0 })
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)
```

---

## 036 — libft: isdigit

```ura
// stdout: is alnum
// expect: 0
// libft/isdigit.ura
use "@/core"
fn isdigit(c char) bool: return c >= '1' and c <= '9'

fn main() i32:
    if isdigit('1'):
        io::out("is alnum\n")
    else:
        io::out("is not alnum\n")
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

@str = private unnamed_addr constant [10 x i8] c"is alnum\0A\00", align 1
@str.1 = private unnamed_addr constant [14 x i8] c"is not alnum\0A\00", align 1

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

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
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
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i1 @isdigit(i8 %0) {
entry:
  %c = alloca i8, align 1
  store i8 %0, i8* %c, align 1
  %"$c" = load i8, i8* %c, align 1
  %GE = icmp sge i8 %"$c", 49
  %"$c1" = load i8, i8* %c, align 1
  %LE = icmp sle i8 %"$c1", 57
  %AND = and i1 %GE, %LE
  ret i1 %AND
}

define i32 @main() {
entry:
  br label %if6

if6:                                              ; preds = %entry
  %call.isdigit = call i1 @isdigit(i8 49)
  br i1 %call.isdigit, label %if6.then, label %else7

if6.then:                                         ; preds = %if6
  call void @"io::out"(%String { i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 9, i32 0 })
  br label %endif

else7:                                            ; preds = %if6
  call void @"io::out"(%String { i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.1, i32 0, i32 0), i32 13, i32 0 })
  br label %endif

endif:                                            ; preds = %else7, %if6.then
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)
```

---

## 037 — libft: lstnew — new nodes, add_back, add_front and size

```ura
// stdout: z a b
// stdout: size=3
// expect: 0
use "@/core"
struct Node:
    content char[]
    next &Node

struct Lst:
    head &Node

fn ft_lstnew(content char[]) &Node:
    n Node[] = new Node[1]
    n[0].content = content
    return &n[0]

fn ft_lstadd_front(lst &Lst, node &Node):
    node.next = &lst.head
    lst.head  = &node

fn ft_lstadd_back(lst &Lst, node &Node):
    if lst.head == null:
        lst.head = &node
        return
    cur &Node = &lst.head
    while cur.next != null:
        cur = &cur.next
    cur.next = &node

fn ft_lstsize(lst &Lst) i32:
    cur &Node = &lst.head
    count i32 = 0
    while cur != null:
        count += 1
        cur = &cur.next
    return count

fn main() i32:
    lst Lst
    ft_lstadd_back(&lst, ft_lstnew("a"))
    ft_lstadd_back(&lst, ft_lstnew("b"))
    ft_lstadd_front(&lst, ft_lstnew("z"))
    cur &Node = &lst.head
    while cur != null:
        io::out($"{cur.content} ")
        cur = &cur.next
    io::out("\n")
    io::out($"size={ft_lstsize(&lst)}\n")
    return 0
```

---

## 038 — libft: lstlast — returns a reference to the last node

```ura
// stdout: c
// expect: 0
use "@/core"
struct Node:
    content char[]
    next &Node

struct Lst:
    head &Node

fn ft_lstnew(content char[]) &Node:
    n Node[] = new Node[1]
    n[0].content = content
    return &n[0]

fn ft_lstadd_back(lst &Lst, node &Node):
    if lst.head == null:
        lst.head = &node
        return
    cur &Node = &lst.head
    while cur.next != null:
        cur = &cur.next
    cur.next = &node

fn ft_lstlast(lst &Lst) &Node:
    cur &Node = &lst.head
    if cur == null:
        return null
    while cur.next != null:
        cur = &cur.next
    return &cur

fn main() i32:
    lst Lst
    ft_lstadd_back(&lst, ft_lstnew("a"))
    ft_lstadd_back(&lst, ft_lstnew("b"))
    ft_lstadd_back(&lst, ft_lstnew("c"))
    L &Node = ft_lstlast(&lst)
    io::out($"{L.content}\n")
    return 0
```

---

## 039 — libft: lstiter — apply a function to each node's content in place

```ura
// stdout: AB CD
// expect: 0
use "@/core"
struct Node:
    content char[]
    next &Node

struct Lst:
    head &Node

fn ft_lstnew(content char[]) &Node:
    n Node[] = new Node[1]
    n[0].content = content
    return &n[0]

fn ft_lstadd_back(lst &Lst, node &Node):
    if lst.head == null:
        lst.head = &node
        return
    cur &Node = &lst.head
    while cur.next != null:
        cur = &cur.next
    cur.next = &node

fn upcase(s char[]) char[]:
    n i32 = s.len as i32
    out char[] = new char[n]
    i i32 = 0
    while i < n:
        c char = s[i]
        if c >= 'a' and c <= 'z':
            out[i] = (c as i32 - 32) as char
        else:
            out[i] = c
        i += 1
    return out

fn ft_lstiter(lst &Lst, f fn(char[]) char[]) void:
    cur &Node = &lst.head
    while cur != null:
        cur.content = f(cur.content)
        cur = &cur.next

fn main() i32:
    lst Lst
    ft_lstadd_back(&lst, ft_lstnew("ab"))
    ft_lstadd_back(&lst, ft_lstnew("cd"))
    ft_lstiter(&lst, upcase)
    cur &Node = &lst.head
    while cur != null:
        io::out($"{cur.content} ")
        cur = &cur.next
    io::out("\n")
    return 0
```

---

## 040 — libft: lstdelone — unlink a node by index and clear the list

```ura
// stdout: a c
// stdout: c
// stdout: size=0
// expect: 0
use "@/core"
struct Node:
    content char[]
    next &Node

struct Lst:
    head &Node

fn ft_lstnew(content char[]) &Node:
    n Node[] = new Node[1]
    n[0].content = content
    return &n[0]

fn ft_lstadd_back(lst &Lst, node &Node):
    if lst.head == null:
        lst.head = &node
        return
    cur &Node = &lst.head
    while cur.next != null:
        cur = &cur.next
    cur.next = &node

fn ft_lstsize(lst &Lst) i32:
    cur &Node = &lst.head
    count i32 = 0
    while cur != null:
        count += 1
        cur = &cur.next
    return count

fn ft_lstdelone(lst &Lst, idx i32):
    if lst.head == null:
        return
    if idx <= 0:
        lst.head = &lst.head.next
        return
    prev &Node = &lst.head
    i i32 = 1
    while i < idx and prev.next != null:
        prev = &prev.next
        i += 1
    if prev.next != null:
        prev.next = &prev.next.next

fn ft_lstclear(lst &Lst):
    lst.head = null

fn show(lst &Lst):
    cur &Node = &lst.head
    while cur != null:
        io::out($"{cur.content} ")
        cur = &cur.next
    io::out("\n")

fn main() i32:
    lst Lst
    ft_lstadd_back(&lst, ft_lstnew("a"))
    ft_lstadd_back(&lst, ft_lstnew("b"))
    ft_lstadd_back(&lst, ft_lstnew("c"))
    ft_lstdelone(&lst, 1)
    show(&lst)
    ft_lstdelone(&lst, 0)
    show(&lst)
    ft_lstclear(&lst)
    io::out($"size={ft_lstsize(&lst)}\n")
    return 0
```

---

## 041 — link: an SDL2 window drawn from ura

```ura
// stdout: window closed after 181 frames
// expect: 0
// opens a 400x300 SDL2 window with a color gradient for 3 seconds
use "@/core"
link "SDL2"
proto SDL_Init(flags i32) i32
proto SDL_CreateWindow(title i8[], x i32, y i32, w i32, h i32, flags i32) i8[]
proto SDL_CreateRenderer(window i8[], index i32, flags i32) i8[]
proto SDL_SetRenderDrawColor(renderer i8[], r i8, g i8, b i8, a i8) i32
proto SDL_RenderDrawPoint(renderer i8[], x i32, y i32) i32
proto SDL_RenderPresent(renderer i8[])
proto SDL_PollEvent(event &Event) i32
proto SDL_Delay(ms i32)
proto SDL_Quit()

struct Event:
    type i32
    a1 i32
    a2 i32
    a3 i32
    a4 i32
    a5 i32
    a6 i32
    a7 i32
    a8 i32
    a9 i32
    a10 i32
    a11 i32
    a12 i32
    a13 i32

fn main() i32:
    width i32 = 400
    height i32 = 300
    SDL_Init(32)
    window i8[] = SDL_CreateWindow("ura + SDL2", 805240832, 805240832, width, height, 0)
    renderer i8[] = SDL_CreateRenderer(window, -1, 0)

    y i32 = 0
    while y < height:
        x i32 = 0
        while x < width:
            red i8 = (x * 255 / width) as i8
            green i8 = (y * 255 / height) as i8
            SDL_SetRenderDrawColor(renderer, red, green, 128 as i8, 255 as i8)
            SDL_RenderDrawPoint(renderer, x, y)
            x += 1
        y += 1
    SDL_RenderPresent(renderer)

    event Event
    running bool = True
    frames i32 = 0
    while running:
        while SDL_PollEvent(&event) != 0:
            if event.type == 256:
                running = False
        SDL_Delay(16)
        frames += 1
        if frames > 180:
            running = False
    SDL_Quit()
    io::out($"window closed after {frames} frames\n")
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
%Event = type { i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32, i32 }

@str = private unnamed_addr constant [11 x i8] c"ura + SDL2\00", align 1
@str.1 = private unnamed_addr constant [31 x i8] c"window closed after %d frames\0A\00", align 1

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

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
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
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

declare i32 @SDL_Init(i32)

declare i8* @SDL_CreateWindow(i8*, i32, i32, i32, i32, i32)

declare i8* @SDL_CreateRenderer(i8*, i32, i32)

declare i32 @SDL_SetRenderDrawColor(i8*, i8, i8, i8, i8)

declare i32 @SDL_RenderDrawPoint(i8*, i32, i32)

declare void @SDL_RenderPresent(i8*)

declare i32 @SDL_PollEvent(%Event*)

declare void @SDL_Delay(i32)

declare void @SDL_Quit()

define i32 @main() {
entry:
  %width = alloca i32, align 4
  %height = alloca i32, align 4
  %window = alloca i8*, align 8
  %renderer = alloca i8*, align 8
  %y = alloca i32, align 4
  %x = alloca i32, align 4
  %red = alloca i8, align 1
  %green = alloca i8, align 1
  %event = alloca %Event, align 8
  %running = alloca i1, align 1
  %frames = alloca i32, align 4
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store i32 400, i32* %width, align 4
  store i32 300, i32* %height, align 4
  %call.SDL_Init = call i32 @SDL_Init(i32 32)
  %"$width" = load i32, i32* %width, align 4
  %"$height" = load i32, i32* %height, align 4
  %call.SDL_CreateWindow = call i8* @SDL_CreateWindow(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str, i32 0, i32 0), i32 805240832, i32 805240832, i32 %"$width", i32 %"$height", i32 0)
  store i8* %call.SDL_CreateWindow, i8** %window, align 8
  %"$window" = load i8*, i8** %window, align 8
  %call.SDL_CreateRenderer = call i8* @SDL_CreateRenderer(i8* %"$window", i32 -1, i32 0)
  store i8* %call.SDL_CreateRenderer, i8** %renderer, align 8
  store i32 0, i32* %y, align 4
  br label %while6

while6:                                           ; preds = %endloop, %entry
  %"$y" = load i32, i32* %y, align 4
  %"$height5" = load i32, i32* %height, align 4
  %LT = icmp slt i32 %"$y", %"$height5"
  br i1 %LT, label %while6.then, label %endloop1

while6.then:                                      ; preds = %while6
  store i32 0, i32* %x, align 4
  br label %while7

while7:                                           ; preds = %while7.then, %while6.then
  %"$x" = load i32, i32* %x, align 4
  %"$width6" = load i32, i32* %width, align 4
  %LT7 = icmp slt i32 %"$x", %"$width6"
  br i1 %LT7, label %while7.then, label %endloop

while7.then:                                      ; preds = %while7
  %"$x8" = load i32, i32* %x, align 4
  %MUL = mul i32 %"$x8", 255
  %"$width9" = load i32, i32* %width, align 4
  %DIV = sdiv i32 %MUL, %"$width9"
  %AS = trunc i32 %DIV to i8
  store i8 %AS, i8* %red, align 1
  %"$y10" = load i32, i32* %y, align 4
  %MUL11 = mul i32 %"$y10", 255
  %"$height12" = load i32, i32* %height, align 4
  %DIV13 = sdiv i32 %MUL11, %"$height12"
  %AS14 = trunc i32 %DIV13 to i8
  store i8 %AS14, i8* %green, align 1
  %"$renderer" = load i8*, i8** %renderer, align 8
  %"$red" = load i8, i8* %red, align 1
  %"$green" = load i8, i8* %green, align 1
  %call.SDL_SetRenderDrawColor = call i32 @SDL_SetRenderDrawColor(i8* %"$renderer", i8 %"$red", i8 %"$green", i8 -128, i8 -1)
  %"$renderer15" = load i8*, i8** %renderer, align 8
  %"$x16" = load i32, i32* %x, align 4
  %"$y17" = load i32, i32* %y, align 4
  %call.SDL_RenderDrawPoint = call i32 @SDL_RenderDrawPoint(i8* %"$renderer15", i32 %"$x16", i32 %"$y17")
  %"$x18" = load i32, i32* %x, align 4
  %ADD = add i32 %"$x18", 1
  store i32 %ADD, i32* %x, align 4
  br label %while7

endloop:                                          ; preds = %while7
  %"$y19" = load i32, i32* %y, align 4
  %ADD20 = add i32 %"$y19", 1
  store i32 %ADD20, i32* %y, align 4
  br label %while6

endloop1:                                         ; preds = %while6
  %"$renderer21" = load i8*, i8** %renderer, align 8
  call void @SDL_RenderPresent(i8* %"$renderer21")
  store %Event zeroinitializer, %Event* %event, align 4
  store i1 true, i1* %running, align 1
  store i32 0, i32* %frames, align 4
  br label %while8

while8:                                           ; preds = %endif3, %endloop1
  %"$running" = load i1, i1* %running, align 1
  br i1 %"$running", label %while8.then, label %endloop4

while8.then:                                      ; preds = %while8
  br label %while9

while9:                                           ; preds = %endif, %while8.then
  %call.SDL_PollEvent = call i32 @SDL_PollEvent(%Event* %event)
  %NQ = icmp ne i32 %call.SDL_PollEvent, 0
  br i1 %NQ, label %while9.then, label %endloop2

while9.then:                                      ; preds = %while9
  br label %if10

if10:                                             ; preds = %while9.then
  %event.type = getelementptr inbounds %Event, %Event* %event, i32 0, i32 0
  %"$event.type" = load i32, i32* %event.type, align 4
  %EQ = icmp eq i32 %"$event.type", 256
  br i1 %EQ, label %if10.then, label %endif

if10.then:                                        ; preds = %if10
  store i1 false, i1* %running, align 1
  br label %endif

endif:                                            ; preds = %if10.then, %if10
  br label %while9

endloop2:                                         ; preds = %while9
  call void @SDL_Delay(i32 16)
  %"$frames" = load i32, i32* %frames, align 4
  %ADD22 = add i32 %"$frames", 1
  store i32 %ADD22, i32* %frames, align 4
  br label %if11

if11:                                             ; preds = %endloop2
  %"$frames23" = load i32, i32* %frames, align 4
  %GT = icmp sgt i32 %"$frames23", 180
  br i1 %GT, label %if11.then, label %endif3

if11.then:                                        ; preds = %if11
  store i1 false, i1* %running, align 1
  br label %endif3

endif3:                                           ; preds = %if11.then, %if11
  br label %while8

endloop4:                                         ; preds = %while8
  call void @SDL_Quit()
  %"$frames24" = load i32, i32* %frames, align 4
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([31 x i8], [31 x i8]* @str.1, i32 0, i32 0), i32 %"$frames24")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD25 = add i32 %"$format.size", 1
  %AS26 = sext i32 %ADD25 to i64
  %call.calloc = call i8* @calloc(i64 %AS26, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf27 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS26, i8* getelementptr inbounds ([31 x i8], [31 x i8]* @str.1, i32 0, i32 0), i32 %"$frames24")
  %"$format.size28" = load i32, i32* %format.size, align 4
  %ADD29 = add i32 %"$format.size28", 1
  %"$format.text30" = load i8*, i8** %format.text, align 8
  %"$format.size31" = load i32, i32* %format.size, align 4
  %0 = insertvalue %String zeroinitializer, i8* %"$format.text30", 0
  %1 = insertvalue %String %0, i32 %"$format.size31", 1
  %2 = insertvalue %String %1, i32 %ADD29, 2
  call void @"io::out"(%String %2)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

---

## 042 — link: a library that is not installed

```ura
// error: library 'NoSuchLib' not found
use "@/core"
link "NoSuchLib"
fn main() i32:
    return 0
```

### stderr

```
error: test.ura:3:1 library 'NoSuchLib' not found
  |
3 | link "NoSuchLib"
  | ^^^^
help: install it, or set URA_LIBC to the folder that holds it
error: aborting due to 1 error
```

### status

```
1
```

---

## 043 — macros: @if / @elif / @else on os and installed()

```ura
use "@/core"
@if os == "linux" or os == "macos":
    name i8[] = "unix"
@elif os == "windows":
    name i8[] = "windows"
@else:
    @error "unsupported platform"

fn main() i32:
    @if installed("SDL2") and not installed("raylib"):
        io::out($"{name} sdl only\n")
    @else:
        io::out("other\n")
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

@name = global i8* null
@str = private unnamed_addr constant [13 x i8] c"%s sdl only\0A\00", align 1

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

define i32 @String.len(%String* %0) {
entry:
  %self = alloca %String*, align 8
  store %String* %0, %String** %self, align 8
  %"*self" = load %String*, %String** %self, align 8
  %"*self.count" = getelementptr inbounds %String, %String* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
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
  %1 = bitcast %String* %text to %"Array<i8>"*
  call void @"Array<i8>.drop"(%"Array<i8>"* %1)
  ret void
}

define i32 @main() {
entry:
  %arr = alloca [5 x i8], align 1
  %format.size = alloca i32, align 4
  %format.text = alloca i8*, align 8
  store [5 x i8] c"unix\00", [5 x i8]* %arr, align 1
  %0 = getelementptr inbounds [5 x i8], [5 x i8]* %arr, i32 0, i32 0
  store i8* %0, i8** @name, align 8
  %"$name" = load i8*, i8** @name, align 8
  %call.snprintf = call i32 (i8*, i64, i8*, ...) @snprintf(i8* null, i64 0, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str, i32 0, i32 0), i8* %"$name")
  store i32 %call.snprintf, i32* %format.size, align 4
  %"$format.size" = load i32, i32* %format.size, align 4
  %ADD = add i32 %"$format.size", 1
  %AS = sext i32 %ADD to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  store i8* %call.calloc, i8** %format.text, align 8
  %"$format.text" = load i8*, i8** %format.text, align 8
  %call.snprintf1 = call i32 (i8*, i64, i8*, ...) @snprintf(i8* %"$format.text", i64 %AS, i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str, i32 0, i32 0), i8* %"$name")
  %"$format.size2" = load i32, i32* %format.size, align 4
  %ADD3 = add i32 %"$format.size2", 1
  %"$format.text4" = load i8*, i8** %format.text, align 8
  %"$format.size5" = load i32, i32* %format.size, align 4
  %1 = insertvalue %String zeroinitializer, i8* %"$format.text4", 0
  %2 = insertvalue %String %1, i32 %"$format.size5", 1
  %3 = insertvalue %String %2, i32 %ADD3, 2
  call void @"io::out"(%String %3)
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @snprintf(i8*, i64, i8*, ...)
```

---

## 044 — macros: @error stops the build with a message and a help

```ura
use "@/core"
@if not installed("nothing_like_this"):
    @error "nothing_like_this is not installed" "linux: sudo apt install libnothing-dev"

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:3:5 nothing_like_this is not installed
  |
3 |     @error "nothing_like_this is not installed" "linux: sudo apt install libnothing-dev"
  |     ^^^^^^
help: linux: sudo apt install libnothing-dev
error: aborting due to 1 error
```

### status

```
1
```

---

## 045 — macros: @if on something it can't check

```ura
use "@/core"
@if version == 2:
    x i32 = 1

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:2:13 @if checks os, arch and installed("lib")
  |
2 | @if version == 2:
  |             ^^
help: use @if os == "linux" and installed("SDL2"):
error: aborting due to 1 error
```

### status

```
1
```

