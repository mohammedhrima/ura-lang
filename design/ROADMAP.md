# Ura Language Roadmap

> See [README.md](README.md) for the full picture (status, navigation, links).
> See [IMPROVEMENTS.md](IMPROVEMENTS.md) for architectural workstreams.

> `+` implemented · `-` not yet · `~` partial · `⊘` deferred

## Table of Contents

- [Variables & Types](#variables--types)
- [Operators](#operators)
- [Control Flow](#control-flow)
- [Functions](#functions)
- [Structs](#structs)
- [Modules](#modules)
- [Memory](#memory)
- [Builtins](#builtins)
- [C Interop & Linking](#c-interop--linking)
- [Standard Library](#standard-library)
- [Error Handling](#error-handling)
- [Filesystem & IO](#filesystem--io)
- [Concurrency](#concurrency)
- [Tooling](#tooling)
- [Distribution](#distribution)
- [Projects & Benchmarks](#projects--benchmarks)

---

## Variables & Types

+ primitive types: int, long, short, char, chars, bool, float, void
+ arrays: array[type]  (stack and heap)
+ global variables (primitives + structs)
+ type casting: x as int
+ sizeof, typeof builtins
+ enums (enum Name: V1, V2 ...)
+ tuples / multiple return values: (int, int)
+ built-in String type (struct with len, push, pop, find, upper, lower, trim, replace, reverse, operators)
- const / immutable variables
- type inference (auto keyword or :=)
- type aliases (type Byte = char)
- string interpolation ("Hello, {name}!")
- nullable / optional types

---

## Operators

+ arithmetic: + - * / %
+ comparison: == != < > <= >=
+ logical: && || !
+ bitwise: & | ^ ~ << >>
+ assignment: = += -= *= /= %=
+ index: []
+ access: . (struct field / method)
+ operator overloading (=, +, -, *, /, %, ==, !=, <, >, <=, >=, +=, -=, *=, /=, %=, &, |, ^, <<, >>)
+ static dispatch: :: (Type::method())
- range operator: 0..10

---

## Control Flow

+ if / elif / else
+ while loop
+ break / continue
- for loop (classic: for i = 0; i < n; i++)
- for-in / range loop (for i in 0..10)
- switch / case
- pattern matching (match expr with enum variants)
- try / catch / finally

---

## Functions

+ declaration: fn name(params) ret:
+ value parameters
+ reference parameters (ref)
+ multiple return values (tuples)
+ variadic functions (proto only, for C interop)
+ forward references (functions/methods can be called before declaration)
+ pub fn: static methods inside structs (no self, called via Type::method())
- function overloading / polymorphism
- default parameter values (fn greet(name chars = "World") void)
- closures / lambdas
- named return values

---

## Structs

+ struct declaration
+ struct fields
+ struct methods (fn method() with implicit self ref)
+ pub fn new() explicit constructor (static, called via Type::new())
+ operator delete() destructor (auto-called at scope exit)
+ operator overloading (=, +, -, *, /, ==, !=, <, >, +=, etc.)
+ static dispatch: Type::method() for pub fn
+ nested structs (struct as field of another struct)
+ global struct instances
+ zero-initialization by default (no auto-init, use pub fn new() for explicit construction)
+ forward method references (methods can call each other regardless of declaration order)
- inheritance / struct embedding
- templates / generics
- clone (deep copy) vs assign (shallow copy)

---

## Modules

+ mod name: block (namespace by indentation, like fn/struct)
+ mod block: functions namespaced as "module.fn"
+ intra-module calls (call sibling without qualification)
+ use "file" imports file as flat namespace (pre-module)
- module static dispatch: std::strlen() (planned, use :: syntax)
- nested modules: mod outer: mod inner: fn foo() -> "outer.inner.foo"
- mod-qualified struct types: p math.Point
- re-export: use inside mod inherits module namespace
- import loop detection (circular use guard)
- selective import: use "file" only fn_name

---

## Memory

+ stack allocation: stack[type](n)
+ heap allocation: heap[type](n)
+ references: ref (C++ style, bind at declaration)
+ pointers: ptr
+ manual free
+ auto-destructor: operator delete() called at scope exit for struct variables
- smart pointers
- garbage collection (optional / opt-in)
- strict mode, to protect user from doing stupid stuff like declaring null structs

---

## Builtins

+ output (print without needing printf)
+ sizeof(expr)
+ typeof(expr)
- assert(cond, "message")
- len(array) (array.len attribute)
- input / read from stdin

---

## C Interop & Linking

+ proto fn (declare external C function)
+ proto fn variadic (e.g. printf)
+ linking external libraries (-l flag)
+ tested: raylib, pthreads, socket API
- linking with C++ libraries
- auto-detect system libraries

---

## Standard Library

+ io (fopen, fclose, printf, scanf, ...)
+ math (floor, ceil, sqrt, ...)
+ memory (malloc, free, memset, ...)
+ string (strlen, strcmp, strcpy, strjoin, ...)
+ String struct (new, from, from_int, assign, join, push, pop, find, contains, starts_with, ends_with, substr, upper, lower, trim, replace, repeat, reverse, to_int, compare, operators)
+ time (clock, time, ...)
+ net (socket, bind, listen, accept, ...)
+ os (argc, argv, exit, getenv, ...)
+ signals (signal, raise, ...)
+ stdlib (rand, atoi, ...)
- collections (list, map, set)
- json parser
- http client
- package manager (config file, install, versioning)

---

## Error Handling

- try / catch / finally
- error codes (Result type)
- assertions
- panic / unreachable

---

## Filesystem & IO

+ import files (use "file")
- open / read / write files
- directory management
- stdin input builtin
- stderr output (errput)

---

## Concurrency

- threads
- atomic operations
- async / await (future)
- channels (future)

---

## Tooling

+ compiler (ura <file>)
+ build system (config.sh: build, tests, update_tests, copy, install)
+ VS Code extension (syntax highlighting for pub fn, operator delete, ::, etc.)
+ test runner (parallel, with IR comparison)
+ -no-debug flag (suppress debug output)
+ -no-exec flag (compile only, no execution)
- formatter (auto-format .ura files)
- language server / LSP (autocomplete, go-to-def)
- REPL (interactive interpreter)
- debugger integration
- build ura-lang compiler using ura-lang itself

---

## Distribution

+ install for macOS
- install for Linux
- install for Windows (partial via MSYS2)
- package manager / registry
- cross-compilation

---

## Projects & Benchmarks

+ 42 libft reimplemented in Ura
+ TCP server: basic chat variant
+ TCP server: command-based variant
+ raylib demos (window, shapes, textures, animation)
+ String test suite (constructors, mutators, search, transforms, operators, conversions)
- donut renderer
- sample CLI tool
- HTTP server
- WebGPU / OpenGL demo
- stock trading simulation (C++ reference)
- build ura-lang compiler using ura-lang language

---

## Architectural Improvements

Higher-level cleanups that take the compiler from ~70/100 to ~85. Detail in [IMPROVEMENTS.md](IMPROVEMENTS.md).

+ ① error recovery (multi-error per run with `parser_recover` at sync points)
~ ② exhaustive semantic checks (7 done — return types, arg counts, struct casts, fn redefinition, etc.)
- ③ multi-pass architecture (split gen_ir into resolve / typecheck / codegen)
- ④ language spec (grammar.ebnf, tour.md, stdlib.md)
⊘ ⑤ incremental compilation (deferred — not useful at current scale)
