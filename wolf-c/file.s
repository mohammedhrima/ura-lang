.intel_syntax noprefix
.include "/wolf-c/import/header.s"

.text
.globl	main
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     BYTE PTR -1[rbp], 0 /* declare s1 */
   mov     al, 108 
   mov     BYTE PTR -1[rbp], al /* assign s1 */
   mov     QWORD PTR -9[rbp], 0 /* declare s2 (ref) */
   lea     rax, -1[rbp]
   mov     QWORD PTR -9[rbp], rax
   /* call _putchar */
   mov     rax, QWORD PTR -9[rbp]
   mov     al, BYTE PTR [rax]
   mov     edi, eax
   call    _putchar
   /* call _putstr */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_main:

STR1: .string "\n"
.section	.note.GNU-stack,"",@progbits
