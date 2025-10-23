#include <iostream>
#include <vector>
#include <thread>
#include "function.h"

using namespace std;

int main() {
    setlocale(LC_ALL, "Rus");

    cout << "������� ������ �������: ";
    cin >> arraySize;
    Arr.assign(arraySize, 0);

    cout << "������� ���������� �������: ";
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

    
    while (activeThreads > 0) {

        bool allStopped = false;
        while (!allStopped) {
            this_thread::sleep_for(100ms);
            allStopped = true;
            for (auto& p : params) {
                if (!p.terminated && !p.stopRequested) {
                    allStopped = false;
                    break;
                }
            }
        }

        cout << "\n������� ��������� �������:" << endl;
        for (int val : Arr)
            cout << val << " ";
        cout << "\n";

        int terminateId;
        bool valid = false;
        while (!valid) {
            cout << "������� ����� ������ ��� ���������� (1-" << threadCount << "): ";
            cin >> terminateId;

            if (terminateId < 1 || terminateId > threadCount) {
                cout << "������������ �����. ��������� ����.\n";
            }
            else if (params[terminateId - 1].terminated) {
                cout << "���� ����� ��� ��������. ��������� ����.\n";
            }
            else {
                valid = true;
            }
        }


        {
            lock_guard<mutex> lock(params[terminateId - 1].mtx);
            params[terminateId - 1].terminateRequested = true;
        }
        params[terminateId - 1].cv.notify_one();

        threads[terminateId - 1].join();
        params[terminateId - 1].terminated = true;
        activeThreads--;

        cout << "\n��������� ������� ����� ���������� ������ #" << terminateId << ":\n";
        for (int val : Arr)
            cout << val << " ";
        cout << "\n";


        for (auto& p : params) {
            if (!p.terminated && p.stopRequested) {
                lock_guard<mutex> lock(p.mtx);
                p.continueRequested = true;
                p.cv.notify_one();
            }
        }
    }

    cout << "��� ������ ���������. ��������� �����������.\n";





    return 0;
}
