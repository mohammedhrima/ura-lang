.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  DWORD PTR -8[rbp], 0 ;// assign [user1.id]
    mov  BYTE PTR -4[rbp], 0 ;// assign [user1.name]
    mov  eax, 0
    leave 
    ret  
.endmain:
.section	.note.GNU-stack,"",@progbits

