# Ura

> A compiled, statically-typed language with Python's clean indentation syntax — built on LLVM and written entirely in C.

Ura started as a passion project: a love for C's raw performance and Python's readable, indentation-based syntax. It compiles directly to native code through LLVM. No braces. No semicolons. Just clean, fast code.

---

## Table of Contents

- [Why Ura?](#why-ura)
- [Quick Start](#quick-start)
- [Language Tour](#language-tour)
  - [Imports](#imports)
  - [Variables](#variables)
  - [Globals](#globals)
  - [Functions](#functions)
  - [Tuples](#tuples)
  - [Control Flow](#control-flow)
  - [Structs](#structs)
  - [References](#references)
  - [Memory](#memory)
  - [Operators](#operators)
  - [Type Casting](#type-casting)
  - [Type Introspection](#type-introspection)
  - [C Interoperability](#c-interoperability)
  - [OS Module](#os-module)
  - [Networking](#networking)
  - [Output Builtin](#output-builtin)
- [The Dungeon — Real Programs](#the-dungeon--real-programs)
- [How It Works](#how-it-works)
  - [Compilation Pipeline](#compilation-pipeline)
  - [Source Files](#source-files)
  - [Why LLVM?](#why-llvm)
- [Compiler Reference](#compiler-reference)
  - [Usage](#usage)
  - [Options](#options)
  - [Build Commands](#build-commands)
  - [config.sh](#configsh)
  - [setup.sh / setup.ps1](#setupsh--setupps1)
- [Project Structure](#project-structure)
- [Status](#status)
- [Requirements](#requirements)
- [Author](#author)
- [License](#license)

---

## Why Ura?

- **Fast** — compiles to native machine code via LLVM
- **Clean syntax** — indentation-based, no braces, no semicolons
- **C interop** — call any C library with a single `proto` declaration
- **Explicit memory control** — choose stack or heap allocation per variable
- **Familiar** — if you know C or Python, you'll feel at home immediately

---

## Quick Start

```bash
# 1. Clone
git clone https://github.com/mohammedhrima/ura-lang
cd ura-lang

# 2. Install all dependencies (once per machine)
bash config/setup.sh          # macOS / Linux
# Windows: run config/setup.ps1 in PowerShell as admin, then open MSYS2

# 3. Load the environment (every session)
source config.sh

# 4. Build the compiler
build

# 5. Write and run your first program
ura src/file.ura
```

**Hello World:**

```ura
main():
    output("Hello, World!\n")
```

No imports needed — `output` is a built-in.

---

## Language Tour

The examples below follow one story: building a dungeon crawler step by step. Each section adds a new feature. By the end, you have a fully fledged game.

---

### Imports

Use `use` to import modules. The `@` prefix resolves to the built-in standard library (`$URA_LIB`):

```ura
use "@/io"        // printf, puts, file I/O, ...
use "@/memory"    // malloc, free, ...
use "@/string"    // strlen, strcmp, strcpy, ...
use "@/stdlib"    // atoi, rand, exit, ...
use "@/time"      // time, clock, difftime, ...
use "@/signals"   // signal, raise, ...
use "@/net"       // socket, bind, listen, accept, connect, send, recv, ...
use "@/os"        // os.argc, os.argv
use "@/header"    // imports all of the above at once
```

Most examples only need `output()`, which is built-in and requires no import. Use `use "@/header"` when you need C functions (`printf`, `strcpy`, `rand`, etc.) or the OS module.

---

### Variables

Every hero needs stats. Variables are declared as `name type = value`:

```ura
main():
    name  chars = "Aldric"
    hp    int   = 100
    mp    int   = 50
    alive bool  = True
    speed float = 1.5
    output("=== ", name, " enters the dungeon ===\n")
    output("HP: ", hp, " | MP: ", mp, " | Speed: ", speed, "\n")
```

```
=== Aldric enters the dungeon ===
HP: 100 | MP: 50 | Speed: 1.5
```

---

### Globals

The dungeon keeps track of progress across the whole run:

```ura
score int = 0
floor int = 1
lives int = 3

main():
    output("Floor ", floor, " | Score: ", score, " | Lives: ", lives, "\n")
    score = score + 100
    output("After first kill — Score: ", score, "\n")
```

```
Floor 1 | Score: 0 | Lives: 3
After first kill — Score: 100
```

Global variables are declared at the top level, before any function. They are visible to every function in the file.

---

### Functions

Time to add combat math. `clamp` keeps damage in range, `damage` uses it, `is_dead` checks the result:

```ura
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

```
Orc takes 17 damage
Orc dead: False
```

Single-line functions:

```ura
fn square(n int) int:     return n * n
fn is_digit(c char) bool: return c >= '0' and c <= '9'
```

---

### Tuples

Some dungeon chests drop both an item and a gold value. Tuples let one function return both:

```ura
fn roll_loot(floor int) (chars, int):
    item  chars = "Iron Sword"
    value int   = floor * 15
    return item, value

main():
    item chars, gold int = roll_loot(3)
    output("Loot: ", item, " worth ", gold, " gold\n")
```

```
Loot: Iron Sword worth 45 gold
```

Return multiple values by listing types in parentheses: `(chars, int)`. Unpack them at the call site with a matching declaration: `item chars, gold int = roll_loot(3)`.

---

### Control Flow

The heart of every fight — a round-by-round battle loop:

```ura
fn simulate_combat(hero_hp int, orc_hp int) void:
    round int = 0
    while hero_hp > 0:
        if orc_hp <= 0:
            output("Victory in round ", round, "!\n")
            return
        hero_hp = hero_hp - 10
        orc_hp  = orc_hp  - 18
        round   = round + 1
        if round > 20:
            output("Timeout — stalemate\n")
            return
    output("Hero fell in round ", round, "\n")

main():
    simulate_combat(100, 60)
```

```
Victory in round 4!
```

| Keyword | Purpose |
|---------|---------|
| `if` / `elif` / `else` | Branching |
| `while` | Loop while condition holds |
| `break` | Exit the loop immediately |
| `continue` | Skip to the next iteration |
| `return` | Exit a function (with or without a value) |

---

### Structs

The hero and every enemy are their own struct. No methods yet — just data:

```ura
struct Player:
    name  chars
    hp    int
    mp    int

struct Enemy:
    name  chars
    hp    int
    atk   int

main():
    hero  Player
    orc   Enemy
    hero.name = "Aldric"
    hero.hp   = 100
    hero.mp   = 50
    orc.name  = "Orc Grunt"
    orc.hp    = 60
    orc.atk   = 14
    output(hero.name, " faces ", orc.name, "\n")
    output("Hero HP: ", hero.hp, " | Orc HP: ", orc.hp, "\n")
```

```
Aldric faces Orc Grunt
Hero HP: 100 | Orc HP: 60
```

**Struct methods** — add behaviour directly to the type. The special `fn init()` runs automatically at declaration; `fn clean()` runs at scope exit:

```ura
use "@/header"

struct Hero:
    name  chars
    title chars

    fn init() void:
        self.name  = heap[char](32)
        self.title = heap[char](32)

    fn setup(name chars, title chars) void:
        strcpy(self.name,  name)
        strcpy(self.title, title)

    fn announce() void:
        output(self.title, " ", self.name, " enters!\n")

    fn clean() void:
        free(self.name)
        free(self.title)

main():
    hero Hero
    hero.setup("Aldric", "The Bold")
    hero.announce()
```

```
The Bold Aldric enters!
```

`self` inside a method refers to the current instance. `init()` is a constructor (auto-called on declaration). `clean()` is a destructor (auto-called on scope exit — no manual free needed at the call site).

**Nested structs** are supported at any depth:

```ura
struct Address:
    city   chars
    street chars

struct Person:
    name    chars
    address Address

main():
    dev Person
    dev.name           = "Mohammed"
    dev.address.city   = "Casablanca"
    dev.address.street = "Rue des Compilateurs"
    output(dev.name, " lives in ", dev.address.city, "\n")
```

---

### References

Surviving a dungeon floor earns a level-up. References let `level_up` modify the caller's variables directly — no copies, no return value needed:

```ura
fn level_up(level int ref, hp int ref, mp int ref) void:
    level = level + 1
    hp    = hp + 20
    mp    = mp + 10
    output("Level up! Now level ", level, "\n")

main():
    lv int = 1
    hp int = 100
    mp int = 50
    level_up(lv, hp, mp)
    output("Stats: HP=", hp, " MP=", mp, "\n")
```

```
Level up! Now level 2
Stats: HP=120 MP=60
```

A reference parameter is marked `ref` after the type. The caller passes the variable as-is — no `&` needed. All writes inside the function go directly to the original.

References can also be declared locally:

```ura
main():
    a int = 1
    r int ref = a   // must bind at declaration
    r = 100         // modifies a
    output(a, "\n") // 100
```

---

### Memory

The dungeon map lives on the heap — size determined at runtime, hand-crafted tile by tile:

```ura
use "@/header"

main():
    map  chars = heap[char](50)
    i    int   = 0
    while i < 49:
        map[i] = '.'
        i = i + 1
    map[24] = '@'
    map[49] = 0 as char
    output("Map: ", map, "\n")
    output("Hero '@' at centre — memory freed\n")
    free(map)
```

```
Map: ........................@........................
Hero '@' at centre — memory freed
```

| | `stack[type](n)` | `heap[type](n)` |
|---|---|---|
| Cleanup | Automatic | Manual (`free`) |
| Speed | Faster | Slightly slower |
| Capacity | Limited | Large |
| Lifetime | Function scope | Until `free()` |

```ura
// Stack allocation — freed automatically when the function returns
local chars = stack[char](64)

// Heap allocation — you control when it's freed
buffer chars = heap[char](256)
free(buffer)
```

You can also use the `array` type annotation for clarity:

```ura
str array[char] = heap[char](20)
str[0] = 'H'
str[1] = 'i'
```

---

### Operators

Status effects are packed into a single integer using bit flags. One `int` holds four conditions at once:

```ura
POISONED int = 1
FROZEN   int = 2
BURNING  int = 4
SHIELDED int = 8

fn has_effect(status int, effect int) bool:
    return (status & effect) != 0

fn apply(status int, effect int) int:
    return status | effect

fn remove(status int, effect int) int:
    return status & ~effect

main():
    status int = 0
    status = apply(status, POISONED)
    status = apply(status, BURNING)
    output("Poisoned: ", has_effect(status, POISONED), "\n")
    output("Frozen:   ", has_effect(status, FROZEN),   "\n")
    output("Burning:  ", has_effect(status, BURNING),  "\n")
    status = remove(status, POISONED)
    output("Cured — Poisoned: ", has_effect(status, POISONED), "\n")
```

```
Poisoned: True
Frozen:   False
Burning:  True
Cured — Poisoned: False
```

| Category   | Operators                             |
|------------|---------------------------------------|
| Arithmetic | `+`  `-`  `*`  `/`  `%`              |
| Bitwise    | `&`  `\|`  `^`  `~`  `<<`  `>>`      |
| Comparison | `==` `!=` `<` `>` `<=` `>=`          |
| Logical    | `and` `or` `not` `&&` `\|\|` `!`     |
| Assignment | `=` `+=` `-=` `*=` `/=` `%=`         |
| Cast       | `as`                                  |

---

### Type Casting

The end-of-floor scoreboard converts the raw score to a percentage. `as` handles the conversion:

```ura
main():
    score     int = 847
    max_score int = 1000
    pct float = score as float / max_score as float * 100.0
    output("Score: ", score, " / ", max_score, "\n")
    output("Percentage: ", pct, "%\n")
    grade char = 'A'
    code  int  = grade as int
    output("'A' as int = ", code, "\n")
```

```
Score: 847 / 1000
Percentage: 84.7%
'A' as int = 65
```

---

### Type Introspection

Need to debug the hero's stat block at runtime? `typeof` and `sizeof` expose the type system:

```ura
main():
    hp    int   = 100
    speed float = 1.5
    name  chars = "Aldric"
    output("typeof hp:    ", typeof(hp),    "\n")
    output("typeof speed: ", typeof(speed), "\n")
    output("typeof name:  ", typeof(name),  "\n")
    output("sizeof name:  ", sizeof(name),  "\n")
```

```
typeof hp:    INT
typeof speed: FLOAT
typeof name:  CHARS
sizeof name:  8
```

---

### C Interoperability

Random enemy encounters use `rand()` from the C standard library — brought in with `use "@/header"`:

```ura
use "@/header"

fn random_encounter() chars:
    roll int = rand() % 4
    if roll == 0:
        return "Giant Spider"
    if roll == 1:
        return "Orc Shaman"
    if roll == 2:
        return "Stone Golem"
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

For full control, declare any C function yourself with `proto`:

```ura
proto fn strlen(s chars) int
proto fn write(fd int, ptr chars, len int) int
proto fn printf(format chars, ...) int   // variadic

main():
    n int = strlen("hello")
    write(1, "hi\n", 3)
    printf("length: %d\n", n)
```

The standard library modules wrap the most common ones:

```ura
use "@/io"       // printf, puts, fopen, fclose, write, read, ...
use "@/memory"   // malloc, heap, stack, free, realloc, ...
use "@/string"   // strlen, strcmp, strcpy, strcat, strdup, ...
use "@/stdlib"   // atoi, rand, exit, ...
use "@/time"     // time, clock, difftime, ...
use "@/signals"  // signal, raise, ...
use "@/net"      // socket, bind, listen, accept, connect, send, recv, ...
use "@/header"   // imports all of the above at once
```

---

### OS Module

The player's name comes from the command line. `os.argc` and `os.argv` are available automatically when you `use "@/header"`:

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

`os` is a global `Os` struct — `os.argc` is the argument count, `os.argv` is the argument array. Both are auto-populated by the runtime before `main()` runs.

---

### Networking

Ura provides direct access to POSIX socket APIs through the `@/net` module:

```ura
use "@/header"

main():
    server_fd int = socket(2, 1, 0)  // AF_INET, SOCK_STREAM, 0
    if server_fd < 0:
        output("socket failed\n")
        return

    port int = 8080
    addr chars = heap[char](16)
    addr[0] = 16 as char             // length
    addr[1] = 2 as char              // sin_family (AF_INET = 2)
    addr[2] = (port >> 8) as char    // sin_port high byte
    addr[3] = (port & 255) as char   // sin_port low byte

    bind(server_fd, addr, 16)
    listen(server_fd, 5)
    output("listening on port 8080...\n")

    buf chars = heap[char](1024)
    client_fd int = accept(server_fd, 0 as chars, 0)
    if client_fd >= 0:
        r int = read(client_fd, buf, 1023)
        if r > 0:
            output("received: ", buf, "\n")
            write(client_fd, "hello from server\n", 18)
        close(client_fd)

    free(buf)
    free(addr)
```

**Real-world TCP examples** are in `tests/projects/ura-tcp-server/`:

- **basic/** — Simple chat room with bidirectional messaging
- **cmd/** — Command-based server supporting `/help`, `/time`, `/whoami`, `/exit`
- **utils.ura** — Shared `SockAddr` struct and timestamped logging

```bash
# Terminal 1
ura tests/projects/ura-tcp-server/basic/server.ura

# Terminal 2
ura tests/projects/ura-tcp-server/basic/client.ura
```

---

### Output Builtin

`output` is a built-in that prints any number of values to stdout — no format string, no import needed:

```ura
main():
    name chars = "Ura"
    n    int   = 42
    flag bool  = True

    output("hello\n")                       // hello
    output("name: ", name, "\n")            // name: Ura
    output("n = ", n, "\n")                 // n = 42
    output("flag: ", flag, "\n")            // flag: True
    output(n, " * 2 = ", n * 2, "\n")       // 42 * 2 = 84
```

Supported types: `int`, `long`, `short`, `float`, `char`, `chars`, `bool`.

For formatted output (`%d`, `%c`, `%f`, padding, etc.), use `printf` from `@/io`.

---

## The Dungeon — Real Programs

The dungeon is complete. Here are self-contained Ura programs that bring everything together.

### Recursive Fibonacci

```ura
fn fib(n int) int:
    if n <= 1:
        return n
    return fib(n - 1) + fib(n - 2)

main():
    i int = 0
    while i < 10:
        output("fib(", i, ") = ", fib(i), "\n")
        i += 1
```

### strlen in Ura (no C imports)

```ura
fn strlen(s chars) int:
    i int = 0
    while s[i] != '\0':
        i += 1
    return i

main():
    output("length: ", strlen("Hello, Ura!"), "\n")   // 11
```

### strcmp in Ura

```ura
fn strcmp(left chars, right chars) int:
    i int = 0
    while left[i] == right[i] and left[i] != '\0':
        i += 1
    return left[i] as int - right[i] as int

main():
    output(strcmp("d", "a"), "\n")   // 3
```

### Combat Simulator (tuples + control flow + globals)

```ura
score int = 0
floor int = 1

fn roll_loot(f int) (chars, int):
    return "Health Potion", f * 10

fn simulate(hero_hp int, orc_hp int) void:
    round int = 0
    while hero_hp > 0:
        if orc_hp <= 0:
            score = score + 50 * round
            output("Floor ", floor, " cleared! Score: ", score, "\n")
            item chars, gold int = roll_loot(floor)
            output("Loot: ", item, " (+", gold, " gold)\n")
            return
        hero_hp = hero_hp - 10
        orc_hp  = orc_hp  - 18
        round   = round + 1
    output("Hero fell on floor ", floor, "\n")

main():
    simulate(100, 60)
    floor = floor + 1
    simulate(120, 80)
```

```
Floor 1 cleared! Score: 200
Loot: Health Potion (+10 gold)
Floor 2 cleared! Score: 450
Loot: Health Potion (+20 gold)
```

### Word Counter

```ura
fn count_words(s chars) int:
    count   int  = 0
    in_word bool = False
    i       int  = 0
    while s[i] != '\0':
        if s[i] != ' ' and s[i] != '\t' and s[i] != '\n':
            if not in_word:
                count   += 1
                in_word  = True
        else:
            in_word = False
        i += 1
    return count

main():
    output("words: ", count_words("the quick brown fox"), "\n")   // 4
```

---

## How It Works

The Ura compiler is written entirely in C. The tokenizer, parser, and semantic analysis are all hand-built from scratch. LLVM is used only for the final step — generating and optimizing machine code — because writing native code emitters for every CPU architecture by hand would be impractical. This way Ura targets any platform LLVM supports.

### Compilation Pipeline

```
source.ura
    │
    ▼
 tokenize()          reads the file and produces a flat list of tokens
    │
    ▼
   AST               recursive descent parser builds the syntax tree (top-down parsing)
    │
    ▼
 gen_ir()            walks the AST: resolves names, declares variables
                     and functions, performs type checking
    │
    ▼
 gen_asm()           walks the AST again and emits LLVM IR instructions
                     via the LLVM C API  →  build/file.ll
    │
    ▼
   llc               LLVM's static compiler: .ll → .s  (native assembly)
    │
    ▼
  clang              assembles and links: .s → executable
    │
    ▼
  exe.out            runs automatically after a successful build
```

### Source Files

The compiler is split into focused C source files:

- **`src/lexer.c`** — reads the source file character by character and produces a flat list of typed tokens. Handles strings, escape sequences, comments, indentation, keywords, and the `use` import system.
- **`src/parser.c`** — a hand-written recursive descent parser that builds the AST top-down. Operator precedence is encoded directly in the call chain (`expr → assign → logic → equality → comparison → add/sub → mul/div → dot → ...`).
- **`src/ir.c`** — a first pass over the AST (`gen_ir`) that resolves identifiers, registers variables and functions in their scopes, and annotates every node with its return type. After this pass the tree is fully typed and ready for code generation.
- **`src/codegen.c`** — a second pass (`gen_asm`) that walks the typed AST and emits LLVM IR instructions using the LLVM C API. Writes the final `.ll` file to `build/`.
- **`src/utils.c`** — shared helpers: error reporting, scope management, string utilities.
- **`src/ura.h`** — thin `static inline` wrappers around the LLVM C API (`LLVMBuildAdd`, `LLVMBuildStore`, `LLVMBuildGEP2`, ...) and higher-level helpers like `allocate_stack` and `allocate_heap`. Keeping LLVM calls isolated here makes the other files readable without knowing the LLVM API.
- **`src/header.h`** — defines the `Token` and `Node` structs that the entire compiler operates on, the `Type` enum (every token type from `INT` to `STRUCT_CALL`), all global variable declarations, and every function prototype.
- **`src/main.c`** — orchestrates everything: parses CLI flags, calls `compile()` for each source file, runs `llc` to produce `.s`, then `clang` to link the final executable, and immediately runs it.

### Why LLVM?

Writing a code generator that targets x86, ARM, and other architectures directly would mean implementing instruction selection, register allocation, and calling conventions for each one — thousands of lines of architecture-specific code. LLVM solves all of that. Ura emits architecture-independent LLVM IR, and LLVM handles the rest. The optimization passes (`-O0` through `-O3`, `-Os`, `-Oz`) are also provided by LLVM's pass pipeline at no extra cost.

---

## Compiler Reference

### Usage

```bash
ura <file.ura> [options]
```

The compiler generates a `build/` directory next to each source file. The executable is placed there as `exe.out` by default, or at the path specified with `-o`.

### Options

| Flag       | Description                                       |
|------------|---------------------------------------------------|
| `-O0`      | No optimization (debug)                           |
| `-O1`      | Basic optimization                                |
| `-O2`      | Standard optimization                             |
| `-O3`      | Aggressive optimization                           |
| `-Os`      | Optimize for size                                 |
| `-Oz`      | Minimize binary size                              |
| `-san`     | Enable AddressSanitizer + debug info (`-g`)       |
| `-o <n>`   | Output executable name (default: `build/exe.out`) |

### Build Commands

| Command              | Description                                        |
|----------------------|----------------------------------------------------|
| `build`              | Compile the Ura compiler                           |
| `ura <file>`         | Compile and run a `.ura` file                      |
| `tests [folder]`     | Run test suite (optionally filter by folder)       |
| `update_tests`       | Regenerate all expected `.ll` snapshot files       |
| `copy <file.ura>`    | Save test (reads `// folder/filename` from line 1) |
| `check`              | Re-run dependency check                            |
| `install`            | Install missing dependencies for this platform     |
| `examples`           | Generate examples.ura from all test files          |
| `indent`             | Auto-format all C source files                     |
| `update`             | Reload `config.sh`                                 |

---

### config.sh

`config.sh` is the development environment for Ura. Source it at the start of every session — it detects your OS, checks dependencies, sets up paths, and gives you short shell commands for every development task.

```bash
source config.sh   # every session
```

On first source (or after a fresh install) it automatically runs a dependency check:

```
  ╔══════════════════════════════╗
  ║     Ura Development Env      ║
  ╚══════════════════════════════╝

  Dependency Check
  ────────────────────────────────────
  ✓  clang              clang 14.0.6
  ✓  llvm-config-14     LLVM 14.0.6
  ✓  llc                14.0.6
  ✓  uncrustify         0.82.0

  All good!  Commands: build  copy  tests  check  install  help
```

**Environment variables set:**

| Variable        | Description                                   |
|-----------------|-----------------------------------------------|
| `$URA_LIB`      | Path to the standard library (`src/ura-lib/`) |
| `$URA_COMPILER` | Path to the compiled compiler binary          |
| `$PATH`         | Adds `build/` so `ura` is available directly  |

**Shell commands:**

| Command              | Description                                                                      |
|----------------------|----------------------------------------------------------------------------------|
| `build`              | Compile the Ura compiler from source                                             |
| `tests [folder]`     | Run all tests (optionally filter by folder name)                                 |
| `update_tests`       | Regenerate all expected `.ll` snapshot files                                     |
| `copy <file.ura>`    | Compile a `.ura` file and save it as a test (reads path from first-line comment) |
| `examples`           | Generate `examples.ura` from all test files                                      |
| `indent`             | Auto-format all `.c` and `.h` files with uncrustify                              |
| `check`              | Re-run the dependency check manually                                             |
| `install`            | Install all missing dependencies for the current platform                        |
| `update`             | Reload `config.sh` without opening a new shell                                   |
| `help`               | Show all commands and compiler flags                                             |

The compiler is built with AddressSanitizer and strict warning flags by default during development, so memory bugs surface immediately while working on the compiler source.

### setup.sh / setup.ps1

One-time bootstrap scripts for fresh machines. Run them once, then use `config.sh` for every session after.

**macOS / Linux:**
```bash
bash config/setup.sh
```

Detects your OS and package manager, installs all missing tools (`llvm-14`, `clang`, `llc`, `uncrustify`), creates any needed symlinks, and verifies everything works.

**Windows:**
```powershell
# In PowerShell as Administrator:
powershell -ExecutionPolicy Bypass -File config/setup.ps1
```

Installs MSYS2 via `winget`, updates the package database, and runs `setup.sh` inside MSYS2 automatically. Then open the **MSYS2 MinGW x64** terminal and `source config.sh` as usual.

---

## Project Structure

```
ura-lang/
├── src/
│   ├── main.c          # Entry point: CLI, compile(), llc + clang pipeline
│   ├── lexer.c         # Tokenizer: source → flat token list
│   ├── parser.c        # Recursive descent parser: tokens → AST
│   ├── ir.c            # gen_ir(): name resolution, type checking
│   ├── codegen.c       # gen_asm(): typed AST → LLVM IR (.ll)
│   ├── utils.c         # Shared helpers: errors, scopes, strings
│   ├── ura.h           # LLVM C API wrappers and inline helpers
│   ├── header.h        # Token, Node, Type enum, globals, declarations
│   ├── file.ura        # Development scratch file
│   └── ura-lib/        # Standard library
│       ├── header.ura  # Imports all modules
│       ├── io.ura
│       ├── memory.ura
│       ├── string.ura
│       ├── stdlib.ura
│       ├── time.ura
│       ├── signals.ura
│       ├── os.ura
│       └── net.ura
├── tests/
│   ├── builtins/       # Built-in functions and memory
│   │   ├── c-funcs/    # C function prototypes (printf, puts, etc.)
│   │   ├── memory/     # Stack and heap allocation
│   │   ├── output/     # output() builtin
│   │   ├── sizeof/     # sizeof operator
│   │   └── typeof/     # typeof operator
│   ├── cond/           # Conditional statements (if/elif/else)
│   ├── data_types/     # Arrays and data types
│   ├── fn/             # Functions and parameters
│   │   └── param/      # Value and reference parameters
│   ├── globals/        # Global variables
│   ├── vars/           # Variables and references
│   ├── while/          # Loops, break, continue
│   ├── op/             # Operators (arithmetic, logical, comparison, bitwise)
│   ├── structs/        # Structs, nested structs, methods
│   ├── tuples/         # Tuple types and multi-return
│   ├── os/             # OS module (argc, argv)
│   ├── net/            # TCP client/server
│   └── projects/       # Real-world programs written in Ura
│       ├── ura-libft/  # Classic libc functions (strlen, strcmp, isalpha, ...)
│       └── ura-tcp-server/
│           ├── basic/        # Simple chat room
│           │   ├── server.ura
│           │   └── client.ura
│           ├── cmd/          # Command-based server (/help, /time, /whoami, /exit)
│           │   ├── server.ura
│           │   └── client.ura
│           └── utils.ura     # Shared utilities (SockAddr, logging)
├── config/
│   ├── setup.sh              # One-time setup: installs all deps (macOS / Linux / MSYS2)
│   ├── setup.ps1             # One-time setup for Windows
│   └── vscode-extension/     # VS Code syntax highlighting + language features
├── build/              # Compiler executable and compiled outputs
├── config.sh           # Development environment (source every session)
└── README.md
```

---

## Status

Ura is under active development. Here's where things stand:

**Working today:**

- Primitive types: `int` `long` `short` `char` `chars` `bool` `float` `void`
- Array type declarations: `array[type]`, `array[[type]]`, ...
- Stack and heap allocation: `stack[type](n)`, `heap[type](n)`
- Global variables and global structs
- Functions, variadic functions, single-line functions
- **Tuples** — multi-value returns `(type, type)` with destructuring at the call site
- Function prototypes for C interop (`proto`)
- References and reference parameters
- Structs, nested structs at any depth, struct methods with `self`
- Struct `init()` constructor and `clean()` destructor (auto-called at scope exit)
- If / elif / else
- While loops with break and continue
- All standard operators: arithmetic, bitwise, logical, comparison, assignment
- Type casting with `as`
- `typeof` and `sizeof`
- `output()` builtin — prints any value without format strings or imports
- Module imports with `use` (`@` for stdlib, relative path for local files)
- OS module — `os.argc` and `os.argv` for command-line argument access
- Multi-file compilation
- Networking via POSIX socket APIs (`@/net`)
- Optimization levels (-O0 → -O3, -Os, -Oz) via LLVM pass pipeline
- AddressSanitizer + memory leak detection (`-san`)

**Coming next:**

- For loops (range-based: `for i in 0..10`)
- Enums + pattern matching
- Switch / case
- Type inference
- Type aliases (`type Byte = char`)
- Default parameter values
- Exception handling

---

## Requirements

| Platform | Requirements |
|----------|-------------|
| macOS | Homebrew, LLVM 14 (`brew install llvm@14`), uncrustify |
| Linux | clang-14, llvm-14, llvm-14-tools, uncrustify |
| Windows | MSYS2 with MinGW64, then `mingw-w64-x86_64-llvm` + `mingw-w64-x86_64-clang` |

Run `bash config/setup.sh` (or `powershell config/setup.ps1` on Windows) to install everything automatically.

**Why LLVM 14?** It uses explicit pointer types (`i32*`, `i32**`) in generated IR instead of the opaque `ptr` from LLVM 15+, which makes debugging and reading the IR significantly easier.

---

## Author

**Mohammed Hrima**

[linkedin.com/in/mohammedhrima](https://www.linkedin.com/in/mohammedhrima)

mohammed.hrima1998@gmail.com

---

## License

MIT — see [LICENSE](LICENSE) for details.
