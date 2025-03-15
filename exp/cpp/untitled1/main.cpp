#include <iostream>
#include <cmath>
using namespace std;

double g_function(double x) {
    return cos(x);
}

int main() {
    double x, xnew, e, x0;
    x0 = 0; // starting point (can be entered by cin >>x 0)
    x = x0;
    e = 1e20;
    while (e > 1e-6) {
        xnew = g_function(x);
        e = fabs(xnew - x); // float absolute
        x = xnew; // copy back
        cout << " x = " << x << " e = " << e << endl;
    }
    return 0;
}