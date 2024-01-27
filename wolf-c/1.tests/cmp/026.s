.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare a */
   mov     rax, 5 
   mov     QWORD PTR -8[rbp], rax /* assign a */
   mov     QWORD PTR -16[rbp], 0 /* declare b */
   mov     rax, 10 
   mov     QWORD PTR -16[rbp], rax /* assign b */
   mov     rax, QWORD PTR -8[rbp]
   cmp     rax, QWORD PTR -16[rbp]
   setl    al
   mov     BYTE PTR -16[rbp], al
   xor     BYTE PTR -16[rbp], 1
if2:                          /* if statement */
   cmp     BYTE PTR -16[rbp], 1
   jne     if3                /* jmp next statement */
   /* call _putstr */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr
   jmp     if1                /* jmp end statement */
if3:                          /* else statement */
   /* call _putstr */
   lea     rax, STR2[rip]
   mov     rdi, rax
   call    _putstr
if1:                          /* end statement */
   leave
   ret
end_main:

STR1: .string "Yes\n"
STR2: .string "No\n"
.section	.note.GNU-stack,"",@progbits
