#include "ennemy.h"
#include "game.h"
#include "search_utils.h"

//global rule thing 
rule_fun default_rule_functions[3] = { move_random, move_rest, move_closest};
uint16_t default_rulescoeff[3] = { 26214, 26214, 13107};//coeffs are uint16 cuz rand()
//is weird and floating point operations are fucked up

er_enrules default_enrules = {3, default_rule_functions, default_rulescoeff};

err_flag move_random(WINDOW * w , er_ennemy * en , er_graph * g, dynarr_points * darp, er_exit * ex){
    /*
    */
    //don't go on the exit ennemy please !!!
    def_err_handler(!en, "move_random en", ERR_NULL);
    def_err_handler(!en->cur_node, "move_random en->cur_node", ERR_NULL);
    def_err_handler(!g, "move_random g", ERR_NULL);
    def_err_handler(!darp, "move_random darp", ERR_NULL);
    def_err_handler(!ex, "move_random en", ERR_NULL);

    if(en->cur_node->cur == 1 && ex->cur_node == en->cur_node->neighboors_ref[0]){
        return ERR_OK;
    }
    if(!fancy_mode){
        if(g->printed_nodes[en->cur_node - g->adjacency_lists]){
            mvwprintw(w,en->y * def_disty, en->x * def_distx, "O");
        }else{
            mvwprintw(w,en->y * def_disty, en->x * def_distx, " ");
        }
    }else{
        mvwprintw(w,en->y * def_disty, en->x * def_distx +1, " ");
    }
    uint32_t index = rand()%en->cur_node->cur;
    while(en->cur_node->neighboors_ref[index] == ex->cur_node){
        index = rand()%en->cur_node->cur;
    }

    en->x = darp->elems[ en->cur_node->neighboors_ref[index] - g->adjacency_lists].x;
    en->y = darp->elems[ en->cur_node->neighboors_ref[index] - g->adjacency_lists].y;
    en->cur_node = en->cur_node->neighboors_ref[index];

    return ERR_OK;
}

err_flag move_closest(WINDOW * w , er_ennemy * en , er_graph * g, dynarr_points * darp, er_player * p){
    /* 
    uses a BFS to find the closest way to approach the player
    */
    def_err_handler(!en,"move_closest en", ERR_NULL);
    def_err_handler(!en->cur_node,"move_closest en->cur_node", ERR_NULL);
    
    if(!en->cur_node->cur){
        return ERR_OK;
    }

    struct s_graph_entry * min_node = NULL ; 
    int64_t min_dist = INT64_MAX  ;

    for(uint32_t i = 0 ; i < en->cur_node->cur ; i++){
        int64_t tmp_dist = - 1 ;
        bfs_graph(g, en->cur_node->neighboors_ref[i],p->cur_node,&tmp_dist);
        if(tmp_dist != -1 && tmp_dist < min_dist){
            min_dist = tmp_dist ; 
            min_node = en->cur_node->neighboors_ref[i] ;
        }
    }
    def_err_handler(!min_node, "move_closest", ERR_NULL);
    
    if(g->printed_nodes[en->cur_node - g->adjacency_lists]){
        if(!fancy_mode){
            mvwprintw(w,en->y * def_disty, en->x * def_distx, "O");
        }else{
            mvwprintw(w,en->y * def_disty+1, en->x * def_distx+1, " ");
        }
    }else{
        if(!fancy_mode){
            mvwprintw(w,en->y * def_disty, en->x * def_distx, " ");
        }else{
             mvwprintw(w,en->y * def_disty+1, en->x * def_distx+1, " ");
        }
    }

    en->cur_node = min_node ; 
   // fprintf(stderr,"minnode %p\n", min_node);
    en->x = darp->elems[min_node - g->adjacency_lists].x;
    en->y = darp->elems[min_node - g->adjacency_lists].y;
  
    return ERR_OK;
}


err_flag move_rest(WINDOW * w , er_ennemy * en , er_graph * g, dynarr_points * darp, er_player * p){

    if(g->printed_nodes[en->cur_node - g->adjacency_lists]){
        if(!fancy_mode){
            mvwprintw(w,en->y * def_disty, en->x * def_distx, "O");
        }else{
             mvwprintw(w,en->y * def_disty+1, en->x * def_distx+1, " ");
        }
    }else{
        if(!fancy_mode){
            mvwprintw(w,en->y * def_disty, en->x * def_distx, " ");
        }else{
             mvwprintw(w,en->y * def_disty+1, en->x * def_distx+1, " ");
        }
    }
    return ERR_OK;
}


