# casting

## index

- 001 — `as` casting + long / short via explicit cast

## 001 — `as` casting + long / short via explicit cast

```ura
// casting/001.ura - `as` casting + long / short via explicit cast

main():
    a int   = 300
    big long  = a as long
    small short = a as short
    c int   = big as int
    output(big, "\n")
    output(small, "\n")
    output(c, "\n")
    output(5 as long, "\n")
    return 0
```

```tree
fn main() : int
├─ = : int
│  ├─ a : int
│  └─ int 300
├─ = : long
│  ├─ big : long
│  └─ cast : long
│     └─ a : int
├─ = : short
│  ├─ small : short
│  └─ cast : short
│     └─ a : int
├─ = : int
│  ├─ c : int
│  └─ cast : int
│     └─ big : long
├─ output : void
│  ├─ big : long
│  └─ chars "\n"
├─ output : void
│  ├─ small : short
│  └─ chars "\n"
├─ output : void
│  ├─ c : int
│  └─ chars "\n"
├─ output : void
│  ├─ cast : long
│  │  └─ int 5
│  └─ chars "\n"
└─ return
   └─ int 0
```

```out
300
300
300
5
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%lld%s\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.2 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [7 x i8] c"%lld%s\00", align 1

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 300, i32* %a, align 4
  %big = alloca i64, align 8
  %a1 = load i32, i32* %a, align 4
  %cast = sext i32 %a1 to i64
  store i64 %cast, i64* %big, align 4
  %small = alloca i16, align 2
  %a2 = load i32, i32* %a, align 4
  %cast3 = trunc i32 %a2 to i16
  store i16 %cast3, i16* %small, align 2
  %c = alloca i32, align 4
  %big4 = load i64, i64* %big, align 4
  %cast5 = trunc i64 %big4 to i32
  store i32 %cast5, i32* %c, align 4
  %big6 = load i64, i64* %big, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i64 %big6, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  %small7 = load i16, i16* %small, align 2
  %s2i = sext i16 %small7 to i32
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.2, i32 0, i32 0), i32 %s2i, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %c8 = load i32, i32* %c, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.4, i32 0, i32 0), i32 %c8, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.6, i32 0, i32 0), i64 5, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```
