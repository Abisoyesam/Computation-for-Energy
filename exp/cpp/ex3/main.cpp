#include <iostream>
using namespace std;

int main() {
 int i, N = 10;
 int sum = 0;
 double moy;

 for (i = 0; i < N; i++) {
 sum += i;
 }

 moy = (double)sum / N;
 cout << "Sum = " << sum << endl
 << "Moy = " << moy << endl;
 return 0;
}