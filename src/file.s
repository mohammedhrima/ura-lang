.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 48
    mov  DWORD PTR -4[rbp], 0 ;// declare [d]
    mov  DWORD PTR -4[rbp], 0 ;// declare [x.a]
    mov  DWORD PTR -8[rbp], 0 ;// declare [x.b]
    mov  DWORD PTR -12[rbp], 0 ;// declare [x.c]
    mov  DWORD PTR -4[rbp], 0 ;// declare [y.a1]
    mov  DWORD PTR -8[rbp], 0 ;// declare [y.b1]
    mov  DWORD PTR -12[rbp], 0 ;// declare [y.c1]
    mov  DWORD PTR -4[rbp], 1 ;// assign [x.a]
    mov  DWORD PTR -8[rbp], 2 ;// assign [y.b1]
    mov  eax, 0
    leave 
    ret  
.endmain:.section	.note.GNU-stack,"",@progbits

