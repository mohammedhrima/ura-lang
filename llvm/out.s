	.file	"if_else"
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
	je	.LBB0_1
# %bb.2:                                # %else_body
	movl	$4, -4(%rsp)
	movl	-4(%rsp), %eax
	retq
.LBB0_1:                                # %if_body
	movl	$3, -4(%rsp)
	movl	-4(%rsp), %eax
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc
                                        # -- End function
	.section	".note.GNU-stack","",@progbits
