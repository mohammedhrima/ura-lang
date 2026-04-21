# Anvil

A fast C++ dev tool for the [ura-lang](https://github.com/mohammedhrima/ura-lang)
compiler. Think `make`: one-shot sub-commands driven by an `anvil.toml` at the
project root.

```
$ anvil build
Built compiler  -> build/ura

$ anvil test
  PASS arrays/001
  ...
Passed: 135
Skipped: 3

$ anvil release
Building release binary...
Building avatar...
packaged build/ura-release-2026-04-22.tar.gz
Syncing anvil...     (dry-run: re-run with --confirm to actually push.)
```

Run `anvil` with no arguments to see every command.

## Commands

| Command | What it does |
|---|---|
| `build [--san \| --release]` | Compile the ura compiler |
| `test [folder…]` | Run the test suite (parallel, IR diff) |
| `copy <file.ura>` | Save a `.ura` + its IR as a test reference |
| `update-tests` | Regenerate every expected `.ll` |
| `update-projects` | Fix first-line path comment in every project `.ura` |
| `examples` | Concatenate the test corpus into `examples.ura` |
| `indent` | `clang-format` every `.c` / `.h` + run `merge_cases.py` |
| `check` | Verify build dependencies |
| `install` | Install missing dependencies (brew / apt / pacman / dnf) |
| `release` | Full release pipeline (default dry-run; `--confirm` to push) |
| `release-anvil` | Sync anvil to its own GitHub repo |
| `release-projects` | Sync `src/tests/projects/*` to their GitHub repos |
| `release-extension` | Sync the VSCode extension repo |
| `shell` | Launch an interactive Linux shell in Docker |
| `docker-build` | Rebuild the Linux dev-env Docker image |
| `leaks <exe>` | Run an executable with LeakSanitizer enabled |
| `reload` | Re-read `anvil.toml` |
| `help` | List commands |

## Build

```
make                      # ./anvil
make install              # /usr/local/bin/anvil  (override with PREFIX=~/.local)
make clean
```

Dependencies: a C++17 compiler and `make`. The TOML parser
([tomlc99](https://github.com/cktan/tomlc99), MIT) is vendored.

## Configuration — `anvil.toml`

Lives at the **project root**. Anvil walks up from the current working
directory to find it, so commands work from any subdirectory (same pattern as
`cargo`, `go`, etc.).

```toml
[paths]
src     = "src"
build   = "build"
tests   = "src/tests"
ura_lib = "src/ura-lib"

[compiler]
llvm_version = 14
sources = ["src/main.c", "src/utils.c", "src/builtins.c"]
output  = "build/ura"

[flags]
warn    = ["-Wall", "-Wextra", "-Werror=incompatible-pointer-types", "-Werror=int-conversion"]
san     = ["-fsanitize=undefined", "-fsanitize=address", "-fsanitize=null", "-g3"]
release = ["-O2", "-DDEBUG=0"]

[test]
max_parallel    = 3
ignore_ir_lines = ["DIFile", "DICompileUnit", "source_filename", "ModuleID"]

[sync]
anvil_repo     = "git@github.com:mohammedhrima/anvil.git"
ura_lib_repo   = "git@github.com:mohammedhrima/ura-lib.git"
extension_repo = "git@github.com:mohammedhrima/ura-vscode-extension.git"
projects_dir   = "src/tests/projects"
project_url    = "git@github.com:mohammedhrima/{name}.git"
```

## Layout

```
config/anvil/
├── Makefile               # make / make install / make clean
├── README.md              # this file
├── src/
│   ├── main.cpp           # entry — one-shot dispatch
│   ├── command.cpp/.h     # command registry
│   ├── config.cpp/.h      # anvil.toml loader
│   ├── paths.cpp/.h       # project-root discovery
│   ├── exec.cpp/.h        # posix_spawn with capture
│   ├── fs.cpp/.h          # walk / read / is_file / is_dir
│   ├── util.cpp/.h        # OS detection, date strings
│   ├── color.h            # ANSI macros
│   └── commands/          # one file per command group
└── vendor/
    └── toml.c/.h
```

## Release

Default is a dry-run — it prints what *would* happen without touching any
remote:

```
$ anvil release
Building release binary...
...
Syncing anvil...     (git@github.com:mohammedhrima/anvil.git)
  dry-run: would clone, mirror, commit, push.
...
  dry-run: re-run with --confirm to actually push.
```

Add `--confirm` to actually push. Each individual sync command also takes
`--confirm`.

## License

MIT. Bundled third-party code: `vendor/toml.*` (MIT, CK Tan).
