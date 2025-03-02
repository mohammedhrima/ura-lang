# TODO

## Core Features

- [ ] Call function
- [ ] Function return
- [ ] Function takes arguments
- [ ] Implement built-in functions
- [ ] Implement output functionality
- [ ] check function existance in generate_ir
- [ ] import files in tokenize

## Assembly Generation

- [ ] Generate assembly with proper indentation (tabs)
main:
    .if1:
        mov eax, 1
    .endif1:


#### 2. **Data_Types.md**
This file will define the data types, their operators, and related notes.

# Data Types

## Primitive Types
### `int`
- **Arithmetic Operators**: `/`, `*`, `+`, `-`, `++`, `--`
- **Logical Operators**: `==`, `!=`, `>`, `<`, `>=`, `<=`

### `string` (heap allocated)
- **Arithmetic Operators**: `+` (concatenation)
- **Logical Operators**: `==`, `!=`

### `char`
- **Arithmetic Operators**: `++`, `--`
- **Logical Operators**: `==`, `!=`

### `array` (heap allocated)
- **Arithmetic Operators**: `+` (concatenation)
- **Logical Operators**: `==`, `!=`

# Language Syntax

## Data Types
### `array`
- Syntax: `data_type[level]`
- Logical Operators: `==`, `!=`
- Arithmetic Operators: `+`
- Memory Layout:
  - `rbp - 14`: 0 or 1 (mutable/immutable)
  - `rbp - 13`: Length
  - `rbp - 8`: Address in stack

### Primitive Types
#### `int`
- Supports negative numbers
- Arithmetic Operators: `/`, `*`, `+`, `-`, `%`, `++`, `--`
- Relational Operators: `<`, `>`, `<=`, `>=`, `==`, `!=`

#### `float`
- Supports negative numbers
- Arithmetic Operators: `/`, `*`, `+`, `-`
- Relational Operators: `<`, `>`, `<=`, `>=`, `==`, `!=`

#### `char`
- Arithmetic Operators: `+` (only for mutable)
- Relational Operators: `==`, `!=`

#### `bigint` / `bigfloat`
- Store value in stack
- Pass address in `rax` between functions

### Classes
- Syntax:
  ```python
  class ClassName:
      attributes:
          # Attributes here
      constructor(data_type1 params...):
          # Code here
      constructor(data_type2 params):  # Polymorphism
          # Code here
      destructor:
          # Code here
      operators:
          [+]:
              # Code here
          [-]:
              # Code here

    inherent class ChildClass from ParentClass:
        constructor(data_type1 params...):
            # Code here
        constructor(data_type2 params):  # Polymorphism
            # Code here
        destructor:
            # Code here
        operators:
            [+]:
                # Code here
            [-]:
                # Code here
    ```

### Control Structures
- try, catch
- switch, case
- Loops:
    - for loops
    - continue 
    - break

### Functions
- Syntax:
    func foo() int:
        # Code here
    - Parameters:
        - Pass by value or reference
        - Variadic arguments
    - return keyword

- Built-in Functions:
- is_dynamic, is_fixed
- dyn: Allocate in heap
- output, input

### Memory Management
- Heap allocation
- Stack allocation
- Garbage Collection:
- Thread-based garbage collector
- Reference counting

### Modes:
- Normal mode: Manual free
- Garbage collection mode: Automatic
- Protect mode: Bounds checking

### File System
- Import files
- Open/close files

### Compiler Directives
- #if0, #else, #endif

### Optimization
- Test heap vs stack variables
- Optimize code generation
- Verify dynamic and fixed data types for all operations

### Error Handling
- Display all parsing errors before exiting
- Index out of range checks for arrays and chars

