.intel_syntax noprefix
.include "/import/header.s"

.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 0
    mov  edi, 123 ;// assign [edi]
    call putnbr
    mov  eax, 0
    leave 
    ret  
.endmain:.section	.note.GNU-stack,"",@progbits

