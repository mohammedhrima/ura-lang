# parser

## index

- 001 — unclosed [ in an array literal

---

## 001 — unclosed [ in an array literal

```ura
// error: expected ']' to close this '['
// help: use a i32[] = [1, 2]
fn main() i32:
    a i32[] = [1, 2
    return a[0]
```

### stderr

```
error: test.ura:4:19 expected ']' to close this '['
  |
4 |     a i32[] = [1, 2
  |                   ^
help: use a i32[] = [1, 2]
```

### status

```
1
```

