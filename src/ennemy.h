#ifndef ER_ENNEMY_H 
#define ER_ENNEMY_H 

#include "common.h"
#include "graph_base.h"
#include "dynarr.h"
#include <curses.h>

//don't include player curses cuz it includes this
typedef struct s_player er_ennemy; 
typedef struct s_player er_exit; 
typedef struct s_player er_player; 

//function ptr macro
typedef  err_flag (*rule_fun)(WINDOW * , er_ennemy *  , er_graph * , dynarr_points *, er_player *);
//rules array 
typedef struct s_ennemy_rules{
    uint32_t size; 
    rule_fun * rule_functions;
    uint16_t * coeffs;
    
}er_enrules;
#define declare_enrules(__enr) er_enrules __enr={0,NULL,NULL};

//don't think I need to export any of these ; the only thing i need 
//is the er_entab 
extern err_flag  move_random( WINDOW * w, er_ennemy * en , er_graph * g, dynarr_points * darp, er_exit * ex);
extern err_flag  move_closest(WINDOW* w,er_ennemy * e , er_graph * g, dynarr_points * darp, er_player * p );
extern err_flag move_rest(WINDOW * w , er_ennemy * en , er_graph * g, dynarr_points * darp, er_player * p);


extern er_enrules default_enrules;

//each ennemy has it's own rule that gets uptated as time goes on
//means that a lot of the exported function will be 
//useless as this will be a standalone thing 
typedef struct s_entab{
    uint32_t max ;
    uint32_t cur ;

    er_ennemy * ennemies ; 
    er_enrules * rules ;
    
}er_entab;
#define declare_entab(__etab) er_entab __etab = {0,0,NULL,NULL};

extern err_flag init_entab(er_entab * entab, uint32_t size);
extern err_flag append_entab(er_entab * entab , er_ennemy * ennemy , er_enrules * rules );
extern err_flag update_entab(er_entab * entab, uint32_t * indexes_incr, double * coeffs_incr );
extern err_flag free_entab(er_entab * en );


err_flag init_enrules(er_enrules * dst, er_enrules * src);

err_flag free_enrules(er_enrules * en );
err_flag update_enrules(er_enrules * en , uint32_t index_incr, double coeff_incr);

err_flag wprint_entab_fancy(WINDOW * w , er_entab * en, uint32_t distx, uint32_t disty);
err_flag wprint_entab(WINDOW * w , er_entab * en, uint32_t distx, uint32_t disty);

#endif 

