# functions / params / value

## index

- 001 — value parameters: take_damage
- 002 — a by-value parameter does not change the caller

## 001 — value parameters: take_damage

```ura
// functions/params/value/001.ura - value parameters: take_damage

fn take_damage(hp int, atk int) int:
    return hp - atk

main():
    result int = take_damage(100, 18)
    output("<", result, ">\n")
```

```out
<82>
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c"<\00", align 1
@str.1 = private unnamed_addr constant [3 x i8] c">\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define i32 @take_damage(i32 %0, i32 %1) {
entry:
  %hp = alloca i32, align 4
  store i32 %0, i32* %hp, align 4
  %atk = alloca i32, align 4
  store i32 %1, i32* %atk, align 4
  %hp1 = load i32, i32* %hp, align 4
  %atk2 = load i32, i32* %atk, align 4
  %sub = sub i32 %hp1, %atk2
  ret i32 %sub
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %call = call i32 @take_damage(i32 100, i32 18)
  store i32 %call, i32* %result, align 4
  %result1 = load i32, i32* %result, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %result1, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 002 — a by-value parameter does not change the caller

```ura
// functions/params/value/002.ura - a by-value parameter does not change the caller

fn inc(n int) int:
    n = n + 1
    return n
main():
    n int = 5
    inc(n)
    return n
```

```out
```

```err
exit: 5
```

```ll

define i32 @inc(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %n1 = load i32, i32* %n, align 4
  %add = add i32 %n1, 1
  store i32 %add, i32* %n, align 4
  %n2 = load i32, i32* %n, align 4
  ret i32 %n2
}

define i32 @main() {
entry:
  %n = alloca i32, align 4
  store i32 5, i32* %n, align 4
  %n1 = load i32, i32* %n, align 4
  %call = call i32 @inc(i32 %n1)
  %n2 = load i32, i32* %n, align 4
  ret i32 %n2
}
```
