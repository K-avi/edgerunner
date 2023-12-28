#ifndef ER_PCURSES_H 
#define ER_PCURSES_H 

#include "common.h"
#include "graph_base.h"
#include <ncurses.h>
#include "dynarr.h"

typedef struct s_player{
    
    uint8_t ch ;
    uint32_t x; 
    uint32_t y; 
    struct s_graph_entry * cur_node;

}er_entity;

typedef er_entity er_player;
typedef er_entity er_exit;
typedef er_entity er_ennemy; 

#define declare_er_player(p,x,y) er_player p={'@',x,y,NULL};
#define declare_er_exit(e,x,y) er_exit e={'%',x,y,NULL};
#define declare_er_ennemy(e,x,y) er_ennemy e={'!',x,y,NULL};

extern err_flag init_ent_pos(er_exit * ex, er_player* pl, er_ennemy * en, er_graph * g,  dynarr_points * darp);


extern err_flag wprint_surroundings(WINDOW *w ,er_entity * en , dynarr_points * darp, uint32_t distx, uint32_t disty, const er_graph * g );

extern err_flag wprint_entity(WINDOW * w , er_player * pl, uint32_t distx, uint32_t disty);
#define wprint_player wprint_entity
#define wprint_exit wprint_entity
#define wprint_ennemy wprint_entity

err_flag wprint_surroundings_fancy(WINDOW *w ,er_entity * en , dynarr_points * darp, uint32_t distx, uint32_t disty, const er_graph * g );

err_flag wprint_entity_fancy(WINDOW * w , er_player * pl, uint32_t distx, uint32_t disty);
#define wprint_player_fancy wprint_entity_fancy
#define wprint_exit_fancy wprint_entity_fancy
#define wprint_ennemy_fancy wprint_entity_fancy

#endif
