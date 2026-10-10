#include "../include/shell.h"

char *truncate_path(char* long_path){

    int length = strlen(long_path);
    
    char *short_path = (char*)malloc((length + 1) * sizeof(char));
    if(short_path == NULL){

        perror("Memory allocation failed");
        exit(1);
    }
    short_path[0] = '\0';
    
    char *tok;
    tok = strtok(long_path, "/");
    while(tok != NULL){

        if(strcmp(tok, "home") == 0){
            tok = strtok(NULL, "/");
            strcat(short_path, "~/");
        }
        else{
            strcat(short_path, tok);
            strcat(short_path, "/");
        }
        tok = strtok(NULL, "/");
    }
    short_path[strlen(short_path)-1] = '\0';
    
    return short_path;
}

void print_prompt(char *hostname, char *username, char *path, int path_size, char *temp_path){
    
    getcwd(path, path_size);
    temp_path = truncate_path(path);
    if(temp_path != NULL){

        strcpy(path, temp_path);
        free(temp_path);
    }

    printf("[%s@%s %s]: ", username, hostname, path);
    fflush(stdout);
}
    
void interactive_shell(){

    char input[MAX_INPUT_SIZE];
    char hostname[NAME_LEN];
    char username[NAME_LEN];
    char path[MAX_PATH_LEN];
    char *temp_path = NULL;
    char *history = NULL;
    gethostname(hostname, sizeof(hostname));
    getlogin_r(username, sizeof(username));
    int run = 1;
    char *input_copy = NULL;
    while(run){

        while(waitpid(-1, NULL, WNOHANG) > 0);

        Command *command = malloc(sizeof(Command));
        memset(command, 0, sizeof(Command));
        //print_prompt(hostname, username, path, sizeof(path), temp_path);
        getcwd(path, sizeof(path));
        temp_path = truncate_path(path);
        if(temp_path != NULL){

            strcpy(path, temp_path);
            free(temp_path);
        }
        // printf("\r");
        printf("[%s@%s %s]: ", username, hostname, path);
        fflush(stdout);
        
        if(fgets(input, sizeof(input), stdin) == NULL){
            perror("Failed to read input\n");
            continue;
        }
        input[strcspn(input, "\n")] = '\0';
        
        // if (input[0] == '\0') 
        //     continue;

        //parsing input
        input_copy = (char*)malloc((strlen(input) + 1) * sizeof(char));
        strcpy(input_copy, input);
        parser(input_copy, command);
        
        //handle execution
        if(command->nr_args == 0)
            continue;

        if(execute_builtins(input, command->args, &history, username, hostname, path, &run))
            continue;

        // execute_command(*command);
        // int fd[2];
        // if(command->next != NULL){
        //     if(pipe(fd) == -1){
        //         perror("Pipe failed");
        //         continue;
        //     }
        //     pid_t pid1 = fork();
        //     if(pid1 < 0){
        //         perror("Fork failed");
        //         continue;
        //     }
        //     if(pid1 == 0){
                
        //         dup2(fd[WRITE_END], STDOUT_FILENO);
        //         close(fd[READ_END]);
        //         close(fd[WRITE_END]);
        //         execvp(command->args[0], command->args);
        //     }
        //     pid_t pid2 = fork();
        //     if(pid2 < 0){
        //         perror("Fork failed");
        //         continue;
        //     }
        //     if(pid2 == 0){
                
        //         dup2(fd[READ_END], STDIN_FILENO);
        //         close(fd[WRITE_END]);
        //         close(fd[READ_END]);
        //         execvp(command->next->args[0], command->next->args);
        //     }
        //     close(fd[READ_END]);
        //     close(fd[WRITE_END]);
        //     waitpid(pid1, NULL, 0);
        //     waitpid(pid2, NULL, 0);
        // }
        // else 
        //     execute_command(*command);

        execute_pipeline(command);
        
        while(command != NULL){
            Command *temp = command;
            command = command->next;
            free(temp);
        }
        free(input_copy);
    }
}