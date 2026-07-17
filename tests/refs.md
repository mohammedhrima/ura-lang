# refs

## index

- 001 — basic ref: bind to hp, ref write through
- 002 — aliasing: two refs to same hp, both see mutations
- 004 — separate refs to different vars: hp ref ref and mp
- 005 — multiply and subtract through refs: double shield, pay mana
- 006 — chain of compound mutations ref through a
- 007 — reference in conditional
- 008 — a ref reads correctly through every operator + as a by-value arg
- 009 — ref? nullable: bound at declaration, guarded reads pass, write-through works
- 010 — nested fn shares an enclosing var through a ref parameter (the sanctioned path)
- 011 — a ref reads correctly through the remaining math / comparison / bitwise / cast / logical ops

## 001 — basic ref: bind to hp, ref write through

```ura
// refs/001.ura - basic ref: bind to hp, ref write through

main():
    hp int = 100
    ref r int = ref hp
    r = 0
    output("<", hp, ">\n")
```

```out
<0>
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c"<\00", align 1
@str.1 = private unnamed_addr constant [3 x i8] c">\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define i32 @main() {
entry:
  %hp = alloca i32, align 4
  store i32 100, i32* %hp, align 4
  %r = alloca i32*, align 8
  store i32* %hp, i32** %r, align 8
  %ref = load i32*, i32** %r, align 8
  store i32 0, i32* %ref, align 4
  %hp1 = load i32, i32* %hp, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %hp1, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 002 — aliasing: two refs to same hp, both see mutations

```ura
// refs/002.ura - aliasing: two refs to same hp, both see mutations

main():
    hp int = 100
    ref a int = ref hp
    ref b int = ref hp
    a = 80
    b = b - 10
    output("<", hp, ">\n")
```

```out
<70>
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c"<\00", align 1
@str.1 = private unnamed_addr constant [3 x i8] c">\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define i32 @main() {
entry:
  %hp = alloca i32, align 4
  store i32 100, i32* %hp, align 4
  %a = alloca i32*, align 8
  store i32* %hp, i32** %a, align 8
  %b = alloca i32*, align 8
  store i32* %hp, i32** %b, align 8
  %ref = load i32*, i32** %a, align 8
  store i32 80, i32* %ref, align 4
  %ref1 = load i32*, i32** %b, align 8
  %ref2 = load i32*, i32** %b, align 8
  %b3 = load i32, i32* %ref2, align 4
  %sub = sub i32 %b3, 10
  store i32 %sub, i32* %ref1, align 4
  %hp4 = load i32, i32* %hp, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %hp4, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 004 — separate refs to different vars: hp ref ref and mp

```ura
// refs/004.ura - separate refs to different vars: hp ref ref and mp

main():
    hp int = 100
    mp int = 50

    ref shield int = ref hp
    shield = shield - 30
    output("HP after hit:   ", hp, "\n")

    ref mana int = ref mp
    mana = mana - 20
    output("MP after spell: ", mp, "\n")
```

```out
HP after hit:   70
MP after spell: 30
```

```err
```

```ll

@str = private unnamed_addr constant [17 x i8] c"HP after hit:   \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1
@str.2 = private unnamed_addr constant [17 x i8] c"MP after spell: \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define i32 @main() {
entry:
  %hp = alloca i32, align 4
  store i32 100, i32* %hp, align 4
  %mp = alloca i32, align 4
  store i32 50, i32* %mp, align 4
  %shield = alloca i32*, align 8
  store i32* %hp, i32** %shield, align 8
  %ref = load i32*, i32** %shield, align 8
  %ref1 = load i32*, i32** %shield, align 8
  %shield2 = load i32, i32* %ref1, align 4
  %sub = sub i32 %shield2, 30
  store i32 %sub, i32* %ref, align 4
  %hp3 = load i32, i32* %hp, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str, i32 0, i32 0), i32 %hp3, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %mana = alloca i32*, align 8
  store i32* %mp, i32** %mana, align 8
  %ref4 = load i32*, i32** %mana, align 8
  %ref5 = load i32*, i32** %mana, align 8
  %mana6 = load i32, i32* %ref5, align 4
  %sub7 = sub i32 %mana6, 20
  store i32 %sub7, i32* %ref4, align 4
  %mp8 = load i32, i32* %mp, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.4, i32 0, i32 0), i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.2, i32 0, i32 0), i32 %mp8, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 005 — multiply and subtract through refs: double shield, pay mana

```ura
// refs/005.ura - multiply and subtract through refs: double shield, pay mana

