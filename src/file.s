	.build_version macos, 16, 0
	.section	__TEXT,__text,regular,pure_instructions
	.globl	_to_ascci                       ; -- Begin function to_ascci
	.p2align	2
_to_ascci:                              ; @to_ascci
	.cfi_startproc
; %bb.0:                                ; %entry
                                        ; kill: def $w0 killed $w0 def $x0
	and	w8, w0, #0xff
	cmp	w8, #122
	b.hi	LBB0_2
; %bb.1:                                ; %switch.lookup
	sxtb	x8, w0
Lloh0:
	adrp	x9, l_switch.table.to_ascci@PAGE
Lloh1:
	add	x9, x9, l_switch.table.to_ascci@PAGEOFF
	ldr	w0, [x9, x8, lsl #2]
	ret
LBB0_2:
	mov	w0, #-1                         ; =0xffffffff
	ret
	.loh AdrpAdd	Lloh0, Lloh1
	.cfi_endproc
                                        ; -- End function
	.globl	_strcmp                         ; -- Begin function strcmp
	.p2align	2
_strcmp:                                ; @strcmp
	.cfi_startproc
; %bb.0:                                ; %entry
	sub	sp, sp, #48
	stp	x20, x19, [sp, #16]             ; 16-byte Folded Spill
	stp	x29, x30, [sp, #32]             ; 16-byte Folded Spill
	.cfi_def_cfa_offset 48
	.cfi_offset w30, -8
	.cfi_offset w29, -16
	.cfi_offset w19, -24
	.cfi_offset w20, -32
	mov	x19, x1
	mov	w8, wzr
LBB1_1:                                 ; %while
                                        ; =>This Inner Loop Header: Depth=1
	ldrb	w9, [x0, w8, sxtw]
	ldrb	w10, [x19, w8, sxtw]
	str	w8, [sp, #12]
	cmp	w9, w10
	ccmp	w9, #0, #4, eq
	b.eq	LBB1_3
; %bb.2:                                ; %while_bloc
                                        ;   in Loop: Header=BB1_1 Depth=1
	add	w8, w8, #1
	b	LBB1_1
LBB1_3:                                 ; %end_while
	ldrb	w0, [x0, w8, sxtw]
	bl	_to_ascci
	ldrsw	x8, [sp, #12]
	ldrb	w8, [x19, x8]
	mov	w19, w0
	mov	w0, w8
	bl	_to_ascci
	sub	w0, w19, w0
	ldp	x29, x30, [sp, #32]             ; 16-byte Folded Reload
	ldp	x20, x19, [sp, #16]             ; 16-byte Folded Reload
	add	sp, sp, #48
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
Lloh2:
	adrp	x0, l_str_literal@PAGE
Lloh3:
	add	x0, x0, l_str_literal@PAGEOFF
Lloh4:
	adrp	x1, l_str_literal.1@PAGE
Lloh5:
	add	x1, x1, l_str_literal.1@PAGEOFF
	bl	_strcmp
	ldp	x29, x30, [sp, #16]             ; 16-byte Folded Reload
	str	w0, [sp, #12]
	add	sp, sp, #32
	ret
	.loh AdrpAdd	Lloh4, Lloh5
	.loh AdrpAdd	Lloh2, Lloh3
	.cfi_endproc
                                        ; -- End function
	.section	__TEXT,__const
l_str_literal:                          ; @str_literal
	.asciz	"b"

l_str_literal.1:                        ; @str_literal.1
	.asciz	"a"

	.p2align	2, 0x0                          ; @switch.table.to_ascci
l_switch.table.to_ascci:
	.long	0                               ; 0x0
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	32                              ; 0x20
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	48                              ; 0x30
	.long	49                              ; 0x31
	.long	50                              ; 0x32
	.long	51                              ; 0x33
	.long	52                              ; 0x34
	.long	53                              ; 0x35
	.long	54                              ; 0x36
	.long	55                              ; 0x37
	.long	56                              ; 0x38
	.long	57                              ; 0x39
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	65                              ; 0x41
	.long	66                              ; 0x42
	.long	67                              ; 0x43
	.long	68                              ; 0x44
	.long	69                              ; 0x45
	.long	70                              ; 0x46
	.long	71                              ; 0x47
	.long	72                              ; 0x48
	.long	73                              ; 0x49
	.long	74                              ; 0x4a
	.long	75                              ; 0x4b
	.long	76                              ; 0x4c
	.long	77                              ; 0x4d
	.long	78                              ; 0x4e
	.long	79                              ; 0x4f
	.long	80                              ; 0x50
	.long	81                              ; 0x51
	.long	82                              ; 0x52
	.long	83                              ; 0x53
	.long	84                              ; 0x54
	.long	85                              ; 0x55
	.long	86                              ; 0x56
	.long	87                              ; 0x57
	.long	88                              ; 0x58
	.long	89                              ; 0x59
	.long	90                              ; 0x5a
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	4294967295                      ; 0xffffffff
	.long	97                              ; 0x61
	.long	98                              ; 0x62
	.long	99                              ; 0x63
	.long	100                             ; 0x64
	.long	101                             ; 0x65
	.long	102                             ; 0x66
	.long	103                             ; 0x67
	.long	104                             ; 0x68
	.long	105                             ; 0x69
	.long	106                             ; 0x6a
	.long	107                             ; 0x6b
	.long	108                             ; 0x6c
	.long	109                             ; 0x6d
	.long	110                             ; 0x6e
	.long	111                             ; 0x6f
	.long	112                             ; 0x70
	.long	113                             ; 0x71
	.long	114                             ; 0x72
	.long	115                             ; 0x73
	.long	116                             ; 0x74
	.long	117                             ; 0x75
	.long	118                             ; 0x76
	.long	119                             ; 0x77
	.long	120                             ; 0x78
	.long	121                             ; 0x79
	.long	122                             ; 0x7a

.subsections_via_symbols
