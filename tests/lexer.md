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
  001.ura:2:5
  |
2 | use @/header
  |     ^
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
  002.ura:2:5
  |
2 | use "@/header
  |     ^
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
  003.ura:2:6
  |
2 | link header.h
  |      ^
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
  004.ura:2:6
  |
2 | link "header.h
  |      ^
```

```ll
```
