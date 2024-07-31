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
   mov     QWORD PTR -24[rbp], 0 /* len of arr */
   mov     QWORD PTR -16[rbp], 0 /* declare arr */
   mov     rsi, 8
   mov     rdi, 3
   call    calloc@PLT
   mov     QWORD PTR -40[rbp], 3 /*len*/
   mov     QWORD PTR -32[rbp], rax
   mov     rbx, QWORD PTR -8[rbp]
   mov     QWORD PTR 0[rax], rbx
   mov     QWORD PTR 8[rax], 2
   mov     QWORD PTR 16[rax], 3
   mov     rax, QWORD PTR -32[rbp]
   mov     QWORD PTR -16[rbp], rax /* assign  arr */
   mov     rax, QWORD PTR -40[rbp] /*array len*/
   mov     QWORD PTR -24[rbp], rax /*array len*/
   /* from array arr[] (int) */
   mov     rax, QWORD PTR -16[rbp]
   add     rax, 0
   mov     QWORD PTR -48[rbp], rax
   /* call _putnbr */
   mov     rax, QWORD PTR -48[rbp]
   mov     rax, QWORD PTR [rax]
   mov     rdi, rax
   call    _putnbr
   /* call _putstr */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_main:

STR1: .string "\n"
.section	.note.GNU-stack,"",@progbits
