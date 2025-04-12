.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

m:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  DWORD PTR -4[rbp], edi ;// assign [a]
    mov  DWORD PTR -8[rbp], esi ;// assign [b]
    mov  DWORD PTR -12[rbp], 0 ;// assign [user.b]
    mov  DWORD PTR -16[rbp], 123 ;// assign [user.a]
    mov  eax, 0
    leave 
    ret  
.endm:

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  DWORD PTR -8[rbp], 0 ;// assign [user1.a]
    mov  DWORD PTR -4[rbp], 0 ;// assign [user1.b]
    mov  eax, 0
    leave 
    ret  
.endmain:
.section	.note.GNU-stack,"",@progbits

