# Ura Lang

Ura is a compiled programming language (work in progress) that compiles to LLVM IR. The compiler is written in C and uses LLVM for code generation and optimization.

## Project Overview

Ura is designed as a statically-typed, imperative language with a focus on simplicity and direct compilation to native code through LLVM. The language supports basic data types, control flow, functions, references, and interoperability with C libraries through prototype declarations.

## Repository Structure

```
.
├── src/                    # Main compiler source code
│   ├── main.c             # Entry point, tokenizer, parser
│   ├── gen.c              # IR generation and LLVM code generation
│   ├── llvm.c             # LLVM C API wrappers
│   ├── utils.c            # Utility functions, memory management
│   ├── header.h           # Type definitions and function declarations
│   ├── config.sh          # Build system and development commands
│   ├── file.ura           # Working file for development
│   ├── tests/             # Test suite organized by feature
│   │   ├── builtins/      # Built-in function tests
│   │   ├── data_types/    # Type system tests (including references)
│   │   ├── if/            # Conditional statement tests
│   │   ├── while/         # Loop tests
│   │   ├── op/            # Operator tests
│   │   └── libft/         # Standard library function implementations
│   ├── ura-lib/           # Standard library prototypes
│   │   ├── header.ura     # Main library header
│   │   ├── io.ura         # I/O operations
│   │   ├── string.ura     # String manipulation
│   │   ├── memory.ura     # Memory management
│   │   ├── math.ura       # Mathematical functions
│   │   ├── stdlib.ura     # Standard library functions
│   │   ├── time.ura       # Time operations
│   │   └── signals.ura    # Signal handling
│   └── build/             # Compiler output directory
│
├── llvm/                   # LLVM experimentation and testing
│   ├── main.c             # Experimental compiler implementation
│   ├── utils.c            # Utility functions for experiments
│   ├── wrapper.c          # LLVM wrapper functions
│   ├── header.h           # Header for experimental code
│   ├── config.sh          # Build configuration for experiments
│   ├── examples/          # Example programs (000-013)
│   ├── tests/             # Expected LLVM IR outputs
│   └── build/             # Build artifacts
│
├── workspaces/            # VS Code workspace configurations
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
- `ref` - Reference type modifier

### Variable Declaration

```ura
main():
    a int = 10
    name chars = "Hello"
    flag bool = True
    c char = 'x'
```

### References

References allow binding to existing variables:

```ura
main():
    a int = 10
    b int ref = a    // b is a reference to a
    b = 20           // modifies a through the reference
    return a         // returns 20
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
fn add(a int, b int) int:
    return a + b

main():
    result int = add(5, 3)
    return result
```

### Function Prototypes

External C functions can be declared using `proto`:

```ura
proto fn puts(str chars) int
proto fn malloc(size int) pointer
proto fn free(ptr pointer) void

main():
    puts("Hello, World!")
```

### Variadic Functions

Functions with variable arguments:

```ura
fn printf(format chars, ...) int
```

### Control Flow

#### If/Elif/Else

```ura
main():
    a int = 5
    if a < 10:
        return 1
    elif a < 20:
        return 2
    else:
        return 3
```

#### While Loops

```ura
main():
    i int = 0
    while i < 10:
        i += 1
    return i
```

#### Break and Continue

```ura
main():
    i int = 0
    while i < 10:
        i += 1
        if i == 5:
            break
    return i
```

### Operators

#### Arithmetic Operators
- `+` Addition
- `-` Subtraction
- `*` Multiplication
- `/` Division
- `%` Modulo

#### Comparison Operators
- `==` or `is` Equal
- `!=` Not equal
- `<` Less than
- `>` Greater than
- `<=` Less than or equal
- `>=` Greater than or equal

#### Logical Operators
- `and` or `&&` Logical AND
- `or` or `||` Logical OR
- `not` or `!` Logical NOT

#### Assignment Operators
- `=` Assignment
- `+=` Add and assign
- `-=` Subtract and assign
- `*=` Multiply and assign
- `/=` Divide and assign
- `%=` Modulo and assign

### Array Access

```ura
main():
    str chars = "hello"
    c char = str[0]    // Access first character
    str[0] = 'H'       // Modify first character
