#include "shell.h"

int execute_builtins(char *input, char **history, char *username, char *hostname, char *path, int *run)
{
    if(strcmp(input, "!!") == 0){
        if(*history == NULL){
            printf("No command in history\n");
        }
        else{
            // could probably implement print promt to do this (get username, hostname...)
            // will see
            strcpy(input, *history);
            printf("[%s@%s %s]: %s\n", username, hostname, path, input);
            fflush(stdout);
        }
        return 1;
    } 
    else{
        free(*history);
        *history = NULL;
        *history = malloc(sizeof(char) * (strlen(input) + 1));
        strcpy(*history, input);
    }
    if(strcmp(input, "exit") == 0){
        *run = 0;
        free(*history);
        exit(EXIT_SUCCESS);
    }
    return 0;
}

void execute_command(char* input, char *args[], char* infile, char* outfile, int background){

    int nr_args = 0;
    parser(input, args, &nr_args, &infile, &outfile, &background);
    if(nr_args == 0)
         return;

    pid_t pid = fork();
    pid_t wpid;
    int status;

    if(pid < 0){
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if(infile != NULL){
        int fd = open(infile, O_RDONLY);
        if (fd < 0) {
            perror("Could not open input file\n");
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }

    if(pid == 0){
        if(execvp(args[0], args) == -1)
            exit(EXIT_FAILURE);
    }
    else{
        if(!background){
            wait(NULL);
            do{
                wpid = waitpid(pid, &status, WUNTRACED);
            } while(!WIFEXITED(status) && !WIFSIGNALED(status));
        }
            
    }
}