.include "boot.inc"
.include "lcd.inc"
.include "data.inc"

.cseg
	
main:
	ldi	r16,	0xFF
	out	DDRB,	r16
	out	PORTB,	r16
	
	rcall	lcd_init

	ldi	ZH,	HIGH(hello) ;say hello
	ldi	ZL,	LOW(hello)
	rcall	lcd_puts

	ldi	r16,	23
	mov	r0,	r16
	rcall	lcd_putc
	mov	r0,	r16
	rcall	lcd_puth

main_loop:
	com	r16
	out	PORTB,	r16
	rcall	delay_1M
	rjmp	main_loop

	ret			;should never be reached
	;; end main
	
