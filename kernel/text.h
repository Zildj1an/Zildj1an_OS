#ifndef _TEXT_H_
#define _TEXT_H_

#include "macros.h"

UINT16* SCREEN_BUFFER;


static inline UINT16 VGA_entry(unsigned char msg){

	return (UINT16) msg | (UINT16) WHITE << 8;
}

/* For now we assume stdout */
static int write(unsigned char* msg, size_t count) {

	 int ret = 0;
	 int i;

	 if (count <= 0 || !msg) {
		ret = -EINVAL;
		goto end;
	 }

	 SCREEN_BUFFER = (UINT16*) VGA_ADDRESS;

	 for(i = 0; i < MAX_TEXT && i < count; ++i) {

		 SCREEN_BUFFER[i] = VGA_entry(*(msg + i));
	}

end:
	return ret;
}


#endif
