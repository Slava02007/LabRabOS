#include "function.h"
#include <gtest/gtest.h>
#include <iostream>
#include <vector>

using namespace std;


TEST(ArrayTest, InitializationTest) {
    Arr.assign(10, 0);
    EXPECT_EQ(Arr.size(), 10);
    for (int val : Arr) {
        EXPECT_EQ(val, 0);
    }
}


TEST(ThreadParamsTest, StructureTest) {
    ThreadParams params;
    params.threadNumber = 1;

    EXPECT_EQ(params.threadNumber, 1);
}

TEST(GlobalVariablesTest, VariablesTest) {
    arraySize = 5;
    threadCount = 3;

    EXPECT_EQ(arraySize, 5);
    EXPECT_EQ(threadCount, 3);
}

TEST(ArraySizeTest, SizeTest) {
    Arr.assign(15, 0);
    EXPECT_EQ(Arr.size(), 15);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

