.section	.note.GNU-stack,"",@progbits
.intel_syntax noprefix
.text
.globl	main

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 30
   mov     QWORD PTR -4[rbp], 15
   mov     QWORD PTR -12[rbp], QWORD PTR -4[rbp]
   add     QWORD PTR -12[rbp], 8
   mov     QWORD PTR -8[rbp], QWORD PTR -12[rbp]
   mov     rax, 0
   leave
   ret

