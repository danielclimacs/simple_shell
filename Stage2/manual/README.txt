NAME
    simpleshell - a simple command line shell implemented in C

SYNOPSIS
    ./bin/simpleshell
    ./bin/simpleshell [batchfile]

DESCRIPTION
    simpleshell is a basic Unix-style command line interpreter written in C.
    The shell reads commands from standard input, parses them into tokens,
    and executes supported built-in commands.

    User input is read into a buffer and then split into tokens using
    separators such as spaces, tabs, and newlines. The tokens are stored
    in an argument array similar to the argv array used by C programs.

    The first token is interpreted as the command name. If the command
    matches one of the built-in commands, the corresponding function
    is executed.

BATCH MODE
    The shell can be invoked with a single command line argument, where the
    argument is a file with specific commands to be executed by the simpleshell.
    
    Batch mode reads from that file instead of the keyboard, and does not prompt
    user for any input. Instead it simply executes any commands given to it and 
    prints to the terminal screen.

INTERNAL COMMANDS
    cd [directory]
        Changes the current working directory. 
        If no directory is provided, it prints the current working directory path.

    clr
        Clears the terminal screen.

    dir [directory]
        Displays the contents of a directory.
        If no directory is provided, it displays the contents of the current 
        working directory

    echo [text]
        Displays the text entered by the user.

    environ
        Lists all environment variables available to the shell.

    help
        Displays the manual page with information for the shell.

    pause
        Pauses execution of the shell until the user presses Enter.

    quit
        Terminates the shell.

EXTERNAL COMMANDS
    Any command not recognised as an internal command is treated as an external
    program and is executed using fork and exec.

    Examples
        ls
        pwd
        cat myfile.txt

I/O REDIRECTION 
    The shell allows for redirection of input and output to the specified file.

    [command] < [file]
        Redirects standard input

    [command] > [file]
        Redirects standard output and overwrites file
        
    [command] >> [file]
        Redirects standard output and appends to file

    Output redirection is also supported for the following internal commands: 
    dir, environ, echo and help.

BACKGROUND EXECUTION
    The shell can be run in the background if the command specified includes 
    the special background execution symbol "&" at the end of the command. 

    [command] &
        Executes a command in the background and 
        immediately returns the user to the terminal

ENVIRONMENT
    The shell sets the environment variable "shell" to the full 
    path of the simpleshell executable.

    shell=/full/path/to/simpleshell

    When an external command is executed, the child inherits the shell's 
    environment variables (because of the execvp function). Also, a "parent"
    environment variable is created inside the child process, which contains
    the full path to the simple shell program that launched it.

FILES
    simpleshell.c
        Main shell loop and command processing.

    builtins.c
        Implementation of built-in shell commands.

    simpleshell.h
        Header file containing definitions and declarations.

    utility.c
        Includes all other functions used by the shell that are not in the built-ins.c.

AUTHOR
    Name: Daniel Clima
    Student ID: 39488
    Source and Inspiration for the Manual Page: https://www.man7.org/linux/man-pages/man1/man.1.html
    I acknowledge the DCU Academic Integrity Policy.
