# operators / comparison

## index

- 001 — all comparison ops on hero/enemy stats
- 002 — comparisons produce bools (with precedence)

## 001 — all comparison ops on hero/enemy stats

```ura
// operators/comparison/001.ura - all comparison ops on hero/enemy stats

fn enemy_atk() int:
    return 18

main():
    hero_hp  int = 100
    orc_hp   int = 60
    min_hp   int = 100

    output("hero_hp == min_hp: ", hero_hp == min_hp, "\n")   // True
    output("hero_hp != orc_hp: ", hero_hp != orc_hp, "\n")   // True
    output("orc_hp < hero_hp:  ", orc_hp < hero_hp,  "\n")   // True
    output("orc_hp > hero_hp:  ", orc_hp > hero_hp,  "\n")   // False
    output("orc_hp <= 60:      ", orc_hp <= 60,       "\n")   // True
    output("hero_hp >= 100:    ", hero_hp >= 100,      "\n")   // True

    // comparison with fn call
    if enemy_atk() > 15:
        output("strong enemy\n")
    else:
        output("weak enemy\n")

    // chained via if
    if hero_hp == 100 and orc_hp < hero_hp:
        output("hero is at full health and winning\n")
```

```tree
fn enemy_atk() : int
└─ return
   └─ int 18

fn main() : int
├─ = : int
│  ├─ hero_hp : int
│  └─ int 100
├─ = : int
│  ├─ orc_hp : int
│  └─ int 60
├─ = : int
│  ├─ min_hp : int
│  └─ int 100
├─ output : void
│  ├─ chars "hero_hp == min_hp: "
│  ├─ == : bool
│  │  ├─ hero_hp : int
│  │  └─ min_hp : int
│  └─ chars "\n"
├─ output : void
│  ├─ chars "hero_hp != orc_hp: "
│  ├─ != : bool
│  │  ├─ hero_hp : int
│  │  └─ orc_hp : int
│  └─ chars "\n"
├─ output : void
│  ├─ chars "orc_hp < hero_hp:  "
│  ├─ < : bool
│  │  ├─ orc_hp : int
│  │  └─ hero_hp : int
│  └─ chars "\n"
├─ output : void
│  ├─ chars "orc_hp > hero_hp:  "
│  ├─ > : bool
│  │  ├─ orc_hp : int
│  │  └─ hero_hp : int
│  └─ chars "\n"
├─ output : void
│  ├─ chars "orc_hp <= 60:      "
│  ├─ <= : bool
│  │  ├─ orc_hp : int
│  │  └─ int 60
│  └─ chars "\n"
├─ output : void
│  ├─ chars "hero_hp >= 100:    "
│  ├─ >= : bool
│  │  ├─ hero_hp : int
│  │  └─ int 100
│  └─ chars "\n"
├─ if
│  ├─ condition > : bool
│  │  ├─ call enemy_atk : int
│  │  └─ int 15
│  ├─ output : void
│  │  └─ chars "strong enemy\n"
│  └─ else
│     └─ output : void
│        └─ chars "weak enemy\n"
└─ if
   ├─ condition and : bool
   │  ├─ == : bool
   │  │  ├─ hero_hp : int
   │  │  └─ int 100
   │  └─ < : bool
   │     ├─ orc_hp : int
   │     └─ hero_hp : int
   └─ output : void
      └─ chars "hero is at full health and winning\n"
```

```out
hero_hp == min_hp: True
hero_hp != orc_hp: True
orc_hp < hero_hp:  True
orc_hp > hero_hp:  False
orc_hp <= 60:      True
hero_hp >= 100:    True
strong enemy
hero is at full health and winning
```

```err
```

