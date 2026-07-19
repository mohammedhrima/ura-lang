# variables

## index

- 001 — hero stats: declare all common types
- 002 — reassignment: hp loss, xp gain, mp drain
- 003 — all primitive types in one place
- 004 — a use before a shadowing declaration binds to the outer one
- 005 — global variables: scalars, arrays, forward reference, shadowing
- 006 — a global struct is initialised and dropped like a local
- 007 — a global is dropped on an early return too
- 008 — an array type carries no size

## 001 — hero stats: declare all common types

```ura
// variables/001.ura - hero stats: declare all common types

main():
    name  chars = "Aldric"
    hp    int   = 100
    mp    int   = 50
    alive bool  = True
    speed float = 1.5
    output("=== ", name, " enters the dungeon ===\n")
    output("HP: ", hp, " | MP: ", mp, " | Speed: ", speed, "\n")
    output("Alive: ", alive, "\n")
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

fn main() : int
├─ = : chars
│  ├─ name : chars
│  └─ chars "Aldric"
├─ = : int
│  ├─ hp : int
│  └─ int 100
├─ = : int
│  ├─ mp : int
│  └─ int 50
├─ = : bool
│  ├─ alive : bool
│  └─ bool True
├─ = : float
│  ├─ speed : float
│  └─ float 1.5
├─ output : void
│  ├─ chars "=== "
│  ├─ name : chars
│  └─ chars " enters the dungeon ===\n"
├─ output : void
│  ├─ chars "HP: "
│  ├─ hp : int
│  ├─ chars " | MP: "
│  ├─ mp : int
│  ├─ chars " | Speed: "
│  ├─ speed : float
│  └─ chars "\n"
└─ output : void
   ├─ chars "Alive: "
   ├─ alive : bool
   └─ chars "\n"
```

```out
=== Aldric enters the dungeon ===
HP: 100 | MP: 50 | Speed: 1.500000
Alive: True
```

```err
```

```ll

@str = private unnamed_addr constant [7 x i8] c"Aldric\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"=== \00", align 1
@str.2 = private unnamed_addr constant [25 x i8] c" enters the dungeon ===\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1
@str.3 = private unnamed_addr constant [5 x i8] c"HP: \00", align 1
@str.4 = private unnamed_addr constant [8 x i8] c" | MP: \00", align 1
@str.5 = private unnamed_addr constant [11 x i8] c" | Speed: \00", align 1
@str.6 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.7 = private unnamed_addr constant [15 x i8] c"%s%d%s%d%s%f%s\00", align 1
@str.8 = private unnamed_addr constant [8 x i8] c"Alive: \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.10 = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1

define i32 @main() {
entry:
  %name = alloca i8*, align 8
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i8** %name, align 8
  %hp = alloca i32, align 4
  store i32 100, i32* %hp, align 4
  %mp = alloca i32, align 4
  store i32 50, i32* %mp, align 4
  %alive = alloca i1, align 1
  store i1 true, i1* %alive, align 1
  %speed = alloca float, align 4
  store float 1.500000e+00, float* %speed, align 4
  %name1 = load i8*, i8** %name, align 8
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0), i8* %name1, i8* getelementptr inbounds ([25 x i8], [25 x i8]* @str.2, i32 0, i32 0))
  %hp2 = load i32, i32* %hp, align 4
  %mp3 = load i32, i32* %mp, align 4
  %speed4 = load float, float* %speed, align 4
  %f2d = fpext float %speed4 to double
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @fmt.7, i32 0, i32 0), i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.3, i32 0, i32 0), i32 %hp2, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.4, i32 0, i32 0), i32 %mp3, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.5, i32 0, i32 0), double %f2d, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.6, i32 0, i32 0))
  %alive5 = load i1, i1* %alive, align 1
  %bool_str = select i1 %alive5, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.10, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.8, i32 0, i32 0), i8* %bool_str, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.9, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 002 — reassignment: hp loss, xp gain, mp drain

```ura
// variables/002.ura - reassignment: hp loss, xp gain, mp drain

main():
    hp int = 100
    mp int = 50
    xp int = 0

    hp = hp - 12
    mp -= 10
    xp += 50

    output("After combat:\n")
    output("HP: ", hp, "\n")
    output("MP: ", mp, "\n")
    output("XP: ", xp, "\n")
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

