// #include "shell.h"

// #define MAX_INPUT_SIZE 1024
// #define MAX_PATH_LEN 1024
// #define NAME_LEN 100

// char *truncate_path(char* long_path){

//     int length = strlen(long_path);
    
//     char *short_path = (char*)malloc((length + 1) * sizeof(char));
//     if(short_path == NULL){

//         perror("Memory allocation failed");
//         exit(1);
//     }
//     short_path[0] = '\0';
    
//     char *tok;
//     tok = strtok(long_path, "/");
//     while(tok != NULL){

//         if(strcmp(tok, "home") == 0){
//             tok = strtok(NULL, "/");
//             strcat(short_path, "~/");
//         }
//         else{
//             strcat(short_path, tok);
//             strcat(short_path, "/");
//         }
//         tok = strtok(NULL, "/");
//     }
//     short_path[strlen(short_path)-1] = '\0';
    
//     return short_path;
// }

// void interactive_shell(){

//     char input[MAX_INPUT_SIZE];
//     char hostname[NAME_LEN];
//     char username[NAME_LEN];
//     char path[MAX_PATH_LEN];
//     gethostname(hostname, sizeof(hostname));
//     getlogin_r(username, sizeof(username));

//     while(1){

//         getcwd(path, sizeof(path));
//         char *temp_path = truncate_path(path);
//         if(temp_path != NULL){

//             strcpy(path, temp_path);
//             free(temp_path);
//         }

//         printf("[%s@%s %s]: ", username, hostname, path);
        
//         //parser

//         fgets(input, sizeof(input), stdin);
//         input[strcspn(input, "\n")] = '\0';
        
//         if(strcmp(input, "exit") == 0){
//             exit(EXIT_SUCCESS);
//         }

//         int nr_args = 0;
//         char *args[100];
//         parser(input, args, &nr_args);
        
//         if(strcmp(args[0], "cd") == 0){
//             if(args[1] == NULL)
//                 chdir(getenv("HOME"));
//             else{
//                 if(chdir(args[1]) != 0)
//                     perror("Could not change to directory");
//             }
//         } else{

//             if(strcmp(args[nr_args-1], "&") == 0){
                
//                 args[nr_args-1] = NULL;
//                 pid_t pid = fork();
//                 if(pid < 0){
//                     perror("Fork failed");
//                     exit(EXIT_FAILURE);
//                 }
//                 else if(pid == 0){

//                     execvp(args[0], args);
                    
//                 }
//             }
//             else{
                
//                 pid_t pid = fork();
//                 if(pid < 0){
                    
//                     perror("Fork failed");
//                     exit(EXIT_FAILURE);
//                 }
//                 else if(pid == 0){

//                     execvp(args[0], args);

//                 }
//                 else {
//                     wait(NULL);
//                 }
//             }
//         }
//     }
// }

// #include "shell.h"

// #define DELIMITERS " \n"
// #define BUFFER_SIZE 64
// #define COMMAND_NR 100

// void parser(char *input, char *args[], int *nr_args){

//     char *tok = strtok(input, DELIMITERS);
//     int i = 0;
//     while(tok != NULL && i < COMMAND_NR){

//         tokens[i] = tok;
//         i++;
//         tok = strtok(NULL, DELIMITERS);
//     }
//     tokens[i] = NULL;
//     *nr_args = i;
// }

// //
// #include "shell.h"
// /*
// int main(){


//     if(isatty(STDIN_FILENO) == 1){
//         interactive_shell();
//     }
//     // else{
//     //     non_interactive_shell();
//     // }
//     return 0;
// }
// */

// char *truncate_path(char* long_path){

//     int length = strlen(long_path);
    
//     char *short_path = (char*)malloc((length + 1) * sizeof(char));
//     if(short_path == NULL){

//         perror("Memory allocation failed");
//         exit(1);
//     }
//     short_path[0] = '\0';
    
//     char *tok;
//     tok = strtok(long_path, "/");
//     while(tok != NULL){

//         if(strcmp(tok, "home") == 0){
//             tok = strtok(NULL, "/");
//             strcat(short_path, "~/");
//         }
//         else{
//             strcat(short_path, tok);
//             strcat(short_path, "/");
//         }
//         tok = strtok(NULL, "/");
//     }
//     short_path[strlen(short_path)-1] = '\0';
    
//     return short_path;
// }
    
// void main(){

//     char input[MAX_INPUT_SIZE];
//     char hostname[NAME_LEN];
//     char username[NAME_LEN];
//     char path[MAX_PATH_LEN];
//     gethostname(hostname, sizeof(hostname));
//     getlogin_r(username, sizeof(username));
//     int run = 1;

//     while(run){

//         getcwd(path, sizeof(path));
//         char *temp_path = truncate_path(path);
//         if(temp_path != NULL){

//             strcpy(path, temp_path);
//             free(temp_path);
//         }

//         printf("[%s@%s %s]: ", username, hostname, path);
//         fflush(stdout);
//         //parser
        
//         if(fgets(input, sizeof(input), stdin) == NULL){
//             perror("Failed to read input\n");
//             continue;
//         }
//         input[strcspn(input, "\n")] = '\0';

//         if(strcmp(input, "exit") == 0){
//             run = 0;
//             exit(EXIT_SUCCESS);
//         }

//         else{
//             int nr_args = 0;
//             char *args[100];
//             parser(input, args, &nr_args);
            
