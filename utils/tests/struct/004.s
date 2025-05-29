.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

m:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  DWORD PTR -4[rbp], edi ;// assign [user.a]
    mov  DWORD PTR -8[rbp], esi ;// assign [user.b]
    mov  DWORD PTR -4[rbp], 1 ;// assign [user.a]
    mov  DWORD PTR -8[rbp], 13 ;// assign [user.b]
    mov  eax, 0
    leave 
    ret  
.endm:
.section	.note.GNU-stack,"",@progbits

