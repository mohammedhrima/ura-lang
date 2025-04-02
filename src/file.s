.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 0
    mov  DWORD PTR -8[rbp], 0 ;// declare [age]
    mov  DWORD PTR -16[rbp], 0 ;// declare [age]
    mov  eax, 0
    leave 
    ret  
.endmain:.section	.note.GNU-stack,"",@progbits

