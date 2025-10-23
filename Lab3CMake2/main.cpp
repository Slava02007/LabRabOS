#include <iostream>
#include <vector>
#include <thread>
#include "function.h"

using namespace std;

int main() {
    setlocale(LC_ALL, "Rus");

    cout << "¬ведите размер массива: ";
    cin >> arraySize;
    Arr.assign(arraySize, 0);

    cout << "¬ведите количество потоков: ";
    cin >> threadCount;

    vector<thread> threads(threadCount);
    vector<ThreadParams> params(threadCount);

    for (int i = 0; i < threadCount; ++i) {
        params[i].threadNumber = i + 1;
        threads[i] = thread(marker, &params[i]);
    }

   
    {
        lock_guard<mutex> lock(startMutex);
        startFlag = true;
    }
    startCv.notify_all();

    int activeThreads = threadCount;

    





    return 0;
}
