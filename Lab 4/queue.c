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


void push(node_t **head, proc *process){
    //default push
    if(*head != NULL){
        node_t *next = (*head);
        while (next -> next != NULL){
            next = next -> next;
        }
        next->next = (node_t *) malloc(sizeof(node_t));
        next->next->data = process;
        next->next->next = NULL;
    } else {    
        //initialize head if NULL
        *head = (node_t *) malloc(sizeof(node_t));
        if(*head==NULL){
            printf("error: Failed to allocate head!\n");
            exit(1);
        }                     
        (*head)->data = process;
             (*head)->next = NULL;
    }
}

proc *pop(node_t **head){

	proc *process = NULL;
	node_t *nextInQueue = NULL;
	if (*head == NULL){
		//handle empty queue
		printf("error: Popping empty queue!\n");
		return process;
	}
	//get next in line
	nextInQueue = (*head)->next;
	process = (*head)->data;
	free(*head);
	*head = nextInQueue;

	return process;
}

proc *peek(node_t **head){
	proc *process = NULL;
    
	if (*head == NULL){
		//handle empty queue
		printf("error: Popping empty queue!\n");
		return process;
	}
	process = (*head)->data;
	//return value without removing from queue
	return process;
}

int queueSize(node_t **head){
	if(*head != NULL){
		node_t *next = (*head);
		int count = 1;
		while (next -> next != NULL){
	    		next = next -> next;
			count += 1;
		}
		return count;
    	} else {    
		return 0;              	
    	}
}
