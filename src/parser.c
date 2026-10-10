#include "../include/shell.h"



void parser(char *input, Command *command){

    Command *current_command = command;

    char *tok = strtok(input, DELIMITERS);
    int i = 0;
    while(tok != NULL && i < COMMAND_NR){
        
        if(strcmp(tok, "<") == 0){
            tok = strtok(NULL, DELIMITERS);
            if(tok != NULL){
                current_command->infile = tok;
            }
            else{
                printf("No input file specified\n");
                return;
            }
        }
        else if(strcmp(tok, ">>") == 0){
            tok = strtok(NULL, DELIMITERS);
            if(tok != NULL){
                current_command->outfile = tok;
                current_command->append = 1;
            }
            else{
                printf("No output file specified\n");
                return;
            }
        }
        else if(strcmp(tok, ">") == 0){
            tok = strtok(NULL, DELIMITERS);
            if(tok != NULL){
                current_command->outfile = tok;
            }
            else{
                printf("No output file specified\n");
                return;
            }
        }
        else if(strcmp(tok, "&") == 0){
            current_command->background = 1;
        }
        else if(strcmp(tok, "|") == 0){
            current_command->args[i] = NULL;
            current_command->nr_args = i;
            i = 0;
            current_command->next = malloc(sizeof(Command));
            memset(current_command->next, 0, sizeof(Command));
            current_command = current_command->next;
            current_command->infile = NULL;
            current_command->outfile = NULL;
            current_command->background = 0;
        }
        else {
            current_command->args[i] = tok;
            i++;
        }
        
        tok = strtok(NULL, DELIMITERS);
    }
    current_command->args[i] = NULL;
    current_command->nr_args = i;
}