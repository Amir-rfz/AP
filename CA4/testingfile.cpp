#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
int main(int argc, char *argv[]) {
    ifstream file(argv[1]); 
    string header;
    getline(file, header);
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string id_str, price_str;
        getline(iss, id_str, ',');
        getline(iss, price_str, ',');
        int price = stoi(price_str);
        int id = stoi(id_str);
        cout << price <<"||"<<id<< endl;
    }

    return 0;
}
