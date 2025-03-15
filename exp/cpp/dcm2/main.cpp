#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;

int main() {
//  initialization

    double tf = 0.4; // final simulation time
    double dt = 0.0001; // time step
    int tk = 0; // array indice
    int Ntf = floor(tf/dt);

//    System variables
    const double R = 0.8, L = 0.011, k = 1.91;
    double J = 0.2; double a = 1e-2; double u0 = 8, Cr0 = 0;
    double u = 0, I = 0, w = 0, t = 0;

//    Initialization
    vector<double> tt(Ntf,0.0); vector<double> ut(Ntf,0.0);
    vector<double> It(Ntf,0.0); vector<double> wt(Ntf,0.0);
    vector<double> Cet(Ntf,0.0);

    ofstream file("data_plot.csv");
    if (file.is_open()){
        file << "tt \t wt \t It \t Cet \t ut" << endl;

        while(t < tf){
            if (t >= 0.01){
                u = u0;
            }

            double dI = (u-R * I - k*w)/L;
            double Ce = k*I; double dw = 0;

            // integration rectangle method
            I = I + dI*dt;
            w = w + dw*dt;

            // Euler integration (rectangle method)
            tt[tk] = t; It[tk] = I; ut[tk] = u;
            wt[tk] = w; Cet[tk] = Ce;

            file << tt[tk] << " \t" << wt[tk] << " \t" << It[tk] << Cet[tk] << " \t" << ut[tk] << " \t" << endl;
            t=t+dt; tk=tk+1; // array indice
        }
        file.close();
        cout << "Program has successfully run!!!!";
    } else {
       cout << "Unable to open file";
    }

    return 0;
}
