#include <iostream>
#include <fstream>
#include <windows.h>
#include <vector>
#include <string>
#include "employee.h"

using namespace std;

void createBinaryFile(string filename) {
    ofstream out(filename, ios::binary);
    int count;
    cout << "Enter number of employees: ";
    cin >> count;

    for (int i = 0; i < count; ++i) {
        employee emp;
        cout << "Enter ID, Name, Hours: ";
        cin >> emp.num >> emp.name >> emp.hours;
        out.write((char*)&emp, sizeof(employee));
    }
    out.close();
}

void printFile(string filename) {
    ifstream in(filename, ios::binary);
    employee emp;
    cout << "\n--- File content ---\n";
    while (in.read((char*)&emp, sizeof(employee))) {
        cout << "ID: " << emp.num << ", Name: " << emp.name << ", Hours: " << emp.hours << endl;
    }
    in.close();
}

int main() {
    string filename;
    cout << "Enter filename: ";
    cin >> filename;
    createBinaryFile(filename);
    printFile(filename);

    
    return 0;
}