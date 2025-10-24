#include "Koloc.h"
#include <iostream>
#include <stdexcept>
using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");

    try {
        int choice;
        cout << "����� ����������!" << endl;
        cout << "�������� ������:" << endl;
        cout << "1. ��������� � ������� n ������������." << endl;
        cout << "2. ��������� ��� ����������." << endl;
        cout << "3. ���������� ������� ������ (���������)." << endl;
        cout << "��� �����: ";
        cin >> choice;
        if (!cin) throw runtime_error("������ �����");

        switch (choice) {
        case 1: {
            int n;
            cout << "������� n: ";
            cin >> n;
            if (!cin) throw runtime_error("������ �����");

            auto factorials = Func1(n);
            cout << "������ " << n << " �����������: ";
            for (size_t i = 0; i < factorials.size(); ++i) {
                cout << factorials[i];
                if (i < factorials.size() - 1) cout << ", ";
            }
            cout << endl;
            break;
        }

        case 2: {
            int size;
            cout << "������� ���������� ���������: ";
            cin >> size;
            if (size < 0 || !cin) throw runtime_error("������ �����");

            vector<int> input(size);
            cout << "������� ��������: ";
            for (int i = 0; i < size; ++i) {
                cin >> input[i];
                if (!cin) throw runtime_error("������ �����");
            }

            auto result = removeDuplicates(input);
            cout << "��������� ��� ����������: ";
            for (size_t i = 0; i < result.size(); ++i) {
                cout << result[i];
                if (i < result.size() - 1) cout << ", ";
            }
            cout << endl;
            break;
        }

        case 3: {
            int size;
            cout << "������� ���������� ��������� ������: ";
            cin >> size;
            if (size <= 0 || !cin) throw runtime_error("������ �����");

            Node* head = nullptr;
            Node* tail = nullptr;

            cout << "������� �������� ������: ";
            for (int i = 0; i < size; ++i) {
                int value;
                cin >> value;
                if (!cin) throw runtime_error("������ �����");

                Node* newNode = new Node(value);
                if (!head)
                    head = tail = newNode;
                else {
                    tail->next = newNode;
                    tail = newNode;
                }
            }

            cout << "�������� ������: ";
            printList(head);

            head = reverseListRecursive(head);

            cout << "����������� ������: ";
            printList(head);

            freeList(head);
            break;
        }

        default:
            cout << "�������� �����!" << endl;
            break;
        }
    }
    catch (const exception& e) {
        cerr << "������: " << e.what() << endl;
        return 1;
    }

    return 0;
}
