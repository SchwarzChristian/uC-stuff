.eseg				;stuff for the EEPROM

hello:
	.db	"waiting for data ...", 0
				;i use 23 (0x17) for new line

.dseg				;stuff for the SRAM

line:	.byte	102		;current line
lcdbuf:	.byte	320		;display background buffer
lcdbuf_end:	
	.byte	60
