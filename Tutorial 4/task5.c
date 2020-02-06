#define _XOPEN_SOURCE 600 
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

double total_bellcurve = 0; 
double total_grade = 0;

void read_grades() 
{
	FILE *file;
	file =fopen("grades.txt", "R");

	int grades[10];

	int i = 0;
	while (!feof (file))
    {  
      fscanf(file, "%d", &grades[i]);    
	  i++;  
    }
}

void save_bellcurve(double grade){
	
}

void *bellcurve(int grade) {
	double curved = grade*1.5;
}

int main()
{
	
}