.intel_syntax noprefix
.include "/import/header.s"

.text
.globl	main

hello:
    push rbp
    mov  rbp, rsp
    sub  rsp, 0
    mov  eax, 1
    leave 
    ret  
.endhello:
main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  DWORD PTR -4[rbp], 6 ;// assign [a]
    mov  DWORD PTR -8[rbp], 15 ;// assign [b]
    mov  eax, 0
    leave 
    ret  
.endmain:.section	.note.GNU-stack,"",@progbits

