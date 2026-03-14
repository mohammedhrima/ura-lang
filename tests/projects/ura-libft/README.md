# ura-libft

A collection of standard C library utility functions implemented in [Ura](https://github.com/mohammedhrima/ura-lang) — a compiled, statically-typed language with Python-like syntax.

## Requirements

No installation needed. The `ura` compiler binary is bundled in this repo.

## Usage

```bash
./ura <file.ura>
```

This compiles and immediately runs the file.

## Functions

| File | Description |
|------|-------------|
| `strlen.ura` | Returns the length of a string |
| `strcmp.ura` | Compares two strings lexicographically |
| `strncmp.ura` | Compares up to n characters of two strings |
| `isalpha.ura` | Returns true if character is alphabetic |
| `isdigit.ura` | Returns true if character is a digit |
| `isalnum.ura` | Returns true if character is alphanumeric |
| `putnbr.ura` | Prints an integer to stdout |

## Example

```bash
./ura strlen.ura
# Output: <3>

./ura isalpha.ura
# Output: true/false
```

## About Ura

Ura is a compiled language that targets native code via LLVM. Source: [github.com/mohammedhrima/ura-lang](https://github.com/mohammedhrima/ura-lang)
