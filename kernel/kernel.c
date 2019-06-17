/*
    Called by start.asm
    Author Carlos Bilbao (Zildj1an) 2019
*/

#include "text.h"

int main_k(void){

	 SCREEN_BUFFER = (UINT16*) VGA_ADDRESS;

	 SCREEN_BUFFER[0] = VGA_entry('W');
 	 SCREEN_BUFFER[0] = VGA_entry('e');
	 SCREEN_BUFFER[0] = VGA_entry('l');
	 SCREEN_BUFFER[0] = VGA_entry('c');
	 SCREEN_BUFFER[0] = VGA_entry('o');
	 SCREEN_BUFFER[0] = VGA_entry('m');
	 SCREEN_BUFFER[0] = VGA_entry('e');
	 SCREEN_BUFFER[0] = VGA_entry('!');

	while(1){}

 return 0;
}
