# ura-lang rewrite — master roadmap

Rewrite ura-lang feature by feature, full pipeline each. Nothing gets skipped, nothing gets forgotten.

**Rules**
- Every feature = full pipeline: lexer → parser → `analyze` → `type_check` → `code_gen` (dispatcher case + helper in utils.c, like `lex_*`) → tests.
- Tests per feature: golden `.ll` (success), `.err` (compile error), `.run` (runtime) — `uv run tasks.py tests` must stay green.
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
- [ ] bool literals — `True`/`False` already lexed (lex_identifier special case); remaining: parser → type_check → code_gen (i1) → tests (2)
- [ ] comparison `== != < > <= >=` + `is` → BOOL (precedence + icmp) (2)
- [ ] logical `and or not` with short-circuit blocks (3)
- [ ] unary `- not ~` in prime_node (2)
- [ ] long/short end-to-end + design: int-literal widening rules (2)
- [ ] float literals end-to-end (f32 in to_llvm_type, fadd/fcmp) (3)
- [ ] char literals end-to-end (2)
- [ ] compound assignment `+= -= *= /= %=` (desugar to binop + store) (2)
- [ ] bitwise `& | ^ ~ << >>` (2)
- [ ] precedence table complete for ALL ops + mixed-precedence golden (1)

## M2 — control flow
- [ ] if / elif / else (3)
- [ ] while (2)
- [ ] break / continue (2)
- [ ] for (to/step) (3)
- [ ] block scoping + shadowing golden (1)
- [ ] NEW range op `0..10` (`..` not lexed yet) + range-for (3)
- [ ] NEW switch / case (no keywords yet) (3)

## M3 — strings & I/O (the real hello-world; do EARLY)
- [ ] chars (string) literals end-to-end (global constants) (2)
- [ ] minimal `proto` (extern fn + variadic) → printf/puts/malloc callable (3)
- [ ] printf C-interop test, then `output()` builtin (add keyword) (4)
- [ ] chars operations basics (3)
- [ ] hello-world golden (closes Foundation) (1)

## M4 — globals, memory, type utilities
- [ ] global variables (lift top-level fn-only restriction; LLVM globals) (3)
- [ ] heap alloc + free (malloc/free via proto) (3)
- [ ] design: explicit `stack` keyword — keep or drop (1)
- [ ] casting `as` (3)
- [ ] typeof / sizeof (2)
- [ ] design: local ref bindings + ref returns (dangling policy) (2)

## M5 — aggregates
- [ ] arrays: 1D → multi-dim → literals → indexing + bounds design (8)
- [ ] structs: decl + fields (3) → field access/GEP + nested (3) → forward refs/opaque (2) → methods `self` (3) → static `::` (2) → `operator =` copy (3) → `delete` destructor (3)
- [ ] enums (3)
- [ ] tuples + destructuring (4)
- [ ] NEW pattern matching `match` over enums (5)

## M6 — collections & String
- [ ] List[T] (5)
- [ ] Map[K, V] (6)
- [ ] String struct + methods (5)
- [ ] NEW string interpolation (3)

## M7 — modules & linking
- [ ] wire lex_use into tokenize; `use "file"` multi-source (4)
- [ ] `use "@/module"` (2)
- [ ] mod namespaces (4)
- [ ] wire lex_link; URA_LINK external linking (3)

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
- [ ] std core modules (io/memory/string/stdlib) compile again (8)
- [ ] std extended modules (math/time/os/net/…) (8)
- [ ] regression tests for old bug list (2)
- [ ] docs refresh per milestone (3) · vscode-extension sync (2)
- [ ] post-1.0: formatter (9) · LSP (13) · package manager on tasks.py/uv (12)

## Design decisions ledger
- closures: **stays dropped** — explicit-state model; capture is a compile error
- int-literal widening (long/short/float targets) — decide at M1
- local refs / ref returns — decide at M4
- explicit stack/heap keywords vs implicit — decide at M4
- try/catch vs Result/Option — decide at M8
