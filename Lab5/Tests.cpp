#include <iostream>
#include <fstream>
#include <cassert>
#include <filesystem>
#include <optional>
#include "employee.h"

using namespace std;
using namespace filesystem;

optional<pair<employee, streampos>> findEmployee(string filename, int id) {
    fstream file(filename, ios::binary | ios::in);
    if (!file) return nullopt;

    employee emp;
    while (file.read((char*)&emp, sizeof(employee))) {
        if (emp.num == id) {
            streampos pos = (int)file.tellg() - (int)sizeof(employee);
            return make_pair(emp, pos);
        }
    }
    return nullopt;
}

void test_file_creation() {
    string testFile = "test_data.bin";
    ofstream out(testFile, ios::binary);

    employee e1 = { 1, "Ivan", 10.0 };
    out.write((char*)&e1, sizeof(employee));
    out.close();

    ifstream in(testFile, ios::binary);
    employee e2;
    in.read((char*)&e2, sizeof(employee));
    in.close();

    assert(e2.num == 1);
    assert(string(e2.name) == "Ivan");
    assert(e2.hours == 10.0);

    remove(testFile); 
    cout << "Test 1 (File Creation): PASSED" << endl;
}

void test_find_employee_success() {
    string testFile = "test_search.bin";
    ofstream out(testFile, ios::binary);
    employee e1 = { 99, "Tester", 5.5 };
    out.write((char*)&e1, sizeof(employee));
    out.close();

    auto result = findEmployee(testFile, 99);

    assert(result.has_value() == true);
    assert(result->first.num == 99);
    assert(string(result->first.name) == "Tester");

    remove(testFile);
    cout << "Test 2 (Search Success): PASSED" << endl;
}

void test_find_employee_fail() {
    string testFile = "test_fail.bin";
    ofstream out(testFile, ios::binary);
    employee e1 = { 5, "User", 1.0 };
    out.write((char*)&e1, sizeof(employee));
    out.close();

    auto result = findEmployee(testFile, 999); 

    assert(result.has_value() == false);

    remove(testFile);
    cout << "Test 3 (Search Fail): PASSED" << endl;
}

int main() {
    cout << "--- Running Unit Tests ---" << endl;

    try {
        test_file_creation();
        test_find_employee_success();
        test_find_employee_fail();
        cout << "--- All Tests Passed Successfully! ---" << endl;
    }
    catch (...) {
        cerr << "Some tests FAILED!" << endl;
        return 1;
    }

    system("pause");
    return 0;
}