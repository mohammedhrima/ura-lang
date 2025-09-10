	.build_version macos, 15, 0
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:                                ; %entry
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	mov	w8, #1                          ; =0x1
LBB0_1:                                 ; %while.sink.split
                                        ; =>This Loop Header: Depth=1
                                        ;     Child Loop BB0_2 Depth 2
	mov	w0, w8
	str	w8, [sp, #12]
LBB0_2:                                 ; %while
                                        ;   Parent Loop BB0_1 Depth=1
                                        ; =>  This Inner Loop Header: Depth=2
	cmp	w0, #9
	b.gt	LBB0_5
; %bb.3:                                ; %while_bloc
                                        ;   in Loop: Header=BB0_2 Depth=2
	cmp	w0, #5
	b.eq	LBB0_2
; %bb.4:                                ; %else
                                        ;   in Loop: Header=BB0_1 Depth=1
	ldr	w8, [sp, #12]
	add	w8, w8, #1
	b	LBB0_1
LBB0_5:                                 ; %end_while
	add	sp, sp, #16
	ret
	.cfi_endproc
                                        ; -- End function
.subsections_via_symbols
