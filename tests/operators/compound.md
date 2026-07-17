# operators / compound

## index

- 001 — compound assignment += -= *= /= %=

## 001 — compound assignment += -= *= /= %=

```ura
// operators/compound/001.ura - compound assignment += -= *= /= %=

main():
    x int = 10
    x += 5
    output(x, "\n")
    x -= 3
    output(x, "\n")
    x *= 4
    output(x, "\n")
    x /= 6
    output(x, "\n")
    x %= 5
    output(x, "\n")
    return 0
```

```tree
fn main() : int
├─ = : int
│  ├─ x : int
│  └─ int 10
├─ += : int
│  ├─ x : int
│  └─ int 5
├─ output : void
│  ├─ x : int
│  └─ chars "\n"
├─ -= : int
│  ├─ x : int
│  └─ int 3
├─ output : void
│  ├─ x : int
│  └─ chars "\n"
├─ *= : int
│  ├─ x : int
│  └─ int 4
├─ output : void
│  ├─ x : int
│  └─ chars "\n"
├─ /= : int
│  ├─ x : int
│  └─ int 6
├─ output : void
│  ├─ x : int
│  └─ chars "\n"
├─ %= : int
│  ├─ x : int
│  └─ int 5
├─ output : void
│  ├─ x : int
│  └─ chars "\n"
└─ return
   └─ int 0
```

```out
15
12
48
8
3
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.2 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@trap_msg = private unnamed_addr constant [145 x i8] c"runtime error: Division by zero\0A   001.ura:11:7\0A   |\0A11 |     x /= 6\0A   |       ^^\0A\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@trap_msg.7 = private unnamed_addr constant [143 x i8] c"runtime error: Modulo by zero\0A   001.ura:13:7\0A   |\0A13 |     x %= 5\0A   |       ^^\0A\00", align 1
@str.8 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.9 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1

define i32 @main() {
entry:
  %x = alloca i32, align 4
  store i32 10, i32* %x, align 4
  %cur = load i32, i32* %x, align 4
  %add = add i32 %cur, 5
  store i32 %add, i32* %x, align 4
  %x1 = load i32, i32* %x, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 %x1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  %cur2 = load i32, i32* %x, align 4
  %sub = sub i32 %cur2, 3
  store i32 %sub, i32* %x, align 4
  %x3 = load i32, i32* %x, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.2, i32 0, i32 0), i32 %x3, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %cur4 = load i32, i32* %x, align 4
  %mul = mul i32 %cur4, 4
  store i32 %mul, i32* %x, align 4
  %x5 = load i32, i32* %x, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.4, i32 0, i32 0), i32 %x5, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %cur6 = load i32, i32* %x, align 4
  br i1 false, label %trap, label %cont

trap:                                             ; preds = %entry
  %3 = call i64 @write(i32 2, i8* getelementptr inbounds ([145 x i8], [145 x i8]* @trap_msg, i32 0, i32 0), i64 144)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %div = sdiv i32 %cur6, 6
  store i32 %div, i32* %x, align 4
  %x7 = load i32, i32* %x, align 4
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.6, i32 0, i32 0), i32 %x7, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  %cur8 = load i32, i32* %x, align 4
  br i1 false, label %trap9, label %cont10

trap9:                                            ; preds = %cont
  %5 = call i64 @write(i32 2, i8* getelementptr inbounds ([143 x i8], [143 x i8]* @trap_msg.7, i32 0, i32 0), i64 142)
  call void @exit(i32 1)
  unreachable

cont10:                                           ; preds = %cont
  %mod = srem i32 %cur8, 5
  store i32 %mod, i32* %x, align 4
  %x11 = load i32, i32* %x, align 4
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.9, i32 0, i32 0), i32 %x11, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.8, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)
```
