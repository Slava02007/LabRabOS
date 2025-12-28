#include <iostream>
#include <windows.h>
#include "employee.h"

using namespace std;

int main() {
    HANDLE hPipe = CreateFileA("\\\\.\\pipe\\WorkPipe", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (hPipe == INVALID_HANDLE_VALUE) {
        cerr << "Connection to pipe failed." << endl;
        return 1;
    }

    while (true) {
        cout << "\n1. Read record\n2. Modify record\n3. Exit\nChoice: ";
        int choice; cin >> choice;

        Request req;
        if (choice == 3) {
            req.op = EXIT;
            DWORD written;
            WriteFile(hPipe, &req, sizeof(Request), &written, NULL);
            break;
        }

        cout << "Enter Employee ID: ";
        cin >> req.id;
        req.op = (choice == 1) ? READ : MODIFY;

        DWORD transferred;
        WriteFile(hPipe, &req, sizeof(Request), &transferred, NULL);
        ReadFile(hPipe, &req, sizeof(Request), &transferred, NULL);

        if (req.id == -1) {
            cout << "Employee not found!" << endl;
            continue;
        }

        cout << "Data: ID=" << req.data.num << ", Name=" << req.data.name << ", Hours=" << req.data.hours << endl;

        if (choice == 2) {
            cout << "Enter new Name and Hours: ";
            cin >> req.data.name >> req.data.hours;
            req.op = SAVE;

            cout << "Press any key to send update to server..." << endl;
            system("pause");

            WriteFile(hPipe, &req, sizeof(Request), &transferred, NULL);
            cout << "Update sent." << endl;
        }
        else {
            cout << "Press any key to finish reading..." << endl;
            system("pause");
        }
    }

    CloseHandle(hPipe);
    return 0;
}