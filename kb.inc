.equ	KBP	=PORTB
.equ	KBD	=0
.equ	KBC	=1

.dseg
kb_sreg:
	.byte	1
	
.cseg
	
kb_init:
	push	r16
	
	cbi	KBP-1,	KBD	;set data pin as input
	cbi	KBP-1,	KBC	;set clock as input
	sbi	KBP,	KBD	;enable pullup
	sbi	KBP,	KBC	;for inputs

	ldi	r16,	0
	sts	kb_sreg,r16

	pop	r16
	ret

kb_getc:
	push	r16
	push	r17
	push	r18

	clr	r16,	0
	clr	r17,	0
	ldi	r18,	12

kb_getc_loop:	
	sbic	KBP,	KBC	;\ repeat until
	rjmp	kb_getc_loop	;/ data is ready
	lsl	r17		;\ shift current data
	rol	r16		;/ through buffer
	sbic	KBP,	KBD	;\ increment if
	inc	r16		;/ data is high
	dec	r18		;  dec loop counter
	brne	kb_getc_loop	;  repeat until counter is 0

	lsr	r16		;  shift out
	ror	r17		;  start bit

	mov	r0,	r17	;  set return value

	pop	r18		;  clean up
	pop	r17
	pop	r16
	ret
