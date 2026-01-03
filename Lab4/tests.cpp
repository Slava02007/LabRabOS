#include <iostream>
#include <fstream>
#include <cassert>
#include <cstring>
#include "Message.h"

using namespace std;

struct Header {
    int head;
    int tail;
    int size;
};

void initQueue(const string& filename, int n) {
    ofstream fout(filename, ios::binary | ios::trunc);
    Header h{ 0, 0, n };
    fout.write(reinterpret_cast<char*>(&h), sizeof(h));
    Message empty = { "" };
    for (int i = 0; i < n; ++i)
        fout.write(reinterpret_cast<char*>(&empty), sizeof(Message));
    fout.close();
}

bool sendMessage(const string& filename, const string& text) {
    fstream file(filename, ios::binary | ios::in | ios::out);
    if (!file) return false;

    Header h;
    file.read(reinterpret_cast<char*>(&h), sizeof(h));

    int nextTail = (h.tail + 1) % h.size;
    if (nextTail == h.head) {
        file.close();
        return false; 
    }

    Message msg;
    strncpy(msg.text, text.c_str(), 20);
    msg.text[20] = '\0';

    file.seekp(sizeof(Header) + h.tail * sizeof(Message));
    file.write(reinterpret_cast<char*>(&msg), sizeof(Message));

    h.tail = nextTail;
    file.seekp(0);
    file.write(reinterpret_cast<char*>(&h), sizeof(h));
    file.close();
    return true;
}

bool receiveMessage(const string& filename, string& outText) {
    fstream file(filename, ios::binary | ios::in | ios::out);
    if (!file) return false;

    Header h;
    file.read(reinterpret_cast<char*>(&h), sizeof(h));

    if (h.head == h.tail) {
        file.close();
        return false; 
    }

    file.seekg(sizeof(Header) + h.head * sizeof(Message));
    Message msg;
    file.read(reinterpret_cast<char*>(&msg), sizeof(Message));
    outText = msg.text;

    Message empty = { "" };
    file.seekp(sizeof(Header) + h.head * sizeof(Message));
    file.write(reinterpret_cast<char*>(&empty), sizeof(Message));

    h.head = (h.head + 1) % h.size;
    file.seekp(0);
    file.write(reinterpret_cast<char*>(&h), sizeof(h));
    file.close();
    return true;
}

void testBasicSendReceive() {
    cout << "Тест 1: Отправка и получение сообщений..." << endl;
    const string file = "queue_test1.bin";
    initQueue(file, 3);

    assert(sendMessage(file, "Hello"));
    assert(sendMessage(file, "World"));

    string msg;
    assert(receiveMessage(file, msg));
    assert(msg == "Hello");

    assert(receiveMessage(file, msg));
    assert(msg == "World");

    cout << " Пройден.\n";
}

void testQueueFull() {
    cout << "Тест 2: Проверка переполнения очереди..." << endl;
    const string file = "queue_test2.bin";
    initQueue(file, 2);

    assert(sendMessage(file, "Msg1"));
    assert(!sendMessage(file, "Msg2")); 

    string msg;
    assert(receiveMessage(file, msg));
    assert(msg == "Msg1");
    assert(sendMessage(file, "Msg2"));
    assert(receiveMessage(file, msg));
    assert(msg == "Msg2");
    cout << " Пройден.\n";
}

void testQueueEmpty() {
    cout << "Тест 3: Проверка пустой очереди..." << endl;
    const string file = "queue_test3.bin";
    initQueue(file, 3);

    string msg;
    assert(!receiveMessage(file, msg)); 
    sendMessage(file, "One");
    assert(receiveMessage(file, msg));
    assert(msg == "One");
    assert(!receiveMessage(file, msg)); 
    cout << " Пройден.\n";
}

int main() {
    setlocale(LC_ALL, "rus");
    cout << "=== Запуск тестов очереди ===" << endl;
    testBasicSendReceive();
    testQueueFull();
    testQueueEmpty();
    cout << "\nВсе тесты успешно пройдены " << endl;
    return 0;
}
