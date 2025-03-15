#include <iostream>

using namespace std;

int main() {
    double x = 99+1;

    if (x == 100)
        cout << "x is 100" << endl;

    if (x == 100) {
        cout << "x is ";
        cout << x << endl;
    }

    if (x == 100)
        cout << "x is 100" << endl;
    else
        cout << "x is not 100" << endl;

    return 0;
}