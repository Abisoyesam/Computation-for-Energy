#include <iostream>

using namespace std;

int *b;
int N;

int main(){
    cout << "Enter any value: ";
    cin >> N;
    b = new int[N];

    for (int i = 0; i < N; i++){
        b[i] = i * 2;
        cout << b[i] << endl;
    }

    return 0;
}