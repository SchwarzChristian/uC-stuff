uC-Test
=======

I'm just playing around with some avr-stuff.

Hardware
========

 * uC: Atmel ATmega8 (i'll switch to ATmega168 or ATmega8535 if the m8 stops working)
    - memory:  8KB Flash, 512B EEPROM, 1KB SRAM
    - I/O:     23 pins (22 usable, 1 on reset)
    - Voltage: 4.5V - 5.5V
    - data sheet: *** commimg soon ***

 * LCD: EA W204-NLED from Electronic Assembly
    - HD4480-compatible
    - 16-pin-Interface (8x I/O, but i'm using 4-bit-mode to save I/O pins)
    - 8 self-definable chars, ASCII predefined
    - Voltage: 5V
    - data sheet (german): *** comming soon ***

 * Programmer: AVRISP mkII clone
    - protocol: avrispmkii

Files
=====

boot.inc
--------

Includes controller-specific definitions, sets ISRs, initializes the uC and jumps to 'main'.

data.inc
--------

Stuff for the Memory.

lcd.inc
-------

API for communication with the display.

m8def.inc
---------

Controller-specific definitions.

main.asm
--------

The main program.
