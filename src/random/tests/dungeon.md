# dungeon

## index

- floor04
- floor09b
- floor14a
- boss

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

%Os = type { i32, { { i8*, i64 }*, i64 } }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [11 x i8] c"Orc takes \00", align 1
@str.1 = private unnamed_addr constant [19 x i8] c" damage \E2\80\94 dead: \00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.2 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt = private unnamed_addr constant [17 x i8] c"%.*s%d%.*s%s%.*s\00", align 1
@str.3 = private unnamed_addr constant [12 x i8] c"4 squared: \00", align 1
@str.4 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.5 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1

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

define i32 @main(i32 %0, i8** %1) {
entry:
  %d = alloca i32, align 4
  %call = call i32 @damage(i32 25, i32 8)
  store i32 %call, i32* %d, align 4
  %d1 = load i32, i32* %d, align 4
  %d2 = load i32, i32* %d, align 4
  %sub = sub i32 %d2, 60
  %call3 = call i1 @is_dead(i32 %sub)
  %bool_str = select i1 %call3, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @fmt, i32 0, i32 0), i32 10, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str, i32 0, i32 0), i32 %d1, i32 18, i8* getelementptr inbounds ([19 x i8], [19 x i8]* @str.1, i32 0, i32 0), i8* %bool_str, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.2, i32 0, i32 0))
  %call4 = call i32 @square(i32 4)
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.5, i32 0, i32 0), i32 11, i8* getelementptr inbounds ([12 x i8], [12 x i8]* @str.3, i32 0, i32 0), i32 %call4, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.4, i32 0, i32 0))
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
  %hp = alloca i32, align 4
  store i32 80, i32* %hp, align 4
  %r = alloca i32*, align 8
  store i32* %hp, i32** %r, align 8
  %ref = load i32*, i32** %r, align 8
  store i32 100, i32* %ref, align 4
  %hp1 = load i32, i32* %hp, align 4
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([7 x i8], [7 x i8]* @fmt, i32 0, i32 0), i32 %hp1, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str, i32 0, i32 0))
  ret i32 0
}

declare i32 @printf(i8*, ...)
```

## floor14a

```ura
// dungeon/floor14a.ura
proto fn strlen(s pointer) i64
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
   ura-lib/common.ura:23:7
   |
23 | proto fn write(fd i32, ptr pointer, len i64) i64
   |       ^^
error: Cannot assign i64 to i32
  floor14a.ura:7:11
  |
7 |     n i32 = strlen("dungeon")
  |           ^
```

```ll
```

## boss

```ura
enum EnemyType: GOBLIN, ORC, DRAGON

struct Stats:
    hp  i32
    atk i32

struct Hero:
    name  char[]
    stats Stats

    pub fn create(name char[], hp i32, atk i32) Hero:
        h Hero
        h.name       = name
        h.stats.hp   = hp
        h.stats.atk  = atk
        return h

    fn alive() bool:
        return self.stats.hp > 0

    operator output() char[]:
        return self.name

fn enemy_hp(kind EnemyType) i32:
    match kind:
        case GOBLIN: return 20
        case ORC:    return 45
        case DRAGON: return 120
    return 0

fn loot_for(kind EnemyType) char[]?:
    match kind:
        case DRAGON: return "Dragonfang"
        default:     return null
    return null

fn strongest(party Hero[], n i32) ref Hero:
    best i32 = 0
    for i in 1..n:
        if party[i].stats.atk > party[best].stats.atk:
            best = i
    return ref party[best]

main():
    @if macos:
        output("== Dungeon (macOS build) ==\n")
    @elif linux:
        output("== Dungeon (Linux build) ==\n")
    @else:
        output("== Dungeon ==\n")

    party Hero[] = new Hero[3]
    party[0] = Hero::create("Aldric", 100, 18)
    party[1] = Hero::create("Borin",  90, 25)
    party[2] = Hero::create("Cass",   80, 22)

    output("Party:\n")
    for h in party:
        output("  ", h, " atk=", h.stats.atk, "\n")

    ref champ Hero = strongest(party, 3)
    output("Champion: ", champ, " (alive=", champ.alive(), ")\n")

    foes EnemyType[] = [GOBLIN, ORC, DRAGON]
    for kind in foes:
        loot char[]? = loot_for(kind)
        output("Foe hp=", enemy_hp(kind), " loot=",
               loot ?? "none", "\n")

    for ref h in party:
        h.stats.hp = h.stats.hp - 10
    output("Trap! ", party[0], " hp=", party[0].stats.hp, "\n")

    output("sizeof=", sizeof(party[0]), " typeof=",
           typeof(champ), "\n")

    clean party
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

