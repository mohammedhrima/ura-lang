# functions

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
- 012 — nested non-capturing helper function
- 013 — a call used as an argument to another call
- 014 — value parameters: take_damage
- 015 — a by-value parameter does not change the caller
- 016 — single ref param: buff hero hp
- 017 — swap two weapon damage values via refs
- 018 — refs used in expression, return result
- 019 — compound ref assign through in a loop
- 020 — ref-to-value: ref passing a where value is expected
- 021 — ref parameter mutates the caller (counter)
- 022 — a void function with a ref parameter

## 001 — void fn: announce dungeon floor

```ura
// functions/001.ura - void fn: announce dungeon floor

fn announce_floor(floor int) void:
    output("=== Entering floor ", floor, " ===\n")

fn announce_boss(name chars) void:
    output("BOSS: ", name, " appears!\n")

main():
    announce_floor(1)
    announce_floor(5)
    announce_boss("The Lich King")
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn announce_floor(floor : i32) : void
└─ output : void
   ├─ chars "=== Entering floor "
   ├─ floor : i32
   └─ chars " ===\n"

fn announce_boss(name : chars) : void
└─ output : void
   ├─ chars "BOSS: "
   ├─ name : chars
   └─ chars " appears!\n"

fn main() : i32
├─ call announce_floor : void
│  └─ int 1
├─ call announce_floor : void
│  └─ int 5
└─ call announce_boss : void
   └─ chars "The Lich King"
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
// functions/002.ura - fn with return value + single-line fns

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

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn victory_bonus(floor : i32) : i32
└─ return
   └─ * : i32
      ├─ floor : i32
      └─ int 50

fn is_alive(hp : i32) : bool
└─ return
   └─ > : bool
      ├─ hp : i32
      └─ int 0

fn is_boss_floor(floor : i32) : bool
└─ return
   └─ == : bool
      ├─ floor : i32
      └─ int 10

fn main() : i32
├─ = : i32
│  ├─ bonus : i32
│  └─ call victory_bonus : i32
│     └─ int 3
├─ output : void
│  ├─ chars "Floor 3 bonus: "
│  ├─ bonus : i32
│  └─ chars "\n"
├─ output : void
│  ├─ chars "Hero alive:    "
│  ├─ call is_alive : bool
│  │  └─ int 80
│  └─ chars "\n"
└─ output : void
   ├─ chars "Boss floor:    "
   ├─ call is_boss_floor : bool
   │  └─ int 10
   └─ chars "\n"
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
// functions/003.ura - multi-fn: clamp, damage, is_dead

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

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn clamp(val : i32, lo : i32, hi : i32) : i32
├─ if
│  ├─ condition < : bool
│  │  ├─ val : i32
│  │  └─ lo : i32
│  └─ return
│     └─ lo : i32
├─ if
│  ├─ condition > : bool
│  │  ├─ val : i32
│  │  └─ hi : i32
│  └─ return
│     └─ hi : i32
└─ return
   └─ val : i32

fn damage(atk : i32, def : i32) : i32
└─ return
   └─ call clamp : i32
      ├─ - : i32
      │  ├─ atk : i32
      │  └─ def : i32
      ├─ int 0
      └─ int 999

fn is_dead(hp : i32) : bool
└─ return
   └─ <= : bool
      ├─ hp : i32
      └─ int 0

fn main() : i32
├─ = : i32
│  ├─ d : i32
│  └─ call damage : i32
│     ├─ int 25
│     └─ int 8
├─ output : void
│  ├─ chars "Orc takes "
│  ├─ d : i32
│  └─ chars " damage\n"
└─ output : void
   ├─ chars "Orc dead: "
   ├─ call is_dead : bool
   │  └─ - : i32
   │     ├─ d : i32
   │     └─ int 60
   └─ chars "\n"
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
// functions/004.ura - fn-typed value: assign top-level fn to a local, call indirectly

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

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn add(a : i32, b : i32) : i32
└─ return
   └─ + : i32
      ├─ a : i32
      └─ b : i32

fn sub(a : i32, b : i32) : i32
└─ return
   └─ - : i32
      ├─ a : i32
      └─ b : i32

fn main() : i32
├─ = : FN_TYPE
│  ├─ op : FN_TYPE
│  └─ FN_TYPE : FN_TYPE
├─ output : void
│  ├─ call op : i32
│  │  ├─ int 2
│  │  └─ int 3
│  └─ chars "\n"
├─ = : FN_TYPE
│  ├─ op : FN_TYPE
│  └─ FN_TYPE : FN_TYPE
└─ output : void
   ├─ call op : i32
   │  ├─ int 10
   │  └─ int 4
   └─ chars "\n"
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
// functions/005.ura - fn-typed parameter: callback passed to higher-order fn

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

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn double_it(n : i32) : i32
└─ return
   └─ * : i32
      ├─ n : i32
      └─ int 2

fn triple_it(n : i32) : i32
└─ return
   └─ * : i32
      ├─ n : i32
      └─ int 3

fn apply(cb : FN_TYPE, x : i32) : i32
└─ return
   └─ call cb : i32
      └─ x : i32

fn main() : i32
├─ output : void
│  ├─ call apply : i32
│  │  ├─ FN_TYPE : FN_TYPE
│  │  └─ int 21
│  └─ chars "\n"
└─ output : void
   ├─ call apply : i32
   │  ├─ FN_TYPE : FN_TYPE
   │  └─ int 7
   └─ chars "\n"
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
// functions/006.ura - return an integer literal

main():
    return 42
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn main() : i32
└─ return
   └─ int 42
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
// functions/007.ura - declare a variable and return it

main():
    a int = 7
    return a
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn main() : i32
├─ = : i32
│  ├─ a : i32
│  └─ int 7
└─ return
   └─ a : i32
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
// functions/008.ura - function parameters and a direct call

fn add(a int, b int) int:
    return a + b
main():
    return add(40, 2)
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn add(a : i32, b : i32) : i32
└─ return
   └─ + : i32
      ├─ a : i32
      └─ b : i32

fn main() : i32
└─ return
   └─ call add : i32
      ├─ int 40
      └─ int 2
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
// functions/009.ura - forward reference: call a function declared later

main():
    return dbl(21)
fn dbl(n int) int:
    return n + n
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn main() : i32
└─ return
   └─ call dbl : i32
      └─ int 21

fn dbl(n : i32) : i32
└─ return
   └─ + : i32
      ├─ n : i32
      └─ n : i32
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
// functions/010.ura - store a function in a fn-typed variable, then call it

fn twice(n int) int:
    return n + n
main():
    f fn(int) int = twice
    return f(21)
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn twice(n : i32) : i32
└─ return
   └─ + : i32
      ├─ n : i32
      └─ n : i32

fn main() : i32
├─ = : FN_TYPE
│  ├─ f : FN_TYPE
│  └─ FN_TYPE : FN_TYPE
└─ return
   └─ call f : i32
      └─ int 21
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
// functions/011.ura - pass a function as an argument (higher-order)

fn twice(n int) int:
    return n + n
fn apply(f fn(int) int, x int) int:
    return f(x)
main():
    return apply(twice, 21)
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn twice(n : i32) : i32
└─ return
   └─ + : i32
      ├─ n : i32
      └─ n : i32

fn apply(f : FN_TYPE, x : i32) : i32
└─ return
   └─ call f : i32
      └─ x : i32

fn main() : i32
└─ return
   └─ call apply : i32
      ├─ FN_TYPE : FN_TYPE
      └─ int 21
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

## 012 — nested non-capturing helper function

```ura
// functions/012.ura - nested non-capturing helper function

