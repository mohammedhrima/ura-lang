.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 30
   mov     QWORD PTR -8[rbp], 0 /* declare x */
   mov     QWORD PTR -8[rbp], 15 /* assign  x */

   mov     QWORD PTR -16[rbp], 0 /* declare y */
   mov     QWORD PTR -16[rbp], 10 /* assign  y */

   mov     rax, QWORD PTR -8[rbp]
   cmp     rax, 10
   sete    al
   mov     BYTE PTR -17[rbp], al
   /* if statement*/
main1:
   cmp     BYTE PTR -17[rbp], 1
   jne     main2 /*jump next*/
   /* call _putstr */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr
   jmp     main2 
   mov     rax, QWORD PTR -8[rbp]
   cmp     rax, 15
   sete    al
   mov     BYTE PTR -18[rbp], al
   /* elif statement*/
main2:
   cmp     BYTE PTR -18[rbp], 1
   jne     main3 /*jump next*/
   /* call _putstr */
   lea     rax, STR2[rip]
   mov     rdi, rax
   call    _putstr
   mov     rax, QWORD PTR -8[rbp]
   cmp     rax, 20
   sete    al
   mov     BYTE PTR -19[rbp], al
   /* elif statement*/
main3:
   cmp     BYTE PTR -19[rbp], 1
   jne     main4 /*jump next*/
   /* call _putstr */
   lea     rax, STR3[rip]
   mov     rdi, rax
   call    _putstr
   mov     rax, QWORD PTR -8[rbp]
   cmp     rax, 25
   sete    al
   mov     BYTE PTR -20[rbp], al
   /* elif statement*/
main4:
   cmp     BYTE PTR -20[rbp], 1
   jne     main5 /*jump next*/
   /* call _putstr */
   lea     rax, STR4[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putstr */
   lea     rax, STR5[rip]
   mov     rdi, rax
   call    _putstr

   leave
   ret

STR1: .string "0 x is 10\n"
STR2: .string "1 x is 15\n"
STR3: .string "2 x is 20\n"
STR4: .string "3 x is 25\n"
STR5: .string "\n"
.section	.note.GNU-stack,"",@progbits
