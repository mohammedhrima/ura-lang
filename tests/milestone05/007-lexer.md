# milestone05

## index

- 007 — lexer: use missing opening quote
- 008 — lexer: use with unclosed quote
- 009 — lexer: link missing opening quote
- 010 — lexer: link with unclosed quote
- 011 — mod: a module function is called with ::
- 012 — mod: nested modules: a::b::f()
- 013 — mod: an enum inside a module, read as mod::Enum::VARIANT
- 014 — mod: a module may be referenced before it is declared
- 015 — mod: a module function calls a sibling unqualified
- 016 — mod: a struct declared in a module, used as mod::Type
- 017 — mod: a pub static method on a struct inside a module
- 018 — mod: calling a module member with . is an error
- 019 — mod: an unknown member of a known module is an error
- 020 — mod: a global inside a module, read and written with ::
- 021 — mod: a module call is arity-checked
- 022 — os: argc and argv from the global os
- 023 — mod: a struct attribute with the name of a module global
- 024 — ura::line, ura::filename, ura::function
- 025 — unic: utf-8 char literals, printed as utf-8
- 026 — multi-line string: newlines and leading spaces dropped
- 027 — multi-line string: as a call argument, blank lines dropped
- 028 — multi-line string: escapes and quotes inside

---

## 007 — lexer: use missing opening quote

```ura
// error: Expected '"' after 'use'
use @/header

fn main() i32:
    output("hi\n")
    return 0
```

### stderr

```
error: test.ura:2:5 unknown character '@'
  |
2 | use @/header
  |     ^
help: remove it
error: aborting due to 1 error
```

### status

```
1
```

---

## 008 — lexer: use with unclosed quote

```ura
// error: Unterminated 'use' path, expected closing '"'
use "@/header

fn main() i32:
    output("hi\n")
    return 0
```

### stderr

```
error: test.ura:2:5 unclosed string
  |
2 | use "@/header
  |     ^^^^^^^^^
help: use use "@/header"
error: aborting due to 1 error
```

### status

```
1
```

---

## 009 — lexer: link missing opening quote

```ura
// error: Expected '"' after 'link'
// errors/lexer/008 — `link` missing opening quote
link header.h

fn main() i32:
    output("hi\n")
    return 0
```

### stderr

```
error: test.ura:3:6 expected a library name after 'link'
  |
3 | link header.h
  |      ^^^^^^
help: use link "name" header.h
error: aborting due to 1 error
```

### status

```
1
```

---

## 010 — lexer: link with unclosed quote

```ura
// error: Unterminated 'link' path, expected closing '"'
// errors/lexer/009 — `link` with unclosed quote
link "header.h

fn main() i32:
    output("hi\n")
    return 0
```

### stderr

```
error: test.ura:3:6 unclosed string
  |
3 | link "header.h
  |      ^^^^^^^^^
help: use link "header.h"
error: aborting due to 1 error
```

### status

```
1
```

---

## 011 — mod: a module function is called with ::

```ura
// stdout: 1
// expect: 0
mod lib:
    fn foo() i32:
        return 1

fn main() i32:
    output(lib::foo(), "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @"lib::foo"() {
entry:
  ret i32 1
}

define i32 @main() {
entry:
  %"call.lib::foo" = call i32 @"lib::foo"()
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"call.lib::foo")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 012 — mod: nested modules: a::b::f()

```ura
// stdout: 42
// expect: 0
mod a:
    mod b:
        fn f() i32:
            return 42

fn main() i32:
    output(a::b::f(), "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @"a::b::f"() {
entry:
  ret i32 42
}

define i32 @main() {
entry:
  %"call.a::b::f" = call i32 @"a::b::f"()
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"call.a::b::f")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 013 — mod: an enum inside a module, read as mod::Enum::VARIANT

```ura
// stdout: 0 2
// expect: 0
mod col:
    enum Color:
        RED, GREEN, BLUE

fn main() i32:
    output(col::Color::RED, " ", col::Color::BLUE, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1

define i32 @main() {
entry:
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 0, i32 2)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 014 — mod: a module may be referenced before it is declared

```ura
// stdout: 7
// expect: 0
fn main() i32:
    output(later::hi(), "\n")
    return 0

mod later:
    fn hi() i32:
        return 7
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @main() {
entry:
  %"call.later::hi" = call i32 @"later::hi"()
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"call.later::hi")
  ret i32 0
}

