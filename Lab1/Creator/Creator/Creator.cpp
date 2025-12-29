#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct employee {
    int num;
    char name[10];
    double hours;
};

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "rus");

    if (argc < 3) {
        return 1;
    }

    string filename = argv[1];
    int n = atoi(argv[2]);

    ofstream fout(filename, ios::binary);
    if (!fout) {
        cerr << "Ошибка открытия файла для записи!" << endl;
        return 1;
    }

    for (int i = 0; i < n; i++) {
        employee emp;
        cout << "[" << i + 1 << "] Введите ID сотрудника: ";
        cin >> emp.num;
        cout << "[" << i + 1 << "] Введите имя (до 9 символов): ";
        string tempName;
        cin >> tempName;

        size_t len = tempName.copy(emp.name, 9);
        emp.name[len] = '\0';

        cout << "[" << i + 1 << "] Введите количество часов: ";
        cin >> emp.hours;

        fout.write((char*)&emp, sizeof(employee));
    }

    fout.close();
    cout << "Данные успешно записаны в " << filename << endl;
    return 0;
}