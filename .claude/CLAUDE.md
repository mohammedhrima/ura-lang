# ura-lang: handoff for Claude Code

Claude Code loads this file by itself in every session of this repo. Read it fully before doing anything.
It was written on 2026-11-19, at commit `f61a7fa` of the `refactor` branch, when the user moved to another machine.

Other files in this folder:

| file | what it holds | how current |
|---|---|---|
| `.claude/CLAUDE.md` (this file) | how to work with the user, project state, plan, next step | current (2026-11-19) |
| `.claude/memory/*.md` | Claude's memory from the old machine, one rule per file, `MEMORY.md` is the index | current; same rules as section 1, with the reasons |
| `.claude/ONBOARDING.md` | older onboarding (2026-10-21): background on the language, history, laptop setup | old; check facts against the code |
| `.claude/SPECS.md` | measured machine specs, every LLVM version 12..23, platforms ura must run on | still valid |
| `TODO.md` (repo root) | the user's phased plan, edited by the user | current; the user's own words win |

On a new machine, Claude's own memory folder is empty. To restore the memory, copy `.claude/memory/*.md` into `~/.claude/projects/<this repo path with / replaced by ->/memory/`, or just read them from here.

---

## 1. How to work with this user (the most important part)

These rules come from explicit corrections. Breaking them makes the user angry and costs them time.

### Talking

- Keep replies short. Answer first. No recaps, no long lists of options, no walls of text.
- Explain designs with a small visual: a table, a before/after code block or an ASCII diagram, then one or two lines. The user said "avoid talking too much and use visual explanation".
- The user writes fast, informal English with typos. Read for intent.
- When proposing a design, show a short sketch and wait. The user corrects designs often, and quickly.
- The user often asks "what do you recommend?". Give one recommendation with the reason, not a survey.
- Ask before deciding anything that is the user's call (language syntax, naming, what to delete). Use plain, full questions.

### Code changes

- **Ask before editing C code** (`src/*.c`, `src/*.h`). The user approves each edit through the permission prompt. Editing `.md` tests and ura-lib `.ura` files is fine without asking.
- **Edit files only with the Edit/Write tools**, never with python, sed or heredoc scripts. One visible edit at a time. Exception: the user allowed one shell line-range move for 1300 lines of `run_test` (phase 6), and only after being asked.
- **Never `git stash`.** The user edits files (TODO.md, projects/, src/test/) while Claude works. For a baseline, use `git worktree add --detach <scratchpad>/base HEAD`.
- **Don't over-engineer.** The user rejected:
  - a `Builtin` struct with function pointers and flags (`takes_one`, `raw_args`, ...): too much machinery;
  - wrapper functions like `i32_type()` instead of `new_type(INT, 32, NULL)` inline;
  - include machinery for `use` (reverted; `use` now pastes tokens);
  - hardcoded hacks like `ura.scopes[ura.scopes_count - 2]`.
  There is almost always a smaller solution. Look for an existing table or helper first.
- **Prefer plain data tables.** The user liked the `Keyword` tables (`keywords[]`, `builtins[]`, `specials[]`): "beautiful readable code". Tables like `precedence[]`, `binary_ops[]` already answer questions (example: `is_operation(type)` is `precedence[type] && precedence[type] < precedence[AS]`).
- **Reuse what exists.** Most helpers already exist (`append`, `format`, `includes`, `find_*`, `ucg_*`). Extend them, never write a parallel mechanism.
- **Never add a field to Token, Node or another struct without asking.**

### Code style (match it exactly)

