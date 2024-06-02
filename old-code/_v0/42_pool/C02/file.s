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
   /* str1[] (char) */
   mov     rbx, QWORD PTR -8[rbp]
   add     rbx, 20
   mov     QWORD PTR -16[rbp], rbx
   mov     al, 49    /* '1' */
   mov     rbx,  QWORD PTR -16[rbp]
   mov     BYTE PTR [rbx], al /* assign ref (null) */
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

STR1: .string "abcdef"
STR2: .string "\n"
.section	.note.GNU-stack,"",@progbits
