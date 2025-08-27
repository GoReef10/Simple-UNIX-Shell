#include "shell.h"

int main(){

    // weird stuff happens with tasks running in the background (see firefox and then ls as commands in the terminal)
    if(isatty(STDIN_FILENO) == 1){
        interactive_shell();
    }
    // else{
    //     non_interactive_shell();
    // }
    return 0;
}