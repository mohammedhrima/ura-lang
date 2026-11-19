# milestone5

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

---

## 007 — lexer: use missing opening quote

```ura
// error: Expected '"' after 'use'
use @/header

fn main() i32:
    output("hi\n")
    return 0
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
    output("argv: ", os.argv.len, "\n")
    return 0
```

