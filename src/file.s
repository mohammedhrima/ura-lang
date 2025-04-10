.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 32
    lea  rax, .STR0[rip]
    mov  QWORD PTR -16[rbp], rax ;// assign [name]
    mov  DWORD PTR -8[rbp], 0 ;// assign [a]
    mov  BYTE PTR -16[rbp], 0 ;// assign [user1.e]
    mov  BYTE PTR -8[rbp], 0 ;// assign [user1.c]
    mov  DWORD PTR -12[rbp], 10 ;// assign [user1.b]
    mov  eax, 0
    leave 
    ret  
.endmain:
.STR0: .string ""
.section	.note.GNU-stack,"",@progbits

