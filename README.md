# Ura Lang

Ura is a compiled programming language (work in progress) that compiles to LLVM IR. The compiler is written in C and uses LLVM for code generation and optimization.

## Project Overview

Ura is designed as a statically-typed, imperative language with a focus on simplicity and direct compilation to native code through LLVM. The language supports basic data types, control flow, functions, references, structs, and interoperability with C libraries through prototype declarations.

## Repository Structure

```
.
├── src/                   # Main compiler source code
│   ├── main.c             # Entry point, tokenizer, parser
│   ├── llvm.c             # LLVM C API wrappers
│   ├── header.h           # Type definitions and function declarations
│   ├── file.ura           # Working file for development
│   ├── ura-lib/           # Standard library prototypes
│   │   ├── header.ura     # Main library header (imports all modules)
│   │   ├── io.ura         # I/O operations (printf, puts, file I/O)
│   │   ├── string.ura     # String manipulation
│   │   ├── memory.ura     # Memory management (malloc, calloc, free)
│   │   ├── math.ura       # Mathematical functions
│   │   ├── stdlib.ura     # Standard library functions
│   │   ├── time.ura       # Time operations
│   │   └── signals.ura    # Signal handling
│   └── build/             # Compiler output directory
│
├── tests/                 # Test suite organized by feature
│   ├── builtins/          # Built-in function tests (printf, puts, stack, typeof)
│   ├── data_types/        # Type system tests (references)
│   ├── fn/                # Function tests (parameters, return values)
│   ├── if/                # Conditional statement tests
│   ├── while/             # Loop tests
│   ├── op/                # Operator tests (arithmetic, logical, comparison)
│   ├── structs/           # Struct tests
│   └── libft/             # Standard library function implementations
│
├── llvm/                  # LLVM experimentation and testing
│   ├── main.c             # Experimental compiler implementation
│   ├── utils.c            # Utility functions for experiments
│   ├── wrapper.c          # LLVM wrapper functions
│   ├── header.h           # Header for experimental code
│   ├── config.sh          # Build configuration for experiments
│   ├── examples/          # Example programs (000-013)
│   ├── tests/             # Expected LLVM IR outputs
│   └── build/             # Build artifacts
│
├── build/                 # Compiler executable output
├── docs/                  # Documentation
├── workspaces/            # VS Code workspace configurations
├── config.sh              # Build system and development commands
├── PLAN.md                # Development roadmap
├── TODO.md                # Current tasks
└── README.md              # This file
```

## Language Syntax

### Data Types

Ura supports the following primitive types:

- `int` - 32-bit signed integer
- `long` - 64-bit signed integer
- `short` - 16-bit signed integer
- `char` - 8-bit character
- `chars` - String (pointer to char array)
- `bool` - Boolean (`True` or `False`)
- `float` - 32-bit floating point
- `void` - No return value
- `pointer` - Generic pointer type

### Variable Declaration

Variables are declared with the pattern: `name type = value`

```ura
main():
    a int = 10
    name chars = "Hello"
    flag bool = True
    c char = 'x'
```

### References

References allow binding to existing variables. The syntax is `name type ref = target`:

```ura
main():
    a int = 10
    b int ref = a    // b is a reference to a
    b = 20           // modifies a through the reference
    return a         // returns 20
```

Reference behavior:
- First assignment binds the reference to a variable (one-time binding)
- All subsequent assignments modify the bound variable's value
- References cannot be rebound to different variables

```ura
main():
    val1 int = 10
    val2 int = 20
    
    r int ref = val1  // Bind to val1
    r = 15            // val1 = 15
    r = 25            // val1 = 25 (still modifying val1)
```

### Structs

Structs are declared with the `struct` keyword:

```ura
struct User:
    name chars
    age int

main():
    user User
    user.name = "alice"
    user.age = 25
```

Nested structs:

```ura
struct Model:
    name chars
    year int

struct Car:
    model Model

main():
    car Car
    car.model.name = "sedan"
    car.model.year = 2024
```

