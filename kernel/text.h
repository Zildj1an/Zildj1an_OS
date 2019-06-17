#ifndef _TEXT_H_
#define _TEXT_H_

/* VGA Colors */
#define VGA_ADDRESS 0xB8000
#define BLACK 0
#define BLUE  1
#define GREEN 2
#define RED   4
#define BROWN 6
#define WHITE 15

typedef unsigned short UINT16;

UINT16* SCREEN_BUFFER;

static UINT16 VGA_entry(unsigned char msg){

	return (UINT16) msg | (UINT16) WHITE << 8;
}



#endif
