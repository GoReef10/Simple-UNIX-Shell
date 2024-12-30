#include "shell.h"

#define DELIMITERS " \n"
#define BUFFER_SIZE 64
#define COMMAND_NR 100

void parser(char *input, char *args[], int *nr_args, char **infile, char **outfile){
    //*infile = NULL;
    //*outfile = NULL
    char *tok = strtok(input, DELIMITERS);
    int i = 0;
    while(tok != NULL && i < COMMAND_NR){
        
        /*if(strcmp(tok, "<") == 0){
            tok = strtok(NULL, " ");
            if(tok != NULL){
                *infile = tok;
            }
        }
        */
        /*if(strcmp(tok, ">") == 0){
            tok = strtok(NULL, " ");
            if(tok != NULL){
                *outfile = tok;
            }
        }
        */
        /*else{

        }
        */
        args[i] = tok;
        i++;

        
        tok = strtok(NULL, DELIMITERS);
    }
    args[i] = NULL;
    *nr_args = i;
}