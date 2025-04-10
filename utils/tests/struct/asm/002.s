.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 64
    mov  QWORD PTR -8[rbp], 0 ;// declare [user1.data.name]
    mov  BYTE PTR -17[rbp], 0 ;// declare [user1.e]
    mov  DWORD PTR -24[rbp], 0 ;// declare [user1.b]
    mov  BYTE PTR -25[rbp], 0 ;// declare [user1.c]
    mov  DWORD PTR -12[rbp], 10 ;// assign [user1.data.a]
    mov  QWORD PTR -40[rbp], 0 ;// declare [user2.data.name]
    mov  BYTE PTR -49[rbp], 0 ;// declare [user2.e]
    mov  DWORD PTR -56[rbp], 0 ;// declare [user2.b]
    mov  BYTE PTR -57[rbp], 0 ;// declare [user2.c]
    mov  DWORD PTR -44[rbp], 11 ;// assign [user2.data.a]
    mov  eax, 0
    leave 
    ret  
.endmain:
.section	.note.GNU-stack,"",@progbits

