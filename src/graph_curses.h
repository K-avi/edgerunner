#ifndef ER_GCURSES_H 
#define ER_GCURSES_H 

#include "common.h"
#include <curses.h>
#include "graph_base.h"
#include "misc.h"
#include "points.h"
#include "ennemy.h"

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
extern err_flag wprint_node_fancy(WINDOW * w, const er_points * p1, uint32_t distx, uint32_t disty, int color);
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
err_flag update_gprint_fancy(WINDOW * w,  er_graph * g, dynarr_points * darp, er_game_entities * gent);

#endif
