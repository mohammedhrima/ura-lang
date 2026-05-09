# Ura — design overview

The full picture in one page. Everything that exists, everything we're planning. Click through for detail.

---

## What Ura is

Ura is a compiled, statically-typed language with Python-style indented syntax. Programs go through a hand-written tokenizer + recursive-descent parser, an IR pass that does name resolution and type checking, then an LLVM backend that emits native code via `clang`. Written in C, lives in `src/`.

```
source.ura  →  tokenize  →  parse (AST)  →  gen_ir  →  LLVM IR  →  native binary
```

---

## Status

```
COMPILER
├── ~5.7K LoC across main.c / parse.c / gen.c / utils.c / header.h
├── full pipeline: parse → IR → LLVM → native
├── Rust-style error messages with source-line + carat
├── multi-error reporting (parser recovers and continues)
└── ~70/100 maturity (single-developer compiler territory)

TESTS
├── ~140 positive fixtures across ~25 feature categories
└──  72 negative fixtures across 14 error categories
   (anvil test errors → all pass; anvil test → full positive suite passes)

TOOLING
├── anvil — build system + test runner (config/anvil/)
├── VSCode extension (config/vscode-extension/)
└── ASan-instrumented build is on by default
```

---

## Implemented features

Full inventory in [ROADMAP.md](ROADMAP.md). Categories at a glance:

| Category | What's there |
|---|---|
| [Variables & Types](ROADMAP.md#variables--types) | int, long, short, char, chars, bool, float, void, arrays, tuples, enums, built-in `String` |
| [Operators](ROADMAP.md#operators) | arithmetic, comparison, logical, bitwise, assignment, indexing, access, **operator overloading**, `::` static dispatch |
| [Control Flow](ROADMAP.md#control-flow) | `if` / `elif` / `else`, `while`, `break` / `continue` |
| [Functions](ROADMAP.md#functions) | declarations, ref params, multi-return, variadic (`proto` only), forward refs, `pub fn` static methods |
| [Structs](ROADMAP.md#structs) | fields, methods (implicit `self`), `pub fn`, `operator delete` destructor, operator overloading, nested structs |
| [Modules](ROADMAP.md#modules) | `mod name:` blocks, namespaced functions, **`::` access** (e.g. `math::add(2, 3)`) |
| [Memory](ROADMAP.md#memory) | `stack[T](n)`, `heap[T](n)`, refs (C++-style), pointers, manual `free`, auto-destructor on scope exit |
| [Builtins](ROADMAP.md#builtins) | `output`, `sizeof`, `typeof` |
| [C Interop](ROADMAP.md#c-interop--linking) | `proto fn` (declares external C symbols), `link "lib.so"`, variadic interop, raylib + pthreads + sockets tested |
| [Standard Library](ROADMAP.md#standard-library) | io, math, memory, string, `String` struct, time, net, os, signals, stdlib |

---

## Architectural improvements

Five workstreams that take Ura from ~70/100 to ~85. Full detail in [IMPROVEMENTS.md](IMPROVEMENTS.md).

| # | Item | Status |
|---|---|---|
| ① | [Error Recovery](IMPROVEMENTS.md) — multi-error per run, `parser_recover` at sync points | **Done** |
| ② | [Exhaustive Semantic Checks](IMPROVEMENTS.md) — bool conditions, infinite types, break-outside-loop, etc. | Partial (7 done, ~10 remaining) |
| ③ | [Multi-Pass Architecture](IMPROVEMENTS.md) — split gen_ir into resolve + typecheck + codegen | Not started |
| ④ | [Language Spec](IMPROVEMENTS.md) — grammar.ebnf, tour.md, stdlib.md | Not started |
| ⑤ | [Incremental Compilation](IMPROVEMENTS.md) — module-level caching | Deferred |

---

## Future deliverables (planned, not yet started)

These are placeholders. They'll get filled in under item ④ in [IMPROVEMENTS.md](IMPROVEMENTS.md).

```
design/
├── grammar.ebnf  — formal EBNF grammar (lexical + syntactic)
├── tour.md       — language tour, ~10 sections with examples
├── stdlib.md     — reference for every public function in src/ura-lib/
└── decisions/    — design-decisions log
                   • ref-binding redesign (paused mid-discussion)
                   • returning refs from functions (open question)
```

---

## How to navigate the project

```
ura-lang/
├── src/                    compiler source (main.c, parse.c, gen.c, utils.c, header.h)
│   ├── ura-lib/            standard library, written in Ura
│   └── tests/
│       ├── <feature>/      positive tests per feature
│       └── errors/         negative tests (parse + sema errors)
├── config/
│   ├── anvil/              build system + test runner
│   └── vscode-extension/   syntax highlighting + IDE integration
├── design/                 ← you are here
│   ├── README.md           this file
│   ├── ROADMAP.md          feature checklist (+ implemented, − planned)
│   ├── IMPROVEMENTS.md     5-item architectural roadmap
│   ├── grammar.ebnf        (planned)
│   ├── tour.md             (planned)
│   └── stdlib.md           (planned)
└── README.md               project-root overview (build/install)
```

---

## TL;DR

- **Want to know what Ura can do today?** → [ROADMAP.md](ROADMAP.md)
- **Want to know where Ura is going?** → [IMPROVEMENTS.md](IMPROVEMENTS.md)
- **Want to see the whole thing in one scroll?** → you're already there.
