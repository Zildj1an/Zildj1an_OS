/*
 * Manage ISRs
 * Author: Pablo Villalobos (pablo-vs)
 */

#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

#include "../macros.h"

#define GDT_ENTRY_SIZE 8
#define GDT_ENTRIES 3
#define GDT_SIZE (GDT_ENTRIES * GDT_ENTRY_SIZE)

#define IDT_SIZE 256

/**
 * GDT descriptor entry
 */
struct GDTDescr {
	UINT32 base;	// Segment base address
	UINT32 limit;	// Segment max address
	UINT8 access;	// Access byte
};

static UINT8 GDT[GDT_SIZE];

extern void load_gdt(UINT8* GDT, size_t size);
extern void reload_segments();

static int add_gdt_entry(int index, struct GDTDescr *entry) {

	int retval;
	UINT8 *offset;
	if (index >= GDT_ENTRIES) {
		retval = -EINVAL;
	} else {
		offset = GDT + (index*GDT_ENTRY_SIZE);

		offset[0] = entry->limit & 0xFF;
		offset[1] = (entry->limit>>8) & 0xFF;
		offset[6] = (entry->limit>>16) & 0xF;

		offset[2] = entry->base & 0xFF;
		offset[3] = (entry->base>>8) & 0xFF;
		offset[4] = (entry->base>>16) & 0xFF;
		offset[7] = (entry->base>>24) & 0xFF;

		offset[5] = entry->access;

		offset[6] |= GDTD_SEGMENT_GRANULARITY_4KB | GDTD_SEGMENT_SIZE_32;

		retval = 0;
	}
	return retval;
}

static void populate_gdt(void) {

	struct GDTDescr des;
	UINT8 i;
	for(i = 0; i < GDT_SIZE; ++i)
		GDT[i] = 0;
	des.base = 0;
	des.limit = 0xFFFFF;
	des.access = GDTD_CODESEG_ACCESS;
	add_gdt_entry(1, &des);
	des.access = GDTD_DATASEG_ACCESS;
	add_gdt_entry(2, &des);
}

/**
 * IDT descriptor entry
 */
struct IDTDescr {
	UINT16 offset_1;	// offset bits 0..15
	UINT16 selector;	// a code segment selector in GDT or LDT
	UINT8 zero;		// unused, set to 0
	UINT8 type_attr;	// type and attributes
	UINT16 offset_2;	// offset bits 16..31
};

static struct IDTDescr IDT[IDT_SIZE];

extern void load_idt(struct IDTDescr* IDT, size_t size);

struct interrupt_frame;

__attribute__((interrupt)) static void empty_isr(struct interrupt_frame* frame) {
	// Empty
}

static int add_idt_gate(void (*isr)(struct interrupt_frame*),
				UINT16 pos, UINT8 type_attr) {
	int retval = -EINVAL;

	if (pos < IDT_SIZE) {

		struct IDTDescr desc;
		desc.offset_1 = ((UINT32) isr) & 0xFFFF;
		desc.offset_2 = (((UINT32) isr) >> 16) & 0xFFFF;
		desc.zero = 0;
		desc.selector = 0x08;	// Note: currently we only have this code segment
		desc.type_attr = type_attr;

		IDT[pos] = desc;
		retval = 0;
	}
	return retval;
}

static void inline pic_eoi(unsigned char irq) {

	if (irq >= 8)
		outb(PIC2_COMMAND, PIC_EOI);
	outb(PIC1_COMMAND, PIC_EOI);
}

static void inline mask_pic(UINT8 irq) {

	UINT8 cmask;

	if (irq < 8) {
		cmask = inb(PIC1_DATA);
		cmask |= 1<<irq;
		outb(PIC1_DATA, cmask);
	} else {
		cmask = inb(PIC2_DATA);
		cmask |= 1<<irq;
		outb(PIC2_DATA, cmask);
	}
}

static void inline umask_pic(UINT8 irq) {

	UINT8 cmask;

	if(irq < 8) {
		cmask = inb(PIC1_DATA);
		cmask &= ~(1<<irq);
		outb(PIC1_DATA, cmask);
	} else {
		cmask = inb(PIC2_DATA);
		cmask &= ~(1<<irq);
		outb(PIC2_DATA, cmask);
	}
}

static void init_pic(void) {

	//Mask all interrupts
	outb(PIC1_DATA, 0xFF);
	outb(PIC2_DATA, 0xFF);

	//Configure both PICs: initialize and set offset
	outb(PIC1_COMMAND, PIC_INITIALIZE);
	io_wait();
	outb(PIC1_COMMAND, PIC_INITIALIZE);
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

	//Mask again
	outb(PIC1_DATA, 0xFF);
	io_wait();
	outb(PIC2_DATA, 0xFF);
}

static void setup_interrupts(void) {

	unsigned int i;

	// First, load the GDT
	populate_gdt();
	load_gdt(GDT, sizeof(GDT));
	reload_segments();

	// TODO Populate IDT
	for (i = 0; i < 256; ++i)
		add_idt_gate(empty_isr, i, GATE_TYPE_INT_32);

	// Load the IDT (and enable interrupts)
	load_idt(IDT, sizeof(IDT));
}

#endif
