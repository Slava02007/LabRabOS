#include <iostream>
#include <vector>
#include <windows.h>
#include <ctime>
#include "function.h"

using namespace std;

DWORD WINAPI marker(LPVOID lpParam) {
    ThreadParams* params = static_cast<ThreadParams*>(lpParam);
    int id = params->threadNumber;

    WaitForSingleObject(startEvent, INFINITE);

    srand(id);
    vector<int> markedIndices;

    while (true) {
        int index = rand() % arraySize;

        EnterCriticalSection(&cs);

        if (Arr[index] == 0) {
            Sleep(5);
            Arr[index] = id;
            markedIndices.push_back(index);
            Sleep(5);
            LeaveCriticalSection(&cs); 
        }
        else {
            LeaveCriticalSection(&cs);
            cout << "Поток #" << id << ": не могу пометить индекс " << index
                << ". Помечено элементов: " << markedIndices.size() << endl;

            SetEvent(params->stopEvent);

            HANDLE waitEvents[2] = { params->continueEvent, params->terminateEvent };
            DWORD result = WaitForMultipleObjects(2, waitEvents, FALSE, INFINITE);

            if (result == WAIT_OBJECT_0 + 1) {
                EnterCriticalSection(&cs);
                for (int idx : markedIndices) {
                    Arr[idx] = 0;
                }
                LeaveCriticalSection(&cs);
                break;
            }
        }
    }

    cout << "Поток #" << id << " завершил работу и очистил метки." << endl;
    return 0;
}






//часть кода на более новом стандарте 
/*#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <atomic>

using namespace std;


namespace ModernCpp {
    mutex mtx;                         // Замена CRITICAL_SECTION
    condition_variable cv;             // Замена Events (для сигналов)
    bool ready = false;                // Флаг для старта
    atomic<bool> should_terminate{false}; 

    void modernMarkerTask(int id, vector<int>& sharedArr) {
        unique_lock<lock_guard> lck(mtx);
        cv.wait(lck, []{ return ready; }); 
        lck.unlock();

        srand(id);
        vector<int> myIndices;

        while (!should_terminate) {
            int idx = rand() % sharedArr.size();

            {
                lock_guard<mutex> lock(mtx);
                if (sharedArr[idx] == 0) {
                    this_thread::sleep_for(chrono::milliseconds(5)); 
                    sharedArr[idx] = id;
                    myIndices.push_back(idx);
                    this_thread::sleep_for(chrono::milliseconds(5));
                } else {
                    cout << "Thread " << id << " stuck at " << idx << endl;
                }
            }

            if (should_terminate) break;
        }

        lock_guard<mutex> lock(mtx);
        for (int i : myIndices) sharedArr[i] = 0;
    }

    void runExample() {
        vector<int> data(10, 0);
        thread t1(modernMarkerTask, 1, ref(data));
        {
            lock_guard<mutex> lock(mtx);
            ready = true;
        }
        cv.notify_all(); 
        if (t1.joinable()) t1.join(); 
    }
}
*/