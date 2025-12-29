#include <iostream>
#include <fstream>
#include <string>
#include <iomanip> 

using namespace std;

struct employee {
    int num;
    char name[10];
    double hours;
};

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "rus");

    if (argc < 4) {
        return 1;
    }

    string binFile = argv[1];
    string reportFile = argv[2];
    double payPerHour = atof(argv[3]);

    ifstream fin(binFile, ios::binary);
    if (!fin) {
        cerr << "Ошибка открытия бинарного файла!" << endl;
        return 1;
    }

    ofstream fout(reportFile);
    if (!fout) {
        cerr << "Ошибка создания файла отчета!" << endl;
        return 1;
    }

    fout << "Отчет по файлу \"" << binFile << "\"" << endl;
    fout << "Номер\tИмя\tЧасы\tЗарплата" << endl;

    employee emp;
    fout << fixed << setprecision(2);

    while (fin.read((char*)&emp, sizeof(employee))) {
        double salary = emp.hours * payPerHour;
        fout << emp.num << "\t" << emp.name << "\t" << emp.hours << "\t" << salary << endl;
    }

    fin.close();
    fout.close();

    cout << "Отчет сформирован в файле: " << reportFile << endl;
    return 0;
}