Struct references:

```ura
struct User:
    id int

fn modify(u User ref) void:
    u.id = 100

main():
    user User
    user.id = 1
    modify(user)
    // user.id is now 100
```

### Functions

Function declaration syntax:

```ura
fn function_name(param1 type1, param2 type2) return_type:
    // function body
    return value
```

Example:

```ura
fn square(n int) int:
    return n * n

main():
    result int = square(5)
```

Single-line function:

```ura
fn isdigit(c char) bool: return c >= '0' and c <= '9'
```

### Function Parameters

Regular parameters:

```ura
fn add(a int, b int) int:
    return a + b
```

Reference parameters (modifications affect the caller):

```ura
fn increment(n int ref) void:
    n = n + 1

main():
    x int = 5
    increment(x)
    // x is now 6
```

### Function Prototypes

External C functions can be declared using `proto fn`:

```ura
proto fn puts(str chars) int
proto fn malloc(size int) pointer
proto fn free(ptr pointer) void

main():
    puts("Hello, World!")
```

### Variadic Functions

Functions with variable arguments use `...`:

```ura
proto fn printf(format chars, ...) int

main():
    printf("Number: %d\n", 42)
    printf("Name: %s, Age: %d\n", "Alice", 30)
```

### Control Flow

#### If/Elif/Else

```ura
main():
    a int = 5
    if a < 10:
        printf("less than 10\n")
    elif a < 20:
        printf("less than 20\n")
    else:
        printf("20 or more\n")
```

Single-line if:

```ura
main():
    if condition: statement
    else: other_statement
```

#### While Loops

```ura
main():
    i int = 0
    while i < 10:
        i = i + 1
```

#### Break and Continue

```ura
main():
    i int = 0
    while i < 10:
        i += 1
        if i == 5:
            break
```

### Operators

#### Arithmetic Operators
- `+` Addition
- `-` Subtraction
- `*` Multiplication
- `/` Division
- `%` Modulo

```ura
main():
    a int = 20
    b int = 4
    sum int = a + b      // 24
    diff int = a - b     // 16
    prod int = a * b     // 80
    quot int = a / b     // 5
    rem int = a % b      // 0
```

#### Comparison Operators
- `==` or `is` Equal
- `!=` Not equal
- `<` Less than
- `>` Greater than
- `<=` Less than or equal
- `>=` Greater than or equal

```ura
main():
    a int = 10
    b int = 20
    
    if a < b:
        printf("a is less than b\n")
    
    if a == 10:
        printf("a equals 10\n")
    
    if a is 10:  // Alternative syntax
        printf("a is 10\n")
```

#### Logical Operators
- `and` or `&&` Logical AND
- `or` or `||` Logical OR
- `not` or `!` Logical NOT

```ura
main():
    a bool = True
    b bool = False
    
    if a and !b:
        printf("a is true and b is false\n")
    
    if a || b:
        printf("at least one is true\n")
```

#### Assignment Operators
- `=` Assignment
- `+=` Add and assign
- `-=` Subtract and assign
- `*=` Multiply and assign
- `/=` Divide and assign
- `%=` Modulo and assign

```ura
main():
    x int = 10
    x += 5   // x = 15
    x -= 3   // x = 12
    x *= 2   // x = 24
    x /= 4   // x = 6
    x %=  4  // x = 2
```

### Array Access

```ura
main():
    str chars = "hello"
    c char = str[0]    // Access first character
    str[0] = 'H'       // Modify first character
```

Array access with references:

```ura
fn strlen(str chars) int:
    i int = 0
    while str[i] != '\0':
        i += 1
    return i
```

### Type Casting

Use `as` keyword for type conversion:

```ura
main():
    a int = 65
    c char = a as char  // Convert int to char
    
    ptr chars = stack(10) as chars  // Cast pointer type
```

### Stack Allocation

Dynamic stack allocation using `stack()` builtin:

