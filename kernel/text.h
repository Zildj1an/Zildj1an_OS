#ifndef _TEXT_H_
#define _TEXT_H_

#include "macros.h"

UINT16* SCREEN_BUFFER;


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

	 if (count <= 0 || !msg || invalid_color(color)) {
		ret = -EINVAL;
		goto end;
	 }

	 SCREEN_BUFFER = (UINT16*) VGA_ADDRESS;

	 for(i = 0; i < MAX_TEXT && i < count; ++i) {

		 SCREEN_BUFFER[i] = VGA_entry(*(msg + i), color);
	}

end:
	return ret;
}


#endif
