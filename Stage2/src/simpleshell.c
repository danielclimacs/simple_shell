// this is the main file for the simple shell program

#include "simpleshell.h" // include the header file

int main(int argc, char *argv[]) {

    //////////////////////////////VARIABLE DECLARATION//////////////////////////////

    char buffer[MAX_BUF]; // defining buffer with max buffer size where input will be stored
    char prompt[PATH_MAX]; // defining prompt with max path size where current directory will be stored
    char shell_path[PATH_MAX]; // defining the shell path where the full path to our program is stored
    char *args[MAX_ARGS]; // defining arg as an array of strings where the tokenised arguments will be stored
    char **arg = args; // defining arg as a pointer to the first element of args array
    FILE *input = stdin; // defining input as a pointer to a file, initially set to standard input

    //////////////////////////////SHELL INITIALISATION//////////////////////////////

    if (realpath(argv[0], shell_path) == NULL) { // getting the real path to our program using the first argument and checking for error
        perror("Error getting path");
        return 1;
    } 

    if (setenv("shell", shell_path, 1) != 0) { // setting the shell environmental variable to the full path of our program, 1 means overwrite the old value if it exists
        perror("Shell path update error");
        return 1;
    }

    if (argc > 2) { // if there are more than 2 command line arguments, we print an error message and exit the program
        perror("Too many arguments\n");
        return 1;
    }

    else if (argc == 2) { // if there is a command line argument provided, we will read commands from the file instead of standard input
        input = fopen(argv[1], "r");

        if (input == NULL) { // if there is an error opening the file, we print an error message and exit the program
            perror("Error opening file");
            return 1;
        }
    }

    //////////////////////////////SHELL RUNNING//////////////////////////////

    while (1) { // infinite loop to continuously prompt for input until the user decides to quit

        while (waitpid(-1, NULL, WNOHANG) > 0); // WNOHANG - not blocking, this process checks for any child processes that have finished executing in the background and removes them to prevent accumulation of zombie processes. It does this by calling waitpid with a pid of -1 to check for any child process

        if (input == stdin) { // if we are reading from standard input, we want to display the prompt
            if (getcwd(prompt, PATH_MAX) == NULL) { // getcwd gets the current working directory and stores it in prompt. If it fails, it returns NULL
                perror("Path error");
                return 1;
            }

            // print the prompt, which consists of the current directory followed by a $ sign only
            printf("%s$ ", prompt);
        }

        if (fgets(buffer, MAX_BUF, input) == NULL) { // fgets reads a line of input from either stdin or a file and stores it in buffer. It returns NULL if an error occurs or end of file is reached
            break; // if fgets returns NULL, we break the loop to exit the program
        }

        arg = args; // reset arg to point to the beginning of the args array for each new command input
        *arg++ = strtok(buffer, SEPARATOR); // splitting the input into tokens using the separator and storing them in the args array, the ++ moves the pointer to the next position in the array for the next token to be assigned in the list

        while ((*arg++ = strtok(NULL, SEPARATOR)) != NULL); // the while loop continues to call strtok with NULL to get the next token until there are no more tokens left, at which point strtok returns NULL and the loop terminates

        // STAGE 2 ADDITION 
        Command cmd; // we define a variable of type Command struct to hold the command information after parsing the input

        if (parse_command(args, &cmd) != 0) { // we parse the command and check for any errors in the input. If there is an error, we print an error message and continue to the next iteration of the loop
            continue;
        }

        //////////////////////////////COMMANDS - STAGE 2 UPDATED////////////////////////////////

        int cmd_result = execute_command(&cmd, shell_path); // we execute the command and store the result to check for errors or quit signal

        if (cmd_result == 1) { // if result is 1, it indicates an error, which was printed inside the function, so we just continue to the next iteration of the loop
            continue;
        }
        else if (cmd_result == 2) { // if the result is 2, it indicates user wants to quit the shell
            break; // exit the program if user wants to quit
        }
    }
    //////////////////////////////END OF SHELL////////////////////////////////

    if (input != stdin) { // if we were reading from a file, we need to close it before exiting the program
        fclose(input);
    }

    printf("\n"); // print a newline before exiting the program
    
    return 0;
}

/*
Daniel Clima
39488
I acknowledge the DCU Academic Integrity Policy.
*/