```ura
proto fn write(fd int, ptr chars, len int) int

main():
    buffer chars = stack(100) as chars
    buffer[0] = 'H'
    buffer[1] = 'i'
    buffer[2] = '\0'
    write(1, buffer, 2)
```

### Type Introspection

The `typeof` builtin returns the type name as a string:

```ura
proto fn puts(str chars) int

main():
    type_name chars = typeof "hello"
    puts(type_name)  // Prints: chars
```

### Comments

```ura
// Single line comment

/*
   Multi-line
   comment
*/
```

### Module System

Import other Ura files using `use`:

```ura
// Import from ura-lib (requires URA_LIB environment variable)
use "@/io"
use "@/memory"
use "@/header"  // Imports all standard library modules

// Import relative files
use "mymodule"
```

The `@` prefix resolves to the `$URA_LIB` environment variable, which is set when you source `config.sh`.

## Build System

### Prerequisites

- C compiler (clang recommended)
- LLVM development libraries (version 14+)
- llvm-config tool
- uncrustify (for code formatting)

### Installation

1. Install LLVM development libraries:

```bash
# macOS
brew install llvm

# Linux (Ubuntu/Debian)
sudo apt-get install llvm-dev

# Linux (Fedora)
sudo dnf install llvm-devel
```

2. Clone the repository:

```bash
git clone <repository-url>
cd ura-lang
```

3. Source the build environment:

```bash
source config.sh
```

This sets up:
- `$URA_LIB` - Path to standard library
- `$PATH` - Adds build directory to PATH
- Shell functions for building and testing

### Build Commands

#### build

Compiles the Ura compiler from C source files.

```bash
build
```

Output: `build/ura` executable

Compilation includes:
- Address sanitizer for memory error detection
- Null pointer checks
- LLVM optimization flags
- Strict warning flags

#### copy

Saves a `.ura` file and its generated IR to the test directory.

```bash
copy <folder> <filename>
```

Example:

```bash
copy op arithmetic
```

Creates:
- `tests/op/arithmetic.ura` (copy of the source file)
- `tests/op/arithmetic.ll` (generated LLVM IR)

#### tests

Runs the test suite.

```bash
tests [folder...]
```

Examples:

```bash
tests              # Run all tests
tests op           # Run operator tests only
tests builtins fn  # Run multiple categories
```

Test process:
1. Compiles each `.ura` file in specified test directories
2. Generates LLVM IR
3. Compares with expected `.ll` file (ignoring metadata)
4. Reports pass/fail for each test

#### indent

Formats C source code using uncrustify.

```bash
indent
```

Applies project code style to all `.c` and `.h` files in `src/`.

#### update

Reloads the configuration file.

```bash
update
```

Useful after modifying `config.sh`.

#### help

Displays all available commands and compiler usage.

```bash
help
```

### Compiler Usage

```bash
ura <file.ura> [options]
```

The compiler generates a `build/` directory next to each source file containing:
- `.ll` - LLVM IR
- `.s` - Assembly code
- `exe.out` - Executable (placed in the same directory as the source file)

#### Optimization Flags

- `-O0` - No optimization (debug mode)
- `-O1` - Basic optimization (mem2reg, instcombine)
- `-O2` - Standard optimization (default for production)
- `-O3` - Aggressive optimization
- `-Os` - Optimize for size
- `-Oz` - Minimize size

#### Sanitizer Flags

- `-san` - Enable AddressSanitizer (detects memory errors)

#### Output Options

- `-o <name>` - Set executable name (default: `exe.out`)

#### Examples

Compile single file:
```bash
ura src/file.ura
```

Compile with optimization:
```bash
ura src/file.ura -O2 -o myapp
```

Compile with sanitizer:
```bash
ura src/file.ura -san -o myapp
```

Compile multiple files:
```bash
ura main.ura utils.ura -O2 -o myapp
```

### Typical Workflow

