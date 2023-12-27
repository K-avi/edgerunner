#include "search_utils.h"
#include "dynarr.h"

uint32_t default_deque_size = 16; 

err_flag init_deque(er_deque * dq, uint32_t max){
    /**/
    def_err_handler(!dq,"init_deque", ERR_NULL);
    
    dq->elems = (struct s_graph_entry **) calloc(max, sizeof(struct s_graph_entry *));
    def_err_handler(!dq->elems,"init_deque",ERR_ALLOC);

    dq->max = max; 
    dq->size = 0 ; 
    dq->start = 0 ;

    return ERR_OK;
}

static inline err_flag realloc_deque(er_deque * dq, double coeff){
    /*
    dq -> not null 
    */
    def_err_handler(!dq,"realloc_deque", ERR_NULL);

    err_flag failure = generic_realloc((void**)&dq->elems, sizeof(struct s_graph_entry *), (uint32_t) ((double) dq->max * coeff +1));
    
    dq->max = (uint32_t) ((double) dq->max * coeff +1);
    def_err_handler(failure, "realloc_deque", failure);
    return ERR_OK;
}

err_flag add_back_deque(er_deque * dq, struct s_graph_entry * elem){
    /*
    dq -> not null & initialized
    */
    def_err_handler(!dq,"realloc_deque dq", ERR_NULL);
    def_err_handler(!dq->elems,"realloc_deque dq->elems", ERR_NULL);

    if(dq->max == dq->size){
        err_flag failure = realloc_deque(dq, default_realloc);
        def_err_handler(failure, "add_front_deque", failure);
    }

    int64_t back_idx =  (dq->start + dq->size) % dq->max;
    dq->elems[back_idx] = elem;

    dq->size ++;

    return ERR_OK;
}
err_flag add_front_deque(er_deque * dq, struct s_graph_entry * elem){
    /*
    dq -> not null & initialized
    */
    def_err_handler(!dq,"realloc_deque dq", ERR_NULL);
    def_err_handler(!dq->elems,"realloc_deque dq->elems", ERR_NULL);

    if(dq->max == dq->size){
        err_flag failure = realloc_deque(dq, default_realloc);
        def_err_handler(failure, "add_front_deque", failure);
    }

    dq->start--; 
    if(dq->start < 0 ) dq->start += dq->max ;

    dq->elems[dq->start] = elem; 
    dq->size++;

    return ERR_OK;
}

err_flag free_deque(er_deque * dq){
    if(dq){
        if(dq->elems){
            free(dq->elems); 
        }
        dq->max = 0 ; 
        dq->size = 0 ; 
        dq->start = 0 ; 
        dq->elems = NULL ;
    }
    return ERR_OK;
}

err_flag pop_front_deque(er_deque * dq, struct s_graph_entry ** elem){

    def_err_handler(!dq,"pop_front_deque dq", ERR_NULL);
    def_err_handler(!elem,"pop_front_deque elem", ERR_NULL);
    warning_handler(dq->size == 0, "pop_front_deque",ERR_VALS, *elem = NULL ; return ERR_OK;);

	*elem = dq->elems[dq->start];
    dq->start++;

	dq->size--;
    return ERR_OK;
}

err_flag pop_back_deque(er_deque * dq, struct s_graph_entry ** elem){

    def_err_handler(!dq,"pop_back_deque", ERR_NULL);
    def_err_handler(!dq,"pop_back_deque", ERR_NULL);
    warning_handler(dq->size == 0, "pop_back_deque",ERR_VALS, *elem = NULL ; return ERR_OK;);

    int64_t back_idx =  (dq->start + dq->size - 1 ) % dq->max;
    *elem = dq->elems[back_idx];

    dq->size--;

    return ERR_OK;
}

err_flag bfs_graph(er_graph * g, struct s_graph_entry * start, const  struct s_graph_entry * dest, int64_t * dist){
    /*
    bfs, returns the min distance between start and dest in *dist 

    O(n+m) 
    */

    if(start == dest ){
        *dist = 0 ;
        return ERR_OK;
    }

    declare_deque(dq);
    init_deque(&dq,default_deque_size);

    uint8_t found = 0 ;

    uint8_t * visited = calloc(g->nb_nodes, sizeof(uint8_t));
    visited[start - g->adjacency_lists] = 1 ;

    uint32_t * dist_tab = calloc(g->nb_nodes ,sizeof(uint32_t));
    memset(dist_tab, -1, g->nb_nodes *sizeof(uint32_t));
    dist_tab[start - g->adjacency_lists] = 0 ; 


    add_front_deque(&dq,start);
    while(dq.size){
        
        struct s_graph_entry * node ;
        

        pop_back_deque(&dq,&node);
        def_err_handler(!node, "bfs_graph node", ERR_VALS);

        for(uint32_t i = 0 ; i < node->cur; i++){
            if(node->neighboors_ref[i] == dest ){
                
                *dist = dist_tab[node - g->adjacency_lists] + 1 ;

                found = 1 ;
                break;
            }
            if(visited[node->neighboors_ref[i] - g->adjacency_lists] == 0){
                add_front_deque(&dq, node->neighboors_ref[i]);
                visited[node->neighboors_ref[i] - g->adjacency_lists] = 1 ;
                dist_tab[node->neighboors_ref[i] - g->adjacency_lists] = dist_tab[node - g->adjacency_lists] + 1;
            }
            
        }
        if(found) break;
       
    }
 
    if(!found){
        *dist = -1;
    }

    free_deque(&dq);
    free(dist_tab);
    free(visited);
    return ERR_OK;
}//not tested ; 100% wrong
