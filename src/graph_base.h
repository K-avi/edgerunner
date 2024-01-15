#ifndef ER_GRAPH_BASE_H 
#define ER_GRAPH_BASE_H 

#include "common.h"
#include "misc.h"
/*
___________    .___            __________                                  
\_   _____/  __| _/ ____   ____\______   \__ __  ____   ____   ___________ 
 |    __)_  / __ | / ___\_/ __ \|       _/  |  \/    \ /    \_/ __ \_  __ \
 |        \/ /_/ |/ /_/  >  ___/|    |   \  |  /   |  \   |  \  ___/|  | \/
/_______  /\____ |\___  / \___  >____|_  /____/|___|  /___|  /\___  >__|   
        \/      \/_____/      \/       \/           \/     \/     \/       

graph_base.h defines the Edgerunner graph structure and it's API. 

It also defines the generate_level function which is used to generate graphs during the game 

it also defines functions to manipulate dynamic arrays of nodes.
*/

typedef struct s_dynar_nref{
     uint32_t cur ; 
     uint32_t max ; 
     struct s_graph_entry ** elems ; 
}er_dynarr_nodes;
//boilerplate first ; think later 
//(I'll have to put every dynarr type inside a single generic dynarr call or smtg)
#define declare_darn(dynarr) er_dynarr_nodes dynarr; dynarr.cur = 0; dynarr.max=0; dynarr.elems=NULL;

err_flag init_dynarr_nodes( er_dynarr_nodes * darn, uint32_t size);
err_flag push_dynarr_nodes( er_dynarr_nodes * dynarr, struct s_graph_entry * elem);
void free_dynarr_nodes( er_dynarr_nodes * dynarr);
err_flag pop_dynarr_nodes( er_dynarr_nodes * dynarr, struct s_graph_entry ** elem);

//adjacency list structure
typedef struct s_graph_entry{

    uint32_t cur ;
    uint32_t max ;

    struct s_graph_entry ** neighboors_ref ; //list of references to other entries
    bool * printed_links ;
    
}er_adjlist ;
#define declare_adjlist(__alist) er_adjlist __alist; (__alist).nb_neighboors = 0 ; (__alist).max_neighboors = 0; (__alist).neighboors_ref = NULL; (__alist).printed_links = NULL;
//graph structure
typedef struct s_graph{ 

    size_t nb_nodes ; 

    er_adjlist * adjacency_lists ; 
    bool * printed_nodes ; 

    uint8_t * visited ; 
    uint8_t * col_cur ; 

}er_graph; 
#define declare_graph(__graph) er_graph __graph; (__graph).nb_nodes = 0 ; (__graph).adjacency_lists = NULL; (__graph).printed_nodes = NULL;

#define DEFAULT_ROW_SIZE 5
extern uint32_t row_size ;
extern double def_pail , def_pajk , def_pdn, def_pal ;

extern err_flag generate_level(er_graph * map);
/*
    map -> not null , not initialized 

    generates a level map inside the graph map. A level map is a connected planar graph. 

    alg : 
            - generates a lattice gsource
            - randomly adds diagonal links to gsource w proba pail , pajk  
            - generates the spanning tree of gsource in map and keeps track of deleted links 
            - deletes a certain number of leaf nodes 
            - adds back links with probability pal  
        all of these operations are ~ linear so it's pretty good even though 
        it's really convoluted
*/

extern err_flag init_graph(er_graph * graph, size_t nb_nodes); 
/*
    graph -> not null 

    initializes a graph with nb_nodes 
*/

extern err_flag free_graph(er_graph * graph);
/*
    graph -> 
    free function yk
*/

extern err_flag copy_graph(er_graph * gsource, er_graph * gdest);
/*
    gsource -> not null , initialized
    gdest -> not null, not initialized (or else memleaks :O)

    copies gsource to gdest
*/

extern err_flag app_link_graph(er_graph * graph , uint32_t node1, uint32_t node2); 
/*
    graph -> not null & initialized 
    node1, node2 -> lesser than graph->nb_nodes

    appends the undirected link  {node1, node2} to graph if it isn't already 
    in the graph. 
    O(d(node1) + d(node2))
*/

#ifdef debug 
extern err_flag fprint_graph(FILE * flux, er_graph * graph); 
extern err_flag app_node_graph(er_graph * graph , uint32_t node1, uint32_t node2); 
#endif

#endif 
