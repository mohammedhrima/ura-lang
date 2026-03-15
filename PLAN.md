+ declaring variable with name self is forbidden


- declare:
   + global variables (primitives + structs; init/clean auto-called in main)
   - const / immutable variables
   - type inference
   - modules / namespaces
   - program arguments (argc / argv in main)

- operators:
   + [] (index operator)
   + assignment operators (+=, -=, *=, /=, %=, etc.)

- conditions, loops:
   - for loop
   - for loop / range-based for (for i in 0..10)
   - switch/case
   - pattern matching (match expression with enums/types)
   - try/catch

- functions:
   + built in functions:
      + output
   - polymorphism (overloading)
   - default parameter values (fn greet(name chars = "World") void)
   - multiple return values / tuples (fn minmax(a int, b int) (int, int))

- structs:
   + structs
   + methods
   + struct as attribute to another struct
   - operator overloading
   - inheritance
   - templates
   - generics (if not templates)

- memory:
   - garbage collection
   + manual memory management (malloc/free style)
   + references / pointers
   - smart pointers (optional)

- types:
   - enums
   - type aliases (type Byte = char)
   - string interpolation ("Hello, {name}!")

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
   + standard library (math, strings, collections, time, etc.)

- error handling:
   - try/catch/finally
   - error codes
   - assertions (assert condition, "message")

- install / distribution:
   + install for linux
   + install for macos
   + install for windows

- tooling:
   - formatter (auto-format .ura files)
   - language server / LSP (syntax highlighting, autocomplete)
   - REPL (interactive interpreter)

- concurrency (optional, advanced):
   - threads
   - atomic operations

- later use "direnv" instead of config.sh

- projects:
   + 42 libft
   - donuts
   - sample CLI tool
   + example web server (tcp server: basic chat + command-based variant)
