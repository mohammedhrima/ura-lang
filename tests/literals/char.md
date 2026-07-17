# literals / char

## index

- 001 — char literals: declare, print, escapes, in a function

## 001 — char literals: declare, print, escapes, in a function

```ura
// literals/char/001.ura - char literals: declare, print, escapes, in a function

fn grade() char:
    return 'A'

main():
    c char = 'z'
    output(c, "\n")
    output('!', "\n")
    output(grade(), "\n")
    output('\n')
    return 0
```

```tree
fn grade() : char
└─ return
   └─ char 'A'

fn main() : int
├─ = : char
│  ├─ c : char
│  └─ char 'z'
├─ output : void
│  ├─ c : char
│  └─ chars "\n"
├─ output : void
│  ├─ char '!'
│  └─ chars "\n"
├─ output : void
│  ├─ call grade : char
│  └─ chars "\n"
├─ output : void
│  └─ char '\n'
└─ return
   └─ int 0
```

```out
z
!
A
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%c%s\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.2 = private unnamed_addr constant [5 x i8] c"%c%s\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [5 x i8] c"%c%s\00", align 1
@fmt.5 = private unnamed_addr constant [3 x i8] c"%c\00", align 1

define i8 @grade() {
entry:
  ret i8 65
}

define i32 @main() {
entry:
  %c = alloca i8, align 1
  store i8 122, i8* %c, align 1
  %c1 = load i8, i8* %c, align 1
  %c2i = sext i8 %c1 to i32
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 %c2i, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.2, i32 0, i32 0), i32 33, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %call = call i8 @grade()
  %c2i2 = sext i8 %call to i32
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.4, i32 0, i32 0), i32 %c2i2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.5, i32 0, i32 0), i32 10)
  ret i32 0
}

declare i32 @printf(i8*, ...)
```
