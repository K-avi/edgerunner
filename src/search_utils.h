#ifndef ER_SU_H
#define ER_SU_H 

/*
Deque implementation based on : 
https://web.engr.oregonstate.edu/~sinisa/courses/OSU/CS261/lectures/Deque.pdf

the explanation is great check it out :D 
*/

#include "common.h"
#include "graph_base.h"
#include "points.h"

typedef struct s_deque{

    struct s_graph_entry ** elems;
    uint32_t max; 
    uint32_t size ;
    int64_t start; 

}er_deque;

#define declare_deque(__dq) er_deque __dq={NULL, 0,0,0} ;



extern err_flag bfs_graph(er_graph * g,  struct s_graph_entry * start, const struct s_graph_entry * dest, int64_t * dist);
extern err_flag generate_spanning_tree(er_graph * g, er_graph * tree, dynarr_links * removed_links);
extern err_flag deletable_nodes(er_graph * tree, er_dynarr_nodes * darn  );

#endif
