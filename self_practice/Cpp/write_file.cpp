#include <iostream>
#include <fstream>

using namespace std;

int main(){
    // Create a file 
    ofstream file ("data.csv");

    if (file.is_open()){
        file << "Name \tAge \tGender\n";
        file << "Abisoye \t 25 \t Male" << endl;
        file.close();
    } else {
        cout << "Unable to open file";
    }
    return 0;
}