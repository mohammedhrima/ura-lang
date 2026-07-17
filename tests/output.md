# output

## index

- 001 — basic output: strings, ints, floats, bools
- 002 — output with expressions: arithmetic inline
- 003 — output all supported types in one block
- 004 — output() auto-formats each argument by its type

## 001 — basic output: strings, ints, floats, bools

```ura
// output/001.ura - basic output: strings, ints, floats, bools

main():
    output("=== Aldric enters the dungeon ===\n")
    output("Floor: ", 1, "\n")
    output("Alive: ", True, "\n")
    output("Speed: ", 1.5, "\n")
    output("Ready to fight!\n")
```

```tree
fn main() : int
├─ output : void
│  └─ chars "=== Aldric enters the dungeon ===\n"
├─ output : void
│  ├─ chars "Floor: "
│  ├─ int 1
│  └─ chars "\n"
├─ output : void
│  ├─ chars "Alive: "
│  ├─ bool True
│  └─ chars "\n"
├─ output : void
│  ├─ chars "Speed: "
│  ├─ float 1.5
│  └─ chars "\n"
└─ output : void
   └─ chars "Ready to fight!\n"
```

```out
=== Aldric enters the dungeon ===
Floor: 1
Alive: True
Speed: 1.500000
Ready to fight!
```

```err
```

```ll

@str = private unnamed_addr constant [35 x i8] c"=== Aldric enters the dungeon ===\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.1 = private unnamed_addr constant [8 x i8] c"Floor: \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.3 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.4 = private unnamed_addr constant [8 x i8] c"Alive: \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1
@str.7 = private unnamed_addr constant [8 x i8] c"Speed: \00", align 1
@str.8 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.9 = private unnamed_addr constant [7 x i8] c"%s%f%s\00", align 1
@str.10 = private unnamed_addr constant [17 x i8] c"Ready to fight!\0A\00", align 1
@fmt.11 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i32 @main() {
entry:
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([35 x i8], [35 x i8]* @str, i32 0, i32 0))
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.3, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.1, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.6, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.4, i32 0, i32 0), i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.9, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.7, i32 0, i32 0), double 1.500000e+00, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.8, i32 0, i32 0))
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.11, i32 0, i32 0), i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.10, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 002 — output with expressions: arithmetic inline

```ura
// output/002.ura - output with expressions: arithmetic inline

main():
    output("Damage dealt: ", 25 - 8, "\n")
    output("XP gained:    ", 3 * 50, "\n")
    output("Gold split:   ", 120 / 4, "\n")
    output("Bonus:        ", 7 + 3 * 10, "\n")
```

```tree
fn main() : int
├─ output : void
│  ├─ chars "Damage dealt: "
│  ├─ - : int
│  │  ├─ int 25
│  │  └─ int 8
│  └─ chars "\n"
├─ output : void
│  ├─ chars "XP gained:    "
│  ├─ * : int
│  │  ├─ int 3
│  │  └─ int 50
│  └─ chars "\n"
├─ output : void
│  ├─ chars "Gold split:   "
│  ├─ / : int
│  │  ├─ int 120
│  │  └─ int 4
│  └─ chars "\n"
└─ output : void
   ├─ chars "Bonus:        "
   ├─ + : int
   │  ├─ int 7
   │  └─ * : int
   │     ├─ int 3
   │     └─ int 10
   └─ chars "\n"
```

```out
Damage dealt: 17
XP gained:    150
Gold split:   30
Bonus:        37
```

```err
```

```ll

@str = private unnamed_addr constant [15 x i8] c"Damage dealt: \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.2 = private unnamed_addr constant [15 x i8] c"XP gained:    \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.5 = private unnamed_addr constant [15 x i8] c"Gold split:   \00", align 1
@trap_msg = private unnamed_addr constant [200 x i8] c"runtime error: Division by zero\0A  002.ura:6:34\0A  |\0A6 |     output(\22Gold split:   \22, 120 / 4, \22\\n\22)\0A  |                                  ^\0A\00", align 1
@str.6 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.7 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.8 = private unnamed_addr constant [15 x i8] c"Bonus:        \00", align 1
@str.9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.10 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define i32 @main() {
entry:
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str, i32 0, i32 0), i32 17, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.4, i32 0, i32 0), i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.2, i32 0, i32 0), i32 150, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  br i1 false, label %trap, label %cont

trap:                                             ; preds = %entry
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([200 x i8], [200 x i8]* @trap_msg, i32 0, i32 0), i64 199)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.7, i32 0, i32 0), i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.5, i32 0, i32 0), i32 30, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.6, i32 0, i32 0))
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.10, i32 0, i32 0), i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.8, i32 0, i32 0), i32 37, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.9, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)
```

## 003 — output all supported types in one block

```ura
// output/003.ura - output all supported types in one block

