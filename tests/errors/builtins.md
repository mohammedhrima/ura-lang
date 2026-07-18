# errors / builtins

## index

- 001 — sizeof without parentheses
- 002 — typeof without parentheses

## 001 — sizeof without parentheses

```ura
main():
    x int = 5
    output(sizeof x)
```

```tree
```

```out
```

```err
error: Expected '(' after 'sizeof'
  001.ura:3:12
  |
3 |     output(sizeof x)
  |            ^^^^^^
error: Expected ')' after output arguments
  001.ura:3:5
  |
3 |     output(sizeof x)
  |     ^^^^^^
```

```ll
```

## 002 — typeof without parentheses

```ura
main():
    x int = 5
    output(typeof x)
```

```tree
```

```out
```

```err
error: Expected '(' after 'typeof'
  002.ura:3:12
  |
3 |     output(typeof x)
  |            ^^^^^^
error: Expected ')' after output arguments
  002.ura:3:5
  |
3 |     output(typeof x)
  |     ^^^^^^
```

```ll
```
