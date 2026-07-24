# refs

## index

- 001 — basic ref: bind to hp, ref write through
- 002 — aliasing: two refs to same hp, both see mutations
- 003 — compound ref assignment through in a loop
- 004 — separate refs to different vars: hp ref ref and mp
- 005 — multiply and subtract through refs: f64 shield, pay mana
- 006 — chain of compound mutations ref through a
- 007 — reference in conditional
- 008 — a ref reads correctly through every operator + as a by-value arg
- 009 — ref? nullable: bound at declaration, guarded reads pass, write-through works
- 010 — nested fn shares an enclosing var through a ref parameter (the sanctioned path)
- 011 — a ref reads correctly through the remaining math / comparison / bitwise / cast / logical ops
- 012 — a reference must be bound when declared
- 013 — binding a reference to the wrong type
- 014 — a ref parameter given a value
- 015 — taking a reference to a literal
- 016 — a by-value parameter given a reference
- 017 — a reference to a name that does not exist
- 018 — a nested function may not capture an enclosing local
- 019 — a guarded use (m?) of an unbound ref? traps; raw m would segfault
- 020 — ref return: create a node on the heap and return a reference to it
- 021 — ref? return: one branch returns a reference, the other returns null
- 022 — linked list: build with ref, walk it with cur = ref cur.next
- 023 — a ref parameter rebinds a struct's ref field in place (add-front)
- 024 — ref? compared to null with == and !=, bound and unbound
- 025 — chained rebind removes the head: lst.head = ref lst.head.next
- 026 — declare a ref and bind it to a ref-returning call
- 027 — a ref-returning call passed straight into a ref parameter
- 028 — = ref rebinds the pointer, = value writes through the referent
- 029 — ref a == ref a is identity true, ref a == ref b is false
- 030 — a value function cannot return a reference
- 031 — a reference function must return a reference, not a bare value
- 032 — a ref declaration cannot be bound to a plain value
- 033 — a non-nullable value cannot be compared to null
- 034 — a field read chained directly on a ref-returning call

## 001 — basic ref: bind to hp, ref write through

```ura
// refs/001.ura - basic ref: bind to hp, ref write through

main():
    hp i32 = 100
    ref r i32 = ref hp
    r = 0
    output("<", hp, ">\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

fn main() : i32
├─ = : i32
│  ├─ hp : i32
│  └─ int 100
├─ = : i32
│  ├─ r : i32
│  └─ ref : i32
│     └─ hp : i32
├─ = : i32
│  ├─ r : i32
│  └─ int 0
└─ output : void
   ├─ char[] "<"
   ├─ hp : i32
   └─ char[] ">\n"
```

```out
<0>
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c"<\00", align 1
@str.1 = private unnamed_addr constant [3 x i8] c">\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %hp = alloca i32, align 4
  store i32 100, i32* %hp, align 4
  %r = alloca i32*, align 8
  store i32* %hp, i32** %r, align 8
  %ref = load i32*, i32** %r, align 8
  store i32 0, i32* %ref, align 4
  %hp1 = load i32, i32* %hp, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %hp1, i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 002 — aliasing: two refs to same hp, both see mutations

```ura
// refs/002.ura - aliasing: two refs to same hp, both see mutations

main():
    hp i32 = 100
    ref a i32 = ref hp
    ref b i32 = ref hp
    a = 80
    b = b - 10
    output("<", hp, ">\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

fn main() : i32
├─ = : i32
│  ├─ hp : i32
│  └─ int 100
├─ = : i32
│  ├─ a : i32
│  └─ ref : i32
│     └─ hp : i32
├─ = : i32
│  ├─ b : i32
│  └─ ref : i32
│     └─ hp : i32
├─ = : i32
│  ├─ a : i32
│  └─ int 80
├─ = : i32
│  ├─ b : i32
│  └─ - : i32
│     ├─ b : i32
│     └─ int 10
└─ output : void
   ├─ char[] "<"
   ├─ hp : i32
   └─ char[] ">\n"
```

```out
<70>
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c"<\00", align 1
@str.1 = private unnamed_addr constant [3 x i8] c">\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
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
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %hp4, i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 003 — compound ref assignment through in a loop

```ura
// refs/003.ura - compound ref assignment through in a loop

main():
    xp      i32 = 0
    xp_gain i32 = 50
    ref r i32 = ref xp
    i       i32 = 0
    while i < 4:
        r += xp_gain
        i = i + 1
    output("<", xp, ">\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

fn main() : i32
├─ = : i32
│  ├─ xp : i32
│  └─ int 0
├─ = : i32
│  ├─ xp_gain : i32
│  └─ int 50
├─ = : i32
│  ├─ r : i32
│  └─ ref : i32
│     └─ xp : i32
├─ = : i32
│  ├─ i : i32
│  └─ int 0
├─ while
│  ├─ condition < : bool
│  │  ├─ i : i32
│  │  └─ int 4
│  ├─ += : i32
│  │  ├─ r : i32
│  │  └─ xp_gain : i32
│  └─ = : i32
│     ├─ i : i32
│     └─ + : i32
│        ├─ i : i32
│        └─ int 1
└─ output : void
   ├─ char[] "<"
   ├─ xp : i32
   └─ char[] ">\n"
```

```out
<200>
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c"<\00", align 1
@str.1 = private unnamed_addr constant [3 x i8] c">\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %xp = alloca i32, align 4
  store i32 0, i32* %xp, align 4
  %xp_gain = alloca i32, align 4
  store i32 50, i32* %xp_gain, align 4
  %r = alloca i32*, align 8
  store i32* %xp, i32** %r, align 8
  %i = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %i1 = load i32, i32* %i, align 4
  %lt = icmp slt i32 %i1, 4
  br i1 %lt, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref = load i32*, i32** %r, align 8
  %xp_gain2 = load i32, i32* %xp_gain, align 4
  %cur = load i32, i32* %ref, align 4
  %add = add i32 %cur, %xp_gain2
  store i32 %add, i32* %ref, align 4
  %i3 = load i32, i32* %i, align 4
  %add4 = add i32 %i3, 1
  store i32 %add4, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %xp5 = load i32, i32* %xp, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %xp5, i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 004 — separate refs to different vars: hp ref ref and mp

```ura
// refs/004.ura - separate refs to different vars: hp ref ref and mp

main():
    hp i32 = 100
    mp i32 = 50

    ref shield i32 = ref hp
    shield = shield - 30
    output("HP after hit:   ", hp, "\n")

    ref mana i32 = ref mp
    mana = mana - 20
    output("MP after spell: ", mp, "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

fn main() : i32
├─ = : i32
│  ├─ hp : i32
│  └─ int 100
├─ = : i32
│  ├─ mp : i32
│  └─ int 50
├─ = : i32
│  ├─ shield : i32
│  └─ ref : i32
│     └─ hp : i32
├─ = : i32
│  ├─ shield : i32
│  └─ - : i32
│     ├─ shield : i32
│     └─ int 30
├─ output : void
│  ├─ char[] "HP after hit:   "
│  ├─ hp : i32
│  └─ char[] "\n"
├─ = : i32
│  ├─ mana : i32
│  └─ ref : i32
│     └─ mp : i32
├─ = : i32
│  ├─ mana : i32
│  └─ - : i32
│     ├─ mana : i32
│     └─ int 20
└─ output : void
   ├─ char[] "MP after spell: "
   ├─ mp : i32
   └─ char[] "\n"
```

```out
HP after hit:   70
MP after spell: 30
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [17 x i8] c"HP after hit:   \00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.2 = private unnamed_addr constant [17 x i8] c"MP after spell: \00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
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
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 16, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str, i32 0, i32 0), i32 %hp3, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %mana = alloca i32*, align 8
  store i32* %mp, i32** %mana, align 8
  %ref4 = load i32*, i32** %mana, align 8
  %ref5 = load i32*, i32** %mana, align 8
  %mana6 = load i32, i32* %ref5, align 4
  %sub7 = sub i32 %mana6, 20
  store i32 %sub7, i32* %ref4, align 4
  %mp8 = load i32, i32* %mp, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.4, i32 0, i32 0), i32 16, i8* getelementptr inbounds ([17 x i8], [17 x i8]* @str.2, i32 0, i32 0), i32 %mp8, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 005 — multiply and subtract through refs: f64 shield, pay mana

```ura
// refs/005.ura - multiply and subtract through refs: f64 shield, pay mana

main():
    shield i32 = 10
    mp     i32 = 80

    ref s i32 = ref shield
    ref m i32 = ref mp
    s *= 2
    m -= 25

    output("<", shield, " ", mp, ">\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

fn main() : i32
├─ = : i32
│  ├─ shield : i32
│  └─ int 10
├─ = : i32
│  ├─ mp : i32
│  └─ int 80
├─ = : i32
│  ├─ s : i32
│  └─ ref : i32
│     └─ shield : i32
├─ = : i32
│  ├─ m : i32
│  └─ ref : i32
│     └─ mp : i32
├─ *= : i32
│  ├─ s : i32
│  └─ int 2
├─ -= : i32
│  ├─ m : i32
│  └─ int 25
└─ output : void
   ├─ char[] "<"
   ├─ shield : i32
   ├─ char[] " "
   ├─ mp : i32
   └─ char[] ">\n"
```

