#ifndef _TEXT_H_
#define _TEXT_H_

#include "macros.h"

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


/* For now we assume stdout */
static int write(unsigned char* msg, size_t count, COLOR color) {

	 int ret = 0;
	 int i;

	 SCREEN_BUFFER = (UINT16*) VGA_ADDRESS;

	 if (count <= 0 || !msg || invalid_color(color)) {
		ret = -EINVAL;
		goto end;
	 }

	 for(i = 0; i < count; ++i) {

		SCREEN_BUFFER[positionX + (positionY * COLUMN_TEXT)] = VGA_entry(*(msg + i), color);

		positionX++;

		if (positionX == COLUMN_TEXT - 1){

			positionX = 0;
			positionY = (positionY + 1) % (ROW_TEXT - 1);
		}
	}

end:
	return ret;
}

static inline void cleanScreen(void){

	int i = 0;
	unsigned char msg = ' ';
	int max = ROW_TEXT * COLUMN_TEXT;

	for (i = 0; i < max; ++i) {

		write((unsigned char*) msg, 1,BLACK);
	}

	positionX = 0;
	positionY = 0;
}


#endif
