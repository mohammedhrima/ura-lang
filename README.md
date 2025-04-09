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
#### Step 2: Compile Your Code
+ modify file.w inside the code directory.
+ Run the build command to compile your file:
```bash
    build 
```

