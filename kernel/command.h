/*
   Manages on-terminal command execution
   Author Carlos Bilbao (Zildj1an)
          Pablo Villalobos (pablo-vs)
*/

#ifndef _COMMAND_H
#define _COMMAND_H

#include "macros.h"
#include "text.h"

struct command {

   unsigned char name[MAX_COMMAND];
   unsigned char description[MAX_COMMAND];
   unsigned int id;
   void (*function)(void *arg);
};

struct command *command_list[NUM_COMMANDS];

void exit_func(void){}

/* Fill when new command (Increase NUM_COMMANDS macro and add define) */
static void init_commands(void){

	strcpy(command_list[EXIT_COMMAND]->name,(unsigned char*)"EXIT");
    	strcpy(command_list[EXIT_COMMAND]->description,(unsigned char*)"Finish the OS session");
	command_list[EXIT_COMMAND]->id = EXIT_COMMAND;
        command_list[EXIT_COMMAND]->function = &exit_func();
}

static int execute_command(unsigned char* command, void *arg){

    unsigned int i;
    int id = -1;
    unsigned char error_msg[] = "Error with the given command!\n";

    for (i = 0; i < NUM_COMMANDS; ++i){

         if (equal_str(command,command_list[i]->name, sizeof(command),sizeof(command_list[i]->name)) > 0) {

               id = command_list[i]->id;
               command_list[i]->function(arg);
               i = NUM_COMMANDS;
         }
    }

    if(id == -1) write_O((unsigned char*) &error_msg, sizeof(error_msg),RED);

  return id;
}


#endif
