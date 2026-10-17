# Ura Language Support for Visual Studio Code

Editor support for the [Ura programming language](https://github.com/mohammedhrima/ura-lang).
The extension grows together with the compiler: it only knows what the compiler
in `../src` accepts today.

> **This extension is fully vibe-coded.** Every line of it — the TextMate grammar,
> the language configuration, the TypeScript, the build script and this README —
> was written by an AI assistant rather than by hand. The compiler it supports is
> not; that one is written the old-fashioned way.

## Features

- **Highlighting**: keywords, types, functions, `struct` names (also where they
  are used, through semantic tokens), strings, numbers, `//` and `/* */` comments.
- **Formatter** (Format Document, Format Selection, and on save by default):
  4-space indentation that keeps the compiler's levels, one space around
  operators, after commas and in declarations, tight calls/indexes/`&`/`.`,
  one blank line between top-level `fn` / `struct` / `proto` / `use` groups.
  Strings and comments are never touched. It never changes what a program means
  (tested: the compiler's IR is identical before and after formatting).
- **Completion**: keywords, types, struct names, functions (with parameter
  placeholders) of the file and of the files it `use`s, visible variables, and
  after `x.` the attributes and methods of `x`'s struct.
- **Hover**: signatures of functions and methods, structs with their attributes,
  types of variables, parameters and attributes.
- **Signature help** while typing a call, methods included (`self` is implicit).
- **Go to definition**, **Find all references**, **highlights** and **rename**
  for variables, parameters, functions, structs, attributes and methods, across
  the files of the workspace. Rename refuses keywords, invalid names, `self` and
  names already taken.
- **Outline / breadcrumbs** and **Go to Symbol in Workspace** (Ctrl+T).
- **`use "path"`**: ctrl+click opens the file; its functions and structs are
  known everywhere (transitively, like the compiler's lookup).
- **Diagnostics from the real compiler**, on open and on save; files reached
  through `use` are compiled too. Errors show on the first line until the
  compiler reports positions.
- **Run / Build** (play button in the editor title, or `Ura: Run File`): compiles
  to `build/<name>` next to the file and runs it in the "Ura" terminal.
- **Snippets**: `fn`, `main`, `struct`, `method`, `if`, `ife`, `while`, `use`, `printf`.
- **Editing**: auto-closing brackets and quotes, indentation on `:`,
  indentation-based folding, file icon.

## Settings

| Setting | Default | Meaning |
|---|---|---|
| `ura.compilerPath` | `""` | Compiler used for diagnostics and Run/Build. Empty: `<workspace>/bin/ura` if it exists, otherwise `ura` from `PATH`. |
| `ura.lint.enable` | `true` | Turn compiler diagnostics on or off. |

`.ura` files default to this extension as formatter, with format on save; change
`editor.formatOnSave` under `[ura]` to turn that off.

## Build, test, install

Requirements: Node.js, and the `code` command for installing and E2E tests.

```bash
make            # npm install + compile (tsc -b: server, client, tests)
make lint       # ESLint, including the layer rules below
make test       # unit tests: server core, no VS Code (also reads ../src/tests/*.md)
make test-e2e   # end-to-end: a real VS Code (the one behind `code`) with the extension
make package    # build ura-lang-<version>.vsix
make install    # package and install into VS Code
make uninstall  # remove it from VS Code
make clean      # remove out/ and the .vsix
make fclean     # also remove node_modules and .vscode-test
```

From the compiler side, `source src/config.sh` gives an `extension` command
that does `make install` for you. After installing, reload VS Code.

## Architecture

The extension is a language client; the language itself lives in a language
server (LSP) running in its own Node process.

```
client/src/                 runs in VS Code: starts the server, registers commands
    extension.ts            wiring only
    services/               logger (the "Ura" output), language client
    commands/               Run / Build
server/src/                 runs as its own process: speaks LSP
    server.ts               builds the services, registers every feature
    services/               documents/workspace, config, logger, compiler
    features/               one LSP request per file (completion, hover, rename, ...)
    core/                   the language: pure functions, no VS Code / LSP / file system
        keywords.ts         the words the language has
        parser.ts           functions, structs, methods, variables, calls, resolution
        imports.ts          following `use`
        symbols.ts          declarations, hovers, outline
        references.ts       occurrences, rename checks
        format.ts           the formatter
        compiler.ts         reading the compiler's errors
test/unit/                  node --test on server/src/core
test/e2e/                   @vscode/test-electron + mocha, fixtures/*.ura
syntaxes/  snippets/  language-configuration.json  icons/
```

Rules, enforced by TypeScript project references and ESLint:

- `core` never imports `vscode`, LSP or `fs`: it takes text and callbacks.
- A feature turns an LSP request into core calls and back, nothing else; features
  never import each other. Adding one = a file in `features/` + a line in `server.ts`.
- Services are built once and passed in; nothing reads settings or globals directly.
- The client never imports server code.
- A request handler never throws (it logs and returns an empty answer), except
  for errors meant for the user, like a refused rename.

## Keeping up with the compiler

When the compiler learns something new, update in this order:

1. `server/src/core/keywords.ts` — mirror the `keywords[]` / `specials[]`
   tables in `src/main.c`.
2. `syntaxes/ura.tmLanguage.json` — the same words for highlighting.
3. `server/src/core/parser.ts` (and `format.ts` for new operators) — if the
   feature declares or writes something new.
4. Record a compiler test with `copy` (it lands in `../src/tests/`, which
   `make test` also reads: the formatter is checked against every program there)
   and add tests in `test/unit/` (and `test/e2e/` for a new editor feature).

## License

MIT, see [LICENSE](LICENSE).
