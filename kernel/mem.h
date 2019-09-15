/*
 * Extremely basic dynamic memory manager using fixed size allocation
 * and a simple linked list
 * Author: Pablo Villalobos (pablo-vs) 2019
 */

#ifndef _MEM_H
#define _MEM_H

#include "macros.h"
#include "text.h"

#define DMEM_BASE  (void*) 0x01000000
#define DMEM_LIMIT (void*) 0x07EE0000
#define BLOCK_SIZE 1024

static void *head = DMEM_BASE;
static void *limit = DMEM_LIMIT;

static int read_mmap(void *minfo) {

	int ret = -1;
	UINT32 flags = *(UINT32*)minfo;
	UINT32 mmap_length;
	void* mmap_addr;
	if ((flags >> 6) & 1) {
		ret = 0;
		mmap_length = *(UINT32*)(minfo+44);
		mmap_addr = *(void**)(minfo+48);
	}
	return ret;
}

static void print_hex(UINT32 kc);

static int init_mem(void *minfo) {

	unsigned int i;

	

	for (i = 0; DMEM_BASE + i < DMEM_LIMIT - BLOCK_SIZE; i += BLOCK_SIZE) {
		*((UINT8**)DMEM_BASE + i) = DMEM_BASE + i + BLOCK_SIZE;
	}	
	*(void**)(DMEM_BASE + i) = NULL;
	

	return 0;
}



static void *malloc() {

	void * ret;
	if (head == NULL) {
		ret = NULL;
	} else {
		ret = head;
		head = *(void**)head;
	}
	return ret;
}

static void free(void *ptr) {
	
	void *next;
	if (ptr) {
		next = head;
		head = ptr;
		*(void**)head = next;
	}
}


#endif