fn main() : int
├─ = : int
│  ├─ hp : int
│  └─ int 100
├─ = : int
│  ├─ mp : int
│  └─ int 50
├─ = : int
│  ├─ xp : int
│  └─ int 0
├─ = : int
│  ├─ hp : int
│  └─ - : int
│     ├─ hp : int
│     └─ int 12
├─ -= : int
│  ├─ mp : int
│  └─ int 10
├─ += : int
│  ├─ xp : int
│  └─ int 50
├─ output : void
│  └─ chars "After combat:\n"
├─ output : void
│  ├─ chars "HP: "
│  ├─ hp : int
│  └─ chars "\n"
├─ output : void
│  ├─ chars "MP: "
│  ├─ mp : int
│  └─ chars "\n"
└─ output : void
   ├─ chars "XP: "
   ├─ xp : int
   └─ chars "\n"
```

```out
After combat:
HP: 88
MP: 40
XP: 50
```

```err
```

```ll

@str = private unnamed_addr constant [15 x i8] c"After combat:\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.1 = private unnamed_addr constant [5 x i8] c"HP: \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.3 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.4 = private unnamed_addr constant [5 x i8] c"MP: \00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.7 = private unnamed_addr constant [5 x i8] c"XP: \00", align 1
@str.8 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.9 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define i32 @main() {
entry:
  %hp = alloca i32, align 4
  store i32 100, i32* %hp, align 4
  %mp = alloca i32, align 4
  store i32 50, i32* %mp, align 4
  %xp = alloca i32, align 4
  store i32 0, i32* %xp, align 4
  %hp1 = load i32, i32* %hp, align 4
  %sub = sub i32 %hp1, 12
  store i32 %sub, i32* %hp, align 4
  %cur = load i32, i32* %mp, align 4
  %sub2 = sub i32 %cur, 10
  store i32 %sub2, i32* %mp, align 4
  %cur3 = load i32, i32* %xp, align 4
  %add = add i32 %cur3, 50
  store i32 %add, i32* %xp, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str, i32 0, i32 0))
  %hp4 = load i32, i32* %hp, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.3, i32 0, i32 0), i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.1, i32 0, i32 0), i32 %hp4, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %mp5 = load i32, i32* %mp, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.6, i32 0, i32 0), i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.4, i32 0, i32 0), i32 %mp5, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  %xp6 = load i32, i32* %xp, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.9, i32 0, i32 0), i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.7, i32 0, i32 0), i32 %xp6, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.8, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 003 — all primitive types in one place

```ura
// variables/003.ura - all primitive types in one place

main():
    name   chars = "Aldric"
    hp     int   = 100
    xp     long  = 999999 as long
    level  short = 12 as short
    grade  char  = 'S'
    alive  bool  = True
    ratio  float = 0.85

    output("name:  ", name,  "\n")
    output("hp:    ", hp,    "\n")
    output("xp:    ", xp,    "\n")
    output("level: ", level, "\n")
    output("grade: ", grade, "\n")
    output("alive: ", alive, "\n")
    output("ratio: ", ratio, "\n")
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

fn main() : int
├─ = : chars
│  ├─ name : chars
│  └─ chars "Aldric"
├─ = : int
│  ├─ hp : int
│  └─ int 100
├─ = : long
│  ├─ xp : long
│  └─ cast : long
│     └─ int 999999
├─ = : short
│  ├─ level : short
│  └─ cast : short
│     └─ int 12
├─ = : char
│  ├─ grade : char
│  └─ char 'S'
├─ = : bool
│  ├─ alive : bool
│  └─ bool True
├─ = : float
│  ├─ ratio : float
│  └─ float 0.85
├─ output : void
│  ├─ chars "name:  "
│  ├─ name : chars
│  └─ chars "\n"
├─ output : void
│  ├─ chars "hp:    "
│  ├─ hp : int
│  └─ chars "\n"
├─ output : void
│  ├─ chars "xp:    "
│  ├─ xp : long
│  └─ chars "\n"
├─ output : void
│  ├─ chars "level: "
│  ├─ level : short
│  └─ chars "\n"
├─ output : void
│  ├─ chars "grade: "
│  ├─ grade : char
│  └─ chars "\n"
├─ output : void
│  ├─ chars "alive: "
│  ├─ alive : bool
│  └─ chars "\n"
└─ output : void
   ├─ chars "ratio: "
   ├─ ratio : float
   └─ chars "\n"
```

