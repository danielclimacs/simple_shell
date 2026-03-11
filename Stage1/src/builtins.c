// this file contains all the built-in command functions for the simple shell program

#include "simpleshell.h"

int do_cd(char *args[]) {

    char cwd[PATH_MAX]; // defining cwd with max path size where current directory will be stored

    if (args[1] == NULL) { // if there is no directory provided
        if (getcwd(cwd, PATH_MAX) == NULL) { // getcwd gets the current working directory and stores it in cwd. If it fails, it returns NULL
            perror("Path error");
            return 1;
        }

        printf("%s\n", cwd);
        return 0;
    }

    if (chdir(args[1]) != 0) { // chdir changes the current working directory to the one provided in args[1]. If it fails, it returns -1
        perror("Directory change error");
        return 1;
    }

    if (getcwd(cwd, PATH_MAX) == NULL) { // getcwd gets the new current working directory and stores it in cwd
        perror("Path error");
        return 1;
    }

    if (setenv("PWD", cwd, 1) != 0) { // setenv updates the PWD environment variable with the new current directory. The 1 means overwrite the old value if it exists
        perror("PWD update error");
        return 1;
    }

    return 0;
}

int do_clr(void) {
    system("clear"); // system call to clear screen

    return 0;
}

int do_dir(char *args[]) {
    char command[MAX_BUF];
    char *directory = "."; // current dir

    if (args[1]) { // if there is a directory provided
        directory = args[1];
    }

    snprintf(command, MAX_BUF, "ls -al %s", directory); // create a command string including the directory argument
    system(command); // execute the command using system call

    return 0;
}

int do_environ(void) {
    char **env = environ; // create a pointer env that points to the first element of the environ array

    while (*env) { // loop through the environment variables until we reach a NULL
        fprintf(stdout, "%s\n", *env); // print each environment variable followed by a newline
        env++; // move the pointer to the next element in the array
    }

    return 0;
}

int do_echo(char *args[]) {
    int i = 1;

    while (args[i] != NULL) { // loop through the args
        printf("%s ", args[i]); // print each argument
        i++;
    }

    printf("\n");

    return 0;
}

int do_help(void) {
    system("more manual/README.txt"); // system call to display the manual file one screen at a time using more
    
    return 0;
}

int do_pause(void) {
    char buffer[MAX_BUF]; // defining buffer with max buffer size where the input will be stored

    printf("Press Enter to continue...");
    fgets(buffer, MAX_BUF, stdin); // fgets waits for the user to press Enter and stores the input in buffer

    return 0;
}

/*
Daniel Clima
39488
I acknowledge the DCU Academic Integrity Policy.
*/