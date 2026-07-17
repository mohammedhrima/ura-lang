# operators / arithmetic

## index

- 002 — arithmetic with precedence (2*3 + 20/4 - 1)
- 003 — reassign an existing variable (2*10 + 3)
- 004 — modulo

## 002 — arithmetic with precedence (2*3 + 20/4 - 1)

```ura
// operators/arithmetic/002.ura - arithmetic with precedence (2*3 + 20/4 - 1)

main():
    return 2 * 3 + 20 / 4 - 1
```

```out
```

```err
exit: 10
```

```ll

@trap_msg = private unnamed_addr constant [175 x i8] c"runtime error: Division by zero\0A  002.ura:4:23\0A  |\0A4 |     return 2 * 3 + 20 / 4 - 1\0A  |                       ^\0A\00", align 1

define i32 @main() {
entry:
  br i1 false, label %trap, label %cont

trap:                                             ; preds = %entry
  %0 = call i64 @write(i32 2, i8* getelementptr inbounds ([175 x i8], [175 x i8]* @trap_msg, i32 0, i32 0), i64 174)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  ret i32 10
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)
```

## 003 — reassign an existing variable (2*10 + 3)

```ura
// operators/arithmetic/003.ura - reassign an existing variable (2*10 + 3)

main():
    a int = 2
    a = a * 10 + 3
    return a
```

```out
```

```err
exit: 23
```

```ll

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 2, i32* %a, align 4
  %a1 = load i32, i32* %a, align 4
  %mul = mul i32 %a1, 10
  %add = add i32 %mul, 3
  store i32 %add, i32* %a, align 4
  %a2 = load i32, i32* %a, align 4
  ret i32 %a2
}
```

## 004 — modulo

```ura
// operators/arithmetic/004.ura - modulo

main():
    return 17 % 5
```

```out
```

```err
exit: 2
```

```ll

@trap_msg = private unnamed_addr constant [153 x i8] c"runtime error: Modulo by zero\0A  004.ura:4:15\0A  |\0A4 |     return 17 % 5\0A  |               ^\0A\00", align 1

define i32 @main() {
entry:
  br i1 false, label %trap, label %cont

trap:                                             ; preds = %entry
  %0 = call i64 @write(i32 2, i8* getelementptr inbounds ([153 x i8], [153 x i8]* @trap_msg, i32 0, i32 0), i64 152)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  ret i32 2
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)
```
