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
- 033 — project: calculator — tokenize, parse, print the AST and evaluate with try/catch
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
target triple = "x86_64-pc-linux-gnu"

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
target triple = "x86_64-pc-linux-gnu"

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
target triple = "x86_64-pc-linux-gnu"

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
target triple = "x86_64-pc-linux-gnu"

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
target triple = "x86_64-pc-linux-gnu"

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
target triple = "x86_64-pc-linux-gnu"

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
target triple = "x86_64-pc-linux-gnu"

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

fn twice(n i32) i32:
    return n + n

fn apply(f fn(i32) i32, x i32) i32:
    return f(x)

fn main() i32:
    return apply(5, 21)
```

### stderr

```
error: test.ura:6:10 parameter 'f' has no type
  |
6 | fn apply(f fn(i32) i32, x i32) i32:
  |          ^
help: like: f i32
error: test.ura:6:12 expected ',' between parameters
  |
6 | fn apply(f fn(i32) i32, x i32) i32:
  |            ^^
error: test.ura:6:12 expected ')' after the parameters
  |
6 | fn apply(f fn(i32) i32, x i32) i32:
  |            ^^
help: use fn apply(f) fn(i32) i32, x i32) i32:
error: test.ura:6:12 expected ':' after the function header
  |
6 | fn apply(f fn(i32) i32, x i32) i32:
  |            ^^
help: use fn apply(f: fn(i32) i32, x i32) i32:
error: test.ura:6:15 expected ')' after the parameters
  |
6 | fn apply(f fn(i32) i32, x i32) i32:
  |               ^^^
help: use fn apply(f fn()i32) i32, x i32) i32:
error: test.ura:6:18 expected ':' after the function header
  |
6 | fn apply(f fn(i32) i32, x i32) i32:
  |                  ^
help: use fn apply(f fn(i32:) i32, x i32) i32:
error: test.ura:6:18 unexpected ')'
  |
6 | fn apply(f fn(i32) i32, x i32) i32:
  |                  ^
error: test.ura:6:23 unexpected ','
  |
6 | fn apply(f fn(i32) i32, x i32) i32:
  |                       ^
error: test.ura:6:30 unexpected ')'
  |
6 | fn apply(f fn(i32) i32, x i32) i32:
  |                              ^
error: test.ura:6:35 unexpected ':'
  |
6 | fn apply(f fn(i32) i32, x i32) i32:
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
target triple = "x86_64-pc-linux-gnu"

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

proto scan(a i32, b i8[]) i32

proto scan(a i32, b i8[], c i32) i32

fn main() i32:
    io::out("unreachable\n")
    return 0
```

### stderr

```
error: test.ura:5:7 'scan' is already declared with other parameters, on line 3
  |
5 | proto scan(a i32, b i8[], c i32) i32
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

fn main() i32:
    f fn(i32) i32
    return f(21)
```

---

## 020 — function: infinite recursion overflows the stack

```ura
// expect: 139

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
target triple = "x86_64-pc-linux-gnu"

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
// expect: 0
// stdin and output depend on the program; run it by hand
use "@/header"

enum Type: NONE, NUMBER, ADD, SUB, MUL, DIV, LPAREN, RPAREN

struct Token:
    value i32
    type Type

    pub fn number(v i32) &Token:
        t &Token = new Token
        t.type = NUMBER
        t.value = v
        ret &t

    pub fn op(k Type) &Token:
        t &Token = new Token
        t.type = k
        ret &t

struct Node:
    token &Token
    left &Node
    right &Node

    pub fn create(token &Token) &Node:
        n &Node = new Node
        n.token = &token
        ret &n

    @no-warn
    operator drop():
        clean self.token
        clean self.left
        clean self.right

