// STAGE 2 ADDITION - this file contains all other not built-in functions that are used by the simple shell program

#include "simpleshell.h"

/*
This function takes the tokenised input and parses it to fill the Command struct with the appropriate information for the command, 
such as the arguments, input/output redirection files, and background execution flag. It also checks for any errors in the input format 
and returns an error code if there is an issue.
*/
int parse_command(char *args[], Command *cmd) {
    int i = 0;
    int j = 0;

    // initialising the Command struct fields to default values
    cmd->argv[0] = NULL;
    cmd->input_file = NULL;
    cmd->output_file = NULL;
    cmd->append = 0;
    cmd->background = 0;

    while (args[i] != NULL) { // we loop through the tokenised input until we reach the end
        if (strcmp(args[i], ">") == 0) { // checks if the current token is the output redirection symbol for overwrite
            if (args[i + 1] == NULL) { // checks if there is a file provided after the symbol, if not it returns an error
                fprintf(stderr, "Usage: command > output_file\n");
                return 1;
            }

            cmd->output_file = args[i + 1]; // if there is a file provided, we set the output_file field of the Command struct to the file name
            cmd->append = 0; // we set the append flag to 0 to indicate that this is an overwrite redirection
            i++; // skip symbol and file name
        }
        else if (strcmp(args[i], ">>") == 0) { // checks if the current token is the output redirection symbol for append
            if (args[i + 1] == NULL) {
                fprintf(stderr, "Usage: command >> output_file\n");
                return 1;
            }

            cmd->output_file = args[i + 1];
            cmd->append = 1; // we set the append flag to 1 to indicate that this is an append redirection
            i++;
        }
        else if (strcmp(args[i], "<") == 0) { // checks if the current token is the input redirection symbol
            if (args[i + 1] == NULL) {
                fprintf(stderr, "Usage: command < output_file\n");
                return 1;
            }

            cmd->input_file = args[i + 1];
            i++;
        }
        else if (strcmp(args[i], "&") == 0) { // checks if the current token is the background execution symbol
            if (i == 0) { // & cannot be the first token
                fprintf(stderr, "Background symbol '&' cannot be the first token\n");
                return 1;
            }

            if (args[i + 1] != NULL) { // & must be the last token, if there are tokens after it we return an error
                fprintf(stderr, "Commands must end with '&'\n");
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

    if (cmd->argv[0] == NULL) { // if there is no input we exit the function and just continue to the next iteration of the loop 
        return 1;
    }

    return 0;
}


/*
This function takes the filled Command struct and checks which command it is, if it is a built-in command it calls the appropriate function to execute it,
if not it calls the external_command function to attempt to execute it as an external command. 
For the built-in commands that support output redirection, it also calls the redirection_start and redirection_end functions to handle the redirection before and after executing the command.
*/
int execute_command(Command *cmd, char *shell_path) {
    int stdout_backup; // backup for the original stdout file descriptor 

    // cd
    if (strcmp(cmd->argv[0], "cd") == 0)
    {
        do_cd(cmd->argv);
    }
    // clr
    else if (strcmp(cmd->argv[0], "clr") == 0)
    {
        do_clr();
    }
    // dir
    // redirection update
    else if (strcmp(cmd->argv[0], "dir") == 0)
    {
        // we call the redirection_start function to check if the command requires redirection, if yes the function handles redirection, if not the command runs normally. Also checking for errors, if there are errors we print and continue to next iteration
        if (redirection_start(cmd, &stdout_backup) == 1) { 
            return 1;
        }

        do_dir(cmd->argv);

        if (cmd->output_file != NULL) { // this checks if redirection occured
            if (redirection_end(stdout_backup) == 1) { // this function restores the orginal output stream if it was changed, also checking for errors
                return 1;
            }
        }
    }
    // environ
    // redirection update
    else if (strcmp(cmd->argv[0], "environ") == 0)
    {
        if (redirection_start(cmd, &stdout_backup) == 1) {
            return 1;
        }

        do_environ();

        if (cmd->output_file != NULL) {
            if (redirection_end(stdout_backup) == 1) {
                return 1;
            }
        }
    }
    // echo
    // redirection update
    else if (strcmp(cmd->argv[0], "echo") == 0)
    {
        if (redirection_start(cmd, &stdout_backup) == 1) {
            return 1;
        }

        do_echo(cmd->argv);

        if (cmd->output_file != NULL) {
            if (redirection_end(stdout_backup) == 1) {
                return 1;
            }
        }
    }
    // help
    // redirection update
    else if (strcmp(cmd->argv[0], "help") == 0)
    {
        if (redirection_start(cmd, &stdout_backup) == 1) {
            return 1;
        }

        do_help();

        if (cmd->output_file != NULL) {
            if (redirection_end(stdout_backup) == 1) {
                return 1;
            }
        }
    }
    // pause
    else if (strcmp(cmd->argv[0], "pause") == 0)
    {
        do_pause();
    }
    // quit
    else if (strcmp(cmd->argv[0], "quit") == 0)
    {
        return 2; // we return 2 to indicate that the user wants to quit, this is used in the main loop to break out and exit the program
    }
    // external commands
    else
    {
        // STAGE 2 ADDITION
        if (external_command(cmd, shell_path) != 0) { // checking for errors when executing external command
            return 1;
        }
    }

    return 0;
}



/* 
This function will run if any of the built-in commands were not recognised, it will attempt to execute whatever unknown command by forking 
a child process and using exec to run the command in the child process, while the parent process waits for the child to finish before continuing to prompt for input again. 
The function also checks for any output/input redirection, and handles it accordingly. It also handles background execution, if the flag is set, the function doesnt wait for the child 
*/
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
                    perror("Error opening file");
                    exit(1); 
                }

                if (dup2(fd, STDOUT_FILENO) == -1) { // we change the stdout stream to point to our file, also checking for errors
                    perror("Redirection failed");
                    close(fd); // we close the file descriptor before returning if there was an error
                    exit(1);
                }

                close(fd); // close the file descriptor, as it is no longer needed
            }

            if (cmd->input_file != NULL) {
                fd = open(cmd->input_file, O_RDONLY); // R_ONLY - open file for reading only
                
                if (fd == -1) {
                    perror("Error opening file");
                    exit(1);
                }

                if (dup2(fd, STDIN_FILENO) == -1) {
                    perror("Redirection failed");
                    exit(1);
                }

                close(fd);
            }

            if (execvp(cmd->argv[0], cmd->argv) == -1) { // using execvp as it searches for PATH automatically. Passing it the command name, followed by any arguments. It will attempt to execute the command, shows error if it fails
                if (cmd->background) { // if background flag is set
                    fprintf(stderr, "\n"); // we print a newline before error message for cleaner output
                }
                fprintf(stderr, "%s: command not found\n", cmd->argv[0]); // we print an error message including the command name that failed to execute
                exit(1);
            }
        default: // parent process
            if (!cmd->background) { // STAGE 2 ADDITTION - this checks if the command has been launched in the background, if not we wait for child
                waitpid(pid, NULL, 0); // waitpid waits for the child process with the specified pid to finish executing before allowing the parent process to continue, NULL means we don't care about the exit status of the child. This is better than wait() as with background proceses, wait() could end up waiting for the wrong child process if there are multiple background processes running, while waitpid with the specific pid ensures we are waiting for the correct child process to finish
            }
    }

    return 0;
}



