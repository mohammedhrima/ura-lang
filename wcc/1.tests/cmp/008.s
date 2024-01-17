.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare a */
   sub     rsp, 200
   mov     QWORD PTR -8[rbp], 10 /* assign a */
   mov     QWORD PTR -16[rbp], 0 /* declare b */
   mov     QWORD PTR -16[rbp], 20 /* assign b */
   mov     BYTE PTR -17[rbp], 0 /* declare c */
   mov     rax, QWORD PTR -8[rbp]
   cmp     rax, QWORD PTR -16[rbp]
   setl    al
   mov     BYTE PTR -17[rbp], al
   /* call _putbool */
   movzx   eax, BYTE PTR -17[rbp]
   mov	   edi, eax
   call	   _putbool
   /* call _putstr */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_main:

STR1: .string "\n"
.section	.note.GNU-stack,"",@progbits
