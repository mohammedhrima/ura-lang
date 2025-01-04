.intel_syntax noprefix
.include "./import/header.s"

.text
.globl	main

main:
    push rbp
    mov  rbp, rsp
    sub  rsp, 16
    mov  DWORD PTR -4[rbp], 0 ;// assign [i]
    .while1:
    cmp  DWORD PTR -4[rbp], 5
    jge   .endwhile1
        mov  eax, DWORD PTR -4[rbp]
        add  eax, 1
        mov  DWORD PTR -4[rbp], eax ;// assign [i]
    jmp  .while1
    .endwhile1:
    mov  eax, 0
    leave 
    ret  
.endmain:.section	.note.GNU-stack,"",@progbits

