#include "function.h"
#include <iostream>
#include <chrono>

using namespace std;

vector<int> Arr;
int arraySize;
int threadCount;

atomic<bool> startFlag{ false };
mutex startMutex;
condition_variable startCv;

void marker(ThreadParams* params) {
    int id = params->threadNumber;


    {
        unique_lock<mutex> lock(startMutex);
        startCv.wait(lock, [] { return startFlag.load(); });
    }

    srand(id);
    vector<int> markedIndices;

    while (true) {
        int index = rand() % Arr.size();

        if (Arr[index] == 0) {
            this_thread::sleep_for(5ms);
            Arr[index] = id;
            markedIndices.push_back(index);
            this_thread::sleep_for(5ms);
        }
        else {
            cout << "Поток #" << id << " остановился." << endl;
            cout << "Количество помеченных элементов: " << markedIndices.size() << endl;
            cout << "Индекс, занятый другим потоком: " << index << endl;

            params->stopRequested = true;

            unique_lock<mutex> lock(params->mtx);
            params->cv.wait(lock, [&] {
                return params->continueRequested || params->terminateRequested;
                });

            if (params->terminateRequested)
                break;

            params->stopRequested = false;
            params->continueRequested = false;
        }
    }

    for (int idx : markedIndices)
        Arr[idx] = 0;

    params->terminated = true;
    cout << "Поток #" << id << " завершил работу." << endl;
}

