#ifndef ER_ENNEMY_H 
#define ER_ENNEMY_H 

#include "common.h"
#include "graph_base.h"
#include "dynarr.h"
#include "player_curses.h"

extern err_flag  move_random( WINDOW * w, er_ennemy * en , er_graph * g, dynarr_points * darp, er_exit * ex);
extern err_flag  move_closest(WINDOW* w,er_ennemy * e , er_graph * g, dynarr_points * darp, er_player * p );
extern err_flag  move_blockexit(er_ennemy * e , er_graph * g, dynarr_points * darp);

#endif 
