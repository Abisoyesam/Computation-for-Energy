#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>

using namespace std;

int main() {
    // Simulation parameters
    double tf = 0.5;       // final simulation time
    double dt = 0.0001;    // time step
    int Ntf = static_cast<int>(floor(tf / dt)); // total number of steps

    // System variables
    double R = 0.8;
    double L = 0.011;
    double k = 1.91;
    double J = 0.2;
    double a = 1E-2;
    double u = 0, I = 0, w = 0, t = 0;
    double u0 = 8, Cr0 = 0;

    // Initialize vectors to store simulation results
    // In MATLAB: tt = zeros(1, Ntf); etc.
    vector<double> tt(Ntf, 0.0);
    vector<double> It(Ntf, 0.0);
    vector<double> ut(Ntf, 0.0);
    vector<double> wt(Ntf, 0.0);
    vector<double> Cet(Ntf, 0.0);

    int tk = 0; // array index (C++ uses 0-based indexing)

    // Simulation loop (replicates MATLAB's while(t < tf))
    while (t < tf && tk < Ntf) {
        // Apply a voltage step after 10 ms
        if (t >= 0.01) {
            u = u0;
        }

        // Compute derivative of current (dI)
        double dI = (u - R * I - k * w) / L;
        double Ce = k * I;
        double dw = 0; // As given; alternatively: (Ce - a*w - Cr0)/J

        // Euler integration (rectangle method)
        I = I + dI * dt;
        w = w + dw * dt;

        // Save simulation results into arrays
        tt[tk] = t;
        It[tk] = I;
        ut[tk] = u;
        wt[tk] = w;
        Cet[tk] = Ce;

        // Update time and index
        t += dt;
        tk++;
    }

    // Create and write to a CSV file (columns: Time, AngularSpeed, Current)
    ofstream csvFile("simulation_data.csv");
    if (!csvFile.is_open()) {
        cerr << "Error: Unable to open file for writing." << endl;
        return 1;
    }

    // Write header line
    csvFile << "Time,AngularSpeed,Current\n";
    for (int i = 0; i < tk; i++) {
        csvFile << tt[i] << " \t" << wt[i] << " \t" << It[i] << endl;
    }
    csvFile.close();

    cout << "Simulation data saved to simulation_data.csv" << endl;
    return 0;
}
