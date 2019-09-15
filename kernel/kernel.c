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

	
	UINT32 flags = *(UINT32*)minfo;

	UINT32 mem_lower = 0;
	UINT32 mem_upper = 0;
	if (flags & 1) {
		mem_lower = *(UINT32*)(minfo+4);
		mem_upper = *(UINT32*)(minfo+8);
	}
	UINT32 mmap_length = 0;
	UINT32* mmap_addr = NULL;
	if ((flags >> 6) & 1) {
		mmap_length = *(UINT32*)(minfo+44);
		mmap_addr = *(UINT32**)(minfo+48);
	}


	setup_interrupts();

	unsigned char msg[]      = "Welcome to Zildj1an-OS\n";
	unsigned char prompt[]   = "ZildOS >";
	unsigned char exit_msg[] = "Good bye!\n";

	cleanScreen();
/*
	print_hex(mem_lower);
	print_hex(mem_upper);	

	print_hex(mmap_length);
	print_hex((void*) main_k);

	int i = 0;
	while(i < mmap_length/4) {
		print_hex(*(mmap_addr+i));
		++i;
		unsigned int size = *(mmap_addr+i);
		print_hex(size);
		print_hex(*(mmap_addr+i+12));
		i += 8+size;
	}
*/
	write_O((unsigned char*) &msg, sizeof(msg), RED);
	init_mem(minfo);
	struct Array *command = array(MAX_COMMAND);
	if(!command) {
		return -1;
	}
	
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
