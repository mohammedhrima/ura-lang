	.text
	.file	"my_module"
	.globl	main
	.p2align	4, 0x90
	.type	main,@function
main:
	.cfi_startproc
	pushq	%rax
	.cfi_def_cfa_offset 16
	movl	$.L__unnamed_1, %edi
	xorl	%eax, %eax
	callq	printf@PLT
	xorl	%eax, %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc

	.type	.L__unnamed_1,@object
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__unnamed_1:
	.asciz	"one\n"
	.size	.L__unnamed_1, 5

	.type	.L__unnamed_2,@object
.L__unnamed_2:
	.asciz	"two\n"
	.size	.L__unnamed_2, 5

	.type	.L__unnamed_3,@object
.L__unnamed_3:
	.asciz	"three\n"
	.size	.L__unnamed_3, 7

	.section	".note.GNU-stack","",@progbits
