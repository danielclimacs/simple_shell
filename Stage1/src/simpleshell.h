// this header file contains all definitions and declarations for the main shell program

#ifndef SIMPLESHELL_H // include guard to prevent multiple inclusions of this header file
#define SIMPLESHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#define MAX_BUF 1024 // max buffer size
#define MAX_ARGS 64 // max number of arguments
#define SEPARATOR " \t\n" // separator for tokenizing input 


extern char **environ; // declare environ as an external variable, which is a pointer to an array of strings containing the environment variables 

int do_cd(char *args[]);
int do_clr(void);
int do_dir(char *args[]);
int do_environ(void);
int do_echo(char *args[]);
int do_help(void);
int do_pause(void);

#endif

/*
Daniel Clima
39488
I acknowledge the DCU Academic Integrity Policy.
*/