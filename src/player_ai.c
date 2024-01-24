#include "player_ai.h"
#include "search_utils.h"
#include "game.h"

err_flag ai_shortest_path(WINDOW * w, er_game_entities * gent, er_graph * g, dynarr_points * darp, int64_t * dest){
  
    def_err_handler(!w, "ai_shortest_path w", ERR_NULL);
    def_err_handler(!gent, "ai_shortest_path gent", ERR_NULL);
    def_err_handler(!g, "ai_shortest_path g", ERR_NULL);
    def_err_handler(!darp, "ai_shortest_path darp", ERR_NULL);
    def_err_handler(!dest, "ai_shortest_path dest", ERR_NULL);

    int64_t dist = -1 ; 
    int64_t min = INT64_MAX ;
    
    int64_t chosen_node_index = -1 ;

    for(uint32_t i = 0 ; i < gent->p->cur_node->cur; i++){ 
        err_flag failure = bfs_graph(g, gent->p->cur_node->neighboors_ref[i], gent->ex->cur_node, &dist );
        def_err_handler(failure, "ai_shortest_path", failure);
        def_err_handler( (dist==-1), "ai_shortest_path", ERR_VALS);
        
        
        //fprintf(stderr, "i=%u *dist=%li chni=%li\n", i,dist, chosen_node_index);

        if(dist < min && dist != -1){
            chosen_node_index = i;
            min = dist ; 
        }
    }
    
    *dest = chosen_node_index ;
    //fprintf(stderr, "p.cn=%d *dest=%li\n", gent->p->cur_node - g->adjacency_lists, *dest);
    return ERR_OK;
}//tested;  works

/*************************************************************************/

err_flag init_psinfo(er_psinfo * psinfo, er_player * p, er_graph * g ){
    /*
        psinfo, p, g -> not null (g could just be g->adjacency_lists tbh)

        allocates and initializes the elements of psinfo with p-> cur nodes 
        and it's neighbors
    */
    def_err_handler(!psinfo, "init_psinfo", ERR_NULL);
    def_err_handler(!p, "init_psinfo", ERR_NULL);

    err_flag failure = init_dynarr_nodes(psinfo->darn_seen, default_arr_size);
    def_err_handler(failure, "init_psinfo", failure);

    failure = init_dynarr_nodes(psinfo->darn_visited, default_arr_size);
    def_err_handler(failure, "init_psinfo", failure);

    failure = init_heap(psinfo->nodes_heap, default_arr_size);
    def_err_handler(failure, "init_psinfo", failure);

    
    push_dynarr_nodes( psinfo->darn_visited, p->cur_node );

    for(uint32_t i = 0 ; i < p->cur_node->cur; i++){
        push_dynarr_nodes(psinfo->darn_seen, p->cur_node->neighboors_ref[i]);
    }
    return ERR_OK;
}//not tested 

void free_psinfo(er_psinfo * psinfo ){

    if(psinfo){
        if(psinfo->darn_seen){
            free_dynarr_nodes(psinfo->darn_seen);
        }
        if(psinfo->darn_visited){
            free_dynarr_nodes(psinfo->darn_visited); 
        }
        if(psinfo->nodes_heap){
            free_heap(psinfo->nodes_heap);
        }
        psinfo->darn_seen = NULL; 
        psinfo->darn_visited = NULL;
        psinfo->nodes_heap = NULL;
    }
    return ;
}

static err_flag get_dist( er_points * p1, er_points * p2 , double * ret){
    /*
    I don't really need a "real" distance the idea is that the distance 
    can be "approximated" by the "highest difference possible"
    */
    uint32_t dx = p2->x > p1->x ? p2->x - p1->x : p1->x - p2->x;
    uint32_t dy = p2->y > p1->y ? p2->y - p1->y : p1->y - p2->y;

    *ret = max(dx, dy);

    return ERR_OK;
}//I'm really not sure about what I'm doing the idea is to consider the distance 
//as a discrete thing bc diagonals aren't really "further" and stuff


err_flag init_psheap(er_psheap * heap, uint32_t size, uint32_t nb_vals ){

    heap->elems_key = calloc(size, sizeof(uint32_t));
    def_err_handler(!heap->elems_key, "init_psheap elems_key", ERR_NULL);

    heap->elems_val = calloc(size, sizeof(uint32_t));
    def_err_handler(!heap->elems_val, "init_psheap elems_val", ERR_ALLOC);

    heap->val_in_heap = calloc(size, sizeof(bool));;
    def_err_handler(!heap->val_in_heap, "init_psheap nodes_in_heap", ERR_ALLOC);


    heap->nb_vals = nb_vals; 
    heap->cur = 0 ; 
    heap->max = size ;

    return ERR_OK;
}

void free_psheap(er_psheap * heap){
    if(heap){
        if(heap->elems_key) free(heap->elems_key);
        if(heap->elems_val) free(heap->elems_val);
        if(heap->val_in_heap) free(heap->val_in_heap); 
        heap->cur = heap->max = 0 ;
        heap->elems_key = heap->elems_val = NULL ; 
        heap->val_in_heap = NULL ;
    }
}

static inline void swap_psheap( er_psheap * heap , uint32_t pi, uint32_t  si){

  uint32_t tmp = heap->elems_key[pi];
  heap->elems_key[pi] = heap->elems_key[si] ; 
  heap->elems_key[si] = tmp ;

  tmp = heap->elems_val[pi];
  heap->elems_val[pi] = heap->elems_val[si] ; 
  heap->elems_val[si] = tmp ;
}

static inline uint32_t lchild(uint32_t p){
    return p*2 + 1 ; 
}

