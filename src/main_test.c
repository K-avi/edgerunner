#include "common.h"
#include "dynarr.h"
#include "points.h"
#include <time.h>
#include "graph_base.h"

int main( int argc , char ** argv){

    time_t t;
    time(&t);
    srand(t);
    /* //points test
    declare_dynnar(dynarr_points, darp);
    init_dynp(&darp, 1) ;
    //er_points p = {2,3};
    
    gen_points(10, 256, &darp);

    print_dynps(&darp);
    free_dynp(&darp);
    */
    declare_adjlist(adjlist);
    adjlist.nb_neighboors = 1 ; 
    adjlist.neighboors_ref = malloc(sizeof(er_adjlist *)) ;
    adjlist.neighboors_ref[0] = &adjlist;

    declare_graph(g)
    init_graph(&g,16);

    append_graph(&g,0,&adjlist);
    append_graph(&g,1,&adjlist);

    fprint_graph(stdout, &g);

    free_graph(&g);
    return 0 ; 
}
