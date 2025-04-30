# Wolf-C Compiler
- still under development
- Welcome to Wolf-C, a lightweight compiler with a syntax inspired by Python and C.

## How to Use (for now)
### Step 1: Build the Compiler
1. Run `make` to create a Docker container.
2. Use `docker ps` to find the container ID of the running project.
3. Access the container using the command:
```bash
    docker exec -it <container_id> bash
```
### Step 2: Compile Your Code
+ modify file.w inside the code directory.
+ Run the build command to compile your file:
```bash
    wcc file.w
```

## Features:
1. Print Something:
```c
    main():
        chars str = "user"
        output("Hello, world! from ", user, "\n")
```

2. Variables and Data Types:
```c
    main():
        chars name = "Alice"      // string
        int age = 25              // integer
        float height = 5.6        // float
        bool is_student = True    // boolean
        output(name, age, height, is_student)
```

3. If Statements:
```c
    main():
        int age = 18
        if age >= 18: output("You're an adult!\n")
        else: output("You're a minor.\n")
```

4. Loops:
```c
    main():
        int count = 0
        while count < 3:
            output("Counting:", count, "\n")
            count += 1
```

5. Functions:
```c
    func int greet(chars name):
        output("Hello, ", name, "\n")
        return 42

    main():
        greet("Alice")
```

6. Structs:
```c
    struct Person:
        chars name
        int age
    
    main():
        Person user
        user.name = "mohammed"
        user.age = 27
```

7. import files:
```c
use config      // will import ./config.w
use :config     // will import /config.w
usr :src:config // will import /src/config
```