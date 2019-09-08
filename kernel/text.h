#ifndef _TEXT_H_
#define _TEXT_H_

#include "macros.h"
#include "keyboard.h"
#include "io.h"


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


/* For now we assume stdout as Output */
static int write_O(unsigned char* msg, size_t count, COLOR color) {

	 int ret = 0;
	 int i;

	 SCREEN_BUFFER = (UINT16*) VGA_ADDRESS;

	 if (count <= 0 || !msg || invalid_color(color)) {
		ret = -EINVAL;
		goto end;
	 }

	 for(i = 0; i < count; ++i) {

		if(*(msg + i) == '\n') goto line;

		SCREEN_BUFFER[positionX + (positionY * COLUMN_TEXT)] = VGA_entry(*(msg + i), color);

		positionX++;

		if (positionX == COLUMN_TEXT - 1){

line:			positionX = 0;
			positionY = (positionY + 1) % (ROW_TEXT - 1);
		}
	}

end:
	return ret;
}

static int replace_O(unsigned char* msg, size_t count, COLOR color) {
	positionY = (ROW_TEXT-1 + positionY - count/(COLUMN_TEXT-1) ) % (ROW_TEXT-1);
	if(positionX < count) positionY = (ROW_TEXT-1 + positionY - 1)%(ROW_TEXT-1);
	positionX = (COLUMN_TEXT-1 + positionX - count%(COLUMN_TEXT-1) ) % (COLUMN_TEXT-1);
	return write_O(msg, count, color);
}

static int delete_O(size_t count) {
	unsigned char empty[count];
	for(int i = 0; i < count; ++i) empty[i] = ' ';
	int ret = replace_O((unsigned char *) empty, count, GREEN);
	positionY = (ROW_TEXT-1 + positionY - count/(COLUMN_TEXT-1) ) % (ROW_TEXT-1);
	if(positionX < count) positionY = (ROW_TEXT-1 + positionY - 1)%(ROW_TEXT-1);
	positionX = (COLUMN_TEXT-1 + positionX - count%(COLUMN_TEXT-1) ) % (COLUMN_TEXT-1);
	return ret;
}

static inline void cleanScreen(void){

	int i = 0;
	unsigned char *msg = (unsigned char*) ' ';
	int max = ROW_TEXT * COLUMN_TEXT;

	for (i = 0; i < max; ++i) {

		write_O((unsigned char*) msg, 1,BLACK);
	}

	positionX = 0;
	positionY = 0;
}

static void intToStr(int num, unsigned char *str) {
	int offset = 0, mult = 1000000;
	while(mult > 0) {
		*(str+offset) = (char) '0' + (num/mult % 10);
		mult = mult / 10;
		offset = offset + 1;
	}
	*(str+offset) = '\n';
}

static void printInt(UINT8 num) {
	unsigned char buf[8];
	intToStr(num, (unsigned char*) &buf);
	write_O((unsigned char*) &buf, 8, GREEN);
}

static void read_I(unsigned char *command){	
	int offset = 0;
	keypress kp;
	do {	
		kp = read_kb();
		if(kp.special != 0xFF && kp.pressed == 0) {
			if(kp.c == '\b') {	
				delete_O(1);
				if(offset > 0) --offset;
			}
			else {
				command[offset] = kp.c;
				write_O(command+offset, 1, GREEN);
				++offset;
			}
		}
	} while((kp.c != '\n' || kp.pressed == 1) && offset < MAX_COMMAND);
	if(*(command+offset-1) != '\n') {
		char c = '\n';
		write_O((unsigned char*)&c, 1, GREEN);
	}
	return;
}

#endif
