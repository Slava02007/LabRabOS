#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include "Message.h"

using namespace std;

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");

    if (argc < 3) return 1;
    string filename = argv[1];
    int id = stoi(argv[2]);

    HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, "FileMutex");
    HANDLE hFull = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "MessagesFull");
    HANDLE hEmpty = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, "MessagesEmpty");

    string eventName = "ReadyEvent" + to_string(id);
    HANDLE hReady = OpenEvent(EVENT_ALL_ACCESS, FALSE, eventName.c_str());
    SetEvent(hReady);

    cout << "--- Sender #" << id << " готов к работе ---\n";

    while (true) {
        cout << "\n1 - Отправить сообщение\n2 - Прекратить работу\n> ";
        int cmd;
        cin >> cmd;

        if (cmd == 2) break;
        if (cmd == 1) {
            cout << "Введите сообщение (до 20 символов): ";
            string text;
            cin >> text;
            if (text.length() > 20) text = text.substr(0, 20);

            cout << "Ожидание свободного места в файле..." << endl;
            WaitForSingleObject(hEmpty, INFINITE);
            WaitForSingleObject(hMutex, INFINITE);

            fstream file(filename, ios::binary | ios::in | ios::out);
            Header h;
            file.read(reinterpret_cast<char*>(&h), sizeof(h));

            file.seekp(sizeof(Header) + h.tail * sizeof(Message));
            Message msg;
            memset(msg.text, 0, 21);
            strcpy_s(msg.text, text.c_str());
            file.write(reinterpret_cast<char*>(&msg), sizeof(Message));

            h.tail = (h.tail + 1) % h.size;
            file.seekp(0);
            file.write(reinterpret_cast<char*>(&h), sizeof(h));
            file.close();

            ReleaseMutex(hMutex);
            ReleaseSemaphore(hFull, 1, NULL);
            cout << "Сообщение отправлено." << endl;
        }
    }

    CloseHandle(hMutex);
    CloseHandle(hFull);
    CloseHandle(hEmpty);
    CloseHandle(hReady);
    return 0;
}