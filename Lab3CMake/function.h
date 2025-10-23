#ifndef FUNCTION_H
#define FUNCTION_H

#include <windows.h>
#include <vector>

using namespace std;

extern vector<int> Arr;
extern HANDLE startEvent;
extern int arraySize;
extern int threadCount;

struct ThreadParams {
    int threadNumber;
    HANDLE stopEvent;
    HANDLE continueEvent;
    HANDLE terminateEvent;
    bool terminated = false;
};

DWORD WINAPI marker(LPVOID lpParam);

#endif