#include "points.h"

err_flag gen_points( uint32_t nbpoints, uint32_t max,  dynarr_points * darp){
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


