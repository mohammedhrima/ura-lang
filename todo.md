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

- inditions and loops:
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

- typedefs: use int as num
- add _ before globals also

- function declaration:
    - syntax:
        data_type func func_name(params ...):
            // code here ...
    - if variable sent by refrence allocate there space in function code then push rbp to the stack
    - starts only with letter
    - take params by refre or value
    - variadic arguments
    - return keyword
    - add '_' before all declared functions:
    - built in functions:
        - output
        - mutable
    - function call
        - before calling function push rbp to the stack and pop it later
        - each function has its own rbp
    - polymorphism

- optimize the code
- verify dynamic and fix data type for all operations
- break continue