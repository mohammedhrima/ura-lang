# Ura

> A compiled, statically-typed language with Python's clean syntax — built on LLVM, written in C.

No braces. No semicolons. Indentation-based like Python, fast like C. Each floor of this tutorial unlocks a new power. By the Boss Room, you'll have a working dungeon game.

---

## See it in action

A recursive-descent calculator: a lexer in its own module, a heap-allocated AST
freed by one `clean`, and a division-by-zero that travels up four call frames
into a `catch`. This is the whole program, nothing elided —
[`tests/projects/calculator.md`](tests/projects/calculator.md), where it also
runs as a test.

```ura
use "@/header"
use "@/error"

enum Type: NONE, NUMBER, ADD, SUB, MUL, DIV, LPAREN, RPAREN

struct Token:
    value i32
    type Type

    pub fn number(v i32) ref? Token:
        ref? t Token = new Token
        t.type = NUMBER
        t.value = v
        ret ref t

    pub fn op(k Type) ref? Token:
        ref? t Token = new Token
        t.type = k
        ret ref t

struct Node:
    ref? token Token
    ref? left Node
    ref? right Node

    pub fn create(ref? token Token) ref? Node:
        ref? n Node = new Node
        n.token = ref token
        ret ref n

    @no-warn
    operator drop:
        clean self.token
        clean self.left
        clean self.right

mod Lexer:
    fn run(ref src String, toks Token[]) i32:
        fn is_digit(c char) bool: ret c >= '0' and c <= '9'
        n i32 = 0
        i i32 = 0
        while i < src.len():
            c char = src.at(i)
            if c == ' ':
                i = i + 1
                continue
            elif is_digit(c):
                v i32 = 0
                while i < src.len() and is_digit(src.at(i)):
                    v = v * 10 + ((src.at(i) as i32) - 48)
                    i = i + 1
                toks[n].type = NUMBER
                toks[n].value = v
                n = n + 1
                continue
            else:
                k Type = NUMBER
                match c:
                    case '+': k = ADD
                    case '-': k = SUB
                    case '*': k = MUL
                    case '/': k = DIV
                    case '(': k = LPAREN
                    case ')': k = RPAREN
                toks[n].type = k
                toks[n].value = 0
                n = n + 1
                i = i + 1
        ret n

mod Parser:
    fn factor(toks Token[], ref p i32, n i32) ref? Node:
        if p < n and toks[p].type == LPAREN:
            p = p + 1
            ref? inner Node = expr(toks, ref p, n)
            if p < n and toks[p].type == RPAREN:
                p = p + 1
            ret ref inner
        ref? lf Node = Node::create(Token::number(toks[p].value))
        p = p + 1
        ret ref lf

    fn term(toks Token[], ref p i32, n i32) ref? Node:
        ref? l Node = factor(toks, ref p, n)
        while p < n and (toks[p].type == MUL or toks[p].type == DIV):
            k Type = toks[p].type
            p = p + 1
            ref? r Node = factor(toks, ref p, n)
            ref? parent Node = Node::create(Token::op(k))
            parent.left = ref l
            parent.right = ref r
            l = ref parent
        ret ref l

    fn expr(toks Token[], ref p i32, n i32) ref? Node:
        ref? l Node = term(toks, ref p, n)
        while p < n and (toks[p].type == ADD or toks[p].type == SUB):
            k Type = toks[p].type
            p = p + 1
            ref? r Node = term(toks, ref p, n)
            ref? parent Node = Node::create(Token::op(k))
            parent.left = ref l
            parent.right = ref r
            l = ref parent
        ret ref l

fn sign(k Type) char[]:
    match k:
        case ADD: ret "+"
        case SUB: ret "-"
        case MUL: ret "*"
        case DIV: ret "/"
        default: ret "?"

fn label(ref? n Node) void:
    if n.token.type == NUMBER: output(n.token.value, "\n")
    else:                      output(sign(n.token.type), "\n")

fn show(ref? n Node, pre char[], last bool) void:
    output(pre)
    if last: output("└─ ")
    else:    output("├─ ")
    label(ref n)
    kid String = String::from(pre)
    if last: kid.join("   ")
    else:    kid.join("│  ")
    if n.left != null:  show(ref n.left, kid.c_str(), n.right == null)
    if n.right != null: show(ref n.right, kid.c_str(), True)

fn tree(ref? n Node) void:
    label(ref n)
    if n.left != null:  show(ref n.left, "", n.right == null)
    if n.right != null: show(ref n.right, "", True)

fn eval(ref? n Node) i32:
    match n?.token?.type:
        case NUMBER: ret n.token.value
        case ADD:    ret eval(ref n.left) + eval(ref n.right)
        case SUB:    ret eval(ref n.left) - eval(ref n.right)
        case MUL:    ret eval(ref n.left) * eval(ref n.right)
        case DIV:
            d i32 = eval(ref n.right)
            if d == 0: throw Error::make("division by zero")
            ret eval(ref n.left) / d
        default:    ret 0


fn calc(text char[]) void:
    output("========================================\n")
    output("calculate expr:  ", text, "\nast:\n")
    src String = String::from(text)
    toks Token[] = new Token[64]
    n i32 = Lexer::run(ref src, toks)
    p i32 = 0
    ref? root Node = Parser::expr(toks, ref p, n)
    tree(ref root)
    try:
        output("result: ", eval(ref root), "\n")
    catch e:
        errput("\nError: ", e.message, "\n")
    clean root
    clean toks


main():
    calc("2 * (3 + 4) - 10 / 5")
    calc("8 / (3 - 3)")
    output("========================================\n")
```

