#include "common.h"
#include "dynarr.h"
#include "points.h"
#include <time.h>
#include "graph_base.h"
#include "graph_gen.h"
#include "graph_curses.h"

#include <ncurses.h>
#include "game.h"
#include "ennemy.h"

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
/*
    initscr();
    er_points p1 = {0,0};
    er_points p2 = {1,0}; 
    er_points p3 = {0,1}; 
    er_points p4 = {1,1};
    er_points p5 = {0,2}; 
    er_points p6 = {2,0};
    er_points p7 = {1,2}; 
    er_points p8 = {2,1};
    er_points p9 = {2,2}; 


    wprint_link_fancy(stdscr,&p1,&p4,6,6);
    wprint_link_fancy(stdscr,&p2,&p4,6,6); 
    wprint_link_fancy(stdscr,&p3,&p4,6,6);
    wprint_link_fancy(stdscr,&p5,&p4,6,6);

    wprint_link_fancy(stdscr,&p6,&p4,6,6);

    wprint_link_fancy(stdscr,&p7,&p4,6,6);
    wprint_link_fancy(stdscr,&p8,&p4,6,6);
    wprint_link_fancy(stdscr,&p9,&p4,6,6);

    */

    //wmove(stdscr,1,1);
    //getch();
    //wprintw(stdscr,"0");
    //wvline(stdscr,'#',5);
    //wrefresh(stdscr);
    //getch();

    /*
    initscr();
    noecho();
    curs_set(0);

    declare_graph(g);
    declare_dynnar(dynarr_points, darp);
  
    declare_er_player(p,0,0);
   
    start_game(stdscr,&g,&darp, &p);

    endwin();
    */
    
    declare_er_ennemy(e1,0,0);
    

    declare_entab(a);
    init_entab(&a,1);
    append_entab(&a, &e1, &default_enrules);
    double t0[1] = {0.2};
    uint32_t t1[1] = {1};
    update_entab(&a, t1, t0);


    append_entab(&a, &e1, &default_enrules);
    printf("en0={%u %u %u}, en1={%u %u %u}\n", a.rules[0].coeffs[0], a.rules[0].coeffs[1], a.rules[0].coeffs[2],  a.rules[1].coeffs[0], a.rules[1].coeffs[1], a.rules[1].coeffs[2] );
    free_entab(&a);
    
   /*
    declare_enrules(enr); 
    init_enrules(&enr , &default_enrules);
        printf("enr.coeffs={%u,%u,%u}\n", enr.coeffs[0], enr.coeffs[1], enr.coeffs[2]);

    update_enrules(&enr, 1,2);
    printf("enr.coeffs={%u,%u,%u}\n", enr.coeffs[0], enr.coeffs[1], enr.coeffs[2]);
    free_enrules(&enr);
    */
    return 0 ; 
}
