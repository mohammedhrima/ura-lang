.intel_syntax noprefix
.include "/wolf-c/import/header.s"

.text
.globl	main
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare x */
   mov     rax, 10 
   mov     QWORD PTR -8[rbp], rax /* assign x */
   mov     QWORD PTR -16[rbp], 0 /* declare y */
   mov     rax, 7 
   mov     QWORD PTR -16[rbp], rax /* assign y */
   mov     rax, QWORD PTR -8[rbp]
   cmp     rax, 10
   sete    al
   /* || operation 0 */
   cmp     al, 1
   je      if3
   mov     rax, QWORD PTR -16[rbp]
   cmp     rax, 5
   sete    al
if3:
   /* && operation 0 */
   cmp     al, 1
   jne     if4
   mov     rax, QWORD PTR -8[rbp]
   cmp     rax, 5
   setg    al
if4:
if2: /* if statement */
   cmp     al, 1
   jne     if5    /* jmp next statement */
   /* call _putstr */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr
   jmp     if1    /* jmp end statement */
if5: /* else statement */
   /* call _putstr */
   lea     rax, STR2[rip]
   mov     rdi, rax
   call    _putstr
if1: /* end statement */
   leave
   ret
end_main:

STR1: .string "condition is true\n"
STR2: .string "condition is false\n"
.section	.note.GNU-stack,"",@progbits
