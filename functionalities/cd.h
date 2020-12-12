#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

// a function to determine the given path is a directory or not
int isDirectory(char name[]){
	DIR* dir = opendir(name);		// DIR datatype representing a directory stream
	if (dir!=NULL){
		closedir(dir);				// if path is directory then close it and return 1 
		// printf("1");
		return 1;
	}
	return 0;
}

// Cd command ----------------------------------------------------------------------------------------------
int cd(char **args){

	// int sze = sizeof(args)/sizeof(args[0]);
    // printf("cd function here\n");
    // printf("%s\n", args[1]);
	int dir_ornot;
	dir_ornot = isDirectory(args[1]);		// check if the path is direcory or not
	if (dir_ornot==1){
		if (args[1]==NULL){
			fprintf(stderr, "NO argument provided");
		}
		chdir( args[1] );                   // system call used to cahnge the directory
		return 1;                           // unistd needed
	}
	fprintf(stderr, "Please specify a directory");
	return 0;
}