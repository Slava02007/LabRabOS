#include <iostream>
#include <cmath>
#include <windows.h>
#include "ArrayFunctions.h"

using namespace std;

int main() {
    setlocale(LC_ALL, "rus");

    int n;
    cout << "Введите размер массива: ";
    cin >> n;

    int* arr = new int[n];
    cout << "Введите элементы массива: ";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];
    }

    ThreadParams params{ arr, n };

    HANDLE hMinMax = CreateThread(NULL, 0, MinMaxThread, &params, 0, NULL);
    HANDLE hAverage = CreateThread(NULL, 0, AverageThread, &params, 0, NULL);

    if (hMinMax == NULL || hAverage == NULL) {
        cout << "Ошибка создания потоков!" << endl;
        return 1;
    }

    WaitForSingleObject(hMinMax, INFINITE);
    WaitForSingleObject(hAverage, INFINITE);

    CloseHandle(hMinMax);
    CloseHandle(hAverage);

    int avgInt = static_cast<int>(round(g_average));

    for (int i = 0; i < n; i++) {
        if (arr[i] == g_minValue || arr[i] == g_maxValue) {
            arr[i] = avgInt;
        }
    }

    cout << "Массив после замены (min/max заменены на " << avgInt << "):" << endl;
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    delete[] arr;
    system("pause"); 
    return 0;
}






/*#include <iostream>
#include <vector>
#include <thread>   
#include <cmath>
#include "ArrayFunctions.h"

using namespace std;

int main() {
    setlocale(LC_ALL, "rus");

    int n;
    cout << "Введите размер массива: ";
    cin >> n;

    vector<int> arr(n);
    cout << "Введите элементы массива: ";
    for (int& x : arr) {
        cin >> x;
    }

    thread tMinMax(minMaxTask, ref(arr));
    thread tAverage(averageTask, ref(arr));

    if (tMinMax.joinable()) tMinMax.join();
    if (tAverage.joinable()) tAverage.join();

    int avgInt = static_cast<int>(round(g_average));
    for (int& x : arr) {
        if (x == g_minValue || x == g_maxValue) {
            x = avgInt;
        }
    }

    cout << "Массив после замены (на " << avgInt << "):" << endl;
    for (int x : arr) {
        cout << x << " ";
    }
    cout << endl;

    system("pause");
    return 0;
}
*/