#pragma once
#include <thread>
#include <vector>

extern int g_minValue;
extern int g_maxValue;
extern double g_average;

struct ThreadParams {
    std::vector<int>& arr;
};

void MinMaxThread(ThreadParams& params);
void AverageThread(ThreadParams& params);