```out
name:  Aldric
hp:    100
xp:    999999
level: 12
grade: S
alive: True
ratio: 0.850000
```

```err
```

```ll

@str = private unnamed_addr constant [7 x i8] c"Aldric\00", align 1
@str.1 = private unnamed_addr constant [8 x i8] c"name:  \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1
@str.3 = private unnamed_addr constant [8 x i8] c"hp:    \00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.5 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.6 = private unnamed_addr constant [8 x i8] c"xp:    \00", align 1
@str.7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.8 = private unnamed_addr constant [9 x i8] c"%s%lld%s\00", align 1
@str.9 = private unnamed_addr constant [8 x i8] c"level: \00", align 1
@str.10 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.11 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.12 = private unnamed_addr constant [8 x i8] c"grade: \00", align 1
@str.13 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.14 = private unnamed_addr constant [7 x i8] c"%s%c%s\00", align 1
@str.15 = private unnamed_addr constant [8 x i8] c"alive: \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.16 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.17 = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1
@str.18 = private unnamed_addr constant [8 x i8] c"ratio: \00", align 1
@str.19 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.20 = private unnamed_addr constant [7 x i8] c"%s%f%s\00", align 1

define i32 @main() {
entry:
  %name = alloca i8*, align 8
  store i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i8** %name, align 8
  %hp = alloca i32, align 4
  store i32 100, i32* %hp, align 4
  %xp = alloca i64, align 8
  store i64 999999, i64* %xp, align 4
  %level = alloca i16, align 2
  store i16 12, i16* %level, align 2
  %grade = alloca i8, align 1
  store i8 83, i8* %grade, align 1
  %alive = alloca i1, align 1
  store i1 true, i1* %alive, align 1
  %ratio = alloca float, align 4
  store float 0x3FEB333340000000, float* %ratio, align 4
  %name1 = load i8*, i8** %name, align 8
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.1, i32 0, i32 0), i8* %name1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %hp2 = load i32, i32* %hp, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.5, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.3, i32 0, i32 0), i32 %hp2, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  %xp3 = load i64, i64* %xp, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.8, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.6, i32 0, i32 0), i64 %xp3, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  %level4 = load i16, i16* %level, align 2
  %s2i = sext i16 %level4 to i32
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.11, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.9, i32 0, i32 0), i32 %s2i, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.10, i32 0, i32 0))
  %grade5 = load i8, i8* %grade, align 1
  %c2i = sext i8 %grade5 to i32
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.14, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.12, i32 0, i32 0), i32 %c2i, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.13, i32 0, i32 0))
  %alive6 = load i1, i1* %alive, align 1
  %bool_str = select i1 %alive6, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.17, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.15, i32 0, i32 0), i8* %bool_str, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.16, i32 0, i32 0))
  %ratio7 = load float, float* %ratio, align 4
  %f2d = fpext float %ratio7 to double
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.20, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.18, i32 0, i32 0), double %f2d, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.19, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 004 — a use before a shadowing declaration binds to the outer one

```ura
// variables/004.ura - use before a shadowing declaration

fn pick(n int) int:
    return n * 2

main():
    x int = 1
    if 1 < 2:
        // resolves to the OUTER x, even though an inner x follows
        output("outer ", x, "\n")
        x int = 2
        output("inner ", x, "\n")
    output("after ", x, "\n")

    y int = pick(3)
    while y > 0:
        output("loop y ", y, "\n")
        y int = 0
        output("shadow y ", y, "\n")
        break
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

fn pick(n : int) : int
└─ return
   └─ * : int
      ├─ n : int
      └─ int 2

fn main() : int
├─ = : int
│  ├─ x : int
│  └─ int 1
├─ if
│  ├─ condition < : bool
│  │  ├─ int 1
│  │  └─ int 2
│  ├─ output : void
│  │  ├─ chars "outer "
│  │  ├─ x : int
│  │  └─ chars "\n"
│  ├─ = : int
│  │  ├─ x : int
│  │  └─ int 2
│  └─ output : void
│     ├─ chars "inner "
│     ├─ x : int
│     └─ chars "\n"
├─ output : void
│  ├─ chars "after "
│  ├─ x : int
│  └─ chars "\n"
├─ = : int
│  ├─ y : int
│  └─ call pick : int
│     └─ int 3
└─ while
   ├─ condition > : bool
   │  ├─ y : int
   │  └─ int 0
   ├─ output : void
   │  ├─ chars "loop y "
   │  ├─ y : int
   │  └─ chars "\n"
   ├─ = : int
   │  ├─ y : int
   │  └─ int 0
   ├─ output : void
   │  ├─ chars "shadow y "
   │  ├─ y : int
   │  └─ chars "\n"
   └─ break
