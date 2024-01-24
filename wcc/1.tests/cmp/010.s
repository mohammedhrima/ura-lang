.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare x */
   mov     QWORD PTR -8[rbp], 10 /* assign x */
   mov     QWORD PTR -16[rbp], 0 /* declare y */
   mov     QWORD PTR -16[rbp], 7 /* assign y */
   mov     QWORD PTR -24[rbp], 0 /* declare z */
   mov     QWORD PTR -24[rbp], 8 /* assign z */
   mov     rax, QWORD PTR -8[rbp]
   cmp     rax, 15
   sete    al
if2:                          /* if statement */
   cmp     al, 1
   jne     if3                /* jmp next statement */
   /* call _putstr */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr
   jmp     if1                /* jmp end statement */
if3:                          /* elif statement */
   mov     rax, QWORD PTR -8[rbp]
   cmp     rax, 10
   sete    al
   cmp     al, 1
   jne     if10                /* jmp next statement */
   /* call _putstr */
   lea     rax, STR2[rip]
   mov     rdi, rax
   call    _putstr
   mov     rax, QWORD PTR -16[rbp]
   cmp     rax, 7
   sete    al
if5:                          /* if statement */
   cmp     al, 1
   jne     if9                /* jmp next statement */
   /* call _putstr */
   lea     rax, STR3[rip]
   mov     rdi, rax
   call    _putstr
   mov     rax, QWORD PTR -24[rbp]
   cmp     rax, 10
   setl    al
if7:                          /* if statement */
   cmp     al, 1
   jne     if8                /* jmp next statement */
   /* call _putstr */
   lea     rax, STR4[rip]
   mov     rdi, rax
   call    _putstr
   jmp     if6                /* jmp end statement */
if8:                          /* else statement */
   /* call _putstr */
   lea     rax, STR5[rip]
   mov     rdi, rax
   call    _putstr
if6:                          /* end statement */
   jmp     if4                /* jmp end statement */
if9:                          /* else statement */
   /* call _putstr */
   lea     rax, STR6[rip]
   mov     rdi, rax
   call    _putstr
if4:                          /* end statement */
   jmp     if1                /* jmp end statement */
if10:                          /* elif statement */
   mov     rax, QWORD PTR -8[rbp]
   cmp     rax, 10
   sete    al
   cmp     al, 1
   jne     if11                /* jmp next statement */
   /* call _putstr */
   lea     rax, STR7[rip]
   mov     rdi, rax
   call    _putstr
   jmp     if1                /* jmp end statement */
if11:                          /* elif statement */
   mov     rax, QWORD PTR -8[rbp]
   cmp     rax, 10
   sete    al
   cmp     al, 1
   jne     if12                /* jmp next statement */
   /* call _putstr */
   lea     rax, STR8[rip]
   mov     rdi, rax
   call    _putstr
   jmp     if1                /* jmp end statement */
if12:                          /* elif statement */
   mov     rax, QWORD PTR -8[rbp]
   cmp     rax, 10
   sete    al
   cmp     al, 1
   jne     if13                /* jmp next statement */
   /* call _putstr */
   lea     rax, STR9[rip]
   mov     rdi, rax
   call    _putstr
   jmp     if1                /* jmp end statement */
if13:                          /* elif statement */
   mov     rax, QWORD PTR -8[rbp]
   cmp     rax, 1
   sete    al
   cmp     al, 1
   jne     if14                /* jmp next statement */
   /* call _putstr */
   lea     rax, STR10[rip]
   mov     rdi, rax
   call    _putstr
   jmp     if1                /* jmp end statement */
if14:                          /* else statement */
   /* call _putstr */
   lea     rax, STR11[rip]
   mov     rdi, rax
   call    _putstr
if1:                          /* end statement */
   leave
   ret
end_main:

STR1: .string "if 0\n"
STR2: .string "elif 1\n"
STR3: .string "1. if 0\n"
STR4: .string "2. if 0\n"
STR5: .string "3. else"
STR6: .string "1. else\n"
STR7: .string "elif 2\n"
STR8: .string "elif 3\n"
STR9: .string "elif 4\n"
STR10: .string "elif 5\n"
STR11: .string "else\n"
.section	.note.GNU-stack,"",@progbits
