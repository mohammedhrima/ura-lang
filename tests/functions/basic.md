# functions / basic

## index

- 001 — void fn: announce dungeon floor
- 002 — fn with return value + single-line fns
- 003 — multi-fn: clamp, damage, is_dead
- 004 — fn-typed value: assign top-level fn to a local, call indirectly
- 005 — fn-typed parameter: callback passed to higher-order fn
- 006 — return an integer literal
- 007 — declare a variable and return it
- 008 — function parameters and a direct call
- 009 — forward reference: call a function declared later
- 010 — store a function in a fn-typed variable, then call it
- 011 — pass a function as an argument (higher-order)
- 013 — nested non-capturing helper function
- 014 — a call used as an argument to another call

## 001 — void fn: announce dungeon floor

```ura
// functions/basic/001.ura - void fn: announce dungeon floor

fn announce_floor(floor int) void:
    output("=== Entering floor ", floor, " ===\n")

fn announce_boss(name chars) void:
    output("BOSS: ", name, " appears!\n")

main():
    announce_floor(1)
    announce_floor(5)
    announce_boss("The Lich King")
```

```out
=== Entering floor 1 ===
=== Entering floor 5 ===
BOSS: The Lich King appears!
```

```err
```

```ll

@str = private unnamed_addr constant [20 x i8] c"=== Entering floor \00", align 1
@str.1 = private unnamed_addr constant [6 x i8] c" ===\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.2 = private unnamed_addr constant [7 x i8] c"BOSS: \00", align 1
@str.3 = private unnamed_addr constant [11 x i8] c" appears!\0A\00", align 1
@fmt.4 = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1
@str.5 = private unnamed_addr constant [14 x i8] c"The Lich King\00", align 1

define void @announce_floor(i32 %0) {
entry:
  %floor = alloca i32, align 4
  store i32 %0, i32* %floor, align 4
  %floor1 = load i32, i32* %floor, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([20 x i8], [20 x i8]* @str, i32 0, i32 0), i32 %floor1, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.1, i32 0, i32 0))
  ret void
}

declare i32 @printf(i8*, ...)

define void @announce_boss(i8* %0) {
entry:
  %name = alloca i8*, align 8
  store i8* %0, i8** %name, align 8
  %name1 = load i8*, i8** %name, align 8
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.4, i32 0, i32 0), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.2, i32 0, i32 0), i8* %name1, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.3, i32 0, i32 0))
  ret void
}

define i32 @main() {
entry:
  call void @announce_floor(i32 1)
  call void @announce_floor(i32 5)
  call void @announce_boss(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.5, i32 0, i32 0))
  ret i32 0
}
```

## 002 — fn with return value + single-line fns

```ura
// functions/basic/002.ura - fn with return value + single-line fns

fn victory_bonus(floor int) int:
    return floor * 50

fn is_alive(hp int) bool: return hp > 0
fn is_boss_floor(floor int) bool: return floor == 10

main():
    bonus int = victory_bonus(3)
    output("Floor 3 bonus: ", bonus, "\n")
    output("Hero alive:    ", is_alive(80), "\n")
    output("Boss floor:    ", is_boss_floor(10), "\n")
```

```out
Floor 3 bonus: 150
Hero alive:    True
Boss floor:    True
```

```err
```

```ll

@str = private unnamed_addr constant [16 x i8] c"Floor 3 bonus: \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.2 = private unnamed_addr constant [16 x i8] c"Hero alive:    \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1
@str.5 = private unnamed_addr constant [16 x i8] c"Boss floor:    \00", align 1
@true_str.6 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.7 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.8 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.9 = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1

define i32 @victory_bonus(i32 %0) {
entry:
  %floor = alloca i32, align 4
  store i32 %0, i32* %floor, align 4
  %floor1 = load i32, i32* %floor, align 4
  %mul = mul i32 %floor1, 50
  ret i32 %mul
}

define i1 @is_alive(i32 %0) {
entry:
  %hp = alloca i32, align 4
  store i32 %0, i32* %hp, align 4
  %hp1 = load i32, i32* %hp, align 4
  %gt = icmp sgt i32 %hp1, 0
  ret i1 %gt
}

define i1 @is_boss_floor(i32 %0) {
entry:
  %floor = alloca i32, align 4
  store i32 %0, i32* %floor, align 4
  %floor1 = load i32, i32* %floor, align 4
  %eq = icmp eq i32 %floor1, 10
  ret i1 %eq
}

define i32 @main() {
entry:
  %bonus = alloca i32, align 4
  %call = call i32 @victory_bonus(i32 3)
  store i32 %call, i32* %bonus, align 4
  %bonus1 = load i32, i32* %bonus, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str, i32 0, i32 0), i32 %bonus1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %call2 = call i1 @is_alive(i32 80)
  %bool_str = select i1 %call2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.4, i32 0, i32 0), i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.2, i32 0, i32 0), i8* %bool_str, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %call3 = call i1 @is_boss_floor(i32 10)
  %bool_str4 = select i1 %call3, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.6, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.7, i32 0, i32 0)
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.9, i32 0, i32 0), i8* getelementptr inbounds ([16 x i8], [16 x i8]* @str.5, i32 0, i32 0), i8* %bool_str4, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.8, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 003 — multi-fn: clamp, damage, is_dead

```ura
// functions/basic/003.ura - multi-fn: clamp, damage, is_dead