main():
    fn dbl(k int) int:
        return k + k
    return dbl(21)
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn main() : i32
├─ fn dbl(k : i32) : i32
│  └─ return
│     └─ + : i32
│        ├─ k : i32
│        └─ k : i32
└─ return
   └─ call dbl : i32
      └─ int 21
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

## 013 — a call used as an argument to another call

```ura
// functions/013.ura - a call used as an argument to another call

fn square(n int) int:
    return n * n
fn add(a int, b int) int:
    return a + b
main():
    return add(square(5), 3)
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn square(n : i32) : i32
└─ return
   └─ * : i32
      ├─ n : i32
      └─ n : i32

fn add(a : i32, b : i32) : i32
└─ return
   └─ + : i32
      ├─ a : i32
      └─ b : i32

fn main() : i32
└─ return
   └─ call add : i32
      ├─ call square : i32
      │  └─ int 5
      └─ int 3
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

## 014 — value parameters: take_damage

```ura
// functions/014.ura - value parameters: take_damage

fn take_damage(hp int, atk int) int:
    return hp - atk

main():
    result int = take_damage(100, 18)
    output("<", result, ">\n")
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn take_damage(hp : i32, atk : i32) : i32
└─ return
   └─ - : i32
      ├─ hp : i32
      └─ atk : i32