1. Write code in `src/file.ura`
2. Build compiler: `build`
3. Compile your program: `ura src/file.ura -O2 -o myapp`
4. Run: `./build/myapp`
5. If working correctly, save to tests: `copy <category> <name>`
6. Run test suite: `tests`

## Quick Start

### Hello World

1. Source the environment:
```bash
source config.sh
```

2. Build the compiler:
```bash
build
```

3. Create `src/file.ura`:
```ura
use "@/io"

main():
    printf("Hello, World!\n")
```

4. Compile and run:
```bash
ura src/file.ura -o hello
./build/hello
```

### Example: Fibonacci

```ura
use "@/io"

fn fib(n int) int:
    if n <= 1:
        return n
    return fib(n - 1) + fib(n - 2)

main():
    result int = fib(10)
    printf("Fibonacci(10) = %d\n", result)
```

### Example: String Length

```ura
use "@/io"

fn strlen(str chars) int:
    i int = 0
    while str[i] != '\0':
        i += 1
    return i

main():
    len int = strlen("Hello, Ura!")
    printf("Length: %d\n", len)
```

### Example: References

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

### Example: Structs

```ura
use "@/io"

struct Point:
    x int
    y int

fn distance_squared(p Point) int:
    return p.x * p.x + p.y * p.y

main():
    point Point
    point.x = 3
    point.y = 4
    
    dist_sq int = distance_squared(point)
    printf("Distance squared: %d\n", dist_sq)
```

## Standard Library

The `src/ura-lib/` directory contains prototype declarations for C standard library functions. Import them using the `@` prefix:

```ura
use "@/header"    // Imports all modules
use "@/io"        // I/O functions only
use "@/memory"    // Memory management only
use "@/string"    // String functions only
```

### Available Modules

#### io.ura
File I/O, formatted I/O (printf, scanf, puts, etc.), file operations

```ura
proto fn printf(format chars, ...) int
proto fn puts(str chars) int
proto fn fopen(path chars, mode chars) pointer
proto fn fclose(file pointer) int
```

#### memory.ura
Memory management functions

```ura
proto fn malloc(size int) pointer
proto fn calloc(len int, size int) pointer
proto fn free(ptr pointer) void
proto fn realloc(ptr pointer, newSize int) pointer
```

#### string.ura
String manipulation (strlen, strcmp, strcpy, etc.)

```ura
proto fn strlen(s chars) int
proto fn strcmp(a chars, b chars) int
proto fn strcpy(dest chars, src chars) chars
proto fn strcat(dest chars, src chars) chars
```

#### math.ura
Mathematical functions

#### stdlib.ura
General utilities (atoi, rand, exit, etc.)

#### time.ura
Time and date functions

#### signals.ura
Signal handling

## Testing

The test suite is organized by feature category in the `tests/` directory:

