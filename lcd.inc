	;; API to access the display.

.equ	LCDDP	= PORTC		;LCD data port
.equ	LCDCP	= PORTD		;LCD control interface port
.equ	LCDE	= 5		;number of Enable pin on LCDCP
.equ	LCDRW	= 6		;number of R/W pin on LCDCP
.equ	LCDRS	= 7		;number of Register Select on LCDCP

.cseg
	
lcd_send: 			;send r0 to display
	push	r16
	push	r17

	cbi	LCDCP,	LCDRW
	
	ldi	r16,	0xF0	;load mask for higher nibble
	and	r16,	r0	;get nibble
	swap	r16		;swap nibbles
	
	in	r17,	LCDDP	;get current output
	andi	r17,	0xF0	;clear lower nibble
	or	r17,	r16	;set data
	out	LCDDP,	r17	;set new output

	sbi	LCDCP,	LCDE	;data ready
	nop
	nop
	nop
	cbi	LCDCP,	LCDE

	ldi	r16,	0x0F	;load mask for lower nibble
	and	r16,	r0	;get nibble
	
	andi	r17,	0xF0	;clear higher nibble
	or	r17,	r16	;set data
	out	LCDDP,	r17	;set new output

	sbi	LCDCP,	LCDE	;data ready
	nop
	nop
	nop
	cbi	LCDCP,	LCDE

	rcall	delay_1K
	rcall	delay_1K

	pop	r17
	pop	r16
	ret
	;; end lcd_send
	
lcd_init:
	push	r16

	ldi	r16,	20	;wait 19ms
	dec	r16
	rcall	delay_1k
	brne	PC-6
	
	ldi	r16,	0x0F	;set used pins to output
	out	LCDDP-1,r16
	sbi	LCDCP-1,LCDRS
	sbi	LCDCP-1,LCDRW
	sbi	LCDCP-1,LCDE

	cbi	LCDCP,	LCDRS
	
	ldi	r16,	0x33	;init
	mov	r0,	r16
	rcall	lcd_send
	
	ldi	r16,	0x32	;switch to 4-bit-mode
	mov	r0,	r16
	rcall	lcd_send

	ldi	r16,	0x28	;function set
	mov	r0,	r16	;4 lines | 5x7-font | unused | unused
	rcall	lcd_send

	ldi	r16,	0x0F	;display control
	mov	r0,	r16	;display on | cursor on | cursor blinking
	rcall	lcd_send

	ldi	r16,	0x06	;cursor settings
	mov	r0,	r16	;cursor incrementing | no auto-shifting
	rcall	lcd_send

	ldi	r16,	1	;clear display
	mov	r0,	r16
	rcall	lcd_send
	
	sbi	LCDCP,	LCDRS	;sending data
	
	pop	r16
	ret
	;; end lcd_init

lcd_puthh:			;do not call!
	mov	r17,	r16	;use lcd_puth instead
	andi	r17,	0x0F
	cpi	r17,	10
	brsh	lcd_puthh_a
	subi	r17,	-'0'
	mov	r0,	r17
	rjmp	lcd_putc

lcd_puthh_a:			;alpha char
	subi	r17,	-'A' + 10
	mov	r0,	r17
	rjmp	lcd_putc
	
lcd_puth:			;send hexcode for r0 to display
	push	r16
	push	r17

	mov	r17,	r0

	ldi	r16, '0'
	mov	r0,	r16
	rcall	lcd_putc
	
	ldi	r16, 'x'
	mov	r0,	r16
	rcall	lcd_putc

	mov	r16,	r17
	swap	r16
	rcall	lcd_puthh
	swap	r16
	rcall	lcd_puthh
	
	pop	r17
	pop	r16
	ret
	;; end lcd_puth
	
lcd_set_line:			;set cursor to line specified by 'line'
	push	r16
	push	ZL
	push	ZH
	
	lds	r16,	line
	andi	r16,	0x03
	ldi	ZH,	HIGH(lines)
	ldi	ZL,	LOW(lines)

	add	ZL,	r16
	clr	r16
	adc	ZH,	r16

	ld	r16,	Z
	
	sbr	r16,	1 << 7
	mov	r0,	r16

	cbi	LCDCP,	LCDRS	;send command
	rcall	lcd_send
	sbi	LCDCP,	LCDRS	;send data

	ldi	r16,	0x00
	sts	line+1,	r16

	pop	ZH
	pop	ZL
	pop	r16
	ret
	;; end lcd_set_line

lcd_putc:
	push	r16

	mov	r16,	r0
	cpi	r16,	23
	breq	lcd_putc_newline

	rcall	lcd_send	;draw char
	
	lds	r16,	line+1	;get cursor pos in line
	cpi	r16,	19
	brlt	lcd_putc_end	;skip if not overflown
lcd_putc_newline:	
	lds	r16,	line	;get current line
	inc	r16		;inc line
	sts	line,	r16	;write back
	rcall	lcd_set_line	;move cursor to new line
	ldi	r16,	0xFF	;will be incremented to 0x00

lcd_putc_end:	
	inc	r16		;increment cursor pos
	sts	line+1,	r16	;store new pos
	pop	r16		;done.
	ret
	
lcd_puts:
	push	r16
	sbic	EECR,	EEWE	; block if not ready
	rjmp	-4

lcd_puts_loop:
	out	EEARH,	ZH	;get data from eeprom
	out	EEARL,	ZL
	sbi	EECR,	EERE
	in	r16,	EEDR
	
	tst	r16
	breq	lcd_puts_end

	mov	r0,	r16
	rcall	lcd_putc
	adiw	ZL,	1
	rjmp	lcd_puts_loop
	
lcd_puts_end:	
	pop	r16
	ret
	;; end lcd_puts
	
delay_10:			;wait 10 cycles
	;; rcall: 		;+ 3
	nop			;+ 1
	nop			;+ 1
	nop			;+ 1
	ret			;+ 4
	 			;=10
	;; end delay10

	
delay_1K:			;wait 1,000 cycles (1ms on 1MHz)
	;; rcall		;+    3
	push	r16		;+    2
	
	ldi	r16,	76	;+    1

delay_1K_loop:			;+  988 (= 13 * 76)
	rcall	delay_10		;+ 10
	dec	r16			;+  1
	brne	delay_1K_loop		;+  2
					;= 13
	 			;-    1
	nop			;+    1
	pop	r16		;+    2
	ret			;+    4
	 			;=1,000
	;; end delay1k

	
delay_1M:			;wait 1,000,000 cycles (1s on 1MHz)
	;; rcall		;+        3
	push	r16		;+        2
	
	ldi	r16,	199	;+        1

delay_1M_loop:			;+  999,975	(= 5,025 * 199)
	rcall	delay_1K		;+ 1,000
	rcall	delay_1K		;+ 1,000
	rcall	delay_1K		;+ 1,000
	rcall	delay_1K		;+ 1,000
	rcall	delay_1K		;+ 1,000
	rcall	delay_10		;+    10
	rcall	delay_10		;+    10
	nop				;+     1
	nop				;+     1
	dec	r16			;+     1
	brne	delay_1M_loop		;+     2
	 				;= 5,025
	 			;-        1
	rcall	delay_10	;+       10
	nop			;+        1
	nop			;+        1
	nop			;+        1
	nop			;+        1
	pop	r16		;+        2
	ret			;+        4
	 			;=1,000,000
	;; end delay1M