ENUM_DEF
├─ ENUM_CALL
├─ ENUM_CALL
└─ ENUM_CALL

struct Stats
├─ hp : i32
└─ atk : i32

struct Hero
├─ name : char[]
├─ stats : STRUCT_CALL
├─ fn Hero.create(name : array, hp : i32, atk : i32) : STRUCT_CALL
│  ├─ h : STRUCT_CALL
│  ├─ = : array
│  │  ├─ .name : char[]
│  │  │  └─ h : STRUCT_CALL
│  │  └─ name : char[]
│  ├─ = : i32
│  │  ├─ .hp : i32
│  │  │  └─ .stats : STRUCT_CALL
│  │  │     └─ h : STRUCT_CALL
│  │  └─ hp : i32
│  ├─ = : i32
│  │  ├─ .atk : i32
│  │  │  └─ .stats : STRUCT_CALL
│  │  │     └─ h : STRUCT_CALL
│  │  └─ atk : i32
│  └─ return
│     └─ h : STRUCT_CALL
├─ fn Hero.alive(self : STRUCT_CALL) : bool
│  └─ return
│     └─ > : bool
│        ├─ .hp : i32
│        │  └─ .stats : STRUCT_CALL
│        │     └─ self : STRUCT_CALL
│        └─ int 0
└─ fn Hero.output(self : STRUCT_CALL) : char[]
   └─ return
      └─ .name : char[]
         └─ self : STRUCT_CALL

fn enemy_hp(kind : i32) : i32
├─ match
│  ├─ subject kind : i32
│  ├─ case
│  │  ├─ value int 0
│  │  └─ return
│  │     └─ int 20
│  ├─ case
│  │  ├─ value int 1
│  │  └─ return
│  │     └─ int 45
│  └─ case
│     ├─ value int 2
│     └─ return
│        └─ int 120
└─ return
   └─ int 0

fn loot_for(kind : i32) : char[]
├─ match
│  ├─ subject kind : i32
│  ├─ case
│  │  ├─ value int 2
│  │  └─ return
│  │     └─ char[] "Dragonfang"
│  └─ default
│     └─ return
│        └─ NULL_LIT : char[]
└─ return
   └─ NULL_LIT : char[]

fn strongest(party : array, n : i32) : STRUCT_CALL
├─ = : i32
│  ├─ best : i32
│  └─ int 0
├─ for
│  ├─ i : i32
│  ├─ range : i32
│  │  ├─ int 1
│  │  └─ n : i32
│  └─ if
│     ├─ condition > : bool
│     │  ├─ .atk : i32
│     │  │  └─ .stats : STRUCT_CALL
│     │  │     └─ index : STRUCT_CALL
│     │  │        ├─ party : STRUCT_CALL[]
│     │  │        └─ i : i32
│     │  └─ .atk : i32
│     │     └─ .stats : STRUCT_CALL
│     │        └─ index : STRUCT_CALL
│     │           ├─ party : STRUCT_CALL[]
│     │           └─ best : i32
│     └─ = : i32
│        ├─ best : i32
│        └─ i : i32
└─ return
   └─ ref : STRUCT_CALL
      └─ index : STRUCT_CALL
         ├─ party : STRUCT_CALL[]
         └─ best : i32

