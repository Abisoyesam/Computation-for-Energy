#include <iostream>
#include <cmath>

// Function to calculate (a + b) / c
double calc(double a, double b, double c)
{
    double r;
    r = (a + b) / c;
    return r;
}

int main()
{
    // Calculate and print the result of (2 + 5) / 2
    std::cout << "z = " << calc(2, 5, 2) << std::endl;
    return 0;
}