.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  DWORD PTR -4[rbp], 2 ;// assign [a]
    lea  rax, -4[rbp]
    mov  QWORD PTR -12[rbp], rax ;// assign [b] is_ref
    mov  rax, QWORD PTR -12[rbp]
    mov  DWORD PTR [rax], 1 ;// assign [b] is_ref
    mov  eax, DWORD PTR -4[rbp]
    call putnbr
    mov  eax, 0
    leave 
    ret  
.endmain:.section	.note.GNU-stack,"",@progbits

