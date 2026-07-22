# ura-lang roadmap

```
lexer → parser → analyze → type_check → code_gen (switch case + stage helper) → tests
```
1 feature = full pipeline + goldens (`.ll` `.err` `.run`, suite green) · check `old.c` first · 1 feature / commit · 1pt ≈ 4h

## ✅ done
`types` bit-width+f64+unsigned · arrays · structs · enums · fn-values · nullable · operators + overloading + `operator drop` · methods/`self` · `pub`/`::` · proto/variadics · `ref`/`ref?` · `@if` · `use` · DWARF · `-san` · `-O0..Oz` · typeof/sizeof · if/while/break/continue/return · `is` · rust diagnostics
**rewrite-only (old.c never had):** match/case · guards `a[i]?` `m?` `÷0` · for-in / for-ref · `as` casting · big ura-lib

## ❌ backlog
| feature | example | old.c | pts | M |
|---|---|---|---|---|
| globals (top-level vars) | `x i32 = 5` outside fn | — | 3 | M4 |
| tuples + multi-return | `a i32, b = f()` | ✓ | 4 | M5 |
| `List[T]` | `List<int>` push/pop/len | ✓ synth | 5 | M6 |
| `Map[K,V]` | `Map<str,int>` | — | 6 | M6 |
| `mod` + `::` | `mod m:` → `m::f()` | ✓ | 4 | M7 |
| wire `link` | `link "raylib"` | no-op | 3 | M7 |
| multi-file CLI | `ura a.ura b.ura` (only last runs) | — | 1 | M7 |
| `input` | `name = input()` | — | 2 | M8 |
| Result / Option | `Result<T>` on enums | — | 6 | M8 |
| try / catch — decide | `try: … catch e: …` | dead scaffold | 0 | M8 |
| type inference | `x := 5` | — | 4 | M9 |
| type aliases | `type Id = i32` | — | 2 | M9 |
| fn overload · default · named ret | `f(a, b=2)` | — | 7 | M9 |
| generics (LAST, monomorphize) | `Vec<T>` | — | 12 | M9 |
| std string | `s.upper()` | ✓ | 5 | M10 |
| std modules | math/time/os/net | — | 8 | M10 |
| regression tests (old bugs) | — | — | 2 | M10 |
| docs + vscode sync | — | — | 5 | M10 |
| `\u`/`\U` escapes | `"café"` | stub | 1 | — |
| post-1.0 | formatter · LSP · pkg-mgr | — | 34 | — |

## ⚖️ open decisions
- try/catch vs Result/Option → M8
- **globals** = biggest ura-lib blocker (105 consts + `os` + `raylib` need it)
- std modules load via `@/header` (ctype/errno/io/math/net/…) but `os`/`raylib` blocked on globals
- guard should throw an error · audit: are all features actually implemented?
