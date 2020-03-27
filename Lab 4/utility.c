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

resources resourceSet(proc process, resources res_avail, int unSet);

int resourceCheck (proc process, resources res_avail);

int set_mem(int *avail_mem, int memory, int p){
	int address = 0;
	int maxmem = MEMORY - 64;
	int maxaddress = MEMORY - 64;	
	//start at dedicated time memory location	
	if (p == 0){
		address = maxmem;
		maxmem = 64;
		maxaddress = MEMORY;
	}
	//iterate through each memory address
	while (address < maxaddress && memory <= maxmem){
		int allocate = 1;
		//check if there is available address with enough consecutive memory        
		for (int i = address; i < address+memory && i < maxaddress; i++){
		    //break if allocated memory is found            
		    if(avail_mem[i]){
			allocate = 0;
			break;
		    }
		}
		//return if allocation is ok
		if(allocate){
		    //set memory to allocated
		    free_mem(avail_mem, address, memory, 1);
		    return address;
		}
		address ++;
	}
	//return negative address to indicate not enough available memory
	return -1;
}

void free_mem(int *avail_mem, int address, int memory, int val){
    for (int i = address; i < address+memory && i < MEMORY; i++){
        avail_mem[i] = val;
    }
}

void pushByQueue(node_t **rt, node_t **p1, node_t **p2, node_t **p3, proc process, int level){
	switch (level) {
		case 0:
			push (rt, process);
			break;
		case 1:
			push (p1, process);
			break;
		case 2:
			push (p2, process);
			break;
		default:
			push (p3, process);
	}
}

int realtimeIterate(node_t **realtime, int * avail_mem, resources res_avail) {
	while(*realtime != NULL){
		//pop next process
		proc process = pop(realtime);
		//set memory array and get starting address
		if ((process.address = set_mem(avail_mem, process.memory, process.priority)) < 0) {
			printf("Error: not enough memory to run process!\n");
			return -1;
		}
		if (resourceCheck(process, res_avail)){
			printf("Error: not enough resources to run process!\n");
			return -1;
		}
		int pid = fork();
		//set pid
		process.pid = pid;


		switch(pid) {
		case -1:
			//error
			printf("Fork error occured!\n");
			exit(-1);

		case 0:
			//child

			//print process information before executing
			process.pid = getpid();//pid would print as 0
			//printf("Name: %7s, Priority: %d, pid: %d, memory: %4d Runtime: %d\n", process.name, process.priority, process.pid, process.memory, process.runtime);
			
			char runtime[32];
			sprintf(runtime, "%d", process.processorTime);

			execl("process", "process", runtime, (char*)NULL);
			//only will reach this statement if exec fails
		    	exit(1);

		default:
			//parent
			sleep(process.processorTime);
			//terminate proc
			kill(pid, SIGINT); 

			//wait for child
			waitpid(pid, 0, 0);
			free_mem(avail_mem, process.address, process.memory, 0);
		}    
	}

	//return 0 on success
	return 0;
}

int priorityIterate(node_t **p1, node_t **p2, node_t **p3, int * avail_mem, resources res_avail, int level) {
	node_t **queue = NULL;	
	switch (level){
		case 1:
			queue = p1;
			break;
		case 2:
			queue = p2;
			break;
		default:
			queue = p3;
	}


	
	while(*queue != NULL){
		//pop next process
		proc process = pop(queue);
		
		//exit if insufficient memory or resources to run process
		if (process.memory > MEMORY-64) {
			printf("Error: not enough memory to run process!\n");
			return -1;
		}
		if (resourceCheck(process, res_avail) == 2){
			printf("Error: not enough resources to run process!\n");
			return -1;
		}
		
		//if enough resources and memory are available run process
		if ((process.address = set_mem(avail_mem, process.memory, process.priority)) >= 0 && resourceCheck(process, res_avail) == 0) { 
			//set resources as used
			res_avail = resourceSet(process, res_avail, -1);
			//check if process has been suspended or is running for the first time
			if (process.pid == 0) {
				int pid = fork();
				//set pid
				process.pid = pid;


				switch(pid) {
				case -1:
					//error
					printf("Fork error occured!\n");
					exit(-1);

				case 0:
					//child

					//print process information before executing
					process.pid = getpid();//pid would print as 0
					
					char runtime[32];
					sprintf(runtime, "%d", process.processorTime);
					execl("process", "process", runtime, (char*)NULL);
					//only will reach this statement if exec fails
				    	exit(1);
				} 
			}
			//continue process if previously suspended 
			else {
				kill(process.pid, SIGCONT);
			}
			
			sleep(1);
			//if runtime over 1 kill the process
			if (process.processorTime > 1) {
				kill(process.pid, SIGTSTP);

				//reduce runtime by 1
				process.processorTime = process.processorTime - 1;

				process.suspended = 1;

				//release resources
				free_mem(avail_mem, process.address, process.memory, 0);
				res_avail = resourceSet(process, res_avail, 1);


				//if queue is empty add back to the same queue
				//otherwise join lower level queue
				if (queue != NULL) {
					pushByQueue(NULL, p1, p2, p3, process, process.priority + level);
				} else {
					pushByQueue(NULL, p1, p2, p3, process, process.priority);
				}
			}
			//1 secord runtime left prepare to kill 
			else {
				kill(process.pid, SIGINT);

				//wait for process

				waitpid(process.pid, 0, 0);

				free_mem(avail_mem, process.address, process.memory, 0);
				res_avail = resourceSet(process, res_avail, 1);

			}			
			
		}
		else {
			push(queue, process);
		}	   
	}

	//return 0 on success
	return 0;
}

resources resourceSet(proc process, resources res_avail, int unSet){
	res_avail.printers = res_avail.printers + unSet*process.printers;
	res_avail.scanners = res_avail.scanners + unSet*process.scanners;
	res_avail.modems = res_avail.modems + unSet*process.modems;
	res_avail.cds = res_avail.cds + unSet*process.cds;
	return res_avail;
}

int resourceCheck (proc process, resources res_avail){
	//check if each required resource type has enough available to run the process
	//if there is not enough resources to run the program check if the resources are above the system limits for resource usage
	/*identify resource avaialibility by codes
	0 - Resources available
	1 - Resources unavailable but may be available in the future
	2 - Resources never available
	*/
	if(process.printers > res_avail.printers){
		if (process.printers > 2)
			return 2;
		return 1;
	}
	if(process.scanners > res_avail.scanners){
		if (process.scanners > 1)
			return 2;
		return 1;
	}
	if(process.modems > res_avail.modems){
		if (process.modems > 1)
			return 2;
		return 1;
	}
	if(process.cds > res_avail.cds){
		if (process.cds > 2)
			return 2;
		return 1;
	}
	return 0;
}
