#include "graph_gen.h"
#include "graph_base.h"

err_flag generate_lattice(er_graph * graph, uint32_t n ){
    /*
    generates n*n lattice. 

    graph -> not null & initialized | not initializecd (throws warning)
    n -> non zero 
    */
    def_err_handler(!graph,"generate_lattice graph", ERR_NULL);
    def_err_handler(!n,"generate_lattice n", ERR_VALS);
    def_war_handler(graph->adjacency_lists,"generate_lattice", ERR_NOTNULL);

    err_flag failure = init_graph(graph, n*n);
    def_err_handler(failure, "generate_lattice", failure);

    register uint32_t  last_row = n-1, last_line = n*(n-1) ; 
    for(uint32_t i = 0 ; i < graph->nb_nodes ; i ++ ){
        if( i%n == last_row && i < last_line){
            ;
        }else if(i%n != last_row && i >= last_line ){
          app_link_graph(graph, i,i+n);
        }else if(i%n == last_row){
            app_link_graph(graph, i,i+1);
        }else{
           app_link_graph(graph, i,i+1);
           app_link_graph(graph, i,i+n);
        }
    }
    return ERR_OK;
}

err_flag randomize_lattice(er_graph * lattice){
    return ERR_OK;
}


err_flag generate_graph(er_graph * graph , dynarr_points * darp){
    /*
    graph -> not null & not initialized | initialized (throws warning)
    points -> not null & initialized 
    */
    def_err_handler(!graph,"generate_graph graph", ERR_NULL);
    def_err_handler(!darp,"generate_graph darp", ERR_NULL);
    def_err_handler(!darp->elems,"generate_graph darp->elems", ERR_NULL);
    def_war_handler(graph->adjacency_lists,"generate_graph", ERR_NOTNULL);
    
    err_flag failure = init_graph(graph, darp->cur);
    def_err_handler(failure, "generate_graph", failure);

    return ERR_OK;
}