```

### Type Casting

```ura
main():
    a int = 65
    c char = a as char
    return c
```

### Stack Allocation

Dynamic stack allocation:

```ura
proto fn calloc(len int, size int) chars

main():
    buffer chars = calloc(100, 1)
    buffer[0] = 'A'
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

Import other Ura files:

```ura
use "io"
use "string"
```

## Compiler Architecture

### Compilation Pipeline

The Ura compiler follows a multi-stage compilation process:

1. **Tokenization** (`tokenize()` in `main.c`)
   - Reads source file
   - Handles `use` statements for imports
   - Produces token stream with type, value, and position information
   - Tracks indentation for block structure

2. **Parsing** (`parse()` in `main.c`)
   - Builds Abstract Syntax Tree (AST)
   - Recursive descent parser
   - Operator precedence handling
   - Scope management

3. **IR Generation** (`gen_ir()` in `gen.c`)
   - Type checking
   - Symbol resolution
   - Semantic analysis
   - Intermediate representation construction

4. **Code Generation** (`gen_asm()` in `gen.c`)
   - LLVM IR emission
   - Register allocation (handled by LLVM)
   - Optimization passes

5. **Output** (`finalize()` in `utils.c`)
   - Module verification
   - LLVM IR file generation (.ll)

### Source Files

#### main.c
- **Tokenizer**: Lexical analysis, converts source text to tokens
- **Parser**: Builds AST from token stream
- **Entry point**: Orchestrates compilation phases
- **Scope management**: Tracks variables, functions, and structs per scope
- **File handling**: Manages `use` statements and file imports

#### gen.c
- **IR Generation**: Type checking and semantic analysis
- **LLVM Code Generation**: Emits LLVM IR instructions
- **Reference handling**: Implements reference semantics
- **Operator implementation**: Binary and unary operations
- **Control flow**: If/while/break/continue code generation
- **Function calls**: Parameter passing and return values

#### llvm.c
- **LLVM Wrappers**: Thin wrappers around LLVM C API
- **Builder operations**: Instruction emission helpers
- **Type operations**: Type creation and manipulation
- **Module operations**: Function and global management

#### utils.c
- **Error handling**: Compilation error reporting
- **Debug output**: Token and AST printing
- **Memory management**: Allocation and cleanup
- **String utilities**: String manipulation helpers
- **Symbol tables**: Variable and function lookup
- **Type utilities**: Type conversion and checking
- **LLVM initialization**: Context, module, and builder setup

#### header.h
- **Type definitions**: Token, Node, LLVM wrapper types
- **Enumerations**: Token types, data types
- **Global variables**: Compiler state
- **Function declarations**: All public interfaces
- **Macros**: Debug, allocation, error checking

### Key Data Structures

#### Token
Represents a lexical unit with:
- Type (keyword, identifier, literal, operator)
- Value (for literals)
- Position (file, line, column)
- Metadata (is_dec, is_ref, is_variadic, etc.)
- LLVM value reference

#### Node
AST node with:
- Token reference
- Left/right children (binary operations)
- Child array (statements, function bodies)
- Symbol tables (variables, functions, structs)

### Scope Management

The compiler maintains a scope stack (`Gscoop`) for:
- Variable declarations and lookups
- Function declarations and lookups
- Nested function support
- Block-level scoping

### Reference Implementation

References are implemented as:
- Pointers at LLVM level
- Automatic dereferencing on read
- Binding semantics on first assignment
- Value assignment on subsequent assignments

## LLVM Directory

The `llvm/` directory contains experimental code and prototypes for testing LLVM features before integration into the main compiler.

