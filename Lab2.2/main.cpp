#include <iostream>
#include <cmath>
#include <thread>
#include <vector>
#include "ArrayFunctions.h"

using namespace std;

int main() {
    setlocale(LC_ALL, "ru");

    int n;
    cout << "Введите размер массива: ";
    cin >> n;

    if (n <= 0) {
        cout << "Размер массива должен быть положительным!" << endl;
        return 1;
    }

    vector<int> arr(n);
    cout << "Введите элементы массива: ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    ThreadParams params{ arr };

    
    thread tMinMax(MinMaxThread, ref(params));
    thread tAvg(AverageThread, ref(params));

    tMinMax.join();
    tAvg.join();

    int avgInt = static_cast<int>(round(g_average));
    for (int& x : arr) {
        if (x == g_minValue || x == g_maxValue) {
            x = avgInt;
        }
    }

    cout << "Массив после замены:" << endl;
    for (int x : arr) {
        cout << x << " ";
    }
    cout << endl;

    return 0;
}
