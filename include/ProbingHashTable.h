#pragma once

#include <string>
#include "HashSlot.h"

// ProbingHashTable — closed hashing / open addressing (linear probing)
//
// Resolves collisions by scanning forward in the array one slot at a time
// until an open slot is found. All data lives inside the array itself.
//
// - std::string keys, int values
// - Same custom hash function as ChainingHashTable
// - Linear probing: next = (index + 1) % capacity_
// - Tombstone deletion so probe sequences aren't broken
// - MAX_LOAD_FACTOR = 0.75 — must stay below 1.0 to avoid clustering
//
class ProbingHashTable {
public:
    explicit ProbingHashTable(int capacity = 7);
    ~ProbingHashTable();

    // Rule of 5: no copy or move
    ProbingHashTable(const ProbingHashTable&) = delete;
    ProbingHashTable& operator=(const ProbingHashTable&) = delete;
    ProbingHashTable(ProbingHashTable&&) = delete;
    ProbingHashTable& operator=(ProbingHashTable&&) = delete;

    void insert(const std::string& key, int value);
    int* search(const std::string& key) const;
    bool remove(const std::string& key);

    size_t hash(const std::string& key) const;
    double load_factor() const;
    void resize();
    void print() const;

    int size() const { return size_; }
    int capacity() const { return capacity_; }
    bool is_empty() const { return size_ == 0; }

private:
    HashSlot* table_;           // flat array of slots
    int size_;                  // number of OCCUPIED slots
    int capacity_;              // total number of slots

    static constexpr double MAX_LOAD_FACTOR = 0.75;
    static int next_prime(int n);
};
