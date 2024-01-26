- NOTES:
    + testb : 'and' bitwise operator
    + write system call:
        rdi : fd
        rsi : pointer
        rdx : length

    + quad: 64bit
    + long: 32bit
    + word: 16bit
    + byte: 08bit

    + Stop container and remove images:
        + docker-compose down -v --rmi all

- EXPECTED RESULT:

- data Types:
    - array:
        + data_type[level]
        - logic operator: == !=
        - arithmetic operator: +
        - set rbp - 14: 0 or 1 (mu / imut)
        - set rbp - 13: len
        - set rbp - 08: address in stack
    
    - single:
        - int:
            - negative numbers
            + arithmetic operators: / * + - % ++ --
            + relational operators: < > <= >= == !=
        - float:
            - negative numbers
            - arithmetic operator: / * + -
            - relational operator: < > <= >= == != 
        - char:
            - arithmetic operator: 
                - only for mutable + 
            + relational operator: == !=
        - bigint / bigfloat:
            - put value in stack
            - send it's address in rax between functions

    - Class:
        - take params by refr or value
        - syntax:
            class class_name:
                constractor(data_type1 params...):
                    // code here ...
                constractor(data_type2 params): (Polymorphism)
                    // code here ...
                destractor:
                    // code here ...
                operators:
                    [+]:
                        // code here ...
                    [-]:
                        // code here ...
        - inherentence:
            inherent class class_name1 from class_name2:
                constractor(data_type1 params...):
                    // code here ...
                constractor(data_type2 params): (Polymorphism)
                    // code here ...
                destractor:
                    // code here ...
                operators:
                    [+]:
                        // code here ...
                    [-]:
                        // code here ...
    
- try, catch
- switch, case
- methods
- typedefs: use int as num
- namespaces
- iteratiin using: []

- file system:
    - import files
    - open / close files

- conditions and loops:
    + continue keyword
    + break keyword
    - add for loops
    
- function declaration:
    + if variable sent by refrence allocate there space in function code then push rbp to the stack
    + take params by refrence or value
    - variadic arguments
    + return keyword
    - built in functions:
        - is_dynamic
        - is_fix
        - dyn: allocate in heap
        + output
        - input
    - polymorphism

- memory management:
    - heap allocation
    - stack allocation
    - check allocation failure
    - garbage collector:
        - use threads 
    - normal mode
        - free function
    - garbage collection mode
        - add bytes at left of pointer undecates how many variable is holding it
    - protect mode:
        - check all []

- add #if0 #else #endif
- test heap vs stack variables (bool ...)
- optimize the code
- verify dynamic and fix data type for all operations
- compile withh Wall Werror Wextra
- display all parsing error before exiting
- build my own version of docker to run in it
