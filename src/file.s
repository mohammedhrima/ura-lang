.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  DWORD PTR -16[rbp], 0 ;// assign [user.id]
    lea  rax, .STR0[rip]
    mov  QWORD PTR -8[rbp], rax ;// assign [user.name]
    mov  eax, 0
    leave 
    ret  
.endmain:
.STR0: .string ""
.section	.note.GNU-stack,"",@progbits

