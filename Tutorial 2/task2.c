#include <stdlib.h>
#include <stdio.h>
int main(void){
    double data[5] = {1.2, 5.5, 2.1, 3.3, 3.3}; 

    int i;
    for (i=0; i < 5; i++){
        if (data[i] > data[i+1]) {
            printf("Greater than. ");
        }
        else if (data[i] < data[i+1]) {
            printf("Less than. ");
        }
        else {
            printf("Equal to. ");
        }
    }
    printf("\n");
}
