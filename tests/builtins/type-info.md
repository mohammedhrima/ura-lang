# builtins / type-info

## index

- 001 — typeof / sizeof on scalars
- 002 — typeof / sizeof on an array

## 001 — typeof / sizeof on scalars

```ura
// builtins/type-info/001.ura - typeof / sizeof on scalars

main():
    x int   = 5
    f float = 1.5
    output("typeof x = ", typeof(x), ", sizeof x = ", sizeof(x), "\n")
    output("typeof f = ", typeof(f), ", sizeof f = ", sizeof(f), "\n")
    output("sizeof long = ", sizeof(1 as long), "\n")
```

```tree
fn main() : int
├─ = : int
│  ├─ x : int
│  └─ int 5
├─ = : float
│  ├─ f : float
│  └─ float 1.5
├─ output : void
│  ├─ chars "typeof x = "
│  ├─ typeof : chars
│  │  └─ x : int
│  ├─ chars ", sizeof x = "
│  ├─ sizeof : int
│  │  └─ x : int
│  └─ chars "\n"
├─ output : void
│  ├─ chars "typeof f = "
│  ├─ typeof : chars
│  │  └─ f : float
│  ├─ chars ", sizeof f = "
│  ├─ sizeof : int
│  │  └─ f : float
│  └─ chars "\n"
└─ output : void
   ├─ chars "sizeof long = "
   ├─ sizeof : int
   │  └─ cast : long
   │     └─ int 1
   └─ chars "\n"
```

```out
typeof x = int, sizeof x = 4
typeof f = float, sizeof f = 4
sizeof long = 8
```

```err
```

```ll

@str = private unnamed_addr constant [12 x i8] c"typeof x = \00", align 1
@typeof = private unnamed_addr constant [4 x i8] c"int\00", align 1
@str.1 = private unnamed_addr constant [14 x i8] c", sizeof x = \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%s%s%s%d%s\00", align 1
@str.3 = private unnamed_addr constant [12 x i8] c"typeof f = \00", align 1
@typeof.4 = private unnamed_addr constant [6 x i8] c"float\00", align 1
@str.5 = private unnamed_addr constant [14 x i8] c", sizeof f = \00", align 1
@str.6 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.7 = private unnamed_addr constant [11 x i8] c"%s%s%s%d%s\00", align 1
@str.8 = private unnamed_addr constant [15 x i8] c"sizeof long = \00", align 1
@str.9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.10 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define i32 @main() {
entry:
  %x = alloca i32, align 4
  store i32 5, i32* %x, align 4
  %f = alloca float, align 4
  store float 1.500000e+00, float* %f, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @typeof, i32 0, i32 0), i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.1, i32 0, i32 0), i32 4, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.7, i32 0, i32 0), i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.3, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @typeof.4, i32 0, i32 0), i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.5, i32 0, i32 0), i32 4, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.6, i32 0, i32 0))
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.10, i32 0, i32 0), i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.8, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.9, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 002 — typeof / sizeof on an array

```ura
// builtins/type-info/002.ura - typeof / sizeof on an array (fat-pointer struct)

main():
    a int[] = [1, 2, 3]
    output("typeof a = ", typeof(a), ", sizeof a = ", sizeof(a), "\n")
```

```tree
fn main() : int
├─ = : array
│  ├─ a : int[]
│  └─ array : int[]
│     ├─ int 1
│     ├─ int 2
│     └─ int 3
└─ output : void
   ├─ chars "typeof a = "
   ├─ typeof : chars
   │  └─ a : int[]
   ├─ chars ", sizeof a = "
   ├─ sizeof : int
   │  └─ a : int[]
   └─ chars "\n"
```

```out
typeof a = array, sizeof a = 16
```

```err
```

```ll

@str = private unnamed_addr constant [12 x i8] c"typeof a = \00", align 1
@typeof = private unnamed_addr constant [6 x i8] c"array\00", align 1
@str.1 = private unnamed_addr constant [14 x i8] c", sizeof a = \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%s%s%s%d%s\00", align 1

define i32 @main() {
entry:
  %a = alloca { i32*, i64 }, align 8
  %arr = alloca i32, i64 3, align 4
  %arr.init = getelementptr i32, i32* %arr, i64 0
  store i32 1, i32* %arr.init, align 4
  %arr.init1 = getelementptr i32, i32* %arr, i64 1
  store i32 2, i32* %arr.init1, align 4
  %arr.init2 = getelementptr i32, i32* %arr, i64 2
  store i32 3, i32* %arr.init2, align 4
  %arr.ptr = insertvalue { i32*, i64 } undef, i32* %arr, 0
  %arr.len = insertvalue { i32*, i64 } %arr.ptr, i64 3, 1
  store { i32*, i64 } %arr.len, { i32*, i64 }* %a, align 8
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @typeof, i32 0, i32 0), i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.1, i32 0, i32 0), i32 16, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```
