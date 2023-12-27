#include "common.h"
#include "dynarr.h"
#include "points.h"
#include <time.h>
#include "graph_base.h"
#include "graph_gen.h"
#include "graph_curses.h"

#include <ncurses.h>
#include "game.h"

#include "search_utils.h"

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

   /*
    declare_graph(g);
    generate_lattice(&g,  6 );
    randomize_lattice(&g, 6, 0.2,0.2,0.8,0.8);
    FILE * f  = fopen("dump", "w");
    fprint_graph(f,&g);
    fclose(f);
    declare_dynnar(dynarr_points, d);
    init_dynp(&d,32);
    gen_coordinates(6,6,&d);

   
    initscr();
    cbreak();
    
 
    //printw("haiiiii :O\n");
    wprintw_graph(stdscr,&d,18,5,&g);
   
    // clear();
   
    //refresh();
    getch();

    endwin();
    free_dynp(&d);
    free_graph(&g);*/

    
    initscr();
    noecho();
    curs_set(0);

    declare_graph(g);
    declare_dynnar(dynarr_points, darp);
  
    declare_er_player(p,0,0);
   
    start_game(stdscr,&g,&darp, &p);
    endwin();
    
    
    /*
    declare_deque(dq);

    init_deque(&dq,1);

    for(uint32_t i = 0 ; i < 15; i++){
        add_back_deque(&dq,i);
    }

    for(uint32_t i = 0 ; i < 15 ; i++){
        int64_t elem = -1 ;
        pop_back_deque(&dq, &elem);
        printf("elem=%li\n", elem);
    }
    free_deque(&dq);
    */
/*
    declare_graph(g);
    generate_lattice(&g,  6 );
    int64_t dist = -1;
    bfs_graph(&g, &g.adjacency_lists[0], &g.adjacency_lists[2],&dist);
    
    printf("dist=%li\n",dist);
    
    free_graph(&g);
*/
    return 0 ; 
}
