#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

// mv command --------------------------------------------------

int my_mv(int argc, char **args){
	if (isDirectory(args[argc-1]) == 0){
		// we do not have to move files into a directory
        // source and destnation file descriptor
        FILE* source;
		FILE* dest;
		
        // defining variables to read lines from the source file
		char *line_buf = NULL;
		size_t line_buf_size = 0;
		ssize_t line_size;
		
		source = fopen(args[1], "r");       // fopen system call to oprn the source file in read mode
		dest = fopen(args[2], "w");         // fopen system call to oprn the destination file in read mode
		line_size = getline(&line_buf, &line_buf_size, source);     // getline to read the source file
		while(line_size>=0){
			fprintf(dest, "%s", line_buf);      // fprint writes the line read into the destination file
			line_size = getline(&line_buf, &line_buf_size, source);
		}
		remove(args[1]);        // remove the source file
		fclose(source);
		fclose(dest);
        return 1;
	}
	else{
        // opening the destination directory
		char *dest = args[argc-1];
		DIR *folder;
		folder = opendir(dest);
		if(folder==NULL){
			mkdir(dest, 0777);
			printf("MAKE DONE\n");
		}
		int ct = 1;
		while(ct<(argc - 1)){

            // getting path of the destination files
			char new_dir[1000];
			char *curr_path;
			curr_path = getcwd(new_dir, 1000);          // current directory
			strcat(new_dir,"/");	                    // concatenate two strings
			strcat(new_dir,dest);
			strcat(new_dir,"/");
			strcat(new_dir, args[ct]);
			printf("%s\n", new_dir);

            // rename function removes the file from its previous position and makes it at the destination
			if(rename(args[ct], new_dir)!=-1){
				printf("Success\n");
            }
			else{
				printf("Error\n");
                return 0;
            }
			ct+=1;
		}
        // printf("%d\n", ct);
		closedir(folder);
        return 1;
	}
}