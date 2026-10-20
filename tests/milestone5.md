# milestone5

## index

- 001 — use: a relative use pulls in a sibling file
- 002 — use: use resolves against the importing file, including a subdirectory
- 003 — use: a diamond loads the shared file once, and says nothing
- 004 — use: an import cycle warns and still compiles
- 005 — use: use of a file that does not exist
- 006 — use: use "@/..." resolves against the standard library
- 007 — mod: a module function is called with ::
- 008 — mod: nested modules: a::b::f()
- 009 — mod: an enum inside a module, member read with ::
- 010 — mod: a module may be referenced before it is declared
- 011 — mod: a module function calls a sibling unqualified
- 012 — mod: a struct declared in a module, used as mod::Type
- 013 — mod: a pub static method on a struct inside a module
- 014 — mod: calling a module member with . is an error
- 015 — mod: an unknown member of a known module is an error
- 016 — mod: only declarations are allowed in a module body
- 017 — mod: a module call is arity-checked
- 018 — os: argc and argv from the global os
- 019 — use: use missing opening quote
- 020 — use: use with unclosed quote
- 021 — use: link missing opening quote
- 022 — use: link with unclosed quote

---

## 001 — use: a relative use pulls in a sibling file

```ura
// stdout: help() = 7
// expect: 0
// ---- helper.ura
fn help() i32:
    return 7

// ---- 001.ura - a relative `use` pulls in a sibling file
use "helper"

fn main() i32:
    output("help() = ", help(), "\n")
    return 0
```

---

## 002 — use: use resolves against the importing file, including a subdirectory

```ura
// stdout: total() = 7
// expect: 0
// ---- sub/deep.ura
fn deep() i32:
    return 3

// ---- side.ura
fn side() i32:
    return 4

// ---- mid.ura
use "sub/deep"
use "side"

fn total() i32:
    return deep() + side()

// ---- 002.ura - `use` resolves against the importing file
use "mid"

fn main() i32:
    output("total() = ", total(), "\n")
    return 0
```

---

## 003 — use: a diamond loads the shared file once, and says nothing

```ura
// stdout: 6 7
// expect: 0
// ---- dbase.ura
fn base() i32:
    return 5

// ---- d1.ura
use "dbase"

fn one() i32:
    return base() + 1

// ---- d2.ura
use "dbase"

fn two() i32:
    return base() + 2

// ---- 003.ura - a diamond loads the shared file once
use "d1"
use "d2"

fn main() i32:
    output(one(), " ", two(), "\n")
    return 0
```

---

## 004 — use: an import cycle warns and still compiles

```ura
// stdout: 3
// stderr: warning: import cycle: 'c1.ura' imports itself through 'c2.ura'
// expect: 1
// ---- c1.ura
use "c2"

fn ping() i32:
    return 1

// ---- c2.ura
use "c1"

fn pong() i32:
    return 2

// ---- 004.ura - an import cycle warns and still compiles
use "c1"

fn main() i32:
    output(ping() + pong(), "\n")
    return 0
```

---

## 005 — use: use of a file that does not exist

```ura
// error: Cannot find file 'nope.ura'
// 005.ura - `use` of a file that does not exist

use "nope"

fn main() i32:
    output("unreachable\n")
    return 0
```

---

## 006 — use: use "@/..." resolves against the standard library

```ura
// stdout: isdigit 1
// stdout: isalpha 0
// stdout: stdlib parsed
// expect: 0

use "@/memory"
use "@/ctype"
use "@/io"

fn main() i32:
    output("isdigit ", isdigit('7' as i32), "\n")
    output("isalpha ", isalpha('7' as i32), "\n")
    output("stdlib parsed\n")
    return 0
```

---

## 007 — mod: a module function is called with ::

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

---

## 008 — mod: nested modules: a::b::f()

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

---

## 009 — mod: an enum inside a module, member read with ::

```ura
// stdout: 0 2
// expect: 0
mod col:
    enum Color:
        RED, GREEN, BLUE

fn main() i32:
    output(col::RED, " ", col::BLUE, "\n")
    return 0
```

---

## 010 — mod: a module may be referenced before it is declared

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

---

## 011 — mod: a module function calls a sibling unqualified

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

---

## 012 — mod: a struct declared in a module, used as mod::Type

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

---

## 013 — mod: a pub static method on a struct inside a module

```ura
// stdout: 3
// expect: 0
mod g:
    struct Point:
        x i32
        pub fn make(v i32) Point:
            p Point
            p.x = v
            return p

fn main() i32:
    p g::Point = g::Point::make(3)
    output(p.x, "\n")
    return 0
```

---

## 014 — mod: calling a module member with . is an error

```ura
// error: 'lib' is a module; reach its members with '::' instead of '.'
mod lib:
    fn foo() i32:
        return 1

fn main() i32:
    output(lib.foo(), "\n")
    return 0
```

---

## 015 — mod: an unknown member of a known module is an error

```ura
// error: Module 'lib' has no member 'bar'
mod lib:
    fn foo() i32:
        return 1

fn main() i32:
    output(lib::bar(), "\n")
    return 0
```

---

## 016 — mod: only declarations are allowed in a module body

```ura
// error: Only 'fn', 'struct', 'enum' and 'mod' can be declared in a module
mod lib:
    x i32 = 5

fn main() i32:
    output("hi\n")
    return 0
```

---

## 017 — mod: a module call is arity-checked

```ura
// error: Wrong number of arguments to 'foo'
mod lib:
    fn foo(a i32) i32:
        return a

fn main() i32:
    output(lib::foo(1, 2), "\n")
    return 0
```

---

## 018 — os: argc and argv from the global os

```ura
// stdout: argc: 1
// stdout: argv: 1
// expect: 0

fn main() i32:
    output("argc: ", os.argc, "\n")
    output("argv: ", os.argv.len, "\n")
    return 0
```

---

## 019 — use: use missing opening quote

```ura
// error: Expected '"' after 'use'
use @/header

fn main() i32:
    output("hi\n")
    return 0
```

---

## 020 — use: use with unclosed quote

```ura
// error: Unterminated 'use' path, expected closing '"'
use "@/header

fn main() i32:
    output("hi\n")
    return 0
```

---

## 021 — use: link missing opening quote

```ura
// error: Expected '"' after 'link'
// errors/lexer/008 — `link` missing opening quote
link header.h

fn main() i32:
    output("hi\n")
    return 0
```

---

## 022 — use: link with unclosed quote

```ura
// error: Unterminated 'link' path, expected closing '"'
// errors/lexer/009 — `link` with unclosed quote
link "header.h

fn main() i32:
    output("hi\n")
    return 0
```

