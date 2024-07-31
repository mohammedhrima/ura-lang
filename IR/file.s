.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main
sayHi:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 16
   mov     QWORD PTR -8[rbp], rdi
   mov     rdi, QWORD PTR -8[rbp]
   call    .putnbr
   leave
   ret
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 16
   mov     rdi, 1
   mov     rsi, 2
   mov     rdx, 3
   mov     rcx, 4
   push    5
   push    6
   push    7
   push    8
   call    sayHi
   leave
   ret
.section	.note.GNU-stack,"",@progbits

