#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

// rm command ------------------------------------------------------------------------------

// function to check a path is a file or not
int isFile(char *path){
	struct stat stats;
    stat(path, &stats);

    // Check for file existence
    if (stats.st_mode & F_OK)
        return 1;

    return 0;
}

// recursive function to remove directories
// change the path variable when we enter into the child directory
int recursive_remove(char* path){
    //printf("path: %s\n",path);

    // open the folder needs to be deleted
    DIR* folder;
    folder = opendir(path);
    if (folder==NULL){
        return 0;
    }

    // using dirent structure to read its files
    struct dirent* file_desc;
    //printf("Hello1\n");
    int ans = 1;

    while(file_desc = readdir(folder)){
        // printf("Hello2\n");

		char inner_path[1000];          // inner directory path
		strcpy(inner_path, path);       // copying path into inner_path
		//printf("%s\n", inner_path);     
		strcat(inner_path, "/");        // strcat concatenates two strings and stores the return value into the 1st argument
        
        char* fname = file_desc->d_name;    // filename is fname
        if (strcmp(fname,".")!=0 && strcmp(fname,"..")!=0){
			strcat(inner_path, fname);

			int check_dir = isDirectory(inner_path);        // checking a directory or file
			int check_file;
			if (check_dir==0){
				check_file = 1;
			}
			else{
				check_file = 0;
			}

            // printf("inner_path %s\n",inner_path);
            // printf("check_file %d\n",check_file);
            if (check_file){
                // if it is a file, remove it by using remove function
				//printf("file removed\n");
                remove(inner_path);
            }
            else if(check_dir){
				// printf("here id directory\n");
                // recurse for the directory
                if (recursive_remove(inner_path)==0){
                    ans = 0;
                }
            }
        }
    }
    closedir(folder);
    // printf("%s\n", path);
    printf("DONE\n");
    remove(path);   // atlast removing the empty directory
    return ans;
}


// remove main function -----------------

int my_rm(int argc, char **args){
	// printf("%s", args[1]);
	if (strcmp(args[1],"-r")==0){
        
        // if it is a directory to be removed
		char current_d[1024];
		getcwd(current_d, sizeof(current_d));       // getting path of current directory
		strcat(current_d,"/");
		strcat(current_d, args[2]);                 // getting path of removing directory
		// printf("%s\n", current_d);
		
        int rm_status;
		rm_status = recursive_remove(current_d);        // recursive function call 
		if (rm_status==-1){
			fprintf(stderr, "Cannot be removed\n");
			return 0;
		}
		return 1;
	}
	else{
        // if it is only a file
        int ct = 1;
        while(ct<argc){
            char current_d[1024];
            // getting path
            getcwd(current_d, sizeof(current_d));
            strcat(current_d,"/");
            strcat(current_d, args[ct]);
            // printf("%s\n", current_d);

            // remove using remove function
            int rm_status;
            rm_status = remove(current_d);
            if (rm_status==-1){
                printf("%s Cannot be removed\n", args[ct]);
            }
            else{
                printf("%s removed\n", args[ct]);
            }
            ct+=1;
        }
        return 1;
	}
}
