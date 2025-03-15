#include <stdio.h>

// Read from console and print to console
int main(){
    double x;
    printf("Enter a number: ");
    scanf("%lf", &x); // %lf for double; &x is the address of x
    printf("You entered: %f\n", x);
    return 0;
}