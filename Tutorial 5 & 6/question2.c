#define _XOPEN_SOURCE 700 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int main(void)
{
    int child = fork();
    if (child == 0){
        sleep(1);
        printf("Child process");
        exit(0);
    }
    wait(NULL);
    printf("Parent process");
}