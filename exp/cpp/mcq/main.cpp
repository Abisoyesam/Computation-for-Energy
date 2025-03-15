#include <iostream>
using namespace std;

double calc(int a, double b, int c){
    double r;
    r = (a + b) / c;
    return r;
}

int main(){
    cout << calc(2,5,2) << endl;
}