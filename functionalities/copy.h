#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

// cp command ------------------------------------------------------

int my_cp(int argc, char **args){

	if(strcmp(args[1], "-r")==0){

		// printf("Entered in Copy 1\n");
        // copying one directory to other
		char *src = args[2];
		char *dest = args[3];

        // open the source folder
		DIR *src_folder = opendir(src);
		if (src_folder==NULL){
			return 0;
		}

        // read each file using dirent structure
		struct dirent* file;
		// printf("yes\n");
		int ans = 1;

        // getting the destination path
		char dest_path[1000];
		strcpy(dest_path, dest);
		
		if(opendir(args[argc-1])==NULL){
			mkdir(args[argc-1], 0777);
			printf("MAKE DONE\n");
		}
		strcat(dest_path, "/");
		strcat(dest_path, src);
		mkdir(dest_path, 0777);

        // creating the copy of source folder in the destination folder (empty)
		DIR* dest_folder = opendir(dest_path);
		if (dest_folder==NULL){
			return 0;
		}

        // iterate over each file in the source directory
		while(file=readdir(src_folder)){
			// printf("yes yes\n");

            // source and destination file descriptors
			FILE* src_file;
			FILE* dest_file;

            // getting the destination file path
			char inner_path[1000];
			strcpy(inner_path, src);
			strcat(inner_path, "/");
			// printf("%s\n", inner_path);

			char dest_path[1000];
			strcpy(dest_path, dest);        // copying second argument into 1st one
			strcat(dest_path, "/");         // concatenate two strings
			strcat(dest_path, src);

			strcat(dest_path, "/");
			char* fname = file->d_name;     // filename from the structure pointer

            // variables to read line
			char *line_buf = NULL;
			size_t line_buf_size = 0;
			ssize_t line_size;
			
			if (strcmp(fname,".")!=0 && strcmp(fname,"..")!=0){
				// printf("Here\n");
				strcat(inner_path, fname);
				strcat(dest_path, fname);

				int check_dir = isDirectory(inner_path);        // check directory or not
				if (check_dir==0){
                    
                    // if not a directory copy the file
					src_file = fopen(inner_path, "r");
					dest_file = fopen(dest_path, "w");

                    // read line by line
					line_size = getline(&line_buf, &line_buf_size, src_file);
					while(line_size>=0){
						fprintf(dest_file, "%s", line_buf);     // printing content in the destination file
						line_size = getline(&line_buf, &line_buf_size, src_file);       // reading next line
					}
					fclose(src_file);
					fclose(dest_file);
				}
			}
		}
		closedir(dest_folder);
		closedir(src_folder);
		// printf("copy end");
		return 1;
	}
	else if (isDirectory(args[argc-1]) == 1){

		// printf("ENTERED COPY 2\n");
        // copying multiple files into a directory
		int ct;
		ct = 1;
		char *dest = args[argc-1];
		// printf("%s\n",args[argc-1]);

        // folder descriptor
		DIR *folder = opendir(dest);
		if (folder==NULL){
			return 0;
		}
		while(ct < (argc-1)){

            // path for the new file
			char newfile[1000];
			strcpy(newfile, dest);
			strcat(newfile, "/");
			strcat(newfile, args[ct]);
			
            // file descriptors
			FILE* old_file;
			FILE* new_one;

            // open source file to read and destination file to write
			old_file = fopen(args[ct], "r");
			new_one = fopen(newfile, "w");

			char *line_buf = NULL;
			size_t line_buf_size = 0;
			ssize_t line_size;
			
            // read line by line using getline
            line_size = getline(&line_buf, &line_buf_size, old_file);
			while(line_size>=0){
				fprintf(new_one, "%s", line_buf);           // copy the line into the new file
				line_size = getline(&line_buf, &line_buf_size, old_file);       //read the next line
			}
			fclose(old_file);
			fclose(new_one);
            // files closed
			ct+=1;
		}
		closedir(folder);           //close the folder
		return 1;
	}
	else{	
		// printf("ENTERED COPY 3\n");
        // copying one file to another
        FILE* source;
		FILE* dest;

        // line read variables
		char *line_buf = NULL;
		size_t line_buf_size = 0;
		ssize_t line_size;
		
		source = fopen(args[1], "r");
		dest = fopen(args[2], "w");

        // read lines one by one using getline
		line_size = getline(&line_buf, &line_buf_size, source);
		while(line_size>=0){
			fprintf(dest, "%s", line_buf);          // write the content in the new file
            // read next line
			line_size = getline(&line_buf, &line_buf_size, source);
		}
		fclose(source);
		fclose(dest);
		return 1;
	}
}