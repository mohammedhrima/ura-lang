# builtins

## index

- 001 — puts: simple messages, char comparison on hero grade
- 002 — proto printf: hello world with a format string
- 003 — typeof / sizeof on scalars
- 004 — typeof / sizeof on an array
- 005 — sizeof without parentheses
- 006 — typeof without parentheses

## 001 — puts: simple messages, char comparison on hero grade

```ura
// builtins/001.ura - puts: simple messages, char comparison on hero grade

proto fn puts(str chars) int

fn is_s_rank(grade char) bool:
    return grade == 'S'

main():
    grade char = 'S'
    if is_s_rank(grade):
        puts("S-rank hero — unstoppable")
    else:
        puts("keep training")
```

```tree
fn puts(str : chars) : int

fn is_s_rank(grade : char) : bool
└─ return
   └─ == : bool
      ├─ grade : char
      └─ char 'S'

fn main() : int
├─ = : char
│  ├─ grade : char
│  └─ char 'S'
└─ if
   ├─ condition call is_s_rank : bool
   │  └─ grade : char
   ├─ call puts : int
   │  └─ chars "S-rank hero — unstoppable"
   └─ else
      └─ call puts : int
         └─ chars "keep training"
```

```out
S-rank hero — unstoppable
```

```err
```

```ll

@str = private unnamed_addr constant [28 x i8] c"S-rank hero \E2\80\94 unstoppable\00", align 1
@str.1 = private unnamed_addr constant [14 x i8] c"keep training\00", align 1

declare i32 @puts(i8*)

define i1 @is_s_rank(i8 %0) {
entry:
  %grade = alloca i8, align 1
  store i8 %0, i8* %grade, align 1
  %grade1 = load i8, i8* %grade, align 1
  %eq = icmp eq i8 %grade1, 83
  ret i1 %eq
}

define i32 @main() {
entry:
  %grade = alloca i8, align 1
  store i8 83, i8* %grade, align 1
  %grade1 = load i8, i8* %grade, align 1
  %call = call i1 @is_s_rank(i8 %grade1)
  br i1 %call, label %then, label %next

endif:                                            ; preds = %next, %then
  ret i32 0

then:                                             ; preds = %entry
  %call2 = call i32 @puts(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @str, i32 0, i32 0))
  br label %endif

next:                                             ; preds = %entry
  %call3 = call i32 @puts(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.1, i32 0, i32 0))
  br label %endif
}
```

## 002 — proto printf: hello world with a format string

```ura
// builtins/002.ura - proto printf: hello world with a format string

proto fn printf(fmt chars, ...) int

main():
    s chars = "ura"
    printf("hello %s %d\n", s, 42)
    return 0
```

```tree
fn printf(fmt : chars) : int

fn main() : int
├─ = : chars
│  ├─ s : chars
│  └─ chars "ura"
├─ call printf : int
│  ├─ chars "hello %s %d\n"
│  ├─ s : chars
│  └─ int 42
└─ return
   └─ int 0
```

```out
hello ura 42
```

```err
```

```ll

@str = private unnamed_addr constant [4 x i8] c"ura\00", align 1
@str.1 = private unnamed_addr constant [13 x i8] c"hello %s %d\0A\00", align 1

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %s = alloca i8*, align 8
  store i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i8** %s, align 8
  %s1 = load i8*, i8** %s, align 8
  %call = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.1, i32 0, i32 0), i8* %s1, i32 42)
  ret i32 0
}
```

## 003 — typeof / sizeof on scalars

```ura
// builtins/003.ura - typeof / sizeof on scalars

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

## 004 — typeof / sizeof on an array

```ura
// builtins/004.ura - typeof / sizeof on an array (fat-pointer struct)

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

## 005 — sizeof without parentheses

```ura
main():
    x int = 5
    output(sizeof x)
```

```tree
```

```out
```

```err
error: Expected '(' after 'sizeof'
  005.ura:3:12
  |
3 |     output(sizeof x)
  |            ^^^^^^
error: Expected ')' after output arguments
  005.ura:3:5
  |
3 |     output(sizeof x)
  |     ^^^^^^
```

```ll
```

## 006 — typeof without parentheses

```ura
main():
    x int = 5
    output(typeof x)
```

```tree
```

```out
```

```err
error: Expected '(' after 'typeof'
  006.ura:3:12
  |
3 |     output(typeof x)
  |            ^^^^^^
error: Expected ')' after output arguments
  006.ura:3:5
  |
3 |     output(typeof x)
  |     ^^^^^^
```

```ll
```
