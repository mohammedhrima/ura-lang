.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  DWORD PTR -4[rbp], 3 ;// assign [a]
    mov  eax, DWORD PTR -4[rbp]
    add  eax, 2
    mov  DWORD PTR -8[rbp], eax ;// assign [b]
    mov  edi, DWORD PTR -8[rbp] ;// assign [n]
    call putnbr
    mov  eax, 0
    leave 
    ret  
.endmain:
.section	.note.GNU-stack,"",@progbits

