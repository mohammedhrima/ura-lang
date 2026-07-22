# ura-lang rewrite — master roadmap

Rewrite ura-lang feature by feature, full pipeline each. Nothing gets skipped, nothing gets forgotten.

**Rules**
- Every feature = full pipeline: lexer → parser → `analyze` → `type_check` → `code_gen` (dispatcher case + helper in its stage file, like `lex_*` in `frontend/lexer.c`) → tests.
- Tests per feature: golden `.ll` (success), `.err` (compile error), `.run` (runtime) — `uv run config/tasks.py tests` must stay green.
- Check `old.c` first for every old feature — port the intent, not the bugs.
- One feature per commit. Update README/ROADMAP when a feature lands.
- Points: 1 pt ≈ 4h.

## M2 — control flow
- [ ] block scoping + shadowing golden (1) — scoping works; struct shadowing is covered (structs 011), variable shadowing golden still missing

## M4 — globals, memory, type utilities
- [ ] global variables (lift top-level fn-only restriction; LLVM globals) (3)
- [ ] design: explicit `stack` keyword — keep or drop (1)

## M5 — aggregates
- [ ] tuples + destructuring (4)

## M6 — collections & String
- [ ] List[T] (5)
- [ ] Map[K, V] (6)
- [ ] NEW string interpolation (3)

## M7 — modules & linking
- [ ] mod namespaces (4) — `mod` is a reserved word with no parser; `::` lexes but is unused. `.` is taken by field access and `a.b(...)` does not parse at all yet
- [ ] wire lex_link; URA_LINK external linking (3) — `link "..."` parses and discards the path, so raylib cannot be linked
- [ ] multi-file CLI: `ura a.ura b.ura` silently compiles only the last file (1)

## M8 — error handling & safety
- [ ] assert / panic keywords on the existing trap machinery (guard_nonzero generalizes) (2)
- [ ] Result / Option — enums now exist, build on them (6)
- [ ] design decision: try/catch — tokens commented out; Result direction suggests DROP (0)

## M9 — language polish
- [ ] const / immutability (3)
- [ ] type inference `:=` (4)
- [ ] type aliases (2)
- [ ] fn overloading (3) · default param values (2) · named returns (2)
- [ ] struct embedding (4)
- [ ] @if conditional compilation (3)
- [ ] generics — LAST, everything monomorphizes (12)

## M10 — stdlib & tooling (continuous)
- [ ] std core: string (5) — commented down to its 37 protos; its blockers (`char[]` indexing, `pub`, `operator`, `::`) now exist
- [ ] std extended modules (math/time/os/net/…) (8) — blocked, each commented with its reason:
      `@if` (dirent, errno, fcntl, net, signals, stat, unistd) · globals (os, raylib, + 105 constants) · f64 (math, atof/strtod) · time
- [ ] regression tests for old bug list (2)
- [ ] docs refresh per milestone (3) · vscode-extension sync (2)
- [ ] post-1.0: formatter (9) · LSP (13) · package manager on tasks.py/uv (12)

## Design decisions ledger (open)
- explicit stack/heap keywords vs implicit — decide at M4
- try/catch vs Result/Option — decide at M8
- the guard should throw an error
- check if all features are implemented
- handle globals — still open, and now the single biggest blocker in ura-lib (105 constants + `os`/`raylib` need it)
- implement input