### Purpose

- **Feature exploration**: Test LLVM capabilities
- **Proof of concepts**: Validate implementation approaches
- **Learning resource**: Examples of LLVM C API usage
- **Regression testing**: Verify LLVM IR generation

### Structure

- **main.c**: Experimental compiler with different syntax
- **utils.c**: Parsing and utility functions
- **wrapper.c**: LLVM API wrappers
- **header.h**: Type definitions
- **examples/**: 14 example programs (000-013)
- **tests/**: Expected LLVM IR output for each example
- **config.sh**: Build and test commands

### Example Programs

The `examples/` directory contains numbered test programs:

- **000**: Simple main function
- **001**: While loop with compound assignment
- **002**: If/elif/else conditionals
- **003**: Built-in functions and array access
- **004**: Functions with multiple parameters
- **005**: Nested functions
- **006**: Variadic functions
- **007**: References
- **008**: Reference parameters
- **009**: Type casting
- **010**: Stack allocation
- **011**: Try/catch exception handling
- **012**: Array bounds checking
- **013**: Additional features

Each example has a corresponding `.ll` file in `tests/` with the expected LLVM IR output.

### Differences from Main Compiler

The LLVM experimental compiler uses slightly different syntax:
- `def` keyword for functions instead of `fn`
- `end` keyword to close blocks instead of indentation
- `protoFunc` instead of `proto fn`
- Different reference semantics

## Build System

### Prerequisites

- C compiler (clang recommended)
- LLVM development libraries
- llvm-config tool
- Standard build tools (make, etc.)

### Commands (src/config.sh)

Source the configuration to load the build environment:

```bash
cd src
source config.sh
```

#### build
Compiles the Ura compiler from C source files.

```bash
build
```

Compiles: `main.c`, `gen.c`, `utils.c`, `llvm.c` with:
- Address sanitizer
- Null pointer checks
- LLVM flags from `llvm-config`
- Warning flags

Output: `build/ura` executable

#### ir
Compiles `file.ura` to LLVM IR.

```bash
ir
```

Requirements: `file.ura` must exist in `src/`
Output: `build/file.ll`

#### asm
Converts LLVM IR to assembly and links to executable.

```bash
asm
```

Requirements: `build/file.ll` must exist (run `ir` first)
Steps:
1. Runs `llc` to generate assembly (`build/file.s`)
2. Links with clang to create executable (`build/exe.out`)

#### comp
Complete compilation pipeline: build + ir + asm.

```bash
comp
```

Equivalent to running `build && ir && asm`

#### run
Compiles and executes the program.

```bash
run
```

Equivalent to `comp && build/exe.out`

#### tests
Runs the test suite.

```bash
tests [folder]
```

- Without argument: runs all tests
- With folder name: runs tests in specific category

Examples:
```bash
tests              # Run all tests
tests op           # Run operator tests only
tests builtins     # Run built-in function tests
```

Test process:
1. Compiles each `.ura` file in `tests/`
2. Generates LLVM IR
3. Compares with expected `.ll` file (ignoring first 2 lines)
4. Reports pass/fail for each test

#### copy
Saves current `file.ura` and generated IR to test directory.

```bash
copy <folder> <filename>
```

Example:
```bash
copy op add
```

Creates:
- `tests/op/add.ura` (copy of `file.ura`)
- `tests/op/add.ll` (generated IR)

#### indent
Formats C source code using astyle.

```bash
indent
```

Formatting rules:
- C mode
- 3-space indentation
- Pad operators and headers
- Keep one-line statements/blocks
- Convert tabs to spaces
- Max line length: 150

#### update
Reloads the configuration file.

```bash
update
```

Useful after modifying `config.sh`

### Typical Workflow

1. Write code in `file.ura`
2. Test compilation: `run`
3. If working, save to tests: `copy <category> <name>`
4. Run test suite: `tests`

## Quick Start

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

### Writing Your First Program

1. Navigate to the src directory:
```bash
cd src
```

2. Source the build environment:
```bash
source config.sh
```

3. Create a program in `file.ura`:
```ura
main():
    a int = 42
    return a
```

4. Compile and run:
```bash
run
```

### Example: Hello World

```ura
proto fn puts(str chars) int

main():
    puts("Hello, World!")
    return 0
```

### Example: Fibonacci

```ura
fn fib(n int) int:
    if n <= 1:
        return n
    return fib(n - 1) + fib(n - 2)

main():
    result int = fib(10)
    return result
```

### Example: String Length

```ura
fn strlen(str chars) int:
    i int = 0
    while str[i] != '\0':
        i += 1
    return i

main():
    len int = strlen("Hello")
    return len
```

## Standard Library

The `src/ura-lib/` directory contains prototype declarations for C standard library functions, organized by category:

- **io.ura**: File I/O, formatted I/O (printf, scanf, etc.)
- **string.ura**: String manipulation (strlen, strcmp, strcpy, etc.)
- **memory.ura**: Memory management (malloc, free, memcpy, etc.)
- **math.ura**: Mathematical functions
- **stdlib.ura**: General utilities
- **time.ura**: Time and date functions
- **signals.ura**: Signal handling

To use standard library functions:

```ura
use "ura-lib/header"

main():
    // Now you can use any standard library function
    puts("Hello")
```

## Testing

The test suite is organized by feature category:

- **builtins/**: Built-in functions (putchar, puts, stack, typeof)
- **data_types/**: Type system tests, especially references
- **if/**: Conditional statements
- **while/**: Loop constructs
- **op/**: All operators (arithmetic, logical, comparison)
- **libft/**: Standard library function implementations

Each test consists of:
- `.ura` file: Source code
- `.ll` file: Expected LLVM IR output

Run tests with:
```bash
tests           # All tests
tests op        # Specific category
```

## Current Status

### Implemented Features

- ✅ Basic data types (int, char, chars, bool, void)
- ✅ Variables
- ✅ Arithmetic operators
- ✅ Comparison operators
- ✅ Logical operators
- ✅ Assignment operators
- ✅ Functions with parameters and return values
- ✅ Function prototypes for C interop
- ✅ If/elif/else conditionals
- ✅ While loops
- ✅ Break and continue
- ✅ Array indexing
- ✅ Type casting
- ✅ References
- ✅ Module imports (use statement)
- ✅ Comments (single and multi-line)
- ✅ Variadic functions
- ✅ Stack allocation

### In Progress / Planned

- ⏳ For loops
- ⏳ Structs and methods
- ⏳ Arrays (proper declarations)
- ⏳ Global variables
- ⏳ Const/immutable variables
- ⏳ Type inference
- ⏳ Exception handling (try/catch)
- ⏳ Operator overloading
- ⏳ Generics/templates
- ⏳ Garbage collection
- ⏳ Package manager

See `PLAN.md` for detailed roadmap and `TODO.md` for current tasks.

## Development

### Debugging

Enable debug output by setting `DEBUG` flag in `header.h`:

```c
#define DEBUG 1
```

This enables:
- Token stream printing
- AST visualization
- IR generation tracing

### Adding New Features

1. Update token types in `header.h` (Type enum)
2. Add tokenization logic in `main.c` (tokenize function)
3. Add parsing logic in `main.c` (appropriate parser function)
4. Add IR generation in `gen.c` (gen_ir function)
5. Add code generation in `gen.c` (gen_asm function)
6. Add tests in `src/tests/`
7. Update documentation

### Code Style

- 3-space indentation
- K&R brace style
- Descriptive variable names
- Comments for complex logic
- Use `indent` command to format

## Contributing

This is a work-in-progress educational project. Contributions, suggestions, and feedback are welcome.

## Acknowledgments

- LLVM Project for the compiler infrastructure
- Inspired by various programming language implementations
