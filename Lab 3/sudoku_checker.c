#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


//pass values to threads to determine which cells to check
typedef struct {
	int row, col;
} parameters;

int grid[9][9];

void *check_box(void *d){
	parameters *data = (parameters *)(d);
	//used as a reference to determine that each parameter appears exactly once in the box
	int ref[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
	
	//get the position of the box in the grid	
	int y = (data->row)*3;	
	int x = (data->col)*3;
	
	//sift through box to check values at each location	
	for (int i = y; i < y + 3; i++){
		for (int j = x; j < x + 3; j++){
			
			if (grid[i][j]-1 >=0){
				//check if value has been read previously
				if (ref[grid[i][j]-1]){
					//if so set to 1 to indicate that the value has been read
					ref[grid[i][j]-1] = 0;
				}
			}
#ifdef TEST
			printf("%d ",grid[i][j]);
#endif
		}
#ifdef TEST
		printf("\n");
#endif
	}
	
	
	//now check values of ref to determine if valid
	for (int i = 0; i < 9; i++){
		if (ref[i] == 1){
			//return 1 to indicate invalid solution
			return (void *) 1;
		}
	}
	
	
	return (void *) 0;
}

void *check_lines(void *d){
	//used to flip between rows or cols
	parameters *data = (parameters *)(d);
		
	//sift through each row to check values at each location	
	for (int i = 0; i < 9; i++){
		//used as a reference to determine that each parameter appears exactly once in the line
		int ref[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
		for (int j = 0; j < 9; j++){
			//check if value has been read already
			int pos;
			//use data to flip between checking rows or columns
			if (data->row){
				pos = grid[i][j]-1;
			} else {
				pos = grid[j][i]-1;
			}
			if (pos >=0){
				//check if value has been read before
				if (ref[pos]){
					ref[pos] = 0;
				}
			}
#ifdef TEST
			printf("%d ",pos+1);
#endif
		}
#ifdef TEST
		printf("\n");
#endif
		//now check values of ref to determine if valid
		for (int i = 0; i < 9; i++){
			//return 1 if line is invalid
			if (ref[i]){
				return (void *) 1;
			}
		}
	}
	//return after each line validates with 0
	
	return (void *) 0;
}



int main (int argc, char *argv[]) {
	//read file
	FILE *fp;
	//user can supply file of choice to check if valid if wanted, will default to 'solution.txt' if no args are supplied
	if (argc < 2){
		fp = fopen("solution.txt","r");
	} else {
		fp = fopen(argv[1], "r");		
	}
	if (!fp){
		printf("Error: cannot open file\n");
		return 1;
	}
	
	//fill the grid with the values from the file
	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++){
			if(fscanf(fp, "%d", &grid[i][j]) == EOF) {
				printf("Error: file ended unexpectedly!\n");
				return 1;
			}
#ifdef PRINTGRID
			printf("%d ",grid[i][j]);
#endif
		}
#ifdef PRINTGRID
		printf("\n");
#endif
	}
	fclose(fp);

	//holds the thread ids of the 11 threads
	pthread_t tid[11];
	//holds the parameters to be passed to the corresponding thread
	parameters *data[11];

	//for each 3 by 3 square on the grid use a thread to check if the solution is valid
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++){
			data[i*3+j] = (parameters *) malloc(sizeof(parameters));
			data[i*3+j]->row = i;
			data[i*3+j]-> col = j;
			pthread_create(&tid[3*i+j], NULL, check_box, data[i*3+j]);
		}
	}

	//use two more threads check if all the rows and columns are valid
	for (int i = 0; i < 2; i++){
		data[i+9] = (parameters *) malloc(sizeof(parameters));
		data[i+9]->row = i;
		data[i+9]-> col = 0;
		pthread_create(&tid[i+9], NULL, check_lines, data[i+9]);
	}
	
	//check if the threads return valid and join the threads
	int check_valid = 1;	
	for (int i = 0; i < 11; i++) {
		//holds return from thread
		void *status;		
		pthread_join(tid[i], &status);
		
		//if status is 1 the thread detects an invalid solution
		if (status){
			check_valid=0;
		}
		
	}
	//display appropriate message once checking is complete
	if (check_valid){
		printf("Valid solution\n");
	}
	else {
		printf("Invalid solution\n");
	}
	for (int i = 0; i < 11; i++){
		free(data[i]);
	}
	return 0;
}
