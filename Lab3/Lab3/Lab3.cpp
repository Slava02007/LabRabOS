#include <iostream>
#include <vector>
#include <windows.h>
#include <ctime>

using namespace std;

struct ThreadParams {
    int threadNumber;
    HANDLE stopEvent;
    HANDLE continueEvent;
    HANDLE terminateEvent;
};

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
        WaitForMultipleObjects(activeThreads, stopEvents.data(), TRUE, INFINITE);

        cout << "\nТекущее состояние массива:" << endl;
        for (int val : Arr)
            cout << val << " ";
        cout << "\n";

        int terminateId;
        cout << "Введите номер потока для завершения (1-" << activeThreads << "): ";
        cin >> terminateId;

        if (terminateId < 1 || terminateId > activeThreads) {
            cout << "Некорректный номер. Повторите ввод." << endl;
            continue;
        }

        SetEvent(params[terminateId - 1].terminateEvent);
        WaitForSingleObject(threads[terminateId - 1], INFINITE);

        CloseHandle(threads[terminateId - 1]);
        CloseHandle(params[terminateId - 1].stopEvent);
        CloseHandle(params[terminateId - 1].continueEvent);
        CloseHandle(params[terminateId - 1].terminateEvent);

        cout << "\nСостояние массива после завершения потока #" << terminateId << ":" << endl;
        for (int val : Arr)
            cout << val << " ";
        cout << "\n";

        threads.erase(threads.begin() + (terminateId - 1));
        params.erase(params.begin() + (terminateId - 1));
        stopEvents.erase(stopEvents.begin() + (terminateId - 1));

        activeThreads--;

        for (int i = 0; i < activeThreads; ++i)
            SetEvent(params[i].continueEvent);
    }

    cout << " Все потоки завершены. Программа завершается." << endl;

    CloseHandle(startEvent);
    return 0;
}