```
calculate expr:  2 * (3 + 4) - 10 / 5
ast:
-
├─ *
│  ├─ 2
│  └─ +
│     ├─ 3
│     └─ 4
└─ /
   ├─ 10
   └─ 5
result: 12
========================================
calculate expr:  8 / (3 - 3)
ast:
/
├─ 8
└─ -
   ├─ 3
   └─ 3

Error: division by zero
```

What that one file uses: `enum` · `struct` · `pub fn` statics called with
`::` · in-file modules (`mod Lexer:`) · `new` / `clean` · `operator drop`
cascading through a tree · `ref?` nullable references · `match` · `try` /
`catch` / `throw` · `ret` · `errput`. No garbage collector, no leaks —
verified with `leaks --atExit`.

---

## Quick Start

```bash
git clone https://github.com/mohammedhrima/ura-lang && cd ura-lang

make build          # compile the ura compiler → build/ura
make dev            # open the task shell (needs uv)
```

`make dev` drops you into the task shell (`uv run config/tasks.py`), where you
type a verb:

| Command | Does |
|---------|------|
| `install` | install LLVM/clang deps (once per machine) |
| `build` | compile the ura compiler → `build/ura` |
| `tests` | run the whole suite (or `tests <group>.md` for one group) |
| `update <group>.md` | regenerate a group's golden output |
| `check` | verify clang + `llvm-config-14` are present |

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
- [Floor 5 — The Battle Loop](#floor-5--the-battle-loop)
- [Floor 6 — Patrol Routes](#floor-6--patrol-routes)
- [Floor 7 — Reading the Runes](#floor-7--reading-the-runes)
- [Floor 8 — The Hero](#floor-8--the-hero)
- [Floor 9 — Hero Abilities](#floor-9--hero-abilities)
- [Floor 10 — Level Up!](#floor-10--level-up)
- [Floor 11 — Empty Chests](#floor-11--empty-chests)
- [Floor 12 — The Dungeon Map](#floor-12--the-dungeon-map)
- [Floor 13 — Status Effects](#floor-13--status-effects)
- [Floor 14 — Gear Score](#floor-14--gear-score)
- [Floor 15 — Power Moves](#floor-15--power-moves)
- [Floor 16 — The Scroll of Words](#floor-16--the-scroll-of-words)
- [Floor 17 — Loot Tables](#floor-17--loot-tables)
- [Floor 18 — The Hero's Identity](#floor-18--the-heros-identity)
- [Floor 19 — Cross-Platform Keep](#floor-19--cross-platform-keep)
- [Floor 20 — C Interop](#floor-20--c-interop)
- [Floor 21 — Command Line](#floor-21--command-line)
- [Floor 22 — Network](#floor-22--network)
- [Floor 23 — Rooms Within Rooms](#floor-23--rooms-within-rooms)
- [Floor 24 — When the Dungeon Fights Back](#floor-24--when-the-dungeon-fights-back)
- [Boss Room — The Full Game](#boss-room--the-full-game)
- [How It Works](#how-it-works)
- [Compiler Reference](#compiler-reference)
- [Status](#status)
- [Requirements](#requirements)
- [Author](#author)

---

## Floor 1 — Hello, Dungeon

Variables are declared `name type = value`. Integers are sized: `i8` `i16` `i32` `i64` and unsigned `u8` `u16` `u32` `u64`. Floats are `f32` `f64`. A single character is `char`; a string is `char[]` (an array of characters). `output` is a built-in — no imports, no format strings, any number of values:

```ura
main():
    hero  char[] = "Aldric"
    hp    i32    = 100
    mp    i32    = 50
    alive bool   = True
    speed f32    = 1.5

    output("=== ", hero, " enters the dungeon ===\n")
    output("HP: ", hp, "  MP: ", mp, "  Speed: ", speed, "\n")
    output("typeof hp: ", typeof(hp), "  sizeof: ", sizeof(hp), "\n")
```

```
=== Aldric enters the dungeon ===
HP: 100  MP: 50  Speed: 1.500000
typeof hp: i32  sizeof: 4
```

`typeof` returns the type name as a string; `sizeof` returns the byte size. For formatted output (`%d`, padding, etc.) use `printf` from C — see Floor 20.

`errput` is `output`'s twin for diagnostics: same arguments, same auto-formatting, but it writes to **stderr** in red. Redirecting stdout leaves it visible.

```ura
main():
    output("normal progress\n")
    errput("something went wrong\n")
```

| | Stream | Colour |
|---|---|---|
| `output(...)` | stdout | plain |
| `errput(...)` | stderr | red |

---

## Floor 2 — Tracking Progress

Global variables live above all functions. Every function in the file can read and write them:

```ura
score i32 = 0
floor i32 = 1
lives i32 = 3

fn earn(pts i32) void:
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

Enums turn raw integers into readable names. Variants count up from `0`, or you can pin a value and let the rest continue from there:

```ura
enum EnemyType:
    ORC,
    SPIDER,
    DRAGON,

enum Dir: NORTH = 1, EAST, SOUTH, WEST

fn threat(e EnemyType) i32:
    match e:
        case DRAGON: return 100
        case ORC:    return 30
        default:     return 10
    return 0

main():
    output("ORC=", ORC, " SPIDER=", SPIDER, " DRAGON=", DRAGON, "\n")
    output("EAST=", EAST, " WEST=", WEST, "\n")
    e EnemyType = DRAGON
    output("threat(DRAGON)=", threat(e), "\n")
```

```
ORC=0 SPIDER=1 DRAGON=2
EAST=2 WEST=4
threat(DRAGON)=100
```

Variants are named `i32` constants — usable anywhere an integer is, including array indices. An enum name is also a real type (`e EnemyType`), and `match` reads best over its variants (Floor 7).

---

## Floor 4 — Combat Math

Functions are `fn name(params) returnType:`. Call them like any language:

```ura
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

```
Orc takes 17 damage — dead: True
4 squared: 16
```

Single-line functions put the body after the colon. Functions and struct methods can be called before they are declared — no forward declarations needed.

`ret` is a shorter spelling of `return`. The two are the same keyword and mix freely in one file; the rest of this tutorial uses `return`, most of the compiler's own `.ura` sources use `ret`.

```ura
fn clamp(val i32, lo i32, hi i32) i32:
    if val < lo: ret lo
    if val > hi: ret hi
    ret val
```

---

## Floor 5 — The Battle Loop

`while` loops, `if`/`elif`/`else` branching, `break`, and `continue` — all in one fight:

```ura
fn simulate(hero_hp i32, orc_hp i32) void:
    round i32 = 0
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
    simulate(100, 60)
    simulate(30,  90)
```

```
Victory in round 4!
Hero fell in round 3
```

| Keyword | Purpose |
|---------|---------|
| `while cond:` | loop while condition holds |
| `loop:` | loop forever (exit with `break`) |
| `break` | exit the loop immediately |
| `continue` | skip to the next iteration |
| `if` / `elif` / `else` | branching |

---

## Floor 6 — Patrol Routes

`for` walks a range or an array. A range `a..b` is exclusive of `b` and counts down when `a > b`; `by` sets the step. `for ref x in arr` aliases each element, so writes go through:

```ura
main():
    for i in 0..5:
        output(i, " ")
    output("| ")

    nums i32[] = [10, 20, 30]
    total i32 = 0
    for x in nums:
        total = total + x
    output("sum=", total, " | ")

    for ref x in nums:
        x = x * 2
    for x in nums:
        output(x, " ")
    output("| ")

    for i in 0..10 by 3:
        output(i, " ")
    output("\n")
```

```
0 1 2 3 4 | sum=60 | 20 40 60 | 0 3 6 9 
```

---

## Floor 7 — Reading the Runes

`match` dispatches on a value — cleaner than an `if`/`elif` ladder. `case` labels can be enum variants or integers; `default` catches the rest:

```ura
enum Tile: FLOOR, WALL, TRAP, EXIT

fn walk(t Tile) i32:
    match t:
        case WALL:
            output("blocked  ")
            return 0
        case TRAP:
            output("ouch     ")
            return -10
        case EXIT:
            output("escape!  ")
            return 100
        default:
            output("step     ")
            return 1
    return 0

main():
    output("gain ", walk(FLOOR), "\n")
    output("gain ", walk(TRAP), "\n")
    output("gain ", walk(EXIT), "\n")
```

```
step     gain 1
ouch     gain -10
escape!  gain 100
```

**One branch, several labels** — list them after `case`, separated by commas:

```ura
fn describe(roll i32) void:
    match roll:
        case 1, 2, 3:
            output("weak    ")
        case 4, 5:
            output("solid   ")
        case 20:
            output("crit!   ")
            break
        default:
            output("ordinary")
    output(" (", roll, ")\n")

main():
    describe(2)
    describe(5)
    describe(20)
    describe(11)
```

```
weak     (2)
solid    (5)
crit!    (20)
ordinary (11)
```

Cases never fall through — no `break` is needed to end one. `break` inside a `match` leaves the `match` early; `continue` belongs to the enclosing loop, not the `match`. Every label must have the subject's type, so an enum subject cannot take an integer case.

---

## Floor 8 — The Hero

Structs group related data. Fields are declared inside the struct body, one per line. Structs are **zero-initialized** by default:

```ura
struct Player:
    name char[]
    hp   i32
    mp   i32

struct Enemy:
    name char[]
    hp   i32
    atk  i32

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

Structs nest to any depth — `hero.stats.hp` reaches straight into a `Stats` field of a `Hero`.

---

## Floor 9 — Hero Abilities

`pub fn` declares a **static method** — no `self`, called with `Type::method()`. Use it as an explicit constructor. A regular method takes an implicit `self`. `operator drop` is the destructor — auto-called when the value goes out of scope:

```ura
struct Hero:
    name char[]
    hp   i32

    pub fn create(name char[], hp i32) Hero:
        h Hero
        h.name = name
        h.hp   = hp
        return h

    fn heal(amount i32) void:
        self.hp = self.hp + amount

    fn announce() void:
        output(self.name, " — HP: ", self.hp, "\n")

    operator drop:
        output("(", self.name, " leaves the dungeon)\n")

main():
    hero Hero = Hero::create("Aldric", 100)
    hero.announce()
    hero.heal(20)
    hero.announce()
```

```
Aldric — HP: 100
Aldric — HP: 120
(Aldric leaves the dungeon)
```

| Concept | Syntax | Notes |
|---------|--------|-------|
| Static method | `pub fn create(...) Hero:` | No `self`, called as `Hero::create(...)` |
| Instance method | `fn heal(amount i32) void:` | Has implicit `self` |
| Destructor | `operator drop:` | Auto-called at scope exit |
| Static dispatch | `Hero::create(...)` | `::` calls a `pub fn` |

The destructor fires from scope exit, `return`, `break` and `continue`. It also fires from `clean` on a heap object (Floor 12) — `clean root` runs `Node.drop`, which is what lets one call free a whole tree. `clean` on an array frees the buffer only.

---

## Floor 10 — Level Up!

A reference is a second name for an existing variable. Mark a parameter `ref` (before the name) and the callee writes straight through to the caller's variable — pass it with `ref` at the call site, no `&`:

```ura
fn level_up(ref level i32, ref hp i32) void:
    level = level + 1
    hp    = hp + 20

main():
    lv i32 = 1
    hp i32 = 100
    level_up(ref lv, ref hp)
    output("lv=", lv, " hp=", hp, "\n")
```

```
lv=2 hp=120
```

A local reference is `ref name type = ref target`. Then `=` **writes through** to the target, and `= ref other` **rebinds** the reference to a new target:

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

```
99 99 88
```

A function can **return a reference** (`fn f() ref T`) so the caller mutates in place. `ref? T` is a nullable reference — it may hold `null`, needed for linked structures:

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
    output("n.v = ", n.v, "\n")
```

```
n.v = 9
```

`ref a == ref b` compares identity (same variable?), and `cur = ref cur.next` walks a linked list.

---

## Floor 11 — Empty Chests

A `T?` type is nullable — it holds a `T` or `null`. Return `null` for "nothing found". Narrow with `!= null`, read through a guard `value?`, or supply a fallback with `??`:

```ura
fn find_weapon(chest i32) char[]?:
    if chest == 1: return "Iron Sword"
    if chest == 2: return "Magic Staff"
    return null

main():
    loot char[]? = find_weapon(1)
    if loot != null:
        output("Found: ", loot?, "\n")
    else:
        output("Chest is empty\n")

    loot = find_weapon(99)
    if loot != null:
        output("Found: ", loot?, "\n")
    else:
        output("Chest is empty\n")

    output("default: ", loot ?? "nothing", "\n")
```

```
Found: Iron Sword
Chest is empty
default: nothing
```

`a ?? b ?? c` chains left to right until a non-null value.

**Nullable and non-nullable are different types.** A `T` widens into a `T?` for free. Coming back the other way, `?` and `??` are not interchangeable — `?` guards a *use*, `??` is what produces a real `T`:

```ura
main():
    sure  char[]  = "Iron Sword"
    maybe char[]? = sure          // widening — always fine

    output(maybe?, "\n")          // ok: guarded read, traps if null
    named char[] = maybe          // error: 'maybe' may be null
    named char[] = maybe?         // error: same — ? does not narrow the type
    named char[] = maybe ?? ""    // ok: ?? is the only way to land in a char[]
```

Checks that could never fire are errors rather than dead code:

| You write | On a `T?` | On a plain `T` |
|---|---|---|
| `x == null` / `x != null` | narrows | **error** — never null |
| `x ?? d` | uses `d` when null | **error** — `d` unreachable |
| `a ?? b ?? c` | chains | the last one must be non-null |
| `x = null` | ok | **error** — needs an optional target |
| `x?` | traps if null | accepted, does nothing |

The same split applies to references: `ref x T` must be bound at its declaration and can never be null, while `ref? x T` may be unbound or `null`, and `x?` traps rather than reading a dangling pointer. That is why a linked-list `next` field is `ref? Node` and a walking cursor is `ref? cur Node` — see Floor 15.

---

## Floor 12 — The Dungeon Map

`new T[n]` allocates a heap array sized at runtime; `clean` frees it and nulls the pointer:

```ura
main():
    map char[] = new char[51]
    i i32 = 0
    while i < 50:
        map[i] = '.'
        i = i + 1
    map[25] = '@'
    map[50] = 0 as char
    output(map, "\n")
    clean map
```

```
.........................@........................
```

**Multi-dimensional arrays** — `new i32[rows][cols]` builds a grid, indexed `[r][c]`. `for row in board` yields each row (an `i32[]`):

```ura
main():
    board i32[][] = new i32[4][3]
    r i32 = 0
    while r < 4:
        c i32 = 0
        while c < 3:
            board[r][c] = r * 3 + c
            c = c + 1
        r = r + 1
    for row in board:
        for v in row:
            output(v, " ")
        output("\n")
    clean board
```

```
0 1 2 
3 4 5 
6 7 8 
9 10 11 
```

`clean` nulls the pointer, so a later guarded index `board[r]?` traps instead of reading freed memory.

**One object, not an array.** Drop the `[n]` and `new T` allocates a single zeroed struct on the heap and hands back its address. It binds to a nullable reference (`ref? T`), which is what lets a function build a node and return it:

```ura
struct Node:
    value i32
    ref? left  Node
    ref? right Node

    pub fn create(v i32) ref? Node:
        ref? n Node = new Node
        n.value = v
        ret ref n

    @no-warn
    operator drop:
        clean self.left        // null-safe, so leaves are fine
        clean self.right

main():
    ref? root Node = Node::create(1)
    ref? l Node = Node::create(2)
    ref? r Node = Node::create(3)
    root.left  = ref l
    root.right = ref r
    clean root                 // frees all three
    output(root == null, "\n")
```

```
True
```

`clean x` on a heap object does three things in order: run `operator drop`, `free` the memory, then set `x` to `null`. That ordering is what makes the tree above collapse from one call — each `drop` cleans its children first. It is also idempotent: `clean` on an unbound or already-cleaned reference is a no-op, and a guarded read `x?` afterwards traps instead of touching freed memory.

| | `new T[n]` | `new T` |
|---|---|---|
| Result | array (fat pointer + length) | one zeroed object |
| Binds to | `T[]` | `ref? T` |
| `clean` | frees the buffer | `operator drop` → `free` → null |

`ref` binds to a variable, not to an expression — `root.left = ref make()` is rejected, so store the call in a local first. `@no-warn` above `operator drop` is Floor 19.

---

## Floor 13 — Status Effects

Bitwise operators pack multiple flags into one integer. Arithmetic, comparison, logical, and assignment operators all work as expected:

```ura
POISONED i32 = 1
FROZEN   i32 = 2
BURNING  i32 = 4
SHIELDED i32 = 8

fn has(status i32, flag i32) bool:  return (status & flag) != 0
fn add(status i32, flag i32) i32:   return status | flag
fn rem(status i32, flag i32) i32:   return status & ~flag

main():
    s i32 = 0
    s = add(s, POISONED)
    s = add(s, BURNING)
    output("Poisoned: ", has(s, POISONED), "\n")
    output("Frozen:   ", has(s, FROZEN),   "\n")
    output("Burning:  ", has(s, BURNING),  "\n")
    s = rem(s, POISONED)
    output("Cured:    ", has(s, POISONED), "\n")
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
| Comparison | `==` `!=` `<` `>` `<=` `>=` — `is` is a synonym for `==` |
| Logical    | `and` `or` `not`, or the symbols `&&` `\|\|` `!` |
| Assignment | `=` `+=` `-=` `*=` `/=` `%=` `&=` `\|=` `^=` `<<=` `>>=` |

---

## Floor 14 — Gear Score

`as` casts between numeric, pointer, and float↔int type pairs. The ASCII table is just an `i32 → char` cast away:

```ura
main():
    score     i32 = 847
    max_score i32 = 1000
    pct f64 = score as f64 / max_score as f64 * 100.0
    output("Score: ", score, "/", max_score, "  Rating: ", pct, "\n")

    grade char = 'A'
    code  i32  = grade as i32
    back  char = (code + 1) as char
    output("Grade: ", grade, " → ", code, " → ", back, "\n")

    diff i32 = 'd' as i32 - 'a' as i32
    output("'d' - 'a' = ", diff, "\n")
```

```
Score: 847/1000  Rating: 84.700000
Grade: A → 65 → B
'd' - 'a' = 3
```

---

## Floor 15 — Power Moves

Structs can define their own behaviour for any operator: `operator OP(param type) returnType:` inside the struct body. The parameter's type is part of the overload name, so one operator can have several overloads:

```ura
struct Vec2:
    x i32
    y i32

    pub fn create(a i32, b i32) Vec2:
        v Vec2
        v.x = a
        v.y = b
        return v

    operator +(other Vec2) Vec2:
        return Vec2::create(self.x + other.x, self.y + other.y)

    operator ==(other Vec2) bool:
        return self.x == other.x and self.y == other.y

    fn show() void:
        output("(", self.x, ", ", self.y, ")\n")

main():
    a Vec2 = Vec2::create(1, 2)
    b Vec2 = Vec2::create(10, 20)
    c Vec2 = a + b
    c.show()
    output("a == a  ", a == a, "\n")
    output("a == b  ", a == b, "\n")
```

```
(11, 22)
a == a  True
a == b  False
```

The full overload set: `=` `+` `-` `*` `/` `%` `==` `!=` `<` `>` `<=` `>=` `+=` `-=` `*=` `/=` `%=` `&` `|` `^` `<<` `>>`. Declare `operator =(ref other T)` to define copy assignment (deep-copy an owned buffer), and `operator drop` to release it — see the heap-owning `String` in the standard library.

---

## Floor 16 — The Scroll of Words

The standard library ships a heap-backed `String`. `use "@/header"` (or `use "@/string"`) imports it. Construct with `String::create()`, `String::from()`, or `String::from_int()`; it frees itself at scope exit:

```ura
use "@/header"

main():
    greeting String = String::from("Hello, Dungeon!")
    output(greeting.value, " (len ", greeting.len(), ")\n")
    output("find 'Dungeon': ", greeting.find("Dungeon"), "\n")

    shout String = greeting.upper()
    output(shout.value, "\n")

    word String = greeting.substr(7, 7)
    output("substr: ", word.value, "\n")

    msg String = String::create()
    msg.push('H')
    msg.push('P')
    output("pushed: ", msg.value, "\n")

    first String = String::from("Shadow")
    last  String = String::from(" Blade")
    name  String = first + last
    output("name: ", name.value, "\n")

    a String = String::from("abc")
    b String = String::from("abc")
    output("equal: ", a == b, "\n")

    n String = String::from_int(100)
    output("from_int: ", n.value, "\n")
```

```
Hello, Dungeon! (len 15)
find 'Dungeon': 7
HELLO, DUNGEON!
substr: Dungeon
pushed: HP
name: Shadow Blade
equal: True
from_int: 100
```

**String API:**

| Category | Methods |
|----------|---------|
| Constructors | `String::create()` `String::from(str)` `String::from_int(n)` |
| Accessors | `.len()` `.empty()` `.at(i)` `.c_str()` |
| Mutators | `.assign(str)` `.join(str)` `.push(c)` `.pop()` `.clear()` |
| Search | `.find(str)` `.contains(str)` `.starts_with(str)` `.ends_with(str)` |
| Transform | `.substr(i, n)` `.upper()` `.lower()` `.trim()` `.replace(old, new)` `.repeat(n)` `.reverse()` |
| Convert | `.to_int()` `.compare(ref other)` |
| Operators | `=` `+` `+=` `==` `!=` `<` `>` (against `String` and `char[]`) |

---

## Floor 17 — Loot Tables

Array literals initialize an array inline with `[value, value, ...]`. A string is a `char[]`, so an array of strings is `char[][]`:

```ura
main():
    enemies char[][] = ["Orc", "Spider", "Dragon"]
    loot    i32[]    = [15, 30, 100]
    output("Enemies: ", enemies[0], ", ", enemies[1], ", ", enemies[2], "\n")
    output("Loot:    ", loot[0], ", ", loot[1], ", ", loot[2], " gold\n")
    roll i32 = 4
    output("Roll ", roll, ": ", enemies[roll % 3], "\n")
```

```
Enemies: Orc, Spider, Dragon
Loot:    15, 30, 100 gold
Roll 4: Spider
```

Elements can be any expression — variables, arithmetic, or function calls.

---

## Floor 18 — The Hero's Identity

A struct prints as `Name{ field: value, ... }` by default. Define `operator output()` to control how it appears inside `output()`. Return `char[]`, or a `String` when you want to build the text with `+`:

```ura
struct Tag:
    name char[]
    n    i32

    operator output() char[]:
        return self.name

struct Plain:
    a i32

main():
    t Tag
    t.name = "custom"
    t.n    = 7
    output("direct: ", t, "\n")

    p Plain
    p.a = 1
    output("plain:  ", p, "\n")
```

```
direct: custom
plain:  Plain{a: 1}
```

| Return type | Behavior |
|-------------|----------|
| `char[]` | used directly |
| `String` | its text is used |
| *(not defined)* | default `Name{ field: value, ... }` printing |

Any other struct return type is not supported today — the printer reads the
returned value as a `char[]`, so an unrelated struct crashes at runtime.

---

## Floor 19 — Cross-Platform Keep

`@if` / `@elif` / `@else` gate code by **target platform** at compile time — the branch that does not match the build target is removed before parsing. Tags: `unix`, `macos`, `linux`, `windows`. Use it to pick the right `proto` per OS:

```ura
@if unix:
    proto fn getpid() i32

main():
    @if macos:
        output("building for macOS\n")
    @elif linux:
        output("building for Linux\n")
    @else:
        output("building for another platform\n")

    @if unix:
        output("pid > 0: ", getpid() > 0, "\n")
```

```
building for macOS
pid > 0: True
```

*(Output is from a macOS build; a Linux build prints the `@elif linux` branch.)* The platform is detected from the LLVM target triple, so no `-D` flags are needed.

**`@no-warn`** silences the warning attached to the declaration directly below it — and only that one. The usual case is a struct that owns heap memory through a `ref?` field, where `operator drop` is deliberate and `operator =` is not wanted:

```ura
struct Node:
    ref? next Node

    @no-warn
    operator drop:
        clean self.next
```

Without it the compiler warns *"Struct Node has 'operator drop' but no 'operator ='; copying it makes two owners of the same memory, and both will be destroyed."* Errors are never silenced — only warnings, and only for the next declaration.

---

## Floor 20 — C Interop

`use "@/module"` imports from the standard library; `@` resolves to `$URA_LIB`, and `use "@/header"` pulls everything at once:

```ura
use "@/io"       // printf, puts, fopen, fclose, write, read, ...
use "@/memory"   // malloc, free, realloc, ...
use "@/string"   // strlen, strcmp, strcpy, strcat, String struct, ...
use "@/stdlib"   // atoi, rand, srand, exit, ...
use "@/math"     // sin, cos, sqrt, pow, ...
use "@/net"      // socket, bind, listen, connect, send, recv, ...
use "@/error"    // the throwable Error struct — see Floor 24
use "@/header"   // every module at once (also wires up os.argc / os.argv)
```

For full control, declare any C function with `proto`. Variadic functions take `...`:

```ura
proto fn strlen(s pointer) i64
proto fn printf(fmt pointer, ...) i32

main():
    n i32 = strlen("dungeon") as i32
    printf("length: %d\n", n)
```

```
length: 7
```

Pull in the whole C stdlib for a random encounter:

```ura
use "@/header"

fn random_encounter() char[]:
    r i32 = rand() % 4
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
Encounter 1: Stone Golem
Encounter 2: Shadow Wraith
Encounter 3: Orc Shaman
```

**Linking external libraries** — set an environment variable, then `use` the module. The compiler reads `URA_LINK_<name>` and passes it to the linker:

```bash
export URA_LINK_raylib="/opt/homebrew/lib/libraylib.a"
```

```ura
use "@/raylib"
```

---

## Floor 21 — Command Line

`os.argc` and `os.argv` are populated before `main()` when you `use "@/header"`:

```ura
use "@/header"

main():
    if os.argc < 2:
        output("Usage: dungeon <hero_name>\n")
    else:
        name char[] = os.argv[1]
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

## Floor 22 — Network

POSIX sockets through `@/net` (bundled in `@/header`). A minimal listener — bind, listen, accept:

```ura
use "@/header"

main():
    fd i32 = socket(2, 1, 0)   // AF_INET, SOCK_STREAM
    if fd < 0:
        output("socket failed\n")
    else:
        addr char[] = new char[16]
        addr[0] = 16 as char
        addr[1] = 2  as char
        addr[2] = (8080 >> 8)  as char
        addr[3] = (8080 & 255) as char
        bind(fd, addr, 16)
        listen(fd, 5)
        output("listening on :8080\n")
        clean addr
```

Full server + client examples live in [github.com/mohammedhrima/ura-tcp-server](https://github.com/mohammedhrima/ura-tcp-server).

---

## Floor 23 — Rooms Within Rooms

`use` brings in another *file*. `mod` carves a namespace inside the file you are already writing. Members are reached with `::`, the same operator that calls a `pub fn`:

```ura
mod Lexer:
    fn run(text char[]) i32:
        return text.len as i32

mod Shapes:
    enum Kind: CIRCLE, SQUARE

    struct Point:
        x i32
        y i32

        pub fn make(v i32) Point:
            p Point
            p.x = v
            return p

main():
    output(Lexer::run("2 * (3 + 4)"), "\n")
    p Shapes::Point = Shapes::Point::make(3)
    k Shapes::Kind  = Shapes::CIRCLE
    output(p.x, " ", k == Shapes::CIRCLE, "\n")
```

```
11
3 True
```

Modules nest (`a::b::f()`), may be used before they are declared, and hold `fn`, `struct`, `enum` and further `mod` declarations — but nothing else; a bare statement in a module body is an error. Inside a module, siblings are called unqualified: `Parser::term` can just say `factor(...)`. And `::` is not `.` — reaching a module member with a dot is rejected.

| | Reaches |
|---|---|
| `use "path"` | another file |
| `mod name:` + `name::x` | a namespace in this file |
| `Type::method()` | a `pub fn` static |
| `value.method()` | an instance method |

---

## Floor 24 — When the Dungeon Fights Back

A `throw` unwinds to the nearest enclosing `catch`, across as many call frames as it takes. The only throwable type is `Error`, from `@/error`:

```ura
use "@/error"

fn divide(a i32, b i32) i32:
    if b == 0: throw Error::make("division by zero")
    return a / b

fn compute(a i32, b i32) i32:
    return divide(a, b) * 2      // no try here — it just propagates

main():
    try:
        output("ok: ", compute(10, 5), "\n")
        output("bad: ", compute(8, 0), "\n")
        output("never reached\n")
    catch e:
        errput("caught: ", e.message, "\n")
```

```
ok: 4
```
```
caught: division by zero
```

*(the second block is stderr, in red)*

`Error` is an ordinary struct — `message char[]` plus a `pub fn make` — so `e.message` is just a field read. Code after a `throw` in the same block is unreachable and dropped. `try` blocks nest, and the innermost `catch` wins. `return` inside a `try` returns from the function; `break` exits the enclosing loop. A throw that reaches `main` uncaught prints its message and exits non-zero.

**Runtime traps are not exceptions.** These abort with a `file:line` message and are not catchable:

| Trap | Cause |
|---|---|
| index out of bounds | `a[9]?` past the end |
| slice out of bounds | `a[lo..hi]?` past the end |
| use after `clean` | reading a freed array or object |
| unbound `ref?` | `x?` on a reference that was never bound |
| divide by zero | integer **and** float |
| null function value | calling a zero-init `fn` variable |
| stack overflow | runaway recursion |

---

## Boss Room — The Full Game

All the skills. One program. The complete listing lives in [tests/dungeon.md](tests/dungeon.md), where the test suite verifies it:

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

```
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

---

## How It Works

The compiler is written entirely in C. LLVM handles only the final step — turning architecture-independent IR into native code — so ura targets any platform LLVM supports with zero extra work.

### Compilation Pipeline

```
source.ura
    │
    ▼  lexer        source text → flat list of tokens
    │
    ▼  @if pass     remove code for non-target platforms
    │
    ▼  parser       recursive descent → typed AST
    │
    ▼  analyze      name resolution, scopes, enum/struct declaration
    │
    ▼  typecheck    type checking, operator-overload dispatch
    │
    ▼  codegen      LLVM IR emission via the LLVM C API → build/file.ll
    │
    ▼  clang        .ll → native executable
```

### Source Files

- **`src/main.c`** — the `compile()` pipeline, tokenizer driver, platform detection, and CLI argument parsing.
- **`src/frontend/lexer.c`** — `lex_*` helpers; turns source text into tokens, plus the `@if` preprocessing pass.
- **`src/frontend/parser.c`** — recursive descent parser (`*_node` helpers) building the typed AST.
- **`src/backend/analyze.c`** — the resolution pass: scopes, name binding, enum/struct declaration.
- **`src/backend/typecheck.c`** — type checking and operator-overload dispatch.
- **`src/backend/codegen.c`** — LLVM IR emission (`code_gen_*` per node kind).
- **`src/frontend/header.h`, `src/backend/header.h`** — `Token`, `Type` enum, `UraGlobal`, and shared prototypes.

---

## Compiler Reference

### Usage

```bash
ura <file.ura> [options]
ura dungeon.ura -O2 -o dungeon
```

### Options

| Flag | Description |
|------|-------------|
| `-o <name>` | Output name (default: source basename) |
| `-O0` … `-O3`, `-Os`, `-Oz` | LLVM optimization level |
| `-san` | UndefinedBehavior/Address sanitizer + debug info |
| `-tree` | Print the typed AST and stop |
| `-debug` | Verbose compiler diagnostics |

### Building & the task shell

```bash
make build      # compile the compiler → build/ura
make release    # -O2 build
make dev        # open the task shell: uv run config/tasks.py
```

Inside `make dev` the verbs are `check`, `install`, `build`, `tests`
(`tests <group>.md` runs one group), `update <group>.md`, `show`, `index`,
`migrate`, `doctor`. The task shell needs [uv](https://github.com/astral-sh/uv).

---

## VS Code Extension

Syntax highlighting for `.ura` files — keywords, types, operators, strings, comments, `pub fn`, `operator`, `@if` directives, and `Type::method()` static dispatch.

```bash
git clone https://github.com/mohammedhrima/ura-vscode-extension
cd ura-vscode-extension && npm install && npm run package
code --install-extension ura-lang-*.vsix
```

---

## Status

**Working today:**
- Sized integers `i8` `i16` `i32` `i64`, unsigned `u8` `u16` `u32` `u64`, floats `f32` `f64`, plus `char`, `char[]`, `bool`, `void`, `pointer`
- `null`, nullable types `T?`, `??` default, guarded deref `value?`
- Enums — named `i32` constants, explicit values, enum-typed variables
- Global variables
- Functions, variadic functions, single-line functions, call-before-declare
- `while` / `loop`, `if` / `elif` / `else`, `break`, `continue`
- `for` loops — `for i in a..b`, `by` step, `for x in arr`, `for ref x in arr`
- `match` / `case` / `default` (enum or integer cases, `case 1, 2, 3` multi-labels, no fall-through)
- Structs, nested structs, methods with `self`
- `pub fn` static methods via `Type::method()`
- References — `ref` params, local refs (`= ref` rebind, `=` write-through), **reference returns** `fn f() ref T`, nullable refs `ref? T`, identity `ref a == ref b`
- Operator overloading — `=` `+` `-` `*` `/` `%` `==` `!=` `<` `>` `<=` `>=` `+=` `-=` `*=` `/=` `%=` `&` `|` `^` `<<` `>>`
- `operator =` copy assignment and `operator drop` destructor (fires on scope exit, `return`, `break`, `continue`)
- `operator output()` — custom struct printing (returns `char[]` or `String`)
- Built-in `String` — constructors, search, transforms, operators
- Array literals `[1, 2, 3]`, multi-dimensional `new T[r][c]`
- `new` / `clean` heap allocation
- Type casting `as` — numeric, pointer, float↔int, char↔int
- `typeof`, `sizeof`
- `@if` / `@elif` / `@else` platform conditional compilation (`unix`/`macos`/`linux`/`windows`), `@no-warn`
- `output` / `errput` builtins — no format string, no imports
- `try` / `catch` / `throw` with the throwable `Error` struct (`@/error`)
- `mod name:` in-file modules, nested, called with `::`
- `use` module system (`@` for stdlib, relative for local files)
- `proto` C interop, variadic supported, `URA_LINK_<name>` external linking
- `os.argc` / `os.argv`
- LLVM optimization passes (`-O0` → `-O3`, `-Os`, `-Oz`), sanitizers (`-san`)

**Coming next:**
- Tuples — multi-value returns with destructuring (`a i32, b i32 = f()`)
- `List[T]`, `Map[K, V]`, `Set[T]` container types
- String interpolation
- `const` / immutability, type inference (`:=`), type aliases
- Function overloading, default parameter values, named returns
- Generics

---

## Requirements

| Platform | What you need |
|----------|--------------|
| macOS | Homebrew + `brew install llvm@14` |
| Linux | `clang-14` + `llvm-14` + `llvm-14-tools` |
| Windows | MSYS2 MinGW64 + `mingw-w64-x86_64-llvm` + clang |

Run `install` from the task shell (`make dev`) to install everything automatically.

> **Why LLVM 14?** It uses explicit pointer types (`i32*`) in IR instead of the opaque `ptr` from LLVM 15+, making the generated `.ll` files much easier to read and debug.

---

## Author

**Mohammed Hrima** — [linkedin.com/in/mohammedhrima](https://www.linkedin.com/in/mohammedhrima) — mohammed.hrima1998@gmail.com

---

MIT License — see [LICENSE](LICENSE)