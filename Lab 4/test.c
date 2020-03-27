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

int main(void) {
	node_t *head = NULL;
	proc t;
	t.arrivalTime = 1;
	t.priority = 2;
	t.processorTime = 3;
	t.memory = 4;
	t.printers = 3;
	t.scanners = 5;
	t.modems = 8;
	t.cds = 10;
	t.pid=0;
	
	push(&head, t);
	

	proc g = pop(&head);
	printf("%d, %d, %d, %d, %d, %d, %d, %d, %d\n", g.arrivalTime, g.priority, g.processorTime, g.memory, g.printers, g.scanners, g.modems, g.cds, g.pid);
}
