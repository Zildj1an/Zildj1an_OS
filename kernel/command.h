/*
   Manages on-terminal command execution
   Author Carlos Bilbao (Zildj1an)
          Pablo Villalobos (pablo-vs)
*/

#ifndef _COMMAND_H
#define _COMMAND_H

#include "macros.h"
#include "io/text.h"
#include "file_system/fs.h"
#include "array.h"

struct command {

	unsigned char name[MAX_COMMAND];
	unsigned char description[MAX_COMMAND];
	unsigned int id;
	void (*function)(struct Array *arg);
};

struct command command_list[NUM_COMMANDS];

void echo_func(struct Array *arg){

	unsigned char *msg;
	size_t csize = get_arg(0, arg->data, &msg);
	size_t argsize;

	if (csize) {
		argsize = arg->size - (msg - (unsigned char *)arg->data) - csize - 1;
		write_O((unsigned char*) msg+csize + 1, argsize, RED);
	}
}

static inline void clear_func(struct Array *arg){
	cleanScreen();
}

/* prototype for asm timer */
int * asm_timer(void);
void time_func(struct Array *arg){
    asm_timer();
}

void go_func(struct Array *arg){

    /* OPCION go back/go top  -> palabras reservadas al crear archivos!!! */
    unsigned int i = 0, find = 0, argsize;
    unsigned char *dir;
    unsigned char err[] = "That is not a valid location!\n";
    char elem;
    size_t csize = get_arg(0, arg->data, &dir);

    argsize = arg->size - (dir - (unsigned char *)arg->data) - csize - 1;

    /* Check desired destination */
    for (;!find &&  i < strlen(hierarchy.files[CURR_FOLDER].data); ++i){

                elem = hierarchy.files[CURR_FOLDER].data[i];

		if (elem != ',' && elem != ' ') {

	             /* TODO problem with this check */
		     if (equal_str((unsigned char*) hierarchy.files[elem - '0'].file_name,
			(unsigned char*)dir+csize + 1, strlen(hierarchy.files[elem - '0'].file_name),
			 argsize) > 0){
				find = 1;
				CURR_FOLDER = elem - '0';
		     }
		}
    }

    if (!find)
          write_O((unsigned char*)err,sizeof(err),RED);
}

void zchannel_func(struct Array *arg){}
void pvs_func(struct Array *arg){}

void ls_func(struct Array *arg){

     unsigned int i = 0;
     char elem;
     unsigned char msg[]    = "\n";
     unsigned char sep[]    = "-      ";
     unsigned char exec[]   = "EXECUTABLE";
     unsigned char text[]   = "TEXT";
     unsigned char folder[] = "FOLDER";


     for (; i < strlen(hierarchy.files[CURR_FOLDER].data) ; ++i){

		 elem = hierarchy.files[CURR_FOLDER].data[i];

		 if (elem != ',' && elem != ' ') {

                 	write_O((unsigned char*)hierarchy.files[elem - '0'].file_name,
				sizeof(hierarchy.files[elem - '0'].file_name),RED);
		        indent(hierarchy.files[elem - '0'].file_name);
			write_O((unsigned char*)sep,sizeof(sep),RED);

			if (hierarchy.files[elem - '0'].type == EXEC_FILE)
				write_O((unsigned char*)exec,sizeof(exec),RED);
			else if (hierarchy.files[elem - '0'].type == TEXT_FILE)
				 write_O((unsigned char*)text,sizeof(text),RED);
			else
				 write_O((unsigned char*)folder,sizeof(folder),RED);

			write_O((unsigned char*)msg,sizeof(msg),RED);
   		 }
    }
}

void info_func(struct Array *arg){

	unsigned char sep[]    = "-      ";
	unsigned char next[]   = "\n";
	unsigned int i = 0;

	for(; i < NUM_COMMANDS; ++i) {

		write_O((unsigned char*)command_list[i].name,sizeof(command_list[i].name),RED);
		indent(command_list[i].name);
		write_O((unsigned char*)sep,sizeof(sep),RED);
		write_O((unsigned char*)command_list[i].description,sizeof(command_list[i].description),RED);
		write_O((unsigned char*)next,sizeof(next),RED);
	}

}


void exit_func(struct Array *arg){}

