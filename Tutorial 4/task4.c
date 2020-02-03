#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <math.h>

double total_grade;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *class_total(void *grade) {
    pthread_mutex_lock(&mutex);
	total_grade += *((double *)grade);
    pthread_mutex_unlock(&mutex);
    return 0;
}

pthread_t thread1, thread2, thread3, thread4, thread5, thread6, thread7, thread8, thread9, thread10;
void *status;

int main(int argc, const char* argv[])
{
    double grade1, grade2, grade3, grade4, grade5, grade6, grade7, grade8, grade9, grade10;
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
    printf("Enter sixth grade: ");
    scanf("%lf", &grade6);
    printf("Enter seventh grade: ");
    scanf("%lf", &grade7);
    printf("Enter eighth grade: ");
    scanf("%lf", &grade8);
    printf("Enter ninth grade: ");
    scanf("%lf", &grade9);
    printf("Enter tenth grade: ");
    scanf("%lf", &grade10);

    double *arg = malloc(sizeof(*arg));
    *arg = grade1;
    pthread_create(&thread1, NULL, class_total, arg);
    pthread_join(thread1, &status); 

    *arg = grade2;
    pthread_create(&thread2, NULL, class_total, arg);
    pthread_join(thread2, &status);

    *arg = grade3;
    pthread_create(&thread3, NULL, class_total, arg);
    pthread_join(thread3, &status); 

    *arg = grade4;
    pthread_create(&thread4, NULL, class_total, arg);
    pthread_join(thread4, &status);

    *arg = grade5;
    pthread_create(&thread5, NULL, class_total, arg);
    pthread_join(thread5, &status);
    
    *arg = grade6;
    pthread_create(&thread6, NULL, class_total, arg);
    pthread_join(thread6, &status);

    *arg = grade7;
    pthread_create(&thread7, NULL, class_total, arg);
    pthread_join(thread7, &status);

    *arg = grade8;
    pthread_create(&thread8, NULL, class_total, arg);
    pthread_join(thread8, &status);

    *arg = grade9;
    pthread_create(&thread9, NULL, class_total, arg);
    pthread_join(thread9, &status);

    *arg = grade10;
    pthread_create(&thread10, NULL, class_total, arg);
    pthread_join(thread10, &status);

    pthread_mutex_lock(&mutex);
    printf("%.2lf \n", total_grade);
    pthread_mutex_unlock(&mutex);
}