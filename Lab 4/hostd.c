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





resources res_avail;

int main(void) {
	int avail_mem[MEMORY];

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
	

	FILE *fp;
	fp = fopen("dispatchlist", "r");
	if (fp == NULL){
		printf("file error!\n");
		return 1;
	}
	//read file line by line untile EOF is reached
	while (1) {
		proc process;
		//read all relevant data from the line in the file
		if (fscanf(fp, "%d%*c %d%*c %d%*c %d%*c %d%*c %d%*c %d%*c %d\n", &process.arrivalTime, &process.priority, &process.processorTime, &process.memory, &process.printers, &process.scanners, &process.modems, &process.cds) == EOF) {
			break;
		}
		//set pid to 0
		process.pid = 0;
		//set address to 0
		process.address = 0;
		//add to appropriate queue
		switch (process.priority) {
			case 0:
				push (&realtime, process);
				break;
			case 1:
				push (&priority1, process);
				break;
			case 2:
				push (&priority2, process);
				break;
			default:
				push (&priority3, process);
		}
	}
	fclose(fp);
	

	
	return 0; 
}


