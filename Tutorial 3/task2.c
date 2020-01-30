#include <stdio.h>

int main(int argc, const char* argv[])
{
    FILE * file;
    file = fopen("question2.txt","r");
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    char numArray[50];
    int count = 0;
    while ((read = getline(&line, &len, file)) != -1) {
        numArray[count] = *line;
        count++;
    }
    printf("%s", &numArray[0]);

    fclose (file);  
}