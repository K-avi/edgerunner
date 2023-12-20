#ifndef ER_GRAPH_BASE_H 
#define ER_GRAPH_BASE_H 
/*
___________    .___            __________                                  
\_   _____/  __| _/ ____   ____\______   \__ __  ____   ____   ___________ 
 |    __)_  / __ | / ___\_/ __ \|       _/  |  \/    \ /    \_/ __ \_  __ \
 |        \/ /_/ |/ /_/  >  ___/|    |   \  |  /   |  \   |  \  ___/|  | \/
/_______  /\____ |\___  / \___  >____|_  /____/|___|  /___|  /\___  >__|   
        \/      \/_____/      \/       \/           \/     \/     \/       

graph_base.h defines basic graph structure and it's API
*/

#include "common.h"
#include "errflags.h"

//should be dynamic array at least for generation 
typedef struct s_graph_entry{

    uint32_t cur ;
    uint32_t max ;

    struct s_graph_entry ** neighboors_ref ; //list of references to other entries
    
}er_adjlist ;
#define declare_adjlist(__alist) er_adjlist __alist; (__alist).nb_neighboors = 0 ; (__alist).max_neighboors = 0; (__alist).neighboors_ref = NULL;

typedef struct s_graph{ 

    size_t nb_nodes ; 
    er_adjlist * adjacency_lists ; 

}er_graph; 
#define declare_graph(__graph) er_graph __graph; (__graph).nb_nodes = 0 ; (__graph).adjacency_lists = NULL;

//not storing node labels bc u just do substractions ;O


extern err_flag init_graph(er_graph * graph, size_t nb_nodes); 
extern err_flag append_graph(er_graph * graph , uint32_t index, const er_adjlist * entry); 
extern err_flag fprint_graph(FILE * flux, er_graph * graph); 
extern err_flag free_graph(er_graph * graph);

//more advanced manipulation functions
extern err_flag app_link_graph(er_graph * graph , uint32_t node1, uint32_t node2); 
//extern err_flag app_node_graph(er_graph * graph , uint32_t node1, uint32_t node2); 
extern err_flag del_link_graph(er_graph * graph , uint32_t node1, uint32_t node2); 
extern err_flag del_node_graph(er_graph * graph , uint32_t node); 



#endif 
