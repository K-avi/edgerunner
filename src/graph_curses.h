#ifndef ER_GCURSES_H 
#define ER_GCURSES_H 

#include "common.h"
#include "graph_base.h"
#include "misc.h"
#include "points.h"
#include "ennemy.h"
/*
___________    .___            __________                                  
\_   _____/  __| _/ ____   ____\______   \__ __  ____   ____   ___________ 
 |    __)_  / __ | / ___\_/ __ \|       _/  |  \/    \ /    \_/ __ \_  __ \
 |        \/ /_/ |/ /_/  >  ___/|    |   \  |  /   |  \   |  \  ___/|  | \/
/_______  /\____ |\___  / \___  >____|_  /____/|___|  /___|  /\___  >__|   
        \/      \/_____/      \/       \/           \/     \/     \/       

graph_curses.h defines the functions used to print the graph in the command line.
*/

#ifdef debug 
extern err_flag wprintw_graph(WINDOW * w, dynarr_points * darp, uint32_t distx, uint32_t disty,  er_graph * g);
//mostly used for debugging / testing purposes
#endif 

extern err_flag wprint_link(WINDOW * w, const er_points * p1, const er_points * p2, uint32_t distx, uint32_t disty);
/*
    w -> not null 
    p1 , p2 -> not null & initialized

    the uglier print function, used when fancy mode is turned off
*/
/*
     w -> not null 
    p1 -> not null & initialized

    the fancier print function used by default
*/
extern err_flag wprint_link_fancy(WINDOW * w, const er_points * p1, const er_points * p2, uint32_t distx, uint32_t disty,  int c1, int c2);
/*
    w -> not null 
    p1 , p2 -> not null & initialized

    the fancier print function used by default 
*/
extern err_flag update_gprint_fancy(WINDOW * w,  er_graph * g, dynarr_points * darp, er_game_entities * gent);
/*
    w, g, darp, gent -> not null 
    g,darp -> initialized ^ matching 

    this function is a way to update the screen , it will refresh the color of the nodes 
    (special colors for visited nodes, current node , ennemy node, exit node), print the node
    surrounding the player, print the entities, and update the links printed on screen 
*/
#endif
