.eseg				;stuff for the EEPROM

hello:
	.db	"You got this message from EEPROM:", 23, "Hello, World!", 0
				;i use 23 (0x17) for new line

preload:
	.db	0x00, 0x40, 0x14, 0x54
preload_end:

.dseg				;stuff for the SRAM

line:
	.byte	2		;cursor position

preloaded:	
lines:
	.byte	4		;line starting points
	
