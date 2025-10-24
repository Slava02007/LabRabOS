#include "Koloc.h"
#include <gtest/gtest.h>
using namespace std;

TEST(Func1Tests, CalculatesFactorials) {
    auto res = Func1(5);
    vector<long long> expected = { 1, 2, 6, 24, 120 };
    EXPECT_EQ(res, expected);
}

TEST(RemoveDuplicatesTests, RemovesCorrectly) {
    vector<int> input = { 1, 2, 2, 3, 1 };
    vector<int> expected = { 1, 2, 3 };
    EXPECT_EQ(removeDuplicates(input), expected);
}

TEST(ReverseListTests, WorksWithThreeElements) {
    Node* head = new Node(1);
    head->next = new Node(2);
    head->next->next = new Node(3);

    Node* newHead = reverseListRecursive(head);

    EXPECT_EQ(newHead->data, 3);
    EXPECT_EQ(newHead->next->data, 2);
    EXPECT_EQ(newHead->next->next->data, 1);
    EXPECT_EQ(newHead->next->next->next, nullptr);

    freeList(newHead);
}
