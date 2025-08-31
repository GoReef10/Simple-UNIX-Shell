#include "shell.h"

#define DELIMITERS " \n\t"
#define BUFFER_SIZE 64


void parser(char *input, char *args[], int *nr_args, char **infile, char **outfile, int *background){

    *infile = NULL;
    *outfile = NULL;
    *background = 0;

    char *tok = strtok(input, DELIMITERS);
    int i = 0;
    while(tok != NULL && i < COMMAND_NR){
        
        if(strcmp(tok, "<") == 0){
            tok = strtok(NULL, DELIMITERS);
            if(tok != NULL){
                *infile = tok;
                args[i] = NULL;
                args[i + 1] = NULL;
                *nr_args = i;
            }
            else{
                printf("No input file specified\n");
            }
            return;
        }
        else if(strcmp(tok, ">") == 0){
            tok = strtok(NULL, DELIMITERS);
            if(tok != NULL){
                *outfile = tok;
                args[i] = NULL;
                args[i + 1] = NULL;
                *nr_args = i;
            }
            else{
                printf("No output file specified\n");
            }
            return;
        }
        else if(strcmp(tok, "&") == 0){
            *background = 1;
            args[i] = NULL;
            *nr_args = i;
            return;
        }
        
        args[i] = tok;
        i++;
        
        tok = strtok(NULL, DELIMITERS);
    }
    args[i] = NULL;
    *nr_args = i;
}