#ifndef _TEXT_H_
#define _TEXT_H_

#define VGA_ADDRESS 0xB800
#define WHITE 15

typedef unsigned short UINT16;

UINT16* SCREEN_BUFFER;

static UINT16 VGA_entry(unsigned char msg){
	return (UINT16) msg | (UINT16)WHITE << 8;
}



#endif
