#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

#include "./functionalities/pwd.h"
#include "./functionalities/cd.h"
#include "./functionalities/ls.h"
#include "./functionalities/mkdir.h"
#include "./functionalities/chmod.h"
#include "./functionalities/remove.h"
#include "./functionalities/mv.h"
#include "./functionalities/cat.h"
#include "./functionalities/grep.h"
#include "./functionalities/copy.h"

#define SIZE 64

// a list of commands implemented
char *builtin_commands[] = {"ls", "cd", "cat", "pwd", "mv", "grep", "mkdir", "chmod", "rm", "cp"};
int builtin_num = sizeof(builtin_commands) / sizeof(char*);         // size of above list

// function to check whether the command belongs to the builtin commands of the file or not
int builtin_command_check(int argc, char **args){
	int j;
	if (args[0]==NULL){
		return 0;
	}
	else{
		for (j = 0;j < builtin_num; j++){
            // strcmp compares the strings and if equal returns 0
			if (strcmp( args[0], builtin_commands[j])==0){
				return 1;
			}
		}
	}
	return 0;
}

// reading through the command line
char *read_command_line(void)
{
	char *line = NULL;          // string pointer having line
	ssize_t bufsize = 0;        // getline allocates a buffer
    ssize_t line_size;

    // getline returns the number of characters read with return type of ssize_t
    line_size = getline(&line, &bufsize, stdin);
	if (line_size <= 0){
        return EXIT_SUCCESS; 
	}
    // printf("READED\n");
	return line;
}

// function 
char **space_separation(char *line){

	char **comp_args = (char **)malloc(sizeof(char *) * SIZE);     // stores the argument (array of arguments)
	char *word;            // pointer to each argument
	char delim[10] = " \t\n\r\a";       // delimiter to break the argument
	int pos = 0;
    int bufsize = SIZE;
	if (!comp_args)
	{
		printf("allocation error\n");
		exit(EXIT_FAILURE);
	}

	word = strtok(line, delim);        // breaks the string into words at delimiter
	while (word != NULL){
		comp_args[pos] = word;            // 1st argument at posotion 0
		pos ++;

        // if the length exceeds reallot a larger amount of memory to buffer
		if (pos >= bufsize)
		{
			bufsize += SIZE;
			line = realloc(line, bufsize*sizeof(char *));
			if (!line){
                printf("Allocation error\n");
                exit(EXIT_FAILURE);
			}
		}
		word = strtok(NULL, delim);        // read the next word
	}

	comp_args[pos] = NULL;         // assign the last value as NULL
	// printf("%d", pos);
	return comp_args;
}

// to count the number of arguments 
// repeating the same process but returning differnt variable

int arg_count(char *line){

	char **comp_args = (char **)malloc(sizeof(char *) * SIZE);     // stores the argument (array of arguments)
	char *word;            // pointer to each argument
	char delim[10] = " \t\n\r\a";       // delimiter to break the argument
	int pos = 0;
    int bufsize = SIZE;
	if (!comp_args)
	{
		printf("allocation error\n");
		exit(EXIT_FAILURE);
	}

	word = strtok(line, delim);        // breaks the string into comp_args at delimiter
	while (word != NULL)
	{
		comp_args[pos] = word;            // 1st argument at posotion 0
		pos ++;

        // if the length exceeds reallot a larger amount of memory to buffer
		if (pos >= bufsize)
		{
			bufsize += SIZE;
			line = realloc(line, bufsize * sizeof(char *));
			if (!line){
                printf("Allocation error\n");
                exit(EXIT_FAILURE);
			}
		}
		word = strtok(NULL, delim);        // read the next word
	}

	comp_args[pos] = NULL;         // assign the last value as NULL
	// printf("%d", pos);
	return pos;
}

// calling the particular function according to the matched command

