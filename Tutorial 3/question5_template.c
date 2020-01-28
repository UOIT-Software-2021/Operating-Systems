#include<stdio.h>
#include<stdlib.h>
#include<math.h>



	typedef struct  
		{
			int  student_id;
			int  mark;
		} grades;
void grade_students(grades *ptr , int num_students );	
int main(void)
{
	


}

void grade_students(grades *ptr , int num_students )
{
	
	FILE *fp=fopen("grades.txt","w");
		
		int sum;
		int average;
		int sd;
		int varsum;
		

			for(int n=0; n<num_students; ++n)
				{
				sum = sum + ptr[n].mark;
				fprintf(fp, "Student #%d 	mark = %d \n", (ptr+n)->student_id,(ptr+n)->mark);
				}
		average= sum/num_students;
			
			for(int n=0; n<num_students; ++n)
				{
				varsum = varsum + pow((ptr[n].mark - average),2);
				}
		sd = sqrt(varsum/num_students);
		fprintf(fp, " average = %d , standard diviation =%d", average, sd);
	fclose(fp);
}
