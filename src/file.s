.intel_syntax noprefix
.include "/import/header.s"
.text
.globl	main

    mov  QWORD PTR -8[rbp], rdi ;// assign [size]
    mov  QWORD PTR -8[rbp], rdi ;// assign [size]
    mov  QWORD PTR -16[rbp], rsi ;// assign [count]
    mov  QWORD PTR -8[rbp], rdi ;// assign [ptr]
    mov  QWORD PTR -16[rbp], rsi ;// assign [size]
    mov  QWORD PTR -8[rbp], rdi ;// assign [str]
    mov  QWORD PTR -8[rbp], rdi ;// assign [dest]
    mov  QWORD PTR -16[rbp], rsi ;// assign [src]
    mov  QWORD PTR -8[rbp], rdi ;// assign [dest]
    mov  QWORD PTR -16[rbp], rsi ;// assign [src]
    mov  QWORD PTR -24[rbp], rdx ;// assign [n]
    mov  QWORD PTR -8[rbp], rdi ;// assign [dest]
    mov  QWORD PTR -16[rbp], rsi ;// assign [src]
    mov  QWORD PTR -8[rbp], rdi ;// assign [dest]
    mov  QWORD PTR -16[rbp], rsi ;// assign [src]
    mov  QWORD PTR -24[rbp], rdx ;// assign [n]
    mov  QWORD PTR -8[rbp], rdi ;// assign [str1]
    mov  QWORD PTR -16[rbp], rsi ;// assign [str2]
    mov  QWORD PTR -8[rbp], rdi ;// assign [str1]
    mov  QWORD PTR -16[rbp], rsi ;// assign [str2]
    mov  QWORD PTR -24[rbp], rdx ;// assign [n]
    mov  QWORD PTR -8[rbp], rdi ;// assign [str]
    mov  QWORD PTR -8[rbp], rdi ;// assign [str]
    mov  DWORD PTR -12[rbp], esi ;// assign [c]
    mov  QWORD PTR -8[rbp], rdi ;// assign [str1]
    mov  QWORD PTR -16[rbp], rsi ;// assign [str2]
    mov  DWORD PTR -4[rbp], edi ;// assign [fd]
    mov  QWORD PTR -12[rbp], rsi ;// assign [buf]
    mov  DWORD PTR -16[rbp], edx ;// assign [count]
    mov  DWORD PTR -4[rbp], edi ;// assign [fd]
    mov  QWORD PTR -12[rbp], rsi ;// assign [buf]
    mov  DWORD PTR -16[rbp], edx ;// assign [count]
    mov  QWORD PTR -8[rbp], rdi ;// assign [str]
    mov  DWORD PTR -4[rbp], edi ;// assign [n]
    mov  QWORD PTR -8[rbp], rdi ;// assign [str]
    mov  DWORD PTR -4[rbp], edi ;// assign [c]
    mov  DWORD PTR -4[rbp], edi ;// assign [domain]
    mov  DWORD PTR -8[rbp], esi ;// assign [type]
    mov  DWORD PTR -12[rbp], edx ;// assign [protocol]
    mov  DWORD PTR -4[rbp], edi ;// assign [n]
    mov  QWORD PTR -8[rbp], rdi ;// assign [n]
    mov  DWORD PTR -4[rbp], edi ;// assign [status]
    main:
        push rbp
        mov  rbp, rsp
        sub  rsp, 0
    mov  eax, 0
    leave 
    ret  
    .endmain:
    .section	.note.GNU-stack,"",@progbits