define i32 @"later::hi"() {
entry:
  ret i32 7
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 015 — mod: a module function calls a sibling unqualified

```ura
// stdout: 6
// expect: 0
mod m:
    fn helper() i32:
        return 5
    fn use_it() i32:
        return helper() + 1

fn main() i32:
    output(m::use_it(), "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @"m::helper"() {
entry:
  ret i32 5
}

define i32 @"m::use_it"() {
entry:
  %"call.m::helper" = call i32 @"m::helper"()
  %ADD = add i32 %"call.m::helper", 1
  ret i32 %ADD
}

define i32 @main() {
entry:
  %"call.m::use_it" = call i32 @"m::use_it"()
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"call.m::use_it")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 016 — mod: a struct declared in a module, used as mod::Type

```ura
// stdout: 9
// expect: 0
mod g:
    struct Point:
        x i32

fn main() i32:
    p g::Point
    p.x = 9
    output(p.x, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%"g::Point" = type { i32 }

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define i32 @main() {
entry:
  %p = alloca %"g::Point", align 8
  store %"g::Point" zeroinitializer, %"g::Point"* %p, align 4
  %p.x = getelementptr inbounds %"g::Point", %"g::Point"* %p, i32 0, i32 0
  store i32 9, i32* %p.x, align 4
  %p.x1 = getelementptr inbounds %"g::Point", %"g::Point"* %p, i32 0, i32 0
  %"$p.x" = load i32, i32* %p.x1, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$p.x")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 017 — mod: a pub static method on a struct inside a module

```ura
// stdout: 3
// expect: 0
mod g:
    struct Point:
        x i32
        pub fn create(v i32) Point:
            p Point
            p.x = v
            return p

fn main() i32:
    p g::Point = g::Point::create(3)
    output(p.x, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%"g::Point" = type { i32 }

@str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define %"g::Point" @"g::Point.create"(i32 %0) {
entry:
  %v = alloca i32, align 4
  %p = alloca %"g::Point", align 8
  store i32 %0, i32* %v, align 4
  store %"g::Point" zeroinitializer, %"g::Point"* %p, align 4
  %p.x = getelementptr inbounds %"g::Point", %"g::Point"* %p, i32 0, i32 0
  %"$v" = load i32, i32* %v, align 4
  store i32 %"$v", i32* %p.x, align 4
  %"$p" = load %"g::Point", %"g::Point"* %p, align 4
  ret %"g::Point" %"$p"
}

define i32 @main() {
entry:
  %p = alloca %"g::Point", align 8
  %"call.g::Point.create" = call %"g::Point" @"g::Point.create"(i32 3)
  store %"g::Point" %"call.g::Point.create", %"g::Point"* %p, align 4
  %p.x = getelementptr inbounds %"g::Point", %"g::Point"* %p, i32 0, i32 0
  %"$p.x" = load i32, i32* %p.x, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$p.x")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 018 — mod: calling a module member with . is an error

```ura
// error: 'lib' is a mod, not a value
mod lib:
    fn foo() i32:
        return 1

fn main() i32:
    output(lib.foo(), "\n")
    return 0
```

### stderr

```
error: test.ura:7:12 'lib' is a mod, not a value
  |
7 |     output(lib.foo(), "\n")
  |            ^^^
help: reach its members with 'lib::'
error: aborting due to 1 error
```

### status

```
1
```

---

## 019 — mod: an unknown member of a known module is an error

```ura
// error: no function named 'lib::bar'
mod lib:
    fn foo() i32:
        return 1

fn main() i32:
    output(lib::bar(), "\n")
    return 0
```

### stderr

```
error: test.ura:7:12 no function named 'lib::bar'
  |
7 |     output(lib::bar(), "\n")
  |            ^^^^^^^^
error: aborting due to 1 error
```

### status

```
1
```

---

## 020 — mod: a global inside a module, read and written with ::

```ura
// stdout: 5 6
// expect: 0
mod lib:
    x i32 = 5
    fn bump():
        x = x + 1

fn main() i32:
    output(lib::x, " ")
    lib::bump()
    output(lib::x, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@"lib::x" = global i32 0
@str = private unnamed_addr constant [4 x i8] c"%d \00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define void @"lib::bump"() {
entry:
  %"$lib::x" = load i32, i32* @"lib::x", align 4
  %ADD = add i32 %"$lib::x", 1
  store i32 %ADD, i32* @"lib::x", align 4
  ret void
}

define i32 @main() {
entry:
  store i32 5, i32* @"lib::x", align 4
  %"$lib::x" = load i32, i32* @"lib::x", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i32 %"$lib::x")
  call void @"lib::bump"()
  %"$lib::x1" = load i32, i32* @"lib::x", align 4
  %call.dprintf2 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 %"$lib::x1")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

---

## 021 — mod: a module call is arity-checked

```ura
// error: 'lib::foo' takes 1 argument, not 2
mod lib:
    fn foo(a i32) i32:
        return a

fn main() i32:
    output(lib::foo(1, 2), "\n")
    return 0
```

### stderr

```
error: test.ura:7:12 'lib::foo' takes 1 argument, not 2
  |
7 |     output(lib::foo(1, 2), "\n")
  |            ^^^^^^^^
help: fn lib::foo(a i32) i32
error: aborting due to 1 error
```

### status

```
1
```

---

## 022 — os: argc and argv from the global os

```ura
// stdout: argc: 1
// stdout: argv: 1
// expect: 0

fn main() i32:
    output("argc: ", os.argc, "\n")
    output("argv: ", os.argv.len(), "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

%Os = type { i32, %"Array<String>" }
%"Array<String>" = type { %String*, i32, i32 }
%String = type { i8*, i32, i32 }
%Range = type { i32, i32, i32 }

@os = global %Os zeroinitializer
@ucg.argc = global i32 0
@ucg.argv = global i8** null
@str = private unnamed_addr constant [10 x i8] c"argc: %d\0A\00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c"argv: %d\0A\00", align 1

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

define %Range @Range.create(i32 %0, i32 %1) {
entry:
  %start = alloca i32, align 4
  %stop = alloca i32, align 4
  %r = alloca %Range, align 8
  store i32 %0, i32* %start, align 4
  store i32 %1, i32* %stop, align 4
  store %Range zeroinitializer, %Range* %r, align 4
  %r.start = getelementptr inbounds %Range, %Range* %r, i32 0, i32 0
  %"$start" = load i32, i32* %start, align 4
  store i32 %"$start", i32* %r.start, align 4
  %r.stop = getelementptr inbounds %Range, %Range* %r, i32 0, i32 1
  %"$stop" = load i32, i32* %stop, align 4
  store i32 %"$stop", i32* %r.stop, align 4
  %r.step = getelementptr inbounds %Range, %Range* %r, i32 0, i32 2
  store i32 1, i32* %r.step, align 4
  br label %if9

if9:                                              ; preds = %entry
  %"$stop1" = load i32, i32* %stop, align 4
  %"$start2" = load i32, i32* %start, align 4
  %LT = icmp slt i32 %"$stop1", %"$start2"
  br i1 %LT, label %if9.then, label %endif

if9.then:                                         ; preds = %if9
  %r.step3 = getelementptr inbounds %Range, %Range* %r, i32 0, i32 2
  store i32 -1, i32* %r.step3, align 4
  br label %endif

endif:                                            ; preds = %if9.then, %if9
  %"$r" = load %Range, %Range* %r, align 4
  ret %Range %"$r"
}

define i1 @Range.in(%Range* %0, i32* %1, i32* %2) {
entry:
  %self = alloca %Range*, align 8
  %i = alloca i32*, align 8
  %x = alloca i32*, align 8
  %done = alloca i1, align 1
  store %Range* %0, %Range** %self, align 8
  store i32* %1, i32** %i, align 8
  store i32* %2, i32** %x, align 8
  %"*x" = load i32*, i32** %x, align 8
  %"*self" = load %Range*, %Range** %self, align 8
  %"*self.start" = getelementptr inbounds %Range, %Range* %"*self", i32 0, i32 0
  %"*i" = load i32*, i32** %i, align 8
  %"*self1" = load %Range*, %Range** %self, align 8
  %"*self.step" = getelementptr inbounds %Range, %Range* %"*self1", i32 0, i32 2
  %"$*i" = load i32, i32* %"*i", align 4
  %"$*self.step" = load i32, i32* %"*self.step", align 4
  %MUL = mul i32 %"$*i", %"$*self.step"
  %"$*self.start" = load i32, i32* %"*self.start", align 4
  %ADD = add i32 %"$*self.start", %MUL
  store i32 %ADD, i32* %"*x", align 4
  %"*self2" = load %Range*, %Range** %self, align 8
  %"*self.step3" = getelementptr inbounds %Range, %Range* %"*self2", i32 0, i32 2
  %"$*self.step4" = load i32, i32* %"*self.step3", align 4
  %GT = icmp sgt i32 %"$*self.step4", 0
  %"*x5" = load i32*, i32** %x, align 8
  %"*self6" = load %Range*, %Range** %self, align 8
  %"*self.stop" = getelementptr inbounds %Range, %Range* %"*self6", i32 0, i32 1
  %"$*x" = load i32, i32* %"*x5", align 4
  %"$*self.stop" = load i32, i32* %"*self.stop", align 4
  %GE = icmp sge i32 %"$*x", %"$*self.stop"
  %AND = and i1 %GT, %GE
  %"*self7" = load %Range*, %Range** %self, align 8
  %"*self.step8" = getelementptr inbounds %Range, %Range* %"*self7", i32 0, i32 2
  %"$*self.step9" = load i32, i32* %"*self.step8", align 4
  %LT = icmp slt i32 %"$*self.step9", 0
  %"*x10" = load i32*, i32** %x, align 8
  %"*self11" = load %Range*, %Range** %self, align 8
  %"*self.stop12" = getelementptr inbounds %Range, %Range* %"*self11", i32 0, i32 1
  %"$*x13" = load i32, i32* %"*x10", align 4
  %"$*self.stop14" = load i32, i32* %"*self.stop12", align 4
  %LE = icmp sle i32 %"$*x13", %"$*self.stop14"
  %AND15 = and i1 %LT, %LE
  %OR = or i1 %AND, %AND15
  store i1 %OR, i1* %done, align 1
  br label %if10

if10:                                             ; preds = %entry
  %"$done" = load i1, i1* %done, align 1
  br i1 %"$done", label %if10.then, label %endif

if10.then:                                        ; preds = %if10
  ret i1 false

endif:                                            ; preds = %if10
  %"*i16" = load i32*, i32** %i, align 8
  %"$*i17" = load i32, i32* %"*i16", align 4
  %ADD18 = add i32 %"$*i17", 1
  store i32 %ADD18, i32* %"*i16", align 4
  ret i1 true
}

define %String* @"new<String>"(i32 %0) {
entry:
  %count = alloca i32, align 4
  store i32 %0, i32* %count, align 4
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (%String* getelementptr (%String, %String* null, i32 1) to i32)
  %AS = sext i32 %MUL to i64
  %call.calloc = call i8* @calloc(i64 %AS, i64 sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64))
  %AS1 = bitcast i8* %call.calloc to %String*
  ret %String* %AS1
}

define void @"copy<String>"(%String* %0, %String* %1, i32 %2) {
entry:
  %dst = alloca %String*, align 8
  %src = alloca %String*, align 8
  %count = alloca i32, align 4
  store %String* %0, %String** %dst, align 8
  store %String* %1, %String** %src, align 8
  store i32 %2, i32* %count, align 4
  %"$dst" = load %String*, %String** %dst, align 8
  %AS = bitcast %String* %"$dst" to i8*
  %"$src" = load %String*, %String** %src, align 8
  %AS1 = bitcast %String* %"$src" to i8*
  %"$count" = load i32, i32* %count, align 4
  %MUL = mul i32 %"$count", ptrtoint (%String* getelementptr (%String, %String* null, i32 1) to i32)
  %AS2 = sext i32 %MUL to i64
  %MUL3 = mul i64 %AS2, sext (i32 ptrtoint (i8* getelementptr (i8, i8* null, i32 1) to i32) to i64)
  %call.memcpy = call i8* @memcpy(i8* %AS, i8* %AS1, i64 %MUL3)
  ret void
}

define void @"free<String>"(%String* %0) {
entry:
  %data = alloca %String*, align 8
  store %String* %0, %String** %data, align 8
  %"$data" = load %String*, %String** %data, align 8
  %AS = bitcast %String* %"$data" to i8*
  call void @free(i8* %AS)
  ret void
}

define void @"Array<String>.reserve"(%"Array<String>"* %0, i32 %1) {
entry:
  %self = alloca %"Array<String>"*, align 8
  %cap = alloca i32, align 4
  %bigger = alloca %String*, align 8
  store %"Array<String>"* %0, %"Array<String>"** %self, align 8
  store i32 %1, i32* %cap, align 4
  br label %if11

if11:                                             ; preds = %entry
  %"*self" = load %"Array<String>"*, %"Array<String>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<String>", %"Array<String>"* %"*self", i32 0, i32 1
  %"$cap" = load i32, i32* %cap, align 4
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %LT = icmp slt i32 %"$cap", %"$*self.count"
  br i1 %LT, label %if11.then, label %endif

if11.then:                                        ; preds = %if11
  %"*self3" = load %"Array<String>"*, %"Array<String>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<String>", %"Array<String>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  store i32 %"$*self.count5", i32* %cap, align 4
  br label %endif

endif:                                            ; preds = %if11.then, %if11
  br label %if12

if12:                                             ; preds = %endif
  %"*self6" = load %"Array<String>"*, %"Array<String>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<String>", %"Array<String>"* %"*self6", i32 0, i32 2
  %"$cap7" = load i32, i32* %cap, align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %LE = icmp sle i32 %"$cap7", %"$*self.cap"
  br i1 %LE, label %if12.then, label %endif1

if12.then:                                        ; preds = %if12
  ret void

endif1:                                           ; preds = %if12
  %"$cap8" = load i32, i32* %cap, align 4
  %"call.new<String>" = call %String* @"new<String>"(i32 %"$cap8")
  store %String* %"call.new<String>", %String** %bigger, align 8
  %"*self9" = load %"Array<String>"*, %"Array<String>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<String>", %"Array<String>"* %"*self9", i32 0, i32 0
  %"*self10" = load %"Array<String>"*, %"Array<String>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<String>", %"Array<String>"* %"*self10", i32 0, i32 1
  %"$bigger" = load %String*, %String** %bigger, align 8
  %"$*self.data" = load %String*, %String** %"*self.data", align 8
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  call void @"copy<String>"(%String* %"$bigger", %String* %"$*self.data", i32 %"$*self.count12")
  br label %if13

if13:                                             ; preds = %endif1
  %"*self13" = load %"Array<String>"*, %"Array<String>"** %self, align 8
  %"*self.cap14" = getelementptr inbounds %"Array<String>", %"Array<String>"* %"*self13", i32 0, i32 2
  %"$*self.cap15" = load i32, i32* %"*self.cap14", align 4
  %GT = icmp sgt i32 %"$*self.cap15", 0
  br i1 %GT, label %if13.then, label %endif2

if13.then:                                        ; preds = %if13
  %"*self16" = load %"Array<String>"*, %"Array<String>"** %self, align 8
  %"*self.data17" = getelementptr inbounds %"Array<String>", %"Array<String>"* %"*self16", i32 0, i32 0
  %"$*self.data18" = load %String*, %String** %"*self.data17", align 8
  call void @"free<String>"(%String* %"$*self.data18")
  br label %endif2

endif2:                                           ; preds = %if13.then, %if13
  %"*self19" = load %"Array<String>"*, %"Array<String>"** %self, align 8
  %"*self.data20" = getelementptr inbounds %"Array<String>", %"Array<String>"* %"*self19", i32 0, i32 0
  %"$bigger21" = load %String*, %String** %bigger, align 8
  store %String* %"$bigger21", %String** %"*self.data20", align 8
  %"*self22" = load %"Array<String>"*, %"Array<String>"** %self, align 8
  %"*self.cap23" = getelementptr inbounds %"Array<String>", %"Array<String>"* %"*self22", i32 0, i32 2
  %"$cap24" = load i32, i32* %cap, align 4
  store i32 %"$cap24", i32* %"*self.cap23", align 4
  ret void
}

define void @"Array<String>.clear"(%"Array<String>"* %0) {
entry:
  %self = alloca %"Array<String>"*, align 8
  store %"Array<String>"* %0, %"Array<String>"** %self, align 8
  br label %if14

if14:                                             ; preds = %entry
  %"*self" = load %"Array<String>"*, %"Array<String>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<String>", %"Array<String>"* %"*self", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %EQ = icmp eq i32 %"$*self.cap", 0
  br i1 %EQ, label %if14.then, label %endif

if14.then:                                        ; preds = %if14
  %"*self1" = load %"Array<String>"*, %"Array<String>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<String>", %"Array<String>"* %"*self1", i32 0, i32 1
  store i32 0, i32* %"*self.count", align 4
  br label %endif

endif:                                            ; preds = %if14.then, %if14
  br label %while15

while15:                                          ; preds = %while15.then, %endif
  %"*self2" = load %"Array<String>"*, %"Array<String>"** %self, align 8
  %"*self.count3" = getelementptr inbounds %"Array<String>", %"Array<String>"* %"*self2", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count3", align 4
  %GT = icmp sgt i32 %"$*self.count", 0
  br i1 %GT, label %while15.then, label %endloop

while15.then:                                     ; preds = %while15
  %"*self4" = load %"Array<String>"*, %"Array<String>"** %self, align 8
  %"*self.count5" = getelementptr inbounds %"Array<String>", %"Array<String>"* %"*self4", i32 0, i32 1
  %"$*self.count6" = load i32, i32* %"*self.count5", align 4
  %SUB = sub i32 %"$*self.count6", 1
  store i32 %SUB, i32* %"*self.count5", align 4
  %"*self7" = load %"Array<String>"*, %"Array<String>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<String>", %"Array<String>"* %"*self7", i32 0, i32 0
  %"*self8" = load %"Array<String>"*, %"Array<String>"** %self, align 8
  %"*self.count9" = getelementptr inbounds %"Array<String>", %"Array<String>"* %"*self8", i32 0, i32 1
  %"$*self.data" = load %String*, %String** %"*self.data", align 8
  %"$*self.count10" = load i32, i32* %"*self.count9", align 4
  %"*self.data[]" = getelementptr inbounds %String, %String* %"$*self.data", i32 %"$*self.count10"
  call void @String.drop(%String* %"*self.data[]")
  store %String zeroinitializer, %String* %"*self.data[]", align 8
  br label %while15

endloop:                                          ; preds = %while15
  ret void
}

define void @"Array<String>.append"(%"Array<String>"* %0, %String %1) {
entry:
  %self = alloca %"Array<String>"*, align 8
  %value = alloca %String, align 8
  %copy = alloca %String, align 8
  store %"Array<String>"* %0, %"Array<String>"** %self, align 8
  store %String %1, %String* %value, align 8
  br label %if16

if16:                                             ; preds = %entry
  %"*self" = load %"Array<String>"*, %"Array<String>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<String>", %"Array<String>"* %"*self", i32 0, i32 1
  %"*self1" = load %"Array<String>"*, %"Array<String>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<String>", %"Array<String>"* %"*self1", i32 0, i32 2
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GE = icmp sge i32 %"$*self.count", %"$*self.cap"
  br i1 %GE, label %if16.then, label %endif

if16.then:                                        ; preds = %if16
  %"*self2" = load %"Array<String>"*, %"Array<String>"** %self, align 8
  %"*self3" = load %"Array<String>"*, %"Array<String>"** %self, align 8
  %"*self.count4" = getelementptr inbounds %"Array<String>", %"Array<String>"* %"*self3", i32 0, i32 1
  %"$*self.count5" = load i32, i32* %"*self.count4", align 4
  %MUL = mul i32 %"$*self.count5", 2
  %ADD = add i32 %MUL, 4
  call void @"Array<String>.reserve"(%"Array<String>"* %"*self2", i32 %ADD)
  br label %endif

endif:                                            ; preds = %if16.then, %if16
  %"*self6" = load %"Array<String>"*, %"Array<String>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<String>", %"Array<String>"* %"*self6", i32 0, i32 0
  %"*self7" = load %"Array<String>"*, %"Array<String>"** %self, align 8
  %"*self.count8" = getelementptr inbounds %"Array<String>", %"Array<String>"* %"*self7", i32 0, i32 1
  %"$*self.data" = load %String*, %String** %"*self.data", align 8
  %"$*self.count9" = load i32, i32* %"*self.count8", align 4
  %"*self.data[]" = getelementptr inbounds %String, %String* %"$*self.data", i32 %"$*self.count9"
  store %String zeroinitializer, %String* %copy, align 8
  call void @"String.="(%String* %copy, %String* %value)
  %"$copy" = load %String, %String* %copy, align 8
  call void @String.drop(%String* %"*self.data[]")
  store %String %"$copy", %String* %"*self.data[]", align 8
  %"*self10" = load %"Array<String>"*, %"Array<String>"** %self, align 8
  %"*self.count11" = getelementptr inbounds %"Array<String>", %"Array<String>"* %"*self10", i32 0, i32 1
  %"$*self.count12" = load i32, i32* %"*self.count11", align 4
  %ADD13 = add i32 %"$*self.count12", 1
  store i32 %ADD13, i32* %"*self.count11", align 4
  call void @String.drop(%String* %value)
  ret void
}

define i32 @"Array<String>.len"(%"Array<String>"* %0) {
entry:
  %self = alloca %"Array<String>"*, align 8
  store %"Array<String>"* %0, %"Array<String>"** %self, align 8
  %"*self" = load %"Array<String>"*, %"Array<String>"** %self, align 8
  %"*self.count" = getelementptr inbounds %"Array<String>", %"Array<String>"* %"*self", i32 0, i32 1
  %"$*self.count" = load i32, i32* %"*self.count", align 4
  ret i32 %"$*self.count"
}

define void @"Array<String>.drop"(%"Array<String>"* %0) {
entry:
  %self = alloca %"Array<String>"*, align 8
  store %"Array<String>"* %0, %"Array<String>"** %self, align 8
  %"*self" = load %"Array<String>"*, %"Array<String>"** %self, align 8
  call void @"Array<String>.clear"(%"Array<String>"* %"*self")
  br label %if17

if17:                                             ; preds = %entry
  %"*self1" = load %"Array<String>"*, %"Array<String>"** %self, align 8
  %"*self.cap" = getelementptr inbounds %"Array<String>", %"Array<String>"* %"*self1", i32 0, i32 2
  %"$*self.cap" = load i32, i32* %"*self.cap", align 4
  %GT = icmp sgt i32 %"$*self.cap", 0
  br i1 %GT, label %if17.then, label %endif

if17.then:                                        ; preds = %if17
  %"*self2" = load %"Array<String>"*, %"Array<String>"** %self, align 8
  %"*self.data" = getelementptr inbounds %"Array<String>", %"Array<String>"* %"*self2", i32 0, i32 0
  %"$*self.data" = load %String*, %String** %"*self.data", align 8
  call void @"free<String>"(%String* %"$*self.data")
  br label %endif

endif:                                            ; preds = %if17.then, %if17
  ret void
}

define %Os @Os.create() {
entry:
  %res = alloca %Os, align 8
  %tmp = alloca %Range, align 8
  %i = alloca i32, align 4
  %i2 = alloca i32, align 4
  store %Os zeroinitializer, %Os* %res, align 8
  %res.argc = getelementptr inbounds %Os, %Os* %res, i32 0, i32 0
  %"$ucg.argc" = load i32, i32* @ucg.argc, align 4
  store i32 %"$ucg.argc", i32* %res.argc, align 4
  %"$ucg.argc1" = load i32, i32* @ucg.argc, align 4
  %call.Range.create = call %Range @Range.create(i32 0, i32 %"$ucg.argc1")
  store %Range %call.Range.create, %Range* %tmp, align 4
  store i32 0, i32* %i, align 4
  store i32 0, i32* %i2, align 4
  br label %while18

while18:                                          ; preds = %while18.then, %entry
  %call.Range.in = call i1 @Range.in(%Range* %tmp, i32* %i, i32* %i2)
  br i1 %call.Range.in, label %while18.then, label %endloop

while18.then:                                     ; preds = %while18
  %res.argv = getelementptr inbounds %Os, %Os* %res, i32 0, i32 1
  %"$ucg.argv" = load i8**, i8*** @ucg.argv, align 8
  %"$i" = load i32, i32* %i2, align 4
  %"$ucg.argv[]" = getelementptr inbounds i8*, i8** %"$ucg.argv", i32 %"$i"
  %"$$ucg.argv[]" = load i8*, i8** %"$ucg.argv[]", align 8
  %call.String.create = call %String @String.create(i8* %"$$ucg.argv[]")
  call void @"Array<String>.append"(%"Array<String>"* %res.argv, %String %call.String.create)
  br label %while18

endloop:                                          ; preds = %while18
  %"$res" = load %Os, %Os* %res, align 8
  ret %Os %"$res"
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %argc = alloca i32, align 4
  %argv = alloca i8**, align 8
  store i32 %0, i32* %argc, align 4
  store i8** %1, i8*** %argv, align 8
  store i32 %0, i32* @ucg.argc, align 4
  store i8** %1, i8*** @ucg.argv, align 8
  %call.Os.create = call %Os @Os.create()
  store %Os %call.Os.create, %Os* @os, align 8
  %"$os.argc" = load i32, i32* getelementptr inbounds (%Os, %Os* @os, i32 0, i32 0), align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str, i32 0, i32 0), i32 %"$os.argc")
  %"call.Array<String>.len" = call i32 @"Array<String>.len"(%"Array<String>"* getelementptr inbounds (%Os, %Os* @os, i32 0, i32 1))
  %call.dprintf1 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i32 %"call.Array<String>.len")
  call void @"Array<String>.drop"(%"Array<String>"* getelementptr inbounds (%Os, %Os* @os, i32 0, i32 1))
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```
---

## 023 — mod: a struct attribute with the name of a module global

```ura
// stdout: 1 2
// expect: 0
mod geo:
    x i32 = 1
    struct Point:
        x i32
        fn show():
            output(x, " ", self.x, "\n")

fn main() i32:
    p geo::Point
    p.x = 2
    p.show()
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%"geo::Point" = type { i32 }

@"geo::x" = global i32 0
@str = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1

define void @"geo::Point.show"(%"geo::Point"* %0) {
entry:
  %self = alloca %"geo::Point"*, align 8
  store %"geo::Point"* %0, %"geo::Point"** %self, align 8
  %"*self" = load %"geo::Point"*, %"geo::Point"** %self, align 8
  %"*self.x" = getelementptr inbounds %"geo::Point", %"geo::Point"* %"*self", i32 0, i32 0
  %"$geo::x" = load i32, i32* @"geo::x", align 4
  %"$*self.x" = load i32, i32* %"*self.x", align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %"$geo::x", i32 %"$*self.x")
  ret void
}

define i32 @main() {
entry:
  %p = alloca %"geo::Point", align 8
  store i32 1, i32* @"geo::x", align 4
  store %"geo::Point" zeroinitializer, %"geo::Point"* %p, align 4
  %p.x = getelementptr inbounds %"geo::Point", %"geo::Point"* %p, i32 0, i32 0
  store i32 2, i32* %p.x, align 4
  call void @"geo::Point.show"(%"geo::Point"* %p)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```
---

## 024 — ura::line, ura::filename, ura::function

```ura
// stdout: test.ura:6 in where
// stdout: show 11
// stdout: 18
// expect: 0
fn where():
    output(ura::filename, ":", ura::line, " in ", ura::function, "\n")

struct Point:
    x i32
    fn show():
        output(ura::function, " ", ura::line, "\n")

fn main() i32:
    where()
    p Point
    p.show()
    n i32 = ura::line + 1
    output(n, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%Point = type { i32 }

@str = private unnamed_addr constant [22 x i8] c"test.ura:%d in where\0A\00", align 1
@str.1 = private unnamed_addr constant [9 x i8] c"show %d\0A\00", align 1
@str.2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1

define void @where() {
entry:
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @str, i32 0, i32 0), i32 6)
  ret void
}

define void @Point.show(%Point* %0) {
entry:
  %self = alloca %Point*, align 8
  store %Point* %0, %Point** %self, align 8
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0), i32 11)
  ret void
}

define i32 @main() {
entry:
  %p = alloca %Point, align 8
  %n = alloca i32, align 4
  call void @where()
  store %Point zeroinitializer, %Point* %p, align 4
  call void @Point.show(%Point* %p)
  store i32 18, i32* %n, align 4
  %"$n" = load i32, i32* %n, align 4
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.2, i32 0, i32 0), i32 %"$n")
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```
---

## 025 — unic: utf-8 char literals, printed as utf-8

```ura
// stdout: a é 浦 🌊 🐚 🐙
// stdout: 233 28006 127754
// expect: 0
fn main() i32:
    a unic = 'a'
    e unic = 'é'
    bay unic = '浦'
    wave unic = '🌊'
    shell unic = '🐚'
    octopus unic = '🐙'
    output(a, " ", e, " ", bay, " ", wave, " ", shell, " ", octopus, "\n")
    output(e as i32, " ", bay as i32, " ", wave as i32, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private unnamed_addr constant [19 x i8] c"%s %s %s %s %s %s\0A\00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c"%d %d %d\0A\00", align 1

define i32 @main() {
entry:
  %a = alloca i21, align 4
  %e = alloca i21, align 4
  %bay = alloca i21, align 4
  %wave = alloca i21, align 4
  %shell = alloca i21, align 4
  %octopus = alloca i21, align 4
  %arr = alloca [5 x i8], align 1
  %arr1 = alloca [5 x i8], align 1
  %arr4 = alloca [5 x i8], align 1
  %arr7 = alloca [5 x i8], align 1
  %arr10 = alloca [5 x i8], align 1
  %arr13 = alloca [5 x i8], align 1
  store i21 97, i21* %a, align 4
  store i21 233, i21* %e, align 4
  store i21 28006, i21* %bay, align 4
  store i21 127754, i21* %wave, align 4
  store i21 128026, i21* %shell, align 4
  store i21 128025, i21* %octopus, align 4
  %"$a" = load i21, i21* %a, align 4
  store [5 x i8] zeroinitializer, [5 x i8]* %arr, align 1
  %0 = getelementptr inbounds [5 x i8], [5 x i8]* %arr, i32 0, i32 0
  %AS = sext i21 %"$a" to i32
  %call.ura.unic = call i8* @ura.unic(i32 %AS, i8* %0)
  %"$e" = load i21, i21* %e, align 4
  store [5 x i8] zeroinitializer, [5 x i8]* %arr1, align 1
  %1 = getelementptr inbounds [5 x i8], [5 x i8]* %arr1, i32 0, i32 0
  %AS2 = sext i21 %"$e" to i32
  %call.ura.unic3 = call i8* @ura.unic(i32 %AS2, i8* %1)
  %"$bay" = load i21, i21* %bay, align 4
  store [5 x i8] zeroinitializer, [5 x i8]* %arr4, align 1
  %2 = getelementptr inbounds [5 x i8], [5 x i8]* %arr4, i32 0, i32 0
  %AS5 = sext i21 %"$bay" to i32
  %call.ura.unic6 = call i8* @ura.unic(i32 %AS5, i8* %2)
  %"$wave" = load i21, i21* %wave, align 4
  store [5 x i8] zeroinitializer, [5 x i8]* %arr7, align 1
  %3 = getelementptr inbounds [5 x i8], [5 x i8]* %arr7, i32 0, i32 0
  %AS8 = sext i21 %"$wave" to i32
  %call.ura.unic9 = call i8* @ura.unic(i32 %AS8, i8* %3)
  %"$shell" = load i21, i21* %shell, align 4
  store [5 x i8] zeroinitializer, [5 x i8]* %arr10, align 1
  %4 = getelementptr inbounds [5 x i8], [5 x i8]* %arr10, i32 0, i32 0
  %AS11 = sext i21 %"$shell" to i32
  %call.ura.unic12 = call i8* @ura.unic(i32 %AS11, i8* %4)
  %"$octopus" = load i21, i21* %octopus, align 4
  store [5 x i8] zeroinitializer, [5 x i8]* %arr13, align 1
  %5 = getelementptr inbounds [5 x i8], [5 x i8]* %arr13, i32 0, i32 0
  %AS14 = sext i21 %"$octopus" to i32
  %call.ura.unic15 = call i8* @ura.unic(i32 %AS14, i8* %5)
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @str, i32 0, i32 0), i8* %call.ura.unic, i8* %call.ura.unic3, i8* %call.ura.unic6, i8* %call.ura.unic9, i8* %call.ura.unic12, i8* %call.ura.unic15)
  %"$e16" = load i21, i21* %e, align 4
  %AS17 = sext i21 %"$e16" to i32
  %"$bay18" = load i21, i21* %bay, align 4
  %AS19 = sext i21 %"$bay18" to i32
  %"$wave20" = load i21, i21* %wave, align 4
  %AS21 = sext i21 %"$wave20" to i32
  %call.dprintf22 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0), i32 %AS17, i32 %AS19, i32 %AS21)
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)

define i8* @ura.unic(i32 %0, i8* %1) {
entry:
  %code = alloca i32, align 4
  %text = alloca i8*, align 8
  store i32 %0, i32* %code, align 4
  store i8* %1, i8** %text, align 8
  br label %if0

if0:                                              ; preds = %entry
  %"$code" = load i32, i32* %code, align 4
  %LT = icmp slt i32 %"$code", 128
  br i1 %LT, label %if0.then, label %elif1

if0.then:                                         ; preds = %if0
  %"$code1" = load i32, i32* %code, align 4
  %DIV = sdiv i32 %"$code1", 1
  %ADD = add i32 %DIV, 0
  %MOD = srem i32 %DIV, 64
  %ADD2 = add i32 %MOD, 128
  %AS = trunc i32 %ADD to i8
  %"$text" = load i8*, i8** %text, align 8
  %"text[]" = getelementptr inbounds i8, i8* %"$text", i32 0
  store i8 %AS, i8* %"text[]", align 1
  %"$text3" = load i8*, i8** %text, align 8
  %"text[]4" = getelementptr inbounds i8, i8* %"$text3", i32 1
  store i8 0, i8* %"text[]4", align 1
  br label %endif

elif1:                                            ; preds = %if0
  %"$code5" = load i32, i32* %code, align 4
  %LT6 = icmp slt i32 %"$code5", 2048
  br i1 %LT6, label %elif1.then, label %elif2

elif1.then:                                       ; preds = %elif1
  %"$code7" = load i32, i32* %code, align 4
  %DIV8 = sdiv i32 %"$code7", 1
  %ADD9 = add i32 %DIV8, 192
  %MOD10 = srem i32 %DIV8, 64
  %ADD11 = add i32 %MOD10, 128
  %AS12 = trunc i32 %ADD11 to i8
  %"$text13" = load i8*, i8** %text, align 8
  %"text[]14" = getelementptr inbounds i8, i8* %"$text13", i32 1
  store i8 %AS12, i8* %"text[]14", align 1
  %"$code15" = load i32, i32* %code, align 4
  %DIV16 = sdiv i32 %"$code15", 64
  %ADD17 = add i32 %DIV16, 192
  %MOD18 = srem i32 %DIV16, 64
  %ADD19 = add i32 %MOD18, 128
  %AS20 = trunc i32 %ADD17 to i8
  %"$text21" = load i8*, i8** %text, align 8
  %"text[]22" = getelementptr inbounds i8, i8* %"$text21", i32 0
  store i8 %AS20, i8* %"text[]22", align 1
  %"$text23" = load i8*, i8** %text, align 8
  %"text[]24" = getelementptr inbounds i8, i8* %"$text23", i32 2
  store i8 0, i8* %"text[]24", align 1
  br label %endif

elif2:                                            ; preds = %elif1
  %"$code25" = load i32, i32* %code, align 4
  %LT26 = icmp slt i32 %"$code25", 65536
  br i1 %LT26, label %elif2.then, label %elif3

elif2.then:                                       ; preds = %elif2
  %"$code27" = load i32, i32* %code, align 4
  %DIV28 = sdiv i32 %"$code27", 1
  %ADD29 = add i32 %DIV28, 224
  %MOD30 = srem i32 %DIV28, 64
  %ADD31 = add i32 %MOD30, 128
  %AS32 = trunc i32 %ADD31 to i8
  %"$text33" = load i8*, i8** %text, align 8
  %"text[]34" = getelementptr inbounds i8, i8* %"$text33", i32 2
  store i8 %AS32, i8* %"text[]34", align 1
  %"$code35" = load i32, i32* %code, align 4
  %DIV36 = sdiv i32 %"$code35", 64
  %ADD37 = add i32 %DIV36, 224
  %MOD38 = srem i32 %DIV36, 64
  %ADD39 = add i32 %MOD38, 128
  %AS40 = trunc i32 %ADD39 to i8
  %"$text41" = load i8*, i8** %text, align 8
  %"text[]42" = getelementptr inbounds i8, i8* %"$text41", i32 1
  store i8 %AS40, i8* %"text[]42", align 1
  %"$code43" = load i32, i32* %code, align 4
  %DIV44 = sdiv i32 %"$code43", 4096
  %ADD45 = add i32 %DIV44, 224
  %MOD46 = srem i32 %DIV44, 64
  %ADD47 = add i32 %MOD46, 128
  %AS48 = trunc i32 %ADD45 to i8
  %"$text49" = load i8*, i8** %text, align 8
  %"text[]50" = getelementptr inbounds i8, i8* %"$text49", i32 0
  store i8 %AS48, i8* %"text[]50", align 1
  %"$text51" = load i8*, i8** %text, align 8
  %"text[]52" = getelementptr inbounds i8, i8* %"$text51", i32 3
  store i8 0, i8* %"text[]52", align 1
  br label %endif

elif3:                                            ; preds = %elif2
  %"$code53" = load i32, i32* %code, align 4
  %LT54 = icmp slt i32 %"$code53", 1114112
  br i1 %LT54, label %elif3.then, label %endif

elif3.then:                                       ; preds = %elif3
  %"$code55" = load i32, i32* %code, align 4
  %DIV56 = sdiv i32 %"$code55", 1
  %ADD57 = add i32 %DIV56, 240
  %MOD58 = srem i32 %DIV56, 64
  %ADD59 = add i32 %MOD58, 128
  %AS60 = trunc i32 %ADD59 to i8
  %"$text61" = load i8*, i8** %text, align 8
  %"text[]62" = getelementptr inbounds i8, i8* %"$text61", i32 3
  store i8 %AS60, i8* %"text[]62", align 1
  %"$code63" = load i32, i32* %code, align 4
  %DIV64 = sdiv i32 %"$code63", 64
  %ADD65 = add i32 %DIV64, 240
  %MOD66 = srem i32 %DIV64, 64
  %ADD67 = add i32 %MOD66, 128
  %AS68 = trunc i32 %ADD67 to i8
  %"$text69" = load i8*, i8** %text, align 8
  %"text[]70" = getelementptr inbounds i8, i8* %"$text69", i32 2
  store i8 %AS68, i8* %"text[]70", align 1
  %"$code71" = load i32, i32* %code, align 4
  %DIV72 = sdiv i32 %"$code71", 4096
  %ADD73 = add i32 %DIV72, 240
  %MOD74 = srem i32 %DIV72, 64
  %ADD75 = add i32 %MOD74, 128
  %AS76 = trunc i32 %ADD75 to i8
  %"$text77" = load i8*, i8** %text, align 8
  %"text[]78" = getelementptr inbounds i8, i8* %"$text77", i32 1
  store i8 %AS76, i8* %"text[]78", align 1
  %"$code79" = load i32, i32* %code, align 4
  %DIV80 = sdiv i32 %"$code79", 262144
  %ADD81 = add i32 %DIV80, 240
  %MOD82 = srem i32 %DIV80, 64
  %ADD83 = add i32 %MOD82, 128
  %AS84 = trunc i32 %ADD81 to i8
  %"$text85" = load i8*, i8** %text, align 8
  %"text[]86" = getelementptr inbounds i8, i8* %"$text85", i32 0
  store i8 %AS84, i8* %"text[]86", align 1
  %"$text87" = load i8*, i8** %text, align 8
  %"text[]88" = getelementptr inbounds i8, i8* %"$text87", i32 4
  store i8 0, i8* %"text[]88", align 1
  br label %endif

endif:                                            ; preds = %elif3.then, %elif3, %elif2.then, %elif1.then, %if0.then
  %"$text89" = load i8*, i8** %text, align 8
  ret i8* %"$text89"
}
```


---

## 026 — multi-line string: newlines and leading spaces dropped

```ura
// stdout: helloworld
// stdout: hello again
// stdout: line 13
// expect: 0
fn main() i32:
    a String = "hello
        world\n"
    output(a)
    b String = "
        hello again\n
        "
    output(b)
    output("line ", ura::line, "\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [12 x i8] c"helloworld\0A\00", align 1
@str.1 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.2 = private unnamed_addr constant [13 x i8] c"hello again\0A\00", align 1
@str.3 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.4 = private unnamed_addr constant [9 x i8] c"line %d\0A\00", align 1

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

define i32 @main() {
entry:
  %a = alloca %String, align 8
  %b = alloca %String, align 8
  store %String { i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i32 11, i32 0 }, %String* %a, align 8
  %call.String.chars = call i8* @String.chars(%String* %a)
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0), i8* %call.String.chars)
  store %String { i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.2, i32 0, i32 0), i32 12, i32 0 }, %String* %b, align 8
  %call.String.chars1 = call i8* @String.chars(%String* %b)
  %call.dprintf2 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.3, i32 0, i32 0), i8* %call.String.chars1)
  %call.dprintf3 = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.4, i32 0, i32 0), i32 13)
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

## 027 — multi-line string: as a call argument, blank lines dropped

```ura
// stdout: onetwo
// stdout: three
// expect: 0
fn show(text String):
    output(text)

fn main() i32:
    show("one

        two\n
        three\n")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%String = type { i8*, i32, i32 }

@str = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.1 = private unnamed_addr constant [14 x i8] c"onetwo\0Athree\0A\00", align 1

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

define void @show(%String %0) {
entry:
  %text = alloca %String, align 8
  store %String %0, %String* %text, align 8
  %call.String.chars = call i8* @String.chars(%String* %text)
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str, i32 0, i32 0), i8* %call.String.chars)
  call void @String.drop(%String* %text)
  ret void
}

define i32 @main() {
entry:
  call void @show(%String { i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.1, i32 0, i32 0), i32 13, i32 0 })
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i8* @memcpy(i8*, i8*, i64)

declare void @free(i8*)

declare i32 @dprintf(i32, i8*, ...)
```

---

## 028 — multi-line string: escapes and quotes inside

```ura
// stdout: name: "ura"
// stdout: 	tab
// expect: 0
fn main() i32:
    output("
        name: \"ura\"\n
        \ttab\n
        ")
    return 0
```

### llvm ir

```llvm
; ModuleID = 'ura'
source_filename = "ura"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@str = private unnamed_addr constant [18 x i8] c"name: \22ura\22\0A\09tab\0A\00", align 1

define i32 @main() {
entry:
  %call.dprintf = call i32 (i32, i8*, ...) @dprintf(i32 1, i8* getelementptr inbounds ([18 x i8], [18 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @dprintf(i32, i8*, ...)
```

