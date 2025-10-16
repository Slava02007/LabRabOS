#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>
#include <random>

using namespace std;

struct ThreadParams {
    int threadNumber;
    atomic<bool> shouldStop{ false };
    atomic<bool> shouldTerminate{ false };
    condition_variable cv;
    mutex mtx;
};

vector<int> Arr;
mutex arrMutex;
atomic<bool> startFlag{ false };
int arraySize;
int threadCount;

void marker(ThreadParams* params) {
    int id = params->threadNumber;

    while (!startFlag) {
        this_thread::yield();
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, Arr.size() - 1);

    vector<int> markedIndices;

    while (true) {
        int index = dist(gen);

        {
            lock_guard<mutex> lock(arrMutex);
            if (Arr[index] == 0) {
                this_thread::sleep_for(chrono::milliseconds(5));
                Arr[index] = id;
                markedIndices.push_back(index);
                this_thread::sleep_for(chrono::milliseconds(5));
                continue;
            }
        }

        cout << " Поток #" << id << " остановился." << endl;
        cout << "Количество помеченных элементов: " << markedIndices.size() << endl;
        cout << "Индекс, занятый другим потоком: " << index << endl;

        params->shouldStop.store(true);

        unique_lock<mutex> lock(params->mtx);
        params->cv.wait(lock, [params]() {
            return params->shouldTerminate.load() || !params->shouldStop.load();
            });

        if (params->shouldTerminate.load()) {
            break;
        }
    }

    {
        lock_guard<mutex> lock(arrMutex);
        for (int idx : markedIndices) {
            Arr[idx] = 0;
        }
    }

    cout << "Поток #" << id << " завершил работу." << endl;
}

int main() {
    setlocale(LC_ALL, "Rus");

    cout << "Введите размер массива: ";
    cin >> arraySize;
    Arr.assign(arraySize, 0);

    cout << "Введите количество потоков: ";
    cin >> threadCount;

    vector<thread> threads;
    vector<unique_ptr<ThreadParams>> params(threadCount);
    vector<bool> activeThreads(threadCount, true);

    for (int i = 0; i < threadCount; ++i) {
        params[i] = make_unique<ThreadParams>();
        params[i]->threadNumber = i + 1;
        threads.emplace_back(marker, params[i].get());
    }

    startFlag.store(true);

    int remainingThreads = threadCount;

    while (remainingThreads > 0) {
       bool allStopped = false;
        while (!allStopped) {
            allStopped = true;
            for (int i = 0; i < threadCount; ++i) {
                if (activeThreads[i] && !params[i]->shouldStop.load()) {
                    allStopped = false;
                    break;
                }
            }
            this_thread::sleep_for(chrono::milliseconds(10));
        }

        cout << "\nТекущее состояние массива:" << endl;
        {
            lock_guard<mutex> lock(arrMutex);
            for (int val : Arr) {
                cout << val << " ";
            }
        }
        cout << "\n";

        int terminateId;
        cout << "Введите номер потока для завершения (1-" << remainingThreads << "): ";
        cin >> terminateId;

        if (terminateId < 1 || terminateId > remainingThreads) {
            cout << "Некорректный номер. Повторите ввод." << endl;
            continue;
        }

        int foundCount = 0;
        int targetIndex = -1;
        for (int i = 0; i < threadCount; ++i) {
            if (activeThreads[i]) {
                foundCount++;
                if (foundCount == terminateId) {
                    targetIndex = i;
                    break;
                }
            }
        }

        if (targetIndex == -1) {
            cout << "Ошибка: поток не найден." << endl;
            continue;
        }

        params[targetIndex]->shouldTerminate.store(true);
        params[targetIndex]->shouldStop.store(false);
        params[targetIndex]->cv.notify_one();

        if (threads[targetIndex].joinable()) {
            threads[targetIndex].join();
        }

        activeThreads[targetIndex] = false;
        remainingThreads--;

        cout << "\nСостояние массива после завершения потока #" << terminateId << ":" << endl;
        {
            lock_guard<mutex> lock(arrMutex);
            for (int val : Arr) {
                cout << val << " ";
            }
        }
        cout << "\n";

        for (int i = 0; i < threadCount; ++i) {
            if (activeThreads[i]) {
                params[i]->shouldStop.store(false);
                params[i]->cv.notify_one();
            }
        }
    }

    cout << " Все потоки завершены. Программа завершается." << endl;

    return 0;
}