#include "graph_curses.h"
#include "game.h"
#include "misc.h"

static err_flag draw_diag_ur(WINDOW * w, chtype ch, er_points p1, er_points p2 ){
    /*
    diagonal up left (p1.y >  p1.x && p2.x > p1.x)
    */
    uint32_t nb_up = p2.y - p1.y ;
    uint32_t nb_ri = p2.x - p1.x ;

    uint32_t max = nb_up > nb_ri ? nb_up : nb_ri ; 
    uint32_t coeffup = nb_up / nb_ri  > 0 ? nb_up / nb_ri : 1 ; 
    uint32_t coeffri = nb_ri / nb_up > 0 ? nb_ri / nb_up : 1  ;

    er_points cur_p = p1;
    move(cur_p.y, cur_p.x);

    if(colors_on){
        attron(COLOR_PAIR(COLOR_LINK));
    }
    waddch(w,ch);
   
    while(max){
        for(uint32_t i = 0 ; i < coeffup ; i++){
            if(nb_up){  
                cur_p.y++; 
                move(cur_p.y, cur_p.x);
                waddch(w,ch);
                nb_up--;
                // if(nb_up== 0)break;
                
            }
        }
        for(uint32_t i = 0 ; i < coeffri; i++){
            if(nb_ri){         
                cur_p.x++;
                move(cur_p.y, cur_p.x);
                waddch(w,ch);
                nb_ri--;
            }
        }
        if(!nb_up && !nb_ri) max = 1 ; 
        max--; 
    }
    if(colors_on){
        attroff(COLOR_PAIR(COLOR_LINK));
    }
    return ERR_OK;
}//tested; works; unsafe (doesn't check args)

static err_flag draw_diag_dr(WINDOW * w, chtype ch, er_points p1, er_points p2 ){
    /*
    diagonal down right (p1.y <  p1.y && p2.x > p1.x)
    */
    uint32_t nb_down = p1.y - p2.y ;
    uint32_t nb_ri = p2.x - p1.x ;

    uint32_t max = nb_down > nb_ri ? nb_down : nb_ri ; 

    uint32_t coeffdo = nb_down / nb_ri  > 0 ? nb_down / nb_ri : 1 ; 
    uint32_t coeffri = nb_ri / nb_down > 0 ? nb_ri / nb_down : 1  ;
    
    er_points cur_p = p1;
    move(cur_p.y, cur_p.x);
    
    if(colors_on){
        attron(COLOR_PAIR(COLOR_LINK));
    }

    waddch(w,ch);
    
    while(max){
        for(uint32_t i = 0 ; i < coeffdo ; i++){
            if(nb_down){  
                cur_p.y--; 
                move(cur_p.y, cur_p.x);
                waddch(w,ch);
                
                nb_down--;
            }
        }
        for(uint32_t i = 0 ; i < coeffri ; i++){
            if(nb_ri){
                cur_p.x++;
                move(cur_p.y, cur_p.x);
                waddch(w,ch);
                nb_ri--;
            }
        }
        if(!nb_down && !nb_ri) max = 1 ; 
        max--; 
    }
    if(colors_on){
        attroff(COLOR_PAIR(COLOR_LINK));
    }
    wrefresh(w);
   return ERR_OK;
}//tested; works; unsafe (doesn't check args)

static err_flag wprint_node_fancy(WINDOW * w, const er_points * p1, uint32_t distx, uint32_t disty, int color){
    wmove(w,p1->y*disty ,p1->x*distx+1);
    if(colors_on){
        attron(COLOR_PAIR(color));
    }
    wprintw(w,"__");
    mvwprintw(w, p1->y*disty + 1, p1->x*distx, "/  \\");
    mvwprintw(w, p1->y*disty + 2, p1->x*distx, "\\__/");
    if(colors_on){
        attroff(COLOR_PAIR(color));     
    }
    return ERR_OK;
}

