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

static err_flag move_player(WINDOW * w, const er_graph * g , dynarr_points * darp, er_player * p, uint32_t index ){

    fprintf(stderr,"index=%u\n",index);
    if(index > p->cur_node->cur){
        fprintf(stderr,"index greater than nb nodes");
        return ERR_OK;
    }
    for(uint32_t i = 0 ; i < p->cur_node->cur ; i++){
        uint32_t cur_index = p->cur_node->neighboors_ref[i] - g->adjacency_lists;
        wmove(w, darp->elems[cur_index].y*disty, darp->elems[cur_index].x*distx);
        waddch(w,'O');
    }
    uint32_t cur_index = p->cur_node - g->adjacency_lists;
    wmove(w,darp->elems[cur_index].y*disty, darp->elems[cur_index].x*distx);
    waddch(w,'O');

    cur_index = p->cur_node->neighboors_ref[index] - g->adjacency_lists;
    wmove(w,darp->elems[cur_index].y*disty, darp->elems[cur_index].x*distx);
    waddch(w,p->ch);

    p->y = darp->elems[cur_index].y;
    p->x = darp->elems[cur_index].x;

    p->cur_node = p->cur_node->neighboors_ref[index];

    wrefresh(w);

    return ERR_OK;
}

err_flag start_game(WINDOW * w , er_graph * g, dynarr_points * darp, er_player *p ){

    chtype ch = 0 ; 

    declare_er_exit(e,0,0);
    do{
        generate_lattice(g,row_size);
        randomize_lattice(g,row_size, 0.3,0.1,0.8,0.85);
        init_dynp(darp, default_arr_size);//shitty ; better flush it
        gen_coordinates(row_size,row_size,darp);

        init_ent_pos(&e,p,g,darp);

        
        wprintw_graph(w,darp,distx,disty,g);
        wprint_player(w,p,distx, disty);
        wprint_surroundings(w,p,darp,distx,disty,g);
        wprint_exit(w,&e,distx,disty);

        

        while(p->cur_node != e.cur_node && ch!='q'){
            ch = wgetch(w);
            if(ch >= '0' && ch <= '9' && ch != 'q'){
                move_player(w,g,darp,p,ch-'0');
                wprint_player(w,p,distx, disty);
                wprint_surroundings(w,p,darp,distx,disty,g);
                wprint_exit(w,&e,distx,disty);
            }
        }
        
        reset_elements(g, darp, p);
        clear();
        
    }while(ch != 'q');


    return ERR_OK ;

}
