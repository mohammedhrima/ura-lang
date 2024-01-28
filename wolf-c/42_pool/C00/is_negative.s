.intel_syntax noprefix
.include "/wolf-c/import/header.s"

.text
.globl	main
isNegative_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     QWORD PTR -8[rbp], rax
   mov     QWORD PTR -16[rbp], 0 /* declare n */
   mov	  rax, QWORD PTR 16[rbp]
   mov     QWORD PTR -16[rbp], rax
   mov     rax, QWORD PTR -16[rbp]
   cmp     rax, 0
   setl    al
   mov     al, al
   mov     rbx, QWORD PTR -8[rbp]
   mov     BYTE PTR [rbx], al
   leave
   ret
end_isNegative_:

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     rax, -10
   push    rax
   mov     QWORD PTR -8[rbp], 0 /*isNegative result*/
   lea     rax, -8[rbp]
   call    isNegative_
   /* call _putbool */
   movzx   eax, BYTE PTR -8[rbp]
   mov	   edi, eax
   call	   _putbool
   /* call _putstr */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr
   mov     rax, 10
   push    rax
   mov     QWORD PTR -16[rbp], 0 /*isNegative result*/
   lea     rax, -16[rbp]
   call    isNegative_
   /* call _putbool */
   movzx   eax, BYTE PTR -16[rbp]
   mov	   edi, eax
   call	   _putbool
   /* call _putstr */
   lea     rax, STR2[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_main:

STR1: .string "\n"
STR2: .string "\n"
.section	.note.GNU-stack,"",@progbits
