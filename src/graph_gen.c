#include "graph_gen.h"
#include "graph_base.h"

uint32_t row_size = DEFAULT_ROW_SIZE;

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
        if( i%n == last_row && i >= last_line){
            ;
        }else if(i%n != last_row && i >= last_line ){
            failure = app_link_graph(graph, i,i+1);
            def_err_handler(failure, "generate_lattice app1", failure);

        }else if(i%n == last_row && i < last_line ){
            failure = app_link_graph(graph, i,i+n);
            def_err_handler(failure, "generate_lattice app2", failure);

        }else{
           failure = app_link_graph(graph, i,i+1);
           def_err_handler(failure, "generate_lattice app3", failure);
           failure = app_link_graph(graph, i,i+n);
           def_err_handler(failure, "generate_lattice app4", failure);
        }
    }
    return ERR_OK;
}//tested ; ok 

err_flag randomize_lattice(er_graph * lattice, uint32_t n, double pdl, double pdn, double pail, double pajk){
    /*
    O(n*m)
    */
    def_err_handler(!lattice,"randomize_lattice", ERR_NULL);
    def_err_handler(!lattice->adjacency_lists,"randomize_lattice adj_lists", ERR_NULL);
    def_war_handler(pdl==1,"randomize_lattice pdl", ERR_VALS);
    def_war_handler(pdn==1,"randomize_lattice pdn", ERR_VALS);
    def_war_handler(pail==1,"randomize_lattice pail", ERR_VALS);
    def_war_handler(pajk==1,"randomize_lattice pajk", ERR_VALS);

    bool * added = calloc(lattice->nb_nodes , sizeof(bool) );

    for(uint32_t i = 0 ; i < lattice->nb_nodes - n   ; i++ ){
      
        if((double)rand()/(double)RAND_MAX < pail ){
            if( i%n != n-1 ){
                err_flag failure = app_link_graph(lattice, i, i+n+1);
                def_err_handler(failure, "randomize_lattice app1", failure);
                added[i] = TRUE;  
            }

        }if((double)rand()/(double)RAND_MAX < pajk ){
            if(i%n != 0 ){
                if(!added[i-1]){
                   err_flag failure = app_link_graph(lattice, i, i+n-1);
                   def_err_handler(failure, "randomize_lattice app2", failure);
                }
            }
        }
    }
    free(added);

    for(uint32_t i = 0 ; i < lattice->nb_nodes ; i++){
        
        if((double)rand()/(double)RAND_MAX < pdn){
            bool all_sup = (lattice->adjacency_lists[i].cur > 1) ;
            uint32_t cpt = 0;
            while (all_sup && (cpt < lattice->adjacency_lists[i].cur) )
                all_sup = lattice->adjacency_lists[cpt++].cur > 1; 
            
            if(all_sup){
                err_flag failure = del_node_graph(lattice, i);
                def_err_handler(failure,"randomize_lattice", failure);
                goto rand_lattice_for_end_it;
            }
            
        }
        uint32_t nb_tries = 0 ; 
        while(nb_tries < lattice->adjacency_lists[i].cur){
            if((double)rand()/(double)RAND_MAX < pdl){
                if(lattice->adjacency_lists[i].cur > 1 && lattice->adjacency_lists[i].neighboors_ref[0]->cur > 1){
                    uint32_t j = lattice->adjacency_lists[i].neighboors_ref[0] - lattice->adjacency_lists;
                    err_flag failure = del_link_graph(lattice,i,j);
                    def_err_handler(failure,"randomize_lattice", failure);
                }
            }
            nb_tries++;
        }
        rand_lattice_for_end_it:;
    }
    return ERR_OK;
}//tested ; seems ok 

