#include "graph_base.h"
#include "errflags.h"
#include "dynarr.h"

#define calloc_gentry(alist, nb_neigh) (alist)->neighboors_ref = calloc( (nb_neigh),sizeof(er_adjlist*) ); (alist)->max = (nb_neigh); (alist)->cur = 0;

/**functions to initialize the adjacency lists **/
static err_flag init_adjlist(er_adjlist * alist, size_t max){
    /*
    graph_entry -> not null & (initialized | not initialized) throws warnings
    */
    def_err_handler(!alist,"init_gentry",ERR_NULL);
    def_war_handler(alist->neighboors_ref,"init_gentry", ERR_NOTNULL);

    calloc_gentry(alist, max);
    def_err_handler(!alist->neighboors_ref, "init_gentry", ERR_REALLOC);

    return ERR_OK;
}//tested ; seems ok

static err_flag realloc_adjlist(er_adjlist * alist, double coeff){
    /*
    alist -> not null 
    */
   def_err_handler(!alist, "realloc_adjlist", ERR_NULL);

    //actual realloc 
   err_flag failure = generic_realloc((void**)&alist->neighboors_ref, sizeof(er_adjlist * ), (uint32_t)( (double)(alist->max+1) * coeff));
   def_err_handler(failure, "realloc_adjlist", failure);
    //set to 0 cuz I don't like habing unitialized stuff lying around
   alist->max = (uint32_t)( (double)(alist->max+1) * coeff) ;
   memset(&alist->neighboors_ref[alist->cur+1], 0, (alist->max - alist->cur - 1 )* sizeof(er_adjlist*));

   return ERR_OK;
}//tested

err_flag append_adjlist(er_adjlist * alist, er_adjlist * neighboor_ref){
    /*
    graph_entry -> not null & initialized  
    neighboor_ref -> not null 
    */
    def_err_handler(!alist,"append_gentry alist",ERR_NULL);
    def_err_handler(!alist->neighboors_ref,"append_gentry alist->elems",ERR_NULL);
    def_err_handler(!neighboor_ref,"append_gentry neighboor_ref",ERR_NULL);

    if(alist->cur == alist->max ){
        err_flag failure = realloc_adjlist(alist, default_realloc);
        def_err_handler(failure, "append_adjlist", ERR_NULL);
    }
    alist->neighboors_ref[alist->cur++] = neighboor_ref;

    return ERR_OK;
}//tested ; seems ok

err_flag del_adjlist(er_adjlist * alist, const er_adjlist * neighboor_ref){
    /*
    graph_entry -> not null & initialized  
    neighboor_ref -> not null 

    O(cur)
    */
    def_err_handler(!alist,"append_gentry alist",ERR_NULL);
    def_war_handler(!alist->neighboors_ref,"append_gentry alist->elems",ERR_NULL);
    def_war_handler(!neighboor_ref,"append_gentry neighboor_ref",ERR_NULL);

    if(alist->cur == 0){
        def_war_handler(ERR_VALS, "del_adjlist", ERR_VALS);
        return ERR_OK;
    }

    for(uint32_t i = 0 ; i < alist->cur ; i ++){
        if(neighboor_ref == alist->neighboors_ref[i]){
            alist->neighboors_ref[i] = alist->neighboors_ref[alist->cur-1];
            alist->neighboors_ref[alist->cur-1] = NULL;
            alist->cur--;
            return ERR_OK;
        }
    }
    def_war_handler(ERR_VALS,"append_gentry neighboor ref_not found", ERR_VALS);
    return ERR_OK;
}//tested ; seems ok

static err_flag free_adjlist( er_adjlist * alist){
    /*
    whatever 
    */
    if(alist){
        if(alist->neighboors_ref){
            free(alist->neighboors_ref);
        }
        alist->neighboors_ref = NULL ; 
        alist->cur = 0 ; 
        alist->max = 0 ; 
    }
    return ERR_OK;
}//tested; ok 

static err_flag fprint_adjlist( FILE * flux, const er_adjlist * alist ){
    /*
    flux -> not null | null 
    gentry -> null | not null 
    */
    if(!flux){
        flux = stdout; 
    }
    if(!alist){
        fprintf(flux, "null pointer passed as an adjlist\n");
    }else{
        fprintf(flux, "node of reference %p with %u neighboors : ", (void*)alist, alist->cur);
        for(uint32_t i = 0 ; i < alist->cur ; i ++){
            fprintf(flux, "%p ", (void*) alist->neighboors_ref[i]);
        }
        fprintf(flux, "\n");
    }
    
    return ERR_OK;
}//tested ; ok 

/**functions to initialize the graph **/

err_flag init_graph(er_graph * graph, size_t nb_nodes){
    /*
    graph -> not null & not initialized | initialized (throws warnings)
    */

    def_err_handler(!graph,"init_graph", ERR_NULL);
    def_war_handler(graph->adjacency_lists,"init_graph", ERR_NOTNULL);
    def_war_handler(!nb_nodes, "init_graph", ERR_VALS);

    graph->adjacency_lists = calloc(nb_nodes, sizeof(er_adjlist));
    def_err_handler(!graph->adjacency_lists, "init_graph", ERR_ALLOC);

    graph->nb_nodes = nb_nodes;

    for(uint32_t i = 0 ; i < nb_nodes ; i ++){
        err_flag failure = init_adjlist(&graph->adjacency_lists[i], default_arr_size);
        def_err_handler(failure,"init_graph", failure);
    }
    return ERR_OK;
}//tested ; seems ok 