```

```out
outer 1
inner 2
after 1
loop y 6
shadow y 0
```

```err
```

```ll

@str = private unnamed_addr constant [7 x i8] c"outer \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.2 = private unnamed_addr constant [7 x i8] c"inner \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.5 = private unnamed_addr constant [7 x i8] c"after \00", align 1
@str.6 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.7 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.8 = private unnamed_addr constant [8 x i8] c"loop y \00", align 1
@str.9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.10 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.11 = private unnamed_addr constant [10 x i8] c"shadow y \00", align 1
@str.12 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.13 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define i32 @pick(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %n1 = load i32, i32* %n, align 4
  %mul = mul i32 %n1, 2
  ret i32 %mul
}

define i32 @main() {
entry:
  %x = alloca i32, align 4
  store i32 1, i32* %x, align 4
  br i1 true, label %then, label %endif

endif:                                            ; preds = %then, %entry
  %x4 = load i32, i32* %x, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.7, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.5, i32 0, i32 0), i32 %x4, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.6, i32 0, i32 0))
  %y = alloca i32, align 4
  %call = call i32 @pick(i32 3)
  store i32 %call, i32* %y, align 4
  br label %while.cond

then:                                             ; preds = %entry
  %x1 = load i32, i32* %x, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str, i32 0, i32 0), i32 %x1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %x2 = alloca i32, align 4
  store i32 2, i32* %x2, align 4
  %x3 = load i32, i32* %x2, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.4, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.2, i32 0, i32 0), i32 %x3, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  br label %endif

while.cond:                                       ; preds = %endif
  %y5 = load i32, i32* %y, align 4
  %gt = icmp sgt i32 %y5, 0
  br i1 %gt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %y6 = load i32, i32* %y, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.10, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.8, i32 0, i32 0), i32 %y6, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.9, i32 0, i32 0))
  %y7 = alloca i32, align 4
  store i32 0, i32* %y7, align 4
  %y8 = load i32, i32* %y7, align 4
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.13, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.11, i32 0, i32 0), i32 %y8, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.12, i32 0, i32 0))
  br label %while.end

while.end:                                        ; preds = %while.body, %while.cond
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 005 — global variables: scalars, arrays, forward reference, shadowing

```ura
// variables/005.ura - top-level variables

name  chars = "ura"
ratio float = 1.5
flag  bool  = True
xs    int[] = int[3]
heap  int[] = new int[2]

fn touch() void:
    total += 1

main():
    output(name, " ", ratio, " ", flag, "\n")
    // forward reference: total is declared below main
    output("total starts ", total, "\n")
    touch()
    touch()
    output("total now ", total, "\n")

    xs[0] = 7
    heap[1] = 9
    output("xs ", xs[0], " len ", xs.len)
    output("  heap ", heap[1], " len ", heap.len, "\n")
    clean heap

    // a local shadows the global for the rest of this scope
    total int = 99
    output("local total ", total, "\n")

total int = 7
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

= : chars
├─ name : chars
└─ chars "ura"

= : float
├─ ratio : float
└─ float 1.5

= : bool
├─ flag : bool
└─ bool True

= : array
├─ xs : int[]
└─ array : int[]
   └─ int 3

= : array
├─ heap : int[]
└─ array : int[]
   └─ int 2

fn touch() : void
└─ += : int
   ├─ total : int
   └─ int 1

fn main() : int
├─ output : void
│  ├─ name : chars
│  ├─ chars " "
│  ├─ ratio : float
│  ├─ chars " "
│  ├─ flag : bool
│  └─ chars "\n"
├─ output : void
│  ├─ chars "total starts "
│  ├─ total : int
│  └─ chars "\n"
├─ call touch : void
├─ call touch : void
├─ output : void
│  ├─ chars "total now "
│  ├─ total : int
│  └─ chars "\n"
├─ = : int
│  ├─ index : int
│  │  ├─ xs : int[]
│  │  └─ int 0
│  └─ int 7
├─ = : int
│  ├─ index : int
│  │  ├─ heap : int[]
│  │  └─ int 1
│  └─ int 9
├─ output : void
│  ├─ chars "xs "
│  ├─ index : int
│  │  ├─ xs : int[]
│  │  └─ int 0
│  ├─ chars " len "
│  └─ .len : int
│     └─ xs : int[]
├─ output : void
│  ├─ chars "  heap "
│  ├─ index : int
│  │  ├─ heap : int[]
│  │  └─ int 1
│  ├─ chars " len "
│  ├─ .len : int
│  │  └─ heap : int[]
│  └─ chars "\n"
├─ clean : void
│  └─ heap : int[]
├─ = : int
│  ├─ total : int
│  └─ int 99
└─ output : void
   ├─ chars "local total "
   ├─ total : int
   └─ chars "\n"

= : int
├─ total : int
└─ int 7
```

