.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  DWORD PTR -4[rbp], 0 ;// declare [a]
    mov  eax, 1
    add  eax, 2
    mov  eax, eax
    add  eax, 3
    mov  DWORD PTR -4[rbp], eax ;// assign [a]
        mov  DWORD PTR -8[rbp], 0 ;// declare [b]
        mov  DWORD PTR -8[rbp], 15 ;// assign [b]
    mov  eax, 0
    leave 
    ret  
.endmain:.section	.note.GNU-stack,"",@progbits

