#include "player_curses.h"
#include "dynarr.h"
#include "ncurses.h"

static err_flag init_player_pos(er_player * pl , er_graph * g,  dynarr_points * darp, uint32_t * player_index){
    /*
    darp -> not null & init 
    pl -> not null & init
    */

   uint32_t pl_index = rand()%darp->cur ;

    pl->x = darp->elems[pl_index].x;
    pl->y = darp->elems[pl_index].y;
    pl->cur_node = &g->adjacency_lists[pl_index];
    *player_index = pl_index;
   return ERR_OK;
}

static err_flag init_exit_pos(er_exit * ex , er_graph * g,  dynarr_points * darp, uint32_t player_index){
    /*
    darp -> not null & init 
    pl -> not null & init
    */

   uint32_t ex_index = rand()%darp->cur ;
   while(ex_index == player_index){
    ex_index = rand()%darp->cur;
   }

    ex->x = darp->elems[ex_index].x;
    ex->y = darp->elems[ex_index].y;
    ex->cur_node = &g->adjacency_lists[ex_index];

   return ERR_OK;
}

err_flag init_ent_pos(er_exit * ex, er_player* pl, er_graph * g,  dynarr_points * darp){
    
    uint32_t pl_i;
    init_player_pos(pl,g,darp,&pl_i);
    init_exit_pos(ex,g,darp,pl_i);
    return ERR_OK;
}

err_flag wprint_entity(WINDOW * w , er_player * pl, uint32_t distx, uint32_t disty){
    
    wmove(w,pl->y*disty, pl->x*distx);
    waddch(w,pl->ch);
    wrefresh(w);
    return ERR_OK;
}
