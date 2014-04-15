.include "m8def.inc"

.cseg
	
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
boot:	sei
	
	ldi	r16,	LOW(RAMEND) ;set ram end
	out	SPL,	r16
	ldi	r16,	HIGH(RAMEND)
	out	SPH,	r16

boot_preload:	 			;move preloaded data
	ldi	ZL,	LOW(preload)	;EEPROM address
	ldi	ZH,	HIGH(preload)
	ldi	XL,	LOW(preloaded)	;SRAM address
	ldi	XH,	HIGH(preloaded)

boot_preload_loop:
	cpi	ZH,	HIGH(preload_end)
	brne	boot_preload_do
	cpi	ZL,	LOW(preload_end)
	breq	boot_preload_end
	
boot_preload_do:	
	sbic	EECR,	EEWE
	rjmp	-4

	out	EEARH,	ZH
	out	EEARL,	ZL
	sbi	EECR,	EERE
	in	r16,	EEDR
	st	X+,	r16
	adiw	ZL,	1
	
	rjmp	boot_preload_loop
	
boot_preload_end:	

	ldi	r16,	0x00
	sts	line,	r16
	sts	line+1,	r16
	rjmp	main
	
