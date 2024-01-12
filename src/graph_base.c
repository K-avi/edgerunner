#include "graph_base.h"
#include "misc.h"
#include "search_utils.h"
#include "points.h"

#define calloc_gentry(alist, nb_neigh) (alist)->neighboors_ref = calloc( (nb_neigh),sizeof(er_adjlist*) ); (alist)->printed_links = calloc( (nb_neigh),sizeof(bool) ); (alist)->max = (nb_neigh); (alist)->cur = 0;

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
   def_err_handler(failure, "realloc_adjlist->neighboors", failure);
    //set to 0 cuz I don't like habing unitialized stuff lying around

   failure = generic_realloc((void**)&alist->printed_links, sizeof(bool ), (uint32_t)( (double)(alist->max+1) * coeff));
   def_err_handler(failure, "realloc_adjlist->printed_links", failure);

    //set to 0 cuz I don't like habing unitialized stuff lying around
   alist->max = (uint32_t)( (double)(alist->max+1) * coeff) ;
   memset(&alist->neighboors_ref[alist->cur+1], 0, (alist->max - alist->cur - 1 )* sizeof(er_adjlist*));
   memset(&alist->printed_links[alist->cur+1], 0, (alist->max - alist->cur - 1 )* sizeof(bool));

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
    alist->neighboors_ref[alist->cur] = neighboor_ref;
    alist->printed_links[alist->cur] = 0;

    alist->cur++;

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
            alist->printed_links[i] = alist->printed_links[alist->cur-1];
            alist->neighboors_ref[alist->cur-1] = NULL;
            alist->printed_links[alist->cur -1 ] = 0;

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
        if(alist->printed_links){
            free(alist->printed_links);
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
    def_err_handler(!graph->adjacency_lists, "init_graph adj_lists", ERR_ALLOC);

    graph->printed_nodes = calloc(nb_nodes, sizeof(bool));
    def_err_handler(!graph->printed_nodes, "init_graph printed_nodes", ERR_ALLOC);


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
    graph->printed_nodes[index] = 0 ;

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
        if(graph->printed_nodes){
            free(graph->printed_nodes);
        }
        graph->adjacency_lists = NULL;
        graph->nb_nodes = 0; 
        graph->printed_nodes = NULL ;
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

err_flag del_link_graph(er_graph * graph , uint32_t node1, uint32_t node2){
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

err_flag del_node_graph(er_graph * graph, uint32_t node){
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

/*****GRAPH CURSES FUNCTIONS ****/
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


static err_flag copy_adjlist(er_adjlist * asource, er_adjlist * adest){

    def_err_handler(!asource , "copy_adjlist asource", ERR_NULL);
    def_err_handler(!adest , "copy_adjlist adest", ERR_NULL);

    err_flag failure = init_adjlist(adest, asource->max);
    def_err_handler(failure, "copy_adjlist", failure);
   
    memcpy(adest->neighboors_ref, asource->neighboors_ref, asource->max * sizeof(struct s_graph_entry * ));
    memset(adest->printed_links, 0, asource->max * sizeof(bool));
    //bc I don't want this info 

    return ERR_OK;
}

err_flag copy_graph(er_graph * gsource, er_graph * gdest){
    def_err_handler(!gsource , "copy_adjlist asource", ERR_NULL);
    def_err_handler(!gdest , "copy_adjlist adest", ERR_NULL);

    err_flag failure = init_graph(gdest, gsource->nb_nodes);
    def_err_handler(failure, "copy_graph", failure);

    for(uint32_t i = 0 ; i < gsource->nb_nodes ; i++){
        failure = copy_adjlist(&gsource->adjacency_lists[i], &gdest->adjacency_lists[i]);
        def_err_handler(failure, "copy_graph", failure);

    }
    memset(gdest->printed_nodes, 0, gsource->nb_nodes * sizeof(bool));

    return ERR_OK;
}


err_flag safe_randomize_lattice( er_graph * gsource , er_graph * gdest , double pail, double pajk, double pdn, double pal){
    /*
        gsource -> not null & initialized 
        gdest -> not null 

        modifies gsource (adds links) and gdest 

        alg : 

            - randomly adds diagonal links to gsource w proba pail , pajk  
            - generates the spanning tree of gsource in gdest and keeps track of deleted links 
            - deletes a certain number of leaf nodes 
            - adds back links with probability pal  
        all of these operations are ~ linear so it's pretty good even though 
        it's really convoluted
    */

    declare_dynarr(dynarr_links, removed_links );
    init_dynl(&removed_links, gsource->nb_nodes *2);
    
    err_flag failure = generate_spanning_tree(gsource, gdest, &removed_links); 
    def_err_handler(failure,"", failure);

    declare_darn(darn);
    init_dynarr_nodes(&darn, (gsource->nb_nodes / 2)+1 );
    deletable_nodes(gdest, &darn);

    free_dynl(&removed_links);
    free_dynarr_nodes(&darn);
    return ERR_OK;
}//not done 
