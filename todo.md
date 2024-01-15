- data Types:
    - array:
        + data_type[level] :
        - logic operator: == !=
        - math operators ...
        - arithmetic operator: +
        - set rbp - 14: 0 or 1 (mu / imut)
        - set rbp - 13: len
        - set rbp - 08: address in stack
    - single:
        - int:
            - negative numbers
            - arithmetic operators: / * + - %
            - dynamic int
        - float:
            - negative numbers
            - arithmetic operator: / * + -
            - dynamic float
        - char:
            - arithmetic operator: 
                - only for mutable + 
            - relational operator: == !=
            - set rbp - 14: 0 or 1
            - set rbp - 13: len
            - set rbp - 08: address in stack
            - fix / dyn

- memory management:
    - heap allocation
    - stack allocation
    - check allocation failure
    - garbage collector
    - normal mode
        - free function
    - garbage collection mode
        - add bytes at left of pointer undecates 
          how many variable is holding it
    - protect mode:
        - check all []

- conditions and loops:
    - continue keyword
    - break keyword

- typedefs: use int as num
- add _ before globals also

- function declaration:
    - if variable sent by refrence allocate there  
      space in function code then push rbp to the stack
    - take params by refre or value
    - variadic arguments
    - return keyword
    - built in functions:
        - is_dynamic
        - is_fix
        - dyn: allocate in heap
    - polymorphism

- verify dynamic and fix data type for all operations
- file system:
    - import files
    - open / close files

- build my own version of docker to run in it
- print all parsing error before exiting
