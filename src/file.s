.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 32
    mov  DWORD PTR -4[rbp], 0 ;// declare [abc.a]
    mov  BYTE PTR -5[rbp], 0 ;// declare [abc.b]
    mov  DWORD PTR -12[rbp], 0 ;// declare [abc.c]
    mov  DWORD PTR -16[rbp], 0 ;// declare [efg.a]
    mov  BYTE PTR -17[rbp], 0 ;// declare [efg.b]
    mov  DWORD PTR -24[rbp], 0 ;// declare [efg.c]
    mov  eax, DWORD PTR -16[rbp]
    mov  DWORD PTR -4[rbp], eax ;// assign [abc.a]
    mov  al, BYTE PTR -17[rbp]
    mov  BYTE PTR -5[rbp], al ;// assign [abc.b]
    mov  eax, DWORD PTR -24[rbp]
    mov  DWORD PTR -12[rbp], eax ;// assign [abc.c]
    mov  eax, 0
    leave 
    ret  
.endmain:.section	.note.GNU-stack,"",@progbits

