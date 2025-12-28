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

    string pipeName = "\\\\.\\pipe\\WorkPipe";
    HANDLE hPipe = CreateNamedPipeA(
        pipeName.c_str(),
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        1, sizeof(Request), sizeof(Request), 0, NULL);

    if (hPipe == INVALID_HANDLE_VALUE) {
        cerr << "Pipe creation failed. Error: " << GetLastError() << endl;
        return 1;
    }

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    char cmd[] = "client.exe";

    if (!CreateProcessA(NULL, cmd, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        cerr << "Client launch failed. Error: " << GetLastError() << endl;
    }

    cout << "Waiting for client connection..." << endl;
    ConnectNamedPipe(hPipe, NULL);
    cout << "Client connected." << endl;

    return 0;
}