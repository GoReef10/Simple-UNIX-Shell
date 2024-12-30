#ifndef PROJECT_H
#define PROJECT_H

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>


/* main.c */
void interactive_shell();
void non_interactive_shell();

/* shell.c */
#define MAX_INPUT_SIZE 1024
#define MAX_PATH_LEN 1024
#define NAME_LEN 100

void parser(char *input, char *args[], int *nr_args);


// void exec_command();

#endif