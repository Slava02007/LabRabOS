#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include "employee.h"

using namespace std;

void printFile(string filename) {
    ifstream in(filename, ios::binary);
    employee emp;
    cout << "\n--- Содержимое файла ---\n";
    while (in.read((char*)&emp, sizeof(employee))) {
        cout << "ID: " << emp.num << ", Имя: " << emp.name << ", Часы: " << emp.hours << endl;
    }
    in.close();
}

int main() {
    setlocale(LC_ALL, "rus");
    string filename;
    cout << "Введите имя файла: "; cin >> filename;

    ofstream out(filename, ios::binary);
    int count;
    cout << "Кол-во сотрудников: "; cin >> count;
    for (int i = 0; i < count; ++i) {
        employee emp;
        cout << "Введите ID, Имя, Часы: ";
        cin >> emp.num >> emp.name >> emp.hours;
        out.write((char*)&emp, sizeof(employee));
    }
    out.close();
    printFile(filename);

    string pipeName = "\\\\.\\pipe\\WorkPipe";
    HANDLE hPipe = CreateNamedPipeA(
        pipeName.c_str(),
        PIPE_ACCESS_DUPLEX, 
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, 
        1, sizeof(Request), sizeof(Request), 0, NULL);

    if (hPipe == INVALID_HANDLE_VALUE) return 1;

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    char cmd[] = "Client.exe";
    if (!CreateProcessA(NULL, cmd, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        cerr << "Ошибка запуска клиента!" << endl;
    }

    cout << "Ожидание подключения клиента..." << endl;
    ConnectNamedPipe(hPipe, NULL);

    while (true) {
        Request req;
        DWORD bytes;
        if (!ReadFile(hPipe, &req, sizeof(Request), &bytes, NULL)) break;
        if (req.op == EXIT) break;

        fstream file(filename, ios::binary | ios::in | ios::out);
        bool found = false;
        streampos pos;
        employee emp;

        while (file.read((char*)&emp, sizeof(employee))) {
            if (emp.num == req.id) {
                req.data = emp;
                pos = (int)file.tellg() - sizeof(employee);
                found = true;
                break;
            }
        }

        if (!found) req.id = -1;

        WriteFile(hPipe, &req, sizeof(Request), &bytes, NULL);

        if (found && req.op == MODIFY) {
            if (ReadFile(hPipe, &req, sizeof(Request), &bytes, NULL)) {
                file.clear();
                file.seekp(pos);
                file.write((char*)&req.data, sizeof(employee));
                cout << "Запись ID " << req.id << " обновлена." << endl;
            }
        }
        file.close();
    }

    cout << "\nФинальные данные в файле:";
    printFile(filename);

    CloseHandle(hPipe);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    system("pause");
    return 0;
}












//часть кода на более новом стандарте
/*#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <vector>
#include <filesystem> // C++17
#include <optional>   // C++17
#include "employee.h"

using namespace std;
using namespace filesystem; 

optional<pair<employee, streampos>> findEmployee(string filename, int id) {
    fstream file(filename, ios::binary | ios::in);
    if (!file) return nullopt;

    employee emp;
    while (file.read((char*)&emp, sizeof(employee))) {
        if (emp.num == id) {
            streampos pos = (int)file.tellg() - (int)sizeof(employee);
            return make_pair(emp, pos);
        }
    }
    return nullopt;
}

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
    path p(filename);
    if (!exists(p)) {
        cout << "File not found!" << endl;
        return;
    }

    cout << "\n--- File: " << absolute(p) << " ---" << endl;

    ifstream in(filename, ios::binary);
    employee emp;
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
    HANDLE hPipe = CreateNamedPipeA(pipeName.c_str(), PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, 1,
        sizeof(Request), sizeof(Request), 0, NULL);

    if (hPipe == INVALID_HANDLE_VALUE) return 1;

    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    char cmd[] = "Client.exe";
    CreateProcessA(NULL, cmd, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);

    cout << "Waiting for client..." << endl;
    ConnectNamedPipe(hPipe, NULL);

    while (true) {
        Request req;
        DWORD bytes;
        if (!ReadFile(hPipe, &req, sizeof(Request), &bytes, NULL)) break;
        if (req.op == EXIT) break;

        auto result = findEmployee(filename, req.id);

        if (result.has_value()) {
            auto [foundEmp, pos] = result.value(); 
            req.data = foundEmp;

            WriteFile(hPipe, &req, sizeof(Request), &bytes, NULL);

            if (req.op == MODIFY) {
                ReadFile(hPipe, &req, sizeof(Request), &bytes, NULL);

                fstream file(filename, ios::binary | ios::in | ios::out);
                file.seekp(pos);
                file.write((char*)&req.data, sizeof(employee));
                file.close();
                cout << "Record " << req.id << " updated in file." << endl;
            }
        }
        else {
            req.id = -1; 
            WriteFile(hPipe, &req, sizeof(Request), &bytes, NULL);
        }
    }

    cout << "\nWork finished. Final data:" << endl;
    printFile(filename);

    CloseHandle(hPipe);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    system("pause");
    return 0;
}
*/