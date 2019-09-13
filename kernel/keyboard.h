/*
 * Basic keyboard driver based on interrupts
 * Author: Pablo Villalobos (pablo-vs)
 *
 * Usage: Call init_kb(), read struct keypresses with read_kb().
 * Issues:
	1. Missing support for right_ctrl characters
	2. Missing support for different layouts
 */


#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "io.h"
#include "interrupts.h"
#include "macros.h"

#define BUF_SIZE 128 

//Keycodes
#define KEY_RELEASED 0xF0
#define KEY_ALT 0x11
#define KEY_SHIFT 0x12
#define KEY_CTRL 0x14
#define KEY_CAPS_LOCK 0x58

unsigned char keycodes[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, '\t', 0, 0,			//0x00-0x0F
	0, 0, 0, 0, 0, 'q', '1', 0,
	0, 0, 'z', 's', 'a', 'w', '2', 0,		//0x10-0x1F
	0, 'c', 'x', 'd', 'e', '4', '3', 0,
	0, ' ', 'v', 'f', 't', 'r', '5', 0,		//0x20-0x2F
	0, 'n', 'b', 'h', 'g', 'y', '6', 0,
	0, 0, 'm', 'j', 'u', '7', '8', 0,		//0x30-0x3F
	0, ',', 'k', 'i', 'o', '0', '9', 0,
	0, '.', '-', 'l', ';', 'p', '\'', 0,		//0x40-0x4F
	0, 0, '\'', 0, '\'', '`', 0, 0,
	0, 0, '\n', '+', 0, 0, 0, 0,			//0x50-0x5F
	0, '<', 0, 0, 0, 0, '\b',
	0, 0, 0, 0, 0, 0, 0, 0, 0,			//0x60-0x6F
};

unsigned char keycodes_shift[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, '\t', 0, 0,			//0x00-0x0F
	0, 0, 0, 0, 0, 'Q', '!', 0,
	0, 0, 'Z', 'S', 'A', 'W', '"', 0,		//0x10-0x1F
	0, 'C', 'X', 'D', 'E', '$', 0, 0,
	0, ' ', 'V', 'F', 'T', 'R', '%', 0,		//0x20-0x2F
	0, 'N', 'B', 'H', 'G', 'Y', '&', 0,
	0, 0, 'M', 'J', 'U', '/', '(', 0,		//0x30-0x3F
	0, ';', 'K', 'I', 'O', '=', ')', 0,
	0, ':', '_', 'L', ';', 'P', '?', 0,		//0x40-0x4F
	0, 0, '"', 0, 0, '^', 0, 0,
	0, 0, '\n', '*', 0, 0, 0, 0,			//0x50-0x5F
	0, '>', 0, 0, 0, 0, '\b', 0,
	0, 0, 0, 0, 0, 0, 0, 0,				//0x60-0x6F
};

/**
 * Represents a key press (or release). c is the ASCII character of the key.
 * The bits in info represent the following:
 *	Bit	Meaning
 *	0	0 for key press, 1 for key release
 *	1	Caps lock enabled
 *	2	Ctrl pressed
 *	3	Alt pressed
 *	4	Shift pressed
 *	5-6	Unused
 *	7	0 if this is a valid struct keypress (a 1 here usually means that there are no
 *		struct keypresses in the output buffer)
 */
struct keypress {
	char c;
	UINT8 info;
};

