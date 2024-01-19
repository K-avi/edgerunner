#ifndef ER_PIA_H 
#define ER_PIA_H 

#include "common.h"
#include "graph_base.h"
#include "ennemy.h"
#include "entities.h"
/*
___________    .___            __________                                  
\_   _____/  __| _/ ____   ____\______   \__ __  ____   ____   ___________ 
 |    __)_  / __ | / ___\_/ __ \|       _/  |  \/    \ /    \_/ __ \_  __ \
 |        \/ /_/ |/ /_/  >  ___/|    |   \  |  /   |  \   |  \  ___/|  | \/
/_______  /\____ |\___  / \___  >____|_  /____/|___|  /___|  /\___  >__|   
        \/      \/_____/      \/       \/           \/     \/     \/       

player_ai.h defines functions for an IA that can replace the player 
*/

extern err_flag ai_shortest_path(WINDOW * w, er_game_entities * gent, er_graph * g, dynarr_points * darp, int64_t * dest);

#endif
