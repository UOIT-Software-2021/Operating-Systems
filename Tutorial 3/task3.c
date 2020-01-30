#include <stdio.h>

struct student {
    int studentID;
    int age;
    int yearStarted;
} book; 

void save_student(struct student student){
    FILE * file;
    file = fopen("students.txt","a");
    fprintf(file, "%i,%i,%i\n", student.studentID, student.age, student.yearStarted);
    fclose(file);
}

int main(int argc, const char* argv[])
{
    int studentID, age, yearStarted;
    struct student student;
    printf("Enter your student ID: ");
    scanf("%i", &studentID);
    printf("Enter your age: ");
    scanf("%i",&age);
    printf("Enter the year you started university: ");
    scanf("%i",&yearStarted);
    student.studentID = studentID;
    student.age = age;
    student.yearStarted = yearStarted;
    save_student(student);
}