#include "graph_curses.h"


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
                    //if(nb_ri== 0)break;    
            }
        }
        if(!nb_up && !nb_ri) max = 1 ; 
        max--; 
    }

    return ERR_OK;
}//doesnt check for args;

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
    waddch(w,ch);
    FILE * f = fopen("dump", "a");
    fprintf(f, "coeffdo=%u coeffri=%u\n", coeffdo, coeffri);
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
    wrefresh(w);
   return ERR_OK;
}//doesnt check args

static err_flag wprintw_edges(WINDOW * w, dynarr_points * darp, uint32_t distx, uint32_t disty, er_graph * g){

    for(uint32_t i = 0 ; i < g->nb_nodes ; i ++){
        for(uint32_t j = 0 ; j < g->adjacency_lists[i].cur; j++){
            if(g->adjacency_lists[i].neighboors_ref[j] > &g->adjacency_lists[i]){
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
    return ERR_OK;
}//doesnt check for shit 

static err_flag wprintw_nodes(WINDOW * w, uint32_t distx, uint32_t disty, dynarr_points * darp){
    
    for(uint32_t i = 0 ; i < darp->cur ; i ++){
        mvwprintw(w,darp->elems[i].y*disty, darp->elems[i].x*distx,"O");
    }
    wrefresh(w);
    return ERR_OK;
}//works ; doenst check for shit


err_flag wprintw_graph(WINDOW * w, dynarr_points * darp, uint32_t distx, uint32_t disty, er_graph * g){

    wprintw_nodes(w,distx,disty,darp);
    wprintw_edges(w,darp,distx,disty,g);
    return ERR_OK;
}//doesn't do checks atm ; not finished
//actually I should make a function to print links.

err_flag wprint_link(WINDOW * w, er_points * p1, er_points * p2, uint32_t distx, uint32_t disty){
    /*
    too lazy to write comment smh
    */
    wmove(w,p1->y*disty,p1->x*distx);
    waddch(w,'O');

    wmove(w,p2->y*disty,p2->x*distx);
    waddch(w,'O');

    if(p1->x == p2->x){
        
        if(p1->y > p2->y){
            wmove(w,p2->y,p2->x);
            wvline(w,ACS_BTEE,p1->y);

        }else if (p1->y < p2->y){
            wmove(w,p1->y,p1->x);
            wvline(w,ACS_BTEE, p2->y);
        }else{
            ;
        }
    }else if(p1->x < p2->x){

        if(p1->y > p2->y){
            draw_diag_dr(w, ACS_BULLET, *p1,*p2);
        }else if(p1->y < p2->y){
            draw_diag_ur(w, ACS_BULLET, *p1,*p2);
        }else{// if(p1->y == p2->y)
            wmove(w, p1->y, p1->x);
            whline(w,ACS_DIAMOND, p2->x);
        }
    }else{ // if(p1->x > p2->x )
        if(p1->y > p2->y){
            draw_diag_dr(w, ACS_BULLET, *p2,*p1);
        }else if(p1->y < p2->y){
            draw_diag_ur(w, ACS_BULLET, *p2,*p1);
        }else{// if(p1->y == p2->y)
            wmove(w, p2->y, p2->x);
            whline(w,ACS_DIAMOND, p1->x);
        }
    }
    wrefresh(w);
    return ERR_OK;
}//not tested; 100% wrong I think
//writing clever code is for babies we stan big ass disjunctive forms in this house. 
//I wanna handle (a^b) v (~a^b) v (a^~b) v etc ? splendid lemme write all of the cases then. 