// this function handles the redirection of output for a command. It checks if an output file is specified, if yes it backs up the current stdout file descriptor, opens the specified output file with the appropriate flags for appending or overwriting, and then redirects stdout to the file. If there are any errors, it handles them and return an error code.
int redirection_start(Command *cmd, int *stdout_backup) {
    int fd;

    if (cmd->output_file == NULL) { // checking if there is an output file specified, if not we simply return as there is no redirection needed
        return 0;
    }
    
    *stdout_backup = dup(STDOUT_FILENO); // backing up our current stdout using dup
    if (*stdout_backup == -1) { // checking if backup failed
        perror("Failed to backup stdout");
        return 1;
    }

    if (!cmd->append) { // chechking if append flag is not set, if not we open the file for overwriting
        fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644); // rewrite
    }
    else {
        fd = open(cmd->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644); // append

    }

    if (fd == -1) { // checking if there was an error opening the file
        perror("Error opening file");
        close(*stdout_backup); // we close the backup file descriptor before returning if there was an error
        return 1;
    }

    if (dup2(fd, STDOUT_FILENO) == -1) { // we change the stdout stream to point to our file, also checking for errors
        perror("Redirection failed");
        // closing everthing before returning if there was an error
        close(fd);
        close(*stdout_backup);
        return 1;
    }

    close(fd); // we can close the file descriptor as it is no longer needed after redirection

    return 0;
}



// this function runs after the redirection_start function to restore the original stdout stream using the backed up file descriptor. It checks for errors in the process and returns an error code if there is an issue.
int redirection_end(int stdout_backup) {
    if (dup2(stdout_backup, STDOUT_FILENO) == -1) { // we restore the original stdout stream using the backup file descriptor, also checking for errors
        perror("Redirection failed.");
        close(stdout_backup);
        return 1;
    }
    close(stdout_backup); // we close the backup file descriptor as it is no longer needed after restoring stdout

    return 0;
}

/*
Daniel Clima
39488
I acknowledge the DCU Academic Integrity Policy.
*/