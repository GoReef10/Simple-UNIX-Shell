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
    //int redirect = 0;
    int background = 0;
    char *infile = NULL;
    char *outfile = NULL;
    char *args[COMMAND_NR];

    while(run){

        //print_prompt(hostname, username, path, sizeof(path), temp_path);
        //parser
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
        
        if(execute_builtins(input, &history, username, hostname, path, &run))
            continue;
        //builtin commands
        // if(strcmp(input, "!!") == 0){
        //     if(history == NULL){
        //         printf("No command in history\n");
        //         continue;
        //     }
        //     else{
        //         strcpy(input, history);
        //         printf("[%s@%s %s]: %s\n", username, hostname, path, input);
        //         fflush(stdout);
        //     }
        // } 
        // else{
        //     free(history);
        //     history = NULL;
        //     history = malloc(sizeof(char) * (strlen(input) + 1));
        //     strcpy(history, input);
        // }

        // if(strcmp(input, "exit") == 0){
        //     run = 0;
        //     free(history);
        //     exit(EXIT_SUCCESS);
        // }
        execute_command(input, args, infile, outfile, background);
        // else{

        //     int nr_args = 0;
        //     char *args[100];
        //     parser(input, args, &nr_args, &infile, &outfile);
        //     if(nr_args == 0)
        //         continue;
        //     // implement cd
        //     // probably should move this at the start of the if block or even 
        //             // outside the block
        //     int i = 0;
        //     for (i = 0; i < nr_args; i++) {
        //         if(strcmp(args[i], "<") == 0){
        //             args[i] = NULL;
        //             if(nr_args - i < 2){
        //                 printf("No input file specified\n");
        //                     break;
        //             }
        //             else{
        //                 infile = args[i + 1];
        //                 redirect = 1;
        //             }
        //         }
        //     }
        //     if(strcmp(args[0], "cd") == 0){
        //         if(args[1] == NULL)
        //             chdir(getenv("HOME"));
        //         else{
        //             if(chdir(args[1]) != 0)
        //                 perror("Could not change to directory");
        //         }
        //     } else{

        //         if(strcmp(args[nr_args-1], "&") == 0){
                    
        //             args[nr_args-1] = NULL;
        //             pid_t pid = fork();
        //             if(pid < 0){
        //                 perror("Fork failed");
        //                 exit(EXIT_FAILURE);
        //             }
        //             else if(pid == 0){
                        
        //                 if(redirect){
        //                     int fd = open(infile, O_RDONLY);
        //                     if (fd < 0) {
        //                         perror("Could not open input file\n");
        //                         exit(EXIT_FAILURE);
        //                     }
        //                     dup2(fd, STDIN_FILENO);
        //                     close(fd);
        //                     args[i + 1] = NULL;
        //                     args[i] = NULL;
        //                 }
        //                 if(execvp(args[0], args) == -1)
        //                     exit(EXIT_FAILURE);
        //             }
        //         }
        //         else{
                    
        //             pid_t pid = fork();
        //             if(pid < 0){
                        
        //                 perror("Fork failed");
        //                 exit(EXIT_FAILURE);
        //             }
        //             else if(pid == 0){

        //                 if(redirect){
        //                     int fd = open(infile, O_RDONLY);
        //                     if (fd < 0) {
        //                         perror("Could not open input file\n");
        //                         exit(EXIT_FAILURE);
        //                     }
        //                     dup2(fd, STDIN_FILENO);
        //                     close(fd);
        //                     args[i + 1] = NULL;
        //                     args[i] = NULL;
        //                 }
        //                 if(execvp(args[0], args) == -1)
        //                     exit(EXIT_FAILURE);
        //             }
        //             else {
        //                 wait(NULL);
        //             }
        //         }
        //     }
        // }
    }
}