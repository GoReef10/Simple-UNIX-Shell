#include "shell.h"

int main(){


    if(isatty(STDIN_FILENO) == 1){
        interactive_shell();
    }
    // else{
    //     non_interactive_shell();
    // }
    return 0;
}