fn main() : i32
├─ output : void
│  └─ char[] "== Dungeon (macOS build) ==\n"
├─ = : array
│  ├─ party : STRUCT_CALL[]
│  └─ array : STRUCT_CALL[]
│     └─ int 3
├─ = : STRUCT_CALL
│  ├─ index : STRUCT_CALL
│  │  ├─ party : STRUCT_CALL[]
│  │  └─ int 0
│  └─ call create : STRUCT_CALL
│     ├─ char[] "Aldric"
│     ├─ int 100
│     └─ int 18
├─ = : STRUCT_CALL
│  ├─ index : STRUCT_CALL
│  │  ├─ party : STRUCT_CALL[]
│  │  └─ int 1
│  └─ call create : STRUCT_CALL
│     ├─ char[] "Borin"
│     ├─ int 90
│     └─ int 25
├─ = : STRUCT_CALL
│  ├─ index : STRUCT_CALL
│  │  ├─ party : STRUCT_CALL[]
│  │  └─ int 2
│  └─ call create : STRUCT_CALL
│     ├─ char[] "Cass"
│     ├─ int 80
│     └─ int 22
├─ output : void
│  └─ char[] "Party:\n"
├─ for
│  ├─ h : STRUCT_CALL
│  ├─ party : STRUCT_CALL[]
│  └─ output : void
│     ├─ char[] "  "
│     ├─ h : STRUCT_CALL
│     ├─ char[] " atk="
│     ├─ .atk : i32
│     │  └─ .stats : STRUCT_CALL
│     │     └─ h : STRUCT_CALL
│     └─ char[] "\n"
├─ = : STRUCT_CALL
│  ├─ champ : STRUCT_CALL
│  └─ call strongest : STRUCT_CALL
│     ├─ party : STRUCT_CALL[]
│     └─ int 3
├─ output : void
│  ├─ char[] "Champion: "
│  ├─ champ : STRUCT_CALL
│  ├─ char[] " (alive="
│  ├─ call alive : bool
│  │  └─ champ : STRUCT_CALL
│  └─ char[] ")\n"
├─ = : array
│  ├─ foes : i32[]
│  └─ array : i32[]
│     ├─ int 0
│     ├─ int 1
│     └─ int 2
├─ for
│  ├─ kind : i32
│  ├─ foes : i32[]
│  ├─ = : array
│  │  ├─ loot : char[]
│  │  └─ call loot_for : char[]
│  │     └─ kind : i32
│  └─ output : void
│     ├─ char[] "Foe hp="
│     ├─ call enemy_hp : i32
│     │  └─ kind : i32
│     ├─ char[] " loot="
│     ├─ FALLBACK : char[]
│     │  ├─ loot : char[]
│     │  └─ char[] "none"
│     └─ char[] "\n"
├─ for
│  ├─ h : STRUCT_CALL
│  ├─ party : STRUCT_CALL[]
│  └─ = : i32
│     ├─ .hp : i32
│     │  └─ .stats : STRUCT_CALL
│     │     └─ h : STRUCT_CALL
│     └─ - : i32
│        ├─ .hp : i32
│        │  └─ .stats : STRUCT_CALL
│        │     └─ h : STRUCT_CALL
│        └─ int 10
├─ output : void
│  ├─ char[] "Trap! "
│  ├─ index : STRUCT_CALL
│  │  ├─ party : STRUCT_CALL[]
│  │  └─ int 0
│  ├─ char[] " hp="
│  ├─ .hp : i32
│  │  └─ .stats : STRUCT_CALL
│  │     └─ index : STRUCT_CALL
│  │        ├─ party : STRUCT_CALL[]
│  │        └─ int 0
│  └─ char[] "\n"
├─ output : void
│  ├─ char[] "sizeof="
│  ├─ sizeof : u64
│  │  └─ index : STRUCT_CALL
│  │     ├─ party : STRUCT_CALL[]
│  │     └─ int 0
│  ├─ char[] " typeof="
│  ├─ typeof : char[]
│  │  └─ champ : STRUCT_CALL
│  └─ char[] "\n"
└─ clean : void
   └─ party : STRUCT_CALL[]
```

```out
== Dungeon (macOS build) ==
Party:
  Aldric atk=18
  Borin atk=25
  Cass atk=22
Champion: Borin (alive=True)
Foe hp=20 loot=none
Foe hp=45 loot=none
Foe hp=120 loot=Dragonfang
Trap! Aldric hp=90
sizeof=24 typeof=Hero
```

```err
```

```ll

%Os = type { i32, { { i8*, i64 }*, i64 } }
%Hero = type { { i8*, i64 }, %Stats }
%Stats = type { i32, i32 }

