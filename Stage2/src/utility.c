// all other functions go here

#include "simpleshell.h"

// STAGE 2 ADDITIONS
// this function takes the tokenised input and parses it to fill the Command struct with the appropriate information for the command, such as the arguments, input/output redirection files, and background execution flag. It also checks for any errors in the input format and returns an error code if there is an issue.
int parse_command(char *args[], Command *cmd) {
    int i = 0;
    int j = 0;

    // initialising the Command struct fields to default values
    cmd->input_file = NULL;
    cmd->output_file = NULL;
    cmd->append = 0;
    cmd->background = 0;

    while (args[i] != NULL) { // we loop through the tokenised input until we reach the end
        if (strcmp(args[i], ">") == 0) { // checks if the current token is the output redirection symbol for overwrite
            if (args[i + 1] == NULL) { // checks if there is a file provided after the symbol, if not it returns an error
                perror("Redirection must contain a file");
                return 1;
            }

            cmd->output_file = args[i + 1]; // if there is a file provided, we set the output_file field of the Command struct to the file name
            cmd->append = 0; // we set the append flag to 0 to indicate that this is an overwrite redirection
            i++; // skip symbol andfile name
        }
        else if (strcmp(args[i], ">>") == 0) { // checks if the current token is the output redirection symbol for append
            if (args[i + 1] == NULL) {
                perror("Redirection must contain a file");
                return 1;
            }

            cmd->output_file = args[i + 1];
            cmd->append = 1; // we set the append flag to 1 to indicate that this is an append redirection
            i++;
        }
        else if (strcmp(args[i], "<") == 0) { // checks if the current token is the input redirection symbol
            if (args[i + 1] == NULL) {
                perror("Redirection must contain a file");
                return 1;
            }

            cmd->input_file = args[i + 1];
            i++;
        }
        else if (strcmp(args[i], "&") == 0) { // checks if the current token is the background execution symbol
            if (args[i + 1] != NULL) {
                perror("Commands must end with &");
                return 1;
            }

            cmd->background = 1; // we set the background flag to 1 to indicate that this command should be run in the background
        }
        else { // otherwise we simply add the token to the argv array in the Command struct, which will be used for executing the command later
            cmd->argv[j] = args[i]; // we use j to keep track of the position in the argv array, which can be different from i as it only holds the actual command and arguments, and skips over the symbols and file names
            j++;
        }
        i++;
    }
    
    cmd->argv[j] = NULL; // we set the last element of the argv array to NULL to indicate the end of the arguments, which is required for the execvp function

    return 0;
}

// this function will run if any of the above commands were not recognised, it will attempt to execute whatever unknown command by forking a child process and using exec to run the command in the child process, while the parent process waits for the child to finish before continuing to prompt for input again
int external_command(Command *cmd, char *shell_path) { // we also get the already stored shell path to set it to parent in the child process environment 
    pid_t pid;
    int fd; // where we will store the file descriptor if there is any redirection needed

    switch (pid = fork()) { // forking and sending its pid into the variable pid
        case -1: // checking if forking failed
            perror("Creating child process failed");
            return 1;
        case 0: // if pid is 0, meaning child process
            if (setenv("parent", shell_path, 1) == -1) {
                perror("Failed setting environment in child process");
                exit(1); // we exit the child process if it failed
            }
            
            if (cmd->output_file != NULL) {

                if (!cmd->append) { // if append flag is not set, we open the file for overwriting
                    fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644); // O_WRONLY - write only, O_CREAT - create if doesnt exist, O_TRUNC - empty file if it isnt empty, 0644 - setting permissions for when creating file, owner can read/write, others only read
                } 
                else {
                    fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644); // O_APPEND - appends to the end of file
                }

                if (fd == -1) { // checking if there was an error opening the file
                    perror("Error opening file.");
                    exit(1); 
                }

                if (dup2(fd, STDOUT_FILENO) == -1) { // we change the stdout stream to point to our file, also checking for errors
                    perror("Redirection failed.");
                    close(fd); // we close the file descriptor before returning if there was an error
                    exit(1);
                }

                close(fd); // close the file descriptor, as it is no longer needed
            }

            if (cmd->input_file != NULL) {
                fd = open(cmd->input_file, O_RDONLY); // R_ONLY - open file for reading only
                
                if (fd == -1) {
                    perror("Error opening file.");
                    exit(1);
                }

                if (dup2(fd, STDIN_FILENO) == -1) {
                    perror("Redirection failed.");
                    exit(1);
                }

                close(fd);
            }

            if (execvp(cmd->argv[0], cmd->argv) == -1) { // using execvp as it searches for PATH automatically. Passing it the command name, followed by any arguments. It will attempt to execute the command, shows error if it fails
                perror("Executing external command failed");
                exit(1);
            };
        default: // parent process
            wait(NULL); // waiting for child to finish before continuing to prompt for input again, NULL means we don't care about the exit status of the child
    }

    return 0;
}