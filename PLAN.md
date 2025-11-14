- add attribute "int used" in Token struct to check how manny times a function or variables is used so it can be removed if used is still 0 in the optimization step

- declare:
   + int, float, long, double, char, chars
   - arrays
   - reference to variables
   - global variables
   - const / immutable variables
   - type inference
   - modules / namespaces

- operators:
   + math operators (+, -, *, /, %, **)
   + logic operators (&&, ||, !, <, <=, >, >=, ==, !=)
   - override operation in structs
   - [] (index operator)
   - assignment operators (+=, -=, *=, /=, etc.)

- conditions, loops:
   + while
   + break, continue
   + if, elif, else
   - for loop
   - switch/case
   - try/catch

- assignment:
   + id = id
   + id = value
   + id = func return
   - id = ref
   - ref = id
   - ref = value
   - ref = ref
   - ref = func return

- functions:
   + built in functions:
      + output
   + declare function
   + function call
   + function return
   + function takes arguments
   - variadic functions
   - polymorphism (overloading)
   - recursion optimization (tail call)

- structs:
   + structs
   - methods
   - operator overloading
   - inheritance
   - templates
   - generics (if not templates)
   - struct as attribute to another struct

- memory:
   - garbage collection
   - manual memory management (malloc/free style)
   - references / pointers
   - smart pointers (optional)

- filesystem:
   - input, output, errput
   + import files
   - open files
   - read/write files
   - directory management

- macros:
   - preprocessing macros

- modules / packages:
   - package manager (config file to install/build)
   - linking external code
   - linking with C/C++ libraries
   - standard library (math, strings, collections, time, etc.)

- error handling:
   - try/catch/finally
   - error codes

- install / distribution:
   - install for linux
   - install for macos
   - install for windows

- concurrency (optional, advanced):
   - threads
   - atomic operations

- use llvm optimizer: opt -O2

- projects:
   - 42 libft
   - donuts
   - sample CLI tool
   - example web server
