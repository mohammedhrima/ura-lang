.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 30
   /* declare a */
   mov     QWORD PTR -8[rbp], 0
   /* assign to a */
   mov     QWORD PTR -8[rbp], 10
   /* declare b */
   mov     QWORD PTR -16[rbp], 0
   /* assign to b */
   mov     QWORD PTR -16[rbp], 20
   /* declare c */
   mov     BYTE PTR -17[rbp], 0
   mov     rax, QWORD PTR -8[rbp]
   cmp     rax, QWORD PTR -16[rbp]
   setl   al
   mov     BYTE PTR -18[rbp], al
   /* assign to c */
   movzx   eax, BYTE PTR -18[rbp]
   mov     BYTE PTR -17[rbp], al
   /* call _putbool */
   movzx   eax, BYTE PTR -17[rbp]
   mov	  edi, eax
   call	  _putbool
   /* call _putstr */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr

   leave
   ret

STR1: .string "\n"
.section	.note.GNU-stack,"",@progbits
