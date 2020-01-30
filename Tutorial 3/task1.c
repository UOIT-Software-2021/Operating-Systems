#include <stdio.h>

int main(int argc, const char* argv[])
{
	char firstName[20];
    int age, height;
    printf("Enter your first name: ");
    scanf("%s", &firstName);
    printf("Enter your age: ");
    scanf("%d",&age);
    printf("Enter your height (cm): ");
    scanf("%d",&height);

    printf("Your first name is %s. You are %i years old. You are %icm tall.",firstName, age, height);
}