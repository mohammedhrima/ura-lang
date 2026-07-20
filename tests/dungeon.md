# dungeon

## index

- floor04
- floor09b
- floor14a

## floor04

```ura
// dungeon/floor04.ura
fn clamp(val i32, lo i32, hi i32) i32:
    if val < lo: return lo
    if val > hi: return hi
    return val

fn damage(atk i32, def i32) i32:
    return clamp(atk - def, 0, 999)

fn is_dead(hp i32) bool:
    return hp <= 0

fn square(n i32) i32:      return n * n
fn is_digit(c char) bool:  return c >= '0' and c <= '9'

main():
    d i32 = damage(25, 8)
    output("Orc takes ", d, " damage — dead: ", is_dead(d - 60), "\n")
    output("4 squared: ", square(4), "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

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

fn square(n : i32) : i32
└─ return
   └─ * : i32
      ├─ n : i32
      └─ n : i32

fn is_digit(c : char) : bool
└─ return
   └─ and : bool
      ├─ >= : bool
      │  ├─ c : char
      │  └─ char '0'
      └─ <= : bool
         ├─ c : char
         └─ char '9'

fn main() : i32
├─ = : i32
│  ├─ d : i32
│  └─ call damage : i32
│     ├─ int 25
│     └─ int 8
├─ output : void
│  ├─ char[] "Orc takes "
│  ├─ d : i32
│  ├─ char[] " damage — dead: "
│  ├─ call is_dead : bool
│  │  └─ - : i32
│  │     ├─ d : i32
│  │     └─ int 60
│  └─ char[] "\n"
└─ output : void
   ├─ char[] "4 squared: "
   ├─ call square : i32
   │  └─ int 4
   └─ char[] "\n"
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
@fmt = private unnamed_addr constant [17 x i8] c"%.*s%d%.*s%s%.*s\00", align 1
@str.3 = private unnamed_addr constant [12 x i8] c"4 squared: \00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.5 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1

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
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt, i32 0, i32 0), i32 10, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str, i32 0, i32 0), i32 %d1, i32 18, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @str.1, i32 0, i32 0), i8* %bool_str, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %call4 = call i32 @square(i32 4)
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.5, i32 0, i32 0), i32 11, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.3, i32 0, i32 0), i32 %call4, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## floor09b

```ura
// dungeon/floor09b.ura
main():
    hp  i32     = 80
    ref r i32 = ref hp
    r   = 100
    output(hp, "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

fn main() : i32
├─ = : i32
│  ├─ hp : i32
│  └─ int 80
├─ = : i32
│  ├─ r : i32
│  └─ ref : i32
│     └─ hp : i32
├─ = : i32
│  ├─ r : i32
│  └─ int 100
└─ output : void
   ├─ hp : i32
   └─ char[] "\n"
```

```out
100
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1

define i32 @main() {
entry:
  %hp = alloca i32, align 4
  store i32 80, i32* %hp, align 4
  %r = alloca i32*, align 8
  store i32* %hp, i32** %r, align 8
  %ref = load i32*, i32** %r, align 8
  store i32 100, i32* %ref, align 4
  %hp1 = load i32, i32* %hp, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %hp1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## floor14a

```ura
// dungeon/floor14a.ura
proto fn strlen(s pointer) i32
proto fn write(fd i32, ptr pointer, len i32) i32
proto fn printf(fmt pointer, ...) i32

main():
    n i32 = strlen("dungeon")
    write(1, "hi\n", 3)
    printf("length: %d\n", n)
```

```tree
```

```out
```

```err
error: Conflicting declaration of 'write': parameter 3 is i32, expected i64
  floor14a.ura:3:7
  |
3 | proto fn write(fd i32, ptr pointer, len i32) i32
  |       ^^
note: Previous declaration of 'write' is here
   ura-lib/common.ura:21:7
   |
21 | proto fn write(fd i32, ptr pointer, len i64) i64
   |       ^^
```

```ll
```
