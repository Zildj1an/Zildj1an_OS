/*
   Manage on-terminal command execution
   Author Carlos Bilbao (Zildj1an)
*/

#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

const char *COMMAND_LIST[1];

COMMAND_LIST[0] = "EXIT";

static void execute_command(unsigned char* command);

#endif
