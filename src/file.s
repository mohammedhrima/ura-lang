.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

m:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  QWORD PTR -8[rbp], rdi ;// assign [user] is_ref
    mov  eax, 0
    leave 
    ret  
.endm:

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  DWORD PTR -8[rbp], 0 ;// assign [user1.id]
    mov  BYTE PTR -4[rbp], 0 ;// assign [user1.name]
    call m
    mov  eax, 0
    leave 
    ret  
.endmain:
.section	.note.GNU-stack,"",@progbits

