.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 64
    lea  rax, .STR0[rip]
    mov  QWORD PTR -32[rbp], rax ;// assign [user1.data.name]
    mov  DWORD PTR -24[rbp], 0 ;// assign [user1.data.a]
    mov  BYTE PTR -16[rbp], 0 ;// assign [user1.e]
    mov  DWORD PTR -12[rbp], 0 ;// assign [user1.b]
    mov  BYTE PTR -8[rbp], 0 ;// assign [user1.c]
    mov  DWORD PTR -24[rbp], 10 ;// assign [user1.data.a]
    lea  rax, .STR0[rip]
    mov  QWORD PTR -64[rbp], rax ;// assign [user2.data.name]
    mov  DWORD PTR -56[rbp], 0 ;// assign [user2.data.a]
    mov  BYTE PTR -48[rbp], 0 ;// assign [user2.e]
    mov  DWORD PTR -44[rbp], 0 ;// assign [user2.b]
    mov  BYTE PTR -40[rbp], 0 ;// assign [user2.c]
    mov  DWORD PTR -56[rbp], 11 ;// assign [user2.data.a]
    mov  eax, 0
    leave 
    ret  
.endmain:
.STR0: .string ""
.STR0: .string ""
.section	.note.GNU-stack,"",@progbits

