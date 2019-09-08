/*
Manage ISRs
Authos: Pablo Villalobos (pablo-vs)
*/

#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

#include "macros.h"


// TODO: more readable GDT description
UINT8 GDT[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	       0xFF, 0xFF, 0x00, 0x00, 0x00, 0x9A, 0xCF, 0x00,
	       0xFF, 0xFF, 0x00, 0x00, 0x00, 0x92, 0xCF, 0x00};

typedef struct {
   UINT16 offset_1; // offset bits 0..15
   UINT16 selector; // a code segment selector in GDT or LDT
   UINT8 zero;      // unused, set to 0
   UINT8 type_attr; // type and attributes, see below
   UINT16 offset_2; // offset bits 16..31
} IDTDescr;

IDTDescr IDT[256];


extern void load_gdt(UINT8* GDT, size_t size);
extern void reload_segments();
extern void load_idt(IDTDescr* IDT, size_t size);

static void setup_interrupts() {
	

	// First, load the GDT
	load_gdt(GDT, sizeof(GDT));
	reload_segments();

	// TODO Populate IDT

	// Load the IDT (and enable interrupts)
	//load_idt((IDTDescr*) &IDT, sizeof(IDT));	
}


#endif
