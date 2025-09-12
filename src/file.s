	.build_version macos, 15, 0
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:                                ; %entry
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	mov	w0, #1                          ; =0x1
LBB0_1:                                 ; %while
                                        ; =>This Inner Loop Header: Depth=1
	cmp	w0, #9
	str	w0, [sp, #12]
	b.gt	LBB0_3
; %bb.2:                                ; %while_bloc
                                        ;   in Loop: Header=BB0_1 Depth=1
	add	w0, w0, #1
	b	LBB0_1
LBB0_3:                                 ; %end_while
	add	sp, sp, #16
	ret
	.cfi_endproc
                                        ; -- End function
.subsections_via_symbols
