.section .note.GNU-stack,"",@progbits
.intel_syntax noprefix
.text 
	.globl main

main:
    push rbp
    mov rbp, rsp
    mov QWORD PTR -4[rbp], 10
    mov QWORD PTR -8[rbp], 5

	mov rax, QWORD PTR -4[rbp]
	add rax, QWORD PTR -8[rbp]

	mov QWORD PTR -12[rbp], rax
	mov rax, QWORD PTR -12[rbp]
    
	pop rbp
    ret

/*
	mov edx, QWORD PTR -4[rbp]
	mov eax, QWORD PTR -8[rbp]
	add eax, edx
	mov QWORD PTR -12[rbp], eax
	mov eax, QWORD PTR -12[rbp]
*/