fn main() : i32
├─ = : i32
│  ├─ result : i32
│  └─ call take_damage : i32
│     ├─ int 100
│     └─ int 18
└─ output : void
   ├─ chars "<"
   ├─ result : i32
   └─ chars ">\n"
```

```out
<82>
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c"<\00", align 1
@str.1 = private unnamed_addr constant [3 x i8] c">\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define i32 @take_damage(i32 %0, i32 %1) {
entry:
  %hp = alloca i32, align 4
  store i32 %0, i32* %hp, align 4
  %atk = alloca i32, align 4
  store i32 %1, i32* %atk, align 4
  %hp1 = load i32, i32* %hp, align 4
  %atk2 = load i32, i32* %atk, align 4
  %sub = sub i32 %hp1, %atk2
  ret i32 %sub
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %call = call i32 @take_damage(i32 100, i32 18)
  store i32 %call, i32* %result, align 4
  %result1 = load i32, i32* %result, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %result1, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 015 — a by-value parameter does not change the caller

```ura
// functions/015.ura - a by-value parameter does not change the caller

fn inc(n int) int:
    n = n + 1
    return n
main():
    n int = 5
    inc(n)
    return n
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn inc(n : i32) : i32
├─ = : i32
│  ├─ n : i32
│  └─ + : i32
│     ├─ n : i32
│     └─ int 1
└─ return
   └─ n : i32

fn main() : i32
├─ = : i32
│  ├─ n : i32
│  └─ int 5
├─ call inc : i32
│  └─ n : i32
└─ return
   └─ n : i32
```

```out
```

```err
exit: 5
```

```ll

define i32 @inc(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %n1 = load i32, i32* %n, align 4
  %add = add i32 %n1, 1
  store i32 %add, i32* %n, align 4
  %n2 = load i32, i32* %n, align 4
  ret i32 %n2
}

define i32 @main() {
entry:
  %n = alloca i32, align 4
  store i32 5, i32* %n, align 4
  %n1 = load i32, i32* %n, align 4
  %call = call i32 @inc(i32 %n1)
  %n2 = load i32, i32* %n, align 4
  ret i32 %n2
}
```

## 016 — single ref param: buff hero hp

```ura
// functions/016.ura - single ref param: buff hero hp

fn buff(ref hp int) void:
    hp = hp + 20

main():
    hero_hp int = 80
    buff(ref hero_hp)
    output("<", hero_hp, ">\n")
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn buff(hp : i32) : void
└─ = : i32
   ├─ hp : i32
   └─ + : i32
      ├─ hp : i32
      └─ int 20

fn main() : i32
├─ = : i32
│  ├─ hero_hp : i32
│  └─ int 80
├─ call buff : void
│  └─ ref : i32
│     └─ hero_hp : i32
└─ output : void
   ├─ chars "<"
   ├─ hero_hp : i32
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

## 017 — swap two weapon damage values via refs

```ura
// functions/017.ura - swap two weapon damage values via refs

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
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn swap_weapons(a : i32, b : i32) : void
├─ = : i32
│  ├─ temp : i32
│  └─ a : i32
├─ = : i32
│  ├─ a : i32
│  └─ b : i32
└─ = : i32
   ├─ b : i32
   └─ temp : i32

