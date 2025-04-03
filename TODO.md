+ struct inside struct
+ struct as function parameter
+ reference to struct

+ try this approach:
    + treat struct as an array of variable like this

struct User:
    chars *name
    int age

struct Card:
    User user
    int id

card.user.name  is chars
card.user.age   is int
card.id         is int


+ variables:
    - declaration 
        + int
        - float
        + chars
        - short
        - long
        - struct
    - assignemt
        + int
        - float
        + chars
        - short
        - long
        - struct

+ access child attributes
+ function call
+ function with parameters
+ if stataments
+ while loop 