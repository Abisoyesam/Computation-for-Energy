#include <iostream>
#include <cmath>

using namespace std;

double g_function(double x) {
    return cos(x);
}

int main() {
    double x, xnew, e, elim, x0;
    cout << "Resolution by Fixed-point theorem" << endl;
    int iter = 0, iterlim = 100;
    elim = 1e-6;
    x0 = 0; // starting point (can be entered by cin >>x 0)
    x = x0;
    e = 1e20;
    while ((e > elim) && (iter < iterlim)) {
        xnew = g_function(x);
        e = fabs(xnew - x);
        x = xnew;
        iter++;
        cout << "iter " << iter << " x = " << x
             << " e = " << e << endl;
    }
    return 0;
}

