#ifndef _TEXT_H_
#define _TEXT_H_

#include "../macros.h"
#include "keyboard.h"
#include "io.h"
#include "../string.h"


UINT16* SCREEN_BUFFER = (UINT16*) VGA_ADDRESS;

static int positionX = 0;
static int positionY = 0;

static inline void init_cursor() {

	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | 10);
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0x80) | 11);
}

static inline void stop_cursor() {

	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

static void update_cursor() {

	UINT16 pos = positionY*COLUMN_TEXT+positionX;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (UINT8) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (UINT8) ((pos >> 8) & 0xFF));
}

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
int __write_O(unsigned char* msg, size_t count, COLOR color) {

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
	update_cursor();
	return ret;
}


/* Wrapper */
int write_O(unsigned char* msg, size_t count, COLOR color){

   unsigned char sep[] = " ";
   int ret;

   if(!positionX)
          __write_O((unsigned char*)sep, 1, color);
   ret =  __write_O(msg, count, color);

   return ret;
}

int write_Ons(unsigned char* msg, COLOR color) {

	int ret;
	size_t count = strlen(msg);
	ret = write_O(msg, count, color);
	return ret;
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

static int replace_Ons(unsigned char* msg, COLOR color) {

	int ret;
	size_t count = strlen(msg);
	ret = replace_O(msg, count, color);
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

	update_cursor();
	return ret;
}

static inline void cleanScreen(void){

	unsigned int i = 0;
	int max = ROW_TEXT * COLUMN_TEXT;

	for (i = 0; i < max; ++i)
		SCREEN_BUFFER[i] = VGA_entry(' ', GREEN);

	positionX = 0;
	positionY = 0;
}

static void read_I(unsigned char *command){

	//Returns size of command

	int offset = 0;
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
					command[offset] = kp.c;
					write_O(command+offset, 1, GREEN);
					++offset;
				}
			}
		}
	} while (kp.c != '\n' || is_released(kp));

	command[offset] = '\0';
	set_len(command, offset);

	return;
}

static inline void indent(unsigned char* msg){

        unsigned char sp[] = " ";
        unsigned int s;

         /* Correct identation */
         for(s = strlen(msg); s < 15; ++s)
                       write_O((unsigned char*)sp,1,RED);
}


#endif
