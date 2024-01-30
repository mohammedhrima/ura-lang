.intel_syntax noprefix
.include "/wolf-c/import/header.s"

.text
.globl	main
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], 0 /* declare str1 */
   mov     BYTE PTR -9[rbp], 0
   mov     BYTE PTR -10[rbp], 100
   mov     BYTE PTR -11[rbp], 99
   mov     BYTE PTR -12[rbp], 98
   mov     BYTE PTR -13[rbp], 97
   lea     rax, QWORD PTR -13[rbp]
   mov     QWORD PTR -8[rbp], rax /* assign  str1 */
   /* call _putstr */
   mov     rax, QWORD PTR -8[rbp]
   mov     rdi, rax
   call    _putstr
   /* call _putstr */
   lea     rax, STR2[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_main:

STR1: .string "abcd"
STR2: .string "\n"
.section	.note.GNU-stack,"",@progbits
