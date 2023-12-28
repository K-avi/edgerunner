#ifndef ER_GAME_H 
#define ER_GAME_H 
/*
___________    .___            __________                                  
\_   _____/  __| _/ ____   ____\______   \__ __  ____   ____   ___________ 
 |    __)_  / __ | / ___\_/ __ \|       _/  |  \/    \ /    \_/ __ \_  __ \
 |        \/ /_/ |/ /_/  >  ___/|    |   \  |  /   |  \   |  \  ___/|  | \/
/_______  /\____ |\___  / \___  >____|_  /____/|___|  /___|  /\___  >__|   
        \/      \/_____/      \/       \/           \/     \/     \/       

game.h defines the game related functions.
*/

#include "common.h"
#include "errflags.h"
#include <ncurses.h>
#include "graph_base.h"
#include "player_curses.h"
#include "dynarr.h"
/*
call once in the main fn
*/
extern err_flag start_game(WINDOW * w, er_graph * g, dynarr_points * darp,er_player * p );


extern uint32_t def_distx ;
extern uint32_t def_disty ;
extern bool fancy_mode; 
#endif
