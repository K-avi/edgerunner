#include "points.h"

err_flag gen_rand_points( uint32_t nbpoints, uint32_t max,  dynarr_points * darp){
    /*
    arr -> not null & initialized | not initialized (throws warning)
    */
    
    def_err_handler(!darp,"gen_points", ERR_NULL);
    warning_handler(!darp->elems, "gen_points", ERR_VALS, 
    {err_flag failure = init_dynp(darp,default_arr_size);def_err_handler(failure,"gen_points", failure);});

    for(uint32_t i = 0 ; i < nbpoints ; i ++){
        er_points tmp = { rand()%max, rand()%max};

        err_flag failure = append_dynp(darp, &tmp);
        def_err_handler(failure, "gen_points", failure);        
    }
    return F_OK;
}//tested ; ok 

err_flag gen_coordinates(uint32_t n, uint32_t m , dynarr_points * darp){
    /*
    darp -> not null & initialized | not initialized

    gen coordinates from (0,0) to (n,m) in lexicographical order
    */
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
}


