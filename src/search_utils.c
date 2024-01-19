#include "search_utils.h"
#include "misc.h"

uint32_t default_deque_size = 16; 
typedef struct s_deque{

    struct s_graph_entry ** elems;
    uint32_t max; 
    uint32_t size ;
    int64_t start; 

}er_deque;
#define declare_deque(__dq) er_deque __dq={NULL, 0,0,0} ;
/*
Deque implementation based on : 
https://web.engr.oregonstate.edu/~sinisa/courses/OSU/CS261/lectures/Deque.pdf

the explanation is great check it out :D 
*/

static err_flag init_deque(er_deque * dq, uint32_t max){
    /*
    dq -> not null 
    inits a dq with max entries.
    */
    def_err_handler(!dq,"init_deque", ERR_NULL);
    
    dq->elems = (struct s_graph_entry **) calloc(max, sizeof(struct s_graph_entry *));
    def_err_handler(!dq->elems,"init_deque",ERR_ALLOC);

    dq->max = max; 
    dq->size = 0 ; 
    dq->start = 0 ;

    return ERR_OK;
}//ok

static inline err_flag realloc_deque(er_deque * dq, double coeff){
    /*
    dq -> not null 
    reallocates dq; new max = old_max * coeff +1
    */
    def_err_handler(!dq,"realloc_deque", ERR_NULL);

    err_flag failure = generic_realloc((void**)&dq->elems, sizeof(struct s_graph_entry *), (uint32_t) ((double) dq->max * coeff +1));
    
    dq->max = (uint32_t) ((double) dq->max * coeff +1);
    def_err_handler(failure, "realloc_deque", failure);
    return ERR_OK;
}//ok

static err_flag add_front_deque(er_deque * dq, struct s_graph_entry * elem){
    /*
    dq -> not null & initialized
    adds elem to the front of the deque, see wiki of deque for more info
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
}//ok

static err_flag free_deque(er_deque * dq){
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
}//ok

static err_flag pop_back_deque(er_deque * dq, struct s_graph_entry ** elem){
    /*
    */
    def_err_handler(!dq,"pop_back_deque", ERR_NULL);
    def_err_handler(!dq,"pop_back_deque", ERR_NULL);
    warning_handler(dq->size == 0, "pop_back_deque",ERR_VALS, *elem = NULL ; return ERR_OK;);

    int64_t back_idx =  (dq->start + dq->size - 1 ) % dq->max;
    *elem = dq->elems[back_idx];

    dq->size--;

    return ERR_OK;
}

err_flag bfs_graph(er_graph * g, struct s_graph_entry * start, const  struct s_graph_entry * dest, int64_t * dist){
    
    def_err_handler(!g, "bfs_graph g", ERR_NULL);
    def_err_handler(!start, "bfs_graph start", ERR_NULL);
    def_err_handler(!dest,"bfs_graph dest", ERR_NULL);
    def_err_handler(!dist,"bfs_graph dist", ERR_NULL);

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
    memset(dist_tab, 255, g->nb_nodes *sizeof(uint32_t));
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
}//tested; maybe wrong; buggy I think

err_flag generate_spanning_tree(er_graph * g, er_graph * tree, dynarr_links * removed_links ){
    
    def_err_handler(!g, "generate_spanning_tree g", ERR_NULL);
    def_err_handler(!tree, "generate_spanning_tree tree", ERR_NULL);
    def_err_handler(!removed_links, "generate_spanning_tree removed_links", ERR_NULL);

    declare_graph(gcopy); 
    err_flag failure = copy_graph(g,&gcopy);
    def_err_handler(failure, "generate_spanning_tree", failure);


    failure = init_graph(tree, gcopy.nb_nodes);
    def_err_handler(failure, "generate_spanning_tree", failure);

    bool * seen = calloc(gcopy.nb_nodes, sizeof(bool));
    uint32_t index_chosen = rand()%gcopy.nb_nodes;

    declare_stacku32(s);
    init_stack(&s, default_arr_size);
    push_stack(&s, index_chosen);

    while(s.cur){

        int64_t node_index;
        failure = pop_stack(&s, &node_index);
        def_err_handler(failure, "generate_spanning_tree", failure);
     
        struct s_graph_entry * cur_node = &gcopy.adjacency_lists[node_index];

        for(uint32_t i = 0 ; i < cur_node->cur; i ++){
            uint32_t neighbor_index = cur_node->neighboors_ref[i] - gcopy.adjacency_lists ; 
            if(!seen[neighbor_index]){
                seen[neighbor_index] = true;
                    
                failure = app_link_graph(tree, node_index, neighbor_index );
                def_err_handler(failure, "generate_spanning_tree", failure);

                failure = push_stack(&s, neighbor_index);
                def_err_handler(failure, "generate_spanning_tree", failure);

                //sets attribute to true to not delete twice 
                cur_node->printed_links[i] = true ;                
                for(uint32_t j = 0 ; j < gcopy.adjacency_lists[neighbor_index].cur; j++){
                    if(gcopy.adjacency_lists[neighbor_index].neighboors_ref[j] == cur_node){
                        gcopy.adjacency_lists[neighbor_index].printed_links[j] = true;
                        break;
                    }
                }
            }
            if( cur_node->printed_links[i] != true ){
                
             
                er_link l = {node_index, neighbor_index };
                failure = append_dynl(removed_links, &l );
                def_err_handler(failure, "generate_spanning_tree", failure);

                //sets attribute to true to not delete twice 
                cur_node->printed_links[i] = true ;                
                for(uint32_t j = 0 ; j < gcopy.adjacency_lists[neighbor_index].cur; j++){
                    if(gcopy.adjacency_lists[neighbor_index].neighboors_ref[j] == cur_node){
                        gcopy.adjacency_lists[neighbor_index].printed_links[j] = true;
                        break;
                    }
                }
            }
        } 
    } 
    free(seen);
    free_stack(&s);
    free_graph(&gcopy);
    return ERR_OK;
}//tested ; works :O

err_flag deletable_nodes(er_graph * tree, er_dynarr_nodes * darn  ){
   
    def_err_handler(!tree, "deletable_nodes tree" ,ERR_NULL);
    def_err_handler(!darn, "deletable_nodes darn" ,ERR_NULL);

    for(uint32_t i = 0 ; i < tree->nb_nodes ; i++){
        if(tree->adjacency_lists[i].cur == 1 ){
           err_flag failure = push_dynarr_nodes(darn, &tree->adjacency_lists[i]);
           def_err_handler(failure, "deletable_nodes", failure);
        }
    }  
    return ERR_OK;
}

#ifdef debug
static err_flag pop_front_deque(er_deque * dq, struct s_graph_entry ** elem){
    /*
    dq -> not null 
    elem -> not null
    */
    def_err_handler(!dq,"pop_front_deque dq", ERR_NULL);
    def_err_handler(!elem,"pop_front_deque elem", ERR_NULL);
    warning_handler(dq->size == 0, "pop_front_deque",ERR_VALS, *elem = NULL ; return ERR_OK;);

	*elem = dq->elems[dq->start];
    dq->start++;

	dq->size--;
    return ERR_OK;
}//ok

static err_flag add_back_deque(er_deque * dq, struct s_graph_entry * elem){
    /*
    dq -> not null & initialized
    adds elem to the back of the deque, see wiki on deque for more info
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
}//ok 
#endif

