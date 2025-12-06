+ declare simple main
+ declare function
+ function takes param
+ function pass param to another function

+ add to_be_implemented function: 
    + that add a label to llvm to shw what should be implimented later

+ skip br in if: in case last instruction was ret:
    + example:
        if:
            ret i32 11
            br label %end_if
+ allocata function paramater only if it got modified inside the function