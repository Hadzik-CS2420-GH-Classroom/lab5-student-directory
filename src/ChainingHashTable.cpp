// =============================================================================
// ChainingHashTable — Separate Chaining (Open Hashing)
// =============================================================================
//
// Implement all 8 TODOs below. Each method follows the same pattern you
// built in Code-Together 10. The headers, main.cpp, and tests are provided
// — do not modify them.
//

#include "ChainingHashTable.h"
#include <iostream>

// =============================================================================
// Helper — next_prime (provided, do not modify)
// =============================================================================
// Returns the smallest prime >= n.
// Used by resize() to pick a new capacity that reduces clustering.
//
int ChainingHashTable::next_prime(int n) {
    if (n <= 2) return 2;
    if (n % 2 == 0) ++n;
    while (true) {
        bool is_prime = true;
        for (int i = 3; i * i <= n; i += 2) {
            if (n % i == 0) { is_prime = false; break; }
        }
        if (is_prime) return n;
        n += 2;
    }
}

// ---------------------------------------------------------------------------
// TODO 1: Constructor — allocate bucket array (5 pts)
// ---------------------------------------------------------------------------
// - Initialize size_ to 0 and capacity_ to the given capacity
// - Allocate an array of ChainNode* pointers on the heap
// - Zero-initialize every bucket to nullptr (empty chain)
//   Hint: new ChainNode*[capacity_]() — the () does the zero-init
//
// Review: CT10 Section 1
//
ChainingHashTable::ChainingHashTable(int capacity)
    : size_(0), capacity_(capacity) {
    buckets_ = new ChainNode*[capacity_]();
}

// ---------------------------------------------------------------------------
// TODO 2: Destructor — clean up all chains (5 pts)
// ---------------------------------------------------------------------------
// - Walk every bucket (outer loop: 0 to capacity_)
// - For each bucket, walk the chain and delete every ChainNode (inner loop)
//   Use the temp-pointer pattern from Module 4:
//     save current, advance to next, delete saved
// - After all chains are deleted, delete[] the bucket array itself
//
// Review: CT10 Section 2
//
ChainingHashTable::~ChainingHashTable() {
    for (int i = 0; i < capacity_; ++i) {
        ChainNode* current = buckets_[i];
        while (current != nullptr) {
            ChainNode* temp = current;
            current = current->next;
            delete temp;
        }
    }
    delete[] buckets_;
}

// ---------------------------------------------------------------------------
// TODO 3: hash() — custom hash function (10 pts)
// ---------------------------------------------------------------------------
// Two steps:
//   Step 1 (multiply-and-add): loop through each character of the key
//     hash_value = hash_value * 31 + character
//   Step 2 (modulo): return hash_value % capacity_
//
// Review: CT10 Section 3
//
size_t ChainingHashTable::hash(const std::string& key) const {
    size_t hash_value = 0;
    for (char c : key) {
        hash_value = hash_value * 31 + c;
    }
    return hash_value % capacity_;
}

// ---------------------------------------------------------------------------
// TODO 4: insert() — add or update a key-value pair (10 pts)
// ---------------------------------------------------------------------------
// - Hash the key to find the bucket index
// - Walk the chain at that bucket looking for a matching key
//   - If found: UPDATE the existing node's value (no duplicates)
//   - If not found: PREPEND a new ChainNode at the head of the chain
//     (same as push_front — O(1))
// - Increment size_ when adding a new entry
// - After inserting, check load_factor() > MAX_LOAD_FACTOR and resize()
//
// Review: CT10 Section 4
//
void ChainingHashTable::insert(const std::string& key, int value) {
    size_t index = hash(key);

    ChainNode* current = buckets_[index];
    while (current != nullptr) {
        if (current->key == key) {
            current->value = value;
            return;
        }
        current = current->next;
    }

    buckets_[index] = new ChainNode(key, value, buckets_[index]);
    ++size_;

    if (load_factor() > MAX_LOAD_FACTOR) {
        resize();
    }
}

