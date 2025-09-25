#include <iostream>
#include <cmath>
#include <windows.h>
#include "CMake_Lab2.h"

using namespace std;

int main() {
    setlocale(LC_ALL, "rus");

    int n;
    cout << "Введите размер массива: ";
    cin >> n;

    if (n <= 0) {
        cout << "Размер массива должен быть положительным!" << endl;
        return 1;
    }

    int* arr = new int[n];
    cout << "Введите элементы массива: ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    ThreadParams params{ arr, n };

    
    HANDLE hMinMax = CreateThread(NULL, 0, MinMaxThread, &params, 0, NULL);
    HANDLE hAverage = CreateThread(NULL, 0, AverageThread, &params, 0, NULL);

    
    WaitForSingleObject(hMinMax, INFINITE);
    WaitForSingleObject(hAverage, INFINITE);

    CloseHandle(hMinMax);
    CloseHandle(hAverage);

    
    int avgInt = static_cast<int>(round(g_average));
    for (int i = 0; i < n; i++) {
        if (arr[i] == g_minValue || arr[i] == g_maxValue) {
            arr[i] = avgInt;
        }
    }

    cout << "Массив после замены:" << endl;
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    delete[] arr;
    return 0;
}
