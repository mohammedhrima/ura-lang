	.build_version macos, 15, 0
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_add_multiply                   ; -- Begin function add_multiply
	.p2align	2
_add_multiply:                          ; @add_multiply
	.cfi_startproc
; %bb.0:                                ; %entry
	sub	sp, sp, #32
	.cfi_def_cfa_offset 32
	add	w8, w0, w1
	stp	w1, w0, [sp, #24]
	mul	w0, w8, w2
	stp	w8, w2, [sp, #16]
	str	w0, [sp, #12]
	add	sp, sp, #32
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:                                ; %entry
	sub	sp, sp, #32
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	.cfi_def_cfa_offset 32
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w0, #5                          ; =0x5
	mov	w1, #3                          ; =0x3
	mov	w2, #2                          ; =0x2
	bl	_add_multiply
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	str	w0, [sp, #12]
	add	sp, sp, #32
	ret
	.cfi_endproc
                                        ; -- End function
.subsections_via_symbols