@os = internal global %Os zeroinitializer
@str = private unnamed_addr constant [11 x i8] c"Dragonfang\00", align 1
@str.1 = private unnamed_addr constant [29 x i8] c"== Dungeon (macOS build) ==\0A\00", align 1
@fmt = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.2 = private unnamed_addr constant [7 x i8] c"Aldric\00", align 1
@str.3 = private unnamed_addr constant [6 x i8] c"Borin\00", align 1
@str.4 = private unnamed_addr constant [5 x i8] c"Cass\00", align 1
@str.5 = private unnamed_addr constant [8 x i8] c"Party:\0A\00", align 1
@fmt.6 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.7 = private unnamed_addr constant [3 x i8] c"  \00", align 1
@fmt.8 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.9 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.10 = private unnamed_addr constant [6 x i8] c" atk=\00", align 1
@str.11 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.12 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.13 = private unnamed_addr constant [11 x i8] c"Champion: \00", align 1
@fmt.14 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.15 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.16 = private unnamed_addr constant [9 x i8] c" (alive=\00", align 1
@true_str = private unnamed_addr constant [5 x i8] c"True\00", align 1
@false_str = private unnamed_addr constant [6 x i8] c"False\00", align 1
@str.17 = private unnamed_addr constant [3 x i8] c")\0A\00", align 1
@fmt.18 = private unnamed_addr constant [11 x i8] c"%.*s%s%.*s\00", align 1
@str.19 = private unnamed_addr constant [8 x i8] c"Foe hp=\00", align 1
@str.20 = private unnamed_addr constant [7 x i8] c" loot=\00", align 1
@str.21 = private unnamed_addr constant [5 x i8] c"none\00", align 1
@str.22 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.23 = private unnamed_addr constant [19 x i8] c"%.*s%d%.*s%.*s%.*s\00", align 1
@str.24 = private unnamed_addr constant [7 x i8] c"Trap! \00", align 1
@fmt.25 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@fmt.26 = private unnamed_addr constant [5 x i8] c"%.*s\00", align 1
@str.27 = private unnamed_addr constant [5 x i8] c" hp=\00", align 1
@str.28 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.29 = private unnamed_addr constant [11 x i8] c"%.*s%d%.*s\00", align 1
@str.30 = private unnamed_addr constant [8 x i8] c"sizeof=\00", align 1
@str.31 = private unnamed_addr constant [9 x i8] c" typeof=\00", align 1
@str.32 = private unnamed_addr constant [5 x i8] c"Hero\00", align 1
@str.33 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@fmt.34 = private unnamed_addr constant [21 x i8] c"%.*s%llu%.*s%.*s%.*s\00", align 1

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

define %Hero @Hero.create({ i8*, i64 } %0, i32 %1, i32 %2) {
entry:
  %name = alloca { i8*, i64 }, align 8
  store { i8*, i64 } %0, { i8*, i64 }* %name, align 8
  %hp = alloca i32, align 4
  store i32 %1, i32* %hp, align 4
  %atk = alloca i32, align 4
  store i32 %2, i32* %atk, align 4
  %h = alloca %Hero, align 8
  store %Hero zeroinitializer, %Hero* %h, align 8
  %name1 = getelementptr %Hero, %Hero* %h, i32 0, i32 0
  %name2 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  store { i8*, i64 } %name2, { i8*, i64 }* %name1, align 8
  %stats = getelementptr %Hero, %Hero* %h, i32 0, i32 1
  %hp3 = getelementptr %Stats, %Stats* %stats, i32 0, i32 0
  %hp4 = load i32, i32* %hp, align 4
  store i32 %hp4, i32* %hp3, align 4
  %stats5 = getelementptr %Hero, %Hero* %h, i32 0, i32 1
  %atk6 = getelementptr %Stats, %Stats* %stats5, i32 0, i32 1
  %atk7 = load i32, i32* %atk, align 4
  store i32 %atk7, i32* %atk6, align 4
  %h8 = load %Hero, %Hero* %h, align 8
  ret %Hero %h8
}

define i1 @Hero.alive(%Hero* %0) {
entry:
  %self = alloca %Hero*, align 8
  store %Hero* %0, %Hero** %self, align 8
  %ref = load %Hero*, %Hero** %self, align 8
  %stats = getelementptr %Hero, %Hero* %ref, i32 0, i32 1
  %hp = getelementptr %Stats, %Stats* %stats, i32 0, i32 0
  %hp1 = load i32, i32* %hp, align 4
  %gt = icmp sgt i32 %hp1, 0
  ret i1 %gt
}

