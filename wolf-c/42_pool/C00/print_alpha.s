.intel_syntax noprefix
.include "/wolf-c/import/header.s"

.text
.globl	main
printAlpha_:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   mov     BYTE PTR -1[rbp], 0 /* declare c */
   mov     al, 97 
   mov     BYTE PTR -1[rbp], al /* assign c */
   jmp     while1 /* jmp to while condition*/
while2: /* while bloc*/
   /* call _putchar */
   mov     al, BYTE PTR -1[rbp]
   mov     edi, eax
   call    _putchar
   /* call _putstr */
   lea     rax, STR1[rip]
   mov     rdi, rax
   call    _putstr
   mov     al, BYTE PTR -1[rbp]
   add     al, 1
   mov     BYTE PTR -2[rbp], al
   mov     al, BYTE PTR -2[rbp]
   mov     BYTE PTR -1[rbp], al /* assign c */
while1: /* while condition */
   mov     al, BYTE PTR -1[rbp]
   cmp     al, 122
   setle   al
   cmp     al, 1
   je      while2 /* je to while bloc*/
while3: /* end while */
   /* call _putstr */
   lea     rax, STR2[rip]
   mov     rdi, rax
   call    _putstr
   leave
   ret
end_printAlpha_:

main:
   push    rbp
   mov     rbp, rsp
   sub     rsp, 100
   call    printAlpha_
   leave
   ret
end_main:

STR1: .string " "
STR2: .string "\n"
.section	.note.GNU-stack,"",@progbits
