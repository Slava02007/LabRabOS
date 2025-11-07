#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
#include <algorithm>

using namespace std;

struct Message {
    char text[21];
};

int main() {
    setlocale(LC_ALL, "Russian");

    string binfile;
    int n, m;

    cout << "Введите имя бинарного файла:" << endl;
    cin >> binfile;
    cout << "Введите количество записей в бинарном файле:" << endl;
    cin >> n;

    
    ofstream fout(binfile, ios::binary | ios::trunc);
    if (!fout) {
        cerr << "Ошибка создания бинарного файла!\n";
        return 1;
    }

    Message empty = { "" };
    for (int i = 0; i < n; i++)
        fout.write(reinterpret_cast<char*>(&empty), sizeof(Message));
    fout.close();

    cout << "Введите количество процессов Sender:" << endl;
    cin >> m;

    
    vector<HANDLE> senderEvents(m);
    for (int i = 0; i < m; i++) {
        string eventName = "SenderReady_" + to_string(i);
        senderEvents[i] = CreateEventA(NULL, TRUE, FALSE, eventName.c_str());
        if (!senderEvents[i]) {
            cerr << "Ошибка создания события " << eventName << endl;
            return 1;
        }
    }

   
    for (int i = 0; i < m; i++) {
        string cmd = "Sender.exe " + binfile + " " + to_string(i);
        wstring wCmd(cmd.begin(), cmd.end());

        STARTUPINFOW si = { sizeof(si) };
        PROCESS_INFORMATION pi;

        if (!CreateProcessW(
            NULL,
            &wCmd[0],
            NULL, NULL, FALSE,
            0, NULL, NULL,
            &si, &pi
        )) {
            cerr << "Ошибка запуска Sender процесса " << i << "\n";
            return 1;
        }

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

    cout << "Ожидание готовности всех Sender...\n";
    WaitForMultipleObjects(m, senderEvents.data(), TRUE, INFINITE);
    cout << "Все Sender готовы!\n";

    
    while (true) {
        cout << "\nКоманды:\n1 — прочитать сообщение\n2 — выход\n";
        int cmd;
        cin >> cmd;

        if (cmd == 2) {
            cout << "Завершение работы Receiver.\n";
            break;
        }
        else if (cmd == 1) {
            ifstream fin(binfile, ios::binary);
            if (!fin) {
                cerr << "Ошибка открытия бинарного файла!\n";
                continue;
            }

            Message msg;
            bool found = false;

           
            for (int i = 0; i < n; i++) {
                fin.read(reinterpret_cast<char*>(&msg), sizeof(Message));
                if (strlen(msg.text) > 0) {
                    cout << "Прочитано сообщение: " << msg.text << endl;
                    found = true;

                    
                    fin.close();
                    fstream fmod(binfile, ios::binary | ios::in | ios::out);
                    fmod.seekp(i * sizeof(Message));
                    Message emptyMsg = { "" };
                    fmod.write(reinterpret_cast<char*>(&emptyMsg), sizeof(Message));
                    fmod.close();
                    break;
                }
            }

            if (!found) {
                cout << "Файл пуст. Ожидание новых сообщений...\n";
                Sleep(2000); 
            }

            fin.close();
        }
        else {
            cout << "Неверная команда.\n";
        }
    }

   
    for (auto e : senderEvents)
        CloseHandle(e);

    return 0;
}
