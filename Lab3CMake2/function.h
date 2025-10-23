#ifndef FUNCTION_H
#define FUNCTION_H

#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <atomic>

using namespace std;

extern vector<int> Arr;
extern int arraySize;
extern int threadCount;


extern atomic<bool> startFlag;
extern mutex startMutex;
extern condition_variable startCv;

struct ThreadParams {
    int threadNumber;
    atomic<bool> stopRequested{ false };
    atomic<bool> continueRequested{ false };
    atomic<bool> terminateRequested{ false };
    atomic<bool> terminated{ false };
    mutex mtx;
    condition_variable cv;
};

void marker(ThreadParams* params);

#endif

