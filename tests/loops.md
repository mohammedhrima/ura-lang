# loops

## index

- 001 — basic while: count enemy kills

## 001 — basic while: count enemy kills

```ura
// loops/001.ura - basic while: count enemy kills

main():
    kills int = 0
    while kills < 5:
        kills = kills + 1
    output("<", kills, ">\n")
```

```out
<5>
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c"<\00", align 1
@str.1 = private unnamed_addr constant [3 x i8] c">\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define i32 @main() {
entry:
  %kills = alloca i32, align 4
  store i32 0, i32* %kills, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %kills1 = load i32, i32* %kills, align 4
  %lt = icmp slt i32 %kills1, 5
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %kills2 = load i32, i32* %kills, align 4
  %add = add i32 %kills2, 1
  store i32 %add, i32* %kills, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %kills3 = load i32, i32* %kills, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %kills3, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```
