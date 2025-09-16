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

    if (argc < 4) {
        cerr << "Usage: Reporter <input_file> <report_file> <pay_per_hour>" << endl;
        return 1;
    }

    string binFile = argv[1];
    string reportFile = argv[2];
    double payPerHour = stod(argv[3]);

    ifstream fin(binFile, ios::binary);
    if (!fin) {
        cerr << "Error: cannot open binary file " << binFile << endl;
        return 1;
    }

    ofstream fout(reportFile);
    if (!fout) {
        cerr << "Error: cannot create report file " << reportFile << endl;
        return 1;
    }

    fout << "Отчет по файлу \"" << binFile << "\"\n";
    fout << "Номер\tИмя\tЧасы\tЗарплата\n";

    employee emp;
    while (fin.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
        double salary = emp.hours * payPerHour;
        fout << emp.num << "\t" << emp.name << "\t"
            << emp.hours << "\t" << salary << "\n";
    }

    fin.close();
    fout.close();

    cout << "Отчет сохранён в файл " << reportFile << endl;
    return 0;
}
