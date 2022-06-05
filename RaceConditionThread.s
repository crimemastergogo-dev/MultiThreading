	.arch armv8-a
	.file	"RaceConditionThread.c"
	.text
	.global	mails
	.bss
	.align	2
	.type	mails, %object
	.size	mails, 4
mails:
	.zero	4
	.text
	.align	2
	.global	Mailbox
	.type	Mailbox, %function
Mailbox:
.LFB6:
	.cfi_startproc
	sub	sp, sp, #16
	.cfi_def_cfa_offset 16
	str	wzr, [sp, 12]
	b	.L2
.L3:
	adrp	x0, mails
	add	x0, x0, :lo12:mails
	ldr	w0, [x0]
	add	w1, w0, 1
	adrp	x0, mails
	add	x0, x0, :lo12:mails
	str	w1, [x0]
	ldr	w0, [sp, 12]
	add	w0, w0, 1
	str	w0, [sp, 12]
.L2:
	ldr	w1, [sp, 12]
	mov	w0, 9999
	cmp	w1, w0
	ble	.L3
	nop
	add	sp, sp, 16
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE6:
	.size	Mailbox, .-Mailbox
	.section	.rodata
	.align	3
.LC0:
	.string	"Thread creation failed"
	.align	3
.LC1:
	.string	"No of mails = %d\n"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
.LFB7:
	.cfi_startproc
	stp	x29, x30, [sp, -64]!
	.cfi_def_cfa_offset 64
	.cfi_offset 29, -64
	.cfi_offset 30, -56
	mov	x29, sp
	str	w0, [sp, 28]
	str	x1, [sp, 16]
	str	wzr, [sp, 60]
	b	.L5
.L7:
	add	x1, sp, 32
	ldrsw	x0, [sp, 60]
	lsl	x0, x0, 3
	add	x4, x1, x0
	mov	x3, 0
	adrp	x0, Mailbox
	add	x2, x0, :lo12:Mailbox
	mov	x1, 0
	mov	x0, x4
	bl	pthread_create
	cmp	w0, 0
	beq	.L6
	adrp	x0, .LC0
	add	x0, x0, :lo12:.LC0
	bl	puts
.L6:
	ldr	w0, [sp, 60]
	add	w0, w0, 1
	str	w0, [sp, 60]
.L5:
	ldr	w0, [sp, 60]
	cmp	w0, 2
	ble	.L7
	ldr	x0, [sp, 32]
	mov	x1, 0
	bl	pthread_join
	ldr	x0, [sp, 40]
	mov	x1, 0
	bl	pthread_join
	ldr	x0, [sp, 48]
	mov	x1, 0
	bl	pthread_join
	adrp	x0, mails
	add	x0, x0, :lo12:mails
	ldr	w0, [x0]
	mov	w1, w0
	adrp	x0, .LC1
	add	x0, x0, :lo12:.LC1
	bl	printf
	mov	w0, 0
	ldp	x29, x30, [sp], 64
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE7:
	.size	main, .-main
	.ident	"GCC: (Debian 8.3.0-6) 8.3.0"
	.section	.note.GNU-stack,"",@progbits
