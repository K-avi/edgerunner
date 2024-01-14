#include "graph_base.h"
#include "misc.h"
#include "search_utils.h"
#include "points.h"

//some globals
double def_pail = 0.5, def_pajk = 1, def_pdn = 0.05 , def_pal = 0.4;
uint32_t row_size = DEFAULT_ROW_SIZE;
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

static err_flag append_adjlist(er_adjlist * alist, er_adjlist * neighboor_ref){
    /*
    graph_entry -> not null & initialized  
    neighboor_ref -> not null 
    */
    def_err_handler(!alist,"append_gentry alist",ERR_NULL);
    def_err_handler(!alist->neighboors_ref,"append_gentry alist->elems",ERR_NULL);
    def_err_handler(!neighboor_ref,"append_gentry neighboor_ref",ERR_NULL);

    for(uint32_t i = 0 ; i < alist->cur ; i++){
        if(alist->neighboors_ref[i] == neighboor_ref){
            //def_war_handler(ERR_VALS,"append_gentry_alist",ERR_VALS);
            return ERR_OK;
        }
    }
    
    if(alist->cur == alist->max ){
        err_flag failure = realloc_adjlist(alist, default_realloc);
        def_err_handler(failure, "append_adjlist", ERR_NULL);
    }
    alist->neighboors_ref[alist->cur] = neighboor_ref;
    alist->printed_links[alist->cur] = 0;

    alist->cur++;

    return ERR_OK;
}//tested ; seems ok

static err_flag del_adjlist(er_adjlist * alist, const er_adjlist * neighboor_ref){
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

/**functions to initialize the graph **/

err_flag init_graph(er_graph * graph, size_t nb_nodes){
 
    def_err_handler(!graph,"init_graph", ERR_NULL);
    def_war_handler(graph->adjacency_lists,"init_graph", ERR_NOTNULL);
    def_war_handler(!nb_nodes, "init_graph", ERR_VALS);

    graph->adjacency_lists = calloc(nb_nodes, sizeof(er_adjlist));
    def_err_handler(!graph->adjacency_lists, "init_graph adj_lists", ERR_ALLOC);

    graph->printed_nodes = calloc(nb_nodes, sizeof(bool));
    def_err_handler(!graph->printed_nodes, "init_graph printed_nodes", ERR_ALLOC);

    if(colors_on){
        graph->visited = calloc(nb_nodes, sizeof(uint8_t));
        def_err_handler(!graph->visited, "init_graph col_prev", ERR_ALLOC);

        graph->col_cur = calloc(nb_nodes, sizeof(uint8_t));
        def_err_handler(!graph->col_cur, "init_graph col_cur", ERR_ALLOC);

        memset(graph->col_cur , COLOR_NODE_NEW, sizeof(uint8_t) * graph->nb_nodes);
    }else{
        graph->visited = graph->col_cur = NULL ;
    }

    graph->nb_nodes = nb_nodes;

    for(uint32_t i = 0 ; i < nb_nodes ; i ++){
        err_flag failure = init_adjlist(&graph->adjacency_lists[i], default_arr_size);
        def_err_handler(failure,"init_graph", failure);
    }
    return ERR_OK;
}//tested ; seems ok 


err_flag free_graph(er_graph * graph){

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
        if(graph->col_cur){
            free(graph->col_cur);
        }
        if(graph->visited){
            free(graph->visited);
        }
        graph->col_cur = graph->visited = NULL;
        graph->adjacency_lists = NULL;
        graph->nb_nodes = 0; 
        graph->printed_nodes = NULL ;
    }
    return ERR_OK;
}//tested seems ok 

