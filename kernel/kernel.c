/*
    Called by start.asm
    Authors:
    Carlos Bilbao (Zildj1an)
    Pablo Villalobos (pablo-vs)
    2019
*/

#include "text.h"
#include "macros.h"
#include "keyboard.h"
#include "interrupts.h"
#include "command.h"
#include "mem.h"
#include "array.h"

int main_k(void* minfo){

	cleanScreen();
	int ret = init_mem(minfo);
	if (ret != 0) {
		return -1;
	}
	setup_interrupts();

	unsigned char msg[]      = "Welcome to Zildj1an-OS\n";
	unsigned char prompt[]   = "ZildOS >";
	unsigned char exit_msg[] = "Good bye!\n";

	write_O((unsigned char*) &msg, sizeof(msg), RED);

	struct Array *command = array(MAX_COMMAND);
	
	init_commands();
	init_kb();

	while (1) {
		write_O((unsigned char*) &prompt, sizeof(prompt),GREEN);
		read_I(command);
		if(execute_command(command) == EXIT_COMMAND){
			write_O((unsigned char*) &exit_msg, sizeof(exit_msg), BLUE);
			break;
		}
	}

 return 0;
}
