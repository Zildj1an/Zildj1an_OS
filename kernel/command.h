/*
   Manage on-terminal command execution
   Author Carlos Bilbao (Zildj1an)
*/

#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

#include "macros.h"

const char *COMMAND_LIST[1];

COMMAND_LIST[EXIT_COMMAND] = "EXIT";

static int execute_command(unsigned char* command);

#endif
