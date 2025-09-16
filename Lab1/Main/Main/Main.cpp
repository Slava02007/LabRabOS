#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct employee {
    int num;
    char name[10];
    double hours;
};

int main() {
    setlocale(LC_ALL, "Russian");

    
    string binFile;
    int n;
    cout << "Введите имя бинарного файла: ";
    cin >> binFile;
    cout << "Введите количество сотрудников: ";
    cin >> n;

   
    string creatorCmd = "Creator.exe " + binFile + " " + to_string(n);
    wstring wCreatorCmd(creatorCmd.begin(), creatorCmd.end());

    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    if (!CreateProcessW(
        NULL,
        (LPWSTR)wCreatorCmd.c_str(), 
        NULL, NULL, FALSE,
        0, NULL, NULL,
        &si, &pi
    )) {
        cerr << "Ошибка запуска Creator\n";
        return 1;
    }

    
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

   
    cout << "\nСодержимое бинарного файла:\n";
    ifstream fin(binFile, ios::binary);
    if (!fin) {
        cerr << "Ошибка открытия бинарного файла!\n";
        return 1;
    }

    employee emp;
    while (fin.read((char*)&emp, sizeof(employee))) {
        cout << emp.num << "\t" << emp.name << "\t" << emp.hours << endl;
    }
    fin.close();

    
    string reportFile;
    double pay;
    cout << "\nВведите имя файла отчета: ";
    cin >> reportFile;
    cout << "Введите оплату за час: ";
    cin >> pay;

   
    string reporterCmd = "Reporter.exe " + binFile + " " + reportFile + " " + to_string(pay);
    wstring wReporterCmd(reporterCmd.begin(), reporterCmd.end());

    STARTUPINFOW si2 = { sizeof(si2) };
    PROCESS_INFORMATION pi2;

    if (!CreateProcessW(
        NULL,
        (LPWSTR)wReporterCmd.c_str(),
        NULL, NULL, FALSE,
        0, NULL, NULL,
        &si2, &pi2
    )) {
        cerr << "Ошибка запуска Reporter\n";
        return 1;
    }

   
    WaitForSingleObject(pi2.hProcess, INFINITE);
    CloseHandle(pi2.hProcess);
    CloseHandle(pi2.hThread);

   
    cout << "\nСодержимое отчета:\n";
    ifstream fr(reportFile);
    if (!fr) {
        cerr << "Ошибка открытия файла отчёта!\n";
        return 1;
    }

    string line;
    while (getline(fr, line)) {
        cout << line << endl;
    }
    fr.close();

    cout << "\nРабота программы завершена.\n";
    return 0;
}
