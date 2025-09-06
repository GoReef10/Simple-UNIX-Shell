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

void execute_command(Command command){

    pid_t pid = fork();
    // pid_t wpid;
    int status;

    if(pid < 0){
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }

    if(pid == 0){
        if(command.infile != NULL){
            int fd = open(command.infile, O_RDONLY);
            if (fd < 0) {
                perror("Could not open input file\n");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        if(command.outfile != NULL){
            int fd = open(command.outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("Could not open output file\n");
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        if(execvp(command.args[0], command.args) == -1)
            exit(EXIT_FAILURE);
    }
    else{
        if(!command.background){
            // wait(NULL);
            do{
                (void)waitpid(pid, &status, WUNTRACED);
                // wpid = waitpid(pid, &status, WUNTRACED);
                // if(wpid == -1){
                //     perror("waitpid failed");
                //     exit(EXIT_FAILURE);
                // }
            } while(!WIFEXITED(status) && !WIFSIGNALED(status));
        } 
    }
}

void execute_pipeline(Command *command){
    int prev_fd = -1;
    int fd[2];
    pid_t pid;
    Command *last_command = NULL;
    while(command != NULL){
        if(command->next != NULL){
            if(pipe(fd) == -1){
                perror("Pipe failed");
                exit(EXIT_FAILURE);
            }
        }
        pid = fork();
        if(pid < 0){
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }
        if(pid == 0){
            // Child processes
            if(prev_fd != -1){
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }
            if(command->next != NULL){
                dup2(fd[WRITE_END], STDOUT_FILENO);
                close(fd[READ_END]);
                close(fd[WRITE_END]);
            }
            if(command->outfile != NULL){
                int out_fd = open(command->outfile, O_WRONLY | O_CREAT | (command->append ? O_APPEND : O_TRUNC), 0644);
                if(out_fd < 0){
                    perror("Could not open output file\n");
                    exit(EXIT_FAILURE);
                }
                dup2(out_fd, STDOUT_FILENO);
                close(out_fd);
            }
            if(command->infile != NULL){
                int in_fd = open(command->infile, O_RDONLY);
                if(in_fd < 0){
                    perror("Could not open input file\n");
                    exit(EXIT_FAILURE);
                }
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }
            if(execvp(command->args[0], command->args) == -1){
                perror("Execution failed");
                exit(EXIT_FAILURE);
            }
        }
        // parent process
        if(prev_fd != -1){
            close(prev_fd);
        }
        if(command->next != NULL){
            close(fd[WRITE_END]);
            prev_fd = fd[READ_END];
        }
        last_command = command;
        command = command->next;
    }
    if(!last_command->background){
        while(wait(NULL) > 0);
    }
}