/*
 * Simple array structure
 */

#ifndef _ARRAY_H
#define _ARRAY_H

#include "mem.h"

struct Array {
	
	size_t size;
	void *data;
};

// Since malloc returns a fixed-size bloc, just store the data in the block following
// the array struct
struct Array *array(size_t size) {

	struct Array *str;
	
	if (size > BLOCK_SIZE-2) {
		str = NULL;
	} else {	
		str = (struct Array *) malloc();
		if (str) {
			str->size = size;
			str->data = (UINT8*)(str+9);
		}
	}
	return str;
}


#endif
