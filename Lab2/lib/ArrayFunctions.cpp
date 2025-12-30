#include "ArrayFunctions.h"
#include <iostream>

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
        if (arr[i] < minv) {
            minv = arr[i];
        }
        Sleep(7); 

        if (arr[i] > maxv) {
            maxv = arr[i];
        }
        Sleep(7); 
    }

    g_minValue = minv;
    g_maxValue = maxv;

    cout << "Минимальное: " << g_minValue << ", Максимальное: " << g_maxValue << endl;
    return 0;
}

DWORD WINAPI AverageThread(LPVOID lpParam) {
    ThreadParams* p = static_cast<ThreadParams*>(lpParam);
    int* arr = p->arr;
    int n = p->n;

    if (n <= 0) return 1;

    double sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += arr[i];
        Sleep(12); 
    }

    g_average = sum / n;
    cout << "Среднее значение: " << g_average << endl;
    return 0;
}




/*#include "ArrayFunctions.h"
#include <iostream>
#include <thread> 
#include <chrono> 

using namespace std;

int g_minValue = 0;
int g_maxValue = 0;
double g_average = 0.0;

void minMaxTask(const vector<int>& arr) {
    if (arr.empty()) return;

    int minv = arr[0];
    int maxv = arr[0];

    for (size_t i = 0; i < arr.size(); ++i) {
        if (arr[i] < minv) minv = arr[i];
        this_thread::sleep_for(chrono::milliseconds(7)); 

        if (arr[i] > maxv) maxv = arr[i];
        this_thread::sleep_for(chrono::milliseconds(7));
    }

    g_minValue = minv;
    g_maxValue = maxv;

    cout << "Максимальное: " << g_minValue << ", Минимальное: " << g_maxValue << endl;
}

void averageTask(const vector<int>& arr) {
    if (arr.empty()) return;

    double sum = 0;
    for (size_t i = 0; i < arr.size(); ++i) {
        sum += arr[i];
        this_thread::sleep_for(chrono::milliseconds(12));
    }

    g_average = sum / arr.size();
    cout << "Среднее значение: " << g_average << endl;
}*/