main():
    shield int = 10
    mp     int = 80

    ref s int = ref shield
    ref m int = ref mp
    s *= 2
    m -= 25

    output("<", shield, " ", mp, ">\n")
```

```out
<20 55>
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c"<\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.2 = private unnamed_addr constant [3 x i8] c">\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%s%d%s%d%s\00", align 1

define i32 @main() {
entry:
  %shield = alloca i32, align 4
  store i32 10, i32* %shield, align 4
  %mp = alloca i32, align 4
  store i32 80, i32* %mp, align 4
  %s = alloca i32*, align 8
  store i32* %shield, i32** %s, align 8
  %m = alloca i32*, align 8
  store i32* %mp, i32** %m, align 8
  %ref = load i32*, i32** %s, align 8
  %cur = load i32, i32* %ref, align 4
  %mul = mul i32 %cur, 2
  store i32 %mul, i32* %ref, align 4
  %ref1 = load i32*, i32** %m, align 8
  %cur2 = load i32, i32* %ref1, align 4
  %sub = sub i32 %cur2, 25
  store i32 %sub, i32* %ref1, align 4
  %shield3 = load i32, i32* %shield, align 4
  %mp4 = load i32, i32* %mp, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %shield3, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i32 %mp4, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.2, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 006 — chain of compound mutations ref through a

```ura
// refs/006.ura - chain of compound mutations ref through a

main():
    score int = 0
    ref r int = ref score
    r += 100
    r += 50
    r *= 2
    r -= 30
    output("<", score, ">\n")
```

```out
<270>
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c"<\00", align 1
@str.1 = private unnamed_addr constant [3 x i8] c">\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%s%d%s\00", align 1

define i32 @main() {
entry:
  %score = alloca i32, align 4
  store i32 0, i32* %score, align 4
  %r = alloca i32*, align 8
  store i32* %score, i32** %r, align 8
  %ref = load i32*, i32** %r, align 8
  %cur = load i32, i32* %ref, align 4
  %add = add i32 %cur, 100
  store i32 %add, i32* %ref, align 4
  %ref1 = load i32*, i32** %r, align 8
  %cur2 = load i32, i32* %ref1, align 4
  %add3 = add i32 %cur2, 50
  store i32 %add3, i32* %ref1, align 4
  %ref4 = load i32*, i32** %r, align 8
  %cur5 = load i32, i32* %ref4, align 4
  %mul = mul i32 %cur5, 2
  store i32 %mul, i32* %ref4, align 4
  %ref6 = load i32*, i32** %r, align 8
  %cur7 = load i32, i32* %ref6, align 4
  %sub = sub i32 %cur7, 30
  store i32 %sub, i32* %ref6, align 4
  %score8 = load i32, i32* %score, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %score8, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 007 — reference in conditional

```ura
// refs/007 — reference in conditional

main():
    hp int = 75
    ref r int = ref hp
    if r > 50:
        output("healthy\n")
    else:
        output("critical\n")
```

```out
healthy
```

```err
```

```ll

@str = private unnamed_addr constant [9 x i8] c"healthy\0A\00", align 1
@fmt = private unnamed_addr constant [3 x i8] c"%s\00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c"critical\0A\00", align 1
@fmt.2 = private unnamed_addr constant [3 x i8] c"%s\00", align 1

define i32 @main() {
entry:
  %hp = alloca i32, align 4
  store i32 75, i32* %hp, align 4
  %r = alloca i32*, align 8
  store i32* %hp, i32** %r, align 8
  %ref = load i32*, i32** %r, align 8
  %r1 = load i32, i32* %ref, align 4
  %gt = icmp sgt i32 %r1, 50
  br i1 %gt, label %then, label %next

endif:                                            ; preds = %next, %then
  ret i32 0

then:                                             ; preds = %entry
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt, i32 0, i32 0), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0))
  br label %endif

