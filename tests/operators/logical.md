# operators / logical

## index

- 002 — and / or with comparisons and precedence

## 002 — and / or with comparisons and precedence

```ura
// operators/logical/002.ura - and / or with comparisons and precedence

main():
    a int = 5
    b int = 9
    output(a < b and b == 9, "\n")
    output(a > b or b == 9, "\n")
    output(a < b and a > b, "\n")
    output(a < b or a > b, "\n")
    return 0
```

```out
True
True
False
True
```

```err
```

```ll

@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%s%s\00", align 1
@true_str.1 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.2 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [5 x i8] c"%s%s\00", align 1
@true_str.5 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.6 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.8 = private unnamed_addr constant [5 x i8] c"%s%s\00", align 1
@true_str.9 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.10 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.11 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.12 = private unnamed_addr constant [5 x i8] c"%s%s\00", align 1

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 5, i32* %a, align 4
  %b = alloca i32, align 4
  store i32 9, i32* %b, align 4
  %a1 = load i32, i32* %a, align 4
  %b2 = load i32, i32* %b, align 4
  %lt = icmp slt i32 %a1, %b2
  %b3 = load i32, i32* %b, align 4
  %eq = icmp eq i32 %b3, 9
  %and = and i1 %lt, %eq
  %bool_str = select i1 %and, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i8* %bool_str, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  %a4 = load i32, i32* %a, align 4
  %b5 = load i32, i32* %b, align 4
  %gt = icmp sgt i32 %a4, %b5
  %b6 = load i32, i32* %b, align 4
  %eq7 = icmp eq i32 %b6, 9
  %or = or i1 %gt, %eq7
  %bool_str8 = select i1 %or, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.1, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.2, i32 0, i32 0)
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.4, i32 0, i32 0), i8* %bool_str8, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %a9 = load i32, i32* %a, align 4
  %b10 = load i32, i32* %b, align 4
  %lt11 = icmp slt i32 %a9, %b10
  %a12 = load i32, i32* %a, align 4
  %b13 = load i32, i32* %b, align 4
  %gt14 = icmp sgt i32 %a12, %b13
  %and15 = and i1 %lt11, %gt14
  %bool_str16 = select i1 %and15, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.5, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.6, i32 0, i32 0)
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.8, i32 0, i32 0), i8* %bool_str16, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  %a17 = load i32, i32* %a, align 4
  %b18 = load i32, i32* %b, align 4
  %lt19 = icmp slt i32 %a17, %b18
  %a20 = load i32, i32* %a, align 4
  %b21 = load i32, i32* %b, align 4
  %gt22 = icmp sgt i32 %a20, %b21
  %or23 = or i1 %lt19, %gt22
  %bool_str24 = select i1 %or23, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.9, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.10, i32 0, i32 0)
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.12, i32 0, i32 0), i8* %bool_str24, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.11, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```
