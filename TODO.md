+ references
+ reference as function parameter
+ assign variable from refrence
+ assign reference from variable
+ math operation using references
+ ....




if left is ref and right is ref:
   mov rax, left_ptr
   mov rbx, right_ptr
   mov rbx, QWORD PTR [rbx]
   mov QWORD PTR [rax], rbx

elif left is ref and right is ptr:
   mov rax, left_ptr
   mov QWORD PTR [rax], right_ptr


elif left is ref and right is creg:
   mov rax, left_ptr
   mov QWORD PTR [rax], right_creg

elif left is ref and right is not ptr:
   mov rax, left_ptr
   mov QWORD PTR [rax], right_value


elif left is ptr and right is ref:
   mov rax, right_ptr
   mov rax, QWORD PTR [rax]
   mov left_ptr, rax

elif left is ptr and right is ptr:
   mov rax, right_ptr
   mov left_ptr, rax

elif left is ptr and right is creg:
   mov left_ptr, rax

elif left is ptr and right is not ptr:
   mov left_ptr, right_value


