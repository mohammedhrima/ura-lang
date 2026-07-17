# functions / params / ref

## index

- 001 — single ref param: buff hero hp
- 002 — swap two weapon damage values via refs
- 003 — refs used in expression, return result
- 004 — compound ref assign through in a loop
- 005 — ref-to-value: ref passing a where value is expected
- 006 — ref parameter mutates the caller (counter)
- 007 — a void function with a ref parameter

## 001 — single ref param: buff hero hp

```ura
// functions/params/ref/001.ura - single ref param: buff hero hp

fn buff(ref hp int) void:
    hp = hp + 20

main():
    hero_hp int = 80
    buff(ref hero_hp)
    output("<", hero_hp, ">\n")
```

```tree
fn buff(hp : int) : void
└─ = : int
   ├─ hp : int
   └─ + : int
      ├─ hp : int
      └─ int 20

fn main() : int
├─ = : int
│  ├─ hero_hp : int
│  └─ int 80
├─ call buff : void
│  └─ ref : int
│     └─ hero_hp : int
└─ output : void
   ├─ chars "<"
   ├─ hero_hp : int
   └─ chars ">\n"
```

```out
<100>
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c"<\00", align 1
@str.1 = private unnamed_addr constant [3 x i8] c">\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define void @buff(i32* %0) {
entry:
  %hp = alloca i32*, align 8
  store i32* %0, i32** %hp, align 8
  %ref = load i32*, i32** %hp, align 8
  %ref1 = load i32*, i32** %hp, align 8
  %hp2 = load i32, i32* %ref1, align 4
  %add = add i32 %hp2, 20
  store i32 %add, i32* %ref, align 4
  ret void
}

define i32 @main() {
entry:
  %hero_hp = alloca i32, align 4
  store i32 80, i32* %hero_hp, align 4
  call void @buff(i32* %hero_hp)
  %hero_hp1 = load i32, i32* %hero_hp, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %hero_hp1, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 002 — swap two weapon damage values via refs

```ura
// functions/params/ref/002.ura - swap two weapon damage values via refs

fn swap_weapons(ref a int, ref b int) void:
    temp int = a
    a = b
    b = temp

main():
    sword  int = 30
    dagger int = 15
    swap_weapons(ref sword, ref dagger)
    output("<", sword, " ", dagger, ">\n")
```

```tree
fn swap_weapons(a : int, b : int) : void
├─ = : int
│  ├─ temp : int
│  └─ a : int
├─ = : int
│  ├─ a : int
│  └─ b : int
└─ = : int
   ├─ b : int
   └─ temp : int

fn main() : int
├─ = : int
│  ├─ sword : int
│  └─ int 30
├─ = : int
│  ├─ dagger : int
│  └─ int 15
├─ call swap_weapons : void
│  ├─ ref : int
│  │  └─ sword : int
│  └─ ref : int
│     └─ dagger : int
└─ output : void
   ├─ chars "<"
   ├─ sword : int
   ├─ chars " "
   ├─ dagger : int
   └─ chars ">\n"
```

```out
<15 30>
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c"<\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.2 = private unnamed_addr constant [3 x i8] c">\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%s%d%s%d%s\00", align 1

define void @swap_weapons(i32* %0, i32* %1) {
entry:
  %a = alloca i32*, align 8
  store i32* %0, i32** %a, align 8
  %b = alloca i32*, align 8
  store i32* %1, i32** %b, align 8
  %temp = alloca i32, align 4
  %ref = load i32*, i32** %a, align 8
  %a1 = load i32, i32* %ref, align 4
  store i32 %a1, i32* %temp, align 4
  %ref2 = load i32*, i32** %a, align 8
  %ref3 = load i32*, i32** %b, align 8
  %b4 = load i32, i32* %ref3, align 4
  store i32 %b4, i32* %ref2, align 4
  %ref5 = load i32*, i32** %b, align 8
  %temp6 = load i32, i32* %temp, align 4
  store i32 %temp6, i32* %ref5, align 4
  ret void
}

