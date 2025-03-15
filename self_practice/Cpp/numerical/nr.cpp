// Newton Raphson Method
#include <iostream>
#include <cmath>

using namespace std;

// parameters
double x0 = 0;
double x = x0;
double e = 1e20;
double elim = 1e-6;
int iter = 0;
int iterlim = 100;

// x - cos(x)
double f_fcn(double x){
    return (x - cos(x));
}

// derivative of x - cos(x)
double df_fcn(double x){
    return (1 + sin(x));
}

// main function
int main(){
    double xnew;
    while ((e > elim) && (iter < iterlim)){
        double df = df_fcn(x);

        if (df != 0){
            xnew = x - f_fcn(x)/df;
        } else {
            cout << "Error: df = 0" << endl;
            break;
        }
        e = fabs(xnew - x);
        x = xnew;
        iter++;
        cout << "Iter: " << iter << " x: " << x << " e: " << e << endl;
    }
    return 0;
}