next:                                             ; preds = %entry
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @fmt.2, i32 0, i32 0), i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0))
  br label %endif
}

declare i32 @printf(i8*, ...)
```

## 008 — a ref reads correctly through every operator + as a by-value arg

```ura
// refs/008 — a ref reads correctly through every operator + as a by-value arg

fn twice(n int) int:
    return n * 2

main():
    x int = 12
    ref r int = ref x
    output(r + 3, "\n")
    output(r - 5, "\n")
    output(r * 2, "\n")
    output(r > 10, "\n")
    output(r == 12, "\n")
    output(r & 10, "\n")
    output(r << 1, "\n")
    output(-r, "\n")
    output(~r, "\n")
    output(twice(r), "\n")
    return 0
```

```out
15
7
24
True
True
8
24
-12
-13
24
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.2 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [5 x i8] c"%s%s\00", align 1
@true_str.7 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.8 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.10 = private unnamed_addr constant [5 x i8] c"%s%s\00", align 1
@str.11 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.12 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.13 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.14 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.15 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.16 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.17 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.18 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.19 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.20 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1

define i32 @twice(i32 %0) {
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
  store i32 12, i32* %x, align 4
  %r = alloca i32*, align 8
  store i32* %x, i32** %r, align 8
  %ref = load i32*, i32** %r, align 8
  %r1 = load i32, i32* %ref, align 4
  %add = add i32 %r1, 3
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 %add, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  %ref2 = load i32*, i32** %r, align 8
  %r3 = load i32, i32* %ref2, align 4
  %sub = sub i32 %r3, 5
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.2, i32 0, i32 0), i32 %sub, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %ref4 = load i32*, i32** %r, align 8
  %r5 = load i32, i32* %ref4, align 4
  %mul = mul i32 %r5, 2
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.4, i32 0, i32 0), i32 %mul, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %ref6 = load i32*, i32** %r, align 8
  %r7 = load i32, i32* %ref6, align 4
  %gt = icmp sgt i32 %r7, 10
  %bool_str = select i1 %gt, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.6, i32 0, i32 0), i8* %bool_str, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  %ref8 = load i32*, i32** %r, align 8
  %r9 = load i32, i32* %ref8, align 4
  %eq = icmp eq i32 %r9, 12
  %bool_str10 = select i1 %eq, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.7, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.8, i32 0, i32 0)
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.10, i32 0, i32 0), i8* %bool_str10, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.9, i32 0, i32 0))
  %ref11 = load i32*, i32** %r, align 8
  %r12 = load i32, i32* %ref11, align 4
  %band = and i32 %r12, 10
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.12, i32 0, i32 0), i32 %band, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.11, i32 0, i32 0))
  %ref13 = load i32*, i32** %r, align 8
  %r14 = load i32, i32* %ref13, align 4
  %shl = shl i32 %r14, 1
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.14, i32 0, i32 0), i32 %shl, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.13, i32 0, i32 0))
  %ref15 = load i32*, i32** %r, align 8
  %r16 = load i32, i32* %ref15, align 4
  %sub17 = sub i32 0, %r16
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.16, i32 0, i32 0), i32 %sub17, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.15, i32 0, i32 0))
  %ref18 = load i32*, i32** %r, align 8
  %r19 = load i32, i32* %ref18, align 4
  %not = xor i32 %r19, -1
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.18, i32 0, i32 0), i32 %not, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.17, i32 0, i32 0))
  %ref20 = load i32*, i32** %r, align 8
  %r21 = load i32, i32* %ref20, align 4
  %call = call i32 @twice(i32 %r21)
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.20, i32 0, i32 0), i32 %call, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.19, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 009 — ref? nullable: bound at declaration, guarded reads pass, write-through works

```ura
// refs/009 — ref? nullable: bound at declaration, guarded reads pass, write-through works

main():
    x int = 42
    ref? m int = ref x
    output(m, "\n")
    m = 7
    output(x, "\n")
    return 0
```

