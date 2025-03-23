.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  BYTE PTR -1[rbp], 0 ;// declare [x.a]
    mov  BYTE PTR -2[rbp], 0 ;// declare [x.b]
    mov  DWORD PTR -4[rbp], 0 ;// declare [x.c]
    mov  eax, 0
    leave 
    ret  
.endmain:.section	.note.GNU-stack,"",@progbits

