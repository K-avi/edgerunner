#ifndef ER_GCURSES_H 
#define ER_GCURSES_H 

#include "common.h"
#include <curses.h>
#include "graph_base.h"
#include "dynarr.h"

extern err_flag wprintw_graph(WINDOW * w, dynarr_points * darp, uint32_t distx, uint32_t disty,  er_graph * g);
extern err_flag wprint_link(WINDOW * w, const er_points * p1, const er_points * p2, uint32_t distx, uint32_t disty);

extern err_flag wprint_node_fancy(WINDOW * w, const er_points * p1, uint32_t distx, uint32_t disty);
extern err_flag wprint_link_fancy(WINDOW * w, const er_points * p1, const er_points * p2, uint32_t distx, uint32_t disty);
#endif
