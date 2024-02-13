.intel_syntax noprefix
.include "/wolf-c/import/header.s"

.text
.globl	main
main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     BYTE PTR -1[rbp], 0 /* declare c */
   mov     BYTE PTR -1[rbp], 0 /* assign  c */
   mov     BYTE PTR -2[rbp], 0 /* declare d */
   mov     al, BYTE PTR -1[rbp]
   xor     al, 1
   mov     BYTE PTR -2[rbp], al
   mov     al, BYTE PTR -2[rbp]
   mov     BYTE PTR -2[rbp], al /* assign  d */
   /* call _putstr */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putbool */
   movzx   eax, BYTE PTR -1[rbp]
   mov	  edi, eax
   call	  _putbool
   /* call _putstr */
   lea     rax, STR2[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putstr */
   lea     rax, STR3[rip]
   mov     rdi, rax
   call    _putstr
   /* call _putbool */
   movzx   eax, BYTE PTR -2[rbp]
   mov	  edi, eax
   call	  _putbool
   /* call _putstr */
   lea     rax, STR4[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_main:

STR1: .string "c: "
STR2: .string "\n"
STR3: .string "d: "
STR4: .string "\n"
.section	.note.GNU-stack,"",@progbits
