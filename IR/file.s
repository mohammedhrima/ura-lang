.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 16
   /*declare n*/
   mov     QWORD PTR -8[rbp], 0
   /*assign n*/
   mov     QWORD PTR -8[rbp], 10
   mov     rax, QWORD PTR -8[rbp]
   mov     rbx, 10
   cmp     rax, rbx
   sete    al
   cmp     al, 1
   jne     endif1
   lea     rdi, .STR1[rip]
   call    _putstr
endif1:
   leave
   ret
.STR1: .string "is ten\n"
.STR0: .string "is ten\n"
.section	.note.GNU-stack,"",@progbits

