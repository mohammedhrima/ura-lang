# variables

## index

- 001 — hero stats: declare all common types
- 002 — reassignment: hp loss, xp gain, mp drain
- 003 — all primitive types in one place

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
