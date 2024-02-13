.intel_syntax noprefix
.include "/wolf-c/import/header.s"

.text
.globl	main
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -16[rbp], 0 /* len of arr */
   mov     QWORD PTR -8[rbp], 0 /* declare arr */
   mov     rsi, 8
   mov     rdi, 5
   call    calloc@PLT
   mov     QWORD PTR -32[rbp], 5 /*len*/
   mov     QWORD PTR -24[rbp], rax
   mov     QWORD PTR 0[rax], 1
   mov     QWORD PTR 8[rax], 2
   mov     QWORD PTR 16[rax], 3
   mov     QWORD PTR 24[rax], 4
   mov     QWORD PTR 32[rax], 5
   mov     rax, QWORD PTR -24[rbp]
   mov     QWORD PTR -8[rbp], rax /* assign  arr */
   mov     rax, QWORD PTR -32[rbp] /*array len*/
   mov     QWORD PTR -16[rbp], rax /*array len*/
   /* from array arr[] (int) */
   mov     rax, QWORD PTR -8[rbp]
   add     rax, 16
   mov     QWORD PTR -40[rbp], rax
   /* call _putnbr */
   mov     rax, QWORD PTR -40[rbp]
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
