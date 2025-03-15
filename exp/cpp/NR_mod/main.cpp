#include <iostream>
#include <cmath>

using namespace std;

double f_function(double x) {
    return (x - cos(x));
}

//double df_function(double x) {
//    return (1 + sin(x));
//}

double df_approxim_function(double x) {
    return ((f_function(x + 1e-8) - f_function(x)) / 1e-8);
}

int main() {
    double x, xnew, e, elim, x0, df;
    cout << "Resolution by N-R" << endl;
    int iter = 0, iterlim = 100;
    elim = 1e-6;
    x0 = 0;
    x = x0;
    e = 1e20;

    while ((e > elim) && (iter < iterlim)) {
        df = df_approxim_function(x);
        if (df != 0) {
            xnew = x - f_function(x) / df;
        } else {
            cout << "df = 0, end of iterations" << endl;
            break;
        }
        e = fabs(xnew - x);
        x = xnew;
        iter++;
        cout << "iter " << iter << " x = " << x
             << " e = " << e << endl;
    }
    return 0;
}