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

void inline echo_func(char *msg){

 	write_O((unsigned char*) &msg, sizeof(msg), RED);
}
void inline exit_func(void *arg){}

/* Fill when new command (Increase NUM_COMMANDS macro and add define)

   Important commands to do:
      0. echo (requires dividing the command in parts @ execute_command)
      1. Man or help for displaying commands description
      2. ls or dir (First we need to take care of the file system)
      3. Providing echo w the possibility to display GLOBAL VARIABLES
      4. Command p-vs will execute scripts in the language p-vslang
         This will be very costly as we need an interpreter of loops and other structures
         but we can start with a basic line-by-line command executor
      5. z-channel -> This is going to be last but fucking amazing.
                        We can implement a socket that -by now- will work assuming
                        all ZildOs users are connected to Eduroam. Probably will be able
                        to see results with this by summer or later
*/
static void init_commands(void){

	strcpy(command_list[EXIT_COMMAND]->name,(unsigned char*)"exit");
	strcpy(command_list[EXIT_COMMAND]->description,(unsigned char*)"Finish the OS session");
	command_list[EXIT_COMMAND]->id = EXIT_COMMAND;
	command_list[EXIT_COMMAND]->function = &exit_func;

        strcpy(command_list[EXIT_COMMAND]->name,(unsigned char*)"echo");
        strcpy(command_list[EXIT_COMMAND]->description,(unsigned char*)"Display a message on screen");
        /* TODO: Echo could also display GLOBAL ENVIRONMENT VARIABLES SO WE CAN DO SCRIPTS */
        command_list[EXIT_COMMAND]->id = ECHO_COMMAND;
        command_list[EXIT_COMMAND]->function = echo_func;
}

static int execute_command(unsigned char* command, void *arg){

    unsigned int i;
    int id = -1;
    unsigned char error_msg[] = "Error with the given command!\n";

    for (i = 0; i < NUM_COMMANDS; ++i){
         size_t sc = sizeof(command), sn = sizeof(command_list[i]->name);
         size_t size = sc > sn ? sn : sc;
	 // TODO divide command and args in char command
         if (equal_str(command,command_list[i]->name, size,size) > 0) {
               id = command_list[i]->id;
               command_list[i]->function(arg);
               i = NUM_COMMANDS;
         }
    }

    if(id == -1) write_O((unsigned char*) &error_msg, sizeof(error_msg),RED);

  return id;
}


#endif