err_flag init_enrules(er_enrules * dst, er_enrules * src){
    def_err_handler(!dst,"init_enrules", ERR_NULL);

    dst->rule_functions = malloc( src->size *sizeof(rule_fun));
    def_err_handler(!dst->rule_functions,"init_enrules", ERR_ALLOC);
    dst->coeffs = malloc( src->size *sizeof(uint16_t));
    def_err_handler(!dst->coeffs,"init_enrules", ERR_ALLOC);
    
    dst->size = src->size ;

    for(uint32_t i = 0 ; i < src->size ; i++){
        dst->coeffs[i] = src->coeffs[i]; 
        dst->rule_functions[i] = src->rule_functions[i];
    }
    
    return ERR_OK;
}

err_flag free_enrules(er_enrules * en ){
    if(en){
        if(en->coeffs) free(en->coeffs); 
        if(en->rule_functions) free(en->rule_functions); 

        en->coeffs = NULL;
        en->rule_functions = NULL;
    }
    return ERR_OK;
}



err_flag update_enrules(er_enrules * en , uint32_t index_incr, double coeff_incr){
    
    def_err_handler(!en,"update_enrules", ERR_NULL);
    def_err_handler(!en->coeffs,"update_enrules coeffs", ERR_NULL);


    uint32_t val_incr = min(UINT16_MAX , coeff_incr * en->coeffs[index_incr]);
    int32_t val_decr = val_incr/(en->size-1);

    en->coeffs[index_incr]= (uint16_t) min( UINT16_MAX, (uint32_t) val_incr + en->coeffs[index_incr]);
    
    for(uint32_t i = 0 ; i < en->size ; i++){
        if(i!=index_incr){   
            en->coeffs[i] = (uint16_t) max(0, ((int32_t)en->coeffs[i]) - val_decr) ;
        }
    }
    return ERR_OK;
}//not tested; doesnt check for shit 
/******/

extern err_flag init_entab(er_entab * entab, uint32_t size){
    def_err_handler(!entab, "init_entab", ERR_NULL);

    entab->ennemies = calloc(size, sizeof(er_ennemy));
    def_err_handler(!entab->ennemies, "init_entab ennemies", ERR_ALLOC);

    entab->rules = calloc(size, sizeof(er_enrules));
    def_err_handler(!entab->rules, "init_entab rules", ERR_ALLOC);

    entab->cur = 0 ; 
    entab->max = size ; 

    return ERR_OK;
}

static inline err_flag realloc_entab(er_entab * entab , double coeff){

    err_flag failure =  generic_realloc((void**)&entab->ennemies, sizeof(er_ennemy), coeff * (double) entab->max +1 );
    def_err_handler(failure, "realloc_entab" ,failure);

    failure =  generic_realloc((void**)&entab->rules, sizeof(er_enrules), coeff * (double)entab->max +1 );
    def_err_handler(failure, "realloc_entab" ,failure);

    entab->max = coeff * (double) entab->max +1 ;
    
    return ERR_OK;
}

err_flag append_entab(er_entab * entab , const er_ennemy * ennemy , er_enrules * rules ){
    
    def_err_handler(!entab, "append_entab entab", ERR_NULL);
    def_err_handler(!ennemy, "append_entab ennemy", ERR_NULL);
    def_err_handler(!rules, "append_entab rules", ERR_NULL);

    if(entab->cur == entab->max){
        err_flag failure = realloc_entab(entab, default_realloc);
        def_err_handler(failure, "append_entab", failure);
    }
    uint32_t cur = entab->cur;

    err_flag failure = init_enrules(&entab->rules[cur], rules);
    def_err_handler(failure, "append_entab", failure);

    //might be very wrong
    memcpy( &entab->ennemies[cur], ennemy, sizeof(er_ennemy));

    entab->cur++;
    return ERR_OK;
}

err_flag free_entab(er_entab * en ){

    for(uint32_t i = 0 ; i < en->cur ; i++){
        free_enrules(&en->rules[i]);
    }
    free(en->ennemies); 
    free(en->rules); 
    en->ennemies = NULL;
    en->rules = NULL; 
    en->cur = en->max = 0 ; 

    return ERR_OK;
}

err_flag update_entab(er_entab * entab, const uint32_t * indexes_incr, const double * coeffs_incr ){
    def_err_handler(!entab, "update_entab", ERR_NULL);

    for(uint32_t i = 0 ; i < entab->cur ; i++){
        err_flag failure = update_enrules(&entab->rules[i], indexes_incr[i], coeffs_incr[i]);
        def_err_handler(failure,"update_entab",failure);
    }
    return ERR_OK;
}

err_flag wprint_entab(WINDOW * w , er_entab * en, uint32_t distx, uint32_t disty){
    
    for(uint32_t i = 0 ; i < en->cur ; i ++){
        wprint_ennemy(w,&en->ennemies[i],distx,disty);
    }
    return ERR_OK;
}

err_flag wprint_entab_fancy(WINDOW * w , er_entab * en, uint32_t distx, uint32_t disty){
    for(uint32_t i = 0 ; i < en->cur ; i ++){
        wprint_ennemy_fancy(w,&en->ennemies[i],distx,disty);
    }
    return ERR_OK;
}
