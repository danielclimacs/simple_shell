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

    The shell can also be invoked with a single command line argument
    that will activate batch mode. Where the shell will read from the input 
    file, line by line, tokenize each line, and similarily, attempt to execute 
    the commands inside the file.

COMMANDS
    cd [directory]
        Changes the current working directory. 
        If no directory is provided, it prints the current working directory path.

    clr
        Clears the terminal screen.

    dir [directory]
        Displays the contents of a directory.
        If no directory is provided, it displays the contents of the current working directory

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

ENVIRONMENT
    The shell sets the environment variable "shell" to the full 
    path of the simpleshell executable.

FILES
    simpleshell.c
        Main shell loop and command processing.

    builtins.c
        Implementation of built-in shell commands.

    simpleshell.h
        Header file containing definitions and declarations.

AUTHOR
    Name: Daniel Clima
    Student ID: 39488
    Source and Inspiration for the Manual Page: https://www.man7.org/linux/man-pages/man1/man.1.html
    I acknowledge the DCU Academic Integrity Policy.
