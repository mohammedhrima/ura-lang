
# Wolf-C Compiler

Welcome to Wolf-C, a lightweight compiler with a syntax inspired by Python and C. Wolf-C is designed to be simple and flexible, currently supporting two data types: `int` and `bool`.

Comments in Wolf-C use familiar C-style syntax:
```c
//  single line comment
/*
    multi line comment
*/
```

## Features
+ Data types: int, bool
+ C-like operations: Arithmetic, assignment, comparison
+ Python-like structure: Indentation for block definition
+ Generates output: Abstract Syntax Tree (AST), Intermediate Representation (IR), and Assembly

## Code Examples
### Example 1: Variable Initialization
```python
main():
    int x = 10
    bool is_valid = True
    bool has_error = False
```

### Example 2: Conditional Statements
```python
main():
    int x = 5
    int y = x * 2
    y += 1

    if y > 10:
        x = 0
    else
        x = 1
    if x == 0:
        y = 42
```

### Example 3: Nested Conditions
```python
main():
    int a = 3
    int b = a + 4

    if b > 5:
        if a == 3:
            b += 10
```

## How to Use (for now)
### Step 1: Build the Compiler
1. Run `make` to create a Docker container.
2. Use `docker ps` to find the container ID of the running project.
3. Access the container using the command:
bash
```bash
docker exec -it <container_id> bash
```
#### Step 2: Compile Your Code
+ modify file.w inside the code directory.
+ Run the build command to compile your file:
```bash
    build 
```

+ This will generate and print:
    + Abstract Syntax Tree (AST)
    + Intermediate Representation (IR)
    + Assembly Code