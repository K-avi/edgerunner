#ifndef ER_ENNEMY_H 
#define ER_ENNEMY_H 

#include "common.h"
#include "graph_base.h"
#include "misc.h"
#include "points.h"
#include <curses.h>
//don't include entities.h cuz it includes this
/*
ennemy.h defines the functions / structures used to manipulate the ennemies during a game
*/

typedef struct s_entity er_ennemy; 
typedef struct s_entity er_exit; 
typedef struct s_entity er_player; 


//function ptr macro
typedef  err_flag (*rule_fun)(WINDOW * , er_ennemy *  , er_graph * , dynarr_points *, er_player *);
//rules array 
typedef struct s_ennemy_rules{
    uint32_t size; 
    rule_fun * rule_functions;
    uint16_t * coeffs;
    
}er_enrules;
#define declare_enrules(__enr) er_enrules __enr={0,NULL,NULL};

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

//don't think I need to export any of these ; the only thing i need 
//is the er_entab 
extern err_flag  move_random( WINDOW * w, er_ennemy * en , er_graph * g, dynarr_points * darp, er_exit * ex);
extern err_flag  move_closest(WINDOW* w,er_ennemy * e , er_graph * g, dynarr_points * darp, er_player * p );
extern err_flag move_rest(WINDOW * w , er_ennemy * en , er_graph * g, dynarr_points * darp, er_player * p);
/*
    rule function to define the behavior of the ennemy. 
    I probably don't need to export them but I do anyways :/
*/



extern err_flag init_entab(er_entab * entab, uint32_t size);
/*
    entab -> not null 
    size -> not zero

    initialized the entab passed by reference to be able to contain size ennemies / rules
*/
extern err_flag append_entab(er_entab * entab , const er_ennemy * ennemy , er_enrules * rules );
/*
    entab -> not null & initialized 
    ennemy -> not null 
    rules -> not null & initialized 

    adds the ennemy and the rules associated with it  at the last index of the
    entab passed as argument. 
*/
extern err_flag update_entab(er_entab * entab, const uint32_t * indexes_incr, const double * coeffs_incr );
/*
    entab -> not null & initialized 
    (indexes_incr & coeffs_incr) -> not null & initialized & (length >= entab->cur )

    updates the entry of the ennemy tab by increasing one index at each entry by each coefficient. 
    i.e : entab->rules[i].coeffs[indexes_incr[i]] = coeffs_incr[i] *  entab->rules[i].coeffs[indexes_incr[i]];
*/
extern err_flag free_entab(er_entab * en );
/*
    en -> not null 

    frees the entab given as entry. 
    also frees it's ennemies and rules arrays.
*/


extern err_flag wprint_entab_fancy(WINDOW * w , er_entab * en, uint32_t distx, uint32_t disty);
/*
    w -> not null 
    en -> not null 

    fancy print of the ennemies in entab in the window w
*/
extern err_flag wprint_entab(WINDOW * w , er_entab * en, uint32_t distx, uint32_t disty);
/*
    w -> not null 
    en -> not null 
    
    prints the ennemies in entab in the window w with the distance coeffs distx and disty 
*/

#endif 