fn clamp(val int, lo int, hi int) int:
    if val < lo:
        return lo
    if val > hi:
        return hi
    return val

fn damage(atk int, def int) int:
    return clamp(atk - def, 0, 999)

fn is_dead(hp int) bool:
    return hp <= 0

main():
    d int = damage(25, 8)
    output("Orc takes ", d, " damage\n")
    output("Orc dead: ", is_dead(d - 60), "\n")
```

```out
Orc takes 17 damage
Orc dead: True
```

```err
```

```ll

@str = private unnamed_addr constant [11 x i8] c"Orc takes \00", align 1
@str.1 = private unnamed_addr constant [9 x i8] c" damage\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.2 = private unnamed_addr constant [11 x i8] c"Orc dead: \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1

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

define i32 @main() {
entry:
  %d = alloca i32, align 4
  %call = call i32 @damage(i32 25, i32 8)
  store i32 %call, i32* %d, align 4
  %d1 = load i32, i32* %d, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str, i32 0, i32 0), i32 %d1, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0))
  %d2 = load i32, i32* %d, align 4
  %sub = sub i32 %d2, 60
  %call3 = call i1 @is_dead(i32 %sub)
  %bool_str = select i1 %call3, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.4, i32 0, i32 0), i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.2, i32 0, i32 0), i8* %bool_str, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 004 — fn-typed value: assign top-level fn to a local, call indirectly

```ura
// functions/basic/004.ura - fn-typed value: assign top-level fn to a local, call indirectly

fn add(a int, b int) int:
    return a + b

fn sub(a int, b int) int:
    return a - b

main():
    op fn(int, int) int = add
    output(op(2, 3), "\n")

    op = sub
    output(op(10, 4), "\n")
```

```out
5
6
```

```err
```

```ll

@trap_msg = private unnamed_addr constant [180 x i8] c"runtime error: Call to a null function value\0A   004.ura:11:12\0A   |\0A11 |     output(op(2, 3), \22\\n\22)\0A   |            ^^\0A\00", align 1
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@trap_msg.1 = private unnamed_addr constant [181 x i8] c"runtime error: Call to a null function value\0A   004.ura:14:12\0A   |\0A14 |     output(op(10, 4), \22\\n\22)\0A   |            ^^\0A\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.3 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1

define i32 @add(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  %b = alloca i32, align 4
  store i32 %1, i32* %b, align 4
  %a1 = load i32, i32* %a, align 4
  %b2 = load i32, i32* %b, align 4
  %add = add i32 %a1, %b2
  ret i32 %add
}

define i32 @sub(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  %b = alloca i32, align 4
  store i32 %1, i32* %b, align 4
  %a1 = load i32, i32* %a, align 4
  %b2 = load i32, i32* %b, align 4
  %sub = sub i32 %a1, %b2
  ret i32 %sub
}

define i32 @main() {
entry:
  %op = alloca i32 (i32, i32)*, align 8
  store i32 (i32, i32)* @add, i32 (i32, i32)** %op, align 8
  %fn = load i32 (i32, i32)*, i32 (i32, i32)** %op, align 8
  %isnull = icmp eq i32 (i32, i32)* %fn, null
  br i1 %isnull, label %trap, label %cont

trap:                                             ; preds = %entry
  %0 = call i64 @write(i32 2, i8* getelementptr inbounds ([180 x i8], [180 x i8]* @trap_msg, i32 0, i32 0), i64 179)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %call = call i32 %fn(i32 2, i32 3)
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 %call, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  store i32 (i32, i32)* @sub, i32 (i32, i32)** %op, align 8
  %fn1 = load i32 (i32, i32)*, i32 (i32, i32)** %op, align 8
  %isnull2 = icmp eq i32 (i32, i32)* %fn1, null
  br i1 %isnull2, label %trap3, label %cont4

trap3:                                            ; preds = %cont
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([181 x i8], [181 x i8]* @trap_msg.1, i32 0, i32 0), i64 180)
  call void @exit(i32 1)
  unreachable

cont4:                                            ; preds = %cont
  %call5 = call i32 %fn1(i32 10, i32 4)
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.3, i32 0, i32 0), i32 %call5, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  ret i32 0
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)

declare i32 @printf(i8*, ...)
```

