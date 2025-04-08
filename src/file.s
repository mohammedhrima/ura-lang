.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 48
    mov  DWORD PTR -12[rbp], 0 ;// assign [user.a]
    mov  BYTE PTR -8[rbp], 0 ;// assign [user.b]
    mov  DWORD PTR -4[rbp], 0 ;// assign [user.d]
    mov  DWORD PTR -36[rbp], 0 ;// assign [user1.a]
    mov  BYTE PTR -32[rbp], 0 ;// assign [user1.b]
    mov  DWORD PTR -28[rbp], 0 ;// assign [user1.d]
    mov  eax, 0
    leave 
    ret  
.endmain:.section	.note.GNU-stack,"",@progbits

