# Avatar

Package manager and build tool for [ura-lang](https://github.com/mohammedhrima/ura-lang)
projects. Cargo-style: one `ura.toml` per project, one-shot commands.

```
$ avatar new hello
Created hello
  hello/ura.toml
  hello/src/main.ura

$ cd hello && avatar run
Built -> target/hello
hello from hello!
```

## Commands

| Command | What it does |
|---|---|
| `avatar new <name>` | Scaffold a new project (`ura.toml`, `src/main.ura`, `.gitignore`) |
| `avatar build` | Compile the project (fetches deps first if needed) |
| `avatar run` | `build`, then execute the produced binary |
| `avatar test` | Compile every `.ura` under `tests/` and report pass/fail |
| `avatar check` | Type-check without linking (`-no-exec`) |
| `avatar add <name> <git>` | Record a dependency in `ura.toml` |
| `avatar update` | Clone / pull every declared dep into `target/deps/` and write `ura.lock` |
| `avatar clean` | Remove `target/` |
| `avatar fmt` | Formatter stub (ura has no formatter yet) |
| `avatar help` | List commands |

## `ura.toml`

```toml
[project]
name    = "hello"
version = "0.1.0"

[build]
src      = "src/main.ura"
output   = "target/hello"
optimize = "O2"

[dependencies]
# http = { git = "https://github.com/user/ura-http" }
```

Paths are relative to `ura.toml`. The `optimize` flag maps directly to the
compiler's `-O*` options (`O0`, `O1`, `O2`, `O3`, `Os`, `Oz`).

## Dependencies

Git URLs only in v1. On `avatar update`:

```
$ avatar add http https://github.com/user/ura-http
added http = https://github.com/user/ura-http
  run `avatar update` to fetch it

$ avatar update
cloning http
locked 1 dep(s) -> ura.lock
```

Deps live at `target/deps/<name>/`. On `avatar build`, each dep's `src/`
dir is passed to the compiler via `-L`, so your code can do
`use "@/http"` and pick it up.

`ura.lock` pins the commit each dep is currently at:

```toml
# avatar lock file — do not edit by hand

http = "a1b2c3d4…"
```

## Environment

- `URA_BIN` — override the ura compiler path (default: `ura` on `$PATH`).
- `URA_LIB` — standard library path. Usually set by the install step.

## Build

```
make                 # ./avatar
make install         # /usr/local/bin/avatar (override with PREFIX=~/.local)
make clean
```

Dependencies: C++17 compiler, `make`. TOML parser
([tomlc99](https://github.com/cktan/tomlc99), MIT) is vendored.

## License

MIT. Bundled: `vendor/toml.*` (MIT, CK Tan).
