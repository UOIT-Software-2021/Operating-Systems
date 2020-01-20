#include <stdlib.h>
#include <stdio.h>
int main(void) {
    char data[] = "Hello World!";
    int i = 0; 
    while (1) {
        if (data[i] != '\0'){
            printf("%c", data[i]);
        }
        else { 
            break;
        }
        i++;
    }
    printf("\n");
}