mod Lexer:
    fn run(src &String, toks Token[]) i32:
        fn is_digit(c char) bool: ret c >= '0' and c <= '9'
        n i32 = 0
        i i32 = 0
        while i < src.len():
            c char = src.at(i)
            if c == ' ':
                i = i + 1
                continue
            elif is_digit(c):
                v i32 = 0
                while i < src.len() and is_digit(src.at(i)):
                    v = v * 10 + ((src.at(i) as i32) - 48)
                    i = i + 1
                toks[n].type = NUMBER
                toks[n].value = v
                n = n + 1
                continue
            else:
                k Type = NUMBER
                match c:
                    case '+': k = ADD
                    case '-': k = SUB
                    case '*': k = MUL
                    case '/': k = DIV
                    case '(': k = LPAREN
                    case ')': k = RPAREN
                toks[n].type = k
                toks[n].value = 0
                n = n + 1
                i = i + 1
        ret n

mod Parser:
    fn factor(toks Token[], p &i32, n i32) &Node:
        if p < n and toks[p].type == LPAREN:
            p = p + 1
            inner &Node = expr(toks, &p, n)
            if p < n and toks[p].type == RPAREN:
                p = p + 1
            ret &inner
        lf &Node = Node::create(Token::number(toks[p].value))
        p = p + 1
        ret &lf

    fn term(toks Token[], p &i32, n i32) &Node:
        l &Node = factor(toks, &p, n)
        while p < n and (toks[p].type == MUL or toks[p].type == DIV):
            k Type = toks[p].type
            p = p + 1
            r &Node = factor(toks, &p, n)
            parent &Node = Node::create(Token::op(k))
            parent.left = &l
            parent.right = &r
            l = &parent
        ret &l

    fn expr(toks Token[], p &i32, n i32) &Node:
        l &Node = term(toks, &p, n)
        while p < n and (toks[p].type == ADD or toks[p].type == SUB):
            k Type = toks[p].type
            p = p + 1
            r &Node = term(toks, &p, n)
            parent &Node = Node::create(Token::op(k))
            parent.left = &l
            parent.right = &r
            l = &parent
        ret &l

fn sign(k Type) char[]:
    match k:
        case ADD: ret "+"
        case SUB: ret "-"
        case MUL: ret "*"
        case DIV: ret "/"
        default: ret "?"

fn label(n &Node):
    if n.token.type == NUMBER: io::out($"{n.token.value}\n")
    else:                      io::out($"op: {sign(n.token.type)}\n")

fn show(n &Node, pre char[], last bool):
    io::out($"{pre}")
    if last: io::out("└─ ")
    else:    io::out("├─ ")
    label(&n)
    kid String = String::create(pre)
    if last: kid.join("   ")
    else:    kid.join("│  ")
    if n.left != null:  show(&n.left, kid.chars(), n.right == null)
    if n.right != null: show(&n.right, kid.chars(), True)

fn tree(n &Node):
    label(&n)
    if n.left != null:  show(&n.left, "", n.right == null)
    if n.right != null: show(&n.right, "", True)

fn eval(n &Node) i32:
    match n.token.type:
        case NUMBER: ret n.token.value
        case ADD:    ret eval(&n.left) + eval(&n.right)
        case SUB:    ret eval(&n.left) - eval(&n.right)
        case MUL:    ret eval(&n.left) * eval(&n.right)
        case DIV:
            d i32 = eval(&n.right)
            if d == 0: throw Error::create(String::create("division by zero"))
            ret eval(&n.left) / d
        default:    ret 0

fn calc(text char[]):
    io::out("========================================\n")
    io::out($"calculate expr:  {text}\nast:\n")
    src String = String::create(text)
    toks Token[] = new Token[64]
    n i32 = Lexer::run(&src, toks)
    p i32 = 0
    root &Node = Parser::expr(toks, &p, n)
    tree(&root)
    try:
        io::out($"result: {eval(&root)}\n")
    catch e:
        io::err($"\nError: {e.message}\n")
    clean root
    clean toks

fn main() i32:
    calc("2 * (3 + 4) - 10 / 5")
    calc("8 / (3 - 3)")
    io::out("========================================\n")
    return 0
