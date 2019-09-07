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

#endif
