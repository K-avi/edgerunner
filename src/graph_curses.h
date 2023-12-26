#ifndef ER_GCURSES_H 
#define ER_GCURSES_H 

#include "common.h"
#include <ncurses.h>
#include "graph_base.h"
#include "dynarr.h"

err_flag wprintw_graph(WINDOW * w, dynarr_points * darp, uint32_t distx, uint32_t disty,  er_graph * g);

err_flag wprint_link(WINDOW * w, er_points * p1, er_points * p2, uint32_t distx, uint32_t disty);

#endif