```

---

## 026 — project: libft (library and its main)

```ura
// expect: 0
// ---- libft.ura
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
// expect: 0
// net_server.ura — a TCP chat server (macOS), single-process event loop.
//
// No fork, no threads: one `poll()` loop owns every fd — stdin, the listener,
// and all connected clients — and handles whichever is ready. A line typed at
// the server is broadcast to every client; a line from a client is printed.
//
// The poll set is a `PollFd[]`: slot 0 is stdin, slot 1 is the listener, and
// slots 2.. are clients. Because the loop owns the client fds and closes them
// deliberately on disconnect, they are tracked as raw fds here (a Socket with
// `operator drop` would close a fd when a copy of it went out of scope).

AF_INET      i32 = 2
SOCK_STREAM  i32 = 1
SOL_SOCKET   i32 = 65535
SO_REUSEADDR i32 = 4
SO_REUSEPORT i32 = 512
PORT         i32 = 8080
POLLIN       i16 = 1
MAX          i32 = 64

// macOS sockaddr_in, 16 bytes: len(1) family(1) port(2,BE) ip(4) zero(8)
struct SockAddr:
    len    char
    family char
    port   i16
    ip     i32
    zero   i64

    pub fn create(port i32, ip i32) SockAddr:
        a SockAddr
        a.len    = 16 as char
        a.family = AF_INET as char
        a.port   = (((port & 255) << 8) | (port >> 8)) as i16
        a.ip     = ip
        a.zero   = 0
        return a

// C: struct pollfd { int fd; short events; short revents; }  (8 bytes)
struct PollFd:
    fd      i32
    events  i16
    revents i16

    fn watch(fd i32):
        self.fd      = fd
        self.events  = POLLIN
        self.revents = 0

    fn has_input() bool:
        return (self.revents & POLLIN) != 0

proto socket(domain i32, type i32, protocol i32) i32
proto bind(fd i32, addr &SockAddr, len i32) i32
proto listen(fd i32, backlog i32) i32
proto accept(fd i32, addr i8[], len i8[]) i32
proto setsockopt(fd i32, level i32, opt i32, val i8[], len i32) i32
proto close(fd i32) i32
proto read(fd i32, buf i8[], n i64) i64
proto poll(fds &PollFd, nfds i32, timeout i32) i32

// Unbuffered terminal write, so prompts appear before we block on poll.
fn log(msg char[]):
    write(1, msg, strlen(msg))

struct Server:
    fd i32

    pub fn create() Server:
        s Server
        s.fd = socket(AF_INET, SOCK_STREAM, 0)
        return s

    fn valid() bool:
        return self.fd >= 0

    fn reuse():
        opt char[] = new char[4]
        opt[0] = 1 as char
        setsockopt(self.fd, SOL_SOCKET, SO_REUSEADDR, opt, 4)
        setsockopt(self.fd, SOL_SOCKET, SO_REUSEPORT, opt, 4)
        clean opt

    fn bind_listen(port i32) bool:
        addr SockAddr = SockAddr::create(port, 0)
        if bind(self.fd, &addr, 16) < 0:
            return False
        listen(self.fd, 5)
        return True

    // raw fd, not a Socket: the poll loop owns and closes it explicitly
    fn accept_raw() i32:
        none i8[] = null
        return accept(self.fd, none, none)

    operator drop():
        if self.fd >= 0:
            close(self.fd)

