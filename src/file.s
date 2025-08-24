	.file	"file.w"
	.text
	.globl	main                            # -- Begin function main
	.p2align	4
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:                                # %entry
	movl	$1, -4(%rsp)
	movl	$0, -8(%rsp)
	xorl	%eax, %eax
	testb	%al, %al
	jne	.LBB0_3
# %bb.1:                                # %if
	movl	$1, -8(%rsp)
	movb	$1, %al
	testb	%al, %al
	je	.LBB0_3
# %bb.2:                                # %if3
	movl	$5, -8(%rsp)
.LBB0_3:                                # %end_if
	movl	-8(%rsp), %eax
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
