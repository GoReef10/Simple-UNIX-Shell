#include "shell.h"

int execute_builtins(char *input, char *args[], char **history, char *username, char *hostname, char *path, int *run)
{
    if(strcmp(args[0], "!!") == 0){
        if(*history == NULL){
            printf("No command in history\n");
        }
        else{
            // implement print promt to do this (get username, hostname...)
            printf("[%s@%s %s]: %s\n", username, hostname, path, *history);
            fflush(stdout);
        }
        return 1;
    } 
    else{
        if(*history){
            free(*history);
            *history = NULL;
        }
        *history = malloc(sizeof(char) * (strlen(input) + 1));
        strcpy(*history, input);
    }
    if(strcmp(args[0], "exit") == 0){
        *run = 0;
        free(*history);
        exit(EXIT_SUCCESS);
    }
    if(strcmp(args[0], "cd") == 0){
        if(args[1] == NULL)
            chdir(getenv("HOME"));
        else{
            if(chdir(args[1]) != 0)
                perror("Could not change to directory");
        }
        return 1;
    }
    return 0;
}

void execute_command(char *args[], char* infile, char* outfile, int background){

    pid_t pid = fork();
    pid_t wpid;
    int status;

    if(pid < 0){
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if(pid == 0){
        if(infile != NULL){
            int fd = open(infile, O_RDONLY);
            if (fd < 0) {
                perror("Could not open input file\n");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        if(outfile != NULL){
            int fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("Could not open output file\n");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
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