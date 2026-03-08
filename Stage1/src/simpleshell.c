// this is the main file for the simple shell program

#include "simpleshell.h" // include the header file

int main(int argc, char *argv[]) {

    char buffer[MAX_BUF]; // defining buffer with max buffer size where input will be stored
    char prompt[PATH_MAX]; // defining prompt with max path size where current directory will be stored
    char *args[MAX_ARGS]; // defining arg as an array of strings where the tokenised arguments will be stored
    char **arg = args; // defining arg as a pointer to the first element of args array

    while (1) { // infinite loop to continuously prompt for input until the user decides to quit
        if (getcwd(prompt, PATH_MAX) == NULL) { // getcwd gets the current working directory and stores it in prompt. If it fails, it returns NULL
            perror("Path error");
            return 1;
        }

        // print the prompt, which consists of the current directory followed by a $ sign
        printf("%s$ ", prompt);

        if (fgets(buffer, MAX_BUF, stdin) == NULL) { // fgets reads a line of input from stdin and stores it in buffer. It returns NULL if an error occurs or end of file is reached
            break; // if fgets returns NULL, we print a newline and break the loop to exit the program
        }

        arg = args; // reset arg to point to the beginning of the args array for each new command input
        *arg++ = strtok(buffer, SEPARATOR); // splitting the input into tokens using the separator and storing them in the args array, the ++ moves the pointer to the next position in the array for the next token to be assigned in the list
        
        while ((*arg++ = strtok(NULL, SEPARATOR))); // the while loop continues to call strtok with NULL to get the next token until there are no more tokens left, at which point strtok returns NULL and the loop terminates

        if (args[0] == NULL) { // if there is no input
            continue;
        }

        //////////////////////////////Commands////////////////////////////////

        if (strcmp(args[0], "cd") == 0)
        {
            do_cd(args);
        }
        else if (strcmp(args[0], "clr") == 0)
        {
            do_clr();
        }
        else if (strcmp(args[0], "dir") == 0)
        {
            do_dir(args);
        }
        else if (strcmp(args[0], "environ") == 0)
        {
            do_environ();
        }
        else if (strcmp(args[0], "echo") == 0)
        {
            do_echo(args);
        }
        else if (strcmp(args[0], "help") == 0)
        {
            do_help();
        }
        else if (strcmp(args[0], "pause") == 0)
        {
            do_pause();
        }
        else if (strcmp(args[0], "quit") == 0)
        {
            break;
        }
        else
        {
            printf("Command not recognised\n");
        }
    }

    
    return 0;
}

/*
Daniel Clima
39488
I acknowledge the DCU Academic Integrity Policy.
*/