```out
ura 1.500000 True
total starts 7
total now 9
xs 7 len 3  heap 9 len 2
local total 99
```

```err
```

```ll

@name = internal global i8* null
@ratio = internal global float 0.000000e+00
@flag = internal global i1 false
@xs = internal global { i32*, i64 } zeroinitializer
@heap = internal global { i32*, i64 } zeroinitializer
@total = internal global i32 0
@str = private unnamed_addr constant [4 x i8] c"ura\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c" \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [13 x i8] c"%s%s%f%s%s%s\00", align 1
@str.4 = private unnamed_addr constant [14 x i8] c"total starts \00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.7 = private unnamed_addr constant [11 x i8] c"total now \00", align 1
@str.8 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.9 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.10 = private unnamed_addr constant [4 x i8] c"xs \00", align 1
@str.11 = private unnamed_addr constant [6 x i8] c" len \00", align 1
@fmt.12 = private unnamed_addr constant [9 x i8] c"%s%d%s%d\00", align 1
@str.13 = private unnamed_addr constant [8 x i8] c"  heap \00", align 1
@str.14 = private unnamed_addr constant [6 x i8] c" len \00", align 1
@str.15 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.16 = private unnamed_addr constant [11 x i8] c"%s%d%s%d%s\00", align 1
@str.17 = private unnamed_addr constant [13 x i8] c"local total \00", align 1
@str.18 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.19 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define void @touch() {
entry:
  %cur = load i32, i32* @total, align 4
  %add = add i32 %cur, 1
  store i32 %add, i32* @total, align 4
  ret void
}

define i32 @main() {
entry:
  store i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str, i32 0, i32 0), i8** @name, align 8
  store float 1.500000e+00, float* @ratio, align 4
  store i1 true, i1* @flag, align 1
  %arr = alloca i32, i64 3, align 4
  %0 = bitcast i32* %arr to i8*
  call void @llvm.memset.p0i8.i64(i8* %0, i8 0, i64 12, i1 false)
  %arr.ptr = insertvalue { i32*, i64 } undef, i32* %arr, 0
  %arr.len = insertvalue { i32*, i64 } %arr.ptr, i64 3, 1
  store { i32*, i64 } %arr.len, { i32*, i64 }* @xs, align 8
  %heap = call i8* @calloc(i64 2, i64 4)
  %arr1 = bitcast i8* %heap to i32*
  %arr.ptr2 = insertvalue { i32*, i64 } undef, i32* %arr1, 0
  %arr.len3 = insertvalue { i32*, i64 } %arr.ptr2, i64 2, 1
  store { i32*, i64 } %arr.len3, { i32*, i64 }* @heap, align 8
  store i32 7, i32* @total, align 4
  %name = load i8*, i8** @name, align 8
  %ratio = load float, float* @ratio, align 4
  %f2d = fpext float %ratio to double
  %flag = load i1, i1* @flag, align 1
  %bool_str = select i1 %flag, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i8* %name, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), double %f2d, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0), i8* %bool_str, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %total = load i32, i32* @total, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.6, i32 0, i32 0), i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.4, i32 0, i32 0), i32 %total, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  call void @touch()
  call void @touch()
  %total4 = load i32, i32* @total, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.9, i32 0, i32 0), i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.7, i32 0, i32 0), i32 %total4, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.8, i32 0, i32 0))
  %xs = load { i32*, i64 }, { i32*, i64 }* @xs, align 8
  %arr.data = extractvalue { i32*, i64 } %xs, 0
  %arr.at = getelementptr i32, i32* %arr.data, i32 0
  store i32 7, i32* %arr.at, align 4
  %heap5 = load { i32*, i64 }, { i32*, i64 }* @heap, align 8
  %arr.data6 = extractvalue { i32*, i64 } %heap5, 0
  %arr.at7 = getelementptr i32, i32* %arr.data6, i32 1
  store i32 9, i32* %arr.at7, align 4
  %xs8 = load { i32*, i64 }, { i32*, i64 }* @xs, align 8
  %arr.data9 = extractvalue { i32*, i64 } %xs8, 0
  %arr.at10 = getelementptr i32, i32* %arr.data9, i32 0
  %idx = load i32, i32* %arr.at10, align 4
  %xs11 = load { i32*, i64 }, { i32*, i64 }* @xs, align 8
  %len = extractvalue { i32*, i64 } %xs11, 1
  %len12 = trunc i64 %len to i32
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.12, i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.10, i32 0, i32 0), i32 %idx, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.11, i32 0, i32 0), i32 %len12)
  %heap13 = load { i32*, i64 }, { i32*, i64 }* @heap, align 8
  %arr.data14 = extractvalue { i32*, i64 } %heap13, 0
  %arr.at15 = getelementptr i32, i32* %arr.data14, i32 1
  %idx16 = load i32, i32* %arr.at15, align 4
  %heap17 = load { i32*, i64 }, { i32*, i64 }* @heap, align 8
  %len18 = extractvalue { i32*, i64 } %heap17, 1
  %len19 = trunc i64 %len18 to i32
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.16, i32 0, i32 0), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.13, i32 0, i32 0), i32 %idx16, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.14, i32 0, i32 0), i32 %len19, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.15, i32 0, i32 0))
  %arr20 = load { i32*, i64 }, { i32*, i64 }* @heap, align 8
  %arr.data21 = extractvalue { i32*, i64 } %arr20, 0
  %free.ptr = bitcast i32* %arr.data21 to i8*
  call void @free(i8* %free.ptr)
  store { i32*, i64 } zeroinitializer, { i32*, i64 }* @heap, align 8
  %total22 = alloca i32, align 4
  store i32 99, i32* %total22, align 4
  %total23 = load i32, i32* %total22, align 4
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.19, i32 0, i32 0), i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.17, i32 0, i32 0), i32 %total23, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.18, i32 0, i32 0))
  ret i32 0
}

; Function Attrs: argmemonly nofree nounwind willreturn writeonly
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i1 immarg) #0

declare i8* @calloc(i64, i64)

declare i32 @printf(i8*, ...)

declare void @free(i8*)

attributes #0 = { argmemonly nofree nounwind willreturn writeonly }
```

