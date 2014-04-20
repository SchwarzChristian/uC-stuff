.include "boot.inc"
.include "lcd.inc"
.include "kb.inc"
.include "data.inc"

.cseg
	
main:
	rcall	lcd_init

	ldi	ZH,	HIGH(hello)
	ldi	ZL,	LOW(hello)
	rcall	lcd_puts

	rcall	kb_init
	
	ldi	r16,	23

main_loop:
	rcall	kb_getc
	rcall	lcd_puth
	mov	r0,	r16
	rcall	lcd_putc
	rjmp	main_loop
	
	ret			;should never be reached
	;; end main
	
