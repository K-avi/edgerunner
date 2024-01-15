#ifndef ER_GAME_H 
#define ER_GAME_H 

#include "common.h"
#include <curses.h>
#include "graph_base.h"
#include "entities.h"
/*
___________    .___            __________                                  
\_   _____/  __| _/ ____   ____\______   \__ __  ____   ____   ___________ 
 |    __)_  / __ | / ___\_/ __ \|       _/  |  \/    \ /    \_/ __ \_  __ \
 |        \/ /_/ |/ /_/  >  ___/|    |   \  |  /   |  \   |  \  ___/|  | \/
/_______  /\____ |\___  / \___  >____|_  /____/|___|  /___|  /\___  >__|   
        \/      \/_____/      \/       \/           \/     \/     \/       

game.h defines the game related functions and globals
*/

extern uint32_t def_distx ;
extern uint32_t def_disty ;
extern bool fancy_mode; 

extern err_flag start_game(WINDOW * w, er_graph * g, dynarr_points * darp );
/*
        w , g, darp -> not null 

        starts a single player game in the window w, the graph and darp are passed as arguments 
        but could be local variables to the function tbh
*/

#endif
