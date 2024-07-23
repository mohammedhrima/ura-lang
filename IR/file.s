.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main
sayHi:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 8
   /*arg c in 24[rbp] */
   /*arg b in 32[rbp] */
   /*arg a in 40[rbp] */
   mov     rdi, QWORD PTR  16[rbp]
   call    _putnbr
   leave
   ret
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 8
   call    sayHi
   leave
   ret
.section	.note.GNU-stack,"",@progbits

