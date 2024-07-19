.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 500
   /*declare str*/
   mov     QWORD PTR -8[rbp], 0
   /*assign str*/
   lea     rdi, [rip + .STR1]
   call    strdup@PLT
   leave
   ret

.STR1: .string "ab"
.section	.note.GNU-stack,"",@progbits

