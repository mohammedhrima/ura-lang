# Ura

> A compiled, statically-typed language with Python's clean syntax — built on LLVM, written in C.

No braces. No semicolons. Indentation-based like Python, fast like C. Each floor of this tutorial unlocks a new power. By the Boss Room, you'll have a working dungeon game.

---

## Quick Start

```bash
git clone https://github.com/mohammedhrima/ura-lang && cd ura-lang

make -C config/anvil install   # build + install `anvil` to /usr/local/bin (one time)
anvil install                  # install LLVM/clang deps (once per machine)
anvil build                    # compile the ura compiler
anvil test                     # run the test suite
```

Dev workflow is driven by **[anvil](config/anvil/README.md)** — a
Makefile-style CLI. Run `anvil` with no args to see every command
(`build`, `test`, `indent`, `release`, `shell`, …).

```ura
main():
    output("Hello, Dungeon!\n")
```

---

## Table of Contents

- [Floor 1 — Hello, Dungeon](#floor-1--hello-dungeon)
- [Floor 2 — Tracking Progress](#floor-2--tracking-progress)
- [Floor 3 — Name Your Enemies](#floor-3--name-your-enemies)
- [Floor 4 — Combat Math](#floor-4--combat-math)
- [Floor 5 — Loot Drops](#floor-5--loot-drops)
- [Floor 6 — The Battle Loop](#floor-6--the-battle-loop)
- [Floor 7 — The Hero](#floor-7--the-hero)
- [Floor 8 — Hero Abilities](#floor-8--hero-abilities)
- [Floor 9 — Level Up!](#floor-9--level-up)
- [Floor 10 — The Dungeon Map](#floor-10--the-dungeon-map)
- [Floor 11 — Status Effects](#floor-11--status-effects)
- [Floor 12 — Gear Score](#floor-12--gear-score)
- [Floor 13 — Power Moves](#floor-13--power-moves)
- [Floor 14 — C Interop](#floor-14--c-interop)
- [Floor 15 — Command Line](#floor-15--command-line)
- [Floor 16 — Graphics](#floor-16--graphics)
- [Floor 17 — Network](#floor-17--network)
- [Floor 18 — The Scroll of Words](#floor-18--the-scroll-of-words)
- [Floor 19 — Loot Tables](#floor-19--loot-tables)
- [Floor 20 — The Hero's Identity](#floor-20--the-heros-identity)
- [Floor 21 — The Armory](#floor-21--the-armory)
- [Boss Room — The Full Game](#boss-room--the-full-game)
- [How It Works](#how-it-works)
- [Compiler Reference](#compiler-reference)
- [Status](#status)
- [Requirements](#requirements)
- [Author](#author)

---

## Floor 1 — Hello, Dungeon

Variables are declared `name type = value`. Types: `int` `long` `short` `float` `char` `chars` `bool`. `output` is a built-in — no imports, no format strings, any number of values:

```ura
main():
    hero  chars = "Aldric"
    hp    int   = 100
    mp    int   = 50
    alive bool  = True
    speed float = 1.5

    output("=== ", hero, " enters the dungeon ===\n")
    output("HP: ", hp, "  MP: ", mp, "  Speed: ", speed, "\n")
    output("typeof hp: ", typeof(hp), "  sizeof chars ptr: ", sizeof(hero), "\n")
```

```
=== Aldric enters the dungeon ===
HP: 100  MP: 50  Speed: 1.5
typeof hp: INT  sizeof chars ptr: 8
```

`output` handles `int`, `long`, `short`, `float`, `char`, `chars`, `bool`. `typeof` returns the type name as a string. `sizeof` returns the byte size. For formatted output (`%d`, padding, etc.) use `printf` from `@/io` — covered in Floor 14.

---

## Floor 2 — Tracking Progress

Global variables live above all functions. Every function in the file can read and write them:

```ura
score int = 0
floor int = 1
lives int = 3

fn earn(pts int) void:
    score = score + pts

main():
    output("Floor ", floor, " | Score: ", score, " | Lives: ", lives, "\n")
    earn(100)
    earn(50)
    output("After two kills — Score: ", score, "\n")
```

```
Floor 1 | Score: 0 | Lives: 3
After two kills — Score: 150
```

---

## Floor 3 — Name Your Enemies

Enums turn raw integers into readable names. Variants start at `0` and count up:

```ura
enum EnemyType:
    ORC, SPIDER, DRAGON, GOLEM

enum DamageType:
    PHYSICAL, FIRE, ICE, POISON

fn announce(enemy int, dtype int) void:
    if enemy == DRAGON:      output("Dragon")
    elif enemy == ORC:       output("Orc")
    elif enemy == SPIDER:    output("Spider")
    else:                    output("Golem")
    output(" — ")
    if dtype == FIRE:        output("fire\n")
    elif dtype == ICE:       output("ice\n")
    elif dtype == POISON:    output("poison\n")
    else:                    output("physical\n")

main():
    announce(DRAGON, FIRE)
    announce(ORC,    PHYSICAL)
    announce(SPIDER, POISON)
```

```
Dragon — fire
Orc — physical
Spider — poison
```

Enum variants are global `int` constants. Use them anywhere an `int` is expected.

---

## Floor 4 — Combat Math

Functions are `fn name(params) returnType:`. Call them like any language:

```ura
fn clamp(val int, lo int, hi int) int:
    if val < lo: return lo
    if val > hi: return hi
    return val

fn damage(atk int, def int) int:
    return clamp(atk - def, 0, 999)

fn is_dead(hp int) bool:
    return hp <= 0

// single-line functions
fn square(n int) int:      return n * n
fn is_digit(c char) bool:  return c >= '0' and c <= '9'

main():
    d int = damage(25, 8)
    output("Orc takes ", d, " damage — dead: ", is_dead(d - 60), "\n")
    output("4 squared: ", square(4), "\n")
```

```
Orc takes 17 damage — dead: False
4 squared: 16
```

---

## Floor 5 — Loot Drops

Return multiple values by listing types in parentheses. Unpack at the call site:

```ura
fn roll_loot(f int) (chars, int):
    item  chars = "Iron Sword"
    value int   = f * 15
    return item, value

main():
    item chars, gold int = roll_loot(3)
    output("Floor 3 loot: ", item, " worth ", gold, " gold\n")

    // ignore one value with _
    _, bonus int = roll_loot(5)
    output("Bonus gold: ", bonus, "\n")
```

```
Floor 3 loot: Iron Sword worth 45 gold
Bonus gold: 75
```

---

## Floor 6 — The Battle Loop

`while` loops, `if`/`elif`/`else` branching, `break`, and `continue` — all in one fight:

```ura
fn simulate(hero_hp int, orc_hp int) void:
    round int = 0
    while hero_hp > 0:
        round = round + 1
        if orc_hp <= 0:
            output("Victory in round ", round - 1, "!\n")
            return
        if round > 20:
            output("Stalemate\n")
            break
        hero_hp = hero_hp - 10
        orc_hp  = orc_hp  - 18

    if hero_hp <= 0:
        output("Hero fell in round ", round, "\n")

main():
    simulate(100, 60)   // hero wins
    simulate(30,  90)   // hero dies
```

```
Victory in round 4!
Hero fell in round 3
```

| Keyword | Purpose |
|---------|---------|
| `while cond:` | loop while condition holds |
| `break` | exit the loop immediately |
| `continue` | skip to the next iteration |
| `if` / `elif` / `else` | branching |

---

## Floor 7 — The Hero

Structs group related data. Fields are declared inside the struct body, one per line. Structs are **zero-initialized** by default:

```ura
struct Player:
    name  chars
    hp    int
    mp    int

struct Enemy:
    name  chars
    hp    int
    atk   int

fn fight(p Player, e Enemy) void:
    output(p.name, " (", p.hp, " HP) faces ", e.name, " (", e.hp, " HP)\n")
    if p.hp > e.hp:
        output(p.name, " wins!\n")
    else:
        output(e.name, " wins!\n")

main():
    hero Player
    hero.name = "Aldric"
    hero.hp   = 100
    hero.mp   = 50

    orc Enemy
    orc.name = "Orc Grunt"
    orc.hp   = 60
    orc.atk  = 14

    fight(hero, orc)
```

```
Aldric (100 HP) faces Orc Grunt (60 HP)
Aldric wins!
```

**Nested structs** work at any depth:

```ura
struct Room:
    name  chars
    floor int

struct Dungeon:
    name  chars
    entry Room

main():
    d Dungeon
    d.name        = "The Abyss"
    d.entry.name  = "Gate Hall"
    d.entry.floor = 1
    output("Welcome to ", d.name, " — enter via ", d.entry.name, "\n")
```

```
Welcome to The Abyss — enter via Gate Hall
```

---

## Floor 8 — Hero Abilities

`pub fn` declares a **static method** — no `self`, called with `Type::method()`. Use it as an explicit constructor. `operator delete()` is the destructor — auto-called when the variable goes out of scope. `self` refers to the current instance in regular methods:

```ura
use "@/header"

struct Hero:
    name  chars
    title chars
    hp    int

    pub fn new() Hero:
        h Hero
        h.name  = heap[char](32)
        h.title = heap[char](32)
        h.hp    = 100
        return h

    fn setup(name chars, title chars) void:
        strcpy(self.name,  name)
        strcpy(self.title, title)

    fn announce() void:
        output(self.title, " ", self.name, " — HP: ", self.hp, "\n")

    fn heal(amount int) void:
        self.hp = self.hp + amount

    operator delete() void:
        free(self.name)
        free(self.title)

main():
    hero Hero = Hero::new()              // pub fn — static call
    hero.setup("Aldric", "The Bold")
    hero.announce()
    hero.heal(20)
    hero.announce()
                                         // operator delete() called here automatically
```

```
The Bold Aldric — HP: 100
The Bold Aldric — HP: 120
```

| Concept | Syntax | Notes |
|---------|--------|-------|
| Static method | `pub fn new() Hero:` | No `self`, called as `Hero::new()` |
| Instance method | `fn heal(amount int) void:` | Has implicit `self` ref |
| Destructor | `operator delete() void:` | Auto-called at scope exit |
| Static dispatch | `Hero::new()` | `::` calls a `pub fn` |
| Forward references | methods can call each other in any order | No declaration-before-use restriction within a struct |

---

## Floor 9 — Level Up!

References let a function modify the caller's variables directly. Mark a parameter `ref` after the type — the caller passes the variable as-is, no `&` needed:

```ura
fn level_up(level int ref, hp int ref, mp int ref) void:
    level = level + 1
    hp    = hp + 20
    mp    = mp + 10
    output("★ Level ", level, "! HP+20 MP+10\n")

fn swap(a int ref, b int ref) void:
    tmp int = a
    a   = b
    b   = tmp

main():
    lv int = 1
    hp int = 100
    mp int = 50
    level_up(lv, hp, mp)
    output("Stats: HP=", hp, " MP=", mp, "\n")

    x int = 3
    y int = 7
    swap(x, y)
    output("x=", x, " y=", y, "\n")
```

```
★ Level 2! HP+20 MP+10
Stats: HP=120 MP=60
x=7 y=3
```

Local references bind at declaration and must not outlive the target:

```ura
main():
    hp  int     = 80
    ref int ref = hp   // binds to hp — must bind at declaration
    ref = 100          // writes through to hp
    output(hp, "\n")   // 100
```

---

## Floor 10 — The Dungeon Map

Ura gives you explicit control over memory. Choose stack (auto-freed) or heap (manual `free`):

```ura
use "@/header"

main():
    // heap: size decided at runtime, you manage lifetime
    map chars = heap[char](51)
    i   int   = 0
    while i < 50:
        map[i] = '.'
        i = i + 1
    map[25] = '@'           // hero at centre
    map[50] = 0 as char     // null-terminate
    output(map, "\n")
    free(map)

    // stack: auto-freed at end of scope, fixed size
    buf chars = stack[char](16)
    buf[0] = 'H'
    buf[1] = 'P'
    buf[2] = 0 as char
    output(buf, "\n")
```

```
.........................@........................
HP
```

| | `stack[type](n)` | `heap[type](n)` |
|---|---|---|
| Freed | automatically at scope exit | manually with `free()` |
| Size | compile-time fixed | runtime variable |
| Speed | faster | slightly slower |

**Multi-dimensional arrays** — the dungeon board is a 2D flat array with chained `[row][col]` indexing:

```ura
main():
    // 4 rows × 3 cols  (stack[[int]] = 2D int array)
    board array[[int]] = stack[[int]](4, 3)

    r int = 0
    while r < 4:
        c int = 0
        while c < 3:
            board[r][c] = r * 3 + c
            c = c + 1
        r = r + 1

    r = 0
    while r < 4:
        c int = 0
        while c < 3:
            output(board[r][c], " ")
            c = c + 1
        output("\n")
        r = r + 1
```

```
0 1 2
3 4 5
6 7 8
9 10 11
```

`array[[int]]` = 2D, `array[[[int]]]` = 3D, and so on. Use `heap[[int]](rows, cols)` for runtime-sized grids.

---

## Floor 11 — Status Effects

Bitwise operators pack multiple flags into one `int`. Standard arithmetic, comparison, logical, and assignment operators all work as expected:

```ura
POISONED int = 1   // 0001
FROZEN   int = 2   // 0010
BURNING  int = 4   // 0100
SHIELDED int = 8   // 1000

fn has(status int, flag int) bool:  return (status & flag) != 0
fn add(status int, flag int) int:   return status | flag
fn rem(status int, flag int) int:   return status & ~flag

main():
    s int = 0
    s = add(s, POISONED)
    s = add(s, BURNING)
    output("Poisoned: ", has(s, POISONED), "\n")   // True
    output("Frozen:   ", has(s, FROZEN),   "\n")   // False
    output("Burning:  ", has(s, BURNING),  "\n")   // True
    s = rem(s, POISONED)
    output("Cured:    ", has(s, POISONED), "\n")   // False
```

```
Poisoned: True
Frozen:   False
Burning:  True
Cured:    False
```

| Category   | Operators |
|------------|-----------|
| Arithmetic | `+`  `-`  `*`  `/`  `%` |
| Bitwise    | `&`  `\|`  `^`  `~`  `<<`  `>>` |
| Comparison | `==` `!=` `<` `>` `<=` `>=` |
| Logical    | `and` `or` `not` |
| Assignment | `=` `+=` `-=` `*=` `/=` `%=` |

---

## Floor 12 — Gear Score

`as` casts between any numeric, pointer, and float↔int type pair. The ASCII table is just an `int → char` cast away:

```ura
main():
    // score percentage
    score     int   = 847
    max_score int   = 1000
    pct       float = score as float / max_score as float * 100.0
    output("Score: ", score, "/", max_score, "  Rating: ", pct, "%\n")

    // char ↔ int (ASCII)
    grade char = 'A'
    code  int  = grade as int          // 65
    back  char = (code + 1) as char    // 'B'
    output("Grade: ", grade, " → ", code, " → ", back, "\n")

    // char difference
    diff int = 'd' as int - 'a' as int
    output("'d' - 'a' = ", diff, "\n")   // 3
```

```
Score: 847/1000  Rating: 84.7%
Grade: A → 65 → B
'd' - 'a' = 3
```

---

## Floor 13 — Power Moves

Structs can define their own behaviour for any operator. Use `operator OP (param type) returnType:` inside the struct body. The first parameter's type is part of the overload name, so you can have multiple overloads of the same operator:

```ura
struct Vec2:
    x int
    y int

    operator = (v Vec2 ref) void:
        self.x = v.x
        self.y = v.y

    operator + (v Vec2 ref) Vec2:
        res Vec2
        res.x = self.x + v.x
        res.y = self.y + v.y
        return res

    operator == (v Vec2 ref) bool:
        return self.x == v.x and self.y == v.y

    fn print() void:
        output("(", self.x, ", ", self.y, ")\n")

fn walk(steps int, dir Vec2) Vec2:
    pos Vec2
    i   int = 0
    while i < steps:
        pos = pos + dir
        i   = i + 1
    return pos

main():
    north Vec2
    north.x = 0
    north.y = 1

    east Vec2
    east.x = 1
    east.y = 0

    pos Vec2 = walk(3, north)
    pos = pos + east
    pos.print()

    origin Vec2
    output("at origin: ", pos == origin, "\n")
```

```
(1, 3)
at origin: False
```

**Multiple overloads per operator** — distinguish by parameter type:

```ura
use "@/header"

struct Label:
    value chars
    len   int

    pub fn new() Label:
        l Label
        l.value = heap[char](64)
        l.len   = 0
        return l

    operator delete() void:
        free(self.value)

    fn set(s chars) void:
        strcpy(self.value, s)
        self.len = strlen(s) as int

    operator = (s chars) void:
        self.set(s)

    operator = (v Label ref) void:
        self.set(v.value)

    operator + (v Label ref) Label:
        res Label = Label::new()
        strcpy(res.value, self.value)
        strcat(res.value, v.value)
        res.len = self.len + v.len
        return res

    fn print() void:
        output(self.value, " (", self.len, " chars)\n")

main():
    title Label = Label::new()
    title = "Shadow"

    name Label = Label::new()
    name = "Aldric"

    full Label = title + name
    full.print()
```

```
ShadowAldric (11 chars)
```

**Copy-constructor semantics** — if you pass a struct by value to a function and `operator = (v StructType ref)` is defined, the compiler deep-copies automatically. The original is never touched:

```ura
use "@/header"

fn rename(hero Label, suffix chars) void:
    strcat(hero.value, suffix)
    output("inside: ", hero.value, "\n")

main():
    hero Label = Label::new()
    hero = "Aldric"
    rename(hero, " the Brave")   // hero is deep-copied for the call
    output("after:  ", hero.value, "\n")
```

```
inside: Aldric the Brave
after:  Aldric
```

**Operator name mangling:**

| Declaration | Internal name |
|---|---|
| `operator = (s chars)` | `Hero.operator.ASSIGN.CHARS` |
| `operator = (v Hero ref)` | `Hero.operator.ASSIGN.Hero` |
| `operator + (v Hero ref)` | `Hero.operator.ADD.Hero` |
| `operator == (v Hero ref)` | `Hero.operator.EQUAL.Hero` |
| `operator < (v Hero ref)` | `Hero.operator.LESS.Hero` |
| `operator delete()` | `Hero.delete` |

---

## Floor 14 — C Interop

`use "@/module"` imports from the standard library. `@` resolves to `$URA_LIB`. `use "@/header"` pulls in everything at once:

```ura
use "@/io"       // printf, puts, fopen, fclose, write, read, ...
use "@/memory"   // malloc, free, realloc, ...
use "@/string"   // strlen, strcmp, strcpy, strcat, strdup, String struct, ...
use "@/stdlib"   // atoi, rand, exit, ...
use "@/time"     // time, clock, difftime, ...
use "@/signals"  // signal, raise, ...
use "@/net"      // socket, bind, listen, connect, send, recv, ...
use "@/os"       // os.argc, os.argv
use "@/header"   // all of the above at once
```

For full control, declare any C function with `proto`. Variadic functions take `...`:

```ura
proto fn strlen(s chars) int
proto fn write(fd int, ptr chars, len int) int
proto fn printf(fmt chars, ...) int

main():
    n int = strlen("dungeon")
    write(1, "hi\n", 3)
    printf("length: %d\n", n)
```

Pull in the entire C stdlib in one line for random encounters:

```ura
use "@/header"

fn random_encounter() chars:
    r int = rand() % 4
    if r == 0: return "Giant Spider"
    if r == 1: return "Orc Shaman"
    if r == 2: return "Stone Golem"
    return "Shadow Wraith"

main():
    srand(42)
    output("Encounter 1: ", random_encounter(), "\n")
    output("Encounter 2: ", random_encounter(), "\n")
    output("Encounter 3: ", random_encounter(), "\n")
```

```
Encounter 1: Orc Shaman
Encounter 2: Shadow Wraith
Encounter 3: Giant Spider
```

**Linking external libraries** — set an environment variable, then `use` the module:

```bash
export URA_LINK_mylib="/path/to/libmylib.a"
```

```ura
use "@/mylib"   // compiler reads URA_LINK_mylib and passes it to the linker
```

---

## Floor 15 — Command Line

`os.argc` and `os.argv` are populated automatically before `main()` runs when you `use "@/header"`:

```ura
use "@/header"

main():
    if os.argc < 2:
        output("Usage: dungeon <hero_name>\n")
        return
    name chars = os.argv[1]
    output("=== Welcome, ", name, "! ===\n")
    output("Your quest begins on floor 1.\n")
```

```bash
ura dungeon.ura
# Usage: dungeon <hero_name>

ura dungeon.ura Aldric
# === Welcome, Aldric! ===
# Your quest begins on floor 1.
```

---

## Floor 16 — Graphics

Link against raylib and get a real dungeon window. Export the path once, then `use "@/raylib"`:

```bash
export URA_LINK_raylib="/opt/homebrew/lib/libraylib.a"   # macOS
export URA_LINK_raylib="/usr/local/lib/libraylib.a"      # Linux
```

```ura
use "@/raylib"

main():
    InitWindow(800, 600, "Dungeon — Floor 1")
    SetTargetFPS(60)
    hero_x int = 100
    speed  int = 3
    while WindowShouldClose() == False:
        hero_x = hero_x + speed
        if hero_x > 800: hero_x = 0
        BeginDrawing()
        ClearBackground(setColor(20, 20, 40, 255))
        DrawRectangle(hero_x, 300, 40, 60, setColor(180, 100, 255, 255))
        DrawText("Dungeon Hero", 10, 10, 24, setColor(255, 220, 0, 255))
        EndDrawing()
    CloseWindow()
```

```bash
ura dungeon.ura && ./dungeon
```

---

## Floor 17 — Network

POSIX sockets through `@/net`. Full server + client examples live in `tests/projects/ura-tcp-server/` ([github.com/mohammedhrima/ura-tcp-server](https://github.com/mohammedhrima/ura-tcp-server)):

```ura
use "@/header"

main():
    fd int = socket(2, 1, 0)   // AF_INET, SOCK_STREAM
    if fd < 0:
        output("socket failed\n")
        return

    port int  = 8080
    addr chars = heap[char](16)
    addr[0] = 16 as char
    addr[1] = 2  as char
    addr[2] = (port >> 8)   as char
    addr[3] = (port & 255)  as char

    bind(fd, addr, 16)
    listen(fd, 5)
    output("listening on :8080\n")

    buf      chars = heap[char](1024)
    client   int   = accept(fd, null, null)
    if client >= 0:
        r int = read(client, buf, 1023)
        if r > 0:
            output("got: ", buf, "\n")
            write(client, "pong\n", 5)
        close(client)
    free(buf)
    free(addr)
```

```bash
# Terminal 1
ura tests/projects/ura-tcp-server/basic/server.ura

# Terminal 2
ura tests/projects/ura-tcp-server/basic/client.ura
```

---

## Floor 18 — The Scroll of Words

The standard library includes a full-featured `String` struct. `use "@/header"` (or `use "@/string"`) imports it. Construct with `String::new()`, `String::from()`, or `String::from_int()`. Destruction is automatic:

```ura
use "@/header"

main():
    // construct
    greeting String = String::from("Hello, Dungeon!")
    output(greeting.value, " (len: ", greeting.len(), ")\n")

    // search
    output("found 'Dungeon' at: ", greeting.find("Dungeon"), "\n")
    output("starts with Hello: ", greeting.starts_with("Hello"), "\n")

    // transform
    shout String = greeting.upper()
    output(shout.value, "\n")

    whisper String = greeting.lower()
    output(whisper.value, "\n")

    word String = greeting.substr(7, 7)
    output("substr: ", word.value, "\n")

    // mutate
    msg String = String::new()
    msg.push('H')
    msg.push('P')
    output("pushed: ", msg.value, "\n")

    // operators
    first String = String::from("Shadow")
    last  String = String::from(" Blade")
    name  String = first + last
    output("name: ", name.value, "\n")

    first += last
    output("+=:   ", first.value, "\n")

    a String = String::from("abc")
    b String = String::from("abc")
    output("equal: ", a == b, "\n")

    // convert
    hp_str String = String::from_int(100)
    output("HP: ", hp_str.value, "\n")

    num String = String::from("42")
    output("parsed: ", num.to_int(), "\n")

    // more transforms
    padded  String = String::from("  trimmed  ")
    trimmed String = padded.trim()
    output("trim: '", trimmed.value, "'\n")

    orig    String = String::from("orc orc orc")
    swapped String = orig.replace("orc", "elf")
    output("replace: ", swapped.value, "\n")

    ha  String = String::from("ha")
    ha3 String = ha.repeat(3)
    output("repeat: ", ha3.value, "\n")

    rev String = String::from("dragon")
    output("reverse: ", rev.reverse().value, "\n")
```

```
Hello, Dungeon! (len: 15)
found 'Dungeon' at: 7
starts with Hello: True
HELLO, DUNGEON!
hello, dungeon!
substr: Dungeon
pushed: HP
name: Shadow Blade
+=:   Shadow Blade
equal: True
HP: 100
parsed: 42
trim: 'trimmed'
replace: elf elf elf
repeat: hahaha
reverse: nogard
```

**String API:**

| Category | Methods |
|----------|---------|
| Constructors | `String::new()` `String::from(str)` `String::from_int(n)` |
| Accessors | `.len()` `.empty()` `.at(i)` `.c_str()` |
| Mutators | `.assign(str)` `.join(str)` `.push(c)` `.pop()` `.clear()` |
| Search | `.find(str)` `.contains(str)` `.starts_with(str)` `.ends_with(str)` |
| Transform | `.substr(i, n)` `.upper()` `.lower()` `.trim()` `.replace(old, new)` `.repeat(n)` `.reverse()` |
| Convert | `.to_int()` `.compare(other)` |
| Operators | `=` `+` `+=` `==` `!=` `<` `>` (both `String` and `chars` overloads) |

---

## Floor 19 — Loot Tables

Array literals let you initialize arrays inline with `[value, value, ...]`:

```ura
use "@/header"

main():
    enemies array[chars] = ["Orc", "Spider", "Dragon"]
    loot    array[int]   = [15, 30, 100]

    output("Enemies: ", enemies[0], ", ", enemies[1], ", ", enemies[2], "\n")
    output("Loot:    ", loot[0], ", ", loot[1], ", ", loot[2], " gold\n")

    roll int = 4
    output("Roll ", roll, ": ", enemies[roll % 3], "\n")

    hits array[int] = [12, 8, 15, 20]
    total int = 0
    i int = 0
    while i < 4:
        total += hits[i]
        i += 1
    output("Total damage: ", total, "\n")
```

```
Enemies: Orc, Spider, Dragon
Loot:    15, 30, 100 gold
Roll 4: Spider
Total damage: 55
```

Elements can be any expression — variables, arithmetic, function calls:

```ura
fn double(n int) int:
    return n * 2

main():
    x int = 5
    v array[int] = [x, x + 1, x * 2, double(x)]
    output(v[0], " ", v[1], " ", v[2], " ", v[3], "\n")   // 5 6 10 10
```

---

## Floor 20 — The Hero's Identity

Structs print as `{ field: value, ... }` by default. Define `operator output()` to customize how a struct appears in `output()`:

```ura
use "@/header"

struct Hero:
    name  chars
    level int
    hp    int

    pub fn new(name chars) Hero:
        h Hero
        h.name  = name
        h.level = 1
        h.hp    = 100
        return h

    operator output() String:
        s String = String::from(self.name)
        s.join(" lv")
        n String = String::from_int(self.level)
        s.join(n.value)
        s.join(" HP:")
        hp String = String::from_int(self.hp)
        s.join(hp.value)
        return s

struct Loot:
    name  chars
    value int

main():
    hero Hero = Hero::new("Aldric")
    output("Hero: ", hero, "\n")

    loot Loot
    loot.name  = "Iron Sword"
    loot.value = 50
    output("Loot: ", loot, "\n")

    s String = String::from("Fireball")
    output("Spell: ", s, "\n")
```

```
Hero: Aldric lv1 HP:100
Loot: { name: Iron Sword, value: 50 }
Spell: Fireball
```

- **Hero** has `operator output()` → returns a formatted `String` → the compiler chains to `String`'s `operator output()` which returns `self.value` (chars)
- **Loot** has no `operator output()` → uses default `{ field: value }` printing
- **String** has `operator output() chars:` → returns `self.value` directly

The returned `String` from `operator output()` is a temporary — the compiler auto-frees it after `output()` finishes. No memory leak.

| Return type | Behavior |
|-------------|----------|
| `chars` | Used directly as `%s` in printf |
| `String` (or any struct) | Compiler chains — calls that struct's `operator output()` recursively |
| *(not defined)* | Default `{ field: value, ... }` printing |

---

## Floor 21 — The Armory

`List[T]` is a built-in dynamic array — push, pop, grow automatically. Works for any type. Requires `use "@/memory"`:

```ura
use "@/memory"

main():
    loot List[int]
    loot.push(15)
    loot.push(30)
    loot.push(100)

    output("Items: ", loot.len(), "\n")
    output("Loot: ", loot[0], ", ", loot[1], ", ", loot[2], "\n")

    total int = 0
    i int = 0
    while i < loot.len():
        total += loot[i]
        i += 1
    output("Total gold: ", total, "\n")

    dropped int = loot.pop()
    output("Dropped ", dropped, " gold. Remaining: ", loot.len(), "\n")
```

```
Items: 3
Loot: 15, 30, 100
Total gold: 145
Dropped 100 gold. Remaining: 2
```

| Method | Description |
|--------|-------------|
| `v.push(e)` | Append element (auto-grows) |
| `v.pop()` | Remove and return last element |
| `v.len()` | Current element count |
| `v.cap()` | Allocated capacity |
| `v[i]` | Access element at index |
| `v.data` | Raw `array[T]` pointer (for C interop) |

Pass `v.data` to functions expecting `array[T]`:

```ura
fn sum(arr array[int], n int) int:
    s int = 0
    i int = 0
    while i < n:
        s += arr[i]
        i += 1
    return s

main():
    v List[int]
    v.push(10)
    v.push(20)
    output("sum=", sum(v.data, v.len()), "\n")   // sum=30
```

`List[T]` is implemented via template monomorphization — the compiler generates a struct `__list_int` (or `__list_char`, etc.) with push/pop/len/cap methods. No generics syntax in the language; the compiler handles it internally. Use `-prep` to see the expanded code.

---

## Boss Room — The Full Game

All the skills. One program. The dungeon is alive:

```ura
score int = 0
floor int = 1

enum EnemyType:
    ORC, SPIDER, DRAGON

struct Stats:
    hp  int
    atk int
    def int

struct Hero:
    name  chars
    stats Stats
    level int

    pub fn new() Hero:
        h Hero
        h.name       = "Aldric"
        h.stats.hp   = 100
        h.stats.atk  = 20
        h.stats.def  = 5
        h.level      = 1
        return h

    fn is_alive() bool:
        return self.stats.hp > 0

    fn take_hit(dmg int) void:
        actual int = dmg - self.stats.def
        if actual < 1: actual = 1
        self.stats.hp = self.stats.hp - actual

    fn level_up() void:
        self.level      = self.level + 1
        self.stats.atk  = self.stats.atk + 5
        self.stats.def  = self.stats.def + 2
        self.stats.hp   = self.stats.hp + 20

    fn status() void:
        output("  ", self.name, " lv", self.level,
               "  HP:", self.stats.hp,
               "  ATK:", self.stats.atk, "\n")

fn enemy_hp(t int) int:
    if t == DRAGON: return 120
    if t == SPIDER: return 40
    return 60

fn enemy_atk(t int) int:
    if t == DRAGON: return 22
    if t == SPIDER: return 8
    return 12

fn enemy_name(t int) chars:
    if t == DRAGON: return "Dragon"
    if t == SPIDER: return "Spider"
    return "Orc"

fn fight(hero Hero ref, enemy int) bool:
    ehp  int = enemy_hp(enemy)
    eatk int = enemy_atk(enemy)
    output("  [", enemy_name(enemy), " — HP:", ehp, " ATK:", eatk, "]\n")
    round int = 0
    while hero.is_alive() and ehp > 0:
        round  = round + 1
        ehp    = ehp - hero.stats.atk
        if ehp > 0:
            hero.take_hit(eatk)
    if hero.is_alive():
        score = score + 50 * floor
        return True
    return False

main():
    hero Hero = Hero::new()
    output("=== ", hero.name, " enters the dungeon ===\n")
    hero.status()

    encounters int = 3
    i          int = 0
    while i < encounters and hero.is_alive():
        enemy int = i % 3   // ORC, SPIDER, DRAGON
        output("Floor ", floor, " encounter ", i + 1, ":\n")
        won bool = fight(hero, enemy)
        if won:
            output("  Victory!\n")
            if i == encounters - 1:
                hero.level_up()
                floor = floor + 1
        else:
            output("  Hero fell.\n")
        hero.status()
        i = i + 1

    output("=== Run complete | Score: ", score, " | Floor: ", floor, " ===\n")
```

```
=== Aldric enters the dungeon ===
  Aldric lv1  HP:100  ATK:20
Floor 1 encounter 1:
  [Orc — HP:60 ATK:12]
  Victory!
  Aldric lv1  HP:93  ATK:20
Floor 1 encounter 2:
  [Spider — HP:40 ATK:8]
  Victory!
  Aldric lv1  HP:88  ATK:20
Floor 1 encounter 3:
  [Dragon — HP:120 ATK:22]
  Victory!
  Aldric lv2  HP:108  ATK:25
=== Run complete | Score: 150 | Floor: 2 ===
```

---

## How It Works

The compiler is written entirely in C. LLVM handles only the final step — converting architecture-independent IR to native code. Ura targets any platform LLVM supports with zero extra work.

### Compilation Pipeline

```
source.ura
    │
    ▼  tokenize()                flat list of tokens
    │
    ▼  synth_list_structs()     monomorphize List[T] → __list_T structs
    │
    ▼  AST                      recursive descent parser → syntax tree
    │
    ▼  ir_gen()                  name resolution, type checking,
    │                            operator overload dispatch
    ▼  asm_gen()                 LLVM IR emission via LLVM C API → build/file.ll
    │
    ▼  llc                      .ll → .s  (native assembly)
    │
    ▼  clang                    .s → executable
```

### Source Files

- **`src/main.c`** (~130 lines) — `compile()` pipeline orchestration and `int main()` argument parsing.
- **`src/parse.c`** (~1200 lines) — tokenizer (`parse_token`, `tokenize`), recursive descent parser, `synth_list_structs` (monomorphizes `List[T]` → `__list_T` struct sources), `generate_list_source` template emitter. Operator precedence is encoded in the call chain: `expr → assign → logic_or → … → as → unary → access → prime`.
- **`src/gen.c`** (~830 lines) — `ir_gen()` (semantic pass: name resolution, type checking, operator overload dispatch) and `asm_gen()` (LLVM IR emission). One thin per-case helper (`ir_<case>` / `asm_<case>`) per AST node kind.
- **`src/utils.c`** (~2470 lines) — LLVM type helpers, scope management, IR/ASM support helpers, debug printing (`pnode`, `_vprint`), `copy_token` (deep-copies LLVM metadata), and the `-prep` serializer (`emit_prep_file` and friends).
- **`src/header.h`** (~530 lines) — `Token`, `Node`, `Type` enum, all global declarations and prototypes.

---

## Compiler Reference

### Usage

```bash
ura <file.ura> [options]
ura src/game.ura -O2 -o dungeon
```

### Options

| Flag | Description |
|------|-------------|
| `-O0` | No optimization (debug) |
| `-O1` | Basic optimization |
| `-O2` | Standard optimization |
| `-O3` | Aggressive optimization |
| `-Os` | Optimize for size |
| `-Oz` | Minimize binary size |
| `-san` | AddressSanitizer + debug info |
| `-no-exec` | Compile only — generate .ll and .s but don't link/run |
| `-no-debug` | Suppress debug output |
| `-prep` | Generate preprocessed `.prep.ura` — expanded generics, resolved operators, inlined imports |
| `-o <name>` | Output name (default: source basename) |

### anvil

Dev tool for the compiler (replaces `config.sh`). See
[config/anvil/README.md](config/anvil/README.md) for the full command list.

```bash
make -C config/anvil install   # one-time install
anvil                          # list commands
anvil build                    # compile the ura compiler
anvil test                     # run the test suite
anvil install                  # install LLVM/clang deps
anvil indent                   # clang-format the C sources
anvil shell                    # drop into the Linux dev container
anvil release --confirm        # build + sync everything
```

Config lives in [anvil.toml](anvil.toml) at the repo root.

---

## VS Code Extension

Syntax highlighting for `.ura` files — keywords, types, operators, strings, comments, `pub fn`, `operator delete`, `Type::method()` static dispatch.

```bash
git clone https://github.com/mohammedhrima/ura-vscode-extension
cd ura-vscode-extension && npm install && npm run package
code --install-extension ura-lang-*.vsix
```

---

## Status

**Working today:**
- All primitive types: `int` `long` `short` `char` `chars` `bool` `float` `void`
- `null` literal, enums, global variables
- Functions, variadic functions, single-line functions
- Forward references — functions and struct methods can be called before declaration
- Tuples — multi-value returns with destructuring
- `while` loops, `if`/`elif`/`else`, `break`, `continue`
- Structs, nested structs, methods with `self`
- `pub fn` — static methods, called via `Type::method()` (explicit constructors)
- `operator delete()` — destructor, auto-called at scope exit
- **Operator overloading** — `=` `+` `-` `*` `/` `%` `==` `!=` `<` `>` `<=` `>=` `+=` `-=` `*=` `/=` `%=` `&` `|` `^` `<<` `>>`
- **Copy-constructor semantics** — struct by-value args auto-deep-copy via `operator =`
- **`operator output()`** — custom struct printing, returns `chars` or `String` (chains recursively)
- **Auto-free temp structs** — struct values returned from functions and not assigned are auto-freed at scope exit
- **Built-in String type** — constructors, search, transforms, operators, `operator output()`
- **Array literals** — `[1, 2, 3]` inline array initialization
- References and reference parameters
- Stack/heap allocation: `stack[type](n)`, `heap[type](n)`
- **Multi-dimensional arrays** — `stack[[int]](rows, cols)`, chained `[r][c]` indexing
- All operators: arithmetic, bitwise, logical, comparison, assignment
- Pointer arithmetic — `chars + int` emits GEP
- Type casting with `as` — numeric, pointer, float↔int, char↔int
- `typeof`, `sizeof`
- `output` builtin — no format string, no imports
- `use` module system (`@` for stdlib, relative for local files)
- `proto` for C interop, variadic supported
- OS module — `os.argc`, `os.argv`
- Multi-file imports via `use` (single `.ura` entry point on the CLI)
- Networking via POSIX socket APIs
- LLVM optimization passes (-O0 → -O3, -Os, -Oz)
- AddressSanitizer support (`-san`)
- **`List[T]`** — generic dynamic array (push, pop, len, cap, `v[i]`, auto-free)
- **`-prep` flag** — preprocessed source output (expanded generics, resolved operators)

**Coming next:**
- For loops (`for i in 0..10`)
- Switch / case
- More container types (Map, Set)
- Type inference
- Type aliases
- Default parameter values

---

## Requirements

| Platform | What you need |
|----------|--------------|
| macOS | Homebrew + `brew install llvm@14` |
| Linux | `clang-14` + `llvm-14` + `llvm-14-tools` |
| Windows | MSYS2 MinGW64 + `mingw-w64-x86_64-llvm` + clang |

Run `anvil install` to install everything automatically.

> **Why LLVM 14?** It uses explicit pointer types (`i32*`) in IR instead of the opaque `ptr` from LLVM 15+, making the generated `.ll` files much easier to read and debug.

---

## Author

**Mohammed Hrima** — [linkedin.com/in/mohammedhrima](https://www.linkedin.com/in/mohammedhrima) — mohammed.hrima1998@gmail.com

---

MIT License — see [LICENSE](LICENSE)
