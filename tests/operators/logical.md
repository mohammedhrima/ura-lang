# operators / logical

## index

- 001 — logical ops: &&, ||, !, and, or, not
- 002 — and / or with comparisons and precedence

## 001 — logical ops: &&, ||, !, and, or, not

```ura
// operators/logical/001.ura - logical ops: &&, ||, !, and, or, not

fn is_alive(hp int) bool:
    return hp > 0

fn has_key(keys int) bool:
    return keys > 0

main():
    hp       int  = 60
    keys     int  = 1
    exhausted bool = False

    // &&
    can_proceed bool = is_alive(hp) && has_key(keys) && !exhausted
    output("can_proceed: ", can_proceed, "\n")   // True

    // ||
    escape bool = hp < 10 || keys == 0
    output("escape:      ", escape,      "\n")   // False

    // !
    blocked bool = !can_proceed
    output("blocked:     ", blocked,     "\n")   // False

    // and / or / not keywords
    if is_alive(hp) and not exhausted:
        output("hero fights\n")

    if hp < 10 or keys == 0:
        output("hero retreats\n")
    else:
        output("hero advances\n")

    // double negation
    alive bool = is_alive(hp)
    output("!!alive: ", !(!alive), "\n")   // True
```

```tree
fn is_alive(hp : int) : bool
└─ return
   └─ > : bool
      ├─ hp : int
      └─ int 0

fn has_key(keys : int) : bool
└─ return
   └─ > : bool
      ├─ keys : int
      └─ int 0

fn main() : int
├─ = : int
│  ├─ hp : int
│  └─ int 60
├─ = : int
│  ├─ keys : int
│  └─ int 1
├─ = : bool
│  ├─ exhausted : bool
│  └─ bool False
├─ = : bool
│  ├─ can_proceed : bool
│  └─ and : bool
│     ├─ and : bool
│     │  ├─ call is_alive : bool
│     │  │  └─ hp : int
│     │  └─ call has_key : bool
│     │     └─ keys : int
│     └─ not : bool
│        └─ exhausted : bool
├─ output : void
│  ├─ chars "can_proceed: "
│  ├─ can_proceed : bool
│  └─ chars "\n"
├─ = : bool
│  ├─ escape : bool
│  └─ or : bool
│     ├─ < : bool
│     │  ├─ hp : int
│     │  └─ int 10
│     └─ == : bool
│        ├─ keys : int
│        └─ int 0
├─ output : void
│  ├─ chars "escape:      "
│  ├─ escape : bool
│  └─ chars "\n"
├─ = : bool
│  ├─ blocked : bool
│  └─ not : bool
│     └─ can_proceed : bool
├─ output : void
│  ├─ chars "blocked:     "
│  ├─ blocked : bool
│  └─ chars "\n"
├─ if
│  ├─ condition and : bool
│  │  ├─ call is_alive : bool
│  │  │  └─ hp : int
│  │  └─ not : bool
│  │     └─ exhausted : bool
│  └─ output : void
│     └─ chars "hero fights\n"
├─ if
│  ├─ condition or : bool
│  │  ├─ < : bool
│  │  │  ├─ hp : int
│  │  │  └─ int 10
│  │  └─ == : bool
│  │     ├─ keys : int
│  │     └─ int 0
│  ├─ output : void
│  │  └─ chars "hero retreats\n"
│  └─ else
│     └─ output : void
│        └─ chars "hero advances\n"
├─ = : bool
│  ├─ alive : bool
│  └─ call is_alive : bool
│     └─ hp : int
└─ output : void
   ├─ chars "!!alive: "
   ├─ not : bool
   │  └─ not : bool
   │     └─ alive : bool
   └─ chars "\n"
```

```out
can_proceed: True
escape:      False
blocked:     False
hero fights
hero advances
!!alive: True
```

```err
```

