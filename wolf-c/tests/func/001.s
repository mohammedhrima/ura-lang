.intel_syntax noprefix
.include "/wolf-c/import/header.s"

.text
.globl	main
sayHi_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov	  QWORD PTR -8[rbp], rax
   mov     QWORD PTR -16[rbp], 0 /* declare y */
   mov     rax, 7 
   mov     QWORD PTR -16[rbp], rax /* assign y */
   mov     rax, QWORD PTR -16[rbp]
   mov     rbx, 7
   cmp     rax, rbx
   sete    al
if2: /* if statement */
   cmp     al, 1
   jne     if3    /* jmp next statement */
   /* call _putstr */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr
   jmp     if1    /* jmp end statement */
if3: /* else statement */
   /* call _putstr */
   lea     rax, STR2[rip]
   mov     rdi, rax
   call    _putstr
if1: /* end statement */
   leave
   ret
end_sayHi_:

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare x */
   mov     rax, 10 
   mov     QWORD PTR -8[rbp], rax /* assign x */