err_flag wprint_link_fancy(WINDOW * w, const er_points * p1, const er_points * p2, uint32_t distx, uint32_t disty, int c1, int c2){
   
    if(colors_on){
        attron(COLOR_PAIR(COLOR_LINK));
    }
    er_points tmp_p1 = {p1->x *distx, p1->y *disty};
    er_points tmp_p2 = {p2->x *distx, p2->y *disty};

    if(p1->x == p2->x){     
        if(p1->y > p2->y){        
            wmove(w,tmp_p2.y+3,tmp_p2.x);
            wvline(w,'#', tmp_p1.y - tmp_p2.y - 2);
        }else if (p1->y < p2->y){         
            wmove(w,tmp_p1.y+3, tmp_p1.x);
            wvline(w,'#', tmp_p2.y - tmp_p1.y - 2);
        }
    }else if(p1->x < p2->x){

        if(p1->y > p2->y){
            tmp_p2.y += 3 ; 
            tmp_p1.x += 3 ; 
            draw_diag_dr(w, '#', tmp_p1,tmp_p2);
        }else if(p1->y < p2->y){
            tmp_p1.y += 3 ; 
            tmp_p1.x += 3 ; 
            draw_diag_ur(w, '#', tmp_p1,tmp_p2);
        }else{// if(p1->y == p2->y)
            tmp_p1.y += 1 ; 
            tmp_p1.x += 4 ; 
            wmove(w, tmp_p1.y, tmp_p1.x);
            whline(w,'#', tmp_p2.x - tmp_p1.x );
        }
    }else if(p1->x > p2->x ){
        if(p1->y > p2->y){          
            tmp_p2.y +=3 ;
            tmp_p2.x += 3 ; 
            draw_diag_ur(w, '#', tmp_p2,tmp_p1);
        }else if(p1->y < p2->y){       
            tmp_p2.x += 3 ; 
            tmp_p1.y +=3 ;          
            draw_diag_dr(w, '#', tmp_p2, tmp_p1);
        }else{// if(p1->y == p2->y)
            tmp_p2.y += 1 ; 
            tmp_p2.x += 4 ;
            wmove(w, tmp_p2.y, tmp_p2.x);
            whline(w, '#', tmp_p1.x - tmp_p2.x);
        }
    }
    if(colors_on){
        attroff(COLOR_PAIR(COLOR_LINK));
    }
    wrefresh(w);
    return ERR_OK;
}//ok

err_flag update_gprint_fancy(WINDOW * w,  er_graph * g, dynarr_points * darp, er_game_entities * gent){
    
    def_err_handler(!w,"wprintw_graph w", ERR_NULL);
    def_err_handler(!darp,"wprintw_graph darp", ERR_NULL);
    def_err_handler(!darp->elems,"wprintw_graph darp->elems", ERR_NULL);
    def_err_handler(!g,"wprintw_graph g", ERR_NULL);
    def_err_handler(!g->adjacency_lists,"wprintw_graph g->adj_lists", ERR_NULL);
    def_err_handler(!gent,"wprintw_graph gent", ERR_NULL);

    if(colors_on){
        for(uint32_t i = 0 ; i < gent->ennemies->cur ; i++){
            uint32_t index_ennemy = gent->ennemies->ennemies[i].cur_node - g->adjacency_lists;
            g->col_cur[index_ennemy] = gent->ennemies->ennemies[i].color ; 
        }

        uint32_t index_exit = gent->ex->cur_node - g->adjacency_lists;
        g->col_cur[index_exit] = gent->ex->color;

        uint32_t index_player = gent->p->cur_node - g->adjacency_lists ; 
        g->col_cur[index_player] = gent->p->color;
    }
    
    wupdate_links_fancy(w, gent->p, darp, def_distx, def_disty,g, gent);

    if(colors_on){
        for(uint32_t i = 0 ; i < g->nb_nodes ; i++ ){
            if(g->printed_nodes[i] && g->visited[i]){
            
                wprint_node_fancy(w, &darp->elems[i], def_distx, def_disty, g->col_cur[i]);
                g->col_cur[i] = COLOR_NODE;
            }else if(g->printed_nodes[i]){
                wprint_node_fancy(w, &darp->elems[i], def_distx, def_disty, g->col_cur[i]);
                g->col_cur[i] = COLOR_NODE_NEW;
            
            }else{
                g->col_cur[i] = COLOR_NODE_NEW;
            }
        }
    }else{
        for(uint32_t i = 0 ; i < g->nb_nodes ; i++ ){
            if(g->printed_nodes[i]){    
                wprint_node_fancy(w, &darp->elems[i], def_distx, def_disty, -1);
            }
        }    
    }
    wprint_entab_fancy(w ,gent->ennemies, def_distx, def_disty);
    wprint_entity_fancy(w,gent->ex, def_distx, def_disty);
    wprint_entity_fancy(w,gent->p, def_distx, def_disty); 
    
    er_player * en = gent->p;
    for(uint32_t i = 0 ; i < en->cur_node->cur; i++){

        uint32_t index = en->cur_node->neighboors_ref[i] - g->adjacency_lists;
        wmove(w, darp->elems[index].y*def_disty+1, darp->elems[index].x*def_distx+1);     
        waddch(w,i+ '0');      
    }
    wrefresh(w);
    return ERR_OK;
}