fn main() i32:
    server Server = Server::create()
    if not server.valid():
        log("\033[0;31m[server] socket failed\033[0m\n")
        return 1

    server.reuse()
    if not server.bind_listen(PORT):
        log("\033[0;31m[server] bind failed\033[0m\n")
        return 1

    log("\033[0;32m[server]\033[0m listening on port 8080...\n")

    // poll set: [0] = stdin, [1] = listener, [2..nfds) = clients
    fds PollFd[] = new PollFd[MAX]
    fds[0].watch(0)
    fds[1].watch(server.fd)
    nfds i32 = 2

    buf char[] = new char[1024]
    r i32 = 0

    log("\033[0;32m[server]\033[0m > ")
    loop:
        poll(&fds[0], nfds, -1)

        // a line typed here -> broadcast to every client
        if fds[0].has_input():
            r = read(0, buf, 1023) as i32
            if r <= 0:
                break
            buf[r - 1] = 0 as char
            i i32 = 2
            while i < nfds:
                write(fds[i].fd, buf, strlen(buf))
                i += 1
            log("\033[0;32m[server]\033[0m > ")
        elif fds[0].revents != 0:
            // stdin is not a live terminal (redirected / closed): a negative
            // fd tells poll to skip it, so we keep serving clients instead of
            // spinning on POLLNVAL.
            fds[0].fd = -1

        // a new connection -> add it to the poll set
        if fds[1].has_input():
            cfd i32 = server.accept_raw()
            if cfd >= 0 and nfds < MAX:
                fds[nfds].watch(cfd)
                nfds += 1
                log("\r\033[2K\033[0;32m[server]\033[0m client connected\n")
                log("\033[0;32m[server]\033[0m > ")

        // client traffic -> print it; compact the set on disconnect
        j i32 = 2
        while j < nfds:
            if not fds[j].has_input():
                j += 1
                continue
            r = read(fds[j].fd, buf, 1023) as i32
            if r <= 0:
                close(fds[j].fd)
                nfds -= 1
                fds[j].fd      = fds[nfds].fd
                fds[j].events  = fds[nfds].events
                fds[j].revents = fds[nfds].revents
                log("\r\033[2K\033[0;31m[server]\033[0m client disconnected\n")
                log("\033[0;32m[server]\033[0m > ")
                continue
            buf[r] = 0 as char
            log("\r\033[2K\033[0;36m[client]\033[0m ")
            log(buf)
            log("\n\033[0;32m[server]\033[0m > ")
            j += 1

    clean fds
    clean buf
    return 0
```

---

## 028 — project: net client

```ura
// expect: 0
// net_client.ura — a TCP chat client (macOS), single-process event loop.
//
// No fork: one `poll()` loop watches both stdin and the socket, and handles
// whichever becomes ready. Structs used:
//   - SockAddr : the C sockaddr, passed to connect() by `ref`
//   - PollFd   : one entry of the poll set; poll() gets `ref fds[0]`
//   - Socket   : the connection, with `operator drop` (safe: one socket)
//
// write / strlen come from the auto-loaded common.ura; the socket calls are
// declared here so their address arguments can take a struct by `ref`.

AF_INET     i32 = 2
SOCK_STREAM i32 = 1
PORT        i32 = 8080
POLLIN      i16 = 1

// macOS sockaddr_in, 16 bytes: len(1) family(1) port(2,BE) ip(4) zero(8)
struct SockAddr:
    len    char
    family char
    port   i16
    ip     i32
    zero   i64

    pub fn create(port i32, ip i32) SockAddr:
        a SockAddr
        a.len    = 16 as char
        a.family = AF_INET as char
        a.port   = (((port & 255) << 8) | (port >> 8)) as i16
        a.ip     = ip
        a.zero   = 0
        return a

// C: struct pollfd { int fd; short events; short revents; }  (8 bytes)
struct PollFd:
    fd      i32
    events  i16
    revents i16

    fn watch(fd i32):
        self.fd      = fd
        self.events  = POLLIN
        self.revents = 0

    fn has_input() bool:
        return (self.revents & POLLIN) != 0

proto socket(domain i32, type i32, protocol i32) i32
proto connect(fd i32, addr &SockAddr, len i32) i32
proto close(fd i32) i32
proto read(fd i32, buf i8[], n i64) i64
proto poll(fds &PollFd, nfds i32, timeout i32) i32

// 127.0.0.1 -> the four octets in network order, packed into one i32.
fn ipv4(a i32, b i32, c i32, d i32) i32:
    return a | (b << 8) | (c << 16) | (d << 24)

