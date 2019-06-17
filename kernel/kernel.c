/*
    Called by start.asm
    Author Carlos Bilbao (Zildj1an) 2019
*/

#include "text.h"
#include "macros.h"

int main_k(void){

	unsigned char msg[] = "Welcome to Zildj1an-OS\n";
	unsigned char prompt[] = "ZildOS >";
	unsigned char command[MAX_COMMAND];

	cleanScreen();

	write_O((unsigned char*) &msg, sizeof(msg), RED);

	while(1){

		write_O((unsigned char*) &prompt, sizeof(prompt),GREEN);
		read_I((unsigned char*) &command);
		//TODO execute
	}

 return 0;
}
