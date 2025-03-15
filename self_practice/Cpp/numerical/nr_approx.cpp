// Newton Raphson method using approximate derivative
#include <iostream>
#include <cmath>

using namespace std;

double x0 = 0.0;
double x = x0;
double elim = 1.0e-6;
double e = 1e20;
int iter = 0;
int iterlim = 100;
double tol = 1.0e-8;

double f_fnc(double x){
    return (x - cos(x));
}

double df_aproxim(double x){
    return (f_fnc(x + tol) - f_fnc(x)) / tol;
}

int main(){
    double xnew; 
    while ((e > elim) && (iter < iterlim)){
        double df = df_aproxim(x);
        if (df != 0){
            xnew = x - f_fnc(x) / df;
        } else {
            cout << "Derivative is zero" << endl;
            break;
        }
        e = fabs(xnew - x);
        x = xnew;
        iter++;
        cout << "Iter: " << iter << " x: " << x << " e: " << e << endl;
    }

    return 0;
}