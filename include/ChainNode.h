#pragma once

#include <string>

// One link in a separate-chaining bucket (singly linked list node)
struct ChainNode {
    std::string key;
    int value;
    ChainNode* next;

    ChainNode(const std::string& k, int v, ChainNode* n = nullptr)
        : key(k), value(v), next(n) {}
};
