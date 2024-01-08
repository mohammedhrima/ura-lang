- variables:
    - starts only with letter
    - by levels

- data Types:

    - single:
        - int:
            - negative numbers
            - arithmetic operators: / * + - %
            - relational operators: < > <= >= == !=
            - dynamic int
        - float:
            - negative numbers
            - arithmetic operator: / * + -
            - relational operator: < > <= >= == != 
            - dynamic float
        - char:
            - arithmetic operator: 
                - only for mutable + 
            - relational operator: == !=
            - set rbp - 14: 0 or 1
            - set rbp - 13: len
            - set rbp - 08: address in stack
            - fix / dyn
        - bool:
            - relational operator: == != !
            - logical operators: && || and or

    - assign: += -= *= /=

- memory management:
    - heap allocation
    - stack allocation
    - check allocation failure
    - garbage collector
    - normal mode
        - free function
    - garbage collection mode
        - add bytes at left of pointer undecates how many variable is holding it
    - protect mode:
        - check all []

- conditions and loops:
    - continue keyword
    - break keyword

- typedefs: use int as num
- add _ before globals also

- function declaration:
    - syntax:
        func data_type func_name(params ...):
            // code here ...
    - if variable sent by refrence allocate there space in function code then push rbp to the stack
    - starts only with letter
    - take params by refre or value
    - variadic arguments
    - return keyword
    - add '_' before all declared functions:
    - built in functions:
        + output
        - is_dynamic
        - is_fix
        - dyn: allocate in heap
    - function call
        + before calling function push rbp to the stack and pop it later
        + each function has its own rbp
    - polymorphism

- optimize the code
- verify dynamic and fix data type for all operations