int execute_builtins(int argc, char **args){
	int flag = 0;
	
	if (strcmp(args[0], builtin_commands[0]) == 0) {
	    flag = my_ls(argc, args);
	}
	if (strcmp(args[0], builtin_commands[1]) == 0) {
		flag = cd(args);
	}
	if (strcmp(args[0], builtin_commands[2]) == 0) {
		flag = my_cat(argc, args);
	}
	if (strcmp(args[0], builtin_commands[4]) == 0) {
		flag = my_mv(argc, args);
	}
	if (strcmp(args[0], builtin_commands[3]) == 0) {
		flag = pwd();
	}
	if (strcmp(args[0], builtin_commands[5]) == 0) {
		flag = my_grep(argc, args);
	}
	if (strcmp(args[0], builtin_commands[6]) == 0) {
		flag = my_mkdir(args[1]);
	}
	if (strcmp(args[0], builtin_commands[7]) == 0) {
		flag = my_chmod(argc, args);
	}
	if (strcmp(args[0], builtin_commands[8]) == 0) {
		flag = my_rm(argc, args);
	}
	if (strcmp(args[0], builtin_commands[9]) == 0) {
		flag = my_cp(argc, args);
	}
	return flag;
}

// using already built binaries 

int rest(int argc, char** comm, int back){
    
    // printf("second type \n");
	pid_t process_id;
	int status;
	process_id = fork();
	if (process_id == 0) {
		// Child process
        // printf("HELLO\n");
		if (execvp(comm[0], comm) == -1){
			// printf("HELLO\n");
			return 0;
		}
		exit(EXIT_FAILURE);
	} 
	else if (process_id < 0) {
		// forking error
		printf("Error: PID < 0");
		return 0;
	} 
	else {
		wait(NULL);
	}
	return 1;
}

// main loop for the terminal
int main(int argc, char **argv){

    // defining necessary pointers and variables
	char *line;
	char **comm;
	int buffsize = 1024;
	int status;
	int flag = 1;
	int j;
	char *line2;

	while(flag){
		char current_d[1024];
		getcwd(current_d, sizeof(current_d));
		printf("\n%s>> ", current_d);
		line = read_command_line();
        // printf("ARG READ\n");
		line2 = strdup(line);
		comm = space_separation(line);
		// printf("%s\n", comm[0]);
		int sze = arg_count(line2);
		// printf("%d\n", sze);
		int background = 0;
		pid_t pid, wpid;
        int done = 0;
        int comm_flag = 0;

		if(comm[0]!=NULL){
            // printf("arg\n");

            // check for the backgroung command
			if (strcmp(comm[sze-1],"&") == 0){

                // it is a background command
				background = 1;
				sze = sze - 1;
				int pstatus;
                // fork the process
                pid = fork();
				// execute the command by making suitable function call
				char** command = malloc(1000*(sizeof(char*)));
				int j = 0;
				while(j<(sze)){
					command[j] = comm[j];
					j++;
				}
                if (pid==0){
					setpgid(0,0); // it provides a Process Group ID of a process described by pid to pgid 
                    // child process
                    // printf("Child executed: %d\n", pid);
                    comm_flag = builtin_command_check(sze, comm);       // check if command in builtin commands list
                    if (comm_flag==1){

						// making a copy to separate & commands
                        done = execute_builtins(sze, command);
                    }
                    else{
                        // otherise take the builtin binaries
                        // printf("Type 2\n");
                        done = rest(sze, command, background);
                    }
                }
				else if (pid<0){
					printf("Error in fork\n");
				}
                else{
					printf("New Process Started\n");
					waitpid(pid, &pstatus, WNOHANG); // Used to kill the child immediately
                }
			}
            else{
                // not a background process
				background = 0;
                // check for the builtin commands in the list
                comm_flag = builtin_command_check(sze, comm);
				if (comm_flag==1){
					// printf("Running...\n");
                    done = execute_builtins(sze, comm);
				}
                else{
                    // printf("Type 2\n");
					done = rest(sze, comm, background);
                }
            }

		}
	}
	return EXIT_SUCCESS;
}