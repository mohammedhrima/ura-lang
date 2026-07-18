# errors / loops

## index

- 001 — for without a loop variable
- 002 — for over a non-iterable
- 003 — loop without a colon

## 001 — for without a loop variable

```ura
main():
    for in 0..5:
        output(1)
```

```tree
```

```out
```

```err
error: Expected a loop variable after 'for'
  001.ura:2:5
  |
2 |     for in 0..5:
  |     ^^^
```

```ll
```

## 002 — for over a non-iterable

```ura
main():
    x int = 5
    for i in x:
        output(i)
```

```tree
```

```out
```

```err
error: 'for i in ...' expects a range (a..b) or an array
  002.ura:3:5
  |
3 |     for i in x:
  |     ^^^
```

```ll
```

## 003 — loop without a colon

```ura
main():
    loop
        output(1)
```

```tree
```

```out
```

```err
error: Expected ':' to open the 'loop' body
  003.ura:2:5
  |
2 |     loop
  |     ^^^^
```

```ll
```