```out
<20 55>
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c"<\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.2 = private unnamed_addr constant [3 x i8] c">\0A\00", align 1
@fmt = private unnamed_addr constant [17 x i8] c"%.*s%d%.*s%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
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
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %shield3, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i32 %mp4, i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.2, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 006 — chain of compound mutations ref through a

```ura
// refs/006.ura - chain of compound mutations ref through a

main():
    score i32 = 0
    ref r i32 = ref score
    r += 100
    r += 50
    r *= 2
    r -= 30
    output("<", score, ">\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

fn main() : i32
├─ = : i32
│  ├─ score : i32
│  └─ int 0
├─ = : i32
│  ├─ r : i32
│  └─ ref : i32
│     └─ score : i32
├─ += : i32
│  ├─ r : i32
│  └─ int 100
├─ += : i32
│  ├─ r : i32
│  └─ int 50
├─ *= : i32
│  ├─ r : i32
│  └─ int 2
├─ -= : i32
│  ├─ r : i32
│  └─ int 30
└─ output : void
   ├─ char[] "<"
   ├─ score : i32
   └─ char[] ">\n"
```

```out
<270>
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c"<\00", align 1
@str.1 = private unnamed_addr constant [3 x i8] c">\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
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
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i32 %score8, i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 007 — reference in conditional

```ura
// refs/007 — reference in conditional

main():
    hp i32 = 75
    ref r i32 = ref hp
    if r > 50:
        output("healthy\n")
    else:
        output("critical\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

fn main() : i32
├─ = : i32
│  ├─ hp : i32
│  └─ int 75
├─ = : i32
│  ├─ r : i32
│  └─ ref : i32
│     └─ hp : i32
└─ if
   ├─ condition > : bool
   │  ├─ r : i32
   │  └─ int 50
   ├─ output : void
   │  └─ char[] "healthy\n"
   └─ else
      └─ output : void
         └─ char[] "critical\n"
```

```out
healthy
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [9 x i8] c"healthy\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.1 = private unnamed_addr constant [10 x i8] c"critical\0A\00", align 1
@fmt.2 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
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
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str, i32 0, i32 0))
  br label %endif

next:                                             ; preds = %entry
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.2, i32 0, i32 0), i32 9, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @str.1, i32 0, i32 0))
  br label %endif
}

declare i32 @printf(i8*, ...)
```

## 008 — a ref reads correctly through every operator + as a by-value arg

```ura
// refs/008 — a ref reads correctly through every operator + as a by-value arg

fn twice(n i32) i32:
    return n * 2

main():
    x i32 = 12
    ref r i32 = ref x
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

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

fn twice(n : i32) : i32
└─ return
   └─ * : i32
      ├─ n : i32
      └─ int 2

fn main() : i32
├─ = : i32
│  ├─ x : i32
│  └─ int 12
├─ = : i32
│  ├─ r : i32
│  └─ ref : i32
│     └─ x : i32
├─ output : void
│  ├─ + : i32
│  │  ├─ r : i32
│  │  └─ int 3
│  └─ char[] "\n"
├─ output : void
│  ├─ - : i32
│  │  ├─ r : i32
│  │  └─ int 5
│  └─ char[] "\n"
├─ output : void
│  ├─ * : i32
│  │  ├─ r : i32
│  │  └─ int 2
│  └─ char[] "\n"
├─ output : void
│  ├─ > : bool
│  │  ├─ r : i32
│  │  └─ int 10
│  └─ char[] "\n"
├─ output : void
│  ├─ == : bool
│  │  ├─ r : i32
│  │  └─ int 12
│  └─ char[] "\n"
├─ output : void
│  ├─ & : i32
│  │  ├─ r : i32
│  │  └─ int 10
│  └─ char[] "\n"
├─ output : void
│  ├─ << : i32
│  │  ├─ r : i32
│  │  └─ int 1
│  └─ char[] "\n"
├─ output : void
│  ├─ - : i32
│  │  ├─ int 0
│  │  └─ r : i32
│  └─ char[] "\n"
├─ output : void
│  ├─ ~ : i32
│  │  └─ r : i32
│  └─ char[] "\n"
├─ output : void
│  ├─ call twice : i32
│  │  └─ r : i32
│  └─ char[] "\n"
└─ return
   └─ int 0
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

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.2 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.4 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.5 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.6 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@true_str.7 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.8 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.9 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.10 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@str.11 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.12 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.13 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.14 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.15 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.16 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.17 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.18 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.19 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.20 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @twice(i32 %0) {
entry:
  %n = alloca i32, align 4
  store i32 %0, i32* %n, align 4
  %n1 = load i32, i32* %n, align 4
  %mul = mul i32 %n1, 2
  ret i32 %mul
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %x = alloca i32, align 4
  store i32 12, i32* %x, align 4
  %r = alloca i32*, align 8
  store i32* %x, i32** %r, align 8
  %ref = load i32*, i32** %r, align 8
  %r1 = load i32, i32* %ref, align 4
  %add = add i32 %r1, 3
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %add, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  %ref2 = load i32*, i32** %r, align 8
  %r3 = load i32, i32* %ref2, align 4
  %sub = sub i32 %r3, 5
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.2, i32 0, i32 0), i32 %sub, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  %ref4 = load i32*, i32** %r, align 8
  %r5 = load i32, i32* %ref4, align 4
  %mul = mul i32 %r5, 2
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.4, i32 0, i32 0), i32 %mul, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  %ref6 = load i32*, i32** %r, align 8
  %r7 = load i32, i32* %ref6, align 4
  %gt = icmp sgt i32 %r7, 10
  %bool_str = select i1 %gt, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.6, i32 0, i32 0), i8* %bool_str, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.5, i32 0, i32 0))
  %ref8 = load i32*, i32** %r, align 8
  %r9 = load i32, i32* %ref8, align 4
  %eq = icmp eq i32 %r9, 12
  %bool_str10 = select i1 %eq, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.7, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.8, i32 0, i32 0)
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.10, i32 0, i32 0), i8* %bool_str10, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.9, i32 0, i32 0))
  %ref11 = load i32*, i32** %r, align 8
  %r12 = load i32, i32* %ref11, align 4
  %band = and i32 %r12, 10
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.12, i32 0, i32 0), i32 %band, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.11, i32 0, i32 0))
  %ref13 = load i32*, i32** %r, align 8
  %r14 = load i32, i32* %ref13, align 4
  %shl = shl i32 %r14, 1
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.14, i32 0, i32 0), i32 %shl, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.13, i32 0, i32 0))
  %ref15 = load i32*, i32** %r, align 8
  %r16 = load i32, i32* %ref15, align 4
  %sub17 = sub i32 0, %r16
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.16, i32 0, i32 0), i32 %sub17, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.15, i32 0, i32 0))
  %ref18 = load i32*, i32** %r, align 8
  %r19 = load i32, i32* %ref18, align 4
  %not = xor i32 %r19, -1
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.18, i32 0, i32 0), i32 %not, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.17, i32 0, i32 0))
  %ref20 = load i32*, i32** %r, align 8
  %r21 = load i32, i32* %ref20, align 4
  %call = call i32 @twice(i32 %r21)
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.20, i32 0, i32 0), i32 %call, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.19, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 009 — ref? nullable: bound at declaration, guarded reads pass, write-through works

```ura
// refs/009 — ref? nullable: bound at declaration, guarded reads pass, write-through works

main():
    x i32 = 42
    ref? m i32 = ref x
    output(m, "\n")
    m = 7
    output(x, "\n")
    return 0
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

fn main() : i32
├─ = : i32
│  ├─ x : i32
│  └─ int 42
├─ = : i32
│  ├─ m : i32
│  └─ ref : i32
│     └─ x : i32
├─ output : void
│  ├─ m : i32
│  └─ char[] "\n"
├─ = : i32
│  ├─ m : i32
│  └─ int 7
├─ output : void
│  ├─ x : i32
│  └─ char[] "\n"
└─ return
   └─ int 0
```

```out
42
7
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.2 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %x = alloca i32, align 4
  store i32 42, i32* %x, align 4
  %m = alloca i32*, align 8
  store i32* %x, i32** %m, align 8
  %ref = load i32*, i32** %m, align 8
  %m1 = load i32, i32* %ref, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %m1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  %ref2 = load i32*, i32** %m, align 8
  store i32 7, i32* %ref2, align 4
  %x3 = load i32, i32* %x, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.2, i32 0, i32 0), i32 %x3, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 010 — nested fn shares an enclosing var through a ref parameter (the sanctioned path)

