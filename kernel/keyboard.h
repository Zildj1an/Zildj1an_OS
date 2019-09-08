/*
Basic keyboard driver based on polling
Author: Pablo Villalobos (pablo-vs)
*/

/*
This is barely usable.
Issues:
	1. Does not suport caps, control sequences, etc.
	2. The keyboard layout is wrong.
*/


#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#include "io.h"
#include "interrupts.h"


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

//PIC
#define PIC1		0x20		// IO base address for master PIC
#define PIC2		0xA0		// IO base address for slave PIC
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)
#define PIC_EOI		0x20
#define PIC1_IRQ_OFFSET 0x40 
#define PIC2_IRQ_OFFSET 0x48

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
	0, 0, 0, 0, 0, 0, '\b', 0, 0, 0, 0, 0, 0, 9, '`', 0,	//0x60-0x6F	
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


typedef struct {
	char c;
	UINT8 special;
	UINT8 pressed;
} keypress;

static UINT8 inputBuffer[128];
static UINT16 input_write_offset = 0, input_read_offset = 0;

static keypress outputBuffer[128];
static UINT16 output_write_offset = 0, output_read_offset = 0;

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

static void pic_eoi(unsigned char irq) {
	if(irq >= 8) outb(PIC2_COMMAND, PIC_EOI);
	outb(PIC1_COMMAND, PIC_EOI);
}

static void process_keypress() {	
	UINT8 complete = 0, pending = 0;
	keypress kp;
	while(complete == 0 && input_read_offset != input_write_offset) {
		UINT8 kc = inputBuffer[input_read_offset];
		++input_read_offset;
		if(input_read_offset >= 128) input_read_offset = 0;

		kp.special = 0;
		if(kc < 0x70) {
			kp.c = fromKeycode(kc);
			kp.pressed = pending;
			pending = 0;
			complete = 1;
		}
		else if(kc == 0xF0) {
			pending = 1;
		}
	}
	if(complete != 0) {
		outputBuffer[output_write_offset] = kp;
		++output_write_offset;
		if(output_write_offset >= 128) output_write_offset = 0;
	}
}

__attribute__((interrupt)) static void kb_isr(struct interrupt_frame* frame) {
	UINT8 kc = inb(DATA_REG);
	inputBuffer[input_write_offset] = kc;
	++input_write_offset;
	if(input_write_offset >= 128) input_write_offset = 0;
	pic_eoi(1);
}


static void init_pic() {
	//Mask all interrupts
	outb(PIC1_DATA, 0xFF);
	outb(PIC2_DATA, 0xFF);

	outb(PIC1_COMMAND, 0x11);
	io_wait();
	outb(PIC1_COMMAND, 0x11);
	io_wait();
	outb(PIC1_DATA, PIC1_IRQ_OFFSET);
	io_wait();
	outb(PIC2_DATA, PIC2_IRQ_OFFSET);
	io_wait();
	outb(PIC1_DATA, 4);
	io_wait();
	outb(PIC2_DATA, 2);
	io_wait();
	outb(PIC1_DATA, 1);
	io_wait();
	outb(PIC2_DATA, 1);
	io_wait();

	outb(PIC1_DATA, 0xFF);
	io_wait();
	outb(PIC2_DATA, 0xFF);
}

static void mask_pic(UINT8 irq) {
	if(irq < 8) {
		UINT8 cmask = inb(PIC1_DATA);
		cmask |= 1<<irq;
		outb(PIC1_DATA, cmask);
	} else {
		UINT8 cmask = inb(PIC2_DATA);
		cmask |= 1<<irq;
		outb(PIC2_DATA, cmask);
	}
}

static void umask_pic(UINT8 irq) {
	if(irq < 8) {
		UINT8 cmask = inb(PIC1_DATA);
		cmask &= ~(1<<irq);
		outb(PIC1_DATA, cmask);
	} else {
		UINT8 cmask = inb(PIC2_DATA);
		cmask &= ~(1<<irq);
		outb(PIC2_DATA, cmask);
	}
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
			if(res == KB_ACK) {
				init_pic();
				addIDTEntry(kb_isr, 0x41, 0b10001110);
				umask_pic(1);
				retval = 0;
			}
		}
	}
	return retval;
}

static keypress read_kb() {
	keypress kp;
	kp.special = 0xFF;
	if(output_read_offset == output_write_offset) process_keypress();
	if(output_read_offset != output_write_offset) {
		kp = outputBuffer[output_read_offset];
		++output_read_offset;
		if(output_read_offset >= 128) output_read_offset = 0;
	}
	return kp;
}


#endif
