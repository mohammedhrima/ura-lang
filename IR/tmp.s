	.text
	.intel_syntax noprefix
	.file	"tmp.c"
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	lea	rdi, [rip + .L.str]
	call	strdup@PLT
	mov	qword ptr [rbp - 8], rax
	mov	rsi, qword ptr [rbp - 8]
	lea	rdi, [rip + .L.str.1]
	mov	al, 0
	call	printf@PLT
	mov	rdi, qword ptr [rbp - 8]
	call	free@PLT
	xor	eax, eax
	add	rsp, 16
	pop	rbp
	ret
.Lfunc_end0:
	.size	main, .Lfunc_end0-main

	.type	.L.str,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"afhhfh5fgh21fffffj56fg4j65ghbcd"
	.size	.L.str, 32

	.type	.L.str.1,@object
.L.str.1:
	.asciz	"%s\n"
	.size	.L.str.1, 4

	.ident	"Homebrew clang version 18.1.8"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym strdup
	.addrsig_sym printf
	.addrsig_sym free
