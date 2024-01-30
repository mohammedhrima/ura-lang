.intel_syntax noprefix
.include "/wolf-c/import/header.s"

.text
.globl	main
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     rax, 5
   mov     rsi, 8
   mov     rdi, rax
   call    calloc@PLT
   mov     QWORD PTR -8[rbp], rax
   mov     QWORD PTR [rax], 1
   add     rax, 8
   mov     QWORD PTR [rax], 2
   add     rax, 8
   mov     QWORD PTR [rax], 3
   add     rax, 8
   mov     QWORD PTR [rax], 4
   add     rax, 8
   mov     QWORD PTR [rax], 5
   add     rax, 8
   leave
   ret
end_main:

.section	.note.GNU-stack,"",@progbits
