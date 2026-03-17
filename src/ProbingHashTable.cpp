// =============================================================================
// ProbingHashTable — Linear Probing (Closed Hashing / Open Addressing)
// =============================================================================
//
// Implement all 5 TODOs below (9-13). Each method follows the same pattern
// you built in Code-Together 11. The headers, main.cpp, and tests are
// provided — do not modify them.
//

#include "ProbingHashTable.h"
#include <iostream>

// =============================================================================
// Helper — next_prime (provided, do not modify)
// =============================================================================
// Returns the smallest prime >= n.
// Used by resize() to pick a new capacity that reduces clustering.
//
int ProbingHashTable::next_prime(int n) {
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

// =============================================================================
// Provided methods (do not modify)
// =============================================================================

// Constructor — allocate a flat array of HashSlots (all default to EMPTY)
ProbingHashTable::ProbingHashTable(int capacity)
    : size_(0), capacity_(capacity) {
    table_ = new HashSlot[capacity_];
}

// Destructor — just delete the flat array (no chains to walk)
ProbingHashTable::~ProbingHashTable() {
    delete[] table_;
}

// hash() — same multiply-and-add algorithm as ChainingHashTable
size_t ProbingHashTable::hash(const std::string& key) const {
    size_t hash_value = 0;
    for (char c : key) {
        hash_value = hash_value * 31 + c;
    }
    return hash_value % capacity_;
}

// load_factor() — size / capacity as a double
double ProbingHashTable::load_factor() const {
    return static_cast<double>(size_) / capacity_;
}

// print() — display each slot's contents
void ProbingHashTable::print() const {
    for (int i = 0; i < capacity_; ++i) {
        std::cout << "  [" << i << "]: ";
        if (table_[i].status == SlotStatus::OCCUPIED) {
            std::cout << "(" << table_[i].key << ", " << table_[i].value << ")";
        } else if (table_[i].status == SlotStatus::DELETED) {
            std::cout << "[deleted]";
        } else {
            std::cout << "[empty]";
        }
        std::cout << "\n";
    }
}

// ---------------------------------------------------------------------------
// TODO 9: Constructor is provided above. Nothing to implement here —
//         just note that HashSlot's default constructor sets status to EMPTY.
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// TODO 10: insert() — linear probing to find open slot (10 pts)
// ---------------------------------------------------------------------------
// - Check load factor FIRST: if (size_ + 1) / capacity_ > MAX_LOAD_FACTOR,
//   call resize() before inserting
// - Hash the key to get the starting index
// - Probe forward (linear probing) until you find:
//   1. EMPTY or DELETED slot → place entry here, set status to OCCUPIED,
//      increment size_
//   2. OCCUPIED slot with SAME key → update the value (no size change)
//   3. OCCUPIED slot with DIFFERENT key → probe to next slot:
//      index = (index + 1) % capacity_
//
// Review: CT11 Section 4
//
void ProbingHashTable::insert(const std::string& key, int value) {
    // TODO 10: linear probing insert
}

// ---------------------------------------------------------------------------
// TODO 11: search() — linear probing to find key (10 pts)
// ---------------------------------------------------------------------------
// - Hash the key to get the starting index
// - Probe forward:
//   - EMPTY → key not in table, return nullptr
//   - OCCUPIED + matching key → return &table_[index].value
//   - OCCUPIED + different key → keep probing
//   - DELETED → keep probing (tombstone — someone was here, key may be past)
//
// Review: CT11 Section 5
//
int* ProbingHashTable::search(const std::string& key) const {
    // TODO 11: linear probing search
    return nullptr;
}

// ---------------------------------------------------------------------------
// TODO 12: remove() — tombstone deletion (15 pts)
// ---------------------------------------------------------------------------
// - Hash the key to get the starting index
// - Probe forward:
//   - EMPTY → key not in table, return false
//   - OCCUPIED + matching key → set status to DELETED (tombstone),
//     decrement size_, return true
//   - OCCUPIED + different key → keep probing
//   - DELETED → keep probing
//
// IMPORTANT: Do NOT set the slot to EMPTY — that would break probe
// sequences for keys inserted after this one. DELETED tells future
// searches: "keep looking, someone might be past me."
//
// Review: CT11 Section 6
//
bool ProbingHashTable::remove(const std::string& key) {
    // TODO 12: tombstone deletion
    return false;
}

// ---------------------------------------------------------------------------
// TODO 13: resize() — grow and rehash, skip tombstones (15 pts)
// ---------------------------------------------------------------------------
// - Save the old capacity and old table_ pointer
// - Set capacity_ to next_prime(old_capacity * 2)
// - Allocate a new HashSlot array (all slots default to EMPTY)
// - Reset size_ to 0
// - Loop through the old table: for each OCCUPIED slot, call insert()
//   to rehash into the new table (DELETED and EMPTY slots are skipped)
// - delete[] the old table
//
// This clears all tombstones — only live entries are rehashed.
//
// Review: CT11 Section 8
//
void ProbingHashTable::resize() {
    // TODO 13: grow table, rehash occupied entries, skip tombstones
}
