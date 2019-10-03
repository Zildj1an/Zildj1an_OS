/*
    ###################################
    Called by start.asm
    Proud Authors:
    Carlos Bilbao (Zildj1an)
    Pablo Villalobos (pablo-vs)
    2019
    ##################################
*/

#include "io/text.h"
#include "io/keyboard.h"
#include "io/interrupts.h"
#include "io/mem.h"
#include "file_system/fs.h"
#include "macros.h"
#include "command.h"
#include "string.h"

static void display_prompt(){

        unsigned char prompt[]    = "ZildOS_";
	unsigned char end[] = ">";

	write_O((unsigned char*) &prompt, sizeof(prompt),WHITE);

	if(CURR_FOLDER)
		write_O((unsigned char*)hierarchy.files[CURR_FOLDER].file_name,
			   sizeof(hierarchy.files[CURR_FOLDER].file_name), WHITE);

	write_O((unsigned char*) &end, 1,RED);
}

static void inline welcome_msg(void){

        unsigned char msg[]       = "#######################\n";
        unsigned char welcome[]   = " Welcome to Zildj1an-OS\n";

        write_O((unsigned char*) &msg, sizeof(msg), RED);
        write_O((unsigned char*) &welcome, sizeof(welcome), WHITE);
        write_O((unsigned char*) &msg, sizeof(msg), RED);
}


int main_k(void* minfo){

	unsigned char exit_msg[]  = "Good bye!\n";
	unsigned char error_msg[] = "Memory error, exiting...\n";
        unsigned char *command;

	cleanScreen();
	init_cursor();

	int ret;

	if ((ret = init_mem(minfo)) < 0) {
		write_O((unsigned char*) &error_msg, sizeof(error_msg), RED);
		return -1;
	}

	setup_interrupts();

	command = string(MAX_COMMAND);
	welcome_msg();
	init_commands();
	init_kb();
        init_fs(); 				/* Setup File System */

	while (1) {

                display_prompt();
		read_I(command);

		if (execute_command(command) == EXIT_COMMAND){
			write_O((unsigned char*) &exit_msg, sizeof(exit_msg), BLUE);
			stop_cursor();
			break;
		}
	}

 return 0;
}
