# ura-lang rewrite — master roadmap

Rewrite ura-lang feature by feature, full pipeline each. Nothing gets skipped, nothing gets forgotten.

**Rules**
- Every feature = full pipeline: lexer → parser → `analyze` → `type_check` → `code_gen` (dispatcher case + helper in its stage file, like `lex_*` in `frontend/lexer.c`) → tests.
- Tests per feature: golden `.ll` (success), `.err` (compile error), `.run` (runtime) — `uv run config/tasks.py tests` must stay green.
- Progress meter: the old suite's SKIP count (129 today) → 0.
- Check `old.c` first for every old feature — port the intent, not the bugs (`short` was a 2-bit int; exception scaffolding was dead).
- One feature per commit. Update README/ROADMAP when a feature lands.
- Points: 1 pt ≈ 4h.

## M0 — done
- [x] Foundation: tasks.py (uv) build/test shell (anvil removed) · CLI `-o -debug -san -O0..Oz -exec` + usage error · caret diagnostics + multi-error recovery · AST + per-pass dispatchers · LLVM init/finalize + `-O` passes + triple · golden harness (14/9/3 green)
- [x] int literals end-to-end · variables (`a int = v`, bare zero-init, scoped) · simple assignment
- [x] arithmetic `+ - * / %` with precedence · divide/mod-by-zero runtime trap (file:line)
- [x] functions: decl/call (with/without params) · by-value · ref params · returns + default return · recursion · forward refs · first-class fn values + indirect calls · nested fns + no-implicit-capture rule
- [x] `-san` (ASan/UBSan + DWARF per-statement lines) · `-exec` (run + cargo-style timing)

## M1 — scalar core (everything depends on this)
- [x] bool literals — `True`/`False`, i1, end-to-end
- [x] comparison `== != < > <= >=` → BOOL (precedence + icmp)
- [x] logical `and or not` (eager LLVMBuildAnd/Or)
- [x] unary `- not ~` in prime_node + parenthesized grouping
- [x] long/short end-to-end + design: int-literal widening → require explicit cast
- [x] float literals — declare/print/return/zero-init, f32 in to_llvm_type
- [x] float arithmetic + comparison — fadd/fsub/fmul/fdiv/frem, ordered fcmp; float ÷0 traps; bitwise stays int-only
- [x] char literals end-to-end
- [x] compound assignment `+= -= *= /= %=` (load-modify-store, reuses guard_nonzero)
- [x] bitwise `& | ^ ~ << >>`
- [x] precedence table complete for ALL ops + mixed-precedence golden (1)

## M2 — control flow
- [x] if / elif / else (3)
- [x] while (2)
- [x] break / continue (2)
- [x] for — over a range and over an array (`for i in 0..5`, descending `5..0`, `for x in arr`, `for ref x in arr`)
- [ ] block scoping + shadowing golden (1) — scoping works; struct shadowing is covered (structs 011), variable shadowing golden still missing
- [x] NEW range op `0..10` + range-for
- [x] match / case / default — scalar switch, multi-value cases, `break` exits match (3)

## M3 — strings & I/O (the real hello-world; do EARLY)
- [x] chars (string) literals end-to-end (global constants)
- [x] minimal `proto` (extern fn + variadic) → printf/calloc/free callable; emits a real LLVM declare, linked by clang
- [x] printf C-interop test, then `output()` builtin
- [ ] chars operations basics (3) — only via `proto` (strlen etc.); no native ops, and `chars` is not indexable
- [x] hello-world golden (closes Foundation)

## M4 — globals, memory, type utilities
- [ ] global variables (lift top-level fn-only restriction; LLVM globals) (3)
- [ ] heap alloc + free (malloc/free via proto) (3)
- [ ] design: explicit `stack` keyword — keep or drop (1)
- [ ] casting `as` (3)
- [ ] typeof / sizeof (2)
- [ ] design: local ref bindings + ref returns (dangling policy) (2)

## M5 — aggregates
- [x] arrays: 1D → multi-dim → literals → indexing + bounds (stack `T[n]`, heap `new T[n]`, slices, `clean`)
- [x] structs: decl + fields → field access/GEP + nested → forward refs → any scope → arrays of structs (stack/heap/2D) → `ref?` fields (linked lists) → params/returns → value copy → `output(p)` field dump with cycle detection
- [ ] structs, remaining: methods `self` (3) → static `::` (2) → `operator =` copy (3) → `delete` destructor (3)
- [ ] enums (3)
- [ ] tuples + destructuring (4)
- [ ] NEW pattern matching `match` over enums (5)

