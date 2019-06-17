/*
    Called by start.asm
    Author Carlos Bilbao (Zildj1an) 2019
*/

#include "text.h"

int main_k(void){

	 SCREEN_BUFFER = (UINT16*) VGA_ADDRESS;

	 SCREEN_BUFFER[0] = VGA_entry('W');
 	 SCREEN_BUFFER[1] = VGA_entry('e');
	 SCREEN_BUFFER[2] = VGA_entry('l');
	 SCREEN_BUFFER[3] = VGA_entry('c');
	 SCREEN_BUFFER[4] = VGA_entry('o');
	 SCREEN_BUFFER[5] = VGA_entry('m');
	 SCREEN_BUFFER[6] = VGA_entry('e');
	 SCREEN_BUFFER[7] = VGA_entry('!');

	while(1){}

 return 0;
}
