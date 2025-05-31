.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 32
    lea  rax, .STR0[rip]
    mov  QWORD PTR -16[rbp], rax ;// assign [user.name]
    mov  DWORD PTR -8[rbp], 0 ;// assign [user.age]
    lea  rax, .STR0[rip]
    mov  QWORD PTR -32[rbp], rax ;// assign [user1.name]
    mov  DWORD PTR -24[rbp], 0 ;// assign [user1.age]
    mov  eax, 0
    leave 
    ret  
.endmain:
.STR0: .string ""
.STR0: .string ""
.section	.note.GNU-stack,"",@progbits