```ura
// refs/010 — nested fn shares an enclosing var through a ref parameter (the sanctioned path)

main():
    x i32 = 5
    fn bump(ref n i32) void:
        n = n + 1
    bump(ref x)
    output(x, "\n")
    return 0
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

fn main() : i32
├─ = : i32
│  ├─ x : i32
│  └─ int 5
├─ fn bump(n : i32) : void
│  └─ = : i32
│     ├─ n : i32
│     └─ + : i32
│        ├─ n : i32
│        └─ int 1
├─ call bump : void
│  └─ ref : i32
│     └─ x : i32
├─ output : void
│  ├─ x : i32
│  └─ char[] "\n"
└─ return
   └─ int 0
```

```out
6
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %x = alloca i32, align 4
  store i32 5, i32* %x, align 4
  call void @bump(i32* %x)
  %x1 = load i32, i32* %x, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %x1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
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
    a i32 = 20
    ref r i32 = ref a
    output(r / 6, "\n")
    output(r % 6, "\n")
    output(r != 20, "\n")
    output(r < 25, "\n")
    output(r <= 20, "\n")
    output(r >= 21, "\n")
    output(r | 3, "\n")
    output(r ^ 4, "\n")
    output(r >> 2, "\n")
    output(r as i64, "\n")
    flag bool = True
    ref b bool = ref flag
    output(b and False, "\n")
    output(b or False, "\n")
    output(not b, "\n")
    return 0
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

fn main() : i32
├─ = : i32
│  ├─ a : i32
│  └─ int 20
├─ = : i32
│  ├─ r : i32
│  └─ ref : i32
│     └─ a : i32
├─ output : void
│  ├─ / : i32
│  │  ├─ r : i32
│  │  └─ int 6
│  └─ char[] "\n"
├─ output : void
│  ├─ % : i32
│  │  ├─ r : i32
│  │  └─ int 6
│  └─ char[] "\n"
├─ output : void
│  ├─ != : bool
│  │  ├─ r : i32
│  │  └─ int 20
│  └─ char[] "\n"
├─ output : void
│  ├─ < : bool
│  │  ├─ r : i32
│  │  └─ int 25
│  └─ char[] "\n"
├─ output : void
│  ├─ <= : bool
│  │  ├─ r : i32
│  │  └─ int 20
│  └─ char[] "\n"
├─ output : void
│  ├─ >= : bool
│  │  ├─ r : i32
│  │  └─ int 21
│  └─ char[] "\n"
├─ output : void
│  ├─ | : i32
│  │  ├─ r : i32
│  │  └─ int 3
│  └─ char[] "\n"
├─ output : void
│  ├─ ^ : i32
│  │  ├─ r : i32
│  │  └─ int 4
│  └─ char[] "\n"
├─ output : void
│  ├─ >> : i32
│  │  ├─ r : i32
│  │  └─ int 2
│  └─ char[] "\n"
├─ output : void
│  ├─ cast : i64
│  │  └─ r : i32
│  └─ char[] "\n"
├─ = : bool
│  ├─ flag : bool
│  └─ bool True
├─ = : bool
│  ├─ b : bool
│  └─ ref : bool
│     └─ flag : bool
├─ output : void
│  ├─ and : bool
│  │  ├─ b : bool
│  │  └─ bool False
│  └─ char[] "\n"
├─ output : void
│  ├─ or : bool
│  │  ├─ b : bool
│  │  └─ bool False
│  └─ char[] "\n"
├─ output : void
│  ├─ not : bool
│  │  └─ b : bool
│  └─ char[] "\n"
└─ return
   └─ int 0
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

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@trap_msg = private unnamed_addr constant [160 x i8] c"runtime error: Division by zero\0A  011.ura:6:14\0A  |\0A6 |     output(r / 6, \22\\n\22)\0A  |              ^\0A\00", align 1
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@trap_msg.1 = private unnamed_addr constant [158 x i8] c"runtime error: Modulo by zero\0A  011.ura:7:14\0A  |\0A7 |     output(r % 6, \22\\n\22)\0A  |              ^\0A\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.3 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.5 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@true_str.6 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.7 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.8 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.9 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@true_str.10 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.11 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.12 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.13 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@true_str.14 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.15 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.16 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.17 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@str.18 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.19 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.20 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.21 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.22 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.23 = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.24 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.25 = private unnamed_addr constant [9 x i8] c"%lld%.*s\00", align 1
@true_str.26 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.27 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.28 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.29 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@true_str.30 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.31 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.32 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.33 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1
@true_str.34 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.35 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.36 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.37 = private unnamed_addr constant [7 x i8] c"%s%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %a = alloca i32, align 4
  store i32 20, i32* %a, align 4
  %r = alloca i32*, align 8
  store i32* %a, i32** %r, align 8
  %ref = load i32*, i32** %r, align 8
  %r1 = load i32, i32* %ref, align 4
  br i1 false, label %trap, label %cont

trap:                                             ; preds = %entry
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([160 x i8], [160 x i8]* @trap_msg, i32 0, i32 0), i64 159)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %div = sdiv i32 %r1, 6
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %div, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  %ref2 = load i32*, i32** %r, align 8
  %r3 = load i32, i32* %ref2, align 4
  br i1 false, label %trap4, label %cont5

trap4:                                            ; preds = %cont
  %4 = call i64 @write(i32 2, i8* getelementptr inbounds ([158 x i8], [158 x i8]* @trap_msg.1, i32 0, i32 0), i64 157)
  call void @exit(i32 1)
  unreachable

cont5:                                            ; preds = %cont
  %mod = srem i32 %r3, 6
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.3, i32 0, i32 0), i32 %mod, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %ref6 = load i32*, i32** %r, align 8
  %r7 = load i32, i32* %ref6, align 4
  %ne = icmp ne i32 %r7, 20
  %bool_str = select i1 %ne, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.5, i32 0, i32 0), i8* %bool_str, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
  %ref8 = load i32*, i32** %r, align 8
  %r9 = load i32, i32* %ref8, align 4
  %lt = icmp slt i32 %r9, 25
  %bool_str10 = select i1 %lt, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.6, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.7, i32 0, i32 0)
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.9, i32 0, i32 0), i8* %bool_str10, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.8, i32 0, i32 0))
  %ref11 = load i32*, i32** %r, align 8
  %r12 = load i32, i32* %ref11, align 4
  %le = icmp sle i32 %r12, 20
  %bool_str13 = select i1 %le, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.10, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.11, i32 0, i32 0)
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.13, i32 0, i32 0), i8* %bool_str13, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.12, i32 0, i32 0))
  %ref14 = load i32*, i32** %r, align 8
  %r15 = load i32, i32* %ref14, align 4
  %ge = icmp sge i32 %r15, 21
  %bool_str16 = select i1 %ge, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.14, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.15, i32 0, i32 0)
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.17, i32 0, i32 0), i8* %bool_str16, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.16, i32 0, i32 0))
  %ref17 = load i32*, i32** %r, align 8
  %r18 = load i32, i32* %ref17, align 4
  %bor = or i32 %r18, 3
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.19, i32 0, i32 0), i32 %bor, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.18, i32 0, i32 0))
  %ref19 = load i32*, i32** %r, align 8
  %r20 = load i32, i32* %ref19, align 4
  %bxor = xor i32 %r20, 4
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.21, i32 0, i32 0), i32 %bxor, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.20, i32 0, i32 0))
  %ref21 = load i32*, i32** %r, align 8
  %r22 = load i32, i32* %ref21, align 4
  %shr = ashr i32 %r22, 2
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.23, i32 0, i32 0), i32 %shr, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.22, i32 0, i32 0))
  %ref23 = load i32*, i32** %r, align 8
  %r24 = load i32, i32* %ref23, align 4
  %cast = sext i32 %r24 to i64
  %13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @fmt.25, i32 0, i32 0), i64 %cast, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.24, i32 0, i32 0))
  %flag = alloca i1, align 1
  store i1 true, i1* %flag, align 1
  %b = alloca i1*, align 8
  store i1* %flag, i1** %b, align 8
  %ref25 = load i1*, i1** %b, align 8
  %b26 = load i1, i1* %ref25, align 1
  %and = and i1 %b26, false
  %bool_str27 = select i1 %and, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.26, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.27, i32 0, i32 0)
  %14 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.29, i32 0, i32 0), i8* %bool_str27, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.28, i32 0, i32 0))
  %ref28 = load i1*, i1** %b, align 8
  %b29 = load i1, i1* %ref28, align 1
  %or = or i1 %b29, false
  %bool_str30 = select i1 %or, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.30, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.31, i32 0, i32 0)
  %15 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.33, i32 0, i32 0), i8* %bool_str30, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.32, i32 0, i32 0))
  %ref31 = load i1*, i1** %b, align 8
  %b32 = load i1, i1* %ref31, align 1
  %not = xor i1 %b32, true
  %bool_str33 = select i1 %not, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.34, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.35, i32 0, i32 0)
  %16 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt.37, i32 0, i32 0), i8* %bool_str33, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.36, i32 0, i32 0))
  ret i32 0
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)

declare i32 @printf(i8*, ...)
```

