	.file	"if_else_chain"
	.text
	.globl	main                            # -- Begin function main
	.p2align	4
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	movl	$1, -4(%rsp)
	movb	$1, %al
	testb	%al, %al
	jne	.LBB0_4
# %bb.1:                                # %then1
	addl	$3, -4(%rsp)
	movl	-4(%rsp), %eax
	retq
.LBB0_4:                                # %cond2
	cmpl	$4, -4(%rsp)
	jne	.LBB0_5
# %bb.3:                                # %then2
	addl	$5, -4(%rsp)
	movl	-4(%rsp), %eax
	retq
.LBB0_5:                                # %else
	addl	$6, -4(%rsp)
	movl	-4(%rsp), %eax
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
