.intel_syntax noprefix
.include "/wolf-c/import/header.s"

.text
.globl	main
putchar_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     BYTE PTR -1[rbp], 0 /* declare c */
   mov	  rax, QWORD PTR 16[rbp]
   mov     BYTE PTR -1[rbp], al
   /* call _putchar */
   mov     al, BYTE PTR -1[rbp]
   mov     edi, eax
   call    _putchar
   leave
   ret
end_putchar_:

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     al, 104
   push    rax
   call    putchar_
   leave
   ret
end_main:

.section	.note.GNU-stack,"",@progbits
