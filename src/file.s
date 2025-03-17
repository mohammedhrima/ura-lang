.intel_syntax noprefix
.include "/import/header.s"

.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  DWORD PTR -4[rbp], 123 ;// assign [a]
    lea  rax, -4[rbp]
    mov  QWORD PTR -12[rbp], rax
    mov  rax, QWORD PTR -12[rbp];// get left address
    mov  DWORD PTR [rax], 20;// assign left address ;// assign [b]
    mov  edi, DWORD PTR -4[rbp] ;// assign [edi]
    call putnbr
    mov  eax, 0
    leave 
    ret  
.endmain:.section	.note.GNU-stack,"",@progbits

