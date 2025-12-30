#include "function.h"
#include <gtest/gtest.h>
#include <vector>

using namespace std;

TEST(ArrayTest, InitializationTest) {
    arraySize = 10;
    Arr.assign(arraySize, 0);

    EXPECT_EQ(Arr.size(), 10);
    for (int val : Arr) {
        EXPECT_EQ(val, 0); 
    }
}

TEST(ThreadParamsTest, StructureTest) {
    ThreadParams params;
    params.threadNumber = 5;
    params.terminated = false;

    EXPECT_EQ(params.threadNumber, 5);
    EXPECT_FALSE(params.terminated);
}

TEST(MarkerLogicTest, CleanupTest) {
    arraySize = 5;
    Arr.assign(arraySize, 0);
    int threadId = 1;

    vector<int> markedIndices = { 0, 2, 4 };
    for (int idx : markedIndices) {
        Arr[idx] = threadId;
    }

    EXPECT_EQ(Arr[0], threadId);
    EXPECT_EQ(Arr[1], 0);

    for (int idx : markedIndices) {
        Arr[idx] = 0;
    }

    for (int val : Arr) {
        EXPECT_EQ(val, 0);
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}