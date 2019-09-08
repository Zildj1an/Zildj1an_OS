/*
Manage ISRs
Authos: Pablo Villalobos (pablo-vs)
*/

#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

extern setup_gdt();
extern reload_segments();
extern setup_idt();


// TODO: more readable GDT description
UINT8 GDT[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	       0xFF, 0xFF, 0x00, 0x00, 0x00, 0x9A, 0xCF, 0x00,
	       0xFF, 0xFF, 0x00, 0x00, 0x00, 0x92, 0xCF, 0x00};



struct IDTDescr {
   uint16_t offset_1; // offset bits 0..15
   uint16_t selector; // a code segment selector in GDT or LDT
   uint8_t zero;      // unused, set to 0
   uint8_t type_attr; // type and attributes, see below
   uint16_t offset_2; // offset bits 16..31
};

IDTDescr IDT[256];




static void setup_interrupts() {
	// First, load the GDT
	setup_gdt(&GDT, sizeof(GDT));
	reload_segments();

	// TODO Populate IDT

	// Load the IDT (and enable interrupts)
	setup_idt(&IDT, sizeof(IDT));	
}


#endif