## 005 — fn-typed parameter: callback passed to higher-order fn

```ura
// functions/basic/005.ura - fn-typed parameter: callback passed to higher-order fn

fn double_it(n int) int:
    return n * 2

fn triple_it(n int) int:
    return n * 3

fn apply(cb fn(int) int, x int) int:
    return cb(x)

main():
    output(apply(double_it, 21), "\n")
    output(apply(triple_it, 7), "\n")
```

```out
42
21
```

```err
```

```ll

@trap_msg = private unnamed_addr constant [170 x i8] c"runtime error: Call to a null function value\0A   005.ura:10:12\0A   |\0A10 |     return cb(x)\0A   |            ^^\0A\00", align 1
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.2 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1

define i32 @double_it(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %n1 = load i32, i32* %n, align 4
  %mul = mul i32 %n1, 2
  ret i32 %mul
}

define i32 @triple_it(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %n1 = load i32, i32* %n, align 4
  %mul = mul i32 %n1, 3
  ret i32 %mul
}

define i32 @apply(i32 (i32)* %0, i32 %1) {
entry:
  %cb = alloca i32 (i32)*, align 8
  store i32 (i32)* %0, i32 (i32)** %cb, align 8
  %x = alloca i32, align 4
  store i32 %1, i32* %x, align 4
  %x1 = load i32, i32* %x, align 4
  %fn = load i32 (i32)*, i32 (i32)** %cb, align 8
  %isnull = icmp eq i32 (i32)* %fn, null
  br i1 %isnull, label %trap, label %cont

trap:                                             ; preds = %entry
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([170 x i8], [170 x i8]* @trap_msg, i32 0, i32 0), i64 169)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %call = call i32 %fn(i32 %x1)
  ret i32 %call
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)

define i32 @main() {
entry:
  %call = call i32 @apply(i32 (i32)* @double_it, i32 21)
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 %call, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  %call1 = call i32 @apply(i32 (i32)* @triple_it, i32 7)
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.2, i32 0, i32 0), i32 %call1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 006 — return an integer literal

```ura
// functions/basic/006.ura - return an integer literal

main():
    return 42
```

```out
```

```err
exit: 42
```

```ll

define i32 @main() {
entry:
  ret i32 42
}
```

## 007 — declare a variable and return it

```ura
// functions/basic/007.ura - declare a variable and return it

main():
    a int = 7
    return a
```

```out
```

```err
exit: 7
```

```ll

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 7, i32* %a, align 4
  %a1 = load i32, i32* %a, align 4
  ret i32 %a1
}
```

## 008 — function parameters and a direct call

```ura
// functions/basic/008.ura - function parameters and a direct call

fn add(a int, b int) int:
    return a + b
main():
    return add(40, 2)
```

```out
```

```err
exit: 42
```

```ll

define i32 @add(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  %b = alloca i32, align 4
  store i32 %1, i32* %b, align 4
  %a1 = load i32, i32* %a, align 4
  %b2 = load i32, i32* %b, align 4
  %add = add i32 %a1, %b2
  ret i32 %add
}

define i32 @main() {
entry:
  %call = call i32 @add(i32 40, i32 2)
  ret i32 %call
}
```

## 009 — forward reference: call a function declared later

```ura
// functions/basic/009.ura - forward reference: call a function declared later

main():
    return dbl(21)
fn dbl(n int) int:
    return n + n
```

```out
```

```err
exit: 42
```

```ll

define i32 @main() {
entry:
  %call = call i32 @dbl(i32 21)
  ret i32 %call
}

define i32 @dbl(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %n1 = load i32, i32* %n, align 4
  %n2 = load i32, i32* %n, align 4
  %add = add i32 %n1, %n2
  ret i32 %add
}
```

## 010 — store a function in a fn-typed variable, then call it

```ura
// functions/basic/010.ura - store a function in a fn-typed variable, then call it

