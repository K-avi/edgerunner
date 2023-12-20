#ifndef ER_DYNARR_H 
#define ER_DYNARR_H 
/*
___________    .___            __________                                  
\_   _____/  __| _/ ____   ____\______   \__ __  ____   ____   ___________ 
 |    __)_  / __ | / ___\_/ __ \|       _/  |  \/    \ /    \_/ __ \_  __ \
 |        \/ /_/ |/ /_/  >  ___/|    |   \  |  /   |  \   |  \  ___/|  | \/
/_______  /\____ |\___  / \___  >____|_  /____/|___|  /___|  /\___  >__|   
        \/      \/_____/      \/       \/           \/     \/     \/       

dynarr.h defines basic API to manipulate every dynamic arrays in EdgeRunner
*/

#include "common.h"
#include "errflags.h"

extern size_t default_arr_size; 
extern double default_realloc;

typedef struct spoints{ 

    uint32_t x; 
    uint32_t y;
    
}er_points;

typedef struct s_dynarr_uint16_t{

    uint32_t max; 
    uint32_t cur; 

    struct spoints * elems; 
}dynarr_points;

#define declare_dynnar(type, name) type name; (name).max=0; (name).cur=0; (name).elems=0;

/*
darp -> !null & initialized | not initialized (throws warning)
initialized the array at darp with max_size elems
*/
extern err_flag init_dynp(dynarr_points * darp, size_t max_size );

 /*
darp -> not null & initialized | not initialized 
point -> not null 

append the values of *points to *darp
*/
extern err_flag append_dynp(dynarr_points * darp, const er_points * point);

/*
darp -> null | !null & initialized | not initialized

frees elements when not null and set everything to null / zero 
*/
extern err_flag free_dynp(dynarr_points * darp );

/*
darp -> null | !null & initialized | not initialized
flux ->     -//-

prints to flux if not null, stdout otherwise, warns when null pointer
*/
extern err_flag fprint_dynp(FILE * flux, const dynarr_points * darp);

#define print_dynps(darp) {fprint_dynp(stdout,(darp));}

extern err_flag generic_realloc(void ** array, size_t elem_size, uint32_t nb_elem);

#endif 