/* Fill when new command (Increase NUM_COMMANDS macro and add define)

   Important commands to do:
	1. Man or help for displaying commands description
	2. ls or dir (First we need to take care of the file system and managing)
	3. Providing echo w the possibility to display GLOBAL VARIABLES
	4. Command p-vs will execute scripts in the language p-vslang
	This will be very costly as we need an interpreter of loops and other structures
	but we can start with a basic line-by-line command executor
	5. z-channel . This is going to be last but fucking amazing.
			We can implement a socket that -by now- will work assuming
			all ZildOs users are connected to Eduroam. Probably will be able
			to see results within this by summer or later
			Z-CHANNEL COULD BE USED TO EXCHANGE p-vslang FILES
	6. ?
	7. mkdir, rmdir
*/
static void init_commands(void){

	strcpy(command_list[EXIT_COMMAND].name,(unsigned char*)"exit");
	strcpy(command_list[EXIT_COMMAND].description,
			(unsigned char*)"Finish the OS session");
	command_list[EXIT_COMMAND].id = EXIT_COMMAND;
	command_list[EXIT_COMMAND].function = &exit_func;
	/*-------------------------------------------------------------------*/
	strcpy(command_list[CLEAR_COMMAND].name,(unsigned char*)"clear");
	strcpy(command_list[CLEAR_COMMAND].description,
			(unsigned char*)"Clear screen");
	command_list[CLEAR_COMMAND].id = CLEAR_COMMAND;
	command_list[CLEAR_COMMAND].function = &clear_func;
        /*-------------------------------------------------------------------*/
	strcpy(command_list[ECHO_COMMAND].name,(unsigned char*)"echo");
	strcpy(command_list[ECHO_COMMAND].description,
			(unsigned char*)"Display a message on screen");
	// TODO: Echo could also display GLOBAL ENVIRONMENT VARIABLES SO WE CAN DO SCRIPTS
	command_list[ECHO_COMMAND].id = ECHO_COMMAND;
	command_list[ECHO_COMMAND].function = &echo_func;
        /*-------------------------------------------------------------------*/
	strcpy(command_list[INFO_COMMAND].name,(unsigned char*)"info");
	strcpy(command_list[INFO_COMMAND].description,
			(unsigned char*)"Help about commands");
	command_list[INFO_COMMAND].id = INFO_COMMAND;
	command_list[INFO_COMMAND].function = &info_func;
        /*-------------------------------------------------------------------*/
	strcpy(command_list[LS_COMMAND].name,(unsigned char*)"ls");
	strcpy(command_list[LS_COMMAND].description,
			(unsigned char*)"Display folder contents");
	// TODO: File System
	command_list[LS_COMMAND].id = LS_COMMAND;
	command_list[LS_COMMAND].function = &ls_func;
        /*-------------------------------------------------------------------*/
	strcpy(command_list[PVS_COMMAND].name,(unsigned char*)"p-vs");
	strcpy(command_list[PVS_COMMAND].description,(unsigned char*)"Execute scripts in the language p-vslang");
	// TODO: Interpreter
	command_list[PVS_COMMAND].id = PVS_COMMAND;
	command_list[PVS_COMMAND].function = &pvs_func;
        /*-------------------------------------------------------------------*/
	strcpy(command_list[ZCHANNEL_COMMAND].name,(unsigned char*)"zchannel");
        strcpy(command_list[ZCHANNEL_COMMAND].description,(unsigned char*)"Connect to the IRC-channel");
	// TODO: A big todo
	command_list[ZCHANNEL_COMMAND].id = ZCHANNEL_COMMAND;
	command_list[ZCHANNEL_COMMAND].function = &zchannel_func;
        /*-------------------------------------------------------------------*/
	strcpy(command_list[TIME_COMMAND].name,(unsigned char*)"time");
        strcpy(command_list[TIME_COMMAND].description,(unsigned char*)"Display current time");
	command_list[TIME_COMMAND].id = TIME_COMMAND;
	command_list[TIME_COMMAND].function = &time_func;
        /*-------------------------------------------------------------------*/
        strcpy(command_list[GO_COMMAND].name,(unsigned char*)"go");
        strcpy(command_list[GO_COMMAND].description,(unsigned char*)"Change the folder");
        command_list[GO_COMMAND].id = GO_COMMAND;
        command_list[GO_COMMAND].function = &go_func;
        /*-------------------------------------------------------------------*/
}

static int execute_command(struct Array *command){

	unsigned int i;
	int id = -1;
	unsigned char error_msg[] = "Error with the given command!\n";
	unsigned char *cmd;
	unsigned int cmdsize;

	cmdsize = get_arg(0, command->data, &cmd);

	if (cmdsize > 0) {
		for (i = 0; i < NUM_COMMANDS; ++i){;
			if (equal_str(cmd,command_list[i].name, cmdsize,cmdsize) > 0
				&& (cmd[cmdsize] == ' ' || cmd[cmdsize] == '\n')) {

				id = command_list[i].id;
				command_list[i].function(command);
				i = NUM_COMMANDS;
			}
		}
	}

	if(id == -1)
		write_O((unsigned char*) &error_msg, sizeof(error_msg),RED);

	return id;
}


#endif
