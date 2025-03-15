// Fixed Point Method for x - cos(x) = 0
#include <iostream>
#include <cmath>

using namespace std;

// parameters
double x0 = 0; // initial guess or starting point
double x = x0;
double e = 1e20; // initialize to something big
double elim = 1e-6; // tolerance
int iter = 0; int iterlim = 100;

// x = g(x); g(x) = cos(x)
double g_fcn(double x){
    return cos(x);
}

int main(){
    while ((e > elim)&&(iter < iterlim)){
        double xnew = g_fcn(x);
        e = fabs(xnew - x);
        x = xnew;
        iter++;
        cout << "iter " << iter << " x = " << x << " e= " << e << endl;
    }
    return 0;
}