// Unbuffered terminal write, so prompts appear before we block on poll.
fn log(msg char[]):
    write(1, msg, strlen(msg))

struct Socket:
    fd i32

    fn valid() bool:
        return self.fd >= 0

    fn send_text(msg char[]):
        write(self.fd, msg, strlen(msg))

    fn recv_into(buf char[], cap i32) i32:
        return read(self.fd, buf, cap as i64) as i32

    operator drop():
        if self.fd >= 0:
            close(self.fd)

fn main() i32:
    sock Socket
    sock.fd = socket(AF_INET, SOCK_STREAM, 0)
    if not sock.valid():
        log("\033[0;31m[client] socket failed\033[0m\n")
        return 1

    addr SockAddr = SockAddr::create(PORT, ipv4(127, 0, 0, 1))
    if connect(sock.fd, &addr, 16) < 0:
        log("\033[0;31m[client] connect failed\033[0m\n")
        return 1

    log("\033[0;36m[client]\033[0m connected to server on port 8080\n")

    // poll set: [0] = stdin, [1] = the socket
    fds PollFd[] = new PollFd[2]
    fds[0].watch(0)
    fds[1].watch(sock.fd)

    buf  char[] = new char[1024]
    rbuf char[] = new char[1024]
    r i32 = 0

    log("\033[0;36m[client]\033[0m > ")
    loop:
        poll(&fds[0], 2, -1)

        // a line typed at the terminal -> send it (drop the newline)
        if fds[0].has_input():
            r = read(0, buf, 1023) as i32
            if r <= 0:
                break
            buf[r - 1] = 0 as char
            sock.send_text(buf)
            log("\033[0;36m[client]\033[0m > ")
        elif fds[0].revents != 0:
            // stdin redirected / closed: skip it (negative fd) and keep
            // receiving from the server instead of spinning on POLLNVAL.
            fds[0].fd = -1

        // bytes from the server -> print them, then reprint the prompt
        if fds[1].has_input():
            r = sock.recv_into(rbuf, 1023)
            if r <= 0:
                log("\r\033[2K\033[0;31m[client]\033[0m server disconnected\n")
                break
            rbuf[r] = 0 as char
            log("\r\033[2K\033[0;33m[server]\033[0m ")
            log(rbuf)
            log("\n\033[0;36m[client]\033[0m > ")

    clean fds
    clean buf
    clean rbuf
    return 0
```

---

## 029 — dungeon: floor04

```ura
// stdout: Orc takes 17 damage — dead: True
// stdout: 4 squared: 16
// expect: 0
// dungeon/floor04.ura
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
target triple = "x86_64-pc-linux-gnu"

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
target triple = "x86_64-pc-linux-gnu"

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
target triple = "x86_64-pc-linux-gnu"

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
// stdout: == Dungeon (macOS build) ==
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
enum EnemyType: GOBLIN, ORC, DRAGON

struct Stats:
    hp  i32
    atk i32

struct Hero:
    name  char[]
    stats Stats

    pub fn create(name char[], hp i32, atk i32) Hero:
        h Hero
        h.name       = name
        h.stats.hp   = hp
        h.stats.atk  = atk
        return h

    fn alive() bool:
        return self.stats.hp > 0

    operator output() char[]:
        return self.name

fn enemy_hp(kind EnemyType) i32:
    match kind:
        case GOBLIN: return 20
        case ORC:    return 45
        case DRAGON: return 120
    return 0

fn loot_for(kind EnemyType) char[]:
    match kind:
        case DRAGON: return "Dragonfang"
        default:     return null
    return null

fn strongest(party Hero[], n i32) &Hero:
    best i32 = 0
    for i in 1..n:
        if party[i].stats.atk > party[best].stats.atk:
            best = i
    return &party[best]