static inline uint32_t rchild(uint32_t p){
    return p*2 + 2 ; 
}

static err_flag realloc_psheap(er_psheap * heap, double coeff){

    err_flag failure = generic_realloc((void**)&heap->elems_key, sizeof(uint32_t ), (uint32_t)( (double)(heap->max+1) * coeff));
    def_err_handler(failure, "realloc_psheap key", failure);
    
    failure = generic_realloc((void**)&heap->elems_val, sizeof(uint32_t ), (uint32_t)( (double)(heap->max+1) * coeff));
    def_err_handler(failure, "realloc_psheap val", failure);

    heap->max = (uint32_t)( (double)(heap->max+1) * coeff) ;
    memset(&heap->elems_key[heap->cur+1], 0, (heap->max - heap->cur - 1  )* sizeof(uint32_t));
    memset(&heap->elems_val[heap->cur+1], 0, (heap->max - heap->cur - 1  )* sizeof(uint32_t));
    //set to 0 cuz I don't like habing unitialized stuff lying around

    return ERR_OK;
}

err_flag add_psheap( er_psheap * heap , uint32_t elem_key, uint32_t elem_val){
  /*
  should check for null && elem val < nb_val
  */
  if(heap->cur == 0 ){
    heap->elems_key[0] = elem_key ;
    heap->elems_val[0] = elem_val ; 
    heap->cur++;
     
  }else{

    if(heap->val_in_heap[elem_val]){
      return ERR_OK;
    }
    if(heap->cur == heap->max){
      err_flag failure = realloc_psheap(heap, default_realloc); 
      def_err_handler(failure, "add_heap", failure);
    }

    uint32_t s_index = heap->cur;
    heap->elems_key[heap->cur+1] = elem_key ;
    heap->elems_val[heap->cur+1] = elem_val ;
    heap->val_in_heap[elem_val] = true ;

    heap->cur++; 
    uint32_t p_index = (s_index -1 )/2;
    
    while(heap->elems_key[s_index] < heap->elems_key[p_index] ){

        swap_psheap(heap, p_index, s_index);

        s_index = p_index ;
        p_index = (p_index - 1) /2 ; 
        if(p_index > heap->cur) break;
    }

  }
  return ERR_OK;
}

err_flag pop_psheap(er_psheap * heap , int64_t * elem_key, int64_t * elem_val){

    if(!heap->cur){
      *elem_key = -1 ;
      *elem_val = -1 ; 
      
    }else if (heap->cur ==1 ){
      *elem_key = heap->elems_key[--heap->cur];
      *elem_val = heap->elems_val[heap->cur];

      heap->val_in_heap[heap->elems_val[heap->cur]] = false ; 
    
    }else{

      *elem_key = heap->elems_key[0]; 
      *elem_val = heap->elems_val[0];
      heap->val_in_heap[heap->elems_val[0]] = false ; 

      swap_psheap(heap, 0 , --heap->cur); 

      uint32_t ci = 0, li = lchild(0), ri = rchild(0); 

      uint32_t minkey = heap->elems_key[ci] < heap->elems_key[li] ? ci : li ; 
      minkey = heap->elems_key[minkey] < heap->elems_key[ri] ? minkey : ri ;

      while( minkey != ci ){
        
        swap_psheap(heap, minkey, ci); 

        ci = minkey ;
        li = lchild(ci); 
        ri = rchild(ci);

        if(li < heap->cur && ri < heap->cur){
          minkey = heap->elems_key[ci] < heap->elems_key[li] ? ci : li ; 
          minkey = heap->elems_key[minkey] < heap->elems_key[ri] ? minkey : ri ;
        }else if(li < heap->cur){
          minkey = heap->elems_key[ci] < heap->elems_key[li] ? ci : li ; 
        }
      }
    }
    return ERR_OK ;
}//really unsure about that tbh 

//need to be able to delete or append from seen / visited in psheap 
//seen and visited need to be size of nb nodes and not dynamic arrays

err_flag ai_realist(WINDOW * w, er_game_entities * gent, er_psinfo * psinfo, er_graph * g, dynarr_points * darp, int64_t * dest){
    
    er_points pex = { gent->ex->x , gent->ex->y};
    for(uint32_t i = 0 ; i < psinfo->darn_seen->cur; i++){
      
      double dist = -1 ;
      get_dist(&darp->elems[psinfo->darn_seen->elems[i] - g->adjacency_lists], &pex, &dist  );
      add_psheap(psinfo->nodes_heap, (uint32_t) dist, psinfo->darn_seen->elems[i] - g->adjacency_lists);

    }
    int64_t node_key, node_val ; 
    pop_psheap(psinfo->nodes_heap,&node_key, &node_val);

    uint32_t chosen_node = 0 ;
    double dist_min = INT64_MAX ;
    for(uint32_t i = 0 ; i < gent->p->cur_node->cur ; i++){
        double cur_dist ; 
        uint32_t cur_node_index = gent->p->cur_node->neighboors_ref[i] - g->adjacency_lists;
        get_dist(&darp->elems[cur_node_index],&pex,&cur_dist);
        if(cur_dist < dist_min){
          chosen_node = i ; 
          dist_min = cur_dist ;
        }

        if(!g->visited[cur_node_index]){
          g->visited[cur_node_index] = true;
          push_dynarr_nodes(psinfo->darn_visited, &g->adjacency_lists[cur_node_index]);
          del_dynarr_nodes(psinfo->darn_seen, &g->adjacency_lists[cur_node_index]);
        }
    }

    *dest = chosen_node;
    return ERR_OK;    
}//done ; 100% wrong ; not tested