## 012 — a reference must be bound when declared

```ura
// refs/012.ura - unbound ref

main():
    ref r i32
    return 0
```

```tree
```

```out
```

```err
error: A reference must be bound when declared (use 'ref?' for an optional reference)
  012.ura:4:9
  |
4 |     ref r i32
  |         ^
```

```ll
```

## 013 — binding a reference to the wrong type

```ura
// refs/013.ura - ref type mismatch

main():
    x i32 = 5
    ref y char[] = ref x
    return 0
```

```tree
```

```out
```

```err
error: Reference type mismatch: expected array, got i32
  013.ura:5:18
  |
5 |     ref y char[] = ref x
  |                  ^
```

```ll
```

## 014 — a ref parameter given a value

```ura
// refs/014.ura - argument must be passed by reference

fn tick(ref n i32) i32:
    n = n + 1
    return n

main():
    return tick(5)
```

```tree
```

```out
```

```err
error: Argument 1 to 'tick' must be passed by reference (ref x)
  014.ura:8:17
  |
8 |     return tick(5)
  |                 ^
```

```ll
```

## 015 — taking a reference to a literal

```ura
// refs/015.ura - ref needs a variable

main():
    ref r i32 = ref 5
    return 0
```

```tree
```

```out
```

```err
error: Expected a variable after 'ref'
  015.ura:4:17
  |
4 |     ref r i32 = ref 5
  |                 ^^^
```

```ll
```

## 016 — a by-value parameter given a reference

```ura
// refs/016.ura - argument does not take a reference

fn twice(n i32) i32:
    return n * 2

main():
    x i32 = 5
    return twice(ref x)
```

```tree
```

```out
```

```err
error: Argument 1 to 'twice' does not take a reference
  016.ura:8:18
  |
8 |     return twice(ref x)
  |                  ^^^
```

```ll
```

## 017 — a reference to a name that does not exist

```ura
// refs/017.ura - ref to an undeclared variable

main():
    ref r i32 = ref ghost
    return 0
```

```tree
```

```out
```

```err
error: Undeclared variable 'ghost'
  017.ura:4:21
  |
4 |     ref r i32 = ref ghost
  |                     ^^^^^
```

```ll
```

## 018 — a nested function may not capture an enclosing local

```ura
// refs/018.ura - capture is not allowed

main():
    x i32 = 5
    ref r i32 = ref x
    fn inner() i32:
        return r
    return inner()
```

```tree
```

```out
```

```err
error: Cannot use 'r' from an enclosing function - pass it as a parameter
  018.ura:7:16
  |
7 |         return r
  |                ^
```

```ll
```

## 019 — a guarded use (m?) of an unbound ref? traps; raw m would segfault

```ura
// refs/019.ura - a guarded use (m?) of an unbound ref? traps; raw m would segfault

main():
    ref? m i32
    output(m?, "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

fn main() : i32
├─ m : i32
└─ output : void
   ├─ m : i32
   └─ char[] "\n"
```

```out
```

```err
runtime error: reference 'm' used before it was bound - assign 'm = ref <target>' first
  019.ura:5:12
  |
5 |     output(m?, "\n")
  |            ^
exit: 1
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@trap_msg = private unnamed_addr constant [211 x i8] c"runtime error: reference 'm' used before it was bound - assign 'm = ref <target>' first\0A  019.ura:5:12\0A  |\0A5 |     output(m?, \22\\n\22)\0A  |            ^\0A\00", align 1
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %m = alloca i32*, align 8
  store i32* null, i32** %m, align 8
  %ref = load i32*, i32** %m, align 8
  %unbound = icmp eq i32* %ref, null
  br i1 %unbound, label %trap, label %cont

trap:                                             ; preds = %entry
  %2 = call i64 @write(i32 2, i8* getelementptr inbounds ([211 x i8], [211 x i8]* @trap_msg, i32 0, i32 0), i64 210)
  call void @exit(i32 1)
  unreachable

cont:                                             ; preds = %entry
  %m1 = load i32, i32* %ref, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %m1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i64 @write(i32, i8*, i64)

declare void @exit(i32)

declare i32 @printf(i8*, ...)
```

## 020 — ref return: create a node on the heap and return a reference to it

```ura
struct Node:
    value i32
    ref? next Node

fn make(v i32, heap Node[]) ref Node:
    heap[0].value = v
    return ref heap[0]

main():
    h Node[] = new Node[1]
    ref? a Node = make(42, h)
    output(a.value, "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Node
├─ value : i32
└─ next : STRUCT_CALL

fn make(v : i32, heap : array) : STRUCT_CALL
├─ = : i32
│  ├─ .value : i32
│  │  └─ index : STRUCT_CALL
│  │     ├─ heap : STRUCT_CALL[]
│  │     └─ int 0
│  └─ v : i32
└─ return
   └─ ref : STRUCT_CALL
      └─ index : STRUCT_CALL
         ├─ heap : STRUCT_CALL[]
         └─ int 0

fn main() : i32
├─ = : array
│  ├─ h : STRUCT_CALL[]
│  └─ array : STRUCT_CALL[]
│     └─ int 1
├─ = : STRUCT_CALL
│  ├─ a : STRUCT_CALL
│  └─ call make : STRUCT_CALL
│     ├─ int 42
│     └─ h : STRUCT_CALL[]
└─ output : void
   ├─ .value : i32
   │  └─ a : STRUCT_CALL
   └─ char[] "\n"
```

```out
42
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Node = type { i32, %Node* }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define %Node* @make(i32 %0, { %Node*, i64 } %1) {
entry:
  %v = alloca i32, align 4
  store i32 %0, i32* %v, align 4
  %heap = alloca { %Node*, i64 }, align 8
  store { %Node*, i64 } %1, { %Node*, i64 }* %heap, align 8
  %heap1 = load { %Node*, i64 }, { %Node*, i64 }* %heap, align 8
  %arr.data = extractvalue { %Node*, i64 } %heap1, 0
  %arr.at = getelementptr %Node, %Node* %arr.data, i32 0
  %value = getelementptr %Node, %Node* %arr.at, i32 0, i32 0
  %v2 = load i32, i32* %v, align 4
  store i32 %v2, i32* %value, align 4
  %heap3 = load { %Node*, i64 }, { %Node*, i64 }* %heap, align 8
  %arr.data4 = extractvalue { %Node*, i64 } %heap3, 0
  %arr.at5 = getelementptr %Node, %Node* %arr.data4, i32 0
  ret %Node* %arr.at5
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %h = alloca { %Node*, i64 }, align 8
  %heap = call i8* @calloc(i64 1, i64 16)
  %arr = bitcast i8* %heap to %Node*
  %arr.ptr = insertvalue { %Node*, i64 } undef, %Node* %arr, 0
  %arr.len = insertvalue { %Node*, i64 } %arr.ptr, i64 1, 1
  store { %Node*, i64 } %arr.len, { %Node*, i64 }* %h, align 8
  %a = alloca %Node*, align 8
  %h1 = load { %Node*, i64 }, { %Node*, i64 }* %h, align 8
  %call = call %Node* @make(i32 42, { %Node*, i64 } %h1)
  store %Node* %call, %Node** %a, align 8
  %ref = load %Node*, %Node** %a, align 8
  %value = getelementptr %Node, %Node* %ref, i32 0, i32 0
  %value2 = load i32, i32* %value, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %value2, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i32 @printf(i8*, ...)
```

## 021 — ref? return: one branch returns a reference, the other returns null

```ura
struct N:
    v i32
    ref? nx N

fn pick(h N[], want bool) ref? N:
    if want:
        return ref h[0]
    return null

main():
    h N[] = new N[1]
    h[0].v = 7
    ref? r N = pick(h, True)
    ref? e N = pick(h, False)
    output(r.v, " ", e == null, "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct N
├─ v : i32
└─ nx : STRUCT_CALL

fn pick(h : array, want : bool) : STRUCT_CALL
├─ if
│  ├─ condition want : bool
│  └─ return
│     └─ ref : STRUCT_CALL
│        └─ index : STRUCT_CALL
│           ├─ h : STRUCT_CALL[]
│           └─ int 0
└─ return
   └─ NULL_LIT : STRUCT_CALL

fn main() : i32
├─ = : array
│  ├─ h : STRUCT_CALL[]
│  └─ array : STRUCT_CALL[]
│     └─ int 1
├─ = : i32
│  ├─ .v : i32
│  │  └─ index : STRUCT_CALL
│  │     ├─ h : STRUCT_CALL[]
│  │     └─ int 0
│  └─ int 7
├─ = : STRUCT_CALL
│  ├─ r : STRUCT_CALL
│  └─ call pick : STRUCT_CALL
│     ├─ h : STRUCT_CALL[]
│     └─ bool True
├─ = : STRUCT_CALL
│  ├─ e : STRUCT_CALL
│  └─ call pick : STRUCT_CALL
│     ├─ h : STRUCT_CALL[]
│     └─ bool False
└─ output : void
   ├─ .v : i32
   │  └─ r : STRUCT_CALL
   ├─ char[] " "
   ├─ == : bool
   │  ├─ e : STRUCT_CALL
   │  └─ NULL_LIT : STRUCT_CALL
   └─ char[] "\n"
```

