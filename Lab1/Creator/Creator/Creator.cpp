#include <iostream>
#include <fstream>
#include <string>
#include <cstring> 
using namespace std;

struct employee {
    int num;
    char name[10];
    double hours;
};

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "rus");

    if (argc < 3) {
        cerr << "Usage: Creator <filename> <number_of_records>" << endl;
        return 1;
    }

    string filename = argv[1];
    int n = stoi(argv[2]);

    ofstream fout(filename, ios::binary);
    if (!fout) {
        cerr << "Error: cannot open file " << filename << endl;
        return 1;
    }

    for (int i = 0; i < n; i++) {
        employee emp;
        cout << "Введите данные для сотрудника " << i + 1 << endl;
        cout << "Номер: ";
        cin >> emp.num;
        cout << "Имя (до 9 символов): ";
        string temp;
        cin >> temp;
        strncpy_s(emp.name, temp.c_str(), 9);
        emp.name[9] = '\0'; 
        cout << "Часы: ";
        cin >> emp.hours;

        fout.write(reinterpret_cast<char*>(&emp), sizeof(employee));
    }

    fout.close();
    cout << "Файл " << filename << " успешно создан." << endl;
    return 0;
}
