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
   /* declare str3 */
   sub     rsp, 30
   /* assign to str3 */
   lea     rax, STR3[rip]
   mov     QWORD PTR -24[rbp], rax
   /* declare str4 */
   mov     rdi, QWORD PTR -16[rbp]
   mov     rsi, QWORD PTR -24[rbp]
   call	  _strjoin
   mov     QWORD PTR -40[rbp], rax
   mov     rdi, QWORD PTR -8[rbp]
   mov     rsi, QWORD PTR -40[rbp]
   call	  _strjoin
   mov     QWORD PTR -48[rbp], rax
   /* assign to str4 */
   mov     rax, QWORD PTR -48[rbp]
   mov     QWORD PTR -32[rbp], rax
   /* call _putstr */
   mov     rax, QWORD PTR -32[rbp]
   mov     rdi, rax
   call    _putstr

   leave
   ret

STR1: .string "abcde"
STR2: .string "ghijkl"
STR3: .string "mno\n"
.section	.note.GNU-stack,"",@progbits