// ---------------------------------------------------------------------------
// TODO 5: search() — find value by key (10 pts)
// ---------------------------------------------------------------------------
// - Hash the key to find the bucket index
// - Walk the chain comparing keys
// - If found: return a pointer to the node's value (&current->value)
// - If not found: return nullptr
//
// Review: CT10 Section 5
//
int* ChainingHashTable::search(const std::string& key) const {
    size_t index = hash(key);

    ChainNode* current = buckets_[index];
    while (current != nullptr) {
        if (current->key == key) {
            return &current->value;
        }
        current = current->next;
    }
    return nullptr;
}

// ---------------------------------------------------------------------------
// TODO 6: remove() — delete a key from its chain (10 pts)
// ---------------------------------------------------------------------------
// - Hash the key to find the bucket index
// - Walk with current and prev pointers (trailing-pointer pattern from CT8)
// - Three cases:
//   1. Key at HEAD of chain → buckets_[index] = current->next
//   2. Key in MIDDLE/TAIL → prev->next = current->next
//   3. Key NOT FOUND → return false
// - Delete the node and decrement size_ when found
//
// Review: CT10 Section 6, CT8 trailing-pointer pattern
//
bool ChainingHashTable::remove(const std::string& key) {
    size_t index = hash(key);

    ChainNode* current = buckets_[index];
    ChainNode* prev = nullptr;

    while (current != nullptr) {
        if (current->key == key) {
            if (prev == nullptr) {
                buckets_[index] = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;
            --size_;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

// ---------------------------------------------------------------------------
// TODO 7: load_factor() and resize() (15 pts)
// ---------------------------------------------------------------------------
// load_factor():
//   - Return size_ / capacity_ as a double
//     (cast to avoid integer division!)
//
// resize():
//   - Save the old capacity and old buckets_ pointer
//   - Set capacity_ to next_prime(old_capacity * 2)
//   - Allocate a new zero-initialized bucket array
//   - Reset size_ to 0
//   - Rehash every entry from the old table using insert()
//     (walk each old bucket's chain, insert each key/value, delete old nodes)
//   - delete[] the old bucket array
//
// Review: CT10 Section 7
//
double ChainingHashTable::load_factor() const {
    return static_cast<double>(size_) / capacity_;
}

void ChainingHashTable::resize() {
    int old_capacity = capacity_;
    ChainNode** old_buckets = buckets_;

    capacity_ = next_prime(old_capacity * 2);
    buckets_ = new ChainNode*[capacity_]();
    size_ = 0;

    for (int i = 0; i < old_capacity; ++i) {
        ChainNode* current = old_buckets[i];
        while (current != nullptr) {
            insert(current->key, current->value);
            ChainNode* temp = current;
            current = current->next;
            delete temp;
        }
    }
    delete[] old_buckets;
}

// ---------------------------------------------------------------------------
// TODO 8: print() — display each bucket's chain (10 pts)
// ---------------------------------------------------------------------------
// Format for each bucket:
//   [i]: empty                          (if chain is nullptr)
//   [i]: (key1, val1) -> (key2, val2)   (if chain has entries)
//
// - Print "  [i]: " for each bucket (two leading spaces)
// - If the bucket is empty (nullptr), print "empty"
// - Otherwise walk the chain printing "(key, value)"
//   with " -> " between nodes (but not after the last one)
// - End each bucket with "\n"
//
// Review: CT10 Section 8
//
void ChainingHashTable::print() const {
    for (int i = 0; i < capacity_; ++i) {
        std::cout << "  [" << i << "]: ";
        ChainNode* current = buckets_[i];
        if (current == nullptr) {
            std::cout << "empty";
        }
        while (current != nullptr) {
            std::cout << "(" << current->key << ", " << current->value << ")";
            if (current->next != nullptr) {
                std::cout << " -> ";
            }
            current = current->next;
        }
        std::cout << "\n";
    }
}
