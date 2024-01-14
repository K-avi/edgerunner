#ifndef ER_COMMON_H 
#define ER_COMMON_H 
/*
___________    .___            __________                                  
\_   _____/  __| _/ ____   ____\______   \__ __  ____   ____   ___________ 
 |    __)_  / __ | / ___\_/ __ \|       _/  |  \/    \ /    \_/ __ \_  __ \
 |        \/ /_/ |/ /_/  >  ___/|    |   \  |  /   |  \   |  \  ___/|  | \/
/_______  /\____ |\___  / \___  >____|_  /____/|___|  /___|  /\___  >__|   
        \/      \/_____/      \/       \/           \/     \/     \/       

common.h includes libraries used in every file (more or less) 
and has some basic defines
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include "misc.h"
#include <stdbool.h>

#define max(val1,val2) val1 > val2 ? val1 : val2 
#define min(val1,val2) val2 > val1 ? val1 : val2 

#define TRUE 1 
#define FALSE 0 

extern bool colors_on ;
#define COLOR_EXIT 1 
#define COLOR_ENNEMY 2 
#define COLOR_CURNODE 3
#define COLOR_LINK 4
#define COLOR_NODE 5
#define COLOR_NODE_NEW 6

//#define debug

#endif 
