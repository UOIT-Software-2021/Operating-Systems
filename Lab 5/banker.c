
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>

//define to enable additional prints
//#define info 1

// May be any values >= 0
#define NUM_CUSTOMERS 5
#define NUM_RESOURCES 3

// Available amount of each resource
int available[NUM_RESOURCES];
// Maximum demand of each customer
int maximum[NUM_CUSTOMERS][NUM_RESOURCES];
// Amount currently allocated to each customer
int allocation[NUM_CUSTOMERS][NUM_RESOURCES];
// Remaining need of each customer
int need[NUM_CUSTOMERS][NUM_RESOURCES];

pthread_mutex_t mutex;

/*functon definitions*/
bool checkDigit (char *s);//utility to ensure arguments are ints
bool safety(void);
bool request_res(int n_customer, int request[]);
bool release_res(int n_customer, int release[]);

void * customer(void *ptr);


int main (int argc, char *argv[]){
	
	//check if the user entered enough arguments
	if (argc <= NUM_RESOURCES) {
		printf("Not enough arguments! Use %d arguments.\n", NUM_RESOURCES);
		return 1;
	}
	srand(time(0)); 
	for (int i = 0; i < NUM_RESOURCES; i++){
		//ensure arguments were inputted as numbers		
		if(checkDigit(argv[i+1])){
			//add to available and maximum if arguments are numbers
			available[i] = atoi (argv[i+1]);
#ifdef info
			printf("available-%d: %d ", i, available[i]);
#endif
			//randomly generate maximum request for each customer from 0 to available 
			for (int j = 0; j < NUM_CUSTOMERS; j++){
				maximum[j][i] = rand() % (available[i] + 1);
				allocation[j][i] = 0;
				need[j][i] = maximum[j][i];
#ifdef info
			printf("j-%d maximum: %d ", j, maximum[j][i]);
#endif	
			}
#ifdef info
			printf("\n");
#endif			
		} else {
			printf("Arguments must be integers!\n");
			return 1;		
		}
	} 

	pthread_t tid[NUM_CUSTOMERS];
	pthread_mutex_init (&mutex, NULL);
	//continous looping, chose 10 iterations for testing purposes
	int x = 0;	
	while (x < 10){
		int index[NUM_CUSTOMERS];
		//print available resources at the start of each cycle
		printf("Cycle: %d\nAvailable: ", x+1);		
		for (int i = 0; i < NUM_RESOURCES; i++){
			printf("%d ",available[i]);
		}
		printf("\n");
		
		//create threads
		for (int i = 0; i < NUM_CUSTOMERS; i++){
			index[i] = i;
			pthread_create(&tid[i], NULL, customer, &index[i]);
		}		
		for (int i = 0; i < NUM_CUSTOMERS; i++) {
			pthread_join(tid[i], NULL);
		}
		printf("\n");
		x++;
	}
	
	pthread_mutex_destroy(&mutex);
	return 0;
}


bool checkDigit (char *s) {
	while (*s) {
		//return false if not digit
		if (isdigit(*s++) == 0) return false;
	}
	//all characters in s are digits: return true
    	return true;
}

bool safety(void){
	int work[NUM_RESOURCES];
	bool finish[NUM_CUSTOMERS];
	//initialize work to available
	for (int i = 0; i < NUM_RESOURCES; i ++)
		work[i] = available[i];
	//initialize finish to false
	for (int i = 0; i < NUM_CUSTOMERS; i ++) 
		finish[i] = false;
	//holds number of finished
	int finished = 0;
	//search until either all customers finish or cannot find a safe customer
	while (finished < NUM_CUSTOMERS) {
		//true if a safe customer found
		bool safe = false;

		//cycle through each customer searching for an index i
		for (int i = 0; i < NUM_CUSTOMERS; i++){
			//skip if customer finished already
			if(!finish[i]) {
				//check if it is possible for customer to finish
				bool possible = true;
				
				//if a need is found that is greater than work, not possible
				for (int j = 0;j < NUM_RESOURCES; j++){
					if(need[i][j] > work[j]) {
						possible = false;
						break;
					}
				}
			
				//if customer is possible
				if (possible) {
					//add allocation to work
					for (int j = 0;j < NUM_RESOURCES; j++)
						work [j] += allocation[i][j];
					
					//set finish[i] to true
					finish[i] = true;
					//increase total number of customers finished
					finished +=1;
					//request is safe so far
					safe = true;
				}
			}
		}
		
		//cannot find a safe customer: system in unsafe state
		if(!safe) {
			return false;
		}
	}
	//all customers finished: system is in a safe state
	return true;
}
bool request_res(int n_customer, int request[]){
	bool earlyFail = false;
	for(int i = 0; i < NUM_RESOURCES; i++) {
		//deny if request exceeds available resources	
		if(available[i] < request[i]){
			earlyFail = true;	
		}
		available[i] -= request[i];
		allocation[n_customer][i] += request[i];
		need[n_customer][i] -= request[i]; 
	}
	//release resources after early fail
	if (earlyFail){
		release_res(n_customer, request);
		return false;
	}
	
	//run saftey algorithm on current state
	bool safe = safety();	
	//return values to original state if unsafe	
	if(!safe) {
		release_res(n_customer, request);
	}
	return safe;
}

bool release_res(int n_customer, int release[]){
	//release resources by reversing request allocation	
	for(int i = 0; i < NUM_RESOURCES; i++) {	
		available[i] += release[i];
		allocation[n_customer][i] -= release[i];
		need[n_customer][i] += release[i]; 
	}

	return true;
}


void * customer(void *ptr){
	//get thread number from pointer
	int tid = *(int *) ptr;
	//holds request and release vectors
	int request[NUM_RESOURCES];
	int release[NUM_RESOURCES];
	//set to true if resources released	
	bool released = false;
	//set request and release to random numbers within the possible bounds
	for (int i = 0; i < NUM_RESOURCES; i++){
		request[i] = rand() % (need[tid][i] + 1);
		release[i] = rand() % (allocation[tid][i] + 1);
		//if release is required set to true		
		if (release[i] > 0)
			released = true;
	}
	//execute critical section
	pthread_mutex_lock(&mutex);
	//store the state of the request: safe or unsafe
	bool safe = request_res(tid, request);
	
	//print required information
	printf("Thread: %d\nRequested: ", tid);
	for (int i = 0; i < NUM_RESOURCES; i++){
		printf("%d ", request[i]);
	}
	
	printf("\nSafety status: %s\nOutcome: %s\n", safe ? "true" : "false", safe ? "accepted" : "denied");
	
	//check if any resources released, if so, release & print them
	if(released){
		release_res(tid, release);
		printf("released: ");
		for (int i = 0; i < NUM_RESOURCES; i++){
			printf("%d ", release[i]);
		}
		printf("\n");
	}
	//end of critical section
	pthread_mutex_unlock(&mutex);
	//exit thread
	pthread_exit(0);
}