static inline int is_released(struct keypress kp) {return kp.info & 1;}
static inline int is_caps(struct keypress kp) {return kp.info & (1<<1);}
static inline int is_ctrl(struct keypress kp) {return kp.info & (1<<2);}
static inline int is_alt(struct keypress kp) {return kp.info & (1<<3);}
static inline int is_shift(struct keypress kp) {return kp.info & (1<<4);}
static inline int is_valid(struct keypress kp) {return !(kp.info & (1<<7));}
static inline int is_alpha(char c) {
	return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

static UINT8 inputBuffer[BUF_SIZE];
static UINT16 inw_offset = 0, inr_offset = 0;

static struct keypress outputBuffer[BUF_SIZE];
static UINT16 outw_offset = 0, outr_offset = 0;

static UINT8 caps = 0, ctrl = 0, alt = 0, shift = 0;


static char fromKeycode(UINT8 kc) {

	char c;
	if (shift)
		c = keycodes_shift[kc];
	else
		c = keycodes[kc];
	if (is_alpha(c))
		c += caps*(shift*('a'-'A') + !shift*('A'-'a'));
	return c;
}

static int sendb(UINT8 data) {
	
	UINT8 ready = 1;
	
	while (ready)
		ready = inb(PS2_STATUS_REG) & 2;
	
	outb(PS2_DATA_REG, data);
	return 0;
}

static UINT8 readb() {
	
	UINT8 ready = 0;
	while (!ready)
		ready = inb(PS2_STATUS_REG) & 1;
	
	UINT8 data = inb(PS2_DATA_REG);
	return data;
}

static int check_kb() {
	
	int retval = -1;
	sendb(KB_ECHO);
	UINT8 val = readb();
	if (val == KB_ECHO)
		retval = 0;
	
	return retval;
}

//Process keypress
static void proc_kp() {

	UINT8 complete = 0, release = 0;
	struct keypress kp;
	while (!complete && inr_offset != inw_offset) {
		UINT8 kc = inputBuffer[inr_offset];
		++inr_offset;
		if (inr_offset >= BUF_SIZE)
			inr_offset = 0;

		kp.info = 0;
		if (kc == KEY_ALT) {
			alt = release ? 0 : 1;
		} else if (kc == KEY_CTRL) {
			ctrl = release ? 0 : 1;
		} else if (kc == KEY_SHIFT) {
			shift = release ? 0 : 1;
		} else if(kc == KEY_CAPS_LOCK) {
			caps = release ? 1-caps : caps;
		} else if(kc < 0x70) {
			// Regular character
			kp.c = fromKeycode(kc);
			kp.info = release | caps<<1 | ctrl<<2 | alt<<3 | shift<<4;
			release = 0;
			complete = 1;
		} else if(kc == KEY_RELEASED) {
			release = 1;
		}
	}
	if (complete) {
		outputBuffer[outw_offset] = kp;
		++outw_offset;
		if (outw_offset >= BUF_SIZE)
			outw_offset = 0;
	}
}


__attribute__((interrupt)) static void kb_isr(struct interrupt_frame* frame) {
	
	UINT8 kc = inb(PS2_DATA_REG);
	inputBuffer[inw_offset] = kc;
	++inw_offset;
	if (inw_offset >= BUF_SIZE)
		inw_offset = 0;
	pic_eoi(1);
}

static int init_kb() {

	UINT8 res;
	int retval = -1;
	if (!check_kb()) {
		//Disable PS/2 controller translation
		outb(PS2_STATUS_REG, PS2_READ_CCB);
		res = readb();
		outb(PS2_STATUS_REG, PS2_WRITE_CCB);
		res &= ~(1<<PS2_TRANSLATION_BIT);
		sendb(res);

		sendb(KB_SET_SCAN_CODE);
		sendb(KB_SCAN_CODE_2);
		res = readb();
		if (res == KB_ACK) {
			sendb(KB_SCAN_ENABLE);
			res = readb();
			if (res == KB_ACK) {
				//Setup interrupt controller
				init_pic();
				add_idt_gate(kb_isr, PIC1_IRQ_OFFSET+PIC_KB_IRQ, GATE_TYPE_INT_32);
				umask_pic(PIC_KB_IRQ);
				retval = 0;
			}
		}
	}
	return retval;
}

static UINT8 read_kc() {

	UINT8 kc = 0;
	if (inr_offset != inw_offset) {
		kc = inputBuffer[inr_offset];
		++inr_offset;
		if (inr_offset >= BUF_SIZE)
			inr_offset = 0;
	}
	return kc;
}

static struct keypress read_kb() {

	struct keypress kp;
	kp.info = 1<<7;
	if (outr_offset == outw_offset)
		proc_kp();
	if (outr_offset != outw_offset) {
		kp = outputBuffer[outr_offset];
		++outr_offset;
		if (outr_offset >= BUF_SIZE)
			outr_offset = 0;
	}
	return kp;
}


#endif
