/*
    Called by start.asm
    Author Carlos Bilbao (Zildj1an) 2019
*/

#include "text.h"
#include "macros.h"
#include "keyboard.h"
#include "interrupts.h"

int main_k(void){
	
	setup_interrupts();

	unsigned char msg[] = "Welcome to Zildj1an-OS\n";
	unsigned char prompt[] = "ZildOS >";
	unsigned char command[MAX_COMMAND];

	cleanScreen();
	write_O((unsigned char*) &msg, sizeof(msg), RED);
	if(init_kb() == 1) {
		return 0;
	}
	while(1){
		write_O((unsigned char*) &prompt, sizeof(prompt),GREEN);
		read_I((unsigned char*) &command);
		//TODO execute
	}

 return 0;
}