```out
7 True
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%N = type { i32, %N* }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [13 x i8] c"%d%.*s%s%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define %N* @pick({ %N*, i64 } %0, i1 %1) {
entry:
  %h = alloca { %N*, i64 }, align 8
  store { %N*, i64 } %0, { %N*, i64 }* %h, align 8
  %want = alloca i1, align 1
  store i1 %1, i1* %want, align 1
  %want1 = load i1, i1* %want, align 1
  br i1 %want1, label %then, label %endif

endif:                                            ; preds = %entry
  ret %N* null

then:                                             ; preds = %entry
  %h2 = load { %N*, i64 }, { %N*, i64 }* %h, align 8
  %arr.data = extractvalue { %N*, i64 } %h2, 0
  %arr.at = getelementptr %N, %N* %arr.data, i32 0
  ret %N* %arr.at
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %h = alloca { %N*, i64 }, align 8
  %heap = call i8* @calloc(i64 1, i64 16)
  %arr = bitcast i8* %heap to %N*
  %arr.ptr = insertvalue { %N*, i64 } undef, %N* %arr, 0
  %arr.len = insertvalue { %N*, i64 } %arr.ptr, i64 1, 1
  store { %N*, i64 } %arr.len, { %N*, i64 }* %h, align 8
  %h1 = load { %N*, i64 }, { %N*, i64 }* %h, align 8
  %arr.data = extractvalue { %N*, i64 } %h1, 0
  %arr.at = getelementptr %N, %N* %arr.data, i32 0
  %v = getelementptr %N, %N* %arr.at, i32 0, i32 0
  store i32 7, i32* %v, align 4
  %r = alloca %N*, align 8
  %h2 = load { %N*, i64 }, { %N*, i64 }* %h, align 8
  %call = call %N* @pick({ %N*, i64 } %h2, i1 true)
  store %N* %call, %N** %r, align 8
  %e = alloca %N*, align 8
  %h3 = load { %N*, i64 }, { %N*, i64 }* %h, align 8
  %call4 = call %N* @pick({ %N*, i64 } %h3, i1 false)
  store %N* %call4, %N** %e, align 8
  %ref = load %N*, %N** %r, align 8
  %v5 = getelementptr %N, %N* %ref, i32 0, i32 0
  %v6 = load i32, i32* %v5, align 4
  %ref7 = load %N*, %N** %e, align 8
  %refcmp = icmp eq %N* %ref7, null
  %bool_str = select i1 %refcmp, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i32 %v6, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i8* %bool_str, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i32 @printf(i8*, ...)
```

## 022 — linked list: build with ref, walk it with cur = ref cur.next

```ura
struct Node:
    value i32
    ref? next Node

main():
    a Node
    a.value = 10
    b Node
    b.value = 20
    c Node
    c.value = 30
    a.next = ref b
    b.next = ref c
    ref? cur Node = ref a
    sum i32 = 0
    while cur != null:
        sum += cur.value
        cur = ref cur.next
    output("sum=", sum, "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Node
├─ value : i32
└─ next : STRUCT_CALL

fn main() : i32
├─ a : STRUCT_CALL
├─ = : i32
│  ├─ .value : i32
│  │  └─ a : STRUCT_CALL
│  └─ int 10
├─ b : STRUCT_CALL
├─ = : i32
│  ├─ .value : i32
│  │  └─ b : STRUCT_CALL
│  └─ int 20
├─ c : STRUCT_CALL
├─ = : i32
│  ├─ .value : i32
│  │  └─ c : STRUCT_CALL
│  └─ int 30
├─ = : STRUCT_CALL
│  ├─ .next : STRUCT_CALL
│  │  └─ a : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ b : STRUCT_CALL
├─ = : STRUCT_CALL
│  ├─ .next : STRUCT_CALL
│  │  └─ b : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ c : STRUCT_CALL
├─ = : STRUCT_CALL
│  ├─ cur : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ a : STRUCT_CALL
├─ = : i32
│  ├─ sum : i32
│  └─ int 0
├─ while
│  ├─ condition != : bool
│  │  ├─ cur : STRUCT_CALL
│  │  └─ NULL_LIT : STRUCT_CALL
│  ├─ += : i32
│  │  ├─ sum : i32
│  │  └─ .value : i32
│  │     └─ cur : STRUCT_CALL
│  └─ = : STRUCT_CALL
│     ├─ cur : STRUCT_CALL
│     └─ ref : STRUCT_CALL
│        └─ .next : STRUCT_CALL
│           └─ cur : STRUCT_CALL
└─ output : void
   ├─ char[] "sum="
   ├─ sum : i32
   └─ char[] "\n"
```

```out
sum=60
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Node = type { i32, %Node* }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [5 x i8] c"sum=\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %a = alloca %Node, align 8
  store %Node zeroinitializer, %Node* %a, align 8
  %value = getelementptr %Node, %Node* %a, i32 0, i32 0
  store i32 10, i32* %value, align 4
  %b = alloca %Node, align 8
  store %Node zeroinitializer, %Node* %b, align 8
  %value1 = getelementptr %Node, %Node* %b, i32 0, i32 0
  store i32 20, i32* %value1, align 4
  %c = alloca %Node, align 8
  store %Node zeroinitializer, %Node* %c, align 8
  %value2 = getelementptr %Node, %Node* %c, i32 0, i32 0
  store i32 30, i32* %value2, align 4
  %next = getelementptr %Node, %Node* %a, i32 0, i32 1
  store %Node* %b, %Node** %next, align 8
  %next3 = getelementptr %Node, %Node* %b, i32 0, i32 1
  store %Node* %c, %Node** %next3, align 8
  %cur = alloca %Node*, align 8
  store %Node* %a, %Node** %cur, align 8
  %sum = alloca i32, align 4
  store i32 0, i32* %sum, align 4
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %ref = load %Node*, %Node** %cur, align 8
  %refcmp = icmp ne %Node* %ref, null
  br i1 %refcmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref4 = load %Node*, %Node** %cur, align 8
  %value5 = getelementptr %Node, %Node* %ref4, i32 0, i32 0
  %value6 = load i32, i32* %value5, align 4
  %cur7 = load i32, i32* %sum, align 4
  %add = add i32 %cur7, %value6
  store i32 %add, i32* %sum, align 4
  %ref8 = load %Node*, %Node** %cur, align 8
  %next9 = getelementptr %Node, %Node* %ref8, i32 0, i32 1
  %ref10 = load %Node*, %Node** %next9, align 8
  store %Node* %ref10, %Node** %cur, align 8
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %sum11 = load i32, i32* %sum, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt, i32 0, i32 0), i32 4, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str, i32 0, i32 0), i32 %sum11, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 023 — a ref parameter rebinds a struct's ref field in place (add-front)

```ura
struct Node:
    content i32
    ref? next Node

struct Lst:
    ref? head Node

fn push(ref lst Lst, ref node Node) void:
    node.next = ref lst.head
    lst.head  = ref node