define { i8*, i64 } @Hero.output(%Hero* %0) {
entry:
  %self = alloca %Hero*, align 8
  store %Hero* %0, %Hero** %self, align 8
  %ref = load %Hero*, %Hero** %self, align 8
  %name = getelementptr %Hero, %Hero* %ref, i32 0, i32 0
  %name1 = load { i8*, i64 }, { i8*, i64 }* %name, align 8
  ret { i8*, i64 } %name1
}

define i32 @enemy_hp(i32 %0) {
entry:
  %kind = alloca i32, align 4
  store i32 %0, i32* %kind, align 4
  %kind1 = load i32, i32* %kind, align 4
  %eq = icmp eq i32 %kind1, 0
  br i1 %eq, label %case.body, label %case.next

match.end:                                        ; preds = %case.next3
  ret i32 0

case.body:                                        ; preds = %entry
  ret i32 20

case.next:                                        ; preds = %entry
  %eq4 = icmp eq i32 %kind1, 1
  br i1 %eq4, label %case.body2, label %case.next3

case.body2:                                       ; preds = %case.next
  ret i32 45

case.next3:                                       ; preds = %case.next
  %eq6 = icmp eq i32 %kind1, 2
  br i1 %eq6, label %case.body5, label %match.end

case.body5:                                       ; preds = %case.next3
  ret i32 120
}

define { i8*, i64 } @loot_for(i32 %0) {
entry:
  %kind = alloca i32, align 4
  store i32 %0, i32* %kind, align 4
  %kind1 = load i32, i32* %kind, align 4
  %eq = icmp eq i32 %kind1, 2
  br i1 %eq, label %case.body, label %case.next

match.end:                                        ; No predecessors!
  ret { i8*, i64 } zeroinitializer

case.body:                                        ; preds = %entry
  ret { i8*, i64 } { i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str, i32 0, i32 0), i64 10 }

case.next:                                        ; preds = %entry
  ret { i8*, i64 } zeroinitializer
}

define %Hero* @strongest({ %Hero*, i64 } %0, i32 %1) {
entry:
  %party = alloca { %Hero*, i64 }, align 8
  store { %Hero*, i64 } %0, { %Hero*, i64 }* %party, align 8
  %n = alloca i32, align 4
  store i32 %1, i32* %n, align 4
  %best = alloca i32, align 4
  store i32 0, i32* %best, align 4
  %n1 = load i32, i32* %n, align 4
  %asc = icmp slt i32 1, %n1
  %step = select i1 %asc, i32 1, i32 -1
  %i = alloca i32, align 4
  store i32 1, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i2 = load i32, i32* %i, align 4
  %lt = icmp slt i32 %i2, %n1
  %gt = icmp sgt i32 %i2, %n1
  %more = select i1 %asc, i1 %lt, i1 %gt
  br i1 %more, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %party3 = load { %Hero*, i64 }, { %Hero*, i64 }* %party, align 8
  %arr.data = extractvalue { %Hero*, i64 } %party3, 0
  %i4 = load i32, i32* %i, align 4
  %arr.at = getelementptr %Hero, %Hero* %arr.data, i32 %i4
  %stats = getelementptr %Hero, %Hero* %arr.at, i32 0, i32 1
  %atk = getelementptr %Stats, %Stats* %stats, i32 0, i32 1
  %atk5 = load i32, i32* %atk, align 4
  %party6 = load { %Hero*, i64 }, { %Hero*, i64 }* %party, align 8
  %arr.data7 = extractvalue { %Hero*, i64 } %party6, 0
  %best8 = load i32, i32* %best, align 4
  %arr.at9 = getelementptr %Hero, %Hero* %arr.data7, i32 %best8
  %stats10 = getelementptr %Hero, %Hero* %arr.at9, i32 0, i32 1
  %atk11 = getelementptr %Stats, %Stats* %stats10, i32 0, i32 1
  %atk12 = load i32, i32* %atk11, align 4
  %gt13 = icmp sgt i32 %atk5, %atk12
  br i1 %gt13, label %then, label %endif

for.inc:                                          ; preds = %endif
  %i15 = load i32, i32* %i, align 4
  %next = add i32 %i15, %step
  store i32 %next, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %party16 = load { %Hero*, i64 }, { %Hero*, i64 }* %party, align 8
  %arr.data17 = extractvalue { %Hero*, i64 } %party16, 0
  %best18 = load i32, i32* %best, align 4
  %arr.at19 = getelementptr %Hero, %Hero* %arr.data17, i32 %best18
  ret %Hero* %arr.at19

endif:                                            ; preds = %then, %for.body
  br label %for.inc

then:                                             ; preds = %for.body
  %i14 = load i32, i32* %i, align 4
  store i32 %i14, i32* %best, align 4
  br label %endif
}

