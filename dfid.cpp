#include <stdlib.h>
#include <vector>
#include "priority_queue.hpp"
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include <sys/time.h>
#include <algorithm>

#define  MAX_LINE_LENGTH 999 

struct Pair {
    state_t *first;
    int second;
};

Pair cost_bounded_dfs_visit(state_t *n, int bound, state_map_t *map){
    int *g = state_map_get(map,n);
    /* Base cases */

    if (is_goal(n)==1){
        Pair *p = new Pair;
        p->first = n;
        p->second = *g;
        return *p;
    }

    if (*g > bound){
        Pair *p = new Pair;
        p->first = NULL;
        p->second = *g;
        return *p;
    }


    /* recursion */
    int ruleid, t = 10000000; //Should be infinite
    state_t child;
    ruleid_iterator_t iter;
 
    init_fwd_iter(&iter,n);
    while( ( ruleid = next_ruleid( &iter ) ) >= 0 ) {
        apply_fwd_rule(ruleid,n,&child);
        const int d = *g + get_fwd_rule_cost(ruleid);
        state_map_add(map,&child,d);
        Pair pair = cost_bounded_dfs_visit(&child,bound,map);
        if (pair.first != NULL)
            return pair;
        t = std::min(t,pair.second);
    }
    Pair *fail = new Pair;
    fail->first = NULL;
    fail->second = t;
    return *fail;
}

int main( int argc, char **argv )
{
    // VARIABLES FOR INPUT
    char str[ MAX_LINE_LENGTH +1 ] ;
    ssize_t nchars; 
    state_t state,state1; // state_t is defined by the PSVN API. It is the type used for individual states.

    PriorityQueue<state_t> open;
    //state_map_t *map = new_state_map();//******
    // VARIABLES FOR ITERATING THROUGH state's SUCCESSORS
    state_t child;
    ruleid_iterator_t iter; // ruleid_terator_t is the type defined by the PSVN API successor/predecessor iterators.
    int ruleid,d; // an iterator returns a number identifying a rule
    int64_t totalNodes;

    // READ A LINE OF INPUT FROM stdin
    printf("Please enter a state followed by ENTER: ");
    if ( fgets(str, sizeof str, stdin) == NULL ) {
        printf("Error: empty input line.\n");
        return 0; 
    }

    // CONVERT THE STRING TO A STATE
    nchars = read_state( str, &state );
    if (nchars <= 0) {
        printf("Error: invalid state entered.\n");
        return 0; 
    }

    state_t root = state;

    printf("The state you entered is: ");
    print_state( stdout, &state );
    printf("\n");

    state_map_t *map = new_state_map();
    /*first_goal_state( &state1, &d ); do {
        state_map_add( map, &state1, 0 );
        open.Add(0, 0, state1 );
    } while( next_goal_state( &state1, &d ) );
*/
    state_map_add(map,&root,0);
    int bound = 0;
    while (true){

        Pair p = cost_bounded_dfs_visit(&root, bound, map);
        if (p.first != NULL){
            //PRINT STUFF
            printf("SUCCES\n");
            return 1; //SUCCES
        }
        bound = p.second;
    }
    printf("FAIL\n");
    return 0;
}
