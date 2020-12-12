#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

// pwd command ----------------------------------------------------------------------------------

int pwd(){
	char current_d[1024];  		// char variable to store the path

	// getcwd function returns the path to current directory into the 1st agrument provided to it
	if (getcwd(current_d, sizeof(current_d)) != NULL){ 
		fprintf(stdout, "%s", current_d);		// print result
	}
	else{
		fprintf(stderr, "getcwd error"); // if there is en error
	}
	return 1;
}