#ifndef ER_SU_H
#define ER_SU_H 

/*
Deque implementation based on : 
https://web.engr.oregonstate.edu/~sinisa/courses/OSU/CS261/lectures/Deque.pdf

the explanation is great check it out :D 
*/

#include "common.h"
#include "graph_base.h"

typedef struct s_deque{

    struct s_graph_entry ** elems;
    uint32_t max; 
    uint32_t size ;
    int64_t start; 

}er_deque;

#define declare_deque(__dq) er_deque __dq={NULL, 0,0,0} ;

extern err_flag init_deque(er_deque * dq, uint32_t max);
extern err_flag free_deque(er_deque * dq);

extern err_flag add_back_deque(er_deque * dq, struct s_graph_entry * elem);
extern err_flag add_front_deque(er_deque * dq,  struct s_graph_entry * elem);

extern err_flag pop_back_deque(er_deque * dq, struct s_graph_entry * * elem);
extern err_flag pop_front_deque(er_deque * dq, struct s_graph_entry * * elem);
//prolly don't need those 


extern err_flag bfs_graph(er_graph * g,  struct s_graph_entry * start, const struct s_graph_entry * dest, int64_t * dist);

#endif