fn twice(n int) int:
    return n + n
main():
    f fn(int) int = twice
    return f(21)
```

```out
```

```err
exit: 42
```

```ll

@trap_msg = private unnamed_addr constant [164 x i8] c"runtime error: Call to a null function value\0A  010.ura:7:12\0A  |\0A7 |     return f(21)\0A  |            ^\0A\00", align 1

define i32 @twice(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %n1 = load i32, i32* %n, align 4
  %n2 = load i32, i32* %n, align 4
  %add = add i32 %n1, %n2
  ret i32 %add
}

define i32 @main() {
entry:
  %f = alloca i32 (i32)*, align 8
  store i32 (i32)* @twice, i32 (i32)** %f, align 8
  %fn = load i32 (i32)*, i32 (i32)** %f, align 8
  %isnull = icmp eq i32 (i32)* %fn, null
  br i1 %isnull, label %trap, label %cont

trap:                                             ; preds = %entry
  %0 = call i64 @write(i32 2, i8* getelementptr inbounds ([164 x i8], [164 x i8]* @trap_msg, i32 0, i32 0), i64 163)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %call = call i32 %fn(i32 21)
  ret i32 %call
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)
```

## 011 — pass a function as an argument (higher-order)

```ura
// functions/basic/011.ura - pass a function as an argument (higher-order)

fn twice(n int) int:
    return n + n
fn apply(f fn(int) int, x int) int:
    return f(x)
main():
    return apply(twice, 21)
```

```out
```

```err
exit: 42
```

```ll

@trap_msg = private unnamed_addr constant [163 x i8] c"runtime error: Call to a null function value\0A  011.ura:6:12\0A  |\0A6 |     return f(x)\0A  |            ^\0A\00", align 1

define i32 @twice(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %n1 = load i32, i32* %n, align 4
  %n2 = load i32, i32* %n, align 4
  %add = add i32 %n1, %n2
  ret i32 %add
}

define i32 @apply(i32 (i32)* %0, i32 %1) {
entry:
  %f = alloca i32 (i32)*, align 8
  store i32 (i32)* %0, i32 (i32)** %f, align 8
  %x = alloca i32, align 4
  store i32 %1, i32* %x, align 4
  %x1 = load i32, i32* %x, align 4
  %fn = load i32 (i32)*, i32 (i32)** %f, align 8
  %isnull = icmp eq i32 (i32)* %fn, null
  br i1 %isnull, label %trap, label %cont

trap:                                             ; preds = %entry
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([163 x i8], [163 x i8]* @trap_msg, i32 0, i32 0), i64 162)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %call = call i32 %fn(i32 %x1)
  ret i32 %call
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)

define i32 @main() {
entry:
  %call = call i32 @apply(i32 (i32)* @twice, i32 21)
  ret i32 %call
}
```

## 013 — nested non-capturing helper function

```ura
// functions/basic/013.ura - nested non-capturing helper function

main():
    fn dbl(k int) int:
        return k + k
    return dbl(21)
```

```out
```

```err
exit: 42
```

```ll

define i32 @main() {
entry:
  %call = call i32 @dbl(i32 21)
  ret i32 %call
}

define i32 @dbl(i32 %0) {
entry:
  %k = alloca i32, align 4
  store i32 %0, i32* %k, align 4
  %k1 = load i32, i32* %k, align 4
  %k2 = load i32, i32* %k, align 4
  %add = add i32 %k1, %k2
  ret i32 %add
}
```

## 014 — a call used as an argument to another call

```ura
// functions/basic/014.ura - a call used as an argument to another call

fn square(n int) int:
    return n * n
fn add(a int, b int) int:
    return a + b
main():
    return add(square(5), 3)
```

```out
```

```err
exit: 28
```

```ll

define i32 @square(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %n1 = load i32, i32* %n, align 4
  %n2 = load i32, i32* %n, align 4
  %mul = mul i32 %n1, %n2
  ret i32 %mul
}

define i32 @add(i32 %0, i32 %1) {
entry:
  %a = alloca i32, align 4
  store i32 %0, i32* %a, align 4
  %b = alloca i32, align 4
  store i32 %1, i32* %b, align 4
  %a1 = load i32, i32* %a, align 4
  %b2 = load i32, i32* %b, align 4
  %add = add i32 %a1, %b2
  ret i32 %add
}

define i32 @main() {
entry:
  %call = call i32 @square(i32 5)
  %call1 = call i32 @add(i32 %call, i32 3)
  ret i32 %call1
}
```
