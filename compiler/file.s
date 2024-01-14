.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 30
   mov     QWORD PTR -8[rbp], 0 /* declare arr */
   mov     rax, 11 
   mov     QWORD PTR -20[rbp], rax 
   mov     rax, 22 
   mov     QWORD PTR -24[rbp], rax 
   sub     rsp, 60
   mov     rax, 33 
   mov     QWORD PTR -28[rbp], rax 

   lea     rax, -20[rbp]    /* addr of 1st elem */
   mov     QWORD PTR -8[rbp], rax
   mov     rax, 2
   mov     rax, QWORD PTR -8[rbp+rax*8] /*int*/
   leave
   ret
end_main:

.section	.note.GNU-stack,"",@progbits
