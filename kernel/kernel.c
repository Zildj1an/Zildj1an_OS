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

int main_k(void){

	setup_interrupts();

	unsigned char msg[] = "Welcome to Zildj1an-OS\n";
	unsigned char prompt[] = "ZildOS >";
	unsigned char command[MAX_COMMAND];

	cleanScreen();
	write_O((unsigned char*) &msg, sizeof(msg), RED);
        init_commands();
        init_kb();

	while(1){
		write_O((unsigned char*) &prompt, sizeof(prompt),GREEN);
		read_I((unsigned char*) &command);
                if(execute_command((unsigned char*) &command) == EXIT_COMMAND) break;
	}

 return 0;
}
