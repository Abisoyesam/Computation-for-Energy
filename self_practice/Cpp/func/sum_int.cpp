#include <iostream>

using namespace std;

int sum = 0;
int N = 10;

int main(){
    for (int i = 0; i < N; i++){
        cout << sum << endl;
        sum = sum + i;
    }
    cout << "Sum of first " << N << " integers is " << sum << endl;
    return 0;
}