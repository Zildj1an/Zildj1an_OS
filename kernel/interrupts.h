/*
Manage ISRs
Authos: Pablo Villalobos (pablo-vs)
*/

#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

#include "macros.h"

#define IDT_SIZE 256



static int write_O(unsigned char * msg, size_t count, COLOR color);

// TODO: more readable GDT description
static UINT8 GDT[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	       0xFF, 0xFF, 0x00, 0x00, 0x00, 0x9A, 0xCF, 0x00,
	       0xFF, 0xFF, 0x00, 0x00, 0x00, 0x92, 0xCF, 0x00};

typedef struct {
   UINT16 offset_1; // offset bits 0..15
   UINT16 selector; // a code segment selector in GDT or LDT
   UINT8 zero;      // unused, set to 0
   UINT8 type_attr; // type and attributes, see below
   UINT16 offset_2; // offset bits 16..31
} IDTDescr;

static IDTDescr IDT[IDT_SIZE];


extern void load_gdt(UINT8* GDT, size_t size);
extern void reload_segments();
extern void load_idt(IDTDescr* IDT, size_t size);

struct interrupt_frame;


__attribute__((interrupt)) static void empty_isr(struct interrupt_frame* frame) {
	// Empty
}


static void addIDTEntry(void (*isr)(struct interrupt_frame*), UINT16 pos, UINT8 type_attr) {
	if(pos < IDT_SIZE) {
		IDTDescr desc;
		desc.offset_1 = ((UINT32) isr) & 0xFFFF;
		desc.offset_2 = (((UINT32) isr) >> 16) & 0xFFFF;
		desc.zero = 0;
		desc.selector = 0x08;	// Note: currently we only have this code segment
		desc.type_attr = type_attr;

		IDT[pos] = desc;
	}
}

static void setup_interrupts() {

	// First, load the GDT
	load_gdt(GDT, sizeof(GDT));
	reload_segments();

	// TODO Populate IDT
	for(int i = 0; i < 256; ++i)
		addIDTEntry(empty_isr, i, 0b10001111);

	// Load the IDT (and enable interrupts)
	load_idt(IDT, sizeof(IDT));	
}


#endif
