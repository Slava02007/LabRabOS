#include <iostream>
#include <vector>
#include <windows.h>
#include "function.h"

using namespace std;

vector<int> Arr;
HANDLE startEvent;
int arraySize;
int threadCount;
CRITICAL_SECTION cs; 

int main() {
    setlocale(LC_ALL, "Rus");

    cout << "Введите размер массива: ";
    cin >> arraySize;
    Arr.assign(arraySize, 0);

    cout << "Введите количество потоков marker: ";
    cin >> threadCount;

    InitializeCriticalSection(&cs); 

    vector<HANDLE> threads(threadCount);
    vector<ThreadParams> params(threadCount);

    startEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

    for (int i = 0; i < threadCount; ++i) {
        params[i].threadNumber = i + 1;
        params[i].stopEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
        params[i].continueEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
        params[i].terminateEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
        params[i].terminated = false;

        threads[i] = CreateThread(NULL, 0, marker, &params[i], 0, NULL);
    }

    SetEvent(startEvent);

    int activeThreads = threadCount;

    while (activeThreads > 0) {
        vector<HANDLE> activeStopEvents;
        vector<int> activeIds;
        for (int i = 0; i < threadCount; ++i) {
            if (!params[i].terminated) {
                activeStopEvents.push_back(params[i].stopEvent);
                activeIds.push_back(i);
            }
        }

        WaitForMultipleObjects(static_cast<DWORD>(activeStopEvents.size()), activeStopEvents.data(), TRUE, INFINITE);

        cout << "\nВсе потоки остановились. Массив:" << endl;
        for (int val : Arr) cout << val << " ";
        cout << endl;

        int terminateId;
        while (true) {
            cout << "Введите номер потока для завершения (1-" << threadCount << "): ";
            cin >> terminateId;
            if (terminateId >= 1 && terminateId <= threadCount && !params[terminateId - 1].terminated) break;
            cout << "Ошибка: поток уже мертв или не существует." << endl;
        }

        SetEvent(params[terminateId - 1].terminateEvent);
        WaitForSingleObject(threads[terminateId - 1], INFINITE);
        params[terminateId - 1].terminated = true;
        activeThreads--;

        cout << "Массив после очистки меток потока #" << terminateId << ":" << endl;
        for (int val : Arr) cout << val << " ";
        cout << endl;

        for (int i = 0; i < threadCount; ++i) {
            if (!params[i].terminated) {
                SetEvent(params[i].continueEvent);
            }
        }
    }

    cout << "Все потоки завершены." << endl;

    DeleteCriticalSection(&cs);
    CloseHandle(startEvent);
    system("pause");
    return 0;
}