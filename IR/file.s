.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main
sayHi:
   lea     rdi, .STR1[rip]
   call    _putstr
   ret
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 24
   /*declare a*/
   mov     QWORD PTR -8[rbp], 0
   /*assign a*/
   mov     QWORD PTR -8[rbp], 10
   /*declare b*/
   mov     QWORD PTR -16[rbp], 0
   /*assign b*/
   mov     QWORD PTR -16[rbp], 15
   call    sayHi
   leave
   ret
.STR1: .string "hi \n"
.section	.note.GNU-stack,"",@progbits

