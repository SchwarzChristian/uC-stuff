CC	=	avra
FILE	=	main

ISP	=	sudo avrdude
PROT	=	-c avrispmkii
DEVICE	=	-P usb
UC	=	-p m8
PFLAGS	=	-y -u -U flash:w:$(FILE).hex -U eeprom:w:$(FILE).eep.hex

all: $(FILES)
	$(CC) $(FILE).asm

show: all
	avr-objdump -d $(FILE).hex | less

flash: all
	$(ISP) $(PROT) $(DEVICE) $(UC) $(PFLAGS)

clean:
	rm -rf *.o *.hex *.obj *.cof *~

.PHONY: clean
