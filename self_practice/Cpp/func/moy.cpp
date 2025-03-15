#include <iostream>
using namespace std;

double moy(double a, double b){
    double r;
    r = (a+b)/2.; // divide by 2 (int) will return an int
    return r;
}

int main(){
    double z;
    z = moy(18, 15);
    cout << "The result is " << z << endl;
}
