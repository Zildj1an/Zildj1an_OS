/*
	This file should contain all the MACROS and typedefs
	except for
	limited exceptions, such as an array size.
*/

#ifndef _MACROS_H_
#define _MACROS_H_

/* VGA Colors */
#define VGA_ADDRESS 0xB8000
#define BLACK 0
#define BLUE  1
#define GREEN 2
#define RED   4
#define BROWN 6
#define WHITE 15

#define MAX_TEXT 25

typedef int size_t;
typedef unsigned short UINT16;
typedef int COLOR;

/* Return values errors */
#define EINVAL 2


#endif