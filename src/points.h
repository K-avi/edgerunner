#ifndef ER_POINTS_H 
#define ER_POINTS_H
/*
___________    .___            __________                                  
\_   _____/  __| _/ ____   ____\______   \__ __  ____   ____   ___________ 
 |    __)_  / __ | / ___\_/ __ \|       _/  |  \/    \ /    \_/ __ \_  __ \
 |        \/ /_/ |/ /_/  >  ___/|    |   \  |  /   |  \   |  \  ___/|  | \/
/_______  /\____ |\___  / \___  >____|_  /____/|___|  /___|  /\___  >__|   
        \/      \/_____/      \/       \/           \/     \/     \/       

points.h defines the functions used to manipulate the basic points structure. It might 
be useless at the moment but I'll probably need it later.
*/

#include "common.h"
#include "dynarr.h"
#include "errflags.h"

extern err_flag gen_points( uint32_t nbpoints, uint32_t max,  dynarr_points * darp);

#endif 
