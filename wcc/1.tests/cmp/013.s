.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main
sayHi_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare y */
   sub     rsp, 200
   mov     QWORD PTR -8[rbp], 7 /* assign y */
   mov     rax, QWORD PTR -8[rbp]
   cmp     rax, 7
   sete    al
if2:                          /* if statement */
   cmp     al, 1
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
end_sayHi_:

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 300
   mov     QWORD PTR -16[rbp], 0 /* declare x */
   mov     QWORD PTR -16[rbp], 10 /* assign x */
   call    sayHi_
   leave
   ret
end_main:

STR1: .string "is seven\n"
STR2: .string "is not seven\n"
.section	.note.GNU-stack,"",@progbits
