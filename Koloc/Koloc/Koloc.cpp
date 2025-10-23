#include "Koloc.h"
#include "vector"
#include <iostream>
#include <vector>
#include <unordered_set>
#include <list>
using namespace std;


vector<long long> Func1(int n) {
    vector<long long> result;
    if (n <= 0) return result;

    result.reserve(n);
    long long factorial = 1;

    for (int i = 1; i <= n; i++) {
        factorial *= i;
        result.push_back(factorial);
    }

    return result;
}

vector<int> removeDuplicates(const vector<int>& input) {
    vector<int> result;
    unordered_set<int> seen;

    for (int num : input) {
        if (seen.find(num) == seen.end()) {
            seen.insert(num);
            result.push_back(num);
        }
    }

    return result;
}



int main() {
    setlocale(LC_ALL, "Rus");
        int choice;
        cout << "Добро пожаловать!" << endl;
        cout << "Выберите задачу:" << endl;
        cout << "1. На вход натуральное число n. На выход контейнер с первыми n факториалами." << endl;
        cout << "2. На вход – контейнер(массив) с числами. На выход контейнер, с неизменным списком элементов, но без дубликатов." << endl;
        cout << "3. Развернуть связный список используя рекурсию." << endl;
        cout << "Ваш выбор: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            int n;
            cout << "Введите n: ";
            cin >> n;
            vector<long long> factorials = Func1(n);

            cout << "Первые " << n << " факториалов: ";
            for (size_t i = 0; i < factorials.size(); i++) {
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

            vector<int> input(size);
            cout << "Введите элементы: ";
            for (int i = 0; i < size; i++) {
                cin >> input[i];
            }

            vector<int> result = removeDuplicates(input);

            cout << "Результат без дубликатов: ";
            for (size_t i = 0; i < result.size(); i++) {
                cout << result[i];
                if (i < result.size() - 1) cout << ", ";
            }
            cout << endl;
            break;
        }
    case(3):


    default:
        cout << "Неверный выбор!" << endl;
        break;
    }
        return 0;
}
