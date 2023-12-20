#ifndef ER_ERFLAGS_H 
#define ER_ERFLAGS_H
/*
___________    .___            __________                                  
\_   _____/  __| _/ ____   ____\______   \__ __  ____   ____   ___________ 
 |    __)_  / __ | / ___\_/ __ \|       _/  |  \/    \ /    \_/ __ \_  __ \
 |        \/ /_/ |/ /_/  >  ___/|    |   \  |  /   |  \   |  \  ___/|  | \/
/_______  /\____ |\___  / \___  >____|_  /____/|___|  /___|  /\___  >__|   
        \/      \/_____/      \/       \/           \/     \/     \/       

errflags.h defines the macros , functions and flags used to report error and warnings in the 
project 
*/
#include "common.h"

typedef uint8_t err_flag; 

/*
the enum of flags ; I might add stuff to it at some point.
*/
typedef enum ERR_FLAGS{
    
    ERR_OK = 0, ERR_NULL , ERR_ALLOC, ERR_REALLOC, ERR_VALS, ERR_NOTNULL,
    
}flags ;

/***********************************************************************/
/*functions and macros*/

/*
flux -> not null 
repport_msg -> not null 
error_msg -> not null 
flag -> 

reports an error in the flux given. 

Some wrappers are defined around this function to avoid code redundancy
*/
extern void er_report( FILE * flux, const char * repport_msg, const char * error_msg , err_flag flag);

//some wrappers arround the er_report function ; I recommand sticking to the def_err/war_handlers 
//bc passing a code block to a macro is very much not safe lol

/*
the error_handler macro function checks if cond==TRUE, reports the error 
associated with flag if it's the case and executes hanlder_code before returning the value of flag.
*/
#define error_handler(cond, msg,flag,handler_code) if((cond)){er_report(stderr, "error", (msg), (flag)); {handler_code;} return (flag);}
/*
same as error_hanler but doesn't return anything
*/
#define warning_handler(cond,msg,flag, handler_code) if((cond)){er_report(stderr, "warning", (msg), (flag)); {handler_code;}}

//I really like these macros I just think they're neat
//the C preprocessor is crazy for this ngl 
/*
the default macro functions are safer / easier to use. I recommand sticking to them. 

they don't execute any code and just report the error / warning if cond is true.
*/
#define def_err_handler(cond,msg,flag) error_handler(cond,msg,flag,;)
#define def_war_handler(cond,msg,flag) warning_handler(cond,msg,flag,;)

#endif 