```ll

@str = private unnamed_addr constant [20 x i8] c"hero_hp == min_hp: \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1
@str.2 = private unnamed_addr constant [20 x i8] c"hero_hp != orc_hp: \00", align 1
@true_str.3 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.4 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1
@str.7 = private unnamed_addr constant [20 x i8] c"orc_hp < hero_hp:  \00", align 1
@true_str.8 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.9 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.10 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.11 = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1
@str.12 = private unnamed_addr constant [20 x i8] c"orc_hp > hero_hp:  \00", align 1
@true_str.13 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.14 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.15 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.16 = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1
@str.17 = private unnamed_addr constant [20 x i8] c"orc_hp <= 60:      \00", align 1
@true_str.18 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.19 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.20 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.21 = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1
@str.22 = private unnamed_addr constant [20 x i8] c"hero_hp >= 100:    \00", align 1
@true_str.23 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.24 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.25 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.26 = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1
@str.27 = private unnamed_addr constant [14 x i8] c"strong enemy\0A\00", align 1
@fmt.28 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.29 = private unnamed_addr constant [12 x i8] c"weak enemy\0A\00", align 1
@fmt.30 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.31 = private unnamed_addr constant [36 x i8] c"hero is at full health and winning\0A\00", align 1
@fmt.32 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i32 @enemy_atk() {
entry:
  ret i32 18
}

define i32 @main() {
entry:
  %hero_hp = alloca i32, align 4
  store i32 100, i32* %hero_hp, align 4
  %orc_hp = alloca i32, align 4
  store i32 60, i32* %orc_hp, align 4
  %min_hp = alloca i32, align 4
  store i32 100, i32* %min_hp, align 4
  %hero_hp1 = load i32, i32* %hero_hp, align 4
  %min_hp2 = load i32, i32* %min_hp, align 4
  %eq = icmp eq i32 %hero_hp1, %min_hp2
  %bool_str = select i1 %eq, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([20 x i8], [20 x i8]* @str, i32 0, i32 0), i8* %bool_str, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %hero_hp3 = load i32, i32* %hero_hp, align 4
  %orc_hp4 = load i32, i32* %orc_hp, align 4
  %ne = icmp ne i32 %hero_hp3, %orc_hp4
  %bool_str5 = select i1 %ne, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.3, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.4, i32 0, i32 0)
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.6, i32 0, i32 0), i8* getelementptr inbounds ([20 x i8], [20 x i8]* @str.2, i32 0, i32 0), i8* %bool_str5, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  %orc_hp6 = load i32, i32* %orc_hp, align 4
  %hero_hp7 = load i32, i32* %hero_hp, align 4
  %lt = icmp slt i32 %orc_hp6, %hero_hp7
  %bool_str8 = select i1 %lt, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.8, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.9, i32 0, i32 0)
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.11, i32 0, i32 0), i8* getelementptr inbounds ([20 x i8], [20 x i8]* @str.7, i32 0, i32 0), i8* %bool_str8, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.10, i32 0, i32 0))
  %orc_hp9 = load i32, i32* %orc_hp, align 4
  %hero_hp10 = load i32, i32* %hero_hp, align 4
  %gt = icmp sgt i32 %orc_hp9, %hero_hp10
  %bool_str11 = select i1 %gt, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.13, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.14, i32 0, i32 0)
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.16, i32 0, i32 0), i8* getelementptr inbounds ([20 x i8], [20 x i8]* @str.12, i32 0, i32 0), i8* %bool_str11, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.15, i32 0, i32 0))
  %orc_hp12 = load i32, i32* %orc_hp, align 4
  %le = icmp sle i32 %orc_hp12, 60
  %bool_str13 = select i1 %le, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.18, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.19, i32 0, i32 0)
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.21, i32 0, i32 0), i8* getelementptr inbounds ([20 x i8], [20 x i8]* @str.17, i32 0, i32 0), i8* %bool_str13, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.20, i32 0, i32 0))
  %hero_hp14 = load i32, i32* %hero_hp, align 4
  %ge = icmp sge i32 %hero_hp14, 100
  %bool_str15 = select i1 %ge, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.23, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.24, i32 0, i32 0)
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.26, i32 0, i32 0), i8* getelementptr inbounds ([20 x i8], [20 x i8]* @str.22, i32 0, i32 0), i8* %bool_str15, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.25, i32 0, i32 0))
  %call = call i32 @enemy_atk()
  %gt16 = icmp sgt i32 %call, 15
  br i1 %gt16, label %then, label %next

endif:                                            ; preds = %next, %then
  %hero_hp19 = load i32, i32* %hero_hp, align 4
  %eq20 = icmp eq i32 %hero_hp19, 100
  %orc_hp21 = load i32, i32* %orc_hp, align 4
  %hero_hp22 = load i32, i32* %hero_hp, align 4
  %lt23 = icmp slt i32 %orc_hp21, %hero_hp22
  %and = and i1 %eq20, %lt23
  br i1 %and, label %then18, label %endif17

then:                                             ; preds = %entry
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.28, i32 0, i32 0), i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.27, i32 0, i32 0))
  br label %endif

next:                                             ; preds = %entry
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.30, i32 0, i32 0), i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.29, i32 0, i32 0))
  br label %endif

endif17:                                          ; preds = %then18, %endif
  ret i32 0

then18:                                           ; preds = %endif
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.32, i32 0, i32 0), i8* getelementptr inbounds ([36 x i8], [36 x i8]* @str.31, i32 0, i32 0))
  br label %endif17
}

declare i32 @printf(i8*, ...)
```