//             if(strcmp(args[0], "cd") == 0){
//                 if(args[1] == NULL)
//                     chdir(getenv("HOME"));
//                 else{
//                     if(chdir(args[1]) != 0)
//                         perror("Could not change to directory");
//                 }
//             } else{

//                 if(strcmp(args[nr_args-1], "&") == 0){
                    
//                     args[nr_args-1] = NULL;
//                     pid_t pid = fork();
//                     if(pid < 0){
//                         perror("Fork failed");
//                         exit(EXIT_FAILURE);
//                     }
//                     else if(pid == 0){

//                         execvp(args[0], args);
                        
//                     }
//                 }
//                 else{
                    
//                     pid_t pid = fork();
//                     if(pid < 0){
                        
//                         perror("Fork failed");
//                         exit(EXIT_FAILURE);
//                     }
//                     else if(pid == 0){

//                         execvp(args[0], args);

//                     }
//                     else {
//                         wait(NULL);
//                     }
//                 }
//             }
//         }
//     }
// }

// /* #include "shell.h"

// #define DELIMITERS " \n"
// #define BUFFER_SIZE 64
// #define COMMAND_NR 100

// char **parser(char *input, int *nr_args){

//     char **tokens = (char **)malloc((BUFFER_SIZE) * sizeof(char *));
//     if(tokens == NULL){

//         perror("Memory allocation failed (parser)\n");
//         exit(EXIT_FAILURE);
//     }

//     char *tok = strtok(input, DELIMITERS);
//     int i = 0;
//     while(tok != NULL){

//         if(i >= BUFFER_SIZE){
//             fprintf(stderr, "Input size exceeded\n");
//             exit(EXIT_FAILURE);
//         }

//         tokens[i] = tok;
//         i++;
//         tok = strtok(NULL, DELIMITERS);
//     }
//     tokens[i] = NULL;
//     *nr_args = i;
    
//     return tokens;
// }
// */
// #include "shell.h"

// #define DELIMITERS " \n"
// #define BUFFER_SIZE 64
// #define COMMAND_NR 100

// void parser(char *input, char *args[], int *nr_args){

//     char *tok = strtok(input, DELIMITERS);
//     int i = 0;
//     while(tok != NULL && i < COMMAND_NR){

//         args[i] = tok;
//         i++;
//         tok = strtok(NULL, DELIMITERS);
//     }
//     args[i] = NULL;
//     *nr_args = i;
// }

// #include "shell.h"

// #define MAX_INPUT_SIZE 1024
// #define MAX_PATH_LEN 1024
// #define NAME_LEN 100

// char *truncate_path(char* long_path){

//     int length = strlen(long_path);
    
//     char *short_path = (char*)malloc((length + 1) * sizeof(char));
//     if(short_path == NULL){

//         perror("Memory allocation failed");
//         exit(1);
//     }
//     short_path[0] = '\0';
    
//     char *tok;
//     tok = strtok(long_path, "/");
//     while(tok != NULL){

//         if(strcmp(tok, "home") == 0){
//             tok = strtok(NULL, "/");
//             strcat(short_path, "~/");
//         }
//         else{
//             strcat(short_path, tok);
//             strcat(short_path, "/");
//         }
//         tok = strtok(NULL, "/");
//     }
//     short_path[strlen(short_path)-1] = '\0';
    
//     return short_path;
// }

// void interactive_shell(){

//     char input[MAX_INPUT_SIZE];
//     char hostname[NAME_LEN];
//     char username[NAME_LEN];
//     char path[MAX_PATH_LEN];
//     gethostname(hostname, sizeof(hostname));
//     getlogin_r(username, sizeof(username));
//     int run = 1;

//     while(run){

//         getcwd(path, sizeof(path));
//         char *temp_path = truncate_path(path);
//         if(temp_path != NULL){

//             strcpy(path, temp_path);
//             free(temp_path);
//         }

//         printf("[%s@%s %s]: ", username, hostname, path);
        
//         //parser
        
//         if(fgets(input, sizeof(input), stdin) == NULL){
//             perror("Failed to read input\n");
//             continue;
//         }
//         input[strcspn(input, "\n")] = '\0';
        
//         if(strcmp(input, "exit") == 0){
//             run = 0;
//             exit(EXIT_SUCCESS);
//         }

//         int nr_args = 0;
//         char *args[100];
//         parser(input, args, &nr_args);
        
//         if(strcmp(args[0], "cd") == 0){
//             if(args[1] == NULL)
//                 chdir(getenv("HOME"));
//             else{
//                 if(chdir(args[1]) != 0)
//                     perror("Could not change to directory");
//             }
//         } else{

//             if(strcmp(args[nr_args-1], "&") == 0){
                
//                 args[nr_args-1] = NULL;
//                 pid_t pid = fork();
//                 if(pid < 0){
//                     perror("Fork failed");
//                     exit(EXIT_FAILURE);
//                 }
//                 else if(pid == 0){

//                     execvp(args[0], args);
                    
//                 }
//             }
//             else{
                
//                 pid_t pid = fork();
//                 if(pid < 0){
                    
//                     perror("Fork failed");
//                     exit(EXIT_FAILURE);
//                 }
//                 else if(pid == 0){

//                     execvp(args[0], args);

//                 }
//                 else {
//                     wait(NULL);
//                 }
//             }
//         }
//     }
// }