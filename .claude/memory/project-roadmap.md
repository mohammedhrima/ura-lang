---
name: project-roadmap
description: "ura-lang state and order of work (updated 2026-11-19): what is done, what is next, decided designs, later ideas"
metadata:
  node_type: memory
  type: project
  originSessionId: 51d91c0b-dbf0-45f3-b976-edcd7cd673d8
  modified: 2026-11-19T07:30:46.009Z
---

Branch `refactor` (local commits, not pushed). ura-lib is a submodule at projects/ura-lib; the raytracer is projects/raytracer.

**Done (2026-11-17..19):** structs, refs, methods, templates (a template instance renames every fn it declares), inheritance `struct A from B`, ownership (ONBOARDING.md §8), packages `use "@/x"`, @if macros, ura init/build/run/install/remove, flags -o -ll -S -c -help -version (one dash only), bitwise & | ^ << >>, enum (`Type::NAME`), match/case/default, try / typed `catch e Type:` / throw of any type (ucg error-flag model, ucg_goto_catch, uncaught errors call core's `fn uncaught(error &Error) i32`), ucg_clean (ucg runs drops on every block exit), literals are @str globals, a literal "abc" is a core String (raw char[] where a char[] is expected), core package `core.ura` auto-loaded (alloc<T>/copy<T>/free<T> over builtins memory_alloc/memory_copy/memory_free, Array<T>, String, Error with `message String`), function overloads, a fn replaces a proto with the same signature, `is` = `==`, operator symbols from `binary_ops`, parallel `tests`.

**Decided 2026-11-19 (later):**
- Python-like core API: append, insert(0, x), pop() / pop(index), first/last, len + size, is_empty, chars() (not c_str), trim stays; String::create(number) overload (constructors are always create).
- `use` at top level = package, loaded once; `use` indented (inside a struct/fn) or inside a used file = the file's tokens copied in place (blind), circular use is an error; a relative path resolves next to the file that has the `use`.
- ura-lib core split: core.ura uses ./core/{memory,array,string,error}; array/{memory,modify,access,operators}, string/{create,search,transform,operators}. Inside a template a bare `Array` means the instance.
- `output` prints a struct (Name{a: 1}), refs as `ref`; the full dump (null, [Circular], arrays, operator fmt) waits for fmt; f"..." strings planned as sugar over fmt; colours as io::red etc.; output/errput go away in Phase 3.
- isNullable becomes isZero; T[N] arrays and .len in Phase 2; an unbound `r &i32` starts as null.
- Phase 1 suites done (2026-11-19): m1, m2, m3, m6 (String), m8, m11, m13; m10 is all for-loops (Phase 2); m13 projects moved to Phase 6. alloc<T> is now new<T>; ret removed; @no-warn/warnings dropped; two protos with one name and other params = error (use ...). operator drop must be safe on zeros; C++-style assignment (operator = in place + move) is in Phase 4. Next Phase 1 items: audit src/old/*.c vs main.c, then the TODO comments in main.c/ucg.c.

**Plan (agreed 2026-11-19), one phase at a time, one commit per item:**
- Phase 1 (now) test cleanup: m1 groups (A crashes, E bugs, C error tests done; D old syntax nearly done; B better error messages next), then m2, m3, m6, m8, m10, m11, m13 (build missing feature or delete a dropped-feature test, re-record good messages/IR). Audit src/old/*.c features vs main.c. Go through the TODO comments in main.c/ucg.c.
- Phase 2 language: array literals [1, 2, 3]; `for x in v` via `operator in(x &T) bool` (no 0..n, no loop); `null` literal only; `mod name:` + `::`; ura variadic functions with access to the extra args; tuple type (multi-return, `a, b = f()`); function values/callbacks; `any` (undecided, template-like).
- Phase 3 fmt/io: `fmt` param type (`fn out(text fmt)`) + `operator fmt`; io::out/io::err/io::input in core, remove output/errput builtins; `alias name = fn` + Stream API (files, sockets); scan (reverse fmt).
- Phase 4 memory/safety: bitmap arena (1 bit per slot, 1 = used) behind alloc/free, default allocator of every ura program; -O0..-O3/-Os/-Oz; -san that reports the ura file:line (DWARF debug info from ucg).
- Phase 5 tooling: ura.project YAML (packages, flags, commands, custom `ura <cmd>`); `ura watch` (rebuild+rerun on change, like nodemon); `ura -tree`; Makefile replaces config.sh, build ucg alone (maybe own repo); installing ura installs ura-lib.
- Phase 6 projects: rewrite raytracer with new features, rewrite ura-tcp-server; later http lib, backend framework, `ura init SDL2`.
- Phase 7 release (only on the user's word): merge refactor -> dev -> main, push ura-lib, rewrite git history and drop removed files to shrink the repo (see [[project-end-tasks]]).
Skipped: cross-platform link.

Repos (GitHub project users/mohammedhrima/projects/4): ura-lang, ura-lib, ura-vscode-extension, ura-libft, ura-tcp-server. `gh` here needs `GODEBUG=netdns=cgo`.

**Why:** the user sets the order and changes it often; keep this file current.
**How to apply:** work only on "Now"; ask before starting "Next". See [[feedback-step-by-step]], [[feedback-user-runs-tests]].
