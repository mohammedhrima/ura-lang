.intel_syntax noprefix
.include "../../import/header.s"

.text
   .globl	main

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 16

   mov     QWORD PTR -8[rbp],  11
   mov     QWORD PTR -16[rbp], 12
   mov     QWORD PTR -24[rbp], 13

   lea     rax, -8[rbp]             /* load address in stack*/
   mov     QWORD PTR -32[rbp], rax  

   mov     rax, QWORD PTR -32[rbp]
   mov     rax, [rax]
   mov     rdi, rax
   call    _putnbr

   lea     rax, STR0[rip]
   mov     rdi, rax
   call    _putstr
   mov     rax, 0
   leave
   ret

STR0: .string "\n"
.section	.note.GNU-stack,"",@progbits
