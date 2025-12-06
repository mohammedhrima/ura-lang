	.build_version macos, 16, 0
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:                                ; %entry
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	mov	w8, #97                         ; =0x61
	strb	w8, [sp, #8]
	ldr	w9, [sp, #8]
	strb	w8, [sp, #15]
	cmp	w9, #127
	ccmp	w9, #0, #8, le
	csinv	w0, w9, wzr, ge
	add	sp, sp, #16
	ret
	.cfi_endproc
                                        ; -- End function
.subsections_via_symbols
