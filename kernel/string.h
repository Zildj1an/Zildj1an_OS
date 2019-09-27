/*
 * Simple string structure
 */

#ifndef _STRING_H
#define _STRING_H

#include "io/mem.h"

#define STRING_MAGIC (UINT8) 0xAF
#define MAX_STRING_SIZE (size_t) 1<<10 //1K

typedef struct {

	UINT8 magic;
	size_t size;
	unsigned char *data;
} String;



static inline String * as_String(unsigned char *str) {
	return (String*)(str - 1 - sizeof(size_t));
}

static unsigned char * as_char_ptr(String *str) {
	return (unsigned char *)(str + 1 + sizeof(size_t));
}

static inline UINT8 is_String(unsigned char *str) {

	return as_String(str)->magic == STRING_MAGIC;
}


// Since malloc returns a fixed-size bloc, just store the data in the block following
// the array struct
static unsigned char *string(size_t size) {

	String *str;

	if (size > BLOCK_SIZE-2 || size > MAX_STRING_SIZE) {
		str = NULL;
	} else {
		str = (String *) malloc();
		if (str) {
			str->magic = STRING_MAGIC;
			str->size = size;
			str->data = (UINT8*)(str + 1 + sizeof(size_t) + sizeof(void*));
			(str->data)[size-1] = '\0';
		}
	}
	return str->data;
}

/*
 * Utility functions. All work for both String and regular null-terminated char array
 */


static size_t strlen(unsigned char * str) {
	
	size_t ret = 0;
	if (is_String(str))
		ret = ((String*)(str - 1 - sizeof(size_t)-sizeof(void*)))->size; 
	else
		for(ret = 1; str[ret] != '\0' && ret < MAX_STRING_SIZE; ++ret);
	return ret;
}

// Change string length
static inline void set_len(unsigned char * str, size_t len) {
	
	if (is_String(str) && len < MAX_STRING_SIZE)
		as_String(str)->size = len;
	str[len] = '\0';
}

static inline char hex_to_char(UINT8 hex) {

	if(hex < 0xA) return '0'+hex;
	else return 'A'-10+hex;
}

static inline int equal_str_size(unsigned char *a, unsigned char *b, size_t size_a, size_t size_b) {
	
	unsigned int i;	

	for (i = 0; i < size_a && i < size_b && a[i] == b[i]
		&& a[i] != '\0' && b[i] != '\0'; ++i){}
	return i;
}

static int equal_str(unsigned char *a, unsigned char *b){

	unsigned int ret = 0, sequal;
	size_t size_a, size_b;

	if (is_String(a) && is_String(b)) {
		size_a = strlen(a);
		size_b = strlen(b);
	} else {
		size_a = MAX_STRING_SIZE;
		size_b = MAX_STRING_SIZE;
	}
	if (size_a == size_b) {
		sequal = equal_str_size(a, b, size_a, size_b);
		ret = (sequal == size_a);
	}
	return ret;
}

static inline int equal_str_upto(unsigned char *a, unsigned char *b, size_t size) {
	
	unsigned int sequal = equal_str_size(a, b, size, size);
	return sequal == size;
}

// Function to implement strcpy() function
unsigned char* strcpy(unsigned char* destination, const unsigned char* source) {

	unsigned char *ptr;

	// return if no memory is allocated to the destination
	if (!destination)
		return NULL;

	ptr = destination;

	while (*source != '\0'){
		*destination = *source;
		destination++;
		source++;
	}

	*destination = '\0';

	return ptr;
}

/**
 * Returns the offset of the argnum argument in command, stored in arg,
 * and the size of the argument, or 0 if there are less than argnum arguments
 */
static size_t get_arg(int argnum, unsigned char *command, size_t *arg) {

	UINT8 i = 0;
	UINT8 num = -1;
	UINT8 found = 0;
	UINT8 prev = 1;
	size_t size = 0;

	*arg = 0;
 	do {
		if (num == argnum && (command[i] == ' ' || command[i] == '\n')) {
			found = 1;
			size = i-*arg;
		} else {
			if (command[i] == ' ' && !prev) {
				prev = 1;
			} else if (command[i] != ' ' && prev) {
				prev = 0;
				*arg = i;
				++num;
			}
		}
		++i;
	} while (!found && command[i] != '\0');

	return size;
}

static inline int is_alpha(char c) {
	return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

static inline int is_digit(char c) {
	return ('0' <= c && c <= '9');
}

#endif
