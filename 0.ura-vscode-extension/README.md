# Ura Language Support for Visual Studio Code

Editor support for the [Ura programming language](https://github.com/mohammedhrima/ura-lang).
The extension grows together with the compiler: it only knows what the compiler
in `../src` accepts today.

> **This extension is fully vibe-coded.** Every line of it — the TextMate grammar,
> the language configuration, the TypeScript, the build script and this README —
> was written by an AI assistant rather than by hand. The compiler it supports is
> not; that one is written the old-fashioned way.

## Features

- **Highlighting** for everything the language has today:
  - functions: `fn name(a i32, p ref(i32)) i32:` and calls
  - types `i32`, `b1`, `ref(type)`
  - `own(x)` (address of) and `dref(p)` (dereference)
  - `if` / `elif` / `else`, `while`, `break`, `continue`, `return`
  - `True`, `False`, integers, `//` comments
  - operators `+ - * / % == != < > <= >= =`
- **Indentation**: a line ending in `:` indents the next one; `elif` / `else`
  outdent; a new line after `return` / `break` / `continue` outdents.
- **Completion**: keywords, types, `own` / `dref`, the functions of the file
  (with parameter placeholders) and the variables visible at the cursor.
- **Hover**: signature of a function, type of a variable or parameter.
- **Signature help** while typing a call.
- **Go to definition** for functions and variables in the same file.
- **Diagnostics from the real compiler**: on open and on save the file is
  compiled with `ura`, and its errors show up in the Problems panel. The
  compiler doesn't report source positions yet, so errors are shown on the
  first line of the file.
- **File icon** for `.ura` files (shown by icon themes that have no icon of
  their own for `.ura`, such as the default Seti theme).

## Settings

| Setting | Default | Meaning |
|---|---|---|
| `ura.compilerPath` | `""` | Compiler used for diagnostics. Empty: `<workspace>/src/build/ura` if it exists, otherwise `ura` from `PATH`. |
| `ura.lint.enable` | `true` | Turn compiler diagnostics on or off. |

## Build and install

Requirements: Node.js, and the `code` command for installing.

```bash
make            # npm install + compile TypeScript
make test       # run the analysis tests (also reads ../src/tests/*.md)
make package    # build ura-lang-<version>.vsix
make install    # package and install into VS Code
make uninstall  # remove it from VS Code
make clean      # remove out/ and the .vsix
make fclean     # also remove node_modules
```

From the compiler side, `source src/config.sh` gives an `extension` command
that does `make install` for you.

After installing, reload VS Code (`Developer: Reload Window`).

## Layout

```
package.json                   extension manifest, settings
language-configuration.json    comments, brackets, indentation rules
syntaxes/ura.tmLanguage.json   highlighting
icons/icon.png                 extension icon and .ura file icon
icons/ura.jpeg                 source image of the icon (not packaged)
src/extension.ts               registers everything
src/analysis/                  language knowledge, no VS Code API
    keywords.ts                the words the language has
    parser.ts                  functions, variables, calls
    compiler.ts                runs ura, reads its errors
src/providers/                 the VS Code features
src/test/                      tests for src/analysis
```

`src/analysis/` never imports `vscode`, so it can move into a language server
later without being rewritten.

## Keeping up with the compiler

When the compiler learns something new, update in this order:

1. `src/analysis/keywords.ts` — mirror the `keywords[]` / `specials[]`
   tables in `src/main.c`.
2. `syntaxes/ura.tmLanguage.json` — the same words for highlighting.
3. `src/analysis/parser.ts` — if the new feature declares something (a new
   type, a struct...).
4. Record a compiler test with `copy` (it lands in `../src/tests/`, which
   `make test` also reads) and add an extension test in `src/test/`.

## License

MIT, see [LICENSE](LICENSE).