## 006 — a global struct is initialised and dropped like a local

```ura
// variables/006.ura - a global struct, built and destroyed

struct Counter:
    hits int

    pub fn create(n int) Counter:
        c Counter
        c.hits = n
        return c

    operator =(other Counter) void:
        self.hits = other.hits

    fn bump() void:
        self.hits += 1

    operator drop:
        output("DROP at ", self.hits, "\n")

tally Counter = Counter::create(10)

fn touch() void:
    tally.bump()

main():
    output("start ", tally.hits, "\n")
    touch()
    touch()
    output("after ", tally.hits, "\n")
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

struct Counter
├─ hits : int
├─ fn Counter.create(n : int) : STRUCT_CALL
│  ├─ c : STRUCT_CALL
│  ├─ = : int
│  │  ├─ .hits : int
│  │  │  └─ c : STRUCT_CALL
│  │  └─ n : int
│  └─ return
│     └─ c : STRUCT_CALL
├─ fn Counter.=.Counter(self : STRUCT_CALL, other : STRUCT_CALL) : void
│  └─ = : int
│     ├─ .hits : int
│     │  └─ self : STRUCT_CALL
│     └─ .hits : int
│        └─ other : STRUCT_CALL
├─ fn Counter.bump(self : STRUCT_CALL) : void
│  └─ += : int
│     ├─ .hits : int
│     │  └─ self : STRUCT_CALL
│     └─ int 1
└─ fn Counter.drop(self : STRUCT_CALL) : void
   └─ output : void
      ├─ chars "DROP at "
      ├─ .hits : int
      │  └─ self : STRUCT_CALL
      └─ chars "\n"

= : STRUCT_CALL
├─ tally : STRUCT_CALL
└─ call create : STRUCT_CALL
   └─ int 10

fn touch() : void
└─ call bump : void
   └─ tally : STRUCT_CALL

fn main() : int
├─ output : void
│  ├─ chars "start "
│  ├─ .hits : int
│  │  └─ tally : STRUCT_CALL
│  └─ chars "\n"
├─ call touch : void
├─ call touch : void
└─ output : void
   ├─ chars "after "
   ├─ .hits : int
   │  └─ tally : STRUCT_CALL
   └─ chars "\n"
```

