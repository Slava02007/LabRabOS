#include <iostream>
#include <vector>
#include <windows.h>
#include "function.h"

using namespace std;

int main() {
    setlocale(LC_ALL, "Rus");

    cout << "Введите размер массива: ";
    cin >> arraySize;
    Arr.assign(arraySize, 0);

    cout << "Введите количество потоков: ";
    cin >> threadCount;

    vector<HANDLE> threads(threadCount);
    vector<ThreadParams> params(threadCount);
    vector<HANDLE> stopEvents(threadCount);

    for (int i = 0; i < threadCount; ++i) {
        params[i].threadNumber = i + 1;
        params[i].stopEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
        params[i].continueEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
        params[i].terminateEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
        stopEvents[i] = params[i].stopEvent;

        threads[i] = CreateThread(NULL, 0, marker, &params[i], CREATE_SUSPENDED, NULL);
    }

    startEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    SetEvent(startEvent);

    for (HANDLE h : threads)
        ResumeThread(h);

    int activeThreads = threadCount;

    while (activeThreads > 0) {
    
        vector<HANDLE> activeStopEvents;
        for (int i = 0; i < threadCount; ++i) {
            if (!params[i].terminated)
                activeStopEvents.push_back(params[i].stopEvent);
        }

        WaitForMultipleObjects(static_cast<DWORD>(activeStopEvents.size()), activeStopEvents.data(), TRUE, INFINITE);

        cout << "\nТекущее состояние массива:" << endl;
        for (int val : Arr)
            cout << val << " ";
        cout << "\n";

        int terminateId;
        bool valid = false;
        while (!valid) {
            cout << "Введите номер потока для завершения (1-" << threadCount << "): ";
            cin >> terminateId;

            if (terminateId < 1 || terminateId > threadCount) {
                cout << "Некорректный номер. Повторите ввод.\n";
            }
            else if (params[terminateId - 1].terminated) {
                cout << "Этот поток уже завершён. Повторите ввод.\n";
            }
            else {
                valid = true;
            }
        }

        
        SetEvent(params[terminateId - 1].terminateEvent);
        WaitForSingleObject(threads[terminateId - 1], INFINITE);

        CloseHandle(threads[terminateId - 1]);
        CloseHandle(params[terminateId - 1].stopEvent);
        CloseHandle(params[terminateId - 1].continueEvent);
        CloseHandle(params[terminateId - 1].terminateEvent);

        params[terminateId - 1].terminated = true; 
        activeThreads--;

        cout << "\nСостояние массива после завершения потока #" << terminateId << ":\n";
        for (int val : Arr)
            cout << val << " ";
        cout << "\n";

       
        for (int i = 0; i < threadCount; ++i) {
            if (!params[i].terminated)
                SetEvent(params[i].continueEvent);
        }
    }

    cout << "Все потоки завершены. Программа завершается.\n";


    
    cout << " Все потоки завершены. Программа завершается." << endl;

    CloseHandle(startEvent);
    return 0;
}