	.build_version macos, 16, 0
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_foo                            ; -- Begin function foo
	.p2align	2
_foo:                                   ; @foo
	.cfi_startproc
; %bb.0:                                ; %entry
	mov	w0, #5                          ; =0x5
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_bar                            ; -- Begin function bar
	.p2align	2
_bar:                                   ; @bar
	.cfi_startproc
; %bb.0:                                ; %entry
	mov	w0, #10                         ; =0xa
	ret
	.cfi_endproc
                                        ; -- End function
	.globl	_main                           ; -- Begin function main
	.p2align	2
_main:                                  ; @main
	.cfi_startproc
; %bb.0:                                ; %entry
	sub	sp, sp, #80
	stp	x20, x19, [sp, #48]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #64]             ; 16-byte Folded Spill
	.cfi_def_cfa_offset 80
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	w8, #10                         ; =0xa
	mov	w9, #20                         ; =0x14
	stp	w9, w8, [sp, #40]
	mov	w9, #30                         ; =0x1e
	mov	w8, #25                         ; =0x19
	stp	w8, w9, [sp, #32]
	bl	_foo
	ldp	w9, w8, [sp, #40]
	add	w8, w8, w0
	add	w9, w9, #25
	stp	w9, w8, [sp, #24]
	mov	w8, #70                         ; =0x46
	str	w8, [sp, #20]
	bl	_bar
	add	w8, w0, #50
	str	w8, [sp, #16]
	bl	_foo
	ldr	w8, [sp, #44]
	add	w8, w0, w8
	str	w8, [sp, #12]
	bl	_bar
	add	w8, w0, #100
	str	w8, [sp, #8]
	bl	_foo
	mov	w19, w0
	bl	_bar
	mov	w8, w0
	ldp	x29, x30, [sp, #64]             ; 16-byte Folded Reload
	add	w8, w19, w8
	ldp	x20, x19, [sp, #48]             ; 16-byte Folded Reload
	mov	w0, wzr
	str	w8, [sp, #4]
	add	sp, sp, #80
	ret
	.cfi_endproc
                                        ; -- End function
.subsections_via_symbols
