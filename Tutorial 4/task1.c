#define _XOPEN_SOURCE 600 
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void *hello_world() {
	printf("Hello world! \n");
}

void *goodbye() {
	printf("Goodbye! \n");
}

pthread_t thread1, thread2;
void *status;

int main() {
	pthread_create(&thread1, NULL, hello_world, (void*)0); 
    pthread_create(&thread2, NULL, goodbye, (void*)1); 

	pthread_join(thread1, &status);
	pthread_join(thread2, &status);
	
	return 0;
}