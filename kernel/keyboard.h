/*
Basic keyboard driver based on polling
Author: Pablo Villalobos (pablo-vs)
*/

/*
This is almost completely useless.
Issues:
	1. This code behaves as if the keyboard sent characters directly and never
	sent key release codes or other special codes, and therefore does not suport
	caps, control sequences, etc.
	2. The keyboard layout is wrong.
	3. It only works via polling, interrupts are preferable.
*/


#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "io.h"

//IO port addresses
#define STATUS_REG (UINT16) 0x64
#define DATA_REG (UINT16) 0x60

#define WAIT_TIMEOUT 10

//Keyboard command codes
#define KB_ACK (UINT8) 0xFA
#define KB_ECHO (UINT8) 0xEE
#define KB_SCAN_ENABLE (UINT8) 0xF4
#define KB_SET_SCAN_CODE (UINT8) 0xF0
#define KB_SCAN_CODE_2 (UINT8) 0x02

//Keycodes

unsigned char keycodes[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 9, '`', 0,	//0x00-0x0F	
	0, 0, 0, 0, 0, 'Q', '1', 0,
	0, 0, 'Z', 'S', 'A', 'W', '2', 0,	//0x10-0x1F	
	0, 'C', 'X', 'D', 'E', '4', '3', 0,
	0, ' ', 'V', 'F', 'T', 'R', '5', 0,	//0x20-0x2F	
	0, 'N', 'B', 'H', 'G', 'Y', '6', 0,
	0, 0, 'M', 'J', 'U', '7', '8', 0,	//0x30-0x3F	
	0, ',', 'K', 'I', 'O', '0', '9', 0,
	0, '.', '/', 'L', ';', 'P', '-', 0,	//0x40-0x4F	
	0, 0, '\'', 0, '[', '=', 0, 0,
	0, 0, '\n', ']', 0, '\\', 0, 0,		//0x50-0x5F	
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, '`', 0,	//0x60-0x6F	
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, '`', 0,	//0x70-0x7F	
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, '`', 0,	//0x80-0x8F	
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, '`', 0,	//0x90-0x9F	
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, '`', 0,	//0xA0-0xAF	
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, '`', 0,	//0xB0-0xBF	
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, '`', 0,	//0xC0-0xCF	
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, '`', 0,	//0xD0-0xDF	
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, '`', 0,	//0xE0-0xEF	
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, '`', 0,	//0xF0-0xFF	
	
};


static char fromKeycode(UINT8 kc) {
	return keycodes[kc];
}

static int sendb(UINT8 data) {
	UINT8 ready = 1;
	while(ready != 0) {
		ready = inb(STATUS_REG) & 2;
	}
	outb(DATA_REG, data);
	return 0;
}

static UINT8 readb() {
	UINT8 ready = 0;
	while(ready == 0) {
		ready = inb(STATUS_REG) & 1;
	}
	UINT8 data = inb(DATA_REG);
	return data;
}

static int check_kb() {
	int retval = -1;
	sendb(KB_ECHO);
	UINT8 val = readb();
	if(val == KB_ECHO) retval = 0;
	return retval;
}

static int init_kb() {
	int retval = -1;
	if(check_kb() == 0) {
		//Disable PS/2 controller translation
		outb(0x64, 0x20);
		UINT8 res = readb();
		outb(0x64, 0x60);
		res &= ~64;
		sendb(res);
		//Set scan code set 2
		sendb(KB_SET_SCAN_CODE);
		sendb(KB_SCAN_CODE_2);
		res = readb();
		if(res == KB_ACK) {
			//Enable scan		
			sendb(KB_SCAN_ENABLE);
			res = readb();
			if(res == KB_ACK) retval = 0;
		}
	}
	return retval;
}

static char read_kb() {
	UINT8 kc = readb();
	while(kc > 0x80) {
		kc = readb();
		kc = readb();	
	}
	return fromKeycode(kc);
}


#endif