```out
start 10
after 12
DROP at 12
```

```err
```

```ll

%Counter = type { i32 }

@tally = internal global %Counter zeroinitializer
@str = private unnamed_addr constant [9 x i8] c"DROP at \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.2 = private unnamed_addr constant [7 x i8] c"start \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.5 = private unnamed_addr constant [7 x i8] c"after \00", align 1
@str.6 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.7 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define %Counter @Counter.create(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %c = alloca %Counter, align 8
  store %Counter zeroinitializer, %Counter* %c, align 4
  %hits = getelementptr %Counter, %Counter* %c, i32 0, i32 0
  %n1 = load i32, i32* %n, align 4
  store i32 %n1, i32* %hits, align 4
  %c2 = load %Counter, %Counter* %c, align 4
  ret %Counter %c2
}

define void @"Counter.=.Counter"(%Counter* %0, %Counter %1) {
entry:
  %self = alloca %Counter*, align 8
  store %Counter* %0, %Counter** %self, align 8
  %other = alloca %Counter, align 8
  store %Counter %1, %Counter* %other, align 4
  %ref = load %Counter*, %Counter** %self, align 8
  %hits = getelementptr %Counter, %Counter* %ref, i32 0, i32 0
  %hits1 = getelementptr %Counter, %Counter* %other, i32 0, i32 0
  %hits2 = load i32, i32* %hits1, align 4
  store i32 %hits2, i32* %hits, align 4
  ret void
}

define void @Counter.bump(%Counter* %0) {
entry:
  %self = alloca %Counter*, align 8
  store %Counter* %0, %Counter** %self, align 8
  %ref = load %Counter*, %Counter** %self, align 8
  %hits = getelementptr %Counter, %Counter* %ref, i32 0, i32 0
  %cur = load i32, i32* %hits, align 4
  %add = add i32 %cur, 1
  store i32 %add, i32* %hits, align 4
  ret void
}

define void @Counter.drop(%Counter* %0) {
entry:
  %self = alloca %Counter*, align 8
  store %Counter* %0, %Counter** %self, align 8
  %ref = load %Counter*, %Counter** %self, align 8
  %hits = getelementptr %Counter, %Counter* %ref, i32 0, i32 0
  %hits1 = load i32, i32* %hits, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0), i32 %hits1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)

define void @touch() {
entry:
  call void @Counter.bump(%Counter* @tally)
  ret void
}

define i32 @main() {
entry:
  %call = call %Counter @Counter.create(i32 10)
  store %Counter %call, %Counter* @tally, align 4
  %hits = load i32, i32* getelementptr inbounds (%Counter, %Counter* @tally, i32 0, i32 0), align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.4, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.2, i32 0, i32 0), i32 %hits, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  call void @touch()
  call void @touch()
  %hits1 = load i32, i32* getelementptr inbounds (%Counter, %Counter* @tally, i32 0, i32 0), align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.7, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.5, i32 0, i32 0), i32 %hits1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.6, i32 0, i32 0))
  call void @Counter.drop(%Counter* @tally)
  ret i32 0
}
```

## 007 — a global is dropped on an early return too

```ura
// variables/007.ura - global drop on the early-return path

struct Res:
    id int

    pub fn create(n int) Res:
        r Res
        r.id = n
        return r

    operator =(other Res) void:
        self.id = other.id

    operator drop:
        output("DROP ", self.id, "\n")

a Res = Res::create(1)
b Res = Res::create(2)

main():
    output("body\n")
    if 1 < 2:
        output("early\n")
        return 0
    output("unreachable\n")
```

