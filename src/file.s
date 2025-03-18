.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

m:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  rax, edi
    mov  edi, DWORD PTR [rax]
    mov  DWORD PTR -4[rbp], edi ;// assign [b]
    mov  edi, edi
    leave 
    ret  
.endm:
main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  DWORD PTR -4[rbp], 2 ;// assign [x]
    mov  edi, 3 ;// assign [edi]
    call m
    mov  eax, 0
    leave 
    ret  
.endmain:.section	.note.GNU-stack,"",@progbits

