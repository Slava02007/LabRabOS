#pragma once
#include <windows.h>

struct ThreadParams {
    int* arr;
    int n;
};

extern int g_minValue;
extern int g_maxValue;
extern double g_average;

DWORD WINAPI MinMaxThread(LPVOID lpParam);
DWORD WINAPI AverageThread(LPVOID lpParam);
