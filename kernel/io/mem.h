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

static void print_hex(UINT32 h);

static void *dmem_head;
static void *dmem_limit;

/* Find the first usable contiguous memory block
 * larger than MINIMUM_MEM
 */
static int read_mmap(void *minfo) {

	int ret = -1;
	UINT8 i = 0;
	UINT8 found = 0;
	UINT32 entry_size;

	UINT32 mem_size;
	UINT32 mem_base;
	UINT32 mem_type;

	UINT32 flags = *(UINT32*)minfo;
	UINT32 mmap_length;
	UINT8* mmap_addr;

	if ((flags >> 6) & 1) {
		mmap_length = *(UINT32*)(minfo+44);
		mmap_addr = *(UINT8**)(minfo+48);

		while (i < mmap_length && !found) {
			entry_size = *(UINT32*)(mmap_addr + i);
			mem_base = *(UINT32*)(mmap_addr + i + 4);
			mem_size = *(UINT32*)(mmap_addr + i + 12);
			mem_type = *(UINT32*)(mmap_addr + i + 20);

			if (mem_type == 1 && mem_size > MINIMUM_MEM) {
				ret = 0;
				found = 1;
				dmem_head = (void*)mem_base;
				dmem_limit = (void*)mem_base + mem_size - BLOCK_SIZE;
			}

			i += entry_size + 4;
		}
	}
	return ret;
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

	unsigned int i;
	unsigned int ret = read_mmap(minfo);

	if (ret == 0) {

		check_kernel_addr();

		for (i = 0; dmem_head + i < dmem_limit - BLOCK_SIZE; i += BLOCK_SIZE) {
			*((UINT8**)dmem_head + i) = dmem_head + i + BLOCK_SIZE;
		}
		*(void**)(dmem_head + i) = NULL;
	} 

	return ret;
}



static void *malloc() {

	void * ret;
	if (dmem_head == NULL) {
		ret = NULL;
	} else {
		ret = dmem_head+0x10; //Reserve the first 16 bytes for OS purposes
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
