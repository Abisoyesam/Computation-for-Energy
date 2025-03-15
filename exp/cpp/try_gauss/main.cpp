#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int main() {
    int n = 4;

    // Define matrix A and vectors B, X, and Xnew.
    double A[4][4] = {
            {  3,  2,  1, -1 },
            {  5,  7, -1,  0 },
            { -2,  3,  3,  1 },
            {  0,  2,  1,  5 }
    };

    double B[4] = { 1, 2, 3, 4 };

    // Initial guess for X and initialize Xnew with zeros.
    vector<double> X = { 1, 1, 1, 1 };
    vector<double> Xnew(n, 0);

    int iterlim = 100;
    double elim = 1e-6;
    double e = 1e20;
    int iter = 0;

    // Gauss-Seidel iteration loop.
    while ((e > elim) && (iter < iterlim)) {
        e = 0.0;

        for (int i = 0; i < n; i++) {
            double sum = B[i];

            // Update using the newest values for indices before i.
            for (int j = 0; j < i; j++) {
                sum -= A[i][j] * Xnew[j];
            }

            // Use the previous iteration's values for indices after i.
            for (int j = i + 1; j < n; j++) {
                sum -= A[i][j] * X[j];
            }

            // Compute the new value for X[i].
            Xnew[i] = sum / A[i][i];

            // Accumulate the error.
            e += fabs(Xnew[i] - X[i]);
        }

        // Update X with the new approximations.
        X = Xnew;
        iter++;

        // Output the current iteration and error.
        cout << "iter " << iter << " e = " << e << "\n";
        cout << "X = ";
        for (int i = 0; i < n; i++) {
            cout << X[i] << " ";
        }
        cout << "\n\n";
    }

    return 0;
}
