#ifndef ER_ERFLAGS_H 
#define ER_ERFLAGS_H

#include "common.h"
/*
___________    .___            __________                                  
\_   _____/  __| _/ ____   ____\______   \__ __  ____   ____   ___________ 
 |    __)_  / __ | / ___\_/ __ \|       _/  |  \/    \ /    \_/ __ \_  __ \
 |        \/ /_/ |/ /_/  >  ___/|    |   \  |  /   |  \   |  \  ___/|  | \/
/_______  /\____ |\___  / \___  >____|_  /____/|___|  /___|  /\___  >__|   
        \/      \/_____/      \/       \/           \/     \/     \/       

misc.h defines the macros , functions and flags used to report error and warnings in the 
*/

typedef uint8_t err_flag; //value to know wether a function encountered a problem , 0 ok, {1..255} -> error code
/*
the enum of flags ; I might add stuff to it at some point.
*/
typedef enum ERR_FLAGS{   
    ERR_OK = 0, ERR_NULL , ERR_ALLOC, ERR_REALLOC, ERR_VALS, ERR_NOTNULL,  
}flags ;

/*functions and macros*/

extern void er_report( FILE * flux, const char * repport_msg, const char * error_msg , err_flag flag);
/*
    flux -> not null 
    repport_msg -> not null 
    error_msg -> not null 
    flag -> 

    reports an error in the flux given. 

    Some wrappers are defined around this function to avoid code redundancy
*/

//some wrappers arround the er_report function ; I recommand sticking to the def_err/war_handlers 
//bc passing a code block to a macro is very much not safe lol
#define error_handler(cond, msg,flag,handler_code) if((cond)){er_report(stderr, "error", (msg), (flag)); {handler_code;} return (flag);}
/*
    the error_handler macro function checks if cond==TRUE, reports the error 
    associated with flag if it's the case and executes hanlder_code before returning the value of flag.
*/

#define warning_handler(cond,msg,flag, handler_code) if((cond)){er_report(stderr, "warning", (msg), (flag)); {handler_code;}}
/*
same as error_hanler but doesn't return anything
*/
//I really like these macros I just think they're neat the C preprocessor is crazy for this ngl 
/*
    the default macro functions are safer / easier to use. I recommand sticking to them. 
    they don't execute any code and just report the error / warning if cond is true.
*/
#define def_err_handler(cond,msg,flag) error_handler(cond,msg,flag,;)
#define def_war_handler(cond,msg,flag) warning_handler(cond,msg,flag,;)

extern size_t default_arr_size; 
extern double default_realloc;

extern err_flag generic_realloc(void ** array, size_t elem_size, uint32_t nb_elem);
/*
    array -> not null 
    *array -> not null 

    generic wrapper around realloc 
*/

//why the fuck is this here 
typedef struct s_dynar_u32{
     uint32_t cur ; 
     uint32_t max ; 
     uint32_t * elems ; 
}er_dynaru32;
typedef er_dynaru32 er_stacku32;

#define declare_dynarru32(dynarr) er_dynaru32 dynarr; dynarr.cur = 0; dynarr.max=0; dynarr.elems=NULL;
#define declare_stacku32 declare_dynarru32

extern err_flag init_dynarr( er_dynaru32 * dynarr, uint32_t size);
#define init_stack init_dynarr
extern err_flag push_dynarr( er_dynaru32 * dynarr, uint32_t elem); 
#define push_stack push_dynarr
extern void free_dynarr( er_dynaru32 * dynarr);
#define free_stack free_dynarr 
extern err_flag pop_dynarr( er_dynaru32 * dynarr, int64_t * elem);
#define pop_stack pop_dynarr

typedef struct s_dynar_nref{
     uint32_t cur ; 
     uint32_t max ; 
     struct s_graph_entry ** elems ; 
}er_dynarr_nodes;
//boilerplate first ; think later 
//(I'll have to put every dynarr type inside a single generic dynarr call or smtg)
#define declare_darn(dynarr) er_dynarr_nodes dynarr; dynarr.cur = 0; dynarr.max=0; dynarr.elems=NULL;

err_flag init_dynarr_nodes( er_dynarr_nodes * darn, uint32_t size);
err_flag push_dynarr_nodes( er_dynarr_nodes * dynarr, struct s_graph_entry * elem);
void free_dynarr_nodes( er_dynarr_nodes * dynarr);
err_flag pop_dynarr_nodes( er_dynarr_nodes * dynarr, struct s_graph_entry ** elem);

#endif 