main():
    name  chars = "Aldric"
    hp    int   = 100
    grade char  = 'S'
    alive bool  = True
    speed float = 1.75

    output("Name:  ", name,  "\n")
    output("HP:    ", hp,    "\n")
    output("Grade: ", grade, "\n")
    output("Alive: ", alive, "\n")
    output("Speed: ", speed, "\n")
```

```tree
fn main() : int
├─ = : chars
│  ├─ name : chars
│  └─ chars "Aldric"
├─ = : int
│  ├─ hp : int
│  └─ int 100
├─ = : char
│  ├─ grade : char
│  └─ char 'S'
├─ = : bool
│  ├─ alive : bool
│  └─ bool True
├─ = : float
│  ├─ speed : float
│  └─ float 1.75
├─ output : void
│  ├─ chars "Name:  "
│  ├─ name : chars
│  └─ chars "\n"
├─ output : void
│  ├─ chars "HP:    "
│  ├─ hp : int
│  └─ chars "\n"
├─ output : void
│  ├─ chars "Grade: "
│  ├─ grade : char
│  └─ chars "\n"
├─ output : void
│  ├─ chars "Alive: "
│  ├─ alive : bool
│  └─ chars "\n"
└─ output : void
   ├─ chars "Speed: "
   ├─ speed : float
   └─ chars "\n"
```

```out
Name:  Aldric
HP:    100
Grade: S
Alive: True
Speed: 1.750000
```

```err
```

```ll

@str = private unnamed_addr constant [7 x i8] c"Aldric\00", align 1
@str.1 = private unnamed_addr constant [8 x i8] c"Name:  \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1
@str.3 = private unnamed_addr constant [8 x i8] c"HP:    \00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.5 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.6 = private unnamed_addr constant [8 x i8] c"Grade: \00", align 1
@str.7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.8 = private unnamed_addr constant [7 x i8] c"%s%c%s\00", align 1
@str.9 = private unnamed_addr constant [8 x i8] c"Alive: \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.10 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.11 = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1
@str.12 = private unnamed_addr constant [8 x i8] c"Speed: \00", align 1
@str.13 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.14 = private unnamed_addr constant [7 x i8] c"%s%f%s\00", align 1

define i32 @main() {
entry:
  %name = alloca i8*, align 8
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i8** %name, align 8
  %hp = alloca i32, align 4
  store i32 100, i32* %hp, align 4
  %grade = alloca i8, align 1
  store i8 83, i8* %grade, align 1
  %alive = alloca i1, align 1
  store i1 true, i1* %alive, align 1
  %speed = alloca float, align 4
  store float 1.750000e+00, float* %speed, align 4
  %name1 = load i8*, i8** %name, align 8
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.1, i32 0, i32 0), i8* %name1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %hp2 = load i32, i32* %hp, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.5, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.3, i32 0, i32 0), i32 %hp2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  %grade3 = load i8, i8* %grade, align 1
  %c2i = sext i8 %grade3 to i32
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.8, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.6, i32 0, i32 0), i32 %c2i, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  %alive4 = load i1, i1* %alive, align 1
  %bool_str = select i1 %alive4, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.11, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.9, i32 0, i32 0), i8* %bool_str, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.10, i32 0, i32 0))
  %speed5 = load float, float* %speed, align 4
  %f2d = fpext float %speed5 to double
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.14, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.12, i32 0, i32 0), double %f2d, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.13, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 004 — output() auto-formats each argument by its type

```ura
// output/004.ura - output() auto-formats each argument by its type

main():
    name chars = "ura"
    n int = 42
    ok bool = True
    output("hello ", name, " ", n, " ", ok, "\n")
    return 0
```

```tree
fn main() : int
├─ = : chars
│  ├─ name : chars
│  └─ chars "ura"
├─ = : int
│  ├─ n : int
│  └─ int 42
├─ = : bool
│  ├─ ok : bool
│  └─ bool True
├─ output : void
│  ├─ chars "hello "
│  ├─ name : chars
│  ├─ chars " "
│  ├─ n : int
│  ├─ chars " "
│  ├─ ok : bool
│  └─ chars "\n"
└─ return
   └─ int 0
```

```out
hello ura 42 True
```

```err
```

```ll

@str = private unnamed_addr constant [4 x i8] c"ura\00", align 1
@str.1 = private unnamed_addr constant [7 x i8] c"hello \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c" \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [15 x i8] c"%s%s%s%d%s%s%s\00", align 1

define i32 @main() {
entry:
  %name = alloca i8*, align 8
  store i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i8** %name, align 8
  %n = alloca i32, align 4
  store i32 42, i32* %n, align 4
  %ok = alloca i1, align 1
  store i1 true, i1* %ok, align 1
  %name1 = load i8*, i8** %name, align 8
  %n2 = load i32, i32* %n, align 4
  %ok3 = load i1, i1* %ok, align 1
  %bool_str = select i1 %ok3, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.1, i32 0, i32 0), i8* %name1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0), i32 %n2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0), i8* %bool_str, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```
