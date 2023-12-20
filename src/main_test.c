#include "common.h"
#include "dynarr.h"
#include "points.h"
#include <time.h>
#include "graph_base.h"
#include "graph_gen.h"

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
    
   /*
    declare_graph(g)
    init_graph(&g,16);
    
    app_link_graph(&g, 1,3);
    app_link_graph(&g, 1,2);

    for(uint32_t i = 0 ; i < 16; i ++){
        app_link_graph(&g, 0, i);
    }



    fprint_graph(stdout, &g);
    printf("---------------\n\n\n");
    
    del_node_graph(&g,0);
    */
    /*
    for(uint32_t i = 0 ; i < 160; i ++){
        app_link_graph(&g, i%16, (16-i-1)%16);
    }*/
    /*adjlist.cur = 6 ; 
    adjlist.max = 6;

    adjlist.neighboors_ref = malloc(6* sizeof(er_adjlist*));

    generic_realloc((void**)&adjlist.neighboors_ref, sizeof(er_adjlist*), (uint32_t)( (double)(adjlist.max+1) * default_realloc));
    adjlist.neighboors_ref[7] = &adjlist;
    append_adjlist(&adjlist, adjlist.neighboors_ref[7]);


    free(adjlist.neighboors_ref);
    */
    declare_graph(g);

    generate_lattice(&g,  4 );

    fprint_graph(stdout, &g);

    free_graph(&g);
    return 0 ; 
}