```ll

@str = private unnamed_addr constant [14 x i8] c"can_proceed: \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1
@str.2 = private unnamed_addr constant [14 x i8] c"escape:      \00", align 1
@true_str.3 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.4 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1
@str.7 = private unnamed_addr constant [14 x i8] c"blocked:     \00", align 1
@true_str.8 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.9 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.10 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.11 = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1
@str.12 = private unnamed_addr constant [13 x i8] c"hero fights\0A\00", align 1
@fmt.13 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.14 = private unnamed_addr constant [15 x i8] c"hero retreats\0A\00", align 1
@fmt.15 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.16 = private unnamed_addr constant [15 x i8] c"hero advances\0A\00", align 1
@fmt.17 = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.18 = private unnamed_addr constant [10 x i8] c"!!alive: \00", align 1
@true_str.19 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.20 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.21 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.22 = private unnamed_addr constant [7 x i8] c"%s%s%s\00", align 1

define i1 @is_alive(i32 %0) {
entry:
  %hp = alloca i32, align 4
  store i32 %0, i32* %hp, align 4
  %hp1 = load i32, i32* %hp, align 4
  %gt = icmp sgt i32 %hp1, 0
  ret i1 %gt
}

define i1 @has_key(i32 %0) {
entry:
  %keys = alloca i32, align 4
  store i32 %0, i32* %keys, align 4
  %keys1 = load i32, i32* %keys, align 4
  %gt = icmp sgt i32 %keys1, 0
  ret i1 %gt
}

define i32 @main() {
entry:
  %hp = alloca i32, align 4
  store i32 60, i32* %hp, align 4
  %keys = alloca i32, align 4
  store i32 1, i32* %keys, align 4
  %exhausted = alloca i1, align 1
  store i1 false, i1* %exhausted, align 1
  %can_proceed = alloca i1, align 1
  %hp1 = load i32, i32* %hp, align 4
  %call = call i1 @is_alive(i32 %hp1)
  %keys2 = load i32, i32* %keys, align 4
  %call3 = call i1 @has_key(i32 %keys2)
  %and = and i1 %call, %call3
  %exhausted4 = load i1, i1* %exhausted, align 1
  %not = xor i1 %exhausted4, true
  %and5 = and i1 %and, %not
  store i1 %and5, i1* %can_proceed, align 1
  %can_proceed6 = load i1, i1* %can_proceed, align 1
  %bool_str = select i1 %can_proceed6, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str, i32 0, i32 0), i8* %bool_str, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %escape = alloca i1, align 1
  %hp7 = load i32, i32* %hp, align 4
  %lt = icmp slt i32 %hp7, 10
  %keys8 = load i32, i32* %keys, align 4
  %eq = icmp eq i32 %keys8, 0
  %or = or i1 %lt, %eq
  store i1 %or, i1* %escape, align 1
  %escape9 = load i1, i1* %escape, align 1
  %bool_str10 = select i1 %escape9, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.3, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.4, i32 0, i32 0)
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.6, i32 0, i32 0), i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.2, i32 0, i32 0), i8* %bool_str10, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  %blocked = alloca i1, align 1
  %can_proceed11 = load i1, i1* %can_proceed, align 1
  %not12 = xor i1 %can_proceed11, true
  store i1 %not12, i1* %blocked, align 1
  %blocked13 = load i1, i1* %blocked, align 1
  %bool_str14 = select i1 %blocked13, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.8, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.9, i32 0, i32 0)
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.11, i32 0, i32 0), i8* getelementptr inbounds ([14 x i8], [14 x i8]* @str.7, i32 0, i32 0), i8* %bool_str14, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.10, i32 0, i32 0))
  %hp15 = load i32, i32* %hp, align 4
  %call16 = call i1 @is_alive(i32 %hp15)
  %exhausted17 = load i1, i1* %exhausted, align 1
  %not18 = xor i1 %exhausted17, true
  %and19 = and i1 %call16, %not18
  br i1 %and19, label %then, label %endif

endif:                                            ; preds = %then, %entry
  %hp22 = load i32, i32* %hp, align 4
  %lt23 = icmp slt i32 %hp22, 10
  %keys24 = load i32, i32* %keys, align 4
  %eq25 = icmp eq i32 %keys24, 0
  %or26 = or i1 %lt23, %eq25
  br i1 %or26, label %then21, label %next

then:                                             ; preds = %entry
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.13, i32 0, i32 0), i8* getelementptr inbounds ([13 x i8], [13 x i8]* @str.12, i32 0, i32 0))
  br label %endif

endif20:                                          ; preds = %next, %then21
  %alive = alloca i1, align 1
  %hp27 = load i32, i32* %hp, align 4
  %call28 = call i1 @is_alive(i32 %hp27)
  store i1 %call28, i1* %alive, align 1
  %alive29 = load i1, i1* %alive, align 1
  %not30 = xor i1 %alive29, true
  %not31 = xor i1 %not30, true
  %bool_str32 = select i1 %not31, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.19, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.20, i32 0, i32 0)
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.22, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.18, i32 0, i32 0), i8* %bool_str32, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.21, i32 0, i32 0))
  ret i32 0

then21:                                           ; preds = %endif
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.15, i32 0, i32 0), i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.14, i32 0, i32 0))
  br label %endif20

next:                                             ; preds = %endif
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.17, i32 0, i32 0), i8* getelementptr inbounds ([15 x i8], [15 x i8]* @str.16, i32 0, i32 0))
  br label %endif20
}

declare i32 @printf(i8*, ...)
```