define i32 @main() {
entry:
  %sword = alloca i32, align 4
  store i32 30, i32* %sword, align 4
  %dagger = alloca i32, align 4
  store i32 15, i32* %dagger, align 4
  call void @swap_weapons(i32* %sword, i32* %dagger)
  %sword1 = load i32, i32* %sword, align 4
  %dagger2 = load i32, i32* %dagger, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %sword1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i32 %dagger2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.2, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 003 — refs used in expression, return result

```ura
// functions/params/ref/003.ura - refs used in expression, return result

fn add_xp(ref a int, ref b int) int:
    result int = a + b
    return result

main():
    kill_xp  int = 30
    bonus_xp int = 20
    z        int
    z = add_xp(ref kill_xp, ref bonus_xp)
    output("<", z, ">\n")
```

```tree
fn add_xp(a : int, b : int) : int
├─ = : int
│  ├─ result : int
│  └─ + : int
│     ├─ a : int
│     └─ b : int
└─ return
   └─ result : int

fn main() : int
├─ = : int
│  ├─ kill_xp : int
│  └─ int 30
├─ = : int
│  ├─ bonus_xp : int
│  └─ int 20
├─ z : int
├─ = : int
│  ├─ z : int
│  └─ call add_xp : int
│     ├─ ref : int
│     │  └─ kill_xp : int
│     └─ ref : int
│        └─ bonus_xp : int
└─ output : void
   ├─ chars "<"
   ├─ z : int
   └─ chars ">\n"
```

```out
<50>
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c"<\00", align 1
@str.1 = private unnamed_addr constant [3 x i8] c">\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define i32 @add_xp(i32* %0, i32* %1) {
entry:
  %a = alloca i32*, align 8
  store i32* %0, i32** %a, align 8
  %b = alloca i32*, align 8
  store i32* %1, i32** %b, align 8
  %result = alloca i32, align 4
  %ref = load i32*, i32** %a, align 8
  %a1 = load i32, i32* %ref, align 4
  %ref2 = load i32*, i32** %b, align 8
  %b3 = load i32, i32* %ref2, align 4
  %add = add i32 %a1, %b3
  store i32 %add, i32* %result, align 4
  %result4 = load i32, i32* %result, align 4
  ret i32 %result4
}

define i32 @main() {
entry:
  %kill_xp = alloca i32, align 4
  store i32 30, i32* %kill_xp, align 4
  %bonus_xp = alloca i32, align 4
  store i32 20, i32* %bonus_xp, align 4
  %z = alloca i32, align 4
  store i32 0, i32* %z, align 4
  %call = call i32 @add_xp(i32* %kill_xp, i32* %bonus_xp)
  store i32 %call, i32* %z, align 4
  %z1 = load i32, i32* %z, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %z1, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 004 — compound ref assign through in a loop

```ura
// functions/params/ref/004.ura - compound ref assign through in a loop

fn tick(ref kills int) void:
    kills += 1

main():
    kills int = 0
    i     int = 0
    while i < 5:
        tick(ref kills)
        i = i + 1
    output("<", kills, ">\n")
```

```tree
fn tick(kills : int) : void
└─ += : int
   ├─ kills : int
   └─ int 1

fn main() : int
├─ = : int
│  ├─ kills : int
│  └─ int 0
├─ = : int
│  ├─ i : int
│  └─ int 0
├─ while
│  ├─ condition < : bool
│  │  ├─ i : int
│  │  └─ int 5
│  ├─ call tick : void
│  │  └─ ref : int
│  │     └─ kills : int
│  └─ = : int
│     ├─ i : int
│     └─ + : int
│        ├─ i : int
│        └─ int 1
└─ output : void
   ├─ chars "<"
   ├─ kills : int
   └─ chars ">\n"
```

```out
<5>
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c"<\00", align 1
@str.1 = private unnamed_addr constant [3 x i8] c">\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define void @tick(i32* %0) {
entry:
  %kills = alloca i32*, align 8
  store i32* %0, i32** %kills, align 8
  %ref = load i32*, i32** %kills, align 8
  %cur = load i32, i32* %ref, align 4
  %add = add i32 %cur, 1
  store i32 %add, i32* %ref, align 4
  ret void
}

