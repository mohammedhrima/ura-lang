# match

## index

- 001 — a single-value case matches
- 002 — a case with several values
- 003 — default runs when nothing matches
- 004 — break leaves the match early
- 005 — break exits the match, not the enclosing loop
- 006 — continue applies to the loop, skipping the rest of the iteration

## 001 — a single-value case matches

```ura
// match/001 — a single-value case matches
main():
    a int = 1
    match a:
        case 1:
            output("one\n")
        default:
            output("other\n")
```

```out
one
```

```err
```

```ll

@str = private unnamed_addr constant [5 x i8] c"one\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.1 = private unnamed_addr constant [7 x i8] c"other\0A\00", align 1
@fmt.2 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 1, i32* %a, align 4
  %a1 = load i32, i32* %a, align 4
  %eq = icmp eq i32 %a1, 1
  br i1 %eq, label %case.body, label %case.next

match.end:                                        ; preds = %case.next, %case.body
  ret i32 0

case.body:                                        ; preds = %entry
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0))
  br label %match.end

case.next:                                        ; preds = %entry
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.2, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0))
  br label %match.end
}

declare i32 @printf(i8*, ...)
```

## 002 — a case with several values

```ura
// match/002 — a case with several values
main():
    a int = 3
    match a:
        case 1, 2, 3:
            output("small\n")
        default:
            output("big\n")
```

```out
small
```

```err
```

```ll

@str = private unnamed_addr constant [7 x i8] c"small\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"big\0A\00", align 1
@fmt.2 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 3, i32* %a, align 4
  %a1 = load i32, i32* %a, align 4
  %eq = icmp eq i32 %a1, 1
  %eq2 = icmp eq i32 %a1, 2
  %case.or = or i1 %eq, %eq2
  %eq3 = icmp eq i32 %a1, 3
  %case.or4 = or i1 %case.or, %eq3
  br i1 %case.or4, label %case.body, label %case.next

match.end:                                        ; preds = %case.next, %case.body
  ret i32 0

case.body:                                        ; preds = %entry
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0))
  br label %match.end

case.next:                                        ; preds = %entry
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.2, i32 0, i32 0), i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0))
  br label %match.end
}

declare i32 @printf(i8*, ...)
```

## 003 — default runs when nothing matches

```ura
// match/003 — default runs when nothing matches
main():
    a int = 99
    match a:
        case 1:
            output("one\n")
        default:
            output("fallback\n")
```

```out
fallback
```

```err
```

```ll

@str = private unnamed_addr constant [5 x i8] c"one\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c"fallback\0A\00", align 1
@fmt.2 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 99, i32* %a, align 4
  %a1 = load i32, i32* %a, align 4
  %eq = icmp eq i32 %a1, 1
  br i1 %eq, label %case.body, label %case.next

match.end:                                        ; preds = %case.next, %case.body
  ret i32 0

case.body:                                        ; preds = %entry
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0))
  br label %match.end

case.next:                                        ; preds = %entry
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.2, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0))
  br label %match.end
}

declare i32 @printf(i8*, ...)
```

## 004 — break leaves the match early

```ura
// match/004 — break leaves the match early
main():
    a int = 1
    match a:
        case 1:
            output("before\n")
            break
            output("after\n")
```

```out
before
```

```err
```

```ll

@str = private unnamed_addr constant [8 x i8] c"before\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 1, i32* %a, align 4
  %a1 = load i32, i32* %a, align 4
  %eq = icmp eq i32 %a1, 1
  br i1 %eq, label %case.body, label %match.end

match.end:                                        ; preds = %case.body, %entry
  ret i32 0

case.body:                                        ; preds = %entry
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str, i32 0, i32 0))
  br label %match.end
}

declare i32 @printf(i8*, ...)
```

## 005 — break exits the match, not the enclosing loop

```ura
// match/005 — break exits the match, not the enclosing loop
main():
    i int = 0
    while i < 3:
        i = i + 1
        match i:
            case 2:
                break
        output(i, "\n")
```

```out
1
2
3
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1

define i32 @main() {
entry:
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %match.end, %entry
  %i1 = load i32, i32* %i, align 4
  %lt = icmp slt i32 %i1, 3
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %i2 = load i32, i32* %i, align 4
  %add = add i32 %i2, 1
  store i32 %add, i32* %i, align 4
  %i3 = load i32, i32* %i, align 4
  %eq = icmp eq i32 %i3, 2
  br i1 %eq, label %case.body, label %match.end

while.end:                                        ; preds = %while.cond
  ret i32 0

match.end:                                        ; preds = %case.body, %while.body
  %i4 = load i32, i32* %i, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 %i4, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  br label %while.cond

case.body:                                        ; preds = %while.body
  br label %match.end
}

declare i32 @printf(i8*, ...)
```

## 006 — continue applies to the loop, skipping the rest of the iteration

```ura
// match/006 — continue applies to the loop, skipping the rest of the iteration
main():
    i int = 0
    while i < 3:
        i = i + 1
        match i:
            case 2:
                continue
        output(i, "\n")
```

```out
1
3
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1

define i32 @main() {
entry:
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %match.end, %case.body, %entry
  %i1 = load i32, i32* %i, align 4
  %lt = icmp slt i32 %i1, 3
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %i2 = load i32, i32* %i, align 4
  %add = add i32 %i2, 1
  store i32 %add, i32* %i, align 4
  %i3 = load i32, i32* %i, align 4
  %eq = icmp eq i32 %i3, 2
  br i1 %eq, label %case.body, label %match.end

while.end:                                        ; preds = %while.cond
  ret i32 0

match.end:                                        ; preds = %while.body
  %i4 = load i32, i32* %i, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 %i4, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  br label %while.cond

case.body:                                        ; preds = %while.body
  br label %while.cond
}

declare i32 @printf(i8*, ...)
```
