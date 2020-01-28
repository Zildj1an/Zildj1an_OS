/*
 * Extremely basic physical memory allocator
 * using a simple linked list.
 * Usage: call init_mem with a pointer to the boot information structure
 * get a pointer to a free frame with malloc(), free the frame with free()
 * Author: Pablo Villalobos (pablo-vs) 2019
 */

#ifndef _MEM_H
#define _MEM_H

#include "../macros.h"
#include "text.h"

static void print_hex(UINT32 k);
int write_O(unsigned char*, size_t, COLOR);
int write_Ons(unsigned char* msg, COLOR color);
int printk(char* msg);
static void print_dec(UINT32 num);

struct boot_info {
	UINT8 mem_size_pr; 	// Memory size present
	UINT8 boot_dev_pr; 	// Boot device present
	UINT8 cmd_pr;		// Cmdline present
	UINT8 mods_pr;		// Mods present
	UINT8 out;			// a.out kernel format
	UINT8 elf;			// ELF kernel format
	UINT8 mmap_pr;			// Memory map present

	UINT32 mem_lower;
	UINT32 mem_upper;
	UINT32 boot_dev;
	UINT32 cmd;
	UINT32 mods_count;
	UINT32 mmap_length;
	UINT32 mmap_addr;
};

void newline() {
    unsigned char nl = '\n';
	write_O(&nl,1,GREEN);
}

static void *dmem_head;
static void *dmem_limit;


/* Find the first usable contiguous memory block
 * larger than MINIMUM_MEM
 */
static int read_mmap(UINT32 mmap_length, UINT8* mmap_addr) {

	newline();

	int ret = -1;
	int entry = 0;
	UINT8 i = 0;
	UINT8 found = 0;
	UINT32 entry_size;

	UINT32 mem_size;
	UINT32 mem_base;
	UINT32 mem_type;

	printk("Entry\t\t| Base\t\t| Size \t| Type\n"); 

	while (i < mmap_length && !found) {
		entry_size = *(UINT32*)(mmap_addr + i);
		mem_base = *(UINT32*)(mmap_addr + i + 4);
		mem_size = *(UINT32*)(mmap_addr + i + 12);
		mem_type = *(UINT32*)(mmap_addr + i + 20);
		printk("Entry "); print_dec(entry);
		printk("\t| "); print_hex(mem_base);
		printk("\t| "); print_hex(mem_size);
		printk("\t| ");

		if (mem_type == 1) printk("Available");
		else if (mem_type == 3) printk("ACPI");
		else if (mem_type == 4) printk("Reserved Hib.");
		else if (mem_type == 5) printk("Failed");
		else printk("Reserved");

		newline();

		if (mem_type == 1 && mem_size > MINIMUM_MEM) {
			ret = 0;
			found = 1;
			dmem_head = (void*)mem_base;
			dmem_limit = (void*)mem_base + mem_size 
						- BLOCK_SIZE;
		}

		i += entry_size + 4;
		++entry;
	}
	return ret;
}

static void read_bootinfo(UINT32 *binfo, struct boot_info *bi) {

	UINT32 flags = *binfo;

	printk("\n\tMultiboot Information Contents\n");

	if (flags & 1) {
		bi->mem_size_pr = 1;
		bi->mem_lower = *(binfo+1);
		bi->mem_upper = *(binfo+2);
		printk("Lower mem size (KB): ");
		print_hex(bi->mem_lower); 
		printk(" \tUpper mem size (KB): ");
		print_hex(bi->mem_upper); 
		newline();
	} else {
		printk("Lower/upper mem info missing.\n");
		bi->mem_size_pr = 0;
	}

	if (flags & 1<<1) {
		bi->boot_dev_pr = 1;
		bi->boot_dev = *(binfo+3);
		printk("Boot device: \t");
		print_hex(bi->boot_dev); 
		newline();
	} else {
		printk("Boot device information missing.\n");
		bi->boot_dev_pr = 0;
	}

	if (flags & 1<<2) {
		bi->cmd_pr = 1;
		bi->cmd = *(binfo+4);
		printk("Command line: \t");
		printk((char*)bi->cmd);
		newline();
	} else {
		bi->cmd_pr = 1;
		printk("Command line information missing.\n");
	}

	if (flags & 1<<3) {
		bi->mods_pr = 1;
		bi->mods_count = *(binfo+5);
		print_hex(bi->mods_count);
		printk("\tmodules loaded.\n");
	} else {
		bi->mods_pr = 0;
		printk("Module information missing.\n");
	}

	if (flags & 1<<4) {
		bi->out = 1;
		printk("a.out kernel image.\n");
	} else {
		bi->out = 0;
	}

	if (flags & 1<<5) {
		bi->elf = 1;
		printk("ELF kernel image.\n");
	} else {
		bi->elf = 0;
	}

	if (flags & 1<<5) {
		bi->mmap_pr = 1;
		bi->mmap_length = *(binfo+11);
		bi->mmap_addr = *(binfo+12);
		printk("Memory map: ");
		read_mmap(bi->mmap_length, (void*) bi->mmap_addr);
	} else {
	}
	

}


/* Check if the kernel image is in the selected memory range
 * and make sure not to overwrite it
 */
static void check_kernel_addr() {

	void* up_diff;
	void* low_diff;

	// Just use the address of this function as a proxy for the load
	// address of the kernel. TODO there must be a better way
	void* kaddr = (void*) check_kernel_addr;

	if (dmem_head < kaddr && kaddr < dmem_limit) {
		up_diff = (void*)(dmem_limit - kaddr);
		low_diff = (void*)(kaddr - dmem_head);
		if (up_diff > low_diff)
			dmem_head = (void*)(kaddr + MEM_SAFE_DISTANCE);
		else
			dmem_limit = (void*)(kaddr - MEM_SAFE_DISTANCE);
	}
}

static int init_mem(void *minfo) {

	struct boot_info bi;
	unsigned int i;
	unsigned int ret = 0; 

	read_bootinfo((UINT32*) minfo, &bi);

	if (ret == 0) {

		check_kernel_addr();

		for (i = 0; dmem_head + i < dmem_limit - BLOCK_SIZE;
						i += BLOCK_SIZE) {

			/* *((UINT8**)dmem_head + i) =
					dmem_head + i + BLOCK_SIZE;*/
		}
		//*(void**)(dmem_head + i) = NULL;
	} 

	return ret;
}



static void *malloc() {

	void * ret;
	if (dmem_head == NULL) {
		ret = NULL;
	} else {
		//Reserve the first 16 bytes for OS purposes
		ret = dmem_head+0x10;
		dmem_head = *(void**)dmem_head;
	}
	return ret;
}

static void free(void *ptr) {

	void *next;
	if (ptr) {
		next = dmem_head;
		dmem_head = ptr-0x10;
		*(void**)dmem_head = next;
	}
}


#endif