/*these functions are for the ugly mode of edgerunners*/
err_flag wprint_link(WINDOW * w, const er_points * p1, const er_points * p2, uint32_t distx, uint32_t disty){
    /*
    too lazy to write comment smh
    */
    def_err_handler(!w,"wprintw_link w", ERR_NULL);
    def_err_handler(!p1,"wprintw_link p1", ERR_NULL);
    def_err_handler(!p2,"wprintw_link p2", ERR_NULL);

    er_points tmp_p1 = {p1->x *distx, p1->y *disty};
    er_points tmp_p2 = {p2->x *distx, p2->y *disty};

    if(colors_on){
        attron(COLOR_PAIR(COLOR_LINK));
    }
    if(p1->x == p2->x){
        
        if(p1->y > p2->y){
       
            wmove(w,tmp_p2.y,tmp_p2.x);
            wvline(w,'#', tmp_p1.y - tmp_p2.y);
        }else if (p1->y < p2->y){
           
            wmove(w,tmp_p1.y, tmp_p1.x);
            wvline(w,'#', tmp_p2.y - tmp_p1.y);
        }

    }else if(p1->x < p2->x){

        if(p1->y > p2->y){
            
            draw_diag_dr(w, '#', tmp_p1,tmp_p2);
        }else if(p1->y < p2->y){
           
            draw_diag_ur(w, '#', tmp_p1,tmp_p2);
        }else{// if(p1->y == p2->y)
           
            wmove(w, tmp_p1.y, tmp_p1.x);
            whline(w,'#', tmp_p2.x - tmp_p1.x);
        }

    }else if(p1->x > p2->x ){
        if(p1->y > p2->y){
    
            draw_diag_ur(w, '#', tmp_p2,tmp_p1);
        }else if(p1->y < p2->y){
             
            draw_diag_dr(w, '#', tmp_p2, tmp_p1);
        }else{// if(p1->y == p2->y)
        
            wmove(w, tmp_p2.y, tmp_p2.x);
            whline(w, '#', tmp_p1.x - tmp_p2.x);
        }
    }
    if(colors_on){
        attroff(COLOR_PAIR(COLOR_LINK));
    }
  
    wrefresh(w);
    return ERR_OK;
}//tested; works; safe (checks for args)
//writing clever code is for babies we stan big ass disjunctive forms in this house. 
//I wanna handle (a^b) v (~a^b) v (a^~b) v etc ? splendid lemme write all of the cases then. 

