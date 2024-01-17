.intel_syntax noprefix
.include "import/header.s"

.text
.globl	main
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   sub     rsp, 200
   lea     rax, STR1[rip]
   mov     QWORD PTR -8[rbp], rax /* assign  str1 */
   lea     rax, STR2[rip]
   mov     QWORD PTR -16[rbp], rax /* assign  str2 */
   lea     rax, STR3[rip]
   mov     QWORD PTR -24[rbp], rax /* assign  str3 */
   mov     rdi, QWORD PTR -16[rbp]
   mov     rsi, QWORD PTR -24[rbp]
   call	  _strjoin
   mov     QWORD PTR -40[rbp], rax
   mov     rdi, QWORD PTR -8[rbp]
   mov     rsi, QWORD PTR -40[rbp]
   call	  _strjoin
   mov     QWORD PTR -48[rbp], rax
   mov     rax, QWORD PTR -48[rbp]
   mov     QWORD PTR -32[rbp], rax /* assign  str4 */
   /* call _putstr */
   mov     rax, QWORD PTR -32[rbp]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_main:

STR1: .string "abcde"
STR2: .string "ghijkl"
STR3: .string "mno\n"
.section	.note.GNU-stack,"",@progbits
