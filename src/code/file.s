	.build_version macos, 15, 0
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_hello                          ; -- Begin function hello
	.p2align	2
_hello:                                 ; @hello
	.cfi_startproc
; %bb.0:                                ; %entry
	mov	w0, #13                         ; =0xd
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
	bl	_hello
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	add	w8, w0, #1
	stp	w8, w0, [sp, #8]
	mov	w0, w8
	add	sp, sp, #32
	ret
	.cfi_endproc
                                        ; -- End function
.subsections_via_symbols
