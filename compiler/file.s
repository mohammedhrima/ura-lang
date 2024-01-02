.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 30
   /* declare c */
   mov     BYTE PTR -1[rbp], 0
   /* assign to c */
   mov     BYTE PTR -1[rbp], 0   /* call output */
   movzx   eax, BYTE PTR -1[rbp]
   mov	  edi, eax
   call	  _putbool
   /* call output */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr

   leave
   ret

STR1: .string "\n"
.section	.note.GNU-stack,"",@progbits
