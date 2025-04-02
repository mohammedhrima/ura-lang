.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  DWORD PTR -4[rbp], 123 ;// assign [user.age]
    mov  edi, DWORD PTR -4[rbp] ;// assign [edi]
    call putnbr
    mov  eax, 0
    leave 
    ret  
.endmain:.section	.note.GNU-stack,"",@progbits

