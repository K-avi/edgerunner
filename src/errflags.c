#include "errflags.h"
/*
this file is the error report system 
very simple but allows to handle failed calls and so on; 
every function in the project returns 
a flag . 
*/

static inline char * str_flag(err_flag flag)
{

  switch (flag) 
  { 
    
    case ERR_NULL: return "null pointer passed";
    case ERR_ALLOC: return "couldn't allocate memory";
    case ERR_REALLOC: return "couldn't reallocate memory";
    case ERR_VALS: return "value given not matching expected values";
    case ERR_NOTNULL: return "pointer is not null"; 
    
    default : return "unknown error";
  }
}//ok

void er_report( FILE * flux, const char * repport_msg, const char * error_msg ,err_flag flag){
    /*
    the real repport function of the project 
    every variant of it will rely on it 
    */
    if(! flux){
        fprintf(stderr,"ussr_report : NULL was passed for flux\n");
        return;
    }else if(!repport_msg){
        fprintf(stderr,"ussr_report : NULL was passed for repport_msg\n");
        return;
    }else if(!error_msg){
        fprintf(stderr,"ussr_report : NULL was passed for error_msg\n");
        return; 
    }
    fprintf(flux," %s : %s at %s\n",repport_msg, str_flag(flag), error_msg);
}

void er_error( const char * msg , err_flag flag){ 
  //could be a macro 
  er_report(stdout, "error", msg, flag);
}

void er_warning(const char * msg , err_flag flag){ 
  //could be a macro 
  er_report(stdout, "warning", msg, flag);
}


