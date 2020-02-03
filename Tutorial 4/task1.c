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

int main() {
	printf("%d \n", pthread_create(&thread1, NULL, hello_world, (void*)0)); 
    printf("%d \n", pthread_create(&thread2, NULL, goodbye, 1)); 

	return 0;
}