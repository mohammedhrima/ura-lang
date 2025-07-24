	.file	"file.w"
	.text
	.globl	m                               # -- Begin function m
	.p2align	4
	.type	m,@function
m:                                      # @m
	.cfi_startproc
# %bb.0:                                # %entry
	movl	$12, %eax
	retq
.Lfunc_end0:
	.size	m, .Lfunc_end0-m
	.cfi_endproc
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	4
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rax
	.cfi_def_cfa_offset 16
	callq	m@PLT
	xorl	%eax, %eax
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	main, .Lfunc_end1-main
	.cfi_endproc
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
