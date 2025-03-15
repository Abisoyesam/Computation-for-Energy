#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;
int main() {
    // systeme variables
    const double R=0.8, L=0.011, k= 1.91, J= 0.2, a= 1E-2;
    double u=0, I=0, w=0, dI, Ce, dw, u0= 200, Cr0= 0, t = 0, dt = 0.0001, tf = 1; // 1 second
    ofstream file("data_plot.csv");
    if (file.is_open()) {
        file << "t \tI \tu \tw \tCe" << endl;
        while (t <= tf) {
            if (t>=0.01) // 10 ms power the DCM by a voltage step
                u=u0;
            dI = (u-R*I-k*w)/L;
            Ce = k*I;
            dw= (Ce-a*w-Cr0)/J;
            // integration rectangle method
            I = I + dI*dt;
            w = w + dw*dt;
            file << t << " \t" << I << " \t" << u;
            file << " \t" << w << " \t" << Ce << endl;
            t += dt;
        }
        file.close();
    } else cout << "Unable to open file";
}