#include <iostream>
#include <windows.h>
#include <string>
#include "Common.h"

using namespace std;

void ConnectToServer(HANDLE& hPipe) {
    while (true) {
        hPipe = CreateFileW(
            PIPE_NAME,
            GENERIC_READ | GENERIC_WRITE,
            0, NULL, OPEN_EXISTING, 0, NULL
        );

        if (hPipe != INVALID_HANDLE_VALUE) {
            cout << "Connected to server!" << endl;
            break;
        }

        if (GetLastError() != ERROR_PIPE_BUSY) {
            cerr << "Failed to connect to server. Error: " << GetLastError() << endl;
            return;
        }

        // Если канал занят, ждем и пробуем снова
        cout << "Waiting for server..." << endl;
        Sleep(1000);
    }

    // Устанавливаем режим чтения сообщений
    DWORD mode = PIPE_READMODE_MESSAGE;
    SetNamedPipeHandleState(hPipe, &mode, NULL, NULL);
}

void ReadRecord(HANDLE hPipe) {
    Request req;
    Response resp;
    DWORD bytesWritten, bytesRead;

    cout << "Enter employee ID to read: ";
    cin >> req.id;
    cin.ignore();  // Очистка буфера

    req.cmd = CMD_CHECK_ID;

    // Отправляем запрос на чтение
    if (!WriteFile(hPipe, &req, sizeof(Request), &bytesWritten, NULL)) {
        cerr << "Error sending request: " << GetLastError() << endl;
        return;
    }

    // Получаем ответ
    if (!ReadFile(hPipe, &resp, sizeof(Response), &bytesRead, NULL)) {
        cerr << "Error reading response: " << GetLastError() << endl;
        return;
    }

    if (resp.found) {
        cout << "\n=== Employee Record ===" << endl;
        cout << "ID: " << resp.data.num << endl;
        cout << "Name: " << resp.data.name << endl;
        cout << "Hours: " << resp.data.hours << endl;
        cout << "=====================\n" << endl;

        // Завершаем доступ к записи
        char choice;
        cout << "Press 'c' to continue: ";
        cin >> choice;
        cin.ignore();

        req.cmd = CMD_END_ACCESS;
        WriteFile(hPipe, &req, sizeof(Request), &bytesWritten, NULL);
    }
    else {
        cout << "Record not found or access denied!" << endl;
    }
}

void UpdateRecord(HANDLE hPipe) {
    Request req;
    Response resp;
    DWORD bytesWritten, bytesRead;

    cout << "Enter employee ID to update: ";
    cin >> req.id;
    cin.ignore();  // Очистка буфера

    // Сначала читаем текущую запись
    req.cmd = CMD_CHECK_ID;
    WriteFile(hPipe, &req, sizeof(Request), &bytesWritten, NULL);
    ReadFile(hPipe, &resp, sizeof(Response), &bytesRead, NULL);

    if (!resp.found) {
        cout << "Cannot update: record not found or locked!" << endl;
        return;
    }

    // Показываем текущие данные
    cout << "\nCurrent data:" << endl;
    cout << "ID: " << resp.data.num << endl;
    cout << "Name: " << resp.data.name << endl;
    cout << "Hours: " << resp.data.hours << endl;

    // Запрашиваем новые данные
    employee newEmp = resp.data;
    cout << "\n Enter new name (or press Enter to keep current): ";
    string name;
    getline(cin, name);
    if (!name.empty() && name.length() < 10) {
        strcpy_s(newEmp.name, name.c_str());
    }

    cout << "Enter new hours (or -1 to keep current): ";
    double hours;
    cin >> hours;
    cin.ignore();
    if (hours >= 0) {
        newEmp.hours = hours;
    }

    // Отправляем запрос на обновление
    req.cmd = CMD_UPDATE;
    req.data = newEmp;
    WriteFile(hPipe, &req, sizeof(Request), &bytesWritten, NULL);

    // Получаем подтверждение
    ReadFile(hPipe, &resp, sizeof(Response), &bytesRead, NULL);

    if (resp.found) {
        cout << "Record updated successfully!" << endl;

        // Завершаем доступ к записи
        char choice;
        cout << "Press 'c' to continue: ";
        cin >> choice;
        cin.ignore();

        req.cmd = CMD_END_ACCESS;
        WriteFile(hPipe, &req, sizeof(Request), &bytesWritten, NULL);
    }
    else {
        cout << "Update failed: record is locked!" << endl;
    }
}

int main() {
    HANDLE hPipe = INVALID_HANDLE_VALUE;

    // Подключение к серверу
    ConnectToServer(hPipe);
    if (hPipe == INVALID_HANDLE_VALUE) {
        return 1;
    }

    // Главный цикл клиента
    while (true) {
        cout << "\n=== Employee Database Client ===" << endl;
        cout << "1. Read employee record" << endl;
        cout << "2. Update employee record" << endl;
        cout << "3. Exit" << endl;
        cout << "Select option: ";

        int choice;
        cin >> choice;
        cin.ignore();  // Очистка буфера

        switch (choice) {
        case 1:
            ReadRecord(hPipe);
            break;
        case 2:
            UpdateRecord(hPipe);
            break;
        case 3:
        {
            Request req;
            req.cmd = CMD_EXIT;
            DWORD bytesWritten;
            WriteFile(hPipe, &req, sizeof(Request), &bytesWritten, NULL);
            cout << "Exiting..." << endl;
        }
        CloseHandle(hPipe);
        return 0;
        default:
            cout << "Invalid option!" << endl;
        }
    }

    CloseHandle(hPipe);
    return 0;
}