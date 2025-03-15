// Fixed point method for x - cos(x) = 0
#include <iostream>
#include <cmath>

using namespace std;

// x = g(x) where g(x) = cos(x)

double g_fcn(double x){
    return cos(x);
}

// parameter
double x0 = 0; // starting point (intial guess)
double x = x0;
double e = 1e20;
double elim = 1e-6;

int main(){
    while (e > elim){
        double xnew = g_fcn(x);
        e = fabs(xnew - x);
        x = xnew;
        cout << "x = " << x << " e = " << e << endl;
    }
    return 0;
}