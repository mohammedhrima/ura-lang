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
  - [Functions](#functions)
  - [Control Flow](#control-flow)
  - [Operators](#operators)
  - [Type Casting](#type-casting)
  - [References](#references)
  - [Structs](#structs)
  - [Memory](#memory)
  - [C Interoperability](#c-interoperability)
  - [Type Introspection](#type-introspection)
- [Real Examples](#real-examples)
  - [Fibonacci](#fibonacci)
  - [String Length](#string-length)
  - [putnbr](#putnbr--print-a-number-without-printf)
  - [strcmp](#strcmp)
  - [Word Counter](#word-counter)
- [How It Works](#how-it-works)
  - [Compilation Pipeline](#compilation-pipeline)
  - [Source Files](#source-files)
  - [Why LLVM?](#why-llvm)
- [Compiler Reference](#compiler-reference)
  - [Usage](#usage)
  - [Options](#options)
  - [Build Commands](#build-commands)
  - [config.sh](#configsh)
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

# 2. Install LLVM (if not already installed)
brew install llvm          # macOS
sudo apt install llvm-dev  # Ubuntu/Debian

# 3. Load the environment
source config.sh

# 4. Build the compiler
build

# 5. Write and run your first program
ura src/file.ura
```

**Hello World:**

```ura
use "@/io"

main():
    printf("Hello, World!\n")
```

---

## Language Tour

### Imports

Use `use` to import modules. The `@` prefix resolves to the built-in standard library (`$URA_LIB`). You can also import relative local files:

```ura
use "@/io"        // standard library: printf, puts, file I/O ...
use "@/memory"    // malloc, calloc, free ...
use "@/string"    // strlen, strcmp, strcpy ...
use "@/header"    // imports all standard modules at once

use "mymodule"    // relative import from the same directory
```

---

### Variables

Variables are declared as `name type = value`:

```ura
main():
    name   chars = "Alice"
    age    int   = 30
    active bool  = True
    letter char  = 'A'
    score  float = 9.5
```

---

### Functions

```ura
use "@/io"

fn greet(name chars) void:
    printf("Hello, %s!\n", name)

fn add(a int, b int) int:
    return a + b

main():
    greet("Ura")
    result int = add(10, 32)
    printf("Result: %d\n", result)
```

Single-line functions:

```ura
fn square(n int) int:     return n * n
fn is_digit(c char) bool: return c >= '0' and c <= '9'
```

---

### Control Flow

```ura
use "@/io"

main():
    a int = 2
    if a == 1:
        printf("is 1\n")
    elif a == 2:
        printf("not 1 but 2\n")
    else:
        printf("neither\n")

    i int = 0
    while i < 5:
        if i == 3: break
        i += 1
    printf("stopped at: %d\n", i)
```

---

### Operators

| Category   | Operators                             |
|------------|---------------------------------------|
| Arithmetic | `+`  `-`  `*`  `/`  `%`              |
| Bitwise    | `&`  `\|`  `^`  `~`  `<<`  `>>`      |
| Comparison | `==` `!=` `<` `>` `<=` `>=` `is`     |
| Logical    | `and` `or` `not` `&&` `\|\|` `!`     |
| Assignment | `=` `+=` `-=` `*=` `/=` `%=`         |
| Cast       | `as`                                  |

**Bitwise operations:**

```ura
use "@/io"

main():
    a int = 60   // 0011 1100
    b int = 13   // 0000 1101

    printf("AND: %d\n", a & b)    // 12  (0000 1100)
    printf("OR:  %d\n", a | b)    // 61  (0011 1101)
    printf("XOR: %d\n", a ^ b)    // 49  (0011 0001)
    printf("NOT: %d\n", ~a)       // -61 (two's complement)
    printf("LEFT SHIFT:  %d\n", 1 << 3)   // 8
    printf("RIGHT SHIFT: %d\n", 60 >> 2)  // 15
```

---

### Type Casting

Use `as` to convert between types:

```ura
use "@/io"

main():
    // char to int
    c char = 'A'
    n int  = c as int
    printf("char '%c' as int: %d\n", c, n)   // 65

    // int to char
    code   int  = 66
    letter char = code as char
    printf("int %d as char: %c\n", code, letter)   // B

    // used in expressions
    left  chars = "d"
    right chars = "a"
    diff  int   = left[0] as int - right[0] as int
    printf("diff: %d\n", diff)   // 3
```

---

### References

References bind to an existing variable — all writes go through to the original:

```ura
use "@/io"

fn swap(a int ref, b int ref) void:
    temp int = a
    a = b
    b = temp

main():
    x int = 10
    y int = 20
    printf("Before: x=%d, y=%d\n", x, y)
    swap(x, y)
    printf("After:  x=%d, y=%d\n", x, y)
```

```
Before: x=10, y=20
After:  x=20, y=10
```

References can also be declared without an immediate binding:

```ura
use "@/io"

main():
    a int = 1
    b int = 2
    choice int = 1

    r int ref
    if choice == 1:
        r = a
    else:
        r = b

    r = 100
    printf("%d\n", a + b)   // 102
```

---

### Structs

```ura
use "@/io"

struct Point:
    x int
    y int

fn print_point(p Point ref) void:
    printf("(%d, %d)\n", p.x, p.y)

main():
    p Point
    p.x = 3
    p.y = 4
    print_point(p)
```

**Struct methods:**

Structs can have methods with a special `self` parameter that refers to the instance:

```ura
proto fn printf(fmt chars, ...) int

struct User:
    name array[char]
    age int

    fn init() void:
        self.name = "new name"
        self.age = 10

    fn greet() void:
        printf("hello from %s, age %d\n", self.name, self.age)

main():
    user User
    user.greet()   // hello from new name, age 10
```

The `init()` method is a special constructor that runs automatically when a struct is declared.

**Nested structs:**

Structs can be nested at any depth:

```ura
use "@/io"

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
    printf("%s lives in %s\n", dev.name, dev.address.city)
```

**Struct references:**

Struct references let functions modify the caller's data:

```ura
use "@/io"

struct User:
    id int

fn update(u User ref) void:
    u.id = 99

main():
    user User
    user.id = 1
    update(user)
    printf("id: %d\n", user.id)   // 99
```

---

### Memory

Ura gives you direct control over where memory lives:

```ura
use "@/io"
use "@/memory"
use "@/string"

main():
    // Stack — fast, freed automatically when the function returns
    local chars = stack[char](64)
    strcpy(local, "stack allocated")
    printf("%s\n", local)

    // Heap — survives the function, must be freed manually
    buffer chars = heap[char](256)
    strcpy(buffer, "heap allocated")
    printf("%s\n", buffer)
    free(buffer)
```

You can also use the `array` type annotation for clarity:

```ura
str array[char] = heap[char](20)
str[0] = 'H'
str[1] = 'i'
```

Multi-level allocation is supported:

```ura
matrix array[[int]]   = heap[[int]](10)    // array of int pointers
cube   array[[[int]]] = heap[[[int]]](5)   // one level deeper
```

| | `stack[type](n)` | `heap[type](n)` |
|---|---|---|
| Cleanup | Automatic | Manual (`free`) |
| Speed | Faster | Slower |
| Capacity | Limited | Large |
| Lifetime | Function scope | Until freed |

---

### C Interoperability

Declare any C function with `proto` and call it immediately:

```ura
proto fn strlen(s chars) int
proto fn write(fd int, ptr chars, len int) int

main():
    n int = strlen("hello")
    write(1, "hi\n", 3)
```

Variadic functions work too:

```ura
proto fn printf(format chars, ...) int

main():
    printf("name: %s, age: %d\n", "Alice", 30)
```

The standard library modules wrap the most common ones:

```ura
use "@/io"       // printf, puts, fopen, fclose, write, read ...
use "@/memory"   // malloc, calloc, free, realloc ...
use "@/string"   // strlen, strcmp, strcpy, strcat, strdup ...
use "@/math"     // sqrt, pow, abs ...
use "@/stdlib"   // atoi, rand, exit ...
use "@/time"     // time, clock, difftime ...
use "@/signals"  // signal, raise ...
use "@/net"      // socket, bind, listen, accept, connect, send, recv ...
use "@/header"   // imports all of the above at once
```

---

### Networking

Ura provides direct access to POSIX socket APIs through the `@/net` module:

```ura
use "@/header"

main():
    server_fd int = socket(2, 1, 0)  // AF_INET, SOCK_STREAM, 0
    if server_fd < 0:
        printf("socket failed\n")
        return 1

    port int = 8080
    addr chars = calloc(16, 1)
    addr[0] = 2 as char      // sin_family low byte  (AF_INET = 2)
    addr[1] = 0 as char      // sin_family high byte
    addr[2] = 31 as char     // sin_port high byte   (8080 >> 8 = 31)
    addr[3] = 144 as char    // sin_port low byte    (8080 & 255 = 144)
    // addr[4..7] = 0 from calloc = INADDR_ANY

    result int = bind(server_fd, addr, 16)
    if result < 0:
        printf("bind failed\n")
        return 1

    listen(server_fd, 5)
    printf("listening on port 8080...\n")

    buf chars = calloc(1024, 1)
    client_fd int = accept(server_fd, 0 as chars, 0)
    if client_fd >= 0:
        r int = read(client_fd, buf, 1023)
        if r > 0:
            printf("received: %s\n", buf)
            write(client_fd, "hello from server\n", 18)
        close(client_fd)

    free(buf)
    free(addr)
    return 0
```

**Real-world TCP examples:**

The `projects/tcp/` directory contains complete working examples:

- **basic/** - Simple chat room with bidirectional messaging between server and client
- **cmd/** - Command-based server supporting `/help`, `/time`, `/whoami`, `/exit` commands
- **utils.ura** - Shared utilities including `SockAddr` struct for address management and timestamped logging

Run the examples:
```bash
# Terminal 1: Start server
ura projects/tcp/basic/server.ura

# Terminal 2: Connect client
ura projects/tcp/basic/client.ura
```

---

### Type Introspection

`typeof` returns the type name as a string. `sizeof` returns the byte size:

```ura
use "@/io"

main():
    type chars = typeof("hello world\n")
    size int   = sizeof(type)
    printf("type: %s, size: %d bytes\n", type, size)
    // type: CHARS, size: 8 bytes

    printf("%s\n", typeof(42))     // INT
    printf("%s\n", typeof('a'))    // CHAR
    printf("%s\n", typeof(True))   // BOOL

    printf("%d\n", sizeof(42))     // 4  (size of INT)
    printf("%d\n", sizeof('a'))    // 1  (size of CHAR)
```

---

## Real Examples

### Fibonacci

```ura
use "@/io"

fn fib(n int) int:
    if n <= 1:
        return n
    return fib(n - 1) + fib(n - 2)

main():
    i int = 0
    while i < 10:
        printf("fib(%d) = %d\n", i, fib(i))
        i += 1
```

### String Length

```ura
use "@/io"

fn strlen(s chars) int:
    i int = 0
    while s[i] != '\0':
        i += 1
    return i

main():
    printf("length: %d\n", strlen("Hello, Ura!"))
```

### putnbr — print a number without printf

```ura
proto fn calloc(len int, size int) chars
proto fn write(fd int, ptr chars, len int) int
proto fn free(ptr chars) int

fn putchar(c char) int:
    str chars = calloc(2, 1)
    str[0] = c
    write(1, str, 1)
    free(str)
    return 0

fn putnbr(n int) void:
    if n < 0:
        putchar('-')
        n = -n
    if n == 0:
        putchar('0')
        return
    digits chars = "0123456789"
    temp   int   = n
    count  int   = 0
    while temp > 0:
        count += 1
        temp = temp / 10
    buffer chars = calloc(count + 1, 1)
    i int = count - 1
    while i >= 0:
        buffer[i] = digits[n % 10]
        n = n / 10
        i -= 1
    i = 0
    while i < count:
        putchar(buffer[i])
        i += 1
    free(buffer)

main():
    putnbr(123)
    putchar('\n')
    putnbr(-456)
    putchar('\n')
```

### strcmp

```ura
use "@/io"

fn strcmp(left chars, right chars) int:
    i int = 0
    while left[i] == right[i] and left[i] != '\0':
        i += 1
    return left[i] as int - right[i] as int

main():
    printf("%d\n", strcmp("d", "a"))   // 3
```

### Word Counter

```ura
use "@/io"

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
    printf("words: %d\n", count_words("the quick brown fox"))   // 4
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

**`src/main.c`** is the heart of the compiler. It contains everything except the LLVM wrappers:

- **Tokenizer** — reads the source file character by character and produces a flat list of typed tokens. Handles strings, escape sequences, comments, indentation, keywords, and the `use` import system.
- **Parser** — a hand-written recursive descent parser that builds the AST top-down. Operator precedence is encoded directly in the call chain (`expr → assign → logic → equality → comparison → add/sub → mul/div → dot → ...`).
- **`gen_ir()`** — a first pass over the AST that resolves identifiers, registers variables and functions in their scopes, and annotates every node with its return type. After this pass the tree is fully typed and ready for code generation.
- **`gen_asm()`** — a second pass that walks the typed AST and emits LLVM IR instructions using the LLVM C API. At the end it writes a `.ll` file to `build/`.
- **`main()`** — orchestrates everything: parses CLI flags, calls `compile()` for each source file, runs `llc` to produce `.s`, then `clang` to link the final executable, and immediately runs it.

**`src/llvm.c`** contains thin wrappers around the LLVM C API (`LLVMBuildAdd`, `LLVMBuildStore`, `LLVMBuildGEP2`, ...) plus higher-level helpers like `allocate_stack`, `allocate_heap`, and the `ref_assign` runtime function. Keeping LLVM calls isolated here makes `main.c` readable without knowing the LLVM API.

**`src/header.h`** defines the `Token` and `Node` structs that the entire compiler operates on, the `Type` enum (every token type from `INT` to `STRUCT_CALL`), all global variable declarations, and every function prototype.

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

| Command             | Description                                      |
|---------------------|--------------------------------------------------|
| `build`             | Compile the Ura compiler                         |
| `ura <file>`        | Compile and run a `.ura` file                    |
| `tests [folder]`    | Run the test suite (optionally filter by folder) |
| `copy <file.ura>`   | Save a test (reads destination from first line)  |
| `examples`          | Generate examples.ura from all test files        |
| `indent`            | Auto-format all C source files                   |
| `update`            | Reload `config.sh`                               |


---

### config.sh

`config.sh` is the development environment for Ura. Source it once and it sets up paths, environment variables, and shell functions so you can build, test, and run the compiler with short commands.

```bash
source config.sh
```

It sets the following environment variables:

| Variable     | Description                                   |
|--------------|-----------------------------------------------|
| `$URA_LIB`   | Path to the standard library (`src/ura-lib/`) |
| `$ASAN_FILE` | Path to the LeakSanitizer suppression file    |
| `$PATH`      | Adds `build/` so `ura` is available directly  |

It also defines these shell functions:

| Function            | Description                                                                                              |
|---------------------|----------------------------------------------------------------------------------------------------------|
| `build`             | Compiles the compiler from source using clang with LLVM flags                                            |
| `tests [folder]`    | Builds the compiler once, then compiles every `.ura` file in `tests/`. Pass a folder name to filter      |
| `copy <file.ura>`   | Compiles a `.ura` file and saves it as a test (reads destination from first line comment)                |
| `examples`          | Generates `examples.ura` containing all test files from the test suite                                   |
| `indent`            | Formats all `.c` and `.h` files using uncrustify                                                         |
| `update`            | Reloads `config.sh` without opening a new shell                                                          |

The compiler itself is built with AddressSanitizer and strict warning flags enabled by default during development, so memory bugs surface immediately while working on the compiler source.

---

## Project Structure

```
ura-lang/
├── src/
│   ├── main.c          # Tokenizer, parser, gen_ir, gen_asm, main
│   ├── llvm.c          # LLVM C API wrappers and helpers
│   ├── header.h        # Token, Node, Type enum, globals, declarations
│   ├── file.ura        # Development scratch file
│   └── ura-lib/        # Standard library
│       ├── header.ura  # Imports all modules
│       ├── io.ura
│       ├── memory.ura
│       ├── string.ura
│       ├── math.ura
│       ├── stdlib.ura
│       ├── time.ura
│       ├── signals.ura
│       └── net.ura         # socket, bind, listen, accept, connect
├── tests/
│   ├── builtins/       # printf, puts, stack, heap, typeof, sizeof
│   ├── data_types/     # references, arrays
│   ├── fn/             # functions, parameters, return values
│   ├── if/             # conditionals
│   ├── while/          # loops, break, continue
│   ├── op/             # arithmetic, logical, comparison, bitwise operators
│   ├── structs/        # structs, nested structs, struct methods
│   ├── net/            # TCP client/server examples
│   └── libft/          # strlen, strcmp, putnbr, isalpha ...
├── projects/
│   └── tcp/            # Real-world TCP networking examples
│       ├── basic/      # Simple chat room (bidirectional messaging)
│       │   ├── server.ura
│       │   └── client.ura
│       ├── cmd/        # Command-based server with /help, /time, /whoami
│       │   ├── server.ura
│       │   └── client.ura
│       └── utils.ura   # Shared utilities (SockAddr, logging, timestamps)
├── build/              # Compiler executable
├── config.sh           # Build system and dev commands
└── README.md
```

---

## Status

Ura is under active development. Here's where things stand:

**Working today:**

- Primitive types: `int` `long` `short` `char` `chars` `bool` `float` `void`
- Array type declarations: `array[type]`, `array[[type]]`, ...
- Stack and heap allocation: `stack[type](n)`, `heap[type](n)`
- Functions, variadic functions, single-line functions
- Function prototypes for C interop (`proto`)
- References and reference parameters
- Structs, nested structs at any depth, struct methods with `self` parameter
- Struct `init()` constructor
- If / elif / else
- While loops with break and continue
- All standard operators: arithmetic, bitwise, logical, comparison, assignment
- Type casting with `as`
- `typeof` and `sizeof`
- Module imports with `use` (`@` for stdlib, relative path for local files)
- Multi-file compilation
- Networking support via POSIX socket APIs
- Optimization levels (-O0 → -O3, -Os, -Oz) via LLVM pass pipeline
- AddressSanitizer + memory leak detection (`-san`)

**Coming next:**

- For loops
- Global variables
- Enums
- Switch / case
- Type inference
- Exception handling

---

## Requirements

- Clang or GCC
- LLVM 14+ with `llvm-config`
- macOS or Linux

---

## Author

**Mohammed Hrima**

[linkedin.com/in/mohammedhrima](https://www.linkedin.com/in/mohammedhrima)

mohammed.hrima1998@gmail.com

---

## License

MIT — see [LICENSE](LICENSE) for details.