## M6 — collections & String
- [ ] List[T] (5)
- [ ] Map[K, V] (6)
- [ ] String struct + methods (5)
- [ ] NEW string interpolation (3)

## M7 — modules & linking
- [x] wire lex_use into tokenize; `use "file"` multi-source — resolves against the importing file, any depth
- [x] `use "@/module"` — URA_LIB, else ura-lib beside the binary, else one level up; no system paths
- [x] import guard: realpath identity, diamond loads once, cycle warns instead of recursing
- [x] `common.ura` auto-loaded — printf/calloc/free/write/exit come from the stdlib, not from hardcoded get_or_declare
- [ ] mod namespaces (4) — `mod` is a reserved word with no parser; `::` lexes but is unused. `.` is taken by field access and `a.b(...)` does not parse at all yet
- [ ] wire lex_link; URA_LINK external linking (3) — `link "..."` parses and discards the path, so raylib cannot be linked
- [ ] multi-file CLI: `ura a.ura b.ura` silently compiles only the last file (1)

## M8 — error handling & safety
- [ ] assert / panic keywords on the existing trap machinery (guard_nonzero generalizes) (2)
- [ ] Result / Option (needs enums) (6)
- [ ] NEW nullable types (3)
- [ ] design decision: try/catch — tokens commented out; Result direction suggests DROP (0)

## M9 — language polish
- [ ] const / immutability (3)
- [ ] type inference `:=` (4)
- [ ] type aliases (2)
- [ ] operator overloading (port from old.c) (5)
- [ ] fn overloading (3) · default param values (2) · named returns (2)
- [ ] struct embedding (4)
- [ ] @if conditional compilation (3)
- [ ] generics — LAST, everything monomorphizes (12)

## M10 — stdlib & tooling (continuous)
- [x] std core: io / memory / stdlib / ctype compile again; `common.ura` added
- [ ] std core: string (5) — needs `chars` indexing, `pub`, `operator`, `::`; commented down to its 37 protos
- [ ] std extended modules (math/time/os/net/…) (8) — blocked, each commented with its reason:
      `@if` (dirent, errno, fcntl, net, signals, stat) · globals (os, raylib, + 105 constants) · f64 (math, atof/strtod)
- [ ] regression tests for old bug list (2)
- [ ] docs refresh per milestone (3) · vscode-extension sync (2)
- [ ] post-1.0: formatter (9) · LSP (13) · package manager on tasks.py/uv (12)

## Design decisions ledger
- closures: **stays dropped** — explicit-state model; capture is a compile error
- int-literal widening (long/short/float targets) — **decided: require explicit cast** (M1)
- local refs / ref returns — decide at M4
- explicit stack/heap keywords vs implicit — decide at M4
- try/catch vs Result/Option — decide at M8
- the guard should throw an error
- check if all feautres are implmented
- structs (M5) — **decided**:
    - copy: shallow (bitwise) until the user overloads `operator =`
    - `ref` fields: only as nullable `ref?` (zero-init = null, guarded reads)
    - scope: `struct` may be declared in any scope, not just top level
    - `output(p)`: auto field dump — `Player{name: "Aldric", hp: 100}`
    - recursive-by-value (`struct N: next N`) and mutual recursion are errors;
      self-reference through an array (`kids N[]`) is legal
    - a struct with no fields is an error
    - `struct` inside a function body must work (any scope)
    - lifetime is the user's business: no implicit deep copy, no implicit free.
      If the user defines a destructor we call it at end of scope, nothing more
    - global struct variables wait on M4 globals
- use keyword — **decided + done**:
    - a lexer-level textual include, not a parser construct: the imported
      file's tokens join the same stream
    - bare names, no namespace at the call site (`printf`, not `io.printf`);
      namespacing waits for `mod` and `::`
    - a file is identified by its realpath, so it is imported once however it
      is spelled; a repeat while still loading is a cycle and warns
    - the stdlib is found next to the binary, never under /usr
- avoid calling hardcoded calloc, printf, free in code — **done**: they are
  protos in `ura-lib/common.ura`, auto-loaded, resolved via `find_function`;
  `get_or_declare` is deleted
- handle globals — still open, and now the single biggest blocker in ura-lib
  (105 constants + `os`/`raylib` need it)
