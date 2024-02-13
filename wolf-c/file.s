.intel_syntax noprefix
.include "/wolf-c/import/header.s"

.text
.globl	main
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare x */
   mov     rax, 10 
   mov     QWORD PTR -8[rbp], rax /* assign x */
   mov     QWORD PTR -16[rbp], 0 /* declare arr */
   mov     rsi, 8
   mov     rdi, 3
   call    calloc@PLT
   mov     QWORD PTR -32[rbp], rax
   mov     rbx, QWORD PTR -8[rbp]
   mov     QWORD PTR [rax], rbx
   add     rax, 8
   mov     QWORD PTR [rax], 22
   add     rax, 8
   mov     QWORD PTR [rax], 3
   add     rax, 8
   mov     rsi, 8
   mov     rdi, 3
   call    calloc@PLT
   mov     QWORD PTR -40[rbp], rax
   mov     QWORD PTR [rax], 14
   add     rax, 8
   mov     QWORD PTR [rax], 55
   add     rax, 8
   mov     QWORD PTR [rax], 66
   add     rax, 8
   mov     rsi, 8
   mov     rdi, 2
   call    calloc@PLT
   mov     QWORD PTR -24[rbp], rax
   mov     rbx, QWORD PTR -32[rbp]
   mov     QWORD PTR [rax], rbx
   add     rax, 8
   mov     rbx, QWORD PTR -40[rbp]
   mov     QWORD PTR [rax], rbx
   add     rax, 8
   mov     rax, QWORD PTR -24[rbp]
   mov     QWORD PTR -16[rbp], rax
   /* call _putstr */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr
   /* arr[] (array) */
   mov     rax, QWORD PTR -16[rbp]
   add     rax, 0
   mov     rax, [rax]
   mov     QWORD PTR -48[rbp], rax
   /* (null)[] (int) */
   mov     rax, QWORD PTR -48[rbp]
   add     rax, 0
   mov     rax, [rax]
   mov     QWORD PTR -56[rbp], rax
   /* call _putnbr */
   mov     rax, QWORD PTR -56[rbp]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR2[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_main:

STR1: .string "arr[0][0] is: "
STR2: .string "\n"
.section	.note.GNU-stack,"",@progbits
