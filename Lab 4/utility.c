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

int set_mem(int *avail_mem, int memory, int p){
	int address = 0;
	int maxmem = MEMORY - 65;	
	//start at dedicated time memory location	
	if (p == 0){
		address = maxmem;
		maxmem = MEMORY;
	}
	//iterate through each memory address
	while (address < maxmem && memory <= maxmem){
		int allocate = 1;
		//check if there is available address with enough consecutive memory        
		for (int i = address; i < address+memory && i < maxmem; i++){
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
