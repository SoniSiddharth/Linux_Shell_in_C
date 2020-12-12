#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

// mkdir command ----------------------------------------------------------------------------

int my_mkdir(char* name_dir){
    int completion_flag;
	completion_flag = mkdir(name_dir, 0777);        // system call mkdir to create a directory
    if(completion_flag == -1)
    {
        printf("Error in creating the directory");
		return 0;
    }
	return 1;
}