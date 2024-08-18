+ 16/08:
    - variable declaration/assignement:
        - int, char, bool, float, chars
    - optimazations on operations
+ 17/08:
    + if / while / break / continue
+ 18/08:
    + function call / declaration / return
    + function return struct:
        + first: handle function take reference
        + declare variable in struct and send it as parmeter
    + Ptr datatype
    - using c standart functions
    # function that takes asm instruction 
    # and injects them in the code
    + asm("mov rax, 0") 
+ 19/08:
    + classes
    + composite data types
+ 21/08:
    + modules
    + swicth case
+ 22/08:
    + package manager

+ data types:
    + primary:
        + int, char, bool, float, chars
        + void: don't allow variable declaration
        + ptr: use only on .wlib files
    + composite: (use stdandart classes from std.wlib)
        + string
        + array:
            + array[][] int
        + maps
    + example: int c

+ operations: + - / * && || and or =

+ classes:
    + attributes
    + methods
    + static members
    + inhertance

    class foo:
        atrribute:
            int x
            int y
        contractor(int x_, int y_):
            x = x_
            y = y_
        destractor():
            // do something
        methods:
            func void whomai():
                sys.puts("I am user\n")

+ statements:
    + if, else, elif
    + while, for, break, continue
    + switch case default

+ functions:
    + delaration
    + return type
    + parameter: value / reference
    + return: value / reference
    + argument compatibility
    + recursion
    + call destractor after each scoop if is not reference

+ input / output

+ memory: allocation / free

+ Error:
    + try, catch, throw

+ Modules:
    + encampsulation
    + file as module

+ Package manager:
    + check update for the package and the language

+ threads

+ run time errors:
    + devision by 0

+ after:
    + handle this "aaaa""efg"
    + handle unsigned data
