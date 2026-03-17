#pragma once

#include <string>

// SlotStatus — tracks the state of each slot in a probing hash table
//   EMPTY    — slot has never been used; safe to stop probing here
//   OCCUPIED — slot holds a live key-value pair
//   DELETED  — slot was occupied but removed (tombstone); probing must skip past
enum class SlotStatus { EMPTY, OCCUPIED, DELETED };

// HashSlot — one slot in the probing table (key + value + status)
struct HashSlot {
    std::string key;
    int value;
    SlotStatus status;

    HashSlot() : key{""}, value{0}, status{SlotStatus::EMPTY} {}
};
