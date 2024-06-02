.section	.note.GNU-stack,"",@progbits
.intel_syntax noprefix
.include "_allocate1.s"

main:
    push	rbp
	mov	    rbp, rsp
	sub	    rsp, 16
	mov	    edi, 100
	call	allocate
	mov	    QWORD PTR -8[rbp], rax
	mov	    edi, 90
	call	allocate
	mov	    QWORD PTR -8[rbp], rax
	mov	    rax, QWORD PTR -8[rbp]
	sub	    rax, 8
	mov	    rax, QWORD PTR [rax]
	mov	    rsi, rax
	lea	    rax, .LC0[rip]
	mov	    rdi, rax
	mov	    eax, 0
	call	printf@PLT
	mov	    edi, 80
	call	allocate
	mov	    QWORD PTR -8[rbp], rax
	mov	    rax, QWORD PTR -8[rbp]
	sub	    rax, 8
	mov	    rax, QWORD PTR [rax]
	mov	    rsi, rax
	lea	    rax, .LC0[rip]
	mov	    rdi, rax
	mov	    eax, 0
	call	printf@PLT
	mov	    edi, 70
	call	allocate
	mov	    QWORD PTR -8[rbp], rax
	mov	    rax, QWORD PTR -8[rbp]
	sub	    rax, 8
	mov	    rax, QWORD PTR [rax]
	mov	    rsi, rax
	lea	    rax, .LC0[rip]
	mov	    rdi, rax
	mov	    eax, 0
	call	printf@PLT
	mov	    edi, 60
	call	allocate
	mov	    QWORD PTR -8[rbp], rax
	mov	    rax, QWORD PTR -8[rbp]
	sub	    rax, 8
	mov	    rax, QWORD PTR [rax]
	mov	    rsi, rax
	lea	    rax, .LC0[rip]
	mov	    rdi, rax
	mov	    eax, 0
	call	printf@PLT
	mov	    eax, 0
	call	free_memory
	mov	    eax, 0
	leave
	ret
