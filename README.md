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

- EXPECTED RESULT:
- variables:
    - starts only with letter
    - by levels

- data Types:

    - single:
        - int:
            - negative numbers
            - arithmetic operators: / * + - % ++ --
            - relational operators: < > <= >= == !=
        - float:
            - negative numbers
            - arithmetic operator: / * + -
            - relational operator: < > <= >= == != 
        - char:
            - arithmetic operator: 
                - only for mutable + 
            - relational operator: == !=
            - set rbp - 14: 0 or 1 (mu / imut)
            - set rbp - 13: len
            - set rbp - 08: address in stack 
        - bool:
            - relational operator: == != !
            - logical operators: && || and or

    - assign: += -= *= /=

    - array:
        - data_type[level]
        - logic operator: == !=
        - arithmetic operator: +
        - set rbp - 14: 0 or 1 (mu / imut)
        - set rbp - 13: len
        - set rbp - 08: address in stack

    - Class:
        - take params by refre or value
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
    


- methods
- typedefs: use int as num
- namespaces
- iteratiin using: []

- file system:
    - import files
    - open / close files

- conditions and loops:
    - continue keyword
    - break keyword
    - syntax 1:
        if statement:
            // code here ...
        elif:
            // code here ...
        else:
            // code here ...
    - syntax 2:
        while condition:
            // code here ...
    - syntax 3:
        for data_type i in array_data_type:
            // code here ...
    
- function declaration:
    - syntax:
        data_type func func_name(params ...):
            // code here ...
    - starts only with letter
    - take params by refre or value
    - variadic arguments
    - return keyword
    - add '_' before all declared functions:
    - built in functions:
        - output
        - is_dynamic
        - is_fix
        - dyn: allocate in heap
    - function call
        - before calling function push rbp to the stack and pop it later
        - each function has its own rbp
    - polymorphism

- memory management:
    - heap allocation
    - stack allocation
    - garbage collector
    - normal mode
        - free function
    - garbage collection mode
    - protect mode:
        - check all []