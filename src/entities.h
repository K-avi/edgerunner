#ifndef ER_PCURSES_H 
#define ER_PCURSES_H 

#include "common.h"
#include "graph_base.h"
#include <curses.h>
#include "misc.h"
#include "ennemy.h"
#include "points.h"

/*
entities.h defines basic manipulation of "entities" , which include players, exit, ennemies ,... 

more advanced manipulation of ennemies is found in ennemies.h
*/

typedef struct s_entity{
    
    uint8_t ch ;
    uint32_t x; 
    uint32_t y; 
    struct s_graph_entry * cur_node;
    int32_t color; 

}er_entity;

typedef er_entity er_player;
typedef er_entity er_exit;

#define declare_er_player(p,x,y) er_player p={'@',x,y,NULL, COLOR_CURNODE};
#define declare_er_exit(e,x,y) er_exit e={'%',x,y,NULL, COLOR_EXIT};
#define declare_er_ennemy(e,x,y) er_ennemy e={'!',x,y,NULL, COLOR_ENNEMY};


extern err_flag init_ent_pos(er_exit * ex, er_player* pl, er_entab * entab, er_graph * g,  dynarr_points * darp);
/*
    (ex & pl ) -> not null
    (entab & g & darp) -> not null & initialized 

    initialized the position of the exit, player and ennemies given a graph g 
    and the coordinates of the nodes of the graph 
*/

extern err_flag wprint_surroundings(WINDOW *w ,er_entity * en , dynarr_points * darp, uint32_t distx, uint32_t disty, const er_graph * g );
/*
    w -> not null 
    (en  & darp & g )-> not null & initialized

    prints in w the surroundings of an entity given a graph , the coordinates of it's nodes 
    and the distx and disty distances used to print the graph.
*/

extern err_flag wprint_entity(WINDOW * w , er_entity * pl, uint32_t distx, uint32_t disty);
/*
    w -> not null 
    pl -> not null & initialized

    prints an entity in the window w given the distx and disty distances used 
*/
#define wprint_player wprint_entity
#define wprint_exit wprint_entity
#define wprint_ennemy wprint_entity

extern err_flag wprint_surroundings_fancy(WINDOW *w ,er_entity * en , dynarr_points * darp, uint32_t distx, uint32_t disty, const er_graph * g, const er_game_entities * gent );
/*
    w -> not null 
    (en & darp & g ) -> not null & initialized

    a fancier print function to print the nodes of g surrounding en in w
*/
extern err_flag wprint_entity_fancy(WINDOW * w , er_player * pl, uint32_t distx, uint32_t disty);
/*
    w -> not null
    pl -> not null 

    a fancier print function to print an entity in w.
*/
#define wprint_player_fancy wprint_entity_fancy
#define wprint_exit_fancy wprint_entity_fancy
#define wprint_ennemy_fancy wprint_entity_fancy

#endif