main():
    lst Lst
    a Node
    a.content = 1
    b Node
    b.content = 2
    push(ref lst, ref a)
    push(ref lst, ref b)
    ref? cur Node = ref lst.head
    while cur != null:
        output(cur.content, " ")
        cur = ref cur.next
    output("\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Node
├─ content : i32
└─ next : STRUCT_CALL

struct Lst
└─ head : STRUCT_CALL

fn push(lst : STRUCT_CALL, node : STRUCT_CALL) : void
├─ = : STRUCT_CALL
│  ├─ .next : STRUCT_CALL
│  │  └─ node : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ .head : STRUCT_CALL
│        └─ lst : STRUCT_CALL
└─ = : STRUCT_CALL
   ├─ .head : STRUCT_CALL
   │  └─ lst : STRUCT_CALL
   └─ ref : STRUCT_CALL
      └─ node : STRUCT_CALL

fn main() : i32
├─ lst : STRUCT_CALL
├─ a : STRUCT_CALL
├─ = : i32
│  ├─ .content : i32
│  │  └─ a : STRUCT_CALL
│  └─ int 1
├─ b : STRUCT_CALL
├─ = : i32
│  ├─ .content : i32
│  │  └─ b : STRUCT_CALL
│  └─ int 2
├─ call push : void
│  ├─ ref : STRUCT_CALL
│  │  └─ lst : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ a : STRUCT_CALL
├─ call push : void
│  ├─ ref : STRUCT_CALL
│  │  └─ lst : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ b : STRUCT_CALL
├─ = : STRUCT_CALL
│  ├─ cur : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ .head : STRUCT_CALL
│        └─ lst : STRUCT_CALL
├─ while
│  ├─ condition != : bool
│  │  ├─ cur : STRUCT_CALL
│  │  └─ NULL_LIT : STRUCT_CALL
│  ├─ output : void
│  │  ├─ .content : i32
│  │  │  └─ cur : STRUCT_CALL
│  │  └─ char[] " "
│  └─ = : STRUCT_CALL
│     ├─ cur : STRUCT_CALL
│     └─ ref : STRUCT_CALL
│        └─ .next : STRUCT_CALL
│           └─ cur : STRUCT_CALL
└─ output : void
   └─ char[] "\n"
```

```out
2 1 
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Lst = type { %Node* }
%Node = type { i32, %Node* }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.2 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define void @push(%Lst* %0, %Node* %1) {
entry:
  %lst = alloca %Lst*, align 8
  store %Lst* %0, %Lst** %lst, align 8
  %node = alloca %Node*, align 8
  store %Node* %1, %Node** %node, align 8
  %ref = load %Lst*, %Lst** %lst, align 8
  %head = getelementptr %Lst, %Lst* %ref, i32 0, i32 0
  %ref1 = load %Node*, %Node** %head, align 8
  %ref2 = load %Node*, %Node** %node, align 8
  %next = getelementptr %Node, %Node* %ref2, i32 0, i32 1
  store %Node* %ref1, %Node** %next, align 8
  %ref3 = load %Node*, %Node** %node, align 8
  %ref4 = load %Lst*, %Lst** %lst, align 8
  %head5 = getelementptr %Lst, %Lst* %ref4, i32 0, i32 0
  store %Node* %ref3, %Node** %head5, align 8
  ret void
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %lst = alloca %Lst, align 8
  store %Lst zeroinitializer, %Lst* %lst, align 8
  %a = alloca %Node, align 8
  store %Node zeroinitializer, %Node* %a, align 8
  %content = getelementptr %Node, %Node* %a, i32 0, i32 0
  store i32 1, i32* %content, align 4
  %b = alloca %Node, align 8
  store %Node zeroinitializer, %Node* %b, align 8
  %content1 = getelementptr %Node, %Node* %b, i32 0, i32 0
  store i32 2, i32* %content1, align 4
  call void @push(%Lst* %lst, %Node* %a)
  call void @push(%Lst* %lst, %Node* %b)
  %cur = alloca %Node*, align 8
  %head = getelementptr %Lst, %Lst* %lst, i32 0, i32 0
  %ref = load %Node*, %Node** %head, align 8
  store %Node* %ref, %Node** %cur, align 8
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %ref2 = load %Node*, %Node** %cur, align 8
  %refcmp = icmp ne %Node* %ref2, null
  br i1 %refcmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %ref3 = load %Node*, %Node** %cur, align 8
  %content4 = getelementptr %Node, %Node* %ref3, i32 0, i32 0
  %content5 = load i32, i32* %content4, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %content5, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  %ref6 = load %Node*, %Node** %cur, align 8
  %next = getelementptr %Node, %Node* %ref6, i32 0, i32 1
  %ref7 = load %Node*, %Node** %next, align 8
  store %Node* %ref7, %Node** %cur, align 8
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.2, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 024 — ref? compared to null with == and !=, bound and unbound

```ura
struct N:
    v i32
    ref? nx N

main():
    a N
    a.v = 1
    ref? p N = ref a
    ref? q N = null
    output(p == null, " ", p != null, " ", q == null, "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct N
├─ v : i32
└─ nx : STRUCT_CALL

fn main() : i32
├─ a : STRUCT_CALL
├─ = : i32
│  ├─ .v : i32
│  │  └─ a : STRUCT_CALL
│  └─ int 1
├─ = : STRUCT_CALL
│  ├─ p : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ a : STRUCT_CALL
├─ = : STRUCT_CALL
│  ├─ q : STRUCT_CALL
│  └─ NULL_LIT : STRUCT_CALL
└─ output : void
   ├─ == : bool
   │  ├─ p : STRUCT_CALL
   │  └─ NULL_LIT : STRUCT_CALL
   ├─ char[] " "
   ├─ != : bool
   │  ├─ p : STRUCT_CALL
   │  └─ NULL_LIT : STRUCT_CALL
   ├─ char[] " "
   ├─ == : bool
   │  ├─ q : STRUCT_CALL
   │  └─ NULL_LIT : STRUCT_CALL
   └─ char[] "\n"
```

```out
False True True
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%N = type { i32, %N* }

@os = internal global %Os zeroinitializer
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@true_str.1 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.2 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c" \00", align 1
@true_str.4 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.5 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.6 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [19 x i8] c"%s%.*s%s%.*s%s%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %a = alloca %N, align 8
  store %N zeroinitializer, %N* %a, align 8
  %v = getelementptr %N, %N* %a, i32 0, i32 0
  store i32 1, i32* %v, align 4
  %p = alloca %N*, align 8
  store %N* %a, %N** %p, align 8
  %q = alloca %N*, align 8
  store %N* null, %N** %q, align 8
  %ref = load %N*, %N** %p, align 8
  %refcmp = icmp eq %N* %ref, null
  %bool_str = select i1 %refcmp, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %ref1 = load %N*, %N** %p, align 8
  %refcmp2 = icmp ne %N* %ref1, null
  %bool_str3 = select i1 %refcmp2, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.1, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.2, i32 0, i32 0)
  %ref4 = load %N*, %N** %q, align 8
  %refcmp5 = icmp eq %N* %ref4, null
  %bool_str6 = select i1 %refcmp5, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.4, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.5, i32 0, i32 0)
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @fmt, i32 0, i32 0), i8* %bool_str, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i8* %bool_str3, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0), i8* %bool_str6, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.6, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 025 — chained rebind removes the head: lst.head = ref lst.head.next

```ura
struct Node:
    v i32
    ref? next Node

struct Lst:
    ref? head Node

main():
    a Node
    a.v = 1
    b Node
    b.v = 2
    a.next = ref b
    lst Lst
    lst.head = ref a
    lst.head = ref lst.head.next
    output(lst.head.v, "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Node
├─ v : i32
└─ next : STRUCT_CALL

struct Lst
└─ head : STRUCT_CALL

fn main() : i32
├─ a : STRUCT_CALL
├─ = : i32
│  ├─ .v : i32
│  │  └─ a : STRUCT_CALL
│  └─ int 1
├─ b : STRUCT_CALL
├─ = : i32
│  ├─ .v : i32
│  │  └─ b : STRUCT_CALL
│  └─ int 2
├─ = : STRUCT_CALL
│  ├─ .next : STRUCT_CALL
│  │  └─ a : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ b : STRUCT_CALL
├─ lst : STRUCT_CALL
├─ = : STRUCT_CALL
│  ├─ .head : STRUCT_CALL
│  │  └─ lst : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ a : STRUCT_CALL
├─ = : STRUCT_CALL
│  ├─ .head : STRUCT_CALL
│  │  └─ lst : STRUCT_CALL
│  └─ ref : STRUCT_CALL
│     └─ .next : STRUCT_CALL
│        └─ .head : STRUCT_CALL
│           └─ lst : STRUCT_CALL
└─ output : void
   ├─ .v : i32
   │  └─ .head : STRUCT_CALL
   │     └─ lst : STRUCT_CALL
   └─ char[] "\n"
```

```out
2
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Node = type { i32, %Node* }
%Lst = type { %Node* }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %a = alloca %Node, align 8
  store %Node zeroinitializer, %Node* %a, align 8
  %v = getelementptr %Node, %Node* %a, i32 0, i32 0
  store i32 1, i32* %v, align 4
  %b = alloca %Node, align 8
  store %Node zeroinitializer, %Node* %b, align 8
  %v1 = getelementptr %Node, %Node* %b, i32 0, i32 0
  store i32 2, i32* %v1, align 4
  %next = getelementptr %Node, %Node* %a, i32 0, i32 1
  store %Node* %b, %Node** %next, align 8
  %lst = alloca %Lst, align 8
  store %Lst zeroinitializer, %Lst* %lst, align 8
  %head = getelementptr %Lst, %Lst* %lst, i32 0, i32 0
  store %Node* %a, %Node** %head, align 8
  %head2 = getelementptr %Lst, %Lst* %lst, i32 0, i32 0
  %ref = load %Node*, %Node** %head2, align 8
  %next3 = getelementptr %Node, %Node* %ref, i32 0, i32 1
  %ref4 = load %Node*, %Node** %next3, align 8
  %head5 = getelementptr %Lst, %Lst* %lst, i32 0, i32 0
  store %Node* %ref4, %Node** %head5, align 8
  %head6 = getelementptr %Lst, %Lst* %lst, i32 0, i32 0
  %ref7 = load %Node*, %Node** %head6, align 8
  %v8 = getelementptr %Node, %Node* %ref7, i32 0, i32 0
  %v9 = load i32, i32* %v8, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %v9, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 026 — declare a ref and bind it to a ref-returning call

```ura
struct Node:
    v i32
    ref? next Node

fn make(v i32, h Node[]) ref Node:
    h[0].v = v
    return ref h[0]

main():
    h Node[] = new Node[1]
    ref? n Node = make(9, h)
    output(n.v, "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Node
├─ v : i32
└─ next : STRUCT_CALL

fn make(v : i32, h : array) : STRUCT_CALL
├─ = : i32
│  ├─ .v : i32
│  │  └─ index : STRUCT_CALL
│  │     ├─ h : STRUCT_CALL[]
│  │     └─ int 0
│  └─ v : i32
└─ return
   └─ ref : STRUCT_CALL
      └─ index : STRUCT_CALL
         ├─ h : STRUCT_CALL[]
         └─ int 0

fn main() : i32
├─ = : array
│  ├─ h : STRUCT_CALL[]
│  └─ array : STRUCT_CALL[]
│     └─ int 1
├─ = : STRUCT_CALL
│  ├─ n : STRUCT_CALL
│  └─ call make : STRUCT_CALL
│     ├─ int 9
│     └─ h : STRUCT_CALL[]
└─ output : void
   ├─ .v : i32
   │  └─ n : STRUCT_CALL
   └─ char[] "\n"
```

```out
9
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Node = type { i32, %Node* }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define %Node* @make(i32 %0, { %Node*, i64 } %1) {
entry:
  %v = alloca i32, align 4
  store i32 %0, i32* %v, align 4
  %h = alloca { %Node*, i64 }, align 8
  store { %Node*, i64 } %1, { %Node*, i64 }* %h, align 8
  %h1 = load { %Node*, i64 }, { %Node*, i64 }* %h, align 8
  %arr.data = extractvalue { %Node*, i64 } %h1, 0
  %arr.at = getelementptr %Node, %Node* %arr.data, i32 0
  %v2 = getelementptr %Node, %Node* %arr.at, i32 0, i32 0
  %v3 = load i32, i32* %v, align 4
  store i32 %v3, i32* %v2, align 4
  %h4 = load { %Node*, i64 }, { %Node*, i64 }* %h, align 8
  %arr.data5 = extractvalue { %Node*, i64 } %h4, 0
  %arr.at6 = getelementptr %Node, %Node* %arr.data5, i32 0
  ret %Node* %arr.at6
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %h = alloca { %Node*, i64 }, align 8
  %heap = call i8* @calloc(i64 1, i64 16)
  %arr = bitcast i8* %heap to %Node*
  %arr.ptr = insertvalue { %Node*, i64 } undef, %Node* %arr, 0
  %arr.len = insertvalue { %Node*, i64 } %arr.ptr, i64 1, 1
  store { %Node*, i64 } %arr.len, { %Node*, i64 }* %h, align 8
  %n = alloca %Node*, align 8
  %h1 = load { %Node*, i64 }, { %Node*, i64 }* %h, align 8
  %call = call %Node* @make(i32 9, { %Node*, i64 } %h1)
  store %Node* %call, %Node** %n, align 8
  %ref = load %Node*, %Node** %n, align 8
  %v = getelementptr %Node, %Node* %ref, i32 0, i32 0
  %v2 = load i32, i32* %v, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %v2, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i32 @printf(i8*, ...)
```

## 027 — a ref-returning call passed straight into a ref parameter

```ura
struct Node:
    v i32
    ref? next Node

struct Lst:
    ref? head Node

fn make(v i32, h Node[]) ref Node:
    h[0].v = v
    return ref h[0]

fn sethead(ref lst Lst, ref node Node) void:
    lst.head = ref node

main():
    h Node[] = new Node[1]
    lst Lst
    sethead(ref lst, make(5, h))
    output(lst.head.v, "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Node
├─ v : i32
└─ next : STRUCT_CALL

struct Lst
└─ head : STRUCT_CALL

fn make(v : i32, h : array) : STRUCT_CALL
├─ = : i32
│  ├─ .v : i32
│  │  └─ index : STRUCT_CALL
│  │     ├─ h : STRUCT_CALL[]
│  │     └─ int 0
│  └─ v : i32
└─ return
   └─ ref : STRUCT_CALL
      └─ index : STRUCT_CALL
         ├─ h : STRUCT_CALL[]
         └─ int 0

fn sethead(lst : STRUCT_CALL, node : STRUCT_CALL) : void
└─ = : STRUCT_CALL
   ├─ .head : STRUCT_CALL
   │  └─ lst : STRUCT_CALL
   └─ ref : STRUCT_CALL
      └─ node : STRUCT_CALL

fn main() : i32
├─ = : array
│  ├─ h : STRUCT_CALL[]
│  └─ array : STRUCT_CALL[]
│     └─ int 1
├─ lst : STRUCT_CALL
├─ call sethead : void
│  ├─ ref : STRUCT_CALL
│  │  └─ lst : STRUCT_CALL
│  └─ call make : STRUCT_CALL
│     ├─ int 5
│     └─ h : STRUCT_CALL[]
└─ output : void
   ├─ .v : i32
   │  └─ .head : STRUCT_CALL
   │     └─ lst : STRUCT_CALL
   └─ char[] "\n"
```

```out
5
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Node = type { i32, %Node* }
%Lst = type { %Node* }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define %Node* @make(i32 %0, { %Node*, i64 } %1) {
entry:
  %v = alloca i32, align 4
  store i32 %0, i32* %v, align 4
  %h = alloca { %Node*, i64 }, align 8
  store { %Node*, i64 } %1, { %Node*, i64 }* %h, align 8
  %h1 = load { %Node*, i64 }, { %Node*, i64 }* %h, align 8
  %arr.data = extractvalue { %Node*, i64 } %h1, 0
  %arr.at = getelementptr %Node, %Node* %arr.data, i32 0
  %v2 = getelementptr %Node, %Node* %arr.at, i32 0, i32 0
  %v3 = load i32, i32* %v, align 4
  store i32 %v3, i32* %v2, align 4
  %h4 = load { %Node*, i64 }, { %Node*, i64 }* %h, align 8
  %arr.data5 = extractvalue { %Node*, i64 } %h4, 0
  %arr.at6 = getelementptr %Node, %Node* %arr.data5, i32 0
  ret %Node* %arr.at6
}

define void @sethead(%Lst* %0, %Node* %1) {
entry:
  %lst = alloca %Lst*, align 8
  store %Lst* %0, %Lst** %lst, align 8
  %node = alloca %Node*, align 8
  store %Node* %1, %Node** %node, align 8
  %ref = load %Node*, %Node** %node, align 8
  %ref1 = load %Lst*, %Lst** %lst, align 8
  %head = getelementptr %Lst, %Lst* %ref1, i32 0, i32 0
  store %Node* %ref, %Node** %head, align 8
  ret void
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %h = alloca { %Node*, i64 }, align 8
  %heap = call i8* @calloc(i64 1, i64 16)
  %arr = bitcast i8* %heap to %Node*
  %arr.ptr = insertvalue { %Node*, i64 } undef, %Node* %arr, 0
  %arr.len = insertvalue { %Node*, i64 } %arr.ptr, i64 1, 1
  store { %Node*, i64 } %arr.len, { %Node*, i64 }* %h, align 8
  %lst = alloca %Lst, align 8
  store %Lst zeroinitializer, %Lst* %lst, align 8
  %h1 = load { %Node*, i64 }, { %Node*, i64 }* %h, align 8
  %call = call %Node* @make(i32 5, { %Node*, i64 } %h1)
  call void @sethead(%Lst* %lst, %Node* %call)
  %head = getelementptr %Lst, %Lst* %lst, i32 0, i32 0
  %ref = load %Node*, %Node** %head, align 8
  %v = getelementptr %Node, %Node* %ref, i32 0, i32 0
  %v2 = load i32, i32* %v, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %v2, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i32 @printf(i8*, ...)
```

## 028 — = ref rebinds the pointer, = value writes through the referent

```ura
main():
    x i32 = 10
    y i32 = 20
    ref cur i32 = ref x
    cur = 99
    output(x, " ")
    cur = ref y
    cur = 88
    output(x, " ", y, "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

fn main() : i32
├─ = : i32
│  ├─ x : i32
│  └─ int 10
├─ = : i32
│  ├─ y : i32
│  └─ int 20
├─ = : i32
│  ├─ cur : i32
│  └─ ref : i32
│     └─ x : i32
├─ = : i32
│  ├─ cur : i32
│  └─ int 99
├─ output : void
│  ├─ x : i32
│  └─ char[] " "
├─ = : i32
│  ├─ cur : i32
│  └─ ref : i32
│     └─ y : i32
├─ = : i32
│  ├─ cur : i32
│  └─ int 88
└─ output : void
   ├─ x : i32
   ├─ char[] " "
   ├─ y : i32
   └─ char[] "\n"
```

```out
99 99 88
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1
@str.1 = private unnamed_addr constant [2 x i8] c" \00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.3 = private unnamed_addr constant [13 x i8] c"%d%.*s%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %x = alloca i32, align 4
  store i32 10, i32* %x, align 4
  %y = alloca i32, align 4
  store i32 20, i32* %y, align 4
  %cur = alloca i32*, align 8
  store i32* %x, i32** %cur, align 8
  %ref = load i32*, i32** %cur, align 8
  store i32 99, i32* %ref, align 4
  %x1 = load i32, i32* %x, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %x1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  store i32* %y, i32** %cur, align 8
  %ref2 = load i32*, i32** %cur, align 8
  store i32 88, i32* %ref2, align 4
  %x3 = load i32, i32* %x, align 4
  %y4 = load i32, i32* %y, align 4
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt.3, i32 0, i32 0), i32 %x3, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.1, i32 0, i32 0), i32 %y4, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 029 — ref a == ref a is identity true, ref a == ref b is false

```ura
struct N:
    v i32

main():
    a N
    b N
    output(ref a == ref a, " ", ref a == ref b, "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct N
└─ v : i32

fn main() : i32
├─ a : STRUCT_CALL
├─ b : STRUCT_CALL
└─ output : void
   ├─ == : bool
   │  ├─ ref : STRUCT_CALL
   │  │  └─ a : STRUCT_CALL
   │  └─ ref : STRUCT_CALL
   │     └─ a : STRUCT_CALL
   ├─ char[] " "
   ├─ == : bool
   │  ├─ ref : STRUCT_CALL
   │  │  └─ a : STRUCT_CALL
   │  └─ ref : STRUCT_CALL
   │     └─ b : STRUCT_CALL
   └─ char[] "\n"
```

```out
True False
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%N = type { i32 }

@os = internal global %Os zeroinitializer
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str = private unnamed_addr constant [2 x i8] c" \00", align 1
@true_str.1 = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str.2 = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.3 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [13 x i8] c"%s%.*s%s%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define i32 @main(i32 %0, i8** %1) {
entry:
  %a = alloca %N, align 8
  store %N zeroinitializer, %N* %a, align 4
  %b = alloca %N, align 8
  store %N zeroinitializer, %N* %b, align 4
  %refcmp = icmp eq %N* %a, %a
  %bool_str = select i1 %refcmp, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %refcmp1 = icmp eq %N* %a, %b
  %bool_str2 = select i1 %refcmp1, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str.1, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str.2, i32 0, i32 0)
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @fmt, i32 0, i32 0), i8* %bool_str, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0), i8* %bool_str2, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.3, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## 030 — a value function cannot return a reference

```ura
fn f() i32:
    x i32 = 5
    return ref x

main():
    output(f(), "\n")
```

```tree
```

```out
```

```err
error: 'f' returns a value, not a reference; drop the `ref`
  030.ura:3:5
  |
3 |     return ref x
  |     ^^^^^^
```

```ll
```

## 031 — a reference function must return a reference, not a bare value

```ura
struct N:
    v i32

fn g(h N[]) ref N:
    return h[0]

main():
    h N[] = new N[1]
    output(g(h).v, "\n")
```

```tree
```

```out
```

```err
error: 'g' returns a reference; return `ref x` (or null), not a value
  031.ura:5:5
  |
5 |     return h[0]
  |     ^^^^^^
```

```ll
```

## 032 — a ref declaration cannot be bound to a plain value

```ura
main():
    ref? m i32 = 5
    output(m, "\n")
```

```tree
```

```out
```

```err
error: A reference must be bound to a variable (ref x)
  032.ura:2:16
  |
2 |     ref? m i32 = 5
  |                ^
```

```ll
```

## 033 — a non-nullable value cannot be compared to null

```ura
main():
    x i32 = 5
    output(x == null, "\n")
```

```tree
```

```out
```

```err
error: Only arrays and pointers can be null, not i32
  033.ura:3:17
  |
3 |     output(x == null, "\n")
  |                 ^^^^
```

```ll
```

## 034 — a field read chained directly on a ref-returning call

```ura
struct Node:
    value i32
    ref? next Node

fn make(v i32, heap Node[]) ref Node:
    heap[0].value = v
    return ref heap[0]

main():
    h Node[] = new Node[1]
    output(make(42, h).value, "\n")
```

```tree
proto fn printf(format : pointer, ...) : i32

proto fn dprintf(fd : i32, format : pointer, ...) : i32

proto fn calloc(len : i64, size : i64) : pointer

proto fn free(ptr : pointer) : void

proto fn write(fd : i32, ptr : pointer, len : i64) : i64

proto fn exit(code : i32) : void

proto fn strlen(s : pointer) : i64

proto fn getenv(name : pointer) : pointer

struct Os
├─ argc : i32
├─ argv : char[][]
└─ fn Os.get(self : STRUCT_CALL, name : array) : pointer
   └─ return
      └─ call getenv : pointer
         └─ name : char[]

os : STRUCT_CALL

struct Node
├─ value : i32
└─ next : STRUCT_CALL

fn make(v : i32, heap : array) : STRUCT_CALL
├─ = : i32
│  ├─ .value : i32
│  │  └─ index : STRUCT_CALL
│  │     ├─ heap : STRUCT_CALL[]
│  │     └─ int 0
│  └─ v : i32
└─ return
   └─ ref : STRUCT_CALL
      └─ index : STRUCT_CALL
         ├─ heap : STRUCT_CALL[]
         └─ int 0

fn main() : i32
├─ = : array
│  ├─ h : STRUCT_CALL[]
│  └─ array : STRUCT_CALL[]
│     └─ int 1
└─ output : void
   ├─ .value : i32
   │  └─ call make : STRUCT_CALL
   │     ├─ int 42
   │     └─ h : STRUCT_CALL[]
   └─ char[] "\n"
```

```out
42
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Node = type { i32, %Node* }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [7 x i8] c"%d%.*s\00", align 1

define i8* @Os.get(%Os* %0, { i8*, i64 } %1) {
entry:
  %self = alloca %Os*, align 8
  store %Os* %0, %Os** %self, align 8
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %1, { i8*, i64 }* %name, align 8
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  %arr.data = extractvalue { i8*, i64 } %name1, 0
  %call = call i8* @getenv(i8* %arr.data)
  ret i8* %call
}

declare i8* @getenv(i8*)

define %Node* @make(i32 %0, { %Node*, i64 } %1) {
entry:
  %v = alloca i32, align 4
  store i32 %0, i32* %v, align 4
  %heap = alloca { %Node*, i64 }, align 8
  store { %Node*, i64 } %1, { %Node*, i64 }* %heap, align 8
  %heap1 = load { %Node*, i64 }, { %Node*, i64 }* %heap, align 8
  %arr.data = extractvalue { %Node*, i64 } %heap1, 0
  %arr.at = getelementptr %Node, %Node* %arr.data, i32 0
  %value = getelementptr %Node, %Node* %arr.at, i32 0, i32 0
  %v2 = load i32, i32* %v, align 4
  store i32 %v2, i32* %value, align 4
  %heap3 = load { %Node*, i64 }, { %Node*, i64 }* %heap, align 8
  %arr.data4 = extractvalue { %Node*, i64 } %heap3, 0
  %arr.at5 = getelementptr %Node, %Node* %arr.data4, i32 0
  ret %Node* %arr.at5
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %h = alloca { %Node*, i64 }, align 8
  %heap = call i8* @calloc(i64 1, i64 16)
  %arr = bitcast i8* %heap to %Node*
  %arr.ptr = insertvalue { %Node*, i64 } undef, %Node* %arr, 0
  %arr.len = insertvalue { %Node*, i64 } %arr.ptr, i64 1, 1
  store { %Node*, i64 } %arr.len, { %Node*, i64 }* %h, align 8
  %h1 = load { %Node*, i64 }, { %Node*, i64 }* %h, align 8
  %call = call %Node* @make(i32 42, { %Node*, i64 } %h1)
  %value = getelementptr %Node, %Node* %call, i32 0, i32 0
  %value2 = load i32, i32* %value, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %value2, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i8* @calloc(i64, i64)

declare i32 @printf(i8*, ...)
```
