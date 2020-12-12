#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

// chmod command ----------------------------------------------------------------

int my_chmod(int argc, char **args){
    int j;
    j = strtol(args[1], 0, 8);      // converts a string into number system provided in third argument, also sparates the string from number and return string into second argument (location of a pointer)
    if (chmod (args[2],j)<0)        // chmod system call to change permissions
    {
        fprintf(stderr, "error in altering permissions");
        return 0;
    }
    return 1;
}