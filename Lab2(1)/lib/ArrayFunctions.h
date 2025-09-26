#pragma once
#include <windows.h>

extern int g_minValue;
extern int g_maxValue;
extern double g_average;

struct ThreadParams {
    int* arr;
    int n;
};

DWORD WINAPI MinMaxThread(LPVOID lpParam);
DWORD WINAPI AverageThread(LPVOID lpParam);
