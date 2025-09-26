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
    int arr[] = { 5, 2, 8, 1, 9, 3 };
    ThreadParams params{ arr, 6 };

    EXPECT_EQ(MinMaxThread(&params), 0);
    EXPECT_EQ(g_minValue, 1);
    EXPECT_EQ(g_maxValue, 9);
}

TEST(MinMaxSearch, SimilarNumbersCase) {
    ResetGlobals();
    int arr[] = { 2, 2, 2, 2, 2, 2 };
    ThreadParams params{ arr, 6 };

    EXPECT_EQ(MinMaxThread(&params), 0);
    EXPECT_EQ(g_minValue, 2);
    EXPECT_EQ(g_maxValue, 2);
}

TEST(MinMaxSearch, NegativeNumbersCase) {
    ResetGlobals();
    int arr[] = { -2, -3, -4, -5, -10, -1900 };
    ThreadParams params{ arr, 6 };

    EXPECT_EQ(MinMaxThread(&params), 0);
    EXPECT_EQ(g_minValue, -1900);
    EXPECT_EQ(g_maxValue, -2);
}

TEST(MinMaxSearch, SingleNumberCase) {
    ResetGlobals();
    int arr[] = { 5 };
    ThreadParams params{ arr, 1 };

    EXPECT_EQ(MinMaxThread(&params), 0);
    EXPECT_EQ(g_minValue, 5);
    EXPECT_EQ(g_maxValue, 5);
}

TEST(MinMaxSearch, EmptyArrayCase) {
    ResetGlobals();
    int arr[1] = {};
    ThreadParams params{ arr, 0 };

    EXPECT_EQ(MinMaxThread(&params), 1);
}

TEST(AverageSearch, EmptyArrayCase) {
    ResetGlobals();
    int arr[1] = {};
    ThreadParams params{ arr, 0 };

    EXPECT_EQ(AverageThread(&params), 1);
}

TEST(AverageSearch, SimpleArrayCase) {
    ResetGlobals();
    int arr[] = { 5, 2, 8, 1, 9, 3 };
    ThreadParams params{ arr, 6 };

    EXPECT_EQ(AverageThread(&params), 0);
    EXPECT_EQ((int)std::round(g_average), 5);
}

TEST(AverageSearch, SimilarNumbersCase) {
    ResetGlobals();
    int arr[] = { 5, 5, 5, 5, 5, 5 };
    ThreadParams params{ arr, 6 };

    EXPECT_EQ(AverageThread(&params), 0);
    EXPECT_EQ((int)std::round(g_average), 5);
}

TEST(AverageSearch, NegativeNumbersCase) {
    ResetGlobals();
    int arr[] = { -2, -3, -4, -5, -10, -13 };
    ThreadParams params{ arr, 6 };

    EXPECT_EQ(AverageThread(&params), 0);
    EXPECT_EQ((int)std::round(g_average), -6);
}

TEST(AverageSearch, SingleNumberCase) {
    ResetGlobals();
    int arr[] = { 6 };
    ThreadParams params{ arr, 1 };

    EXPECT_EQ(AverageThread(&params), 0);
    EXPECT_EQ((int)std::round(g_average), 6);
}

TEST(AverageSearch, HugeNumbers) {
    ResetGlobals();
    int arr[] = { 1000000000, 1500000000, 2000000000 };
    ThreadParams params{ arr, 3 };

    EXPECT_EQ(AverageThread(&params), 0);
    EXPECT_EQ((int)std::round(g_average), 1500000000);
}
