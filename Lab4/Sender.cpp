#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

using namespace std;

struct Message {
    char text[21];
};

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");

    if (argc < 3) {
        cerr << "Использование: Sender.exe <имя_файла> <номер_процесса>\n";
        return 1;
    }

    string binfile = argv[1];
    int senderId = stoi(argv[2]);

    
    string eventName = "SenderReady_" + to_string(senderId);

    
    HANDLE readyEvent = OpenEventA(EVENT_MODIFY_STATE, FALSE, eventName.c_str());
    if (!readyEvent) {
        cerr << "Ошибка открытия события " << eventName << "!\n";
        return 1;
    }

   
    fstream file(binfile, ios::binary | ios::in | ios::out);
    if (!file) {
        cerr << "Ошибка открытия бинарного файла!\n";
        CloseHandle(readyEvent);
        return 1;
    }

    
    SetEvent(readyEvent);
    cout << "Sender #" << senderId << " готов к работе.\n";

    while (true) {
        cout << "\nКоманды:\n1 — отправить сообщение\n2 — выход\n";
        int cmd;
        cin >> cmd;

        if (cmd == 2) {
            cout << "Завершение работы Sender #" << senderId << endl;
            break;
        }
        else if (cmd == 1) {
            cout << "Введите сообщение (до 20 символов): ";
            string text;
            cin.ignore();
            getline(cin, text);

            if (text.length() > 20) {
                cout << "Сообщение слишком длинное! Будет обрезано до 20 символов.\n";
                text = text.substr(0, 20);
            }

            bool written = false;

            while (!written) {
                file.clear();
                file.seekg(0, ios::beg);

                for (int i = 0;; i++) {
                    Message msg;
                    file.read(reinterpret_cast<char*>(&msg), sizeof(Message));

                    if (file.eof()) break;

                    if (strlen(msg.text) == 0) {
                        file.seekp(i * sizeof(Message), ios::beg);
                        Message newMsg;
                        strcpy_s(newMsg.text, text.c_str());
                        file.write(reinterpret_cast<char*>(&newMsg), sizeof(Message));
                        file.flush();
                        cout << "Сообщение записано в ячейку #" << i << endl;
                        written = true;
                        break;
                    }
                }

                if (!written) {
                    cout << "Файл заполнен, ожидание освобождения...\n";
                    Sleep(2000);
                }
            }
        }
        else {
            cout << "Неверная команда.\n";
        }
    }

    file.close();
    CloseHandle(readyEvent);
    return 0;
}
