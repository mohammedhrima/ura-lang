	.text
	.intel_syntax noprefix
	.file	"tmp.c"
	.globl	print
	.p2align	4, 0x90
	.type	print,@function
print:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	qword ptr [rbp - 8], rdi
	mov	rax, qword ptr [rbp - 8]
	mov	esi, dword ptr [rax]
	movabs	rdi, offset .L.str
	mov	al, 0
	call	printf
	add	rsp, 16
	pop	rbp
	ret
.Lfunc_end0:
	.size	print, .Lfunc_end0-print

	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 16
	mov	dword ptr [rbp - 4], 10
	lea	rax, [rbp - 4]
	mov	qword ptr [rbp - 16], rax
	mov	rdi, qword ptr [rbp - 16]
	call	print
	xor	eax, eax
	add	rsp, 16
	pop	rbp
	ret
.Lfunc_end1:
	.size	main, .Lfunc_end1-main

	.type	.L.str,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"%d"
	.size	.L.str, 3

	.ident	"Ubuntu clang version 12.0.1-19ubuntu3"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym print
	.addrsig_sym printf
