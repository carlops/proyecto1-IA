#include <stdlib.h>
#include <vector>
#include "priority_queue.hpp"
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include <sys/time.h>

#define  MAX_LINE_LENGTH 999 

class List {
    struct Node {
        state_t *data;
        int color; //0:White,1:Gray,2:Black
        int distance;
        Node * next;
    };

    Node *head;

    public:
    List() {
        head = NULL;
    }

    ~List() {
        while(head != NULL) {
            Node * n = head->next;
            delete head;
            head = n;
        }
    }

    void add(state_t *value) {
        Node * n = new Node;
        n->data = value;
        printf("ADD");
        print_state(stdout,value);
        printf("\n");
        n->color = 0;
        n->distance = 10000000;//should be max_int
        n->next = head;
        head = n;
    }

    bool is_on(state_t *value){
        Node *aux = head;
        while(aux != NULL){
            if (compare_states(aux->data,value)==0)
                return true;
            else
                aux = aux->next;
        }
        return false;
    }

    bool change_color(state_t *value, int color){
        Node *aux = head;
        while (aux != NULL){
           if (compare_states(aux->data,value)==0){
               aux->color = color;
               printf("TRUE!,change color %d ,",aux->color);
print_state(stdout,value);
printf("\n");
               return true;
           }
           else
               aux = aux->next;
        }
        return false;
    }

    int get_color(state_t *value){
        Node *aux = head;
        while (aux != NULL){
           if (compare_states(aux->data,value)==0){
 printf("TRUE! get color ");
print_state(stdout,value);

 printf(" Color: %d\n",aux->color);
               return aux->color;
           }
           else
               aux = aux->next;
        }
        return 42; // 42 is flag for 'value' not found
    }

    bool change_distance(state_t *value, int distance){
        Node *aux = head;
        while (aux != NULL){
           if (compare_states(aux->data,value)==0){
               aux->distance = distance;
 printf("TRUE! change distance %d ,",aux->distance);
 print_state(stdout,value);
 printf("\n");

               return true;
           }else
               aux = aux->next;
        }
        return false;
    }

    int get_distance(state_t *value){
        Node *aux = head;
        while (aux != NULL){
           if (compare_states(aux->data,value)==0){
 printf("TRUE! get distance ");

print_state(stdout,value);
 printf(" Distancia: %d\n",aux->distance);
               return aux->distance;
           }
           else
               aux = aux->next;
        }
        return 42; // 42 is a flag for 'value' not found
    }
    // ...
};

int main( int argc, char **argv )
{
    // VARIABLES FOR INPUT
    char str[ MAX_LINE_LENGTH +1 ] ;
    ssize_t nchars; 
    state_t state; // state_t is defined by the PSVN API. It is the type used for individual states.

    PriorityQueue<state_t> open;
    //state_map_t *map = new_state_map();//******
    // VARIABLES FOR ITERATING THROUGH state's SUCCESSORS
    state_t child;
    ruleid_iterator_t iter; // ruleid_terator_t is the type defined by the PSVN API successor/predecessor iterators.
    int ruleid ; // an iterator returns a number identifying a rule
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

    printf("The state you entered is: ");
    print_state( stdout, &state );
    printf("\n");

    List StateList = List(); 
   // state_map_add( map, &state, 0 );//******dont know if its a must
    open.Add(0,0,state);
    StateList.add(&state);
    StateList.change_color(&state,1);// Gray
   // StateList.change_distance(&state,0);

    totalNodes = 0;
    int d = 0;
    /* Search */
    while ( !open.Empty() ) {

        // g.n
        d = open.CurrentPriority();
printf("D: %d\n",d);
        state = open.Top();
        open.Pop();

        totalNodes++;

        /* DDD */
        if (StateList.get_color(&state)==0 || (StateList.get_distance(&state)>(d-1))){
            StateList.change_distance(&state,d);
            if (is_goal(&state)==1){
                //PRINT STUFF
                printf("Estado: ");
                print_state( stdout, &state);
                printf(" Estados Generados: %"PRId64" , costo: %d",totalNodes,StateList.get_distance(&state));
                return 1;//SUCCES
            }

            /* expand node */
            init_fwd_iter(&iter, &state);
            while((ruleid = next_ruleid(&iter)) >= 0){
                apply_fwd_rule(ruleid,&state,&child);//'child' is a succesor state_t of 'state'
                StateList.add(&child);
                StateList.change_color(&child, 1);//Gray
                const int child_d = d + get_fwd_rule_cost(ruleid);
                StateList.change_distance( &child , child_d );
                open.Add( child_d , child_d , child );
            }
            StateList.change_color(&state,2); //Black
        }

    }
    printf("FAIL!");
    return 2; //FAIL
}
