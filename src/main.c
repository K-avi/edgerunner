#include "common.h"
#include "errflags.h"
#include "game.h"

void print_help(void){
    printf("placeholder\n");
}

int main(int argc , char ** argv){
    
    //parse options 
    if(!argc){
        print_help(); 
        return 0 ;
    }

    err_flag failure = start_game();
    def_err_handler(failure, "main", failure);
    
    return 0;
}
