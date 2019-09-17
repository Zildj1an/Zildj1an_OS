#ifndef _TEXT_H_
#define _TEXT_H_

#include "../macros.h"
#include "keyboard.h"
#include "io.h"
#include "../array.h"

UINT16* SCREEN_BUFFER;

static int positionX = 0;
static int positionY = 0;

static inline UINT16 VGA_entry(unsigned char msg, COLOR color){

	return (UINT16) msg | (UINT16) color << 8;
}

static inline int invalid_color(COLOR color){

	return (color != BLACK && color != BLUE && color != GREEN && color != RED
	  && color != BROWN && color != WHITE);
}

static void scroll_half() {

	unsigned int i, j;
	for (i = ROW_TEXT/2; i < ROW_TEXT; ++i) {
		for (j = 0; j < COLUMN_TEXT; ++j) {
			SCREEN_BUFFER[j + (i-ROW_TEXT/2)*COLUMN_TEXT] =
				SCREEN_BUFFER[j + i*COLUMN_TEXT];

			SCREEN_BUFFER[j + i*COLUMN_TEXT] = ' ';
		}

	}
	positionY -= ROW_TEXT/2;
}

/* For now we assume stdout as Output */
static int write_O(unsigned char* msg, size_t count, COLOR color) {

	 int ret = 0;
	 int i;

	 SCREEN_BUFFER = (UINT16*) VGA_ADDRESS;

	 if (count <= 0 || !msg || invalid_color(color)) {
		ret = -EINVAL;
		goto end;
	 }

	 for (i = 0; i < count && msg[i] != '\0'; ++i) {

		if(*(msg + i) == '\n') goto line;

		SCREEN_BUFFER[positionX + (positionY * COLUMN_TEXT)] = VGA_entry(*(msg + i), color);

		positionX++;

		if (positionX == COLUMN_TEXT - 1){

line:			positionX = 0;
			positionY = (positionY + 1);
			if (positionY > ROW_TEXT-2)
				scroll_half();
		}
	}

end:
	return ret;
}


static inline char hex_to_char(UINT8 hex) {

	if(hex < 0xA) return '0'+hex;
	else return 'A'-10+hex;
}

static void print_hex(UINT32 kc) {

	// For debugging purposes
	unsigned char buff[9];
	unsigned int i;
	for(i = 0; i < 8; i += 2) {
		UINT8 fh, sh;
		fh = kc & 0xF;
		sh = (kc >> 4) & 0xF;
		kc = kc >> 8;
		buff[8-i-2] = hex_to_char(sh);
		buff[8-i-1] = hex_to_char(fh);
	}
	buff[8] = ' ';
	write_O((unsigned char *)buff, 9, GREEN);
}

static int replace_O(unsigned char* msg, int count, COLOR color) {

	int ret;
	positionY = (ROW_TEXT - 1 + positionY - count/(COLUMN_TEXT)) % (ROW_TEXT-1);
	
	if (positionX < count%(COLUMN_TEXT-1))
		positionY = (ROW_TEXT-1 + positionY - 1)%(ROW_TEXT-1);
	
	positionX = (COLUMN_TEXT-1 + positionX - (count%COLUMN_TEXT)) % (COLUMN_TEXT-1);
	ret = write_O(msg, count, color);

	return ret;
}

static int delete_O(size_t count) {

	int ret,i;
	unsigned char empty[count];

	for (i = 0; i < count; ++i) 
		empty[i] = ' ';

	ret = replace_O((unsigned char *) empty, count, GREEN);
	positionY = (ROW_TEXT-1 + positionY - count/(COLUMN_TEXT-1) ) % (ROW_TEXT-1);

	if (positionX < count) 
		positionY = (ROW_TEXT-1 + positionY - 1)%(ROW_TEXT-1);
	positionX = (COLUMN_TEXT-1 + positionX - count%(COLUMN_TEXT-1) ) % (COLUMN_TEXT-1);

	return ret;
}


static inline void cleanScreen(void){

	unsigned int i = 0;
	unsigned char *msg = (unsigned char*) ' ';
	int max = ROW_TEXT * COLUMN_TEXT;

	for (i = 0; i < max; ++i) 
		write_O((unsigned char*) msg, 1,BLACK);

	positionX = 0;
	positionY = 0;
}

static inline int equal_str(unsigned char *a, unsigned char *b, unsigned int size_a, unsigned int size_b){

	unsigned int i;
	if (size_a != size_b) return -EINVAL;
	for (i = 0; i < size_a && a[i] == b[i] && a[i] != '\0'; ++i){}
	return (i == size_a || (b[i] == '\0' && a[i] == '\0'));
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
 * Returns a pointer to the argnum argument in command, stored in arg,
 * and the size of the argument, or 0 if there are less than argnum arguments
 */
static size_t get_arg(int argnum, unsigned char *command, unsigned char **arg) {

	UINT8 i = 0;
	UINT8 num = -1;
	UINT8 found = 0;
	UINT8 prev = 1;
	size_t size = 0;

	*arg = command;
 	do {
		if (num == argnum && (command[i] == ' ' || command[i] == '\n')) {
			found = 1;
			size = command+i-*arg;
		} else {
			if (command[i] == ' ' && !prev) {
				prev = 1;				
			} else if (command[i] != ' ' && prev) { 
				prev = 0;
				*arg = command+i;
				++num;
			}
		}	
		++i;
	} while (!found && command[i] != '\0');

	return size;
}

static void read_I(struct Array *command){

	//Returns size of command

	int offset = 0;
	unsigned char *buff = (unsigned char *) command->data;
	struct keypress kp;
	
	do {
		kp = read_kb();
		if (is_valid(kp) && !is_released(kp)) {
			
			if (is_ctrl(kp)) {
			//Ctrl+<>
			} else if(is_alt(kp)) {
			//Alt+<>
			} else {
				if (kp.c == '\b') {
					if (offset > 0) {
						delete_O(1); //TODO add Ctr + K (?)
						--offset;
					}
				} else if (offset < MAX_COMMAND - 2 || kp.c == '\n') {
					buff[offset] = kp.c;
					write_O(buff+offset, 1, GREEN);
					++offset;
				}
			}
		}
	} while (kp.c != '\n' || is_released(kp));

	buff[offset] = '\0';
	command->size = offset;

	return;
}

#endif
