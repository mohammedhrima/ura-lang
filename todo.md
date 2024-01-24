- data Types:
    - array:
        + data_type[level] :
        - logic operator: == !=
        - arithmetic operator: +
        - set rbp - 14: 0 or 1 (mu / imut)
        - set rbp - 13: len
        - set rbp - 08: address in stack
    - single:
        - int:
            - negative numbers
            - dynamic int
        - float:
            - negative numbers
            - arithmetic operator: / * + -
            - dynamic float
        - char:
            - arithmetic operator: 
                - only for mutable + 
            - fix / dyn

- memory management:
    - heap allocation
    - stack allocation
    - check allocation failure
    - garbage collector:
        - use threads
    - normal mode
        - free function
    - garbage collection mode
        - add bytes at left of pointer undecates 
          how many variable is holding it
    - protect mode:
        - check all []

- try, catch
- typedefs: use int as num
- add _ before globals also

- reference syntax:
    func int f(ref int v):
        // code here ...
    
    int x = 5
    ref int y = x
    


- function declaration:
    - if variable sent by refrence allocate there space in function code then push rbp to the stack
    - take params by refre or value:
        - set variable address same as orgine address
    - variadic arguments
    - return keyword
    - built in functions:
        - is_dynamic
        - is_fix
        - dyn: allocate in heap
    - polymorphism

- freeing refrence memory may caus problem
- verify dynamic and fix data type for all operations
- file system:
    - import files
    - open / close files

- print all parsing error before exiting