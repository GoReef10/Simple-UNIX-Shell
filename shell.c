#include "shell.h"

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

    while(run){

        getcwd(path, sizeof(path));
        temp_path = truncate_path(path);
        if(temp_path != NULL){

            strcpy(path, temp_path);
            free(temp_path);
        }

        printf("[%s@%s %s]: ", username, hostname, path);
        fflush(stdout);
        //parser
        
        if(fgets(input, sizeof(input), stdin) == NULL){
            perror("Failed to read input\n");
            continue;
        }
        input[strcspn(input, "\n")] = '\0';
        
        if(strcmp(input, "!!") == 0){
            if(history == NULL){
                printf("No command in history\n");
                continue;
            }
            else{
                strcpy(input, history);
                printf("[%s@%s %s]: %s\n", username, hostname, path, input);
                fflush(stdout);
            }
        } 
        else{
            free(history);
            history = NULL;
            history = malloc(sizeof(char) * (strlen(input) + 1));
            strcpy(history, input);
        }

        if(strcmp(input, "exit") == 0){
            run = 0;
            free(history);
            exit(EXIT_SUCCESS);
        }
        else{

            int nr_args = 0;
            char *args[100];
            parser(input, args, &nr_args);
            if(nr_args == 0)
                continue;
            if(strcmp(args[0], "cd") == 0){
                if(args[1] == NULL)
                    chdir(getenv("HOME"));
                else{
                    if(chdir(args[1]) != 0)
                        perror("Could not change to directory");
                }
            } else{

                if(strcmp(args[nr_args-1], "&") == 0){
                    
                    args[nr_args-1] = NULL;
                    pid_t pid = fork();
                    if(pid < 0){
                        perror("Fork failed");
                        exit(EXIT_FAILURE);
                    }
                    else if(pid == 0){

                        if(execvp(args[0], args) == -1);
                            exit(EXIT_FAILURE);
                    }
                }
                else{
                    
                    pid_t pid = fork();
                    if(pid < 0){
                        
                        perror("Fork failed");
                        exit(EXIT_FAILURE);
                    }
                    else if(pid == 0){

                        if(execvp(args[0], args) == -1);
                            exit(EXIT_FAILURE);

                    }
                    else {
                        wait(NULL);
                    }
                }
            }
        }
    }
}