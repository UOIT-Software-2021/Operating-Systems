#define _POSIX_SOURCE //required to not get warning for using kill
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h> 
#include "queue.h"
#include "hostd.h"


/*
	This is will be the same as hostd but with testing prints enabled

*/

int main(void) {
	int avail_mem[MEMORY];
	resources res_avail;

	
	//use to initialize avail_mem to 0
    	free_mem(avail_mem, 0, MEMORY, 0);
	
	//initialize empty queues	
	node_t *realtime = NULL;
	node_t *priority1 = NULL;
	node_t *priority2 = NULL;
	node_t *priority3 = NULL;
	res_avail.printers = 2;
	res_avail.scanners = 1;
	res_avail.modems = 1;
	res_avail.cds = 2;
	
	int timePassed = -1;
	FILE *fp;
	fp = fopen("dispatchlist", "r");
	if (fp == NULL){
		printf("file error!\n");
		return 1;
	}
	//read file line by line untile EOF is reached
	while (1) {
		proc *process = (proc *) malloc(sizeof(proc));
		//read all relevant data from the line in the file
		if (fscanf(fp, "%d%*c %d%*c %d%*c %d%*c %d%*c %d%*c %d%*c %d\n", &process->arrivalTime, &process->priority, &process->processorTime, &process->memory, &process->printers, &process->scanners, &process->modems, &process->cds) == EOF) {
			break;
		}
		//set timePassed to start at initial arrival time
		if (timePassed < 0){
			timePassed = process->arrivalTime;
		}
		
		//set pid to 0
		process->pid = 0;
		//set address to 0
		process->address = 0;
		//set suspended to 0
		process->suspended = 0;
		//add to appropriate queue
		process->started = -1;

		printf("%d, %d, %d, %d, %d, %d, %d, %d\n", process->arrivalTime, process->priority, process->processorTime, process->memory, process->printers, process->scanners, process->modems, process->cds);
		pushByQueue(&realtime, &priority1, &priority2, &priority3, process);
	}
	fclose(fp);		
	printf("Time Passed %d\n", timePassed);
	priorityIterate(&realtime, &priority1, &priority2, &priority3, avail_mem, res_avail, timePassed, 1);
	
	return 0; 
}


