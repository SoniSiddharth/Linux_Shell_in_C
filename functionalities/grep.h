#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

// grep command ------------------------------------------------------
int my_grep(int argc, char **args){
	FILE *file;
	// printf("%d\n", argc);
	if (argc==2){
        // if no file is specified
		while(1){
			char content[1000];
            // fgets reads a line from the terminal (input)
			fgets(content, sizeof(content), stdin);
            // strstr searches the first occurence of 2nd argument in 1st one and returns the line followed by it
			char *match = strstr(content, args[1]);
			if (match!=NULL){
				printf("%s", match);
				continue;
			}
			else{
				printf("NO MATCH\n");
				return 1;
			}
		}
		return 1;
	}
	else{
		int ct = 2;
		while(ct<argc){
			printf("%s\n", args[ct]);
			file = fopen(args[ct], "r");        // open the file one by one
            
			char *line_buf = NULL;
  			size_t line_buf_size = 0;
  			ssize_t line_size;
			if (!file){
				fprintf(stderr, "error in opening the file\n");
				return 0;
			}
			else{
                // read each line one by one and compare
				line_size = getline(&line_buf, &line_buf_size, file);
				while(line_size>=0){
					// printf("%s", line_buf);
					char *match = strstr(line_buf, args[1]);        // check the string is found or not
					if (match!=NULL){
						printf("%s", match);        // if found print
					}
                    // read the next line
					line_size = getline(&line_buf, &line_buf_size, file);
				}
				free(line_buf);     // free the line_buf memory
				line_buf = NULL;
				fclose(file);       // close the file
			}
			ct+=1;
			printf("\n");
		}
		return 1;
	}
}