err_flag app_link_graph(er_graph * graph , uint32_t node1, uint32_t node2){

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

static err_flag del_link_graph(er_graph * graph , uint32_t node1, uint32_t node2){
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

static err_flag del_node_graph(er_graph * graph, uint32_t node){
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

static err_flag generate_lattice(er_graph * graph, uint32_t n ){
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

static err_flag copy_adjlist(er_adjlist * asource, er_adjlist * adest, struct s_graph_entry * first_ref_source, struct s_graph_entry * first_ref_dest){

    def_err_handler(!asource , "copy_adjlist asource", ERR_NULL);
    def_err_handler(!adest , "copy_adjlist adest", ERR_NULL);

    err_flag failure = init_adjlist(adest, asource->max);
    def_err_handler(failure, "copy_adjlist", failure);
    
    for(uint32_t i = 0 ; i < asource->cur ; i++){
        adest->neighboors_ref[i] = (asource->neighboors_ref[i] -first_ref_source) + first_ref_dest;
    }

    adest->cur = asource->cur ; 
    adest->max = asource->max;
    //bc I don't want this info 

    return ERR_OK;
}

err_flag copy_graph(er_graph * gsource, er_graph * gdest){
    def_err_handler(!gsource , "copy_adjlist asource", ERR_NULL);
    def_err_handler(!gdest , "copy_adjlist adest", ERR_NULL);

    gdest->adjacency_lists = calloc( gsource->nb_nodes, sizeof(er_adjlist));
    def_err_handler(!gdest->adjacency_lists, "copy_graph adj_lists", ERR_ALLOC);

    gdest->printed_nodes = calloc(gsource->nb_nodes, sizeof(bool));
    def_err_handler(!gdest->printed_nodes, "copy_graph printed_nodes", ERR_ALLOC);

    if(colors_on){
        gdest->visited = calloc(gsource->nb_nodes, sizeof(uint8_t));
        def_err_handler(!gdest->visited, "init_graph col_prev", ERR_ALLOC);

        gdest->col_cur = calloc(gsource->nb_nodes, sizeof(uint8_t));
        def_err_handler(!gdest->col_cur, "init_graph col_cur", ERR_ALLOC);

        memset(gdest->col_cur , COLOR_NODE_NEW, sizeof(uint8_t) * gsource->nb_nodes);
    }else{
        gdest->visited = gdest->col_cur = NULL ; 
    }

    gdest->nb_nodes = gsource->nb_nodes ;

    for(uint32_t i = 0 ; i < gsource->nb_nodes ; i++){
        err_flag failure = copy_adjlist(&gsource->adjacency_lists[i], &gdest->adjacency_lists[i], gsource->adjacency_lists, gdest->adjacency_lists);
        def_err_handler(failure, "copy_graph", failure);
    }
    return ERR_OK;
}

static err_flag safe_randomize_lattice( er_graph * gsource , er_graph * gdest ,uint32_t n, double pail, double pajk, double pdn, double pal){
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
    bool * added = calloc(gsource->nb_nodes , sizeof(bool) );
    for(uint32_t i = 0 ; i < gsource->nb_nodes - n   ; i++ ){
      
        if((double)rand()/(double)RAND_MAX < pail ){
            if( i%n != n-1 ){
                err_flag failure = app_link_graph(gsource, i, i+n+1);
                def_err_handler(failure, "randomize_lattice app1", failure);
                added[i] = TRUE;  
            }
        }if((double)rand()/(double)RAND_MAX < pajk ){
            if(i%n != 0 ){
                if(!added[i-1]){
                   err_flag failure = app_link_graph(gsource, i, i+n-1);
                   def_err_handler(failure, "randomize_lattice app2", failure);
                }
            }
        }
    }
    free(added);

    declare_dynarr(dynarr_links, removed_links );
    init_dynl(&removed_links, gsource->nb_nodes *2);
    
    err_flag failure = generate_spanning_tree(gsource, gdest, &removed_links); 
    def_err_handler(failure,"", failure);

    uint32_t nb_delnodes = 0 ; 
    for(uint32_t i = 0 ; i < gdest->nb_nodes ; i++){
        if(((double)rand()/(double)RAND_MAX < pdn )){
            nb_delnodes++;
        }
    }
    
    declare_darn(darn);
    init_dynarr_nodes(&darn, (gsource->nb_nodes / 2)+1 );
    deletable_nodes(gdest, &darn);

    nb_delnodes = nb_delnodes > darn.cur ? darn.cur : nb_delnodes;
    for(uint32_t i = 0 ; i < nb_delnodes ; i++){
        del_node_graph(gdest, darn.elems[i] - gdest->adjacency_lists);
    }
    free_dynarr_nodes(&darn);
    for(uint32_t i = 0 ; i < removed_links.cur ; i++){
        if((double)rand()/(double)RAND_MAX < pal ){
            app_link_graph(gdest, removed_links.elems[i].x, removed_links.elems[i].y);
        }
    }
    free_dynl(&removed_links);  
    return ERR_OK;
}//tested; ok  

err_flag generate_level(er_graph * map){
  
    def_err_handler(!map, "generate_level", ERR_NULL);

    declare_graph(g);
    err_flag failure = generate_lattice(&g, row_size);
    def_err_handler(failure, "generate_level", failure);

    failure= safe_randomize_lattice(&g, map, row_size, def_pail, def_pajk, def_pdn, def_pal);
    def_err_handler(failure, "generate_level", failure);

    free_graph(&g);
    return ERR_OK;
}

#ifdef debug 
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
#endif
