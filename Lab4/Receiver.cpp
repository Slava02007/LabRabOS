#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <vector>
#include "Message.h"

using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");

    string filename;
    int queueSize, senderCount;

    cout << "--- Receiver Process ---\n";
    cout << "Введите имя бинарного файла: ";
    cin >> filename;
    cout << "Введите количество записей в очереди: ";
    cin >> queueSize;
    cout << "Введите количество процессов Sender: ";
    cin >> senderCount;

    {
        ofstream fout(filename, ios::binary | ios::trunc);
        Header h{ 0, 0, queueSize };
        fout.write(reinterpret_cast<char*>(&h), sizeof(h));
        Message empty = { "" };
        for (int i = 0; i < queueSize; ++i)
            fout.write(reinterpret_cast<char*>(&empty), sizeof(Message));
    }

    HANDLE hMutex = CreateMutex(NULL, FALSE, "FileMutex");
    HANDLE hFull = CreateSemaphore(NULL, 0, queueSize, "MessagesFull");
    HANDLE hEmpty = CreateSemaphore(NULL, queueSize, queueSize, "MessagesEmpty");

    vector<HANDLE> hReadyEvents(senderCount);
    vector<PROCESS_INFORMATION> pi(senderCount);

    for (int i = 0; i < senderCount; ++i) {
        STARTUPINFO si = { sizeof(si) };
        string cmd = "Sender.exe " + filename + " " + to_string(i);

        if (!CreateProcess(NULL, &cmd[0], NULL, NULL, FALSE,
            CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi[i])) {
            cout << "Ошибка запуска Sender #" << i << endl;
        }

        string eventName = "ReadyEvent" + to_string(i);
        hReadyEvents[i] = CreateEvent(NULL, TRUE, FALSE, eventName.c_str());
    }

    cout << "Ожидание готовности всех Sender-ов..." << endl;
    WaitForMultipleObjects(senderCount, hReadyEvents.data(), TRUE, INFINITE);
    cout << "Все Sender-ы запущены. Можно работать.\n";

    while (true) {
        cout << "\n1 - Прочитать сообщение\n2 - Завершить работу\n> ";
        int cmd;
        cin >> cmd;

        if (cmd == 2) break;
        if (cmd == 1) {
            cout << "Ожидание сообщения из файла..." << endl;

            WaitForSingleObject(hFull, INFINITE);
            WaitForSingleObject(hMutex, INFINITE);

            fstream file(filename, ios::binary | ios::in | ios::out);
            Header h;
            file.read(reinterpret_cast<char*>(&h), sizeof(h));

            file.seekg(sizeof(Header) + h.head * sizeof(Message));
            Message msg;
            file.read(reinterpret_cast<char*>(&msg), sizeof(Message));
            cout << "Получено сообщение: " << msg.text << endl;

            h.head = (h.head + 1) % h.size;
            file.seekp(0);
            file.write(reinterpret_cast<char*>(&h), sizeof(h));
            file.close();

            ReleaseMutex(hMutex);
            ReleaseSemaphore(hEmpty, 1, NULL);
        }
    }

    for (int i = 0; i < senderCount; i++) {
        CloseHandle(pi[i].hProcess);
        CloseHandle(pi[i].hThread);
        CloseHandle(hReadyEvents[i]);
    }
    CloseHandle(hMutex);
    CloseHandle(hFull);
    CloseHandle(hEmpty);

    return 0;
}