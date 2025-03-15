#include <stdio.h>

int main() {
    int i, N = 10;
    int sum = 0;
    double moy;

    for (i = 0; i < N; i++) {
        sum += i;
    }
    
    moy = sum / N;
    printf("Sum = %d\nMoy = %f", sum, moy);
    return 0;
}