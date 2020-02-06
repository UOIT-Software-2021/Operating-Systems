#define _XOPEN_SOURCE 600 
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

struct student {
	char studentName[100];
    int studentID;
    int grade;
} student; 

void *bellcurve(struct student student) {
	double curved = *((double *)student.grade)*1.5;
    printf("%.2lf \n", curved);
}

pthread_t thread[5];
void *status;

int main()
{
	char studentName[100];
    int studentID;
    int grade;
    struct student stu0, stu1, stu2, stu3, stu4;

    printf("Enter first grade: ");
    scanf("%d", &grade);
    printf("Enter first Student ID: ");
	scanf("%d", &studentID);
	printf("Enter first Student Name: ");
	scanf("%s", &studentName);

	stu0.grade = grade;
	stu0.studentID = studentID;
	strncpy(stu0.studentName, studentName, 100);

	printf("Enter second grade: ");
    scanf("%lf", &grade);
    printf("Enter second Student ID: ");
	scanf("%lf", &studentID);
	printf("Enter second Student Name: ");
	scanf("%s", &studentName);

	stu1.grade = &grade;
	stu1.studentID = &studentID;
	strncpy(stu1.studentName, studentName, 100);

	printf("Enter third grade: ");
    scanf("%lf", &grade);
    printf("Enter third Student ID: ");
	scanf("%lf", &studentID);
	printf("Enter third Student Name: ");
	scanf("%s", &studentName);

	stu2.studentID = &studentID;
	stu2.grade = &grade;
	strncpy(stu2.studentName, studentName, 100);

	printf("Enter fourth grade: ");
    scanf("%lf", &grade);
    printf("Enter fourth Student ID: ");
	scanf("%lf", &studentID);
	printf("Enter fourth Student Name: ");
	scanf("%s", &studentName);

	stu3.studentID = &studentID;
	stu3.grade = &grade;
	strncpy(stu3.studentName, studentName, 100);

	printf("Enter fifth grade: ");
    scanf("%lf", &grade);
    printf("Enter fifth Student ID: ");
	scanf("%lf", &studentID);
	printf("Enter fifth Student Name: ");
	scanf("%s", &studentName);

	stu4.studentID = &studentID;
	stu4.grade = &grade;
	strncpy(stu4.studentName, studentName, 100);

    struct student *arg = malloc(5 * sizeof(*arg));
    *arg = stu0;
    pthread_create(&thread[0], NULL, bellcurve, arg);

    *arg = stu1;
    pthread_create(&thread[1], NULL, bellcurve, arg);

    *arg = stu2;
    pthread_create(&thread[2], NULL, bellcurve, arg);

    *arg = stu3;
    pthread_create(&thread[3], NULL, bellcurve, arg);

    *arg = stu4;
    pthread_create(&thread[4], NULL, bellcurve, arg);

	int i;
	for (i = 0; i < 5; i++){
		pthread_join(thread[i], &status); 
	}
}