fn main() i32:
    @if os == "macos":
        io::out("== Dungeon (macOS build) ==\n")
    @elif os == "linux":
        io::out("== Dungeon (Linux build) ==\n")
    @else:
        io::out("== Dungeon ==\n")

    party Hero[] = new Hero[3]
    party[0] = Hero::create("Aldric", 100, 18)
    party[1] = Hero::create("Borin",  90, 25)
    party[2] = Hero::create("Cass",   80, 22)

    io::out("Party:\n")
    for h in party:
        io::out($"  {h} atk={h.stats.atk}\n")

    champ &Hero = strongest(party, 3)
    io::out($"Champion: {champ} (alive={champ.alive()})\n")

    foes EnemyType[] = [GOBLIN, ORC, DRAGON]
    for kind in foes:
        loot char[] = loot_for(kind)
        io::out($"Foe hp={enemy_hp(kind)} loot={loot ?? "none"}\n")

    for ref h in party:
        h.stats.hp = h.stats.hp - 10
    io::out($"Trap! {party[0]} hp={party[0].stats.hp}\n")

    io::out($"sizeof={sizeof(party[0])} typeof={typeof(champ)}\n")

    clean party
    return 0
```

---

## 033 — project: calculator — tokenize, parse, print the AST and evaluate with try/catch

```ura
// stdout: ========================================
// stdout: calculate expr:  2 * (3 + 4) - 10 / 5
// stdout: ast:
// stdout: -
// stdout: ├─ *
// stdout: │  ├─ 2
// stdout: │  └─ +
// stdout: │     ├─ 3
// stdout: │     └─ 4
// stdout: └─ /
// stdout:    ├─ 10
// stdout:    └─ 5
// stdout: result: 12
// stdout: ========================================
// stdout: calculate expr:  8 / (3 - 3)
// stdout: ast:
// stdout: /
// stdout: ├─ 8
// stdout: └─ -
// stdout:    ├─ 3
// stdout:    └─ 3
// stdout: ========================================
// stderr: [0;31m
// expect: 1
// projects/calculator.ura
use "@/header"

enum Type: NONE, NUMBER, ADD, SUB, MUL, DIV, LPAREN, RPAREN

struct Token:
    value i32
    type Type

    pub fn number(v i32) &Token:
        t &Token = new Token
        t.type = NUMBER
        t.value = v
        ret &t

    pub fn op(k Type) &Token:
        t &Token = new Token
        t.type = k
        ret &t

struct Node:
    token &Token
    left &Node
    right &Node

    pub fn create(token &Token) &Node:
        n &Node = new Node
        n.token = &token
        ret &n

    @no-warn
    operator drop():
        clean self.token
        clean self.left
        clean self.right

mod Lexer:
    fn run(src &String, toks Token[]) i32:
        fn is_digit(c char) bool: ret c >= '0' and c <= '9'
        n i32 = 0
        i i32 = 0
        while i < src.len():
            c char = src.at(i)
            if c == ' ':
                i = i + 1
                continue
            elif is_digit(c):
                v i32 = 0
                while i < src.len() and is_digit(src.at(i)):
                    v = v * 10 + ((src.at(i) as i32) - 48)
                    i = i + 1
                toks[n].type = NUMBER
                toks[n].value = v
                n = n + 1
                continue
            else:
                k Type = NUMBER
                match c:
                    case '+': k = ADD
                    case '-': k = SUB
                    case '*': k = MUL
                    case '/': k = DIV
                    case '(': k = LPAREN
                    case ')': k = RPAREN
                toks[n].type = k
                toks[n].value = 0
                n = n + 1
                i = i + 1
        ret n

