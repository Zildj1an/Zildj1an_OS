/*
    Called by start.asm
    Authors:
    Carlos Bilbao (Zildj1an)
    Pablo Villalobos (pablo-vs)
    2019
*/

#include "io/text.h"
#include "io/keyboard.h"
#include "io/interrupts.h"
#include "io/mem.h"
#include "file_system/fs.h"
#include "macros.h"
#include "command.h"
#include "array.h"

int main_k(void* minfo){

	unsigned char msg[]      = "Welcome to Zildj1an-OS\n################\n";
	unsigned char prompt[]   = "ZildOS >";
	unsigned char exit_msg[] = "Good bye!\n";
	unsigned char error_msg[] = "Memory error, exiting...\n";

	cleanScreen();

	int ret = init_mem(minfo);

	if (ret < 0) {
		write_O((unsigned char*) &error_msg, sizeof(error_msg), RED);
		return -1;
	}

	setup_interrupts();

	write_O((unsigned char*) &msg, sizeof(msg), RED);

	struct Array *command = array(MAX_COMMAND);

	init_commands();
	init_kb();
        init_fs(); 				/* Inite File System */

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
