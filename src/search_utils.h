#ifndef ER_SU_H
#define ER_SU_H 

#include "common.h"
#include "graph_base.h"
#include "points.h"
/*
___________    .___            __________                                  
\_   _____/  __| _/ ____   ____\______   \__ __  ____   ____   ___________ 
 |    __)_  / __ | / ___\_/ __ \|       _/  |  \/    \ /    \_/ __ \_  __ \
 |        \/ /_/ |/ /_/  >  ___/|    |   \  |  /   |  \   |  \  ___/|  | \/
/_______  /\____ |\___  / \___  >____|_  /____/|___|  /___|  /\___  >__|   
        \/      \/_____/      \/       \/           \/     \/     \/       

search_utils.h defines function to do graph search related operations on 
the edgerunner graphs
*/

extern err_flag bfs_graph(er_graph * g,  struct s_graph_entry * start, const struct s_graph_entry * dest, int64_t * dist);
/*
    g,start, dest, dist -> Not null
    g -> initialized
    
    bfs, returns the min distance between start and dest in *dist, does so by assigning distance 
    (in nb nodes) from start to it's neighbor with the bfs (kinda like dijkstra but simpler cuz just bfs)
    until dest is found. 
    sets dist to -1 if dest isn't found

    O(n+m) 
*/

extern err_flag generate_spanning_tree(er_graph * g, er_graph * tree, dynarr_links * removed_links);
/*
    g -> not null & initialized & connex 
    tree -> not null 

    creates a spanning tree from g in tree by doing a DFS. IF g isn't connex 
    the tree will be the spanning tree of one of it's connected componnents.
    
    The DFS also keeps track of the deleted nodes in the removed_links array. 
*/

extern err_flag deletable_nodes(er_graph * tree, er_dynarr_nodes * darn  );
 /*
    tree -> not null & not empty 
    darn -> not null 

    returns the node of degree 1 of the graph , which are nodes that can be removed without 
    making the graph non connected
    O(tree->nb_nodes)
*/
#endif
