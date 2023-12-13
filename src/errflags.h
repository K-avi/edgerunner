#ifndef ER_ERFLAGS_H 
#define ER_ERFLAGS_H

#include "common.h"

typedef uint8_t err_flag; 


typedef enum ERR_FLAGS{
    
    ERR_OK = 0, ERR_NULL , ERR_ALLOC, ERR_REALLOC, ERR_VALS, ERR_NOTNULL,
    
}flags ;

extern void er_report( FILE * flux, const char * repport_msg, const char * error_msg , err_flag flag);

//some wrappers arround the er_report function ; I recommand sticking to the def_err/war_handlers 
//bc passing a code block to a macro is very much not safe lol

#define error_handler(cond, msg,flag,handler_code) if((cond)){er_report(stderr, "error", (msg), (flag)); {handler_code;} return (flag);}
#define warning_handler(cond,msg,flag, handler_code) if((cond)){er_report(stderr, "warning", (msg), (flag)); {handler_code;}}

//I really like these macros I just think they're neat
//the C preprocessor is crazy for this ngl 
#define def_err_handler(cond,msg,flag) error_handler(cond,msg,flag,;)
#define def_war_handler(cond,msg,flag) warning_handler(cond,msg,flag,;)

#endif 
