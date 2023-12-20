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


extern err_flag generate_lattice(er_graph * graph, uint32_t n );

#endif
