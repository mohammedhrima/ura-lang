	.build_version macos, 15, 0
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_isalpha                        ; -- Begin function isalpha
	.p2align	2
_isalpha:                               ; @isalpha
	.cfi_startproc
; %bb.0:                                ; %entry
	sxtb	w9, w0
	mov	w8, #123                        ; =0x7b
	cmp	w9, #96
	ccmp	w9, w8, #0, gt
	cset	w0, lt
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_isdigit                        ; -- Begin function isdigit
	.p2align	2
_isdigit:                               ; @isdigit
	.cfi_startproc
; %bb.0:                                ; %entry
	sxtb	w9, w0
	mov	w8, #58                         ; =0x3a
	cmp	w9, #48
	ccmp	w9, w8, #0, gt
	cset	w0, lt
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_islanum                        ; -- Begin function islanum
	.p2align	2
_islanum:                               ; @islanum
	.cfi_startproc
; %bb.0:                                ; %entry
	stp	x20, x19, [sp, #-32]!           ; 16-byte Folded Spill
	stp	x29, x30, [sp, #16]             ; 16-byte Folded Spill
	.cfi_def_cfa_offset 32
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	w19, w0
	bl	_isalpha
	mov	w20, w0
	mov	w0, w19
	bl	_isdigit
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	orr	w8, w20, w0
	and	w0, w8, #0x1
	ldp	x20, x19, [sp], #32             ; 16-byte Folded Reload
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:                                ; %entry
	stp	x29, x30, [sp, #-16]!           ; 16-byte Folded Spill
	.cfi_def_cfa_offset 16
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	mov	w0, #42                         ; =0x2a
	bl	_isdigit
	mov	w8, #11                         ; =0xb
	tst	w0, #0x1
	cinc	w0, w8, eq
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.cfi_endproc
                                        ; -- End function
.subsections_via_symbols
