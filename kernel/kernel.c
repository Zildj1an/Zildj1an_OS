/*
    Called by start.asm
    Author Carlos Bilbao (Zildj1an) 2019
*/

#include "text.h"
#include "macros.h"

int main_k(void){

	unsigned char msg[] = "Welcome to Zildj1an-OS";

	write((unsigned char*)&msg, sizeof(msg));

	while(1){}

 return 0;
}
