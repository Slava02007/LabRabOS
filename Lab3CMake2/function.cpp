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


