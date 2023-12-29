.section	.note.GNU-stack,"",@progbits
.intel_syntax noprefix
.text
    .globl	main
main:
	push	rbp
	mov	rbp, rsp
    mov rax, 5
    mov rbx, 2
	imul rbx
    mov rax, rbx
	leave
	ret
