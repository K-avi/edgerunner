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

typedef struct struct_heap_keyval{
        uint32_t cur ; 
        uint32_t max ;

        uint32_t * elems_key ; //node score
        uint32_t * elems_val ; //node index

        uint32_t nb_vals ;
        bool * val_in_heap ; 
}er_psheap;

#define declare_psheap(heap) er_psheap heap = {0,0,NULL,NULL};

typedef struct s_player_search_infos{
        
        er_dynarr_nodes * darn_seen ; 
        er_dynarr_nodes * darn_visited ; 

        er_psheap * nodes_heap ; 
}er_psinfo;

extern err_flag ai_shortest_path(WINDOW * w, er_game_entities * gent, er_graph * g, dynarr_points * darp, int64_t * dest);
/*
        w,gent,g,darp,dest -> not null 
        g,gent,darp -> initialized coherent (part of the same level)

        dest is the index of the NEIGHBOR of gent->p chosen
        simple AI function for the player. The destination chosen is the node on the shortest path 
        from gent->p to gent->ex 
*/

extern err_flag ai_realist(WINDOW * w, er_game_entities * gent, er_psinfo * psinfo, er_graph * g, dynarr_points * darp, int64_t * dest);
/*
        w,gent,g,darp,dest -> not null 
        g,gent,darp -> initialized coherent (part of the same level)

        dest is the index of the NEIGHBOR of gent->p chosen
        more "player-like" AI function.


        nb : the pseudocode doesn't take ennemies into account 
        the actual function should 
        pseudocode algorithm : 

        choice_function( p:player, v:visited_nodes, s:seen_nodes, ex:exit) :
                h:heap <- create_min_heap()

                for every node n in s : 
                        h.insert( distance_visited(n,ex,v))
                end for
                destination <- h.pop_min()

                min <- +infinity
                node_next <- null

                for every neighbor n of p.node : 
                        if(distance_visited(n,destination,v) < min): 
                                min <- distance_visited(n,destination,v)
                                node_next <- n
                        end if 
                end for

                move_player(p, node_next) 
                        
                if not node_next in v : 
                        add(v,node_next)
                        delete(s,node_next)

                        for every neighbor n of node_next : 
                                add_seen(s, n)
                        end for 
                end if
        end function      
*/
#endif
