.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

    mov  QWORD PTR -8[rbp], 0 ;// declare [name]
    main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 64
    mov  QWORD PTR -8[rbp], 0 ;// declare [name]
    mov  DWORD PTR -32[rbp], 0 ;// declare [age]
    mov  eax, 0
    leave 
    ret  
.endmain:.section	.note.GNU-stack,"",@progbits

