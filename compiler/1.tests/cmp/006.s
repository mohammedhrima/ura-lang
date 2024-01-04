.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 30
   /* declare str1 */
   /* assign to str1 */
   lea     rax, STR1[rip]
   mov     QWORD PTR -8[rbp], rax
   /* declare str2 */
   /* assign to str2 */
   lea     rax, STR2[rip]
   mov     QWORD PTR -16[rbp], rax
   /* declare b */
   mov     BYTE PTR -17[rbp], 0
   mov     rsi, QWORD PTR -8[rbp]
   mov     rdi, QWORD PTR -16[rbp]
   call	  _strcmp
   mov     BYTE PTR -18[rbp], al
   /* assign to b */
   movzx   eax, BYTE PTR -18[rbp]
   mov     BYTE PTR -17[rbp], al
   /* call _putbool */
   movzx   eax, BYTE PTR -17[rbp]
   mov	  edi, eax
   call	  _putbool
   /* call _putstr */
   lea     rax, STR3[rip]
   mov     rdi, rax
   call    _putstr

   leave
   ret

STR1: .string "abd"
STR2: .string "abcd"
STR3: .string "\n"
.section	.note.GNU-stack,"",@progbits
