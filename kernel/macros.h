/*
	This file should contain all the MACROS and typedefs
	except for
	limited exceptions, such as an array size.
*/

#ifndef _MACROS_H_
#define _MACROS_H_

#define NULL 0

/* VGA Colors */
#define VGA_ADDRESS 0xB8000
#define BLACK 0
#define BLUE  1
#define GREEN 2
#define RED   4
#define BROWN 6
#define WHITE 15

#define ROW_TEXT 25
#define COLUMN_TEXT 80

/* Interrupts */
#define GDTD_SEGMENT_GRANULARITY_4KB 1<<7
#define GDTD_SEGMENT_SIZE_32 1<<6
#define GDTD_CODESEG_ACCESS 0x9A
#define GDTD_DATASEG_ACCESS 0x92

#define GATE_TYPE_INT_32 0b10001111

#define PIC1		0x20
#define PIC2		0xA0
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)
#define PIC_EOI		0x20
#define PIC1_IRQ_OFFSET 0x40 
#define PIC2_IRQ_OFFSET 0x48
#define PIC_INITIALIZE 0x11
#define PIC_KB_IRQ 1

/* Keyboard */
#define PS2_STATUS_REG (UINT16) 0x64
#define PS2_DATA_REG (UINT16) 0x60
#define PS2_READ_CCB 0x20
#define PS2_WRITE_CCB 0x60
#define PS2_TRANSLATION_BIT 6

#define KB_ACK (UINT8) 0xFA
#define KB_ECHO (UINT8) 0xEE
#define KB_SCAN_ENABLE (UINT8) 0xF4
#define KB_SET_SCAN_CODE (UINT8) 0xF0
#define KB_SCAN_CODE_2 (UINT8) 0x02


typedef int size_t;
typedef unsigned short UINT16;
typedef unsigned char UINT8;
typedef unsigned int UINT32;
typedef int COLOR;

/* Return values errors */
#define EINVAL 2

/* Commands*/
#define MAX_COMMAND      30
#define NUM_COMMANDS     6
#define EXIT_COMMAND     0
#define ECHO_COMMAND     1
#define MAN_COMMAND      2
#define LS_COMMAND       3
#define PVS_COMMAND      4
#define ZCHANNEL_COMMAND 5


#endif
