#include "misc.h"
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

void er_report( FILE * flux, const char * repport_msg, const char * error_msg ,err_flag flag)
{
    /*
    flux -> not null 
    repport_msg -> not null 
    error_msg -> not null 
    flag -> 
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

void er_error( const char * msg , err_flag flag)
{ 
  //could be a macro 
  er_report(stdout, "error", msg, flag);
}

void er_warning(const char * msg , err_flag flag)
{ 
  //could be a macro 
  er_report(stdout, "warning", msg, flag);
}


size_t default_arr_size = 8 ; 
double default_realloc = 1.5 ;


err_flag generic_realloc(void ** array, size_t elem_size, uint32_t nb_elem)
{
    def_err_handler(!array,"generic_realloc",ERR_NULL);
    def_err_handler(!*array,"generic_realloc",ERR_NULL);

    *array = realloc(*array, nb_elem * elem_size);
    def_err_handler(!*array, "generic_realloc", ERR_REALLOC);

    return ERR_OK;
}

err_flag init_dynarr( er_dynaru32 * dynarr, uint32_t size){
  def_err_handler(!dynarr, "init_dynarr", ERR_NULL);

  dynarr->elems = calloc(size , sizeof(uint32_t));
  def_err_handler(!dynarr->elems, "init_dynarr", ERR_ALLOC);
  
  dynarr->cur = 0 ; 
  dynarr->max = size ; 

  return ERR_OK;
}

static err_flag realloc_dynarr(er_dynaru32 * dynarr , double coeff){
  
  err_flag failure = generic_realloc((void**)&dynarr->elems, sizeof(uint32_t ), (uint32_t)( (double)(dynarr->max+1) * coeff));
  def_err_handler(failure, "realloc_dynarr", failure);

   dynarr->max = (uint32_t)( (double)(dynarr->max+1) * coeff) ;
   memset(&dynarr->elems[dynarr->cur+1], 0, (dynarr->max - dynarr->cur - 1  )* sizeof(uint32_t));
  //set to 0 cuz I don't like habing unitialized stuff lying around

  return ERR_OK;
}
err_flag push_dynarr( er_dynaru32 * dynarr, uint32_t elem){
    def_err_handler(!dynarr, "push_dynarr", ERR_NULL);

    if(dynarr->cur == dynarr->max){
        err_flag failure = realloc_dynarr(dynarr, default_realloc);
        def_err_handler(failure, "push_dynarr", failure);
    }
    dynarr->elems[dynarr->cur++] = elem ;

  return ERR_OK;
}

void free_dynarr( er_dynaru32 * dynarr){
    if(dynarr){
      if (dynarr->elems)
      {
        free(dynarr->elems); 
      }
      dynarr->elems = NULL; 
      dynarr->cur = dynarr->max = 0 ; 
    }
}

err_flag pop_dynarr( er_dynaru32 * dynarr, int64_t * elem){
      def_err_handler(!dynarr, "pop_dynarr dynarr", ERR_NULL);
      def_err_handler(!elem, "pop_dynarr elem", ERR_NULL);

      if(dynarr->cur == 0 ){
        *elem = -1; 
      }else{

        dynarr->cur--;
        *elem = dynarr->elems[dynarr->cur];
      }
      return ERR_OK;
}
