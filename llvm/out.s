	.build_version macos, 16, 0
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:                                ; %entry
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	mov	w0, wzr
LBB0_1:                                 ; %while.cond.0
                                        ; =>This Inner Loop Header: Depth=1
	cmp	w0, #4
	str	w0, [sp, #12]
	b.gt	LBB0_3
; %bb.2:                                ; %while.body.1
                                        ;   in Loop: Header=BB0_1 Depth=1
	add	w0, w0, #1
	b	LBB0_1
LBB0_3:                                 ; %while.end.2
	add	sp, sp, #16
	ret
	.cfi_endproc
                                        ; -- End function
.subsections_via_symbols
