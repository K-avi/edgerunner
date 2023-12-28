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

    initscr();
    noecho();
    curs_set(0);

    declare_graph(g);
    declare_dynnar(dynarr_points, darp);
  
    declare_er_player(p,0,0);
   
    start_game(stdscr,&g,&darp, &p);
    endwin();
   
    return 0;
}
