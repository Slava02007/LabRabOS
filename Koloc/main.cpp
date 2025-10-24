#include "Koloc.h"
#include <iostream>
#include <stdexcept>
using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");

    try {
        int choice;
        cout << "Добро пожаловать!" << endl;
        cout << "Выберите задачу:" << endl;
        cout << "1. Контейнер с первыми n факториалами." << endl;
        cout << "2. Контейнер без дубликатов." << endl;
        cout << "3. Развернуть связный список (рекурсией)." << endl;
        cout << "Ваш выбор: ";
        cin >> choice;
        if (!cin) throw runtime_error("Ошибка ввода");

        switch (choice) {
        case 1: {
            int n;
            cout << "Введите n: ";
            cin >> n;
            if (!cin) throw runtime_error("Ошибка ввода");

            auto factorials = Func1(n);
            cout << "Первые " << n << " факториалов: ";
            for (size_t i = 0; i < factorials.size(); ++i) {
                cout << factorials[i];
                if (i < factorials.size() - 1) cout << ", ";
            }
            cout << endl;
            break;
        }

        case 2: {
            int size;
            cout << "Введите количество элементов: ";
            cin >> size;
            if (size < 0 || !cin) throw runtime_error("Ошибка ввода");

            vector<int> input(size);
            cout << "Введите элементы: ";
            for (int i = 0; i < size; ++i) {
                cin >> input[i];
                if (!cin) throw runtime_error("Ошибка ввода");
            }

            auto result = removeDuplicates(input);
            cout << "Результат без дубликатов: ";
            for (size_t i = 0; i < result.size(); ++i) {
                cout << result[i];
                if (i < result.size() - 1) cout << ", ";
            }
            cout << endl;
            break;
        }

        case 3: {
            int size;
            cout << "Введите количество элементов списка: ";
            cin >> size;
            if (size <= 0 || !cin) throw runtime_error("Ошибка ввода");

            Node* head = nullptr;
            Node* tail = nullptr;

            cout << "Введите элементы списка: ";
            for (int i = 0; i < size; ++i) {
                int value;
                cin >> value;
                if (!cin) throw runtime_error("Ошибка ввода");

                Node* newNode = new Node(value);
                if (!head)
                    head = tail = newNode;
                else {
                    tail->next = newNode;
                    tail = newNode;
                }
            }

            cout << "Исходный список: ";
            printList(head);

            head = reverseListRecursive(head);

            cout << "Развернутый список: ";
            printList(head);

            freeList(head);
            break;
        }

        default:
            cout << "Неверный выбор!" << endl;
            break;
        }
    }
    catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }

    return 0;
}