#ifdef debug
static err_flag wprintw_links(WINDOW * w, dynarr_points * darp, uint32_t distx, uint32_t disty, er_graph * g){

    if(colors_on){
        attron(COLOR_PAIR(COLOR_LINK));
    }
    for(uint32_t i = 0 ; i < g->nb_nodes ; i ++){
        for(uint32_t j = 0 ; j < g->adjacency_lists[i].cur; j++){
            if(g->adjacency_lists[i].neighboors_ref[j] > &g->adjacency_lists[i]){
                if(!g->adjacency_lists[i].printed_links[j] ){
                
                //sets the link attributes to 1 lol
                g->adjacency_lists[i].printed_links[j] = 1; 
                for(uint32_t k = 0 ; k < g->adjacency_lists[i].neighboors_ref[j]->cur; k++){
                    if(g->adjacency_lists[i].neighboors_ref[j]->neighboors_ref[k] == &g->adjacency_lists[i]){
                        g->adjacency_lists[i].neighboors_ref[j]->printed_links[k] = 1; 
                    }
                }

                //fprintf(stderr, "i=%u, j=%u, g->nb_nodes=%u\n", i,j, g->nb_nodes);
                er_points p2 = darp->elems[g->adjacency_lists[i].neighboors_ref[j] - g->adjacency_lists];
                er_points p1 = darp->elems[i];

                if(p1.x == p2.x){
                   
                    move(p1.y*disty+1, p1.x*distx);
                    wvline(w,ACS_BLOCK, p2.y*disty - p1.y*disty - 1   );
                    wrefresh(w);
                }else if(p1.y == p2.y){
                
                    move(p1.y*disty, p1.x*distx+1);
                    whline(w,ACS_DIAMOND,p2.x*distx - p1.x*distx - 1  );
                    wrefresh(w);
                }else if(p1.x < p2.x ){
                    //diagonal case :O
                    p1.x=p1.x*distx + 1 ; 
                    p1.y*=disty;
                    p2.x*=distx; 
                    p2.y= p2.y * disty;
                 
                    if(p1.y < p2.y){
                        p2.y -=1; 
                        draw_diag_ur(w,ACS_BULLET,p1,p2);
                    }else{//p1.y > p2.y
                        p2.y+=1;
                        draw_diag_dr(w,ACS_BULLET,p1,p2);
                    }
                }
                }
            }
        }
    }
    if(colors_on){
        attroff(COLOR_PAIR(COLOR_LINK));
    }
    return ERR_OK;
}//doesnt check for shit ; ok though since it's static :)

static err_flag wprintw_nodes(WINDOW * w, uint32_t distx, uint32_t disty, dynarr_points * darp, er_graph * g){
    
    if(colors_on){
        attron(COLOR_PAIR(COLOR_NODE));
    }
    for(uint32_t i = 0 ; i < darp->cur ; i ++){
        mvwprintw(w,darp->elems[i].y*disty, darp->elems[i].x*distx,"O");
        g->printed_nodes[i] = 1; 
    }
    if(colors_on){
        attroff(COLOR_PAIR(COLOR_NODE));
    }
    wrefresh(w);
    return ERR_OK;
}//tested; works ; doesn't check args (it's ok bc it's static)


err_flag wprintw_graph(WINDOW * w, dynarr_points * darp, uint32_t distx, uint32_t disty, er_graph * g){
    /*
    prints a graph in the window w 
    */
    def_err_handler(!w,"wprintw_graph w", ERR_NULL);
    def_err_handler(!darp,"wprintw_graph darp", ERR_NULL);
    def_err_handler(!darp->elems,"wprintw_graph darp->elems", ERR_NULL);
    def_err_handler(!g,"wprintw_graph g", ERR_NULL);
    def_err_handler(!g->adjacency_lists,"wprintw_graph g->adj_lists", ERR_NULL);

    wprintw_nodes(w,distx,disty,darp,g);
    wprintw_links(w,darp,distx,disty,g);
    return ERR_OK;
}//tested; works ; might not use in the game (replace w print link)
#endif
