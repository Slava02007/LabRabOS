#include "ArrayFunctions.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>

using namespace std;

int g_minValue = 0;
int g_maxValue = 0;
double g_average = 0.0;

void MinMaxThread(ThreadParams& params) {
    auto& arr = params.arr;
    if (arr.empty()) return;

    int minv = arr[0];
    int maxv = arr[0];

    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] < minv) minv = arr[i];
        this_thread::sleep_for(chrono::milliseconds(7));
        if (arr[i] > maxv) maxv = arr[i];
        this_thread::sleep_for(chrono::milliseconds(7));
    }

    g_minValue = minv;
    g_maxValue = maxv;

    cout << "Minimal alement: " << g_minValue << endl;
    cout << "Maksimal alement: " << g_maxValue << endl;
}

void AverageThread(ThreadParams& params) {
    auto& arr = params.arr;
    if (arr.empty()) return;

    long long sum = 0;
    for (int x : arr) {
        sum += x;
        this_thread::sleep_for(chrono::milliseconds(12));
    }

    g_average = static_cast<double>(sum) / arr.size();
    cout << "Srednee arifmiticheskoe: " << g_average << endl;
}
