.intel_syntax noprefix
.text
.globl	main
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     rax, QWORD PTR -8[rbp]
   add     rax, 10
   /*assign a*/
   mov     QWORD PTR -8[rbp], rax
   leave
   ret
.section	.note.GNU-stack,"",@progbits

