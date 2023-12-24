#ifndef ER_GRAPH_GEN_H 
#define ER_GRAPH_GEN_H 
/*
___________    .___            __________                                  
\_   _____/  __| _/ ____   ____\______   \__ __  ____   ____   ___________ 
 |    __)_  / __ | / ___\_/ __ \|       _/  |  \/    \ /    \_/ __ \_  __ \
 |        \/ /_/ |/ /_/  >  ___/|    |   \  |  /   |  \   |  \  ___/|  | \/
/_______  /\____ |\___  / \___  >____|_  /____/|___|  /___|  /\___  >__|   
        \/      \/_____/      \/       \/           \/     \/     \/       

grap_gen.h defines the functions used to generate graphs. At the moment only one type of generation 
will be implemented but other might come at some point.
*/

#include "common.h"
#include "graph_base.h"
#include "points.h"

#define DEFAULT_ROW_SIZE 5
extern uint32_t row_size ;

extern err_flag generate_lattice(er_graph * graph, uint32_t n );
extern err_flag randomize_lattice(er_graph * lattice, uint32_t n , double  pdl, double pdn, double pail, double pajk);
//lattice : pointer to a lattice graph, Probability Delete Link : proba to delete any link,
// Probability Delete Node : -//- delete any node Probability Add I,L : let (i,j,k,l) be (n,n+1, 2n,2n+1) 
/*
nodes of the lattice pail is the probability to add the link {i,l} in the lattice. 
similarly, pajk is the probability to add {j,k}. You can't add BOTH i,l and j,k since 
it would make the graph non planar.
*/
#endif
