#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

int main() {
    double x, z,
            t = 0, dt = 0.01, tf = 1; // 1 second
    ofstream myfile("dataplot.csv");
    if (myfile.is_open()) {
        myfile << "t \tx \tz" << endl;
        while (t <= tf) {
            x = sin(10 * t);
            z = 10 * t; // helicoidal motion
            myfile << t << " \t" << x << " \t" << z << endl;
            t += dt;
        }
        myfile.close();
    } else cout << "Unable to open file";
}