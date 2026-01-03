#include <iostream>
#include <windows.h>
#include "employee.h"

using namespace std;

int main() {
    setlocale(LC_ALL, "rus");
    HANDLE hPipe = CreateFileA("\\\\.\\pipe\\WorkPipe", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (hPipe == INVALID_HANDLE_VALUE) {
        cerr << "Не удалось подключиться к каналу." << endl;
        system("pause");
        return 1;
    }

    while (true) {
        cout << "\n1. Чтение записи\n2. Модификация записи\n3. Выход\nВыбор: ";
        int choice; cin >> choice;

        Request req;
        if (choice == 3) {
            req.op = EXIT;
            DWORD written;
            WriteFile(hPipe, &req, sizeof(Request), &written, NULL);
            break;
        }

        cout << "Введите ID сотрудника: ";
        cin >> req.id;
        req.op = (choice == 1) ? READ : MODIFY;

        DWORD transferred;
        WriteFile(hPipe, &req, sizeof(Request), &transferred, NULL);
        ReadFile(hPipe, &req, sizeof(Request), &transferred, NULL);

        if (req.id == -1) {
            cout << "Сотрудник не найден!" << endl;
            continue;
        }

        cout << "Найдено: ID=" << req.data.num << ", Имя=" << req.data.name << ", Часы=" << req.data.hours << endl;

        if (choice == 2) {
            cout << "Введите новое имя: "; cin >> req.data.name;
            cout << "Введите новые часы: "; cin >> req.data.hours;
            req.op = SAVE;

            cout << "Нажмите ENTER для отправки изменений..." << endl;
            system("pause");

            WriteFile(hPipe, &req, sizeof(Request), &transferred, NULL);
            cout << "Данные обновлены на сервере." << endl;
        }
        else {
            cout << "Просмотр завершен. Нажмите любую клавишу..." << endl;
            system("pause");
        }
    }

    CloseHandle(hPipe);
    return 0;
}