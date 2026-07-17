# dungeon

## index

- floor04
- floor09b
- floor14a

## floor04

```ura
// dungeon/floor04.ura
fn clamp(val int, lo int, hi int) int:
    if val < lo: return lo
    if val > hi: return hi
    return val

fn damage(atk int, def int) int:
    return clamp(atk - def, 0, 999)

fn is_dead(hp int) bool:
    return hp <= 0

fn square(n int) int:      return n * n
fn is_digit(c char) bool:  return c >= '0' and c <= '9'

main():
    d int = damage(25, 8)
    output("Orc takes ", d, " damage — dead: ", is_dead(d - 60), "\n")
    output("4 squared: ", square(4), "\n")
```

```tree
fn clamp(val : int, lo : int, hi : int) : int
├─ if
│  ├─ condition < : bool
│  │  ├─ val : int
│  │  └─ lo : int
│  └─ return
│     └─ lo : int
├─ if
│  ├─ condition > : bool
│  │  ├─ val : int
│  │  └─ hi : int
│  └─ return
│     └─ hi : int
└─ return
   └─ val : int

fn damage(atk : int, def : int) : int
└─ return
   └─ call clamp : int
      ├─ - : int
      │  ├─ atk : int
      │  └─ def : int
      ├─ int 0
      └─ int 999

fn is_dead(hp : int) : bool
└─ return
   └─ <= : bool
      ├─ hp : int
      └─ int 0

fn square(n : int) : int
└─ return
   └─ * : int
      ├─ n : int
      └─ n : int

fn is_digit(c : char) : bool
└─ return
   └─ and : bool
      ├─ >= : bool
      │  ├─ c : char
      │  └─ char '0'
      └─ <= : bool
         ├─ c : char
         └─ char '9'

fn main() : int
├─ = : int
│  ├─ d : int
│  └─ call damage : int
│     ├─ int 25
│     └─ int 8
├─ output : void
│  ├─ chars "Orc takes "
│  ├─ d : int
│  ├─ chars " damage — dead: "
│  ├─ call is_dead : bool
│  │  └─ - : int
│  │     ├─ d : int
│  │     └─ int 60
│  └─ chars "\n"
└─ output : void
   ├─ chars "4 squared: "
   ├─ call square : int
   │  └─ int 4
   └─ chars "\n"
```

```out
Orc takes 17 damage — dead: True
4 squared: 16
```

```err
```

