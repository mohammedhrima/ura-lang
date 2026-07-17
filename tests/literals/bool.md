# literals / bool

## index

- 001 — bool literals: declare, zero-init, bool function, reassign

## 001 — bool literals: declare, zero-init, bool function, reassign

```ura
// literals/bool/001.ura - bool literals: declare, zero-init, bool function, reassign

fn flag() bool:
    return True

main():
    b bool = flag()
    c bool = False
    d bool
    c = True
    return 9
```

```tree
fn flag() : bool
└─ return
   └─ bool True

fn main() : int
├─ = : bool
│  ├─ b : bool
│  └─ call flag : bool
├─ = : bool
│  ├─ c : bool
│  └─ bool False
├─ d : bool
├─ = : bool
│  ├─ c : bool
│  └─ bool True
└─ return
   └─ int 9
```

```out
```

```err
exit: 9
```

```ll

define i1 @flag() {
entry:
  ret i1 true
}

define i32 @main() {
entry:
  %b = alloca i1, align 1
  %call = call i1 @flag()
  store i1 %call, i1* %b, align 1
  %c = alloca i1, align 1
  store i1 false, i1* %c, align 1
  %d = alloca i1, align 1
  store i1 false, i1* %d, align 1
  store i1 true, i1* %c, align 1
  ret i32 9
}
```
