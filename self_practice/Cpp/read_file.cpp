#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(){
    string line; 
    
    ifstream file ("data.csv");
    if (file.is_open()){
        while (getline(file, line)){
            cout << line << endl;
        }
        file.close();
    } else {
        cout << "Unable to read file" << endl;
    }
    return 0;
}