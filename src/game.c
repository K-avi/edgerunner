#include "game.h"
#include "graph_gen.h"
#include "graph_curses.h"

uint32_t distx = 18; 
uint32_t disty = 5;

static err_flag reset_elements(er_graph * g, dynarr_points * darp, er_player * p){

    free_graph(g); 
    free_dynp(darp);
    p->x = 0 ; 
    p->y = 0 ;

    return ERR_OK;
}

err_flag start_game(WINDOW * w , er_graph * g, dynarr_points * darp, er_player *p ){

    chtype ch ; 

    declare_er_exit(e,0,0);
    do{
        generate_lattice(g,row_size);
        randomize_lattice(g,row_size, 0.5,0.1,0.6,0.7);
        init_dynp(darp, default_arr_size);//shitty ; better flush it
        gen_coordinates(row_size,row_size,darp);

        init_ent_pos(&e,p,g,darp);

        
        wprintw_graph(w,darp,distx,disty,g);
        wprint_player(w,p,distx, disty);
        wprint_exit(w,&e,distx,disty);

        ch = wgetch(w);

        reset_elements(g, darp, p);
        clear();
    }while(ch != 'q');


    return ERR_OK ;

}
