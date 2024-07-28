.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main
sayHi:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 24
   /*declare b*/
   mov     QWORD PTR  8[rbp], 0
   lea     rdi, .STR1[rip]
   call    _putstr
   leave
   ret
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 24
   /*declare a*/
   mov     QWORD PTR  16[rbp], 0
   /*assign a*/
   mov     QWORD PTR  16[rbp], 10
   leave
   ret
.STR1: .string "hi\n"
.section	.note.GNU-stack,"",@progbits