- **builtins/** - Built-in functions (printf, puts, stack, typeof)
- **data_types/** - Type system tests, especially references
- **fn/** - Function tests (parameters, return values, references)
- **if/** - Conditional statements
- **while/** - Loop constructs
- **op/** - All operators (arithmetic, logical, comparison)
- **structs/** - Struct declarations and usage
- **libft/** - Standard library function implementations

Each test consists of:
- `.ura` file - Source code
- `.ll` file - Expected LLVM IR output

Run tests with:
```bash
tests              # All tests
tests op           # Specific category
tests op structs   # Multiple categories
```

## Compiler Architecture

### Compilation Pipeline

1. **Tokenization** - Lexical analysis, converts source to tokens
2. **Parsing** - Builds Abstract Syntax Tree (AST)
3. **Semantic Analysis** - Type checking, symbol resolution
4. **IR Generation** - Generates LLVM IR
5. **Optimization** - LLVM optimization passes (based on -O flags)
6. **Code Generation** - Produces object files and executables

### Source Files

#### main.c
- Tokenizer (lexical analysis)
- Parser (AST construction)
- Semantic analysis
- Entry point and orchestration

#### llvm.c
- LLVM C API wrappers
- Instruction builders
- Type operations
- Module management

#### header.h
- Type definitions (Token, Node, LLVM types)
- Enumerations (token types, operators)
- Global variables
- Function declarations

### Key Features

#### Indentation-Based Syntax
Ura uses indentation to define code blocks (similar to Python):

```ura
main():
    if condition:
        statement1
        statement2
    else:
        statement3
```

#### Reference Semantics
References provide safe aliasing:
- First assignment binds to a variable (one-time binding)
- All subsequent assignments modify the bound variable
- Cannot be rebound to different variables
- Type-safe at compile time

#### Struct Support
Structs support:
- Field access with dot notation
- Nested structs
- Passing by value or reference
- Member initialization

## LLVM Directory

The `llvm/` directory contains experimental code for testing LLVM features before integration into the main compiler.

### Purpose

- Feature exploration and prototyping
- LLVM API learning and testing
- Proof of concepts
- Regression testing for LLVM IR generation

### Structure

- **main.c** - Experimental compiler with different syntax
- **utils.c** - Parsing and utility functions
- **wrapper.c** - LLVM API wrappers
- **header.h** - Type definitions
- **examples/** - 14 example programs (000-013)
- **tests/** - Expected LLVM IR output for each example
- **config.sh** - Build commands

### Differences from Main Compiler

The experimental compiler uses slightly different syntax for testing purposes:
- `def` keyword instead of `fn`
- `end` keyword to close blocks
- Different reference semantics

## Current Status

### Implemented Features

- ✅ Basic data types (int, char, chars, bool, void, long, short, float)
- ✅ Variables and constants
- ✅ Arithmetic operators (+, -, *, /, %)
- ✅ Comparison operators (==, !=, <, >, <=, >=, is)
- ✅ Logical operators (and, or, not, &&, ||, !)
- ✅ Assignment operators (=, +=, -=, *=, /=, %=)
- ✅ Functions with parameters and return values
- ✅ Function prototypes for C interop
- ✅ Variadic functions
- ✅ If/elif/else conditionals
- ✅ While loops with break and continue
- ✅ Array indexing
- ✅ Type casting (as keyword)
- ✅ References (binding and rebinding)
- ✅ Reference parameters
- ✅ Structs with fields
- ✅ Nested structs
- ✅ Struct references
- ✅ Module imports (use statement)
- ✅ Standard library integration
- ✅ Comments (single and multi-line)
- ✅ Stack allocation (stack builtin)
- ✅ Type introspection (typeof builtin)
- ✅ Multiple file compilation
- ✅ Optimization levels (-O0 to -O3, -Os, -Oz)
- ✅ AddressSanitizer support

### In Progress / Planned

- ⏳ For loops
- ⏳ Struct methods
- ⏳ Arrays (proper declarations)
- ⏳ Global variables
- ⏳ Const/immutable variables
- ⏳ Type inference
- ⏳ Exception handling (try/catch)
- ⏳ Operator overloading
- ⏳ Generics/templates
- ⏳ Garbage collection
- ⏳ Package manager
- ⏳ Switch/case statements

See `PLAN.md` for detailed roadmap and `TODO.md` for current tasks.

## Development

### Debugging

The compiler includes debug output that can be enabled during development. Check the source code for debug flags.

### Adding New Features

1. Update token types in `header.h` (Type enum)
2. Add tokenization logic in `main.c`
3. Add parsing logic in `main.c`
4. Add code generation in `main.c`
5. Add tests in `tests/`
6. Update documentation

### Code Style

- Use `indent` command to format code
- Follow existing patterns in the codebase
- Add comments for complex logic
- Write tests for new features

## Contributing

This is a work-in-progress educational project. Contributions, suggestions, and feedback are welcome.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- LLVM Project for the compiler infrastructure
- Inspired by various programming language implementations

## Contact

**Mohammed Hrima**
- Email: mohammed.hrima1998@gmail.com
- LinkedIn: [linkedin.com/in/mohammedhrima](https://www.linkedin.com/in/mohammedhrima)
