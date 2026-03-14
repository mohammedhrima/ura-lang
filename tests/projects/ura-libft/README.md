# ura-libft

A collection of standard C library utility functions implemented in [Ura](https://github.com/mohammedhrima/ura-lang) — a compiled, statically-typed language with Python-like syntax.

## Getting the compiler

```bash
curl -L https://github.com/mohammedhrima/ura-lang/raw/main/build/ura -o ura
chmod +x ura
```

## Usage

```bash
./ura <file.ura>
```

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
```

## About Ura

Ura is a compiled language that targets native code via LLVM. Source: [github.com/mohammedhrima/ura-lang](https://github.com/mohammedhrima/ura-lang)
