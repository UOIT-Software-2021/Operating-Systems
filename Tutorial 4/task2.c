#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>

void *bellcurve(void *grade) {
	double curved = *((double *)grade)*1.5;
    printf("%.2lf \n", curved);
}

pthread_t thread1, thread2, thread3, thread4, thread5;
void *status;

int main(int argc, const char* argv[])
{
    double grade1, grade2, grade3, grade4, grade5;
    printf("Enter first grade: ");
    scanf("%lf", &grade1);
    printf("Enter second grade: ");
    scanf("%lf", &grade2);
    printf("Enter third grade: ");
    scanf("%lf", &grade3);
    printf("Enter fourth grade: ");
    scanf("%lf", &grade4);
    printf("Enter fifth grade: ");
    scanf("%lf", &grade5);

    double *arg = malloc(sizeof(*arg));
    *arg = grade1;
    pthread_create(&thread1, NULL, bellcurve, arg);
    pthread_join(thread1, &status); 

    *arg = grade2;
    pthread_create(&thread2, NULL, bellcurve, arg);
    pthread_join(thread2, &status);

    *arg = grade3;
    pthread_create(&thread3, NULL, bellcurve, arg);
    pthread_join(thread3, &status); 

    *arg = grade4;
    pthread_create(&thread4, NULL, bellcurve, arg);
    pthread_join(thread4, &status);

    *arg = grade5;
    pthread_create(&thread5, NULL, bellcurve, arg);
    pthread_join(thread5, &status);
}