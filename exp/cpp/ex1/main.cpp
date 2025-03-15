#include <iostream>

using namespace std;

// Function declaration
double moy(double a, double b);

int main() {
 double z;
 z = moy(18, 15.5);
 cout << "The result is " << z;
}

// Function definition
double moy(double a, double b) {
    double r;
    r = (a + b) / 2.;
    return r;
}