define i32 @main(i32 %0, i8** %1) {
entry:
  %2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt, i32 0, i32 0), i32 28, i8* getelementptr inbounds ([29 x i8], [29 x i8]* @str.1, i32 0, i32 0))
  %party = alloca { %Hero*, i64 }, align 8
  %heap = call i8* @calloc(i64 3, i64 24)
  %arr = bitcast i8* %heap to %Hero*
  %arr.ptr = insertvalue { %Hero*, i64 } undef, %Hero* %arr, 0
  %arr.len = insertvalue { %Hero*, i64 } %arr.ptr, i64 3, 1
  store { %Hero*, i64 } %arr.len, { %Hero*, i64 }* %party, align 8
  %party1 = load { %Hero*, i64 }, { %Hero*, i64 }* %party, align 8
  %arr.data = extractvalue { %Hero*, i64 } %party1, 0
  %arr.at = getelementptr %Hero, %Hero* %arr.data, i32 0
  %call = call %Hero @Hero.create({ i8*, i64 } { i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.2, i32 0, i32 0), i64 6 }, i32 100, i32 18)
  store %Hero %call, %Hero* %arr.at, align 8
  %party2 = load { %Hero*, i64 }, { %Hero*, i64 }* %party, align 8
  %arr.data3 = extractvalue { %Hero*, i64 } %party2, 0
  %arr.at4 = getelementptr %Hero, %Hero* %arr.data3, i32 1
  %call5 = call %Hero @Hero.create({ i8*, i64 } { i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.3, i32 0, i32 0), i64 5 }, i32 90, i32 25)
  store %Hero %call5, %Hero* %arr.at4, align 8
  %party6 = load { %Hero*, i64 }, { %Hero*, i64 }* %party, align 8
  %arr.data7 = extractvalue { %Hero*, i64 } %party6, 0
  %arr.at8 = getelementptr %Hero, %Hero* %arr.data7, i32 2
  %call9 = call %Hero @Hero.create({ i8*, i64 } { i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.4, i32 0, i32 0), i64 4 }, i32 80, i32 22)
  store %Hero %call9, %Hero* %arr.at8, align 8
  %3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.6, i32 0, i32 0), i32 7, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.5, i32 0, i32 0))
  %party10 = load { %Hero*, i64 }, { %Hero*, i64 }* %party, align 8
  %arr.data11 = extractvalue { %Hero*, i64 } %party10, 0
  %arr.len12 = extractvalue { %Hero*, i64 } %party10, 1
  %idx = alloca i64, align 8
  store i64 0, i64* %idx, align 8
  %h = alloca %Hero, align 8
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i = load i64, i64* %idx, align 8
  %more = icmp slt i64 %i, %arr.len12
  br i1 %more, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %elem = getelementptr %Hero, %Hero* %arr.data11, i64 %i
  %x = load %Hero, %Hero* %elem, align 8
  store %Hero %x, %Hero* %h, align 8
  %4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.8, i32 0, i32 0), i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.7, i32 0, i32 0))
  %out = call { i8*, i64 } @Hero.output(%Hero* %h)
  %out.len = extractvalue { i8*, i64 } %out, 1
  %out.data = extractvalue { i8*, i64 } %out, 0
  %len32 = trunc i64 %out.len to i32
  %5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.9, i32 0, i32 0), i32 %len32, i8* %out.data)
  %stats = getelementptr %Hero, %Hero* %h, i32 0, i32 1
  %atk = getelementptr %Stats, %Stats* %stats, i32 0, i32 1
  %atk13 = load i32, i32* %atk, align 4
  %6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.12, i32 0, i32 0), i32 5, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.10, i32 0, i32 0), i32 %atk13, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.11, i32 0, i32 0))
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %i14 = load i64, i64* %idx, align 8
  %next = add i64 %i14, 1
  store i64 %next, i64* %idx, align 8
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %champ = alloca %Hero*, align 8
  %party15 = load { %Hero*, i64 }, { %Hero*, i64 }* %party, align 8
  %call16 = call %Hero* @strongest({ %Hero*, i64 } %party15, i32 3)
  store %Hero* %call16, %Hero** %champ, align 8
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.14, i32 0, i32 0), i32 10, i8* getelementptr inbounds ([11 x i8], [11 x i8]* @str.13, i32 0, i32 0))
  %ref = load %Hero*, %Hero** %champ, align 8
  %out17 = call { i8*, i64 } @Hero.output(%Hero* %ref)
  %out.len18 = extractvalue { i8*, i64 } %out17, 1
  %out.data19 = extractvalue { i8*, i64 } %out17, 0
  %len3220 = trunc i64 %out.len18 to i32
  %8 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.15, i32 0, i32 0), i32 %len3220, i8* %out.data19)
  %ref21 = load %Hero*, %Hero** %champ, align 8
  %call22 = call i1 @Hero.alive(%Hero* %ref21)
  %bool_str = select i1 %call22, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @true_str, i32 0, i32 0), i8* getelementptr inbounds ([6 x i8], [6 x i8]* @false_str, i32 0, i32 0)
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.18, i32 0, i32 0), i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.16, i32 0, i32 0), i8* %bool_str, i32 2, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str.17, i32 0, i32 0))
  %foes = alloca { i32*, i64 }, align 8
  %arr23 = alloca i32, i64 3, align 4
  %arr.init = getelementptr i32, i32* %arr23, i64 0
  store i32 0, i32* %arr.init, align 4
  %arr.init24 = getelementptr i32, i32* %arr23, i64 1
  store i32 1, i32* %arr.init24, align 4
  %arr.init25 = getelementptr i32, i32* %arr23, i64 2
  store i32 2, i32* %arr.init25, align 4
  %arr.ptr26 = insertvalue { i32*, i64 } undef, i32* %arr23, 0
  %arr.len27 = insertvalue { i32*, i64 } %arr.ptr26, i64 3, 1
  store { i32*, i64 } %arr.len27, { i32*, i64 }* %foes, align 8
  %foes28 = load { i32*, i64 }, { i32*, i64 }* %foes, align 8
  %arr.data29 = extractvalue { i32*, i64 } %foes28, 0
  %arr.len30 = extractvalue { i32*, i64 } %foes28, 1
  %idx31 = alloca i64, align 8
  store i64 0, i64* %idx31, align 8
  %kind = alloca i32, align 4
  br label %for.cond32

