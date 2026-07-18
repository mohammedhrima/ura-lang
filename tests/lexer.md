# lexer

## index

- 001 — `use` missing opening quote
- 002 — `use` with unclosed quote
- 003 — `link` missing opening quote
- 004 — `link` with unclosed quote

## 001 — `use` missing opening quote

```ura
// parse_errors/005 — `use` missing opening quote
use @/header

main():
    output("hi\n")
```

```tree
```

```out
```

```err
error: Expected '"' after 'use'
```

```ll
```

## 002 — `use` with unclosed quote

```ura
// parse_errors/006 — `use` with unclosed quote
use "@/header

main():
    output("hi\n")
```

```tree
```

```out
```

```err
error: Unterminated 'use' path, expected closing '"'
```

```ll
```

## 003 — `link` missing opening quote

```ura
// errors/lexer/008 — `link` missing opening quote
link header.h

main():
    output("hi\n")
```

```tree
```

```out
```

```err
error: Expected '"' after 'link'
```

```ll
```

## 004 — `link` with unclosed quote

```ura
// errors/lexer/009 — `link` with unclosed quote
link "header.h

main():
    output("hi\n")
```

```tree
```

```out
```

```err
error: Unterminated 'link' path, expected closing '"'
```

```ll
```
