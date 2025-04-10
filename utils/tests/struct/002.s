.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 160
    lea  rax, .STR0[rip]
    mov  QWORD PTR -16[rbp], rax ;// assign [user1.data.name]
    mov  BYTE PTR -32[rbp], 0 ;// assign [user1.e]
    mov  DWORD PTR -28[rbp], 0 ;// assign [user1.b]
    mov  BYTE PTR -24[rbp], 0 ;// assign [user1.c]
    mov  DWORD PTR -8[rbp], 10 ;// assign [user1.data.a]
    lea  rax, .STR0[rip]
    mov  QWORD PTR -96[rbp], rax ;// assign [user2.data.name]
    mov  BYTE PTR -112[rbp], 0 ;// assign [user2.e]
    mov  DWORD PTR -108[rbp], 0 ;// assign [user2.b]
    mov  BYTE PTR -104[rbp], 0 ;// assign [user2.c]
    mov  DWORD PTR -88[rbp], 11 ;// assign [user2.data.a]
    mov  eax, 0
    leave 
    ret  
.endmain:
.STR0: .string ""
.STR0: .string ""
.section	.note.GNU-stack,"",@progbits

