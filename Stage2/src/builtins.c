// this file contains all the built-in command functions for the simple shell program

#include "simpleshell.h"

// CD function. Changes the current working directory to the one provided as an argument. If no argument is provided, it prints the current working directory. It also updates the PWD environment variable to reflect the new current directory.
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

// CLR function. Clears the terminal screen using the system call "clear" command.
int do_clr(void) {
    system("clear"); // system call to clear screen

    return 0;
}

// DIR function. Lists the contents of the directory provided as an argument. If no argument is provided, it lists the contents of the current directory. It uses the system call "ls -al" to display all the contents in long format.
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

// ENVIRON function. Prints all the environment variables. It loops through the environ array and prints each environment variable followed by a newline.
int do_environ(void) {
    char **env = environ; // create a pointer env that points to the first element of the environ array

    while (*env) { // loop through the environment variables until we reach a NULL
        fprintf(stdout, "%s\n", *env); // print each environment variable followed by a newline
        env++; // move the pointer to the next element in the array
    }

    return 0;
}

// ECHO function. Prints the arguments provided after the echo command.
int do_echo(char *args[]) {
    int i = 1; // array starts from index 1 as index 0 is the command "echo"

    while (args[i] != NULL) { // loop through the args
        printf("%s ", args[i]); // print each argument
        i++;
    }

    printf("\n");

    return 0;
}

// HELP function. Displays the manual page for the simpleshell program using the system call "more" command to show the contents of the manual file one screen at a time.
int do_help(void) {
    system("more manual/README.txt"); // system call to display the manual file one screen at a time using more
    
    return 0;
}

// PAUSE function. Pauses the shell until the user presses Enter.
int do_pause(void) {
    char buffer[MAX_BUF]; // defining buffer with max buffer size where the input will be stored

    printf("Press Enter to continue...");
    fgets(buffer, MAX_BUF, stdin); // fgets waits for the user to press Enter and stores the input in buffer

    return 0;
}

// STAGE 2 ADDITIONS
// this function will run if any of the above commands were not recognised, it will attempt to execute whatever unknown command by forking a child process and using exec to run the command in the child process, while the parent process waits for the child to finish before continuing to prompt for input again
int external_command(char *args[], char *shell_path) { // we also get the already stored shell path to set it to parent in the child process environment 
    pid_t pid;

    switch (pid = fork()) { // forking and sending its pid into the variable pid
        case -1: // checking if forking failed
            perror("Creating child process failed.");
            return 1;
        case 0: // if pid is 0, meaning child process
            if (setenv("parent", shell_path, 1) == -1) {
                perror("Failed setting environment in child process");
                exit(1); // we exit the child process if it failed
            }
            if (execvp(args[0], args) == -1) { // using execvp as it searches for PATH automatically. Passing it the command name, followed by any arguments. It will attempt to execute the command, shows error if it fails
                perror("Executing external command failed");
                return 1;
            };
        default: // parent process
            wait(NULL); // waiting for child to finish before continuing to prompt for input again, NULL means we don't care about the exit status of the child
    }

    return 0;
}

/*
Daniel Clima
39488
I acknowledge the DCU Academic Integrity Policy.
*/