#include "player_ai.h"
#include "search_utils.h"
#include "game.h"

err_flag ai_shortest_path(WINDOW * w, er_game_entities * gent, er_graph * g, dynarr_points * darp, int64_t * dest){
    /*
    */
    def_err_handler(!w, "ai_shortest_path w", ERR_NULL);
    def_err_handler(!gent, "ai_shortest_path gent", ERR_NULL);
    def_err_handler(!g, "ai_shortest_path g", ERR_NULL);
    def_err_handler(!darp, "ai_shortest_path darp", ERR_NULL);
    def_err_handler(!dest, "ai_shortest_path dest", ERR_NULL);

    int64_t dist = -1 ; 
    int64_t min = INT64_MAX ;
    
    int64_t chosen_node_index = -1 ;

    for(uint32_t i = 0 ; i < gent->p->cur_node->cur; i++){ 
        err_flag failure = bfs_graph(g, gent->p->cur_node->neighboors_ref[i], gent->ex->cur_node, &dist );
        def_err_handler(failure, "ai_shortest_path", failure);
        def_err_handler( (dist==-1), "ai_shortest_path", ERR_VALS);
        
        
        //fprintf(stderr, "i=%u *dist=%li chni=%li\n", i,dist, chosen_node_index);

        if(dist < min && dist != -1){
            chosen_node_index = i;
            min = dist ; 
        }
    }
    
    *dest = chosen_node_index ;
    //fprintf(stderr, "p.cn=%d *dest=%li\n", gent->p->cur_node - g->adjacency_lists, *dest);
    return ERR_OK;
}//not done 
