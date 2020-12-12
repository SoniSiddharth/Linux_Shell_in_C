#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

// function to check for a file
int is_file(const char *path)
{
	int flag = 0;
    struct stat path_nw;
    stat(path, &path_nw);
    flag =  S_ISREG(path_nw.st_mode);
	return flag;
}

// ls command ---------------------------------------------------------------------------------------------

int my_ls(int argc, char **args){
	int pos = 1;
	// printf("%d\n", argc);
	// printf("%s\n", args[0]);
	// printf("%s\n", args[1]);
	if (argc==1){
		DIR *dropen;            // DIR datatype for directory stream
		struct dirent *dir;         // dirent structure stores the names of the files in pointer d_name
		dropen = opendir(".");      // open the directory
		
        if (dropen){
			// printf("p1ls\n");
			while ((dir = readdir(dropen)) != NULL){
				printf("%s   ", dir->d_name);
			}
			closedir(dropen);
			return 1;
		}
	}
	else if (isDirectory(args[argc-1])==1){
		// printf("DIRECTORY\n");
		while(pos<argc){
			DIR *dropen;
			// printf("p2ls\n");
			struct dirent *dir;
			dropen = opendir(args[pos]);
			printf("%s:\n", args[pos]);

            // readdir return a pointer to the structure representing the directory stream
			if (dropen){
				while ((dir = readdir(dropen)) != NULL){
					printf("%s   ", dir->d_name);
				}
				closedir(dropen);
			}
			pos+=1;
			printf("\n");
		}
	}
	else{
		printf("FILES ONLY\n");
		while(pos<argc){
			if (is_file(args[pos])==1){
				printf("%s   ", args[pos]);
			}
			pos+=1;
		}
	}
    return 1;
}
