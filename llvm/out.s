	.build_version macos, 15, 0
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_sum                            ; -- Begin function sum
	.p2align	2
_sum:                                   ; @sum
	.cfi_startproc
; %bb.0:                                ; %entry
	sub	sp, sp, #32
	.cfi_def_cfa_offset 32
	add	x9, sp, #32
	add	x8, sp, #8
	stp	xzr, x9, [sp]
LBB0_1:                                 ; %loop_header
                                        ; =>This Inner Loop Header: Depth=1
	ldr	w9, [sp]
	cmp	w9, w0
	b.ge	LBB0_3
; %bb.2:                                ; %loop_body
                                        ;   in Loop: Header=BB0_1 Depth=1
	lsl	w10, w9, #3
	ldr	w11, [sp, #4]
	add	w9, w9, #1
	ldr	w10, [x8, w10, sxtw]
	add	w10, w11, w10
	stp	w9, w10, [sp]
	b	LBB0_1
LBB0_3:                                 ; %loop_exit
	ldr	w0, [sp, #4]
	add	sp, sp, #32
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:                                ; %entry
	sub	sp, sp, #48
	stp	x29, x30, [sp, #32]             ; 16-byte Folded Spill
	.cfi_def_cfa_offset 48
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w8, #30                         ; =0x1e
	mov	w9, #20                         ; =0x14
	mov	w0, #3                          ; =0x3
	stp	x9, x8, [sp, #8]
	mov	w8, #10                         ; =0xa
	str	x8, [sp]
	bl	_sum
	ldp	x29, x30, [sp, #32]             ; 16-byte Folded Reload
	add	sp, sp, #48
	ret
	.cfi_endproc
                                        ; -- End function
.subsections_via_symbols
