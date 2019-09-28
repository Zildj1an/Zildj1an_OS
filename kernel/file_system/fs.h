#ifndef _FS_H_
#define _FS_H_

#include "../macros.h"
#include "../io/text.h"

struct file_str {

        /* EXEC_FILE,TEXT_FILE,FOLDER_FILE*/
        unsigned int type;
        unsigned char file_name[MAX_FILE_NAME];
        unsigned int days_from_1970;
        unsigned int sudo;
        unsigned int file_descriptor;
        /* PRIVATE_FILE, PUBLIC_FILE */
        unsigned char data[MAX_FILE_SIZE];
};

struct File_System {

       unsigned int bitmap[NUM_FILES];
       struct file_str files[NUM_FILES];
};

static struct File_System hierarchy;

static int create_File(unsigned char *name, unsigned int secured, unsigned int type){

      unsigned int i = 0;

      for (; i < NUM_FILES && hierarchy.bitmap[i];++i){}

      if (i == NUM_FILES)
                  return -1;

      hierarchy.bitmap[i] = 1;
      hierarchy.files[i].type = type;
      hierarchy.files[i].sudo = secured;
      //TODO 1970
      strcpy(hierarchy.files[i].file_name, name);
      hierarchy.files[i].data[0] = '\0';

      return i;
}

static void inline init_hierarchy(void){

     unsigned int i = 0;

     for (; i < NUM_FILES; i++){
		hierarchy.bitmap[i] = 0;
     		hierarchy.files[i].file_descriptor = 0;
     }
}

//TODO add folder
static int inline fill_folder(int pos, unsigned char* files){

     unsigned int ret = 0, fpos;
     size_t fend, fsize, offset, i = 0, wend;

     /* There is not such folder (TODO msg)*/
     if (!hierarchy.bitmap[pos] || hierarchy.files[pos].type != FOLDER_FILE)
          return -1;

     fend = strlen(hierarchy.files[pos].data)-1;
     if (fend + strlen(files) + 1 > MAX_FILE_SIZE)
          return -1;      

     if (fend > 0) {
          hierarchy.files[pos].data[fend] = ',';     
          fend += 1;
     }
     strcpy(hierarchy.files[pos].data+fend, files);

     fsize = get_arg_sep(i, files, &offset, ",", '\n');
     while (fsize > 0) {
          
          fpos = (unsigned int) substoi(files+offset);
          
          if (hierarchy.bitmap[fpos] && hierarchy.files[fpos].type == FOLDER_FILE) {
               hierarchy.files[fpos].data[0] = '>';
               wend = itos(pos,hierarchy.files[fpos].data+1);
               hierarchy.files[fpos].data[wend+1] = '\n';
	  }
          ++i;
          fsize = get_arg_sep(i, files, &offset, ",", '\n');
     } 

     return ret;
}

static int init_fs(void){

       unsigned char root_folder[] = "Root Folder";
       unsigned char home_folder[] = "home";
       unsigned char sys_folder[]  = "sys";
       unsigned char files[] = "1,2\n";
       int ret;

       /* The initial position is CURR_FOLDER = 0 */

       init_hierarchy();

       if ((ret = create_File((unsigned char*)root_folder,PUBLIC_FILE,FOLDER_FILE) < 0) ||
	   (ret = create_File((unsigned char*)home_folder,PUBLIC_FILE,FOLDER_FILE) < 0) ||
           (ret = create_File((unsigned char*)sys_folder,PUBLIC_FILE,FOLDER_FILE) < 0))
                return ret;

       if ((ret = fill_folder(0,(unsigned char*)files)) < 0)
           return ret;

       /* TODO create files README, ENV and some pvslang script (when command implemented) */

       return ret;
}

#endif
