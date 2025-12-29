#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct employee {
    int num;
    char name[10];
    double hours;
};

void runProcess(wstring cmdLine) {
    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    vector<wchar_t> cmdBuf(cmdLine.begin(), cmdLine.end());
    cmdBuf.push_back(0); 

    if (!CreateProcessW(
        NULL,
        cmdBuf.data(),
        NULL, NULL, FALSE,
        CREATE_NEW_CONSOLE, 
        NULL, NULL,
        &si, &pi
    )) {
        cerr << "Ошибка при запуске процесса! Код ошибки: " << GetLastError() << endl;
        return;
    }

    WaitForSingleObject(pi.hProcess, INFINITE); 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

int main() {
    setlocale(LC_ALL, "Russian");

    string binFile;
    int n;
    cout << "--- Подготовка данных ---" << endl;
    cout << "Введите имя бинарного файла: ";
    cin >> binFile;
    cout << "Введите количество сотрудников: ";
    cin >> n;

    wstring wCreatorCmd = L"Creator.exe " + wstring(binFile.begin(), binFile.end()) + L" " + to_wstring(n);
    runProcess(wCreatorCmd);

    cout << "\nСодержимое бинарного файла:" << endl;
    ifstream fin(binFile, ios::binary);
    if (!fin) {
        cerr << "Ошибка: не удалось открыть созданный файл " << binFile << endl;
    }
    else {
        employee emp;
        while (fin.read((char*)&emp, sizeof(employee))) {
            cout << "ID: " << emp.num << "\tИмя: " << emp.name << "\tЧасы: " << emp.hours << endl;
        }
        fin.close();
    }

    string reportFile;
    double pay;
    cout << "\n--- Формирование отчета ---" << endl;
    cout << "Введите имя файла отчета: ";
    cin >> reportFile;
    cout << "Введите оплату за час: ";
    cin >> pay;

    wstring wReporterCmd = L"Reporter.exe " + wstring(binFile.begin(), binFile.end()) +
        L" " + wstring(reportFile.begin(), reportFile.end()) +
        L" " + to_wstring(pay);
    runProcess(wReporterCmd);

    cout << "\nИтоговый отчет:" << endl;
    ifstream fr(reportFile);
    if (!fr) {
        cerr << "Ошибка: файл отчета не был создан!" << endl;
    }
    else {
        string line;
        while (getline(fr, line)) {
            cout << line << endl;
        }
        fr.close();
    }

    cout << "\nПрограмма Main завершила работу." << endl;
    system("pause");
    return 0;
}