for.cond32:                                       ; preds = %for.inc34, %for.end
  %i36 = load i64, i64* %idx31, align 8
  %more37 = icmp slt i64 %i36, %arr.len30
  br i1 %more37, label %for.body33, label %for.end35

for.body33:                                       ; preds = %for.cond32
  %elem38 = getelementptr i32, i32* %arr.data29, i64 %i36
  %x39 = load i32, i32* %elem38, align 4
  store i32 %x39, i32* %kind, align 4
  %loot = alloca { i8*, i64 }, align 8
  %kind40 = load i32, i32* %kind, align 4
  %call41 = call { i8*, i64 } @loot_for(i32 %kind40)
  store { i8*, i64 } %call41, { i8*, i64 }* %loot, align 8
  %kind42 = load i32, i32* %kind, align 4
  %call43 = call i32 @enemy_hp(i32 %kind42)
  %loot44 = load { i8*, i64 }, { i8*, i64 }* %loot, align 8
  %opt.ptr = extractvalue { i8*, i64 } %loot44, 0
  %isnull = icmp eq i8* %opt.ptr, null
  %fallback = select i1 %isnull, { i8*, i64 } { i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.21, i32 0, i32 0), i64 4 }, { i8*, i64 } %loot44
  %str.len = extractvalue { i8*, i64 } %fallback, 1
  %len3245 = trunc i64 %str.len to i32
  %str.data = extractvalue { i8*, i64 } %fallback, 0
  %10 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @fmt.23, i32 0, i32 0), i32 7, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.19, i32 0, i32 0), i32 %call43, i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.20, i32 0, i32 0), i32 %len3245, i8* %str.data, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.22, i32 0, i32 0))
  br label %for.inc34

