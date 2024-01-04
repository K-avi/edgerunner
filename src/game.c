#include "game.h"
#include "graph_gen.h"
#include "ennemy.h"
#include "graph_curses.h"

uint32_t def_distx = 8; 
uint32_t def_disty = 8;
bool fancy_mode = 1; 

static err_flag reset_elements(er_graph * g, dynarr_points * darp, er_player * p, er_entab * entab){

    free_graph(g); 
    free_dynp(darp);
    free_entab(entab);
    p->x = 0 ; 
    p->y = 0 ;
    p->cur_node = NULL ; 

    return ERR_OK;
}

static err_flag move_player(WINDOW * w, const er_graph * g , dynarr_points * darp, er_player * p, uint32_t index ){

   
    if(index >= p->cur_node->cur){
        return ERR_OK;
    }

    for(uint32_t i = 0 ; i < p->cur_node->cur ; i++){
        uint32_t cur_index = p->cur_node->neighboors_ref[i] - g->adjacency_lists;
        if(!fancy_mode){
            wmove(w, darp->elems[cur_index].y*def_disty, darp->elems[cur_index].x*def_distx);
            waddch(w,'O');
        }else{
            wmove(w, darp->elems[cur_index].y*def_disty +1, darp->elems[cur_index].x*def_distx+1);
            waddch(w,' ');
        }
    }

    uint32_t cur_index = p->cur_node - g->adjacency_lists;
    if(!fancy_mode){
        wmove(w,darp->elems[cur_index].y*def_disty, darp->elems[cur_index].x*def_distx);
        waddch(w,'O');
    }else{
        wmove(w,darp->elems[cur_index].y*def_disty+1, darp->elems[cur_index].x*def_distx+1);
        waddch(w,' ');
    }

    cur_index = p->cur_node->neighboors_ref[index] - g->adjacency_lists;
    if(!fancy_mode){
        wmove(w,darp->elems[cur_index].y*def_disty, darp->elems[cur_index].x*def_distx);
        waddch(w,p->ch);
    }else{
        wmove(w,darp->elems[cur_index].y*def_disty+1, darp->elems[cur_index].x*def_distx+1);
        waddch(w,p->ch);
    }

    p->y = darp->elems[cur_index].y;
    p->x = darp->elems[cur_index].x;
    p->cur_node = p->cur_node->neighboors_ref[index];

    wrefresh(w);
    return ERR_OK;
}

err_flag start_game(WINDOW * w , er_graph * g, dynarr_points * darp, er_player *p ){
    /*
    function to call to start the game. 

    w -> not null 
    g -> not null & initialized | not initialized
    darp -> not null & initialized | not initialized
    p -> not null
    */
    def_err_handler(!w, "start_game w", ERR_NULL);
    def_err_handler(!g,"start_game g", ERR_NULL);
    def_err_handler(!darp, "start_game darp", ERR_NULL);
    def_err_handler(!p,"start_game p", ERR_NULL);
    def_err_handler(g->nb_nodes > darp->cur, "start_game", ERR_VALS);


    chtype ch = 0 ; 
    uint32_t nb_points = 0;

    declare_er_exit(e,0,0);
    declare_er_ennemy(en,0,0);
    declare_entab(entab);   

    do{
        
        uint32_t nb_pts_won = 20 ; 

        generate_lattice(g,row_size);
        randomize_lattice(g,row_size, 0.3,0.1,0.8,0.85);
        init_dynp(darp, default_arr_size);//shitty ; better flush it
        init_entab(&entab, 2);
        append_entab(&entab, &en, &default_enrules);

        gen_coordinates(row_size,row_size,darp);

        init_ent_pos(&e,p,&entab,g,darp);
    
        if(!fancy_mode){
            wprint_surroundings(w,p,darp,def_distx,def_disty,g);
            wprint_player(w,p,def_distx, def_disty);
            wprint_exit(w,&e,def_distx,def_disty);
            //wprint_ennemy(w,&en,def_distx,def_disty);
            wprint_entab_fancy(w,&entab,def_distx,def_disty);
        }else{
            wprint_surroundings_fancy(w,p,darp,def_distx,def_disty,g);
            wprint_player_fancy(w,p,def_distx, def_disty);
            wprint_exit_fancy(w,&e,def_distx,def_disty);
            //wprint_ennemy_fancy(w,&en,def_distx,def_disty);
            wprint_entab_fancy(w,&entab,def_distx,def_disty);
        }
   

        while(p->cur_node != e.cur_node && ch!='q'){
            
            ch = wgetch(w);

            if(ch >= '0' && ch <= '9' && ch != 'q'){
                if(ch-'0' < p->cur_node->cur){
                    nb_pts_won = nb_pts_won > 0 ? nb_pts_won - 1 : 0 ;

                    move_player(w,g,darp,p,ch-'0'); 
                    //move_random(w, &en , g, darp,  &e);
                    //move_closest(w,&en,g,darp,p);
                    bool lost = 0 ; 
                    move_ennemies(w,&entab,p,darp,g,&lost);

                    if(!fancy_mode){
                        wprint_surroundings(w,p,darp,def_distx,def_disty,g);
                        wprint_player(w,p,def_distx, def_disty);
                        wprint_exit(w,&e,def_distx,def_disty);
                        //wprint_ennemy(w,&en,def_distx,def_disty);
                        wprint_entab(w,&entab,def_distx,def_disty);
                    }else{
                        wprint_surroundings_fancy(w,p,darp,def_distx,def_disty,g);
                        wprint_player_fancy(w,p,def_distx, def_disty);
                        wprint_exit_fancy(w,&e,def_distx,def_disty);
                        //wprint_ennemy_fancy(w,&en,def_distx,def_disty);
                        wprint_entab_fancy(w,&entab,def_distx,def_disty);
                    }

                    if(lost){
                        
                        mvwprintw(w,0,0,"you died\n");
                        wrefresh(w);
                        wgetch(w);

                        ch = 'q';
                        break;  
                    }    
                }           
            }
        }

       
        nb_points += nb_pts_won;
        reset_elements(g, darp, p, &entab);
        clear();
        
    }while(ch != 'q');
    mvwprintw(w,0,0,"you scored %u points\n",nb_points);
    wrefresh(w);
    wgetch(w);
    return ERR_OK ;
}
