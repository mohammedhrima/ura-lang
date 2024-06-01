.intel_syntax noprefix
.include "/wolf-c/import/header.s"

.text
.globl	main
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare str1 */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _strdup
   mov     QWORD PTR -8[rbp], rax /* assign  str1 */
   mov     QWORD PTR -16[rbp], 0 /* declare str2 */
   lea     rax, STR2[rip]
   mov     rdi, rax
   call    _strdup
   mov     QWORD PTR -16[rbp], rax /* assign  str2 */
   mov     BYTE PTR -17[rbp], 0 /* declare b */
   mov     rsi, QWORD PTR -8[rbp]
   mov     rdi, QWORD PTR -16[rbp]
   call	  _strcmp
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
end_main:

STR1: .string "abd"
STR2: .string "abcd"
STR3: .string "\n"
.section	.note.GNU-stack,"",@progbits
