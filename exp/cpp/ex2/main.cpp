#include <iostream>

using namespace std;

double moy(double a, double b) {
    double r;
    r = (a + b) / 2.;
    return r;
}

void double_it(double &a, double &b) {
    a *= 2;
    b *= 2;
}

int main() {
    double z;
    z = moy(18, 15.5);
    cout << "The result is " << z << endl;
    double x = 3.5, y = 4.5;
    double_it(x, y);
    cout << "new values are x=" << x << " y=" << y << endl;
}
