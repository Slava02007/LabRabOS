#include "Koloc.h"
#include <unordered_set>
#include <iostream>
#include <stdexcept>
using namespace std;

vector<long long> Func1(int n) {
    if (n < 0) throw invalid_argument("n должно быть неотрицательным числом");

    vector<long long> result;
    if (n == 0) return result;

    result.reserve(n);
    long long factorial = 1;

    for (int i = 1; i <= n; ++i) {
        factorial *= i;
        result.push_back(factorial);
    }

    return result;
}

vector<int> removeDuplicates(const vector<int>& input) {
    vector<int> result;
    unordered_set<int> seen;
    result.reserve(input.size());

    for (int num : input) {
        if (seen.find(num) == seen.end()) {
            seen.insert(num);
            result.push_back(num);
        }
    }

    return result;
}

Node::Node(int value) : data(value), next(nullptr) {}

Node* reverseListRecursive(Node* head) {
    if (head == nullptr || head->next == nullptr)
        return head;

    Node* newHead = reverseListRecursive(head->next);
    head->next->next = head;
    head->next = nullptr;
    return newHead;
}

void printList(Node* head) {
    for (Node* curr = head; curr != nullptr; curr = curr->next) {
        cout << curr->data;
        if (curr->next) cout << " -> ";
    }
    cout << endl;
}

void freeList(Node* head) {
    while (head) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }
}
