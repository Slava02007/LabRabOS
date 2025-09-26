#include "ArrayFunctions.h"
#include <iostream>
#include <cmath>

using namespace std;


int g_minValue = 0;
int g_maxValue = 0;
double g_average = 0.0;

DWORD WINAPI MinMaxThread(LPVOID lpParam) {
    ThreadParams* p = static_cast<ThreadParams*>(lpParam);
    int* arr = p->arr;
    int n = p->n;

    if (n <= 0) return 1;

    int minv = arr[0];
    int maxv = arr[0];

    for (int i = 1; i < n; ++i) {
        if (arr[i] < minv) minv = arr[i];
        Sleep(7);
        if (arr[i] > maxv) maxv = arr[i];
        Sleep(7);
    }

    g_minValue = minv;
    g_maxValue = maxv;

    cout << "Minimal el: " << g_minValue << endl;
    cout << "Maximal el: " << g_maxValue << endl;

    return 0;
}

DWORD WINAPI AverageThread(LPVOID lpParam) {
    ThreadParams* p = static_cast<ThreadParams*>(lpParam);
    int* arr = p->arr;
    int n = p->n;

    if (n <= 0) return 1;

    long long sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += arr[i];
        Sleep(12);
    }

    g_average = static_cast<double>(sum) / n;
    cout << "Srednee arifmeticheskoe elementov massiva: " << g_average << endl;

    return 0;
}