fn main() : i32
├─ = : i32
│  ├─ sword : i32
│  └─ int 30
├─ = : i32
│  ├─ dagger : i32
│  └─ int 15
├─ call swap_weapons : void
│  ├─ ref : i32
│  │  └─ sword : i32
│  └─ ref : i32
│     └─ dagger : i32
└─ output : void
   ├─ chars "<"
   ├─ sword : i32
   ├─ chars " "
   ├─ dagger : i32
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

## 018 — refs used in expression, return result

```ura
// functions/018.ura - refs used in expression, return result

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
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn add_xp(a : i32, b : i32) : i32
├─ = : i32
│  ├─ result : i32
│  └─ + : i32
│     ├─ a : i32
│     └─ b : i32
└─ return
   └─ result : i32

fn main() : i32
├─ = : i32
│  ├─ kill_xp : i32
│  └─ int 30
├─ = : i32
│  ├─ bonus_xp : i32
│  └─ int 20
├─ z : i32
├─ = : i32
│  ├─ z : i32
│  └─ call add_xp : i32
│     ├─ ref : i32
│     │  └─ kill_xp : i32
│     └─ ref : i32
│        └─ bonus_xp : i32
└─ output : void
   ├─ chars "<"
   ├─ z : i32
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

## 019 — compound ref assign through in a loop

```ura
// functions/019.ura - compound ref assign through in a loop

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
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn tick(kills : i32) : void
└─ += : i32
   ├─ kills : i32
   └─ int 1

fn main() : i32
├─ = : i32
│  ├─ kills : i32
│  └─ int 0
├─ = : i32
│  ├─ i : i32
│  └─ int 0
├─ while
│  ├─ condition < : bool
│  │  ├─ i : i32
│  │  └─ int 5
│  ├─ call tick : void
│  │  └─ ref : i32
│  │     └─ kills : i32
│  └─ = : i32
│     ├─ i : i32
│     └─ + : i32
│        ├─ i : i32
│        └─ int 1
└─ output : void
   ├─ chars "<"
   ├─ kills : i32
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

## 020 — ref-to-value: ref passing a where value is expected

```ura
// functions/020.ura - ref-to-value: ref passing a where value is expected

fn show_hp(hp int) void:
    output("HP: ", hp, "\n")

main():
    hero_hp int = 75
    ref r int = ref hero_hp
    show_hp(r)
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn show_hp(hp : i32) : void
└─ output : void
   ├─ chars "HP: "
   ├─ hp : i32
   └─ chars "\n"

fn main() : i32
├─ = : i32
│  ├─ hero_hp : i32
│  └─ int 75
├─ = : i32
│  ├─ r : i32
│  └─ ref : i32
│     └─ hero_hp : i32
└─ call show_hp : void
   └─ r : i32
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

## 021 — ref parameter mutates the caller (counter)

```ura
// functions/021.ura - ref parameter mutates the caller (counter)

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
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn tick(n : i32) : i32
├─ = : i32
│  ├─ n : i32
│  └─ + : i32
│     ├─ n : i32
│     └─ int 1
└─ return
   └─ n : i32

fn main() : i32
├─ = : i32
│  ├─ n : i32
│  └─ int 0
├─ call tick : i32
│  └─ ref : i32
│     └─ n : i32
├─ call tick : i32
│  └─ ref : i32
│     └─ n : i32
└─ return
   └─ n : i32
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

## 022 — a void function with a ref parameter

```ura
// functions/022.ura - a void function with a ref parameter

fn bump(ref n int) void:
    n = n + 1
main():
    n int = 5
    bump(ref n)
    return n
```

```tree
proto fn printf(format : chars, ...) : i32

proto fn calloc(len : i64, size : i64) : chars

proto fn free(ptr : chars) : void

proto fn write(fd : i32, ptr : chars, len : i64) : i64

proto fn exit(code : i32) : void

fn bump(n : i32) : void
└─ = : i32
   ├─ n : i32
   └─ + : i32
      ├─ n : i32
      └─ int 1

fn main() : i32
├─ = : i32
│  ├─ n : i32
│  └─ int 5
├─ call bump : void
│  └─ ref : i32
│     └─ n : i32
└─ return
   └─ n : i32
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