## 002 — and / or with comparisons and precedence

```ura
// operators/logical/002.ura - and / or with comparisons and precedence

main():
    a int = 5
    b int = 9
    output(a < b and b == 9, "\n")
    output(a > b or b == 9, "\n")
    output(a < b and a > b, "\n")
    output(a < b or a > b, "\n")
    return 0
```

```tree
fn main() : int
├─ = : int
│  ├─ a : int
│  └─ int 5
├─ = : int
│  ├─ b : int
│  └─ int 9
├─ output : void
│  ├─ and : bool
│  │  ├─ < : bool
│  │  │  ├─ a : int
│  │  │  └─ b : int
│  │  └─ == : bool
│  │     ├─ b : int
│  │     └─ int 9
│  └─ chars "\n"
├─ output : void
│  ├─ or : bool
│  │  ├─ > : bool
│  │  │  ├─ a : int
│  │  │  └─ b : int
│  │  └─ == : bool
│  │     ├─ b : int
│  │     └─ int 9
│  └─ chars "\n"
├─ output : void
│  ├─ and : bool
│  │  ├─ < : bool
│  │  │  ├─ a : int
│  │  │  └─ b : int
│  │  └─ > : bool
│  │     ├─ a : int
│  │     └─ b : int
│  └─ chars "\n"
├─ output : void
│  ├─ or : bool
│  │  ├─ < : bool
│  │  │  ├─ a : int
│  │  │  └─ b : int
│  │  └─ > : bool
│  │     ├─ a : int
│  │     └─ b : int
│  └─ chars "\n"
└─ return
   └─ int 0
```

```out
True
True
False
True
```

```err
```

```ll

@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%s%s\00", align 1
@true_str.1 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.2 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [5 x i8] c"%s%s\00", align 1
@true_str.5 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.6 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.7 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.8 = private unnamed_addr constant [5 x i8] c"%s%s\00", align 1
@true_str.9 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.10 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.11 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.12 = private unnamed_addr constant [5 x i8] c"%s%s\00", align 1

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 5, i32* %a, align 4
  %b = alloca i32, align 4
  store i32 9, i32* %b, align 4
  %a1 = load i32, i32* %a, align 4
  %b2 = load i32, i32* %b, align 4
  %lt = icmp slt i32 %a1, %b2
  %b3 = load i32, i32* %b, align 4
  %eq = icmp eq i32 %b3, 9
  %and = and i1 %lt, %eq
  %bool_str = select i1 %and, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i8* %bool_str, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  %a4 = load i32, i32* %a, align 4
  %b5 = load i32, i32* %b, align 4
  %gt = icmp sgt i32 %a4, %b5
  %b6 = load i32, i32* %b, align 4
  %eq7 = icmp eq i32 %b6, 9
  %or = or i1 %gt, %eq7
  %bool_str8 = select i1 %or, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.1, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.2, i32 0, i32 0)
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.4, i32 0, i32 0), i8* %bool_str8, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %a9 = load i32, i32* %a, align 4
  %b10 = load i32, i32* %b, align 4
  %lt11 = icmp slt i32 %a9, %b10
  %a12 = load i32, i32* %a, align 4
  %b13 = load i32, i32* %b, align 4
  %gt14 = icmp sgt i32 %a12, %b13
  %and15 = and i1 %lt11, %gt14
  %bool_str16 = select i1 %and15, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.5, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.6, i32 0, i32 0)
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.8, i32 0, i32 0), i8* %bool_str16, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.7, i32 0, i32 0))
  %a17 = load i32, i32* %a, align 4
  %b18 = load i32, i32* %b, align 4
  %lt19 = icmp slt i32 %a17, %b18
  %a20 = load i32, i32* %a, align 4
  %b21 = load i32, i32* %b, align 4
  %gt22 = icmp sgt i32 %a20, %b21
  %or23 = or i1 %lt19, %gt22
  %bool_str24 = select i1 %or23, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.9, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.10, i32 0, i32 0)
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.12, i32 0, i32 0), i8* %bool_str24, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.11, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```
