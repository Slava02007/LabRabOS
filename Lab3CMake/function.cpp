#include <iostream>
#include <vector>
#include <windows.h>
#include <ctime>
#include "function.h"

using namespace std;

vector<int> Arr;
HANDLE startEvent;
int arraySize;
int threadCount;

DWORD WINAPI marker(LPVOID lpParam) {
    ThreadParams* params = static_cast<ThreadParams*>(lpParam);
    int id = params->threadNumber;

    WaitForSingleObject(startEvent, INFINITE);

    srand(id);
    vector<int> markedIndices;

    while (true) {
        int index = rand() % Arr.size();

        if (Arr[index] == 0) {
            Sleep(5);
            Arr[index] = id;
            markedIndices.push_back(index);
            Sleep(5);
        }
        else {
            cout << " Поток #" << id << " остановился." << endl;
            cout << "Количество помеченных элементов: " << markedIndices.size() << endl;
            cout << "Индекс, занятый другим потоком: " << index << endl;

            SetEvent(params->stopEvent);

            HANDLE events[2] = { params->continueEvent, params->terminateEvent };
            DWORD result = WaitForMultipleObjects(2, events, FALSE, INFINITE);

            if (result == WAIT_OBJECT_0 + 1) {
                break;
            }
        }
    }

    for (int idx : markedIndices)
        Arr[idx] = 0;

    cout << "Поток #" << id << " завершил работу." << endl;
    return 0;
}