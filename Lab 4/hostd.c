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

#define MEMORY 1024

int main(void) {

	node_t *realtime = NULL;
	node_t *priority1 = NULL;
	node_t *priority2 = NULL;
	node_t *priority3 = NULL;
	
	FILE *fp;
	fp = fopen("dispacthlist", "r");
	if (fp == NULL){
		printf("file error!\n");
		return 1;
	}
	//read file line by line untile EOF is reached
	while (1) {
		proc process;
		if (fscanf(fp, "%d%*c %d%*c %d%*c %d%*c %d%*c %d%*c %d%*c %d\n", process.arrivalTime, process.priority, process.processorTime, process.memory, process.printers, process.scanners, process.modems, process.cds, process.pid) == EOF) {
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