mod Parser:
    fn factor(toks Token[], p &i32, n i32) &Node:
        if p < n and toks[p].type == LPAREN:
            p = p + 1
            inner &Node = expr(toks, &p, n)
            if p < n and toks[p].type == RPAREN:
                p = p + 1
            ret &inner
        lf &Node = Node::create(Token::number(toks[p].value))
        p = p + 1
        ret &lf

    fn term(toks Token[], p &i32, n i32) &Node:
        l &Node = factor(toks, &p, n)
        while p < n and (toks[p].type == MUL or toks[p].type == DIV):
            k Type = toks[p].type
            p = p + 1
            r &Node = factor(toks, &p, n)
            parent &Node = Node::create(Token::op(k))
            parent.left = &l
            parent.right = &r
            l = &parent
        ret &l

    fn expr(toks Token[], p &i32, n i32) &Node:
        l &Node = term(toks, &p, n)
        while p < n and (toks[p].type == ADD or toks[p].type == SUB):
            k Type = toks[p].type
            p = p + 1
            r &Node = term(toks, &p, n)
            parent &Node = Node::create(Token::op(k))
            parent.left = &l
            parent.right = &r
            l = &parent
        ret &l

fn sign(k Type) char[]:
    match k:
        case ADD: ret "+"
        case SUB: ret "-"
        case MUL: ret "*"
        case DIV: ret "/"
        default: ret "?"

fn label(n &Node):
    if n.token.type == NUMBER: io::out($"{n.token.value}\n")
    else:                      io::out($"{sign(n.token.type)}\n")

fn show(n &Node, pre char[], last bool):
    io::out($"{pre}")
    if last: io::out("└─ ")
    else:    io::out("├─ ")
    label(&n)
    kid String = String::create(pre)
    if last: kid.join("   ")
    else:    kid.join("│  ")
    if n.left != null:  show(&n.left, kid.chars(), n.right == null)
    if n.right != null: show(&n.right, kid.chars(), True)

fn tree(n &Node):
    label(&n)
    if n.left != null:  show(&n.left, "", n.right == null)
    if n.right != null: show(&n.right, "", True)

fn eval(n &Node) i32:
    match n.token.type:
        case NUMBER: ret n.token.value
        case ADD:    ret eval(&n.left) + eval(&n.right)
        case SUB:    ret eval(&n.left) - eval(&n.right)
        case MUL:    ret eval(&n.left) * eval(&n.right)
        case DIV:
            d i32 = eval(&n.right)
            if d == 0: throw Error::create(String::create("division by zero"))
            ret eval(&n.left) / d
        default:    ret 0


fn calc(text char[]):
    io::out("========================================\n")
    io::out($"calculate expr:  {text}\nast:\n")
    src String = String::create(text)
    toks Token[] = new Token[64]
    n i32 = Lexer::run(&src, toks)
    p i32 = 0
    root &Node = Parser::expr(toks, &p, n)
    tree(&root)
    try:
        io::out($"result: {eval(&root)}\n")
    catch e:
        io::err($"\nError: {e.message}\n")
    clean root
    clean toks


fn main() i32:
    calc("2 * (3 + 4) - 10 / 5")
    calc("8 / (3 - 3)")
    io::out("========================================\n")
    return 0
```

---

## 034 — libft: isalnum

```ura
// stdout: is alnum
// expect: 0
// libft/isalnum.ura
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
target triple = "x86_64-pc-linux-gnu"

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
target triple = "x86_64-pc-linux-gnu"

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
target triple = "x86_64-pc-linux-gnu"

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
target triple = "x86_64-pc-linux-gnu"

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
link "NoSuchLib"
fn main() i32:
    return 0
```

### stderr

```
error: test.ura:2:1 library 'NoSuchLib' not found
  |
2 | link "NoSuchLib"
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
target triple = "x86_64-pc-linux-gnu"

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
@if not installed("nothing_like_this"):
    @error "nothing_like_this is not installed" "linux: sudo apt install libnothing-dev"

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:2:5 nothing_like_this is not installed
  |
2 |     @error "nothing_like_this is not installed" "linux: sudo apt install libnothing-dev"
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
@if version == 2:
    x i32 = 1

fn main() i32:
    return 0
```

### stderr

```
error: test.ura:1:13 @if checks os, arch and installed("lib")
  |
1 | @if version == 2:
  |             ^^
help: use @if os == "linux" and installed("SDL2"):
error: aborting due to 1 error
```

### status

```
1
```

