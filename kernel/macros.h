/*
	This file should contain all the MACROS and typedefs
	except for
	limited exceptions, such as an array size.
*/

#ifndef _MACROS_H_
#define _MACROS_H_

typedef int size_t;
typedef unsigned short UINT16;
typedef unsigned char UINT8;
typedef unsigned int UINT32;
typedef unsigned long UINT64;
typedef int COLOR;

static int CURR_FOLDER = 0;

#define NULL 0
#define INVALID_UINT 0xFFFFFFFF
#define INVALID_INT 0x7FFFFFFF

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

/* Memory */
#define BLOCK_SIZE 1024
#define MINIMUM_MEM (UINT32)(1<<24) //16 MiB
#define MEM_SAFE_DISTANCE (UINT32)(1<<20) //1MiB


/* Return values errors */
#define EINVAL 2

/* Commands*/
#define MAX_COMMAND      60
#define NUM_COMMANDS     9
#define EXIT_COMMAND     0
#define ECHO_COMMAND     1
#define INFO_COMMAND     2
#define LS_COMMAND       3
#define PVS_COMMAND      4
#define ZCHANNEL_COMMAND 5
#define CLEAR_COMMAND    6
#define TIME_COMMAND     7
#define GO_COMMAND       8

/* Files */
#define MAX_FILE_NAME    10
#define MAX_FILE_SIZE    30
#define NUM_FILES        20
#define EXEC_FILE        0
#define TEXT_FILE        1
#define FOLDER_FILE      2
#define PRIVATE_FILE     0
#define PUBLIC_FILE      1

#endif