- ColumnLimit 87. Format with llvm-15's clang-format (`~/.local/opt/llvm-15/bin/clang-format`; llvm-12's is too old).
- A condition that doesn't fit one line is split into named bools first: `bool is_x = ...; bool has_y = ...; if (is_x && has_y)`. Never a multi-line `&&`/`||`, never a wrapped ternary.
- Long calls go Flutter-style: break after `(`, one argument per line, `)` alone.
- Every `add_error(` call puts each of its 4 arguments on its own line, even short ones.
- `// clang-format off` / `// clang-format on` around multi-label `case A: case B:` lines and around one-line cases.
- One-statement cases are one line: `case RETURN: llvm_ret(curr->to_ret); break;`, columns aligned, spaces only (no tabs).
- No comments above functions. Don't add comments to code. Don't delete the user's comments unless asked.
- Loops use early `continue` guards, not one combined `&&`.
- Named types: `typedef struct X X;` with the other forward typedefs at the top of main.c, `struct X {...}` next to the other struct definitions. Never an inline `typedef struct {...} X;`.
- Names follow the families already in use: `find_x`, `new_x`, `parse_x`, `check_x`, `codegen_x`, `is_x`, `x_of`, `ucg_x` (records a node), `llvm_x` (emits LLVM). When unsure, ask for the name.
- A keyword is a Type enum value (in ucg.c's enum and `to_string`) plus a row in a `Keyword` table. Never `strcmp2(token->name, "from")`.
- Constructors are always `Type::create(...)`, never make/from/new.

### Architecture rules

- **All errors go through `add_error(s, e, msg, help)`, called only from `check_*` functions in `src/check.c`** (plus `expect`, the parser's "consume or report"). Errors are stored in ucg (`ucg_add_error`, `ucg_check_errors`).
- **ura calls `ucg_*` only during codegen**, never from tokenize, parse or analyze. ucg is meant to become its own library.
- **LLVM API calls go only in `llvm_*` functions** in ucg.c. `ucg_*` functions never call `LLVMBuild...` directly.
- **Portable:** must build with LLVM 12, 15 and 21, and with clang on Linux, macOS and Windows. No GNU-only or POSIX-only calls without a portable path.

### Tests (the user runs them, not Claude)

- **Never run `tests`, `record` or `ucg` yourself.** Only build (`source src/config.sh; build`). For ucg, `_ura_build ucg_test.c ucg -DUCG=1` builds without running.
- After a batch of work, give **one block** of commands (record + tests), not a command after every message.
- No probe loops (check scripts over many tests): they cost too many tokens.
- Read the user's log once. The user saves output with: `tests > build-tests.log 2>&1` (run from the repo root).
- Every feature gets a test entry in `tests/milestoneN.md`; every new ucg feature gets a `case N:` in `run_test` in `src/ucg_test.c`.
- A pure refactor must not change any recorded IR or stderr. If a refactor needs re-recording, it is a bug, unless the change is an intended output change (then say so and list the tests).

### Git

- Commit messages: short, lowercase descriptions, like `same_params: one parameter list compare for overrides, proto clashes and duplicate fns`.
- **Never** put Claude or Anthropic attribution in commits or PRs: no `Co-Authored-By`, no "Generated with". The message ends with the description. (Also in the user's global `~/.claude/CLAUDE.md`.)
- Commit only when the user says, one phase per commit. Push only when the user says.
- Never merge to main, rewrite history or push without an explicit go (see section 8).

---

## 2. What ura is

ura is a programming language with readable, Python-like syntax (indentation, `fn`, `struct`, `:`), compiled to native code through LLVM. The compiler is C.

```ura
use "@/io"

struct Point:
    x i32
    y i32
    pub fn create(x i32, y i32) Point:
        p Point
        p.x = x
        p.y = y
        return p

fn main() i32:
    p Point = Point::create(1, 2)
    for i in Range::create(0, 3):
        output("i = ", i, " x = ", p.x, "\n")
    return 0
```

### Pipeline

```
tokenize (main.c)  ->  parse_prime / parse_expr (main.c)  ->  analyze + check_* (main.c, check.c)  ->  codegen (main.c)
                                                                                                         |
                                                                              ucg (ucg.c): an IR over LLVM, ucg_compile -> llvm_* -> LLVM
```

### Files

| file | lines | what |
|---|---|---|
| `src/main.c` | ~3280 | tokenizer, parser, analyze, codegen, CLI (`ura init/build/run/install`), tables (`keywords`, `builtins`, `specials`, `binary_ops`, `precedence`) |
| `src/check.c` | ~1780 | every `check_*` function and every error |
| `src/ucg.c` | ~2450 | ucg: the IR library over LLVM (`ucg_*` records, `llvm_*` emits, `ucg_compile`), the Type enum, `to_string` |
| `src/ucg_test.c` | ~1340 | `#include "./ucg.c"` plus `run_test` (ucg's own tests, `case 0..36`) and the test `main` |
| `src/package.c` | ~130 | `find_package(name, File *from)`: `@/x` packages and relative paths |
| `src/proto.h` | generated | prototypes, regenerated by `build` with Universal Ctags (`#if !UCG` around main.c/check.c/package.c protos) |
| `src/config.sh` | ~1070 | the task shell: `build`, `ucg`, `tests`, `record`, `copy`, ... |
| `src/lib/header.h` | | `expand(T, name)` (dynamic array fields: `name`, `name_count`, `name_size`), `zero_init`, `append`, arena allocator, `format`, `strcmp2` (true when equal) |
| `tests/milestone1..13.md` | | the test suites (format in section 4) |
| `projects/ura-lib` | | git submodule: the standard library (branch `new-syntax`) |
| `projects/raytracer` | | a demo project |
| `src/test/` | | the user's scratch ura project (`ura run` there) |

### Core library (projects/ura-lib, auto-loaded as `@/core`)

- `core.ura` uses `./core/{memory, array, string, error, range, os}`.
- `core/memory.ura`: `template<T>: fn new(count i32) T[]`, `copy<T>`, `free<T>`, over the builtins `memory_alloc`, `memory_copy`, `memory_free`. (These builtins can't become libc protos: the template renames its own `free`, so a `free(...)` call inside would recurse.)
- `core/array.ura`: `template<T>: struct Array` with `data`, `count`, `cap`; uses `./array/{memory, modify, access, operators}`. Inside a template a bare `Array` means the instance.
- `core/string.ura`: `struct String from Array<char>`; `./string/{create, search, transform, operators}`.
- Python-like API: `append`, `insert`, `pop()` / `pop(index)`, `first`, `last`, `len`, `size`, `is_empty`, `at`, `chars()` (not c_str), `trim`, `String::create(text)`, `String::create(number)`.
- `core/range.ura`: `Range::create(start, stop[, step])` with `operator in`.
- `core/os.ura`: `struct Os` with `argc i32`, `argv Array<String>`, and the global `os Os = Os::create()`.
- `core/error.ura`: `Error` with `message String`; core's `fn uncaught(error &Error) i32` prints uncaught errors.

---

## 3. Language features and decisions (what exists today)

| feature | syntax / rule |
|---|---|
| types | `i8 i16 i32 i64 u8..u64 f32 f64 bool char void`, `T[]` raw arrays, `&T` refs, `Array<T>`, `String` |
| literals | `2.5f` is f32; negative and wide int literals adapt to the target; a literal too big for i64 is an error |
| strings | a literal `"abc"` is a core String; a raw `char[]` where a char[] is expected |
| array literals | `[1, 2, 3]` is a raw `T[]` or an `Array<T>` decided by the target; nested, empty, indexed directly |
| structs | attributes, methods (`self.`), `pub fn` statics (`Type::create`), nested structs, local structs in blocks, `struct A from B` (inheritance, overrides win) |
| output | `output(...)` / `errput(...)` print anything; a struct prints `Name{a: 1}`; a String goes through `chars()` |
| refs | `r &i32 = &x`; `null` (Type ZERO) fits only `&T` or `T[]`; `r == null` compares the ref slot; an unbound ref starts null; `&ref_attr` gives the ref |
| `isZero(x)` | builtin, returns bool |
| for | `for x in v:` calls `v.operator in(i &i32, x &T) bool` with a hidden counter; `break` / `continue` work |
| enum | `enum Color: RED, GREEN`, read as `Color::RED` |
| match | `match x: case 1: ... default: ...` |
| try | `try:` / typed `catch e Type:` / `throw` of any type; uncaught errors in main exit 1 through core's `uncaught` |
| templates | `template<T>: struct ...` / `fn ...`; an instance renames every fn the template declares |
| ownership | named values copy through `operator =`, temporaries are moved, `drop(x)`, `operator drop` runs at block ends in reverse order, globals drop at the end of main; `operator drop` must be safe on zeroed values |
| `use` | top level `use "@/x"` = package, loaded once; an indented `use`, or one inside a used file, pastes the file's tokens in place; circular use is an error; a relative path resolves next to the using file |
| `mod` | `mod math:` prefixes the names declared at the top of its body with `math::` (in place on the tokens, `prefix_names`); `math::x` merges into one name (`parse_module_name`); nested mods; a mod may be used before its declaration (calls/values only, not types) |
| builtins | soft keywords from the `Keyword builtins[]` table: `output errput input sizeof typeof drop isZero memory_alloc memory_copy memory_free ura::argc ura::argv`; a name after `.` or `::` is never a builtin (`p.drop()`, `fn drop()` work); a call builtin without `(` gets `expected '(' after 'sizeof'` |
| `ura::argc` / `ura::argv` | values; ucg gives main `(argc, argv)` only when a program uses them (`ucg_argc`, `ucg_argv`) |
| unused package globals | a global declared in a package (not the main file) that nothing reads is not emitted (no declaration, initializer or drop) |
| keywords | `and`/`or` (AND_LIT/OR_LIT) and `&&`/`||` (AND/OR) both exist; `~` bitwise not; `is` means `==`; `self` keyword; `from`; `null` |
| macros | `@if os == "linux"`, `@elif`, `@else`, `@error "msg" "help"`, `installed("lib")` |
| protos | `proto name(...)`; two protos with one name and other params are an error (use `...`); a fn replaces a proto with the same signature |

Rejected or postponed by the user (don't propose again without being asked):
- `ret` keyword (only `return`), warnings / `@no-warn`, `??`.
- C++-style assignment (operator = in place + move): postponed to Phase 4.
- Symbol-heavy syntax like `out << x`.
- `strip` (keep `trim`), `c_str` (use `chars()`), `push_back` (use `append`), `alloc<T>` (now `new<T>`).

---

## 4. Build and tests

```bash
source src/config.sh      # loads the task shell (from the repo root or src/)
build                     # builds bin/ura (regenerates proto.h); -Werror
ucg [N]                   # builds bin/ucg from src/ucg_test.c and runs ucg test N (all when none)  [USER RUNS]
tests [milestoneN] [NNN]  # runs the suites in tests/  [USER RUNS]
record milestoneN NNN     # re-records one test's IR/stderr/status  [USER RUNS]
```

- `tests` with no argument runs every `tests/*.md`. It prints a file header and one line per test: green = pass, red with `(compile error, status N)`, `(llvm ir differs)` or `(stderr differs)` plus a short diff.
- Nothing writes `build-tests.log` automatically. The user redirects into it. A stale log misled Claude once (section 7).

### Test entry format (tests/milestoneN.md)

```markdown
## 023 — mod: short description

```ura
// stdout: expected line          (informational)
// expect: 0                      (informational)
// error: expected first error    (informational; the real check is the recorded stderr)
fn main() i32:
    ...
```

### llvm ir          (recorded by `record`; compared when the compile succeeds)
### stderr / ### status   (recorded for entries that must fail to compile)
```

- Each file starts with `# milestoneN` and an `## index` list (`- 023 — mod: ...`). Keep the index in sync.
- Deleted tests keep the other numbers (no renumbering).
- An entry with no recorded `llvm ir` section fails as soon as it compiles (the runner compares against empty IR).

### Current results (711 passed, 75 failed)

| suite | passed | failed | what the failures need |
|---|---|---|---|
| m1 | 256 | 1 | |
| m2 | 111 | 19 | sized arrays `T[N]`, VLA, `.len` on raw arrays (Phase 2) |
| m3 | 139 | 4 | `Room[N]`, `.len` (Phase 2) |
| m4 | 16 | 0 | |
| m5 | 16 | 0 | (mod, lexer, os) |
| m6 | 15 | 12 | old errput colour tests, struct dump null/[Circular]/arrays, multi line string, input (Phase 3 fmt/io) |
| m7 | 22 | 0 | |
| m8 | 33 | 7 | sized arrays, globals test, errput of ref? (Phase 2/3) |
| m9 | 26 | 15 | old syntax like `operator drop:` without `()`, ... |
| m10 | 10 | 0 | |
| m11 | 33 | 2 | divide by zero tests (no recorded IR; runtime error check not built) |
| m12 | 12 | 0 | |
| m13 | 22 | 15 | function values (Phase 2), projects (Phase 6), macros |

---

## 5. The plan (TODO.md has the details, the user edits it)

- **Phase 1** test cleanup: done.
- **Phase 2** language features (current). Done this session: array literals, for-in, null/isZero, mod, os (argc/argv). Left, in TODO order:
  - sized arrays `Room[2]`, `Room[2][3]` (zeroed, on the stack, runtime size allowed);
  - `unic`: a 32-bit Unicode code point type (char stays i8);
  - `ura::line`, `ura::filename`, `ura::function` (like `__LINE__`, `__FILE__`, `__func__`). Planned design: rows in `Keyword builtins[]`, turned into an INT/CHARS at check time (`line_at`, file path relative to cwd like `ucg_check_errors` does, the enclosing FN_DEC name); not started;
  - variadic functions written in ura; tuples (`q, r = divmod(7, 2)`); function values / callbacks (m13 010-013, 015, 019, 039);
  - `**` power operator (right-associative, tighter than `*`), i128/u128 and bigint in ucg;
  - `any` (not decided).
- **Phase 3** fmt and io: `fmt` param type, f-strings, `operator fmt`, colours (`io::red`), io in core replaces output/errput, alias + Stream, scan, organize ura-lib in folders.
- **Phase 4** memory and safety: bitmap arena allocator, C++-style assignment, -O flags, `@cache` / `@vector` / `@gpu` annotations, -san with file:line.
- **Phase 5** tooling: ura.project YAML, `ura watch`, `-tree`, Makefile instead of config.sh, test with each LLVM, emit without the .ll file.
- **Phase 6** projects: raytracer, ura-tcp-server, the m13 projects.
- **Phase 7** release (only when the user says): merge refactor -> dev -> main, push ura-lib, rewrite git history.

---

## 6. The cleanup refactor (in progress when the session ended)

The user said the codebase got ugly ("too much hardcoded checks", "too verbose") and asked for a clean, linear, beautiful codebase with less code. Plan and status:

| phase | what | status |
|---|---|---|
| 2 | ucg runtime functions found by name (`ucg_runtime`, `ucg_extern("calloc", ret, n, ...)`, `ucg_call(fn, n, ...)`); `ucg_read` 134 -> ~40 lines | done `3d03cb7` |
| 1 | builtins as a `Keyword builtins[]` table, soft keywords parsed into their own node (`check_builtin`, `codegen_builtin`) | done `89965d7` (the `Builtin` struct version was rejected and reverted) |
| 3 | `is_operation(type)` from `precedence[]`, `codegen_operation`, `analyze_block` | done `3093c12` |
| 5 | `same_params(a, b)` shared by `remove_overridden`, proto clash, duplicate fn | done `3d9ee0f` |
| - | `specials` is a `Keyword` table too | done `22f7126` |
| 6 | `run_test` moved to `src/ucg_test.c`; one-line cases in ucg.c; `to_string` table by `#define NAME(t) [t] = #t`; `type_names` removed | done `f61a7fa` |
| 4 | parser helpers + the `mod` struct-attribute collision | **next**, not applied (the session was interrupted) |

### Phase 4, ready to apply (ask the user first; it's a C edit)

Bug: in a mod, a struct attribute with the same name as a mod global is renamed too, so `p.x` breaks:

```ura
mod geo:
    x i32 = 1
    struct Point:
        x i32          # renamed to geo::x by prefix_names, so p.x / self.x no longer find it
```

Fix agreed with the user: in `prefix_names` (main.c), an ID that starts a line directly inside a struct body (space == struct's space + 1) and is followed by a type is an attribute declaration and is not renamed. Attributes are always reached through `.`, which is already skipped. A small helper replaces the duplicated "ID followed by a type" check:

```c
bool is_declaration(int i) {
    Token *curr = ura.tokens[i];
    Token *after = ura.tokens[i + 1];
    bool is_same_line = after->line == curr->line;
    return curr->type == ID && is_same_line && (after->is_type || after->type == ID);
}
```

In `prefix_names`: the first loop uses `is_declaration(i)` for globals; the second loop tracks `int struct_space = -1` (set when a line starts with STRUCT, reset when a line starts at or left of it) and skips `is_attr = starts_line && is_in_struct && is_declaration(i)`. Then add test m5 023:

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

The "parser helpers" half of phase 4 (`is_type_name`, `same_line`) was judged not worth it (adds functions to save 2-3 lines). Don't do it unless the user asks.

Ideas still open from the review: dispatch by function-pointer tables was rejected (keep `switch`); the 61 `(Ucg *[]){...}` call sites inside `run_test` could use `ucg_call` (optional, test code).

---

## 7. Things learned the hard way (gotchas)

- `parse_module_name` consumes tokens. Anything that looks at the previous token (`ura.tokens[ura.exe_pos - 2]`) must read it **before** calling it (a bug once made `ura::argc` look like a member name and broke every program, 77 passed / 709 failed).
- `MOD` is the `%` operator in the Type enum; the `mod` keyword is `MODULE`. `LINE` and `FILE` are macros (used in `eprint`), so don't name enum values that.
- Node's unions share slots: `left/var/data_type/ret_type/cond/to_call/loop/operand/scope/symbol` are one slot, `right/args/next/declared/to_type/index/to_copy` another. Setting `data_type` then `to_call` overwrites. FOR uses `right` for its iterable, so `next` is not free there.
- `strcmp2(a, b)` is true when equal.
- `tests` (full run) once showed 701 while every suite looked fine: the per-suite numbers came from a stale `build-tests.log`. Get a fresh log from the repo root before comparing.
- When unused package globals stopped being emitted, recorded IR that held `@EPERM`/`@ENOENT`... errno globals had to be re-recorded (an intended change).
- ucg's first pass in `ucg_compile` keeps a stack of open functions (`outer_fns`): a struct method defined inside a loop in main is a nested function in the instruction stream.
- `ucg_find_throwing` marks functions that can throw (fixpoint); `thrown` checks stay only after calls to those.
- A builtin name is a soft keyword only where an expression starts; `fn drop()` and `p.drop()` stay plain names.

---

## 8. Repository and end tasks

- Branch `refactor` (pushed at the end of this session). `main` and `dev` are behind and different.
- `projects/ura-lib` is a submodule on branch `new-syntax`, pushed too. After cloning: `git submodule update --init`.
- Repos: ura-lang, ura-lib, ura-vscode-extension, ura-libft, ura-tcp-server (GitHub user mohammedhrima). `gh` on the old machine needed `GODEBUG=netdns=cgo`.
- Only when the user says the work is finished: merge `refactor` -> `dev` -> `main`, rewrite ura-lang's git history (it's heavy and holds removed files). Confirm in full sentences right before.

## 9. Where to start on the new machine

1. `git submodule update --init`, then `source src/config.sh && build`. Check that `bin/ura` builds with the local LLVM (SPECS.md lists the versions).
2. Ask the user to run `tests > build-tests.log 2>&1` once and compare with section 4 (711 / 75).
3. Then phase 4 (section 6), after the user says yes, or whatever the user asks for.
