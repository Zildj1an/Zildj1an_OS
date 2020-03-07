/*
	P-VS Lang
*/

#include "macros.h"
#include "io/text.h"
#include "file_system/fs.h"
#include "string.h"
#include "command.h"

#pragma once

/* Returns 0 if it cannot be executed */
int __check_exec(int file) {

        /* First, we should check all the commands are known */

	/* Then, we should enforce the addition of the exit command */

	/* TODO more... */

	return 0;
}

void __read_EXEC(int file, unsigned char* command){

	// TODO
	// Reads the next command in the file
	// Should use a function to read files
}

void __exec_file(int file){

	unsigned char *command;
	int ret;

	/* [1] Check syntax errors and suggest improvements */
	ret = __check_exec(file);

	/* [2] Execute instructions one by one              */
	while (ret){

		/* Retrieve next command in the list */
		__read_EXEC(file,command);

		if (execute_command(command) == EXIT_COMMAND){
        	         break;
        	}
	}
}

void pvs_func(unsigned char *arg){

        unsigned int i = 0, file, find = 0, exec = 0;
        char elem;
        unsigned char msg_err[]  = "File not found in current folder\n";
        unsigned char msg_err2[] = "File is NOT an executable!\n";
        size_t offset,dir;
        size_t fsize = get_arg_sep(i, hierarchy.files[CURR_FOLDER].data,
                                &offset, ",", '\n');

        size_t csize = get_arg(1, arg, &dir);

        while (!find && fsize) {

                elem = hierarchy.files[CURR_FOLDER].data[offset];

                if (elem != '>'){

                        file = (unsigned int) substoi(hierarchy.files[CURR_FOLDER].data+offset);

                        if (equal_str_upto((unsigned char*)hierarchy.files[file].file_name,
                                (unsigned char*)arg+dir, csize) > 0){

                                if (hierarchy.files[file].type == EXEC_FILE)
                                        /* Flag the execution of the file */
                                        exec = 1;
                                else
                                        write_Ons((unsigned char*)msg_err2,RED);

                                find = 1;
                        }
                }
           ++i;
           fsize = get_arg_sep(i, hierarchy.files[CURR_FOLDER].data,
                                &offset, ",", '\n');
        }

        if (!find){
                write_Ons((unsigned char*)msg_err,RED);
        }
        else if (exec) {
                __exec_file(file);
        }
}


