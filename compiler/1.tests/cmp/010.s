.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 30
   mov     QWORD PTR -8[rbp], 0 /* declare x */
   mov     QWORD PTR -8[rbp], 10 /* assign x */
   mov     QWORD PTR -16[rbp], 0 /* declare y */
   mov     QWORD PTR -16[rbp], 7 /* assign y */
   mov     rax, QWORD PTR -8[rbp]
   cmp     rax, 10
   sete    al
   /* || operation 0 */
   cmp     al, 1
   je      main3
   mov     rax, QWORD PTR -16[rbp]
   cmp     rax, 5
   sete    al
main3:
   /* && operation 0 */
   cmp     al, 1
   jne     main4
   mov     rax, QWORD PTR -8[rbp]
   cmp     rax, 5
   setg    al
main4:
main2:                          /* if statement */
   cmp     al, 1
   jne     main5                /* jmp next statement */
   /* call _putstr */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr
   jmp     main1                /* jmp end statement */
main5:                          /* else statement */
   /* call _putstr */
   lea     rax, STR2[rip]
   mov     rdi, rax
   call    _putstr
main1:                          /* end statement */
   leave
   ret

STR1: .string "condition is true\n"
STR2: .string "condition is false\n"
.section	.note.GNU-stack,"",@progbits