```ll

@str = private unnamed_addr constant [11 x i8] c"Orc takes \00", align 1
@str.1 = private unnamed_addr constant [19 x i8] c" damage \E2\80\94 dead: \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%s%d%s%s%s\00", align 1
@str.3 = private unnamed_addr constant [12 x i8] c"4 squared: \00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.5 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define i32 @clamp(i32 %0, i32 %1, i32 %2) {
entry:
  %val = alloca i32, align 4
  store i32 %0, i32* %val, align 4
  %lo = alloca i32, align 4
  store i32 %1, i32* %lo, align 4
  %hi = alloca i32, align 4
  store i32 %2, i32* %hi, align 4
  %val1 = load i32, i32* %val, align 4
  %lo2 = load i32, i32* %lo, align 4
  %lt = icmp slt i32 %val1, %lo2
  br i1 %lt, label %then, label %endif

endif:                                            ; preds = %entry
  %val6 = load i32, i32* %val, align 4
  %hi7 = load i32, i32* %hi, align 4
  %gt = icmp sgt i32 %val6, %hi7
  br i1 %gt, label %then5, label %endif4

then:                                             ; preds = %entry
  %lo3 = load i32, i32* %lo, align 4
  ret i32 %lo3

endif4:                                           ; preds = %endif
  %val9 = load i32, i32* %val, align 4
  ret i32 %val9

then5:                                            ; preds = %endif
  %hi8 = load i32, i32* %hi, align 4
  ret i32 %hi8
}

define i32 @damage(i32 %0, i32 %1) {
entry:
  %atk = alloca i32, align 4
  store i32 %0, i32* %atk, align 4
  %def = alloca i32, align 4
  store i32 %1, i32* %def, align 4
  %atk1 = load i32, i32* %atk, align 4
  %def2 = load i32, i32* %def, align 4
  %sub = sub i32 %atk1, %def2
  %call = call i32 @clamp(i32 %sub, i32 0, i32 999)
  ret i32 %call
}

define i1 @is_dead(i32 %0) {
entry:
  %hp = alloca i32, align 4
  store i32 %0, i32* %hp, align 4
  %hp1 = load i32, i32* %hp, align 4
  %le = icmp sle i32 %hp1, 0
  ret i1 %le
}

define i32 @square(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %n1 = load i32, i32* %n, align 4
  %n2 = load i32, i32* %n, align 4
  %mul = mul i32 %n1, %n2
  ret i32 %mul
}

define i1 @is_digit(i8 %0) {
entry:
  %c = alloca i8, align 1
  store i8 %0, i8* %c, align 1
  %c1 = load i8, i8* %c, align 1
  %ge = icmp sge i8 %c1, 48
  %c2 = load i8, i8* %c, align 1
  %le = icmp sle i8 %c2, 57
  %and = and i1 %ge, %le
  ret i1 %and
}

define i32 @main() {
entry:
  %d = alloca i32, align 4
  %call = call i32 @damage(i32 25, i32 8)
  store i32 %call, i32* %d, align 4
  %d1 = load i32, i32* %d, align 4
  %d2 = load i32, i32* %d, align 4
  %sub = sub i32 %d2, 60
  %call3 = call i1 @is_dead(i32 %sub)
  %bool_str = select i1 %call3, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str, i32 0, i32 0), i32 %d1, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @str.1, i32 0, i32 0), i8* %bool_str, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %call4 = call i32 @square(i32 4)
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.5, i32 0, i32 0), i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.3, i32 0, i32 0), i32 %call4, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## floor09b

```ura
// dungeon/floor09b.ura
main():
    hp  int     = 80
    ref r int = ref hp
    r   = 100
    output(hp, "\n")
```

```tree
fn main() : int
├─ = : int
│  ├─ hp : int
│  └─ int 80
├─ = : int
│  ├─ r : int
│  └─ ref : int
│     └─ hp : int
├─ = : int
│  ├─ r : int
│  └─ int 100
└─ output : void
   ├─ hp : int
   └─ chars "\n"
```

```out
100
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1

define i32 @main() {
entry:
  %hp = alloca i32, align 4
  store i32 80, i32* %hp, align 4
  %r = alloca i32*, align 8
  store i32* %hp, i32** %r, align 8
  %ref = load i32*, i32** %r, align 8
  store i32 100, i32* %ref, align 4
  %hp1 = load i32, i32* %hp, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 %hp1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## floor14a

```ura
// dungeon/floor14a.ura
proto fn strlen(s chars) int
proto fn write(fd int, ptr chars, len int) int
proto fn printf(fmt chars, ...) int

main():
    n int = strlen("dungeon")
    write(1, "hi\n", 3)
    printf("length: %d\n", n)
```

```tree
fn strlen(s : chars) : int

fn write(fd : int, ptr : chars, len : int) : int

fn printf(fmt : chars) : int

fn main() : int
├─ = : int
│  ├─ n : int
│  └─ call strlen : int
│     └─ chars "dungeon"
├─ call write : int
│  ├─ int 1
│  ├─ chars "hi\n"
│  └─ int 3
└─ call printf : int
   ├─ chars "length: %d\n"
   └─ n : int
```

```out
hi
length: 7
```

```err
```

```ll

@str = private unnamed_addr constant [8 x i8] c"dungeon\00", align 1
@str.1 = private unnamed_addr constant [4 x i8] c"hi\0A\00", align 1
@str.2 = private unnamed_addr constant [12 x i8] c"length: %d\0A\00", align 1

declare i32 @strlen(i8*)

declare i32 @write(i32, i8*, i32)

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %n = alloca i32, align 4
  %call = call i32 @strlen(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str, i32 0, i32 0))
  store i32 %call, i32* %n, align 4
  %call1 = call i32 @write(i32 1, i8* getelementptr inbounds ([4 x i8], [4 x i8]* @str.1, i32 0, i32 0), i32 3)
  %n2 = load i32, i32* %n, align 4
  %call3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.2, i32 0, i32 0), i32 %n2)
  ret i32 0
}
```
