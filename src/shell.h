#ifndef PROJECT_H
#define PROJECT_H

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

/* main.c */
void interactive_shell();
void non_interactive_shell();

/* execute_commands.c */
#define COMMAND_NR 100

typedef struct Command{
    char *args[COMMAND_NR]; 
    int nr_args;
    char *infile;
    char *outfile;
    int background;
    int append;
    struct Command *next;
} Command;

int execute_builtins(char *input, char *args[], char **history, char *username, char *hostname, char *path, int *run);
void execute_command(Command command);

/* shell.c */
#define MAX_INPUT_SIZE 1024
#define MAX_PATH_LEN 1024
#define NAME_LEN 100

/* parser.c */
#define DELIMITERS " \n\t"
#define BUFFER_SIZE 64

void parser(char *input, Command *command);

#endif