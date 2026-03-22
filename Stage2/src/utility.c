// all other functions go here

#include "simpleshell.h"

// STAGE 2 ADDITIONS
int parse_command(char *args[], Command *cmd) {
    int i = 0;
    int j = 0;

    while (args[i] != NULL) {
        if (strcmp(args[i], ">") == 0) {
            if (args[i + 1] == NULL) {
                perror("Redirection must contain a file.");
                return 1;
            }

            cmd->output_file = args[i + 1];
            cmd->append = 0;
        }
        else if (strcmp(args[i], ">>") == 0) {
            if (args[i + 1] == NULL) {
                perror("Redirection must contain a file.");
                return 1;
            }

            cmd->output_file = args[i + 1];
            cmd->append = 1;
        }
        else if (strcmp(args[i], "<") == 0) {
            if (args[i + 1] == NULL) {
                perror("Redirection must contain a file.");
                return 1;
            }

            cmd->input_file = args[i + 1];
        }
        else if (strcmp(args[i], "&") == 0) {
            if (args[i + 1] != NULL) {
                perror("Commands must end with &.");
                return 1;
            }

            cmd->background = 1;
        }
        else {
            cmd->argv[j] = args[i];
            j++;
        }
        i++;
    }
    
    cmd->argv[j] = NULL; //

    return 0;
}

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