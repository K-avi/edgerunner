#include "ennemy.h"
#include "game.h"
#include "search_utils.h"

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
    def_err_handler(!en,"move_closest", ERR_NULL);
    def_err_handler(!en->cur_node,"move_closest", ERR_NULL);
    
    if(!en->cur_node->cur){
        return ERR_OK;
    }

    struct s_graph_entry * min_node = NULL ; 
    int64_t min_dist = INT64_MAX  ;

    for(uint32_t i = 0 ; i < en->cur_node->cur ; i++){
        int64_t tmp_dist = - 1 ;
        bfs_graph(g, en->cur_node->neighboors_ref[i],p->cur_node,&tmp_dist);
        if(tmp_dist != -1 && tmp_dist < min_dist){
            min_dist = tmp_dist ; 
            min_node = en->cur_node->neighboors_ref[i] ;
        }
    }
    def_err_handler(!min_node, "move_closest", ERR_NULL);
    
    if(g->printed_nodes[en->cur_node - g->adjacency_lists]){
        if(!fancy_mode){
            mvwprintw(w,en->y * def_disty, en->x * def_distx, "O");
        }else{
             mvwprintw(w,en->y * def_disty+1, en->x * def_distx+1, "O");
        }
    }else{
        if(!fancy_mode){
            mvwprintw(w,en->y * def_disty, en->x * def_distx, " ");
        }else{
             mvwprintw(w,en->y * def_disty+1, en->x * def_distx+1, " ");
        }
    }

    en->cur_node = min_node ; 
   // fprintf(stderr,"minnode %p\n", min_node);
    en->x = darp->elems[min_node - g->adjacency_lists].x;
    en->y = darp->elems[min_node - g->adjacency_lists].y;
  
    return ERR_OK;
}