for.inc34:                                        ; preds = %for.body33
  %i46 = load i64, i64* %idx31, align 8
  %next47 = add i64 %i46, 1
  store i64 %next47, i64* %idx31, align 8
  br label %for.cond32

for.end35:                                        ; preds = %for.cond32
  %party48 = load { %Hero*, i64 }, { %Hero*, i64 }* %party, align 8
  %arr.data49 = extractvalue { %Hero*, i64 } %party48, 0
  %arr.len50 = extractvalue { %Hero*, i64 } %party48, 1
  %idx51 = alloca i64, align 8
  store i64 0, i64* %idx51, align 8
  br label %for.cond52

for.cond52:                                       ; preds = %for.inc54, %for.end35
  %i56 = load i64, i64* %idx51, align 8
  %more57 = icmp slt i64 %i56, %arr.len50
  br i1 %more57, label %for.body53, label %for.end55

for.body53:                                       ; preds = %for.cond52
  %elem58 = getelementptr %Hero, %Hero* %arr.data49, i64 %i56
  %stats59 = getelementptr %Hero, %Hero* %elem58, i32 0, i32 1
  %hp = getelementptr %Stats, %Stats* %stats59, i32 0, i32 0
  %stats60 = getelementptr %Hero, %Hero* %elem58, i32 0, i32 1
  %hp61 = getelementptr %Stats, %Stats* %stats60, i32 0, i32 0
  %hp62 = load i32, i32* %hp61, align 4
  %sub = sub i32 %hp62, 10
  store i32 %sub, i32* %hp, align 4
  br label %for.inc54

for.inc54:                                        ; preds = %for.body53
  %i63 = load i64, i64* %idx51, align 8
  %next64 = add i64 %i63, 1
  store i64 %next64, i64* %idx51, align 8
  br label %for.cond52

for.end55:                                        ; preds = %for.cond52
  %11 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.25, i32 0, i32 0), i32 6, i8* getelementptr inbounds ([7 x i8], [7 x i8]* @str.24, i32 0, i32 0))
  %party65 = load { %Hero*, i64 }, { %Hero*, i64 }* %party, align 8
  %arr.data66 = extractvalue { %Hero*, i64 } %party65, 0
  %arr.at67 = getelementptr %Hero, %Hero* %arr.data66, i32 0
  %out68 = call { i8*, i64 } @Hero.output(%Hero* %arr.at67)
  %out.len69 = extractvalue { i8*, i64 } %out68, 1
  %out.data70 = extractvalue { i8*, i64 } %out68, 0
  %len3271 = trunc i64 %out.len69 to i32
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @fmt.26, i32 0, i32 0), i32 %len3271, i8* %out.data70)
  %party72 = load { %Hero*, i64 }, { %Hero*, i64 }* %party, align 8
  %arr.data73 = extractvalue { %Hero*, i64 } %party72, 0
  %arr.at74 = getelementptr %Hero, %Hero* %arr.data73, i32 0
  %stats75 = getelementptr %Hero, %Hero* %arr.at74, i32 0, i32 1
  %hp76 = getelementptr %Stats, %Stats* %stats75, i32 0, i32 0
  %hp77 = load i32, i32* %hp76, align 4
  %13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @fmt.29, i32 0, i32 0), i32 4, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.27, i32 0, i32 0), i32 %hp77, i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.28, i32 0, i32 0))
  %14 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @fmt.34, i32 0, i32 0), i32 7, i8* getelementptr inbounds ([8 x i8], [8 x i8]* @str.30, i32 0, i32 0), i64 24, i32 8, i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.31, i32 0, i32 0), i32 4, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @str.32, i32 0, i32 0), i32 1, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @str.33, i32 0, i32 0))
  %arr78 = load { %Hero*, i64 }, { %Hero*, i64 }* %party, align 8
  %arr.data79 = extractvalue { %Hero*, i64 } %arr78, 0
  %free.ptr = bitcast %Hero* %arr.data79 to i8*
  call void @free(i8* %free.ptr)
  store { %Hero*, i64 } zeroinitializer, { %Hero*, i64 }* %party, align 8
  ret i32 0
}

declare i32 @printf(i8*, ...)

declare i8* @calloc(i64, i64)

declare void @free(i8*)
```
