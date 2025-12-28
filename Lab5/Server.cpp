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
    while (true) {
        Request req;
        DWORD bytesRead;
        if (!ReadFile(hPipe, &req, sizeof(Request), &bytesRead, NULL)) break;

        if (req.op == EXIT) break;

        fstream file(filename, ios::binary | ios::in | ios::out);
        employee emp;
        bool found = false;
        streampos pos;

        while (file.read((char*)&emp, sizeof(employee))) {
            if (emp.num == req.id) {
                req.data = emp;
                pos = (int)file.tellg() - sizeof(employee);
                found = true;
                break;
            }
        }

        if (!found) req.id = -1; 

        WriteFile(hPipe, &req, sizeof(Request), &bytesRead, NULL);

        if (found && req.op == MODIFY) {
            ReadFile(hPipe, &req, sizeof(Request), &bytesRead, NULL);
            file.clear();
            file.seekp(pos);
            file.write((char*)&req.data, sizeof(employee));
            cout << "Record " << req.id << " was updated." << endl;
        }
        file.close();
    }

    cout << "\nClient disconnected. Final file content:" << endl;
    printFile(filename);

    CloseHandle(hPipe);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    cout << "\nServer finished. Press any key to close..." << endl;
    system("pause");
    return 0;
}