# Ura Language Roadmap

> `+` implemented · `-` not yet

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
+ global variables (primitives + structs; init/clean auto-called in main)
+ type casting: x as int
+ sizeof, typeof builtins
+ enums (enum Name: V1, V2 ...)
+ tuples / multiple return values: (int, int)
- const / immutable variables
- type inference (auto keyword or :=)
- type aliases (type Byte = char)
- string interpolation ("Hello, {name}!")
- built-in string type (struct with len, data, operator overloads)
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
- operator overloading (for structs like matrix, string)
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
+ reference parameters (&)
+ multiple return values (tuples)
+ variadic functions (proto only, for C interop)
+ forward function use (function used before declaration in same file)
- function overloading / polymorphism
- default parameter values (fn greet(name chars = "World") void)
- closures / lambdas
- named return values

---

## Structs

+ struct declaration
+ struct fields
+ struct methods (fn method(self &StructName))
+ fn init() constructor (auto-called on allocation)
+ nested structs (struct as field of another struct)
+ global struct instances (init auto-called in main)
- operator overloading (==, +, etc. on struct)
- inheritance / struct embedding
- templates (fn foo[T](v T))
- generics (if not templates)
- clone (deep copy) vs assign (shallow copy)

---

## Modules

+ mod name: block (namespace by indentation, like fn/struct)
+ mod block: functions namespaced as "module.fn"
+ intra-module calls (call sibling without qualification)
+ use "file" imports file as flat namespace (pre-module)
- nested modules: mod outer: mod inner: fn foo() -> "outer.inner.foo"
- mod-qualified struct types: p math.Point
- re-export: use inside mod inherits module namespace
- import loop detection (circular use guard)
- selective import: use "file" only fn_name

---

## Memory

+ stack allocation: stack[type](n)
+ heap allocation: heap[type](n)
+ references: & (C++ style, bind at declaration)
+ pointers: ptr
+ manual free
- auto-free for heap (scope-based or ref-counted)
- smart pointers
- garbage collection (optional / opt-in)

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
+ string (strlen, strcmp, strcpy, ...)
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
+ build system (config.sh: build, tests, install)
+ VS Code extension (syntax highlighting)
+ test runner (tests command with IR comparison)
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
- donut renderer
- sample CLI tool
- HTTP server
- WebGPU / OpenGL demo
- stock trading simulation (C++ reference)
- build ura-lang compiler using ura-lang language
