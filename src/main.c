#include "common.h"
#include "misc.h"
#include "game.h"
#include "points.h"

#include <time.h>
#include <unistd.h>

bool colors_on = 0 ;
void print_help(void){
    printf("placeholder\n");
}

int main(int argc , char ** argv){
    
    //parse options 

    uint64_t opt_mask = 0;
    char c;
     while ((c = getopt(argc, argv, "hx:y:n:fsc")) != -1) {
        
        switch (c) {
        case 'h': //help
            opt_mask |= 1; 
            break;
        case 'x': //change def_distx global
            opt_mask |= 1<<1;
            def_distx = (uint32_t) atoi(optarg) < 4 ? def_distx : (uint32_t)atoi(optarg) ;
            break;    
        case 'y': //change def_disty global
            opt_mask |= 1<<2;
            def_disty = (uint32_t)atoi(optarg) < 4 ? def_disty :(uint32_t) atoi(optarg);
            break;  
        case 'n': //change the row size of the lattice
            opt_mask |= 1<<3;
            row_size = (uint32_t)atoi(optarg) == 0 ? row_size : (uint32_t)atoi(optarg);
            break; 
        case 'f': //unsets fancy mode
            opt_mask |= 1<<4;
            fancy_mode = !fancy_mode;
            break;  
        case 's': //prints high scores and exits
            opt_mask |= 1<<5;
            printf("placeholder : score saving isn't implemented yet\n");
            break;  
        case 'c': //prints high scores and exits
            opt_mask |= 1<<6;
            colors_on = false ; 
            break; 
        case '?':
            
            fprintf(stderr, "Unknown option character `\\x%x'.", optopt);          
            break;
        default:
            abort();
        }
    }

    if(opt_mask & 1){
        printf("Edgerunner is a rogue-like where the character '@' tries to reach the exit node in a graph '%%' and avoid ennemies '!'\nFor details on usage check out the docs section.\n");
        return ERR_OK;
    }

    initscr();
    
    //handles size problems
    if(opt_mask & 0xE){
        int x,y;
        getmaxyx(stdscr, y,x);
        if(fancy_mode){
            while(def_disty > 4){
                if( (int) ( (def_disty+4)*row_size )<  y){
                    break;
                }
                def_disty--;
            }
            while(def_distx > 4){
                if( (int) ( (def_distx+4)*row_size ) <  x){
                    break;
                }
                def_distx--;
            }
            if(def_disty == 3 || def_distx == 3){
                printf("Edgerunner needs a window of size %ux%u minimum to play in fancy mode with a row_size of %u, try the ugly mode with the option -f or use a smaller row_size with -n [arg]\n", 8*row_size, 8*row_size, row_size);
                return 0;
            }
        }else{
            while(def_disty > 1){
                if( (int) ( (def_disty+2)*row_size )<  y){
                    break;
                }
                def_disty--;
            }
            while(def_distx > 1){
                if( (int) ( (def_distx+2)*row_size ) <  x){
                    break;
                }
                def_distx--;
            }
            if(def_disty == 0 || def_distx == 0){
                printf("Edgerunner needs a window of size %ux%u minimum to play in fancy mode with a row_size of %u, try the ugly mode with the option -f or use a smaller row_size with -n [arg]\n", 2*row_size, 2*row_size, row_size);
                return 0;
            }
        }
    }

    colors_on = false ; 
    if( has_colors() && !(opt_mask & 1<<6)){
        colors_on = true ; 
        start_color();
        use_default_colors();
        init_pair(1, COLOR_GREEN, -1);
        init_pair(2, COLOR_RED, -1);
        init_pair(3, COLOR_BLUE, -1);
        init_pair(4, COLOR_YELLOW, -1);
        init_pair(5, COLOR_MAGENTA, -1);
        init_pair(6, -1, -1);

    }
    

    time_t t;
    time(&t);
    srand(t);

    noecho();
    curs_set(0);

    declare_graph(g);
    declare_dynarr(dynarr_points, darp);
  
   
    start_game(stdscr,&g,&darp);
    endwin();
   
    return 0;
}
