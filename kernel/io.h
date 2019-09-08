/*
Basic IO read/write operations
*/

#ifndef _IO_H
#define _IO_H

#include "macros.h"

static inline UINT8 inb(UINT16 port)
{
    UINT8 ret;
    asm volatile ( "inb %1, %0"
                   : "=a"(ret)
                   : "Nd"(port) );
    return ret;
}

static inline void outb(UINT16 port, UINT8 val)
{
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

static inline void io_wait(void)
{
    /* TODO: This is probably fragile. */
    asm volatile ( "jmp 1f\n\t"
                   "1:jmp 2f\n\t"
                   "2:" );
}

#endif