```out
42
7
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.2 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1

define i32 @main() {
entry:
  %x = alloca i32, align 4
  store i32 42, i32* %x, align 4
  %m = alloca i32*, align 8
  store i32* %x, i32** %m, align 8
  %ref = load i32*, i32** %m, align 8
  %m1 = load i32, i32* %ref, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 %m1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  %ref2 = load i32*, i32** %m, align 8
  store i32 7, i32* %ref2, align 4
  %x3 = load i32, i32* %x, align 4
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.2, i32 0, i32 0), i32 %x3, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 010 — nested fn shares an enclosing var through a ref parameter (the sanctioned path)

```ura
// refs/010 — nested fn shares an enclosing var through a ref parameter (the sanctioned path)

main():
    x int = 5
    fn bump(ref n int) void:
        n = n + 1
    bump(ref x)
    output(x, "\n")
    return 0
```

```out
6
```

```err
```

```ll

@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1

define i32 @main() {
entry:
  %x = alloca i32, align 4
  store i32 5, i32* %x, align 4
  call void @bump(i32* %x)
  %x1 = load i32, i32* %x, align 4
  %0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 %x1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

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

declare i32 @printf(i8*, ...)
```

## 011 — a ref reads correctly through the remaining math / comparison / bitwise / cast / logical ops

```ura
// refs/011 — a ref reads correctly through the remaining math / comparison / bitwise / cast / logical ops

main():
    a int = 20
    ref r int = ref a
    output(r / 6, "\n")
    output(r % 6, "\n")
    output(r != 20, "\n")
    output(r < 25, "\n")
    output(r <= 20, "\n")
    output(r >= 21, "\n")
    output(r | 3, "\n")
    output(r ^ 4, "\n")
    output(r >> 2, "\n")
    output(r as long, "\n")
    flag bool = True
    ref b bool = ref flag
    output(b and False, "\n")
    output(b or False, "\n")
    output(not b, "\n")
    return 0
```

```out
3
2
False
True
True
False
23
16
5
20
False
True
False
```

```err
```

```ll

@trap_msg = private unnamed_addr constant [160 x i8] c"runtime error: Division by zero\0A  011.ura:6:14\0A  |\0A6 |     output(r / 6, \22\\n\22)\0A  |              ^\0A\00", align 1
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@trap_msg.1 = private unnamed_addr constant [158 x i8] c"runtime error: Modulo by zero\0A  011.ura:7:14\0A  |\0A7 |     output(r % 6, \22\\n\22)\0A  |              ^\0A\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.3 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.5 = private unnamed_addr constant [5 x i8] c"%s%s\00", align 1
@true_str.6 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.7 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.8 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.9 = private unnamed_addr constant [5 x i8] c"%s%s\00", align 1
@true_str.10 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.11 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.12 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.13 = private unnamed_addr constant [5 x i8] c"%s%s\00", align 1
@true_str.14 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.15 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.16 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.17 = private unnamed_addr constant [5 x i8] c"%s%s\00", align 1
@str.18 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.19 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.20 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.21 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.22 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.23 = private unnamed_addr constant [5 x i8] c"%d%s\00", align 1
@str.24 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.25 = private unnamed_addr constant [7 x i8] c"%lld%s\00", align 1
@true_str.26 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.27 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.28 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.29 = private unnamed_addr constant [5 x i8] c"%s%s\00", align 1
@true_str.30 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.31 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.32 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.33 = private unnamed_addr constant [5 x i8] c"%s%s\00", align 1
@true_str.34 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.35 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.36 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.37 = private unnamed_addr constant [5 x i8] c"%s%s\00", align 1

define i32 @main() {
entry:
  %a = alloca i32, align 4
  store i32 20, i32* %a, align 4
  %r = alloca i32*, align 8
  store i32* %a, i32** %r, align 8
  %ref = load i32*, i32** %r, align 8
  %r1 = load i32, i32* %ref, align 4
  br i1 false, label %trap, label %cont

trap:                                             ; preds = %entry
  %0 = call i64 @write(i32 2, i8* getelementptr inbounds ([160 x i8], [160 x i8]* @trap_msg, i32 0, i32 0), i64 159)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %div = sdiv i32 %r1, 6
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 %div, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  %ref2 = load i32*, i32** %r, align 8
  %r3 = load i32, i32* %ref2, align 4
  br i1 false, label %trap4, label %cont5

trap4:                                            ; preds = %cont
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([158 x i8], [158 x i8]* @trap_msg.1, i32 0, i32 0), i64 157)
  call void @exit(i32 1)
  unreachable

cont5:                                            ; preds = %cont
  %mod = srem i32 %r3, 6
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.3, i32 0, i32 0), i32 %mod, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %ref6 = load i32*, i32** %r, align 8
  %r7 = load i32, i32* %ref6, align 4
  %ne = icmp ne i32 %r7, 20
  %bool_str = select i1 %ne, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.5, i32 0, i32 0), i8* %bool_str, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  %ref8 = load i32*, i32** %r, align 8
  %r9 = load i32, i32* %ref8, align 4
  %lt = icmp slt i32 %r9, 25
  %bool_str10 = select i1 %lt, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.6, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.7, i32 0, i32 0)
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.9, i32 0, i32 0), i8* %bool_str10, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.8, i32 0, i32 0))
  %ref11 = load i32*, i32** %r, align 8
  %r12 = load i32, i32* %ref11, align 4
  %le = icmp sle i32 %r12, 20
  %bool_str13 = select i1 %le, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.10, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.11, i32 0, i32 0)
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.13, i32 0, i32 0), i8* %bool_str13, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.12, i32 0, i32 0))
  %ref14 = load i32*, i32** %r, align 8
  %r15 = load i32, i32* %ref14, align 4
  %ge = icmp sge i32 %r15, 21
  %bool_str16 = select i1 %ge, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.14, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.15, i32 0, i32 0)
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.17, i32 0, i32 0), i8* %bool_str16, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.16, i32 0, i32 0))
  %ref17 = load i32*, i32** %r, align 8
  %r18 = load i32, i32* %ref17, align 4
  %bor = or i32 %r18, 3
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.19, i32 0, i32 0), i32 %bor, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.18, i32 0, i32 0))
  %ref19 = load i32*, i32** %r, align 8
  %r20 = load i32, i32* %ref19, align 4
  %bxor = xor i32 %r20, 4
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.21, i32 0, i32 0), i32 %bxor, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.20, i32 0, i32 0))
  %ref21 = load i32*, i32** %r, align 8
  %r22 = load i32, i32* %ref21, align 4
  %shr = ashr i32 %r22, 2
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.23, i32 0, i32 0), i32 %shr, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.22, i32 0, i32 0))
  %ref23 = load i32*, i32** %r, align 8
  %r24 = load i32, i32* %ref23, align 4
  %cast = sext i32 %r24 to i64
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.25, i32 0, i32 0), i64 %cast, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.24, i32 0, i32 0))
  %flag = alloca i1, align 1
  store i1 true, i1* %flag, align 1
  %b = alloca i1*, align 8
  store i1* %flag, i1** %b, align 8
  %ref25 = load i1*, i1** %b, align 8
  %b26 = load i1, i1* %ref25, align 1
  %and = and i1 %b26, false
  %bool_str27 = select i1 %and, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.26, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.27, i32 0, i32 0)
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.29, i32 0, i32 0), i8* %bool_str27, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.28, i32 0, i32 0))
  %ref28 = load i1*, i1** %b, align 8
  %b29 = load i1, i1* %ref28, align 1
  %or = or i1 %b29, false
  %bool_str30 = select i1 %or, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.30, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.31, i32 0, i32 0)
  %13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.33, i32 0, i32 0), i8* %bool_str30, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.32, i32 0, i32 0))
  %ref31 = load i1*, i1** %b, align 8
  %b32 = load i1, i1* %ref31, align 1
  %not = xor i1 %b32, true
  %bool_str33 = select i1 %not, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.34, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.35, i32 0, i32 0)
  %14 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.37, i32 0, i32 0), i8* %bool_str33, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.36, i32 0, i32 0))
  ret i32 0
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)

declare i32 @printf(i8*, ...)
```
