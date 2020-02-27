#define _XOPEN_SOURCE 700 // required for barriers to work
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int *moving_sum[5] = {0,0,0,0,0};
pthread_t thread1, thread2, thread3, thread4, thread5;
void *status;
sem_t mutex;

struct Info {
    int number;
    int index;
};

void factorial(struct Info element){
    int result;
    for (int i = 1; i <= element.number; i++)
        result = result * i;
    sem_init(&mutex, 0, 1); 
    if (*moving_sum[element.index-1] > 0){
        result += *moving_sum[element.index-1];
        *moving_sum[element.index] += result;
    }
    while(*moving_sum[element.index-1] > 0){
        //do signal and wait here
    }
}

int main(void)
{
    struct Info First;
    struct Info Second;
    struct Info Third;
    struct Info Fourth;
    struct Info Fifth;
    
    printf("Enter first number: ");
    scanf("%i", &First.number);
    First.index = 0;
    printf("Enter second number: ");
    scanf("%i", &Second.number);
    Second.index = 1;
    printf("Enter third number: ");
    scanf("%i", &Third.number);
    Third.index = 2;
    printf("Enter fourth number: ");
    scanf("%i", &Fourth.number);
    Fourth.index = 3;
    printf("Enter fifth number: ");
    scanf("%i", &Fifth.number);
    Fifth.index = 4;

    struct Info *arg = malloc(sizeof(*arg));
    *arg = First;
    pthread_create(&thread1, NULL, factorial, arg);
    pthread_join(thread1, &status); 

    *arg = Second;
    pthread_create(&thread2, NULL, factorial, arg);
    pthread_join(thread2, &status); 

    *arg = Third;
    pthread_create(&thread3, NULL, factorial, arg);
    pthread_join(thread3, &status); 

    *arg = Fourth;
    pthread_create(&thread4, NULL, factorial, arg);
    pthread_join(thread4, &status); 

    *arg = Fifth;
    pthread_create(&thread5, NULL, factorial, arg);
    pthread_join(thread5, &status); 
}