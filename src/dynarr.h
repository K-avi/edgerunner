#ifndef ER_DYNARR_H 
#define ER_DYNARR_H 

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

extern err_flag init_dynp(dynarr_points * darp, size_t max_size );
extern err_flag append_dynp(dynarr_points * darp, const er_points * point);
extern err_flag free_dynp(dynarr_points * darp );
extern err_flag fprint_dynp(FILE * flux, const dynarr_points * darp);

#define print_dynps(darp) {fprint_dynp(stdout,(darp));}


#endif 
