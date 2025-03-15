#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

double x, z, t = 0;
double dt = 0.01, tf = 1;

int main(){
    ofstream file ("dataplot.csv");
    if (file.is_open()){
        file << "t \tx \tz" << endl;

        while (t < tf){
            x = sin(10*t);
            z = 10*t;
            file << t << " \t" << x << " \t" << z << endl;
            t += dt;
        }
        file.close();
        cout << "Data has been generated!" << endl;
    } else {
        cout << "Unable to open file" << endl;
    }
    return 0;
}