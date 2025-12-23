#include <iostream>
#include <fstream>
#include <windows.h>
#include <vector>
#include <map>
#include "Common.h"

using namespace std;

const char* FILE_NAME = "employees.bin";

// Структура для хранения информации о блокировках записей
struct RecordLock {
    bool writeLocked;
    int readLocks;
    HANDLE hMutex;  // Мьютекс для синхронизации доступа к этой записи
};

map<int, RecordLock> recordLocks;  // Карта блокировок по ID записи
HANDLE globalMutex;  // Глобальный мьютекс для синхронизации доступа к карте блокировок

void CreateFile() {
    int count;
    cout << "Enter number of employees: ";
    cin >> count;
    cin.ignore();  // Очистка буфера после ввода числа

    ofstream fout(FILE_NAME, ios::binary | ios::trunc);
    for (int i = 0; i < count; ++i) {
        employee emp;
        emp.num = i + 1;
        cout << "\nEmployee ID " << emp.num << endl;
        cout << "Name: ";
        cin.getline(emp.name, 10);
        cout << "Hours: ";
        cin >> emp.hours;
        cin.ignore();  // Очистка буфера после ввода числа
        fout.write((char*)&emp, sizeof(employee));
    }
    fout.close();
}

void DisplayFile() {
    cout << "\n=== File Content ===" << endl;
    ifstream fin(FILE_NAME, ios::binary);
    employee emp;
    while (fin.read((char*)&emp, sizeof(employee))) {
        cout << "ID: " << emp.num << "\tName: " << emp.name
            << "\tHours: " << emp.hours << endl;
    }
    fin.close();
    cout << "====================\n" << endl;
}

bool FindRecord(int id, employee& outEmp) {
    ifstream fin(FILE_NAME, ios::binary);
    employee temp;
    while (fin.read((char*)&temp, sizeof(employee))) {
        if (temp.num == id) {
            outEmp = temp;
            fin.close();
            return true;
        }
    }
    fin.close();
    return false;
}

void UpdateRecord(const employee& newEmp) {
    fstream f(FILE_NAME, ios::binary | ios::in | ios::out);
    employee temp;
    while (f.read((char*)&temp, sizeof(employee))) {
        if (temp.num == newEmp.num) {
            long pos = (long)f.tellg() - sizeof(employee);
            f.seekp(pos);
            f.write((char*)&newEmp, sizeof(employee));
            f.flush();
            break;
        }
    }
    f.close();
}

// Функции для работы с блокировками
bool AcquireReadLock(int id) {
    WaitForSingleObject(globalMutex, INFINITE);

    RecordLock& lock = recordLocks[id];
    if (lock.writeLocked) {
        ReleaseMutex(globalMutex);
        return false;  // Запись заблокирована на запись
    }

    lock.readLocks++;
    ReleaseMutex(globalMutex);
    return true;
}

bool AcquireWriteLock(int id) {
    WaitForSingleObject(globalMutex, INFINITE);

    RecordLock& lock = recordLocks[id];
    if (lock.writeLocked || lock.readLocks > 0) {
        ReleaseMutex(globalMutex);
        return false;  // Запись заблокирована
    }

    lock.writeLocked = true;
    ReleaseMutex(globalMutex);
    return true;
}

void ReleaseReadLock(int id) {
    WaitForSingleObject(globalMutex, INFINITE);

    RecordLock& lock = recordLocks[id];
    lock.readLocks--;
    if (lock.readLocks < 0) lock.readLocks = 0;

    ReleaseMutex(globalMutex);
}

void ReleaseWriteLock(int id) {
    WaitForSingleObject(globalMutex, INFINITE);

    RecordLock& lock = recordLocks[id];
    lock.writeLocked = false;

    ReleaseMutex(globalMutex);
}

void InitializeLocks() {
    // Создаем мьютекс для синхронизации доступа к карте блокировок
    globalMutex = CreateMutex(NULL, FALSE, NULL);

    // Инициализируем блокировки для всех записей в файле
    ifstream fin(FILE_NAME, ios::binary);
    employee emp;
    while (fin.read((char*)&emp, sizeof(employee))) {
        RecordLock lock;
        lock.writeLocked = false;
        lock.readLocks = 0;
        recordLocks[emp.num] = lock;
    }
    fin.close();
}

