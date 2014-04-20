.include "m8def.inc"

.def	BUFSH	=r14
.def	BUFSL	=r15
	
.cseg

.org	0x00
null:	rjmp	boot		;reset
	reti			;INT0
	reti			;INT1
	reti			;TIMER2 COMP
	reti			;TIMER2 OVF
	reti			;TIMER1 CAPT
	reti			;TIMER1 COMPA
	reti			;TIMER1 COMPB
	reti			;TIMER1 OVF
	reti			;TIMER0 OVF
	reti			;SPI, STC
	reti			;USART, RXC
	reti			;USART, UDRE
	reti			;USART, TXC
	reti			;ADC
	reti			;EE_RDY
	reti			;ANA_COMP
	reti			;TWI
	reti			;SPM_RDY

boot:	ldi	r16,	LOW(RAMEND) ;set ram end
	out	SPL,	r16
	ldi	r16,	HIGH(RAMEND)
	out	SPH,	r16

	ldi	r16,	0x03
	sts	line,	r16
	ldi	r16,	0x00
	sts	line+1,	r16
	
	ldi	XH,	HIGH(lcdbuf)
	ldi	XL,	LOW(lcdbuf)

	mov	BUFSH,	XH
	mov	BUFSL,	XL

	ldi	r16,	80
	ldi	r17,	' '
	mov	ZH,	XH
	mov	ZL,	XL

boot_init_loop:
	tst	r16
	breq	boot_init_loop_end

	st	Z+,	r17
	dec	r16
	
	rjmp	boot_init_loop
boot_init_loop_end:
	
	adiw	XL,	60
	
	rjmp	main
	
