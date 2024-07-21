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
   mov     rbx, 1
   cmp     rax, rbx
   sete    al
   cmp     al, 1
   jne     else3
   lea     rdi, .STR1[rip]
   call    _putstr
   mov     rax, QWORD PTR -8[rbp]
   mov     rbx, 2
   cmp     rax, rbx
   sete    al
   cmp     al, 1
   jne     endif2
   lea     rdi, .STR2[rip]
   call    _putstr
endif2:
   jmp     endif1
else3:
   lea     rdi, .STR3[rip]
   call    _putstr
endif1:
   leave
   ret
.STR1: .string "cond 1\n"
.STR2: .string "cond 12\n"
.STR3: .string "else"
.section	.note.GNU-stack,"",@progbits

