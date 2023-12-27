#include "ennemy.h"
#include "game.h"

err_flag move_random(WINDOW * w , er_ennemy * en , er_graph * g, dynarr_points * darp, er_exit * ex){
    /*
    */
    //don't go on the exit ennemy please !!!
    def_err_handler(!en, "move_random en", ERR_NULL);
    def_err_handler(!en->cur_node, "move_random en->cur_node", ERR_NULL);
    def_err_handler(!g, "move_random g", ERR_NULL);
    def_err_handler(!darp, "move_random darp", ERR_NULL);
    def_err_handler(!ex, "move_random en", ERR_NULL);

    if(en->cur_node->cur == 1 && ex->cur_node == en->cur_node->neighboors_ref[0]){
        return ERR_OK;
    }
    if(g->printed_nodes[en->cur_node - g->adjacency_lists]){
        mvwprintw(w,en->y * def_disty, en->x * def_distx, "O");
    }else{
        mvwprintw(w,en->y * def_disty, en->x * def_distx, " ");
    }
    uint32_t index = rand()%en->cur_node->cur;
    while(en->cur_node->neighboors_ref[index] == ex->cur_node){
        index = rand()%en->cur_node->cur;
    }

    en->x = darp->elems[ en->cur_node->neighboors_ref[index] - g->adjacency_lists].x;
    en->y = darp->elems[ en->cur_node->neighboors_ref[index] - g->adjacency_lists].y;  
    en->cur_node = en->cur_node->neighboors_ref[index];

    return ERR_OK;
}

err_flag move_closest(WINDOW * w , er_ennemy * en , er_graph * g, dynarr_points * darp, er_player * p){
    /* 
    uses a BFS to find the closest way to approach the player
    */
    return ERR_OK;
}