err_flag append_graph(er_graph * graph , uint32_t index, const er_adjlist * entry){
    /*
    graph -> not null & initialized 
    entry -> not null & initialized 
    */
    def_err_handler(!graph,"append_graph graph",ERR_NULL);
    def_err_handler(!graph->adjacency_lists,"append_graph graph->adjlists",ERR_NULL);
    def_err_handler(!entry,"append_graph entry",ERR_NULL);

    def_err_handler(index > graph->nb_nodes , "append_graph", ERR_VALS) ;
    def_war_handler(graph->adjacency_lists[index].neighboors_ref, "append_graph_entry", ERR_NOTNULL);

    graph->adjacency_lists[index].cur = entry->cur ;
    graph->adjacency_lists[index].neighboors_ref = entry->neighboors_ref ;

    return ERR_OK;
}//tested seems ok 


err_flag free_graph(er_graph * graph){
    /*
    whatever
    */
    if(graph){
        if(graph->adjacency_lists){
            for(uint32_t i = 0 ; i < graph->nb_nodes ; i++){
                free_adjlist(&graph->adjacency_lists[i]);
            }
            free(graph->adjacency_lists);
        }
        graph->adjacency_lists = NULL;
        graph->nb_nodes = 0; 
    }
    return ERR_OK;
}//tested seems ok 

err_flag fprint_graph(FILE * flux, er_graph * graph){
    /*
    whatever
    */
    if(!flux){
        flux = stdout ; 
    }
    if(!graph){
        fprintf(flux, "null pointer passed as graph\n");
        return ERR_OK;
    }
    if(!graph->adjacency_lists){
        fprintf(flux, "null pointer passed as graph->adjacency_lists\n");
        return ERR_OK;
    }

    for(uint32_t i = 0 ; i < graph->nb_nodes ; i ++){
        fprint_adjlist(flux, &graph->adjacency_lists[i]);
    }

    return ERR_OK;
}//yeah 


/*new manipulation functions*/

err_flag app_link_graph(er_graph * graph , uint32_t node1, uint32_t node2){
    /*
    graph -> not null & initialized
    node1 -> node1 < graph->nb_nodes
    node2 -> node2 < graph->nb_nodes
    */
    def_err_handler(!graph,"app_link_graph", ERR_NULL); 
    def_err_handler(!graph->adjacency_lists,"app_link_graph adjlists", ERR_NULL); 
    def_err_handler(node1 > graph->nb_nodes,"app_link_graph node1", ERR_VALS);
    def_err_handler(node2 > graph->nb_nodes,"app_link_graph node2", ERR_VALS);

    er_adjlist * node1_ref = graph->adjacency_lists + node1 ; 
    er_adjlist * node2_ref = graph->adjacency_lists + node2 ; 

    err_flag failure = append_adjlist(&graph->adjacency_lists[node1], node2_ref);
    def_err_handler(failure, "app_link_graph add node1", failure);

    failure = append_adjlist(&graph->adjacency_lists[node2], node1_ref);
    def_err_handler(failure, "app_link_graph add node2", failure);

    return ERR_OK;
}//tested 

extern err_flag del_link_graph(er_graph * graph , uint32_t node1, uint32_t node2){
    /*
    graph -> not null & initialized
    node1 -> node1 < graph->nb_nodes
    node2 -> node2 < graph->nb_nodes
    */
    def_err_handler(!graph,"del_link_graph", ERR_NULL); 
    def_err_handler(!graph->adjacency_lists,"del_link_graph adjlists", ERR_NULL); 
    def_err_handler(node1 > graph->nb_nodes,"del_link_graph node1", ERR_VALS);
    def_err_handler(node2 > graph->nb_nodes,"del_link_graph node2", ERR_VALS);

    const er_adjlist * node1_ref = graph->adjacency_lists + node1 ; 
    const er_adjlist * node2_ref = graph->adjacency_lists + node2 ; 


    err_flag failure = del_adjlist(&graph->adjacency_lists[node1], node2_ref);
    def_err_handler(failure, "app_link_graph add node1", failure);

    failure = del_adjlist(&graph->adjacency_lists[node2], node1_ref);
    def_err_handler(failure, "app_link_graph add node2", failure);

    return ERR_OK;
}//tested; ok

extern err_flag del_node_graph(er_graph * graph, uint32_t node){
    /*
    graph -> not null & initialized
    graph->adjancely_lists -> not null 
    node -> node < graph->nb_nodes
    */
    def_err_handler(!graph,"del_link_graph", ERR_NULL); 
    def_err_handler(!graph->adjacency_lists,"del_link_graph adjlists", ERR_NULL); 
    def_err_handler(node > graph->nb_nodes,"del_link_graph node1", ERR_VALS);
    
    while(graph->adjacency_lists[node].cur){//I really like this one ngl
        
        err_flag failure = del_link_graph(graph,node,  graph->adjacency_lists[node].neighboors_ref[0]-graph->adjacency_lists);
        def_err_handler(failure, "del_node_graph",failure);
    }
    return ERR_OK;
}//tested ; ok 
