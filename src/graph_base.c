#include "graph_base.h"
#include "errflags.h"

#define calloc_gentry(alist, nb_neigh) (alist)->neighboors_ref = calloc( (nb_neigh),sizeof(er_adjlist*) ); (alist)->nb_neighboors = (nb_neigh);

/**functions to initialize the adjacency lists **/
static err_flag init_adjlist(er_adjlist * alist, size_t nb_neighboors){
    /*
    graph_entry -> not null & (initialized | not initialized) throws warnings
    */
    def_err_handler(!alist,"init_gentry",ERR_NULL);
    def_war_handler(alist->neighboors_ref,"init_gentry", ERR_NOTNULL);

    calloc_gentry(alist, nb_neighboors);
    def_err_handler(!alist->neighboors_ref, "init_gentry", ERR_ALLOC);

    return ERR_OK;
}//tested ; seems ok 

static err_flag append_adjlist(er_adjlist * alist, uint32_t index,  er_adjlist * neighboor_ref){
    /*
    graph_entry -> not null & initialized  
    neighboor_ref -> not null 
    */
    def_err_handler(!alist,"append_gentry alist",ERR_NULL);
    def_err_handler(!alist->neighboors_ref,"append_gentry alist->elems",ERR_NULL);
    def_err_handler(!neighboor_ref,"append_gentry neighboor_ref",ERR_NULL);
    
    alist->neighboors_ref[index] = neighboor_ref;

    return ERR_OK;
}//tested ; might be wrong 

static err_flag free_adjlist( er_adjlist * alist){
    /*
    whatever 
    */
    if(alist){
        if(alist->neighboors_ref){
            free(alist->neighboors_ref);
        }
        alist->neighboors_ref = NULL ; 
        alist->nb_neighboors = 0 ; 
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
        fprintf(flux, "node of reference %p with %u neigboors\n", (void*)alist, alist->nb_neighboors);
        for(uint32_t i = 0 ; i < alist->nb_neighboors ; i ++){
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

    graph->adjacency_lists[index].nb_neighboors = entry->nb_neighboors ;
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

