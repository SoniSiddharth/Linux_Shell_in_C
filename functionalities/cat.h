#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

// cat command ----------------------------------------------------
int my_cat(int argc, char **args){
	int ct = 1;
	while(ct<argc){
		int file_desc;
        int j;
		char buf[2];
        // open the file using open system call
		file_desc = open(args[ct], O_RDONLY, 0777);
		printf("%s:\n", args[ct]);
		if(file_desc == -1){
			printf("file open error");
		}
		else{
            // print each character of the file in the terminal
			while((j = read(file_desc,buf,1))>0){
				printf("%c",buf[0]);
			}
			close(file_desc);
		}
		ct+=1;
		printf("\n\n");
	}
}