# operators / bitwise

## index

- 002 — bitwise & | ^ << >>

## 002 — bitwise & | ^ << >>

```ura
// operators/bitwise/002.ura - bitwise & | ^ << >>

main():
    a int = 12
    b int = 10
    output(a & b, "\n")
    output(a | b, "\n")
    output(a ^ b, "\n")
    output(a << 2, "\n")
    output(a >> 1, "\n")
    return 0
```

```tree
fn main() : int
├─ = : int
│  ├─ a : int
│  └─ int 12
├─ = : int
│  ├─ b : int
│  └─ int 10
├─ output : void
│  ├─ & : int
│  │  ├─ a : int
│  │  └─ b : int
│  └─ chars "\n"
├─ output : void
│  ├─ | : int
│  │  ├─ a : int
│  │  └─ b : int
│  └─ chars "\n"
├─ output : void
│  ├─ ^ : int
│  │  ├─ a : int
│  │  └─ b : int
│  └─ chars "\n"
├─ output : void
│  ├─ << : int
│  │  ├─ a : int
│  │  └─ int 2
│  └─ chars "\n"
├─ output : void
│  ├─ >> : int
│  │  ├─ a : int
│  │  └─ int 1
│  └─ chars "\n"
└─ return
   └─ int 0
```

```out
8
14
6
48
6
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
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.8 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 12, i32* %a, align 4
  %b = alloca i32, align 4
  store i32 10, i32* %b, align 4
  %a1 = load i32, i32* %a, align 4
  %b2 = load i32, i32* %b, align 4
  %band = and i32 %a1, %b2
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 %band, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  %a3 = load i32, i32* %a, align 4
  %b4 = load i32, i32* %b, align 4
  %bor = or i32 %a3, %b4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.2, i32 0, i32 0), i32 %bor, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %a5 = load i32, i32* %a, align 4
  %b6 = load i32, i32* %b, align 4
  %bxor = xor i32 %a5, %b6
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.4, i32 0, i32 0), i32 %bxor, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %a7 = load i32, i32* %a, align 4
  %shl = shl i32 %a7, 2
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.6, i32 0, i32 0), i32 %shl, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  %a8 = load i32, i32* %a, align 4
  %shr = ashr i32 %a8, 1
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.8, i32 0, i32 0), i32 %shr, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```
