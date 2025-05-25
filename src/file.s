.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 32
    mov  DWORD PTR -12[rbp], 0 ;// assign [user1.a]
    mov  DWORD PTR -8[rbp], 0 ;// assign [user1.b]
    mov  DWORD PTR -4[rbp], 0 ;// assign [user1.c]
    mov  DWORD PTR -24[rbp], 0 ;// assign [user2.a]
    mov  DWORD PTR -20[rbp], 0 ;// assign [user2.b]
    mov  DWORD PTR -16[rbp], 0 ;// assign [user2.c]
    mov  eax, DWORD PTR -12[rbp]
    mov  DWORD PTR -24[rbp], eax ;// assign [user2.a]
    mov  eax, DWORD PTR -8[rbp]
    mov  DWORD PTR -20[rbp], eax ;// assign [user2.b]
    mov  eax, DWORD PTR -4[rbp]
    mov  DWORD PTR -16[rbp], eax ;// assign [user2.c]
    mov  eax, 0
    leave 
    ret  
.endmain:
.section	.note.GNU-stack,"",@progbits

