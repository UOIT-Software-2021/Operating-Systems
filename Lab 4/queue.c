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


void push(node_t **head, proc process){
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

proc pop(node_t **head){

    proc process = {0, 0, 0, 0, 0, 0, 0, 0, 0};
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
