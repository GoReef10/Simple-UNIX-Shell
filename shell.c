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
    int background = 0;
    int nr_args = 0;
    char *infile = NULL;
    char *outfile = NULL;
    char *args[COMMAND_NR];
    char* input_copy = NULL;
    
    while(run){

        //print_prompt(hostname, username, path, sizeof(path), temp_path);
        getcwd(path, sizeof(path));
        temp_path = truncate_path(path);
        if(temp_path != NULL){

            strcpy(path, temp_path);
            free(temp_path);
        }

        printf("[%s@%s %s]: ", username, hostname, path);
        fflush(stdout);
        
        if(fgets(input, sizeof(input), stdin) == NULL){
            perror("Failed to read input\n");
            continue;
        }
        input[strcspn(input, "\n")] = '\0';
        
        //parsing input
        input_copy = (char*)malloc((strlen(input) + 1) * sizeof(char));
        strcpy(input_copy, input);
        parser(input_copy, args, &nr_args, &infile, &outfile, &background);
        
        //handle execution
        if(nr_args == 0)
            continue;

        if(execute_builtins(input, args, &history, username, hostname, path, &run))
            continue;

        execute_command(args, infile, outfile, background);
        free(input_copy);
    }
}