## 002 — comparisons produce bools (with precedence)

```ura
// operators/comparison/002.ura - comparisons produce bools (with precedence)

main():
    a int = 5
    b int = 9
    t bool = a < b
    u bool = a == 5
    v bool = b != a
    w bool = b >= 9
    x bool = a <= 4
    y bool = a > b
    z bool = a is 5
    m bool = a + 1 < b * 2
    return 3
```

```tree
fn main() : int
├─ = : int
│  ├─ a : int
│  └─ int 5
├─ = : int
│  ├─ b : int
│  └─ int 9
├─ = : bool
│  ├─ t : bool
│  └─ < : bool
│     ├─ a : int
│     └─ b : int
├─ = : bool
│  ├─ u : bool
│  └─ == : bool
│     ├─ a : int
│     └─ int 5
├─ = : bool
│  ├─ v : bool
│  └─ != : bool
│     ├─ b : int
│     └─ a : int
├─ = : bool
│  ├─ w : bool
│  └─ >= : bool
│     ├─ b : int
│     └─ int 9
├─ = : bool
│  ├─ x : bool
│  └─ <= : bool
│     ├─ a : int
│     └─ int 4
├─ = : bool
│  ├─ y : bool
│  └─ > : bool
│     ├─ a : int
│     └─ b : int
├─ = : bool
│  ├─ z : bool
│  └─ == : bool
│     ├─ a : int
│     └─ int 5
├─ = : bool
│  ├─ m : bool
│  └─ < : bool
│     ├─ + : int
│     │  ├─ a : int
│     │  └─ int 1
│     └─ * : int
│        ├─ b : int
│        └─ int 2
└─ return
   └─ int 3
```

```out
```

```err
exit: 3
```

```ll

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 5, i32* %a, align 4
  %b = alloca i32, align 4
  store i32 9, i32* %b, align 4
  %t = alloca i1, align 1
  %a1 = load i32, i32* %a, align 4
  %b2 = load i32, i32* %b, align 4
  %lt = icmp slt i32 %a1, %b2
  store i1 %lt, i1* %t, align 1
  %u = alloca i1, align 1
  %a3 = load i32, i32* %a, align 4
  %eq = icmp eq i32 %a3, 5
  store i1 %eq, i1* %u, align 1
  %v = alloca i1, align 1
  %b4 = load i32, i32* %b, align 4
  %a5 = load i32, i32* %a, align 4
  %ne = icmp ne i32 %b4, %a5
  store i1 %ne, i1* %v, align 1
  %w = alloca i1, align 1
  %b6 = load i32, i32* %b, align 4
  %ge = icmp sge i32 %b6, 9
  store i1 %ge, i1* %w, align 1
  %x = alloca i1, align 1
  %a7 = load i32, i32* %a, align 4
  %le = icmp sle i32 %a7, 4
  store i1 %le, i1* %x, align 1
  %y = alloca i1, align 1
  %a8 = load i32, i32* %a, align 4
  %b9 = load i32, i32* %b, align 4
  %gt = icmp sgt i32 %a8, %b9
  store i1 %gt, i1* %y, align 1
  %z = alloca i1, align 1
  %a10 = load i32, i32* %a, align 4
  %eq11 = icmp eq i32 %a10, 5
  store i1 %eq11, i1* %z, align 1
  %m = alloca i1, align 1
  %a12 = load i32, i32* %a, align 4
  %add = add i32 %a12, 1
  %b13 = load i32, i32* %b, align 4
  %mul = mul i32 %b13, 2
  %lt14 = icmp slt i32 %add, %mul
  store i1 %lt14, i1* %m, align 1
  ret i32 3
}
```