void CleanupLocks() {
    recordLocks.clear();
    CloseHandle(globalMutex);
}

void ProcessClientRequest(HANDLE hPipe) {
    Request req;
    Response resp;
    DWORD bytesRead, bytesWritten;

    while (true) {
        // Читаем запрос от клиента
        if (!ReadFile(hPipe, &req, sizeof(Request), &bytesRead, NULL)) {
            cout << "Client disconnected or error reading." << endl;
            break;
        }

        cout << "\nReceived command: ";
        switch (req.cmd) {
        case CMD_CHECK_ID:
            cout << "READ record ID " << req.id << endl;
            break;
        case CMD_UPDATE:
            cout << "UPDATE record ID " << req.id << endl;
            break;
        case CMD_END_ACCESS:
            cout << "END ACCESS to record ID " << req.id << endl;
            break;
        case CMD_EXIT:
            cout << "EXIT" << endl;
            break;
        }

        if (req.cmd == CMD_EXIT) {
            cout << "Exit command received. Closing connection." << endl;
            break;
        }

        resp.found = false;

        if (req.cmd == CMD_CHECK_ID) {
            // Запрос на чтение записи
            if (AcquireReadLock(req.id)) {
                if (FindRecord(req.id, resp.data)) {
                    resp.found = true;
                    cout << "Record found and locked for reading." << endl;
                }
                else {
                    ReleaseReadLock(req.id);  // Если запись не найдена, снимаем блокировку
                    cout << "Record not found." << endl;
                }
            }
            else {
                cout << "Record is write-locked. Cannot read." << endl;
            }

            // Отправляем ответ клиенту
            WriteFile(hPipe, &resp, sizeof(Response), &bytesWritten, NULL);
        }
        else if (req.cmd == CMD_UPDATE) {
            // Запрос на обновление записи
            if (AcquireWriteLock(req.id)) {
                cout << "Record locked for writing. Updating..." << endl;
                UpdateRecord(req.data);

                // Подтверждаем успешное обновление
                resp.found = true;
                resp.data = req.data;
                WriteFile(hPipe, &resp, sizeof(Response), &bytesWritten, NULL);
            }
            else {
                cout << "Record is locked. Cannot update now." << endl;
                resp.found = false;
                WriteFile(hPipe, &resp, sizeof(Response), &bytesWritten, NULL);
            }
        }
        else if (req.cmd == CMD_END_ACCESS) {
            // Завершение доступа к записи
            ReleaseWriteLock(req.id);
            ReleaseReadLock(req.id);
            cout << "Access to record " << req.id << " released." << endl;
        }
    }
}

int main() {
    // Создание и отображение файла
    CreateFile();
    DisplayFile();

    // Инициализация системы блокировок
    InitializeLocks();

    // Создание именованного канала
    HANDLE hPipe = CreateNamedPipeW(
        PIPE_NAME,
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
        PIPE_UNLIMITED_INSTANCES,
        1024, 1024, 0, NULL
    );

    if (hPipe == INVALID_HANDLE_VALUE) {
        cerr << "Error creating pipe: " << GetLastError() << endl;
        return 1;
    }

    cout << "Server started. Waiting for client connections..." << endl;

    // Ожидание подключения клиента
    if (!ConnectNamedPipe(hPipe, NULL)) {
        cerr << "Error connecting pipe: " << GetLastError() << endl;
        CloseHandle(hPipe);
        return 1;
    }

    cout << "Client connected!" << endl;

    // Обработка запросов клиента
    ProcessClientRequest(hPipe);

    // Завершение работы
    DisconnectNamedPipe(hPipe);
    CloseHandle(hPipe);
    CleanupLocks();

    // Вывод конечного состояния файла
    cout << "\n=== Final file content ===" << endl;
    DisplayFile();

    cout << "Press Enter to exit..." << endl;
    cin.get();

    return 0;
}