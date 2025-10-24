#pragma once
#include <vector>

std::vector<long long> Func1(int n);

std::vector<int> removeDuplicates(const std::vector<int>& input);

struct Node {
    int data;
    Node* next;
    Node(int value);
};

Node* reverseListRecursive(Node* head);
void printList(Node* head);
void freeList(Node* head);
