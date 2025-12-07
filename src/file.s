	.build_version macos, 16, 0
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_putchar                        ; -- Begin function putchar
	.p2align	2
_putchar:                               ; @putchar
	.cfi_startproc
; %bb.0:                                ; %entry
	sub	sp, sp, #48
	stp	x29, x30, [sp, #32]             ; 16-byte Folded Spill
	.cfi_def_cfa_offset 48
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	ldur	x1, [sp, #14]
	strb	w0, [sp, #31]
	mov	w2, #1                          ; =0x1
	strb	w0, [x1]
	mov	w0, #1                          ; =0x1
	str	x1, [sp, #16]
	bl	_write
	ldp	x29, x30, [sp, #32]             ; 16-byte Folded Reload
	mov	w0, wzr
	add	sp, sp, #48
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
	mov	w0, #97                         ; =0x61
	bl	_putchar
	mov	w0, #10                         ; =0xa
	bl	_putchar
	mov	w0, wzr
	ldp	x29, x30, [sp], #16             ; 16-byte Folded Reload
	ret
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__cstring,cstring_literals
l_STR0:                                 ; @STR0
	.space	1

.subsections_via_symbols
