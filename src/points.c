#include "points.h"

#define generic_calloc_dynarr(arr,max_size,elem_size) {(arr)->cur = 0 ; (arr)->max = (max_size) ; (arr)->elems = calloc((max_size),(elem_size));}

err_flag init_dynp(dynarr_points * darp , size_t max_size ){
    /*
    darp -> !null & initialized | not initialized (throws warning)
    */
   def_err_handler(!darp, "init_dynp", ERR_NULL);
   def_war_handler(darp->elems, "init_dynp", ERR_NOTNULL);
   generic_calloc_dynarr(darp, max_size, sizeof(struct spoints));
   def_err_handler(!darp->elems, "init_dynp", ERR_ALLOC );
   
   darp->max = max_size ;
   darp->cur = 0 ; 

   return ERR_OK;
}//tested; ok

static err_flag realloc_dynp(dynarr_points * darp){
    /*
    darp -> (!NULL) & (initialized | not initialized (doesn't throw warnings)
    */
    def_err_handler(!darp,"realloc_dynp", ERR_NULL);
    
    if(!darp->elems){
        err_flag failure = init_dynp(darp, default_arr_size);
        def_err_handler(failure,"init_dynp",failure);

        return ERR_OK;
    }
    darp->elems = realloc(darp->elems, ((darp->max  * default_realloc ) +1 )* sizeof(er_points));
    darp->max = (darp->max * default_realloc) + 1 ;
    def_err_handler(!darp->elems,"realloc_dynp", ERR_REALLOC);

    return ERR_OK;
}//tested ;ok

err_flag append_dynp(dynarr_points * darp, const er_points * point){
   
    def_err_handler(!darp, "append_dynp darp", ERR_NULL);
    def_err_handler(!point, "append_dynp point", ERR_NULL);
    warning_handler(!darp->elems,"append_dynp", ERR_NOTNULL, {err_flag failure = init_dynp(darp, default_arr_size ); def_err_handler(failure, "append_dynp", failure); });

    if(darp->cur == darp->max){
        err_flag failure = realloc_dynp(darp);
        def_err_handler(failure, "append_dynp", failure);
    }

    darp->elems[darp->cur].x = point->x ; 
    darp->elems[darp->cur].y =  point->y ;
    darp->cur ++; 

    return ERR_OK ;
}//tested ok

err_flag free_dynp(dynarr_points * darp){
    /*
    darp -> null | !null & initialized | not initialized
    */
    if(darp){
        if(darp->elems) free(darp->elems);
        
        darp->elems = NULL; 
        darp->cur = 0 ; 
        darp->max = 0 ; 
    }
    return ERR_OK;
}//not tested;

err_flag fprint_dynp(FILE * flux, const dynarr_points * darp){
    /*
    */
    if(!flux){ 
        flux = stdout ; 
    }
    if(!darp){
        fprintf(flux, "null pointer passed\n");
        return ERR_OK;
    }
    if(!darp->elems){
        fprintf(flux, "null pointer in darp->elems\n");
    }
    for(uint32_t i = 0 ; i < darp->cur ; i++){
        fprintf(flux,"darp[%u]=%u,%u\n",i, darp->elems[i].x, darp->elems[i].y);
    }
    return ERR_OK;
}//not tested

err_flag gen_coordinates(uint32_t n, uint32_t m , dynarr_points * darp){
 
    def_err_handler(!darp,"gen_coord", ERR_NULL);
    warning_handler(!darp->elems, "gen_coord", ERR_VALS, 
    {err_flag failure = init_dynp(darp,default_arr_size);def_err_handler(failure,"gen_coord", failure);});

    for(uint32_t i = 0 ; i < n ; i ++){
        for (uint32_t j = 0; j < m; j++){
            er_points p = {i,j};
            err_flag failure = append_dynp(darp,&p);
            def_err_handler(failure, "gen_coordinates", failure);
        }
    }
    return ERR_OK;
}//ok


