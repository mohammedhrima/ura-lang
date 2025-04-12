.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 96
    lea  rax, .STR0[rip]
    mov  QWORD PTR -32[rbp], rax ;// assign [user1.data.name]
    mov  DWORD PTR -24[rbp], 0 ;// assign [user1.data.a]
    mov  BYTE PTR -16[rbp], 0 ;// assign [user1.e]
    mov  DWORD PTR -12[rbp], 0 ;// assign [user1.b]
    mov  BYTE PTR -8[rbp], 0 ;// assign [user1.c]
    lea  rax, .STR0[rip]
    mov  QWORD PTR -64[rbp], rax ;// assign [user2.data.name]
    mov  DWORD PTR -56[rbp], 0 ;// assign [user2.data.a]
    mov  BYTE PTR -48[rbp], 0 ;// assign [user2.e]
    mov  DWORD PTR -44[rbp], 0 ;// assign [user2.b]
    mov  BYTE PTR -40[rbp], 0 ;// assign [user2.c]
    lea  rax, .STR0[rip]
    mov  QWORD PTR -96[rbp], rax ;// assign [user3.data.name]
    mov  DWORD PTR -88[rbp], 0 ;// assign [user3.data.a]
    mov  BYTE PTR -80[rbp], 0 ;// assign [user3.e]
    mov  DWORD PTR -76[rbp], 0 ;// assign [user3.b]
    mov  BYTE PTR -72[rbp], 0 ;// assign [user3.c]
    mov  eax, 0
    leave 
    ret  
.endmain:
.STR0: .string ""
.STR0: .string ""
.STR0: .string ""
.section	.note.GNU-stack,"",@progbits

