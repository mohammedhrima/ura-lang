.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

m:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  QWORD PTR -8[rbp], rdi ;// assign [a] is_ref
    mov  rax, QWORD PTR -8[rbp]
    mov  DWORD PTR [rax], 1 ;// assign [a] is_ref
    mov  eax, 1
    leave 
    ret  
.endm:

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  DWORD PTR -4[rbp], 2 ;// assign [x]
    lea  rdi, -4[rbp] ;// assign [a] is_ref
    call m
    mov  edi, DWORD PTR -4[rbp] ;// assign [n]
    call putnbr
    mov  eax, 0
    leave 
    ret  
.endmain:
.section	.note.GNU-stack,"",@progbits

