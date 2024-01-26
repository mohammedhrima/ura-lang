.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main
chihaja_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], rax
   mov     rax, 5
   mov     rbx, QWORD PTR -8[rbp]
   mov     QWORD PTR [rbx], rax
   leave
   ret
end_chihaja_:

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /*chihaja result*/
   lea     rax, -8[rbp]
   call    chihaja_
   /* call _putnbr */
   mov     rax, QWORD PTR -8[rbp]
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
