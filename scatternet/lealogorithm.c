#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#define MAX_CONNECTION_LIMIT 2
#define BUFFER_SIZE 1024
#define TIMEOUT_SECONDS 20
#define NUM_STATES 6

typedef enum {
       	ADV_NEIGHBOUR_ADDR,
	DELEGATE,
	DELEGATED
	CONNECT,
	DONE,
	NUM_STATES
}StateType;

typedef struct {
	StateType state;
	void (*func)(void);
}StateMachineType;

//State function prototypes
void adv_own(void);
void adv_neighbour(void);
void delegate(void);
void delegated(void);
void connect(void);
void done(void);

StateMachineType state_machine[] = 
{
	{ADV_NEIGHBOUR_ADDR, adv_neighbour},
	{DELEGATE, delegate},
	{DELEGATED, delegated},
	{CONNECT, connect},
	{DONE, done}
}

//Function prototypes
//void advertise(void);
void scan(void);
void common_neighbours(void);	//Calculate common neighbours
void max_neighbour(void);		//Calculate the greatest neighbour

StateType state = ADV_OWN_ADDR;
//------------------------Global variables
int i_am_prey = 0; // if 1 then this device is a prey
//------------------------
void adv_neighbour(void) {
	//Advertise our neighbours
	//Scan for neighbours and if a new neighbour is found add it to memory and prey_list
	
	//If timer times out and if neighbour_max > own_addr then reset timer and keep to same state
	//If (timer times out and neigbour_max < own_addr) and prey_length <= connection_capacity go to the CONNECT state
	//If (timer times out and neigbour_max < own_addr) and prey_length > connection_capacity go to the DELEGATE state
	//If we receive prey msg we go to the DELEGATED state
	
	prinf("hej\n");
  
  time_t start = time(0);
  char arr[10][18];
  int counter = 0;
  int current = 0;
  struct nb_object *nb_object = NULL;
  
  while (1) {
    char neighbour [] = "";
    if ((nb_object != NULL)) {
      printf("this is being advertised %s\n", arr[current]);
      advertise(arr[current]);
      current++;
      if(current > counter){
	current = 0;
      }
    } else {
      advertise(neighbour);
    }
    nb_object = scan(nb_object);
    
    ll_foreach(nb_object, it){
    if('T' == it->de;){
    	if(0 == strcmp(my_bd, it->nb_bdaddr)){
    		state = DELEGATED;
    		}
    	}
      	add_to_array(arr, nb_object, &counter);
    }
    
  
  
	  // Add entries in datastructure
	  struct nb_object *ptr[16];
	  *ptr = fill_entries(ptr, nb_object);
	  
	  // Show neighbours neighbour, remove when sure it works.
	  struct nb_object *rtn = NULL;
	  rtn = rtn_nb_ptr(ptr);
	  printf("%s\n", rtn->nb_bdaddr);
	  printf("dab on the haters\n");
	  ll_foreach(rtn, it){
		printf("dont dab on the haters\n");
		print_nb_nb(ptr, it->nb_bdaddr);
	  }
	  
	  // Add prey list
	  char prey[10][18];
	  char *my_bd_addr = print_own_bd_addr(); // This function is in connect handler
	  char my_bd [18] = *my_bdaddr();
	  //char my_bd [18] = *print_own_bd_addr();
	  int nmb_of_prey = 0;
	  
	  ll_foreach(rtn, it){
		if(0 < strcmp(my_bd, it->nb_bdaddr)) { // This means my_bd > it->nb_bdaddr
		  prey[nmb_of_prey] = it->nb_bdaddr;
		  nmb_of_prey++;
		}
		else{ // This node has a nb with a higher unique identifier
		  i_am_prey = 1;  
		}
	  }
	  
	  if (time(0) - start >= 20) {
	  	if(i_am_pray) {
	  		continue;
	  	} else {
	  		state = DELEGATE;
	  	}
      }

	  if(nmb_of_prey <= 2) { // Go into connect phase
		state = CONNECT;
	  }
	}

}

void delegate(void) {
	//Advertise a prey msg to neigbour_max
	//Remove neighbour_max from prey_list
	//Remove Common neighbours from prey 
	//Decrement connection_capacity
	//Color edge to neighbour_max blue
	//If (timer times out and neigbour_max < own_addr) and prey_length > connection_capacity go to the DELEGATE state
	//If (timer times out and neigbour_max < own_addr) and prey_length <= connection_capacity go to the CONNECT state	
}
void delegated(void) {
	//Calculate Common neighbours
	//Connect to Common neighbours
	//Color edges
	//Go to the DONE state
}
void connect(void) {
	//Connect to neighbours
	//color edges 
	//Go to the DONE state
}
void done(void) {
	//Go to Phase 2, I guess...
}

int main(){
	if(state < NUM_STATES) {
		(*state_machine[state].func();
	} else {
		perror("Invalid state");
	}
}