define i32 @main() {
entry:
  %kills = alloca i32, align 4
  store i32 0, i32* %kills, align 4
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %i1 = load i32, i32* %i, align 4
  %lt = icmp slt i32 %i1, 5
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  call void @tick(i32* %kills)
  %i2 = load i32, i32* %i, align 4
  %add = add i32 %i2, 1
  store i32 %add, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %kills3 = load i32, i32* %kills, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %kills3, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 005 — ref-to-value: ref passing a where value is expected

```ura
// functions/params/ref/005.ura - ref-to-value: ref passing a where value is expected

fn show_hp(hp int) void:
    output("HP: ", hp, "\n")

main():
    hero_hp int = 75
    ref r int = ref hero_hp
    show_hp(r)
```

```tree
fn show_hp(hp : int) : void
└─ output : void
   ├─ chars "HP: "
   ├─ hp : int
   └─ chars "\n"

fn main() : int
├─ = : int
│  ├─ hero_hp : int
│  └─ int 75
├─ = : int
│  ├─ r : int
│  └─ ref : int
│     └─ hero_hp : int
└─ call show_hp : void
   └─ r : int
```

```out
HP: 75
```

```err
```

```ll

@str = private unnamed_addr constant [5 x i8] c"HP: \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define void @show_hp(i32 %0) {
entry:
  %hp = alloca i32, align 4
  store i32 %0, i32* %hp, align 4
  %hp1 = load i32, i32* %hp, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i32 %hp1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %hero_hp = alloca i32, align 4
  store i32 75, i32* %hero_hp, align 4
  %r = alloca i32*, align 8
  store i32* %hero_hp, i32** %r, align 8
  %ref = load i32*, i32** %r, align 8
  %r1 = load i32, i32* %ref, align 4
  call void @show_hp(i32 %r1)
  ret i32 0
}
```

## 006 — ref parameter mutates the caller (counter)

```ura
// functions/params/ref/006.ura - ref parameter mutates the caller (counter)

fn tick(ref n int) int:
    n = n + 1
    return n
main():
    n int = 0
    tick(ref n)
    tick(ref n)
    return n
```

```tree
fn tick(n : int) : int
├─ = : int
│  ├─ n : int
│  └─ + : int
│     ├─ n : int
│     └─ int 1
└─ return
   └─ n : int

fn main() : int
├─ = : int
│  ├─ n : int
│  └─ int 0
├─ call tick : int
│  └─ ref : int
│     └─ n : int
├─ call tick : int
│  └─ ref : int
│     └─ n : int
└─ return
   └─ n : int
```

```out
```

```err
exit: 2
```

```ll

define i32 @tick(i32* %0) {
entry:
  %n = alloca i32*, align 8
  store i32* %0, i32** %n, align 8
  %ref = load i32*, i32** %n, align 8
  %ref1 = load i32*, i32** %n, align 8
  %n2 = load i32, i32* %ref1, align 4
  %add = add i32 %n2, 1
  store i32 %add, i32* %ref, align 4
  %ref3 = load i32*, i32** %n, align 8
  %n4 = load i32, i32* %ref3, align 4
  ret i32 %n4
}

define i32 @main() {
entry:
  %n = alloca i32, align 4
  store i32 0, i32* %n, align 4
  %call = call i32 @tick(i32* %n)
  %call1 = call i32 @tick(i32* %n)
  %n2 = load i32, i32* %n, align 4
  ret i32 %n2
}
```

## 007 — a void function with a ref parameter

```ura
// functions/params/ref/007.ura - a void function with a ref parameter

fn bump(ref n int) void:
    n = n + 1
main():
    n int = 5
    bump(ref n)
    return n
```

```tree
fn bump(n : int) : void
└─ = : int
   ├─ n : int
   └─ + : int
      ├─ n : int
      └─ int 1

fn main() : int
├─ = : int
│  ├─ n : int
│  └─ int 5
├─ call bump : void
│  └─ ref : int
│     └─ n : int
└─ return
   └─ n : int
```

```out
```

```err
exit: 6
```

```ll

define void @bump(i32* %0) {
entry:
  %n = alloca i32*, align 8
  store i32* %0, i32** %n, align 8
  %ref = load i32*, i32** %n, align 8
  %ref1 = load i32*, i32** %n, align 8
  %n2 = load i32, i32* %ref1, align 4
  %add = add i32 %n2, 1
  store i32 %add, i32* %ref, align 4
  ret void
}

define i32 @main() {
entry:
  %n = alloca i32, align 4
  store i32 5, i32* %n, align 4
  call void @bump(i32* %n)
  %n1 = load i32, i32* %n, align 4
  ret i32 %n1
}
```
