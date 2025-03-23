.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

m:
    push rbp
    mov  rbp, rsp
    sub  rsp, 0
    mov  rax, rdi
    mov  DWORD PTR [rax], 1 ;// assign [a] isref
    mov  eax, 1
    leave 
    ret  
.endm:
main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  DWORD PTR -4[rbp], 2 ;// assign [x]
    lea  rax, -4[rbp]
    mov  rdi, rax ;// assign [edi] isref
    call m
    mov  edi, DWORD PTR -4[rbp] ;// assign [edi]
    call putnbr
    mov  eax, 0
    leave 
    ret  
.endmain:.section	.note.GNU-stack,"",@progbits