```tree
proto fn printf(format : chars, ...) : int

proto fn calloc(len : long, size : long) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : int, ptr : chars, len : long) : long

proto fn exit(code : int) : void

struct Res
├─ id : int
├─ fn Res.create(n : int) : STRUCT_CALL
│  ├─ r : STRUCT_CALL
│  ├─ = : int
│  │  ├─ .id : int
│  │  │  └─ r : STRUCT_CALL
│  │  └─ n : int
│  └─ return
│     └─ r : STRUCT_CALL
├─ fn Res.=.Res(self : STRUCT_CALL, other : STRUCT_CALL) : void
│  └─ = : int
│     ├─ .id : int
│     │  └─ self : STRUCT_CALL
│     └─ .id : int
│        └─ other : STRUCT_CALL
└─ fn Res.drop(self : STRUCT_CALL) : void
   └─ output : void
      ├─ chars "DROP "
      ├─ .id : int
      │  └─ self : STRUCT_CALL
      └─ chars "\n"

= : STRUCT_CALL
├─ a : STRUCT_CALL
└─ call create : STRUCT_CALL
   └─ int 1

= : STRUCT_CALL
├─ b : STRUCT_CALL
└─ call create : STRUCT_CALL
   └─ int 2

fn main() : int
├─ output : void
│  └─ chars "body\n"
├─ if
│  ├─ condition < : bool
│  │  ├─ int 1
│  │  └─ int 2
│  ├─ output : void
│  │  └─ chars "early\n"
│  └─ return
│     └─ int 0
└─ output : void
   └─ chars "unreachable\n"
```

```out
body
early
DROP 2
DROP 1
```

```err
```

```ll

%Res = type { i32 }

@a = internal global %Res zeroinitializer
@b = internal global %Res zeroinitializer
@str = private unnamed_addr constant [6 x i8] c"DROP \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.2 = private unnamed_addr constant [6 x i8] c"body\0A\00", align 1
@fmt.3 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.4 = private unnamed_addr constant [7 x i8] c"early\0A\00", align 1
@fmt.5 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.6 = private unnamed_addr constant [13 x i8] c"unreachable\0A\00", align 1
@fmt.7 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define %Res @Res.create(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %r = alloca %Res, align 8
  store %Res zeroinitializer, %Res* %r, align 4
  %id = getelementptr %Res, %Res* %r, i32 0, i32 0
  %n1 = load i32, i32* %n, align 4
  store i32 %n1, i32* %id, align 4
  %r2 = load %Res, %Res* %r, align 4
  ret %Res %r2
}

define void @"Res.=.Res"(%Res* %0, %Res %1) {
entry:
  %self = alloca %Res*, align 8
  store %Res* %0, %Res** %self, align 8
  %other = alloca %Res, align 8
  store %Res %1, %Res* %other, align 4
  %ref = load %Res*, %Res** %self, align 8
  %id = getelementptr %Res, %Res* %ref, i32 0, i32 0
  %id1 = getelementptr %Res, %Res* %other, i32 0, i32 0
  %id2 = load i32, i32* %id1, align 4
  store i32 %id2, i32* %id, align 4
  ret void
}

define void @Res.drop(%Res* %0) {
entry:
  %self = alloca %Res*, align 8
  store %Res* %0, %Res** %self, align 8
  %ref = load %Res*, %Res** %self, align 8
  %id = getelementptr %Res, %Res* %ref, i32 0, i32 0
  %id1 = load i32, i32* %id, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str, i32 0, i32 0), i32 %id1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %call = call %Res @Res.create(i32 1)
  store %Res %call, %Res* @a, align 4
  %call1 = call %Res @Res.create(i32 2)
  store %Res %call1, %Res* @b, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.3, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.2, i32 0, i32 0))
  br i1 true, label %then, label %endif

endif:                                            ; preds = %entry
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.7, i32 0, i32 0), i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.6, i32 0, i32 0))
  call void @Res.drop(%Res* @b)
  call void @Res.drop(%Res* @a)
  ret i32 0

then:                                             ; preds = %entry
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.5, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.4, i32 0, i32 0))
  call void @Res.drop(%Res* @b)
  call void @Res.drop(%Res* @a)
  ret i32 0
}
```

## 008 — an array type carries no size

```ura
// variables/008.ura - a sized array type is rejected

main():
    xs int[3]
    xs[0] = 7
```

```tree
```

```out
```

```err
error: An array type carries no size; ex. 'arr int[] = int[3]' to make one
  008.ura:4:11
  |
4 |     xs int[3]
  |           ^
```

```ll
```
