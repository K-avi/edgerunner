#include "entities.h"
#include "misc.h"
#include "curses.h"
#include "graph_curses.h"
#include "ennemy.h"

static err_flag init_player_pos(er_player * pl , er_graph * g,  dynarr_points * darp, uint32_t * player_index){
    /*
    darp -> not null & init 
    pl -> not null & init
    */
    uint32_t pl_index = rand()%darp->cur ;
    while(g->adjacency_lists[pl_index].cur == 0){
        pl_index = rand()%darp->cur;
    }

    pl->x = darp->elems[pl_index].x;
    pl->y = darp->elems[pl_index].y;
    pl->cur_node = &g->adjacency_lists[pl_index];
    *player_index = pl_index;
   return ERR_OK;
}//ok

static err_flag init_exit_pos(er_exit * ex , er_graph * g,  dynarr_points * darp, uint32_t player_index, uint32_t * ex_pos){
    /*
    darp -> not null & init 
    pl -> not null & init
    */
    uint32_t ex_index = rand()%darp->cur ;
    while(ex_index == player_index || g->adjacency_lists[ex_index].cur == 0){
        ex_index = rand()%darp->cur;
    }

    ex->x = darp->elems[ex_index].x;
    ex->y = darp->elems[ex_index].y;
    ex->cur_node = &g->adjacency_lists[ex_index];
   *ex_pos = ex_index;
   return ERR_OK;
}//ok

static err_flag init_ennemy_pos(er_ennemy * en , er_graph * g,  dynarr_points * darp, uint32_t player_index, uint32_t exit_index){
    /*
    darp -> not null & init 
    pl -> not null & init
    */
    uint32_t en_index = rand()%darp->cur ;
    while(en_index == player_index || en_index == exit_index || g->adjacency_lists[en_index].cur == 0 ){
        en_index = rand()%darp->cur;
    }

    en->x = darp->elems[en_index].x;
    en->y = darp->elems[en_index].y;
    en->cur_node = &g->adjacency_lists[en_index];

   return ERR_OK;
}//ok

static err_flag init_pos_entab(er_entab * entab,er_graph * g,  dynarr_points * darp, uint32_t player_index, uint32_t exit_index){

    for(uint32_t i = 0 ; i < entab->cur ; i++){
        err_flag failure = init_ennemy_pos(&entab->ennemies[i],g,darp,player_index,exit_index);
        def_err_handler(failure, "init_pos_entab", failure);
    }
    return ERR_OK;

}

err_flag init_ent_pos(er_exit * ex, er_player* pl, er_entab * entab, er_graph * g,  dynarr_points * darp){
    uint32_t pl_i, ex_i;
    init_player_pos(pl,g,darp,&pl_i);
    init_exit_pos(ex,g,darp,pl_i, &ex_i);
    init_pos_entab(entab,g,darp,pl_i, ex_i);
    return ERR_OK;
}

err_flag wprint_entity(WINDOW * w , er_player * pl, uint32_t distx, uint32_t disty){
    
    wmove(w,pl->y*disty, pl->x*distx);
    waddch(w,pl->ch);
    wrefresh(w);
    return ERR_OK;
}//ok

err_flag wprint_surroundings(WINDOW *w ,er_entity * en , dynarr_points * darp, uint32_t distx, uint32_t disty, const er_graph * g ){
    /*
    */
   g->printed_nodes[en->cur_node - g->adjacency_lists] = 1 ;
   for(uint32_t i = 0 ; i < en->cur_node->cur; i++){
        uint32_t index = en->cur_node->neighboors_ref[i] - g->adjacency_lists;
        if(!en->cur_node->printed_links[i]){
            en->cur_node->printed_links[i] = 1 ;
            for(uint32_t k = 0 ; k < en->cur_node->neighboors_ref[i]->cur ; k++){
                if(en->cur_node->neighboors_ref[i]->neighboors_ref[k] == en->cur_node){
                    en->cur_node->neighboors_ref[i]->printed_links[k] = 1;
                }
            }
            
            err_flag failure = wprint_link(w, &darp->elems[ en->cur_node - g->adjacency_lists], &darp->elems[index],distx, disty );
            def_err_handler(failure,"wprint_surroundings",failure);
        }
        wmove(w, darp->elems[index].y*disty, darp->elems[index].x*distx);     
        waddch(w,i+ '0');      
   }
   wrefresh(w);
   return ERR_OK;
}

err_flag wupdate_links_fancy(WINDOW *w ,er_entity * en , dynarr_points * darp, uint32_t distx, uint32_t disty, const er_graph * g, const er_game_entities * gent ){
    g->printed_nodes[en->cur_node - g->adjacency_lists] = 1 ;
   if(colors_on) g->visited[en->cur_node - g->adjacency_lists] = 1;
   for(uint32_t i = 0 ; i < en->cur_node->cur; i++){

        uint32_t index = en->cur_node->neighboors_ref[i] - g->adjacency_lists;    
        
        en->cur_node->printed_links[i] = 1 ;
        g->printed_nodes[index] = 1;
        for(uint32_t k = 0 ; k < en->cur_node->neighboors_ref[i]->cur ; k++){
            if(en->cur_node->neighboors_ref[i]->neighboors_ref[k] == en->cur_node){
                en->cur_node->neighboors_ref[i]->printed_links[k] = 1;
            }
        }       
        if(colors_on){
            err_flag failure = wprint_link_fancy(w, &darp->elems[ en->cur_node - g->adjacency_lists], &darp->elems[index],distx, disty, g->col_cur[en->cur_node - g->adjacency_lists] , g->col_cur[index]  );
            def_err_handler(failure,"wprint_surroundings",failure);
        }else{
            err_flag failure = wprint_link_fancy(w, &darp->elems[ en->cur_node - g->adjacency_lists], &darp->elems[index],distx, disty, -1, -1  );
            def_err_handler(failure,"wprint_surroundings",failure);
        }       
   }
   return ERR_OK;
}   


err_flag wprint_entity_fancy(WINDOW * w , er_player * pl, uint32_t distx, uint32_t disty ){
    
    if(colors_on){
        attron(COLOR_PAIR(pl->color));
    }
    wmove(w,pl->y*disty+1, pl->x*distx+1);
    waddch(w,pl->ch);
    if(colors_on){
        attroff(COLOR_PAIR(pl->color));
    }
    wrefresh(w);
    return ERR_OK;
}
