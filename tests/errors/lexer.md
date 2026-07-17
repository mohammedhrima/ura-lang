# errors / lexer

## index

- 005 — `use` missing opening quote
- 006 — `use` with unclosed quote
- 008 — `link` missing opening quote
- 009 — `link` with unclosed quote

## 005 — `use` missing opening quote

```ura
// parse_errors/005 — `use` missing opening quote
use @/header

main():
    output("hi\n")
```

```out
```

```err
error: Expected '"' after `use`
```

```ll
```

## 006 — `use` with unclosed quote

```ura
// parse_errors/006 — `use` with unclosed quote
use "@/header

main():
    output("hi\n")
```

```out
```

```err
error: Unterminated `use` path, expected closing '"'
```

```ll
```

## 008 — `link` missing opening quote

```ura
// errors/lexer/008 — `link` missing opening quote
link header.h

main():
    output("hi\n")
```

```out
```

```err
error: Expected '"' after `link`
```

```ll
```

## 009 — `link` with unclosed quote

```ura
// errors/lexer/009 — `link` with unclosed quote
link "header.h

main():
    output("hi\n")
```

```out
```

```err
error: Unterminated `link` path, expected closing '"'
```

```ll
```
