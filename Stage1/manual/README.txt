NAME
    simpleshell - a simple command line shell implemented in C

SYNOPSIS
    ./simpleshell

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

COMMANDS
    cd [directory]
        Changes the current working directory.

    clr
        Clears the terminal screen.

    dir [directory]
        Displays the contents of a directory.

    echo [text]
        Displays the text entered by the user.

    environ
        Lists all environment variables available to the shell.

    help
        Displays help information for the shell.

    pause
        Pauses execution of the shell until the user presses Enter.

    quit
        Terminates the shell.

ENVIRONMENT
    The shell accesses environment variables using the POSIX-compliant
    environment list provided by the variable 'environ'.

    Environment variables are stored as strings in the format:

        NAME=value

FILES
    simpleshell.c
        Main shell loop and command processing.

    builtins.c
        Implementation of built-in shell commands.

    simpleshell.h
        Header file containing definitions and declarations.

LIMITATIONS
    This shell is a simplified implementation.

    Only built-in commands are supported.
    Input length is limited by the buffer size.
    The number of arguments is limited by MAX_ARGS.

AUTHOR
    Your NameNAME
    simpleshell - a simple command line shell implemented in C

SYNOPSIS
    ./simpleshell

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

COMMANDS
    cd [directory]
        Changes the current working directory.

    clr
        Clears the terminal screen.

    dir [directory]
        Displays the contents of a directory.

    echo [text]
        Displays the text entered by the user.

    environ
        Lists all environment variables available to the shell.

    help
        Displays help information for the shell.

    pause
        Pauses execution of the shell until the user presses Enter.

    quit
        Terminates the shell.

ENVIRONMENT
    The shell accesses environment variables using the POSIX-compliant
    environment list provided by the variable 'environ'.

    Environment variables are stored as strings in the format:

        NAME=value

FILES
    simpleshell.c
        Main shell loop and command processing.

    builtins.c
        Implementation of built-in shell commands.

    simpleshell.h
        Header file containing definitions and declarations.

LIMITATIONS
    This shell is a simplified implementation.

    Only built-in commands are supported.
    Input length is limited by the buffer size.
    The number of arguments is limited by MAX_ARGS.

AUTHOR
    Daniel Clima
    39488
    I acknowledge the DCU Academic Integrity Policy.
