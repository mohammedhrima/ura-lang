# errors / match

## index

- 001 — a case needs at least one value
- 002 — case value type must match the subject
- 003 — a case with no enclosing match
- 004 — a default with no enclosing match
- 005 — break needs a loop or match

## 001 — a case needs at least one value

```ura
// errors/match/001 — a case needs at least one value
main():
    a int = 1
    match a:
        case:
            output("x\n")
```

```out
```

```err
error: Expected an expression after 'case'
  001.ura:5:9
  |
5 |         case:
  |         ^^^^
```

```ll
```

## 002 — case value type must match the subject

```ura
// errors/match/002 — case value type must match the subject
main():
    a int = 1
    match a:
        case "x":
            output("x\n")
```

```out
```

```err
error: This case value is CHARS but the subject is INT; they must be the same type
  002.ura:5:15
  |
5 |         case "x":
  |               ^
```

```ll
```

## 003 — a case with no enclosing match

```ura
// errors/match/003 — a case with no enclosing match
main():
    case 1:
        output("x\n")
```

```out
```

```err
error: 'case' without a matching 'match'
  003.ura:3:5
  |
3 |     case 1:
  |     ^^^^
```

```ll
```

## 004 — a default with no enclosing match

```ura
// errors/match/004 — a default with no enclosing match
main():
    default:
        output("x\n")
```

```out
```

```err
error: 'default' without a matching 'match'
  004.ura:3:5
  |
3 |     default:
  |     ^^^^^^^
```

```ll
```

## 005 — break needs a loop or match

```ura
// errors/match/005 — break needs a loop or match
main():
    break
```

```out
```

```err
error: 'break' outside a loop or match
  005.ura:3:5
  |
3 |     break
  |     ^^^^^
```

```ll
```
