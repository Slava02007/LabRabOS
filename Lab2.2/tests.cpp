#include <gtest/gtest.h>
#include <cmath>
#include "ArrayFunctions.h"


void ResetGlobals() {
    g_minValue = 0;
    g_maxValue = 0;
    g_average = 0.0;
}



TEST(MinMaxSearch, SimpleArrayCase) {
    ResetGlobals();
    std::vector<int> arr = { 5, 2, 8, 1, 9, 3 };
    ThreadParams params{ arr };

    MinMaxThread(params);

    EXPECT_EQ(g_minValue, 1);
    EXPECT_EQ(g_maxValue, 9);
}

TEST(MinMaxSearch, SimilarNumbersCase) {
    ResetGlobals();
    std::vector<int> arr = { 2, 2, 2, 2, 2, 2 };
    ThreadParams params{ arr };

    MinMaxThread(params);

    EXPECT_EQ(g_minValue, 2);
    EXPECT_EQ(g_maxValue, 2);
}

TEST(MinMaxSearch, NegativeNumbersCase) {
    ResetGlobals();
    std::vector<int> arr = { -2, -3, -4, -5, -10, -1900 };
    ThreadParams params{ arr };

    MinMaxThread(params);

    EXPECT_EQ(g_minValue, -1900);
    EXPECT_EQ(g_maxValue, -2);
}

TEST(MinMaxSearch, SingleNumberCase) {
    ResetGlobals();
    std::vector<int> arr = { 5 };
    ThreadParams params{ arr };

    MinMaxThread(params);

    EXPECT_EQ(g_minValue, 5);
    EXPECT_EQ(g_maxValue, 5);
}

TEST(MinMaxSearch, EmptyArrayCase) {
    ResetGlobals();
    std::vector<int> arr;
    ThreadParams params{ arr };

    MinMaxThread(params);

    EXPECT_EQ(g_minValue, 0);
    EXPECT_EQ(g_maxValue, 0);
}


TEST(AverageSearch, EmptyArrayCase) {
    ResetGlobals();
    std::vector<int> arr;
    ThreadParams params{ arr };

    AverageThread(params);

    EXPECT_EQ(g_average, 0.0);
}

TEST(AverageSearch, SimpleArrayCase) {
    ResetGlobals();
    std::vector<int> arr = { 5, 2, 8, 1, 9, 3 };
    ThreadParams params{ arr };

    AverageThread(params);

    EXPECT_EQ((int)std::round(g_average), 5);
}

TEST(AverageSearch, SimilarNumbersCase) {
    ResetGlobals();
    std::vector<int> arr = { 5, 5, 5, 5, 5, 5 };
    ThreadParams params{ arr };

    AverageThread(params);

    EXPECT_EQ((int)std::round(g_average), 5);
}

TEST(AverageSearch, NegativeNumbersCase) {
    ResetGlobals();
    std::vector<int> arr = { -2, -3, -4, -5, -10, -13 };
    ThreadParams params{ arr };

    AverageThread(params);

    EXPECT_EQ((int)std::round(g_average), -6);
}

TEST(AverageSearch, SingleNumberCase) {
    ResetGlobals();
    std::vector<int> arr = { 6 };
    ThreadParams params{ arr };

    AverageThread(params);

    EXPECT_EQ((int)std::round(g_average), 6);
}

TEST(AverageSearch, HugeNumbers) {
    ResetGlobals();
    std::vector<int> arr = { 1000000000, 1500000000, 2000000000 };
    ThreadParams params{ arr };

    AverageThread(params);

    EXPECT_EQ((int)std::round(g_average), 1500000000);
}
