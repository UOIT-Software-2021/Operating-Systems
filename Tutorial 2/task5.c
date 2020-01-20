#include <stdlib.h>
#include <stdio.h>
#include <math.h>

double euclid_dist(int x1, int y1, int x2, int y2){
    return sqrt(pow((double)(x2 - x1), 2) + pow((double)(y2 - y1), 2));
}

int main(void) {
    int i;
    for (i = 0; i < 10; i++) {
        printf("%lf ", euclid_dist(rand() % 100, rand() % 100, rand() % 100, rand() % 100));
    }
    printf("\n");
}
