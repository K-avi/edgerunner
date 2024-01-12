#ifndef ER_POINTS_H 
#define ER_POINTS_H
/*
___________    .___            __________                                  
\_   _____/  __| _/ ____   ____\______   \__ __  ____   ____   ___________ 
 |    __)_  / __ | / ___\_/ __ \|       _/  |  \/    \ /    \_/ __ \_  __ \
 |        \/ /_/ |/ /_/  >  ___/|    |   \  |  /   |  \   |  \  ___/|  | \/
/_______  /\____ |\___  / \___  >____|_  /____/|___|  /___|  /\___  >__|   
        \/      \/_____/      \/       \/           \/     \/     \/       

points.h defines the functions used to manipulate the basic points structure. It might 
be useless at the moment but I'll probably need it later.
*/

#include "common.h"
#include "misc.h"

typedef struct spoints{ 

    uint32_t x; 
    uint32_t y;
    
}er_points;
typedef er_points er_link; 

typedef struct s_dynarr_points{

    uint32_t max; 
    uint32_t cur; 

    struct spoints * elems; 
}dynarr_points;
typedef dynarr_points dynarr_links;

#define declare_dynarr(type, name) type name; (name).max=0; (name).cur=0; (name).elems=0;

/*
darp -> !null & initialized | not initialized (throws warning)
initialized the array at darp with max_size elems
*/
extern err_flag init_dynp(dynarr_points * darp, size_t max_size );
#define init_dynl init_dynp
 /*
darp -> not null & initialized | not initialized 
point -> not null 

append the values of *points to *darp
*/
extern err_flag append_dynp(dynarr_points * darp, const er_points * point);
#define append_dynl append_dynp
/*
darp -> null | !null & initialized | not initialized

frees elements when not null and set everything to null / zero 
*/
extern err_flag free_dynp(dynarr_points * darp );
#define free_dynl free_dynp
/*
darp -> null | !null & initialized | not initialized
flux ->     -//-

prints to flux if not null, stdout otherwise, warns when null pointer
*/
extern err_flag fprint_dynp(FILE * flux, const dynarr_points * darp);
#define print_dynps(darp) {fprint_dynp(stdout,(darp));}

extern err_flag gen_rand_points( uint32_t nbpoints, uint32_t max,  dynarr_points * darp);
extern err_flag gen_coordinates(uint32_t n, uint32_t m , dynarr_points * darp);
#endif 
